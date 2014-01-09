#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([msdos_gnuish_binaries],[0.0.1],[/dev/null])
AC_CONFIG_SRCDIR([montip.uue])

# This file is a shell script fragment that supplies the information
# necessary to tailor a template configure script into the configure
# script appropriate for this directory.  For more information, check
# any existing configure script.

srctrigger=montip.uue
srcname="gnuish binaries"
target_dependent=true

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AM_PROG_INSTALL_STRIP
AM_PROG_INSTALL_SH
AC_PROG_RANLIB
AC_PATH_PROG([UUDECODE],[uudecode])

# per-host:
AC_CANONICAL_HOST

# per-target:
AC_CANONICAL_TARGET
AC_EXEEXT

# Output:
AC_CONFIG_FILES([Makefile])
AC_OUTPUT

