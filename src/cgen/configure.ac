dnl#                                               -*- Autoconf -*-
dnl# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.59])
AC_INIT([cgen],[1.1],[dje@sebabeach.org])
AC_CONFIG_SRCDIR([read.scm])
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE([1.9.6 gnits dejagnu])

AC_PROG_INSTALL
AC_EXEEXT

# Set target cpu.
cpu_arch=${target_cpu}
AC_SUBST([cpu_arch])

AM_MAINTAINER_MODE

# See doc/Makefile.am.
AM_CONDITIONAL([GENINSRC_NEVER],[false])

AC_CONFIG_FILES([Makefile doc/Makefile])
AC_OUTPUT
