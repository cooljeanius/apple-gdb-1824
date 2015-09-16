
/**
 * @file tpProcess.c
 *
 *  Parse and process the template data descriptions
 *
 * Time-stamp:        "2012-01-14 10:55:04 bkorb"
 *
 * This file is part of AutoGen.
 * AutoGen Copyright (c) 1992-2012 by Bruce Korb - all rights reserved
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

static tFpStack fpRoot = { 0, NULL, NULL, NULL };

/* = = = START-STATIC-FORWARD = = = */
static void
trace_macro(tTemplate * pT, tMacro * pMac);

static void
do_stdout_tpl(tTemplate * pTF);

static void
open_output(tOutSpec * spec);
/* = = = END-STATIC-FORWARD = = = */

/**
 *  Generate all the text within a block.  The caller must
 *  know the exact bounds of the block.  "pEnd" actually
 *  must point to the first entry that is *not* to be emitted.
 */
LOCAL void
generateBlock(tTemplate * pT, tMacro * pMac, tMacro * pEnd)
{
    /*
     *  Set up the processing context for this block of macros.
     *  It is used by the Guile callback routines and the exception
     *  handling code.  It is all for user friendly diagnostics.
     */
    pCurTemplate = pT;

    while ((pMac != NULL) && (pMac < pEnd)) {
        teFuncType fc = pMac->funcCode;
        if (fc >= FUNC_CT)
            fc = FTYP_BOGUS;

        if (OPT_VALUE_TRACE >= TRACE_EVERYTHING)
            trace_macro(pT, pMac);

        pCurMacro = pMac;
        pMac = (*(apHdlrProc[ fc ]))(pT, pMac);
        ag_scribble_free();
    }
}

/**
 *  Print out information about the invocation of a macro
 */
static void
trace_macro(tTemplate * pT, tMacro * pMac)
{
    teFuncType fc = pMac->funcCode;
    if (fc >= FUNC_CT)
        fc = FTYP_BOGUS;

    fprintf(pfTrace, TRACE_MACRO_FMT, apzFuncNames[fc], pMac->funcCode,
            pT->pzTplFile, pMac->lineNo);

    if (pMac->ozText > 0) {
        int   ct;
        char* pz = pT->pzTemplText + pMac->ozText;
        fputs("  ", pfTrace);
        for (ct=0; ct < 32; ct++) {
            char ch = *(pz++);
            if (ch == NUL)
                break;
            if (ch == NL)
                break;
            putc(ch, pfTrace);
        }
        putc(NL, pfTrace);
    }
}

/**
 *  The template output goes to stdout.  Perhaps because output
 *  is for a CGI script.  In any case, this case must be handled
 *  specially.
 */
static void
do_stdout_tpl(tTemplate * pTF)
{
    char   const *    pzRes;
    SCM    res;

    pzLastScheme = NULL; /* We cannot be in Scheme processing */

    switch (setjmp (fileAbort)) {
    case SUCCESS:
        break;

    case PROBLEM:
        if (*pzOopsPrefix != NUL) {
            fprintf(stdout, DO_STDOUT_TPL_ABANDONED, pzOopsPrefix);
            pzOopsPrefix = zNil;
        }
        fclose(stdout);
        return;

    default:
        fprintf(stdout, DO_STDOUT_TPL_BADR, pzOopsPrefix);

    case FAILURE:
        exit(EXIT_FAILURE);
    }

    pzCurSfx      = DO_STDOUT_TPL_NOSFX;
    currDefCtx    = rootDefCtx;
    pCurFp        = &fpRoot;
    fpRoot.pFile  = stdout;
    fpRoot.pzOutName = DO_STDOUT_TPL_STDOUT;
    fpRoot.flags  = FPF_NOUNLINK | FPF_STATIC_NM;
    if (OPT_VALUE_TRACE >= TRACE_EVERYTHING)
        fputs(DO_STDOUT_TPL_START_STD, pfTrace);

    /*
     *  IF there is a CGI prefix for error messages,
     *  THEN divert all output to a temporary file so that
     *  the output will be clean for any error messages we have to emit.
     */
    if (*pzOopsPrefix == NUL)
        generateBlock(pTF, pTF->aMacros, pTF->aMacros + pTF->macroCt);

    else {
        (void)ag_scm_out_push_new(SCM_UNDEFINED);

        generateBlock(pTF, pTF->aMacros, pTF->aMacros + pTF->macroCt);

        /*
         *  Read back in the spooled output.  Make sure it starts with
         *  a content-type: prefix.  If not, we supply our own HTML prefix.
         */
        res   = ag_scm_out_pop(SCM_BOOL_T);
        pzRes = AG_SCM_CHARS(res);

        /* 13:  "content-type:" */
        if (strneqvcmp(pzRes, DO_STDOUT_TPL_CONTENT, 13) != 0)
            fputs(DO_STDOUT_TPL_CONTENT, stdout);

        fwrite(pzRes, AG_SCM_STRLEN(res), (size_t)1, stdout);
    }

    fclose(stdout);
}

/**
 * pop the current output spec structure.  Deallocate it and the
 * file name, too, if necessary.
 */
LOCAL tOutSpec *
nextOutSpec(tOutSpec * pOS)
{
    tOutSpec * res = pOS->pNext;

    if (pOS->deallocFmt)
        AGFREE(pOS->pzFileFmt);

    AGFREE(pOS);
    return res;
}


LOCAL void
processTemplate(tTemplate* pTF)
{
    forInfo.fi_depth = 0;

    /*
     *  IF the template file does not specify any output suffixes,
     *  THEN we will generate to standard out with the suffix set to zNoSfx.
     *  With output going to stdout, we don't try to remove output on errors.
     */
    if (pOutSpecList == NULL) {
        do_stdout_tpl(pTF);
        return;
    }

    do  {
        tOutSpec*  pOS    = pOutSpecList;

        /*
         * We cannot be in Scheme processing.  We've either just started
         * or we've made a long jump from our own code.  If we've made a
         * long jump, we've printed a message that is sufficient and we
         * don't need to print any scheme expressions.
         */
        pzLastScheme = NULL;

        /*
         *  HOW was that we got here?
         */
        switch (setjmp(fileAbort)) {
        case SUCCESS:
            if (OPT_VALUE_TRACE >= TRACE_EVERYTHING) {
                fprintf(pfTrace, PROC_TPL_START, pOS->zSuffix);
                fflush(pfTrace);
            }
            /*
             *  Set the output file name buffer.
             *  It may get switched inside open_output.
             */
            open_output(pOS);
            memcpy(&fpRoot, pCurFp, sizeof(fpRoot));
            AGFREE(pCurFp);
            pCurFp         = &fpRoot;
            pzCurSfx       = pOS->zSuffix;
            currDefCtx     = rootDefCtx;
            pCurFp->flags &= ~FPF_FREE;
            pCurFp->pPrev  = NULL;
            generateBlock(pTF, pTF->aMacros, pTF->aMacros + pTF->macroCt);

            do  {
                out_close(AG_FALSE);  /* keep output */
            } while (pCurFp->pPrev != NULL);
            break;

        case PROBLEM:
            /*
             *  We got here by a long jump.  Close/purge the open files.
             */
            do  {
                out_close(AG_TRUE);  /* discard output */
            } while (pCurFp->pPrev != NULL);
            pzLastScheme = NULL; /* "problem" means "drop current output". */
            break;

        default:
            fprintf(pfTrace, PROC_TPL_BOGUS_RET, pzOopsPrefix);
            pzOopsPrefix = zNil;
            /* FALLTHROUGH */

        case FAILURE:
            /*
             *  We got here by a long jump.  Close/purge the open files.
             */
            do  {
                out_close(AG_TRUE);  /* discard output */
            } while (pCurFp->pPrev != NULL);

            /*
             *  On failure (or unknown jump type), we quit the program, too.
             */
            procState = PROC_STATE_ABORTING;
            do pOS = nextOutSpec(pOS);
            while (pOS != NULL);
            exit(EXIT_FAILURE);
        }

        pOutSpecList = nextOutSpec(pOS);
    } while (pOutSpecList != NULL);
}


LOCAL void
out_close(ag_bool purge)
{
    if ((pCurFp->flags & FPF_NOCHMOD) == 0)
        make_readonly(fileno(pCurFp->pFile));

    if (OPT_VALUE_TRACE > TRACE_DEBUG_MESSAGE)
        fprintf(pfTrace, OUT_CLOSE_TRACE_WRAP, __func__, pCurFp->pzOutName);

    fclose(pCurFp->pFile);

    /*
     *  Only stdout and /dev/null are marked, "NOUNLINK"
     */
    if ((pCurFp->flags & FPF_NOUNLINK) == 0) {
        /*
         *  IF we are told to purge the file OR the file is an AutoGen temp
         *  file, then get rid of the output.
         */
        if (purge || ((pCurFp->flags & FPF_UNLINK) != 0))
            unlink(pCurFp->pzOutName);

        else {
            struct utimbuf tbuf;

            tbuf.actime  = time(NULL);
            tbuf.modtime = outTime;

            /*
             *  The putative start time is one second earlier than the
             *  earliest output file time, regardless of when that is.
             */
            if (outTime <= startTime)
                startTime = outTime - 1;

            utime(pCurFp->pzOutName, &tbuf);
        }
    }

    /*
     *  Do not deallocate statically allocated names
     */
    if ((pCurFp->flags & FPF_STATIC_NM) == 0)
        AGFREE((void*)pCurFp->pzOutName);

    /*
     *  Do not deallocate the root entry.  It is not allocated!!
     */
    if ((pCurFp->flags & FPF_FREE) != 0) {
        tFpStack* p = pCurFp;
        pCurFp = p->pPrev;
        AGFREE((void*)p);
    }
}


/**
 *  Figure out what to use as the base name of the output file.
 *  If an argument is not provided, we use the base name of
 *  the definitions file.
 */
static void
open_output(tOutSpec * spec)
{
    static char const write_mode[] = "w" FOPEN_BINARY_FLAG "+";

    char const * out_file = NULL;

    if (strcmp(spec->zSuffix, OPEN_OUTPUT_NULL) == 0) {
        static int const flags = FPF_NOUNLINK | FPF_NOCHMOD | FPF_TEMPFILE;
    null_open:
        open_output_file(DEV_NULL, DEV_NULL_LEN, write_mode, flags);
        return;
    }

    /*
     *  IF we are to skip the current suffix,
     *  we will redirect the output to /dev/null and
     *  perform all the work.  There may be side effects.
     */
    if (HAVE_OPT(SKIP_SUFFIX)) {
        int     ct  = STACKCT_OPT(SKIP_SUFFIX);
        const char ** ppz = STACKLST_OPT(SKIP_SUFFIX);

        while (--ct >= 0) {
            if (strcmp(spec->zSuffix, *ppz++) == 0)
                goto null_open;
        }
    }

    /*
     *  Remove any suffixes in the last file name
     */
    {
        char const * def_file = OPT_ARG(BASE_NAME);
        char   z[AG_PATH_MAX];
        const char * pst = strrchr(def_file, '/');
        char * end;

        pst = (pst == NULL) ? def_file : (pst + 1);

        /*
         *  We allow users to specify a suffix with '-' and '_', but when
         *  stripping a suffix from the "base name", we do not recognize 'em.
         */
        end = strchr(pst, '.');
        if (end != NULL) {
            size_t len = (unsigned)(end - pst);
            if (len >= sizeof(z))
                AG_ABEND("--base-name name is too long");

            memcpy(z, pst, len);
            z[ end - pst ] = NUL;
            pst = z;
        }

        /*
         *  Now formulate the output file name in the buffer
         *  provided as the input argument.
         */
        out_file = aprf(spec->pzFileFmt, pst, spec->zSuffix);
        if (out_file == NULL)
            AG_ABEND(aprf(OPEN_OUTPUT_BAD_FMT, spec->pzFileFmt, pst,
                          spec->zSuffix));
    }

    open_output_file(out_file, strlen(out_file), write_mode, 0);
    free((void *)out_file);
}
/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * indent-tabs-mode: nil
 * End:
 * end of agen5/tpProcess.c */
