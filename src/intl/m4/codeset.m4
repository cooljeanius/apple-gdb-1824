# codeset.m4 serial 6 (gettext-0.19.2)
dnl# Copyright (C) 2000-2002, 2006, 2008-2014 Free Software Foundation, Inc
dnl# This file is free software; the Free Software Foundation
dnl# gives unlimited permission to copy and/or distribute it,
dnl# with or without modifications, as long as this notice is preserved.

dnl# From Bruno Haible.

AC_DEFUN([AM_LANGINFO_CODESET],[
  AC_CACHE_CHECK([for nl_langinfo and CODESET],[am_cv_langinfo_codeset],
    [AC_LINK_IFELSE(
       [AC_LANG_PROGRAM(
          [[#include <langinfo.h>]],
          [[char* cs = nl_langinfo(CODESET); return !cs;]])],
       [am_cv_langinfo_codeset=yes],
       [am_cv_langinfo_codeset=no])
    ])
  if test "x${am_cv_langinfo_codeset}" = "xyes"; then
    AC_DEFINE([HAVE_LANGINFO_CODESET],[1],
      [Define if you have <langinfo.h> and nl_langinfo(CODESET).])
  fi
  AC_CHECK_FUNCS_ONCE([langinfo codeset])dnl
])dnl
