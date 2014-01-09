[= AutoGen5 Template

  h  pot

(define time-stamp "2012-02-18 10:10:47")

##  This file is part of AutoOpts, a companion to AutoGen.
##  AutoOpts is free software.
##  AutoOpts is Copyright (c) 1992-2012 by Bruce Korb - all rights reserved
##
##  AutoOpts is available under any one of two licenses.  The license
##  in use must be one of these two and the choice is under the control
##  of the user of the license.
##
##   The GNU Lesser General Public License, version 3 or later
##      See the files "COPYING.lgplv3" and "COPYING.gplv3"
##
##   The Modified Berkeley Software Distribution License
##      See the file "COPYING.mbsd"
##
##  These files have the following md5sums:
##
##  43b91e8ca915626ed3818ffb1b71248b COPYING.gplv3
##  06a1a2e4760c90ea5e1dad8dfaac4d39 COPYING.lgplv3
##  66a5cedaf62c4b2637025f049f9b826f COPYING.mbsd

=][=  CASE (suffix) =][=

==  h               =][=

(define ref-list "")
(define cch-ct   0)
(dne " *  " "/*  ") =]
 *
 *  This file handles all the bookkeeping required for tracking all the little
 *  tiny strings used by the AutoOpts library.  There are [= (count "utxt") =]
 *  of them.  This is not versioned because it is entirely internal to the
 *  library and accessed by client code only in a very well-controlled way:
 *  they may substitute translated strings using a procedure that steps through
 *  all the string pointers.
 *
[= (license-full "lgpl" "AutoOpts" " *  " "Bruce Korb" (shell "date +1992-%Y"))
 =]
 */
[=
(make-header-guard "autoopts")
=]

#undef  cch_t
#define cch_t char const

/*
 *  One structure to hold all the pointers to all the stringlets.
 */
typedef struct {
  int       field_ct;[=
FOR utxt        =][=
  (if (exist? "ut-type")
      (emit (sprintf "\n  %-9s utpz_%s;"
                     (string-append (get "ut-type") "*")
                     (get "ut-name")  ))
      (set! cch-ct (+ cch-ct 1))  ) =][=

ENDFOR  utxt    =]
  cch_t*    apz_str[ [= (. cch-ct) =] ];
} usage_text_t;

/*
 *  Declare the global structure with all the pointers to translated
 *  strings.  This is then used by the usage generation procedure.
 */
extern usage_text_t option_usage_text;

#if defined(AUTOOPTS_INTERNAL) /* DEFINE ALL THE STRINGS = = = = = */
/*
 *  Provide a mapping from a short name to fields in this structure.
 */[=

(string-table-new  "usage_txt")
(define str-ix     0)
(define const-list "")
(define typed-list "")
(set! cch-ct 0)    =][=

FOR utxt        =]
#define z[= (sprintf "%-20s" (get "ut-name"))
        =] (option_usage_text.[=

  IF (exist? "ut-type") =]utpz_[= ut-name =][=
     (set! typed-list (string-append typed-list "\n" (get "ut-name"))) =][=
  ELSE
    =][=
    (ag-fprintf 0 "apz_str[%3d]" cch-ct)
    (set! cch-ct (+ 1 cch-ct))
    =][=
  ENDIF                 =])[=
ENDFOR =]

  /*
   *  First, set up the strings.  Some of these are writable.  These are all in
   *  English.  This gets compiled into libopts and is distributed here so that
   *  xgettext (or equivalents) can extract these strings for translation.
   */
[=
FOR utxt  =][=
  (if (exist? "ut-type")
      (sprintf "\n  static %-7s eng_z%s[] = %s;"
              (get "ut-type") (get "ut-name") (kr-string (get "ut-text"))
      )

      (begin
        (set! str-ix (string-table-add "usage_txt" (get "ut-text")))
        (set! const-list (string-append const-list
                         (sprintf "usage_txt +%4d\n" str-ix)  ))
  )   )
  =][=
ENDFOR  utxt        =][=

(emit-string-table "usage_txt")

=]

  /*
   *  Now, define (and initialize) the structure that contains
   *  the pointers to all these strings.
   *  Aren't you glad you don't maintain this by hand?
   */
  usage_text_t option_usage_text = {
    [= (count "utxt") =],
[= (shell (string-append
  "CLexe=${AGexe%/agen5/*}/columns/columns
  test -x \"${CLexe}\" || {
    CLexe=${AGexe%/autogen}/columns
    test -x \"${CLexe}\" || die 'columns program is not findable'
  }
  ${CLexe} -W84 -I4 --spread=1 -f'eng_z%s,' <<_EOF_" typed-list
  "\n_EOF_
  echo '    {'
  ${CLexe} -W84 -I6 --spread=1 -S, <<_EOF_\n" const-list
  "_EOF_" )) =]
    }
  };

#endif /* DO_TRANSLATIONS */
#endif /* [= (. header-guard) =] */
[=


==  pot


\=]
# Automated Option parsing usage text.
# copyright (c) [=`date +1999-%Y`=] by Bruce Korb - all rights reserved
# This file is distributed under the same license as the AutoOpts package.
# Bruce Korb <bkorb@gnu.org> [=`date +%Y`=]
#
#, fuzzy
msgid ""
msgstr ""
"Project-Id-Version: autogen [=`echo ${AG_VERSION}`=]\n"
"Report-Msgid-Bugs-To: autogen-users@lists.sourceforge.net\n"
"POT-Creation-Date: [=`date '+%Y-%m-%d %H:%M%z'`=]\n"
"PO-Revision-Date: YEAR-MO-DA HO:MI+ZONE\n"
"Last-Translator: FULL NAME <EMAIL@ADDRESS>\n"
"Language-Team: LANGUAGE <LL@li.org>\n"
"MIME-Version: 1.0\n"
"Content-Type: text/plain; charset=CHARSET\n"
"Content-Transfer-Encoding: 8bit\n"
[=

FOR utxt
  =][=
  (set! ref-list (shellf
     "list=`grep -n -w z%s *.c [agpo]*.h tpl/opt*.t* | \\
         sed -n 's/\\([^:]*:[^:]*\\):.*/\\1/p'`
     echo ${list}" (get "ut-name")))

  (if (< (string-length ref-list) 2)
      (error (sprintf "No references to z%s string" (get "ut-name")))  )

  (sprintf "\n#: %s\nmsgid %s\n" ref-list (c-string (get "ut-text")))

  =][=
ENDFOR utxt

=]
[=

ESAC

# Local Variables:
# Mode: text
# time-stamp-format: "\"%:y-%02m-%02d %02H:%02M:%02S\""
# time-stamp-pattern: "(define time-stamp "
# time-stamp-end: ")"
# End:

\=]
