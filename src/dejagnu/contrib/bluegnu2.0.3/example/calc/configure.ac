dnl#                                               -*- Autoconf -*-
dnl# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([bluegnu-example-calc],[2.0.3],[neurdenburgj@acm.org])
AC_CONFIG_SRCDIR([calc.c])
AC_CONFIG_HEADERS([calc.h])

# Checks for programs.
AC_PROG_CC
MY_CC=${CC-cc}

#
# Look for various header files
#
unset ac_cv_header_stdlib_h
AC_CHECK_HEADER([stdlib.h],[],[AC_DEFINE([HAVE_STDLIB_H],[1],[Define to 1 if we have stdlib.h])])
AC_CHECK_HEADERS([ctype.h stdio.h])

#
# Look for various functions
#
AC_CHECK_FUNC([strcmp],[AC_DEFINE([HAVE_STRCMP],[1],[Define to 1 if we have the strcmp function])])
AC_CHECK_FUNCS([atoi fflush fgets printf])

# Checks for declarations
AC_CHECK_DECLS([SIZE])
AC_CHECK_DECLS([NWORD])
AC_CHECK_DECLS([VERSION])

#
# Output Makefile with substitutions
VERSION=${PACKAGE_VERSION}
AC_SUBST([VERSION])
AC_SUBST([MY_CC])
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
