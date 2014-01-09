
/**
 * @file expString.c
 *
 *  Time-stamp:        "2012-02-12 09:01:34 bkorb"
 *
 *  This module implements expression functions that
 *  manipulate string values.
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

/* = = = START-STATIC-FORWARD = = = */
static size_t
string_size(char const * pzScn, size_t newLineSize);

static SCM
makeString(char const * pzText, char const * pzNewLine, size_t newLineSize);

static size_t
stringify_for_sh(char * pzNew, uint_t qt, char const * pzDta, size_t dtaSize);

static SCM
shell_stringify(SCM obj, uint_t qt);

static int
sub_count(char const * haystack, char const * needle);

static void
do_substitution(
    char const * src_str,
    ssize_t      str_len,
    SCM          match,
    SCM          repl,
    char **      ppz_res,
    ssize_t *    res_len);
/* = = = END-STATIC-FORWARD = = = */

static size_t
string_size(char const * pzScn, size_t newLineSize)
{
    /*
     *  Start by counting the start and end quotes, plus the NUL.
     */
    size_t dtaSize = 3;

    for (;;) {
        char ch = *(pzScn++);
        if ((ch >= ' ') && (ch <= '~')) {

            /*
             *  One for each character, plus a backquote when needed
             */
            dtaSize++;
            if ((ch == '"') || (ch == '\\'))
                dtaSize++;
        }

        /*
         *  When not a normal character, then count the characters
         *  required to represent whatever it is.
         */
        else switch (ch) {
        case NUL:
            return dtaSize;

        case NL:
            dtaSize += newLineSize;
            break;

        case TAB:
        case '\a':
        case '\b':
        case '\f':
        case '\r':
        case '\v':
            dtaSize += 2;
            break;

        default:
            dtaSize += 4;
        }
    }
}

static SCM
makeString(char const * pzText, char const * pzNewLine, size_t newLineSize)
{
    char     z[SCRIBBLE_SIZE];
    char*    pzDta;
    char*    pzFre;
    char const * pzScn   = pzText;
    size_t   dtaSize = string_size(pzText, newLineSize);

    /*
     *  We now know how big the string is going to be.
     *  Allocate what we need.
     */
    if (dtaSize >= sizeof(z))
         pzFre = pzDta = AGALOC(dtaSize, "quoting string");
    else pzFre = pzDta = z;

    *(pzDta++) = '"';

    for (;;) {
        unsigned char ch = (unsigned char)*pzScn;
        if ((ch >= ' ') && (ch <= '~')) {
            if ((ch == '"') || (ch == '\\'))
                /*
                 *  We must escape these characters in the output string
                 */
                *(pzDta++) = '\\';
            *(pzDta++) = ch;

        } else switch (ch) {
        case NUL:
            goto copyDone;

        case NL:
            /*
             *  place contiguous new-lines on a single line
             */
            while (pzScn[1] == NL) {
                *(pzDta++) = '\\';
                *(pzDta++) = 'n';
                pzScn++;
            }

            /*
             *  Replace the new-line with its escaped representation.
             *  Also, break and restart the output string, indented
             *  7 spaces (so that after the '"' char is printed,
             *  any contained tabbing will look correct).
             *  Do *not* start a new line if there are no more data.
             */
            if (pzScn[1] == NUL) {
                *(pzDta++) = '\\';
                *(pzDta++) = 'n';
                goto copyDone;
            }

            strcpy(pzDta, pzNewLine);
            pzDta += newLineSize;
            break;

        case '\a':
            *(pzDta++) = '\\';
            *(pzDta++) = 'a';
            break;

        case '\b':
            *(pzDta++) = '\\';
            *(pzDta++) = 'b';
            break;

        case '\f':
            *(pzDta++) = '\\';
            *(pzDta++) = 'f';
            break;

        case '\r':
            *(pzDta++) = '\\';
            *(pzDta++) = 'r';
            break;

        case TAB:
            *(pzDta++) = '\\';
            *(pzDta++) = 't';
            break;

        case '\v':
            *(pzDta++) = '\\';
            *(pzDta++) = 'v';
            break;

        default:
            /*
             *  sprintf is safe here, because we already computed
             *  the amount of space we will be using.
             */
            sprintf(pzDta, MK_STR_OCT_FMT, ch);
            pzDta += 4;
        }

        pzScn++;
    } copyDone:

    /*
     *  End of string.  Terminate the quoted output.
     *  If necessary, deallocate the text string.
     *  Return the scan resumption point.
     */
    *(pzDta++) = '"';
    *pzDta = NUL;

    {
        SCM res = AG_SCM_STR02SCM(pzFre);
        if (pzFre != z)
            AGFREE(pzFre);
        return res;
    }
}

static size_t
stringify_for_sh(char * pzNew, uint_t qt, char const * pzDta, size_t dtaSize)
{
    char * pz = pzNew;
    *(pz++) = qt;

    for (;;) {
        char c = (*(pz++) = *(pzDta++));
        switch (c) {
        case NUL:
            pz[-1]  = qt;
            *pz     = NUL;
            dtaSize = (pz - pzNew);

            return (pz - pzNew);

        case '\\':
            /*
             *  If someone went to the trouble to escape a backquote or a
             *  dollar sign, then we should not neutralize it.  Note that
             *  we handle a following backslash as a normal character.
             *
             *  i.e.  \\ --> \\\\ *BUT* \\$ --> \\\$
             */
            c = *pzDta;
            switch (*pzDta) {
            case '$':
                break;

            case '"':
            case '`':
                /*
                 *  IF the ensuing quote character does *NOT* match the
                 *  quote character for the string, then we will preserve
                 *  the single copy of the backslash.  If it does match,
                 *  then we will double the backslash and a third backslash
                 *  will be inserted when we emit the quote character.
                 */
                if (c != qt)
                    break;
                /* FALLTHROUGH */

            default:
                *(pz++) = '\\';   /* \   -->  \\    */
            }
            break;

        case '"': case '`':
            if (c == qt) {
                /*
                 *  This routine does both `xx` and "xx" strings, we have
                 *  to worry about this stuff differently.  I.e., in ""
                 *  strings, add a single \ in front of ", and in ``
                 *  preserve a add \ in front of `.
                 */
                pz[-1]  = '\\';       /* "   -->   \"   */
                *(pz++) = c;
            }
        }
    }
}

static SCM
shell_stringify(SCM obj, uint_t qt)
{
    char*  pzNew;
    size_t dtaSize = 3;
    char*  pzDta   = ag_scm2zchars(obj, "AG Object");
    char*  pz      = pzDta;

    for (;;) {
        char c = *(pz++);

        switch (c) {
        case NUL:
            goto loopDone1;

        case '"': case '`': case '\\':
            dtaSize += 2;
            break;

        default:
            dtaSize++;
        }
    } loopDone1:;

    pzNew = AGALOC(dtaSize, "shell string");
    dtaSize = stringify_for_sh(pzNew, qt, pzDta, dtaSize);

    {
        SCM res = AG_SCM_STR2SCM(pzNew, dtaSize);
        AGFREE(pzNew);
        return res;
    }
}

static int
sub_count(char const * haystack, char const * needle)
{
    int repCt = 0;
    size_t needle_len = strlen(needle);

    for (;;) {
        haystack = strstr(haystack, needle);
        if (haystack == NULL) break;
        repCt++;
        haystack += needle_len;
    }
    return repCt;
}

/**
 *  Replace marker text.
 *
 *  Replace all occurrances of the marker text with the substitution text.
 *  The result is stored in an automatically freed temporary buffer.
 *
 *  @param src_str  The source string
 *  @param str_len  The length of the string
 *  @param match    the SCM-ized marker string
 *  @param repl     the SCM-ized replacement string
 *  @param ppz_res  pointer to the result pointer
 *  @param res_len  pointer to result length
 */
static void
do_substitution(
    char const * src_str,
    ssize_t      str_len,
    SCM          match,
    SCM          repl,
    char **      ppz_res,
    ssize_t *    res_len)
{
    char* pzMatch  = ag_scm2zchars(match, "match text");
    char* rep_str  = ag_scm2zchars(repl,  "repl text");
    int   mark_len = AG_SCM_STRLEN(match);
    int   repl_len = AG_SCM_STRLEN(repl);

    {
        int ct = sub_count(src_str, pzMatch);
        if (ct == 0)
            return; /* No substitutions -- no work. */

        str_len += (repl_len - mark_len) * ct;
    }

    {
        char * dest = ag_scribble(str_len + 1);
        *ppz_res = dest;
        *res_len = str_len;

        for (;;) {
            char const * next = strstr(src_str, pzMatch);
            size_t len;

            if (next == NULL)
                break;
            len = next - src_str;
            if (len != 0) {
                memcpy(dest, src_str, len);
                dest += len;
            }
            memcpy(dest, rep_str, (size_t)repl_len);
            dest   += repl_len;
            src_str = next + mark_len;
        }

        strcpy(dest, src_str);
    }
}


/*
 *  Recursive routine.  It calls itself for list values and calls
 *  "do_substitution" for string values.  Each substitution will
 *  be done in the order found in the tree walk of list values.
 *  The "match" and "repl" trees *must* be identical in structure.
 */
LOCAL void
do_multi_subs(char ** ppzStr, ssize_t * pStrLen, SCM match, SCM repl)
{
    char* pzStr = *ppzStr;
    char* pzNxt = pzStr;

    /*
     *  Loop for as long as our list has more entries
     */
    while (! AG_SCM_NULLP(match)) {
        /*
         *  "CAR" is the current value, "CDR" is rest of list
         */
        SCM  matchCar  = SCM_CAR(match);
        SCM  replCar   = SCM_CAR(repl);

        match = SCM_CDR(match);
        repl  = SCM_CDR(repl);

        if (AG_SCM_STRING_P(matchCar)) {
            do_substitution(pzStr, *pStrLen, matchCar, replCar,
                            &pzNxt, pStrLen);

            // coverity[use_after_free] -- invalid alias analysis
            pzStr = pzNxt;
        }

        else if (AG_SCM_LIST_P(matchCar))
            do_multi_subs(&pzStr, pStrLen, matchCar, replCar);

        else
            /*
             *  Whatever it is it is not part of what we would expect.  Bail.
             */
            break;
    }

    *ppzStr = pzStr;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  EXPRESSION EVALUATION ROUTINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*=gfunc in_p
 *
 * what:   test for string in list
 * general_use:
 * exparg: test-string, string to look for
 * exparg: string-list, list of strings to check,, list
 *
 * doc:  Return SCM_BOOL_T if the first argument string is found
 *      in one of the entries in the second (list-of-strings) argument.
=*/
SCM
ag_scm_in_p(SCM obj, SCM list)
{
    int     len;
    size_t  lenz;
    SCM     car;
    char const * pz1;

    if (! AG_SCM_STRING_P(obj))
        return SCM_UNDEFINED;

    pz1  = AG_SCM_CHARS(obj);
    lenz = AG_SCM_STRLEN(obj);

    /*
     *  If the second argument is a string somehow, then treat
     *  this as a straight out string comparison
     */
    if (AG_SCM_STRING_P(list)) {
        if (  (AG_SCM_STRLEN(list) == lenz)
           && (strncmp(pz1, AG_SCM_CHARS(list), lenz) == 0))
            return SCM_BOOL_T;
        return SCM_BOOL_F;
    }

    len = scm_ilength(list);
    if (len == 0)
        return SCM_BOOL_F;

    /*
     *  Search all the lists and sub-lists passed in
     */
    while (len-- > 0) {
        car  = SCM_CAR(list);
        list = SCM_CDR(list);

        /*
         *  This routine is listed as getting a list as the second
         *  argument.  That means that if someone builds a list and
         *  hands it to us, it magically becomes a nested list.
         *  This unravels that.
         */
        if (! AG_SCM_STRING_P(car)) {
            if (ag_scm_in_p(obj, car) == SCM_BOOL_T)
                return SCM_BOOL_T;
            continue;
        }

        if (  (AG_SCM_STRLEN(car) == lenz)
           && (strncmp(pz1, AG_SCM_CHARS(car), lenz) == 0) )
            return SCM_BOOL_T;
    }

    return SCM_BOOL_F;
}


/*=gfunc join
 *
 * what:   join string list with separator
 * general_use:
 * exparg: separator, string to insert between entries
 * exparg: list, list of strings to join,, list
 *
 * doc:  With the first argument as the separator string,
 *       joins together an a-list of strings into one long string.
 *       The list may contain nested lists, partly because you
 *       cannot always control that.
=*/
SCM
ag_scm_join(SCM sep, SCM list)
{
    int      l_len, sv_l_len;
    SCM      car;
    SCM      alist = list;
    size_t   sep_len;
    size_t   str_len;
    char *   pzRes;
    char const * pzSep;
    char *   pzScan;

    if (! AG_SCM_STRING_P(sep))
        return SCM_UNDEFINED;

    sv_l_len = l_len = scm_ilength(list);
    if (l_len == 0)
        return AG_SCM_STR02SCM(zNil);

    pzSep   = AG_SCM_CHARS(sep);
    sep_len = AG_SCM_STRLEN(sep);
    str_len = 0;

    /*
     *  Count up the lengths of all the strings to be joined.
     */
    for (;;) {
        car  = SCM_CAR(list);
        list = SCM_CDR(list);

        /*
         *  This routine is listed as getting a list as the second
         *  argument.  That means that if someone builds a list and
         *  hands it to us, it magically becomes a nested list.
         *  This unravels that.
         */
        if (! AG_SCM_STRING_P(car)) {
            if (car != SCM_UNDEFINED)
                car = ag_scm_join(sep, car);
            if (! AG_SCM_STRING_P(car))
                return SCM_UNDEFINED;
        }

        str_len += AG_SCM_STRLEN(car);

        if (--l_len <= 0)
            break;

        str_len += sep_len;
    }

    l_len = sv_l_len;
    pzRes = pzScan = ag_scribble(str_len);

    /*
     *  Now, copy each one into the output
     */
    for (;;) {
        size_t cpy_len;

        car   = SCM_CAR(alist);
        alist = SCM_CDR(alist);

        /*
         *  This unravels nested lists.
         */
        if (! AG_SCM_STRING_P(car))
            car = ag_scm_join(sep, car);

        cpy_len = AG_SCM_STRLEN(car);
        memcpy((void*)pzScan, AG_SCM_CHARS(car), cpy_len);
        pzScan += cpy_len;

        /*
         *  IF we reach zero, then do not insert a separation and bail out
         */
        if (--l_len <= 0)
            break;
        memcpy((void*)pzScan, (void*)pzSep, sep_len);
        pzScan += sep_len;
    }

    return AG_SCM_STR2SCM(pzRes, str_len);
}


/*=gfunc prefix
 *
 * what:  prefix lines with a string
 * general_use:
 *
 * exparg: prefix, string to insert at start of each line
 * exparg: text, multi-line block of text
 *
 * doc:
 *  Prefix every line in the second string with the first string.
 *
 *  For example, if the first string is "# " and the second contains:
 *  @example
 *  two
 *  lines
 *  @end example
 *  @noindent
 *  The result string will contain:
 *  @example
 *  # two
 *  # lines
 *  @end example
=*/
SCM
ag_scm_prefix(SCM prefix, SCM text)
{
    char*    pzPfx;
    char*    pzText;
    char*    pzDta;
    size_t   out_size, rem_size;
    size_t   pfx_size;
    char*    pzRes;

    pzPfx    = ag_scm2zchars(prefix, "prefix");
    pzDta    = \
    pzText   = ag_scm2zchars(text, "text");
    pfx_size = strlen(pzPfx);
    out_size = pfx_size;

    for (;;) {
        switch (*(pzText++)) {
        case NUL:
            goto exit_count;
        case NL:
            out_size += pfx_size;
            /* FALLTHROUGH */
        default:
            out_size++;
        }
    } exit_count:;

    pzText = pzDta;
    pzRes  = pzDta = ag_scribble(rem_size = out_size);
    strcpy(pzDta, pzPfx);
    pzDta    += pfx_size;
    rem_size -= pfx_size;
    pfx_size++;

    for (;;) {
        char ch = *(pzText++);
        switch (ch) {
        case NUL:
            if (rem_size != 0)
                AG_ABEND(PREFIX_FAIL);

            return AG_SCM_STR2SCM(pzRes, out_size);

        case NL:
            *pzDta    = ch;
            strcpy(pzDta+1, pzPfx);
            pzDta    += pfx_size;
            rem_size -= pfx_size;
            break;

        default:
            rem_size--;
            *(pzDta++) = ch;
            break;
        }
    }
}

/*=gfunc raw_shell_str
 *
 * what:  single quote shell string
 * general_use:
 *
 * exparg: string, string to transform
 *
 * doc:
 *  Convert the text of the string into a singly quoted string
 *  that a normal shell will process into the original string.
 *  (It will not do macro expansion later, either.)
 *  Contained single quotes become tripled, with the middle quote
 *  escaped with a backslash.  Normal shells will reconstitute the
 *  original string.
 *
 *  @strong{Notice}:  some shells will not correctly handle unusual
 *  non-printing characters.  This routine works for most reasonably
 *  conventional ASCII strings.
=*/
SCM
ag_scm_raw_shell_str(SCM obj)
{
    char*      pzDta;
    char*      pz;
    char*      pzFree;

    pzDta = ag_scm2zchars(obj, "AG Object");

    {
        size_t dtaSize = AG_SCM_STRLEN(obj) + 3; /* NUL + 2 quotes */
        pz = pzDta-1;
        for (;;) {
            pz = strchr(pz+1, '\'');
            if (pz == NULL)
                break;
            dtaSize += 3; /* '\'' -> 3 additional chars */
        }

        pzFree = pz = AGALOC(dtaSize + 2, "raw string");
    }

    /*
     *  Handle leading single quotes before starting the first quote.
     */
    while (*pzDta == '\'') {
        *(pz++) = '\\';
        *(pz++) = '\'';

        /*
         *  IF pure single quotes, then we're done.
         */
        if (*++pzDta == NUL) {
            *pz = NUL;
            goto returnString;
        }
    }

    /*
     *  Start quoting.  If the string is empty, we wind up with two quotes.
     */
    *(pz++) = '\'';

    for (;;) {
        switch (*(pz++) = *(pzDta++)) {
        case NUL:
            goto loopDone;

        case '\'':
            /*
             *  We've inserted a single quote, which ends the quoting session.
             *  Now, insert escaped quotes for every quote char we find, then
             *  restart the quoting.
             */
            pzDta--;
            do {
                *(pz++) = '\\';
                *(pz++) = '\'';
            } while (*++pzDta == '\'');
            if (*pzDta == NUL) {
                *pz = NUL;
                goto returnString;
            }
            *(pz++) = '\'';
        }
    } loopDone:;
    pz[-1] = '\'';
    *pz    = NUL;

 returnString:
    {
        SCM res = AG_SCM_STR02SCM(pzFree);
        AGFREE(pzFree);
        return res;
    }
}


/*=gfunc shell_str
 *
 * what:  double quote shell string
 * general_use:
 *
 * exparg: string, string to transform
 *
 * doc:
 *
 *  Convert the text of the string into a double quoted string that a normal
 *  shell will process into the original string, almost.  It will add the
 *  escape character @code{\\} before two special characters to
 *  accomplish this: the backslash @code{\\} and double quote @code{"}.
 *
 *  @strong{Notice}: some shells will not correctly handle unusual
 *  non-printing characters.  This routine works for most reasonably
 *  conventional ASCII strings.
 *
 *  @strong{WARNING}:
 *@*
 *  This function omits the extra backslash in front of a backslash, however,
 *  if it is followed by either a backquote or a dollar sign.  It must do this
 *  because otherwise it would be impossible to protect the dollar sign or
 *  backquote from shell evaluation.  Consequently, it is not possible to
 *  render the strings "\\$" or "\\`".  The lesser of two evils.
 *
 *  All others characters are copied directly into the output.
 *
 *  The @code{sub-shell-str} variation of this routine behaves identically,
 *  except that the extra backslash is omitted in front of @code{"} instead
 *  of @code{`}.  You have to think about it.  I'm open to suggestions.
 *
 *  Meanwhile, the best way to document is with a detailed output example.
 *  If the backslashes make it through the text processing correctly,
 *  below you will see what happens with three example strings.  The first
 *  example string contains a list of quoted @code{foo}s, the second is
 *  the same with a single backslash before the quote characters and the
 *  last is with two backslash escapes.  Below each is the result of the
 *  @code{raw-shell-str}, @code{shell-str} and @code{sub-shell-str} functions.
 *
 *  @example
 *  foo[0]           ''foo'' 'foo' "foo" `foo` $foo
 *  raw-shell-str -> \'\''foo'\'\'' '\''foo'\'' "foo" `foo` $foo'
 *  shell-str     -> "''foo'' 'foo' \"foo\" `foo` $foo"
 *  sub-shell-str -> `''foo'' 'foo' "foo" \`foo\` $foo`
 *
 *  foo[1]           \'bar\' \"bar\" \`bar\` \$bar
 *  raw-shell-str -> '\'\''bar\'\'' \"bar\" \`bar\` \$bar'
 *  shell-str     -> "\\'bar\\' \\\"bar\\\" \`bar\` \$bar"
 *  sub-shell-str -> `\\'bar\\' \"bar\" \\\`bar\\\` \$bar`
 *
 *  foo[2]           \\'BAZ\\' \\"BAZ\\" \\`BAZ\\` \\$BAZ
 *  raw-shell-str -> '\\'\''BAZ\\'\'' \\"BAZ\\" \\`BAZ\\` \\$BAZ'
 *  shell-str     -> "\\\\'BAZ\\\\' \\\\\"BAZ\\\\\" \\\`BAZ\\\` \\\$BAZ"
 *  sub-shell-str -> `\\\\'BAZ\\\\' \\\"BAZ\\\" \\\\\`BAZ\\\\\` \\\$BAZ`
 *  @end example
 *
 *  There should be four, three, five and three backslashes for the four
 *  examples on the last line, respectively.  The next to last line should
 *  have four, five, three and three backslashes.  If this was not accurately
 *  reproduced, take a look at the agen5/test/shell.test test.  Notice the
 *  backslashes in front of the dollar signs.  It goes from zero to one to
 *  three for the "cooked" string examples.
=*/
SCM
ag_scm_shell_str(SCM obj)
{
    return shell_stringify(obj, (unsigned)'"');
}

/*=gfunc sub_shell_str
 *
 * what:  back quoted (sub-)shell string
 * general_use:
 *
 * exparg: string, string to transform
 *
 * doc:
 *   This function is substantially identical to @code{shell-str}, except
 *   that the quoting character is @code{`} and the "leave the escape alone"
 *   character is @code{"}.
=*/
SCM
ag_scm_sub_shell_str(SCM obj)
{
    return shell_stringify(obj, (unsigned)'`');
}


/*=gfunc stack
 *
 * what:  make list of AutoGen values
 *
 * exparg: ag-name, AutoGen value name
 *
 * doc:  Create a scheme list of all the strings that are associated
 *       with a name.  They must all be text values or we choke.
=*/
SCM
ag_scm_stack(SCM obj)
{
    SCM         res;
    SCM *       pos = &res;
    tDefEntry** ppDE;
    tDefEntry*  pDE;
    SCM         str;

    res = SCM_EOL;

    ppDE = findEntryList(ag_scm2zchars(obj, "AG Object"));
    if (ppDE == NULL)
        return SCM_EOL;

    for (;;) {
        pDE = *(ppDE++);

        if (pDE == NULL)
            break;

        if (pDE->valType != VALTYP_TEXT)
            return SCM_UNDEFINED;

        str  = AG_SCM_STR02SCM(pDE->val.pzText);
        *pos = scm_cons(str, SCM_EOL);
        pos  = SCM_CDRLOC(*pos);
    }

    return res;
}


/*=gfunc kr_string
 *
 * what:  emit string for K&R C
 * general_use:
 *
 * exparg: string, string to reformat
 *
 *  doc:
 *  Reform a string so that, when printed, a K&R C compiler will be able
 *  to compile the data and construct a string that contains exactly
 *  what the current string contains.  Many non-printing characters are
 *  replaced with escape sequences.  New-lines are replaced with a
 *  backslash-n-backslash and newline sequence,
=*/
SCM
ag_scm_kr_string(SCM str)
{
    return makeString(ag_scm2zchars(str, "str"),
                      KR_STRING_NEWLINE, KR_STRING_NEWLINE_LEN);
}


/*=gfunc c_string
 *
 * what:  emit string for ANSI C
 * general_use:
 *
 * exparg: string, string to reformat
 *
 * doc:
 *  Reform a string so that, when printed, the C compiler will be able to
 *  compile the data and construct a string that contains exactly what the
 *  current string contains.  Many non-printing characters are replaced with
 *  escape sequences.  Newlines are replaced with a backslash, an @code{n}, a
 *  closing quote, a newline, seven spaces and another re-opening quote.  The
 *  compiler will implicitly concatenate them.  The reader will see line
 *  breaks.
 *
 *  A K&R compiler will choke.  Use @code{kr-string} for that compiler.
 *
=*/
SCM
ag_scm_c_string(SCM str)
{
    return makeString(ag_scm2zchars(str, "str"),
                      C_STRING_NEWLINE, C_STRING_NEWLINE_LEN);
}


/*=gfunc string_tr_x
 *
 * what:  convert characters
 * general_use:
 *
 *  exparg:  source, string to transform
 *  exparg:  match,  characters to be converted
 *  exparg:  translation, conversion list
 *
 * doc: This is the same as the @code{tr(1)} program, except the
 *      string to transform is the first argument.  The second and
 *      third arguments are used to construct mapping arrays for the
 *      transformation of the first argument.
 *
 *      It is too bad this little program has so many different
 *      and incompatible implementations!
=*/
SCM
ag_scm_string_tr_x(SCM str, SCM from_xform, SCM to_xform)
{
    unsigned char ch_map[ 1 << 8 /* bits-per-byte */ ];
    int   i      = sizeof(ch_map) - 1;
    char* pzFrom = ag_scm2zchars(from_xform, "str");
    char* pzTo   = ag_scm2zchars(to_xform, "str");

    do  {
        ch_map[i] = i;
    } while (--i > 0);

    for (;i <= sizeof(ch_map) - 1;i++) {
        unsigned char fch = (unsigned char)*(pzFrom++);
        unsigned char tch = (unsigned char)*(pzTo++);

        if (tch == NUL) {
            pzTo--;
            tch = pzTo[-1];
        }

        switch (fch) {
        case NUL:
            goto mapDone;

        case '-':
            if ((i > 0) && (tch == '-')) {
                unsigned char fs = (unsigned char)pzFrom[-2];
                unsigned char fe = (unsigned char)pzFrom[0];
                unsigned char ts = (unsigned char)pzTo[-2];
                unsigned char te = (unsigned char)pzTo[0];
                if (te != NUL) {
                    while (fs < fe) {
                        ch_map[ fs++ ] = ts;
                        if (ts < te) ts++;
                    }
                    break;
                }
            }

        default:
            ch_map[ fch ] = tch;
        }
    } mapDone:;

    pzTo = (char*)(void*)AG_SCM_CHARS(str);
    i    = AG_SCM_STRLEN(str);
    while (i-- > 0) {
        *pzTo = ch_map[ (int)*pzTo ];
        pzTo++;
    }
    return str;
}

/*=gfunc string_tr
 *
 * what:  convert characters with new result
 * general_use:
 *
 *  exparg:  source, string to transform
 *  exparg:  match,  characters to be converted
 *  exparg:  translation, conversion list
 *
 * doc: This is identical to @code{string-tr!}, except that it does not
 *      over-write the previous value.
=*/
SCM
ag_scm_string_tr(SCM Str, SCM From, SCM To)
{
    size_t lenz  = AG_SCM_STRLEN(Str);
    SCM    res   = AG_SCM_STR2SCM(AG_SCM_CHARS(Str), lenz);
    return ag_scm_string_tr_x(res, From, To);
}

/*=gfunc string_substitute
 *
 * what:  multiple global replacements
 * general_use:
 *
 *  exparg:  source, string to transform
 *  exparg:  match,  substring or substring list to be replaced
 *  exparg:  repl,   replacement strings or substrings
 *
 * doc: @code{match} and  @code{repl} may be either a single string or
 *      a list of strings.  Either way, they must have the same structure
 *      and number of elements.  For example, to replace all amphersands,
 *      less than and greater than characters, do something like this:
 *
 * @example
 *      (string-substitute source
 *          (list "&"     "<"    ">")
 *          (list "&amp;" "&lt;" "&gt;"))
 * @end example
=*/
SCM
ag_scm_string_substitute(SCM Str, SCM Match, SCM Repl)
{
    char const *  pzStr;
    ssize_t len;
    SCM     res;

    if (! AG_SCM_STRING_P(Str))
        return SCM_UNDEFINED;

    pzStr = AG_SCM_CHARS(Str);
    len   = AG_SCM_STRLEN(Str);

    if (AG_SCM_STRING_P(Match))
        do_substitution(pzStr, len, Match, Repl, (char**)&pzStr, &len);
    else
        do_multi_subs((char**)&pzStr, &len, Match, Repl);

    res = AG_SCM_STR2SCM(pzStr, len);
    return res;
}

/*=gfunc time_string_to_number
 *
 * what:   duration string to seconds
 * general_use:
 * exparg: time_spec, string to parse
 *
 * doc:    Convert the argument string to a time period in seconds.
 *         The string may use multiple parts consisting of days, hours
 *         minutes and seconds.  These are indicated with a suffix of
 *         @code{d}, @code{h}, @code{m} and @code{s} respectively.
 *         Hours, minutes and seconds may also be represented with
 *         @code{HH:MM:SS} or, without hours, as @code{MM:SS}.
=*/
SCM
ag_scm_time_string_to_number(SCM time_spec)
{
    extern time_t parse_duration(char const * in_pz);

    char const * pz;
    time_t  time_period;

    if (! AG_SCM_STRING_P(time_spec))
        return SCM_UNDEFINED;

    pz = AG_SCM_CHARS(time_spec);
    time_period = parse_duration(pz);
    
    return AG_SCM_INT2SCM((int)time_period);
}

/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * indent-tabs-mode: nil
 * End:
 * end of agen5/expString.c */
