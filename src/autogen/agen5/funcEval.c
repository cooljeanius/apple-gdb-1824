
/**
 * @file funcEval.c
 *
 *  This module evaluates macro expressions.
 *
 *  Time-stamp:        "2012-01-29 09:49:11 bkorb"
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

/* = = = START-STATIC-FORWARD = = = */
static inline char const *
tpl_text(tTemplate * tpl, tMacro * mac);

static void
tpl_warning(tTemplate * tpl, tMacro * mac, char const * msg);

static int
exprType(char* pz);
/* = = = END-STATIC-FORWARD = = = */

LOCAL char const *
resolveSCM(SCM s)
{
    static char  z[48];
    char const * pzRes = z;

    switch (ag_scm_type_e(s)) {
    case GH_TYPE_BOOLEAN:
        z[0] = AG_SCM_NFALSEP(s) ? '1' : '0'; z[1] = NUL;
        break;

    case GH_TYPE_STRING:
    case GH_TYPE_SYMBOL:
        pzRes = ag_scm2zchars(s, "SCM Result");
        break;

    case GH_TYPE_CHAR:
        z[0] = AG_SCM_CHAR(s); z[1] = NUL; break;

    case GH_TYPE_VECTOR:
        pzRes = RESOLVE_SCM_VECTOR; break;

    case GH_TYPE_PAIR:
        pzRes = RESOLVE_SCM_PAIR; break;

    case GH_TYPE_NUMBER:
        snprintf(z, sizeof(z), RESOLVE_SCM_NUMBER, AG_SCM_TO_ULONG(s)); break;

    case GH_TYPE_PROCEDURE:
#ifdef SCM_SUBR_ENTRY
    {
        void * x = &SCM_SUBR_ENTRY(s);

        snprintf(z, sizeof(z), RESOLVE_SCM_PROC,
                 (unsigned long)x);
        break;
    }
#else
        pzRes = "** PROCEDURE **";
        break;
#endif

    case GH_TYPE_LIST:
        pzRes = RESOLVE_SCM_LIST; break;

    case GH_TYPE_INEXACT:
        pzRes = RESOLVE_SCM_INEXACT; break;

    case GH_TYPE_EXACT:
        pzRes = RESOLVE_SCM_EXACT; break;

    case GH_TYPE_UNDEFINED:
        pzRes = (char*)zNil; break;

    default:
        pzRes = RESOLVE_SCM_UNKNOWN; break;
    }

    return pzRes;
}

/**
 * Return the text associated with a macro.
 */
static inline char const *
tpl_text(tTemplate * tpl, tMacro * mac)
{
    if (mac->ozText == 0)
        return zNil;

    return tpl->pzTemplText + mac->ozText;
}

static void
tpl_warning(tTemplate * tpl, tMacro * mac, char const * msg)
{
    fprintf(pfTrace, TPL_WARN_FMT, tpl->pzTplFile, mac->lineNo, msg);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/**
 *  Evaluate an expression and return a string pointer.  Always.
 *  It may need to be deallocated, so a boolean pointer is used
 *  to tell the caller.  Also, the match existence and non-existence
 *  pay attention to the address of the empty string that gets
 *  returned.  If it is, specifically, "zNil", then this code is
 *  saying, "we could not compute a value, so we're returning this
 *  empty string".  This is used by the Select_Match_Existence and
 *  Select_Match_NonExistence code to distinguish non-existence from
 *  an empty string value.
 */
LOCAL char const *
evalExpression(ag_bool* pMustFree)
{
    tTemplate *   pT      = pCurTemplate;
    tMacro *      pMac    = pCurMacro;
    ag_bool       isIndexed;
    tDefEntry *   pDef;
    int           code    = pMac->res;
    char const *  pzText  = NULL; /* warning patrol */

    *pMustFree = AG_FALSE;

    if ((code & EMIT_NO_DEFINE) != 0) {
        pzText = tpl_text(pT, pMac);
        code  &= EMIT_PRIMARY_TYPE;
        pDef   = NULL; /* warning patrol */
    }

    else {
        /*
         *  Get the named definition entry, maybe
         */
        pDef = findDefEntry(pT->pzTemplText + pMac->ozName, &isIndexed);

        if (pDef == NULL) {
            switch (code & (EMIT_IF_ABSENT | EMIT_ALWAYS)) {
            case EMIT_IF_ABSENT:
                /*
                 *  There is only one expression.  It applies because
                 *  we did not find a definition.
                 */
                pzText = tpl_text(pT, pMac);
                code &= EMIT_PRIMARY_TYPE;
                break;

            case EMIT_ALWAYS:
                /*
                 *  There are two expressions.  Take the second one.
                 */
                pzText =  pT->pzTemplText + pMac->endIndex;
                code   = ((code & EMIT_SECONDARY_TYPE)
                          >> EMIT_SECONDARY_SHIFT);
                break;

            case 0:
                /*
                 *  Emit only if found
                 */
                return zNotDefined;

            case (EMIT_IF_ABSENT | EMIT_ALWAYS):
                /*
                 *  Emit inconsistently :-}
                 */
                AG_ABEND_IN(pT, pMac, EVAL_EXPR_PROG_ERR);
                /* NOTREACHED */
            }
        }

        /*
         *  OTHERWISE, we found an entry.  Make sure we were supposed to.
         */
        else {
            if ((code & EMIT_IF_ABSENT) != 0)
                return (char*)zNil;

            if (  (pDef->valType != VALTYP_TEXT)
               && ((code & EMIT_PRIMARY_TYPE) == EMIT_VALUE)  ) {
                tpl_warning(pT, pMac, EVAL_EXPR_BLOCK_IN_EVAL);
                return (char*)zNil;
            }

            /*
             *  Compute the expression string.  There are three possibilities:
             *  1.  There is an expression string in the macro, but it must
             *      be formatted with the text value.
             *      Make sure we have a value.
             *  2.  There is an expression string in the macro, but it is *NOT*
             *      to be formatted.  Use it as is.  Do *NOT* verify that
             *      the define value is text.
             *  3.  There is no expression with the macro invocation.
             *      The define value *must* be text.
             */
            if ((code & EMIT_FORMATTED) != 0) {
                /*
                 *  And make sure what we found is a text value
                 */
                if (pDef->valType != VALTYP_TEXT) {
                    tpl_warning(pT, pMac, EVAL_EXPR_BLOCK_IN_EVAL);
                    return (char*)zNil;
                }

                *pMustFree = AG_TRUE;
                pzText = aprf(tpl_text(pT, pMac), pDef->val.pzText);
            }

            else if (pMac->ozText != 0)
                pzText = pT->pzTemplText + pMac->ozText;

            else {
                /*
                 *  And make sure what we found is a text value
                 */
                if (pDef->valType != VALTYP_TEXT) {
                    tpl_warning(pT, pMac, EVAL_EXPR_BLOCK_IN_EVAL);
                    return (char*)zNil;
                }

                pzText = pDef->val.pzText;
            }

            code &= EMIT_PRIMARY_TYPE;
        }
    }

    /*
     *  The "code" tells us how to handle the expression
     */
    switch (code) {
    case EMIT_VALUE:
        assert(pDef != NULL);
        if (*pMustFree) {
            AGFREE((void*)pzText);
            *pMustFree = AG_FALSE;
        }

        pzText = pDef->val.pzText;
        break;

    case EMIT_EXPRESSION:
    {
        SCM res = ag_eval(pzText);

        if (*pMustFree) {
            AGFREE((void*)pzText);
            *pMustFree = AG_FALSE;
        }

        pzText = resolveSCM(res);
        break;
    }

    case EMIT_SHELL:
    {
        char* pz = shell_cmd(pzText);

        if (*pMustFree)
            AGFREE((void*)pzText);

        if (pz != NULL) {
            *pMustFree = AG_TRUE;
            pzText = pz;
        }
        else {
            *pMustFree = AG_FALSE;
            pzText = (char*)zNil;
        }
        break;
    }

    case EMIT_STRING:
        break;
    }

    return pzText;
}


/*=gfunc error_source_line
 *
 * what: display of file & line
 * general_use:
 * doc:  This function is only invoked just before Guile displays
 *       an error message.  It displays the file name and line number
 *       that triggered the evaluation error.  You should not need to
 *       invoke this routine directly.  Guile will do it automatically.
=*/
SCM
ag_scm_error_source_line(void)
{
    fprintf(stderr, SCM_ERROR_FMT, pCurTemplate->pzTplName, pCurMacro->lineNo,
            pCurTemplate->pzTemplText + pCurMacro->ozText);
    fflush(stderr);
    guileFailure = 1;

    return SCM_UNDEFINED;
}


/*=gfunc emit
 *
 * what: emit the text for each argument
 *
 * exparg: alist, list of arguments to stringify and emit, , list
 *
 * doc:  Walk the tree of arguments, displaying the values of displayable
 *       SCM types.  EXCEPTION: if the first argument is a number, then
 *       that number is used to index the output stack.  "0" is the default,
 *       the current output.
=*/
SCM
ag_scm_emit(SCM val)
{
    static int depth = 0;
    static FILE * fp;

    switch (depth) {
    case 1:
    {
        tFpStack* pSaveFp;
        unsigned long pnum;

        if (! AG_SCM_NUM_P(val))
            break;

        pSaveFp = pCurFp;
        pnum    = AG_SCM_TO_ULONG(val);

        for (; pnum > 0; pnum--) {
            pSaveFp = pSaveFp->pPrev;
            if (pSaveFp == NULL)
                AG_ABEND(aprf(EMIT_INVAL_PORT, AG_SCM_TO_ULONG(val)));
        }

        fp = pSaveFp->pFile;
        return SCM_UNDEFINED;
    }

    case 0:
        fp = pCurFp->pFile; // initialize the first time through
        break;
    } 

    depth++;
    for (;;) {
        if (val == SCM_UNDEFINED)
            break;

        if (AG_SCM_NULLP(val))
            break;

        if (AG_SCM_STRING_P(val)) {
            fputs((char*)ag_scm2zchars(val, "emit val"), fp);
            fflush(fp);
            break;
        }

        switch (ag_scm_type_e(val)) {
        case GH_TYPE_LIST:
        case GH_TYPE_PAIR:
            ag_scm_emit(SCM_CAR(val));
            val = SCM_CDR(val);
            continue;

        default:
            fputs(resolveSCM(val), fp);
            fflush(fp);
            break;
        }

        break;
    }

    depth--;
    return SCM_UNDEFINED;
}

/*
 *  eval
 *
 *  The global evaluation function.
 *  The string to "evaluate" may be a literal string, or may
 *  need Scheme interpretation.  So, we do one of three things:
 *  if the string starts with a Scheme comment character or
 *  evaluation character (';' or '('), then run a Scheme eval.
 *  If it starts with a quote character ('\'' or '"'), then
 *  digest the string and return that.  Otherwise, just return
 *  the string.
 */
LOCAL SCM
eval(char const* pzExpr)
{
    ag_bool allocated = AG_FALSE;
    char*   pzTemp;
    SCM   res;

    switch (*pzExpr) {
    case '(':
    case ';':
        res = ag_eval((char*)pzExpr);
        break;

    case '`':
        AGDUPSTR(pzTemp, pzExpr, "shell script");
        (void)spanQuote(pzTemp);
        pzExpr = shell_cmd(pzTemp);
        AGFREE((void*)pzTemp);
        res = AG_SCM_STR02SCM((char*)pzExpr);
        AGFREE((void*)pzExpr);
        break;

    case '"':
    case '\'':
        AGDUPSTR(pzTemp, pzExpr, "quoted string");
        (void)spanQuote(pzTemp);
        allocated = AG_TRUE;
        pzExpr = pzTemp;
        /* FALLTHROUGH */

    default:
        res = AG_SCM_STR02SCM((char*)pzExpr);
        if (allocated)
            AGFREE((void*)pzExpr);
    }

    return res;
}


/*=macfunc EXPR
 *
 *  what:  Evaluate and emit an Expression
 *  alias: + - + ? + % + ; + ( + '`' + '"' + "'" + . +
 *
 *  handler_proc:
 *  load_proc:
 *
 *  desc:
 *   This macro does not have a name to cause it to be invoked
 *   explicitly, though if a macro starts with one of the apply codes
 *   or one of the simple expression markers, then an expression
 *   macro is inferred.  The result of the expression evaluation
 *   (@pxref{expression syntax}) is written to the current output.
=*/
tMacro*
mFunc_Expr(tTemplate* pT, tMacro* pMac)
{
    ag_bool needFree;
    char const * pz = evalExpression(&needFree);

    if (*pz != NUL) {
        fputs(pz, pCurFp->pFile);
        fflush(pCurFp->pFile);
    }

    if (needFree)
        AGFREE((void*)pz);

    return pMac + 1;
}


static int
exprType(char* pz)
{
    switch (*pz) {
    case ';':
    case '(':
        return EMIT_EXPRESSION;

    case '`':
        spanQuote(pz);
        return EMIT_SHELL;

    case '"':
    case '\'':
        spanQuote(pz);
        /* FALLTHROUGH */

    default:
        return EMIT_STRING;
    }
}


/*
 *  mLoad_Expression
 */
tMacro*
mLoad_Expr(tTemplate* pT, tMacro* pMac, char const ** ppzScan)
{
    char *        pzCopy; /* next text dest   */
    char const *  pzSrc    = (char const*)pMac->ozText; /* macro text */
    size_t        srcLen   = (long)pMac->res;           /* macro len  */
    char const *  pzSrcEnd = pzSrc + srcLen;
    tMacro *      pNextMac;

    switch (*pzSrc) {
    case '-':
        pMac->res = EMIT_IF_ABSENT;
        pzSrc++;
        break;

    case '?':
        pMac->res = EMIT_ALWAYS;
        pzSrc++;
        if (*pzSrc == '%') {
            pMac->res |= EMIT_FORMATTED;
            pzSrc++;
        }
        break;

    case '%':
        pMac->res = EMIT_FORMATTED;
        pzSrc++;
        break;

    case '`':
        pNextMac  = mLoad_Unknown(pT, pMac, ppzScan);
        pMac->res = EMIT_NO_DEFINE | EMIT_SHELL;
        spanQuote(pT->pzTemplText + pMac->ozText);
        return pNextMac;

    case '"':
    case '\'':
        pNextMac  = mLoad_Unknown(pT, pMac, ppzScan);
        pMac->res = EMIT_NO_DEFINE | EMIT_STRING;
        spanQuote(pT->pzTemplText + pMac->ozText);
        return pNextMac;

    case '(':
    case ';':
        pNextMac  = mLoad_Unknown(pT, pMac, ppzScan);
        pMac->res = EMIT_NO_DEFINE | EMIT_EXPRESSION;
        return pNextMac;

    default:
        pMac->res = EMIT_VALUE; /* zero */
    }

    pzCopy = pT->pNext;
    pMac->ozName = (pzCopy - pT->pzTemplText);
    {
        size_t remLen = canonicalizeName(pzCopy, pzSrc, (int)srcLen);
        if (remLen > srcLen)
            AG_ABEND_IN(pT, pMac, LD_EXPR_BAD_NAME);
        pzSrc  += srcLen - remLen;
        srcLen  = remLen;
        pzCopy += strlen(pzCopy) + 1;
    }

    if (pzSrc >= pzSrcEnd) {
        if (pMac->res != EMIT_VALUE)
            AG_ABEND_IN(pT, pMac, LD_EXPR_NO_TEXT);

        pMac->ozText = 0;

    } else {
        char* pz = pzCopy;
        long  ct = srcLen = (long)(pzSrcEnd - pzSrc);

        pMac->ozText = (pzCopy - pT->pzTemplText);
        /*
         *  Copy the expression
         */
        do { *(pzCopy++) = *(pzSrc++); } while (--ct > 0);
        *(pzCopy++) = NUL; *(pzCopy++) = NUL; /* double terminate */

        /*
         *  IF this expression has an "if-present" and "if-not-present"
         *  THEN find the ending expression...
         */
        if ((pMac->res & EMIT_ALWAYS) != 0) {
            char* pzNextExpr = (char*)skipExpression(pz, srcLen);

            /*
             *  The next expression must be within bounds and space separated
             */
            if (pzNextExpr >= pz + srcLen)
                AG_ABEND_IN(pT, pMac, LD_EXPR_NEED_TWO);

            if (! IS_WHITESPACE_CHAR(*pzNextExpr))
                AG_ABEND_IN(pT, pMac, LD_EXPR_NO_SPACE);

            /*
             *  NUL terminate the first expression, skip intervening
             *  white space and put the secondary expression's type
             *  into the macro type code as the "secondary type".
             */
            *(pzNextExpr++) = NUL;
            pzNextExpr = SPN_WHITESPACE_CHARS(pzNextExpr);
            pMac->res |= (exprType(pzNextExpr) << EMIT_SECONDARY_SHIFT);
            pMac->endIndex = pzNextExpr - pT->pzTemplText;
        }

        pMac->res |= exprType(pz);
    }

    pT->pNext = pzCopy;
    return pMac + 1;
}
/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * indent-tabs-mode: nil
 * End:
 * end of agen5/funcEval.c */
