dnl#                                               -*- Autoconf -*-
dnl# Process this file file with autoconf to produce a configure script.

AC_PREREQ([2.5])
AC_INIT([gdb.chill-testsuite],[1824],[gdb@sources.redhat.com])
dnl# bug report address is from:
dnl# http://sourceware.org/ml/gdb-announce/2002/msg00005.html
AC_CONFIG_SRCDIR([chexp.exp])dnl

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
AC_PROG_CC

# Output:
AC_CONFIG_FILES([Makefile])dnl
AC_OUTPUT
