dnl#                                               -*- Autoconf -*-
dnl# Process this file with autoconf to produce a configure script.

AC_INIT([insight],[1.0],[insight@sources.redhat.com])
dnl# (bug report address taken from README)
AC_CONFIG_SRCDIR([generic/gdbtk.h])
AC_CONFIG_AUX_DIR([../..])
AC_CONFIG_MACRO_DIR([../m4])
AC_CANONICAL_TARGET
AC_OBJEXT
AC_EXEEXT

# Checks for programs.
AC_PROG_MAKE_SET
AC_PROG_CC
m4_ifdef([AM_PROG_CC_C_O],[
  AM_PROG_CC_C_O
])dnl

# Checks for header files.
AC_DEFUN([AC_REQUIRE_HEADER_STDC],[
  AC_REQUIRE([AC_HEADER_STDC])dnl
])dnl
AC_CHECK_HEADERS([fcntl.h stdio.h sys/ioctl.h sys/time.h])dnl

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_C_PROTOTYPES
AC_C_VOLATILE
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_ALLOCA
AC_FUNC_ERROR_AT_LINE
AC_FUNC_MEMCMP
AC_FUNC_REALLOC
AC_FUNC_STAT
AC_CHECK_FUNCS([memset strchr strdup strrchr strtol])dnl

# Automake:
AM_INIT_AUTOMAKE([1.11.6 foreign dejagnu no-dist -Wall -Wno-obsolete])
AM_MAINTAINER_MODE
m4_ifdef([AC_REQUIRE_AUX_FILE],[
  AC_REQUIRE_AUX_FILE([depcomp])dnl
])dnl

dnl########################################################################
dnl# Port to Automake 1.11.
dnl# This section can be removed once we assume Automake 1.14 or later.
: ${AM_DEFAULT_VERBOSITY=1}
: ${AM_V=${AM_DEFAULT_VERBOSITY}}
: ${AM_DEFAULT_V=${AM_DEFAULT_VERBOSITY}}
dnl# now subst:
AC_SUBST([AM_V])dnl
AM_SUBST_NOTMAKE([AM_V])dnl
AC_SUBST([AM_DEFAULT_V])dnl
AM_SUBST_NOTMAKE([AM_DEFAULT_V])dnl
AC_SUBST([AM_DEFAULT_VERBOSITY])dnl

# Libtool:
LT_INIT([win32-dll])dnl
LT_LANG([C])dnl
LT_LANG([Windows Resource])dnl

# Output:
AC_CONFIG_SUBDIRS([plugins])dnl
AC_CONFIG_FILES([Makefile])dnl
AC_OUTPUT
