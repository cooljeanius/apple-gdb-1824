#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

# This file is a shell script fragment that supplies the information
# necessary to tailor a template configure script into the configure
# script appropriate for this directory.  For more information, check
# any existing configure script.

AC_PREREQ([2.69])
AC_INIT([MiscUtilities],[unknown],[/dev/null])
AC_CONFIG_SRCDIR([Makefile.in])
AC_CANONICAL_TARGET

srctrigger=Makefile.in
srcname="MiscUtilities"
target_dependent=true
AC_SUBST([srctrigger])
AC_SUBST([srcname])
AC_SUBST([target_dependent])

# per-host:
case "${host}" in
  *dos*)
    host_subdirs=doschk
    ;;
  *)
    ;;
esac
AC_SUBST([host_subdirs])

# per-target:
case "${target}" in
  *dos*)
    target_subdirs=doschk
    ;;
  *)
    ;;
esac
AC_SUBST([target_subdirs])

configdirs="doschk"
AC_SUBST([configdirs])

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_RANLIB

# Output
AC_CONFIG_FILES([Makefile])
AC_CONFIG_SUBDIRS([doschk])
AC_OUTPUT

