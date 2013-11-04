dnl#                                               -*- Autoconf -*-
dnl# Process this file with autoconf to produce a configure script.
AC_PREREQ([2.69])
AC_INIT([calc],[1.1],[])
AC_CONFIG_SRCDIR([calc.c])
AC_CONFIG_HEADERS([calc.h])
AM_INIT_AUTOMAKE([1.4 dejagnu foreign])
dnl# Yes, that is a "1.4" and not a "1.14" there.
dnl# (the aclocal.m4 I found in here was really old)

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL

#
# Look for various header files
#
AC_CHECK_HEADERS([ctype.h stdio.h])

#
# Look for various functions
#
AC_CHECK_FUNCS([atoi fflush fgets printf strcmp])

AM_MAINTAINER_MODE
	
#
# Output Makefile with substitutions
AC_SUBST([CC])
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
