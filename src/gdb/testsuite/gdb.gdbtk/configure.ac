dnl#                                                     -*- Autoconf -*-
dnl# Process this file file with autoconf to produce a configure script.
dnl# This file is a shell script fragment that supplies the information
dnl# necessary to tailor a template configure script into the configure
dnl# script appropriate for this directory.  For more information, check
dnl# any existing configure script.

AC_PREREQ([2.5])
AC_INIT([gdb.gdbtk-testsuite],[1824],[jingham@apple.com])
dnl# bug report address is from documentation on gdbtk
AC_CONFIG_SRCDIR([defs])

MY_CC=${CC-cc}
AC_SUBST([MY_CC])
AC_CONFIG_AUX_DIR([../../..])
AC_CANONICAL_TARGET

dnl# Check for exe extension set on certain hosts (e.g. Win32)
AC_EXEEXT

# Checks for programs
if test "x${CC}" = "x"; then
  test -z "${CC}"
  AC_PROG_CC
else
  test ! -z "${CC}"
  AC_PROG_GCC_TRADITIONAL
fi
AC_PROG_CPP
AC_PROG_CXX

# Checks for library functions.
AC_FUNC_MALLOC

# Output:
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
