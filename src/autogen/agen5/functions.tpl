[= AutoGen5 template h   -*- Mode: C -*-

# functions.tpl
#
## This file is part of AutoGen.
## Copyright (C) 1992-2013 Bruce Korb - all rights reserved
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

=]
[=
(define decl-list        "")
(define load-list        "\nmLoad_Ending")
(define handle-list      "")
(define temp-txt         "")
(define func-name        "")
(define func-str-off     0)
(define output-file-name (out-name))
(define enum-list        "\n")
(define chk-mac-func (lambda () (begin

  (if (and (exist? ".alias") (exist? ".unnamed"))
      (error (string-append
         "The " (get "name") " function is unnamed, but has aliases?!?!")) )

  (if (and (not (exist? ".load-proc"))
           (exist? ".in-context")
           (exist? ".handler-proc") )
      (error (string-append
         "The " (get "name") " function is situational and has a handler.")) )

  (if (exist? "handler-proc") (begin
      (set! temp-txt (get "handler-proc"))

      (if (< 0 (string-length temp-txt))
          (set! temp-txt (string-capitalize! temp-txt))
          (set! temp-txt func-name) )

      (set! handle-list (string-append handle-list
                       "\nmFunc_" temp-txt ))
  )   )

  (if (exist? "load-proc") (begin
      (set! temp-txt (get "load-proc"))

      (if (< 0 (string-length temp-txt))
          (set! temp-txt (string-capitalize! temp-txt))
          (set! temp-txt func-name) )

      (set! load-list (string-append load-list
                       "\nmLoad_" temp-txt ))
  )   )

  (set! temp-txt    (string-upcase! (get "name")))
  (set! enum-list   (string-append enum-list "FTYP_" temp-txt "\n"))

  (string-append
     "\n *  "
     (if (exist? "alias")          "A "
         (if (exist? "unnamed")    "U " "  "))

     (if (exist? "load-proc")      "L "
         (if (exist? "in-context") "C " "  "))

     (if (exist? "unload-proc")    "R " "  ")

     (if (exist? "handler-proc")   "H"  " ")

     (sprintf " - %-10s" temp-txt)
     (get "what")
  )
)))

(dne "-d" " *  " "/*  ")=]
 */
/** \file [= (out-name) =]
 *
 *  Tables of Text Functions for AutoGen
 *
 *  Copyright (C) 1992-2013 by Bruce Korb - all rights reserved
 *
[=(gpl "AutoGen" " *  ")=]
 *
 *  The [=(count "macfunc")
         =] AutoGen macros are tagged with special attributes:
 *
 *  A - is invoked via an alias
 *
 *  U - is unnamed.  May *not* be explicitly invoked.  May not have
 *      have an alias.  These three are used by AutoGen for its purposes.
 *
 *  L - has a special load procedure defined
 *
 *  C - in context.  May be explicitly invoked in certain situations.
 *      For example, "ELSE" may only be specified within an "IF" block.
 *      Their load procedures are enabled by the block macro (e.g. IF),
 *      and disabled by the block ending macro (e.g. ENDIF).
 *      While disabled, the load procedure is the "Bogus" method.
 *
 *      If a function is neither has a special load procedure nor is
 *      situational, then the "Unknown" load method is applied.
 *
 *  R - has a special remove (unload) procedure
 *
 *  H - has a handler procedure defined.  Only these procedures should
 *      be encountered by the dispatcher during processing.
 *[=
FOR macfunc =][=
  (define func-name (string-capitalize! (get "name")))
  (chk-mac-func) =][=
ENDFOR macfunc =]
 *
 * @addtogroup autogen
 * @{
 */
[= (make-header-guard "autogen") =]
/**
 * The number of native AutoGen functions, omitting the
 * comparison type / selection functions.
 */
#define FUNC_CT    [= (count "macfunc") =]

/**
 *  Enumerate all the AutoGen macro types.
 */
typedef enum {
[=
(shell "columns -I4 -S, --ending ',' <<\\_EOF_" enum-list
       "_EOF_")                         =]

    FTYP_SELECT_COMPARE_FULL          = 0x8000,  /* *==* */
    FTYP_SELECT_COMPARE_SKP_START     = 0x8001,  /* *==  */
    FTYP_SELECT_COMPARE_SKP_END       = 0x8002,  /*  ==* */
    FTYP_SELECT_COMPARE               = 0x8003,  /*  ==  */

    FTYP_SELECT_EQUIVALENT_FULL       = 0x8004,  /* *=*  */
    FTYP_SELECT_EQUIVALENT_SKP_START  = 0x8005,  /* *=   */
    FTYP_SELECT_EQUIVALENT_SKP_END    = 0x8006,  /*  =*  */
    FTYP_SELECT_EQUIVALENT            = 0x8007,  /*  =   */

    FTYP_SELECT_MATCH_FULL            = 0x8008,  /* *~~* */
    FTYP_SELECT_MATCH_SKP_START       = 0x8009,  /* *~~  */
    FTYP_SELECT_MATCH_SKP_END         = 0x800A,  /*  ~~* */
    FTYP_SELECT_MATCH                 = 0x800B,  /*  ~~  */

    FTYP_SELECT_EQV_MATCH_FULL        = 0x800C,  /* *~*  */
    FTYP_SELECT_EQV_MATCH_SKP_START   = 0x800D,  /* *~   */
    FTYP_SELECT_EQV_MATCH_SKP_END     = 0x800E,  /*  ~*  */
    FTYP_SELECT_EQV_MATCH             = 0x800F,  /*  ~   */

    FTYP_SELECT_MATCH_ANYTHING        = 0x801C,  /*  *   */
    FTYP_SELECT_MATCH_EXISTENCE       = 0x801D,  /* +E   */
    FTYP_SELECT_MATCH_NONEXISTENCE    = 0x801E   /* !E   */
} mac_func_t;

/**
 *  The function processing procedures.
 */
hdlr_proc_t
[=
(shell (string-append
"{ sort -u | columns --spread=1 -S, -I4 --end=';'\n}<<_EOList_"
    handle-list "\n_EOList_"
)      ) =]

/**
 *  Template Loading Functions.
 */
load_proc_t
[=
(shell (string-append
"{ sort -u | columns --spread=1 -S, -I4 --end=';'\n}<<_EOList_"
    load-list "\n_EOList_"
)      ) =]

/* tpParse.c use only * * * * * * * * * * * * * * * */
/**
 *  Parsing function tables for load processing (template scanning phase).
 */
static load_proc_p_t const base_load_table[FUNC_CT] = {[=
FOR macfunc "," =]
    /* [=% name "%-8s" =] */ mLoad_[=
  IF   (> (len "load-proc") 0)=][=(string-capitalize! (get "load-proc"))=][=
  ELIF (exist? "load-proc")   =][=(string-capitalize! (get "name"))=][=
  ELIF (exist? "in-context")  =]Bogus    /*dynamic*/[=
  ELSE                        =]Unknown  /*default*/[=
  ENDIF =][=
ENDFOR macfunc =]
};

/**
 *  This global pointer is used to switch parsing tables.  The block
 *  functions (CASE, DEFINE, FOR, and IF) change this to point
 *  to their tables that include relevant additional functions.
 */
load_proc_p_t const * load_proc_table = base_load_table;

/**
 *  name-to-function type mapping table.
 *  This table must be sorted alphabetically by the content
 *  of the naming string.
 */
typedef struct fn_name_type fn_name_type_t;
struct fn_name_type {
    size_t        cmpLen;  //!< compare length (sans NUL)
    char const *  pName;   //!< ptr to name
    mac_func_t    fType;   //!< function type enum
};

/**
 *  Define all the strings that are used to determine the function enumeration
 *  number.  These are used in a table separated by aliases and sorted by these
 *  ASCII values.
 */[=
 (set! func-name "")
 (set! decl-list "")          =][=

FOR macfunc                   =][=
  IF (exist? "unnamed")       =][= CONTINUE =][= ENDIF =][=

    IF (exist? "alias")       =][=
      FOR alias               =][=

        (set! func-name (string-append func-name
                 "\"" (if (== (get "alias") "\"") "\\\""
                       (get "alias")) "\\0\"\n"))

        (set! decl-list (string-append decl-list
              (get "alias") " ::    "
              (sprintf "{  1, zFnStrg +%3d, FTYP_%s },\n"
                  func-str-off (string-upcase! (get "name"))  )  ))

        (set! func-str-off (+ func-str-off 2)) =][=

      ENDFOR alias            =][=

    ELSE                      =][=
      (set! temp-txt (string-upcase! (get "name")))
      (set! func-name (string-append func-name "\"" temp-txt "\\0\"\n"))
      (set! decl-list (string-append decl-list
            temp-txt " ::    "
            (sprintf "{ %2d, zFnStrg +%3d, FTYP_%s },\n" (len "name")
                     func-str-off temp-txt  )  ))
      (shellf "%s=%d" temp-txt func-str-off)
      (set! func-str-off (+ func-str-off (len "name") 1))

      =][=

    ENDIF                     =][=
ENDFOR macfunc

=][= (out-push-new)           =]
echo
echo 'static char const zFnStrg[[=(. func-str-off)=]] ='
columns -I4 --spread=1 --end=';'<<\_EOF_
[=(. func-name)=]_EOF_

file="[=(base-name)=]-$$.tmp"
cat > "${file}" <<\_EOF_
[=(. decl-list)=]_EOF_
hict=`egrep    '^[A-Z]' "${file}" | wc -l`
loct=`egrep -v '^[A-Z]' "${file}" | wc -l`

cat <<- _EOF_

	/**
	 *  The number of names by which the macros go.
	 *  Some have multiple names (aliases, e.g. selection clauses).
	 */
	#define FUNC_ALIAS_LOW_INDEX    0
	#define FUNC_ALIAS_HIGH_INDEX   `expr $loct - 1`
	#define FUNC_NAMES_LOW_INDEX    `echo $loct`
	#define FUNC_NAMES_HIGH_INDEX   `expr $hict + $loct - 1`
	#define FUNCTION_NAME_CT        `expr $hict + $loct`

	/* * * * * * * * tpParse.c use only * * * * * * * * * * * * * * */
	/**
	 *  The table separated by aliasing and then sorted by string content
	 */
	static fn_name_type_t const fn_name_types[FUNCTION_NAME_CT] = {
	_EOF_
{
    egrep -v '^[A-Z]' "$file" | sort
    echo
    egrep    '^[A-Z]' "$file" | sort
} | sed -e 's/^.*:://' -e '$s/,$/ };/'

rm -f "$file"

echo
echo 'static char const * const ag_fun_names[FUNC_CT] = {'

{[=

FOR macfunc    =]
  echo [=

  (if (exist? "unnamed")
      (string-append "\\\"" (string-capitalize! (get "name")) "\\\"")
      (if (exist? "alias")
          (string-append "\\\"" (string-upcase! (get "name")) "\\\"")
          (sprintf "zFnStrg+${%1$s:-%s}" (string-upcase! (get "name")))
  )   )

=][=

ENDFOR macfunc      =]
} | columns -I4 -S, --spread=1 --end=' };'
[= (shell (out-pop #t)) =]

/* * * * * * * * tpProcess.c use only * * * * * * * * * * * * * */
/**
 *  Template Processing Function Table
 *
 *  Pointers to the procedure to call when the function code
 *  is encountered.
 */
static hdlr_proc_p_t const load_procs[FUNC_CT] = {[=
  (define set-unload-proc (lambda () (begin
     (set! func-name (string-append
           "mUnload_" (string-capitalize (get "name"))))

     (if (exist? "unload-proc")
         (begin
           (set! unload-procs (string-append unload-procs ", " func-name))
           (set! unload-table (string-append unload-table func-name "\n"))
         )
         (set! unload-table (string-append unload-table "NULL\n"))
     )
  )))
  (define unload-procs "") (define unload-table "") =][=

FOR macfunc "," =][=
  (set-unload-proc)
  (sprintf "\n    /* %-8s */ mFunc_" (get "name")) =][=

  CASE handler-proc =][=
  !E            =]Bogus[=
  == ''         =][= (string-capitalize! (get "name")) =][=
  *             =][= (string-capitalize! (get "handler-proc")) =][=
  ESAC          =][=

ENDFOR macfunc  =]
};

/* * * * * * * * * * tpLoad.c use only * * * * * * * * * * * * * */
/**
 *  Template Unloading Function Table
 *
 *  Pointers to the procedure to call when the function code
 *  is encountered in a template being unloaded.
 */
unload_proc_t [= (substring unload-procs 2) =];

static unload_proc_p_t const unload_procs[FUNC_CT] = {
[= (shell "columns -I4 --sep=, --spread=1 --end=' };'<<_EOF_\n"
      unload-table "_EOF_") =]

[=(shell "set -- `sha1sum " output-file-name " | \
    sed 's/\\(.\\{4\\}\\).*/\\1/' | tr '[a-z]' '[A-Z]'`
    echo '#define FUNCTION_CKSUM 0x'${1}") =]

#endif /* [= (. header-guard) =] */
/** @} */
/* end of [=(. output-file-name)=][=

# end of functions.tpl =] */
