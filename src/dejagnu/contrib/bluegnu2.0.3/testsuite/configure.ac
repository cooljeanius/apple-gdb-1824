dnl#                                               -*- Autoconf -*-
dnl# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([bluegnu-testsuite],[2.0.3],[neurdenburgj@acm.org])
AC_CONFIG_AUX_DIR([../../..])
AC_CONFIG_SRCDIR([Makefile.in])dnl

m4_ifdef([AM_MAINTAINER_MODE],[
AM_MAINTAINER_MODE
])dnl

# Checks for programs.
AC_PROG_INSTALL

# Checks for header files.
CY_AC_PATH_TCLH

if test -d calc; then
  test -x calc/configure && echo "adding calc subdir to config subdirs..."
  AC_CONFIG_SUBDIRS([calc])
fi

# Output.
AC_SUBST([host])dnl
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
