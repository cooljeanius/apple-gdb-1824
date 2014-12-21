# memmem.m4 serial 10 -*- Autoconf -*-
dnl# Copyright (C) 2002-2004, 2007-2009
dnl# Free Software Foundation, Inc.
dnl# This file is free software; the Free Software Foundation
dnl# gives unlimited permission to copy and/or distribute it,
dnl# with or without modifications, as long as this notice is preserved.

dnl# Check that memmem is present:
AC_DEFUN([gl_FUNC_MEMMEM_SIMPLE],[
  dnl# Persuade glibc <string.h> to declare memmem() for us:
  AC_REQUIRE([AC_USE_SYSTEM_EXTENSIONS])dnl

  AC_REQUIRE([gl_HEADER_STRING_H_DEFAULTS])dnl
  AC_REPLACE_FUNCS([memmem])dnl
  AC_CHECK_DECLS_ONCE([memmem])dnl
  ## for the replacement:
  if test "x${ac_cv_have_decl_memmem}" = "xno"; then
    HAVE_DECL_MEMMEM=0
  fi
  gl_PREREQ_MEMMEM
]) dnl# gl_FUNC_MEMMEM_SIMPLE

dnl# Additionally check that memmem is efficient and handles empty needles:
AC_DEFUN([gl_FUNC_MEMMEM],[
  AC_REQUIRE([gl_FUNC_MEMMEM_SIMPLE])dnl
  ## only makes sense to check how it works if we have it:
  if test "x${ac_cv_have_decl_memmem}" = "xye"s; then
    AC_CACHE_CHECK([whether memmem works in linear time],
      [gl_cv_func_memmem_works],
      [AC_RUN_IFELSE([AC_LANG_PROGRAM([[
#include <string.h> /* for memmem() */
#include <stdlib.h> /* for malloc() */
#include <unistd.h> /* for alarm() */
]],[[size_t m = 1000000;
    char *haystack = (char *)malloc(2 * m + 1);
    char *needle = (char *)malloc(m + 1);
    void *result = 0;
    /* Failure to compile this test due to missing alarm is okay,
     * since all such platforms (mingw) also lack memmem.  */
    alarm(5);
    /* Check for quadratic performance: */
    if (haystack && needle)
      {
	memset(haystack, 'A', 2 * m);
	haystack[2 * m] = 'B';
	memset(needle, 'A', m);
	needle[m] = 'B';
	result = memmem(haystack, 2 * m + 1, needle, m + 1);
      }
    /* Check for empty needle behavior: */
    return !result || !memmem("a", 1, 0, 0);]])],
	[gl_cv_func_memmem_works=yes],[gl_cv_func_memmem_works=no],
	[dnl# pessimistically assume the worst, since even glibc 2.6.1
	 dnl# has quadratic complexity in its memmem:
	 gl_cv_func_memmem_works="guessing no"])])dnl
    ## ...based on the result, do this:
    if test "x${gl_cv_func_memmem_works}" != "xyes"; then
      REPLACE_MEMMEM=1
      AC_LIBOBJ([memmem])dnl
      ## (libobj available from gnulib)
    fi
  fi
]) dnl# end gl_FUNC_MEMMEM

dnl# Prerequisites of lib/memmem.c (none so far):
AC_DEFUN([gl_PREREQ_MEMMEM],[echo "memmem has no prerequisites"])dnl
