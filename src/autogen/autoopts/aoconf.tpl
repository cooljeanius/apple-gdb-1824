[= AutoGen5 Template -*- Mode: shell-script -*-

in=autoopts-config.in

## Author:          Bruce Korb <bkorb@gnu.org>
##
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
##
## This file is used for bootstrapping only.  Useful _only_ for
## checked out source files.

=]
[=

# = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

AUTOOPTS-CONFIG script:

=][=

(define opt-list (join "\n" (stack "cfg.c-name")))
(define txt      "")
(define name-len 0)
(define name-max 0) =][=

FOR cfg  =][= (set! name-len (string-length (get "c-name")))
              (if (> name-len name-max) (set! name-max name-len)) =][=
ENDFOR   =][=

(set! name-max   (+ 1 name-max))
(define def-fmt  (sprintf "\n%%%ds=\"%%s\""   name-max))
(define def-fmt@ (sprintf "\n%%%ds=\"@%%s@\"" name-max))
(define ele-fmt  (sprintf "\n  %%-%ds)"       name-max))
(define cse-fmt  (sprintf "\n  %%-%ds) val=\"${val} ${%%-%ds ;;"
                         name-max (+ 2 name-max))) =][=

INVOKE script-preamble \=]
optlist="\
[= (shellf "(
sort | columns -I4 --spread=1
) <<_EOF_
%s
_EOF_
" opt-list)  =]"

usage()
{
  test $# -gt 0 && {
    exec 1>&2
    echo autoopts-config error: "$*"
  }

  echo Usage: autoopts-config \<\<OPTION\>\> [ ... ]
  echo Options may be one or more of:

  for o in $optlist
  do  echo "       ${o}"
  done | sed 's,_,-,g'
  echo 'NB: "everything" will print out the list of all names and values.'
  exit $#
}

test $# -gt 0 || usage "No value specified"

# Figure out what's wanted
#
val=''
for o in "$@" ; do
  o=`echo ${o} | sed 's,^-*,,;s/-/_/g'`
  case "$o" in
  help | h | \? )   usage ;;
  *[!a-zA-Z0-9_]* ) usage "Invalid name:  ${o}" ;;
[=

FOR cfg         =][=

  CASE c-name   =][=
  =  everything =][=
     (sprintf ele-fmt "everything") =]
     for o in ${optlist}
     do test ${o} = everything && continue
        eval v=\"\${${o}}\"
        test -z "${v}" && echo ${o} || \
           printf "%-[= (. name-max) =]s $v\n" ${o}
     done
     exit 0
     ;;[=

  *             =][=
     (set! txt (get "c-name"))
     (sprintf cse-fmt txt (string-append txt "}\"")) =][=
  ESAC          =][=

ENDFOR

=]

  * ) usage "Unknown value name:  ${o}" ;;
  esac
done

echo "${val}"
## end of [= (out-name) =]
[=

# = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

MAN PAGE TEMPLATE:  =][=

(out-switch "autoopts-config.1")
(make-tmp-dir)
(define temp-list "")
(dne "\\\" " "\\\" ")

=]
\"
.TH autoopts-config 1 [= `date '+%Y-%m-%d'` =] "" "Programmer's Manual"
.SH NAME
autoopts-config \- script to get information about installed version of
autoopts
.SH SYNOPSIS
.B autoopts-config
.B { <value-name> [...] | everything }
.PP
.SH DESCRIPTION
\fBautoopts-config\fP is a tool that is used by configure to determine
the compile and linker flags that should be used to compile and link
programs that use autoopts.  \fIvalue-name\fPs may be preceded by
one or more hyphens.  They are silently ignored.
.SH "VALUE NAMES"
[=

FOR cfg

  =][=
 (set! txt (string-append tmp-dir "/" (get "c-name")))
 (set! temp-list (string-append temp-list txt "\n"))

 (out-push-new txt) \=]
.TP
.BR [= c-name =]
.sp
[= c-desc =][=
  CASE c-val  =][=
  !E          =]
.br
The unconfigured value is:  @[= c-name =]@[=
  == ""       =][=
  *           =]
.br
The unconfigured value is:  [= c-val =][=
  ESAC        =]
[= (out-pop)  =][=

ENDFOR cfg

=][= (shellf "cat `sort <<_EOF_\n%s_EOF_\n`" temp-list) =]
.SH "SEE ALSO"
.IR Autogen
Info system documentation.
.SH AUTHORS
AutoGen is the work of Bruce Korb <bkorb@gnu.org>.
.br
Bruce Korb <bkorb@gnu.org> and
.br
Luca Filipozzi <lfilipoz@debian.org>
created this manpage.
.PP
AutoOpts is released under either the GNU General Public License with the
Library exception (LGPL), or else the advertising clause free BSD license.
Which license used is at the discretion of the licensee.
\" end of [= (out-name) =]
[=

# = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

AUTOOPTS.PC Generator script     =][=

(out-switch "mk-autoopts-pc.in") =][=

INVOKE script-preamble \=]
PS4='>mk-aopc> '
dirname=`dirname ${1}`
test -d ${dirname} || mkdir -p ${dirname} || exit 1

cat > ${1} <<-  _EOF_
	# pkg-config information for AutoOpts ${dotver}
	#[=

FOR   cfg       =][=
  CASE c-name   =][=
  ~~ everything|cflags|ldflags =][=
  * =][= (sprintf "\n\t%1$s=\"${%1$s}\"" (get "c-name")) =][=
  ESAC c-name   =][=

ENDFOR

=]

	Name:           AutoOpts
	Description:    A semi-automated generated/library option parser
	URL:            http://www.gnu.org/software/autogen
	Version:        ${dotver}
	Libs:           ${ldflags}
	Cflags:         ${cflags}
	_EOF_
## end of [= (out-name) =]
[=

# = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

MACROS:

=][=

DEFINE script-preamble

\=]
#! @CONFIG_[=# defer substitution #=]SHELL@
## ---------------------------------------------------------------------
## [= (out-name) =] -- Describe AutoOpts configuration
##
##  Autoopts Copyright (C) [=`date +1992-%Y`=] by Bruce Korb
##
[= (dne "## ") =]
##[=

FOR cfg       =][=
  CASE c-val  =][=
  !E          =][= (sprintf def-fmt@ (get "c-name") (get "c-name")) =][=
  == ""       =][=
  *           =][= (sprintf def-fmt  (get "c-name") (get "c-val"))  =][=
  ESAC        =][=
ENDFOR

=]
test 'X@ENABLE_STATIC@' = Xno && static_libs=''
case "${libdir}" in
/lib | /lib64 | /usr/lib | /usr/lib64 )
    ldopts=''
    ldflags=-lopts
    ;;

* )
    ldflags="${ldopts} ${ldflags}"
    ;;
esac
libs=${ldflags}
test "${includedir}" = "/usr/include" && cflags=""
[=

ENDDEF script-preamble

=][= #

end of aoconf.tpl  \=]
