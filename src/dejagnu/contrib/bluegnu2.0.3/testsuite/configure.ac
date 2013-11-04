dnl#                                               -*- Autoconf -*-
dnl# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([bluegnu-testsuite],[2.0.3],[])
AC_CONFIG_SRCDIR([Makefile.in])

# Checks for programs.
AC_PROG_INSTALL

# Checks for header files.
CY_AC_PATH_TCLH

if test -d calc; then
  test -e calc/configure
  AC_CONFIG_SUBDIRS([calc])
fi

AC_SUBST([host])
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
