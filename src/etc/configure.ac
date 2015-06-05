dnl#                                               -*- Autoconf -*-
dnl# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.5])
AC_INIT([gdb-etc],[???],[/dev/null])
AC_CONFIG_SRCDIR([Makefile.in])

AM_SANITY_CHECK
AM_MAINTAINER_MODE
AM_MAKE_INCLUDE

# Checks for programs.
AC_ARG_PROGRAM
AC_PROG_SED

if test -e ./config.cache; then
  sed -i "s|ac_cv_env_CFLAGS_value|bad_CFLAGS|g" ./config.cache
fi

AC_PROG_INSTALL
AC_PROG_MAKE_SET

AM_PATH_LISPDIR

# Checks for header files.
AC_DEFUN([AC_REQUIRE_HEADER_STDC],[
  AC_REQUIRE([AC_HEADER_STDC])dnl
])dnl
AC_REQUIRE_HEADER_STDC

# Output.
AC_CONFIG_FILES([Makefile])dnl
AC_OUTPUT
