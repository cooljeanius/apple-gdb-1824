
/**
 * @file funcFor.c
 *
 *  Time-stamp:        "2012-01-29 20:13:07 bkorb"
 *
 *  This module implements the FOR text macro.
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

#define ENTRY_END  INT_MAX
#define UNASSIGNED 0x7BAD0BAD

static tForState*  pFS;  /* Current "FOR" information (state) */

static char const zNoEnd[] = "%s ERROR:  FOR loop `%s' does not end\n";

/* = = = START-STATIC-FORWARD = = = */
static ag_bool
nextDefinition(ag_bool invert, tDefEntry** ppList);

static int
doForByStep(tTemplate* pT, tMacro* pMac, tDefEntry * pFoundDef);

static int
doForEach(tTemplate * pT, tMacro * pMac, tDefEntry * pFoundDef);

static void
load_ForIn(char const * pzSrc, size_t srcLen, tTemplate* pT, tMacro* pMac);
/* = = = END-STATIC-FORWARD = = = */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *  Operational Functions */

/*=gfunc first_for_p
 *
 * what:    detect first iteration
 * exparg:  for_var, which for loop, opt
 * doc:     Returns SCM_BOOL_T if the named FOR loop (or, if not named, the
 *          current innermost loop) is on the first pass through the data.
 *          Outside of any FOR loop, it returns SCM_UNDEFINED.
 *          @xref{FOR}.
=*/
SCM
ag_scm_first_for_p(SCM which)
{
    if (forInfo.fi_depth <= 0)
        return SCM_UNDEFINED;

    if (! AG_SCM_STRING_P(which))
        return (pFS->for_firstFor) ? SCM_BOOL_T : SCM_BOOL_F;

    {
        tForState* p   = forInfo.fi_data + (forInfo.fi_depth - 1);
        char *     pz  = ag_scm2zchars(which, "which for");
        SCM        res = SCM_UNDEFINED;
        int        ct  = forInfo.fi_depth;

        do  {
            if (strcmp(p->for_pzName, pz) == 0) {
                res = (p->for_firstFor ? SCM_BOOL_T : SCM_BOOL_F);
                break;
            }
            p--;
        } while (--ct > 0);

        return res;
    }
}


/*=gfunc last_for_p
 *
 * what:    detect last iteration
 * exparg:  for_var, which for loop, opt
 * doc:     Returns SCM_BOOL_T if the named FOR loop (or, if not named, the
 *          current innermost loop) is on the last pass through the data.
 *          Outside of any FOR loop, it returns SCM_UNDEFINED.
 *          @xref{FOR}.
=*/
SCM
ag_scm_last_for_p(SCM which)
{
    if (forInfo.fi_depth <= 0)
        return SCM_UNDEFINED;

    if (! AG_SCM_STRING_P(which))
        return (pFS->for_lastFor ? SCM_BOOL_T : SCM_BOOL_F);

    {
        tForState* p   = forInfo.fi_data + (forInfo.fi_depth - 1);
        char*      pz  = ag_scm2zchars(which, "which for");
        SCM        res = SCM_UNDEFINED;
        int        ct  = forInfo.fi_depth;

        do  {
            if (strcmp(p->for_pzName, pz) == 0) {
                res = (p->for_lastFor ? SCM_BOOL_T : SCM_BOOL_F);
                break;
            }
            p--;
        } while (--ct > 0);

        return res;
    }
}


/*=gfunc for_index
 *
 * what:    get current loop index
 * exparg:  for_var, which for loop, opt
 * doc:     Returns the current index for the named FOR loop.
 *          If not named, then the index for the innermost loop.
 *          Outside of any FOR loop, it returns SCM_UNDEFINED.
 *          @xref{FOR}.
=*/
SCM
ag_scm_for_index(SCM which)
{
    if (forInfo.fi_depth <= 0)
        return SCM_UNDEFINED;

    if (! AG_SCM_STRING_P(which))
        return AG_SCM_INT2SCM(pFS->for_index);

    {
        tForState* p   = forInfo.fi_data + (forInfo.fi_depth - 1);
        char*      pz  = ag_scm2zchars(which, "which for");
        SCM        res = SCM_UNDEFINED;
        int        ct  = forInfo.fi_depth;

        do  {
            if (strcmp(p->for_pzName, pz) == 0) {
                res = AG_SCM_INT2SCM(p->for_index);
                break;
            }
            p--;
        } while (--ct > 0);

        return res;
    }
}


/*=gfunc for_from
 *
 * what:   set initial index
 * exparg: from, the initial index for the AutoGen FOR macro
 *
 * doc:  This function records the initial index information
 *       for an AutoGen FOR function.
 *       Outside of the FOR macro itself, this function will emit an error.
 *       @xref{FOR}.
=*/
SCM
ag_scm_for_from(SCM from)
{
    if ((! pFS->for_loading) || (! AG_SCM_NUM_P(from)))
        return SCM_UNDEFINED;
    pFS->for_from = AG_SCM_TO_INT(from);
    return SCM_BOOL_T;
}


/*=gfunc for_to
 *
 * what:   set ending index
 * exparg: to, the final index for the AutoGen FOR macro
 *
 * doc:  This function records the terminating value information
 *       for an AutoGen FOR function.
 *       Outside of the FOR macro itself, this function will emit an error.
 *       @xref{FOR}.
=*/
SCM
ag_scm_for_to(SCM to)
{
    if ((! pFS->for_loading) || (! AG_SCM_NUM_P(to)))
        return SCM_UNDEFINED;
    pFS->for_to = AG_SCM_TO_INT(to);
    return SCM_BOOL_T;
}


/*=gfunc for_by
 *
 * what:   set iteration step
 * exparg: by, the iteration increment for the AutoGen FOR macro
 *
 * doc:  This function records the "step by" information
 *       for an AutoGen FOR function.
 *       Outside of the FOR macro itself, this function will emit an error.
 *       @xref{FOR}.
=*/
SCM
ag_scm_for_by(SCM by)
{
    if ((! pFS->for_loading) || (! AG_SCM_NUM_P(by)))
        return SCM_UNDEFINED;
    pFS->for_by = AG_SCM_TO_INT(by);
    return SCM_BOOL_T;
}


/*=gfunc for_sep
 *
 * what:   set loop separation string
 * exparg: separator, the text to insert between the output of
 *         each FOR iteration
 *
 * doc:  This function records the separation string that is to be inserted
 *       between each iteration of an AutoGen FOR function.  This is often
 *       nothing more than a comma.
 *       Outside of the FOR macro itself, this function will emit an error.
=*/
SCM
ag_scm_for_sep(SCM obj)
{
    if (! pFS->for_loading)
        return SCM_UNDEFINED;
    AGDUPSTR(pFS->for_pzSep, ag_scm2zchars(obj, "sep str"), "sep str");
    return SCM_BOOL_T;
}


static ag_bool
nextDefinition(ag_bool invert, tDefEntry** ppList)
{
    ag_bool     haveMatch = AG_FALSE;
    tDefEntry*  pList     = *ppList;

    while (pList != NULL) {
        /*
         *  Loop until we find or pass the current index value
         *
         *  IF we found an entry for the current index,
         *  THEN break out and use it
         */
        if (pList->index == pFS->for_index) {
            haveMatch = AG_TRUE;
            break;
        }

        /*
         *  IF the next definition is beyond our current index,
         *       (that is, the current index is inside of a gap),
         *  THEN we have no current definition and will use
         *       only the set passed in.
         */
        if ((invert)
            ? (pList->index < pFS->for_index)
            : (pList->index > pFS->for_index)) {

            /*
             *  When the "by" step is zero, force syncronization.
             */
            if (pFS->for_by == 0) {
                pFS->for_index = pList->index;
                haveMatch = AG_TRUE;
            }
            break;
        }

        /*
         *  The current index (pFS->for_index) is past the current value
         *  (pB->index), so advance to the next entry and test again.
         */
        pList = (invert) ? pList->pPrevTwin : pList->pTwin;
    }

    /*
     *  Save our restart point and return the find indication
     */
    *ppList = pList;
    return haveMatch;
}


static int
doForByStep(tTemplate* pT, tMacro* pMac, tDefEntry * pFoundDef)
{
    int         loopCt    = 0;
    tDefEntry   textDef;
    ag_bool     invert    = (pFS->for_by < 0) ? AG_TRUE : AG_FALSE;
    t_word      loopLimit = OPT_VALUE_LOOP_LIMIT;
    tDefCtx     ctx       = currDefCtx;

    if (pFS->for_pzSep == NULL)
        pFS->for_pzSep = (char*)zNil;

    /*
     *  IF the for-from and for-to values have not been set,
     *  THEN we set them from the indices of the first and last
     *       entries of the twin set.
     */
    {
        tDefEntry* pLast = (pFoundDef->pEndTwin != NULL)
                           ? pFoundDef->pEndTwin : pFoundDef;

        if (pFS->for_from == UNASSIGNED)
            pFS->for_from = (invert) ? pLast->index : pFoundDef->index;

        if (pFS->for_to == UNASSIGNED)
            pFS->for_to = (invert) ? pFoundDef->index : pLast->index;

        /*
         *  "loopLimit" is intended to catch runaway ending conditions.
         *  However, if you really have a gazillion entries, who am I
         *  to stop you?
         */
        if (loopLimit < pLast->index - pFoundDef->index)
            loopLimit = (pLast->index - pFoundDef->index) + 1;
    }

    /*
     *  Make sure we have some work to do before we start.
     */
    if (invert) {
        if (pFS->for_from < pFS->for_to)
            return 0;
    } else {
        if (pFS->for_from > pFS->for_to)
            return 0;
    }

    pFS->for_index = pFS->for_from;

    /*
     *  FROM `from' THROUGH `to' BY `by',
     *  DO...
     */
    for (;;) {
        int     nextIdx;
        ag_bool gotNewDef = nextDefinition(invert, &pFoundDef);

        if (loopLimit-- < 0) {
            fprintf(pfTrace, TRACE_FOR_STEP_TOO_FAR,
                    pT->pzTplName, pMac->lineNo);
            fprintf(pfTrace, TRACE_FOR_BY_STEP,
                    pT->pzTemplText + pMac->ozText,
                    pFS->for_from, pFS->for_to, pFS->for_by,
                    (int)OPT_VALUE_LOOP_LIMIT);
            break;
        }

        if (pFS->for_by != 0) {
            nextIdx = pFS->for_index + pFS->for_by;

        } else if (invert) {
            nextIdx = (pFoundDef->pPrevTwin == NULL)
                ? pFS->for_to - 1  /* last iteration !! */
                : pFoundDef->pPrevTwin->index;

        } else {
            nextIdx = (pFoundDef->pTwin == NULL)
                ? pFS->for_to + 1  /* last iteration !! */
                : pFoundDef->pTwin->index;
        }

        /*
         *  IF we have a non-base definition, use the old def context
         */
        if (! gotNewDef)
            currDefCtx = ctx;

        /*
         *  ELSE IF this macro is a text type
         *  THEN create an un-twinned version of it to be found first
         */
        else if (pFoundDef->valType == VALTYP_TEXT) {
            textDef = *pFoundDef;
            textDef.pNext = textDef.pTwin = NULL;

            currDefCtx.pDefs = &textDef;
            currDefCtx.pPrev = &ctx;
        }

        /*
         *  ELSE the current definitions are based on the block
         *       macro's values
         */
        else {
            currDefCtx.pDefs = pFoundDef->val.pDefEntry;
            currDefCtx.pPrev = &ctx;
        }

        pFS->for_lastFor = (invert)
            ? ((nextIdx < pFS->for_to) ? AG_TRUE : AG_FALSE)
            : ((nextIdx > pFS->for_to) ? AG_TRUE : AG_FALSE);

        generateBlock(pT, pMac+1, pT->aMacros + pMac->endIndex);
        loopCt++;
        pFS = forInfo.fi_data + (forInfo.fi_depth - 1);

        if (pFS->for_lastFor)
            break;

        fputs(pFS->for_pzSep, pCurFp->pFile);
        fflush(pCurFp->pFile);
        pFS->for_firstFor = AG_FALSE;
        pFS->for_index = nextIdx;
    }

    currDefCtx = ctx;  /* Restore the def context */
    return loopCt;
}

static int
doForEach(tTemplate * pT, tMacro * pMac, tDefEntry * pFoundDef)
{
    int     loopCt = 0;
    tDefCtx ctx    = currDefCtx;

    currDefCtx.pPrev = &ctx;

    for (;;) {
        tDefEntry  textDef;

        /*
         *  IF this loops over a text macro,
         *  THEN create a definition that will be found *before*
         *       the repeated (twinned) copy.  That way, when it
         *       is found as a macro invocation, the current value
         *       will be extracted, instead of the value list.
         */
        if (pFoundDef->valType == VALTYP_TEXT) {
            textDef = *pFoundDef;
            textDef.pNext = textDef.pTwin = NULL;

            currDefCtx.pDefs = &textDef;
        } else {
            currDefCtx.pDefs = pFoundDef->val.pDefEntry;
        }

        /*
         *  Set the global current index
         */
        pFS->for_index = pFoundDef->index;

        /*
         *  Advance to the next twin
         */
        pFoundDef = pFoundDef->pTwin;
        if (pFoundDef == NULL)
            pFS->for_lastFor = AG_TRUE;

        generateBlock(pT, pMac+1, pT->aMacros + pMac->endIndex);

        loopCt++;
        pFS = forInfo.fi_data + (forInfo.fi_depth - 1);

        if (pFoundDef == NULL)
            break;
        pFS->for_firstFor = AG_FALSE;

        /*
         *  Emit the iteration separation
         */
        fputs(pFS->for_pzSep, pCurFp->pFile);
        fflush(pCurFp->pFile);
    }

    currDefCtx = ctx;  /* Restore the def context */
    return loopCt;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static void
load_ForIn(char const * pzSrc, size_t srcLen, tTemplate* pT, tMacro* pMac)
{
    char* pzName = pT->pzTemplText + pMac->ozName;
    int   ix     = 0;
    char* pz;
    tDefEntry*  pPrev = NULL;

    /*
     *  Find the first text value
     */
    pz = SPN_WHITESPACE_CHARS(pzSrc + 3);
    if (*pz == NUL)
        AG_ABEND_IN(pT, pMac, FOR_IN_LISTLESS);
    srcLen -= pz - pzSrc;
    pzSrc = pz;

    {
        size_t nmlen = strlen(pzName);

        pz = AGALOC(srcLen + 2 + nmlen, "copy FOR text");
        strcpy(pz, pzName);
        pzName = pz;
        manageAllocatedData(pz);
        pz += nmlen + 1;
    }

    memcpy(pz, pzSrc, srcLen);
    pz[ srcLen ] = NUL;

    do  {
        tDefEntry* pDef = getEntry();

        pDef->pzDefName  = pzName;
        pDef->index      = ix++;
        pDef->valType    = VALTYP_TEXT;
        pDef->val.pzText = pz;

        switch (*pz) {
        case '\'':
        case '"':
            pz = spanQuote(pz);
            /*
             *  Clean up trailing commas
             */
            pz = SPN_WHITESPACE_CHARS(pz);
            if (*pz == ',')
                pz++;
            break;

        default:
            for (;;) {
                char ch = *(pz++);
                switch (ch) {
                case ' ': case TAB: case '\f': case '\v': case NL:
                    pz[-1] = NUL;
                    if (*pz != ',')
                        break;
                    pz++;
                    /* FALLTHROUGH */

                case ',':
                    pz[-1] = NUL;
                    break;

                case NUL:
                    pz--;
                    break;

                default:
                    continue;
                }
                break;
            }
            break;
        }

        /*
         *  Clean up trailing white space
         */
        pz = SPN_WHITESPACE_CHARS(pz);

        /*
         *  IF there is a previous entry, link its twin to this one.
         *  OTHERWISE, it is the head of the twin list.
         *  Link to funcPrivate.
         */
        if (pPrev != NULL)
            pPrev->pTwin = pDef;
        else
            pMac->funcPrivate = pDef;

        pPrev = pDef;
    } while (*pz != NUL);

    pMac->ozText = 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*=macfunc FOR
 *
 *  what:    Emit a template block multiple times
 *  cindex:  looping, for
 *  cindex:  for loop
 *  handler_proc:
 *  load_proc:
 *
 *  desc:
 *  This macro has a slight variation on the standard syntax:
 *  @example
 *  FOR <value-name> [ <separator-string> ]
 *
 *  FOR <value-name> (...Scheme expression list)
 *
 *  FOR <value-name> IN "string" [ ... ]
 *  @end example
 *
 *  Other than for the last form, the first macro argument must be the name of
 *  an AutoGen value.  If there is no value associated with the name, the
 *  @code{FOR} template block is skipped entirely.  The scope of the @code{FOR}
 *  macro extends to the corresponding @code{ENDFOR} macro.  The last form will
 *  create an array of string values named @code{<value-name>} that only exists
 *  within the context of this @code{FOR} loop.  With this form, in order to
 *  use a @code{separator-string}, you must code it into the end of the
 *  template block using the @code{(last-for?)} predicate function
 *  (@pxref{SCM last-for?}).
 *
 *  If there are any arguments after the @code{value-name}, the initial
 *  characters are used to determine the form.  If the first character is
 *  either a semi-colon (@code{;}) or an opening parenthesis (@code{(}), then
 *  it is presumed to be a Scheme expression containing the FOR macro specific
 *  functions @code{for-from}, @code{for-by}, @code{for-to}, and/or
 *  @code{for-sep}.  @xref{AutoGen Functions}.  If it consists of an '@code{i}'
 *  an '@code{n}' and separated by white space from more text, then the
 *  @code{FOR x IN} form is processed.  Otherwise, the remaining text is
 *  presumed to be a string for inserting between each iteration of the loop.
 *  This string will be emitted one time less than the number of iterations of
 *  the loop.  That is, it is emitted after each loop, excepting for the last
 *  iteration.
 *
 *  If the from/by/to functions are invoked, they will specify which copies of
 *  the named value are to be processed.  If there is no copy of the named
 *  value associated with a particular index, the @code{FOR} template block
 *  will be instantiated anyway.  The template must use methods for detecting
 *  missing definitions and emitting default text.  In this fashion, you can
 *  insert entries from a sparse or non-zero based array into a dense, zero
 *  based array.
 *
 *  @strong{NB:} the @code{for-from}, @code{for-to}, @code{for-by} and
 *  @code{for-sep} functions are disabled outside of the context of the
 *  @code{FOR} macro.  Likewise, the @code{first-for}, @code{last-for}
 *  and @code{for-index} functions are disabled outside of the range
 *  of a @code{FOR} block.
 *
 *  @strong{Also:} the @code{<value-name>} must be a single level name,
 *  not a compound name (@pxref{naming values}).
 *
 *  @example
 *  [+FOR var (for-from 0) (for-to <number>) (for-sep ",") +]
 *  ... text with @code{var}ious substitutions ...[+
 *  ENDFOR var+]
 *  @end example
 *
 *  @noindent
 *  this will repeat the @code{... text with @code{var}ious
 *  substitutions ...} <number>+1 times.  Each repetition,
 *  except for the last, will have a comma @code{,} after it.
 *
 *  @example
 *  [+FOR var ",\n" +]
 *  ... text with @code{var}ious substitutions ...[+
 *  ENDFOR var +]
 *  @end example
 *
 *  @noindent
 *  This will do the same thing, but only for the index
 *  values of @code{var} that have actually been defined.
=*/
/*=macfunc ENDFOR
 *
 *  what:   Terminates the @code{FOR} function template block
 *  in-context:
 *
 *  desc:
 *    This macro ends the @code{FOR} function template block.
 *    For a complete description @xref{FOR}.
=*/
tMacro*
mFunc_For(tTemplate* pT, tMacro* pMac)
{
    tMacro*     pMRet = pT->aMacros + pMac->endIndex;
    ag_bool     isIndexed;
    tDefEntry*  pDef;
    int         loopCt;

    /*
     *  "funcPrivate" is used by the "FOR x IN ..." variation of the macro.
     *  When parsed, a chain of text definitions are hung off of "funcPrivate".
     *  "doForEach()" will then chase through the linked list of text
     *  values.  This winds up being identical to [+ FOR var +] where
     *  a list of "var" values has been set.
     */
    if (pMac->funcPrivate != NULL)
        pDef = pMac->funcPrivate;
    else {
        pDef = findDefEntry(pT->pzTemplText + pMac->ozName, &isIndexed);
        if (pDef == NULL) {
            if (OPT_VALUE_TRACE >= TRACE_BLOCK_MACROS) {
                fprintf(pfTrace, TRACE_FN_FOR_SKIP,
                        pT->pzTemplText + pMac->ozName);

                if (OPT_VALUE_TRACE == TRACE_EVERYTHING)
                    fprintf(pfTrace, TAB_FILE_LINE_FMT,
                            pT->pzTplFile, pMac->lineNo);
            }

            return pMRet;
        }
    }

    /*
     *  Push the current FOR context onto a stack.
     */
    if (++(forInfo.fi_depth) > forInfo.fi_alloc) {
        forInfo.fi_alloc += 5;
        if (forInfo.fi_data == NULL)
             forInfo.fi_data = (tForState*)
                 AGALOC(5 * sizeof(tForState), "Init FOR sate");
        else forInfo.fi_data = (tForState*)
                 AGREALOC((void*)forInfo.fi_data,
                          forInfo.fi_alloc * sizeof(tForState), "FOR state");
    }

    pFS = forInfo.fi_data + (forInfo.fi_depth - 1);
    memset((void*)pFS, 0, sizeof(tForState));
    pFS->for_firstFor = AG_TRUE;
    pFS->for_pzName   = pT->pzTemplText + pMac->ozName;

    if (OPT_VALUE_TRACE >= TRACE_BLOCK_MACROS)
        fprintf(pfTrace, TRACE_FN_FOR, pT->pzTemplText + pMac->ozName,
                pT->pzTplFile, pMac->lineNo);

    /*
     *  Check for a FOR iterating based on scheme macros
     */
    if (pT->pzTemplText[ pMac->ozText ] == '(') {
        pFS->for_from  = \
        pFS->for_to    = UNASSIGNED;

        pFS->for_loading = AG_TRUE;
        (void) eval(pT->pzTemplText + pMac->ozText);
        pFS->for_loading = AG_FALSE;
        loopCt = doForByStep(pT, pMac, pDef);
    }
    else {
        /*
         *  The FOR iterates over a list of definitions
         */
        pFS->for_pzSep = pT->pzTemplText + pMac->ozText;
        loopCt = doForEach(pT, pMac, pDef);
    }

    forInfo.fi_depth--;

    if (OPT_VALUE_TRACE >= TRACE_BLOCK_MACROS) {
        fprintf(pfTrace, TRACE_FN_FOR_REPEAT, pT->pzTemplText + pMac->ozName,
                loopCt);

        if (OPT_VALUE_TRACE == TRACE_EVERYTHING)
            fprintf(pfTrace, TAB_FILE_LINE_FMT, pT->pzTplFile, pMac->lineNo);
    }

    return pMRet;
}


tMacro*
mLoad_For(tTemplate* pT, tMacro* pMac, char const ** ppzScan)
{
    char *        pzCopy = pT->pNext; /* next text dest   */
    char const *  pzSrc  = (char const*)pMac->ozText; /* macro text */
    size_t        srcLen = (size_t)pMac->res;         /* macro len  */
    tMacro *      pEndMac;

    /*
     *  Save the global macro loading mode
     */
    tpLoadProc const * papLP = papLoadProc;

    static tpLoadProc apForLoad[ FUNC_CT ] = { NULL };

    papLoadProc = apForLoad;
    if (srcLen == 0)
        AG_ABEND_IN(pT, pMac, LD_FOR_NAMELESS);

    /*
     *  IF this is the first time here,
     *  THEN set up the "FOR" mode callout table.
     *  It is the standard table, except entries are inserted
     *  for functions that are enabled only while processing
     *  a FOR macro
     */
    if (apForLoad[0] == NULL) {
        memcpy((void*)apForLoad, apLoadProc, sizeof(apLoadProc));
        apForLoad[ FTYP_ENDFOR ] = &mLoad_Ending;
    }

    /*
     *  pzSrc points to the name of the iteration "variable"
     *  Special hack:  if the name is preceeded by a `.',
     *  then the lookup is local-only and we will accept it.
     */
    pMac->ozName = pT->pNext - pT->pzTemplText;
    if (*pzSrc == '.') {
        *(pzCopy++) = *(pzSrc++);
        if (! IS_VAR_FIRST_CHAR(*pzSrc))
            pzCopy--; /* force an error */
    }

    while (IS_VALUE_NAME_CHAR(*pzSrc)) *(pzCopy++) = *(pzSrc++);
    *(pzCopy++) = NUL;

    if (pT->pzTemplText[ pMac->ozName ] == NUL)
        AG_ABEND_IN(pT, pMac, LD_FOR_INVALID_VAR);

    /*
     *  Skip space to the start of the text following the iterator name
     */
    pzSrc = SPN_WHITESPACE_CHARS(pzSrc);
    srcLen -= pzSrc - (char*)pMac->ozText;

    /* * * * *
     *
     *  No source -> zero offset to text
     */
    if ((ssize_t)srcLen <= 0) {
        pMac->ozText = 0;
    }

    /* * * * *
     *
     *  FOR foo IN ...  -> no text, but we create an array of text values
     */
    else if (  (strneqvcmp(pzSrc, LD_FOR_IN, 2) == 0)
            && IS_WHITESPACE_CHAR(pzSrc[2])) {
        load_ForIn(pzSrc, srcLen, pT, pMac);
    }

    /* * * * *
     *
     *  *EITHER* a:  FOR foo "<<separator>>"
     *  *OR*         FOR foo (scheme ...) ...
     */
    else {
        char* pzCopied = pzCopy;
        pMac->ozText = pzCopy - pT->pzTemplText;
        do  {
            *(pzCopy++) = *(pzSrc++);
        } while (--srcLen > 0);
        *(pzCopy++) = NUL;
        if ((*pzCopied == '"') || (*pzCopied == '\''))
            spanQuote(pzCopied);
    }
    /*
     * * * * */

    pT->pNext = pzCopy;

    pEndMac = parseTemplate(pMac + 1, ppzScan);
    if (*ppzScan == NULL)
        AG_ABEND_IN(pT, pMac, LD_FOR_NO_ENDFOR);

    pMac->endIndex = pMac->sibIndex = pEndMac - pT->aMacros;

    papLoadProc = papLP;
    return pEndMac;
}
/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * indent-tabs-mode: nil
 * End:
 * end of agen5/funcFor.c */
