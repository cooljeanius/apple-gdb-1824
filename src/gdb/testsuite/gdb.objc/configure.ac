dnl#                                               -*- Autoconf -*-
dnl# Process this file file with autoconf to produce a configure script.
dnl# This file is a shell script fragment that supplies the information
dnl# necessary to tailor a template configure script into the configure
dnl# script appropriate for this directory.  For more information, check
dnl# any existing configure script.

AC_PREREQ([2.5])
AC_INIT([gdb.objc-testsuite],[1824],[])
AC_CONFIG_SRCDIR([myclass.m])dnl

MY_CC=${CC-cc}
AC_SUBST([MY_CC])dnl
AC_CONFIG_AUX_DIR([../../..])dnl

# architecture stuff:
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

# Compilers.
AC_PROG_CC
AC_PROG_OBJC
AC_PROG_OBJCPP

AC_C_CONST

# Output:
AC_CONFIG_FILES([Makefile])dnl
AC_OUTPUT
