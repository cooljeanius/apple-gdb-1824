/**
 * @file defDirect.c
 *
 *  Time-stamp:        "2012-01-29 20:35:55 bkorb"
 *
 *  This module processes definition file directives.
 *
 *  blocksort spacing=2 \
 *    output=defDirect-sorted.c \
 *    input=defDirect.c \
 *    pat='^/\*=directive' \
 *    start='^doDir_IGNORE' \
 *    trail='\+\+\+ End of Directives'
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
#define NO_MATCH_ERR(_typ) \
AG_ABEND(aprf(DIRECT_NOMATCH_FMT, pCurCtx->pzCtxFname, pCurCtx->lineNo, _typ))

static int  ifdefLevel = 0;

static teDirectives
findDirective(char* pzDirName);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *  processDirective
 *
 *  THIS IS THE ONLY EXTERNAL ENTRY POINT
 *
 *  A directive character has been found.
 *  Decide what to do and return a pointer to the character
 *  where scanning is to resume.
 */
LOCAL char*
processDirective(char* pzScan)
{
    const tDirTable * pTbl = dirTable;
    char *     pzDir;
    char *     pzEnd;

    /*
     *  Search for the end of the #-directive.
     *  Replace "\\\n" sequences with "  ".
     */
    for (;;) {
        pzEnd = strchr(pzScan, NL);

        if (pzEnd == NULL) {
            /*
             *  The end of the directive is the end of the string
             */
            pzEnd = pzScan + strlen(pzScan);
            break;
        }
        pCurCtx->lineNo++;

        if (pzEnd[-1] != '\\') {
            /*
             *  The end of the directive is the end of the line
             *  and the line has not been continued.
             */
            *(pzEnd++) = NUL;
            break;
        }

        /*
         *  Replace the escape-newline pair with spaces and
         *  find the next end of line
         */
        pzEnd[-1] = pzEnd[0] = ' ';
    }

    /*
     *  Ignore ``#!'' as a comment, enabling a definition file to behave
     *  as a script that gets interpreted by autogen.  :-)
     */
    if (*pzScan == '!')
        return pzEnd;

    /*
     *  Find the start of the directive name.  Ensure it _is_ a name.
     */
    pzScan = SPN_WHITESPACE_CHARS(pzScan);
    if (! IS_ALPHABETIC_CHAR(*pzScan))
        return pzEnd;

    /*
     *  Find the *END* of the directive name.
     */
    pzDir  = pzScan;
    pzScan = SPN_ALPHABETIC_CHARS(pzScan+1);

    /*
     *  IF there is anything that follows the name, ...
     */
    if (*pzScan != NUL) {
        char * pz;

        /*
         *  IF something funny immediately follows the directive name,
         *  THEN we will ignore it completely.
         */
        if (! IS_WHITESPACE_CHAR(*pzScan))
            return pzEnd;

        /*
         *  Terminate the name being defined
         *  and find the start of anything else.
         */
        *pzScan = NUL;
        pzScan  = SPN_WHITESPACE_CHARS(pzScan+1);


        /*
         *  Trim off trailing white space
         */
        pz = pzScan + strlen(pzScan);
        while (  (pz > pzScan)
              && IS_WHITESPACE_CHAR(pz[-1])) pz--;
        *pz = NUL;
    }

    pTbl = dirTable + (int)findDirective(pzDir);
    return (*(pTbl->pDirProc))(pzScan, pzEnd);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *  Figure out the index of a directive.  We will return the directive
 *  count if it is a bogus name.
 */

static teDirectives
findDirective(char* pzDirName)
{
    teDirectives res = (teDirectives)0;
    const tDirTable *  pTbl = dirTable;

    do  {
        if (strneqvcmp(pzDirName, pTbl->pzDirName, pTbl->nameSize) != 0)
            continue;

        /*
         *  A directive name ends with either white space or a NUL
         */
        if (IS_END_TOKEN_CHAR(pzDirName[ pTbl->nameSize ]))
            return res;

    } while (pTbl++, ++res < DIRECTIVE_CT);

    {
        char ch;
        if (strlen(pzDirName) > 32) {
            ch = pzDirName[32];
            pzDirName[32] = NUL;
        } else {
            ch = NUL;
        }

        fprintf(pfTrace, FIND_DIRECT_UNKNOWN, pCurCtx->pzCtxFname,
                pCurCtx->lineNo, pzDirName);

        if (ch != NUL)
            pzDirName[32] = ch;
    }
    return res;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *  Support routines for the directives
 *
 *  skipToEndif
 *
 *  Skip through the text to a matching "#endif".  We do this when we
 *  have processed the allowable text (found an "#else" after
 *  accepting the preceeding text) or when encountering a "#if*def"
 *  while skipping a block of text due to a failed test.
 */
static char*
skipToEndif(char* pzStart)
{
    char* pzScan = pzStart;
    char* pzRet;

    for (;;) {
        /*
         *  'pzScan' is pointing to the first character on a line.
         *  Check for a directive on the current line before scanning
         *  later lines.
         */
        if (*pzScan == '#')
            pzScan++;
        else {
            char* pz = strstr(pzScan, DIRECT_CK_LIST_MARK);
            if (pz == NULL)
                AG_ABEND(aprf(DIRECT_NOENDIF_FMT, pCurCtx->pzCtxFname,
                              pCurCtx->lineNo));

            pzScan = pz + 2;
        }

        pzScan = SPN_WHITESPACE_CHARS(pzScan);

        switch (findDirective(pzScan)) {
        case DIR_ENDIF:
        {
            /*
             *  We found the endif we are interested in
             */
            char* pz = strchr(pzScan, NL);
            if (pz != NULL)
                 pzRet = pz+1;
            else pzRet = pzScan + strlen(pzScan);
            goto leave_func;
        }

        case DIR_IFDEF:
        case DIR_IFNDEF:
            /*
             *  We found a nested ifdef/ifndef
             */
            pzScan = skipToEndif(pzScan);
            break;

        default:
            /*
             *  We do not care what we found
             */
            break; /* ignore it */
        }  /* switch (findDirective(pzScan)) */
    }

 leave_func:
    while (pzStart < pzRet) {
        if (*(pzStart++) == NL)
            pCurCtx->lineNo++;
    }
    return pzRet;
}


static char*
skipToEndmac(char* pzStart)
{
    char* pzScan = pzStart;
    char* pzRet;

    for (;;) {
        /*
         *  'pzScan' is pointing to the first character on a line.
         *  Check for a directive on the current line before scanning
         *  later lines.
         */
        if (*pzScan == '#')
            pzScan++;
        else {
            char* pz = strstr(pzScan, DIRECT_CK_LIST_MARK);
            if (pz == NULL)
                AG_ABEND(aprf(DIRECT_NOENDIF_FMT, pCurCtx->pzCtxFname,
                              pCurCtx->lineNo));

            pzScan = pz + 2;
        }

        pzScan = SPN_WHITESPACE_CHARS(pzScan);

        if (findDirective(pzScan) == DIR_ENDMAC) {
            /*
             *  We found the endmac we are interested in
             */
            char* pz = strchr(pzScan, NL);
            if (pz != NULL)
                 pzRet = pz+1;
            else pzRet = pzScan + strlen(pzScan);
            break;
        }
    }

    while (pzStart < pzRet) {
        if (*(pzStart++) == NL)
            pCurCtx->lineNo++;
    }
    return pzRet;
}


/*
 *  skipToElseEnd
 *
 *  Skip through the text to a matching "#endif" or "#else" or
 *  "#elif*def".  We do this when we are skipping code due to a failed
 *  "#if*def" test.
 */
static char*
skipToElseEnd(char* pzStart)
{
    char* pzScan = pzStart;
    char* pzRet;

    for (;;) {
        /*
         *  'pzScan' is pointing to the first character on a line.
         *  Check for a directive on the current line before scanning
         *  later lines.
         */
        if (*pzScan == '#')
            pzScan++;
        else {
            char* pz = strstr(pzScan, DIRECT_CK_LIST_MARK);
            if (pz == NULL)
                AG_ABEND(aprf(DIRECT_NOENDIF_FMT, pCurCtx->pzCtxFname,
                              pCurCtx->lineNo));

            pzScan = pz + 2;
        }

        pzScan = SPN_WHITESPACE_CHARS(pzScan);

        switch (findDirective(pzScan)) {
        case DIR_ELSE:
            /*
             *  We found an "else" directive for an "ifdef"/"ifndef"
             *  that we were skipping over.  Start processing the text.
             */
            ifdefLevel++;
            /* FALLTHROUGH */

        case DIR_ENDIF:
        {
            /*
             *  We reached the end of the "ifdef"/"ifndef" we were
             *  skipping (or we dropped in from above).
             *  Start processing the text.
             */
            char* pz = strchr(pzScan, NL);
            if (pz != NULL)
                 pzRet = pz+1;
            else pzRet = pzScan + strlen(pzScan);
            goto leave_func;
        }

        case DIR_IFDEF:
        case DIR_IFNDEF:
            /*
             *  We have found a nested "ifdef"/"ifndef".
             *  Call "skipToEndif()" to find *its* end, then
             *  resume looking for our own "endif" or "else".
             */
            pzScan = skipToEndif(pzScan);
            break;

        default:
            /*
             *  We either don't know what it is or we do not care.
             */
            break;
        }  /* switch (findDirective(pzScan)) */
    }

 leave_func:
    while (pzStart < pzRet) {
        if (*(pzStart++) == NL)
            pCurCtx->lineNo++;
    }
    return pzRet;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *  Special routines for each directive.  These routines are *ONLY*
 *  called from the table when the input is being processed.
 *  After this routine are either declarations or definitions of
 *  directive handling routines.  The documentation for these routines
 *  is extracted from this file.  See 'makedef.sh' for how it works.
 *  Each declared directive should have either a 'dummy:' section
 *  (if the directive is to be ignored) or a 'text:' section
 *  (if there is some form of implementation).  If the directive
 *  needs or may take arguments (e.g. '#define'), then there should
 *  also be an 'arg:' section describing the argument(s).
 */
static char*
doDir_IGNORE(char* pzArg, char* pzScan)
{
    return pzScan;
}


/*=directive assert
 *
 *  arg:  `shell-script` | (scheme-expr) | <anything else>
 *
 *  text:
 *  If the @code{shell-script} or @code{scheme-expr} do not yield @code{true}
 *  valued results, autogen will be aborted.  If @code{<anything else>} or
 *  nothing at all is provided, then this directive is ignored.
 *
 *  When writing the shell script, remember this is on a preprocessing
 *  line.  Multiple lines must be backslash continued and the result is a
 *  single long line.  Separate multiple commands with semi-colons.
 *
 *  The result is @code{false} (and fails) if the result is empty, the
 *  number zero, or a string that starts with the letters 'n' or 'f' ("no"
 *  or "false").
=*/
static void
check_assert_str(char const* pz, char const* pzArg)
{
    pz = SPN_WHITESPACE_CHARS(pz);

    if (IS_FALSE_TYPE_CHAR(*pz))
        AG_ABEND(aprf(DIRECT_ASSERT_FMT, pz, pzArg));
}

static char*
doDir_assert(char* pzArg, char* pzScan)
{
    switch (*pzArg) {
    case '`':
    {
        char* pzS = pzArg+1;
        char* pzR;

        pzR = strrchr(pzS, '`');
        if (pzR == NULL)
            break; /* not a valid script */

        *pzR = NUL;
        pzS = shell_cmd((char const*)pzS);
        check_assert_str(pzS, pzArg);
        free(pzS);
        break;
    }

    case '(':
    {
        SCM res = ag_scm_c_eval_string_from_file_line(
            pzArg, pCurCtx->pzCtxFname, pCurCtx->lineNo );

        check_assert_str(resolveSCM(res), pzArg);
        break;
    }

    default:
        break;
    }

    return pzScan;
}


/*=directive define
 *
 *  arg:  name [ <text> ]
 *
 *  text:
 *  Will add the name to the define list as if it were a DEFINE program
 *  argument.  Its value will be the first non-whitespace token following
 *  the name.  Quotes are @strong{not} processed.
 *
 *  After the definitions file has been processed, any remaining entries
 *  in the define list will be added to the environment.
=*/
static char*
doDir_define(char* pzArg, char* pzScan)
{
    char*  pzName = pzArg;

    /*
     *  Skip any #defines that do not look reasonable
     */
    if (! IS_VAR_FIRST_CHAR(*pzArg))
        return pzScan;

    pzArg = SPN_VARIABLE_NAME_CHARS(pzArg);

    /*
     *  IF this is a macro definition (rather than a value def),
     *  THEN we will ignore it.
     */
    if (*pzArg == '(')
        return pzScan;

    /*
     *  We have found the end of the name.
     *  IF there is no more data on the line,
     *  THEN we do not have space for the '=' required by PUTENV.
     *       Therefore, move the name back over the "#define"
     *       directive itself, giving us the space needed.
     */
    if (! IS_WHITESPACE_CHAR(*pzArg)) {
        char* pzS = pzName;
        char* pzD = (pzName -= 6);

        *pzArg = NUL;
        while ((*(pzD++) = *(pzS++)) != NUL)   ;
        pzD[-1] = '=';
        pzD[ 0] = NUL;

    } else {
        /*
         *  Otherwise, insert the '=' and move any data up against it.
         *  We only accept one name-type, space separated token.
         *  We are not ANSI-C.  ;-)
         */
        char*  pz = pzArg+1;
        *pzArg++ = '=';
        pz = SPN_WHITESPACE_CHARS(pz);

        for (;;) {
            if ((*pzArg++ = *pz++) == NUL)
                break;
            if (! IS_UNQUOTABLE_CHAR(*pz)) {
                *pzArg = NUL;
                break;
            }
        }
    }

    SET_OPT_DEFINE(pzName);
    return pzScan;
}


/*=directive elif
 *
 *  text:
 *  This must follow an @code{#if}
 *  otherwise it will generate an error.
 *  It will be ignored.
=*/
static char*
doDir_elif(char* pzArg, char* pzScan)
{
    AG_ABEND(aprf(DIRECT_ELIF_BAD_FMT, pCurCtx->pzCtxFname, pCurCtx->lineNo));
    /* NOTREACHED */
    return NULL;
}


/*=directive else
 *
 *  text:
 *  This must follow an @code{#if}, @code{#ifdef} or @code{#ifndef}.
 *  If it follows the @code{#if}, then it will be ignored.  Otherwise,
 *  it will change the processing state to the reverse of what it was.
=*/
static char*
doDir_else(char* pzArg, char* pzScan)
{
    if (--ifdefLevel < 0)
        NO_MATCH_ERR(DIRECT_ELSE_BAD);

    return skipToEndif(pzScan);
}


/*=directive endif
 *
 *  text:
 *  This must follow an @code{#if}, @code{#ifdef} or @code{#ifndef}.
 *  In all cases, this will resume normal processing of text.
=*/
static char*
doDir_endif(char* pzArg, char* pzScan)
{
    if (--ifdefLevel < 0)
        NO_MATCH_ERR(DIRECT_ENDIF_BAD);

    return pzScan;
}


/*=directive endmac
 *
 *  text:
 *  This terminates a "macdef", but must not ever be encountered directly.
=*/
static char*
doDir_endmac(char* pzArg, char* pzScan)
{
    NO_MATCH_ERR(DIRECT_ENDMAC_BAD);
    /* NOTREACHED */
    return NULL;
}


/*=directive endshell
 *
 *  text:
 *  Ends the text processed by a command shell into autogen definitions.
=*/
static char*
doDir_endshell(char* pzArg, char* pzScan)
{
    /*
     *  In actual practice, the '#endshell's must be consumed inside
     *  the 'doDir_shell()' procedure.
     */
    NO_MATCH_ERR(DIRECT_ENDSHELL_BAD);

    /* NOTREACHED */
    return NULL;
}


/*=directive error
 *
 *  arg:  [ <descriptive text> ]
 *
 *  text:
 *  This directive will cause AutoGen to stop processing
 *  and exit with a status of EXIT_FAILURE.
=*/
static char*
doDir_error(char* pzArg, char* pzScan)
{
    AG_ABEND(aprf(DIRECT_ERROR_FMT, pCurCtx->pzCtxFname, pCurCtx->lineNo,
                  pzArg));
    /* NOTREACHED */
    return NULL;
}


/*=directive ident
 *
 *  dummy:  Ident directives are ignored.
=*/


/*=directive if
 *
 *  arg:  [ <ignored conditional expression> ]
 *
 *  text:
 *  @code{#if} expressions are not analyzed.  @strong{Everything} from here
 *  to the matching @code{#endif} is skipped.
=*/
static char*
doDir_if(char* pzArg, char* pzScan)
{
    return skipToEndif(pzScan);
}


/*=directive ifdef
 *
 *  arg:  name-to-test
 *
 *  text:
 *  The definitions that follow, up to the matching @code{#endif} will be
 *  processed only if there is a corresponding @code{-Dname} command line
 *  option or if a @code{#define} of that name has been previously encountered.
=*/
static char*
doDir_ifdef(char* pzArg, char* pzScan)
{
    if (getDefine(pzArg, AG_FALSE) == NULL)
        return skipToElseEnd(pzScan);
    ifdefLevel++;
    return pzScan;
}


/*=directive ifndef
 *
 *  arg:  name-to-test
 *
 *  text:
 *  The definitions that follow, up to the matching @code{#endif} will be
 *  processed only if there is @strong{not} a corresponding @code{-Dname}
 *  command line option or there was a canceling @code{-Uname} option.
=*/
static char*
doDir_ifndef(char* pzArg, char* pzScan)
{
    if (getDefine(pzArg, AG_FALSE) != NULL)
        return skipToElseEnd(pzScan);
    ifdefLevel++;
    return pzScan;
}


/*=directive include
 *
 *  arg:  unadorned-file-name
 *
 *  text:
 *  This directive will insert definitions from another file into
 *  the current collection.  If the file name is adorned with
 *  double quotes or angle brackets (as in a C program), then the
 *  include is ignored.
=*/
static char*
doDir_include(char* pzArg, char* pzScan)
{
    static char const * const apzSfx[] = { DIRECT_INC_DEF_SFX, NULL };
    tScanCtx*  pCtx;
    size_t     inclSize;
    char       zFullName[ AG_PATH_MAX + 1 ];

    /*
     *  Ignore C-style includes.  This allows "C" files to be processed
     *  for their "#define"s.
     */
    if ((*pzArg == '"') || (*pzArg == '<'))
        return pzScan;
    pCurCtx->pzScan  = pzScan;

    if (! SUCCESSFUL(
            findFile(pzArg, zFullName, apzSfx, pCurCtx->pzCtxFname))) {
        fprintf(pfTrace, DIRECT_INC_CANNOT_FIND,
                pzArg);
        return pzScan;
    }

    /*
     *  Make sure the specified file is a regular file and we can get
     *  the correct size for it.
     */
    {
        struct stat stbf;
        if (stat(zFullName, &stbf) != 0) {
            fprintf(pfTrace, DIRECT_INC_CANNOT_STAT, errno, strerror(errno),
                    zFullName);
            return pzScan;
        }
        if (! S_ISREG(stbf.st_mode)) {
            fprintf(pfTrace, DIRECT_INC_NOT_REG, zFullName);
            return pzScan;
        }
        inclSize = stbf.st_size;
        if (outTime <= stbf.st_mtime)
            outTime = stbf.st_mtime + 1;
    }

    if (inclSize == 0)
        return pzScan;

    /*
     *  Get the space for the output data and for context overhead.
     *  This is an extra allocation and copy, but easier than rewriting
     *  'loadData()' for this special context.
     */
    {
        size_t sz = sizeof(tScanCtx) + 4 + inclSize;
        pCtx = (tScanCtx*)AGALOC(sz, "inc def head");

        memset((void*)pCtx, 0, sz);
        pCtx->lineNo = 1;
    }

    /*
     *  Link it into the context stack
     */
    pCtx->pCtx       = pCurCtx;
    pCurCtx          = pCtx;
    AGDUPSTR(pCtx->pzCtxFname, zFullName, "def file");

    pCtx->pzScan     =
    pCtx->pzData     =
    pzScan           = (char*)(pCtx + 1);

    /*
     *  Read all the data.  Usually in a single read, but loop
     *  in case multiple passes are required.
     */
    {
        FILE*  fp = fopen(zFullName, "r" FOPEN_TEXT_FLAG);
        char*  pz = pzScan;

        if (fp == NULL)
            AG_CANT(DIRECT_INC_CANNOT_OPEN, zFullName);

        if (pfDepends != NULL)
            add_source_file(zFullName);

        do  {
            size_t rdct = fread((void*)pz, (size_t)1, inclSize, fp);

            if (rdct == 0)
                AG_CANT(DIRECT_INC_CANNOT_READ, zFullName);

            pz += rdct;
            inclSize -= rdct;
        } while (inclSize > 0);

        fclose(fp);
        *pz = NUL;
    }

    return pzScan;
}


/*=directive let
 *
 *  dummy:  let directives are ignored.
=*/


/*=directive line
 *
 *  text:
 *
 *  Alters the current line number and/or file name.  You may wish to
 *  use this directive if you extract definition source from other files.
 *  @command{getdefs} uses this mechanism so AutoGen will report the correct
 *  file and approximate line number of any errors found in extracted
 *  definitions.
=*/
static char*
doDir_line(char* pzArg, char* pzScan)
{
    /*
     *  The sequence must be:  #line <number> "file-name-string"
     *
     *  Start by scanning up to and extracting the line number.
     */
    pzArg = SPN_WHITESPACE_CHARS(pzArg);
    if (! IS_DEC_DIGIT_CHAR(*pzArg))
        return pzScan;

    pCurCtx->lineNo = strtol(pzArg, &pzArg, 0);

    /*
     *  Now extract the quoted file name string.
     *  We dup the string so it won't disappear on us.
     */
    pzArg = SPN_WHITESPACE_CHARS(pzArg);
    if (*(pzArg++) != '"')
        return pzScan;
    {
        char* pz = strchr(pzArg, '"');
        if (pz == NULL)
            return pzScan;
        *pz = NUL;
    }

    AGDUPSTR(pCurCtx->pzCtxFname, pzArg, "#line");

    return pzScan;
}


/*=directive macdef
 *
 *  text:
 *  This is a new AT&T research preprocessing directive.  Basically, it is
 *  a multi-line #define that may include other preprocessing directives.
=*/
static char*
doDir_macdef(char* pzArg, char* pzScan)
{
    return skipToEndmac(pzScan);
}


/*=directive option
 *
 *  arg:  opt-name [ <text> ]
 *
 *  text:
 *
 *  This directive will pass the option name and associated text to the
 *  AutoOpts optionLoadLine routine (@pxref{libopts-optionLoadLine}).  The
 *  option text may span multiple lines by continuing them with a backslash.
 *  The backslash/newline pair will be replaced with two space characters.
 *  This directive may be used to set a search path for locating template files
 *  For example, this:
 *
 *  @example
 *    #option templ-dirs $ENVVAR/dirname
 *  @end example
 *  @noindent
 *  will direct autogen to use the @code{ENVVAR} environment variable to find
 *  a directory named @code{dirname} that (may) contain templates.  Since these
 *  directories are searched in most recently supplied first order, search
 *  directories supplied in this way will be searched before any supplied on
 *  the command line.
=*/
static char*
doDir_option(char* pzArg, char* pzScan)
{
    optionLoadLine(&autogenOptions, pzArg);
    return pzScan;
}


/*=directive pragma
 *
 *  dummy:  pragma directives are ignored.
=*/


/*=directive shell
 *
 *  text:
 *  Invokes @code{$SHELL} or @file{/bin/sh} on a script that should
 *  generate AutoGen definitions.  It does this using the same server
 *  process that handles the back-quoted @code{`} text.
 *  @strong{CAUTION}@:  let not your @code{$SHELL} be @code{csh}.
=*/
static char*
doDir_shell(char* pzArg, char* pzScan)
{
    static size_t const endshell_len = sizeof("\n#endshell") - 1;

    tScanCtx*  pCtx;
    char*      pzText = pzScan;

    /*
     *  The output time will always be the current time.
     *  The dynamic content is always current :)
     */
    outTime = time(NULL);

    /*
     *  IF there are no data after the '#shell' directive,
     *  THEN we won't write any data
     *  ELSE we have to find the end of the data.
     */
    if (strncmp(pzText, DIRECT_SHELL_END_SHELL+1, endshell_len-1) == 0)
        return pzScan;

    {
        char* pz = strstr(pzScan, DIRECT_SHELL_END_SHELL);
        if (pz == NULL)
            AG_ABEND(aprf(DIRECT_SHELL_NOEND, pCurCtx->pzCtxFname,
                          pCurCtx->lineNo));

        while (pzScan < pz) {
            if (*(pzScan++) == NL) pCurCtx->lineNo++;
        }

        *pzScan = NUL;
    }

    /*
     *  Advance the scan pointer to the next line after '#endshell'
     *  IF there is no such line,
     *  THEN the scan will resume on a zero-length string.
     */
    pzScan = strchr(pzScan + endshell_len, NL);
    if (pzScan == NULL)
        pzScan = (void*)zNil;

    /*
     *  Save the scan pointer into the current context
     */
    pCurCtx->pzScan  = pzScan;

    /*
     *  Run the shell command.  The output text becomes the
     *  "file text" that is used for more definitions.
     */
    pzText = shell_cmd(pzText);
    if (pzText == NULL)
        return pzScan;

    if (*pzText == NUL) {
        AGFREE(pzText);
        return pzScan;
    }

    /*
     *  Get the space for the output data and for context overhead.
     *  This is an extra allocation and copy, but easier than rewriting
     *  'loadData()' for this special context.
     */
    pCtx = (tScanCtx*)AGALOC(sizeof(tScanCtx) + strlen(pzText) + 4,
                             "shell output");

    /*
     *  Link the new scan data into the context stack
     */
    pCtx->pCtx  = pCurCtx;
    pCurCtx     = pCtx;

    /*
     *  Set up the rest of the context structure
     */
    AGDUPSTR(pCtx->pzCtxFname, DIRECT_SHELL_COMP_DEFS, DIRECT_SHELL_COMP_DEFS);
    pCtx->pzScan     =
    pCtx->pzData     = (char*)(pCtx+1);
    pCtx->lineNo     = 0;
    strcpy(pCtx->pzScan, pzText);
    AGFREE(pzText);

    return pCtx->pzScan;
}


/*=directive undef
 *
 *  arg:  name-to-undefine
 *
 *  text:
 *  Will remove any entries from the define list
 *  that match the undef name pattern.
=*/
static char*
doDir_undef(char* pzArg, char* pzScan)
{
    SET_OPT_UNDEFINE(pzArg);
    return pzScan;
}


/*+++ End of Directives +++*/
/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * indent-tabs-mode: nil
 * End:
 * end of agen5/defDirect.c */
