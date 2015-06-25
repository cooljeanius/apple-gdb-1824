# extensions.m4 serial 14  -*- Autoconf -*-
# Enable extensions on systems that normally disable them.

# Copyright (C) 2003, 2006-2013 Free Software Foundation, Inc.
# This file is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

dnl# This definition of AC_USE_SYSTEM_EXTENSIONS is stolen from git
dnl# Autoconf.  Perhaps we can remove this once we can assume Autoconf
dnl# 2.70 or later everywhere, but since Autoconf mutates rapidly
dnl# enough in this area it is likely we will need to redefine
dnl# AC_USE_SYSTEM_EXTENSIONS for quite some time.

dnl# If autoconf reports a warning such as:
dnl#  warning: AC_COMPILE_IFELSE was called before AC_USE_SYSTEM_EXTENSIONS
dnl# or  warning: AC_RUN_IFELSE was called before AC_USE_SYSTEM_EXTENSIONS
dnl# the fix is:
dnl#   1) to ensure that AC_USE_SYSTEM_EXTENSIONS is never directly invoked
dnl#      but always AC_REQUIREd,
dnl#   2) to ensure that for each occurrence of
dnl#        AC_REQUIRE([AC_USE_SYSTEM_EXTENSIONS])
dnl#      or
dnl#        AC_REQUIRE([gl_USE_SYSTEM_EXTENSIONS])
dnl#      the corresponding gnulib module description has 'extensions' in
dnl#      its dependencies. This ensures that the gl_USE_SYSTEM_EXTENSIONS
dnl#      invocation occurs in gl_EARLY, not in gl_INIT.

dnl# AC_USE_SYSTEM_EXTENSIONS
dnl# ------------------------
dnl# Enable extensions on systems that normally disable them,
dnl# typically due to standards-conformance issues.
dnl#
dnl# Remember that #undef in AH_VERBATIM gets replaced with #define by
dnl# AC_DEFINE.  The goal here is to define all known feature-enabling
dnl# macros, then, if reports of conflicts are made, disable macros that
dnl# cause problems on some platforms (such as __EXTENSIONS__).
AC_DEFUN_ONCE([AC_USE_SYSTEM_EXTENSIONS],
[AC_BEFORE([$0],[AC_COMPILE_IFELSE])dnl
AC_BEFORE([$0],[AC_RUN_IFELSE])dnl

  unset ac_cv_header_minix_config_h
  AC_CHECK_HEADERS([minix/config.h],[MINIX=yes],[MINIX=])
  if test "x${MINIX}" = "xyes"; then
    AC_DEFINE([_POSIX_SOURCE],[1],
      [Define to 1 if you need to in order for 'stat' and other
       things to work.])
    AC_DEFINE([_POSIX_1_SOURCE],[2],
      [Define to 2 if the system does not provide POSIX.1 features
       except with this defined.])
    AC_DEFINE([_MINIX],[1],
      [Define to 1 if on MINIX.])
    AC_DEFINE([_NETBSD_SOURCE],[1],
      [Define to 1 to make NetBSD features available.  MINIX 3 needs this])
  fi

dnl# Use a different key than __EXTENSIONS__, as that name broke existing
dnl# configure.ac when using autoheader 2.62.
  AH_VERBATIM([USE_SYSTEM_EXTENSIONS],
[/* Enable extensions on AIX 3, Interix.  */
#ifndef _ALL_SOURCE
# undef _ALL_SOURCE
#endif /* !_ALL_SOURCE */
/* Enable general extensions on OS X.  */
#ifndef _DARWIN_C_SOURCE
# undef _DARWIN_C_SOURCE
#endif /* !_DARWIN_C_SOURCE */
/* Enable GNU extensions on systems that have them.  */
#ifndef _GNU_SOURCE
# undef _GNU_SOURCE
#endif /* !_GNU_SOURCE */
/* Use GNU style printf and scanf.  */
#ifndef __USE_MINGW_ANSI_STDIO
# undef __USE_MINGW_ANSI_STDIO
#endif /* !__USE_MINGW_ANSI_STDIO */
/* Enable threading extensions on Solaris.  */
#ifndef _POSIX_PTHREAD_SEMANTICS
# undef _POSIX_PTHREAD_SEMANTICS
#endif /* !_POSIX_PTHREAD_SEMANTICS */
/* Enable extensions on HP NonStop.  */
#ifndef _TANDEM_SOURCE
# undef _TANDEM_SOURCE
#endif /* !_TANDEM_SOURCE */
/* Enable X/Open extensions if necessary.  HP-UX 11.11 defines
 * mbstate_t only if _XOPEN_SOURCE is defined to 500, regardless of
 * whether compiling with -Ae or -D_HPUX_SOURCE=1.  */
#ifndef _XOPEN_SOURCE
# undef _XOPEN_SOURCE
#endif /* !_XOPEN_SOURCE */
/* Enable general extensions on Solaris.  */
#ifndef __EXTENSIONS__
# undef __EXTENSIONS__
#endif /* !__EXTENSIONS__ */
])
  AC_CACHE_CHECK([whether it is safe to define __EXTENSIONS__],
    [ac_cv_safe_to_define___extensions__],
    [AC_COMPILE_IFELSE(
       [AC_LANG_PROGRAM([[
#         define __EXTENSIONS__ 1
          ]AC_INCLUDES_DEFAULT])],
       [ac_cv_safe_to_define___extensions__=yes],
       [ac_cv_safe_to_define___extensions__=no]) dnl# end compile test
  ]) dnl# end cache check

  test "x${ac_cv_safe_to_define___extensions__}" = "xyes" &&
    AC_DEFINE([__EXTENSIONS__])
  AC_DEFINE([_ALL_SOURCE])
  AC_DEFINE([_DARWIN_C_SOURCE])
  AC_DEFINE([_GNU_SOURCE])
  AC_DEFINE([__USE_MINGW_ANSI_STDIO])
  AC_DEFINE([_POSIX_PTHREAD_SEMANTICS])
  AC_DEFINE([_TANDEM_SOURCE])
  AC_CHECK_HEADERS_ONCE([wchar.h])
  test -z "${ac_cv_type_mbstate_t}"
  AC_CHECK_TYPES([mbstate_t],[],[],[
#ifdef HAVE_WCHAR_H
# include <wchar.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "this conftest for mbstate_t expects <wchar.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_WCHAR_H */
  ])
  AC_CACHE_CHECK([whether _XOPEN_SOURCE should be defined],
    [ac_cv_should_define__xopen_source],
    [ac_cv_should_define__xopen_source=no
     AC_COMPILE_IFELSE(
       [AC_LANG_PROGRAM([[
          #include <wchar.h>
          mbstate_t x;]])],
       [],
       [AC_COMPILE_IFELSE(
          [AC_LANG_PROGRAM([[
             #define _XOPEN_SOURCE 500
             #include <wchar.h>
             mbstate_t x;]])],
          [ac_cv_should_define__xopen_source=yes]) dnl# end of 2nd comptest
       ]) dnl# end of 1st comptest
  ])dnl# end of cache check

  test "x$ac_cv_should_define__xopen_source" = "xyes" &&
    AC_DEFINE([_XOPEN_SOURCE],[500])
])dnl# AC_USE_SYSTEM_EXTENSIONS

dnl# AC_GNU_SOURCE
dnl# --------------
AC_DEFUN([AC_GNU_SOURCE],
[AH_VERBATIM([_GNU_SOURCE],
[/* Enable GNU extensions on systems that have them.  */
#ifndef _GNU_SOURCE
# undef _GNU_SOURCE
#endif /* !_GNU_SOURCE */])dnl
AC_BEFORE([$0],[AC_COMPILE_IFELSE])dnl
AC_BEFORE([$0],[AC_RUN_IFELSE])dnl
AC_DEFINE([_GNU_SOURCE])
])dnl

dnl# gl_USE_SYSTEM_EXTENSIONS
dnl# ------------------------
dnl# Enable extensions on systems that normally disable them,
dnl# typically due to standards-conformance issues.
AC_DEFUN_ONCE([gl_USE_SYSTEM_EXTENSIONS],[
  dnl# Require this macro before AC_USE_SYSTEM_EXTENSIONS.
  dnl# gnulib does not need it. But if it gets required by third-party
  dnl# macros after AC_USE_SYSTEM_EXTENSIONS is required,
  dnl# autoconf 2.62..2.63 emit a warning:
  dnl# "AC_COMPILE_IFELSE was called before AC_USE_SYSTEM_EXTENSIONS".
  dnl# Note: We can do this only for one of the macros out of AC_AIX,
  dnl# AC_GNU_SOURCE, and AC_MINIX. If people still use AC_AIX or AC_MINIX,
  dnl# then they are out of luck.
  AC_REQUIRE([AC_GNU_SOURCE])dnl

  AC_REQUIRE([AC_USE_SYSTEM_EXTENSIONS])dnl

  AC_REQUIRE([AC_EXEEXT])dnl
  AC_REQUIRE([AC_OBJEXT])dnl
])dnl
