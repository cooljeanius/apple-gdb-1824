dnl#                                                     -*- Autoconf -*-
dnl# Process this file file with autoconf to produce a configure script.
dnl# This file is a shell script fragment that supplies the information
dnl# necessary to tailor a template configure script into the configure
dnl# script appropriate for this directory.  For more information, check
dnl# any existing configure script.

AC_PREREQ([2.59])
AC_INIT([gdb.hp.defects-testsuite],[1824],[sympa@lists.hp.com])
dnl# found bug report address on HP's website
AC_CONFIG_SRCDIR([bs14602.exp])dnl

MY_CC=${CC-cc}
AC_SUBST([MY_CC])dnl
AC_CONFIG_AUX_DIR([../../../..])dnl

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
if test "x${CC}" = "x"; then
  test -z "${CC}"
  AC_PROG_CC
else
  test ! -z "${CC}"
  if test "x${ac_cv_c_compiler_gnu}" = "x"; then
    test -z "${ac_cv_c_compiler_gnu}"
    if test "x${GCC}" = "xyes" || test "x${CC}" = "xgcc"; then
      export ac_cv_c_compiler_gnu=yes
    else
      export ac_cv_c_compiler_gnu=no
    fi
  fi
  AC_PROG_GCC_TRADITIONAL
fi
AC_PROG_CPP

# Header(s).
AC_CHECK_HEADERS([stdio.h])dnl

# Output:
AC_CONFIG_FILES([Makefile])dnl
AC_OUTPUT
