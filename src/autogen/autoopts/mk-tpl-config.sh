#! /bin/sh

prog=`basename $0 .sh`

die() {
    echo "$prog failure:  $*"
    kill -TERM $progpid
    sleep 1
    exit 1
}

init() {
    PS4='>tpc-${FUNCNAME}> '
    set -e
    progpid=$$
    prog=`basename $0`
    progdir=`\cd \`dirname $0\` >/dev/null ; pwd`
    readonly progpid progdir prog

    for d in top_srcdir srcdir top_builddir builddir
    do
        eval v=\${$d}
        test -d "$v" || die "$d does not reference a directory"
        v=`cd $v >/dev/null && pwd`
        eval ${d}=${v}
    done
}

collect_src() {
    exec 8>&1 1>&2
    cd ${builddir}
    sentinel_file=${1} ; shift
    cat 1>&8 <<- _EOF_
	#include "autoopts/project.h"
	#define  AUTOOPTS_INTERNAL 1
	#include "compat/compat.h"
	#define  LOCAL static
	#include "ao-strs.h"
	_EOF_

    for f in "$@"
    do  test "X$f" = "Xproject.h" || \
            printf '#include "%s"\n' $f
    done 1>&8
}

extension_defines() {
    cd ${builddir}/tpl

    test -f tpl-config.tlib || die "tpl-config.tlib not configured"
    test -f ${top_builddir}/config.h || die "config.h missing"
    grep 'extension-defines' tpl-config.tlib >/dev/null && return

    txt=`sed -n '/POSIX.*SOURCE/,/does not conform to ANSI C/{
	    /^#/p
	}
	/does not conform to ANSI C/q' ${top_builddir}/config.h`

    {
        sed '/define  *top-build-dir/d;/^;;;/d' tpl-config.tlib
        cat <<- _EOF_
	(define top-build-dir   "`cd ${top_builddir} >/dev/null
		pwd`")
	(define top-src-dir     "`cd ${top_srcdir} >/dev/null
		pwd`")
	(define extension-defines
	   "${txt}") \\=]
	_EOF_
    } > tpl-config.$$
    mv -f  tpl-config.$$  tpl-config.tlib
}

set_shell_prog() {
    case `uname -s` in
    SunOS )
      while : ; do
        POSIX_SHELL=`which bash`
        test -x "${POSIX_SHELL}" && break
        POSIX_SHELL=/usr/xpg4/bin/sh
        test -x "${POSIX_SHELL}" && break
        die "You are hosed.  You are on Solaris and have no usable shell."
      done
      ;;
    esac

    for f in ${srcdir}/tpl/*.sh ${srcdir}/tpl/*.pl
    do
        d=`basename $f | sed 's/\.[sp][hl]$//'`
        st=`sed 1q $f`

        case "$st" in
        */perl ) echo '#!' `which perl`
                 sed 1d $f
                 ;;

        */sh )   echo '#!' ${POSIX_SHELL}
                 sed 1d $f
                 ;;

        * )      die "Invalid script type: $st"
                 ;;
        esac > $d
        chmod 755 $d
    done
}

set_cat_prog() {
    while :
    do
        \unalias -a
        unset -f command cat which
        POSIX_CAT=`which cat`
        test -x "$POSIX_CAT" && break
        POSIX_CAT=`
            PATH=\`command -p getconf CS_PATH\`
            command -v cat `
        test -x "${POSIX_CAT}" && break
        die "cannot locate 'cat' command"
    done

    for f in man mdoc texi
    do
        for g in man mdoc texi
        do
            test -f ${f}2${g} || {
                echo '#!' ${POSIX_CAT} > ${f}2${g}
                chmod 755 ${f}2${g}
            }
        done
    done
}

init
collect_src "$@" > ${builddir}/libopts.c
extension_defines
set_shell_prog
set_cat_prog
touch ${sentinel_file}
