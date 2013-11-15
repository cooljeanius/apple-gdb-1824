dnl#                                               -*- Autoconf -*-
dnl# Autoconf configure script for GDB, the GNU debugger.
dnl# Copyright (C) 1995-2013 Free Software Foundation, Inc.
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
dnl# along with this program. If not, see <http://www.gnu.org/licenses/>.

dnl# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.64])dnl
AC_INIT([libgnu],[2013.11.14],[])
AC_CONFIG_SRCDIR([import/memmem.c])
AC_CONFIG_HEADER([config.h])
AM_MAINTAINER_MODE

AC_PROG_CC
AC_USE_SYSTEM_EXTENSIONS
gl_EARLY
AC_PROG_CPP
am_cv_prog_cc_stdc="${ac_cv_prog_cc_stdc}"

AC_CONFIG_AUX_DIR([../..])
AC_CONFIG_MACRO_DIR([m4])
AC_CANONICAL_TARGET

AC_MSG_NOTICE([initializing gnulib])
gl_INIT
AC_MSG_NOTICE([done with gnulib initialization checks])

# We do NOT use automake, but gnulib does. This line lets us generate
# its Makefile.in.
if test -f ${srcdir}/config.status; then
  AC_MSG_NOTICE([config.status is already present, removing it.])
  rm -f ${srcdir}/config.status
fi
AM_INIT_AUTOMAKE([1.5 gnu no-define])

# --------------------- #
# Checks for programs.  #
# --------------------- #

AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_RANLIB

AC_CHECK_TOOL([AR],[ar])

# ---------------------- #
# Checks for libraries.  #
# ---------------------- #
AC_CHECK_LIB([c],[printf])

# Checks for header files.
AC_CHECK_HEADER_STDBOOL
AC_CHECK_HEADERS([OS.h fcntl.h float.h langinfo.h limits.h \
                  locale.h malloc.h malloc/malloc.h netdb.h stddef.h \
                  stdio.h sys/param.h wchar.h wctype.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_BIGENDIAN
AC_C_INLINE
AC_C_PROTOTYPES
AC_C_RESTRICT
AC_TYPE_INT32_T
AC_TYPE_OFF_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UID_T
AC_TYPE_UINT32_T

# Checks for library functions.
AC_FUNC_ALLOCA
AC_FUNC_MALLOC
AC_FUNC_MBRTOWC
AC_FUNC_REALLOC
AC_FUNC_STRNLEN
AC_CHECK_FUNCS([getpagesize memchr mempcpy memset nl_langinfo setlocale \
                stpcpy strchr strcspn strdup strerror strndup strpbrk \
                strrchr strspn strstr wcwidth])

AC_CONFIG_FILES([Makefile Makefile.orig import/Makefile])
AC_CONFIG_COMMANDS([default],[
case x$CONFIG_HEADERS in
xconfig.h:config.in)
echo > stamp-h ;;
esac
],[])
AC_OUTPUT

if test "${srcdir}" != "."; then
  if test -e ${srcdir}/config.status -a ! -e `pwd`/config.status; then
    cp -v ${srcdir}/config.status `pwd`/config.status || echo ""
  elif test -e `pwd`/config.status -a ! -e ${srcdir}/config.status; then
    cp -v `pwd`/config.status ${srcdir}/config.status || echo ""
  fi
fi

exit 0
