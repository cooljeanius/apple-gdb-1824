[= AutoGen5 template  -*- Mode: Text -*-

# Time-stamp:        "2011-01-31 14:16:48 bkorb"

##
## This file is part of AutoGen.
## AutoGen Copyright (c) 1992-2012 by Bruce Korb - all rights reserved
##
## AutoGen is free software: you can redistribute it and/or modify it
## under the terms of the GNU General Public License as published by the
## Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
##
## AutoGen is distributed in the hope that it will be useful, but
## WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
## See the GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License along
## with this program.  If not, see <http://www.gnu.org/licenses/>.
##

(setenv "SHELL" "/bin/sh")

ini =]
[=#

This template will emit the code necessary for registering callout routines
for Guile/Scheme.  The name of the output file will be ``basename.ini''
where ``basename'' may be of your choosing.

The following definitions are used:

group      A module prefix that preceeds the "scm_" prefix to all symbols
init       the name of the created initialization routine.  This defaults
           to "scm_init" or "group_init", if "group" is specified.
           You must specify this for shared libraries.
init-code  code to put at the start of the init routine
fini-code  code to put at the end of the init routine

gfunc      this is a compound definition containing the following definitions
  name     the name of the function.  The Scheme string name will normally be
           derived from this name, but it may be over-ridden with the "string"
           attribute.  The transforming sed expression is:

                sed -e's/_p$/?/;s/_x$/!/;s/_/-/g;s/-to-/->/'

  string   the Scheme name for the function, if not derivable from the name.
  static   If defined, then the function will not be exported.

  exparg   "EXPression ARGument"  for each argument your routine handles,
           you must specify one of these.  This is a compound definition
           with the following "attributes" that may be defined:

    arg-name      The name of the argument.  required
    arg-desc      A very brief description of the argument.  required
    arg-optional  Specify this if the argument is only optional
    arg-list      Specify this for the last argument if the last argument
                  may be an SCM list (i.e. an SCM-flavor of var args).

syntax     This defines a Guile syntax element.  Read the Guile doc for
           descriptions of the following attributes:
  name       the name of the C variable to hold the value
  type
  cfn
  string     the Scheme name for the syntax element, if not derivable.

symbol     This defines a Guile symbol.
  name       the name of the C variable to hold the value
  init-val   initial scm value for object
  const-val  initial integer value for object (signed long)
  string     the Scheme name for the symbol, if not derivable.

If you are using a definitions file, these are defined in the normal
way.  If you are extracting them from `getdefs(1AG)' comments, then:

1.  `group' and `init' should be defined on the command line thus:
        getdefs assign=group=XX  assign=init=init_proc_name

2.  `init-code' and `fini-code' should be defined in a traditional
    definitions file and be incorporated from a command line option:
        getdefs copy=file-name.def

3.  `gfunc', `syntax', and `symbol' are getdefs' entry types.
    The `name' attributes come from the getdefs entry name.
    The remaining attributes are specified in the comment, per
    the getdefs documentation.

=][=

(define ix 0)
(define scm-prefix
        (if (exist? "group")
            (string-append (get "group") "_scm_")
            "scm_" ))
(out-push-new (string-append (base-name) ".h"))
(dne " *  " "/* ")=]
 *
 *  copyright (c) 1992-2012 Bruce Korb - all rights reserved
 *
[=(gpl "AutoGen" " *  ")=]
 *
 *  Guile Implementation Routines[=% group " - for the %s group" =]
 */
[=(make-header-guard  "GUILE_PROCS")=]
#if GUILE_VERSION >= 108000
# include <libguile.h>
#else
# include <guile/gh.h>
#endif

typedef enum {
    GH_TYPE_UNDEFINED = 0,
    GH_TYPE_BOOLEAN,
    GH_TYPE_SYMBOL,
    GH_TYPE_CHAR,
    GH_TYPE_VECTOR,
    GH_TYPE_PAIR,
    GH_TYPE_NUMBER,
    GH_TYPE_STRING,
    GH_TYPE_PROCEDURE,
    GH_TYPE_LIST,
    GH_TYPE_INEXACT,
    GH_TYPE_EXACT
} teGuileType;
[=
FOR gfunc       =]
extern SCM [= (string-append scm-prefix (get "name") "(") =][=
  IF (exist? "exparg") =][=
    FOR exparg ", " =]SCM[=
    ENDFOR      =][=
  ELSE          =]void[=
  ENDIF         =]);[=
ENDFOR gfunc    =][=

FOR symbol      =][=
  IF (exist? "global") =]
extern SCM [= (string-append scm-prefix "sym_" (get "name") ";") =][=
  ENDIF         =][=
ENDFOR symbol   =]

#endif /* [=(. header-guard)=] */
[=

(out-pop)
(dne " *  " "/* ")

=]
 *
 *  copyright (c) 1992-2012 Bruce Korb - all rights reserved
 *
[=
(string-table-new "g_nm")

(define add-to-g_nm (lambda ()
  (string-table-add "g_nm"
    (if (exist? "string")
        (get "string")
        (shellf
          "echo '%s'|sed -e's/_p$/?/' -e's/_x$/!/' -e's/_/-/g' -e's/-to-/->/'"
          (get "name")  )  ) ) ))

(gpl "AutoGen" " *  ")=]
 *
 *  Guile Initializations - [=% group (string-capitalize! "%s ")
                            =]Global Variables
 */
#include "[= (. header-file) =]"
typedef SCM (*scm_callback_t)(void);
void [=
(define init-proc
      (if (exist? "init")
          (get "init")
          (if (exist? "group")
              (string-append (get "group") "_init")
              "scm_init")))

   init-proc    =](void);
[=

  FOR symbol =][=
    (sprintf "\n%s SCM %ssym_%-18s = SCM_BOOL_F;"
             (if (exist? "global") "extern" "static")
             scm-prefix (get "name") )  =][=
  ENDFOR symbol

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */=][=

IF (exist? "debug-enabled")     =]
#ifdef DEBUG_ENABLED[=

  FOR     gfunc

=]
static SCM
agrelay_scm_[= (get "name")     =]([=
    IF (not (exist? "exparg"))  =]void[=
         (define pass-list "")  =][=
    ELSE                        =][=
         (out-push-new)         =][=
         FOR exparg ", "        =]SCM scm[= (for-index) =][= ENDFOR =][=
         (define call-list (out-pop #t))
         (define pass-list (shellf "echo '%s' | sed 's/SCM s/s/g'" call-list))
         call-list              =][=
    ENDIF exparg exists/not     =])
{
    if (OPT_VALUE_TRACE >= TRACE_EVERYTHING) {
        static char const proc_z[] =
            "Called ag_scm_[= name =]()\n";
        fwrite(proc_z, sizeof(proc_z) - 1, 1, pfTrace);
    }
    return ag_scm_[= name =]([= (. pass-list) =]);
}
[= ENDFOR  gfunc                =]
#if GUILE_VERSION >= 108000
#define NEW_PROC(_As, _Ar, _Ao, _Ax, _An)   \
  scm_c_define_gsubr((char*)(_As),          \
                   _Ar, _Ao, _Ax, (scm_callback_t)(void*)agrelay_scm_ ## _An)
#else
#define NEW_PROC(_As, _Ar, _Ao, _Ax, _An)                                    \
  gh_new_procedure((char*)(_As), (scm_callback_t)(void*)agrelay_scm_ ## _An, \
                   _Ar, _Ao, _Ax)
#endif

#else /* DEBUG_ENABLED *not* */[=

ENDIF debug-enabled exists

=]
#if GUILE_VERSION >= 108000
#define NEW_PROC(_As, _Ar, _Ao, _Ax, _An)   \
  scm_c_define_gsubr((char*)(_As),          \
                   _Ar, _Ao, _Ax, (scm_callback_t)(void*)ag_scm_ ## _An)
#else
#define NEW_PROC(_As, _Ar, _Ao, _Ax, _An)                                   \
  gh_new_procedure((char*)(_As), (scm_callback_t)(void*)ag_scm_ ## _An,     \
                   _Ar, _Ao, _Ax)
#endif
[= (if (exist? "debug-enabled") "#endif /* DEBUG_ENABLED */\n") =]
/*
 * [=group=] Initialization procedure.
 */
void
[=(. init-proc)=](void)
{[=

  (out-push-new)

  (if (exist? "init-code")
      (prefix "    " (get "init-code")) "") =][=

  FOR  gfunc            =][=
    INVOKE mk-new-proc  =][=
  ENDFOR gfunc          =][=

  FOR  syntax           =]
    scm_make_synt(g_nm+[= (add-to-g_nm) =], [=type=], [=cfn=]);[=
  ENDFOR syntax         =][=

  FOR symbol =]
    [=(. scm-prefix)=]sym_[=name=] = scm_permanent_object[=
    IF (not (and (exist? "init_val") (exist? "const_val")))
         =](SCM_CAR (scm_intern0 (g_nm+[= (add-to-g_nm) =])));[=

    ELSE            =](scm_intern0 (g_nm+[= (add-to-g_nm) =]));
    SCM_SETCDR ([=(. scm-prefix)=]sym_[=name=], [=
      ?% init_val "%s" (sprintf "scm_long2num(%s)" (get "const_val"))=]);[=
    ENDIF           =][=
  ENDFOR symbol     =][=

  (out-suspend "main")
  (emit-string-table "g_nm")
  (out-resume "main")
  (out-pop #t)      =][=
  (if (exist? "fini-code")
      (prefix "    " (get "fini-code")) "") =]
}
#undef NEW_PROC
/* end of [= (out-name) =] */
[= #

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
=][=

DEFINE mk-new-proc =][=

   (set! ix (add-to-g_nm))

   (ag-fprintf 0 "\n    NEW_PROC(g_nm +%4d, " ix) =][=

   IF (not (exist? "exparg"))

    =]0, 0, 0[=

   #  Count of all the arguments:       (count "exparg")
      Of these, some may be optional:   (count "exparg.arg_optional")
      Of the optional, one may be an arg_list.
      The sum of the three numbers must be:  (count "exparg")  =][=

   ELIF (not (exist? "exparg.arg_list")) =][=
    (- (count "exparg") (count "exparg.arg_optional")) =], [=
    (count "exparg.arg_optional" ) =], 0[=

   ELIF (not (exist? "exparg.arg_optional")) =][=
    (- (count "exparg") 1) =], 0, 1[=

   ELSE  =][=
    (- (count "exparg") (count "exparg.arg_optional")) =], [=
    (- (count "exparg.arg_optional" ) 1) =], 1[=
   ENDIF =], [=

   name =]);[=

ENDDEF =][=

end of snarf.tpl \=]
