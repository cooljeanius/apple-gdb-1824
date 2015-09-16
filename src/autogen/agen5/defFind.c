/**
 * @file defFind.c
 *
 *  Time-stamp:        "2012-01-29 07:42:06 bkorb"
 *
 *  This module locates definitions.
 *
 *  This file is part of AutoGen.
 *  AutoGen Copyright (c) 1992-2012 by Bruce Korb - all rights reserved
 *
 * AutoGen is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * AutoGen is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

struct defEntryList {
    size_t         allocCt;
    size_t         usedCt;
    tDefEntry**    papDefEntry;
    int            nestLevel;
};
typedef struct defEntryList tDefEntryList;

typedef struct hash_name_s hash_name_t;
struct hash_name_s {
    hash_name_t *   next;
    char            str[0];
};

static hash_name_t ** hash_table = NULL;
static int  hash_table_ct = 0;
static char zDefinitionName[ AG_PATH_MAX ];

#define ILLFORMEDNAME() \
    AG_ABEND(aprf(BAD_NAME_FMT, zDefinitionName, \
              pCurTemplate->pzTplFile, pCurMacro->lineNo));

/* = = = START-STATIC-FORWARD = = = */
static tDefEntry *
find_by_index(tDefEntry * pE, char * pzScan);

static void
add_to_def_list(tDefEntry* pDE, tDefEntryList* pDEL);

static size_t
bad_def_name(char * pzD, char const * pzS, size_t srcLen);

static tDefEntry*
find_def(char * pzName, tDefCtx* pDefCtx, ag_bool* pIsIndexed);

static int
hash_string(unsigned char const * pz);

static void
stringAdd(char const * pz);

static tDefEntry **
get_def_list(char * pzName, tDefCtx * pDefCtx);
/* = = = END-STATIC-FORWARD = = = */

static tDefEntry *
find_by_index(tDefEntry * pE, char * pzScan)
{
    int  idx;

    /*
     *  '[]' means the first entry of whatever index number
     */
    if (*pzScan == ']')
        return pE;

    /*
     *  '[$]' means the last entry of whatever index number
     */
    if (*pzScan == '$') {
        pzScan = SPN_WHITESPACE_CHARS(pzScan + 1);
        if (*pzScan != ']')
            return NULL;

        if (pE->pEndTwin != NULL)
            return pE->pEndTwin;

        return pE;
    }

    /*
     *  '[nn]' means the specified index number
     */
    if (IS_DEC_DIGIT_CHAR(*pzScan)) {
        char* pz;
        idx = strtol(pzScan, &pz, 0);

        /*
         *  Skip over any trailing space and make sure we have a closer
         */
        pz = SPN_WHITESPACE_CHARS(pz);
        if (*pz != ']')
            return NULL;
    }

    else {
        /*
         *  '[XX]' means get the index from our definitions
         */
        char* pzDef = pzScan;
        char const* pzVal;

        if (! IS_VAR_FIRST_CHAR(*pzScan))
            return NULL;

        pzScan = SPN_VALUE_NAME_CHARS(pzScan);

        /*
         *  Temporarily remove the character under *pzScan and
         *  find the corresponding defined value.
         */
        {
            char  svch = *pzScan;
            *pzScan = NUL;
            pzVal   = getDefine(pzDef, AG_TRUE);
            *pzScan = svch;
        }

        /*
         *  Skip over any trailing space and make sure we have a closer
         */
        pzScan = SPN_WHITESPACE_CHARS(pzScan);
        if (*pzScan != ']')
            return NULL;

        /*
         *  make sure we found a defined value
         */
        if ((pzVal == NULL) || (*pzVal == NUL))
            return NULL;

        idx = strtol(pzVal, &pzDef, 0);

        /*
         *  Make sure we got a legal number
         */
        if (*pzDef != NUL)
            return NULL;
    }

    /*
     *  Search for the entry with the specified index.
     */
    do  {
        if (pE->index > idx)
            return NULL;
        if (pE->index == idx)
            break;
        pE = pE->pTwin;
    } while (pE != NULL);

    return pE;
}


/*
 *  find entry support routines:
 *
 *  add_to_def_list:  place a new definition entry on the end of the
 *              list of found definitions (reallocating list size as needed).
 */
static void
add_to_def_list(tDefEntry* pDE, tDefEntryList* pDEL)
{
    if (++(pDEL->usedCt) > pDEL->allocCt) {
        pDEL->allocCt += pDEL->allocCt + 8; /* 8, 24, 56, ... */
        pDEL->papDefEntry = (tDefEntry**)
            AGREALOC((void*)pDEL->papDefEntry, pDEL->allocCt * sizeof(void*),
                     "add find");
    }

    pDEL->papDefEntry[ pDEL->usedCt-1 ] = pDE;
}

static size_t
bad_def_name(char * pzD, char const * pzS, size_t srcLen)
{
    memcpy((void*)pzD, (void*)pzS, srcLen);
    pzD[srcLen] = NUL;
    fprintf(pfTrace, BAD_NAME_FMT, pzD,
            pCurTemplate->pzTplFile, pCurMacro->lineNo);
    return srcLen + 1;
}


/*
 *  canonicalizeName:  remove white space and roughly verify the syntax.
 *  This procedure will consume everything from the source string that
 *  forms a valid AutoGen compound definition name.
 *  We leave legally when:
 *  1.  the state is "CN_NAME_ENDED", AND
 *  2.  We stumble into a character that is not either '[' or name_sep_ch
 *      (always skipping white space).
 *  We start in CN_START.
 */
LOCAL int
canonicalizeName(char* pzD, char const* pzS, int srcLen)
{
    typedef enum {
        CN_START_NAME = 0,   /* must find a name */
        CN_NAME_ENDED,       /* must find '[' or name_sep_ch or we end */
        CN_INDEX,            /* must find name, number, '$' or ']' */
        CN_INDEX_CLOSE,      /* must find ']' */
        CN_INDEX_ENDED       /* must find name_sep_ch or we end */
    } teConState;

    teConState state = CN_START_NAME;

    char const* pzOri = pzS;
    char*       pzDst = pzD;
    size_t      stLen = srcLen;

    /*
     *  Before anything, skip a leading name_sep_ch as a special hack
     *  to force a current context lookup.
     */
    pzS = SPN_WHITESPACE_CHARS(pzS);
    if (pzOri != pzS) {
        srcLen -= pzS - pzOri;
        if (srcLen <= 0)
            pzS = zNil;
    }

    if (*pzS == name_sep_ch) {
        *(pzD++) = name_sep_ch;
        pzS++;
        if (--srcLen <= 0)
            pzS = zNil;
    }

 nextSegment:

    /*
     *  The next segment may always start with an alpha character,
     *  but an index may also start with a number.  The full number
     *  validation will happen in find_by_index().
     */
    {
        char * p = SPN_WHITESPACE_CHARS(pzS);
        if (p != pzS) {
            srcLen -= p - pzS;
            if (srcLen <= 0)
                pzS = zNil;
            pzS = p;
        }
    }

    switch (state) {
    case CN_START_NAME:
        if (! IS_VAR_FIRST_CHAR(*pzS))
            return bad_def_name(pzDst, pzOri, stLen);
        state = CN_NAME_ENDED;  /* we found the start of our first name */
        break;  /* fall through to name/number consumption code */

    case CN_NAME_ENDED:
        switch (*pzS++) {
        case '[':
            *(pzD++) = '[';
            state = CN_INDEX;
            break;

        case '.': case '/':
            if (pzS[-1] == name_sep_ch) {
                *(pzD++) = name_sep_ch;
                state = CN_START_NAME;
                break;
            }

        default:
            /* legal exit -- we have a name already */
            *pzD = NUL;
            return srcLen;
        }

        if (--srcLen <= 0)
            return bad_def_name(pzDst, pzOri, stLen);
        goto nextSegment;

    case CN_INDEX:
        /*
         *  An index.  Valid syntaxes are:
         *
         *    '[' <#define-d name> ']'
         *    '[' <number> ']'
         *    '['  '$'  ']'
         *    '['       ']'
         *
         *  We will check for and handle the last case right here.
         *  The next cycle must find the index closer (']').
         */
        state = CN_INDEX_CLOSE;

        /*
         *  Numbers and #define-d names are handled at the end of the switch.
         *  '$' and ']' are handled immediately below.
         */
        if (IS_ALPHANUMERIC_CHAR(*pzS))
            break;

        /*
         *  A solitary '$' is the highest index, whatever that happens to be
         *  We process that right here because down below we only accept
         *  name-type characters and this is not VMS.
         */
        if (*pzS == '$') {
            if (--srcLen < 0)
                return bad_def_name(pzDst, pzOri, stLen);

            *(pzD++) = *(pzS++);
            goto nextSegment;
        }
        /* FALLTHROUGH */

    case CN_INDEX_CLOSE:
        /*
         *  Nothing else is okay.
         */
        if ((*(pzD++) = *(pzS++)) != ']')
            return bad_def_name(pzDst, pzOri, stLen);

        if (--srcLen <= 0) {
            *pzD = NUL;
            return srcLen;
        }
        state = CN_INDEX_ENDED;
        goto nextSegment;

    case CN_INDEX_ENDED:
        if ((*pzS != name_sep_ch) || (--srcLen < 0)) {
            *pzD = NUL;
            return srcLen;
        }
        *(pzD++) = *(pzS++);

        state = CN_START_NAME;
        goto nextSegment;
    }

    /*
     *  The next state must be either looking for what comes after the
     *  end of a name, or for the close bracket after an index.
     *  Whatever, the next token must be a name or a number.
     */
    assert((state == CN_NAME_ENDED) || (state == CN_INDEX_CLOSE));
    assert(IS_ALPHANUMERIC_CHAR(*pzS));

    /*
     *  Copy the name/number.  We already know the first character is valid.
     *  However, we must *NOT* downcase #define names...
     */
    while (IS_VALUE_NAME_CHAR(*pzS)) {
        char ch = *(pzS++);
        if ((state != CN_INDEX_CLOSE) && IS_UPPER_CASE_CHAR(ch))
            *(pzD++) = tolower(ch);

        else switch (ch) { /* force the separator chars to be '_' */
        case '-':
        case '^':
            *(pzD++) = '_';
            break;

        default:
            *(pzD++) = ch;
        }

        if (--srcLen <= 0) {
            pzS = zNil;
            break;
        }
    }

    goto nextSegment;
}


/**
 *  Find the definition entry for the name passed in.
 *  It is okay to find block entries IFF they are found on the
 *  current level.  Once you start traversing up the tree,
 *  the macro must be a text macro.  Return an indicator saying if
 *  the element has been indexed (so the caller will not try
 *  to traverse the list of twins).
 */
static tDefEntry*
find_def(char * pzName, tDefCtx* pDefCtx, ag_bool* pIsIndexed)
{
    char *       brace;
    char         br_ch;
    tDefEntry *  ent;
    ag_bool      dummy;
    ag_bool      noNesting    = AG_FALSE;

    static int   nestingDepth = 0;

    /*
     *  IF we are at the start of a search, then canonicalize the name
     *  we are hunting for, copying it to a modifiable buffer, and
     *  initialize the "indexed" boolean to false (we have not found
     *  an index yet).
     */
    if (nestingDepth == 0) {
        canonicalizeName(zDefinitionName, pzName, (int)strlen(pzName));
        pzName = zDefinitionName;

        if (pIsIndexed != NULL)
             *pIsIndexed = AG_FALSE;
        else pIsIndexed  = &dummy;

        if (*pzName == name_sep_ch) {
            noNesting = AG_TRUE;
            pzName++;
        }
    }

    brace  = BRK_NAME_SEP_CHARS(pzName);
    br_ch  = *brace;
    *brace = NUL;

    if (br_ch == '[') *pIsIndexed = AG_TRUE;

    for (;;) {
        /*
         *  IF we are at the end of the definitions (reached ROOT),
         *  THEN it is time to bail out.
         */
        ent = pDefCtx->pDefs;
        if (ent == NULL)
            return NULL;

        do  {
            /*
             *  IF the name matches
             *  THEN break out of the double loop
             */
            if (strcmp(ent->pzDefName, pzName) == 0)
                goto found_def_entry;

            ent = ent->pNext;
        } while (ent != NULL);

        /*
         *  IF we are nested, then we cannot change the definition level.
         *  So, we did not find anything.
         */
        if ((nestingDepth != 0) || noNesting)
            return NULL;

        /*
         *  Let's go try the definitions at the next higher level.
         */
        pDefCtx = pDefCtx->pPrev;
        if (pDefCtx == NULL)
            return NULL;
    } found_def_entry:;

    /*
     *  At this point, we have found the entry that matches the supplied name,
     *  up to the '[' or name_sep_ch or NUL character.  It *must* be one of
     *  those three characters.
     */
    *brace = br_ch;

    switch (br_ch) {
    case NUL:
        return ent;

    case '[':
        /*
         *  We have to find a specific entry in a list.
         */
        brace = SPN_WHITESPACE_CHARS(brace + 1);

        ent = find_by_index(ent, brace);
        if (ent == NULL)
            return ent;

        /*
         *  We must find the closing brace, or there is an error
         */
        brace = strchr(brace, ']');
        if (brace == NULL)
            ILLFORMEDNAME();

        /*
         *  What follows the closing brace?  IF we are at the end of the
         *  definition, THEN return what we found.  However, if there's
         *  another name, then we have to go look that one up, too.
         */
        switch (*++brace) {
        case NUL:
            return ent;

        case '.': case '/':
            /*
             *  Which one?  One is valid, the other not and it is not known
             *  at compile time.
             */
            if (*brace == name_sep_ch) {
                pzName = brace + 1;
                break;
            }
            /* FALLTHROUGH */

        default:
            ILLFORMEDNAME();
        }
        break;

    case '.': case '/':
        if (br_ch == name_sep_ch) {
            /*
             *  Which one?  One is valid, the other not and it is not known
             *  at compile time.
             *
             *  It is a segmented value name.  Set the name pointer
             *  to the next segment and search starting from the newly
             *  available set of definitions.
             */
            pzName = brace + 1;
            break;
        }
        /* FALLTHROUGH */

    default:
        ILLFORMEDNAME();
    }

    /*
     *  We cannot find a member of a non-block type macro definition.
     */
    if (ent->valType != VALTYP_BLOCK)
        return NULL;

    /*
     *  Loop through all the twins of the entry we found until
     *  we find the entry we want.  We ignore twins if we just
     *  used a subscript.
     */
    nestingDepth++;
    {
        tDefCtx ctx = { NULL, &currDefCtx };

        ctx.pDefs = ent->val.pDefEntry;

        for (;;) {
            tDefEntry* res;

            res = find_def(pzName, &ctx, pIsIndexed);
            if ((res != NULL) || (br_ch == '[')) {
                nestingDepth--;
                return res;
            }
            ent = ent->pTwin;
            if (ent == NULL)
                break;
            ctx.pDefs = ent->val.pDefEntry;
        }
    }

    nestingDepth--;
    return NULL;
}

/*
 *  This makes certain assumptions about the underlying architecture.
 *  Doesn't matter tho.  A high collision rate just makes it a teensy
 *  bit slower.
 */
static int
hash_string(unsigned char const * pz)
{
    unsigned int res = 0;
    while (*pz)
        res ^= (res << 3) ^ *(pz++);

    return res & (hash_table_ct - 1);
}

static void
stringAdd(char const * pz)
{
    unsigned char z[SCRIBBLE_SIZE];
    size_t z_len;

    /*
     *  If there is no hash table, create one.
     */
    if (hash_table_ct == 0) {
        size_t ct = pCurTemplate->macroCt;
        if (ct < SCRIBBLE_SIZE)
            ct = SCRIBBLE_SIZE;
        else {
            int bit_ct = 0;
            while (ct > 1) {
                bit_ct++;
                ct >>= 1;
            }
            ct = 1 << bit_ct;
        }

        hash_table_ct = ct;
        hash_table    = malloc(ct * sizeof (*hash_table));
        memset(hash_table, NUL, ct * sizeof (*hash_table));
    }

    /*
     *  Save only the last component of the name, sans any index, too.
     */
    {
        char const * p = strrchr(pz, name_sep_ch);
        if (p != NULL)
            pz = p + 1;
        p = strchr(pz, '[');

        if (p != NULL) {
            z_len = (p - pz) + 1;
            if (z_len > sizeof(z))
                return;
            memcpy(z, pz, z_len - 1);
            z[z_len - 1] = NUL;

        } else {
            z_len = strlen(pz) + 1;
            if (z_len > sizeof(z))
                return;
            memcpy(z, pz, z_len);
        }
    }

    /*
     *  canonicalize the name
     */
    strtransform((char *)z, (char *)z);

    /*
     *  If a new name, insert it in order.
     */
    {
        int ix = hash_string(z);

        hash_name_t ** hptr = &(hash_table[ix]), *new;
        while (*hptr != NULL) {
            int cmp = memcmp(z, (*hptr)->str, z_len);
            if (cmp == 0)
                return; /* old name */

            if (cmp > 0)
                break; /* no matching name can be found */

            hptr = &((*hptr)->next);
        }

        new       = malloc(sizeof (*new) + z_len);
        new->next = *hptr;
        *hptr     = new;
        memcpy(new->str, z, z_len);
    }
}

LOCAL tDefEntry*
findDefEntry(char * pzName, ag_bool* pIsIndexed)
{
    if (HAVE_OPT(USED_DEFINES))
        stringAdd(pzName);
    return find_def(pzName, &currDefCtx, pIsIndexed);
}


LOCAL void
print_used_defines(void)
{
    if (hash_table_ct == 0)
        return;

    {
        int ix = 0;
        
        FILE * fp = popen(USED_DEFINES_FMT, "w");
        if (fp == NULL) return;

        while (ix < hash_table_ct) {
            hash_name_t * hn = hash_table[ix++];
            while (hn != NULL) {
                fprintf(fp, USED_DEFINES_LINE_FMT, hn->str);
                hn = hn->next;
            }
        }
        pclose(fp);
    }
}


/**
 *  Find the definition entries for the name passed in.  It is okay to find
 *  block entries IFF they are found on the current level.  Once you start
 *  traversing up the tree, the macro must be a text macro.  Return an
 *  indicator saying if the element has been indexed (so the caller will
 *  not try to traverse the list of twins).
 */
static tDefEntry **
get_def_list(char * pzName, tDefCtx * pDefCtx)
{
    static tDefEntryList defList = { 0, 0, NULL, 0 };

    char*      pcBrace;
    char       breakCh;
    tDefEntry* pE;
    ag_bool    noNesting = AG_FALSE;

    /*
     *  IF we are at the start of a search, then canonicalize the name
     *  we are hunting for, copying it to a modifiable buffer, and
     *  initialize the "indexed" boolean to false (we have not found
     *  an index yet).
     */
    if (defList.nestLevel == 0) {
        if (*pzName == name_sep_ch) {
            noNesting = AG_TRUE;
            pzName = SPN_WHITESPACE_CHARS(pzName + 1);
        }

        if (! IS_VAR_FIRST_CHAR(*pzName)) {
            strncpy(zDefinitionName, pzName, sizeof(zDefinitionName) - 1);
            zDefinitionName[ sizeof(zDefinitionName) - 1] = NUL;
            ILLFORMEDNAME();
        }

        canonicalizeName(zDefinitionName, pzName, (int)strlen(pzName));
        pzName = zDefinitionName;
        defList.usedCt = 0;
    }

    pcBrace  = BRK_NAME_SEP_CHARS(pzName);
    breakCh  = *pcBrace;
    *pcBrace = NUL;

    for (;;) {
        /*
         *  IF we are at the end of the definitions (reached ROOT),
         *  THEN it is time to bail out.
         */
        pE = pDefCtx->pDefs;
        if (pE == NULL) {
            /*
             *  Make sure we are not nested.  Once we start to nest,
             *  then we cannot "change definition levels"
             */
        not_found:
            if (defList.nestLevel != 0)
                ILLFORMEDNAME();

            /*
             *  Don't bother returning zero entry list.  Just return NULL.
             */
            return NULL;
        }

        do  {
            /*
             *  IF the name matches
             *  THEN go add it, plus all its twins
             */
            if (strcmp(pE->pzDefName, pzName) == 0)
                goto found_def_entry;

            pE = pE->pNext;
        } while (pE != NULL);

        /*
         *  IF we are nested, then we cannot change the definition level.
         *  Just go and return what we have found so far.
         */
        if ((defList.nestLevel != 0) || noNesting)
            goto returnResult;

        /*
         *  Let's go try the definitions at the next higher level.
         */
        pDefCtx = pDefCtx->pPrev;
        if (pDefCtx == NULL)
            goto not_found;
    } found_def_entry:;

    /*
     *  At this point, we have found the entry that matches the supplied name,
     *  up to the '[' or name_sep_ch or NUL character.  It *must* be one of
     *  those three characters.
     */
    *pcBrace = breakCh;

    switch (breakCh) {
    case NUL:
        do  {
            add_to_def_list(pE, &defList);
            pE = pE->pTwin;
        } while (pE != NULL);
        goto returnResult;

    case '[':
        /*
         *  We have to find a specific entry in a list.
         */
        pcBrace = SPN_WHITESPACE_CHARS(pcBrace + 1);

        pE = find_by_index(pE, pcBrace);
        if (pE == NULL)
            goto returnResult;

        /*
         *  We must find the closing brace, or there is an error
         */
        pcBrace = strchr(pcBrace, ']');
        if (pcBrace == NULL)
            ILLFORMEDNAME();

        /*
         *  IF we are at the end of the definition,
         *  THEN return what we found
         */
        switch (*++pcBrace) {
        case NUL:
            goto returnResult;

        case '.': case '/':
            /*
             *  Which one?  One is valid, the other not and it is not known
             *  at compile time.
             */
            if (*pcBrace == name_sep_ch)
                break;
            /* FALLTHROUGH */

        default:
            ILLFORMEDNAME();
        }
        pzName = pcBrace + 1;
        break;

    case '.': case '/':
        if (breakCh == name_sep_ch) {
            /*
             *  Which one?  One is valid, the other not and it is not known
             *  at compile time.
             *
             *  It is a segmented value name.  Set the name pointer to the
             *  next segment and search starting from the newly available set
             *  of definitions.
             */
            pzName = pcBrace + 1;
            break;
        }

    default:
        ILLFORMEDNAME();
    }

    /*
     *  Loop through all the twins of the entry.  We only look once if we used
     *  a subscript.  Ignore any entry types that are not "Blocks" because
     *  text entries won't have any children.
     */
    defList.nestLevel++;
    do  {
        tDefCtx ctx = { pE->val.pDefEntry, &currDefCtx };

        if (pE->valType == VALTYP_BLOCK)
            (void)get_def_list(pzName, &ctx);

        if (breakCh == '[')
            break;

        pE = pE->pTwin;
    } while (pE != NULL);

    defList.nestLevel--;

 returnResult:
    if (defList.nestLevel == 0)
        add_to_def_list(NULL, &defList);

    return defList.papDefEntry;
}


LOCAL tDefEntry**
findEntryList(char* pzName)
{
    return get_def_list(pzName, &currDefCtx);
}
/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * indent-tabs-mode: nil
 * End:
 * end of agen5/defFind.c */
