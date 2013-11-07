dnl#                                               -*- Autoconf -*-
dnl# Process this file file with autoconf to produce a configure script.
dnl# This file is a shell script fragment that supplies the information
dnl# necessary to tailor a template configure script into the configure
dnl# script appropriate for this directory.  For more information, check
dnl# any existing configure script.

AC_PREREQ([2.5])
AC_INIT([gdb.objc-testsuite],[1824],[])
AC_CONFIG_SRCDIR([myclass.m])

MY_CC=${CC-cc}
AC_SUBST([MY_CC])
AC_CONFIG_AUX_DIR([../../..])
AC_CANONICAL_TARGET

if test "x$CC" = "x"; then
	test -z "$CC"
	AC_PROG_CC
else
	test ! -z "$CC"
	AC_PROG_GCC_TRADITIONAL
fi

AC_PROG_OBJC
AC_PROG_OBJCPP

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
