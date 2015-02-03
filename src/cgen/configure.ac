dnl#                                               -*- Autoconf -*-
dnl# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.59])
AC_INIT([cgen],[1.1],[dje@sebabeach.org])
AC_CONFIG_SRCDIR([read.scm])
AC_CANONICAL_TARGET
if test -f ${srcdir}/config.status; then
  AC_MSG_NOTICE([${srcdir}/config.status is already present, removing it.])
  rm -f ${srcdir}/config.status
fi
AM_INIT_AUTOMAKE([1.9.6 gnits dejagnu])dnl

AC_PROG_INSTALL
AC_EXEEXT

# Set target cpu, and various things that depend upon it:
cpu_arch=${target_cpu}
AC_SUBST([cpu_arch])dnl

AM_MAINTAINER_MODE

# See doc/Makefile.am (this is a hack).
AM_CONDITIONAL([GENINSRC_NEVER],[false])dnl

# Output.
AC_CONFIG_COMMANDS([config_status_vpath_build_fix],[
if test "${srcdir}" != "."; then
  if test -e ${srcdir}/config.status -a ! -e `pwd`/config.status; then
    cp -v ${srcdir}/config.status `pwd`/config.status || echo ""
  elif test -e `pwd`/config.status -a ! -e ${srcdir}/config.status; then
    cp -v `pwd`/config.status ${srcdir}/config.status || echo ""
  fi
fi
],[])dnl
AC_CONFIG_FILES([Makefile doc/Makefile])dnl
AC_OUTPUT
