dnl#                                               -*- Autoconf -*-
dnl# Process this file file with autoconf to produce a configure script.
dnl# This file is a shell script fragment that supplies the information
dnl# necessary to tailor a template configure script into the configure
dnl# script appropriate for this directory.  For more information, check
dnl# any existing configure script.

AC_PREREQ([2.5])
AC_INIT([gdb.apple-testsuite],[1824],[])
AC_CONFIG_SRCDIR([cfm.c])
AC_CONFIG_AUX_DIR([../../..])
AC_CONFIG_HEADERS([config.h])

MY_CC=${CC-cc}
AC_SUBST([MY_CC])
AC_CANONICAL_TARGET
AC_OBJEXT
AC_EXEEXT

if test "x$CC" = "x"; then
	test -z "$CC"
	AC_PROG_CC
else
	test ! -z "$CC"
	AC_PROG_GCC_TRADITIONAL
fi
AC_PROG_AWK
AC_PROG_CPP
AC_PROG_CXX
AC_PROG_OBJC

# Checks for typedefs, structures, and compiler characteristics.
AC_C_BIGENDIAN
AC_C_INLINE
AC_C_PROTOTYPES
AC_TYPE_SIZE_T
AC_TYPE_UINT8_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T

# Checks for header files.
AC_HEADER_STDBOOL dnl# AC_CHECK_HEADER_STDBOOL
AC_CHECK_HEADERS([limits.h stdio.h])

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([alarm getcwd sqrt])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
