dnl#                                               -*- Autoconf -*-
dnl# Process this file file with autoconf to produce a configure script.

AC_PREREQ([2.5])
AC_INIT([gdb.chill-testsuite],[1824],[gdb@sources.redhat.com])
dnl# bug report address is from:
dnl# http://sourceware.org/ml/gdb-announce/2002/msg00005.html
AC_CONFIG_SRCDIR([chexp.exp])

MY_CC=${CC-cc}
AC_SUBST([MY_CC])
AC_CONFIG_AUX_DIR([../../..])
AC_CANONICAL_TARGET

if test "x${CC}" = "x"; then
  test -z "${CC}"
  AC_PROG_CC
else
  test ! -z "${CC}"
  AC_PROG_GCC_TRADITIONAL
fi

# Output:
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
