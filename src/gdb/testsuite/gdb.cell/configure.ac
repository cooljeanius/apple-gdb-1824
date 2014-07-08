dnl#                                               -*- Autoconf -*-
dnl# Process this file file with autoconf to produce a configure script.
dnl# This file is a shell script fragment that supplies the information
dnl# necessary to tailor a template configure script into the configure
dnl# script appropriate for this directory.  For more information, check
dnl# any existing configure script.

AC_PREREQ([2.59])
AC_INIT([gdb.cell-testsuite],[0.0.1],[doko@ubuntu.com])
dnl# bug report address is maintainer of cell-gdb package according to:
dnl# https://launchpad.net/ubuntu/+source/cell-gdb
AC_CONFIG_SRCDIR([arch.exp])

MY_CC=${CC-cc}
AC_SUBST([MY_CC])
AC_CONFIG_AUX_DIR([../../..])
AC_CANONICAL_BUILD
AC_CANONICAL_HOST
AC_CANONICAL_TARGET

if test "x${CC}" = "x"; then
  test -z "${CC}"
  AC_PROG_CC
else
  test ! -z "${CC}"
  AC_PROG_GCC_TRADITIONAL
fi
AC_PROG_CPP

AC_PROG_INSTALL

AC_HEADER_SYS_WAIT
AC_CHECK_HEADERS([stdio.h])

AC_FUNC_FORK

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
