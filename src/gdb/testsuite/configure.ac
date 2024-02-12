dnl#                                                       -*- Autoconf -*-
dnl# Process this file with autoconf to produce a configure script.

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

echo "this message should be the first one printed from this subdir"

sinclude(../../config/acx.m4)

echo "CPPFLAGS are '${CPPFLAGS}'"

AC_PREREQ([2.70])dnl

echo "initializing autoconf stuff for the testsuite..."

AC_INIT([gdb-testsuite],[1824],[bug-gdb@gnu.org])dnl
AC_CONFIG_SRCDIR([gdb.base])dnl
AC_CONFIG_HEADERS([config.h])dnl

# Architecture stuff:
AC_MSG_CHECKING([architecture stuff])
AC_MSG_RESULT([ ])
AC_CANONICAL_BUILD
AC_CANONICAL_HOST
AC_CANONICAL_TARGET

ACX_NONCANONICAL_TARGET

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
esac])dnl

# Add stabs tests when appropriate.
if test "x${with_stabs}" = "xyes"; then
   AC_CONFIG_SUBDIRS([gdb.stabs])dnl
   ## 3...
fi

# Check for 'sed' before doing the rest of this script:
AC_DEFUN([AC_REQUIRE_PROG_SED],[
  AC_REQUIRE([AC_PROG_SED])dnl
])dnl
AC_REQUIRE_PROG_SED

# Hack:
if test -e ./config.cache; then
  echo "hacking CPPFLAGS out of ./config.cache"
  sed -i "s|ac_cv_env_CPPFLAGS_set|bad_CPPFLAGS_status|g" ./config.cache
  sed -i "s|ac_cv_env_CPPFLAGS_value|bad_CPPFLAGS|g" ./config.cache
elif test -e ../config.cache; then
  echo "hacking CPPFLAGS out of ../config.cache"
  sed -i "s|ac_cv_env_CPPFLAGS_set|bad_CPPFLAGS_status|g" ../config.cache
  sed -i "s|ac_cv_env_CPPFLAGS_value|bad_CPPFLAGS|g" ../config.cache
elif test -r .././config.cache -a -w .././config.cache; then
  echo "hacking CPPFLAGS out of .././config.cache"
  sed -i "s|ac_cv_env_CPPFLAGS_set|bad_CPPFLAGS_status|g" .././config.cache
  sed -i "s|ac_cv_env_CPPFLAGS_value|bad_CPPFLAGS|g" .././config.cache
fi

# Check for the 'make' the user wants to use.
AC_CHECK_PROGS([MAKE],[make gmake gnumake remake])
MAKE_IS_GNU=""
case "`${MAKE} --version 2>&1 | sed 1q`" in
  *GNU*)
    MAKE_IS_GNU=yes
    ;;
esac
AM_CONDITIONAL([GMAKE],[test "x${MAKE_IS_GNU}" = "xyes"])
AC_PROG_MAKE_SET

# Enable gdbtk.
AC_ARG_ENABLE([gdbtk],
[AS_HELP_STRING([--enable-gdbtk],
                [enable gdbtk graphical user interface (GUI)])],[],
  [if test -d ${srcdir}/../gdbtk && test -d ${srcdir}/gdb.gdbtk; then
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
[],[enable_shared=yes])dnl

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
AC_SUBST([RPATH_ENVVAR])dnl

# Checks for programs.
AC_PROG_AWK
AC_DEFUN([AC_REQUIRE_COMPILER],[
  AC_REQUIRE([AC_PROG_CC])dnl
  AC_REQUIRE([AM_PROG_CC_C_O])dnl
  AC_REQUIRE([AC_PROG_CPP])dnl
  AC_REQUIRE([AC_PROG_CXX])dnl
  AC_SUBST([GCC])dnl
])dnl
AC_REQUIRE_COMPILER
AC_PROG_INSTALL
dnl# ('sed' is now checked above)

# Checks for header files.
AC_HEADER_DIRENT
AC_HEADER_STDBOOL dnl# also "_CHECK"s it
AC_HEADER_SYS_WAIT
AC_HEADER_TIME
AC_CHECK_HEADERS([arpa/inet.h fcntl.h limits.h stddef.h stdio.h \
                  sys/socket.h sys/time.h wchar.h])dnl
AC_CHECK_HEADERS([pthread.h])dnl

if test "x${ac_cv_header_stdint_h}" = "x"; then
  test -z "${ac_cv_header_stdint_h}" || unset ac_cv_header_stdint_h
  AC_CHECK_HEADERS([stdint.h])
fi
if test "x${ac_cv_header_unistd_h}" = "x"; then
  test -z "${ac_cv_header_unistd_h}" || unset ac_cv_header_unistd_h
  AC_CHECK_HEADERS([unistd.h])
fi

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
]])],[ac_cv_type_signal=int],[ac_cv_type_signal=void])dnl
])dnl
  ## define:
AC_DEFINE_UNQUOTED([RETSIGTYPE],[${ac_cv_type_signal}],
                   [Define as the return type of signal handlers
                    (`int' or `void').])dnl
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
                select socket strchr strerror strstr strtol strtoul])dnl

if test "x${ac_cv_func_getpagesize}" = "x"; then
  test -z "${ac_cv_func_getpagesize}" || unset ac_cv_func_getpagesize
  AC_CHECK_FUNCS([getpagesize])
fi

# Other:
AC_EXEEXT
AC_OBJEXT

m4_ifdef([AM_MAINTAINER_MODE],[
  AM_MAINTAINER_MODE
],[
  MAINT='#'
  AC_SUBST([MAINT])dnl
])dnl

if test "x${build}" = "x${host}" && test "x${host}" = "x${target}"; then
   case "${host}" in
     *gnu*)
       EXTRA_RULES=read1
       ;;
   esac
fi
AC_SUBST([EXTRA_RULES])dnl

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
AC_SUBST([host_alias])dnl

if test -z "${target_alias}"; then
  if test ! -z "${target_canonical}"; then
    export target_alias="${target_canonical}"
  elif test ! -z "${target}"; then
    export target_alias="${target}"
  fi
fi
AC_SUBST([target_alias])dnl

if test -z "${gdb_target_cpu}"; then
  if test ! -z "${target_cpu}"; then
    export gdb_target_cpu="${target_cpu}"
  fi
fi
AC_SUBST([gdb_target_cpu])dnl

# The rest of the output:
AC_CONFIG_SUBDIRS([gdb.asm])dnl
AC_CONFIG_SUBDIRS([gdb.base])dnl
AC_CONFIG_SUBDIRS([gdb.c++])dnl
AC_CONFIG_SUBDIRS([gdb.cell])dnl
AC_CONFIG_SUBDIRS([gdb.chill])dnl
AC_CONFIG_SUBDIRS([gdb.disasm])dnl
AC_CONFIG_SUBDIRS([gdb.threads])dnl
AC_CONFIG_FILES([Makefile \
  gdb.ada/Makefile \
  gdb.arch/Makefile \
  gdb.btrace/Makefile \
  gdb.cp/Makefile \
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
  gdb.trace/Makefile \
  gdb.xml/Makefile])dnl
AC_OUTPUT
