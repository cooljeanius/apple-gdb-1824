
/**
 * @file agInit.c
 *
 *  Do all the initialization stuff.  For daemon mode, only
 *  children will return.
 *
 *  Time-stamp:      "2012-02-12 12:14:50 bkorb"
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

/* = = = START-STATIC-FORWARD = = = */
static char const *
make_quote_str(char const * str);

static void
dep_usage(char const * fmt, ...);

static void
add_sys_env(char* pzEnvName);

static void
add_env_vars(void);
/* = = = END-STATIC-FORWARD = = = */

#ifdef DAEMON_ENABLED
 static ag_bool evalProto(char const ** ppzS, uint16_t* pProto);
 static void spawnPipe(char const* pzFile);
 static void spawnListens(char const * pzPort, sa_family_t af);
 static void daemonize(char const *, char const *, char const *,
                       char const *);
#endif

#include "expr.ini"

/**
 * Various initializations
 */
LOCAL void
initialize(int arg_ct, char** arg_vec)
{
    ag_scribble_init();

    /*
     *  Initialize all the Scheme functions.
     */
    ag_init();
    pzLastScheme = SCHEME_INIT_TEXT;
    ag_scm_c_eval_string_from_file_line(
        SCHEME_INIT_TEXT, AG_TEXT_STRTABLE_FILE, SCHEME_INIT_TEXT_LINENO);

    SCM_EVAL_CONST(INIT_SCM_ERRS_FMT);

    pzLastScheme = NULL;
    procState = PROC_STATE_OPTIONS;
    add_env_vars();

    doOptions(arg_ct, arg_vec);
    exit_code = AUTOGEN_EXIT_LOAD_ERROR;

    if (OPT_VALUE_TRACE > TRACE_NOTHING)
        SCM_EVAL_CONST(INIT_SCM_DEBUG_FMT);

#ifdef DAEMON_ENABLED

    if (! HAVE_OPT(DAEMON))
        return;

#ifdef DEBUG_ENABLED
    {
        static char const logf[] = "/tmp/ag-debug.txt";
        daemonize("/", logf, logf, logf);
    }
#else
    daemonize("/", DEV_NULL, DEV_NULL, DEV_NULL);
#endif /* DEBUG_ENABLED */

    {
        sa_family_t  af  = AF_INET;
        char const * pzS = OPT_ARG(DAEMON);

        if (evalProto(&pzS, &af))
            spawnListens(pzS, af);
        else
            spawnPipe(pzS);
    }
#endif /* DAEMON_ENABLED */
}

/**
 * make a name resilient to machinations made by 'make'.
 */
static char const *
make_quote_str(char const * str)
{
    size_t sz = strlen(str) + 1;
    char const * scan = str;
    char * res;

    for (;;) {
        char * p = strchr(scan, '$');
        if (p == NULL)
            break;
        sz++;
        scan = scan + 1;
    }

    res  = AGALOC(sz, "t name");
    scan = res;

    for (;;) {
        char * p = strchr(str, '$');

        if (p == NULL)
            break;
        sz = (p - str) + 1;
        memcpy(res, str, sz);
        res += sz;
        str += sz;
        *(res++) = '$';
    }

    strcpy(res, str);
    return scan;
}

/**
 * Error in dependency specification
 */
static void
dep_usage(char const * fmt, ...)
{
    char * msg;

    {
        va_list ap;
        va_start(ap, fmt);
        (void)vasprintf(&msg, fmt, ap);
        va_end(ap);
    }

    usage_message(USAGE_INVAL_DEP_OPT_FMT, msg);
    /* NOTREACHED */
}

/**
 * Configure dependency option
 */
LOCAL void
config_dep(tOptions * pOptions, tOptDesc * pOptDesc)
{
    char const * popt = pOptDesc->optArg.argString;

    /*
     *  The option argument is optional.  Make sure we have one.
     */
    if (popt == NULL)
        return;

    while (*popt == 'M')  popt++;

retry:

    switch (*popt) {
    case ' ': case TAB: case '\r': case NL:
        popt = SPN_WHITESPACE_CHARS(popt + 1);
        goto retry;

    case 'Q':
        if (pzDepTarget != NULL)
            dep_usage(CFGDEP_DUP_TARGET_MSG);

        popt = SPN_WHITESPACE_CHARS(popt + 1);
        pzDepTarget = make_quote_str(popt);
        break;

    case 'T':
        if (pzDepTarget != NULL)
            dep_usage(CFGDEP_DUP_TARGET_MSG);

        popt = SPN_WHITESPACE_CHARS(popt + 1);
        AGDUPSTR(pzDepTarget, popt, "t name");
        break;

    case 'D':
    case 'G':
    case NUL:
        /*
         *  'D' and 'G' make sense to GCC, not us.  Ignore 'em.  If we
         *  found a NUL byte, then we found -MM on the command line.
         */
        break;

    case 'F':
        if (pzDepFile != NULL)
            dep_usage(CFGDEP_DUP_TARGET_MSG);

        popt = SPN_WHITESPACE_CHARS(popt + 1);
        pzDepFile = aprf(CFGDEP_TARGET_TMP_FMT, popt);
        break;

    case 'P':
        dep_phonies = AG_TRUE;
        break;

    default:
        dep_usage(CFGDEP_UNKNOWN_DEP_FMT, popt);
    }
}

static void
add_sys_env(char* pzEnvName)
{
    int i = 2;

    for (;;) {
        if (IS_UPPER_CASE_CHAR(pzEnvName[i]))
            pzEnvName[i] = tolower(pzEnvName[i]);
        else if (! IS_ALPHANUMERIC_CHAR(pzEnvName[i]))
            pzEnvName[i] = '_';

        if (pzEnvName[ ++i ] == NUL)
            break;
    }

    /*
     *  If the user already has something in the environment, do not
     *  override it.
     */
    if (getenv(pzEnvName) == NULL) {
        char* pz;

        if (OPT_VALUE_TRACE > TRACE_DEBUG_MESSAGE)
            fprintf(pfTrace, TRACE_ADD_TO_ENV_FMT, pzEnvName);
        pz = aprf(ADD_SYS_ENV_VAL_FMT, pzEnvName);
        TAGMEM(pz, ADD_TO_ENV_MSG);
        putenv(pz);
    }
}

static void
add_env_vars(void)
{
    /*
     *  Set the last resort search directories first (lowest priority)
     *  The lowest of the low is the config time install data dir.
     *  Next is the *current* directory of this executable.
     */
    SET_OPT_TEMPL_DIRS(DFT_TPL_DIR_DATA);
    SET_OPT_TEMPL_DIRS(DFT_TPL_DIR_RELATIVE);

    {
        char z[ SCRIBBLE_SIZE ] = "__autogen__";
#if defined(HAVE_SOLARIS_SYSINFO)
        static const int nm[] = {
            SI_SYSNAME, SI_HOSTNAME, SI_ARCHITECTURE, SI_HW_PROVIDER,
#ifdef      SI_PLATFORM
            SI_PLATFORM,
#endif
            SI_MACHINE };
        int  ix;
        long sz;

        add_sys_env(z);
        for (ix = 0; ix < sizeof(nm)/sizeof(nm[0]); ix++) {
            sz = sysinfo(nm[ix], z+2, sizeof(z) - 2);
            if (sz > 0) {
                sz += 2;
                while (z[sz-1] == NUL)  sz--;
                strcpy(z + sz, ADD_ENV_VARS_SUFFIX_FMT+2);
                add_sys_env(z);
            }
        }

#elif defined(HAVE_UNAME_SYSCALL)
        struct utsname unm;

        add_sys_env(z);
        if (uname(&unm) != 0)
            AG_CANT(UNAME_CALL_NAME, SYSCALL_NAME);

        sprintf(z+2, ADD_ENV_VARS_SUFFIX_FMT, unm.sysname);
        add_sys_env(z);

        sprintf(z+2, ADD_ENV_VARS_SUFFIX_FMT, unm.machine);
        add_sys_env(z);

        sprintf(z+2, ADD_ENV_VARS_SUFFIX_FMT, unm.nodename);
        add_sys_env(z);
#else

        add_sys_env(z);
#endif
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *  T H E   F O L L O W I N G   I S   D E A D   C O D E
 *
 *  Someday, I want to enable daemon code, but need lotsa time.....
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifdef DAEMON_ENABLED

  static ag_bool
evalProto(char const ** ppzS, uint16_t* pProto)
{
    char const * pzS = *ppzS;

    if (IS_ALPHABETIC_CHAR(*pzS)) {
        inet_family_map_t* pMap = inet_family_map;
        do  {
            if (strncmp(pzS, pMap->pz_name, pMap->nm_len) == 0) {
                *pProto = pMap->family;
                *ppzS += pMap->nm_len;
                return AG_TRUE;
            }
        } while ((++pMap)->pz_name != NULL);
    }

    return IS_DEC_DIGIT_CHAR(*pzS);
}

  LOCAL void
handleSighup(int sig)
{
    redoOptions = AG_TRUE;
}

  static void
spawnPipe(char const * pzFile)
{
#   define S_IRW_ALL \
        S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH
    tFdPair fdpair;
    char* pzIn;
    char* pzOut;

    {
        size_t len = 2 * (strlen(pzFile) + 5);
        pzIn = AGALOC(len + 5, "fifo name");
        pzOut = pzIn + sprintf(pzIn, PIPE_FIFO_IN_NAME_FMT, pzFile) + 1;
    }

    unlink(pzIn);
    if ((mkfifo(pzIn, S_IRW_ALL) != 0) && (errno != EEXIST))
        AG_CANT(PIPE_MKFIFO_NAME,    pzIn);

    (void)sprintf(pzOut, PIPE_FIFO_OUT_NAME_FMT, pzFile);
    unlink(pzOut);
    if ((mkfifo(pzOut, S_IRW_ALL) != 0) && (errno != EEXIST))
        AG_CANT(PIPE_MKFIFO_NAME,    pzOut);

    fdpair.readFd = open(pzIn, O_RDONLY);
    if (fdpair.readFd < 0)
        AG_CANT(PIPE_FIFO_OPEN, pzIn);

    {
        struct pollfd polls[1];
        polls[0].fd     = fdpair.readFd;
        polls[0].events = POLLIN | POLLPRI;

        for (;;) {
            int ct = poll(polls, 1, -1);
            struct strrecvfd recvfd;
            pid_t child;

            switch (ct) {
            case -1:
                if ((errno != EINTR) || (! redoOptions))
                    goto spawnpipe_finish;

                optionRestore(&autogenOptions);
                doOptions(autogenOptions.origArgCt,
                          autogenOptions.origArgVect);
                SET_OPT_DEFINITIONS(PIPE_DEFS_STDIN_STR);
                break;

            case 1:
                if ((polls[0].revents & POLLIN) == 0)
                    continue;

                child = fork();
                switch (child) {
                default:
                    waitpid(child, &ct, 0);
                    continue;

                case -1:
                    AG_CANT(PIPE_FORK_NAME, zNil);

                case 0:
                }

                if (dup2(fdpair.readFd, STDIN_FILENO) != STDIN_FILENO)
                    AG_CANT(PIPE_DUP2_NAME_STR, PIPE_DEFS_STDIN_NAME);

                fdpair.writeFd = open(pzOut, O_WRONLY);
                if (fdpair.writeFd < 0)
                    AG_CANT(PIPE_FIFO_OPEN, pzOut);

                polls[0].fd = fdpair.writeFd;
                polls[0].events = POLLOUT;
                if (poll(polls, 1, -1) != 1)
                    AG_CANT(PIPE_POLL_NAME_STR, PIPE_WRITE_NAME_STR);

                if (dup2(fdpair.writeFd, STDOUT_FILENO) != STDOUT_FILENO)
                    AG_CANT(PIPE_DUP2_NAME_STR, pzOut);

                return;
            }
        }
    }

 spawnpipe_finish:
    unlink(pzIn);
    unlink(pzOut);
    AGFREE(pzIn);

#   undef S_IRW_ALL

    exit(EXIT_SUCCESS);
}


  static void
spawnListens(char const * pzPort, sa_family_t addr_family)
{
    int socket_fd = socket(addr_family, SOCK_STREAM, 0);
    union {
        struct sockaddr     addr;
        struct sockaddr_in  in_addr;
        struct sockaddr_un  un_addr;
    } sa;

    uint32_t        addr_len;

    if (socket_fd < 0)
        AG_CANT("socket", "AF_INET/SOCK_STREAM");

    switch (addr_family) {

    case AF_UNIX:
    {
        uint32_t p_len = strlen(pzPort);

        if (p_len > sizeof(sa.un_addr.sun_path))
            AG_ABEND(aprf("AF_UNIX path exceeds %d", p_len));
        sa.un_addr.sun_family  = AF_UNIX;
        strncpy(sa.un_addr.sun_path, pzPort, p_len);
        addr_len = sizeof(sa.un_addr) - sizeof(sa.un_addr.sun_path) + p_len;
    }
    break;

    case AF_INET:
    {
        uint16_t port;
        char* pz;

        sa.in_addr.sin_family      = AF_INET;
        sa.in_addr.sin_addr.s_addr = INADDR_ANY;

        errno = 0;
        if ((unlink(pzPort) != 0) && (errno != ENOENT))
            AG_CANT("unlink", pzPort);

        port = (uint16_t)strtol(pzPort, &pz, 0);
        if ((errno != 0) || (*pz != NUL))
            AG_ABEND(aprf("Invalid port number:  '%s'", pzPort));

        sa.in_addr.sin_port = htons((short)port);
        addr_len = sizeof(sa.in_addr);
    }
    break;

    default:
        AG_ABEND(aprf("The '%d' address family cannot be handled",
                      addr_family));
    }

    if (bind(socket_fd, &sa.addr, addr_len) < 0) {
        char* pz = aprf(LISTEN_PORT_FMT, pzPort, addr_family);
        AG_CANT("bind", pz);
    }

    if (fcntl(socket_fd, F_SETFL, O_NONBLOCK) < 0)
        AG_CANT("socket-fcntl", "FNDELAY");

    if (listen(socket_fd, 5) < 0)
        AG_CANT("listen", aprf(LISTEN_PORT_FMT, pzPort));

    for (;;) {
        fd_set fds;
        int    max_fd = socket_fd;
        int    new_conns;

        FD_ZERO(&fds);
        FD_SET(socket_fd, &fds);

        new_conns = select(max_fd, &fds, NULL, NULL, NULL);
        if (new_conns < 0) {
            if (errno == EINTR)
                continue;

            if (! redoOptions) {
                unlink(pzPort);
                exit(EXIT_SUCCESS);
            }

            optionRestore(&autogenOptions);
            doOptions(autogenOptions.origArgCt,
                      autogenOptions.origArgVect);
            SET_OPT_DEFINITIONS("-");

            continue;
        }

        if (new_conns > 0) {
            switch (fork()) {
            default: continue;
            case -1:
                AG_CANT("fork", zNil);

            case 0:  break;
            }
            break;
        }
    }

    for (;;) {
        static int try_ct = 0;
        struct sockaddr addr;
        socklen_t addr_len;
        int fd = accept(socket_fd, &addr, &addr_len);
        if (fd < 0)
            switch (errno) {
            case EINTR:
            case EAGAIN:
#if (EAGAIN != EWOULDBLOCK)
            case EWOULDBLOCK:
#endif
                if (try_ct++ < 10000) {
                    sleep(1);
                    continue;
                }
            }
        socket_fd = fd;
        break;
    }

    if (dup2(socket_fd, STDOUT_FILENO) != STDOUT_FILENO)
        AG_CANT("dup2", "out on socket_fd");
    if (dup2(socket_fd, STDIN_FILENO) != STDIN_FILENO)
        AG_CANT("dup2", "in on socket_fd");
}


  static void
daemonize(char const * pzStdin, char const * pzStdout, char const * pzStderr,
          char const * pzDaemonDir)
{
    static char const zNoFork[] = "Error %d while forking:  %s\n";
    /*
     *  Become a daemon process by exiting the current process
     *  and allowing the child to continue.  Also, change stdin,
     *  stdout and stderr to point to /dev/null and change to
     *  the root directory ('/').
     */
    {
        int ret = fork();

        switch (ret) {
        case -1:
            fprintf(stderr, zNoFork, errno, strerror(errno));
        default:
            exit(ret);

        case 0:
            break;
        }
    }

    /*
     *  Now, become a process group and session group leader.
     */
    if (setsid() == -1) {
        fprintf(stderr, "Error %d setting session ID:  %s\n",
                errno, strerror(errno));
        exit(99);
    }

    /*
     *  There is now no controlling terminal.  However, if we open anything
     *  that resembles a terminal, it will become our controlling terminal.
     *  So, we will fork again and the child will not be a group leader and
     *  thus cannot gain a controlling terminal.
     */
    switch (fork()) {
    case -1:
        fprintf(stderr, zNoFork, errno, strerror(errno));
        exit(99);

    case 0:
        break;

    default:
        exit(EXIT_SUCCESS);  /* parent process - silently go away */
    }

    umask(0);
    if (pzDaemonDir == (char*)NULL)
        pzDaemonDir = "/";

    chdir(pzDaemonDir);

    /*
     *  Reopen the input, output and error files, unless we were told not to
     */
    if (pzStdin != (char*)NULL)
        freopen(pzStdin,  "r", stdin);

    if (pzStdout != (char*)NULL)
        freopen(pzStdout, "w", stdout);

    if (pzStderr != (char*)NULL)
        freopen(pzStderr, "w", stderr);

    /* We are a daemon now */
}
#endif /* DAEMON_ENABLED */
/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * indent-tabs-mode: nil
 * End:
 * end of agen5/agInit.c */
