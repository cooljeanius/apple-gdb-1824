dnl#                                               -*- Autoconf -*-
dnl# Autoconf configure script for MMALLOC, the GNU mmemory allocator.
dnl# Copyright 2000 Free Software Foundation, Inc.
dnl#
dnl# This file is part of GDB.
dnl# 
dnl# This program is free software; you can redistribute it and/or modify
dnl# it under the terms of the GNU General Public License as published by
dnl# the Free Software Foundation; either version 2 of the License, or
dnl# (at your option) any later version.
dnl#
dnl# This program is distributed in the hope that it will be useful,
dnl# but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
dnl# GNU General Public License for more details.
dnl# 
dnl# You should have received a copy of the GNU General Public License
dnl# along with this program; if not, write to:
dnl#  The Free Software Foundation, Inc.
dnl#  59 Temple Place - Suite 330
dnl#  Boston, MA 02111-1307, USA

dnl# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.12.1])dnl
AC_INIT([mmalloc],[2005-03-24],[jmolenda@apple.com])
dnl# bug report address is email address of last person to touch this
dnl# directory, according to the Changelog-Apple in this directory
AC_CONFIG_SRCDIR([mmalloc.c])
AC_CONFIG_AUX_DIR([..])
AC_CONFIG_MACRO_DIR([m4])dnl

# Architecture stuff:
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

# Pieces of automake that make sense here:
AM_MAINTAINER_MODE
AM_SANITY_CHECK

dnl# Guard our config.h against inclusions from other subdirs:
AH_TOP([
#ifndef HAVE_CONFIG_H
# define HAVE_CONFIG_H 1
#endif /* !HAVE_CONFIG_H */
])dnl
AH_VERBATIM([00_CONFIG_H_CHECK],[
/* Check that config.h is #included before system headers
 * (this works only for glibc, but that should be enough). */
#if defined(__GLIBC__) && !defined(__FreeBSD_kernel__) && !defined(__CONFIG_H__)
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
 #  warning "config.h should be #included before system headers"
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* __GLIBC__ && !__FreeBSD_kernel__ && !__CONFIG_H__ */
#ifndef __CONFIG_H__
# define __CONFIG_H__ 1
# ifndef __MMALLOC_CONFIG_H__
#  define __MMALLOC_CONFIG_H__ 1
# endif /* __MMALLOC_CONFIG_H__ */])dnl
AH_BOTTOM([
#endif /* __CONFIG_H__ */
])dnl

# Checks for programs.
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CPP
AC_PROG_CXX
AC_PROG_CXXCPP
gl_PROG_ANSI_CXX([CXX],[ANSICXX])dnl
gt_NO_CXX
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
AC_PROG_FGREP
AC_PROG_INSTALL
AC_PROG_LN_S
AM_MISSING_PROG([MAKEINFO],[makeinfo])dnl
AC_PATH_PROG([CONFIGURED_TEX],[tex])dnl
AC_PROG_MKDIR_P
m4_ifdef([AM_PROG_AR],[
  AM_PROG_AR
  ## ...
  if test "x${AR}" = "x" && test "x${ac_cv_prog_AR}" = "x"; then
    test -z "${AR}" || unset AR
    test -z "${ac_cv_prog_AR}" || unset ac_cv_prog_AR
    AC_PROG_AR
  fi
],[
  AC_CHECK_TOOL([AR],[ar])dnl
])dnl
AC_CHECK_TOOL([RANLIB],[ranlib],[:])dnl
  ## keep autoscan happy:
if test "x${RANLIB}" = "x"; then
  test -z "${RANLIB}"
  AC_PROG_RANLIB
else
  test -n "${RANLIB}" && export RANLIB
  AC_SUBST([RANLIB])dnl
  ## (keep from being empty)
fi

# Checks for libraries.
AC_CHECK_LIB([c],[printf])dnl
AC_SEARCH_LIBS([vm_region],
               [System c kern mach gnumach arclite arclite_macosx clang xnu])dnl

# Checks for header files.
AC_DEFUN([AC_REQUIRE_HEADER_STDC],[
  m4_ifdef([AC_HEADER_STDC],[
    AC_REQUIRE([AC_HEADER_STDC])dnl
  ],[
    AC_CHECK_HEADERS_ONCE([inttypes.h stdint.h strings.h unistd.h])dnl
  ])dnl
])dnl
AC_REQUIRE_HEADER_STDC
AC_CHECK_HEADERS([AvailabilityMacros.h db.h errno.h fcntl.h \
                  ipc/ipc_types.h limits.h mach/error.h mach/mach.h \
                  mach/mach_error.h mach/mach_port.h mach/mach_vm.h \
                  mach/port.h mach/vm_region.h stddef.h stdio.h \
                  sys/mman.h])dnl

# Checks for typedefs, structures, and compiler characteristics.
AC_C_BIGENDIAN
AC_C_CONST
AC_C_PROTOTYPES
AC_TYPE_OFF_T
AC_TYPE_SIZE_T
AC_CHECK_TYPES([caddr_t])dnl
AC_CHECK_TYPES([port_t],[],[
  # failed to find port_t, try a replacement type:
  AC_CHECK_TYPES([mach_port_t],[
    # mach_port_t IS found, check some associated types:
    AC_CHECK_TYPES([mach_port_array_t],[],[],[
#ifdef HAVE_MACH_PORT_H
# include <mach/port.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "This conftest for mach_port_array_t wants <mach/port.h>"
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_MACH_PORT_H */
    ])dnl
    AC_CHECK_TYPES([mach_port_type_t],[],[],[
#ifdef HAVE_MACH_PORT_H
# include <mach/port.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "This conftest for mach_port_type_t wants <mach/port.h>"
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_MACH_PORT_H */
    ])dnl
  ],[
    # failed to find mach_port_t, try a replacement type:
    AC_CHECK_TYPES([ipc_port_t],[],[],[
#ifdef HAVE_IPC_IPC_TYPES_H
# include <ipc/ipc_types.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "This conftest for ipc_port_t wants <ipc/ipc_types.h>"
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_IPC_IPC_TYPES_H */
    ])dnl
  ],[
#ifdef HAVE_MACH_PORT_H
# include <mach/port.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "This conftest for mach_port_t wants to include <mach/port.h>"
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_MACH_PORT_H */
  ])dnl
],[
#ifdef HAVE_MACH_PORT_H
# include <mach/port.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "This conftest for port_t expects <mach/port.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_MACH_PORT_H */
])dnl
AC_TYPE_INTPTR_T
AC_TYPE_INT32_T
AC_TYPE_UINTPTR_T
AC_TYPE_UINT32_T

# APPLE LOCAL: mmap() always works right on Darwin. Take our word for it.
case "${host}" in
  *-apple-darwin* | *-apple-macos*)
    ac_cv_func_mmap_fixed_mapped=yes
  ;;
esac
AC_FUNC_MMAP

# Checks for other functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([close lseek mach_vm_region memmove memset msync munmap \
                sbrk strerror vm_region])dnl
AC_CHECK_DECLS_ONCE([getenv])dnl

BFD_NEED_DECLARATION([sbrk])dnl
BFD_NEED_DECLARATION([getpagesize])dnl

if test "x${ac_cv_func_getpagesize}" = "x"; then
  test -z "${ac_cv_func_getpagesize}" || unset ac_cv_func_getpagesize
  AC_CHECK_FUNCS([getpagesize])
fi

AC_MSG_CHECKING([for page-protection malloc support])
case "${host}" in
*-apple-darwin*)
	PAGECHECK_SRC=pagecheck-sup.c
	PAGECHECK_OBJ=pagecheck-sup.o
	;;
*)
	PAGECHECK_SRC=""
	PAGECHECK_OBJ=""
	;;
esac
AC_SUBST([PAGECHECK_SRC])dnl
AC_SUBST([PAGECHECK_OBJ])dnl
AC_MSG_RESULT([${PAGECHECK_SRC}])dnl

# Output.
AC_CONFIG_HEADERS([config.h])dnl
AC_CONFIG_FILES([Makefile])dnl
AC_OUTPUT
