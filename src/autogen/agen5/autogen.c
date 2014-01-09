
/**
 *  @file autogen.c
 *
 *  Time-stamp:        "2012-01-07 10:40:56 bkorb"
 *
 *  This is the main routine for autogen.
 *
 *  This file is part of AutoGen.
 *  Copyright (c) 1992-2012 Bruce Korb - all rights reserved
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
#ifdef HAVE_SYS_RESOURCE_H
#include <sys/resource.h>
#endif

typedef void (void_main_proc_t)(int, char **);

typedef enum {
    EXIT_PCLOSE_WAIT,
    EXIT_PCLOSE_NOWAIT
} wait_for_pclose_enum_t;

#define _State_(n)  #n,
static char const * const state_names[] = { STATE_TABLE };
#undef _State_

static sigjmp_buf  abendJumpEnv;
static int         abendJumpSignal = 0;

typedef void (sighandler_proc_t)(int sig);
static sighandler_proc_t ignore_signal, catch_sig_and_bail;

/* = = = START-STATIC-FORWARD = = = */
static void
inner_main(void * closure, int argc, char ** argv);

static void
exit_cleanup(wait_for_pclose_enum_t cl_wait);

static void
cleanup_and_abort(int sig);

static void
catch_sig_and_bail(int sig);

static void
ignore_signal(int sig);

static void
done_check(void);

static void
setup_signals(sighandler_proc_t * chldHandler,
              sighandler_proc_t * abrtHandler,
              sighandler_proc_t * dfltHandler);
/* = = = END-STATIC-FORWARD = = = */

/**
 * main routine under Guile guidance
 * @param  closure Guile closure parameter.  Not used.
 * @param  argc    argument count
 * @param  argv    argument vector
 */
static void
inner_main(void * closure, int argc, char ** argv)
{
    atexit(done_check);
    initialize(argc, argv);

    procState = PROC_STATE_LOAD_DEFS;
    readDefines();

    /*
     *  Activate the AutoGen specific Scheme functions.
     *  Then load, process and unload the main template
     */
    procState = PROC_STATE_LOAD_TPL;

    {
        tTemplate* pTF = loadTemplate(pzTemplFileName, NULL);

        procState = PROC_STATE_EMITTING;
        processTemplate(pTF);

        procState = PROC_STATE_CLEANUP;
        cleanup(pTF);
    }

    procState = PROC_STATE_DONE;
    setup_signals(SIG_DFL, SIG_IGN, SIG_DFL);
    exit_code = AUTOGEN_EXIT_SUCCESS;
    done_check();
    /* NOTREACHED */
}

/**
 * main() called from _start()
 * @param  argc    argument count
 * @param  argv    argument vector
 * @return nothing -- Guile never returns, but calls exit(2).
 */
int
main(int argc, char ** argv)
{
    /*
     *  IF we've been kicked with a signal,
     *  THEN abort, passing the signal that whacked us.
     */
    if (sigsetjmp(abendJumpEnv, 0) != 0)
        cleanup_and_abort(abendJumpSignal);

    setup_signals(ignore_signal, SIG_DFL, catch_sig_and_bail);
    optionSaveState(&autogenOptions);
    pfTrace = stderr;

    /*
     *  as of 2.0.2, Guile will fiddle with strings all on its own accord.
     *  Coerce the environment into being POSIX ASCII strings so it keeps
     *  its bloody stinking nose out of our data.
     */
    putenv((char*)(void*)LC_ALL_IS_C);

    /*
     *  If GUILE_WARN_DEPRECATED has not been defined, then likely we are
     *  not in a development environment and likely we don't want to give
     *  our users any angst.
     */
    if (getenv(GUILE_WARN_DEP_STR) == NULL)
        putenv((char*)(void*)GUILE_WARN_NO_ENV);

    AG_SCM_BOOT_GUILE(argc, argv, inner_main);

    /* NOT REACHED */
    return EXIT_FAILURE;
}

/**
 *  This code must run regardless of which exit path is taken
 */
static void
exit_cleanup(wait_for_pclose_enum_t cl_wait)
{
    /*
     *  There are contexts wherein this function can get run twice.
     */
    {
        static int exit_cleanup_done = 0;
        if (exit_cleanup_done) {
            if (OPT_VALUE_TRACE > TRACE_NOTHING)
                fprintf(pfTrace, EXIT_CLEANUP_MULLIGAN);
            return;
        }

        exit_cleanup_done = 1;
    }

#ifdef SHELL_ENABLED
    SCM_EVAL_CONST(EXIT_CLEANUP_STR);
#endif

    close_server_shell();

    if (OPT_VALUE_TRACE > TRACE_NOTHING)
        fprintf(pfTrace, EXIT_CLEANUP_DONE_FMT,
                (cl_wait == EXIT_PCLOSE_WAIT)
                ? EXIT_CLEANUP_WAITED : EXIT_CLEANUP_NOWAIT,
                (unsigned int)getpid());

    do  {
        if (pfTrace == stderr)
            break;

        if (! trace_is_to_pipe) {
            fclose(pfTrace);
            break;
        }


        fflush(pfTrace);
        pclose(pfTrace);
        if (cl_wait == EXIT_PCLOSE_WAIT) {
            int status;
            while (wait(&status) > 0)  ;
        }
    } while (0);

    fflush(stdout);
    fflush(stderr);
}

/**
 *  A signal was caught, siglongjmp called and main() has called this.
 *  We do not deallocate stuff so it can be found in the core dump.
 */
static void
cleanup_and_abort(int sig)
{
    if (*pzOopsPrefix != NUL) {
        fputs(pzOopsPrefix, stderr);
        pzOopsPrefix = zNil;
    }

    fprintf(stderr, AG_SIG_ABORT_FMT, sig, strsignal(sig),
            ((unsigned)procState <= PROC_STATE_DONE)
            ? state_names[ procState ] : BOGUS_TAG);

    if (procState == PROC_STATE_ABORTING) {
        exit_cleanup(EXIT_PCLOSE_NOWAIT);
        _exit(sig + 128);
    }

    procState = PROC_STATE_ABORTING;
    setup_signals(SIG_DFL, SIG_DFL, SIG_DFL);

    /*
     *  IF there is a current template, then we should report where we are
     *  so that the template writer knows where to look for their problem.
     */
    if (pCurTemplate != NULL) {
        int line;
        teFuncType fnCd;
        char const * pzFn;
        char const * pzFl;

        if (pCurMacro == NULL) {
            pzFn = PSEUDO_MACRO_NAME_STR;
            line = 0;
            fnCd = -1;

        } else {
            teFuncType f =
                (pCurMacro->funcCode > FUNC_CT)
                    ? FTYP_SELECT : pCurMacro->funcCode;
            pzFn = apzFuncNames[ f ];
            line = pCurMacro->lineNo;
            fnCd = pCurMacro->funcCode;
        }
        pzFl = pCurTemplate->pzTplFile;
        if (pzFl == NULL)
            pzFl = NULL_FILE_NAME_STR;
        fprintf(stderr, AG_ABORT_LOC_FMT, pzFl, line, pzFn, fnCd);
    }

#ifdef HAVE_SYS_RESOURCE_H
    /*
     *  If `--core' has been specified and if we have get/set rlimit,
     *  then try to set the core limit to the "hard" maximum before aborting.
     */
    if (HAVE_OPT(CORE)) {
        struct rlimit rlim;
        getrlimit(RLIMIT_CORE, &rlim);
        rlim.rlim_cur = rlim.rlim_max;
        setrlimit(RLIMIT_CORE, &rlim);
    }
#endif

    exit_cleanup(EXIT_PCLOSE_NOWAIT);
    abort();
}


/**
 *  catch_sig_and_bail catches signals we abend on.  The "siglongjmp"
 *  goes back to the real "main()" procedure and it will call
 *  "cleanup_and_abort()", above.
 */
static void
catch_sig_and_bail(int sig)
{
    switch (procState) {
    case PROC_STATE_ABORTING:
        exit_code = AUTOGEN_EXIT_SIGNAL;

    case PROC_STATE_DONE:
        break;

    default:
        abendJumpSignal = sig;
        exit_code = AUTOGEN_EXIT_SIGNAL;
        siglongjmp(abendJumpEnv, sig);
    }
}


/**
 *  ignore_signal is the handler for SIGCHLD.  If we set it to default,
 *  it will kill us.  If we set it to ignore, it will be inherited.
 *  Therefore, always in all programs set it to call a procedure.
 *  The "wait(3)" call will do magical things, but will not override SIGIGN.
 */
static void
ignore_signal(int sig)
{
    return;
}


/**
 *  This is _always_ called for exit processing.
 *  This only returns if "exit(3C)" is called during option processing.
 *  We have no way of determining the correct exit code.
 *  (Requested version or help exits EXIT_SUCCESS.  Option failures
 *  are EXIT_FAILURE.  We cannot determine here.)
 */
static void
done_check(void)
{
    /*
     *  There are contexts wherein this function can get called twice.
     */
    {
        static int done_check_done = 0;
        if (done_check_done) {
            if (OPT_VALUE_TRACE > TRACE_NOTHING)
                fprintf(pfTrace, DONE_CHECK_REDONE);
            return;
        }
        done_check_done = 1;
    }

    switch (procState) {
    case PROC_STATE_EMITTING:
    case PROC_STATE_INCLUDING:
        /*
         *  A library (viz., Guile) procedure has called exit(3C).
         *  The AutoGen abort paths all set procState to PROC_STATE_ABORTING.
         */
        if (*pzOopsPrefix != NUL) {
            /*
             *  Emit the CGI page header for an error message.  We will rewind
             *  stderr and write the contents to stdout momentarily.
             */
            fputs(pzOopsPrefix, stdout);
            pzOopsPrefix = zNil;
        }

        if (OPT_VALUE_TRACE > TRACE_NOTHING)
            scm_backtrace();

        fprintf(stderr, SCHEME_EVAL_ERR_FMT, pCurTemplate->pzTplFile,
                pCurMacro->lineNo);

        /*
         *  We got here because someone called exit early.
         */
        do  {
#ifndef DEBUG_ENABLED
            out_close(AG_FALSE);
#else
            out_close(AG_TRUE);
#endif
        } while (pCurFp->pPrev != NULL);
        exit_code = AUTOGEN_EXIT_BAD_TEMPLATE;
        break; /* continue failure exit */

    case PROC_STATE_LOAD_DEFS:
        exit_code = AUTOGEN_EXIT_BAD_DEFINITIONS;
        /* FALLTHROUGH */

    default:
        fprintf(stderr, AG_ABEND_STATE_FMT, state_names[procState]);
        goto autogen_aborts;

    case PROC_STATE_ABORTING:
        exit_code = AUTOGEN_EXIT_BAD_TEMPLATE;

    autogen_aborts:
        if (*pzOopsPrefix != NUL) {
            /*
             *  Emit the CGI page header for an error message.  We will rewind
             *  stderr and write the contents to stdout momentarily.
             */
            fputs(pzOopsPrefix, stdout);
            pzOopsPrefix = zNil;
        }
        break; /* continue failure exit */

    case PROC_STATE_OPTIONS:
        /* Exiting in option processing state is verbose enough */
        break;

    case PROC_STATE_DONE:
        break; /* continue normal exit */
    }

    if (pzLastScheme != NULL)
        fprintf(stderr, GUILE_CMD_FAIL_FMT, pzLastScheme);

    /*
     *  IF we diverted stderr, then now is the time to copy the text to stdout.
     *  This is done for CGI mode wherein we produce an error page in case of
     *  an error, but otherwise discard stderr.
     */
    if (cgi_stderr != NULL) {
        do {
            long pos = ftell(stderr);
            char* pz;

            /*
             *  Don't bother with the overhead if there is no work to do.
             */
            if (pos <= 0)
                break;
            pz = AGALOC(pos, "stderr redir");
            rewind(stderr);
            fread( pz, (size_t)1, (size_t)pos, stderr);
            fwrite(pz, (size_t)1, (size_t)pos, stdout);
            AGFREE(pz);
        } while (0);

        unlink(cgi_stderr);
        AGFREE(cgi_stderr);
        cgi_stderr = NULL;
    }

    ag_scribble_deinit();

    if (OPT_VALUE_TRACE > TRACE_NOTHING)
        fprintf(pfTrace, DONE_CHECK_DONE);

    exit_cleanup(EXIT_PCLOSE_WAIT);

    /*
     *  When processing options, return so that the option processing exit code
     *  is used.  Otherwise, terminate execution now with the decided upon
     *  exit code.  (Always EXIT_FAILURE, unless this was called at the end
     *  of inner_main().)
     */
    if (procState != PROC_STATE_OPTIONS)
        _exit(exit_code);
}


LOCAL void
ag_abend_at(char const * pzMsg
#ifdef DEBUG_ENABLED
            , char const * pzFile, int line
#endif
    )
{
    if (*pzOopsPrefix != NUL) {
        fputs(pzOopsPrefix, stderr);
        pzOopsPrefix = zNil;
    }

#ifdef DEBUG_ENABLED
    fprintf(stderr, "Giving up in %s line %d\n", pzFile, line);
#endif

    if ((procState >= PROC_STATE_LIB_LOAD) && (pCurTemplate != NULL)) {
        int l_no = (pCurMacro == NULL) ? -1 : pCurMacro->lineNo;
        fprintf(stderr, ERROR_IN_TPL_FMT, pCurTemplate->pzTplFile, l_no);
    }
    fputs(pzMsg, stderr);
    pzMsg += strlen(pzMsg);
    if (pzMsg[-1] != NL)
        fputc(NL, stderr);

    {
        teProcState oldState = procState;
        procState = PROC_STATE_ABORTING;

        switch (oldState) {
        case PROC_STATE_EMITTING:
        case PROC_STATE_INCLUDING:
        case PROC_STATE_CLEANUP:
            longjmp(fileAbort, FAILURE);
            /* NOTREACHED */
        default:
            exit(EXIT_FAILURE);
            /* NOTREACHED */
        }
    }
}


static void
setup_signals(sighandler_proc_t * chldHandler,
              sighandler_proc_t * abrtHandler,
              sighandler_proc_t * dfltHandler)
{
    struct sigaction  sa;
    int    sigNo  = 1;
#ifdef SIGRTMIN
    const int maxSig = SIGRTMIN-1;
#else
    const int maxSig = NSIG;
#endif

    sa.sa_flags   = 0;
    sigemptyset(&sa.sa_mask);

    do  {
        switch (sigNo) {
            /*
             *  Signal handling for SIGCHLD needs to be ignored.  Do *NOT* use
             *  SIG_IGN to do this.  That gets inherited by programs that need
             *  to be able to use wait(2) calls.  At the same time, we don't
             *  want our children to become zombies.  We may run out of zombie
             *  space.  Therefore, set the handler to an empty procedure.
             *  POSIX oversight.  Allowed to be fixed for next POSIX rev, tho
             *  it is "optional" to reset SIGCHLD on exec(2).
             */
#ifndef SIGCHLD
#  define SIGCHLD SIGCLD
#endif
        case SIGCHLD:
            sa.sa_handler = chldHandler;
            break;

        case SIGABRT:
            sa.sa_handler = abrtHandler;
            break;

            /*
             *  Signals we must leave alone.
             */
        case SIGKILL:
        case SIGSTOP:

            /*
             *  Signals we choose to leave alone.
             */
#ifdef SIGTSTP
        case SIGTSTP:
#endif
            continue;

#if defined(DEBUG_ENABLED)
        case SIGBUS:
        case SIGSEGV:
            /*
             *  While debugging AutoGen, we want seg faults to happen and
             *  trigger core dumps.  Make sure this happens.
             */
            sa.sa_handler = SIG_DFL;
            break;
#endif

            /*
             *  Signals to ignore with SIG_IGN.
             */
        case 0: /* cannot happen, but the following might not be defined */
#ifdef SIGWINCH
        case SIGWINCH:
#endif
#ifdef SIGTTIN
        case SIGTTIN:  /* tty input  */
#endif
#ifdef SIGTTOU
        case SIGTTOU:  /* tty output */
#endif
            sa.sa_handler = SIG_IGN;
            break;

#ifdef DAEMON_ENABLED
# error DAEMON-ization of AutoGen is not ready for prime time
  Choke Me.
        case SIGHUP:
            if (HAVE_OPT(DAEMON)) {
                sa.sa_handler = handleSighup;
                break;
            }
            /* FALLTHROUGH */
#endif

        default:
            sa.sa_handler = dfltHandler;
        }
        sigaction(sigNo,  &sa, NULL);
    } while (++sigNo < maxSig);
}

#ifndef HAVE_STRFTIME
#  include <compat/strftime.c>
#endif

#ifndef HAVE_STRSIGNAL
#  include <compat/strsignal.c>
#endif

LOCAL void *
ao_malloc (size_t sz)
{
    void * res = malloc(sz);
    if (res == NULL) {
        fprintf(stderr, MALLOC_FAIL_FMT, sz);
        exit(EXIT_FAILURE);
    }
    return res;
}


LOCAL void *
ao_realloc (void *p, size_t sz)
{
    void * res = (p == NULL) ? malloc(sz) : realloc(p, sz);
    if (res == NULL) {
        fprintf(stderr, REALLOC_FAIL_FMT, sz, p);
        exit(EXIT_FAILURE);
    }
    return res;
}

LOCAL char *
ao_strdup (char const * str)
{
    char * res = strdup(str);
    if (res == NULL) {
        fprintf(stderr, STRDUP_FAIL_FMT, (int)strlen(str));
        exit(EXIT_FAILURE);
    }
    return res;
}

#ifdef __GNUC__
    void ignore_vars(void);
    void ignore_vars(void) {
        (void)option_load_mode, (void)program_pkgdatadir;
    }
#endif
/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * indent-tabs-mode: nil
 * End:
 * end of agen5/autogen.c */
