#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.12])
AC_INIT([amd-udi],[unknown],[/dev/null])
AC_CONFIG_SRCDIR([mondfe/bkpt.c])
AC_CONFIG_HEADERS([config.h])
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE([foreign dejagnu])
AM_MAINTAINER_MODE

# Checks for programs.
AC_PROG_CC
AC_PROG_CPP
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_RANLIB

# Checks for header files.
AC_CHECK_HEADERS([stdio.h])

AC_CONFIG_FILES([Makefile mondfe/Makefile montip/Makefile udi/Makefile])
AC_OUTPUT
