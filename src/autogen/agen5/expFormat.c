
/**
 * @file expFormat.c
 *
 *  Time-stamp:        "2012-01-29 09:42:56 bkorb"
 *
 *  This module implements formatting expression functions.
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

typedef enum {
    LSEG_INFO   = 1,
    LSEG_DESC   = 2,
    LSEG_FULL   = 3,
    LSEG_NAME   = 4
} lic_segment_e_t;

/*=gfunc dne
 *
 * what:  '"Do Not Edit" warning'
 *
 * exparg: prefix,       string for starting each output line
 * exparg: first_prefix, for the first output line, opt
 * exparg: optpfx,       shifted prefix, opt
 *
 * doc:
 *  Generate a "DO NOT EDIT" or "EDIT WITH CARE" warning string.
 *  Which depends on whether or not the @code{--writable} command line
 *  option was set.
 *
 *  The first argument may be an option:  -d
 *
 *  This will suppress the variable text (date and version information).
 *  If specified, then the "prefix" and "first" arguments are shifted
 *  to the next arguments.
 *
 *  The first argument is a per-line string prefix.  The optional second
 *  argument is a prefix for the first-line and, in read-only mode, activates
 *  the editor hints.
 *  @*
 *  @example
 *  -*- buffer-read-only: t -*- vi: set ro:
 *  @end example
 *  @noindent
 *  The warning string also includes information about the template used
 *  to construct the file and the definitions used in its instantiation.
 *
 *  The optional third argument is used when the first argument is actually an
 *  invocation option and the prefix arguments get shifted.  The first
 *  argument must be, specifically, "@code{-d}".  That is used to signify that
 *  the date stamp should not be inserted into the output.
=*/
SCM
ag_scm_dne(SCM prefix, SCM first, SCM opt)
{
    char const *  date_str;
    char const *  pzRes;
    char const *  pzFirst;
    size_t        pfxLen;
    char const *  pzPrefix;
    SCM           res;

    if (! AG_SCM_STRING_P(prefix))
        return SCM_UNDEFINED;

    date_str = NULL;
    pzFirst  = zNil;
    pfxLen   = AG_SCM_STRLEN(prefix);
    pzPrefix = ag_scm2zchars(prefix, "dne-prefix");

    /*
     *  Check for the ``-d'' option
     */
    if ((pfxLen == 2) && (strncmp(pzPrefix, "-d", (size_t)2) == 0)) {
        date_str = zNil;
        pfxLen   = AG_SCM_STRLEN(first);
        pzPrefix = ag_scm2zchars(first, "dne-prefix");
        first    = opt;
    }

    /*
     *  IF we also have a 'first' prefix string,
     *  THEN we set it to something other than ``zNil'' and deallocate later.
     */
    if (AG_SCM_STRING_P(first))
        pzFirst = aprf(ENABLED_OPT(WRITABLE) ? "%s\n" : EXP_FMT_DNE1,
                       ag_scm2zchars(first, "first-prefix"), pzPrefix);

    if (date_str == NULL) {
        static char const tim_fmt[] =
            "  %B %e, %Y at %r by AutoGen " AUTOGEN_VERSION;

        size_t const  tsiz = sizeof(tim_fmt) + sizeof("september") * 2;
        time_t     curTime = time(NULL);
        struct tm*   pTime = localtime(&curTime);

        date_str = ag_scribble(tsiz);
        strftime((char *)date_str, tsiz, tim_fmt, pTime);
    }

    {
        char const * pz;
        tFpStack *   pfp = pCurFp;
        char const * tpl_name = strrchr(pzTemplFileName, DIRCH);
        if (tpl_name == NULL)
            tpl_name = pzTemplFileName;
        else
            tpl_name++;

        while (pfp->flags & FPF_UNLINK)  pfp = pfp->pPrev;
        if (! ENABLED_OPT(DEFINITIONS))
            pz = "<<no definitions>>";

        else if (*pzOopsPrefix != NUL)
            pz = "<<CGI-definitions>>";

        else {
            pz = OPT_ARG(DEFINITIONS);
            if (strcmp(pz, "-") == 0)
                pz = "stdin";
        }

        pzRes = aprf(ENABLED_OPT(WRITABLE) ? EXP_FMT_DNE2 : EXP_FMT_DNE,
                     pzPrefix, pfp->pzOutName, date_str,
                     pz, tpl_name, pzFirst);
    }

    if (pzRes == NULL)
        AG_ABEND("Allocating Do-Not-Edit string");

    res = AG_SCM_STR02SCM(pzRes);

    /*
     *  Deallocate any temporary buffers.  pzFirst either points to
     *  the zNil string, or to an allocated buffer.
     */
    AGFREE((void*)pzRes);
    if (pzFirst != zNil)
        AGFREE((void*)pzFirst);

    return res;
}


/*=gfunc error
 *
 * what:  display message and exit
 *
 * exparg: @message@message to display before exiting@@
 * doc:
 *
 *  The argument is a string that printed out as part of an error
 *  message.  The message is formed from the formatting string:
 *
 *  @example
 *  DEFINITIONS ERROR in %s line %d for %s:  %s\n
 *  @end example
 *
 *  The first three arguments to this format are provided by the
 *  routine and are:  The name of the template file, the line within
 *  the template where the error was found, and the current output
 *  file name.
 *
 *  After displaying the message, the current output file is removed
 *  and autogen exits with the EXIT_FAILURE error code.  IF, however,
 *  the argument begins with the number 0 (zero), or the string is the
 *  empty string, then processing continues with the next suffix.
=*/
SCM
ag_scm_error(SCM res)
{
    char const *  pzMsg;
    tSuccess      abrt = FAILURE;
    char          zNum[16];
    int           msgLen;

    switch (ag_scm_type_e(res)) {
    case GH_TYPE_BOOLEAN:
        if (AG_SCM_FALSEP(res))
            abrt = PROBLEM;
        pzMsg = zNil;
        break;

    case GH_TYPE_NUMBER:
    {
        unsigned long val = AG_SCM_TO_ULONG(res);
        if (val == 0)
            abrt = PROBLEM;
        snprintf(zNum, sizeof(zNum), "%d", (int)val);
        pzMsg = zNum;
        break;
    }

    case GH_TYPE_CHAR:
        zNum[0] = AG_SCM_CHAR(res);
        if ((zNum[0] == NUL) || (zNum[0] == '0'))
            abrt = PROBLEM;
        zNum[1] = NUL;
        pzMsg = zNum;
        break;

    case GH_TYPE_STRING:
        pzMsg  = ag_scm2zchars(res, "error string");
        pzMsg  = SPN_WHITESPACE_CHARS(pzMsg);
        msgLen = strlen(pzMsg);

        /*
         *  IF the message starts with the number zero,
         *    OR the message is the empty string,
         *  THEN this is just a warning that is ignored
         */
        if (msgLen <= 0)
            abrt = PROBLEM;
        else if (IS_DEC_DIGIT_CHAR(*pzMsg) && (strtol(pzMsg, NULL, 0) == 0))
            abrt = PROBLEM;
        break;

    default:
        pzMsg = BAD_MSG_STR;
    }

    /*
     *  IF there is a message,
     *  THEN print it.
     */
    if (*pzMsg != NUL) {
        char* pz = aprf(DEF_NOTE_FMT, (abrt != PROBLEM) ? ERROR_STR : WARN_STR,
                        pCurTemplate->pzTplFile, pCurMacro->lineNo,
                        pCurFp->pzOutName, pzMsg);
        if (abrt != PROBLEM)
            AG_ABEND(pz);
        fputs(pz, pfTrace);
        AGFREE((void*)pz);
    }

    longjmp(fileAbort, abrt);
    /* NOTREACHED */
    return SCM_UNDEFINED;
}

/**
 * Assemble the copyright preamble and long license description.
 *
 * @param txt a pointer to the first of two newlines separating
 *            copyright information from the description.
 */
static void
assemble_full_desc(char * txt, char const * pfx)
{
    char * pd;
    char * md;

    size_t prefix_len = strlen(pfx) + 1;
    while (  (prefix_len > 0)
          && IS_WHITESPACE_CHAR(pfx[prefix_len - 2]))
        prefix_len--;

    /*
     *  Preserve the first newline.  Set the move destination
     *  out past where we will be inserting the "<PFX>\n" marker.
     */
    pd = txt + 1;          /* prefix destination */
    md = pd  + prefix_len; /* move destination */

    while (*txt == NL) txt++;
    /*
     *  Maybe there were exactly enough NL characters we don't need to move
     */
    if (md != txt)
        memmove(md, txt, strlen(txt) + 1);
    memmove(pd, pfx, --prefix_len);
    pd[prefix_len] = NL;

    /*
     *  Look for a trailing license name and trim it and trailing white space
     */
    txt = strstr(md, "\n\n");
    if (txt == NULL)
        txt = md + strlen(md);

    while (  (txt > md)
          && IS_WHITESPACE_CHAR(txt[-1]))  txt--;
    *txt = NUL;
}

/**
 * Trim off the license name.  It is the third double-newline stanza
 * in the license file.
 *
 * @param p  a pointer to the first of two newlines separating
 *            copyright information from the description.
 * @return pointer to second stanza, sans the license name trailer.
 */
static char *
trim_lic_name(char * p)
{
    char * res;
    /* skip the leading white space.  It starts with NL. */
    p = SPN_WHITESPACE_CHARS(p + 1);
    if (*p == NUL)
        return p;

    res = p;

    /*
     *  The last section ends with two consecutive new lines.
     *  All trailing newlines are trimmed (not all white space).
     */
    p = strstr(p, "\n\n");
    if (p == NULL)
        p = res + strlen(res);
    while (  (p > res)
          && IS_WHITESPACE_CHAR(p[-1]))  p--;
    *p = NUL;

    return res;
}

/**
 * Extract the license name.  It is the third double-newline stanza
 * in the license file.
 *
 * @param txt a pointer to the first of two newlines separating
 *            copyright information from the description.
 * @return pointer to the license name trailer.
 */
static char *
get_lic_name(char * p)
{
    char * scan = p;
    while (*(++scan) == NL)   ; /* skip the leading NL's. */

    /*
     * Find the third stanza.  If there.  If not, we supply some static
     * text:  "an unknown license"
     */
    scan = strstr(scan, "\n\n");
    if (scan == NULL) {
        strcpy(p, EXP_FMT_BAD_LIC);
        return p;
    }
    while (*scan == NL) scan++;
    return scan;
}

/**
 * Find the kind of text being requested.  It may be "full" (the first
 * two stanzas), "info" (the first -- copyright info + license name),
 * "description" (the second -- a one paragraph description), or
 * "name" -- the third stanza.
 *
 * @param txt a pointer to the first of two newlines separating
 *            copyright information from the description.
 * @return pointer to the requested text.
 */
static char *
find_lic_text(
    lic_segment_e_t segment, SCM lic, ssize_t * txt_len, char const * pfx)
{
    static char const * const lic_sfx[] = { FIND_LIC_TEXT_SFX, NULL };

    char const * lic_pz = ag_scm2zchars(lic, "license");
    char fname[ AG_PATH_MAX ];
    char * ftext;
    size_t flen;

    /*
     * auto-convert "bsd" into "mbsd" for compatibility.
     */
    if (strcmp(lic_pz, FIND_LIC_TEXT_MBSD+1) == 0)
        lic_pz = FIND_LIC_TEXT_MBSD;

    if (! SUCCESSFUL(findFile(lic_pz, fname, lic_sfx, NULL)))
        return NULL;

    {
        struct stat stbf;
        if (stat(fname, &stbf) != 0)
            AG_CANT(FIND_LIC_TEXT_NO_LIC, fname);
        if (! S_ISREG(stbf.st_mode)) {
            errno = EINVAL;
            AG_CANT(FIND_LIC_TEXT_BAD_FILE, fname);
        }
        flen = stbf.st_size;
    }

    ftext = ag_scribble(flen + EXP_FMT_BAD_LIC_LEN + 1);
    *txt_len = flen;

    {
        FILE * fp = fopen(fname, "r");

        if (fp == NULL)
            AG_CANT(FIND_LIC_TEXT_OPEN, fname);

        if (fread(ftext, 1, flen, fp) != flen)
            AG_CANT(FIND_LIC_TEXT_BAD_FILE, fname);

        ftext[flen] = NUL;
        fclose(fp);
    }

    if (pfDepends != NULL)
        add_source_file(fname);

    {
        char * p = strstr(ftext, DOUBLE_NEWLINE);

        if (p == NULL)
            AG_ABEND(aprf(FIND_LIC_TEXT_INVAL, fname));

        switch (segment) {
        case LSEG_INFO: p[1]  = NUL;                break;
        case LSEG_DESC: ftext = trim_lic_name(p);   break;
        case LSEG_NAME: ftext = get_lic_name(p);    break;
        case LSEG_FULL: assemble_full_desc(p, pfx); break;
        }
    }

    return ftext;
}

/**
 * Construct an SCM for the kind of text being requested.
 *
 * It may be "full" (the first two stanzas), "info" (the first -- copyright
 * info + license name), "description" (the second -- a one paragraph
 * description), or "name" -- the third stanza.
 *
 * @param seg    which segment of license is desired
 * @param lic    The name of the license
 * @param prog   the name of the program
 * @param pfx    a per-line prefix
 * @param owner  who owns the copyright
 * @param years  the copyright years
 *
 * @return the SCM-ized string
 */
static SCM
construct_license(
    lic_segment_e_t seg, SCM lic, SCM prog, SCM pfx, SCM owner, SCM years)
{
    static SCM subs  = SCM_UNDEFINED;
    static SCM empty = SCM_UNDEFINED;

    SCM     vals = SCM_UNDEFINED;
    char *  lic_text;
    ssize_t text_len;
    char const * pfx_pz = ag_scm2zchars(pfx, "lic-prefix");

    if (subs == SCM_UNDEFINED) {
        static char const * const slst[] = {
            MK_LIC_PROG, MK_LIC_PFX, MK_LIC_OWN, MK_LIC_YRS
        };
        subs = scm_gc_protect_object(
            scm_list_4(AG_SCM_STR02SCM(slst[0]), AG_SCM_STR02SCM(slst[1]),
                       AG_SCM_STR02SCM(slst[2]), AG_SCM_STR02SCM(slst[3])));

        empty = scm_gc_protect_object(AG_SCM_STR02SCM(""));
    }

    if (! AG_SCM_STRING_P(lic))
        AG_ABEND(MK_LIC_NOT_STR);

    lic_text = find_lic_text(seg, lic, &text_len, pfx_pz);
    if (lic_text == NULL)
        AG_ABEND(aprf(MK_LIC_NO_LIC, ag_scm2zchars(lic, "lic")));

    if (! AG_SCM_STRING_P(owner))   owner = empty;
    if (! AG_SCM_STRING_P(years))   years = empty;
    vals = scm_list_4(prog, pfx, owner, years);

    do_multi_subs(&lic_text, &text_len, subs, vals);

    return AG_SCM_STR02SCM(lic_text);
}

/*=gfunc license_full
 *
 * what:  Emit the licensing information and description
 * general_use:
 *
 * exparg: license,   name of license type
 * exparg: prog-name, name of the program under the GPL
 * exparg: prefix,    String for starting each output line
 * exparg: owner,     owner of the program, optional
 * exparg: years,     copyright years, optional
 *
 * doc:
 *
 *  Emit all the text that @code{license-info} and @code{license-description}
 *  would emit (@pxref{SCM license-info, @code{license-info}},
 *  and @pxref{SCM license-description, @code{license-description}}),
 *  with all the same substitutions.
 *
 *  All of these depend upon the existence of a license file named after the
 *  @code{license} argument with a @code{.lic} suffix.  That file should
 *  contain three blocks of text, each separated by two or more newline
 *  characters.
 *
 *  The first section describes copyright attribution and the name of the usage
 *  licence.  For GNU software, this should be the text that is to be displayed
 *  with the program version.  Four text markers can be replaced: <PFX>,
 *  <program>, <years> and <owner>.
 *
 *  The second section is a short description of the terms of the license.
 *  This is typically the kind of text that gets displayed in the header of
 *  source files.  The third section is strictly the name of the license
 *  without any substitution markers.  Only the <PFX>, <owner> and <program>
 *  markers are substituted.
 *
 *  The third section is strictly the name of the license.
 *  No marker substitutions are performed.
 *
 *  @example
 *  <PFX>Copyright (C) <years> <owner>, all rights reserved.
 *  <PFX>This is free software. It is licensed for use,
 *  <PFX>modification and redistribution under the terms
 *  <PFX>of the GNU General Public License, version 3 or later
 *  <PFX>    <http://gnu.org/licenses/gpl.html>
 *
 *  <PFX><program> is free software: you can redistribute it
 *  <PFX>and/or modify it under the terms of the GNU General
 *  <PFX>Public License as published by the Free Software ...
 *
 *  the GNU General Public License, version 3 or later
 *  @end example
=*/
SCM
ag_scm_license_full(SCM lic, SCM prog, SCM pfx, SCM owner, SCM years)
{
    return construct_license(LSEG_FULL, lic, prog, pfx, owner, years);
}

/*=gfunc license_description
 *
 * what:  Emit a license description
 * general_use:
 *
 * exparg: license,   name of license type
 * exparg: prog-name, name of the program under the GPL
 * exparg: prefix,    String for starting each output line
 * exparg: owner,     owner of the program, optional
 *
 * doc:
 *
 *  Emit a string that contains a detailed license description, with
 *  substitutions for program name, copyright holder and a per-line prefix.
 *  This is the text typically used as part of a source file header.
 *  For more details, @xref{SCM license-full, the license-full command}.
 *
=*/
SCM
ag_scm_license_description(SCM lic, SCM prog, SCM pfx, SCM owner)
{
    return construct_license(LSEG_DESC, lic, prog, pfx, owner, SCM_UNDEFINED);
}

/*=gfunc license_info
 *
 * what:  Emit the licensing information and copyright years
 * general_use:
 *
 * exparg: license,   name of license type
 * exparg: prog-name, name of the program under the GPL
 * exparg: prefix,    String for starting each output line
 * exparg: owner,     owner of the program, optional
 * exparg: years,     copyright years, optional
 *
 * doc:
 *
 *  Emit a string that contains the licensing description, with some
 *  substitutions for program name, copyright holder, a list of years when the
 *  source was modified, and a per-line prefix.  This text typically includes a
 *  brief license description and is often printed out when a program starts
 *  running or as part of the @code{--version} output.
 *  For more details, @xref{SCM license-full, the license-full command}.
 *
=*/
SCM
ag_scm_license_info(SCM lic, SCM prog, SCM pfx, SCM owner, SCM years)
{
    return construct_license(LSEG_INFO, lic, prog, pfx, owner, years);
}

/*=gfunc license_name
 *
 * what:  Emit the name of the license
 * general_use:
 *
 * exparg: license,   name of license type
 *
 * doc:
 *
 *  Emit a string that contains the full name of the license.
=*/
SCM
ag_scm_license_name(SCM lic)
{
    ssize_t text_len;
    char * txt = find_lic_text(LSEG_NAME, lic, &text_len, "");
    char * e;

    txt = SPN_WHITESPACE_CHARS(txt);
    e   = txt + strlen(txt);
    while (IS_WHITESPACE_CHAR(e[-1]) && (e > txt))  e--;
    *e  = NUL;
    lic = AG_SCM_STR02SCM(txt);
    return lic;
}

/*=gfunc gpl
 *
 * what:  GNU General Public License
 * general_use:
 *
 * exparg: prog-name, name of the program under the GPL
 * exparg: prefix, String for starting each output line
 *
 * doc:
 *
 *  Emit a string that contains the GNU General Public License.
 *  This function is now deprecated.  Please @xref{SCM license-description}.
=*/
SCM
ag_scm_gpl(SCM prog_name, SCM prefix)
{
    static SCM lic = SCM_UNDEFINED;

    if (lic == SCM_UNDEFINED)
        lic = scm_gc_protect_object(AG_SCM_STR02SCM(FIND_LIC_TEXT_LGPL+1));
    return ag_scm_license_description(lic, prog_name, prefix, SCM_UNDEFINED);
}

/*=gfunc agpl
 *
 * what:  GNU Affero General Public License
 * general_use:
 *
 * exparg: prog-name, name of the program under the GPL
 * exparg: prefix, String for starting each output line
 *
 * doc:
 *
 *  Emit a string that contains the GNU Affero General Public License.
 *  This function is now deprecated.  Please @xref{SCM license-description}.
=*/
SCM
ag_scm_agpl(SCM prog_name, SCM prefix)
{
    static SCM lic = SCM_UNDEFINED;

    if (lic == SCM_UNDEFINED)
        lic = scm_gc_protect_object(AG_SCM_STR02SCM(FIND_LIC_TEXT_AGPL));
    return ag_scm_license_description(lic, prog_name, prefix, SCM_UNDEFINED);
}

/*=gfunc lgpl
 *
 * what:  GNU Library General Public License
 * general_use:
 *
 * exparg: prog_name, name of the program under the LGPL
 * exparg: owner, Grantor of the LGPL
 * exparg: prefix, String for starting each output line
 *
 * doc:
 *
 *  Emit a string that contains the GNU Library General Public License.
 *  This function is now deprecated.  Please @xref{SCM license-description}.
=*/
SCM
ag_scm_lgpl(SCM prog_name, SCM owner, SCM prefix)
{
    static SCM lic = SCM_UNDEFINED;

    if (lic == SCM_UNDEFINED)
        lic = scm_gc_protect_object(AG_SCM_STR02SCM(FIND_LIC_TEXT_LGPL));
    return ag_scm_license_description(lic, prog_name, prefix, owner);
}

/*=gfunc bsd
 *
 * what:  BSD Public License
 * general_use:
 *
 * exparg: prog_name, name of the program under the BSD
 * exparg: owner, Grantor of the BSD License
 * exparg: prefix, String for starting each output line
 *
 * doc:
 *
 *  Emit a string that contains the Free BSD Public License.
 *  This function is now deprecated.  Please @xref{SCM license-description}.
 *
=*/
SCM
ag_scm_bsd(SCM prog_name, SCM owner, SCM prefix)
{
    static SCM lic = SCM_UNDEFINED;

    if (lic == SCM_UNDEFINED)
        lic = scm_gc_protect_object(AG_SCM_STR02SCM(FIND_LIC_TEXT_MBSD));
    return ag_scm_license_description(lic, prog_name, prefix, owner);
}

/*=gfunc license
 *
 * what:  an arbitrary license
 * general_use:
 *
 * exparg: lic_name, file name of the license
 * exparg: prog_name, name of the licensed program or library
 * exparg: owner, Grantor of the License
 * exparg: prefix, String for starting each output line
 *
 * doc:
 *  Emit a string that contains the named license.
 *  This function is now deprecated.  Please @xref{SCM license-description}.
=*/
SCM
ag_scm_license(SCM license, SCM prog_name, SCM owner, SCM prefix)
{
    char const * pzPfx   = ag_scm2zchars(prefix,    "GPL line prefix");
    char const * pzPrg   = ag_scm2zchars(prog_name, "program name");
    char const * pzOwner = ag_scm2zchars(owner,     "owner");
    static struct {
        char const * pzFN;
        tmap_info_t  mi;
    } lic = { NULL, { NULL, 0, 0, 0, 0, 0, 0, 0 }};

    char*     pzRes;

    if (! AG_SCM_STRING_P(license))
        return SCM_UNDEFINED;

    {
        static char const * const apzSfx[] = { MK_LIC_SFX, NULL };
        static char fname[ AG_PATH_MAX ];
        char const * pzLicense = ag_scm2zchars(license, "lic file");

        /*
         *  Find the template file somewhere
         */
        if (! SUCCESSFUL(findFile(pzLicense, fname, apzSfx, NULL))) {
            errno = ENOENT;
            AG_CANT(MK_LIC_NO_LIC, pzLicense);
        }

        if ((lic.pzFN != NULL) && (strcmp(fname, lic.pzFN) != 0)) {
            text_munmap(&lic.mi);
            AGFREE((void*)lic.pzFN);
            lic.pzFN = NULL;
        }

        if (lic.pzFN == NULL) {
            text_mmap(fname, PROT_READ|PROT_WRITE, MAP_PRIVATE, &lic.mi);
            if (TEXT_MMAP_FAILED_ADDR(lic.mi.txt_data))
                AG_ABEND(aprf(MK_LIC_NO_OPEN, pzLicense));

            if (pfDepends != NULL)
                add_source_file(pzLicense);

            AGDUPSTR(lic.pzFN, fname, "lic f name");
        }
    }

    /*
     *  Trim trailing white space.
     */
    {
        char* pz = (char*)lic.mi.txt_data + lic.mi.txt_size;
        while (  (pz > (char*)lic.mi.txt_data)
              && IS_WHITESPACE_CHAR(pz[-1]))
            pz--;
        *pz = NUL;
    }

    /*
     *  Get the addresses of the program name prefix and owner strings.
     *  Make sure they are reasonably sized (less than
     *  SCRIBBLE_SIZE).  Copy them to the scratch buffer.
     */
    if (AG_SCM_STRLEN(prog_name) >= SCRIBBLE_SIZE)
        AG_ABEND(aprf(MK_LIC_TOO_BIG_FMT, MK_LIC_BIG_PROG, SCRIBBLE_SIZE));

    if (AG_SCM_STRLEN(prefix) >= SCRIBBLE_SIZE)
        AG_ABEND(aprf(MK_LIC_TOO_BIG_FMT, MK_LIC_BIG_PFX, SCRIBBLE_SIZE));

    if (AG_SCM_STRLEN(owner) >= SCRIBBLE_SIZE)
        AG_ABEND(aprf(MK_LIC_TOO_BIG_FMT, MK_LIC_BIG_OWN, SCRIBBLE_SIZE));

    /*
     *  Reformat the string with the given arguments
     */
    pzRes = aprf((char*)lic.mi.txt_data, pzPrg, pzOwner);
    {
        int     pfx_size = strlen(pzPfx);
        char*   pzScan   = pzRes;
        char*   pzOut;
        char*   pzSaveRes;
        size_t  out_size = pfx_size;

        /*
         *  Figure out how much space we need (text size plus
         *  a prefix size for each newline)
         */
        for (;;) {
            switch (*(pzScan++)) {
            case NUL:
                goto exit_count;
            case NL:
                out_size += pfx_size;
                /* FALLTHROUGH */
            default:
                out_size++;
            }
        } exit_count:;

        /*
         *  Create our output buffer and insert the first prefix
         */
        pzOut = pzSaveRes = ag_scribble(out_size);

        strcpy(pzOut, pzPfx);
        pzOut += pfx_size;
        pzScan = pzRes;

        for (;;) {
            switch (*(pzOut++) = *(pzScan++)) {
            case NUL:
                goto exit_copy;

            case NL:
                strcpy(pzOut, pzPfx);
                pzOut += pfx_size;
                break;

            default:
                break;
            }
        }
    exit_copy:;

        /*
         *  We allocated a temporary buffer that has all the
         *  formatting done, but need the prefixes on each line.
         */
        AGFREE((void*)pzRes);

        return AG_SCM_STR2SCM(pzSaveRes, (size_t)((pzOut - pzSaveRes) - 1));
    }
}
/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * indent-tabs-mode: nil
 * End:
 * end of agen5/expFormat.c */
