#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_INIT([insight],[1.0],[insight@sources.redhat.com])
dnl# bug report address taken from README
AC_CONFIG_SRCDIR([generic/gdbtk.h])
AC_CONFIG_AUX_DIR([../..])
AC_CONFIG_MACRO_DIR([../m4])
AC_CANONICAL_TARGET
AC_OBJEXT
AC_EXEEXT

# Checks for programs.
AC_PROG_MAKE_SET
AC_PROG_CC

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h stdio.h sys/ioctl.h sys/time.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_ALLOCA
AC_FUNC_ERROR_AT_LINE
AC_FUNC_REALLOC
AC_CHECK_FUNCS([memset strchr strdup strrchr strtol])

# Automake
AM_INIT_AUTOMAKE([foreign dejagnu -Wall])
AM_MAINTAINER_MODE

# Libtool
LT_INIT([win32-dll])
LT_LANG([C])
LT_LANG([Windows Resource])

AC_CONFIG_SUBDIRS([plugins])
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
