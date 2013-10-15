dnl#                                               -*- Autoconf -*-
dnl# Process this file with autoconf to produce a configure script.
AC_PREREQ([2.3])dnl
AC_INIT([wince],[???],[/dev/null])
AC_CONFIG_SRCDIR([cesetup.c])
AC_CONFIG_HEADERS([config.h])

my_CC=${CC-cc}
AC_SUBST([my_CC])
AC_PROG_CC
AC_EXEEXT

AC_PROG_INSTALL

AC_CONFIG_AUX_DIR([../..])
AC_CANONICAL_TARGET
AC_ARG_PROGRAM

PROGS='cesetup$(EXEEXT)'
AC_SUBST([PROGS])

# Checks for libraries.
AC_CHECK_LIB([c],[printf])

# Checks for header files.
AC_HEADER_DIRENT
AC_CHECK_HEADERS([stdio.h])

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([exit fclose fopen fprintf fputs strcasecmp strchr \
                strncasecmp strstr])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
