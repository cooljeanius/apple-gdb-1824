
/**
 * @file funcDef.c
 *
 *  This module implements the DEFINE text function.
 *
 *  Time-stamp:        "2012-02-12 09:01:11 bkorb"
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

typedef int (tCmpProc)(const void*, const void*);

typedef struct def_list tDefList;
struct def_list {
    tDefEntry  de;
    char*      pzExpr;
};

/* = = = START-STATIC-FORWARD = = = */
static int
order_def_list(const void* p1, const void* p2);

static tDefList *
link_twins(tDefList* pDL, tDefList* pNext, int* pCt);

static uint_t
count_named_values(tTemplate* pT, tMacro* pMac);

static char *
gather_assigned_value(char * pzScan, tDefList * pDL);

static void
fill_in_values(tDefList * pDL, char * pzScan, tTemplate* pT, tMacro* pMac);

static void
prep_invoke_args(tMacro * pMac);

static void
build_defs(int defCt, tDefList* pList);
/* = = = END-STATIC-FORWARD = = = */

static int
order_def_list(const void* p1, const void* p2)
{
    tDefEntry * pDL1 = (tDefEntry*)p1;
    tDefEntry * pDL2 = (tDefEntry*)p2;
    int cmp = streqvcmp(pDL1->pzDefName, pDL2->pzDefName);

    /*
     *  IF the names are the same, then we order them based on which name
     *  appears first.  Do not reorder entries with the same name.
     */
    if (cmp == 0)
        cmp = (int)(pDL1->pzDefName - pDL2->pzDefName);
    return cmp;
}

static tDefList *
link_twins(tDefList* pDL, tDefList* pNext, int* pCt)
{
    tDefList* pN;
    int  ct  = *pCt;
    int  idx = 1;

    pDL->de.pTwin = &(pNext->de);
    pNext->de.pPrevTwin = &(pDL->de);

    for (;;) {
        pNext->de.index = idx++;
        pN = pNext + 1; /* We return this, valid or not */
        if (--ct <= 0)  /* count each successive twin   */
            break;
        if (streqvcmp(pNext->de.pzDefName, pN->de.pzDefName) != 0)
            break;

        /*
         *  We have found another twin.  Link it in and advance
         */
        pNext->de.pTwin  = &(pN->de);
        pN->de.pPrevTwin = &(pNext->de);
        pNext = pN;
    }

    pDL->de.pEndTwin  = &(pNext->de);
    pNext->de.pTwin   = NULL; /* NULL terminated list */
    pDL->de.pPrevTwin = NULL; /* NULL terminated list */
    *pCt = ct;
    pDL->de.pNext = NULL;     /* in case ct == 0      */
    return pN; /* If ct is zero, then this is invalid */
}


static uint_t
count_named_values(tTemplate* pT, tMacro* pMac)
{
    char * pzScan = pT->pzTemplText + pMac->ozText;
    uint_t ct = 0;

    while (*pzScan != NUL) {
        ct++;
        if (! IS_VAR_FIRST_CHAR(*pzScan)) {
            fprintf(stderr, NAMED_VALUES_WHERE, ct, pzScan);
            AG_ABEND_IN(pT, pMac, NAMED_VALUES_NO_NAME);
        }

        pzScan = SPN_VALUE_NAME_CHARS(pzScan);
        pzScan = SPN_WHITESPACE_CHARS(pzScan);
        if (*pzScan != '=')
            continue;
        pzScan = SPN_WHITESPACE_CHARS(pzScan+1);
        pzScan = (char*)skipExpression(pzScan, strlen(pzScan));
        pzScan = SPN_WHITESPACE_CHARS(pzScan);
    }

    return ct;
}


static char *
gather_assigned_value(char * pzScan, tDefList * pDL)
{
    pzScan = SPN_WHITESPACE_CHARS(pzScan);
    strtransform(pDL->de.pzDefName, pDL->de.pzDefName);
    pDL->pzExpr = pzScan;
    pDL->de.valType = VALTYP_TEXT;
    pzScan = (char*)skipExpression(pzScan, strlen(pzScan));

    /*
     *  Figure out what kind of expression we have
     */
    switch (*pDL->pzExpr) {
    case ';':
    case '(':
        /*
         *  These expressions will need evaluation
         */
        break;

    case '`':
    {
        char* pz;
        /*
         *  Process the quoted string, but leave a '`' marker, too
         */
        AGDUPSTR(pz, pDL->pzExpr, "macro arg expr");
        spanQuote(pz);
        strcpy(pDL->pzExpr+1, pz);
        AGFREE((void*)pz);
        break;
    }
    case '"':
    case '\'':
        /*
         *  Process the quoted strings now
         */
        if ((pzScan - pDL->pzExpr) < 24) {
            char* pz = (char*)AGALOC(24, "quoted string");
            memcpy((void*)pz, pDL->pzExpr, (size_t)(pzScan - pDL->pzExpr));
            pDL->pzExpr = pz;
            manageAllocatedData(pz);
        }
        spanQuote(pDL->pzExpr);
        /* FALLTHROUGH */

    default:
        /*
         *  Default:  the raw sequence of characters is the value
         */
        pDL->de.val.pzText = pDL->pzExpr;
        pDL->pzExpr        = NULL;
    }

    return pzScan;
}


static void
fill_in_values(tDefList * pDL, char * pzScan, tTemplate* pT, tMacro* pMac)
{
    for (;; pDL++ ) {
        pDL->de.pzDefName = pzScan;
        pzScan = SPN_VALUE_NAME_CHARS(pzScan);

        switch (*pzScan) {
        case NUL:
            pDL->de.val.pzText = (char*)zNil;
            return;

        default:
            AG_ABEND_IN(pT, pMac, FILL_IN_VAL_NO_ASSIGN);

        case ' ': case TAB: case NL: case '\f':
            *(pzScan++) = NUL;
            pzScan = SPN_WHITESPACE_CHARS(pzScan);

            /*
             *  The name was separated by space, but has no value
             */
            if (*pzScan != '=') {
                pDL->de.val.pzText = (char*)zNil;
                if (*pzScan == NUL)
                    return;
                continue;
            }
            /* FALLTHROUGH */

        case '=':
            *(pzScan++) = NUL;
        }

        /*
         *  When we arrive here, we have just clobbered a '=' char.
         *  Now we have gather up the assigned value.
         */
        pzScan = gather_assigned_value(pzScan, pDL);

        /*
         *  IF the next char is NUL, we are done.
         *  OTHERWISE, the next character must be a space
         */
        if (*pzScan == NUL)
            break;

        if (! IS_WHITESPACE_CHAR(*pzScan))
            AG_ABEND_IN(pT, pMac, FILL_IN_VAL_NO_SEP);

        /*
         *  Terminate the string value and skip over any additional space
         */
        *(pzScan++) = NUL;
        pzScan = SPN_WHITESPACE_CHARS(pzScan);
    }
}

/*
 *  parseMacroArgs
 *
 *  This routine is called just before the first call to mFunc_Define
 *  for a particular macro invocation.  It scans the text of the invocation
 *  for name-value assignments that are only to live for the duration
 *  of the processing of the user defined macro.
 */
LOCAL void
parseMacroArgs(tTemplate* pT, tMacro* pMac)
{
    char *      pzScan = pT->pzTemplText + pMac->ozText;
    uint_t      ct;
    tDefList *  pDL;
    tDefList *  pN;

    /*
     *  If there is no argument text, then the arg count is zero.
     */
    if (pMac->ozText == 0) {
        pMac->res = 0;
        return;
    }

    ct = count_named_values(pT, pMac);

    /*
     *  The result is zero if we don't have any
     */
    pMac->sibIndex = ct;
    if (ct == 0) {
        pMac->ozText = 0;
        pMac->res = 0;
        return;
    }

    /*
     *  Allocate the array of definition descriptors
     */
    pzScan = pT->pzTemplText + pMac->ozText;
    pDL = (tDefList*)AGALOC(ct * sizeof(tDefList), "array of def desc");
    memset((void*)pDL, 0, ct * sizeof(tDefList));
    pMac->res = (uintptr_t)pDL;

    /*
     *  Fill in the array of value assignments
     */
    fill_in_values(pDL, pzScan, pT, pMac);

    if (ct > 1) {
        /*
         *  There was more than one value assignment.
         *  Sort them just so we know the siblings are together.
         *  Order is preserved by comparing string addresses,
         *  if the strings compare equal.
         */
        pDL = (tDefList*)pMac->res;
        qsort((void*)pDL, (size_t)ct, sizeof(tDefList), order_def_list);

        /*
         *  Now, link them all together.  Singly linked list.
         */
        for (;;) {
            if (--ct == 0) {
                pDL->de.pNext = NULL;
                break;
            }

            pN = pDL + 1;

            /*
             *  IF the next entry has the same name,
             *  THEN it is a "twin".  Link twins on the twin list.
             */
            if (streqvcmp(pDL->de.pzDefName, pN->de.pzDefName) == 0) {
                pN = link_twins(pDL, pN, (int*)&ct);
                if (ct <= 0)
                    break;  /* pN is now invalid */
            }

            pDL->de.pNext = &(pN->de);
            pDL = pN;
        }
    }
}

static void
prep_invoke_args(tMacro * pMac)
{
    char * pzText;
    tTemplate * pT = pCurTemplate;

    if (pMac->ozText == 0)
        AG_ABEND_IN(pT, pMac, PREP_INVOKE_NO_NAME);
    pMac->ozName = pMac->ozText;
    pzText = pT->pzTemplText + pMac->ozText;
    pzText = (char*)skipExpression(pzText, strlen(pzText));

    /*
     *  IF there is no more text,
     *  THEN there are no arguments
     */
    if (*pzText == NUL) {
        pMac->ozText = 0;
        pMac->res = 0;
    }

    /*
     *  OTHERWISE, skip to the start of the text and process
     *  the arguments to the macro
     */
    else {
        if (! IS_WHITESPACE_CHAR(*pzText))
            AG_ABEND_IN(pT, pMac, PREP_INVOKE_NO_SEP);
        *pzText = NUL;
        pzText = SPN_WHITESPACE_CHARS(pzText + 1);
        pMac->ozText = pzText - pT->pzTemplText;
        parseMacroArgs(pT, pMac);
        pCurTemplate = pT;
    }
}


/*=macfunc DEBUG
 *
 *  handler-proc:
 *  load-proc:
 *
 *  what:  Print debug message to trace output
 *  desc:
 *
 *      If the tracing level is at "debug-message" or above
 *      (@pxref{autogen trace}), this macro prints a debug message to trace
 *      output.  This message is not evaluated.  This macro can also be used to
 *      set useful debugger breakpoints.  By inserting [+DEBUG n+] into your
 *      template, you can set a debugger breakpoint on the #n case element
 *      below (in the AutoGen source) and step through the processing of
 *      interesting parts of your template.
 *
 *      To be useful, you have to have access to the source tree where autogen
 *      was built and the template being processed.  The definitions are also
 *      helpful, but not crucial.  Please contact the author if you think you
 *      might actually want to use this.
=*/
tMacro *
mFunc_Debug(tTemplate* pT, tMacro* pMac)
{
    static int dummy = 0;
    char const * pz  = pT->pzTemplText + pMac->ozText;
    int  for_index = (forInfo.fi_depth <= 0)
        ? -1
        : forInfo.fi_data[ forInfo.fi_depth - 1].for_index;

    fprintf(pfTrace, FN_DEBUG, pz, for_index);

    /*
     *  The case element values were chosen to thwart most
     *  optimizers that might be too bright for its own good.
     *  (`dummy' is write-only and could be ignored)
     */
    do  {
        if (IS_DEC_DIGIT_CHAR(*pz)) {
            for_index = atoi(pz);
            break;
        }
    } while (*(pz++) != NUL);

    if (for_index < 0)
        for_index = -1;

    switch (for_index) {
    case -1:   dummy = 'X'; break;
    case 0:    dummy = 'A'; break;
    case 1:    dummy = 'u'; break;
    case 2:    dummy = 't'; break;
    case 3:    dummy = 'o'; break;
    case 4:    dummy = 'G'; break;
    case 5:    dummy = 'e'; break;
    case 6:    dummy = 'n'; break;
    case 7:    dummy = 'N'; break;
    case 8:    dummy = 'U'; break;
    case 9:    dummy = 'T'; break;
    case 10:   dummy = '.'; break;
    default:   dummy++;
    }
    if (IS_GRAPHIC_CHAR(dummy))
        fprintf(pfTrace, FN_DEBUG_GRAPHIC, dummy);
    putc(NL, pfTrace);
    return pMac+1;
}


/*
 *  build_defs
 *
 *  Build up a definition context created by passed-in macro arguments
 */
static void
build_defs(int defCt, tDefList* pList)
{
    tDefEntry* pDefs = &(pList->de);
    currDefCtx.pDefs = pDefs;

    /*
     *  FOR each definition, evaluate the associated expression
     *      and set the text value to it.
     */
    do  {
        if (pList->pzExpr == NULL)
            continue;

    retryExpression:
        switch (*(pList->pzExpr)) {
        case ';':
        {
            char* pz = strchr(pList->pzExpr, NL);
            if (pz != NULL) {
                pz = SPN_WHITESPACE_CHARS(pz + 1);
                pList->pzExpr = pz;
                goto retryExpression;
            }
            /* FALLTHROUGH */
        }
        case NUL:
            pList->pzExpr = NULL;
            pList->de.val.pzText = (char*)zNil;
            break;

        case '(':
        {
            SCM res;

            /*
             *  It is a scheme expression.  Accept only string
             *  and number results.
             */
            if (OPT_VALUE_TRACE >= TRACE_EXPRESSIONS) {
                fprintf(pfTrace, TRACE_BUILD_DEFS,
                        pCurMacro->sibIndex - defCt, pList->pzExpr);
            }

            res = ag_eval(pList->pzExpr);
            if (AG_SCM_STRING_P(res)) {
                AGDUPSTR(pList->de.val.pzText, ag_scm2zchars(res, "eval res"),
                         "dup eval res");
            }
            else if (AG_SCM_NUM_P(res)) {
                pList->de.val.pzText = AGALOC(16, "number buf");
                snprintf(pList->de.val.pzText, (size_t)16, BUILD_DEFS_LONG_FMT,
                         AG_SCM_TO_ULONG(res));
            }
            else
                AGDUPSTR(pList->de.val.pzText, zNil, "empty str");
            break;
        }

        case '`':
            if (OPT_VALUE_TRACE >= TRACE_EXPRESSIONS) {
                fprintf(pfTrace, TRACE_BUILD_DEFS,
                        pCurMacro->sibIndex - defCt, pList->pzExpr+1);
            }
            pList->de.val.pzText = shell_cmd(pList->pzExpr+1);
            break;
        }
    } while (pList++, --defCt > 0);
}


/*=macfunc DEFINE
 *
 *  what:    Define a user AutoGen macro
 *  cindex:  define macro
 *  handler_proc:
 *  load_proc:
 *  unload-proc:
 *
 *  desc:
 *
 *  This function will define a new macro.  You must provide a name for the
 *  macro.  You do not specify any arguments, though the invocation may
 *  specify a set of name/value pairs that are to be active during the
 *  processing of the macro.
 *
 *  @example
 *  [+ define foo +]
 *  ... macro body with macro functions ...
 *  [+ enddef +]
 *  ... [+ foo bar='raw text' baz=<<text expression>> +]
 *  @end example
 *
 *  Once the macro has been defined, this new macro can be invoked by
 *  specifying the macro name as the first token after the start macro marker.
 *  Alternatively, you may make the invocation explicitly invoke a defined
 *  macro by specifying @code{INVOKE} (@pxref{INVOKE}) in the macro
 *  invocation.  If you do that, the macro name can be computed with an
 *  expression that gets evaluated every time the INVOKE macro is encountered.
 *
 *  Any remaining text in the macro invocation will be used to create new
 *  name/value pairs that only persist for the duration of the processing of
 *  the macro.  The expressions are evaluated the same way basic
 *  expressions are evaluated.  @xref{expression syntax}.
 *
 *  The resulting definitions are handled much like regular
 *  definitions, except:
 *
 *  @enumerate
 *  @item
 *  The values may not be compound.  That is, they may not contain
 *  nested name/value pairs.
 *  @item
 *  The bindings go away when the macro is complete.
 *  @item
 *  The name/value pairs are separated by whitespace instead of
 *  semi-colons.
 *  @item
 *  Sequences of strings are not concatenated.
 *  @end enumerate
 *
 *  @quotation
 *  @strong{NB:} The macro is extracted from the template as the template is
 *  scanned.  You cannot conditionally define a macro by enclosing it in an
 *  @code{IF}/@code{ENDIF} (@pxref{IF}) macro pair.  If you need to dynamically
 *  select the format of a @code{DEFINE}d macro, then put the flavors into
 *  separate template files that simply define macros.  @code{INCLUDE}
 *  (@pxref{INCLUDE}) the appropriate template when you have computed which
 *  you need.
 *  @end quotation
 *
 *  Due to this, it is acceptable and even a good idea to place all the
 *  @code{DEFINE} macros at the end of the template.  That puts the main
 *  body of the template at the beginning of the file.
=*/
/*=macfunc ENDDEF
 *
 *  what:   Ends a macro definition.
 *  in-context:
 *
 *  desc:
 *    This macro ends the @code{DEFINE} function template block.
 *    For a complete description @xref{DEFINE}.
=*/
/*
 *  mFunc_Define
 *
 *  This routine runs the invocation.
 */
tMacro*
mFunc_Define(tTemplate* pT, tMacro* pMac)
{
    tDefList*   pList  = (tDefList*)pMac->res;
    int         defCt  = pMac->sibIndex;
    tDefCtx     ctx;

    pT = (tTemplate*)pMac->funcPrivate;

    if (OPT_VALUE_TRACE > TRACE_NOTHING) {
        fprintf(pfTrace, TPL_INVOKED, pT->pzTplName, defCt);
        if (OPT_VALUE_TRACE == TRACE_EVERYTHING)
            fprintf(pfTrace, TAB_FILE_LINE_FMT,
                    pCurTemplate->pzTplFile, pMac->lineNo);
    }

    /*
     *  IF we have no special definitions, then do not nest definitions
     */
    if (defCt != 0) {
        ctx  = currDefCtx;
        currDefCtx.pPrev = &ctx;
        build_defs(defCt, pList);
    }

    {
        tTemplate*  pOldTpl = pCurTemplate;
        pCurTemplate = pT;

        generateBlock(pT, pT->aMacros, pT->aMacros + pT->macroCt);
        pCurTemplate = pOldTpl;
    }

    if (defCt != 0)
        currDefCtx = ctx;

    if ((defCt = pMac->sibIndex) > 0) {
        pList = (tDefList*)pMac->res;
        while (defCt-- > 0) {
            if (pList->pzExpr != NULL) {
                AGFREE((void*)pList->de.val.pzText);
                pList->de.val.pzText = NULL;
            }
            pList++;
        }
    }

    return pMac+1;
}


void
mUnload_Define(tMacro* pMac)
{
    void* p = (void*)(pMac->res);
    if (p != NULL)
        AGFREE(p);
}


/*=macfunc INVOKE
 *
 *  handler_proc:
 *  what:  Invoke a User Defined Macro
 *
 *  desc:
 *
 *  User defined macros may be invoked explicitly or implicitly.
 *  If you invoke one implicitly, the macro must begin with the
 *  name of the defined macro.  Consequently, this may @strong{not}
 *  be a computed value.  If you explicitly invoke a user defined macro,
 *  the macro begins with the macro name @code{INVOKE} followed by
 *  a @i{basic expression} that must yield a known user defined macro.
 *  A macro name _must_ be found, or AutoGen will issue a diagnostic
 *  and exit.
 *
 *  Arguments are passed to the invoked macro by name.
 *  The text following the macro name must consist of a series of
 *  names each of which is followed by an equal sign (@code{=}) and
 *  a @i{basic expression} that yields a string.
 *
 *  The string values may contain template macros that are parsed
 *  the first time the macro is processed and evaluated again every
 *  time the macro is evaluated.
=*/
tMacro *
mFunc_Invoke(tTemplate * pT, tMacro * pMac)
{
    char* pzText;
    SCM   macName;
    tTemplate* pInv;

    /*
     *  IF this is the first time through,
     *  THEN separate the name from the rest of the arguments.
     */
    if (pMac->ozName == 0) {
        prep_invoke_args(pMac);

        /*
         *  IF the name is constant and not an expression,
         *  THEN go find the template now and bind the macro call
         *       to a particular template macro
         */
        if (IS_VAR_FIRST_CHAR(pT->pzTemplText[ pMac->ozName ])) {
            pMac->funcCode    = FTYP_DEFINE;
            pMac->funcPrivate = (void*)findTemplate(
                pT->pzTemplText + pMac->ozName );

            if (pMac->funcPrivate == NULL) {
                pzText = aprf(BAD_MAC_NM_FMT, pT->pzTemplText + pMac->ozName);
                AG_ABEND_IN(pT, pMac, pzText);
                /* NOTREACHED */
            }

            return mFunc_Define(pT, pMac);
        }
    }

    /*
     *  Call `eval' to determine the macro name.  Every invocation
     *  may be different!!
     */
    macName = eval(pT->pzTemplText + pMac->ozName);

    pzText = ag_scm2zchars(macName, "macro name");
    pInv = findTemplate(pzText);
    if (pInv == NULL) {
        pzText = aprf(BAD_MAC_NM_FMT, pzText);
        AG_ABEND_IN(pT, pMac, pzText);
        /* NOTREACHED */
    }

    pMac->funcPrivate = (void*)pInv;

    return mFunc_Define(pT, pMac);
}

/**
 * Loads the debug function for load time breakpoints.
 * @param pT     containing template
 * @param pMac   the debug macro data
 * @param ppzSan pointer to scanning pointer
 * @returns      the next open macro slot
 */
tMacro *
mLoad_Debug(tTemplate * pT, tMacro * pMac, char const ** ppzScan)
{
    if (OPT_VALUE_TRACE >= TRACE_DEBUG_MESSAGE)
        return mLoad_Unknown(pT, pMac, ppzScan);
    return mLoad_Comment(pT, pMac, ppzScan);
}

tMacro *
mLoad_Define(tTemplate * pT, tMacro * pMac, char const ** ppzScan)
{
    char *       pzCopy;             /* next text dest   */
    tTemplate *  pNewT;

    /*
     *  Save the global macro loading mode
     */
    tpLoadProc const * papLP = papLoadProc;
    static tpLoadProc apDefineLoad[ FUNC_CT ] = { NULL };

    if (pMac->ozText == 0)
        AG_ABEND_IN(pT, pMac, LD_DEF_NEED_NAME);

    /*
     *  IF this is the first time here,
     *  THEN set up the "DEFINE" block callout table.
     *  It is the standard table, except entries are inserted
     *  for functions that are enabled only while processing
     *  a DEFINE block (viz. "ENDDEF" and removing "DEFINE").
     */
    if (apDefineLoad[0] == NULL) {
        memcpy((void*)apDefineLoad, apLoadProc, sizeof(apLoadProc));
        apDefineLoad[ FTYP_ENDDEF ] = &mLoad_Ending;
        apDefineLoad[ FTYP_DEFINE ] = &mLoad_Bogus;
    }
    papLoadProc = apDefineLoad;

    {
        char const*    pzScan = *ppzScan;  /* text after macro */
        char const*    pzSrc  = (char const*)pMac->ozText; /* macro text */
        int            macCt  = pT->macroCt - (pMac - pT->aMacros);

        size_t alocSize = sizeof(*pNewT)
            + (macCt * sizeof(tMacro)) + strlen(pzScan) + 0x100;
        alocSize &= ~0x0F;

        /*
         *  Allocate a new template block that is much larger than needed.
         */
        pNewT = (tTemplate*)AGALOC(alocSize, "AG macro def");
        memset((void*)pNewT, 0, alocSize);
        pNewT->magic      = pT->magic;
        pNewT->descSize   = alocSize;
        pNewT->macroCt    = macCt;
        pNewT->pzTplFile  = strdup(pT->pzTplFile);

        pzCopy = pNewT->pzTplName = (void*)(pNewT->aMacros + macCt);
        if (! IS_VAR_FIRST_CHAR(*pzSrc))
            AG_ABEND_IN(pT, pMac, LD_DEF_NEED_NAME);

        while (IS_VALUE_NAME_CHAR(*pzSrc))
            *(pzCopy++) = *(pzSrc++);
    }

    if (OPT_VALUE_TRACE >= TRACE_BLOCK_MACROS)
        fprintf(pfTrace, TRACE_MACRO_DEF,
                pNewT->pzTplName, pNewT->pzTplFile);

    *(pzCopy++) = NUL;
    pNewT->pzTemplText = pzCopy;
    pNewT->pNext = pzCopy+1;
    strcpy(pNewT->zStartMac, pT->zStartMac);
    strcpy(pNewT->zEndMac, pT->zEndMac);
    pCurTemplate = pNewT;

    {
        tMacro* pMacEnd = parseTemplate(pNewT->aMacros, ppzScan);
        int     ct;

        /*
         *  Make sure all of the input string was *NOT* scanned.
         */
        if (*ppzScan == NULL)
            AG_ABEND_IN(pNewT, pNewT->aMacros, LD_DEF_WOOPS);

        ct = pMacEnd - pNewT->aMacros;

        /*
         *  IF there are empty macro slots,
         *  THEN pack the text
         */
        if (ct < pNewT->macroCt) {
            int   delta = sizeof(tMacro) * (pNewT->macroCt - ct);
            void* data  = (pNewT->pzTplName == NULL) ?
                pNewT->pzTemplText : pNewT->pzTplName;
            size_t size = pNewT->pNext - (char*)data;
            memmove((void*)pMacEnd, data, size);

            pNewT->pzTemplText -= delta;
            pNewT->pNext       -= delta;
            pNewT->pzTplName   -= delta;
            pNewT->macroCt      = ct;
        }
    }

    /*
     *  Adjust the sizes.  Remove absolute pointers.  Reallocate to the correct
     *  size.  Restore the offsets to pointer values.
     */
    {
        size_t sz = pNewT->pNext - (char*)pNewT;
        if (sz < pNewT->descSize) {
            pNewT->descSize     = sz;
            pNewT->pzTplName   -= (long)pNewT;
            pNewT->pzTemplText -= (long)pNewT;
            pNewT = AGREALOC((void*)pNewT, pNewT->descSize, "resize AG mac");
            pNewT->pzTplName   += (long)pNewT;
            pNewT->pzTemplText += (long)pNewT;
        }
    }

#if defined(DEBUG_ENABLED)
    if (HAVE_OPT(SHOW_DEFS)) {
        static char const zSum[] = "loaded %d macros from %s\n"
            "\tBinary template size:  0x%X\n\n";
        fprintf(pfTrace, zSum, pNewT->macroCt, pNewT->pzTplFile,
                (unsigned int)pNewT->descSize);
    }
#endif

    pNewT->pNext  = (char*)pNamedTplList;
    pNamedTplList = pNewT;
    papLoadProc   = papLP;
    memset((void*)pMac, 0, sizeof(*pMac));
    pCurTemplate  = pT;
    return pMac;
}
/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * indent-tabs-mode: nil
 * End:
 * end of agen5/funcDef.c */
