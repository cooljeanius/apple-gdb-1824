dnl# Process this file with autoconf to create configure. -*- autoconf -*-
#
#    Copyright (C) 2004, 2005, 2007, 2008 Free Software Foundation, Inc.
#    Written by Gary V. Vaughan, 2004
#
#    NOTE: The canonical source of this file is maintained with the
#    GNU Libtool package.  Report bugs to bug-libtool@gnu.org.
#
# GNU Libltdl is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.
#
# As a special exception to the GNU Lesser General Public License,
# if you distribute this file as part of a program or library that
# is built using GNU libtool, you may include this file under the
# same distribution terms that you use for the rest of that program.
#
# GNU Libltdl is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU LesserGeneral Public
# License along with GNU Libltdl; see the file COPYING.LIB.  If not, a
# copy can be downloaded from http://www.gnu.org/licenses/lgpl.html,
# or obtained by writing to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
####

# This configure.ac is not used at all by the libtool bootstrap, but
# is copied to the ltdl subdirectory if you libtoolize --ltdl your own
# project. Adding LT_WITH_LTDL to your project configure.ac will then
# configure this directory if your user does NOT want to use the installed
# libltdl.

AC_PREREQ([2.59])dnl# We use AS_HELP_STRING
m4_include([../../config/warnings.m4])dnl

## ------------------------ ##
## Autoconf initialisation. ##
## ------------------------ ##
AC_INIT([libltdl],[2.4.2],[bug-libtool@gnu.org])
AC_CONFIG_HEADERS([config.h:config-h.in])
AC_CONFIG_SRCDIR([ltdl.c])
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_MACRO_DIR([m4])
LT_CONFIG_LTDL_DIR([.])dnl# I am me!

## -------------------- ##
## architecture checks. ##
## -------------------- ##

AC_CANONICAL_TARGET
if test -z "${BUILD_ARCHITECTURE}" -a -n "${build_cpu}"; then
  export BUILD_ARCHITECTURE="${build_cpu}"
  AC_SUBST([BUILD_ARCHITECTURE])
fi
if test -z "${HOST_ARCHITECTURE}" -a -n "${host_cpu}"; then
  export HOST_ARCHITECTURE="${host_cpu}"
  AC_SUBST([HOST_ARCHITECTURE])
fi
if test -z "${TARGET_ARCHITECTURE}" -a -n "${target_cpu}"; then
  export TARGET_ARCHITECTURE="${target_cpu}"
  AC_SUBST([TARGET_ARCHITECTURE])
fi

AC_USE_SYSTEM_EXTENSIONS

if test -n "${HOST_ARCHITECTURE}"; then
  ACX_PROG_CC_WARNING_OPTS(["-arch ${HOST_ARCHITECTURE}"],[C_ARCHFLAGS])
  test -n "${C_ARCHFLAGS}" && export C_ARCHFLAGS
  AC_SUBST([C_ARCHFLAGS])
fi
if test "x${CFLAGS}" = "x"; then
  test -z "${CFLAGS}" && export CFLAGS="${C_ARCHFLAGS}"
else
  test -n "${CFLAGS}" && export CFLAGS="${CFLAGS} ${C_ARCHFLAGS}"
fi

## ------------------------ ##
## Automake Initialization. ##
## ------------------------ ##

AM_INIT_AUTOMAKE([1.11.6 foreign subdir-objects])
AM_MAINTAINER_MODE

# Checks for programs.
AM_PROG_AS
AC_LANG([C])dnl
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_GNU_M4

## ------------------------------- ##
## Libtool specific configuration. ##
## ------------------------------- ##
pkgdatadir='${datadir}'"/${PACKAGE}"


## ----------------------- ##
## Libtool initialization. ##
## ----------------------- ##
LT_INIT([dlopen win32-dll])
_LTDL_SETUP
LT_LANG([C])dnl

# Checks for libraries.
AC_SEARCH_LIBS([dlsym],[dl dlcompat dyld dld svld])dnl
AC_CHECK_LIB([dld],[dld_unlink_by_file],[],[
  AC_CHECK_LIB([dld],[dld_get_func])dnl
])dnl

# Checks for header files.
AC_DEFUN([AC_REQUIRE_HEADER_STDC],[
  AC_REQUIRE([AC_HEADER_STDC])dnl
])dnl
AC_REQUIRE_HEADER_STDC
AC_HEADER_DIRENT
AC_CHECK_HEADERS([stddef.h stdio.h])dnl

# Checks for typedefs, structures, and compiler characteristics.
AC_C_BIGENDIAN
AC_C_CONST
AC_C_PROTOTYPES
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_CLOSEDIR_VOID
AC_CHECK_FUNCS([memmove memset strchr strrchr])dnl

# Override function checks that would require libobjs:
AC_DEFUN([AC_FUNC_MALLOC],[AC_CHECK_FUNCS([malloc])])dnl
AC_FUNC_MALLOC
AC_DEFUN([AC_FUNC_REALLOC],[AC_CHECK_FUNCS([realloc])])dnl
AC_FUNC_REALLOC

## -------- ##
## Outputs. ##
## -------- ##
AC_CONFIG_FILES([Makefile])dnl
AC_OUTPUT
