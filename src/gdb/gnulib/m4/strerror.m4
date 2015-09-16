# strerror.m4 serial 18 -*- Autoconf -*-
dnl# Copyright (C) 2002, 2007-2014 Free Software Foundation, Inc.
dnl# This file is free software; the Free Software Foundation
dnl# gives unlimited permission to copy and/or distribute it,
dnl# with or without modifications, as long as this notice is preserved.

AC_DEFUN([gl_FUNC_STRERROR],[
  AC_REQUIRE([gl_HEADER_STRING_H_DEFAULTS])dnl
  AC_REQUIRE([gl_HEADER_ERRNO_H])dnl
  AC_REQUIRE([gl_FUNC_STRERROR_0])dnl
  AC_REQUIRE([AC_CANONICAL_HOST]) dnl# for cross-compiles
  m4_ifdef([gl_FUNC_STRERROR_R_WORKS],[
    AC_REQUIRE([gl_FUNC_STRERROR_R_WORKS])dnl
  ])dnl
  ## need to check both:
  if test "${ERRNO_H}:${REPLACE_STRERROR_0}" = ":0"; then
    AC_CACHE_CHECK([for working strerror function],
     [gl_cv_func_working_strerror],
     [AC_RUN_IFELSE(
        [AC_LANG_PROGRAM([[#include <string.h>]],
                         [[if (!*strerror(-2)) return 1;]])],
        [gl_cv_func_working_strerror=yes],
        [gl_cv_func_working_strerror=no],
        [case "${host_os}" in
                   # Guess yes on glibc systems:
           *-gnu*) gl_cv_func_working_strerror="guessing yes" ;;
                   # If we do NOT know, then assume the worst:
           *)      gl_cv_func_working_strerror="guessing no" ;;
         esac
        ])dnl
    ])dnl
    ## ...based on that, do this:
    case "${gl_cv_func_working_strerror}" in
      *yes) ;;
      *)
        dnl# The system strerror() fails to return a string for
        dnl# out-of-range integers, so replace it:
        REPLACE_STRERROR=1
        ;;
    esac
    m4_ifdef([gl_FUNC_STRERROR_R_WORKS],[
      dnl# If the system strerror_r or __xpg_strerror_r clobbers strerror's
      dnl# buffer, then we must replace strerror:
      case "${gl_cv_func_strerror_r_works}" in
        *no) REPLACE_STRERROR=1 ;;
      esac
    ])
  else
    dnl# The system strerror() cannot know about the new errno values that
    dnl# we add to <errno.h>, or any fix for strerror(0), so replace it:
    REPLACE_STRERROR=1
  fi
])dnl

dnl# Detect if strerror(0) passes (that is, neither sets errno, nor returns
dnl# a string that matches strerror(-1)):
AC_DEFUN([gl_FUNC_STRERROR_0],[
  AC_REQUIRE([AC_CANONICAL_HOST]) dnl# for cross-compiles
  REPLACE_STRERROR_0=0
  AC_CACHE_CHECK([whether strerror(0) succeeds],
   [gl_cv_func_strerror_0_works],
   [AC_RUN_IFELSE(
      [AC_LANG_PROGRAM(
         [[#include <string.h>
           #include <errno.h>]],
         [[int result = 0;
           char *str;
           errno = 0;
           str = strerror(0);
           if (!*str) result |= 1;
           if (errno) result |= 2;
           if (strstr(str, "nknown") || strstr(str, "ndefined"))
             result |= 4;
           return result;]])],
      [gl_cv_func_strerror_0_works=yes],
      [gl_cv_func_strerror_0_works=no],
      [case "${host_os}" in
                 # Guess yes on glibc systems:
         *-gnu*) gl_cv_func_strerror_0_works="guessing yes" ;;
                 # If we do NOT know, then assume the worst:
         *)      gl_cv_func_strerror_0_works="guessing no" ;;
       esac
      ])dnl
  ])dnl
  ## ...based on that, do this:
  case "${gl_cv_func_strerror_0_works}" in
    *yes) ;;
    *)
      REPLACE_STRERROR_0=1
      AC_DEFINE([REPLACE_STRERROR_0],[1],[Define to 1 if strerror(0)
        does not return a message implying success.])dnl
      ## that should cover it...
      ;;
  esac
])dnl
