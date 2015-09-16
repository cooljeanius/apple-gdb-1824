
/**
 * @file funcIf.c
 *
 *  Time-stamp:        "2012-01-29 20:47:38 bkorb"
 *
 *  This module implements the _IF text function.
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

typedef struct if_stack tIfStack;
struct if_stack {
    tMacro*  pIf;
    tMacro*  pElse;
};

static tIfStack  current_if;
static tLoadProc mLoad_Elif, mLoad_Else;

/* = = = START-STATIC-FORWARD = = = */
static ag_bool
eval_true(void);

static tMacro*
mLoad_Elif(tTemplate* pT, tMacro* pMac, char const ** ppzScan);

static tMacro*
mLoad_Else(tTemplate* pT, tMacro* pMac, char const ** ppzScan);
/* = = = END-STATIC-FORWARD = = = */

/*
 *  eval_true - should a string be interpreted as TRUE?
 *
 *  It is always true unless:
 *
 *  1.  it is the empty string
 *  2.  it starts with a digit and the number evaluates to zero
 *  3.  it starts with either "#f" or "#F"
 *  4.  For its length or its first five characters (whichever is less)
 *      it matches the string "false"
 */
static ag_bool
eval_true(void)
{
    ag_bool needFree;
    ag_bool res = AG_TRUE;
    char const * pz = evalExpression(&needFree);

    if (IS_DEC_DIGIT_CHAR(*pz))
        res = (atoi(pz) == 0) ? AG_FALSE : AG_TRUE;

    else switch (*pz) {
    case NUL:
        res = AG_FALSE;
        break;

    case '#':
        if ((pz[1] == 'f') || (pz[1] == 'F'))
            res = AG_FALSE;
        break;

    case 'f':
    case 'F':
    {
        int len = strlen(pz);
        if (len > 5)
            len = 5;
        if (strneqvcmp(EVAL_TRUE_FALSE_STR, pz, len) == 0)
            res = AG_FALSE;
        break;
    }
    }

    if (needFree)
        AGFREE(pz);

    return res;
}


/*=macfunc IF
 *
 *  what:    Conditionally Emit a Template Block
 *  cindex:  conditional emit
 *  cindex:  if test
 *  handler_proc:
 *  load_proc:
 *
 *  desc:
 *  Conditional block.  Its arguments are evaluated (@pxref{EXPR}) and
 *  if the result is non-zero or a string with one or more bytes,
 *  then the condition is true and the text from that point
 *  until a matched @code{ELIF}, @code{ELSE} or @code{ENDIF} is emitted.
 *  @code{ELIF} introduces a conditional alternative if the @code{IF}
 *  clause evaluated FALSE and @code{ELSE} introduces an unconditional
 *  alternative.
 *
 *  @example
 *  [+IF <full-expression> +]
 *  emit things that are for the true condition[+
 *
 *  ELIF <full-expression-2> +]
 *  emit things that are true maybe[+
 *
 *  ELSE "This may be a comment" +]
 *  emit this if all but else fails[+
 *
 *  ENDIF "This may *also* be a comment" +]
 *  @end example
 *
 *  @noindent
 *  @code{<full-expression>} may be any expression described in the
 *  @code{EXPR} expression function, including the use of apply-codes
 *  and value-names.  If the expression yields an empty string, it
 *  is interpreted as @i{false}.
=*/
/*=macfunc ENDIF
 *
 *  what:   Terminate the @code{IF} Template Block
 *  in-context:
 *
 *  desc:
 *    This macro ends the @code{IF} function template block.
 *    For a complete description @xref{IF}.
=*/
tMacro*
mFunc_If(tTemplate* pT, tMacro* pMac)
{
    tMacro* pRet = pT->aMacros + pMac->endIndex;
    tMacro* pIf  = pMac;

    do  {
        /*
         *  The current macro becomes the 'ELIF' or 'ELSE' macro
         */
        pCurMacro = pMac;

        /*
         *  'ELSE' is equivalent to 'ELIF true'
         */
        if (  (pMac->funcCode == FTYP_ELSE)
           || eval_true()) {

            if (OPT_VALUE_TRACE >= TRACE_BLOCK_MACROS) {
                fprintf(pfTrace, TRACE_FN_IF, (pMac->funcCode == FTYP_ELSE)
                        ? FN_IF_ELSE : pT->pzTemplText + pMac->ozText,
                        pMac->lineNo);

                if (OPT_VALUE_TRACE == TRACE_EVERYTHING)
                    fprintf(pfTrace, TAB_FILE_LINE_FMT,
                            pCurTemplate->pzTplFile, pIf->lineNo);
            }

            generateBlock(pT, pMac+1, pT->aMacros + pMac->sibIndex);
            break;
        }
        pMac = pT->aMacros + pMac->sibIndex;
    } while (pMac < pRet);

    if ((OPT_VALUE_TRACE >= TRACE_BLOCK_MACROS) && (pMac >= pRet)) {
        fprintf(pfTrace, TRACE_FN_IF_NOTHING,
                pCurTemplate->pzTemplText + pCurMacro->ozText);

        if (OPT_VALUE_TRACE == TRACE_EVERYTHING)
            fprintf(pfTrace, TAB_FILE_LINE_FMT,
                    pCurTemplate->pzTplFile, pIf->lineNo);
    }

    return pRet;
}


/*=macfunc WHILE
 *
 *  what:    Conditionally loop over a Template Block
 *  cindex:  conditional emit
 *  cindex:  while test
 *  handler_proc:
 *  load_proc:
 *
 *  desc:
 *  Conditionally repeated block.  Its arguments are evaluated (@pxref{EXPR})
 *  and as long as the result is non-zero or a string with one or more bytes,
 *  then the condition is true and the text from that point
 *  until a matched @code{ENDWHILE} is emitted.
 *
 *  @example
 *  [+WHILE <full-expression> +]
 *  emit things that are for the true condition[+
 *
 *  ENDWHILE +]
 *  @end example
 *
 *  @noindent
 *  @code{<full-expression>} may be any expression described in the
 *  @code{EXPR} expression function, including the use of apply-codes
 *  and value-names.  If the expression yields an empty string, it
 *  is interpreted as @i{false}.
=*/
/*=macfunc ENDWHILE
 *
 *  what:   Terminate the @code{WHILE} Template Block
 *  in-context:
 *
 *  desc:
 *    This macro ends the @code{WHILE} function template block.
 *    For a complete description @xref{WHILE}.
=*/
tMacro*
mFunc_While(tTemplate* pT, tMacro* pMac)
{
    tMacro* pRet = pT->aMacros + pMac->endIndex;
    int     ct   = 0;

    if (OPT_VALUE_TRACE >= TRACE_BLOCK_MACROS)
        fprintf(pfTrace, TRACE_FN_WHILE_START,
                pCurTemplate->pzTemplText + pCurMacro->ozText,
                pT->pzTplFile, pMac->lineNo);

    for (;;) {
        pCurTemplate = pT;
        pCurMacro    = pMac;

        if (! eval_true())
            break;
        ct++;
        generateBlock(pT, pMac+1, pT->aMacros + pMac->sibIndex);
    }

    if (OPT_VALUE_TRACE >= TRACE_BLOCK_MACROS) {
        fprintf(pfTrace, TRACE_FN_WHILE_END, ct);

        if (OPT_VALUE_TRACE == TRACE_EVERYTHING)
            fprintf(pfTrace, TAB_FILE_LINE_FMT, pT->pzTplFile, pMac->lineNo);
    }

    return pRet;
}


/*=macfunc ELIF
 *
 *  what:   Alternate Conditional Template Block
 *  in-context:
 *
 *  desc:
 *    This macro must only appear after an @code{IF} function, and
 *    before any associated @code{ELSE} or @code{ENDIF} functions.
 *    It denotes the start of an alternate template block for the
 *    @code{IF} function.  Its expression argument is evaluated as are
 *    the arguments to @code{IF}.  For a complete description @xref{IF}.
=*/
static tMacro*
mLoad_Elif(tTemplate* pT, tMacro* pMac, char const ** ppzScan)
{
    if ((int)pMac->res == 0)
        AG_ABEND_IN(pT, pMac, NO_IF_EXPR);
    /*
     *  Load the expression
     */
    (void)mLoad_Expr(pT, pMac, ppzScan);

    current_if.pElse->sibIndex = pMac - pT->aMacros;
    current_if.pElse = pMac;
    return pMac + 1;
}


/*=macfunc ELSE
 *
 *  what:   Alternate Template Block
 *  in-context:
 *
 *  desc:
 *    This macro must only appear after an @code{IF} function,
 *    and before the associated @code{ENDIF} function.
 *    It denotes the start of an alternate template block for
 *    the @code{IF} function.  For a complete description @xref{IF}.
=*/
static tMacro*
mLoad_Else(tTemplate* pT, tMacro* pMac, char const ** ppzScan)
{
    /*
     *  After processing an "ELSE" macro,
     *  we have a special handler function for 'ENDIF' only.
     */
    static tpLoadProc apElseLoad[ FUNC_CT ] = { NULL };

    if (apElseLoad[0] == NULL) {
        memcpy((void*)apElseLoad, apLoadProc, sizeof(apLoadProc));
        apElseLoad[ FTYP_ENDIF ] = &mLoad_Ending;
    }

    papLoadProc = apElseLoad;

    current_if.pElse->sibIndex = pMac - pT->aMacros;
    current_if.pElse = pMac;
    pMac->ozText = 0;

    return pMac+1;
}


/*
 *  mLoad_Ending is the common block termination function.
 *  By returning NULL, it tells the macro parsing loop to return.
 */
tMacro*
mLoad_Ending(tTemplate* pT, tMacro* pMac, char const ** ppzScan)
{
    memset((void*)pMac, 0, sizeof(*pMac));
    return NULL;
}


tMacro*
mLoad_If(tTemplate* pT, tMacro* pMac, char const ** ppzScan)
{
    size_t              srcLen     = (size_t)pMac->res; /* macro len  */
    tIfStack            save_stack = current_if;
    tpLoadProc const *  papLP      = papLoadProc;
    tMacro *            pEndifMac;

    /*
     *  While processing an "IF" macro,
     *  we have handler functions for 'ELIF', 'ELSE' and 'ENDIF'
     *  Otherwise, we do not.  Switch the callout function table.
     */
    static tpLoadProc apIfLoad[ FUNC_CT ] = { NULL };

    /*
     *  IF there is no associated text expression
     *  THEN woops!  what are we to case on?
     */
    if (srcLen == 0)
        AG_ABEND_IN(pT, pMac, NO_IF_EXPR);

    if (apIfLoad[0] == NULL) {
        memcpy((void*)apIfLoad, apLoadProc, sizeof(apLoadProc));
        apIfLoad[ FTYP_ELIF ]  = &mLoad_Elif;
        apIfLoad[ FTYP_ELSE ]  = &mLoad_Else;
        apIfLoad[ FTYP_ENDIF ] = &mLoad_Ending;
    }

    papLoadProc = apIfLoad;

    /*
     *  We will need to chain together the 'IF', 'ELIF', and 'ELSE'
     *  macros.  The 'ENDIF' gets absorbed.
     */
    current_if.pIf = current_if.pElse = pMac;

    /*
     *  Load the expression
     */
    (void)mLoad_Expr(pT, pMac, ppzScan);

    /*
     *  Now, do a nested parse of the template.
     *  When the matching 'ENDIF' macro is encountered,
     *  the handler routine will cause 'parseTemplate()'
     *  to return with the text scanning pointer pointing
     *  to the remaining text.
     */
    pEndifMac = parseTemplate(pMac+1, ppzScan);
    if (*ppzScan == NULL)
        AG_ABEND_IN(pT, pMac, LD_IF_NO_ENDIF);

    current_if.pIf->endIndex   = \
    current_if.pElse->sibIndex = pEndifMac - pT->aMacros;

    /*
     *  Restore the context of any encompassing block macros
     */
    current_if  = save_stack;
    papLoadProc = papLP;
    return pEndifMac;
}


tMacro *
mLoad_While(tTemplate* pT, tMacro* pMac, char const ** ppzScan)
{
    size_t              srcLen = (size_t)pMac->res; /* macro len  */
    tpLoadProc const *  papLP  = papLoadProc;
    tMacro *            pEndMac;

    /*
     *  While processing an "IF" macro,
     *  we have handler functions for 'ELIF', 'ELSE' and 'ENDIF'
     *  Otherwise, we do not.  Switch the callout function table.
     */
    static tpLoadProc apWhileLoad[ FUNC_CT ] = { NULL };

    /*
     *  IF there is no associated text expression
     *  THEN woops!  what are we to case on?
     */
    if (srcLen == 0)
        AG_ABEND_IN(pT, pMac, LD_WHILE_NO_EXPR);

    if (apWhileLoad[0] == NULL) {
        memcpy((void*)apWhileLoad, apLoadProc, sizeof(apLoadProc));
        apWhileLoad[ FTYP_ENDWHILE ] = &mLoad_Ending;
    }

    papLoadProc = apWhileLoad;

    /*
     *  Load the expression
     */
    (void)mLoad_Expr(pT, pMac, ppzScan);

    /*
     *  Now, do a nested parse of the template.  When the matching 'ENDWHILE'
     *  macro is encountered, the handler routine will cause 'parseTemplate()'
     *  to return with the text scanning pointer pointing to the remaining
     *  text.
     */
    pEndMac = parseTemplate(pMac+1, ppzScan);
    if (*ppzScan == NULL)
        AG_ABEND_IN(pT, pMac, LD_WHILE_NO_ENDWHILE);

    pMac->sibIndex = pMac->endIndex = pEndMac - pT->aMacros;

    /*
     *  Restore the context of any encompassing block macros
     */
    papLoadProc = papLP;
    return pEndMac;
}


/*=gfunc set_writable
 *
 * what:   Make the output file be writable
 *
 * exparg: + set? + boolean arg, false to make output non-writable + opt
 *
 * doc:    This function will set the current output file to be writable
 *         (or not).  This is only effective if neither the @code{--writable}
 *         nor @code{--not-writable} have been specified.  This state
 *         is reset when the current suffix's output is complete.
=*/
SCM
ag_scm_set_writable(SCM set)
{
    switch (STATE_OPT(WRITABLE)) {
    case OPTST_DEFINED:
    case OPTST_PRESET:
        fprintf(pfTrace, SET_WRITE_WARN, pCurTemplate->pzTplFile,
                pCurMacro->lineNo);
        break;

    default:
        if (AG_SCM_BOOL_P(set) && (set == SCM_BOOL_F))
            CLEAR_OPT(WRITABLE);
        else
            SET_OPT_WRITABLE;
    }

    return SCM_UNDEFINED;
}
/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * indent-tabs-mode: nil
 * End:
 * end of agen5/funcIf.c */
