dnl#                                               -*- Autoconf -*-
dnl# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.12.1])
AC_INIT([rdi-share],[1.0],[support-sw@arm.com])
AC_CONFIG_SRCDIR([adp.h])
AC_CONFIG_AUX_DIR([../..])
AC_CONFIG_MACRO_DIR([../m4])
AC_CONFIG_HEADERS([config.h])
AC_CANONICAL_TARGET

PACKAGE=rdi-share
VERSION=1.0

dnl# Checks for programs.
AC_PROG_CC
AC_PROG_CPP
AC_PROG_CXX
AC_PROG_GCC_TRADITIONAL
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AC_PROG_RANLIB

dnl# Checks for libraries.
AC_CHECK_LIB([c],[printf])

dnl# Checks for header files.
AC_DEFUN([AC_REQUIRE_HEADER_STDC],[
  AC_REQUIRE([AC_HEADER_STDC])
])
AC_REQUIRE_HEADER_STDC
AC_HEADER_STDBOOL dnl# calls AC_CHECK_HEADER_STDBOOL
AC_HEADER_TIME
AC_SYS_POSIX_TERMIOS
AC_CHECK_HEADERS([arpa/inet.h ctype.h errno.h fcntl.h io.h ioctl.h \
                  malloc.h malloc/malloc.h netdb.h netinet/in.h \
                  signal.h stdarg.h stddef.h stdio.h sbusdev/bpp_io.h \
                  sys/bpp_io.h sys/filio.h sys/ioccom.h sys/ioctl.h \
                  sys/socket.h sys/time.h sys/ttydev.h \
                  term.h termio.h termios.h varargs.h \
                  windows.h wingdi.h winsock.h])

dnl# Checks for typedefs, structures, and compiler characteristics.
AC_C_BIGENDIAN
AC_C_CONST
AC_C_PROTOTYPES
AC_CHECK_DECLS([sys_nerr])
AC_CHECK_DECLS([sys_errlist])
AC_TYPE_SIZE_T
dnl# Begin what used to be AC\_TYPE\_SIGNAL
AC_CACHE_CHECK([return type of signal handlers],[ac_cv_type_signal],[
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <sys/types.h>
#include <signal.h>
]],[[
return *(signal (0, 0)) (0) == 1;
]])],[ac_cv_type_signal=int],[ac_cv_type_signal=void])
])
AC_DEFINE_UNQUOTED([RETSIGTYPE],[${ac_cv_type_signal}],[Define as the return type of signal handlers (`int' or `void').])
dnl# End what used to be AC\_TYPE\_SIGNAL

dnl# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_STRTOD
AC_FUNC_VPRINTF
AC_CHECK_FUNCS([close gethostbyname gettimeofday inet_ntoa ioctl isatty \
                memcpy memmove memset panic recvfrom select socket strcmp \
                strdup strerror strtol strtoul])

# Automake
if test -f ${srcdir}/config.status; then
  AC_MSG_NOTICE([config.status is already present, removing it.])
  rm -f ${srcdir}/config.status
fi
AM_INIT_AUTOMAKE([gnits dejagnu -Wall])
AM_MAINTAINER_MODE
AC_EXEEXT

AC_CONFIG_FILES([Makefile])
AC_CONFIG_COMMANDS([config_status_vpath_build_fix],[
if test "${srcdir}" != "."; then
  if test -e ${srcdir}/config.status -a ! -e `pwd`/config.status; then
    cp -v ${srcdir}/config.status `pwd`/config.status || echo ""
  elif test -e `pwd`/config.status -a ! -e ${srcdir}/config.status; then
    cp -v `pwd`/config.status ${srcdir}/config.status || echo ""
  fi
fi
],[])
AC_OUTPUT


