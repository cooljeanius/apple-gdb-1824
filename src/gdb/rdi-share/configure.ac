dnl#                                               -*- Autoconf -*-
dnl# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.12.1])
AC_INIT([rdi-share],[1.0],[])
AC_CONFIG_SRCDIR([adp.h])
AC_CONFIG_AUX_DIR([../..])
AC_CONFIG_MACRO_DIR([../m4])
AC_CONFIG_HEADERS([config.h])

PACKAGE=rdi-share
VERSION=1.0

dnl# Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AC_PROG_RANLIB

dnl# Checks for libraries.

dnl# Checks for header files.
AC_DEFUN([AC_REQUIRE_HEADER_STDC],[
  AC_REQUIRE([AC_HEADER_STDC])
])
AC_REQUIRE_HEADER_STDC
AC_HEADER_STDBOOL dnl# calls AC_CHECK_HEADER_STDBOOL
AC_CHECK_HEADERS([arpa/inet.h fcntl.h netdb.h netinet/in.h stdarg.h \
                  stddef.h stdio.h sys/filio.h sys/ioctl.h sys/socket.h \
                  sys/time.h termios.h varargs.h winsock.h])

dnl# Checks for typedefs, structures, and compiler characteristics.
AC_PROG_GCC_TRADITIONAL
AC_C_CONST
AC_TYPE_SIZE_T
AC_HEADER_TIME
dnl# Begin what used to be AC\_TYPE\_SIGNAL
AC_CACHE_CHECK([return type of signal handlers],[ac_cv_type_signal],[
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <sys/types.h>
#include <signal.h>
]],[[
return *(signal (0, 0)) (0) == 1;
]])],[ac_cv_type_signal=int],[ac_cv_type_signal=void])
])
AC_DEFINE_UNQUOTED([RETSIGTYPE],[$ac_cv_type_signal],[Define as the return type of signal handlers (`int' or `void').])
dnl# End what used to be AC\_TYPE\_SIGNAL

dnl# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_STRTOD
AC_FUNC_VPRINTF
AC_CHECK_FUNCS([gethostbyname gettimeofday inet_ntoa memcpy memmove \
                memset select socket strdup strerror strtoul])

# Automake
AM_INIT_AUTOMAKE([foreign dejagnu -Wall])
AM_MAINTAINER_MODE
AC_EXEEXT

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
