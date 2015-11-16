# build-with-cxx.m4 serial 1                               -*- Autoconf -*-
dnl# Copyright (C) 2014-2015 Free Software Foundation, Inc.
dnl#
dnl# This file is part of GDB.
dnl#
dnl# This program is free software; you can redistribute it and/or modify
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
dnl# along with this program.  If not, see <http://www.gnu.org/licenses/>.

dnl# GDB_AC_BUILD_WITH_CXX([])
dnl# Provide an --enable-build-with-cxx/--disable-build-with-cxx set of
dnl# options allowing a user to build with a C++ compiler.

AC_DEFUN([GDB_AC_BUILD_WITH_CXX],[
  AC_ARG_ENABLE([build-with-cxx],
     [AS_HELP_STRING([--enable-build-with-cxx],
                     [build with C++ compiler instead of C compiler])],
    [case ${enableval} in
      yes)
        AC_MSG_WARN([building gdb with a C++ compiler is experimental])
        ;;
      no)
	;;
      *)
	AC_MSG_ERROR([bad value ${enableval} for --enable-build-with-cxx])
	;;
    esac],
    [enable_build_with_cxx=no])dnl

  dnl# Yes, these are actually variable names, and not macro names:
  m4_pattern_allow([AM_V_])
  if test "x${enable_build_with_cxx}" = "xyes"; then
    COMPILER='$(CXX)'
    LINKER='$(CXXLD)'
    AM_V_COMPILER='$(AM_V_CXX)'
    AM_V_LINKER='$(AM_V_CXXLD)'
    TAG_FOR_LIBTOOL='--tag=CXX'
   else
    COMPILER='$(CC)'
    LINKER='$(CCLD)'
    AM_V_COMPILER='$(AM_V_CC)'
    AM_V_LINKER='$(AM_V_CCLD)'
    TAG_FOR_LIBTOOL='--tag=CC'
  fi
  AC_SUBST([COMPILER])dnl
  AC_SUBST([LINKER])dnl
  AC_SUBST([AM_V_COMPILER])dnl
  AC_SUBST([AM_V_LINKER])dnl
  AC_SUBST([TAG_FOR_LIBTOOL])dnl
])dnl
