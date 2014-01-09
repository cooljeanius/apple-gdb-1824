
/**
 * @file functions.c
 *
 *  Time-stamp:        "2012-01-29 20:21:28 bkorb"
 *
 *  This module implements text functions.
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

/*=macfunc INCLUDE
 *
 *  what:   Read in and emit a template block
 *  handler_proc:
 *  load_proc:
 *
 *  desc:
 *
 *  The entire contents of the named file is inserted at this point.
 *  The contents of the file are processed for macro expansion.  The
 *  arguments are eval-ed, so you may compute the name of the file to
 *  be included.  The included file must not contain any incomplete
 *  function blocks.  Function blocks are template text beginning with
 *  any of the macro functions @samp{CASE}, @samp{DEFINE}, @samp{FOR},
 *  @samp{IF} and @samp{WHILE}; extending through their respective
 *  terminating macro functions.
=*/
tMacro*
mFunc_Include(tTemplate* pT, tMacro* pMac)
{
    tTemplate *   pNewTpl;
    ag_bool       needFree;
    char const *  pzFile = evalExpression(&needFree);
    tMacro*       pM;

    if (*pzFile != NUL) {
        pNewTpl = loadTemplate(pzFile, pT->pzTplFile);

        /*
         *  Strip off trailing white space from included templates
         */
        pM = pNewTpl->aMacros + (pNewTpl->macroCt - 1);
        if (pM->funcCode == FTYP_TEXT) {
            char* pz  = pNewTpl->pzTemplText + pM->ozText;
            char* pzE = pz + strlen(pz);
            while ((pzE > pz) && IS_WHITESPACE_CHAR(pzE[-1]))  --pzE;

            /*
             *  IF there is no text left, remove the macro entirely
             */
            if (pz == pzE)
                 pNewTpl->macroCt--;
            else *pzE = NUL;
        }

        if (OPT_VALUE_TRACE > TRACE_DEBUG_MESSAGE) {
            fprintf(pfTrace, TRACE_FN_INC_TPL, pNewTpl->pzTplFile);
            if (OPT_VALUE_TRACE == TRACE_EVERYTHING)
                fprintf(pfTrace, TRACE_FN_INC_LINE, pCurTemplate->pzTplFile,
                        pMac->lineNo);
        }

        generateBlock(pNewTpl, pNewTpl->aMacros,
                      pNewTpl->aMacros + pNewTpl->macroCt);
        unloadTemplate(pNewTpl);
        pCurTemplate = pT;
    }

    if (needFree)
        AGFREE((void*)pzFile);

    return pMac + 1;
}


/*
 *  mLoad_Include  --  digest an INCLUDE macro
 *
 *  Simply verify that there is some argument to this macro.
 *  Regular "expr" macros are their own argument, so there is always one.
 */
tMacro*
mLoad_Include(tTemplate* pT, tMacro* pMac, char const ** ppzScan)
{
    if ((int)pMac->res == 0)
        AG_ABEND_IN(pT, pMac, LD_INC_NO_FNAME);
    return mLoad_Expr(pT, pMac, ppzScan);
}


/*=macfunc UNKNOWN
 *
 *  what:  Either a user macro or a value name.
 *  handler_proc:
 *  load_proc:
 *  unnamed:
 *
 *  desc:
 *
 *  The macro text has started with a name not known to AutoGen.  If, at run
 *  time, it turns out to be the name of a defined macro, then that macro is
 *  invoked.  If it is not, then it is a conditional expression that is
 *  evaluated only if the name is defined at the time the macro is invoked.
 *
 *  You may not specify @code{UNKNOWN} explicitly.
=*/
tMacro*
mFunc_Unknown(tTemplate* pT, tMacro* pMac)
{
    tTemplate * pInv = findTemplate(pT->pzTemplText + pMac->ozName);
    if (pInv != NULL) {
        if (OPT_VALUE_TRACE >= TRACE_EVERYTHING)
            fprintf(pfTrace, TRACE_FN_REMAPPED, TRACE_FN_REMAP_INVOKE,
                    pMac->funcCode, pT->pzTplFile, pMac->lineNo);
        pMac->funcCode    = FTYP_DEFINE;
        pMac->funcPrivate = (void*)pInv;
        parseMacroArgs(pT, pMac);
        return mFunc_Define(pT, pMac);
    }

    if (OPT_VALUE_TRACE >= TRACE_EVERYTHING) {
        fprintf(pfTrace, TRACE_FN_REMAPPED, TRACE_FN_REMAP_EXPR,
                pMac->funcCode, pT->pzTplFile, pMac->lineNo);
        fprintf(pfTrace, TRACE_FN_REMAP_BASE, pT->pzTemplText + pMac->ozName);
    }

    pMac->funcCode = FTYP_EXPR;
    if (pMac->ozText == 0) {
        pMac->res = EMIT_VALUE;

    } else {
        char* pzExpr = pT->pzTemplText + pMac->ozText;
        switch (*pzExpr) {
        case ';':
        case '(':
            pMac->res = EMIT_EXPRESSION;
            break;

        case '`':
            pMac->res = EMIT_SHELL;
            spanQuote(pzExpr);
            break;

        case '"':
        case '\'':
            spanQuote(pzExpr);
            /* FALLTHROUGH */

        default:
            pMac->res = EMIT_STRING;
        }

        if (OPT_VALUE_TRACE >= TRACE_EVERYTHING)
            fprintf(pfTrace, TRACE_UNKNOWN_FMT, pMac->res, pzExpr);
    }

    return mFunc_Expr(pT, pMac);
}


/*=macfunc BOGUS
 *
 *  what:  Out-of-context or unknown functions are bogus.
 *  handler_proc:
 *  load_proc:
 *  unnamed:
=*/
tMacro*
mFunc_Bogus(tTemplate* pT, tMacro* pMac)
{
    char * pz = aprf(FN_BOGUS_FMT, pMac->funcCode,
                     (pMac->funcCode < FUNC_CT)
                     ? apzFuncNames[ pMac->funcCode ]
                     : FN_BOGUS_HUH);

    AG_ABEND_IN(pT, pMac, pz);
    /* NOTREACHED */
    return pMac;
}


/*=macfunc TEXT
 *
 *  what:  A block of text to be emitted.
 *  handler_proc:
 *  unnamed:
=*/
tMacro*
mFunc_Text(tTemplate* pT, tMacro* pMac)
{
    fputs(pT->pzTemplText + pMac->ozText, pCurFp->pFile);
    fflush(pCurFp->pFile);
    return pMac + 1;
}


/*=macfunc COMMENT
 *
 *  what:  A block of comment to be ignored
 *  load_proc:
 *  alias: "#"
 *
 *  desc:
 *    This function can be specified by the user, but there will
 *    never be a situation where it will be invoked at emit time.
 *    The macro is actually removed from the internal representation.
 *
 *    If the native macro name code is @code{#}, then the
 *    entire macro function is treated as a comment and ignored.
 *
 *    @example
 *    [+ # say what you want, but no '+' before any ']' chars +]
 *    @end example
=*/
tMacro *
mLoad_Comment(tTemplate * pT, tMacro * pMac, char const ** ppzScan)
{
    memset((void*)pMac, 0, sizeof(*pMac));
    return pMac;
}


/*
 *  mLoad_Unknown  --  the default (unknown) load function
 *
 *  Move any text into the text offset field.
 *  This is used as the default load mechanism.
 */
tMacro *
mLoad_Unknown(tTemplate * pT, tMacro * pMac, char const ** ppzScan)
{
    char *        pzCopy = pT->pNext;
    char const *  pzSrc;
    size_t        srcLen = (size_t)pMac->res;         /* macro len  */

    if (srcLen <= 0)
        goto return_emtpy_expression;

    pzSrc = (char const*)pMac->ozText; /* macro text */

    switch (*pzSrc) {
    case ';':
        /*
         *  Strip off scheme comments
         */
        do  {
            while (--srcLen, (*++pzSrc != NL)) {
                if (srcLen <= 0)
                    goto return_emtpy_expression;
            }

            while (--srcLen, IS_WHITESPACE_CHAR(*++pzSrc)) {
                if (srcLen <= 0)
                    goto return_emtpy_expression;
            }
        } while (*pzSrc == ';');
        break;

    case '[':
    case '.':
    {
        size_t remLen;

        /*
         *  We are going to recopy the definition name,
         *  this time as a canonical name (i.e. with '[', ']' and '.'
         *  characters and all blanks squeezed out)
         */
        pzCopy = pT->pzTemplText + pMac->ozName;

        /*
         *  Move back the source pointer.  We may have skipped blanks,
         *  so skip over however many first, then back up over the name.
         */
        while (IS_WHITESPACE_CHAR(pzSrc[-1]))
            pzSrc--, srcLen++;
        remLen  = strlen(pzCopy);
        pzSrc  -= remLen;
        srcLen += remLen;

        /*
         *  Now copy over the full canonical name.  Check for errors.
         */
        remLen = canonicalizeName(pzCopy, pzSrc, (int)srcLen);
        if (remLen > srcLen)
            AG_ABEND_IN(pT, pMac, LD_UNKNOWN_INVAL_DEF);

        pzSrc  += srcLen - remLen;
        srcLen  = remLen;

        pT->pNext = pzCopy + strlen(pzCopy) + 1;
        if (srcLen <= 0)
            goto return_emtpy_expression;
        break;
    }
    }

    /*
     *  Copy the expression
     */
    pzCopy = pT->pNext; /* next text dest   */
    pMac->ozText = (pzCopy - pT->pzTemplText);
    pMac->res    = 0;
    memcpy(pzCopy, pzSrc, srcLen);
    pzCopy      += srcLen;
    *(pzCopy++)  = NUL;
    *pzCopy      = NUL; /* double terminate */
    pT->pNext    = pzCopy;

    return pMac + 1;

 return_emtpy_expression:
    pMac->ozText = 0;
    pMac->res    = 0;
    return pMac + 1;
}


/**
 *  Some functions are known to AutoGen, but invalid out of context.
 *  For example, ELIF, ELSE and ENDIF are all known to AutoGen.
 *  However, the load function pointer for those functions points
 *  here, until an "IF" function is encountered.
 */
tMacro*
mLoad_Bogus(tTemplate* pT, tMacro* pMac, char const ** ppzScan)
{
    char const * pzSrc = (char const*)pMac->ozText; /* macro text */
    char const * pzMac;

    char z[ 64 ];

    if (pzSrc != NULL) {
        z[0] = ':';
        z[1] = z[2] = ' ';
        strncpy(z+3, pzSrc, (size_t)60);
        z[63] = NUL;
        pzSrc = z;
    }
    else
        pzSrc = zNil;

    {
        int ix = pMac->funcCode;
        if ((unsigned)ix >= FUNC_CT)
            ix = 0;

        pzMac = apzFuncNames[ ix ];
    }

    pzSrc = aprf(LD_BOGUS_UNKNOWN, pT->pzTplFile, pMac->lineNo, pzMac, pzSrc);

    AG_ABEND_IN(pT, pMac, pzSrc);
    /* NOTREACHED */
    return NULL;
}
/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * indent-tabs-mode: nil
 * End:
 * end of agen5/functions.c */
