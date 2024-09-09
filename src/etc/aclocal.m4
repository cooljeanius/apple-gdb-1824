# generated automatically by aclocal 1.17 -*- Autoconf -*-

# Copyright (C) 1996-2024 Free Software Foundation, Inc.

# This file is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY, to the extent permitted by law; without
# even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE.

m4_ifndef([AC_CONFIG_MACRO_DIRS], [m4_defun([_AM_CONFIG_MACRO_DIRS], [])m4_defun([AC_CONFIG_MACRO_DIRS], [_AM_CONFIG_MACRO_DIRS($@)])])
# AM_CONDITIONAL                                            -*- Autoconf -*-

# Copyright (C) 1997-2024 Free Software Foundation, Inc.
#
# This file is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# AM_CONDITIONAL(NAME, SHELL-CONDITION)
# -------------------------------------
# Define a conditional.
AC_DEFUN([AM_CONDITIONAL],
[AC_PREREQ([2.52])dnl
 m4_if([$1], [TRUE],  [AC_FATAL([$0: invalid condition: $1])],
       [$1], [FALSE], [AC_FATAL([$0: invalid condition: $1])])dnl
AC_SUBST([$1_TRUE])dnl
AC_SUBST([$1_FALSE])dnl
_AM_SUBST_NOTMAKE([$1_TRUE])dnl
_AM_SUBST_NOTMAKE([$1_FALSE])dnl
m4_define([_AM_COND_VALUE_$1], [$2])dnl
if $2; then
  $1_TRUE=
  $1_FALSE='#'
else
  $1_TRUE='#'
  $1_FALSE=
fi
AC_CONFIG_COMMANDS_PRE(
[if test -z "${$1_TRUE}" && test -z "${$1_FALSE}"; then
  AC_MSG_ERROR([[conditional "$1" was never defined.
Usually this means the macro was only invoked conditionally.]])
fi])])

# Copyright (C) 1996-2024 Free Software Foundation, Inc.
#
# This file is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# AM_PATH_LISPDIR
# ---------------
AC_DEFUN([AM_PATH_LISPDIR],
[AC_PREREQ([2.60])dnl
 # If set to t, that means we are running in a shell under Emacs.
 # If you have an Emacs named "t", then use the full path.
 test x"$EMACS" = xt && EMACS=
 AC_CHECK_PROGS([EMACS], [emacs xemacs], [no])
 AC_ARG_VAR([EMACS], [the Emacs editor command])
 AC_ARG_VAR([EMACSLOADPATH], [the Emacs library search path])
 AC_ARG_WITH([lispdir],
 [AS_HELP_STRING([--with-lispdir],
                 [override the default lisp directory])],
 [ lispdir="$withval"
   AC_MSG_CHECKING([where .elc files should go])
   AC_MSG_RESULT([$lispdir])],
 [
 AC_CACHE_CHECK([where .elc files should go], [am_cv_lispdir], [
   if test $EMACS != "no"; then
     if test x${lispdir+set} != xset; then
  # If $EMACS isn't GNU Emacs or XEmacs, this can blow up pretty badly
  # Some emacsen will start up in interactive mode, requiring C-x C-c to exit,
  #  which is non-obvious for non-emacs users.
  # Redirecting /dev/null should help a bit; pity we can't detect "broken"
  #  emacsen earlier and avoid running this altogether.
  AC_RUN_LOG([$EMACS -batch -no-site-file -eval '(while load-path (princ (concat (car load-path) "\n")) (setq load-path (cdr load-path)))' </dev/null >conftest.out])
	am_cv_lispdir=`sed -n \
       -e 's,/$,,' \
       -e '/.*\/lib\/x*emacs\/site-lisp$/{s,.*/lib/\(x*emacs/site-lisp\)$,${libdir}/\1,;p;q;}' \
       -e '/.*\/share\/x*emacs\/site-lisp$/{s,.*/share/\(x*emacs/site-lisp\),${datarootdir}/\1,;p;q;}' \
       conftest.out`
       rm conftest.out
     fi
   fi
   test -z "$am_cv_lispdir" && am_cv_lispdir='${datadir}/emacs/site-lisp'
  ])
  lispdir="$am_cv_lispdir"
])
AC_SUBST([lispdir])
])# AM_PATH_LISPDIR

# Add --enable-maintainer-mode option to configure.         -*- Autoconf -*-
# From Jim Meyering

# Copyright (C) 1996-2024 Free Software Foundation, Inc.
#
# This file is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# AM_MAINTAINER_MODE([DEFAULT-MODE])
# ----------------------------------
# Control maintainer-specific portions of Makefiles.
# Default is to disable them, unless 'enable' is passed literally.
# For symmetry, 'disable' may be passed as well.  Anyway, the user
# can override the default with the --enable/--disable switch.
AC_DEFUN([AM_MAINTAINER_MODE],
[m4_case(m4_default([$1], [disable]),
       [enable], [m4_define([am_maintainer_other], [disable])],
       [disable], [m4_define([am_maintainer_other], [enable])],
       [m4_define([am_maintainer_other], [enable])
        m4_warn([syntax], [unexpected argument to AM@&t@_MAINTAINER_MODE: $1])])
AC_MSG_CHECKING([whether to enable maintainer-specific portions of Makefiles])
  dnl maintainer-mode's default is 'disable' unless 'enable' is passed
  AC_ARG_ENABLE([maintainer-mode],
    [AS_HELP_STRING([--]am_maintainer_other[-maintainer-mode],
      am_maintainer_other[ make rules and dependencies not useful
      (and sometimes confusing) to the casual installer])],
    [USE_MAINTAINER_MODE=$enableval],
    [USE_MAINTAINER_MODE=]m4_if(am_maintainer_other, [enable], [no], [yes]))
  AC_MSG_RESULT([$USE_MAINTAINER_MODE])
  AM_CONDITIONAL([MAINTAINER_MODE], [test $USE_MAINTAINER_MODE = yes])
  MAINT=$MAINTAINER_MODE_TRUE
  AC_SUBST([MAINT])dnl
]
)

# Check to see how 'make' treats includes.	            -*- Autoconf -*-

# Copyright (C) 2001-2024 Free Software Foundation, Inc.
#
# This file is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# AM_MAKE_INCLUDE()
# -----------------
# Check whether make has an 'include' directive that can support all
# the idioms we need for our automatic dependency tracking code.
AC_DEFUN([AM_MAKE_INCLUDE],
[AC_MSG_CHECKING([whether ${MAKE-make} supports the include directive])
cat > confinc.mk << 'END'
am__doit:
	@echo this is the am__doit target >confinc.out
.PHONY: am__doit
END
am__include="#"
am__quote=
# BSD make does it like this.
echo '.include "confinc.mk" # ignored' > confmf.BSD
# Other make implementations (GNU, Solaris 10, AIX) do it like this.
echo 'include confinc.mk # ignored' > confmf.GNU
_am_result=no
for s in GNU BSD; do
  AM_RUN_LOG([${MAKE-make} -f confmf.$s && cat confinc.out])
  AS_CASE([$?:`cat confinc.out 2>/dev/null`],
      ['0:this is the am__doit target'],
      [AS_CASE([$s],
          [BSD], [am__include='.include' am__quote='"'],
          [am__include='include' am__quote=''])])
  if test "$am__include" != "#"; then
    _am_result="yes ($s style)"
    break
  fi
done
rm -f confinc.* confmf.*
AC_MSG_RESULT([${_am_result}])
AC_SUBST([am__include])])
AC_SUBST([am__quote])])

# Copyright (C) 2001-2024 Free Software Foundation, Inc.
#
# This file is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# AM_RUN_LOG(COMMAND)
# -------------------
# Run COMMAND, save the exit status in ac_status, and log it.
# (This has been adapted from Autoconf's _AC_RUN_LOG macro.)
AC_DEFUN([AM_RUN_LOG],
[{ echo "$as_me:$LINENO: $1" >&AS_MESSAGE_LOG_FD
   ($1) >&AS_MESSAGE_LOG_FD 2>&AS_MESSAGE_LOG_FD
   ac_status=$?
   echo "$as_me:$LINENO: \$? = $ac_status" >&AS_MESSAGE_LOG_FD
   (exit $ac_status); }])

# Check to make sure that the build environment is sane.    -*- Autoconf -*-

# Copyright (C) 1996-2024 Free Software Foundation, Inc.
#
# This file is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# _AM_SLEEP_FRACTIONAL_SECONDS
# ----------------------------
AC_DEFUN([_AM_SLEEP_FRACTIONAL_SECONDS], [dnl
AC_CACHE_CHECK([whether sleep supports fractional seconds],
               am_cv_sleep_fractional_seconds, [dnl
AS_IF([sleep 0.001 2>/dev/null], [am_cv_sleep_fractional_seconds=yes],
                                 [am_cv_sleep_fractional_seconds=no])
])])

# _AM_FILESYSTEM_TIMESTAMP_RESOLUTION
# -----------------------------------
# Determine the filesystem's resolution for file modification
# timestamps.  The coarsest we know of is FAT, with a resolution
# of only two seconds, even with the most recent "exFAT" extensions.
# The finest (e.g. ext4 with large inodes, XFS, ZFS) is one
# nanosecond, matching clock_gettime.  However, it is probably not
# possible to delay execution of a shell script for less than one
# millisecond, due to process creation overhead and scheduling
# granularity, so we don't check for anything finer than that. (See below.)
AC_DEFUN([_AM_FILESYSTEM_TIMESTAMP_RESOLUTION], [dnl
AC_REQUIRE([_AM_SLEEP_FRACTIONAL_SECONDS])
AC_CACHE_CHECK([filesystem timestamp resolution],
               am_cv_filesystem_timestamp_resolution, [dnl
# Default to the worst case.
am_cv_filesystem_timestamp_resolution=2

# Only try to go finer than 1 sec if sleep can do it.
# Don't try 1 sec, because if 0.01 sec and 0.1 sec don't work,
# - 1 sec is not much of a win compared to 2 sec, and
# - it takes 2 seconds to perform the test whether 1 sec works.
# 
# Instead, just use the default 2s on platforms that have 1s resolution,
# accept the extra 1s delay when using $sleep in the Automake tests, in
# exchange for not incurring the 2s delay for running the test for all
# packages.
#
am_try_resolutions=
if test "$am_cv_sleep_fractional_seconds" = yes; then
  # Even a millisecond often causes a bunch of false positives,
  # so just try a hundredth of a second. The time saved between .001 and
  # .01 is not terribly consequential.
  am_try_resolutions="0.01 0.1 $am_try_resolutions"
fi

# In order to catch current-generation FAT out, we must *modify* files
# that already exist; the *creation* timestamp is finer.  Use names
# that make ls -t sort them differently when they have equal
# timestamps than when they have distinct timestamps, keeping
# in mind that ls -t prints the *newest* file first.
rm -f conftest.ts?
: > conftest.ts1
: > conftest.ts2
: > conftest.ts3

# Make sure ls -t actually works.  Do 'set' in a subshell so we don't
# clobber the current shell's arguments. (Outer-level square brackets
# are removed by m4; they're present so that m4 does not expand
# <dollar><star>; be careful, easy to get confused.)
if (
     set X `[ls -t conftest.ts[12]]` &&
     {
       test "$[]*" != "X conftest.ts1 conftest.ts2" ||
       test "$[]*" != "X conftest.ts2 conftest.ts1";
     }
); then :; else
  # If neither matched, then we have a broken ls.  This can happen
  # if, for instance, CONFIG_SHELL is bash and it inherits a
  # broken ls alias from the environment.  This has actually
  # happened.  Such a system could not be considered "sane".
  _AS_ECHO_UNQUOTED(
    ["Bad output from ls -t: \"`[ls -t conftest.ts[12]]`\""],
    [AS_MESSAGE_LOG_FD])
  AC_MSG_FAILURE([ls -t produces unexpected output.
Make sure there is not a broken ls alias in your environment.])
fi

for am_try_res in $am_try_resolutions; do
  # Any one fine-grained sleep might happen to cross the boundary
  # between two values of a coarser actual resolution, but if we do
  # two fine-grained sleeps in a row, at least one of them will fall
  # entirely within a coarse interval.
  echo alpha > conftest.ts1
  sleep $am_try_res
  echo beta > conftest.ts2
  sleep $am_try_res
  echo gamma > conftest.ts3

  # We assume that 'ls -t' will make use of high-resolution
  # timestamps if the operating system supports them at all.
  if (set X `ls -t conftest.ts?` &&
      test "$[]2" = conftest.ts3 &&
      test "$[]3" = conftest.ts2 &&
      test "$[]4" = conftest.ts1); then
    #
    # Ok, ls -t worked. If we're at a resolution of 1 second, we're done,
    # because we don't need to test make.
    make_ok=true
    if test $am_try_res != 1; then
      # But if we've succeeded so far with a subsecond resolution, we
      # have one more thing to check: make. It can happen that
      # everything else supports the subsecond mtimes, but make doesn't;
      # notably on macOS, which ships make 3.81 from 2006 (the last one
      # released under GPLv2). https://bugs.gnu.org/68808
      # 
      # We test $MAKE if it is defined in the environment, else "make".
      # It might get overridden later, but our hope is that in practice
      # it does not matter: it is the system "make" which is (by far)
      # the most likely to be broken, whereas if the user overrides it,
      # probably they did so with a better, or at least not worse, make.
      # https://lists.gnu.org/archive/html/automake/2024-06/msg00051.html
      #
      # Create a Makefile (real tab character here):
      rm -f conftest.mk
      echo 'conftest.ts1: conftest.ts2' >conftest.mk
      echo '	touch conftest.ts2' >>conftest.mk
      #
      # Now, running
      #   touch conftest.ts1; touch conftest.ts2; make
      # should touch ts1 because ts2 is newer. This could happen by luck,
      # but most often, it will fail if make's support is insufficient. So
      # test for several consecutive successes.
      #
      # (We reuse conftest.ts[12] because we still want to modify existing
      # files, not create new ones, per above.)
      n=0
      make=${MAKE-make}
      until test $n -eq 3; do
        echo one > conftest.ts1
        sleep $am_try_res
        echo two > conftest.ts2 # ts2 should now be newer than ts1
        if $make -f conftest.mk | grep 'up to date' >/dev/null; then
          make_ok=false
          break # out of $n loop
        fi
        n=`expr $n + 1`
      done
    fi
    #
    if $make_ok; then
      # Everything we know to check worked out, so call this resolution good.
      am_cv_filesystem_timestamp_resolution=$am_try_res
      break # out of $am_try_res loop
    fi
    # Otherwise, we'll go on to check the next resolution.
  fi
done
rm -f conftest.ts?
# (end _am_filesystem_timestamp_resolution)
])])

# AM_SANITY_CHECK
# ---------------
AC_DEFUN([AM_SANITY_CHECK],
[AC_REQUIRE([_AM_FILESYSTEM_TIMESTAMP_RESOLUTION])
# This check should not be cached, as it may vary across builds of
# different projects.
AC_MSG_CHECKING([whether build environment is sane])
# Reject unsafe characters in $srcdir or the absolute working directory
# name.  Accept space and tab only in the latter.
am_lf='
'
case `pwd` in
  *[[\\\"\#\$\&\'\`$am_lf]]*)
    AC_MSG_ERROR([unsafe absolute working directory name]);;
esac
case $srcdir in
  *[[\\\"\#\$\&\'\`$am_lf\ \	]]*)
    AC_MSG_ERROR([unsafe srcdir value: '$srcdir']);;
esac

# Do 'set' in a subshell so we don't clobber the current shell's
# arguments.  Must try -L first in case configure is actually a
# symlink; some systems play weird games with the mod time of symlinks
# (eg FreeBSD returns the mod time of the symlink's containing
# directory).
am_build_env_is_sane=no
am_has_slept=no
rm -f conftest.file
for am_try in 1 2; do
  echo "timestamp, slept: $am_has_slept" > conftest.file
  if (
    set X `ls -Lt "$srcdir/configure" conftest.file 2> /dev/null`
    if test "$[]*" = "X"; then
      # -L didn't work.
      set X `ls -t "$srcdir/configure" conftest.file`
    fi
    test "$[]2" = conftest.file
  ); then
    am_build_env_is_sane=yes
    break
  fi
  # Just in case.
  sleep "$am_cv_filesystem_timestamp_resolution"
  am_has_slept=yes
done

AC_MSG_RESULT([$am_build_env_is_sane])
if test "$am_build_env_is_sane" = no; then
  AC_MSG_ERROR([newly created file is older than distributed files!
Check your system clock])
fi

# If we didn't sleep, we still need to ensure time stamps of config.status and
# generated files are strictly newer.
am_sleep_pid=
AS_IF([test -e conftest.file || grep 'slept: no' conftest.file >/dev/null 2>&1],, [dnl
  ( sleep "$am_cv_filesystem_timestamp_resolution" ) &
  am_sleep_pid=$!
])
AC_CONFIG_COMMANDS_PRE(
  [AC_MSG_CHECKING([that generated files are newer than configure])
   if test -n "$am_sleep_pid"; then
     # Hide warnings about reused PIDs.
     wait $am_sleep_pid 2>/dev/null
   fi
   AC_MSG_RESULT([done])])
rm -f conftest.file
])

# Copyright (C) 2006-2024 Free Software Foundation, Inc.
#
# This file is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# _AM_SUBST_NOTMAKE(VARIABLE)
# ---------------------------
# Prevent Automake from outputting VARIABLE = @VARIABLE@ in Makefile.in.
# This macro is traced by Automake.
AC_DEFUN([_AM_SUBST_NOTMAKE])

# AM_SUBST_NOTMAKE(VARIABLE)
# --------------------------
# Public sister of _AM_SUBST_NOTMAKE.
AC_DEFUN([AM_SUBST_NOTMAKE], [_AM_SUBST_NOTMAKE($@)])

