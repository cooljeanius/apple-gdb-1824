
/**
 * @file expOutput.c
 *
 *  Time-stamp:        "2012-01-29 11:58:30 bkorb"
 *
 *  This module implements the output file manipulation function
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

#ifndef S_IAMB
/*
 *  Access Mask Bits (3 special plus RWX for User Group & Others (9))
 */
#  define S_IAMB      (S_ISUID|S_ISGID|S_ISVTX|S_IRWXU|S_IRWXG|S_IRWXO)
#endif

#define WRITE_MASK ((unsigned)(~(S_IWUSR|S_IWGRP|S_IWOTH)))

typedef struct {
    char const *  pzSuspendName;
    tFpStack*     pOutDesc;
} tSuspendName;

static int            suspendCt   = 0;
static int            suspAllocCt = 0;
static tSuspendName*  pSuspended  = NULL;
static int            outputDepth = 1;

/**
 * chmod u+w on a file.
 */
static void
make_writable(char* pzFileName)
{
    struct stat sbuf;

#ifdef DEBUG_ENABLED
    /*
     *  "stat(2)" does not initialize the entire structure.
     */
    memset(&sbuf, NUL, sizeof(sbuf));
#endif

    stat(pzFileName, &sbuf);

    /*
     *  Or in the user write bit
     */
    sbuf.st_mode |= S_IWUSR;
    chmod(pzFileName, sbuf.st_mode & S_IAMB);
}

/**
 * return the current line number
 */
static int
current_line(FILE * fp)
{
    int lnno = 1;

    while (! feof(fp)) {
        int ch = getc(fp);
        if (ch == NL)
            lnno++;
    }

    return lnno;
}

/**
 * guts of the output file/line function
 */
static SCM
do_output_file_line(int line_delta, char const * fmt)
{
    char * buf;
    char const * fname = pCurFp->pzOutName;

    if (pCurFp->flags & FPF_TEMPFILE) {
        fname = OUTPUT_TEMP_FILE;
        line_delta = 0;

    } else if (fseek(pCurFp->pFile, 0, SEEK_SET) == 0) {
        line_delta += current_line(pCurFp->pFile);

    } else {
        line_delta = 0;
    }

    {
        size_t sz = strlen(fmt) + strlen(fname) + 24;
        buf = ag_scribble(sz);
    }

    {
        void * args[2] = {
            (void *)fname,
            (void *)(uintptr_t)line_delta
        };
        sprintfv(buf, fmt, (snv_constpointer *)args);
    }

    return AG_SCM_STR02SCM(buf);
}

/**
 * chmod a-w on a file descriptor.
 */
LOCAL void
make_readonly(int fd)
{
    struct stat sbuf;

    /*
     *  If the output is supposed to be writable, then also see if
     *  it is a temporary condition (set vs. a command line option).
     */
    if (ENABLED_OPT(WRITABLE)) {
        if (! HAVE_OPT(WRITABLE))
            CLEAR_OPT(WRITABLE);
        return;
    }

    /*
     *  Set our usage mask to all all the access
     *  bits that do not provide for write access
     */
    fstat(fd, &sbuf);

    /*
     *  Mask off the write permission bits, but ensure that
     *  the user read bit is set.
     */
    sbuf.st_mode = ((unsigned)sbuf.st_mode & WRITE_MASK) | S_IRUSR;
    fchmod(fd, sbuf.st_mode & S_IAMB);
}

/**
 * Some common code for creating a new file
 */
LOCAL void
open_output_file(char const * fname, size_t nmsz, char const * mode, int flags)
{
    char *    pz;
    tFpStack* p  = AGALOC(sizeof(*p), "out file stack");

    pz = (char*)AGALOC(nmsz + 1, "file name string");
    memcpy(pz, fname, nmsz);
    pz[ nmsz ] = NUL;
    memset(p, NUL, sizeof(*p));
    p->pzOutName = pz;

    /*
     *  IF we are creating the file and we are allowed to unlink the output,
     *  then start by unlinking the thing.
     */
    if ((*mode == 'w') && ((flags & FPF_NOUNLINK) == 0)) {
        if ((unlink(pz) != 0) && (errno != ENOENT))
            AG_CANT(OUTPUT_NO_UNLINK, pz);
    }

    {
        static int const w_ok = ~WRITE_MASK;
        struct stat sbuf;
        if (stat(fname, &sbuf) == 0) {
            if ((sbuf.st_mode & w_ok) == 0) {
                sbuf.st_mode |= w_ok;
                chmod(fname, sbuf.st_mode & 07777);
            }
        }
    }

    p->pFile = fopen(pz, mode);
    if (p->pFile == NULL)
        AG_CANT(OUTPUT_NO_OPEN, pz);

    p->pPrev = pCurFp;
    pCurFp   = p;
    p->flags = FPF_FREE | flags;
    outputDepth++;

    make_writable(pz);

    if (OPT_VALUE_TRACE > TRACE_DEBUG_MESSAGE)
        fprintf(pfTrace, TRACE_OPEN_OUT, __func__, fname, mode);

    /*
     * Avoid printing temporary file names in the dependency file
     */
    if ((pfDepends != NULL) && ((flags & FPF_TEMPFILE) == 0))
        add_target_file(fname);
}

/*=gfunc out_delete
 *
 * what: delete current output file
 * doc:
 *  Remove the current output file.  Cease processing the template for
 *  the current suffix.  It is an error if there are @code{push}-ed
 *  output files.  Use the @code{(error "0")} scheme function instead.
 *  @xref{output controls}.
=*/
SCM
ag_scm_out_delete(void)
{
    /*
     *  Delete the current output file
     */
    if (OPT_VALUE_TRACE > TRACE_DEBUG_MESSAGE)
        fprintf(pfTrace, TRACE_OUT_DELETE, pCurFp->pzOutName);
    rm_target_file(pCurFp->pzOutName);
    outputDepth = 1;
    longjmp(fileAbort, PROBLEM);
    /* NOTREACHED */
    return SCM_UNDEFINED;
}


/*=gfunc out_move
 *
 * what:   change name of output file
 * exparg: new-name, new name for the current output file
 *
 * doc:
 *
 *  Rename current output file.  @xref{output controls}.
 *  Please note: changing the name will not save a temporary file from
 *  being deleted.  It @i{may}, however, be used on the root output file.
 *
 *  NOTE: if you are creating a dependency file, @i{both} the original
 *  file name @i{and} the new file name will be listed as derived files.
=*/
SCM
ag_scm_out_move(SCM new_file)
{
    size_t sz = AG_SCM_STRLEN(new_file);
    char*  pz = (char*)AGALOC(sz + 1, "new file name string");
    memcpy(pz, AG_SCM_CHARS(new_file), sz);
    pz[ sz ] = NUL;

    if (OPT_VALUE_TRACE > TRACE_DEBUG_MESSAGE)
        fprintf(pfTrace, TRACE_MOVE_FMT, __func__, pCurFp->pzOutName, pz);
    rename(pCurFp->pzOutName, pz);

    if (pfDepends != NULL) {
        rm_target_file(pCurFp->pzOutName);
        add_target_file(pz);
    }

    if ((pCurFp->flags & FPF_STATIC_NM) == 0)
        AGFREE((void*)pCurFp->pzOutName);

    AGDUPSTR(pCurFp->pzOutName, pz, "file name");
    pCurFp->flags &= ~FPF_STATIC_NM;
    return SCM_UNDEFINED;
}


/*=gfunc out_pop
 *
 * what:   close current output file
 * exparg: disp, return contents of the file, optional
 * doc:
 *  If there has been a @code{push} on the output, then close that
 *  file and go back to the previously open file.  It is an error
 *  if there has not been a @code{push}.  @xref{output controls}.
 *
 *  If there is no argument, no further action is taken.  Otherwise,
 *  the argument should be @code{#t} and the contents of the file
 *  are returned by the function.
=*/
SCM
ag_scm_out_pop(SCM ret_contents)
{
    SCM res = SCM_UNDEFINED;

    if (pCurFp->pPrev == NULL)
        AG_ABEND(SCM_OUT_POP_EMPTY);

    if (OPT_VALUE_TRACE >= TRACE_EXPRESSIONS)
        fprintf(pfTrace, TRACE_POP_FMT, __func__, pCurFp->pzOutName,
                (ret_contents == SCM_UNDEFINED) ? "" : " #t");

    if (AG_SCM_BOOL_P(ret_contents) && AG_SCM_NFALSEP(ret_contents)) {
        long  pos = ftell(pCurFp->pFile);
        char * pz;

        if (pos <= 0) {
            pz  = (void*)zNil; // const-ness not important
            pos = 0;

        } else {
            pz = ag_scribble((size_t)pos + 1);
            rewind(pCurFp->pFile);
            if (fread(pz, (size_t)pos, (size_t)1, pCurFp->pFile) != 1)
                AG_CANT(SCM_OUT_POP_NO_REREAD, pCurFp->pzOutName);
        }

        res = AG_SCM_STR2SCM(pz, pos);
    }

    outputDepth--;
    out_close(AG_FALSE);
    return res;
}

/*=gfunc output_file_next_line
 *
 * what:   print the file name and next line number
 *
 * exparg: line_off, offset to line number,   optional
 * exparg: alt_fmt,  alternate format string, optional
 *
 * doc:
 *  Returns a string with the current output file name and line number.
 *  The default format is: # <line+1> "<output-file-name>" The argument may be
 *  either a number indicating an offset from the current output line number
 *  or an alternate formatting string.  If both are provided, then the first
 *  must be a numeric offset.
 *
 *  Be careful that you are directing output to the final output file.
 *  Otherwise, you will get the file name and line number of the temporary
 *  file.  That won't be what you want.
=*/
SCM
ag_scm_output_file_next_line(SCM num_or_str, SCM str)
{
    char const * pzFmt = FILE_LINE_FMT;
    int  line_off = 1;

    if (AG_SCM_NUM_P(num_or_str))
        line_off = AG_SCM_TO_LONG(num_or_str);
    else
        str = num_or_str;

    if (AG_SCM_STRING_P(str))
        pzFmt = ag_scm2zchars(str, "file/line format");

    return do_output_file_line(line_off, pzFmt);
}


/*=gfunc out_suspend
 *
 * what:   suspend current output file
 * exparg: suspName, A name tag for reactivating
 *
 * doc:
 *  If there has been a @code{push} on the output, then set aside the output
 *  descriptor for later reactiviation with @code{(out-resume "xxx")}.  The
 *  tag name need not reflect the name of the output file.  In fact, the
 *  output file may be an anonymous temporary file.  You may also change the
 *  tag every time you suspend output to a file, because the tag names are
 *  forgotten as soon as the file has been "resumed".
=*/
SCM
ag_scm_out_suspend(SCM susp_nm)
{
    if (pCurFp->pPrev == NULL)
        AG_ABEND(OUT_SUSPEND_CANNOT);

    if (++suspendCt > suspAllocCt) {
        suspAllocCt += 8;
        if (pSuspended == NULL)
            pSuspended = (tSuspendName*)
                AGALOC(suspAllocCt * sizeof(tSuspendName), "susp file list");
        else
            pSuspended = (tSuspendName*)
                AGREALOC((void*)pSuspended,
                         suspAllocCt * sizeof(tSuspendName), "add to susp f");
    }

    pSuspended[ suspendCt-1 ].pzSuspendName = AG_SCM_TO_NEWSTR(susp_nm);
    pSuspended[ suspendCt-1 ].pOutDesc      = pCurFp;
    if (OPT_VALUE_TRACE >= TRACE_EXPRESSIONS)
        fprintf(pfTrace, TRACE_SUSPEND, __func__, pCurFp->pzOutName,
                pSuspended[ suspendCt-1 ].pzSuspendName);

    pCurFp = pCurFp->pPrev;
    outputDepth--;

    return SCM_UNDEFINED;
}


/*=gfunc out_resume
 *
 * what:   resume suspended output file
 * exparg: susp_nm, A name tag for reactivating
 * doc:
 *  If there has been a suspended output, then make that output descriptor
 *  current again.  That output must have been suspended with the same tag
 *  name given to this routine as its argument.
=*/
SCM
ag_scm_out_resume(SCM susp_nm)
{
    int  ix  = 0;
    char const * pzName = ag_scm2zchars(susp_nm, "resume name");

    for (; ix < suspendCt; ix++) {
        if (strcmp(pSuspended[ ix ].pzSuspendName, pzName) == 0) {
            pSuspended[ ix ].pOutDesc->pPrev = pCurFp;
            pCurFp = pSuspended[ ix ].pOutDesc;
            free((void*)pSuspended[ ix ].pzSuspendName); /* Guile alloc */
            if (ix < --suspendCt)
                pSuspended[ ix ] = pSuspended[ suspendCt ];
            ++outputDepth;
            if (OPT_VALUE_TRACE >= TRACE_EXPRESSIONS)
                fprintf(pfTrace, TRACE_RESUME, __func__,
                        pCurFp->pzOutName, pzName);
            return SCM_UNDEFINED;
        }
    }

    AG_ABEND(aprf(OUT_RESUME_CANNOT, pzName));
    /* NOTREACHED */
    return SCM_UNDEFINED;
}


/*=gfunc out_emit_suspended
 *
 * what:   emit the text of suspended output
 * exparg: susp_nm, A name tag of suspended output
 * doc:
 *  This function is equivalent to
 *  @code{(begin (out-resume <name>) (out-pop #t))}
=*/
SCM
ag_scm_out_emit_suspended(SCM susp_nm)
{
    (void)ag_scm_out_resume(susp_nm);
    return ag_scm_out_pop(SCM_BOOL_T);
}


/*=gfunc ag_fprintf
 *
 * what:  format to autogen stream
 *
 * exparg: ag-diversion, AutoGen diversion name or number
 * exparg: format,       formatting string
 * exparg: format-arg,   list of arguments to formatting string, opt, list
 *
 * doc:  Format a string using arguments from the alist.
 *       Write to a specified AutoGen diversion.
 *       That may be either a specified suspended output stream
 *       (@pxref{SCM out-suspend}) or an index into the output stack
 *       (@pxref{SCM out-push-new}).  @code{(ag-fprintf 0 ...)} is
 *       equivalent to @code{(emit (sprintf ...))}, and
 *       @code{(ag-fprintf 1 ...)} sends output to the most recently
 *       suspended output stream.
=*/
SCM
ag_scm_ag_fprintf(SCM port, SCM fmt, SCM alist)
{
    int   list_len = scm_ilength(alist);
    char const * pzFmt = ag_scm2zchars(fmt, WORD_FORMAT);
    SCM   res      = run_printf(pzFmt, list_len, alist);

    /*
     *  If "port" is a string, it must match one of the suspended outputs.
     *  Otherwise, we'll fall through to the abend.
     */
    if (AG_SCM_STRING_P(port)) {
        int  ix  = 0;
        char const * pzName = ag_scm2zchars(port, "resume name");

        for (; ix < suspendCt; ix++) {
            if (strcmp(pSuspended[ ix ].pzSuspendName, pzName) == 0) {
                tFpStack* pSaveFp = pCurFp;
                pCurFp = pSuspended[ ix ].pOutDesc;
                (void) ag_scm_emit(res);
                pCurFp = pSaveFp;
                return SCM_UNDEFINED;
            }
        }
    }

    /*
     *  If "port" is a number, it is an index into the output stack with "0"
     *  (zero) representing the current output and "1" the last suspended
     *  output.  If the number is out of range, we'll fall through to the
     *  abend.
     */
    else if (AG_SCM_NUM_P(port)) {
        tFpStack* pSaveFp = pCurFp;
        unsigned long val = AG_SCM_TO_ULONG(port);

        for (; val > 0; val--) {
            pCurFp = pCurFp->pPrev;
            if (pCurFp == NULL) {
                pCurFp = pSaveFp;
                goto fprintf_woops;
            }
        }

        (void) ag_scm_emit(res);
        pCurFp  = pSaveFp;
        return SCM_UNDEFINED;
    }

    /*
     *  Still here?  We have a bad "port" specification.
     */
    fprintf_woops:

    AG_ABEND(AG_FPRINTF_BAD_PORT);
    /* NOTREACHED */
    return SCM_UNDEFINED;
}

/*=gfunc out_push_add
 *
 * what:   append output to file
 * exparg: file-name, name of the file to append text to
 *
 * doc:
 *  Identical to @code{push-new}, except the contents are @strong{not}
 *  purged, but appended to.  @xref{output controls}.
=*/
SCM
ag_scm_out_push_add(SCM new_file)
{
    static char const append_mode[] = "a" FOPEN_BINARY_FLAG "+";

    if (! AG_SCM_STRING_P(new_file))
        AG_ABEND(OUT_ADD_INVALID);

    open_output_file(AG_SCM_CHARS(new_file), AG_SCM_STRLEN(new_file),
                     append_mode, 0);

    return SCM_UNDEFINED;
}


/*=gfunc make_tmp_dir
 *
 * what:   create a temporary directory
 *
 * doc:
 *  Create a directory that will be cleaned up upon exit.
=*/
SCM
ag_scm_make_tmp_dir(void)
{
    if (pz_temp_tpl == NULL) {
        char * tmpdir = shell_cmd(MK_TMP_DIR_CMD);
        char * cmdbf  =
            ag_scribble(SET_TMP_DIR_CMD_LEN + 2 * MK_TMP_DIR_CMD_LEN);

        pz_temp_tpl = tmpdir;
        temp_tpl_dir_len = strlen(tmpdir) - 9;    // "ag-XXXXXX"

        tmpdir[temp_tpl_dir_len - 1] = NUL;       // trim dir char
        sprintf(cmdbf, SET_TMP_DIR_CMD, tmpdir);
        tmpdir[temp_tpl_dir_len - 1] = DIRCH;     // restore dir char

        ag_scm_c_eval_string_from_file_line(cmdbf, __FILE__, __LINE__);
    }

    return SCM_UNDEFINED;
}


/*=gfunc out_push_new
 *
 * what:   purge and create output file
 * exparg: file-name, name of the file to create, optional
 *
 * doc:
 *  Leave the current output file open, but purge and create
 *  a new file that will remain open until a @code{pop} @code{delete}
 *  or @code{switch} closes it.  The file name is optional and, if omitted,
 *  the output will be sent to a temporary file that will be deleted when
 *  it is closed.
 *  @xref{output controls}.
=*/
SCM
ag_scm_out_push_new(SCM new_file)
{
    static char const write_mode[] = "w" FOPEN_BINARY_FLAG "+";

    if (AG_SCM_STRING_P(new_file)) {
        open_output_file(AG_SCM_CHARS(new_file), AG_SCM_STRLEN(new_file),
                         write_mode, 0);
        return SCM_UNDEFINED;
    }

    /*
     *  "ENABLE_FMEMOPEN" is defined if we have either fopencookie(3GNU) or
     *  funopen(3BSD) is available and autogen was not configured with fmemopen
     *  disabled.  We cannot use the POSIX fmemopen.
     */
#if defined(ENABLE_FMEMOPEN)
    if (! HAVE_OPT(NO_FMEMOPEN)) {
        char *     pzNewFile;
        tFpStack * p;

        /*
         *  This block is used IFF ENABLE_FMEMOPEN is defined and if
         *  --no-fmemopen is *not* selected on the command line.
         */
        p = (tFpStack*)AGALOC(sizeof(tFpStack), "out file stack");
        p->pPrev  = pCurFp;
        p->flags  = FPF_FREE;
        p->pFile  = ag_fmemopen(NULL, (ssize_t)0, "w" FOPEN_BINARY_FLAG "+");
        pzNewFile = (char*)MEM_FILE_STR;
        p->flags |= FPF_STATIC_NM | FPF_NOUNLINK | FPF_NOCHMOD;

        if (p->pFile == NULL)
            AG_CANT(OUT_PUSH_NEW_FAIL, pzNewFile);

        p->pzOutName = pzNewFile;
        outputDepth++;
        pCurFp    = p;

        if (OPT_VALUE_TRACE > TRACE_DEBUG_MESSAGE)
            fprintf(pfTrace, TRACE_OUT_PUSH_NEW, __func__, pzNewFile);
        return SCM_UNDEFINED;
    }
#endif

    /*
     *  Either --no-fmemopen was specified or we cannot use ag_fmemopen().
     */
    {
        static size_t tmplen;
        char *  tmp_fnm;
        int     tmpfd;

        if (pz_temp_tpl == NULL)
            ag_scm_make_tmp_dir();

        tmplen  = temp_tpl_dir_len + 10;
        tmp_fnm = ag_scribble(tmplen + 1);
        memcpy(tmp_fnm, pz_temp_tpl, tmplen + 1);
        tmpfd   = mkstemp(tmp_fnm);

        if (tmpfd < 0)
            AG_ABEND(aprf(OUT_PUSH_NEW_FAILED, pz_temp_tpl));

        open_output_file(tmp_fnm, tmplen, write_mode, FPF_TEMPFILE);
        close(tmpfd);
    }

    return SCM_UNDEFINED;
}

/*=gfunc out_switch
 *
 * what:   close and create new output
 * exparg: file-name, name of the file to create
 *
 * doc:
 *  Switch output files - close current file and make the current
 *  file pointer refer to the new file.  This is equivalent to
 *  @code{out-pop} followed by @code{out-push-new}, except that
 *  you may not pop the base level output file, but you may
 *  @code{switch} it.  @xref{output controls}.
=*/
SCM
ag_scm_out_switch(SCM new_file)
{
    struct utimbuf tbuf;
    char*  pzNewFile;

    if (! AG_SCM_STRING_P(new_file))
        return SCM_UNDEFINED;
    {
        size_t sz = AG_SCM_STRLEN(new_file);
        pzNewFile = AGALOC(sz + 1, "new file name");
        memcpy(pzNewFile, AG_SCM_CHARS(new_file), sz);
        pzNewFile[ sz ] = NUL;
    }

    /*
     *  IF no change, THEN ignore this
     */
    if (strcmp(pCurFp->pzOutName, pzNewFile) == 0) {
        AGFREE((void*)pzNewFile);
        return SCM_UNDEFINED;
    }

    make_readonly(fileno(pCurFp->pFile));

    /*
     *  Make sure we get a new file pointer!!
     *  and try to ensure nothing is in the way.
     */
    unlink(pzNewFile);
    if (  freopen(pzNewFile, "w" FOPEN_BINARY_FLAG "+", pCurFp->pFile)
       != pCurFp->pFile)

        AG_CANT(OUT_SWITCH_FAIL, pzNewFile);

    /*
     *  Set the mod time on the old file.
     */
    tbuf.actime  = time(NULL);
    tbuf.modtime = outTime;
    utime(pCurFp->pzOutName, &tbuf);
    if (OPT_VALUE_TRACE > TRACE_DEBUG_MESSAGE)
        fprintf(pfTrace, TRACE_OUT_SWITCH,
                __func__, pCurFp->pzOutName, pzNewFile);
    pCurFp->pzOutName = pzNewFile;  /* memory leak */

    return SCM_UNDEFINED;
}


/*=gfunc out_depth
 *
 * what: output file stack depth
 * doc:  Returns the depth of the output file stack.
 *       @xref{output controls}.
=*/
SCM
ag_scm_out_depth(void)
{
    return AG_SCM_INT2SCM(outputDepth);
}


/*=gfunc out_name
 *
 * what: current output file name
 * doc:  Returns the name of the current output file.  If the current file
 *       is a temporary, unnamed file, then it will scan up the chain until
 *       a real output file name is found.
 *       @xref{output controls}.
=*/
SCM
ag_scm_out_name(void)
{
    tFpStack* p = pCurFp;
    while (p->flags & FPF_UNLINK)  p = p->pPrev;
    return AG_SCM_STR02SCM((void*)p->pzOutName);
}


/*=gfunc out_line
 *
 * what: output file line number
 * doc:  Returns the current line number of the output file.
 *       It rewinds and reads the file to count newlines.
=*/
SCM
ag_scm_out_line(void)
{
    int lineNum = 1;

    do {
        long svpos = ftell(pCurFp->pFile);
        long pos   = svpos;

        if (pos == 0)
            break;

        rewind(pCurFp->pFile);
        do {
            int ich = fgetc(pCurFp->pFile);
            unsigned char ch = ich;
            if (ich < 0)
                break;
            if (ch == (unsigned char)NL)
                lineNum++;
        } while (--pos > 0);
        fseek(pCurFp->pFile, svpos, SEEK_SET);
    } while(0);

    return AG_SCM_INT2SCM(lineNum);
}

#if 0 /* for compilers that do not like C++ comments... */
// This is done so that comment delimiters can be included in the doc.
//
// /*=gfunc   make_header_guard
//  *
//  * what:   make self-inclusion guard
//  *
//  * exparg: name , header group name
//  *
//  * doc:
//  * This function will create a @code{#ifndef}/@code{#define}
//  * sequence for protecting a header from multiple evaluation.
//  * It will also set the Scheme variable @code{header-file}
//  * to the name of the file being protected and it will set
//  * @code{header-guard} to the name of the @code{#define} being
//  * used to protect it.  It is expected that this will be used
//  * as follows:
//  * @example
//  * [+ (make-header-guard "group_name") +]
//  * ...
//  * #endif /* [+ (. header-guard) +] */
//  *
//  * #include "[+ (. header-file)  +]"
//  * @end example
//  * @noindent
//  * The @code{#define} name is composed as follows:
//  *
//  * @enumerate
//  * @item
//  * The first element is the string argument and a separating underscore.
//  * @item
//  * That is followed by the name of the header file with illegal
//  * characters mapped to underscores.
//  * @item
//  * The end of the name is always, "@code{_GUARD}".
//  * @item
//  * Finally, the entire string is mapped to upper case.
//  * @end enumerate
//  *
//  * The final @code{#define} name is stored in an SCM symbol named
//  * @code{header-guard}.  Consequently, the concluding @code{#endif} for the
//  * file should read something like:
//  *
//  * @example
//  * #endif /* [+ (. header-guard) +] */
//  * @end example
//  *
//  * The name of the header file (the current output file) is also stored
//  * in an SCM symbol, @code{header-file}.  Therefore, if you are also
//  * generating a C file that uses the previously generated header file,
//  * you can put this into that generated file:
//  *
//  * @example
//  * #include "[+ (. header-file) +]"
//  * @end example
//  *
//  * Obviously, if you are going to produce more than one header file from
//  * a particular template, you will need to be careful how these SCM symbols
//  * get handled.
// =*/
#endif
SCM
ag_scm_make_header_guard(SCM name)
{

    char const * opz; // output file name string
    size_t       osz;

    char const * gpz; // guard name string
    size_t       gsz;

    {
        tFpStack* p = pCurFp;
        while (p->flags & FPF_UNLINK)  p = p->pPrev;
        opz = p->pzOutName;
        osz = strlen(opz);
    }

    /*
     *  Construct the gard name using the leader (passed in or "HEADER")
     *  and the trailer (always "_GUARD") and the output file name in between.
     */
    {
        /*
         *  Leader string and its length.  Usually passed, but defaults
         *  to "HEADER"
         */
        char const * lpz =
            AG_SCM_STRING_P(name) ? AG_SCM_CHARS(name) : HEADER_STR;
        size_t lsz = (lpz == HEADER_STR)
            ? HEADER_STR_LEN : AG_SCM_STRLEN(name);

        /*
         *  Full, maximal length of output
         */
        size_t hsz = lsz + osz + GUARD_SFX_LEN + 2;
        char * scan_p;

        /*
         * Sanity:
         */
        if (*lpz == NUL) {
            lpz = HEADER_STR;
            lsz = HEADER_STR_LEN;
            hsz += lsz;
        }
        scan_p = AGALOC(hsz, "header guard string");

        gpz = scan_p;  // gpz must be freed
        do  {
            *(scan_p++) = toupper(*(lpz++));
        } while (--lsz > 0);

        /*
         *  This copy converts non-alphanumerics to underscores,
         *  but never inserts more than one at a time.  Thus, we may
         *  not use all of the space in "gpz".
         */
        lpz = opz;
        do  {
            *(scan_p++) = '_';
            lpz = BRK_ALPHANUMERIC_CHARS(lpz);
            while (IS_ALPHANUMERIC_CHAR(*lpz))
                *(scan_p++) = toupper(*(lpz++));
        } while (*lpz != NUL);

        memcpy(scan_p, GUARD_SFX, GUARD_SFX_LEN + 1);
        gsz = (scan_p - gpz) + GUARD_SFX_LEN;
    }

    {
        size_t sz1 = MK_HEAD_GUARD_SCM_LEN + gsz + osz;
        size_t sz2 = MK_HEAD_GUARD_GUARD_LEN + 2 * gsz;
        size_t sz  = (sz1 < sz2) ? sz2 : sz1;
        char * p = ag_scribble(sz);
        sprintf(p, MK_HEAD_GUARD_SCM, opz, gpz);
        (void)ag_scm_c_eval_string_from_file_line(p, __FILE__, __LINE__);

        sprintf(p, MK_HEAD_GUARD_GUARD, gpz);
        name = AG_SCM_STR02SCM(p);
    }

    AGFREE(gpz);
    return (name);
}

/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * indent-tabs-mode: nil
 * End:
 * end of agen5/expOutput.c */
