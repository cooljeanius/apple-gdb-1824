dnl#                                               -*- Autoconf -*-
dnl# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.61])
AC_INIT([rem-multi],[old],[])
AC_CONFIG_SRCDIR([remote_gutils.c])
AC_CONFIG_AUX_DIR([..]) dnl# (maybe one level further up?)
AC_CONFIG_MACRO_DIR([../m4])dnl

dnl# set up header:
AC_CONFIG_HEADER([config.h])
AH_TOP([
#ifndef HAVE_CONFIG_H
# define HAVE_CONFIG_H 1
#endif /* !HAVE_CONFIG_H */
])dnl
AH_VERBATIM([00_CONFIG_H_CHECK],
[/* Check that config.h is #included before system headers
  * (this works only for glibc, but that should be enough).  */
#if defined(__GLIBC__) && !defined(__FreeBSD_kernel__) && !defined(__CONFIG_H__)
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "config.h should be #included before system headers"
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* __GLIBC__ && !__FreeBSD_kernel__ && !__CONFIG_H__ */
#ifndef __CONFIG_H__
# define __CONFIG_H__ 1
# ifndef __REM_MULTI_CONFIG_H__
#  define __REM_MULTI_CONFIG_H__ 1
# endif /* __REM_MULTI_CONFIG_H__ */])dnl
AH_BOTTOM([
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  if defined(__GDB_CONFIG_H__)
#   warning "the gdb-general config.h might have already been included"
#  elif defined(__REM_MULTI_CONFIG_H__)
#   warning "this config.h might have already been included"
#  else
#   warning "a different config.h might have already been included, not sure which though"
#  endif /* __GDB_CONFIG_H__ || __MULTI_ICE_CONFIG_H__ */
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* __CONFIG_H__ */
])dnl

# Set up automake.
AC_CANONICAL_TARGET
if test -z "${HOST_ARCHITECTURE}" -a -n "${host_cpu}"; then
  export HOST_ARCHITECTURE="${host_cpu}"
  AC_SUBST([HOST_ARCHITECTURE])
fi
if test -z "${TARGET_ARCHITECTURE}" -a -n "${target_cpu}"; then
  export TARGET_ARCHITECTURE="${target_cpu}"
  AC_SUBST([TARGET_ARCHITECTURE])
fi
AM_INIT_AUTOMAKE([1.11.6 foreign dejagnu])dnl
AM_MAINTAINER_MODE

# Checks for programs.
AC_PROG_CC
AC_PROG_CPP
AC_PROG_CXX
AC_PROG_GCC_TRADITIONAL
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_RANLIB

# Checks for header files.
AC_DEFUN([AC_REQUIRE_HEADER_STDC],[
  AC_REQUIRE([AC_HEADER_STDC])dnl
])dnl
AC_REQUIRE_HEADER_STDC
AC_HEADER_DIRENT
AC_HEADER_SYS_WAIT
unset ac_cv_header_sys_types_h
AC_CHECK_HEADERS([a.out.h errno.h fcntl.h limits.h locale.h mach/kmod.h \
                  machine/reg.h sgtty.h signal.h stdarg.h stddef.h \
                  stdio.h sys/dir.h sys/file.h sys/ioctl.h sys/param.h \
                  sys/ptrace.h sys/types.h sys/user.h varargs.h])dnl
  ## for "../config.h":
EXTRA_DEFS=""
if test "x${ac_cv_header_sys_types_h}" = "xyes"; then
  # this has to go separate from the configure header, because we check
  # for it to be defined at the top of it (for caddr_t for ptrace):
  export DEFS="${DEFS} -DHAVE_SYS_TYPES_H"
  export EXTRA_DEFS="-DHAVE_SYS_TYPES_H"
fi
AC_SUBST([EXTRA_DEFS])dnl

# Checks for typedefs, structures, and compiler characteristics.
AC_C_BIGENDIAN([
  AC_DEFINE([BITS_BIG_ENDIAN],[1],
            [Define to 1 on a big endian system])dnl
  AC_DEFINE([REM_HOST_BYTE_ORDER],[4321],
            [Define to '4321' on a big endian system])dnl
  AC_DEFINE([REM_TARGET_BYTE_ORDER],[4321],
            [Define to '4321' on a big endian system])dnl
],[
  AC_DEFINE([BITS_BIG_ENDIAN],[0],
            [Define to 0 on a little endian system])dnl
  AC_DEFINE([REM_HOST_BYTE_ORDER],[1234],
            [Define to '1234' on a little endian system])dnl
  AC_DEFINE([REM_TARGET_BYTE_ORDER],[1234],
            [Define to '1234' on a little endian system])dnl
],[],[])dnl
AC_C_CONST
AC_C_PROTOTYPES
AC_TYPE_GETGROUPS
AC_TYPE_PID_T
dnl# begin what used to be AC\_TYPE\_SIGNAL
AC_CACHE_CHECK([return type of signal handlers],[ac_cv_type_signal],[
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <sys/types.h>
#include <signal.h>
]],[[return *(signal (0, 0)) (0) == 1;]])],
		   [ac_cv_type_signal=int],
		   [ac_cv_type_signal=void])])dnl
AC_DEFINE_UNQUOTED([RETSIGTYPE],[${ac_cv_type_signal}],
                   [Define as the return type of signal handlers
		    (`int' or `void').])dnl
dnl# end what used to be AC\_TYPE\_SIGNAL
GDB_CHECK_PTRACE

# Checks for library functions.
AC_FUNC_ALLOCA
AC_FUNC_ERROR_AT_LINE
AC_FUNC_MALLOC
AC_FUNC_REALLOC
GDB_FUNC_SETPGRP
AC_FUNC_FORK
AC_CHECK_FUNCS([_exit bcopy clearerr close execle exit fflush fgetc fgets \
                fileno fprintf fputc free gets ioctl perror printf ptrace \
                read signal strcat strcpy strlen wait write])dnl

# Checks for declarations.
AC_CHECK_DECLS([basename])dnl

# Output.
AC_CONFIG_FILES([Makefile])dnl
AC_OUTPUT
