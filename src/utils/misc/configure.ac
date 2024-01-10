dnl#                                               -*- Autoconf -*-
dnl# Process this file with autoconf to produce a configure script.

# This file is a shell script fragment that supplies the information
# necessary to tailor a template configure script into the configure
# script appropriate for this directory.  For more information, check
# any existing configure script.

AC_PREREQ([2.69])
AC_INIT([MiscUtilities],[unknown],[/dev/null])
AC_CONFIG_SRCDIR([Makefile.in])
AC_CONFIG_AUX_DIR([../..])
AC_CANONICAL_TARGET

srctrigger=Makefile.in
srcname="MiscUtilities"
target_dependent=true
AC_SUBST([srctrigger])dnl
AC_SUBST([srcname])dnl
AC_SUBST([target_dependent])dnl

m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES([yes])])dnl

if test "x${V}" = "x"; then
  test -z "${V}" && export V=0
fi
AC_SUBST([V])dnl

# per-host:
case "${host}" in
  *dos*)
    host_subdirs=doschk
    ;;
  *)
    ;;
esac
AC_SUBST([host_subdirs])dnl

# per-target:
case "${target}" in
  *dos*)
    target_subdirs=doschk
    ;;
  *)
    ;;
esac
AC_SUBST([target_subdirs])dnl

configdirs="doschk"
AC_SUBST([configdirs])dnl

# Checks for programs.
AC_PROG_AR
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_RANLIB

# Output
AC_CONFIG_FILES([Makefile])
AC_CONFIG_SUBDIRS([doschk])
AC_OUTPUT

