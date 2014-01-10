/*
 *  EDIT THIS FILE WITH CAUTION  (collapse.c)
 *
 *  It has been AutoGen-ed  March  8, 2013 at 03:16:23 PM by AutoGen 5.17.3.pre6
 *  From the definitions    collapse.def
 *  and the template file   fsm
 *
 *  Automated Finite State Machine
 *
 *  Copyright (C) 1992-2013 Bruce Korb - all rights reserved
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name ``Bruce Korb'' nor the name of any other
 *    contributor may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * AutoFSM IS PROVIDED BY Bruce Korb ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL Bruce Korb OR ANY OTHER CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#define DEFINE_FSM
#include "collapse-fsm.h"
#include <stdio.h>

/*
 *  Do not make changes to this file, except between the START/END
 *  comments, or it will be removed the next time it is generated.
 */
/* START === USER HEADERS === DO NOT CHANGE THIS COMMENT */
#include <stdlib.h>
#include <string.h>
#include "cright-cmap.h"
#include "opts.h"

extern te_cyr_event get_next_token(char ** plist, unsigned long * val);

static char * tbuf;
static char * tscn;
static size_t list_len;
static char * save_list = NULL;

static unsigned long tkn_val;
/* END   === USER HEADERS === DO NOT CHANGE THIS COMMENT */

/* * * * * * * * * THE CODE STARTS HERE * * * * * * * */
/**
 *  Print out an invalid transition message and return EXIT_FAILURE
 */
static int
cyr_invalid_transition( te_cyr_state st, te_cyr_event evt )
{
    /* START == INVALID TRANS MSG == DO NOT CHANGE THIS COMMENT */
    char const * fmt = zCyrStrings + CyrFsmErr_off;
    fprintf( stderr, fmt, st, CYR_STATE_NAME(st), evt, CYR_EVT_NAME(evt));
    /* END   == INVALID TRANS MSG == DO NOT CHANGE THIS COMMENT */

    return EXIT_FAILURE;
}

/**
 *  Run the FSM.  Will return CYR_ST_DONE or CYR_ST_INVALID
 */
te_cyr_state
cyr_run_fsm(
    char * list )
{
    te_cyr_state cyr_state = CYR_ST_INIT;
    te_cyr_event trans_evt;
    te_cyr_state nxtSt;
    te_cyr_trans trans;
    char * saved_list = list;
    (void)saved_list;

    while (cyr_state < CYR_ST_INVALID) {

        /* START == FIND TRANSITION == DO NOT CHANGE THIS COMMENT */
        if (cyr_state == CYR_ST_INIT) {
            first_yr  = 0;
            save_list = list;
        }
        trans_evt = get_next_token(&list, &tkn_val);
        /* END   == FIND TRANSITION == DO NOT CHANGE THIS COMMENT */

#ifndef __COVERITY__
        if (trans_evt >= CYR_EV_INVALID) {
            nxtSt = CYR_ST_INVALID;
            trans = CYR_TR_INVALID;
        } else
#endif /* __COVERITY__ */
        {
            const t_cyr_transition* pTT =
            cyr_trans_table[ cyr_state ] + trans_evt;
            nxtSt = pTT->next_state;
            trans = pTT->transition;
        }


        switch (trans) {
        case CYR_TR_ADD_COMMA:
            /* START == ADD_COMMA == DO NOT CHANGE THIS COMMENT */
            *(tscn++) = ',';
            *(tscn++) = ' ';
            /* END   == ADD_COMMA == DO NOT CHANGE THIS COMMENT */
            break;


        case CYR_TR_ADD_YEAR:
            /* START == ADD_YEAR == DO NOT CHANGE THIS COMMENT */
            tscn += sprintf(tscn, "%4.4lu", tkn_val);
            /* END   == ADD_YEAR == DO NOT CHANGE THIS COMMENT */
            break;


        case CYR_TR_FINISH:
            /* START == FINISH == DO NOT CHANGE THIS COMMENT */
            tscn = SPN_WHITESPACE_BACK(tbuf, tscn);

            if ((tscn - tbuf) > list_len) {
                fprintf(stderr, "year list too long: %u > %u\n",
                        (tscn - tbuf), list_len);
            invalid_ending:
                nxtSt = CYR_ST_INVALID;

            } else {
                memcpy(save_list, tbuf, tscn - tbuf);
                save_list[tscn - tbuf] = NUL;
            }
            free(tbuf);
            /* END   == FINISH == DO NOT CHANGE THIS COMMENT */
            break;


        case CYR_TR_HYPHENATE:
            /* START == HYPHENATE == DO NOT CHANGE THIS COMMENT */
            if (tscn[-1] != '-')
                *(tscn++) = '-';
            /* END   == HYPHENATE == DO NOT CHANGE THIS COMMENT */
            break;


        case CYR_TR_INITIALIZE:
            /* START == INITIALIZE == DO NOT CHANGE THIS COMMENT */
            list_len = strlen(list) + 4;
            tbuf     = tscn = malloc(list_len + 8);
            tscn    += sprintf(tscn, "%4.4lu", tkn_val);
            /* END   == INITIALIZE == DO NOT CHANGE THIS COMMENT */
            break;


        case CYR_TR_INVALID:
            /* START == INVALID == DO NOT CHANGE THIS COMMENT */
            (void)cyr_invalid_transition(cyr_state, trans_evt);
            fprintf(stderr, "invalid year specification: '%s'\n", save_list);
            goto invalid_ending;
            /* END   == INVALID == DO NOT CHANGE THIS COMMENT */
            break;


        case CYR_TR_REM_YEAR:
            /* START == REM_YEAR == DO NOT CHANGE THIS COMMENT */
            tscn -= 4;
            /* END   == REM_YEAR == DO NOT CHANGE THIS COMMENT */
            break;


        default:
            /* START == BROKEN MACHINE == DO NOT CHANGE THIS COMMENT */
            fprintf(stderr, "year parsing FSM is broken\n");
            goto invalid_ending;
            /* END   == BROKEN MACHINE == DO NOT CHANGE THIS COMMENT */
        }

        cyr_state = nxtSt;
    }
    return cyr_state;
}
/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * indent-tabs-mode: nil
 * End:
 * end of collapse-fsm.c */
