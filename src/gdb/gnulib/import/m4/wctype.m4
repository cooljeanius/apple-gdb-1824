# wctype.m4 serial 3
dnl# Copyright (C) 2011-2014 Free Software Foundation, Inc.
dnl# This file is free software; the Free Software Foundation
dnl# gives unlimited permission to copy and/or distribute it,
dnl# with or without modifications, as long as this notice is preserved.

AC_DEFUN([gl_FUNC_WCTYPE],
[
  AC_REQUIRE([gl_WCTYPE_H_DEFAULTS])
  AC_REQUIRE([gl_WCTYPE_H])
  if test "x${HAVE_WCTYPE_T}" != "x"; then
    test -n "${HAVE_WCTYPE_T}" && export HAVE_WCTYPE="${HAVE_WCTYPE_T}"
  else
    test -z "${HAVE_WCTYPE_T}" && HAVE_WCTYPE=""
  fi
  AC_SUBST([HAVE_WCTYPE])
])
