dnl#                                               -*- Autoconf -*-
dnl# Process this file file with autoconf to produce a configure script.
dnl# This file is a shell script fragment that supplies the information
dnl# necessary to tailor a template configure script into the configure
dnl# script appropriate for this directory.  For more information, check
dnl# any existing configure script.

AC_PREREQ([2.5])
AC_INIT([gdb.apple-testsuite],[1824],[opensource@apple.com])
AC_CONFIG_SRCDIR([cfm.c])
AC_CONFIG_AUX_DIR([../../..])
AC_CONFIG_HEADERS([config.h])dnl

if test "x${subdirs}" = "x"; then
  test -z "${subdirs}" && subdirs=""
  AC_SUBST([subdirs])
fi

MY_CC=${CC-cc}
AC_SUBST([MY_CC])dnl

# architecture stuff:
AC_CANONICAL_BUILD
AC_CANONICAL_HOST
AC_CANONICAL_TARGET

if test -z "${BUILD_ARCHITECTURE}" -a -n "${build_cpu}"; then
  export BUILD_ARCHITECTURE="${build_cpu}"
  AC_SUBST([BUILD_ARCHITECTURE])
fi
if test -z "${HOST_ARCHITECTURE}" -a -n "${host_cpu}"; then
  export HOST_ARCHITECTURE="${host_cpu}"
  AC_SUBST([HOST_ARCHITECTURE])
fi
if test -z "${TARGET_ARCHITECTURE}" -a -n "${target_cpu}"; then
  export TARGET_ARCHITECTURE="${target_cpu}"
  AC_SUBST([TARGET_ARCHITECTURE])
fi

AC_OBJEXT
AC_EXEEXT

if test "x${CC_C_O}" = "x"; then
  test -z "${CC_C_O}" || unset CC_C_O
  AM_PROG_CC_C_O
fi
AC_PROG_CC
AC_PROG_AWK
AC_PROG_CPP
AC_PROG_CXX
AC_PROG_OBJC
if test "x${OBJCPP}" = "x" && test -e /lib/cpp; then
  test -z "${OBJCPP}" && test -x /lib/cpp
  AC_PROG_OBJCPP
else
  test -n "${OBJCPP}"
  AC_SUBST([OBJCPP])dnl
  ## just in case... I do something further here? idk...
fi
AC_PATH_PROG([RUBY],[ruby],[ruby])
AC_PATH_PROGS([TIMEOUT],[timeout gtimeout],[timeout])dnl

# Checks for typedefs, structures, and compiler characteristics.
AC_C_BIGENDIAN
AC_C_CONST
AC_C_INLINE
AC_C_PROTOTYPES
AC_C_VOLATILE
AC_TYPE_SIZE_T
AC_TYPE_UINT8_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
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

# Checks for header files.
AC_HEADER_STDBOOL dnl# also "_CHECK"s it
AC_CHECK_HEADERS([limits.h stdio.h])dnl

if test "x${ac_cv_header_stdint_h}" = "x"; then
  test -z "${ac_cv_header_stdint_h}" || unset ac_cv_header_stdint_h
  AC_CHECK_HEADERS([stdint.h])
fi
if test "x${ac_cv_header_unistd_h}" = "x"; then
  test -z "${ac_cv_header_unistd_h}" || unset ac_cv_header_unistd_h
  AC_CHECK_HEADERS([unistd.h])
fi

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_STAT
AC_CHECK_FUNCS([alarm getcwd sqrt])dnl

# Output:
AC_CONFIG_FILES([Makefile \
  debug-in-ofile/Makefile \
  gused/Makefile \
  ld-r/Makefile \
  xcode/Makefile])dnl
AC_OUTPUT
