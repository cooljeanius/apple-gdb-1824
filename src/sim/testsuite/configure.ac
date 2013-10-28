dnl#                                               -*- Autoconf -*-
dnl# Process this file file with autoconf to produce a configure script.
dnl# This file is a shell script fragment that supplies the information
dnl# necessary to tailor a template configure script into the configure
dnl# script appropriate for this directory.  For more information, check
dnl# any existing configure script.

AC_PREREQ([2.5])
AC_INIT([sim-testsuite],[0.0.1],[/dev/null])
AC_CONFIG_SRCDIR([common/bits-tst.c])

my_CC=${CC-cc}
AC_SUBST([my_CC])
AC_CONFIG_AUX_DIR([../..])
AC_CANONICAL_TARGET

AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL

AC_DEFUN([AC_REQUIRE_HEADER_STDC],[
  AC_REQUIRE([AC_HEADER_STDC])
])
AC_REQUIRE_HEADER_STDC
AC_CHECK_HEADERS([stdio.h])

# Directories to use in all configurations.
configdirs=""

# add test sub-directory for appropriate targets
testdir=`echo ${target} | sed -e 's/-.*-/-/'`
if test -r ${srcdir}/${testdir}/configure ; then
  configdirs="${configdirs} $testdir"
fi

# add any extra subdirectories
case $target in
  *) ;;
esac

# Compute the target architecture.
# FIXME: Will need to canonicalize some values.
# FIXME: Should be in generally accessable place.
case $target in
  *) arch=${target_cpu} ;;
esac
AC_SUBST([arch])

# configure the subdirectories too
AC_CONFIG_SUBDIRS([$configdirs])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
