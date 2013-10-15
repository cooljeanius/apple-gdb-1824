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
AC_CANONICAL_TARGET

AC_PROG_AWK
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_CHECK_TOOL([AR],[ar])
AC_CHECK_TOOL([RANLIB],[ranlib],[:])
if test "x$RANLIB" = "x"; then
	test -z "$RANLIB"
	AC_PROG_RANLIB
else
	test ! -z "$RANLIB" && export RANLIB
	AC_SUBST([RANLIB])
fi

# Checks for libraries.
AC_CHECK_LIB([c],[printf])
AC_SEARCH_LIBS([vm_region],[System])

# Checks for header files.
AC_CHECK_HEADERS([AvailabilityMacros.h db.h errno.h fcntl.h \
                  ipc/ipc_types.h limits.h mach/mach.h \
                  mach/mach_port.h mach/mach_vm.h mach/port.h \
                  mach/vm_region.h stddef.h stdio.h sys/mman.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_BIGENDIAN
AC_C_PROTOTYPES
AC_TYPE_OFF_T
AC_TYPE_SIZE_T
AC_CHECK_TYPES([caddr_t])
AC_CHECK_TYPES([port_t],[],[
  AC_CHECK_TYPES([mach_port_t],[
    AC_CHECK_TYPES([mach_port_array_t],[],[],[
#ifdef HAVE_MACH_PORT_H
# include <mach/port.h>
#else
# warning this conftest expects <mach/port.h> to be included.
#endif /* HAVE_MACH_PORT_H */
    ])
    AC_CHECK_TYPES([mach_port_type_t],[],[],[
#ifdef HAVE_MACH_PORT_H
# include <mach/port.h>
#else
# warning this conftest expects <mach/port.h> to be included.
#endif /* HAVE_MACH_PORT_H */
    ])
  ],[
    AC_CHECK_TYPES([ipc_port_t],[],[],[
#ifdef HAVE_IPC_IPC_TYPES_H
# include <ipc/ipc_types.h>
#else
# warning this conftest expects <ipc/ipc_types.h> to be included.
#endif /* HAVE_IPC_IPC_TYPES_H */
    ])
  ],[
#ifdef HAVE_MACH_PORT_H
# include <mach/port.h>
#else
# warning this conftest expects <mach/port.h> to be included.
#endif /* HAVE_MACH_PORT_H */
  ])
],[
#ifdef HAVE_MACH_PORT_H
# include <mach/port.h>
#else
# warning this conftest expects <mach/port.h> to be included.
#endif /* HAVE_MACH_PORT_H */
])
AC_TYPE_INT32_T
AC_TYPE_UINT32_T

# APPLE LOCAL: mmap() always works right on Darwin. Take our word for it.
case "$host" in
  *-apple-darwin* | *-apple-macos*)
    ac_cv_func_mmap_fixed_mapped=yes
  ;;
esac
AC_FUNC_MMAP

# Checks for other functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([close lseek mach_vm_region memmove memset msync munmap \
                sbrk strerror vm_region])

BFD_NEED_DECLARATION([sbrk])
BFD_NEED_DECLARATION([getpagesize])

AC_MSG_CHECKING([for page-protection malloc support])
case "$host" in
*-apple-darwin*)
	PAGECHECK_SRC=pagecheck-sup.c
	PAGECHECK_OBJ=pagecheck-sup.o
	;;
*)
	PAGECHECK_SRC=
	PAGECHECK_OBJ=
	;;
esac
AC_SUBST([PAGECHECK_SRC])
AC_SUBST([PAGECHECK_OBJ])
AC_MSG_RESULT([$PAGECHECK_SRC])

AC_CONFIG_HEADERS([config.h:config.in])
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
