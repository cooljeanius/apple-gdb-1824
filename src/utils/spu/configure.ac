#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

# Copyright 2002 Free Software Foundation, Inc.

# This file is part of SPU (pronounced "spew").

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

AC_PREREQ([2.69])
AC_INIT([spu],[0.5],[/dev/null])
AC_CONFIG_SRCDIR([spu.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL

dnl# Checks for header files.
dnl# The program does NOT actually handle failure.
AC_DEFUN([AC_REQUIRE_HEADER_STDC],[
  AC_REQUIRE([AC_HEADER_STDC])
])
AC_REQUIRE_HEADER_STDC
AC_CHECK_HEADERS([ctype.h stdio.h time.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST

dnl# Checks for library functions.
dnl# The program does NOT actually handle failure.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([fprintf memset strcmp strcpy strlen strtol xmalloc])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
