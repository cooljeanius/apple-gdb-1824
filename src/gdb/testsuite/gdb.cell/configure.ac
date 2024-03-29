dnl#                                               -*- Autoconf -*-
dnl# Process this file file with autoconf to produce a configure script.
dnl# This file is a shell script fragment that supplies the information
dnl# necessary to tailor a template configure script into the configure
dnl# script appropriate for this directory.  For more information, check
dnl# any existing configure script.

AC_PREREQ([2.59])
AC_INIT([gdb.cell-testsuite],[0.0.1],[doko@ubuntu.com])
dnl# bug report address is maintainer of cell-gdb package according to:
dnl# https://launchpad.net/ubuntu/+source/cell-gdb
AC_CONFIG_SRCDIR([arch.exp])dnl

MY_CC=${CC-cc}
AC_SUBST([MY_CC])dnl
AC_CONFIG_AUX_DIR([../../..])dnl

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

# Programs.
if test "x${CC_C_O}" = "x"; then
  test -z "${CC_C_O}" || unset CC_C_O
  AM_PROG_CC_C_O
fi
AC_PROG_CC
AC_PROG_CPP

AC_PROG_INSTALL

# Headers.
AC_HEADER_SYS_WAIT
AC_CHECK_HEADERS([stdio.h])dnl

if test "x${ac_cv_header_unistd_h}" = "x"; then
  test -z "${ac_cv_header_unistd_h}" || unset ac_cv_header_unistd_h
  AC_CHECK_HEADERS([unistd.h])
fi

# Function(s).
AC_FUNC_FORK

# Output.
AC_CONFIG_FILES([Makefile])dnl
AC_OUTPUT
