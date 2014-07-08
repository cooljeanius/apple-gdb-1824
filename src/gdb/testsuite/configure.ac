#                                                       -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

# Copyright 2002, 2003, 2004, 2005
# Free Software Foundation, Inc.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
# 02111-1307, USA.

sinclude(../../config/acx.m4)

AC_PREREQ([2.61])
AC_INIT([gdb-testsuite],[1824],[bug-gdb@gnu.org])
AC_CONFIG_SRCDIR([gdb.base])

AC_CANONICAL_BUILD
AC_CANONICAL_HOST
AC_CANONICAL_TARGET

ACX_NONCANONICAL_TARGET

# Add HP-specific tests when appropriate.
case ${target} in
  hppa*-*-hpux*)
    AC_CONFIG_SUBDIRS([gdb.hp]) ;;
esac

# With stabs.
AC_ARG_WITH([stabs],
[AS_HELP_STRING([--with-stabs],
                [arrange to use stabs instead of host debug format])],[],
[# We enable stabs tests by default on selected targets.
case ${target} in
   *-apple-darwin* \
 | powerpc-*-aix* \
 | rs6000-*-aix*  \
 | *-*-*bsd*      \
 | *-*-go32*      \
 | *-*-linux*     \
 | *-*-lynxos*    \
 | *-sun-*        \
 | hppa*-*-*      \
 | *-*-elf*       \
 )
    with_stabs=yes ;;
 *)
    with_stabs=no ;;
esac])

# Add stabs tests when appropriate.
if test "x${with_stabs}" = "xyes"; then
   AC_CONFIG_SUBDIRS([gdb.stabs])
fi

# Enable gdbtk.
AC_ARG_ENABLE([gdbtk],
[AS_HELP_STRING([--enable-gtk],
                [enable gdbtk graphical user interface (GUI)])],[],
  [if test -d ${srcdir}/gdb.gdbtk; then
    enable_gdbtk=yes
  else
    enable_gdbtk=no
  fi])
# We unconditionally disable gdbtk tests on selected platforms.
# APPLE LOCAL Darwin
case "${target}" in
  go32* | windows* | *darwin* )
    enable_gdbtk=no ;;
esac

# Other system-related checks:
AC_SYS_LARGEFILE
AC_SYS_LONG_FILE_NAMES

# Add gdbtk tests when appropriate.
if test "x${enable_gdbtk}" = "xyes"; then
   AC_CONFIG_SUBDIRS([gdb.gdbtk])
fi

# Enable shared libraries.
AC_ARG_ENABLE([shared],
[AS_HELP_STRING([--enable-shared],
                [build shared libraries [deault=yes]])],
[],[enable_shared=yes])

# If we have shared libraries, try to set RPATH_ENVVAR reasonably,
# such that we can find the shared libraries in the build tree.
if test "x${enable_shared}" = "xno"; then
  # The variable `RPATH_ENVVAR' itself is not likely to be used on any
  # platform.
  RPATH_ENVVAR=RPATH_ENVVAR
else
  # The variable `LD_LIBRARY_PATH' is used on most platforms.
  RPATH_ENVVAR=LD_LIBRARY_PATH
  # The following exceptions are taken from Libtool 1.4.3.
  case ${host_os} in
    aix*)
      if test "x${host_cpu}" != "xia64"; then
        RPATH_ENVVAR=LIBPATH
      fi ;;
    *darwin* | rhapsody*)
      RPATH_ENVVAR=DYLD_LIBRARY_PATH ;;
    hpux*)
      RPATH_ENVVAR=SHLIB_PATH ;;
  esac
fi
AC_SUBST([RPATH_ENVVAR])

# Checks for programs.
AC_PROG_AWK
AC_DEFUN([AC_REQUIRE_COMPILER],[
  AC_REQUIRE([AC_PROG_CC])
  AC_REQUIRE([AC_PROG_CPP])
  AC_REQUIRE([AC_PROG_CXX])
  AC_REQUIRE([AC_PROG_GCC_TRADITIONAL])
])
AC_REQUIRE_COMPILER
AC_PROG_INSTALL
AC_PROG_SED

# Checks for header files.
AC_HEADER_DIRENT
AC_HEADER_STDBOOL dnl# also "_CHECK"s it
AC_HEADER_SYS_WAIT
AC_HEADER_TIME
AC_CHECK_HEADERS([arpa/inet.h fcntl.h limits.h stddef.h stdio.h \
                  sys/socket.h sys/time.h wchar.h])
AC_CHECK_HEADERS([pthread.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_C_INLINE
AC_C_PROTOTYPES
AC_C_RESTRICT
AC_C_VOLATILE
AC_TYPE_INT8_T
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_OFF_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UINT8_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_STRUCT_TIMEZONE dnl# also does the "_TM" one
dnl# Begin what used to be AC\_TYPE\_SIGNAL
AC_CACHE_CHECK([return type of signal handlers],[ac_cv_type_signal],[
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <sys/types.h>
#include <signal.h>
]],[[
return *(signal (0, 0)) (0) == 1;
]])],[ac_cv_type_signal=int],[ac_cv_type_signal=void])
])
AC_DEFINE_UNQUOTED([RETSIGTYPE],[${ac_cv_type_signal}],[Define as the return type of signal handlers (`int' or `void').])
dnl# End what used to be AC\_TYPE\_SIGNAL

# Checks for library functions.
AC_FUNC_ALLOCA
AC_FUNC_CLOSEDIR_VOID
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_FUNC_MEMCMP
AC_FUNC_MMAP
AC_FUNC_REALLOC
AC_FUNC_SELECT_ARGTYPES
AC_FUNC_STAT
AC_FUNC_VPRINTF
AC_CHECK_FUNCS([atexit alarm clock_gettime gettimeofday memset munmap \
                select socket strchr strerror strstr strtol strtoul])

# Other:
AC_EXEEXT
AC_OBJEXT

# Output:
case "${target}" in
    *-*-darwin*) AC_CONFIG_FILES([gdb.apple/Makefile])
                 AC_CONFIG_FILES([gdb.apple/gused/Makefile])
                 AC_CONFIG_FILES([gdb.apple/ld-r/Makefile])
		 AC_CONFIG_FILES([gdb.apple/debug-in-ofile/Makefile])
                 AC_CONFIG_FILES([gdb.apple/xcode/Makefile])
                 AC_CONFIG_SUBDIRS([gdb.apple])
                 AC_CONFIG_SUBDIRS([gdb.objc])
                 ;;
esac

if test -z "${host_alias}"; then
  if test ! -z "${host_canonical}"; then
    export host_alias="${host_canonical}"
  elif test ! -z "${host}"; then
    export host_alias="${host}"
  fi
fi
AC_SUBST([host_alias])

if test -z "${target_alias}"; then
  if test ! -z "${target_canonical}"; then
    export target_alias="${target_canonical}"
  elif test ! -z "${target}"; then
    export target_alias="${target}"
  fi
fi
AC_SUBST([target_alias])

if test -z "${gdb_target_cpu}"; then
  if test ! -z "${target_cpu}"; then
    export gdb_target_cpu="${target_cpu}"
  fi
fi
AC_SUBST([gdb_target_cpu])

AC_CONFIG_SUBDIRS([gdb.cell])
AC_CONFIG_SUBDIRS([gdb.chill])
AC_CONFIG_FILES([Makefile \
  gdb.ada/Makefile \
  gdb.arch/Makefile \
  gdb.asm/Makefile \
  gdb.base/Makefile \
  gdb.btrace/Makefile \
  gdb.cp/Makefile \
  gdb.disasm/Makefile \
  gdb.dlang/Makefile \
  gdb.dwarf2/Makefile \
  gdb.fortran/Makefile \
  gdb.gdb/Makefile \
  gdb.go/Makefile \
  gdb.guile/Makefile \
  gdb.java/Makefile \
  gdb.linespec/Makefile \
  gdb.mi/Makefile \
  gdb.modula2/Makefile \
  gdb.multi/Makefile \
  gdb.opencl/Makefile \
  gdb.opt/Makefile \
  gdb.pascal/Makefile \
  gdb.perf/Makefile \
  gdb.reverse/Makefile \
  gdb.server/Makefile \
  gdb.threads/Makefile \
  gdb.trace/Makefile \
  gdb.xml/Makefile])
AC_OUTPUT
