
/**
 * @file funcCase.c
 *
 *  This module implements the CASE text function.
 *
 *  Time-stamp:        "2012-01-29 08:10:09 bkorb"
 */
/*
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

#undef  IS_LOW
#define IS_LOW(c)  (((c) <= 'z') && ((c) >= 'a'))

#ifndef _toupper
#  ifdef __toupper
#    define _toupper(c)     __toupper(c)
#  else
#    define _toupper(c)     toupper(c)
#  endif
#endif

#define PTRUP(p) STMTS(if(IS_LOW(*(p))) *(p)=_toupper(*(p));(p)++)

typedef tSuccess (tSelectProc)(char const * sample, char const * pattern);
static tSelectProc
    Select_Compare,
    Select_Compare_End,
    Select_Compare_Start,
    Select_Compare_Full,
    Select_Equivalent,
    Select_Equivalent_End,
    Select_Equivalent_Start,
    Select_Equivalent_Full,
    Select_Match,
    Select_Match_End,
    Select_Match_Start,
    Select_Match_Full,
    Select_Match_Always;

/*
 *  This is global data used to keep track of the current CASE
 *  statement being processed.  When CASE statements nest,
 *  these data are copied onto the stack and restored when
 *  the nested CASE statement's ESAC function is found.
 */
typedef struct case_stack tCaseStack;
struct case_stack {
    tMacro*  pCase;
    tMacro*  pSelect;
};

static tCaseStack current_case;
static tLoadProc  mLoad_Select;

static tpLoadProc apCaseLoad[ FUNC_CT ]   = { NULL };
static tpLoadProc apSelectOnly[ FUNC_CT ] = { NULL };

/* = = = START-STATIC-FORWARD = = = */
static void
compile_re(regex_t* pRe, char const * pzPat, int flags);

static inline void
up_case(char* pz);

static tSuccess
Select_Compare(char const * sample, char const * pattern);

static tSuccess
Select_Compare_End(char const * sample, char const * pattern);

static tSuccess
Select_Compare_Start(char const * sample, char const * pattern);

static tSuccess
Select_Compare_Full(char const * sample, char const * pattern);

static tSuccess
Select_Equivalent(char const * sample, char const * pattern);

static tSuccess
Select_Equivalent_End(char const * sample, char const * pattern);

static tSuccess
Select_Equivalent_Start(char const * sample, char const * pattern);

static tSuccess
Select_Equivalent_Full(char const * sample, char const * pattern);

static tSuccess
Select_Match(char const * sample, char const * pattern);

static tSuccess
Select_Match_End(char const * sample, char const * pattern);

static tSuccess
Select_Match_Start(char const * sample, char const * pattern);

static tSuccess
Select_Match_Full(char const * sample, char const * pattern);

static tSuccess
Select_Match_Always(char const * sample, char const * pattern);

static tSuccess
Select_Match_Existence(char const * sample, char const * pattern);

static tSuccess
Select_Match_NonExistence(char const * sample, char const * pattern);

static tMacro*
mLoad_Select(tTemplate * pT, tMacro* pMac, char const ** ppzScan);
/* = = = END-STATIC-FORWARD = = = */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static void
compile_re(regex_t* pRe, char const * pzPat, int flags)
{
    void * const pat = (void *)pzPat;
    int  rerr = regcomp(pRe, pat, flags);
    if (rerr != 0) {
        char zEr[ SCRIBBLE_SIZE ];
        regerror(rerr, pRe, zEr, sizeof(zEr));
        fprintf(stderr, BAD_RE_FMT, rerr, zEr, pzPat);
        AG_ABEND(COMPILE_RE_BAD);
    }
}


static inline void
up_case(char* pz)
{
    while (*pz != NUL) PTRUP(pz);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*=gfunc string_contains_p
 *
 * what:   substring match
 * general_use:
 * exparg: text, text to test for pattern
 * exparg: match, pattern/substring to search for
 *
 * string: "*==*"
 *
 * doc:  Test to see if a string contains a substring.  "strstr(3)"
 *       will find an address.
=*/
static tSuccess
Select_Compare(char const * sample, char const * pattern)
{
    return (strstr(sample, pattern)) ? SUCCESS : FAILURE;
}

SCM
ag_scm_string_contains_p(SCM text, SCM substr)
{
    char* pzText   = ag_scm2zchars(text, "text to match");
    char* pzSubstr = ag_scm2zchars(substr, "match expr");

    return (strstr(pzText, pzSubstr) == NULL) ? SCM_BOOL_F : SCM_BOOL_T;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*=gfunc string_ends_with_p
 *
 * what:   string ending
 * general_use:
 * exparg: text, text to test for pattern
 * exparg: match, pattern/substring to search for
 *
 * string: "*=="
 *
 * doc:  Test to see if a string ends with a substring.
 *       strcmp(3) returns zero for comparing the string ends.
=*/
static tSuccess
Select_Compare_End(char const * sample, char const * pattern)
{
    size_t   vlen = strlen(pattern);
    size_t   tlen = strlen(sample);
    tSuccess res;

    if (tlen < vlen)
        res = FAILURE;
    else if (strcmp(sample + (tlen - vlen), pattern) == 0)
         res = SUCCESS;
    else res = FAILURE;

    return res;
}

SCM
ag_scm_string_ends_with_p(SCM text, SCM substr)
{
    char* pzText   = ag_scm2zchars(text, "text to match");
    char* pzSubstr = ag_scm2zchars(substr, "match expr");
    return (SUCCESSFUL(Select_Compare_End(pzText, pzSubstr)))
        ? SCM_BOOL_T : SCM_BOOL_F;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*=gfunc string_starts_with_p
 *
 * what:   string starting
 * general_use:
 *
 * exparg: text, text to test for pattern
 * exparg: match, pattern/substring to search for
 *
 * string: "==*"
 *
 * doc:  Test to see if a string starts with a substring.
=*/
static tSuccess
Select_Compare_Start(char const * sample, char const * pattern)
{
    size_t  vlen = strlen(pattern);
    tSuccess res;

    if (strncmp(sample, pattern, vlen) == 0)
         res = SUCCESS;
    else res = FAILURE;

    return res;
}

SCM
ag_scm_string_starts_with_p(SCM text, SCM substr)
{
    char* pzText   = ag_scm2zchars(text, "text to match");
    char* pzSubstr = ag_scm2zchars(substr, "match expr");
    return (SUCCESSFUL(Select_Compare_Start(pzText, pzSubstr)))
        ? SCM_BOOL_T : SCM_BOOL_F;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*=gfunc string_equals_p
 *
 * what:   string matching
 * general_use:
 *
 * exparg: text, text to test for pattern
 * exparg: match, pattern/substring to search for
 *
 * string: "=="
 *
 * doc:  Test to see if two strings exactly match.
=*/
static tSuccess
Select_Compare_Full(char const * sample, char const * pattern)
{
    return (strcmp(sample, pattern) == 0) ? SUCCESS : FAILURE;
}

SCM
ag_scm_string_equals_p(SCM text, SCM substr)
{
    char* pzText   = ag_scm2zchars(text, "text to match");
    char* pzSubstr = ag_scm2zchars(substr, "match expr");

    return (strcmp(pzText, pzSubstr) == 0) ? SCM_BOOL_T : SCM_BOOL_F;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*=gfunc string_contains_eqv_p
 *
 * what:   caseless substring
 * general_use:
 *
 * exparg: text, text to test for pattern
 * exparg: match, pattern/substring to search for
 *
 * string: "*=*"
 *
 * doc:  Test to see if a string contains an equivalent string.
 *       `equivalent' means the strings match, but without regard
 *       to character case and certain characters are considered `equivalent'.
 *       Viz., '-', '_' and '^' are equivalent.
=*/
static tSuccess
Select_Equivalent(char const * sample, char const * pattern)
{
    char*    pz;
    tSuccess res = SUCCESS;
    AGDUPSTR(pz, sample, "equiv chars");
    up_case(pz);
    if (strstr(pz, pattern) == NULL)
        res = FAILURE;
    AGFREE((void*)pz);

    return res;
}

SCM
ag_scm_string_contains_eqv_p(SCM text, SCM substr)
{
    char * pzSubstr;
    SCM    res;

    AGDUPSTR(pzSubstr, ag_scm2zchars(substr, "search"), "substr");

    up_case(pzSubstr);
    if (SUCCESSFUL(Select_Equivalent(ag_scm2zchars(text, "sample"),
                                     pzSubstr)))
         res = SCM_BOOL_T;
    else res = SCM_BOOL_F;
    AGFREE((void*)pzSubstr);
    return res;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*=gfunc string_ends_eqv_p
 *
 * what:   caseless string ending
 * general_use:
 *
 * exparg: text, text to test for pattern
 * exparg: match, pattern/substring to search for
 *
 * string: "*="
 *
 * doc:  Test to see if a string ends with an equivalent string.
=*/
static tSuccess
Select_Equivalent_End(char const * sample, char const * pattern)
{
    size_t   vlen = strlen(pattern);
    size_t   tlen = strlen(sample);

    if (tlen < vlen)
        return FAILURE;

    return (streqvcmp(sample + (tlen - vlen), pattern) == 0)
           ? SUCCESS
           : FAILURE;
}

SCM
ag_scm_string_ends_eqv_p(SCM text, SCM substr)
{
    char* pzText   = ag_scm2zchars(text, "text to match");
    char* pzSubstr = ag_scm2zchars(substr, "match expr");
    return (SUCCESSFUL(Select_Equivalent_End( pzText, pzSubstr )))
        ? SCM_BOOL_T : SCM_BOOL_F;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*=gfunc string_starts_eqv_p
 *
 * what:   caseless string start
 * general_use:
 *
 * exparg: text, text to test for pattern
 * exparg: match, pattern/substring to search for
 *
 * string: "=*"
 *
 * doc:  Test to see if a string starts with an equivalent string.
=*/
static tSuccess
Select_Equivalent_Start(char const * sample, char const * pattern)
{
    size_t   vlen = strlen(pattern);

    return (strneqvcmp(sample, pattern, (int)vlen) == 0)
           ? SUCCESS
           : FAILURE;
}

SCM
ag_scm_string_starts_eqv_p(SCM text, SCM substr)
{
    char* pzText   = ag_scm2zchars(text, "text to match");
    char* pzSubstr = ag_scm2zchars(substr, "match expr");
    return (SUCCESSFUL(Select_Equivalent_Start(pzText, pzSubstr)))
        ? SCM_BOOL_T : SCM_BOOL_F;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*=gfunc string_eqv_p
 *
 * what:   caseless string match
 * general_use:
 *
 * exparg: text, text to test for pattern
 * exparg: match, pattern/substring to search for
 *
 * string: "="
 *
 * doc:  Test to see if two strings are equivalent.  `equivalent' means the
 *       strings match, but without regard to character case and certain
 *       characters are considered `equivalent'.  Viz., '-', '_' and '^' are
 *       equivalent.  If the arguments are not strings, then the result of the
 *       numeric comparison is returned.
 *
 *       This is an overloaded operation.  If the arguments are not both
 *       strings, then the query is passed through to @code{scm_num_eq_p()}.
=*/
static tSuccess
Select_Equivalent_Full(char const * sample, char const * pattern)
{
    return (streqvcmp(sample, pattern) == 0) ? SUCCESS : FAILURE;
}

SCM
ag_scm_string_eqv_p(SCM text, SCM substr)
{
    char* pzText;
    char* pzSubstr;

    /*
     *  We are overloading the "=" operator.  Our arguments may be
     *  numbers...
     */
    if (! AG_SCM_STRING_P(text) || ! AG_SCM_STRING_P(substr))
        return scm_num_eq_p(text, substr);

    pzText   = ag_scm2zchars(text, "text to match");
    pzSubstr = ag_scm2zchars(substr, "match expr");

    return (streqvcmp(pzText, pzSubstr) == 0) ? SCM_BOOL_T : SCM_BOOL_F;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*=gfunc string_has_match_p
 *
 * what:   contained regex match
 * general_use:
 *
 * exparg: text, text to test for pattern
 * exparg: match, pattern/substring to search for
 *
 * string: "*~~*"
 *
 * doc:  Test to see if a string contains a pattern.
 *       Case is significant.
=*/
/*=gfunc string_has_eqv_match_p
 *
 * what:   caseless regex contains
 * general_use:
 *
 * exparg: text, text to test for pattern
 * exparg: match, pattern/substring to search for
 *
 * string: "*~*"
 *
 * doc:  Test to see if a string contains a pattern.
 *       Case is not significant.
=*/
static tSuccess
Select_Match(char const * sample, char const * pattern)
{
    /*
     *  On the first call for this macro, compile the expression
     */
    if (pCurMacro->funcPrivate == NULL) {
        void *    mat = (void *)pattern;
        regex_t*  pRe = AGALOC(sizeof(*pRe), "select match re");
        compile_re(pRe, mat, (int)pCurMacro->res);
        pCurMacro->funcPrivate = (void*)pRe;
    }

    if (regexec((regex_t*)pCurMacro->funcPrivate, sample, (size_t)0,
                 NULL, 0) != 0)
        return FAILURE;
    return SUCCESS;
}

SCM
ag_scm_string_has_match_p(SCM text, SCM substr)
{
    SCM      res;
    regex_t  re;

    compile_re(&re, ag_scm2zchars( substr, "match expr" ), REG_EXTENDED);

    if (regexec(&re, ag_scm2zchars(text, "text to match"), (size_t)0,
                 NULL, 0) == 0)
         res = SCM_BOOL_T;
    else res = SCM_BOOL_F;
    regfree(&re);

    return res;
}

SCM
ag_scm_string_has_eqv_match_p(SCM text, SCM substr)
{
    char* pzText   = ag_scm2zchars(text, "text to match");
    char* pzSubstr = ag_scm2zchars(substr, "match expr");
    SCM      res;
    regex_t  re;

    compile_re(&re, pzSubstr, REG_EXTENDED | REG_ICASE);

    if (regexec(&re, pzText, (size_t)0, NULL, 0) == 0)
         res = SCM_BOOL_T;
    else res = SCM_BOOL_F;
    regfree(&re);

    return res;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*=gfunc string_end_match_p
 *
 * what:   regex match end
 * general_use:
 *
 * exparg: text, text to test for pattern
 * exparg: match, pattern/substring to search for
 *
 * string: "*~~"
 *
 * doc:  Test to see if a string ends with a pattern.
 *       Case is significant.
=*/
/*=gfunc string_end_eqv_match_p
 *
 * what:   caseless regex ending
 * general_use:
 *
 * exparg: text, text to test for pattern
 * exparg: match, pattern/substring to search for
 *
 * string: "*~"
 *
 * doc:  Test to see if a string ends with a pattern.
 *       Case is not significant.
=*/
static tSuccess
Select_Match_End(char const * sample, char const * pattern)
{
    regmatch_t m[2];
    /*
     *  On the first call for this macro, compile the expression
     */
    if (pCurMacro->funcPrivate == NULL) {
        void *    mat = (void *)pattern;
        regex_t*  pRe = AGALOC(sizeof(*pRe), "select match end re");
        compile_re(pRe, mat, (int)pCurMacro->res);
        pCurMacro->funcPrivate = (void*)pRe;
    }

    if (regexec((regex_t*)pCurMacro->funcPrivate, sample, (size_t)2, m, 0)
        != 0)
        return FAILURE;
    if (m[0].rm_eo != strlen(sample))
        return FAILURE;
    return SUCCESS;
}

SCM
ag_scm_string_end_match_p(SCM text, SCM substr)
{
    char* pzText   = ag_scm2zchars(text, "text to match");
    char* pzSubstr = ag_scm2zchars(substr, "match expr");
    SCM        res;
    regex_t    re;
    regmatch_t m[2];

    compile_re(&re, pzSubstr, REG_EXTENDED);

    if (regexec(&re, pzText, (size_t)2, m, 0) != 0)
         res = SCM_BOOL_F;
    else if (m[0].rm_eo != strlen(pzText))
         res = SCM_BOOL_F;
    else res = SCM_BOOL_T;

    regfree(&re);

    return res;
}

SCM
ag_scm_string_end_eqv_match_p(SCM text, SCM substr)
{
    char* pzText   = ag_scm2zchars(text, "text to match");
    char* pzSubstr = ag_scm2zchars(substr, "match expr");
    SCM        res;
    regex_t    re;
    regmatch_t m[2];

    compile_re(&re, pzSubstr, REG_EXTENDED | REG_ICASE);

    if (regexec(&re, pzText, (size_t)2, m, 0) != 0)
         res = SCM_BOOL_F;
    else if (m[0].rm_eo != strlen(pzText))
         res = SCM_BOOL_F;
    else res = SCM_BOOL_T;

    regfree(&re);

    return res;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*=gfunc string_start_match_p
 *
 * what:   regex match start
 * general_use:
 *
 * exparg: text, text to test for pattern
 * exparg: match, pattern/substring to search for
 *
 * string: "~~*"
 *
 * doc:  Test to see if a string starts with a pattern.
 *       Case is significant.
=*/
/*=gfunc string_start_eqv_match_p
 *
 * what:   caseless regex start
 * general_use:
 *
 * exparg: text, text to test for pattern
 * exparg: match, pattern/substring to search for
 *
 * string: "~*"
 *
 * doc:  Test to see if a string starts with a pattern.
 *       Case is not significant.
=*/
static tSuccess
Select_Match_Start(char const * sample, char const * pattern)
{
    regmatch_t m[2];
    /*
     *  On the first call for this macro, compile the expression
     */
    if (pCurMacro->funcPrivate == NULL) {
        void *    mat = (void *)pattern;
        regex_t*  pRe = AGALOC(sizeof(*pRe), "select match start re");
        compile_re(pRe, mat, (int)pCurMacro->res);
        pCurMacro->funcPrivate = (void*)pRe;
    }

    if (regexec((regex_t*)pCurMacro->funcPrivate, sample, (size_t)2, m, 0)
        != 0)
        return FAILURE;
    if (m[0].rm_so != 0)
        return FAILURE;
    return SUCCESS;
}

SCM
ag_scm_string_start_match_p(SCM text, SCM substr)
{
    char* pzText   = ag_scm2zchars(text, "text to match");
    char* pzSubstr = ag_scm2zchars(substr, "match expr");
    SCM        res;
    regex_t    re;
    regmatch_t m[2];

    compile_re(&re, pzSubstr, REG_EXTENDED);

    if (regexec(&re, pzText, (size_t)2, m, 0) != 0)
         res = SCM_BOOL_F;
    else if (m[0].rm_so != 0)
         res = SCM_BOOL_F;
    else res = SCM_BOOL_T;

    regfree(&re);

    return res;
}

SCM
ag_scm_string_start_eqv_match_p(SCM text, SCM substr)
{
    char* pzText   = ag_scm2zchars(text, "text to match");
    char* pzSubstr = ag_scm2zchars(substr, "match expr");
    SCM        res;
    regex_t    re;
    regmatch_t m[2];

    compile_re(&re, pzSubstr, REG_EXTENDED | REG_ICASE);

    if (regexec(&re, pzText, (size_t)2, m, 0) != 0)
         res = SCM_BOOL_F;
    else if (m[0].rm_so != 0)
         res = SCM_BOOL_F;
    else res = SCM_BOOL_T;

    regfree(&re);

    return res;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*=gfunc string_match_p
 *
 * what:   regex match
 * general_use:
 *
 * exparg: text, text to test for pattern
 * exparg: match, pattern/substring to search for
 *
 * string: "~~"
 *
 * doc:  Test to see if a string fully matches a pattern.
 *       Case is significant.
=*/
/*=gfunc string_eqv_match_p
 *
 * what:   caseless regex match
 * general_use:
 *
 * exparg: text, text to test for pattern
 * exparg: match, pattern/substring to search for
 *
 * string: "~"
 *
 * doc:  Test to see if a string fully matches a pattern.
 *       Case is not significant, but any character equivalences
 *       must be expressed in your regular expression.
=*/
static tSuccess
Select_Match_Full(char const * sample, char const * pattern)
{
    regmatch_t m[2];

    /*
     *  On the first call for this macro, compile the expression
     */
    if (pCurMacro->funcPrivate == NULL) {
        void *    mat = (void *)pattern;
        regex_t*  pRe = AGALOC(sizeof(*pRe), "select match full re");

        if (OPT_VALUE_TRACE > TRACE_EXPRESSIONS) {
            fprintf(pfTrace, TRACE_SEL_MATCH_FULL,
                     pattern, pCurMacro->res);
        }
        compile_re(pRe, mat, (int)pCurMacro->res);
        pCurMacro->funcPrivate = pRe;
    }

    if (regexec((regex_t*)pCurMacro->funcPrivate, sample, (size_t)2, m, 0)
        != 0)
        return FAILURE;

    if (  (m[0].rm_eo != strlen( sample ))
       || (m[0].rm_so != 0))
        return FAILURE;
    return SUCCESS;
}

SCM
ag_scm_string_match_p(SCM text, SCM substr)
{
    char* pzText   = ag_scm2zchars(text, "text to match");
    char* pzSubstr = ag_scm2zchars(substr, "match expr");
    SCM        res;
    regex_t    re;
    regmatch_t m[2];

    compile_re(&re, pzSubstr, REG_EXTENDED);

    if (regexec(&re, pzText, (size_t)2, m, 0) != 0)
         res = SCM_BOOL_F;
    else if (  (m[0].rm_eo != strlen(pzText))
            || (m[0].rm_so != 0) )
         res = SCM_BOOL_F;
    else res = SCM_BOOL_T;

    regfree(&re);

    return res;
}

SCM
ag_scm_string_eqv_match_p(SCM text, SCM substr)
{
    char* pzText   = ag_scm2zchars(text, "text to match");
    char* pzSubstr = ag_scm2zchars(substr, "match expr");
    SCM        res;
    regex_t    re;
    regmatch_t m[2];

    compile_re(&re, pzSubstr, REG_EXTENDED | REG_ICASE);

    if (regexec(&re, pzText, (size_t)2, m, 0) != 0)
         res = SCM_BOOL_F;
    else if (  (m[0].rm_eo != strlen(pzText))
            || (m[0].rm_so != 0) )
         res = SCM_BOOL_F;
    else res = SCM_BOOL_T;

    regfree(&re);

    return res;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/**
 *  We don't bother making a Guile function for any of these :)
 */
static tSuccess
Select_Match_Always(char const * sample, char const * pattern)
{
    return SUCCESS;
}

/**
 *  If the "sample" addresses "zNil", then we couldn't find a value and
 *  defaulted to an empty string.  So, the result is true if the sample
 *  address is anything except "zNil".
 */
static tSuccess
Select_Match_Existence(char const * sample, char const * pattern)
{
    return (sample != zNotDefined) ? SUCCESS : FAILURE;
}

/**
 *  If the "sample" addresses "zUndefined", then we couldn't find a value and
 *  defaulted to an empty string.  So, the result false if the sample address
 *  is anything except "zUndefined".
 */
static tSuccess
Select_Match_NonExistence(char const * sample, char const * pattern)
{
    return (sample == zNotDefined) ? SUCCESS : FAILURE;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*=macfunc CASE
 *
 *  what:   Select one of several template blocks
 *  handler_proc:
 *  load_proc:
 *
 *  desc:
 *
 *  The arguments are evaluated and converted to a string, if necessary.  A
 *  simple name will be interpreted as an AutoGen value name and its value will
 *  be used by the @code{SELECT} macros (see the example below and the
 *  expression evaluation function, @pxref{EXPR}).  The scope of the macro is
 *  up to the matching @code{ESAC} macro.  Within the scope of a @code{CASE},
 *  this string is matched against case selection macros.  There are sixteen
 *  match macros that are derived from four different ways matches may be
 *  performed, plus an "always true", "true if the AutoGen value was found",
 *  and "true if no AutoGen value was found" matches.  The codes for the
 *  nineteen match macros are formed as follows:
 *
 *  @enumerate
 *  @item
 *  Must the match start matching from the beginning of the string?
 *  If not, then the match macro code starts with an asterisk (@code{*}).
 *  @item
 *  Must the match finish matching at the end of the string?
 *  If not, then the match macro code ends with an asterisk (@code{*}).
 *  @item
 *  Is the match a pattern match or a string comparison?
 *  If a comparison, use an equal sign (@code{=}).
 *  If a pattern match, use a tilde (@code{~}).
 *  @item
 *  Is the match case sensitive?
 *  If alphabetic case is important, double the tilde or equal sign.
 *  @item
 *  Do you need a default match when none of the others match?
 *  Use a single asterisk (@code{*}).
 *  @item
 *  Do you need to distinguish between an empty string value and a value
 *  that was not found?  Use the non-existence test (@code{!E}) before
 *  testing a full match against an empty string (@code{== ''}).
 *  There is also an existence test (@code{+E}), more for symmetry than
 *  for practical use.
 *  @end enumerate
 *
 *  @noindent
 *  For example:
 *
 *  @example
 *  [+ CASE <full-expression> +]
 *  [+ ~~*  "[Tt]est" +]reg exp must match at start, not at end
 *  [+ ==   "TeSt"    +]a full-string, case sensitive compare
 *  [+ =    "TEST"    +]a full-string, case insensitive compare
 *  [+ !E             +]not exists - matches if no AutoGen value found
 *  [+ ==   ""        +]expression yielded a zero-length string
 *  [+ +E             +]exists - matches if there is any value result
 *  [+ *              +]always match - no testing
 *  [+ ESAC +]
 *  @end example
 *
 *  @code{<full-expression>} (@pxref{expression syntax}) may be any expression,
 *  including the use of apply-codes and value-names.  If the expression yields
 *  a number, it is converted to a decimal string.
 *
 *  These case selection codes have also been implemented as
 *  Scheme expression functions using the same codes.  They are documented
 *  in this texi doc as ``string-*?'' predicates (@pxref{Common Functions}).
=*/
/*=macfunc ESAC
 *
 *  what:   Terminate the @code{CASE} Template Block
 *  in-context:
 *
 *  desc:
 *    This macro ends the @code{CASE} function template block.
 *    For a complete description, @xref{CASE}.
=*/
tMacro*
mFunc_Case(tTemplate* pT, tMacro* pMac)
{
    typedef tSuccess (t_match_proc)(char const *, char const *);
    /*
     *  There are only 15 procedures because the case insenstive matching
     *  get mapped into the previous four.  The last three are "match always",
     *  "match if a value was found" "match if no value found".
     */
    static t_match_proc * const match_procs[] = {
        &Select_Compare_Full,
        &Select_Compare_End,
        &Select_Compare_Start,
        &Select_Compare,

        &Select_Equivalent_Full,
        &Select_Equivalent_End,
        &Select_Equivalent_Start,
        &Select_Equivalent,

        &Select_Match_Full,
        &Select_Match_End,
        &Select_Match_Start,
        &Select_Match,

        &Select_Match_Always,
        &Select_Match_Existence,
        &Select_Match_NonExistence
    };

    static char const * const match_names[] = {
        "COMPARE_FULL",
        "COMPARE_END",
        "COMPARE_START",
        "CONTAINS",

        "EQUIVALENT_FULL",
        "EQUIVALENT_END",
        "EQUIVALENT_START",
        "EQUIV_CONTAINS",

        "MATCH_FULL",
        "MATCH_END",
        "MATCH_START",
        "MATCH_WITHIN",

        "MATCH_ALWAYS",
        "MATCH_EXISTENCE",
        "MATCH_NONEXISTENCE"
    };

    tMacro*   pEnd = pT->aMacros + pMac->endIndex;
    ag_bool needFree;
    char const * pzSampleText = evalExpression(&needFree);

    /*
     *  Search through the selection clauses until we either
     *  reach the end of the list for this CASE macro, or we match.
     */
    for (;;) {
        tSuccess mRes;
        pMac = pT->aMacros + pMac->sibIndex;
        if (pMac >= pEnd) {
            if (OPT_VALUE_TRACE >= TRACE_BLOCK_MACROS) {
                fprintf(pfTrace, TRACE_CASE_FAIL, pzSampleText);

                if (OPT_VALUE_TRACE == TRACE_EVERYTHING)
                    fprintf(pfTrace, TAB_FILE_LINE_FMT,
                            pCurTemplate->pzTplFile, pMac->lineNo);
            }

            break;
        }

        /*
         *  The current macro becomes the selected selection macro
         */
        pCurMacro = pMac;
        mRes = (*(match_procs[pMac->funcCode & 0x0F])
               )(pzSampleText, pT->pzTemplText + pMac->ozText);

        /*
         *  IF match, THEN generate and stop looking for a match.
         */
        if (SUCCEEDED(mRes)) {
            if (OPT_VALUE_TRACE >= TRACE_BLOCK_MACROS) {
                fprintf(pfTrace, TRACE_CASE_MATCHED,
                        pzSampleText,
                        match_names[pMac->funcCode & 0x0F],
                        pT->pzTemplText + pMac->ozText);

                if (OPT_VALUE_TRACE == TRACE_EVERYTHING)
                    fprintf(pfTrace, TAB_FILE_LINE_FMT,
                            pCurTemplate->pzTplFile, pMac->lineNo);
            }

            generateBlock(pT, pMac + 1, pT->aMacros + pMac->sibIndex);
            break;
        }
        else if (OPT_VALUE_TRACE == TRACE_EVERYTHING) {
            fprintf(pfTrace, TRACE_CASE_NOMATCH,
                    pzSampleText,
                    match_names[pMac->funcCode & 0x0F],
                    pT->pzTemplText + pMac->ozText);
        }
    }

    if (needFree)
        AGFREE((void*)pzSampleText);

    return pEnd;
}

/*
 *  mLoad_CASE
 *
 *  This function is called to set up (load) the macro
 *  when the template is first read in (before processing).
 */
tMacro*
mLoad_Case(tTemplate* pT, tMacro* pMac, char const ** ppzScan)
{
    size_t         srcLen     = (size_t)pMac->res;   /* macro len  */
    tCaseStack     save_stack = current_case;
    tMacro*        pEsacMac;

    /*
     *  Save the global macro loading mode
     */
    tpLoadProc const * papLP = papLoadProc;

    /*
     *  IF there is no associated text expression
     *  THEN woops!  what are we to case on?
     */
    if (srcLen == 0)
        AG_ABEND_IN(pT, pMac, LD_CASE_NO_EXPR);

    /*
     *  Load the expression
     */
    (void)mLoad_Expr(pT, pMac, ppzScan);

    /*
     *  IF this is the first time here,
     *  THEN set up the "CASE" mode callout tables.
     *  It is the standard table, except entries are inserted
     *  for SELECT and ESAC.
     */
    if (apCaseLoad[0] == NULL) {
        int i;

        /*
         *  Until there is a selection clause, only comment and select
         *  macros are allowed.
         */
        for (i=0; i < FUNC_CT; i++)
            apSelectOnly[i] = mLoad_Bogus;

        memcpy((void*)apCaseLoad, apLoadProc, sizeof( apLoadProc ));
        apSelectOnly[ FTYP_COMMENT] = mLoad_Comment;
        apSelectOnly[ FTYP_SELECT ] = \
        apCaseLoad[   FTYP_SELECT ] = mLoad_Select;
        apCaseLoad[   FTYP_ESAC   ] = mLoad_Ending;
    }

    /*
     *  Set the "select macro only" loading mode
     */
    papLoadProc = apSelectOnly;

    /*
     *  Save global pointers to the current macro entry.
     *  We will need this to link the CASE, SELECT and ESAC
     *  functions together.
     */
    current_case.pCase = current_case.pSelect = pMac;

    /*
     *  Continue parsing the template from this nested level
     */
    pEsacMac = parseTemplate(pMac+1, ppzScan);
    if (*ppzScan == NULL)
        AG_ABEND_IN(pT, pMac, LD_CASE_NO_ESAC);

    /*
     *  Tell the last select macro where its end is.
     *  (It ends with the "next" sibling.  Since there
     *  is no "next" at the end, it is a tiny lie.)
     *
     *  Also, make sure the CASE macro knows where the end is.
     */
    pMac->endIndex = \
    current_case.pSelect->sibIndex = (pEsacMac - pT->aMacros);

    /*
     *  Restore any enclosing CASE function's context.
     */
    current_case = save_stack;

    /*
     *  Restore the global macro loading mode
     */
    papLoadProc  = papLP;

    /*
     *  Return the next available macro descriptor
     */
    return pEsacMac;
}


/*=macfunc SELECT
 *
 *  what:    Selection block for CASE function
 *  in-context:
 *  alias:   | ~ | = | * | ! | + |
 *  unload-proc:
 *
 *  desc:
 *    This macro selects a block of text by matching an expression
 *    against the sample text expression evaluated in the @code{CASE}
 *    macro.  @xref{CASE}.
 *
 *    You do not specify a @code{SELECT} macro with the word ``select''.
 *    Instead, you must use one of the 19 match operators described in
 *    the @code{CASE} macro description.
=*/
static tMacro*
mLoad_Select(tTemplate * pT, tMacro* pMac, char const ** ppzScan)
{
    char const *  pzScan = *ppzScan;  /* text after macro */
    char*         pzCopy = pT->pNext; /* next text dest   */
    char const *  pzSrc  = (char*)pMac->ozText; /* macro text */
    long          srcLen = pMac->res;           /* macro len  */
    int typ = (int)FTYP_SELECT_COMPARE_FULL;

    /*
     *  Set the global macro loading mode
     */
    papLoadProc = apCaseLoad;
    pMac->res   = 0;
    if (srcLen == 0)
        AG_ABEND_IN(pT, pMac, LD_SEL_EMPTY);

    /*
     *  IF the first character is an asterisk,
     *  THEN the match can start anywhere in the string
     */
    if (*pzSrc == '*') {
        pzSrc++;
        if (IS_WHITESPACE_CHAR(*pzSrc) || (*pzSrc == NUL)) {
            typ    = (int)FTYP_SELECT_MATCH_ANYTHING;
            srcLen = 0;
            pMac->ozText = 0;
            goto selection_done;
        }

        typ |= (int)FTYP_SELECT_COMPARE_SKP_START;
    }

    /*
     *  The next character must indicate whether we are
     *  pattern matching ('~') or doing string compares ('=')
     */
    switch (*pzSrc++) {
    case '~':
        /*
         *  Or in the pattern matching bit
         */
        typ |= (int)FTYP_SELECT_MATCH_FULL;
        pMac->res = REG_EXTENDED;
        /* FALLTHROUGH */

    case '=':
        /*
         *  IF the '~' or '=' is doubled,
         *  THEN it is a case sensitive match.  Skip over the char.
         *  ELSE or in the case insensitive bit
         */
        if (pzSrc[0] == pzSrc[-1]) {
            pzSrc++;
        } else {
            typ |= (int)FTYP_SELECT_EQUIVALENT_FULL;
        }
        break;

    case '!':
    case '+':
        switch (*pzSrc) {
        case 'e':
        case 'E':
            break;
        default:
            goto bad_sel;
        }
        if ((pzSrc[1] != NUL) && (! IS_WHITESPACE_CHAR(pzSrc[1])))
            goto bad_sel;

        typ = (int)((pzSrc[-1] == '!')
            ? FTYP_SELECT_MATCH_NONEXISTENCE : FTYP_SELECT_MATCH_EXISTENCE);
        srcLen = 0;
        pMac->ozText = 0;
        goto selection_done;

    default:
    bad_sel:
        AG_ABEND_IN(pT, pMac, LD_SEL_INVAL);
    }

    /*
     *  IF the last character is an asterisk,
     *  THEN the match may end before the test string ends.
     *       OR in the "may end early" bit.
     */
    if (*pzSrc == '*') {
        pzSrc++;
        typ |= (int)FTYP_SELECT_COMPARE_SKP_END;
    }

    if (! IS_WHITESPACE_CHAR(*pzSrc))
        AG_ABEND_IN(pT, pMac, LD_SEL_INVAL);

    pzSrc = SPN_WHITESPACE_CHARS(pzSrc);
    srcLen -= pzSrc - (char const *)pMac->ozText;
    if (srcLen <= 0)
        AG_ABEND_IN(pT, pMac, LD_SEL_INVAL);

    /*
     *  See if we are doing case insensitive regular expressions
     */
    if (  (typ & (int)FTYP_SELECT_EQV_MATCH_FULL)
       == (int)FTYP_SELECT_EQV_MATCH_FULL) {
        int  bitSet;
        pMac->res = REG_EXTENDED | REG_ICASE;

        /*
         *  Turn off the case comparison mode for regular expressions.
         *  We don't have to worry about it.  It is done for us.
         */
        bitSet  = ~(int)FTYP_SELECT_EQUIVALENT_FULL;
        bitSet |= (int)FTYP_SELECT_COMPARE_FULL; /* dont turn this bit off! */
        typ    &= bitSet;
    }

    /*
     *  Copy the expression
     */
    pzScan = pzCopy;
    pMac->ozText = (pzCopy - pT->pzTemplText);
    if (typ == (int)FTYP_SELECT_EQUIVALENT) {
        do  {
            *(pzCopy++) = toupper(*(pzSrc++));
        } while (--srcLen > 0);
    } else {
        do  {
            *(pzCopy++) = *(pzSrc++);
        } while (--srcLen > 0);
    }
    *(pzCopy++) = NUL;
    *(pzCopy++) = NUL;
    pT->pNext = pzCopy;

    /*
     * If the value is a quoted string, strip the quotes and
     * process the string (backslash fixup).
     */
    if ((*pzScan == '"') || (*pzScan == '\''))
        spanQuote((void *)pzScan);

 selection_done:
    pMac->funcCode = (teFuncType)typ;

    current_case.pSelect->sibIndex = (pMac - pT->aMacros);
    current_case.pSelect = (tMacro*)pMac;

    return pMac + 1;
}

void
mUnload_Select(tMacro* pMac)
{
    if (pMac->funcPrivate != NULL) {
        regex_t* pRe = (regex_t*)pMac->funcPrivate;
        regfree(pRe);
        AGFREE(pRe);
    }
}

/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * indent-tabs-mode: nil
 * End:
 * end of agen5/funcCase.c */
