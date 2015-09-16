[= AutoGen5 Template

  h

(define time-stamp "2013-02-23 06:16:33")

##  This file is part of AutoOpts, a companion to AutoGen.
##  AutoOpts is free software.
##  AutoOpts is Copyright (C) 1992-2013 by Bruce Korb - all rights reserved
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
##  These files have the following sha256 sums:
##
##  8584710e9b04216a394078dc156b781d0b47e1729104d666658aecef8ee32e95  COPYING.gplv3
##  4379e7444a0e2ce2b12dd6f5a52a27a4d02d39d247901d3285c88cf0d37f477b  COPYING.lgplv3
##  13aa749a5b0a454917a944ed8fffc530b784f5ead522b1aacaf4ec8aa55a6239  COPYING.mbsd

=][=

(make-tmp-dir)
(define ref-list "")
(define cch-ct   0)
(dne " * " "/* ") =]
 *
[= (shell "sed -n 's/^##/ */p' " (tpl-file #t)) =]
 */
/** @file [= (out-name) =]
 *
 *  This file handles all the bookkeeping required for tracking all the little
 *  tiny strings used by the AutoOpts library.  There are [= (count "utxt") =]
 *  of them.  This is not versioned because it is entirely internal to the
 *  library and accessed by client code only in a very well-controlled way:
 *  they may substitute translated strings using a procedure that steps through
 *  all the string pointers.
 */
[=
(make-header-guard "autoopts")
=]

/*
 *  One structure to hold all the pointers to all the translatable strings.
 */
typedef struct {
  int           field_ct;[=
FOR utxt        =][=
  (if (exist? "ut-mutable")
      (string-append "\n  char *        utpz_" (get "ut-name") ";")
      (set! cch-ct (+ cch-ct 1))
  )             =][=

ENDFOR  utxt    =]
  char const *  apz_str[[= (. cch-ct) =]];
} usage_text_t;

/*
 *  Declare the global structure with all the pointers to translatable
 *  strings and the text array containing untranslatable strings.
 */
extern usage_text_t option_xlateable_txt;
[=
;; Divert output until we know what the external text block size is
;;
(out-push-new)
(string-table-new  "option_lib_text")
(out-push-new) (out-suspend "strings")
(define str-ix     0)
(define const-list "")
(define typed-list "")
(set!   cch-ct     0)           =][=

FOR utxt                        =][=
  IF
     (ag-fprintf 0 "\n#define z%-20s (option_xlateable_txt." (get "ut-name"))
     (exist? "ut-mutable")

    =]utpz_[= ut-name =])[=

    (set! typed-list (string-append typed-list "\n" (get "ut-name")))
    (ag-fprintf "strings" "\nstatic char %-18s = %s;"
          (string-append "eng_z" (get "ut-name") "[]")
          (kr-string (get "ut-text"))
    )
    =][=

  ELSE                          =][=
    (ag-fprintf 0 "apz_str[%3d])" cch-ct)
    (set! cch-ct (+ 1 cch-ct))
    (set! str-ix (string-table-add "option_lib_text" (get "ut-text")))
    (set! const-list (string-append const-list
                     (sprintf "option_lib_text +%5d\n" str-ix)  ))
    =][=
  ENDIF                         =][=
ENDFOR =]

  /*
   *  First, set up the strings.  Some of these are writable.  These are all in
   *  English.  This gets compiled into libopts and is distributed here so that
   *  xgettext (or equivalents) can extract these strings for translation.
   */[=
(out-resume "strings") (out-pop #t) =][=

(out-push-new (string-append tmp-dir "/usage.txt"))
(emit-string-table "option_lib_text")
(out-pop)
(emit (shell
"usage_extern=$(
  sed -n '/^static char const /{;s/static/extern/;s/ *=$/;/;p;q}' \
  ${tmp_dir}/usage.txt)
sed 's/^static char const /char const /' ${tmp_dir}/usage.txt"))
(out-suspend "txt")
(shell "echo $usage_extern")
=]

#if defined(AUTOOPTS_INTERNAL)
/*
 *  Provide a mapping from a short name to either the text directly
 *  (for untranslatable strings), or to pointers to the text, rendering
 *  them translatable.
 */[=
;; The external declaration for option_lib_text has now been emitted.
;; The rest is only interpreted by the autoopts compilation.
(out-resume "txt") (out-pop #t) =]

/*
 *  Now, define (and initialize) the structure that contains
 *  the pointers to all these strings.
 *  Aren't you glad you don't maintain this by hand?
 */
usage_text_t option_xlateable_txt = {
  [= (count "utxt") =],
[= (shell (string-append
  "CLexe=${AGexe%/agen5/*}/columns/columns
  test -x \"${CLexe}\" || {
    CLexe=${AGexe%/autogen}/columns
    test -x \"${CLexe}\" || die 'columns program is not findable'
  }
  ${CLexe} -W84 -I2 --spread=1 -f'eng_z%s,' <<_EOF_" typed-list
  "\n_EOF_
  echo '    {'
  ${CLexe} -W84 -I4 --spread=1 -S, <<_EOF_\n" const-list
  "_EOF_" )) =]
  } };
#endif /* AUTOOPTS_INTERNAL */

#ifdef XGETTEXT_SCAN_DO_NOT_COMPILE
do not compile this section.
/* TRANSLATORS: The following dummy functions were crated solely so that
 * xgettext can extract the correct strings.  These strings are actually
 * referenced where the preceding "#line" directive states, though you will
 * not see the literal string there.  The literal string is defined above in
 * the @code{option_lib_text} table and referenced via a #define name that
 * redirects into the @code{option_xlateable_txt} structure above.  When
 * translating is activated, the pointers in @code{option_xlateable_txt} are
 * updated to point to translated strings.
 */
static void dummy_func(void) {[=

(out-push-new (string-append (base-name) ".pot"))

\=]
# Automated Option parsing usage text.
# Copyright (C) [=`date +1999-%Y`=] by Bruce Korb - all rights reserved
# This file is distributed under the same licenses as the AutoOpts package.
# Bruce Korb <bkorb@gnu.org>
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

(out-push-new)

=]
str=$(cat << \_EOF_
%s
_EOF_
)
emit_puts z%s >&${%s}[=
(define puts-fmt (out-pop #t))
(out-suspend "pot")
(out-push-new)

=]
exec 4>${tmp_dir}/pot 5>${tmp_dir}/msg 6>${tmp_dir}/use 7>/dev/null
msg=5 use=6 no=7
printf '\n  /* LIBOPTS-MESSAGES: */\n'  >&$msg
printf '  /* END-LIBOPTS-MESSAGES */\n\n  /* USAGE-TEXT: */\n' >&$use

emit_puts() {
    puts="  puts(_($str));"
    f=$(grep -n -w $1 *.c [agpo]*.h | \
        sed 's@\([^:]*:[^:]*\):.*@../\1@')
    test -z "$f" && die "$1 not found in AutoOpts sources"
    printf "\n#: $(echo $f)\nmsgid %s\n" "$str" >&4
    echo "$f" | while IFS=: read s l
    do
        printf '#line %d "%s"\n' $l $s
        echo "$puts"
    done
}[=

FOR utxt  =][=
 (sprintf puts-fmt (c-string (get "ut-text")) (get "ut-name") (get "ut-i18n"))
=][=
ENDFOR utxt

=]
exec 4>&- 5>&- 6>&-
[=
(shell (out-pop #t))
(out-resume "pot")
(emit (shell "cat ${tmp_dir}/pot"))
(out-pop)
(shell "cat ${tmp_dir}/msg ${tmp_dir}/use")=]
  /* END-USAGE-TEXT */
}
#endif /* XGETTEXT_SCAN_DO_NOT_COMPILE */
#endif /* [= (. header-guard)     =] */
[=

# Local Variables:
# Mode: text
# time-stamp-format: "\"%:y-%02m-%02d %02H:%02M:%02S\""
# time-stamp-pattern: "(define time-stamp "
# time-stamp-end: ")"
# End:

\=]
