dnl#                                               -*- Autoconf -*-
dnl# Process this file file with autoconf to produce a configure script.
dnl# This file is a shell script fragment that supplies the information
dnl# necessary to tailor a template configure script into the configure
dnl# script appropriate for this directory.  For more information, check
dnl# any existing configure script.

AC_PREREQ([2.70])
AC_INIT
AC_CONFIG_SRCDIR([pthreads.exp])
AC_CONFIG_HEADERS([config.h:config_h.in])

MY_CC=${CC-cc}
AC_SUBST([MY_CC])dnl

AC_CONFIG_AUX_DIR([../../..])
AC_CANONICAL_TARGET

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL

# Checks for header files.
AC_CHECK_HEADERS([pthread.h sys/time.h])dnl

if test "x${ac_cv_header_stdint_h}" = "x"; then
  test -z "${ac_cv_header_stdint_h}"
  AC_CHECK_HEADERS([stdint.h])
fi
if test "x${ac_cv_header_unistd_h}" = "x"; then
  test -z "${ac_cv_header_unistd_h}"
  AC_CHECK_HEADERS([unistd.h])
fi

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_PID_T
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_CHECK_FUNCS([alarm atexit clock_gettime memset select strchr strerror strtol])

# Output.
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
