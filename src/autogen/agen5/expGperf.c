/**
 * @file expGperf.c
 *
 *  Time-stamp:        "2012-01-15 08:38:23 bkorb"
 *
 *  Create a perfect hash function program and use it to compute
 *  index values for a list of provided names.  It also documents how
 *  to incorporate that hashing function into a generated C program.
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

#ifndef SHELL_ENABLED
HIDE_FN(SCM ag_scm_make_gperf(SCM name, SCM hlist))
{
    return SCM_UNDEFINED;
}

HIDE_FN(SCM ag_scm_gperf(SCM name, SCM str))
{
    return SCM_UNDEFINED;
}
#else

/*=gfunc make_gperf
 *
 * what:   build a perfect hash function program
 * general_use:
 *
 * exparg: name , name of hash list
 * exparg: strings , list of strings to hash ,, list
 *
 * doc:  Build a program to perform perfect hashes of a known list of input
 *       strings.  This function produces no output, but prepares a program
 *       named, @file{gperf_<name>} for use by the gperf function
 *       @xref{SCM gperf}.
 *
 *       This program will be obliterated as AutoGen exits.
 *       However, you may incorporate the generated hashing function
 *       into your C program with commands something like the following:
 *
 *       @example
 *       [+ (shellf "sed '/^int main(/,$d;/^#line/d' $@{gpdir@}/%s.c"
 *                  name ) +]
 *       @end example
 *
 *       where @code{name} matches the name provided to this @code{make-perf}
 *       function.  @code{gpdir} is the variable used to store the name of the
 *       temporary directory used to stash all the files.
=*/
SCM
ag_scm_make_gperf(SCM name, SCM hlist)
{
    static ag_bool do_cleanup = AG_TRUE;

    char const * pzName  = ag_scm2zchars(name, "gperf name");
    char const * pzList;
    SCM          newline = AG_SCM_STR2SCM(NEWLINE, (size_t)1);

    if (! AG_SCM_STRING_P(name))
        return SCM_UNDEFINED;

    /*
     *  Construct the newline separated list of values
     */
    hlist  = ag_scm_join(newline, hlist);
    pzList = ag_scm2zchars(hlist, "hash list");

    /*
     *  Stash the concatenated list somewhere, hopefully without an alloc.
     */
    {
        char * cmd = aprf(MK_GPERF_SCRIPT, zMakeProg, pzName, pzList);

        /*
         *  Run the command and ignore the results.
         *  In theory, the program should be ready.
         */
        pzList = shell_cmd(cmd);
        AGFREE(cmd);

        if (pzList != NULL)
            free((void *)pzList);
    }

    if (do_cleanup) {
        SCM_EVAL_CONST(MAKE_GPERF_CLEANUP);
        do_cleanup = AG_FALSE;
    }

    return SCM_BOOL_T;
}


/*=gfunc gperf
 *
 * what:   perform a perfect hash function
 * general_use:
 *
 * exparg: name , name of hash list
 * exparg: str  , string to hash
 *
 * doc:  Perform the perfect hash on the input string.  This is only useful if
 *       you have previously created a gperf program with the @code{make-gperf}
 *       function @xref{SCM make-gperf}.  The @code{name} you supply here must
 *       match the name used to create the program and the string to hash must
 *       be one of the strings supplied in the @code{make-gperf} string list.
 *       The result will be a perfect hash index.
 *
 *       See the documentation for @command{gperf(1GNU)} for more details.
=*/
SCM
ag_scm_gperf(SCM name, SCM str)
{
    char const * cmd;
    char const * key2hash = ag_scm2zchars(str,  "key-to-hash");
    char const * gp_name  = ag_scm2zchars(name, "gperf name");

    /*
     *  Format the gperf command and check the result.  If it fits in
     *  scribble space, use that.
     *  (If it does fit, then the test string fits already).
     */
    cmd = aprf(RUN_GPERF_CMD, gp_name, key2hash);
    key2hash = shell_cmd(cmd);
    if (*key2hash == NUL)
        str = SCM_UNDEFINED;
    else
        str = AG_SCM_STR02SCM(key2hash);

    AGFREE((void *)cmd);
    AGFREE((void *)key2hash);
    return str;
}
#endif
/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * indent-tabs-mode: nil
 * End:
 * end of agen5/expGperf.c */
