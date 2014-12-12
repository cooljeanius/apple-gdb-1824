dnl# lcmessage.m4 (formerly part of gettext.m4)            -*- Autoconf -*-

dnl# Check whether LC_MESSAGES is available in <locale.h>.
dnl# Ulrich Drepper <drepper@cygnus.com>, 1995.
dnl#
dnl# This file file be copied and used freely without restrictions.  It can
dnl# be used in projects which are not available under the GPL
dnl# but which still want to provide support for the GNU gettext
dnl# functionality.
dnl# Please note that the actual code is *not* freely available.

#serial 2

AC_DEFUN([AM_LC_MESSAGES],
  [if test "x${ac_cv_header_locale_h}" = "xyes"; then
    AC_CACHE_CHECK([for LC_MESSAGES],[am_cv_val_LC_MESSAGES],
      [AC_LINK_IFELSE([AC_LANG_SOURCE([[#include <locale.h>]],[[return LC_MESSAGES]])],
       [am_cv_val_LC_MESSAGES=yes],[am_cv_val_LC_MESSAGES=no])])
    if test "x${am_cv_val_LC_MESSAGES}" = "xyes"; then
      AC_DEFINE([HAVE_LC_MESSAGES],[1],
		[Define if your locale.h file contains LC_MESSAGES.])
    fi
  fi
])dnl

dnl# EOF
