
/**
 * @file expState.c
 *
 *  This module implements expression functions that
 *  query and get state information from AutoGen data.
 *
 *  Time-stamp:        "2012-01-29 20:34:50 bkorb"
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

#ifdef SCM_HAVE_T_UINT64
  typedef uint64_t ver_type_t;
# define VER_UNIT_SHIFT   16ULL
# if ((SCM_MAJOR_VERSION * 100) + SCM_MINOR_VERSION) >= 108
#  define SCM_FROM(v)  scm_from_uint64(v)
# else
#  define SCM_FROM(v)  gh_ulong2scm((unsigned long)v)
# endif

#else
  typedef uint32_t ver_type_t;
# define VER_UNIT_SHIFT   8
# ifdef HAVE_SCM_FROM_UINT32
#  define SCM_FROM(v)  scm_from_uint32(v)
# else
#  define SCM_FROM(v)  gh_ulong2scm((unsigned long)v)
# endif
#endif

/* = = = START-STATIC-FORWARD = = = */
static int
entry_length(char* name);

static int
count_entries(char* name);

static SCM
find_entry_value(SCM op, SCM obj, SCM test);

static ver_type_t
str2int_ver(char* pz);

static SCM
do_tpl_file_line(int line_delta, char const * fmt);
/* = = = END-STATIC-FORWARD = = = */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  EXPRESSION EVALUATION SUPPORT ROUTINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static int
entry_length(char* name)
{
    tDefEntry**  papDefs = findEntryList(name);
    int          res     = 0;

    if (papDefs == NULL)
        return 0;

    for (;;) {
        tDefEntry*   pDE = *(papDefs++);
        if (pDE == NULL)
            break;
        if (pDE->valType == VALTYP_TEXT)
            res += strlen(pDE->val.pzText);
        else
            res++;
    }
    return res;
}


static int
count_entries(char* name)
{
    tDefEntry**  papDefs = findEntryList(name);
    int          res     = 0;

    if (papDefs == NULL)
        return 0;

    for (;;) {
        tDefEntry*   pDE = *(papDefs++);
        if (pDE == NULL)
            break;
        res++;
    }
    return res;
}

/**
 * Find a definition with a specific value
 */
static SCM
find_entry_value(SCM op, SCM obj, SCM test)
{
    ag_bool     isIndexed;
    tDefEntry*  pE;
    char*       pzField;

    {
        char * name = ag_scm2zchars(obj, "find name");

        if (OPT_VALUE_TRACE >= TRACE_EXPRESSIONS)
            fprintf(pfTrace, TRACE_FIND_ENT, name);

        pzField = strchr(name, name_sep_ch);
        if (pzField != NULL)
            *(pzField++) = NUL;

        pE = findDefEntry(name, &isIndexed);
    }

    /*
     *  No such entry?  return FALSE
     */
    if (pE == NULL) {
        if (OPT_VALUE_TRACE >= TRACE_EXPRESSIONS)
            fputs(FIND_ENT_FAIL, pfTrace);
        return SCM_BOOL_F;
    }

    /*
     *  No subfield?  Check the values
     */
    if (pzField == NULL) {
        SCM result;
        SCM field;
        if (pE->valType != VALTYP_TEXT) {
            if (OPT_VALUE_TRACE >= TRACE_EXPRESSIONS)
                fputs(FIND_ENT_FAIL, pfTrace);
            return SCM_BOOL_F; /* Cannot match string -- not a text value */
        }

        field  = AG_SCM_STR02SCM(pE->val.pzText);
        result = AG_SCM_APPLY2(op, field, test);
        if (! isIndexed)
            while (result == SCM_BOOL_F) {

                pE = pE->pTwin;
                if (pE == NULL)
                    break;

                field = AG_SCM_STR02SCM(pE->val.pzText);
                result = AG_SCM_APPLY2(op, field, test);
            }

        if (OPT_VALUE_TRACE >= TRACE_EXPRESSIONS)
            fputs((result == SCM_BOOL_T) ? FIND_ENT_SUCC : FIND_ENT_FAIL,
                  pfTrace);
        return result;
    }

    /*
     *  a subfield for a text macro?  return FALSE
     */
    if (pE->valType == VALTYP_TEXT) {
        if (OPT_VALUE_TRACE >= TRACE_EXPRESSIONS)
            fputs(FIND_ENT_FAIL, pfTrace);
        return SCM_BOOL_F;
    }

    /*
     *  Search the members for what we want.
     */
    pzField[-1] = name_sep_ch;
    {
        SCM field   = AG_SCM_STR02SCM(pzField);
        SCM result;
        tDefCtx ctx = currDefCtx;

        currDefCtx.pPrev = &ctx;
        currDefCtx.pDefs = pE->val.pDefEntry;

        result = find_entry_value(op, field, test);

        if (! isIndexed)
            while (result == SCM_BOOL_F) {

                pE = pE->pTwin;
                if (pE == NULL)
                    break;

                currDefCtx.pDefs = pE->val.pDefEntry;
                result = find_entry_value(op, field, test);
            }

        currDefCtx = ctx;
        return result;
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  EXPRESSION ROUTINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*=gfunc base_name
 *
 * what:   base output name
 *
 * doc:  Returns a string containing the base name of the output file(s).
 *       Generally, this is also the base name of the definitions file.
=*/
SCM
ag_scm_base_name(void)
{
    return AG_SCM_STR02SCM((char*)(void*)OPT_ARG(BASE_NAME));
}

/*=gfunc version_compare
 *
 * what:   compare two version numbers
 * general_use:
 *
 * exparg: op, comparison operator
 * exparg: v1, first version
 * exparg: v2, compared-to version
 *
 * doc:  Converts v1 and v2 strings into 64 bit values and returns the
 *       result of running 'op' on those values.  It assumes that the version
 *       is a 1 to 4 part dot-separated series of numbers.  Suffixes like,
 *       "5pre4" or "5-pre4" will be interpreted as two numbers.  The first
 *       number ("5" in this case) will be decremented and the number after
 *       the "pre" will be added to 0xC000.  (Unless your platform is unable
 *       to support 64 bit integer arithmetic.  Then it will be added to 0xC0.)
 *       Consequently, these yield true:
 *       @example
 *       (version-compare > "5.8.5"       "5.8.5-pre4")
 *       (version-compare > "5.8.5-pre10" "5.8.5-pre4")
 *       @end example
=*/
static ver_type_t
str2int_ver(char* pz)
{
    char* pzStr = pz;
    ver_type_t  val = 0;
    int ix = 4;

    while (--ix >= 0) {
        unsigned int v;
        val <<= VER_UNIT_SHIFT;
        pz = SPN_WHITESPACE_CHARS(pz);

    next_number:
        if (! IS_DEC_DIGIT_CHAR(*pz)) break;
        v = (unsigned int)strtoul(pz, &pz, 0) & ((1 << VER_UNIT_SHIFT) - 1);
        if (pz == NULL)
            break;
        val += v;
        if (*pz == '-') pz++;

        switch (*pz) {
        case 'p':
            if ((pz[1] == 'r') && (pz[2] == 'e')) {
                pz += 3;
                val = (val << 2) - 1;
                val <<= (VER_UNIT_SHIFT - 2);
                if (--ix < 0) goto leave_str2int_ver;
                goto next_number;
            }
            /* FALLTHROUGH */

        default:
            goto leave_str2int_ver;

        case '.':
            if (! IS_DEC_DIGIT_CHAR(*(++pz)))
                goto leave_str2int_ver;
            break;
        }
    } leave_str2int_ver: ;

    while (--ix >= 0)  val <<= VER_UNIT_SHIFT;
    if (OPT_VALUE_TRACE >= TRACE_EXPRESSIONS)
        fprintf(pfTrace, TRACE_VER_CONVERT, (long long)val, pzStr);
    return val;
}

/**
 * Convert version number strings into a binary representation and compare.
 */
SCM
ag_scm_version_compare(SCM op, SCM v1, SCM v2)
{
    ver_type_t val1 = str2int_ver(ag_scm2zchars(v1, "ver"));
    ver_type_t val2 = str2int_ver(ag_scm2zchars(v2, "ver"));
    v1 = SCM_FROM(val1);
    v2 = SCM_FROM(val2);
    return scm_apply(op, v1, scm_cons(v2, AG_SCM_LISTOFNULL()));
}

/*=gfunc count
 *
 * what:   definition count
 *
 * exparg: ag-name, name of AutoGen value
 *
 * doc:  Count the number of entries for a definition.
 *      The input argument must be a string containing the name
 *      of the AutoGen values to be counted.  If there is no
 *      value associated with the name, the result is an SCM
 *      immediate integer value of zero.
=*/
SCM
ag_scm_count(SCM obj)
{
    int ent_len = count_entries(ag_scm2zchars(obj, "ag object"));

    return AG_SCM_INT2SCM(ent_len);
}


/*=gfunc def_file
 *
 * what:   definitions file name
 *
 * doc:  Get the name of the definitions file.
 *       Returns the name of the source file containing the AutoGen
 *       definitions.
=*/
SCM
ag_scm_def_file(void)
{
    return AG_SCM_STR02SCM((char*)(void*)pBaseCtx->pzCtxFname);
}


/*=gfunc exist_p
 *
 * what:   test for value name
 *
 * exparg: ag-name, name of AutoGen value
 *
 * doc:  return SCM_BOOL_T iff a specified name has an AutoGen value.
 *       The name may include indexes and/or member names.
 *       All but the last member name must be an aggregate definition.
 *       For example:
 *       @example
 *       (exist? "foo[3].bar.baz")
 *       @end example
 *       will yield true if all of the following is true:
 *       @*
 *       There is a member value of either group or string type
 *       named @code{baz} for some group value @code{bar} that
 *       is a member of the @code{foo} group with index @code{3}.
 *       There may be multiple entries of @code{bar} within
 *       @code{foo}, only one needs to contain a value for @code{baz}.
=*/
SCM
ag_scm_exist_p(SCM obj)
{
    ag_bool x;
    SCM     res;

    if (findDefEntry(ag_scm2zchars(obj, "ag object"), &x) == NULL)
         res = SCM_BOOL_F;
    else res = SCM_BOOL_T;

    return res;
}


/*=gfunc ag_function_p
 *
 * what:   test for function
 *
 * exparg: ag-name, name of AutoGen macro
 *
 * doc:  return SCM_BOOL_T if a specified name is a user-defined AutoGen
 *       macro, otherwise return SCM_BOOL_F.
=*/
SCM
ag_scm_ag_function_p(SCM obj)
{
    SCM     res;

    if (findTemplate(ag_scm2zchars(obj, "ag user macro")) == NULL)
         res = SCM_BOOL_F;
    else res = SCM_BOOL_T;

    return res;
}


/*=gfunc match_value_p
 *
 * what:   test for matching value
 *
 * exparg: op,       boolean result operator
 * exparg: ag-name,  name of AutoGen value
 * exparg: test-str, string to test against
 *
 * doc:  This function answers the question, "Is there an AutoGen value named
 *       @code{ag-name} with a value that matches the pattern @code{test-str}
 *       using the match function @code{op}?"  Return SCM_BOOL_T iff at least
 *       one occurrence of the specified name has such a value.  The operator
 *       can be any function that takes two string arguments and yields a
 *       boolean.  It is expected that you will use one of the string matching
 *       functions provided by AutoGen.
 *       @*
 *       The value name must follow the same rules as the
 *       @code{ag-name} argument for @code{exist?} (@pxref{SCM exist?}).
=*/
SCM
ag_scm_match_value_p(SCM op, SCM obj, SCM test)
{
    if (  (! AG_SCM_IS_PROC(op))
       || (! AG_SCM_STRING_P(obj)) )
        return SCM_UNDEFINED;

    if (OPT_VALUE_TRACE >= TRACE_EXPRESSIONS)
        fprintf(pfTrace, TRACE_MATCH_VAL, ag_scm2zchars(test, "test val"));

    return find_entry_value(op, obj, test);
}


/*=gfunc get
 *
 * what:   get named value
 *
 * exparg: ag-name, name of AutoGen value
 * exparg: alt-val, value if not present, optional
 *
 * doc:
 *  Get the first string value associated with the name.
 *  It will either return the associated string value (if
 *  the name resolves), the alternate value (if one is provided),
 *  or else the empty string.
=*/
SCM
ag_scm_get(SCM agName, SCM altVal)
{
    tDefEntry*  pE;
    ag_bool     x;

    pE = (! AG_SCM_STRING_P(agName)) ? NULL :
        findDefEntry(ag_scm2zchars(agName, "ag value"), &x);

    if ((pE == NULL) || (pE->valType != VALTYP_TEXT)) {
        if (AG_SCM_STRING_P(altVal))
            return altVal;
        return AG_SCM_STR02SCM(zNil);
    }

    return AG_SCM_STR02SCM(pE->val.pzText);
}


/*=gfunc get_c_name
 *
 * what:   get named value, mapped to C name syntax
 *
 * exparg: ag-name, name of AutoGen value
 *
 * doc:
 *
 *  Get the first string value associated with the name.  It will either
 *  return the associated string value (if the name resolves), the alternate
 *  value (if one is provided), or else the empty string.  The result is
 *  passed through "string->c-name!".
=*/
SCM
ag_scm_get_c_name(SCM agName)
{
    return ag_scm_string_to_c_name_x(
        ag_scm_get(agName, SCM_UNDEFINED));
}


/*=gfunc get_up_name
 *
 * what:   get upper cased named value, mapped to C name syntax
 *
 * exparg: ag-name, name of AutoGen value
 *
 * doc:
 *
 *  Get the first string value associated with the name.  It will either
 *  return the associated string value (if the name resolves), the alternate
 *  value (if one is provided), or else the empty string.  The result is
 *  passed through "string->c-name!" and "string->up-case!".
=*/
SCM
ag_scm_get_up_name(SCM agName)
{
    return ag_scm_string_upcase_x(ag_scm_get_c_name(agName));
}


/*=gfunc get_down_name
 *
 * what:   get lower cased named value, mapped to C name syntax
 *
 * exparg: ag-name, name of AutoGen value
 *
 * doc:
 *
 *  Get the first string value associated with the name.  It will either
 *  return the associated string value (if the name resolves), the alternate
 *  value (if one is provided), or else the empty string.  The result is
 *  passed through "string->c-name!" and "string->down-case!".
=*/
SCM
ag_scm_get_down_name(SCM agName)
{
    return ag_scm_string_downcase_x(ag_scm_get_c_name(agName));
}


/*=gfunc high_lim
 *
 * what:   get highest value index
 *
 * exparg: ag-name, name of AutoGen value
 *
 * doc:
 *
 *  Returns the highest index associated with an array of definitions.
 *  This is generally, but not necessarily, one less than the
 *  @code{count} value.  (The indexes may be specified, rendering a
 *  non-zero based or sparse array of values.)
 *
 *  This is very useful for specifying the size of a zero-based array
 *  of values where not all values are present.  For example:
 *
 *  @example
 *  tMyStruct myVals[ [+ (+ 1 (high-lim "my-val-list")) +] ];
 *  @end example
=*/
SCM
ag_scm_high_lim(SCM obj)
{
    tDefEntry*  pE;
    ag_bool     isIndexed;

    pE = findDefEntry(ag_scm2zchars(obj, "ag value"), &isIndexed);

    /*
     *  IF we did not find the entry we are looking for
     *  THEN return zero
     *  ELSE search the twin list for the high entry
     */
    if (pE == NULL)
        return AG_SCM_INT2SCM(0);

    if (isIndexed)
        return AG_SCM_INT2SCM((int)pE->index);

    if (pE->pEndTwin != NULL)
        pE = pE->pEndTwin;

    return AG_SCM_INT2SCM((int)pE->index);
}


/*=gfunc len
 *
 * what:   get count of values
 *
 * exparg: ag-name, name of AutoGen value
 *
 * doc:  If the named object is a group definition, then "len" is
 *       the same as "count".  Otherwise, if it is one or more text
 *       definitions, then it is the sum of their string lengths.
 *       If it is a single text definition, then it is equivalent to
 *       @code{(string-length (get "ag-name"))}.
=*/
SCM
ag_scm_len(SCM obj)
{
    int len = entry_length(ag_scm2zchars(obj, "ag value"));

    return AG_SCM_INT2SCM(len);
}


/*=gfunc low_lim
 *
 * what:   get lowest value index
 *
 * exparg: ag-name, name of AutoGen value
 *
 * doc:  Returns the lowest index associated with an array of definitions.
=*/
SCM
ag_scm_low_lim(SCM obj)
{
    tDefEntry*  pE;
    ag_bool     x;

    pE = findDefEntry(ag_scm2zchars(obj, "ag value"), &x);

    /*
     *  IF we did not find the entry we are looking for
     *  THEN return zero
     *  ELSE we have the low index.
     */
    if (pE == NULL)
        return AG_SCM_INT2SCM(0);

    return AG_SCM_INT2SCM((int)pE->index);
}


/*=gfunc set_option
 *
 * what:  Set a command line option
 *
 * exparg: opt, AutoGen option name + its argument
 *
 * doc:   The text argument must be an option name followed by any needed
 *        option argument.  Returns SCM_UNDEFINED.
=*/
SCM
ag_scm_set_option(SCM opt)
{
    optionLoadLine(&autogenOptions, ag_scm2zchars(opt, "opt + arg"));
    return SCM_UNDEFINED;
}


/*=gfunc suffix
 *
 * what:   get the current suffix
 *
 * doc:
 *  Returns the current active suffix (@pxref{pseudo macro}).
=*/
SCM
ag_scm_suffix(void)
{
    return AG_SCM_STR02SCM((char*)pzCurSfx);
}


/*=gfunc tpl_file
 *
 * what:   get the template file name
 *
 * exparg: full_path, include full path to file, optonal
 *
 * doc:  Returns the name of the current template file.
 *       If @code{#t} is passed in as an argument, then the template
 *       file is hunted for in the template search path.  Otherwise,
 *       just the unadorned name.
=*/
SCM
ag_scm_tpl_file(SCM full)
{
    if (AG_SCM_BOOL_P(full) && AG_SCM_NFALSEP(full)) {
        static char const * const sfx[] = { TPL_FILE_TPL, NULL };

        char z[AG_PATH_MAX];
        if (SUCCESSFUL(findFile(pzTemplFileName, z, sfx, NULL)))
            return AG_SCM_STR02SCM(z);
    }

    return AG_SCM_STR02SCM((char*)(void*)pzTemplFileName);
}

/**
 * guts of the template file/line functions
 */
static SCM
do_tpl_file_line(int line_delta, char const * fmt)
{
    void * args[2] = {
        [0] = (void*)pCurTemplate->pzTplFile,
        [1] = (void*)((long)pCurMacro->lineNo + line_delta)
    };
    char * buf = strrchr(args[0], DIRCH);
    if (buf != NULL)
        args[0] = buf + 1;

    {
        size_t sz = strlen(fmt) + strlen(args[0]) + 24;
        buf = ag_scribble(sz);
    }

    sprintfv(buf, fmt, (snv_constpointer*)args);
    return AG_SCM_STR02SCM(buf);
}

/*=gfunc tpl_file_line
 *
 * what:   get the template file+line number
 *
 * exparg: msg-fmt, formatting for line message, optional
 *
 * doc:
 *  Returns the file and line number of the current template macro using
 *  either the default format, "from %s line %d", or else the format you
 *  supply.  For example, if you want to insert a "C" language file-line
 *  directive, you would supply the format "# %2$d \"%1$s\"", but that
 *  is also already supplied with the scheme variable
 *  @xref{SCM c-file-line-fmt}.  You may use it thus:
 *  @example
 *  (tpl-file-line c-file-line-fmt)
 *  @end example
 *
 *  It is also safe to use the formatting string, "%2$d".  AutoGen uses
 *  an argument vector version of printf: @xref{snprintfv},
 *  and it does not need to know the types of each argument in order to
 *  skip forward to the second argument.
=*/
SCM
ag_scm_tpl_file_line(SCM fmt)
{
    char const * pzFmt = TPL_FILE_LINE_FMT;
    if (AG_SCM_STRING_P(fmt))
        pzFmt = ag_scm2zchars(fmt, "f/l fmt");

    return do_tpl_file_line(0, pzFmt);
}

/*=gfunc tpl_file_next_line
 *
 * what:   get the template file plus next line number
 *
 * exparg: msg-fmt, formatting for line message, optional
 *
 * doc:
 *  This is almost the same as @xref{SCM tpl-file-line}, except that
 *  the line referenced is the next line, per C compiler conventions, and
 *  consequently defaults to the format:  # <line-no+1> "<file-name>"
=*/
SCM
ag_scm_tpl_file_next_line(SCM fmt)
{
    char const * pzFmt = TPL_FILE_NEXT_LINE_FMT;
    if (AG_SCM_STRING_P(fmt))
        pzFmt = ag_scm2zchars(fmt, "f/l fmt");

    return do_tpl_file_line(1, pzFmt);
}

/*=gfunc def_file_line
 *
 * what:   get a definition file+line number
 *
 * exparg: ag-name, name of AutoGen value
 * exparg: msg-fmt, formatting for line message, optional
 *
 * doc:
 *  Returns the file and line number of a AutoGen defined value, using
 *  either the default format, "from %s line %d", or else the format you
 *  supply.  For example, if you want to insert a "C" language file-line
 *  directive, you would supply the format "# %2$d \"%1$s\"", but that
 *  is also already supplied with the scheme variable
 *  @xref{SCM c-file-line-fmt}.  You may use it thus:
 *
 *  @example
 *  (def-file-line "ag-def-name" c-file-line-fmt)
 *  @end example
 *
 *  It is also safe to use the formatting string, "%2$d".  AutoGen uses
 *  an argument vector version of printf: @xref{snprintfv}.
=*/
SCM
ag_scm_def_file_line(SCM obj, SCM fmt)
{
    char const * pzFmt = DEF_FILE_LINE_FMT;
    char * buf;
    ag_bool     x;

    tDefEntry * pE = findDefEntry(ag_scm2zchars(obj, "ag value"), &x);

    /*
     *  IF we did not find the entry we are looking for
     *  THEN return UNDEFINED
     */
    if (pE == NULL)
        return SCM_UNDEFINED;

    if (AG_SCM_STRING_P(fmt))
        pzFmt = ag_scm2zchars(fmt, "f/l fmt");

    {
        void * args[2] = {
            (void*)pE->pzSrcFile,
            (void*)(long)pE->srcLineNum
        };
        size_t maxlen;

        buf = strrchr(args[0], DIRCH);
        if (buf != NULL)
            args[0] = buf + 1;

        maxlen = strlen(args[0]) + strlen(pzFmt) + LOG10_2to32 + 1;
        buf = ag_scribble(maxlen);
        sprintfv(buf, pzFmt, (snv_constpointer*)args);
    }

    return AG_SCM_STR02SCM(buf);
}
/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * indent-tabs-mode: nil
 * End:
 * end of agen5/expState.c */
