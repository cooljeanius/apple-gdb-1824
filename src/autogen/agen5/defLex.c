
/**
 * @file defLex.c
 *
 *  Time-stamp:        "2012-01-29 09:39:37 bkorb"
 *
 *  This module scans the template variable declarations and passes
 *  tokens back to the parser.
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
/*
 *  This keyword table must match those found in agParse.y.
 *  You will find them in a %token statement that follows
 *  a comment  "Keywords"
 */
#define KEYWORD_TABLE                           \
  _KW_( AUTOGEN )                               \
  _KW_( DEFINITIONS )

#define _KW_(w) static char const z ## w [] = #w;
KEYWORD_TABLE
#undef _KW_

#define _KW_(w) z ## w,
static char const*  apzKeywords[] = { KEYWORD_TABLE };
#undef _KW_

#define _KW_(w) DP_EV_ ## w,
te_dp_event aKeywordTkn[] = { KEYWORD_TABLE };
#undef _KW_

#define KEYWORD_CT  (sizeof(apzKeywords) / sizeof(apzKeywords[0]))

#define ERROR  (-1)
#define FINISH (-1)

#define SET_LIT_TKN(t) lastToken = DP_EV_LIT_ ## t; *(pCurCtx->pzScan++) = NUL;

/* = = = START-STATIC-FORWARD = = = */
static void
pop_context(void);

static void
trim_whitespace(void);

static void
lex_escaped_char(void);

static tSuccess
lex_backquote(void);

static tSuccess
lex_comment(void);

static tSuccess
lex_here_string(void);

static void
loadScheme(void);

static void
alist_to_autogen_def(void);

static char*
gather_name(char * pzScan, te_dp_event * pRetVal);

static char*
assembleHereString(char* pzScan);
/* = = = END-STATIC-FORWARD = = = */

/**
 *  Pop off an include context and resume from the including file.
 */
static void
pop_context(void)
{
    tScanCtx* pCX = pCurCtx;
    pCurCtx   = pCurCtx->pCtx;
    pCX->pCtx = pDoneCtx;
    pDoneCtx  = pCX;
}

static void
trim_whitespace(void)
{
    char * pz = pCurCtx->pzScan;

    /*
     *  This ensures that any names found previously
     *  are NUL terminated.
     */
    if (*pz == NL)
        pCurCtx->lineNo++;
    *pz = NUL;

    for (;;) {
        pz = SPN_HORIZ_WHITE_CHARS(pz+1);
        if (*pz != NL)
            break;
        pCurCtx->lineNo++;
    }

    pCurCtx->pzScan = pz;
}

static void
lex_escaped_char(void)
{
    static int const semi_colon = ';';

    char* pz = strchr(pCurCtx->pzScan, semi_colon);

    for (;;) {
        if (pz == NULL) {
            pz = pCurCtx->pzScan + strlen(pCurCtx->pzScan);
            break;
        }
        if (IS_WHITESPACE_CHAR(pz[1])) {
            *pz = NUL;
            pz[1] = semi_colon;
            break;
        }
        pz = strchr(pz+1, semi_colon);
    }

    lastToken = DP_EV_STRING;
    pz_token = pz;
}

static tSuccess
lex_backquote(void)
{
    int line_no = pCurCtx->lineNo;
    char* pz = ao_string_cook(pCurCtx->pzScan, &line_no);

    if (pz == NULL)
        return FAILURE;

    pz_token = pCurCtx->pzScan;

    pCurCtx->pzScan = pz;

    lastToken = DP_EV_STRING;
    pz = shell_cmd((char const*)pz_token);
    pCurCtx->lineNo = line_no;

    if (pz == NULL)
        return PROBLEM;
    TAGMEM(pz, "shell def str");
    pz_token = pz;
    manageAllocatedData(pz);
    return SUCCESS;
}

static tSuccess
lex_comment(void)
{
    /*
     *  Allow for a comment, C or C++ style
     */
    switch (pCurCtx->pzScan[1]) {
    case '*':
    {
        char* pz = strstr(pCurCtx->pzScan+2, END_C_COMMENT);
        if (pz != NULL) {
            char* p = pCurCtx->pzScan+1;
            for (;;) {
                p = strchr(p+1, NL);
                if ((p == NULL) || (p > pz))
                    break;
                pCurCtx->lineNo++;
            }
            pCurCtx->pzScan = pz+2;
            return SUCCESS;
        }
        break;
    }
    case '/':
    {
        char* pz = strchr(pCurCtx->pzScan+2, NL);
        if (pz != NULL) {
            pCurCtx->pzScan = pz+1;
            pCurCtx->lineNo++;
            return SUCCESS;
        }
        break;
    }
    }

    return FAILURE;
}

static tSuccess
lex_here_string(void)
{
    char* pz;
    if (pCurCtx->pzScan[1] != '<')
        return FAILURE;

    pz = assembleHereString(pCurCtx->pzScan + 2);
    if (pz == NULL) {
        lastToken = DP_EV_INVALID;
        return PROBLEM;
    }

    lastToken = DP_EV_HERE_STRING;
    pCurCtx->pzScan = pz;
    return SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *   LEXICAL SCANNER
 */
LOCAL te_dp_event
yylex(void)
{
    lastToken = DP_EV_INVALID;

scanAgain:
    /*
     *  Start the process of locating a token.
     *  We branch here after skipping over a comment
     *  or processing a directive (which may change our context).
     */
    if (IS_WHITESPACE_CHAR(*pCurCtx->pzScan))
        trim_whitespace();

    switch (*pCurCtx->pzScan) {
    case NUL:
        /*
         *  IF we are not inside an include context,
         *  THEN go finish.
         */
        if (pCurCtx->pCtx == NULL)
            goto lex_done;

        pop_context();
        goto scanAgain;

    case '#':
    {
        extern char * processDirective(char*);
        char * pz = processDirective(pCurCtx->pzScan+1);
        /*
         *  Ensure that the compiler doesn't try to save a copy of
         *  "pCurCtx" in a register.  It must be reloaded from memory.
         */
        pCurCtx->pzScan = pz;
        goto scanAgain;
    }

    case '{': SET_LIT_TKN(O_BRACE);   break;
    case '=': SET_LIT_TKN(EQ);        break;
    case '}': SET_LIT_TKN(C_BRACE);   break;
    case '[': SET_LIT_TKN(OPEN_BKT);  break;
    case ']': SET_LIT_TKN(CLOSE_BKT); break;
    case ';': SET_LIT_TKN(SEMI);      break;
    case ',': SET_LIT_TKN(COMMA);     break;

    case '\'':
    case '"':
    {
        char* pz = ao_string_cook(pCurCtx->pzScan, &(pCurCtx->lineNo));
        if (pz == NULL)
            goto NUL_error;

        pz_token = pCurCtx->pzScan;

        lastToken = DP_EV_STRING;
        pCurCtx->pzScan = pz;
        break;
    }

    case '<':
        switch (lex_here_string()) {
        case SUCCESS: break;
        case FAILURE: goto BrokenToken;
        case PROBLEM: return DP_EV_INVALID;
        }
        break;

    case '(':
        loadScheme();
        break;

    case '\\':
        if (strncmp(pCurCtx->pzScan+1, START_SCHEME_LIST, (size_t)2) == 0) {
            alist_to_autogen_def();
            goto scanAgain;
        }
        lex_escaped_char();
        break;

    case '`':
        switch (lex_backquote()) {
        case FAILURE: goto NUL_error;
        case PROBLEM: goto scanAgain;
        case SUCCESS: break;
        }
        break;

    case '/':
        switch (lex_comment()) {
        case SUCCESS: goto scanAgain;
        default: break;
        }
        /* FALLTHROUGH */ /* to Invalid input char */

    default:
    BrokenToken:
        pCurCtx->pzScan = gather_name(pCurCtx->pzScan, &lastToken);
        break;
    }   /* switch (*pCurCtx->pzScan) */

    return lastToken;

NUL_error:

    AG_ABEND(aprf(DEF_ERR_FMT, pzProg, YYLEX_UNENDING_QUOTE,
                  pCurCtx->pzCtxFname, pCurCtx->lineNo));
    return DP_EV_INVALID;

lex_done:
    /*
     *  First time through, return the DP_EV_END token.
     *  Second time through, we really finish.
     */
    if (pCurCtx->pzScan == zNil) {
        pCurCtx->pCtx = pDoneCtx;
        pDoneCtx      = pCurCtx;

        return DP_EV_INVALID;
    }

    pCurCtx->pzScan = (char*)zNil;
    return DP_EV_END;
}


LOCAL void
yyerror(char* s)
{
    char * pz;

    if (strlen(pCurCtx->pzScan) > 64 )
        pCurCtx->pzScan[64] = NUL;

    switch (lastToken) {
    case DP_EV_VAR_NAME:
    case DP_EV_OTHER_NAME:
    case DP_EV_STRING:
    case DP_EV_NUMBER:
        if (strlen(pz_token) > 64 )
            pz_token[64] = NUL;

        pz = aprf(YYLEX_TOKEN_STR, DP_EVT_NAME(lastToken), pz_token);
        break;

    default:
        pz = aprf(YYLEX_DF_STR, DP_EVT_NAME(lastToken));
    }
    AG_ABEND(aprf(YYLEX_ERR_FMT, s, pCurCtx->pzCtxFname, pCurCtx->lineNo,
                  pz, pCurCtx->pzScan));
}


static void
loadScheme(void)
{
    char*    pzText    = pCurCtx->pzScan;
    char*    pzEnd     = (char*)skipScheme(pzText, pzText + strlen(pzText));
    char     endCh     = *pzEnd;
    int      schemeLen = (pzEnd - pzText);
    int      next_ln;
    SCM      res;

    /*
     *  NUL terminate the Scheme expression, run it, then restore
     *  the NUL-ed character.
     */
    if (*pzEnd == NUL)
        AG_ABEND(aprf(DEF_ERR_FMT, pzProg, LOAD_SCM_ENDLESS,
                      pCurCtx->pzCtxFname, pCurCtx->lineNo));

    *pzEnd  = NUL;
    next_ln = pCurCtx->lineNo + count_nl(pzText);

    procState = PROC_STATE_GUILE_PRELOAD;
    res = ag_scm_c_eval_string_from_file_line(
        pzText, pCurCtx->pzCtxFname, pCurCtx->lineNo );
    procState = PROC_STATE_LOAD_DEFS;
    *pzEnd = endCh;

    pCurCtx->pzScan = pzEnd;
    pzEnd = (char*)resolveSCM(res); /* ignore const-ness */
    pCurCtx->lineNo = next_ln;

    if (strlen(pzEnd) >= schemeLen) {
        AGDUPSTR(pzEnd, pzEnd, "SCM Result");

        pz_token = pzEnd;
        manageAllocatedData(pz_token);
    }

    else {
        /*
         *  We know the result is smaller than the source.  Copy in place.
         */
        strcpy(pzText, pzEnd);
        pz_token = pzText;
    }

    lastToken = DP_EV_STRING;
}

/*
 *  process a single scheme expression, yielding text that gets processed
 *  into AutoGen definitions.
 */
static void
alist_to_autogen_def(void)
{
    char*  pzText  = ++(pCurCtx->pzScan);
    char*  pzEnd   = (char*)skipScheme(pzText, pzText + strlen(pzText));

    SCM    res;
    size_t res_len;
    tScanCtx*  pCtx;

    /*
     *  Wrap the scheme expression with the `alist->autogen-def' function
     */
    {
        char endCh = *pzEnd;
        *pzEnd = NUL;
        pzText = aprf(ALIST_TO_AG_WRAP, pzText);
        *pzEnd = endCh;
    }

    /*
     *  Run the scheme expression.  The result is autogen definition text.
     */
    procState = PROC_STATE_GUILE_PRELOAD;
    res = ag_scm_c_eval_string_from_file_line(
        pzText, pCurCtx->pzCtxFname, pCurCtx->lineNo );

    /*
     *  The result *must* be a string, or we choke.
     */
    if (! AG_SCM_STRING_P(res))
        AG_ABEND(ALIST_TO_AG_ERR);

    res_len   = AG_SCM_STRLEN(res);
    procState = PROC_STATE_LOAD_DEFS;
    pCurCtx->pzScan = pzEnd;
    AGFREE(pzText);

    /*
     *  Now, push the resulting string onto the input stack
     *  and link the new scan data into the context stack
     */
    pCtx = (tScanCtx*)AGALOC(sizeof(tScanCtx) + 4 + res_len, "lex ctx");
    pCtx->pCtx  = pCurCtx;
    pCurCtx     = pCtx;

    /*
     *  Set up the rest of the context structure
     */
    AGDUPSTR(pCtx->pzCtxFname, ALIST_TO_AG_TEXT, "scheme text");
    pCtx->pzScan = \
    pCtx->pzData = (char*)(pCtx+1);
    pCtx->lineNo = 0;
    memcpy((void*)(pCtx->pzScan), (void*)AG_SCM_CHARS(res), res_len);
    pCtx->pzScan[ res_len ] = NUL;

    /*
     *  At this point, the next token will be obtained
     *  from the newly allocated context structure.
     *  When empty, input will resume from the '}' that we
     *  left as the next input token in the old context.
     */
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *  It may be a number, a name, a keyword or garbage.
 *  Figure out which.
 */
static char*
gather_name(char * pzScan, te_dp_event * pRetVal)
{
    /*
     *  Check for a number.
     *  Scan it in and advance "pzScan".
     */
    if (  IS_DEC_DIGIT_CHAR(*pzScan)
       || (  (*pzScan == '-')
          && IS_DEC_DIGIT_CHAR(pzScan[1])
       )  )  {
        pz_token = pzScan;
        (void)strtol(pzScan, &pzScan, 0);
        *pRetVal = DP_EV_NUMBER;
        return pzScan;
    }

    if (! IS_UNQUOTABLE_CHAR(*pzScan))
        AG_ABEND(aprf("%s Error: Invalid input char '%c' in %s on line %d\n",
                      pzProg, *pzScan, pCurCtx->pzCtxFname, pCurCtx->lineNo));

    {
        char* pz = SPN_VALUE_NAME_CHARS(pzScan);

        if (IS_UNQUOTABLE_CHAR(*pz)) {
            *pRetVal = DP_EV_OTHER_NAME;
            pz = SPN_UNQUOTABLE_CHARS(pz+1);

        } else
            *pRetVal = DP_EV_VAR_NAME;

        /*
         *  Return a NAME token, maybe.
         *  If the name is actually a keyword,
         *  we will return that token code instead.
         */
        pz_token = pzScan;
        pzScan   = (char*)pz;
    }

    /*
     *  Now scan the keyword table.
     */
    if (*pRetVal == DP_EV_VAR_NAME) {
        char sv_ch = *pzScan;  /* preserve the following character */
        int  kw_ix = 0;
        *pzScan = NUL;         /* NUL terminate the name           */

        do  {
            if (streqvcmp(apzKeywords[ kw_ix ], (char*)pz_token) == 0) {
                /*
                 *  Return the keyword token code instead of DP_EV_NAME
                 */
                *pRetVal = aKeywordTkn[ kw_ix ];
                break;
            }
        } while (++kw_ix < KEYWORD_CT);

        *pzScan = sv_ch;         /* restore the following character  */
    }

    return pzScan;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *  A quoted string has been found.
 *  Find the end of it and compress any escape sequences.
 */
static char*
assembleHereString(char* pzScan)
{
    ag_bool  trimTabs = AG_FALSE;
    char     zMark[ MAX_HEREMARK_LEN ];
    size_t   markLen = 0;
    char*    pzDest;
    int      here_string_line_no;

    /*
     *  See if we are to strip leading tab chars
     */
    if (*pzScan == '-') {
        trimTabs = AG_TRUE;
        pzScan++;
    }

    /*
     *  Skip white space up to the marker or EOL
     */
    while (IS_WHITESPACE_CHAR(*pzScan)) {
        if (*pzScan++ == NL)
            AG_ABEND(aprf(DEF_ERR_FMT, pzProg, "HereString missing the mark",
                          pCurCtx->pzCtxFname, pCurCtx->lineNo));
    }

    /*
     *  Copy the marker, noting its length
     */
    {
        char * pz = SPN_VARIABLE_NAME_CHARS(pzScan);
        markLen = pz - pzScan;

        if (markLen == 0)
            AG_ABEND(aprf(DEF_ERR_FMT, pzProg, HERE_MISS_MARK_STR,
                          pCurCtx->pzCtxFname, pCurCtx->lineNo));

        if (markLen >= sizeof(zMark))
            AG_ABEND(aprf(DEF_ERR_FMT, pzProg, HERE_MARK_TOO_LONG,
                          pCurCtx->pzCtxFname, pCurCtx->lineNo));

        memcpy(zMark, pzScan, markLen);
        zMark[markLen] = NUL;
    }

    pzDest   = pzScan;
    pz_token = pzDest;

    /*
     *  Skip forward to the EOL after the marker.
     */
    pzScan = strchr(pzScan, NL);
    if (pzScan == NULL)
        AG_ABEND(aprf(DEF_ERR_FMT, pzProg, HERE_ENDLESS_STR,
                      pCurCtx->pzCtxFname, pCurCtx->lineNo));

    /*
     *  And skip the first new line + conditionally skip tabs
     */
    here_string_line_no = pCurCtx->lineNo++;
    pzScan++;

    if (trimTabs)
        while (*pzScan == TAB)  ++pzScan;

    /*
     *  FOR as long as the text does not match the mark
     *       OR it matches but is a substring
     *  DO copy characters
     */
    while (  (strncmp(pzScan, zMark, markLen) != 0)
          || IS_VARIABLE_NAME_CHAR(pzScan[markLen]) )  {

        for (;;) {
            switch (*(pzDest++) = *(pzScan++)) {
            case NL:
                pCurCtx->lineNo++;
                goto lineDone;

            case NUL:
                AG_ABEND(aprf(DEF_ERR_FMT, pzProg, HERE_ENDLESS_STR,
                              pCurCtx->pzCtxFname, here_string_line_no));
            }
        } lineDone:;

        if (trimTabs)
            while (*pzScan == TAB)  ++pzScan;
    } /* while strncmp ... */

    /*
     *  pzDest may still equal pz_token, if no data were copied
     */
    if (pzDest > (char*)pz_token)
         pzDest[-1] = NUL;
    else pzDest[0]  = NUL;

    return pzScan + markLen;
}
/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * indent-tabs-mode: nil
 * End:
 * end of agen5/defLex.c */
