
/**
 * @file tpDep.c
 *
 * Time-stamp:        "2011-12-30 17:38:15 bkorb"
 *
 *  This module will load a template and return a template structure.
 *
 * This file is part of AutoGen.
 * Copyright (c) 1992-2012 Bruce Korb - all rights reserved
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

typedef struct flist flist_t;
struct flist {
    flist_t *   next;
    char        fname[1];
};

static flist_t * src_flist  = NULL;
static flist_t * targ_flist = NULL;

/**
 *  Add a source file to the dependency list
 *
 * @param pz pointer to file name
 */
LOCAL void
add_source_file(char const * pz)
{
    flist_t ** lp;

    /*
     * If a source is also a target, then we've created it.
     * Do not list in source dependencies.
     */
    lp = &targ_flist;
    while (*lp != NULL) {
        if (strcmp(pz, (*lp)->fname) == 0)
            return;
        lp = &((*lp)->next);
    }

    /*
     * No check for duplicate in source list.  Add if not found.
     */
    lp = &src_flist;
    while (*lp != NULL) {
        if (strcmp(pz, (*lp)->fname) == 0)
            return;
        lp = &((*lp)->next);
    }

    {
        flist_t * p = AGALOC(sizeof(*p) + strlen(pz), "sfile");
        *lp = p;
        p->next = NULL;
        strcpy(p->fname, pz);
        if (OPT_VALUE_TRACE >= TRACE_SERVER_SHELL)
            fprintf(pfTrace, TRACE_ADD_SRC_FILE_FMT, p->fname);
    }
}

/**
 *  remove a source file from the dependency list
 *
 * @param pz pointer to file name
 */
LOCAL void
rm_source_file(char const * pz)
{
    flist_t ** pp = &src_flist; //!< point to where to stash removed "next"
    flist_t ** lp = &src_flist; //!< list scanning pointer

    for (;;) {
        if (*lp == NULL)
            return;
        if (strcmp(pz, (*lp)->fname) == 0)
            break;
        pp = lp;
        lp = &((*lp)->next);
    }
    {
        flist_t * p = *lp;
        *pp = p->next;
        if (OPT_VALUE_TRACE >= TRACE_SERVER_SHELL)
            fprintf(pfTrace, TRACE_RM_SRC_FILE_FMT, p->fname);
        AGFREE(p);
    }
}

/**
 *  Add a target file to the dependency list.  Avoid files in temp directory.
 *
 * @param pz pointer to file name
 */
LOCAL void
add_target_file(char const * pz)
{
    flist_t ** lp;

    /*
     *  Skip anything stashed in the temp directory.
     */
    if (  (pz_temp_tpl != NULL)
       && (strncmp(pz, pz_temp_tpl, temp_tpl_dir_len) == 0))
        return;

    /*
     *  Target files override sources, just in case.
     *  (We sometimes extract from files we are about to replace.)
     */
    rm_source_file(pz);

    /*
     *  avoid duplicates and add to end of list
     */
    lp = &targ_flist;
    while (*lp != NULL) {
        if (strcmp(pz, (*lp)->fname) == 0)
            return;
        lp = &((*lp)->next);
    }

    {
        flist_t * p = AGALOC(sizeof(*p) + strlen(pz), "sfile");
        *lp = p;
        p->next = NULL;
        strcpy(p->fname, pz);
        if (OPT_VALUE_TRACE >= TRACE_SERVER_SHELL)
            fprintf(pfTrace, TRACE_ADD_TARG_FILE_FMT, p->fname);
    }
}

/**
 *  Remove a target file from the dependency list
 *
 * @param pz pointer to file name
 */
LOCAL void
rm_target_file(char const * pz)
{
    flist_t ** lp = &targ_flist; //!< list scanning pointer

    for (;;) {
        if (*lp == NULL)
            return;
        if (strcmp(pz, (*lp)->fname) == 0)
            break;
        lp = &((*lp)->next);
    }
    {
        flist_t * p = *lp;
        *lp = p->next;
        if (OPT_VALUE_TRACE >= TRACE_SERVER_SHELL)
            fprintf(pfTrace, TRACE_RM_TARG_FILE_FMT, p->fname);
        AGFREE(p);
    }
}

/**
 * Create a dependency output file
 */
LOCAL void
start_dep_file(void)
{
    char * pz;

    if (pzDepFile == NULL)
        pzDepFile = aprf(START_DEP_TEMP_FMT, OPT_ARG(BASE_NAME));
    mkstemp((char *)pzDepFile);
    pfDepends = fopen(pzDepFile, "w");

    if (pfDepends == NULL)
        AG_CANT(START_DEP_FOPEN_MSG, pzDepFile);

    fprintf(pfDepends, START_DEP_FILE_FMT, autogenOptions.pzProgPath);

    {
        int     ac = autogenOptions.origArgCt;
        char ** av = autogenOptions.origArgVect;

        for (;;) {
            char * arg = *(av++);
            fprintf(pfDepends, START_DEP_ARG_FMT, arg);
            if (--ac == 0) break;
            fputs(DEP_FILE_SPLICE_STR, pfDepends);
        }
        fputs("\n", pfDepends);
    }

    if (pzDepTarget == NULL) {
        char * p;
        AGDUPSTR(pzDepTarget, pzDepFile, "targ name");
        p  = (char *)pzDepTarget + strlen(pzDepTarget) - 7;
        *p = NUL;
    }

    {
        char const * pnm = autogenOptions.pzPROGNAME;
        char const * bnm = strchr(pzDepTarget, '/');

        if (bnm != NULL)
            bnm++;
        else
            bnm = pzDepTarget;

        {
            size_t sz = strlen(pnm) + strlen(bnm) + 2; // underscore + NUL

            pz_targ_base = pz = AGALOC(sz, "targ list");
            sprintf(pz, DEP_FILE_TARG_FMT, pnm, bnm);
        }
    }

    /*
     * Now scan over the characters in "pz_targ_base".  Anything that
     * is not a legal name character gets replaced with an underscore.
     */
    for (;;) {
        unsigned int ch = (unsigned int)*(pz++);
        if (ch == NUL)
            break;
        if (! IS_ALPHANUMERIC_CHAR(ch))
            pz[-1] = '_';
    }
}

/**
 *  Set modification time and rename into result file name.
 */
static void
tidy_dep_file(void)
{
    static mode_t const fil_mode =
        S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

    /*
     * "startTime" is engineered to be 1 second earlier than the mod time
     * of any of the output files.
     */
    {
        struct utimbuf tbuf = {
            .actime  = time(NULL),
            .modtime = startTime
        };

        utime(pzDepFile, &tbuf);

        /*
         * If the target is not the dependency file, then ensure that the
         * file exists and set its time to the same time.  Ignore all errors.
         */
        if (strcmp(pzDepFile, pzDepTarget) != 0) {
            if (access(pzDepTarget, R_OK) != 0)
                close( open(pzDepTarget, O_CREAT, fil_mode));

            utime(pzDepTarget, &tbuf);
        }
    }

    chmod(pzDepFile, fil_mode);

    /*
     * Trim off the temporary suffix and rename the dependency file into
     * place.  We used a mkstemp name in case autogen failed.
     */
    {
        char * pze = strrchr(pzDepFile, '-');
        char * pzn;

        *pze = NUL;
        AGDUPSTR(pzn, pzDepFile, "dep file");
        *pze = '-';
        rename(pzDepFile, pzn);
        AGFREE(pzn);
    }
}

/**
 *  Finish off the dependency file
 */
LOCAL void
wrap_up_depends(void)
{
    flist_t * flist = targ_flist; //!< list scanning pointer

    if (temp_tpl_dir_len == 0) {
        temp_tpl_dir_len = 1;
        pz_temp_tpl = DEP_FILE_WRAP_FMT;
    }

    fprintf(pfDepends, "%s_TList =", pz_targ_base);
    while (flist != NULL) {
        flist_t * p = flist;
        if (strncmp(pz_temp_tpl, p->fname, temp_tpl_dir_len) != 0)
            fprintf(pfDepends, " \\\n\t%s", p->fname);
        flist = p->next;
        AGFREE(p);
    }

    fprintf(pfDepends, "\n\n%s_SList =", pz_targ_base);
    flist = src_flist;
    while (flist != NULL) {
        flist_t * p = flist;
        if (strncmp(pz_temp_tpl, p->fname, temp_tpl_dir_len) != 0)
            fprintf(pfDepends, " \\\n\t%s", p->fname);
        flist = p->next;
        AGFREE(p);
    }
    temp_tpl_dir_len--;

    targ_flist = src_flist = NULL;
    fprintf(pfDepends, DEP_FILE_WRAP_FMT, pz_targ_base,
            pzSourceList, pzDepTarget);
#if 0
    if (serv_id != NULLPROCESS) {
        char * pz = shell_cmd("echo ${AG_Dep_File}");
        if (*pz != NUL) {
            /*
             * The target we are crating will now depend upon the target
             * created by the spawned autogen run.  That spawned run script
             * is responsible for ensuring that if there are multiple targets,
             * then they are all chained together so we only worry about one.
             */
            static char const incfmt[] =
                "\n%s : %s\ninclude %s\n";

            size_t ln = strlen(pz);
            char * pt = AGALOC(ln + 6, "targ");
            memcpy(pt, pz, ln - 3);
            strcpy(pt + ln - 3, "targ");
            fprintf(pfDepends, incfmt, pzDepTarget, pt, pz);
            AGFREE(pt);
        }

        AGFREE(pz);
    }
#endif
    fclose(pfDepends);
    pfDepends = NULL;
    tidy_dep_file();
}
