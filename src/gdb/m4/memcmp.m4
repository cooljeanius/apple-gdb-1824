# memcmp.m4 serial 14 -*- Autoconf -*-
dnl# Copyright (C) 2002-2004, 2007, 2009 Free Software Foundation, Inc
dnl# This file is free software; the Free Software Foundation
dnl# gives unlimited permission to copy and/or distribute it,
dnl# with or without modifications, as long as this notice is preserved.

AC_DEFUN([gl_FUNC_MEMCMP],[
  AC_REQUIRE([AC_FUNC_MEMCMP])dnl
  dnl# Note: AC_FUNC_MEMCMP does AC_LIBOBJ(memcmp).
    ## define:
  if test "x${ac_cv_func_memcmp_working}" = "xno"; then
    AC_DEFINE([memcmp],[rpl_memcmp],
      [Define to rpl_memcmp if the replacement function should be used.])
    gl_PREREQ_MEMCMP
  fi
])dnl

dnl# Prerequisites of lib/memcmp.c (none so far):
AC_DEFUN([gl_PREREQ_MEMCMP],[echo "memcmp has no prerequisites"])dnl
