dnl#                                                     -*- Autoconf -*-
dnl# Process this file file with autoconf to produce a configure script.
dnl# This file is a shell script fragment that supplies the information
dnl# necessary to tailor a template configure script into the configure
dnl# script appropriate for this directory.  For more information, check
dnl# any existing configure script.

AC_PREREQ([2.59])
AC_INIT([gdb.hp-testsuite],[1824],[guo@cup.hp.com])
dnl# bug report address is listed as a "past maintainer" for gdb.hp in
dnl# MAINTAINERS a few levels up, but there is no current one, so a past
dnl# one will have to do...
AC_CONFIG_SRCDIR([gdb.aCC])dnl

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

# Programs.
AS_IF([test "x${CC}" = "x"],[
  test -z "${CC}" || unset CC
  AC_PROG_CC
],[
  test -n "${CC}" && export CC
  if test "x${ac_cv_c_compiler_gnu}" = "x"; then
    test -z "${ac_cv_c_compiler_gnu}" || unset ac_cv_c_compiler_gnu
    if test "x${GCC}" = "xyes" || test "x${CC}" = "xgcc"; then
      export ac_cv_c_compiler_gnu=yes
    else
      export ac_cv_c_compiler_gnu=no
    fi
  fi
])
AC_PROG_CPP
AC_PROG_CXX
AC_PROG_MAKE_SET

# Header(s).
AC_CHECK_HEADERS([stdio.h])dnl

if test "x${ac_cv_header_unistd_h}" = "x"; then
  test -z "${ac_cv_header_unistd_h}"
  AC_CHECK_HEADERS([unistd.h])
fi

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_FORK
AC_FUNC_MALLOC

# Output:
AC_CONFIG_SUBDIRS([gdb.aCC gdb.base-hp gdb.compat gdb.defects gdb.objdbg])
AC_CONFIG_FILES([Makefile])dnl
AC_OUTPUT
