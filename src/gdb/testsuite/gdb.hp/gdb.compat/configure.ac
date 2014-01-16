dnl#                                                     -*- Autoconf -*-
dnl# Process this file file with autoconf to produce a configure script.
dnl# This file is a shell script fragment that supplies the information
dnl# necessary to tailor a template configure script into the configure
dnl# script appropriate for this directory.  For more information, check
dnl# any existing configure script.

AC_PREREQ([2.59])
AC_INIT([gdb.hp.compat-testsuite],[1824],[sympa@lists.hp.com])
dnl# found bug report address on HP's website
AC_CONFIG_SRCDIR([xdb1.exp])

MY_CC=${CC-cc}
AC_SUBST([MY_CC])
AC_CONFIG_AUX_DIR([../../../..])
AC_CANONICAL_TARGET

if test "x${CC}" = "x"; then
	test -z "${CC}"
	AC_PROG_CC
else
	test ! -z "${CC}"
	AC_PROG_GCC_TRADITIONAL
fi
AC_PROG_CPP

AC_CHECK_HEADERS([stdio.h])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
