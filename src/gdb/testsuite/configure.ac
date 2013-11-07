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

AC_PREREQ([2.69])
AC_INIT([gdb-testsuite],[1824],[])
AC_CONFIG_SRCDIR([gdb.base])

AC_CANONICAL_BUILD
AC_CANONICAL_HOST
AC_CANONICAL_TARGET

# Add HP-specific tests when appropriate.
case $target in
  hppa*-*-hpux*)
    AC_CONFIG_SUBDIRS([gdb.hp]) ;;
esac

# With stabs.
AC_ARG_WITH([stabs],
[AS_HELP_STRING([--with-stabs],[arrange to use stabs instead of host debug format])],[],
[# We enable stabs tests by default on selected targets.
case $target in
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
if test $with_stabs = yes; then
   AC_CONFIG_SUBDIRS([gdb.stabs])
fi

# Enable gdbtk.
AC_ARG_ENABLE([gdbtk],
[AS_HELP_STRING([--enable-gtk],[enable gdbtk graphical user interface (GUI)])],[],
  [if test -d $srcdir/gdb.gdbtk; then
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

# Add gdbtk tests when appropriate.
if test $enable_gdbtk = yes; then
   AC_CONFIG_SUBDIRS([gdb.gdbtk])
fi

# Enable shared libraries.
AC_ARG_ENABLE([shared],
[AS_HELP_STRING([--enable-shared],[build shared libraries [deault=yes]])],[],
  [enable_shared=yes])

# If we have shared libraries, try to set RPATH_ENVVAR reasonably,
# such that we can find the shared libraries in the build tree.
if test $enable_shared = no; then
  # The variable `RPATH_ENVVAR' itself is not likely to be used on any
  # platform.
  RPATH_ENVVAR=RPATH_ENVVAR
else
  # The variable `LD_LIBRARY_PATH' is used on most platforms.
  RPATH_ENVVAR=LD_LIBRARY_PATH
  # The following exceptions are taken from Libtool 1.4.3.
  case $host_os in
    aix*)
      if test $host_cpu != ia64; then
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
AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL

# Checks for header files.
AC_HEADER_STDBOOL dnl# calls AC_CHECK_HEADER_STDBOOL
AC_CHECK_HEADERS([fcntl.h limits.h stddef.h stdio.h sys/time.h])
AC_CHECK_HEADERS([pthread.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_OFF_T
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T

# Checks for library functions.
AC_FUNC_ALLOCA
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_FUNC_MMAP
AC_CHECK_FUNCS([alarm memset select])

# Other
AC_EXEEXT

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

AC_CONFIG_FILES([Makefile \
  gdb.ada/Makefile \
  gdb.arch/Makefile \
  gdb.asm/Makefile \
  gdb.base/Makefile \
  gdb.cp/Makefile \
  gdb.disasm/Makefile \
  gdb.dwarf2/Makefile \
  gdb.fortran/Makefile \
  gdb.server/Makefile \
  gdb.java/Makefile \
  gdb.mi/Makefile \
  gdb.threads/Makefile \
  gdb.trace/Makefile])
AC_OUTPUT
