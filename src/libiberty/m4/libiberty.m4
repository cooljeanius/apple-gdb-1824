# libiberty.m4 serial 2                                    -*- Autoconf -*-
dnl# Bits libiberty clients must do on their autoconf step.
dnl#
dnl#   Copyright (C) 2012-2015 Free Software Foundation, Inc.
dnl#
dnl# This file is free software; you can redistribute it and/or modify
dnl# it under the terms of the GNU General Public License as published by
dnl# the Free Software Foundation; either version 3 of the License, or
dnl# (at your option) any later version.
dnl#
dnl# This program is distributed in the hope that it will be useful,
dnl# but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
dnl# GNU General Public License for more details.
dnl#
dnl# You should have received a copy of the GNU General Public License
dnl# along with this program; see the file COPYING3.  If not see
dnl# <http://www.gnu.org/licenses/>.
dnl#

dnl# Checks for declarations ansidecl.h and libiberty.h themselves
dnl# check with HAVE_DECL_XXX, etc.

AC_DEFUN([libiberty_INIT],[
  # Check for presence of long long:
  AC_CHECK_TYPE([long long],
    [AC_DEFINE([HAVE_LONG_LONG],[1],
               [Define if you have the `long long' type.])dnl
     AC_CHECK_SIZEOF([long long])dnl
     AC_CHECK_ALIGNOF([long long])],
    [:])dnl

  AC_CHECK_DECLS_ONCE([alloca])dnl
  ## basename is weird:
  if test "x${ac_cv_have_decl_basename}" = "x"; then
    test -z "${ac_cv_have_decl_basename}"
    if test "x${enable_build_with_cxx}" = "xyes"; then
      AC_LANG_PUSH([C++])dnl
      AC_CHECK_DECLS([basename(char *)])dnl
      AC_LANG_POP([C++])
    elif test "x${enable_build_with_cxx}" = "xno"; then
      test -n "${enable_build_with_cxx}"
      AC_CHECK_DECLS([basename],[],[],[
#ifdef HAVE_LIBGEN_H
# include <libgen.h>
#endif /* HAVE_LIBGEN_H */
      ])dnl
      ## end C++ conditional
    fi
  fi
  AC_CHECK_DECLS_ONCE([ffs, asprintf, vasprintf, snprintf, vsnprintf])dnl
  AC_CHECK_DECLS_ONCE([strtol, strtoul, strtoll, strtoull])dnl
  AC_CHECK_DECLS_ONCE([strverscmp])dnl
])dnl
