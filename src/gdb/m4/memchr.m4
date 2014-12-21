# memchr.m4 serial 6 -*- Autoconf -*-
dnl# Copyright (C) 2002, 2003, 2004, 2009 Free Software Foundation, Inc.
dnl# This file is free software; the Free Software Foundation
dnl# gives unlimited permission to copy and/or distribute it,
dnl# with or without modifications, as long as this notice is preserved.

AC_DEFUN([gl_FUNC_MEMCHR],[
  AC_REPLACE_FUNCS([memchr])dnl
  ## in case of failure:
  if test "x${ac_cv_func_memchr}" = "xno"; then
    gl_PREREQ_MEMCHR
  fi
])dnl

dnl# Prerequisites of lib/memchr.c:
AC_DEFUN([gl_PREREQ_MEMCHR],[
  AC_CHECK_HEADERS([bp-sym.h])dnl
])dnl
