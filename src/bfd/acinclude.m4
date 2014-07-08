dnl# bfd/acinclude.m4
dnl#
dnl#   Copyright 2012 Free Software Foundation
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

sinclude(../config/acx.m4)
sinclude(../bfd/bfd.m4)
sinclude(../bfd/warning.m4)
sinclude([../config/zlib.m4])

dnl# See whether we need to use fopen-bin.h rather than fopen-same.h.
AC_DEFUN([BFD_BINARY_FOPEN],
[AC_REQUIRE([AC_CANONICAL_TARGET])
case "${host}" in
changequote(,)dnl
*-*-msdos* | *-*-go32* | *-*-mingw32* | *-*-cygwin* | *-*-windows*)
changequote([,])dnl
  AC_DEFINE([USE_BINARY_FOPEN],[1],
            [Use b modifier when opening binary files?]) ;;
esac])dnl

dnl# Get a default for CC_FOR_BUILD to put into Makefile.
AC_DEFUN([BFD_CC_FOR_BUILD],
[# Put a plausible default for CC_FOR_BUILD in Makefile.
AC_REQUIRE([AC_PROG_CC])
if test -z "${CC_FOR_BUILD}"; then
  if test "x${cross_compiling}" = "xno"; then
    CC_FOR_BUILD='$(CC)'
  else
    CC_FOR_BUILD=gcc
  fi
fi
AC_SUBST([CC_FOR_BUILD])
# Also set EXEEXT_FOR_BUILD.
AC_REQUIRE([AC_EXEEXT])
if test "x${cross_compiling}" = "xno"; then
  EXEEXT_FOR_BUILD='$(EXEEXT)'
else
  AC_CACHE_CHECK([for build system executable suffix],
                 [bfd_cv_build_exeext],
    [rm -f conftest* || rm -rfv conftest* || rmdir conftest*
     echo 'int main () { return 0; }' > conftest.c
     bfd_cv_build_exeext=""
     ${CC_FOR_BUILD} -o conftest conftest.c 1>&5 2>&5
     for file in conftest.*; do
       case ${file} in
       *.c | *.o | *.obj | *.ilk | *.pdb) ;;
       *) bfd_cv_build_exeext=`echo ${file} | sed -e s/conftest//` ;;
       esac
     done
     rm -f conftest* || rm -rfv conftest* || rmdir conftest*
     test x"${bfd_cv_build_exeext}" = x"" && bfd_cv_build_exeext=no])
  EXEEXT_FOR_BUILD=""
  test x"${bfd_cv_build_exeext}" != xno && EXEEXT_FOR_BUILD=${bfd_cv_build_exeext}
fi
AC_SUBST([EXEEXT_FOR_BUILD])])dnl

sinclude(../libtool-old.m4)
dnl# The lines below arrange for aclocal not to bring libtool.m4
dnl# AM_PROG_LIBTOOL into aclocal.m4, while still arranging for automake
dnl# to add a definition of LIBTOOL to Makefile.in.
dnl# (is this really a good idea?)
ifelse([yes],[no],[
AC_DEFUN([AM_PROG_LIBTOOL],[])
AC_DEFUN([AM_DISABLE_SHARED],[])
AC_SUBST([LIBTOOL])
])

sinclude(../gettext.m4)
ifelse([yes],[no],[
AC_DEFUN([CY_WITH_NLS],[])
AC_SUBST([INTLLIBS])
])

AC_DEFUN([AM_INSTALL_LIBBFD],
[AC_MSG_CHECKING([whether to install libbfd])
  AC_ARG_ENABLE([install-libbfd],
[AS_HELP_STRING([--enable-install-libbfd],
                [controls installation of libbfd and related headers])],
      [install_libbfd_p=$enableval],
      [if test "x${host}" = "x${target}" || test "x${enable_shared}" = "xyes"; then
        install_libbfd_p=yes
      else
        install_libbfd_p=no
      fi])
  AC_MSG_RESULT([${install_libbfd_p}])
  AM_CONDITIONAL([INSTALL_LIBBFD],[test "x${install_libbfd_p}" = "xyes"])
  # Need _noncanonical variables for this.
  AC_REQUIRE([ACX_NONCANONICAL_HOST])
  AC_REQUIRE([ACX_NONCANONICAL_TARGET])
  # libbfd.a is a host library containing target dependent code
  bfdlibdir='$(libdir)'
  bfdincludedir='$(includedir)'
  if test "x${host}" != "x${target}"; then
    bfdlibdir='$(exec_prefix)/$(host_noncanonical)/$(target_noncanonical)/lib'
    bfdincludedir='$(exec_prefix)/$(host_noncanonical)/$(target_noncanonical)/include'
  else
    bfdlibdir='$(libdir)'
    bfdincludedir='$(includedir)'
  fi
  AC_SUBST([bfdlibdir])
  AM_SUBST_NOTMAKE([bfdlibdir])
  AC_SUBST([bfdincludedir])
  AM_SUBST_NOTMAKE([bfdincludedir])
])
