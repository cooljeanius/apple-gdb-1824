dnl#                                               -*- Autoconf -*-
dnl# Process this file with autoconf to produce a configure script.

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
AC_CONFIG_AUX_DIR([../..])
AC_CONFIG_HEADERS([config.h])dnl

# Use maintainer mode from automake:
AM_MAINTAINER_MODE

# Checks for programs.
AC_PROG_SED
if test -e ../config.cache; then
  echo "hacking ../config.cache"
  sed -i.bak -e "s|ac_cv_env_CPP_set|bad_CPP_set|g" ../config.cache
  sed -i.bak -e "s|ac_cv_env_CPP_value|bad_CPP_value|g" ../config.cache
  sed -i.bak -e "s|ac_cv_prog_CPP|bad_CPP|g" ../config.cache
elif test -e .././config.cache; then
  echo "hacking .././config.cache"
  sed -i.bak -e "s|ac_cv_env_CPP_set|bad_CPP_set|g" .././config.cache
  sed -i.bak -e "s|ac_cv_env_CPP_value|bad_CPP_value|g" .././config.cache
  sed -i.bak -e "s|ac_cv_prog_CPP|bad_CPP|g" .././config.cache
else
  echo "config.cache is missing, which means we can skip attempts to hack it"
fi
AC_PROG_CC
AC_PROG_CPP
AC_ARG_VAR([CLANG_ANALYZER],[Path to the clang static analyzer])dnl
AC_CACHE_CHECK([for the clang static analyzer],[ac_cv_path_CLANG_ANALYZER],
  [AC_PATH_PROGS_FEATURE_CHECK([CLANG_ANALYZER],
    [clang clang++ clang-mp-3.5 clang-mp-3.4 clang-mp-3.3 clang-mp-3.2],
    [[${ac_path_CLANG_ANALYZER} --analyze /dev/null > /dev/null 2>&1 && \
      ac_cv_path_CLANG_ANALYZER=${ac_path_CLANG_ANALYZER}
      ac_path_CLANG_ANALYZER_found=:]],
    [AC_MSG_WARN([we will not be able to do static analysis with clang])],
    [${PATH}])dnl# end program check
  ])dnl# end cache check
  ## (need this extra line here)
AC_SUBST([CLANG_ANALYZER],[${ac_cv_path_CLANG_ANALYZER}])dnl
AC_PROG_INSTALL
AC_PROG_MKDIR_P

sinclude(../../config/m4.m4)
AC_PROG_GNU_M4

dnl# Checks for header files.
dnl# The program does NOT actually handle failure.
AC_DEFUN([AC_REQUIRE_HEADER_STDC],[
  AC_REQUIRE([AC_HEADER_STDC])dnl
])dnl
AC_REQUIRE_HEADER_STDC
AC_CHECK_HEADERS([ctype.h math.h stdio.h time.h])dnl

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_C_PROTOTYPES
AC_TYPE_SIZE_T
AC_STRUCT_TIMEZONE dnl# also does the "_TM" one

dnl# Checks for library functions.
dnl# The program does NOT actually handle failure.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([abs fprintf memset signbit strcmp strcpy strlen strtol \
                xmalloc])dnl

# Output.
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
