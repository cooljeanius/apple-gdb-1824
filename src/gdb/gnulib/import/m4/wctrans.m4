# wctrans.m4 serial 3
dnl# Copyright (C) 2011-2014 Free Software Foundation, Inc.
dnl# This file is free software; the Free Software Foundation
dnl# gives unlimited permission to copy and/or distribute it,
dnl# with or without modifications, as long as this notice is preserved.

AC_DEFUN([gl_FUNC_WCTRANS],
[
  AC_REQUIRE([gl_WCTYPE_H_DEFAULTS])
  AC_REQUIRE([gl_WCTYPE_H])
  if test "x${HAVE_WCTRANS_T}" != "x"; then
    test -n "${HAVE_WCTRANS_T}" && export HAVE_WCTRANS="${HAVE_WCTRANS_T}"
  else
    test -z "${HAVE_WCTRANS_T}" && HAVE_WCTRANS=""
  fi
  AC_SUBST([HAVE_WCTRANS])
])
