/**
 * @file agUtils.c
 *
 * Various utilities for AutoGen.
 *
 *  Time-stamp:        "2012-01-29 20:18:27 bkorb"
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
static void
define_base_name(void);

static void
put_defines_into_env(void);

static char const *
skipQuote(char const * pzQte);
/* = = = END-STATIC-FORWARD = = = */

LOCAL char *
aprf(char const * pzFmt, ...)
{
    char* pz;
    va_list ap;
    va_start(ap, pzFmt);
    (void)vasprintf(&pz, pzFmt, ap);
    va_end(ap);

    if (pz == NULL) {
        char z[ 2 * SCRIBBLE_SIZE ];
        snprintf(z, sizeof(z), APRF_ALLOCATE_FAIL, pzFmt);
        AG_ABEND(z);
    }
    return pz;
}

static void
define_base_name(void)
{
    char const *  pz;
    char* pzD;

    if (! ENABLED_OPT(DEFINITIONS)) {
        OPT_ARG(BASE_NAME) = DFT_BASE_NAME;
        return;
    }

    pz = strrchr(OPT_ARG(DEFINITIONS), '/');
    /*
     *  Point to the character after the last '/', or to the full
     *  definition file name, if there is no '/'.
     */
    pz = (pz == NULL) ? OPT_ARG(DEFINITIONS) : (pz + 1);

    /*
     *  IF input is from stdin, then use "stdin"
     */
    if ((pz[0] == '-') && (pz[1] == NUL)) {
        OPT_ARG(BASE_NAME) = STDIN_FILE_NAME;
        return;
    }

    /*
     *  Otherwise, use the basename of the definitions file
     */
    OPT_ARG(BASE_NAME) = \
        pzD = AGALOC(strlen(pz)+1, "derived base");

    while ((*pz != NUL) && (*pz != '.'))  *(pzD++) = *(pz++);
    *pzD = NUL;
}

static void
put_defines_into_env(void)
{
    int     ct  = STACKCT_OPT(DEFINE);
    char const **   ppz = STACKLST_OPT(DEFINE);

    do  {
        char const * pz = *(ppz++);
        /*
         *  IF there is no associated value,  THEN set it to '1'.
         *  There are weird problems with empty defines.
         *  FIXME:  we loose track of this memory.  Don't know what to do,
         *  really, there is no good recovery mechanism for environment
         *  data.
         */
        if (strchr(pz, '=') == NULL) {
            size_t siz = strlen(pz)+3;
            char*  p   = AGALOC(siz, "env define");

            strcpy(p, pz);
            strcpy(p+siz-3, DFT_ENV_VAL);
            pz = p;
        }

        /*
         *  Now put it in the environment
         */
        putenv((char*)pz);
    } while (--ct > 0);
}

/**
 *  Open trace output file.
 *
 *  If the name starts with a pipe character (vertical bar), then
 *  use popen on the command.  If it starts with ">>", then append
 *  to the file name that  follows that.
 *
 *  The trace output starts with the command and arguments used to
 *  start autogen.
 */
LOCAL void
open_trace_file(char ** av, tOptDesc * odsc)
{
    char const * fname = odsc->optArg.argString;

    trace_is_to_pipe = (*fname == '|');
    if (trace_is_to_pipe)
        pfTrace = popen(++fname, "w");

    else if ((fname[0] == '>') && (fname[1] == '>')) {
        fname = SPN_WHITESPACE_CHARS(fname + 2);
        pfTrace = fopen(fname, "a");
    }

    else
        pfTrace = fopen(fname, "w");

    if (pfTrace == NULL)
        AG_ABEND(aprf(OPEN_ERROR_FMT, errno, strerror(errno), fname));

#ifdef _IONBF
    setvbuf(pfTrace, NULL, _IONBF, 0);
#endif

    fprintf(pfTrace, TRACE_START_FMT, (unsigned int)getpid(), *av);
    while (*(++av) != NULL)
        fprintf(pfTrace, TRACE_AG_ARG_FMT, *av);
    putc(NL, pfTrace);
}

LOCAL void
check_make_dep_env(void)
{
    ag_bool have_opt_string = AG_FALSE;
    ag_bool set_opt         = AG_FALSE;

    char const * mdep = getenv(AG_MAKE_DEP_NAME);
    if (mdep == NULL) {
        mdep = getenv(DEP_OUT_NAME);
        if (mdep == NULL)
            return;
    }
    switch (*mdep) {
    case NUL: break;
    case '1':
        set_opt = (mdep[0] == '1');
        /* FALLTHROUGH */

    case '0':
        if (mdep[1] != NUL)
            have_opt_string = AG_TRUE;
        break;

    case 'y':
    case 'Y':
        set_opt = AG_TRUE;
        have_opt_string = (streqvcmp(mdep + 1, YES_NAME_STR+1) != 0);
        break;

    case 'n':
    case 'N':
        set_opt = \
            have_opt_string = (streqvcmp(mdep + 1, NO_NAME_STR+1) != 0);
        break;

    case 't':
    case 'T':
        set_opt = AG_TRUE;
        have_opt_string = (streqvcmp(mdep + 1, TRUE_NAME_STR+1) != 0);
        break;

    case 'f':
    case 'F':
        set_opt = \
            have_opt_string = (streqvcmp(mdep + 1, FALSE_NAME_STR+1) != 0);
        break;

    default:
        have_opt_string = \
            set_opt = AG_TRUE;
    }
    if (! set_opt) return;
    if (! have_opt_string) {
        SET_OPT_MAKE_DEP("");
        return;
    }

    {
        char * pz = AGALOC(strlen(mdep) + 5, "mdep");
        char * fp = pz;

        *(pz++) = 'F';
        for (;;) {
            int ch = *(unsigned char *)(mdep++);
            if (IS_END_TOKEN_CHAR(ch))
                break;

            *(pz++) = ch;
        }
        *pz = NUL;

        SET_OPT_SAVE_OPTS(fp);
        mdep = SPN_WHITESPACE_CHARS(mdep);
        if (*mdep == NUL)
            return;

        pz = fp;
        *(pz++) = 'T';
        for (;;) {
            int ch = *(unsigned char *)(mdep++);
            if (IS_END_TOKEN_CHAR(ch))
                break;

            *(pz++) = ch;
        }
        *pz = NUL;
        SET_OPT_SAVE_OPTS(fp);
        AGFREE(fp);
    }
}

LOCAL void
doOptions(int arg_ct, char ** arg_vec)
{
    /*
     *  Advance the argument counters and pointers past any
     *  command line options
     */
    {
        int  optCt = optionProcess(&autogenOptions, arg_ct, arg_vec);

        /*
         *  Make sure we have a source file, even if it is "-" (stdin)
         */
        switch (arg_ct - optCt) {
        case 1:
            if (! HAVE_OPT(DEFINITIONS)) {
                OPT_ARG(DEFINITIONS) = *(arg_vec + optCt);
                break;
            }
            /* FALLTHROUGH */

        default:
            usage_message(DOOPT_TOO_MANY_DEFS, pzProg);
            /* NOTREACHED */

        case 0:
            if (! HAVE_OPT(DEFINITIONS))
                OPT_ARG(DEFINITIONS) = DFT_DEF_INPUT_STR;
            break;
        }
    }

    if ((OPT_VALUE_TRACE > TRACE_NOTHING) && HAVE_OPT(TRACE_OUT))
        open_trace_file(arg_vec, &DESC(TRACE_OUT));

    startTime = time(NULL);

    if (! HAVE_OPT(TIMEOUT))
        OPT_ARG(TIMEOUT) = (char const *)AG_DEFAULT_TIMEOUT;

    /*
     *  IF the definitions file has been disabled,
     *  THEN a template *must* have been specified.
     */
    if (  (! ENABLED_OPT(DEFINITIONS))
       && (! HAVE_OPT(OVERRIDE_TPL)) )
        AG_ABEND(NO_TEMPLATE_ERR_MSG);

    /*
     *  IF we do not have a base-name option, then we compute some value
     */
    if (! HAVE_OPT(BASE_NAME))
        define_base_name();

    check_make_dep_env();

    if (HAVE_OPT(MAKE_DEP))
        start_dep_file();

    strequate(OPT_ARG(EQUATE));

    /*
     *  IF we have some defines to put in our environment, ...
     */
    if (HAVE_OPT(DEFINE))
        put_defines_into_env();
}

LOCAL char const *
getDefine(char const * pzDefName, ag_bool check_env)
{
    char const **   ppz;
    int     ct;
    if (HAVE_OPT(DEFINE)) {
        ct  = STACKCT_OPT( DEFINE);
        ppz = STACKLST_OPT(DEFINE);

        while (ct-- > 0) {
            char const * pz   = *(ppz++);
            char * pzEq = strchr(pz, '=');
            int    res;

            if (pzEq != NULL)
                *pzEq = NUL;

            res = strcmp(pzDefName, pz);
            if (pzEq != NULL)
                *pzEq = '=';

            if (res == 0)
                return (pzEq != NULL) ? pzEq+1 : zNil;
        }
    }
    return check_env ? getenv(pzDefName) : NULL;
}


/*
 *  The following routine scans over quoted text, shifting
 *  it in the process and eliminating the starting quote,
 *  ending quote and any embedded backslashes.  They may
 *  be used to embed the quote character in the quoted text.
 *  The quote character is whatever character the argument
 *  is pointing at when this procedure is called.
 */
LOCAL char*
spanQuote(char* pzQte)
{
    char  q = *pzQte;          /*  Save the quote character type */
    char* p = pzQte++;         /*  Destination pointer           */

    while (*pzQte != q) {
        switch (*p++ = *pzQte++) {
        case NUL:
            return pzQte-1;      /* Return address of terminating NUL */

        case '\\':
            if (q != '\'') {
                int ct = ao_string_cook_escape_char(pzQte, p-1, 0x7F);
                if (p[-1] == 0x7F)  p--;
                pzQte += ct;

            } else {
                switch (*pzQte) {
                case '\\':
                case '\'':
                case '#':
                    p[-1] = *pzQte++;
                }
            }
            break;

        default:
            ;
        }
    }

    *p = NUL;
    return pzQte+1; /* Return addr of char after the terminating quote */
}

/*
 *  The following routine skips over quoted text.
 *  The quote character is whatever character the argument
 *  is pointing at when this procedure is called.
 */
static char const *
skipQuote(char const * pzQte)
{
    char  q = *pzQte++;        /*  Save the quote character type */

    while (*pzQte != q) {
        switch (*pzQte++) {
        case NUL:
            return pzQte-1;      /* Return address of terminating NUL */

        case '\\':
            if (q == '\'') {
                /*
                 *  Single quoted strings process the backquote specially
                 *  only in fron of these three characters:
                 */
                switch (*pzQte) {
                case '\\':
                case '\'':
                case '#':
                    pzQte++;
                }

            } else {
                char p[10];  /* provide a scratch pad for escape processing */
                int ct = ao_string_cook_escape_char(pzQte, p, 0x7F);
                pzQte += ct;
            } /* if (q == '\'')      */
        }     /* switch (*pzQte++)   */
    }         /* while (*pzQte != q) */

    return pzQte+1; /* Return addr of char after the terminating quote */
}


LOCAL char const *
skipScheme(char const * pzSrc,  char const * pzEnd)
{
    int  level = 0;

    for (;;) {
        if (pzSrc >= pzEnd)
            return pzEnd;
        switch (*(pzSrc++)) {
        case '(':
            level++;
            break;

        case ')':
            if (--level == 0)
                return pzSrc;
            break;

        case '"':
            pzSrc = skipQuote(pzSrc-1);
        }
    }
}


LOCAL int
count_nl(char const * pz)
{
    int ct = 0;
    for (;;) {
        char const * p = strchr(pz, NL);
        if (p == NULL)
            break;
        ct++;
        pz = p + 1;
    }
    return ct;
}


LOCAL char const *
skipExpression(char const * pzSrc, size_t len)
{
    char const * pzEnd = pzSrc + len;

 guess_again:

    pzSrc = SPN_WHITESPACE_CHARS(pzSrc);
    if (pzSrc >= pzEnd)
        return pzEnd;
    switch (*pzSrc) {
    case ';':
        pzSrc = strchr(pzSrc, NL);
        if (pzSrc == NULL)
            return pzEnd;
        goto guess_again;

    case '(':
        return skipScheme(pzSrc, pzEnd);

    case '"':
    case '\'':
    case '`':
        pzSrc = skipQuote(pzSrc);
        return (pzSrc > pzEnd) ? pzEnd : pzSrc;

    default:
        break;
    }

    pzSrc = BRK_WHITESPACE_CHARS(pzSrc);
    return (pzSrc > pzEnd) ? pzEnd : pzSrc;
}
/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * indent-tabs-mode: nil
 * End:
 * end of agen5/agUtils.c */
