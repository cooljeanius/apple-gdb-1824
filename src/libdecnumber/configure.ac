dnl# configure.ac for libdecnumber			-*- Autoconf -*-
dnl# Process this file with autoconf264 to generate a configuration script.

# Copyright 2005, 2006, 2009 Free Software Foundation, Inc.

# This file is part of GCC.

# GCC is free software; you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free
# Software Foundation; either version 3, or (at your option) any
# later #version.

# GCC is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
# or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
# License #for more details.

# You should have received a copy of the GNU General Public License
# along with GCC; see the file COPYING3.  If not see
# <http://www.gnu.org/licenses/>.

AC_PREREQ([2.64])
dnl# override.m4 is annoying
AC_INIT([libdecnumber],[0.0.1],[gcc-bugs@gcc.gnu.org],[libdecnumber])
AC_CONFIG_SRCDIR([decNumber.h])dnl
AC_CONFIG_MACRO_DIR([../config])dnl

# Checks for programs.
AC_PROG_MAKE_SET
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_RANLIB

MISSING=`cd ${ac_aux_dir} && ${PWDCMD-pwd}`/missing
AC_CHECK_PROGS([ACLOCAL],[aclocal],[${MISSING} aclocal])
AC_CHECK_PROGS([AUTOCONF],[autoconf],[${MISSING} autoconf])
AC_CHECK_PROGS([AUTOHEADER],[autoheader],[${MISSING} autoheader])dnl

# Figure out what compiler warnings we can enable.
# See ../config/warnings.m4 for details.

dnl# '-W' is redundant with '-Wextra'
ACX_PROG_CC_WARNING_OPTS([-Wall -Wextra -Wno-unused-value -Wwrite-strings \
-Wstrict-prototypes -Wmissing-prototypes -Wmissing-parameter-type \
-Wmissing-declarations -Wnested-externs -Wundef -Wshadow -Wformat=2 \
-Wold-style-definition -Wold-style-declaration -Wc++-compat -Wodr -Wabi \
-Wmissing-format-attribute -Wcast-qual -Wout-of-line-declaration \
-Wmisleading-indentation -Wnull-dereference])
ACX_PROG_CC_WARNING_ALMOST_PEDANTIC([-Wno-long-long])dnl

# Only enable with --enable-werror-always until existing warnings are
# corrected.
ACX_PROG_CC_WARNINGS_ARE_ERRORS([manual])dnl

# Checks for header files.
AC_DEFUN([AC_REQUIRE_HEADER_STDC],[
  AC_REQUIRE([AC_PROG_CPP])dnl
  AC_REQUIRE([AC_HEADER_STDC])dnl
])dnl
AC_REQUIRE_HEADER_STDC dnl# just in case
AC_CHECK_HEADERS([ctype.h fenv.h stddef.h string/string.h stdio.h])dnl
GCC_HEADER_STDINT([gstdint.h])dnl

# Checks for typedefs, structures, and compiler characteristics.
dnl# (endianness check is done near the bottom)
AC_C_CONST
AC_TYPE_INT32_T
AC_TYPE_OFF_T
AC_TYPE_SIZE_T
AC_TYPE_UINT8_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_CHECK_SIZEOF([int])dnl
AC_CHECK_ALIGNOF([int])dnl
AC_CHECK_SIZEOF([long])dnl
AC_CHECK_ALIGNOF([long])dnl

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_MEMCMP
AC_CHECK_FUNCS([memset])dnl

AC_ARG_ENABLE([maintainer-mode],
[AS_HELP_STRING([--enable-maintainer-mode],
                [enable rules only needed by maintainers])],[],
[enable_maintainer_mode=no])dnl

if test "x${enable_maintainer_mode}" = "xno"; then
  MAINT='#'
else
  MAINT=""
fi
AC_SUBST([MAINT])dnl

AC_CANONICAL_TARGET

# Default decimal format
# If you change the defaults here, be sure to change them in the GCC
# directory (if it exists) also
AC_MSG_CHECKING([for decimal floating point])
AC_ARG_ENABLE([decimal-float],
[AS_HELP_STRING([--enable-decimal-float={no,yes,bid,dpd}],[
			enable decimal float extension to C. Select 'bid'
			or 'dpd' to choose which decimal floating point
			format to use])],
[
  case ${enable_decimal_float} in
    yes | no | bid | dpd) ;;
    *) AC_MSG_ERROR(['${enable_decimal_float}' is an invalid value for --enable-decimal-float.
Valid choices are 'yes', 'bid', 'dpd', and 'no'.]) ;;
  esac
],
[
  case ${target} in
    powerpc*-*-linux* | i?86*-*-linux* | x86_64*-*-linux*)
      enable_decimal_float=yes
      ;;
    *)
      enable_decimal_float=no
      ;;
  esac
])dnl

# x86's use BID format instead of DPD
# In theory --enable-decimal-float=no should not compile anything
# For the sake of simplicity, just use the default format in this directory
if test "x${enable_decimal_float}" = "xyes" || test "x${enable_decimal_float}" = "xno"; then
  case ${target} in
    i?86*-*-linux* | x86_64*-*-linux*)
      enable_decimal_float=bid
      ;;
    *)
      enable_decimal_float=dpd
      ;;
  esac
fi

# If BID is being used, then additional objects should be linked in.
if test "x${enable_decimal_float}" = "xbid"; then
  ADDITIONAL_OBJS="${ADDITIONAL_OBJS} \$(bid_OBJS)"
else
  ADDITIONAL_OBJS=""
fi

AC_MSG_RESULT([${enable_decimal_float}])
AC_SUBST([enable_decimal_float])dnl
AC_SUBST([ADDITIONAL_OBJS])dnl

AC_C_BIGENDIAN

# Output.
AC_CONFIG_HEADERS([config.h])dnl
AC_CONFIG_FILES([Makefile])dnl
AC_OUTPUT
