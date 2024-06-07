dnl#                                               -*- Autoconf -*-
dnl# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.5])
AC_INIT([gdb-etc],[???],[/dev/null])
AC_CONFIG_AUX_DIR([..])
AC_CONFIG_SRCDIR([Makefile.in])

AM_SANITY_CHECK
AM_MAINTAINER_MODE
AM_MAKE_INCLUDE

# Checks for programs.
AC_ARG_PROGRAM
AC_PROG_SED

if test -e `pwd`/config.cache; then
  sed -i.bak -e "s|ac_cv_env_CFLAGS_value|bad_CFLAGS|g" `pwd`/config.cache
elif test -e ./config.cache; then
  sed -i.bak -e "s|ac_cv_env_CFLAGS_value|bad_CFLAGS|g" ./config.cache
else
  echo "config.cache is missing, which means we can skip attempts to hack it"
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
