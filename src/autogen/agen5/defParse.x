
/**
 * @file defParse.x
 *
 * Definition parser functions.
 *
 *  Time-stamp:        "2012-01-07 10:50:52 bkorb"
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

#ifdef FSM_USER_HEADERS
static char* pz_new_name = NULL;
#endif /* FSM_USER_HEADERS */

#ifdef FSM_FIND_TRANSITION
trans_evt = yylex();
#endif /* FSM_FIND_TRANSITION */

#ifdef FSM_HANDLER_CODE
/*
 *  Print out an invalid transition message and return EXIT_FAILURE
 */
static int
dp_invalid_transition(te_dp_state st, te_dp_event evt)
{
    char const * fmt_pz = zDpStrings + DpFsmErr_off;
    fprintf(stderr, fmt_pz, st, DP_STATE_NAME(st), evt, DP_EVT_NAME(evt));

    return EXIT_FAILURE;
}

static te_dp_state
dp_do_empty_val(
    te_dp_state initial,
    te_dp_state maybe_next,
    te_dp_event trans_evt )
{
    /*
     *  Our state is either "have-name" or "indx-name" and we found a ';',
     *  end of statement.  It is a string value with an empty string.
     */
    tDefEntry * pDE = findPlace(pz_new_name, NULL);

    pDE->val.pzText = (char *)zNil;
    pDE->valType    = VALTYP_TEXT;
    if (OPT_VALUE_TRACE >= TRACE_EXPRESSIONS)
        print_def(pDE);
    return maybe_next;
}

static te_dp_state
dp_do_end_block(
    te_dp_state initial,
    te_dp_state maybe_next,
    te_dp_event trans_evt )
{
    if (stackDepth <= 0)
        yyerror((void*)"Too many close braces");

    pCurrentEntry = ppParseStack[ stackDepth-- ];
    return maybe_next;
}

static te_dp_state
dp_do_have_name_lit_eq(
    te_dp_state initial,
    te_dp_state maybe_next,
    te_dp_event trans_evt )
{
    /*
     *  Create a new entry but defer "makeMacro" call until we have the
     *  assigned value.
     */
    findPlace(pz_new_name, NULL);
    return maybe_next;
}

static te_dp_state
dp_do_indexed_name(
    te_dp_state initial,
    te_dp_state maybe_next,
    te_dp_event trans_evt )
{
    /*
     *  Create a new entry with a specified indes, but defer "makeMacro" call
     *  until we have the assigned value.
     */
    findPlace(pz_new_name, pz_token);
    return maybe_next;
}

static te_dp_state
dp_do_invalid(
    te_dp_state initial,
    te_dp_state maybe_next,
    te_dp_event trans_evt )
{
    dp_invalid_transition(initial, trans_evt);
    yyerror((void*)"invalid transition");
    /* NOTREACHED */
    return DP_ST_INVALID;
}

static te_dp_state
dp_do_need_name_end(
    te_dp_state initial,
    te_dp_state maybe_next,
    te_dp_event trans_evt )
{
    if (stackDepth != 0)
        yyerror((void*)"definition blocks were left open");

    /*
     *  We won't be using the parse stack any more.
     *  We only process definitions once.
     */
    if (ppParseStack != parseStack)
        AGFREE(ppParseStack);

    /*
     *  The seed has now done its job.  The real root of the
     *  definitions is the first entry off of the seed.
     */
    rootDefCtx.pDefs = rootDefCtx.pDefs->val.pDefEntry;
    return maybe_next;
}

static te_dp_state
dp_do_need_name_var_name(
    te_dp_state initial,
    te_dp_state maybe_next,
    te_dp_event trans_evt )
{
    pz_new_name = pz_token;
    return maybe_next;
}

static te_dp_state
dp_do_next_val(
    te_dp_state initial,
    te_dp_state maybe_next,
    te_dp_event trans_evt )
{
    /*
     *  Clone the entry name of the current entry.
     */
    findPlace(pCurrentEntry->pzDefName, NULL);
    return maybe_next;
}

static te_dp_state
dp_do_start_block(
    te_dp_state initial,
    te_dp_state maybe_next,
    te_dp_event trans_evt )
{
    if (pCurrentEntry->valType == VALTYP_TEXT)
        yyerror((void*)"assigning a block value to text name");
    pCurrentEntry->valType = VALTYP_BLOCK; /* in case not yet determined */

    if (OPT_VALUE_TRACE >= TRACE_EXPRESSIONS)
        print_def(pCurrentEntry);

    if (++stackDepth >= stackSize) {
        tDefEntry** ppDE;
        stackSize += stackSize / 2;

        if (ppParseStack == parseStack) {
            ppDE = AGALOC(stackSize * sizeof(void*), "def stack");
            memcpy(ppDE, parseStack, sizeof(parseStack));
        } else {
            ppDE = AGREALOC(ppParseStack, stackSize * sizeof(void*),
                            "stretch def stack");
        }
        ppParseStack = ppDE;
    }
    ppParseStack[ stackDepth ] = pCurrentEntry;
    pCurrentEntry = NULL;
    return maybe_next;
}

static te_dp_state
dp_do_str_value(
    te_dp_state initial,
    te_dp_state maybe_next,
    te_dp_event trans_evt )
{
    if (pCurrentEntry->valType == VALTYP_BLOCK)
        yyerror((void*)"assigning a block value to text name");

    pCurrentEntry->val.pzText = pz_token;
    pCurrentEntry->valType = VALTYP_TEXT;

    if (OPT_VALUE_TRACE >= TRACE_EXPRESSIONS)
        print_def(pCurrentEntry);

    /*
     *  The "here string" marker is the line before where the text starts.
     */
    if (trans_evt == DP_EV_HERE_STRING)
        pCurrentEntry->srcLineNum++;
    return maybe_next;
}

static te_dp_state
dp_do_tpl_name(
    te_dp_state initial,
    te_dp_state maybe_next,
    te_dp_event trans_evt )
{
    /*
     *  Allow this routine to be called multiple times.
     *  This may happen if we include another definition file.
     */
    if (rootDefCtx.pDefs == NULL) {
        static char const zBogus[] = "@BOGUS@";
        static tDefEntry  seed = {
            NULL, NULL, NULL, NULL, (char*)zBogus, 0, { NULL },
            (char*)zBogus, 0, VALTYP_BLOCK };

        rootDefCtx.pDefs = &seed;

        if (! HAVE_OPT(OVERRIDE_TPL))
             pzTemplFileName = pz_token;

        stackDepth = 0;
        ppParseStack[0] = &seed;
    }
    return maybe_next;
}
#endif /* FSM_HANDLER_CODE */

/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * indent-tabs-mode: nil
 * End:
 * end of agen5/defParse.x */
