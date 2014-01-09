/**
 * \file getdefs.c
 *
 *  getdefs Copyright (c) 1999-2012 by Bruce Korb - all rights reserved
 *
 *  Time-stamp:        "2011-01-31 13:06:12 bkorb"
 *  Author:            Bruce Korb <bkorb@gnu.org>
 *
 *  This file is part of AutoGen.
 *  AutoGen copyright (c) 1992-2012 by Bruce Korb - all rights reserved
 *
 *  AutoGen is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by the
 *  Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  AutoGen is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

static char const zBogusDef[] = "Bogus definition:\n%s\n";

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *  Forward procedure pointers
 */
typedef int (compar_func)(const void *, const void *);
static compar_func compar_text, compar_defname;

/* START-STATIC-FORWARD */
static char*
assignIndex(char*  pzOut,  char*  pzDef);

static int
awaitAutogen(void);

static void
buildDefinition(char * pzDef, char const * pzFile, int line, char * pzOut);

static tSuccess
buildPreamble(char ** ppzDef, char ** ppzOut, char const * pzFile, int line);

static int
compar_defname(const void* p1, const void* p2);

static int
compar_text(const void* p1, const void* p2);

static void
doPreamble(FILE* outFp);

static void
printEntries(FILE* fp);

static void
processFile(char const * pzFile);

static void
set_first_idx(void);

static FILE*
open_ag_file(char ** pzBase);

static FILE*
open_ag_proc_pipe(char ** pzBase);

static void
exec_autogen(char ** pzBase);

static FILE*
startAutogen(void);

static void
updateDatabase(void);
/* END-STATIC-FORWARD */

#ifndef HAVE_STRSIGNAL
#  include "compat/strsignal.c"
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *   MAIN
 */
int
main(int argc, char ** argv)
{
    FILE* outFp;

    optionProcess(&getdefsOptions, argc, argv);
    validateOptions();

    outFp = startAutogen();

    doPreamble(outFp);

    /*
     *  Process each input file
     */
    {
        int    ct  = STACKCT_OPT(INPUT);
        char const ** ppz = STACKLST_OPT(INPUT);

        do  {
            processFile(*ppz++);
        } while (--ct > 0);
    }

    /*
     *  IF we don't have an ordering file, but we do have a "first index",
     *  THEN alphabetize by definition name.
     */
    if ((pzIndexText == NULL) && HAVE_OPT(FIRST_INDEX)) {
        qsort((void*)papzBlocks, blkUseCt, sizeof(char*), compar_defname);
        set_first_idx();
    }

    else if (ENABLED_OPT(ORDERING) && (blkUseCt > 1))
        qsort((void*)papzBlocks, blkUseCt, sizeof(char*), &compar_text);

    printEntries(outFp);
    fclose(outFp);

    /*
     *  IF output is to a file
     *  THEN set the permissions and modification times
     */
    if (  (WHICH_IDX_AUTOGEN == INDEX_OPT_OUTPUT)
       && (outFp != stdout) )  {
        struct utimbuf tbuf;
        tbuf.actime  = time((time_t*)NULL);
        tbuf.modtime = modtime + 1;
        utime(OPT_ARG(OUTPUT), &tbuf);
        chmod(OPT_ARG(OUTPUT), S_IRUSR|S_IRGRP|S_IROTH);
    }

    /*
     *  IF we are keeping a database of indexes
     *     AND we have augmented the contents,
     *  THEN append the new entries to the file.
     */
    if ((pzIndexText != NULL) && (pzEndIndex != pzIndexEOF))
        updateDatabase();

    if (agPid != -1)
        return awaitAutogen();

    return EXIT_SUCCESS;
}


/*
 *  assignIndex
 */
static char*
assignIndex(char*  pzOut,  char*  pzDef)
{
    char*  pzMatch;
    size_t len = strlen(pzDef);
    int    idx;

    /*
     *  Make the source text all lower case and map
     *  '-', '^' and '_' characters to '_'.
     */
    strtransform(pzDef, pzDef);

    /*
     * IF there is already an entry,
     * THEN put the index into the output.
     */
    pzMatch = strstr(pzIndexText, pzDef);
    if (pzMatch != NULL) {
        pzMatch += len;
        while (isspace(*pzMatch)) pzMatch++;
        while ((*pzOut++ = *pzMatch++) != ']') ;
        return pzOut;
    }

    /*
     *  We have a new entry.  Make sure we have room for it
     *  in our in-memory string
     */
    if (((pzEndIndex - pzIndexText) + len + 64 ) > indexAlloc) {
        char* pz;
        indexAlloc +=  0x1FFF;
        indexAlloc &= ~0x0FFF;
        pz = (char*)realloc((void*)pzIndexText, indexAlloc);
        if (pz == NULL) {
            fputs("Realloc of index text failed\n", stderr);
            exit(EXIT_FAILURE);
        }

        /*
         *  IF the allocation moved,
         *  THEN adjust all our pointers.
         */
        if (pz != pzIndexText) {
            pzIndexEOF  = pz + (pzIndexEOF - pzIndexText);
            pzEndIndex  = pz + (pzEndIndex - pzIndexText);
            pzIndexText = pz;
        }
    }

    /*
     *  IF there are no data in our text database,
     *  THEN use default index.
     */
    if (pzEndIndex == pzIndexText)
        idx = OPT_VALUE_FIRST_INDEX;
    else do {
        char* pz = strrchr(pzDef, ' ');
        *pz = NUL;
        len = strlen(pzDef);

        /*
         *  Find the last entry for the current category of entries
         */
        pzMatch = strstr(pzIndexText, pzDef);
        if (pzMatch == NULL) {
            /*
             *  No entries for this category.  Use default index.
             */
            idx = OPT_VALUE_FIRST_INDEX;
            *pz = ' ';
            break;
        }

        for (;;) {
            char* pzn = strstr(pzMatch + len, pzDef);
            if (pzn == NULL)
                break;
            pzMatch = pzn;
        }

        /*
         *  Skip forward to the '[' character and convert the
         *  number that follows to a long.
         */
        *pz = ' ';
        pzMatch = strchr(pzMatch + len, '[');
        idx = strtol(pzMatch+1, (char**)NULL, 0)+1;
    } while (0);

    /*
     *  Add the new entry to our text database and
     *  place a copy of the value into our output.
     */
    pzEndIndex += sprintf(pzEndIndex, "%-40s  [%d]\n", pzDef, idx);
    pzOut += sprintf(pzOut, "[%d]", idx);

    return pzOut;
}


/*
 *  awaitAutogen
 */
static int
awaitAutogen(void)
{
    int  status;
    waitpid(agPid, &status, 0);
    if (WIFEXITED(status)) {
        status = WEXITSTATUS(status);
        if (status != EXIT_SUCCESS) {
            fprintf(stderr, "ERROR:  %s exited with status %d\n",
                    pzAutogen, status);
        }
        return status;
    }

    if (WIFSIGNALED( status )) {
        status = WTERMSIG( status );
        fprintf(stderr, "ERROR:  %s exited due to %d signal (%s)\n",
                pzAutogen, status, strsignal(status));
    }
    else
        fprintf(stderr, "ERROR:  %s exited due to unknown reason %d\n",
                pzAutogen, status);

    return EXIT_FAILURE;
}


/*
 *  buildDefinition
 */
static void
buildDefinition(char * pzDef, char const * pzFile, int line, char * pzOut)
{
    static char const zSrcFile[] = "    %s = '%s';\n";
    static char const zLineNum[] = "    %s = '%d';\n";

    ag_bool    these_are_global_defs;
    tSuccess   preamble;
    int        re_res;
    char*      pzNextDef = NULL;
    regmatch_t match[2];

    if (*pzDef == '*') {
        these_are_global_defs = AG_TRUE;
        strcpy(pzOut, zGlobal);
        pzOut += sizeof(zGlobal)-1;
        pzOut += sprintf(pzOut, zLineId, line, pzFile);

        pzDef = strchr(pzDef, '\n');
        preamble = PROBLEM;

    } else {
        these_are_global_defs = AG_FALSE;
        preamble = buildPreamble(&pzDef, &pzOut, pzFile, line);
        if (FAILED(preamble)) {
            *pzOut = NUL;
            return;
        }
    }

    /*
     *  FOR each attribute for this entry, ...
     */
    for (;;) {
        /*
         *  Find the next attribute regular expression
         */
        re_res = regexec(&attrib_re, pzDef, COUNT(match), match, 0);
        switch (re_res) {
        case 0:
            /*
             *  NUL-terminate the current attribute.
             *  Set the "next" pointer to the start of the next attribute name.
             */
            pzDef[ match[0].rm_so ] = NUL;
            if (pzNextDef != NULL)
                pzOut = emitDefinition(pzNextDef, pzOut);
            pzNextDef = pzDef = pzDef + match[1].rm_so;
            break;

        case REG_NOMATCH:
            /*
             *  No more attributes.
             */
            if (pzNextDef == NULL) {
                *pzOut++ = '\n'; *pzOut++ = '#';
                sprintf(pzOut,  zNoData, pzFile, line);
                fputs(pzOut, stderr);
                pzOut += strlen(pzOut);
                return;
            }

            pzOut = emitDefinition(pzNextDef, pzOut);
            goto eachAttrDone;
            break;

        default:
        {
            char zRER[ MAXNAMELEN ];
            static char const zErr[] = "error %d (%s) finding `%s' in\n%s\n\n";
            regerror(re_res, &attrib_re, zRER, sizeof(zRER));
            *pzOut++ = '\n';
            *pzOut++ = '#';
            sprintf(pzOut, zErr, re_res, zRER, zAttribRe, pzDef);
            fprintf(stderr, "getdefs:  %s", zErr);
            return;
        }
        }
    } eachAttrDone:;

    if (these_are_global_defs) {
        *pzOut = NUL;
        return;
    }

    if (HAVE_OPT(COMMON_ASSIGN)) {
        int    ct  = STACKCT_OPT(COMMON_ASSIGN);
        char const ** ppz = STACKLST_OPT(COMMON_ASSIGN);
        do  {
            pzOut += sprintf(pzOut, "    %s;\n", *ppz++);
        } while (--ct > 0);
    }

    if (HAVE_OPT(SRCFILE))
        pzOut += sprintf(pzOut, zSrcFile, OPT_ARG(SRCFILE), pzFile);

    if (HAVE_OPT(LINENUM))
        pzOut += sprintf(pzOut, zLineNum, OPT_ARG(LINENUM), line);

    /*
     *  IF the preamble had a problem, it is because it could not
     *  emit the final "#endif\n" directive.  Do that now.
     */
    if (HADGLITCH(preamble))
         strcpy(pzOut, "};\n#endif\n");
    else strcpy(pzOut, "};\n");
}


/*
 *  buildPreamble
 */
static tSuccess
buildPreamble(char ** ppzDef, char ** ppzOut, char const * pzFile, int line)
{
    char * pzDef      = *ppzDef;
    char * pzOut      = *ppzOut;

    char   zDefText[ MAXNAMELEN ];
    char * pzDefText  = zDefText;
    char   zNameText[ MAXNAMELEN ];
    char * pzNameText = zNameText;
    char * pzIfText   = NULL;

    /*
     *  Copy out the name of the entry type
     */
    *pzDefText++ = '`';
    while (isalnum(*pzDef) || (*pzDef == '_') || (*pzDef == '.')
          || (*pzDef == '[') || (*pzDef == ']'))
        *pzDefText++ = *pzDef++;

    *pzDefText = NUL;

    pzDef += strspn(pzDef, "* \t");

    /*
     *  Copy out the name for this entry of the above entry type.
     */
    while (isalnum(*pzDef) || (*pzDef == '_'))
        *pzNameText++ = *pzDef++;
    *pzNameText = NUL;

    if (  (zDefText[1]  == NUL)
       || (zNameText[0] == NUL) )  {
        fprintf(stderr, zNoData, pzFile, line);
        return FAILURE;
    }

    pzDef += strspn(pzDef, " \t");

    /*
     *  IF these names are followed by a comma and an "if" clause,
     *  THEN we emit the definition with "#if..."/"#endif" around it
     */
    if (*pzDef == ',') {
        pzDef += strspn(pzDef+1, " \t")+1;
        if ((pzDef[0] == 'i') && (pzDef[1] == 'f'))
            pzIfText = pzDef;
    }

    pzDef = strchr(pzDef, '\n');
    if (pzDef == NULL) {
        fprintf(stderr, zNoData, pzFile, line);
        return FAILURE;
    }

    *pzDef = NUL;

    /*
     *  Now start the output.  First, the "#line" directive,
     *  then any "#ifdef..." line and finally put the
     *  entry type name into the output.
     */
    pzOut += sprintf(pzOut, zLineId, line, pzFile);
    if (pzIfText != NULL)
        pzOut += sprintf(pzOut, "#%s\n", pzIfText);
    {
        char*  pz = zDefText+1;
        while (*pz != NUL)
            *pzOut++ = *pz++;
    }

    /*
     *  IF we are indexing the entries,
     *  THEN build the string by which we are indexing
     *       and insert the index into the output.
     */
    if (pzIndexText != NULL) {
        sprintf(pzDefText, "  %s'", zNameText);
        pzOut = assignIndex(pzOut, zDefText);
    }

    /*
     *  Now insert the name with a consistent name string prefix
     *  that we use to locate the sort key later.
     */
    pzOut  += sprintf(pzOut, "%s%s';\n", zNameTag, zNameText);
    *ppzOut = pzOut;
    *ppzDef = pzDef;
    *pzDef  = '\n';  /* restore the newline.  Used in pattern match */

    /*
     *  Returning "PROBLEM" means the caller must emit the "#endif\n"
     *  at the end of the definition.
     */
    return (pzIfText != NULL) ? PROBLEM : SUCCESS;
}


/*
 *  compar_defname
 */
static int
compar_defname(const void* p1, const void* p2)
{
    char const * pzS1 = *(char const * const *)p1;
    char const * pz1  = strstr(pzS1, zNameTag);
    char const * pzS2 = *(char const * const *)p2;
    char const * pz2  = strstr(pzS2, zNameTag);

    if (pz1 == NULL) {
        if (strncmp(*(char const * const *)p1, zGlobal, sizeof(zGlobal)-1) == 0)
            return -1;

        die(zBogusDef, *(char const * const *)p1);
    }

    if (pz2 == NULL) {
        if (strncmp(*(char const * const *)p2, zGlobal, sizeof(zGlobal)-1) == 0)
            return 1;

        die(zBogusDef, *(char const * const *)p2);
    }

    /*
     *  Back up to the name of the definition
     */
    while  ((pz1 > pzS1) && (*--pz1 != '\n'))  ;
    while  ((pz2 > pzS2) && (*--pz2 != '\n'))  ;

    return strcmp(pz1, pz2);
}


/*
 *  compar_text
 *
 *  merely returns the relative ordering of two input strings.
 *  The arguments are pointers to pointers to NUL-terminated strings.
 *  IF the definiton was mal-formed, an error message was printed
 *  earlier.  When we get here, we wil fail to find the "zNameTag"
 *  string and EXIT_FAILURE.
 */
static int
compar_text(const void* p1, const void* p2)
{
    char* pz1 = strstr(*(char const * const *)p1, zNameTag);
    char* pe1;
    char* pz2 = strstr(*(char const * const *)p2, zNameTag);
    char* pe2;
    int   res;

    if (pz1 == NULL) {
        if (strncmp(*(char const * const *)p1, zGlobal, sizeof(zGlobal)-1) == 0)
            return -1;

        die(zBogusDef, *(char const * const *)p1);
    }

    if (pz2 == NULL) {
        if (strncmp(*(char const * const *)p2, zGlobal, sizeof(zGlobal)-1) == 0)
            return 1;

        die(zBogusDef, *(char const * const *)p2);
    }

    pz1 += sizeof(zNameTag)-1;
    pe1 = strchr(pz1, '\'');

    if (pe1 == NULL)
        die(zBogusDef, *(char const * const *)p1);

    pz2 += sizeof(zNameTag)-1;
    pe2 = strchr(pz2, '\'');

    if (pe2 == NULL)
        die(zBogusDef, *(char const * const *)p2);

    *pe1 = *pe2 = NUL;

    /*
     *  We know ordering is enabled because we only get called when
     *  it is enabled.  If the option was also specified, then
     *  we sort without case sensitivity (and we compare '-', '_'
     *  and '^' as being equal as well).  Otherwise, we do a
     *  strict string comparison.
     */
    if (HAVE_OPT(ORDERING))
         res = streqvcmp(pz1, pz2);
    else res = strcmp(pz1, pz2);
    *pe1 = *pe2 = '\'';
    return res;
}


/*
 *  doPreamble
 */
static void
doPreamble(FILE* outFp)
{
    /*
     *  Emit the "autogen definitions xxx;" line
     */
    fprintf(outFp, zAgDef, OPT_ARG(TEMPLATE));

    if (HAVE_OPT(FILELIST)) {
        static char const zFmt[] = "%-12s = '%s';\n";
        char const * pzName = OPT_ARG(FILELIST);

        if ((pzName == NULL) || (*pzName == NUL))
            pzName = "infile";

        if (HAVE_OPT(INPUT)) {
            int    ct  = STACKCT_OPT(INPUT);
            char const ** ppz = STACKLST_OPT(INPUT);

            do  {
                fprintf(outFp, zFmt, pzName, *ppz++);
            } while (--ct > 0);
        }

        if (HAVE_OPT(COPY)) {
            int    ct  = STACKCT_OPT(COPY);
            char const ** ppz = STACKLST_OPT(COPY);

            do  {
                fprintf(outFp, zFmt, pzName, *ppz++);
            } while (--ct > 0);
        }
        fputc('\n', outFp);
    }

    /*
     *  IF there are COPY files to be included,
     *  THEN emit the '#include' directives
     */
    if (HAVE_OPT(COPY)) {
        int    ct  = STACKCT_OPT(COPY);
        char const ** ppz = STACKLST_OPT(COPY);
        do  {
            fprintf(outFp, "#include %s\n", *ppz++);
        } while (--ct > 0);
        fputc('\n', outFp);
    }

    /*
     *  IF there are global assignments, then emit them
     *  (these do not get sorted, so we write directly now.)
     */
    if (HAVE_OPT(ASSIGN)) {
        int    ct  = STACKCT_OPT(ASSIGN);
        char const ** ppz = STACKLST_OPT(ASSIGN);
        do  {
            fprintf(outFp, "%s;\n", *ppz++);
        } while (--ct > 0);
        fputc('\n', outFp);
    }
}


/*
 *  loadFile
 */
LOCAL char*
loadFile(char const * pzFname)
{
    FILE*  fp = fopen(pzFname, "r" FOPEN_BINARY_FLAG);
    int    res;
    char*  pzText;
    char*  pzRead;
    size_t rdsz;

    if (fp == (FILE*)NULL)
        return NULL;
    /*
     *  Find out how much data we need to read.
     *  And make sure we are reading a regular file.
     */
    {
        struct stat stb;
        res = fstat(fileno(fp), &stb);
        if (res != 0)
            fserr_die("stat-ing %s\n", pzFname);

        if (! S_ISREG(stb.st_mode)) {
            fprintf(stderr, "error file %s is not a regular file\n",
                    pzFname);
            exit(EXIT_FAILURE);
        }
        rdsz = stb.st_size;
        if (rdsz < 16)
            die("Error file %s only contains %d bytes.\n"
                "\tit cannot contain autogen definitions\n",
                pzFname, (int)rdsz);
    }

    /*
     *  Allocate the space we need for the ENTIRE file.
     */
    pzRead = pzText = (char*)malloc(rdsz + 1);
    if (pzText == NULL)
        die("Error: could not allocate %d bytes\n", (int)rdsz + 1);

    /*
     *  Read as much as we can get until we have read the file.
     */
    do  {
        size_t rdct = fread((void*)pzRead, (size_t)1, rdsz, fp);

        if (rdct == 0)
            fserr_die("reading file %s\n", pzFname);

        pzRead += rdct;
        rdsz   -= rdct;
    } while (rdsz > 0);

    *pzRead = NUL;
    fclose(fp);
    return pzText;
}


/*
 *  printEntries
 */
static void
printEntries(FILE* fp)
{
    int     ct  = blkUseCt;
    char**  ppz = papzBlocks;

    if (ct == 0)
        exit(EXIT_FAILURE);

    for (;;) {
        char* pz = *(ppz++);
        fputs(pz, fp);
        free((void*)pz);
        if (--ct <= 0)
            break;
        fputc('\n', fp);
    }
    free((void*)papzBlocks);
}


/*
 *  processFile
 */
static void
processFile(char const * pzFile)
{
    char* pzText = loadFile(pzFile); /* full text */
    char* pzScan;  /* Scanning Pointer  */
    char* pzDef;   /* Def block start   */
    char* pzNext;  /* start next search */
    char* pzDta;   /* data value        */
    int   lineNo = 1;
    char* pzOut;
    regmatch_t  matches[MAX_SUBMATCH+1];

    if (pzText == NULL)
        fserr_die("read opening %s\n", pzFile);

    processEmbeddedOptions(pzText);
    pzNext = pzText;

    while (pzScan = pzNext,
           regexec(&define_re, pzScan, COUNT(matches), matches, 0) == 0) {

        static char const zNoEnd[] =
            "Error:  definition in %s at line %d has no end\n";
        static char const zNoSubexp[] =
            "Warning: entry type not found on line %d in %s:\n\t%s\n";

        int  linesInDef = 0;

        /*
         *  Make sure there is a subexpression match!!
         */
        if (matches[1].rm_so == -1) {
            char* pz = NULL;
            char  ch = NUL;

            pzDef = pzScan + matches[0].rm_so;
            if (strlen(pzDef) > 30) {
                pz  = pzDef + 30;
                ch  = *pz;
                *pz = NUL;
            }

            fprintf(stderr, zNoSubexp, lineNo, pzFile, pzDef);
            if (pz != NULL)
                *pz = ch;
            continue;
        }

        pzDef = pzScan + matches[0].rm_so + sizeof("/*=") - 1;
        pzNext = strstr(pzDef, "=*/");
        if (pzNext == NULL)
            die(zNoEnd, pzFile, lineNo);

        *pzNext = NUL;
        pzNext += 3;
        /*
         *  Count the number of lines skipped to the start of the def.
         */
        for (;;) {
            pzScan = strchr(pzScan, '\n');
            if (pzScan++ == NULL)
                break;
            if (pzScan >= pzDef)
                break;
            lineNo++;
        }

        pzOut = pzDta = (char*)malloc(2 * strlen(pzDef) + 8000);

        /*
         *  Count the number of lines in the definition itself.
         *  It will find and stop on the "=* /\n" line.
         */
        pzScan = pzDef;
        for (;;) {
            pzScan = strchr(pzScan, '\n');
            if (pzScan++ == NULL)
                break;
            linesInDef++;
        }

        /*
         *  OK.  We are done figuring out where the boundaries of the
         *  definition are and where we will resume our processing.
         */
        buildDefinition(pzDef, pzFile, lineNo, pzOut);
        pzDta   = (char*)realloc((void*)pzDta, strlen(pzDta) + 1);
        lineNo += linesInDef;

        if (++blkUseCt > blkAllocCt) {
            blkAllocCt += 32;
            papzBlocks = (char**)realloc((void*)papzBlocks,
                                         blkAllocCt * sizeof(char*));
            if (papzBlocks == (char**)NULL)
                die("Realloc error for %d pointers\n", (int)blkAllocCt);
        }
        papzBlocks[ blkUseCt-1 ] = pzDta;
    }

    free((void*)pzText);
}


/*
 *  set_first_idx
 *
 *  Go through all our different kinds of defines.  On the first occurrence
 *  of each different name, check for an index value.  If not supplied,
 *  then insert ``[OPT_VALUE_FIRST_INDEX]'' after the object name.
 */
static void
set_first_idx(void)
{
    char    zNm[ 128 ] = { NUL };
    int     nmLn = 1;
    int     ct   = blkUseCt;
    char**  ppz  = papzBlocks;

    if (ct == 0)
        exit(EXIT_FAILURE);

    for (; --ct >= 0; ppz++) {
        char *  pzOld = *ppz;
        int     changed = (strneqvcmp(pzOld, zNm, nmLn) != 0);
        char *  pzNew;

        /*
         *  IF the name still matches, then check the following character.
         *  If it is whitespace or an open bracket, then
         *  it's the old type.  Continue to the next entry.
         */
        if (! changed) {
            if (isspace(pzOld[ nmLn ]) || (pzOld[nmLn] == '['))
                continue;
        }

        pzNew = zNm;
        nmLn  = 0;
        while (isalnum(*pzOld)
               || (*pzOld == '_') || (*pzOld == '-') || (*pzOld == '^')) {
            nmLn++;
            *(pzNew++) = *(pzOld++);
        }
        *pzNew = NUL;

        /*
         *  IF the source has specified its own index, then do not
         *  supply our own new one.
         */
        if (*pzOld != '[') {
            pzNew = (char*)malloc(strlen(pzOld) + nmLn + 10);
            sprintf(pzNew, "%s[%d]%s", zNm,
                    (int)OPT_VALUE_FIRST_INDEX, pzOld);
            free((void*)(*ppz));
            *ppz = pzNew;
        }
    }
}

static FILE*
open_ag_file(char ** pzBase)
{
    switch (WHICH_IDX_AUTOGEN) {
    case INDEX_OPT_OUTPUT:
    {
        static char const zFileFmt[] = " *      %s\n";
        FILE*  fp;

        if (*pzBase != NULL)
            free(*pzBase);

        if (strcmp(OPT_ARG(OUTPUT), "-") == 0)
            return stdout;

        unlink(OPT_ARG(OUTPUT));
        fp = fopen(OPT_ARG(OUTPUT), "w" FOPEN_BINARY_FLAG);
        fprintf(fp, zDne, OPT_ARG(OUTPUT));

        if (HAVE_OPT(INPUT)) {
            int    ct  = STACKCT_OPT(INPUT);
            char const ** ppz = STACKLST_OPT(INPUT);
            do  {
                fprintf(fp, zFileFmt, *ppz++);
            } while (--ct > 0);
        }

        fputs(" */\n", fp);
        return fp;
    }

    case INDEX_OPT_AUTOGEN:
        if (! ENABLED_OPT(AUTOGEN)) {
            if (*pzBase != NULL)
                free(*pzBase);

            return stdout;
        }

        if (  ( OPT_ARG(AUTOGEN) != NULL)
              && (*OPT_ARG(AUTOGEN) != NUL ))
            pzAutogen = OPT_ARG(AUTOGEN);

        break;
    }

    return NULL;
}

static FILE*
open_ag_proc_pipe(char ** pzBase)
{
    FILE * agFp;

    int  pfd[2];

    if (pipe(pfd) != 0)
        fserr_die("creating pipe\n");

    agPid = fork();

    switch (agPid) {
    case 0:
        /*
         *  We are the child.  Close the write end of the pipe
         *  and force STDIN to become the read end.
         */
        close(pfd[1]);
        if (dup2(pfd[0], STDIN_FILENO) != 0)
            fserr_die("dup pipe[0]\n");
        break;

    case -1:
        fserr_die("on fork()\n");

    default:
        /*
         *  We are the parent.  Close the read end of the pipe
         *  and get a FILE* pointer for the write file descriptor
         */
        close(pfd[0]);
        agFp = fdopen(pfd[1], "w" FOPEN_BINARY_FLAG);
        if (agFp == (FILE*)NULL)
            fserr_die("fdopening pipe[1]\n");
        free(*pzBase);
        return agFp;
    }

    return NULL;
}

static void
exec_autogen(char ** pzBase)
{
    char const ** paparg;
    char const ** pparg;
    int    argCt = 5;

    /*
     *  IF we don't have template search directories,
     *  THEN allocate the default arg counter of pointers and
     *       set the program name into it.
     *  ELSE insert each one into the arg list.
     */
    if (! HAVE_OPT(AGARG)) {
        paparg = pparg = (char const **)malloc(argCt * sizeof(char*));
        *pparg++ = pzAutogen;

    } else {
        int    ct  = STACKCT_OPT(AGARG);
        char const ** ppz = STACKLST_OPT(AGARG);

        argCt += ct;
        paparg = pparg = (char const **)malloc(argCt * sizeof(char*));
        *pparg++ = pzAutogen;

        do  {
            *pparg++ = *ppz++;
        } while (--ct > 0);
    }

    *pparg++ = *pzBase;
    *pparg++ = "--";
    *pparg++ = "-";
    *pparg++ = NULL;

#ifdef DEBUG
    fputc('\n', stderr);
    pparg = paparg;
    for (;;) {
        fputs(*pparg++, stderr);
        if (*pparg == NULL)
            break;
        fputc(' ', stderr);
    }
    fputc('\n', stderr);
    fputc('\n', stderr);
#endif

    execvp(pzAutogen, (char**)(void*)paparg);
    fserr_die("exec of %s %s %s %s\n", paparg[0], paparg[1], paparg[2],
              paparg[3]);
}

/*
 *  startAutogen
 */
static FILE*
startAutogen(void)
{
    char*  pz;
    FILE*  agFp;
    char*  pzBase = NULL;

    /*
     *  Compute the base name.
     *
     *  If an argument was specified, use that without question.
     *  IF a definition pattern is supplied, and it looks like
     *     a normal name, then use that.
     *  If neither of these work, then use the current directory name.
     */
    if (HAVE_OPT(BASE_NAME)) {
        pzBase = malloc(strlen(OPT_ARG(BASE_NAME)) + 3);
        strcpy(pzBase, "-b");
        strcpy(pzBase+2, OPT_ARG(BASE_NAME));
    }
    else {
        /*
         *  IF we have a definition name pattern,
         *  THEN copy the leading part that consists of name-like characters.
         */
        if (HAVE_OPT(DEFS_TO_GET)) {
            char const * pzS = OPT_ARG(DEFS_TO_GET);
            pzBase = malloc(strlen(pzS) + 3);
            strcpy(pzBase, "-b");

            pz = pzBase + 2;
            while (isalnum(*pzS) || (*pzS == '_'))
                *pz++ = *pzS++;
            if (pz == pzBase + 2) {
                free(pzBase);
                pzBase = NULL;
            }
            else
                *pz = NUL;
        }

        /*
         *  IF no pattern or it does not look like a name, ...
         */
        if (pzBase == NULL) {
            char zSrch[ MAXPATHLEN ];
            if (getcwd(zSrch, sizeof(zSrch)) == NULL)
                fserr_die("on getcwd\n");

            pz = strrchr(zSrch, '/');
            if (pz == NULL)
                 pz = zSrch;
            else pz++;
            pzBase = malloc(strlen(pz) + 3);
            strcpy(pzBase, "-b");
            strcpy(pzBase+2, pz);
        }
    }

    /*
     *  For our template name, we take the argument (if supplied).
     *  If not, then whatever we decided our base name was will also
     *  be our template name.
     */
    if (! HAVE_OPT(TEMPLATE))
        SET_OPT_TEMPLATE(strdup(pzBase+2));

    /*
     *  Now, what kind of output have we?
     *  If it is a file, open it up and return.
     *  If it is an alternate autogen program,
     *  then set it to whatever the argument said it was.
     *  If the option was not supplied, we default to
     *  whatever we set the "pzAutogen" pointer to above.
     */
    if (HAVE_OPT(AUTOGEN)) {
        agFp = open_ag_file(&pzBase);
        if (agFp != NULL)
            return agFp;
    }

    agFp = open_ag_proc_pipe(&pzBase);
    if (agFp != NULL)
        return agFp;

    exec_autogen(&pzBase);
    return (FILE*)NULL;
}


/*
 *  updateDatabase
 */
static void
updateDatabase(void)
{
    FILE* fp;

    if (chmod(OPT_ARG(ORDERING), 0666) == 0) {
        fp = fopen(OPT_ARG(ORDERING), "a" FOPEN_BINARY_FLAG);

    } else {
        unlink(OPT_ARG(ORDERING));
        fp = fopen(OPT_ARG(ORDERING), "w" FOPEN_BINARY_FLAG);
        pzIndexEOF = pzIndexText;
    }

    if (fp == (FILE*)NULL)
        fserr_die("opening %s for write/append\n", OPT_ARG(ORDERING));

    fwrite(pzIndexEOF, (size_t)(pzEndIndex - pzIndexEOF), (size_t)1, fp);
    fclose(fp);
    chmod(OPT_ARG(ORDERING), 0444);
}

/* emacs
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * indent-tabs-mode: nil
 * End:
 * end of getdefs/getdefs.c */
