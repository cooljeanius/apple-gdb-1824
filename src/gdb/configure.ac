dnl#                                               -*- Autoconf -*-
dnl# Autoconf configure script for GDB, the GNU debugger.
dnl# Copyright 1995, 1996, 1997, 1998, 1999, 2000, 2001, 2002, 2003, 2004
dnl# Free Software Foundation, Inc.
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

AC_PREREQ([2.61])dnl
AC_INIT([gdb],[1824],[jmolenda@apple.com])
dnl# bug report address is email address of last person to touch this
dnl# directory, according to the Changelog-Apple in this directory
AC_CONFIG_SRCDIR([main.c])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADER([config.h])
AM_MAINTAINER_MODE

if test -e ${srcdir}/development.sh; then
  # Set the 'development' global.
  AC_MSG_NOTICE([sourcing ${srcdir}/development.sh])
  . ${srcdir}/development.sh
fi

AC_PROG_CC
AC_USE_SYSTEM_EXTENSIONS
AC_SEARCH_LIBS([strerror],[cposix]) dnl# used to be AC\_ISC\_POSIX
AM_PROG_CC_STDC

AC_CONFIG_AUX_DIR([..])
AC_CANONICAL_TARGET
if test -z "${HOST_ARCHITECTURE}" -a -n "${host_cpu}"; then
  export HOST_ARCHITECTURE="${host_cpu}"
  AC_SUBST([HOST_ARCHITECTURE])
fi
if test -z "${TARGET_ARCHITECTURE}" -a -n "${target_cpu}"; then
  export TARGET_ARCHITECTURE="${target_cpu}"
  AC_SUBST([TARGET_ARCHITECTURE])
fi

# Check for the 'make' the user wants to use:
AC_CHECK_PROGS([MAKE],[make])
AC_SUBST([MAKE])
MAKE_IS_GNU=""
case "`${MAKE} --version 2>&1 | sed 1q`" in
  *GNU*)
    MAKE_IS_GNU=yes
    AC_SUBST([MAKE_IS_GNU])
    ;;
esac
AM_CONDITIONAL([GMAKE],[test "x${MAKE_IS_GNU}" = "xyes"])

# Automake (we can use this now that gnulib has a Makefile.am, even if
# srcdir itself still uses just a Makefile.in)
if test -f ${srcdir}/config.status; then
  AC_MSG_NOTICE([config.status is already present, removing it.])
  rm -f ${srcdir}/config.status
fi
AM_INIT_AUTOMAKE([1.11 gnu dejagnu dist-bzip2 dist-xz dist-zip dist-tarZ])
if test -x ${srcdir}/missing; then
  test -e missing
  AM_SANITY_CHECK
fi

dnl# List of object files and targets accumulated by configure.

CONFIG_OBS=""
CONFIG_DEPS=""
CONFIG_SRCS=""
ENABLE_CFLAGS=""
ENABLE_LDFLAGS=""

CONFIG_ALL=""
CONFIG_CLEAN=""
CONFIG_INSTALL=""
CONFIG_UNINSTALL=""

AC_MSG_NOTICE([doing some preliminary APPLE_LOCAL checks])
dnl# APPLE LOCAL begin mmap() always works right on Darwin. Take our word
dnl# for it.
dnl# CY_GNU_GETTEXT will call AC_FUNC_MMAP for us, so needs to be up here,
dnl# rather than further down.
case "${host}" in
  *-apple-darwin* | *-apple-macos*)
   ac_cv_func_mmap_fixed_mapped=yes
  ;;
esac
dnl# APPLE LOCAL end mmap() always works right on Darwin. Take our word
dnl# for it.

dnl# APPLE LOCAL if we have mach_exc.def, use the 64 bit exceptions.
case "${host}" in
  arm-apple-*)
    # No 64 bit exceptions for Apple ARM (yet?)
    AC_MSG_CHECKING([whether we can use 64 bit mach exceptions])
    MACH_EXC_SERVER_PREFACE=""
    AC_MSG_RESULT([(disabled!) no])
    AC_SUBST([MACH_EXC_SERVER_PREFACE])
    ;;
  *-apple-darwin* | *-apple-macosx*)
    AC_MSG_CHECKING([whether we can use 64 bit mach exceptions])
    if test -r "${SDKROOT}/usr/include/mach/mach_exc.defs"; then
      AC_DEFINE([HAVE_64_BIT_MACH_EXCEPTIONS],[1],
                [Define to 1 if you have 64 bit mach exceptions.])
      MACH_EXC_SERVER_PREFACE='mach_'
      AC_MSG_RESULT([yes])
    else
      MACH_EXC_SERVER_PREFACE=""
      AC_MSG_RESULT([no])
    fi
    AC_SUBST([MACH_EXC_SERVER_PREFACE])
;;
esac

dnl# APPLE LOCAL check for posix_spawn and the "arch" program. Note that we
dnl# do NOT trust the "arch" program if the posix_spawn headers are NOT
dnl# present, since Tiger has an "arch" program, but it only echoes the
dnl# current system architecture.
case "${host}" in
  *-apple-darwin* | *-apple-macosx*)
    AC_CHECK_HEADERS_ONCE([spawn.h])
    # not sure why a bunch of other headers are checked between these 2...
    # (it is because of the "_ONCE" in the header check, FIXME)
    AC_MSG_CHECKING([whether posix_spawn supports SETEXEC])
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM(
[[#include <spawn.h>;]],
[[int foo = POSIX_SPAWN_SETEXEC;]])],
      [gdb_cv_posix_spawn=yes],
      [gdb_cv_posix_spawn=no])
    AC_MSG_RESULT([${gdb_cv_posix_spawn}])

    if test "x${gdb_cv_posix_spawn}" = "xyes"; then
      AC_DEFINE([USE_POSIX_SPAWN],[1],
                [Define to 1 to use posix_spawn instead of execvp.])

      AC_ARG_ENABLE([arch-for-exec],
      	[AS_HELP_STRING([--disable-arch-for-exec],
                        [disable use of arch for exec])],
      	[case ${enableval} in
          yes | no) 
            enable_arch_exec="${enableval}" ;;
          *)
            AC_MSG_ERROR([bad value '${enableval}' for --disable-arch-for-exec])
            ;;
      	esac],
      	[enable_arch_exec=yes])
  
      AC_MSG_CHECKING([whether we should use arch for exec])
      if test x"${enable_arch_exec}" = x"yes"; then
        if test -r "/usr/bin/arch"; then
          AC_DEFINE([USE_ARCH_FOR_EXEC],[1],
                    [Define to 1 to use the arch program instead of exec.])
          AC_MSG_RESULT([yes])
        else
          AC_MSG_RESULT([no])
        fi # done with check for arch
      else
        AC_MSG_RESULT([(disabled) no])
      fi # done with check for enabling arch for exec
    fi # done with things to do with posix_spawn
    ;;
esac

dnl# APPLE LOCAL check for the 64 bit stack logging function named
dnl# __mach_stack_logging_enumerate_records and define 
dnl# HAVE_64_BIT_STACK_LOGGING if it is found. If it is NOT found, 
dnl# check for the 32 bit stack logging function named 
dnl# stack_logging_enumerate_records and define 
dnl# HAVE_32_BIT_STACK_LOGGING if it is available.
case "${host}" in
  *-apple-darwin* | *-apple-macosx*)
    AC_MSG_CHECKING([whether 64 bit stack logging is supported])
    AC_LINK_IFELSE([AC_LANG_PROGRAM(
[[extern int __mach_stack_logging_enumerate_records();]],
[[__mach_stack_logging_enumerate_records();]])],
      [gdb_cv_stack_logging_64=yes],
      [gdb_cv_stack_logging_64=no])
    if test "x${gdb_cv_stack_logging_64}" = "xyes"; then
      AC_DEFINE([HAVE_64_BIT_STACK_LOGGING],[1],
                [Define to 1 to use 64 bit stack logging functions.])
    fi
    AC_MSG_RESULT([${gdb_cv_stack_logging_64}])

    if test "x${gdb_cv_stack_logging_64}" = "xno"; then
    	AC_MSG_CHECKING([whether 32 bit stack logging is supported])
    	AC_LINK_IFELSE([AC_LANG_PROGRAM(
[[extern int stack_logging_enumerate_records();]],
[[stack_logging_enumerate_records();]])],
      	[gdb_cv_stack_logging_32=yes],
      	[gdb_cv_stack_logging_32=no])
    	if test "x${gdb_cv_stack_logging_32}" = "xyes"; then
      	  AC_DEFINE([HAVE_32_BIT_STACK_LOGGING],[1],
                    [Define to 1 to use 64 bit stack logging functions.])
    	fi
    	AC_MSG_RESULT([${gdb_cv_stack_logging_32}])
    fi
    ;;
esac

dnl# APPLE LOCAL check for task_get_state.
case "${host}" in
  *-apple-darwin* | *-apple-macosx*)
    AC_CHECK_HEADERS_ONCE([mach/task.h])
    AC_MSG_CHECKING([whether task_set_state is available])
    AC_LINK_IFELSE([AC_LANG_PROGRAM(
[[#include <mach/mach.h>]
 [#include <mach/task.h>]],
[[task_t task;]
 [thread_state_flavor_t flavor;]
 [thread_state_t new_state;]
 [mach_msg_type_number_t new_stateCnt;]
 [task_set_state(task, flavor, new_state, new_stateCnt);]])],
      [gdb_cv_task_set_state=yes],
      [gdb_cv_task_set_state=no])
    if test "x${gdb_cv_task_set_state}" = "xyes"; then
      AC_DEFINE([HAVE_TASK_SET_STATE],[1],
                [Define to 1 to use task_set_state.])
    fi
    AC_MSG_RESULT([${gdb_cv_task_set_state}])
    AC_CHECK_TYPES([task_t],[],[],[
#ifdef HAVE_MACH_TASK_H
# include <mach/task.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "This conftest for task_t expects <mach/task.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_MACH_TASK_H */
    ])
    ;;
esac

AC_MSG_NOTICE([done with first batch of APPLE_LOCAL checks])

dnl# Set up for gettext. PACKAGE is used when we call bindtextdomain.
AC_MSG_CHECKING([gettext-related things (Cygnus-style)])
AC_MSG_RESULT([ ])
if test -d ../intl; then
  if test -z "${LDFLAGS}"; then
    export LDFLAGS="-L../intl"
  else
    export LDFLAGS="${LDFLAGS} -L../intl"
  fi
elif test -d ../libintl; then
  if test -z "${LDFLAGS}"; then
    export LDFLAGS="-L../libintl"
  else
    export LDFLAGS="${LDFLAGS} -L../libintl"
  fi
fi
ALL_LINGUAS=""
CY_GNU_GETTEXT
AC_CHECK_FUNCS([gettext _gettext])

localedir='${datadir}/locale'
AC_SUBST([localedir])

if test "x${POSUB}" != "x"; then

  dnl# Lifted from GCC's config/gettext.m4.
  AC_MSG_CHECKING([for catalogs to be installed])
  # Look for .po and .gmo files in the source directory.
  CATALOGS=""  AC_SUBST([CATALOGS])
  XLINGUAS=""
  for cat in ${srcdir}/po/*.gmo ${srcdir}/po/*.po; do
    # If there are NOT any .gmo files the shell will give us the
    # literal string "../path/to/srcdir/po/*.gmo" which has to be
    # weeded out.
    case "${cat}" in *\**)
      continue;;
    esac
    # The quadruple backslash is collapsed to a double backslash
    # by the backticks, then collapsed again by the double quotes,
    # leaving us with one backslash in the sed expression (right
    # before the dot that must NOT act as a wildcard).
    cat=`echo ${cat} | sed -e "s!${srcdir}/!!" -e "s!\\\\.po!.gmo!"`
    lang=`echo ${cat} | sed -e 's!po/!!' -e "s!\\\\.gmo!!"`
    # The user is allowed to set LINGUAS to a list of languages to
    # install catalogs for. If it is empty, that means "all of them."
    if test "x${LINGUAS}" = "x"; then
      CATALOGS="${CATALOGS} ${cat}"
      XLINGUAS="${XLINGUAS} ${lang}"
    else
      case "${LINGUAS}" in *${lang}*)
        CATALOGS="${CATALOGS} ${cat}"
        XLINGUAS="${XLINGUAS} ${lang}"
        ;;
      esac
    fi
  done
  LINGUAS="${XLINGUAS}"
  AC_MSG_RESULT([${LINGUAS}])

  CONFIG_ALL="${CONFIG_ALL} all-po"
  CONFIG_CLEAN="${CONFIG_CLEAN} clean-po"		   
  CONFIG_INSTALL="${CONFIG_INSTALL} install-po"
  CONFIG_UNINSTALL="${CONFIG_UNINSTALL} uninstall-po"
elif test x"${USE_NLS}" = x"yes"; then
  CONFIG_ALL="${CONFIG_ALL} all-po"
  CONFIG_CLEAN="${CONFIG_CLEAN} clean-po"
  CONFIG_INSTALL="${CONFIG_INSTALL} install-po"
  CONFIG_UNINSTALL="${CONFIG_UNINSTALL} uninstall-po"
fi

GNULIB_IMPORT_DIR=build-gnulib/import

# For Makefile dependencies.
GNULIB_STDINT_H=""
if test x"${STDINT_H}" != x""; then
  test -n "${STDINT_H}" && test -n "${GNULIB_IMPORT_DIR}"
  export GNULIB_STDINT_H=${GNULIB_IMPORT_DIR}/${STDINT_H}
fi
AC_SUBST([GNULIB_STDINT_H])

PACKAGE=gdb
AC_DEFINE_UNQUOTED([PACKAGE],["${PACKAGE}"],[Name of this package. ])
AC_SUBST([PACKAGE])

AC_MSG_NOTICE([done with Cygnus gettext checks])

# We never need to detect it in this sub-configure.
# But preserve it for config.status --recheck.
AC_ARG_VAR([MAKEINFO],
  [Parent configure detects if it is of sufficient version.])
AC_ARG_VAR([MAKEINFOFLAGS],
  [Parameters for MAKEINFO.])

MAKEINFO_EXTRA_FLAGS=""
if test "x${MAKEINFO}" != "x"; then
  test -n "${MAKEINFO}"
  AC_CACHE_CHECK([whether ${MAKEINFO} supports @click],
                 [gdb_cv_have_makeinfo_click],
    [echo '@clicksequence{a @click{} b}' >conftest.texinfo
     if eval "${MAKEINFO} conftest.texinfo >&5 2>&5"; then
       gdb_cv_have_makeinfo_click=yes
     else
       gdb_cv_have_makeinfo_click=no
     fi])
  if test x"${gdb_cv_have_makeinfo_click}" = x"yes"; then
    MAKEINFO_EXTRA_FLAGS="${MAKEINFO_EXTRA_FLAGS} -DHAVE_MAKEINFO_CLICK"
  fi
  AC_SUBST([MAKEINFO_EXTRA_FLAGS])
fi

# debugdir setting:
debugdir=${libdir}/debug
	 
AC_ARG_WITH([separate-debug-dir],
[AS_HELP_STRING([--with-separate-debug-dir=path],
       [Look for global separate debug info in this path [LIBDIR/debug]])],
[debugdir="${withval}"])
	
AC_DEFINE_DIR([DEBUGDIR],[debugdir],
              [Global directory for separate debug files. ])
if test -z "${DEBUGDIR}" -a ! -z "${debugdir}" -a "x${GCC}" = "xno"; then
  test -d "${debugdir}"
  AC_DEFINE_UNQUOTED([DEBUGDIR],["${debugdir}"],
                     [Global directory for separate debug files. ])
fi

AC_CONFIG_SUBDIRS([doc testsuite])
AC_CONFIG_SUBDIRS([gnulib])

if test "x${target}" = "x${host}"; then
  gdb_native=yes
else
  gdb_native=no
fi

# Make sure shell scripts work:
AC_SYS_INTERPRETER

if test "x${SHELL}" != "x"; then
  test -n "${SHELL}" && export SHELL
  AC_SUBST([SHELL])
fi

# Now we can source some shell scripts:
AC_MSG_NOTICE([sourcing ${srcdir}/configure.host ])
. ${srcdir}/configure.host

AC_MSG_NOTICE([sourcing ${srcdir}/configure.tgt ])
. ${srcdir}/configure.tgt

# Fetch the default architecture and default target vector from BFD.
AC_MSG_NOTICE([sourcing ${srcdir}/../bfd/config.bfd for ${target} ])
targ=${target}; . ${srcdir}/../bfd/config.bfd

# We only want the first architecture, so strip off the others if
# there is more than one.
targ_archs=`echo ${targ_archs} | sed 's/ .*//'`

if test "x${targ_archs}" != "x"; then
  AC_DEFINE_UNQUOTED([DEFAULT_BFD_ARCH],[${targ_archs}],
    [Define to BFD's default architecture. ])
fi
if test "x${targ_defvec}" != "x"; then
  AC_DEFINE_UNQUOTED([DEFAULT_BFD_VEC],[${targ_defvec}],
    [Define to BFD's default target vector. ])
fi

AC_ARG_PROGRAM

# Some system checks.
m4_ifdef([ACX_LARGEFILE],[
ACX_LARGEFILE
],[
AC_SYS_LARGEFILE
])
AC_SYS_LONG_FILE_NAMES
AC_OBJEXT

# The CLI cannot be disabled yet, but may be in the future.

# Enable CLI.
AC_ARG_ENABLE([gdbcli],
[AS_HELP_STRING([--disable-gdbcli],
                [disable command-line interface (CLI)])],
  [case ${enableval} in
    yes)
      ;;
    no)
      AC_MSG_ERROR([the command-line interface cannot be disabled yet]) ;;
    *)
      AC_MSG_ERROR([bad value ${enableval} for --enable-gdbcli]) ;;
  esac],
  [enable_gdbcli=yes])
if test x"${enable_gdbcli}" = x"yes"; then
  if test -d ${srcdir}/cli; then
    CONFIG_OBS="${CONFIG_OBS} \$(SUBDIR_CLI_OBS)"
    CONFIG_DEPS="${CONFIG_DEPS} \$(SUBDIR_CLI_DEPS)"
    CONFIG_SRCS="${CONFIG_SRCS} \$(SUBDIR_CLI_SRCS)"
    ENABLE_CFLAGS="${ENABLE_CFLAGS} \$(SUBDIR_CLI_CFLAGS)"
    ENABLE_LDFLAGS="${ENABLE_LDFLAGS} \$(SUBDIR_CLI_LDFLAGS)"
  fi
fi

# Enable MI.
AC_ARG_ENABLE([gdbmi],
[AS_HELP_STRING([--disable-gdbmi],
                [disable machine-interface (MI)])],
  [case ${enableval} in
    yes | no)
      ;;
    *)
      AC_MSG_ERROR([bad value ${enableval} for --enable-gdbmi]) ;;
  esac],
  [enable_gdbmi=yes])
if test x"${enable_gdbmi}" = xyes; then
  if test -d ${srcdir}/mi; then
    CONFIG_OBS="${CONFIG_OBS} \$(SUBDIR_MI_OBS)"
    CONFIG_DEPS="${CONFIG_DEPS} \$(SUBDIR_MI_DEPS)"
    CONFIG_SRCS="${CONFIG_SRCS} \$(SUBDIR_MI_SRCS)"
    ENABLE_CFLAGS="${ENABLE_CFLAGS} \$(SUBDIR_MI_CFLAGS)"
    ENABLE_LDFLAGS="${ENABLE_LDFLAGS} \$(SUBDIR_MI_LDFLAGS)"
  fi
fi

# APPLE LOCAL: TUI is disabled by default
# Enable TUI.
AC_ARG_ENABLE([tui],
[AS_HELP_STRING([--enable-tui],
                [enable full-screen terminal user interface (TUI)])],
  [case ${enableval} in
    yes | no)
      ;;
    *)
      AC_MSG_ERROR([bad value ${enableval} for --enable-tui]) ;;
  esac],[enable_tui=no])

# Enable gdbtk.
AC_ARG_ENABLE([gdbtk],
[AS_HELP_STRING([--enable-gdbtk],
                [enable gdbtk graphical user interface (GUI)])],
  [case ${enableval} in
    yes | no)
      ;;
    *)
      AC_MSG_ERROR([bad value ${enableval} for --enable-gdbtk]) ;;
  esac],
  [if test -d ${srcdir}/gdbtk -a -d ${srcdir}/../itcl; then
    enable_gdbtk=yes
  else
    enable_gdbtk=no
  fi])
# We unconditionally disable gdbtk tests on selected platforms.
case ${host_os} in
  go32* | windows*)
    AC_MSG_WARN([gdbtk is NOT supported on ${host}; disabling])
    enable_gdbtk=no ;;
  darwin*)
    if test "x${enable_gdbtk}" = "xyes"; then
      AC_MSG_WARN([gdbtk support on darwin is tentative])
      echo "(but it is still okay to enable it though)"
    fi
    ;;
esac

# Libunwind support.
AC_ARG_WITH([libunwind],
[AS_HELP_STRING([--with-libunwind],
                [Use libunwind frame unwinding support])],
[case "${withval}" in
  yes) enable_libunwind=yes ;;
  no) enable_libunwind=no ;;
  *)
    AC_MSG_ERROR([bad value ${withval} for GDB with-libunwind option])
    ;;
esac],[
  AC_CHECK_HEADERS([libunwind.h])
  AC_CHECK_HEADERS([libunwind-ia64.h])
  if test x"${ac_cv_header_libunwind_h}" = x"yes" -a x"${ac_cv_header_libunwind_ia64_h}" = x"yes"; then
    enable_libunwind=yes;
  fi
])
   
if test x"${enable_libunwind}" = x"yes"; then
  AC_CHECK_HEADERS([libunwind.h])
  AC_CHECK_HEADERS([libunwind-ia64.h])
  AC_CHECK_DECLS([acc])
  AC_CHECK_FUNCS([unw_create_addr_space_p unw_get_saveloc_p \
                  unw_search_unwind_table_p])
  AC_DEFINE([HAVE_LIBUNWIND],[1],
            [Define if libunwind library is being used.])
  CONFIG_OBS="${CONFIG_OBS} libunwind-frame.o"
  CONFIG_DEPS="${CONFIG_DEPS} libunwind-frame.o"
  CONFIG_SRCS="${CONFIG_SRCS} libunwind-frame.c"
fi

# Profiling support.
AC_ARG_ENABLE([profiling],
[AS_HELP_STRING([--enable-profiling],[enable profiling of GDB])],
  [case ${enableval} in
    yes | no)
      ;;
    *)
      AC_MSG_ERROR([bad value ${enableval} for --enable-profile]) ;;
  esac],
 [enable_profiling=no])

AC_CHECK_FUNCS([monstartup _mcleanup])
AC_CACHE_CHECK([for _etext],[ac_cv_var__etext],
[AC_LINK_IFELSE([AC_LANG_PROGRAM([[
#include <stdlib.h>
extern char _etext;
 ]],[[
free(&_etext);
 ]])],[ac_cv_var__etext=yes],[ac_cv_var__etext=no])
])
if test "x${ac_cv_var__etext}" = "xyes"; then
  AC_DEFINE([HAVE__ETEXT],[1],
            [Define to 1 if your system has the _etext variable. ])
fi
if test "${enable_profiling}" = yes ; then
  if test "x${ac_cv_func_monstartup}" = "xno" || test "x${ac_cv_func__mcleanup}" = "xno"; then
    AC_MSG_ERROR([--enable-profiling requires monstartup and _mcleanup])
  fi
  PROFILE_CFLAGS=-pg
  OLD_CFLAGS="${CFLAGS}"
  CFLAGS="${CFLAGS} ${PROFILE_CFLAGS}"

  AC_CACHE_CHECK([whether ${CC} supports -pg],[ac_cv_cc_supports_pg],
    [AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[]],[[int x;]])],
                       [ac_cv_cc_supports_pg=yes],
                       [ac_cv_cc_supports_pg=no])])

  if test "x${ac_cv_cc_supports_pg}" = "xno"; then
   AC_MSG_ERROR([--enable-profiling needs a ${CC} supporting the -pg flag])
  fi

  CFLAGS="${OLD_CFLAGS}"
fi

m4_ifdef([ACX_PKGVERSION],[
ACX_PKGVERSION([GDB])
if test "x${PKGVERSION}" != "x"; then
  test -n "${PKGVERSION}"
  AC_DEFINE_UNQUOTED([PKGVERSION],["${PKGVERSION}"],
                     [Additional package description])
fi
],[
test -z "${PKGVERSION}"
])
m4_ifdef([ACX_BUGURL],[
ACX_BUGURL([http://www.gnu.org/software/gdb/bugs/])
if test "x${REPORT_BUGS_TO}" != "x"; then
  test -n "${REPORT_BUGS_TO}"
  AC_DEFINE_UNQUOTED([REPORT_BUGS_TO],["${REPORT_BUGS_TO}"],
                     [Bug reporting address])
fi
],[
test -z "${REPORT_BUGS_TO}"
])

# --------------------- #
# Checks for programs.  #
# --------------------- #

AC_MSG_CHECKING([programs])
AC_MSG_RESULT([ ])
AN_PROGRAM([ar],[AM_PROG_AR])
AN_MAKEVAR([AR],[AM_PROG_AR])
if test -x `which automake`; then
  test -z "${AR}"
  AM_PROG_AR
else
  test -z "${AR}"
  AC_CHECK_TOOL([AR],[ar])
fi
if test -e ./config.cache; then
  sed -i "s|ac_cv_env_CFLAGS_value|bad_CFLAGS|g" ./config.cache
  sed -i "s|ac_cv_env_CXXFLAGS_value|bad_CXXFLAGS|g" ./config.cache
  sed -i "s|ac_cv_exeext|bad_exeext|g" ./config.cache
fi
AN_PROGRAM([as],[AM_PROG_AS])
AN_MAKEVAR([AS],[AM_PROG_AS])
AM_PROG_AS
AC_PROG_CPP
AC_PROG_CXX

AC_MSG_CHECKING([libtool things])
AC_MSG_RESULT([ ])
LT_INIT([disable-fast-install dlopen])
LT_LANG([C])
dnl# even though gdb is written only in C, it has tests written
dnl# for debugging the following languages, so check them, too:
LT_LANG([C++])
LT_LANG([Go])
LT_LANG([Java])
LT_LANG([Fortran 77])
LT_LANG([Fortran])
LT_LANG([Windows Resource])

AC_MSG_CHECKING([other programs])
AC_MSG_RESULT([ ])
if test "x${AWK}" = "x"; then
  test -z "${AWK}"
  AC_PROG_AWK
else
  test ! -z "${AWK}" && export AWK
  AC_SUBST([AWK])
fi
AC_PROG_INSTALL
AM_PROG_INSTALL_STRIP
AM_PROG_LEX
AM_PATH_LISPDIR
AC_PROG_LN_S
AC_PROG_MKDIR_P
AM_PATH_PYTHON
AC_PROG_OBJC
AC_PROG_OBJCPP
AC_PROG_OBJCXX
AC_PROG_OBJCXXCPP
if test "x${RANLIB}" = "x"; then
  test -z "${RANLIB}"
  AC_PROG_RANLIB
else
  test ! -z "${RANLIB}" && export RANLIB
  AC_SUBST([RANLIB])
fi
AC_BAKEFILE_METROWERKS_EXTO
AX_COMPILER_VENDOR

AC_PATH_PROG([XCODEBUILD],[xcodebuild])

# ResourceManager-related tools:
AC_PATH_PROG([REZ],[Rez],[],[],[${PATH}:/Developer/Tools])
AC_PATH_PROG([DEREZ],[DeRez],[],[],[${PATH}:/Developer/Tools])
AC_PATH_PROG([REZDET],[RezDet],[],[],[${PATH}:/Developer/Tools])
AC_PATH_PROG([REZWACK],[RezWack],[],[],[${PATH}:/Developer/Tools])
AC_PATH_PROG([RESMERGER],[ResMerger],[],[],[${PATH}:/Developer/Tools])
AC_SUBST([REZ])
AC_SUBST([DEREZ])
AC_SUBST([REZDET])
AC_SUBST([REZWACK])
AC_SUBST([RESMERGER])

# Other Mac-specific tools:
AC_PATH_PROG([LD_CLASSIC],[ld_classic])
AC_PATH_PROG([MERGE_PEF],[MergePef])
AC_PATH_PROG([CODESIGN],[codesign])

# A few more compiler-y things
AM_PROG_VALAC([],[],[])
AC_PROG_YACC
AX_PROG_BISON([
  AC_MSG_WARN([Actually using bison broke stuff once when I tried it...])
],[
  AC_MSG_WARN([Using something other than bison is untested.])
])

AC_CHECK_TOOL([DLLTOOL],[dlltool])
AC_CHECK_TOOL([WINDRES],[windres])

# Needed for GNU/Hurd.
# (and for separate reasons for OS X)
AC_CHECK_TOOL([MIG],[mig])

# Tools for docs:
AC_PATH_PROGS([ROFF],[nroff groff troff roff])
AC_PATH_PROG([CXREF],[cxref])

dnl# We could not originally use AM\_INIT\_AUTOMAKE here (I have since 
dnl# added it above since then), but we might still need some of its
dnl# checks separately, so define this lite version of the macro
dnl# (only the AC\_REQUIRE'd parts, to prevent duplication):
AC_DEFUN([AM_INIT_MY_AUTOMAKE_LITE],[
  AC_PREREQ([2.65])
  dnl# Autoconf wants to disallow AM_ names. We explicitly allow
  dnl# the ones we care about.
  m4_pattern_allow([^AM_[A-Z]+FLAGS$])dnl
  AC_REQUIRE([AM_SET_CURRENT_AUTOMAKE_VERSION])dnl
  AC_REQUIRE([AC_PROG_INSTALL])dnl
  AC_REQUIRE([AC_PROG_SED])dnl

  # test whether we have cygpath
  if test -z "${CYGPATH_W}"; then
    if (cygpath --version) >/dev/null 2>/dev/null; then
      CYGPATH_W='cygpath -w'
    else
      CYGPATH_W=echo
    fi
  fi
  AC_SUBST([CYGPATH_W])

  # Some tools Automake needs.
  AC_REQUIRE([AM_SANITY_CHECK])dnl
  AC_REQUIRE([AC_ARG_PROGRAM])dnl
  AC_REQUIRE([AM_MISSING_HAS_RUN])dnl
  AM_MISSING_PROG([AUTOCONF],[autoconf])
  AM_MISSING_PROG([AUTOHEADER],[autoheader])
  AM_MISSING_PROG([MAKEINFO],[makeinfo])
  AC_REQUIRE([AM_PROG_INSTALL_SH])dnl
  AC_REQUIRE([AM_PROG_INSTALL_STRIP])dnl
  AC_REQUIRE([AC_PROG_MKDIR_P])dnl
  # For better backward compatibility. To be removed once Automake 1.9.x
  # dies out for good.
  AC_SUBST([mkdir_p],['$(MKDIR_P)'])
  # We need awk for the "check" target. The system "awk" is bad on
  # some platforms.
  AC_REQUIRE([AC_PROG_AWK])dnl
  AC_REQUIRE([AC_PROG_MAKE_SET])dnl
  AC_REQUIRE([AM_SET_LEADING_DOT])dnl
  AC_REQUIRE([AM_SILENT_RULES])dnl
  AC_REQUIRE([AM_MAINTAINER_MODE])dnl
  AC_REQUIRE([AC_PROG_GREP])dnl
  AC_REQUIRE([AM_PROG_CC_STDC])dnl
  AC_REQUIRE([AM_SET_DEPDIR])dnl
  AC_REQUIRE([AM_OUTPUT_DEPENDENCY_COMMANDS])dnl
  AC_REQUIRE([AM_MAKE_INCLUDE])dnl
  AC_REQUIRE([AM_DEP_TRACK])dnl
])
AM_INIT_MY_AUTOMAKE_LITE

# ---------------------- #
# Checks for libraries.  #
# ---------------------- #

AC_MSG_CHECKING([libraries])
AC_MSG_RESULT([ ])

# We might need to link with -lm; most simulators need it.
LT_LIB_M
AC_CHECK_LIB([m],[main])
case "${host}" in
*-*-nextstep*|*-apple-rhapsody*|*-apple-macos10*|*-apple-darwin*)
  # libm is present but Apple does not use it...
  # do NOT overwrite it when "clearing" it though:
  test -z "${LIBM}" && LIBM=""
  ;;
*)
  # as with Apple, do NOT overwrite an existing value, even though
  # this time we are giving it one instead of clearing it:
  test -z "${LIBM}" && export LIBM="-lm"
  AC_SUBST([LIBM])
  ;;
esac

# I like forcing a link against -lc:
AC_CHECK_LIB([c],[printf])

# APPLE LOCAL begin: Get the libxml2 headers and libraries
AC_PATH_PROG([LIBXML2_CONFIG],[xml2-config],[no])
if test "x${LIBXML2_CONFIG}" != "xno"
then
  AC_MSG_CHECKING([for libxml2 includes])
  LIBXML2_INCLUDES=`${LIBXML2_CONFIG} --cflags`
  AC_MSG_RESULT([${LIBXML2_INCLUDES}])
  AC_MSG_CHECKING([for libxml2 libraries])
  LIBXML2_LIBS=`${LIBXML2_CONFIG} --libs`
  AC_MSG_RESULT([${LIBXML2_LIBS}])
  CPPFLAGS="${CPPFLAGS} ${LIBXML2_INCLUDES}"
  AC_CHECK_HEADERS([libxml/parser.h libxml/tree.h])
  AC_CHECK_LIB([xml2],[xmlParseFile])
fi
if test "x${ac_cv_header_libxml_tree_h}" = "xyes" -a \
     "x${ac_cv_header_libxml_parser_h}" = "xyes" -a \
     "x${ac_cv_lib_xml2_xmlParseFile}" = "xyes"
then
  AC_DEFINE([LIBXML2_IS_USABLE],[1],[libxml2 on this system is usable])
fi
AC_SUBST([LIBXML2_INCLUDES])
AC_SUBST([LIBXML2_LIBS])
# APPLE LOCAL end: Get the libxml2 headers and libraries

# We need to link with -lw to get `wctype' on Solaris before Solaris
# 2.6.  Solaris 2.6 and beyond have this function in libc, and have a
# libw that some versions of the GNU linker cannot hanle (GNU ld 2.9.1
# is known to have this problem).  Therefore we avoid libw if we can.
AC_CHECK_FUNC([wctype],[],
  [AC_CHECK_LIB([w],[wctype])])

# Some systems (e.g. Solaris) have `gethostbyname' in libnsl.
AC_SEARCH_LIBS([gethostbyname],[nsl])

# Some systems (e.g. Solaris) have `socketpair' in libsocket.
AC_SEARCH_LIBS([socketpair],[socket])

# On HP/UX we may need libxpdl for dlgetmodinfo (used by solib-pa64.c).
AC_SEARCH_LIBS([dlgetmodinfo],[dl xpdl dlcompat dyld])

# For the TUI, we need enhanced curses functionality.
#
# FIXME: kettenis/20040905: We prefer ncurses over the vendor-supplied
# curses library because the latter might not provide all the
# functionality we need.  However, this leads to problems on systems
# where the linker searches /usr/local/lib, but the compiler does NOT
# search /usr/local/include, if ncurses is installed in /usr/local. A
# default installation of ncurses on alpha*-dec-osf* will lead to such
# a situation.
AC_SEARCH_LIBS([waddstr],[ncurses cursesX curses])

# Since GDB uses Readline, we need termcap functionality. In many
# cases this will be provided by the curses library, but some systems
# have a seperate termcap library, or no curses library at all.

case ${host_os} in
  cygwin*)
    if test -d ${srcdir}/libtermcap; then
      LIBS="../libtermcap/libtermcap.a ${LIBS}"
      ac_cv_search_tgetent="../libtermcap/libtermcap.a"
    fi ;;
  go32* | *djgpp*)
    ac_cv_search_tgetent="none required"
    ;;
  *mingw32*)	 
    ac_cv_search_tgetent="none required"
    CONFIG_OBS="${CONFIG_OBS} win32-termcap.o"
    ;;
  *hpux*)
    # FIXME: kettenis/20030102: We seem to prefer HP curses (Hcurses) over
    # Xcurses on HP-UX; see the `-D__HP_CURSES' in the relevant host
    # Makefile fragments. That is why we need to have `Hcurses' before
    # `curses'. I do NOT see why we should use HP curses if a more
    # standard curses is available, except that according to HP's
    # documentation one needs to compile `-D_XOPEN_SOURCE_EXTENDED' on
    # HP-UX 10.10 and 10.20.
    AC_SEARCH_LIBS([initscr],[ncurses Hcurses curses pdcurses],[
        # Check whether the wborder() function is provided by the curses
        # library just detected. In certain implementations such as
        # the HP/UX Hcurses for instance, this function is provided by an
        # additional library. So if we did not find this function inside
        # the curses library, try some alternate libraries we know might
        # provide it.
        AC_SEARCH_LIBS([wborder],[cur_colr],[],
            [AC_MSG_WARN([wborder function not found, disabling tui...])
             export enable_tui=no])
        ],[AC_MSG_WARN([no curses library found])])
    ;;
esac

if test -d ../readline; then
  # Keep these the same as the libraries checked by Readline:
  AC_SEARCH_LIBS([tgetent],[termcap tinfo curses ncurses])
else
  case ${host_os} in
    aix*)
      AC_SEARCH_LIBS([tgetent],[tinfo ncurses curses termcap])
      ;;
    *)
      AC_SEARCH_LIBS([tgetent],
                   [termcap tinfo ncurses Hcurses curses termlib terminfo])
      ;;
  esac
fi

if test "x${ac_cv_search_tgetent}" = "xno"; then
  AC_MSG_ERROR([no termcap library found])
fi

# Other libraries that autoscan added to configure.scan:
AC_CHECK_LIB([bfd],[bfd_set_reloc],[],[
  AC_CHECK_LIB([bfd],[main])
])
AC_CHECK_LIB([gnu],[gl_dummy_symbol],[],[
  AC_CHECK_LIB([gnu],[main],[],[])
])
AC_CHECK_LIB([iberty],[xmalloc])
AC_SEARCH_LIBS([gettext],
             [intl c gettext gnu asprintf gettextlib gettextpo gettextsrc])
AC_CHECK_LIB([intl],[gettext],[],[
  AC_CHECK_LIB([intl],[main],[],[])
])
AC_CHECK_LIB([opcodes],[op_cmp],[],[
  AC_CHECK_LIB([opcodes],[main])
])
AC_CHECK_LIB([readline],[rl_initialize])
AC_CHECK_LIB([sqlite3],[sqlite3_initialize])
AC_CHECK_LIB([z],[gzread])

# ------------------------- #
# Checks for header files.  #
# ------------------------- #

AC_MSG_CHECKING([headers])
AC_MSG_RESULT([ ])

AC_HEADER_DIRENT
AC_HEADER_STAT
AC_DEFUN([AC_REQUIRE_HEADER_STDC],[
  AC_REQUIRE([AC_HEADER_STDC])
])
AC_REQUIRE_HEADER_STDC
AC_HEADER_SYS_WAIT
AC_CHECK_HEADERS([nlist.h])
AC_CHECK_HEADERS([link.h],[],[],[
#if HAVE_SYS_TYPES_H
# include <sys/types.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "This conftest for link.h expects <sys/types.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_SYS_TYPES_H */
#if HAVE_NLIST_H
# include <nlist.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "This conftest for link.h expects <nlist.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_NLIST_H */
])
AC_CHECK_HEADERS([machine/reg.h])
AC_CHECK_HEADERS([poll.h sys/poll.h])
AC_CHECK_HEADERS([proc_service.h thread_db.h gnu/libc-version.h])
AC_CHECK_HEADERS([stddef.h])
AC_CHECK_HEADERS([stdio.h])
AC_CHECK_HEADERS([sys/fault.h])
AC_CHECK_HEADERS([sys/file.h])
AC_CHECK_HEADERS([sys/filio.h])
AC_CHECK_HEADERS([sys/ioctl.h])
if test "x${ac_cv_header_sys_procfs_h}" = "x"; then
  test -z "${ac_cv_header_sys_procfs_h}"
  AC_CHECK_HEADERS([sys/procfs.h])
fi
AC_CHECK_HEADERS([sys/proc.h],[],[],[
#if HAVE_SYS_PARAM_H
# include <sys/param.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "This conftest for sys/proc.h wants to include <sys/param.h>"
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_SYS_PARAM_H */
])
AC_CHECK_HEADERS([sys/ptrace.h ptrace.h])
AC_CHECK_HEADERS([sys/resource.h])
AC_CHECK_HEADERS([sys/reg.h sys/debugreg.h])
if test "x${ac_cv_header_sys_select_h}" = "x"; then
  test -z "${ac_cv_header_sys_select_h}"
  AC_CHECK_HEADERS([sys/select.h])
fi
AC_CHECK_HEADERS([sys/syscall.h])
AC_CHECK_HEADERS([sys/user.h],[],[],[
#if HAVE_SYS_PARAM_H
# include <sys/param.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "This conftest for sys/user.h wants to include <sys/param.h>"
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_SYS_PARAM_H */
])
if test "x${ac_cv_header_sys_wait_h}" = "x"; then
  test -z "${ac_cv_header_sys_wait_h}"
  AC_CHECK_HEADERS([sys/wait.h])
fi
AC_CHECK_HEADERS([wait.h])
AC_CHECK_HEADERS([termios.h termio.h sgtty.h])
AC_HEADER_TIOCGWINSZ
AC_SYS_POSIX_TERMIOS

# On Solaris 2.[789], we need to define _MSE_INT_H to avoid a clash
# between <widec.h> and <wchar.h> that would cause AC_CHECK_HEADERS to
# think that we do NOT have <curses.h> if we are using GCC.
case ${host_os} in
  solaris2.[[789]])
    if test "x${GCC}" = "xyes"; then
      AC_DEFINE([_MSE_INT_H],[1],
        [Define to 1 to avoid a clash between <widec.h> and <wchar.h> on
         Solaris 2.[789] when using GCC. ])
    fi ;;
esac
AC_CHECK_HEADERS([curses.h cursesX.h ncurses.h ncurses/ncurses.h])
AC_CHECK_HEADERS([ncurses/term.h])
AC_CHECK_HEADERS([term.h],[],[],[
#if HAVE_CURSES_H
# include <curses.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "This conftest for term.h expects <curses.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_CURSES_H */
])

# FIXME: kettenis/20030102: In most cases we include these
# unconditionally, so what is the point in checking these?
# ^How about "why not?"?
AC_CHECK_HEADERS([ctype.h time.h])

# Other headers that autoscan complains about:
AC_HEADER_STDBOOL dnl# calls AC_CHECK_HEADER_STDBOOL
AC_HEADER_TIME
if test -d ../intl; then
  if test -z "${CPPFLAGS}"; then
    export CPPFLAGS="-I../intl"
  else
    export CPPFLAGS="${CPPFLAGS} -I../intl"
  fi
elif test -d ../libintl; then
  if test -z "${CPPFLAGS}"; then
    export CPPFLAGS="-I../libintl"
  else
    export CPPFLAGS="${CPPFLAGS} -I../libintl"
  fi
fi
unset ac_cv_header_libintl_h
unset ac_cv_header_limits_h
unset ac_cv_header_locale_h
unset ac_cv_header_malloc_h
unset ac_cv_header_nl_types_h
unset ac_cv_header_sys_param_h
unset ac_cv_header_sys_socket_h
unset ac_cv_header_wchar_h
AC_CHECK_HEADERS([arpa/inet.h fcntl.h langinfo.h libintl.h limits.h \
                  locale.h mach/mach.h malloc.h malloc/malloc.h \
                  netdb.h netinet/in.h nl_types.h paths.h \
                  sys/param.h sys/socket.h sys/sysctl.h sys/time.h \
                  utmp.h wchar.h wctype.h])

# Other headers needed for things that I have ifdef-ed:
unset ac_cv_header_dlfcn_h
unset ac_cv_header_memory_h
unset ac_cv_header_sys_types_h
AC_CHECK_HEADERS([a.out.h dlfcn.h errno.h regex.h sys/types.h])
# check these two together:
AC_CHECK_HEADERS([stdarg.h varargs.h])
# even more header checks:
AC_HEADER_ASSERT
AC_CHECK_HEADERS([CoreFoundation/CoreFoundation.h assert.h \
                  architecture/byte_order.h bits/endian.h gettext.h \
                  libgen.h libgettext.h linux/byteorder/big_endian.h \
                  linux/byteorder/little_endian.h mach.h mach_error.h \
                  mach/kern_return.h mach/mach_error.h mach/mach_init.h \
                  mach/mach_port.h mach/message.h mach/port.h \
                  mach/thread_info.h mach/thread_status.h \
                  mach-o/loader.h mach-o/nlist.h memory.h pc.h pthread.h \
                  rpc/rpc.h servers/netname.h servers/netname_defs.h \
                  setjmp.h sys/uio.h sys/utsname.h])
# The gnulib subdirectory needs this:
gl_STDINT_H
AC_SUBST([STDINT_H])
gl_HEADER_STRING_H

# ------------------------- #
# Checks for declarations.  #
# ------------------------- #

AC_MSG_CHECKING([declarations])
AC_MSG_RESULT([ ])

AC_CHECK_DECLS([sys_siglist])
# just a plain check for sys_syglist might be wrong, so redo it
dnl# using the autoupdated version of AC\_DECL\_SYS\_SIGLIST
if test "x${ac_cv_have_decl_sys_siglist}" = "xno"; then
  unset ac_cv_have_decl_sys_siglist
  AC_CHECK_DECLS([sys_siglist],[],[],[
#include <signal.h>
/* NetBSD declares sys_siglist in unistd.h: */
#ifdef HAVE_UNISTD_H
# include <unistd.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__) && defined(__NetBSD__)
#  warning "This 2nd conftest for sys_siglist wants to include <unistd.h>"
# endif /* __GNUC__ && !__STRICT_ANSI__ && __NetBSD__ */
#endif /* HAVE_UNISTD_H */
  ])
fi
AC_CHECK_DECLS([free, malloc, realloc])
AC_CHECK_DECLS([strerror, strstr])
AC_CHECK_DECLS([getopt, snprintf, vsnprintf])
AC_CHECK_DECLS([UNW_SLT_NONE],[],[],[
#ifdef HAVE_LIBUNWIND_H
# include <libunwind.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "This conftest for UNW_SLT_NONE wants to include <libunwind.h>"
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_LIBUNWIND_H */
])
AC_CHECK_DECLS([UNW_SLT_MEMORY],[],[],[
#ifdef HAVE_LIBUNWIND_H
# include <libunwind.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "A conftest for UNW_SLT_MEMORY wants to include <libunwind.h>"
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_LIBUNWIND_H */
])
AC_CHECK_DECLS([UNW_SLT_REG],[],[],[
#ifdef HAVE_LIBUNWIND_H
# include <libunwind.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "This conftest for UNW_SLT_REG wants to include <libunwind.h>"
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_LIBUNWIND_H */
])
m4_ifdef([AM_LC_MESSAGES],[
unset am_cv_val_LC_MESSAGES
AM_LC_MESSAGES
],[:])
AC_CHECK_DECLS([basename],[],[],[
#ifdef HAVE_LIBGEN_H
# include <libgen.h>
#endif /* HAVE_LIBGEN_H */
])
AC_CHECK_DECLS([name_server_port],[],[],[
#ifdef HAVE_MACH_MACH_INIT_H
# include <mach/mach_init.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "This conftest for name_server_port wants <mach/mach_init.h>"
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_MACH_MACH_INIT_H */
])

# ----------------------- #
# Checks for structures.  #
# ----------------------- #

AC_MSG_CHECKING([structures])
AC_MSG_RESULT([ ])

AC_STRUCT_ST_BLOCKS
AC_CHECK_MEMBERS([struct stat.st_blksize])
AC_CHECK_MEMBERS([struct stat.st_rdev])
AC_STRUCT_TIMEZONE dnl# also does the "_TM" one

# ------------------ #
# Checks for types.  #
# ------------------ #

AC_MSG_CHECKING([types])
AC_MSG_RESULT([ ])

dnl# Begin what used to be AC\_TYPE\_SIGNAL
AC_CACHE_CHECK([return type of signal handlers],[ac_cv_type_signal],[
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <sys/types.h>
#include <signal.h>
]],[[
return *(signal (0, 0)) (0) == 1;
]])],[ac_cv_type_signal=int],[ac_cv_type_signal=void])
])
AC_DEFINE_UNQUOTED([RETSIGTYPE],[${ac_cv_type_signal}],
         [Define as the return type of signal handlers (`int' or `void').])
dnl# End what used to be AC\_TYPE\_SIGNAL
AC_CHECK_TYPES([socklen_t],[],[],
[#include <sys/types.h>
#include <sys/socket.h>
])
AC_CHECK_TYPES([uintptr_t],[],[],[
#ifdef HAVE_STDINT_H
# include <stdint.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "This conftest for uintptr_t expects <stdint.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_STDINT_H */
])
AC_CHECK_TYPES([ptrdiff_t])
AC_TYPE_INT8_T
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_INTMAX_T
AC_TYPE_MODE_T
AC_TYPE_PID_T
AC_TYPE_SSIZE_T
AC_TYPE_UID_T
AC_TYPE_UINT8_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINTMAX_T
AC_CHECK_TYPES([thread_t])
AC_CHECK_TYPES([regex_t],[],[],[
#ifdef HAVE_REGEX_H
# include <regex.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "This conftest for regex_t expects <regex.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_REGEX_H */
])
AC_CHECK_TYPES([unw_dyn_info_t],[],[],[
#ifdef HAVE_LIBUNWIND_H
# include <libunwind.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "The conftest for unw_dyn_info_t wants to include <libunwind.h>"
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_LIBUNWIND_H */
])
AC_CHECK_TYPES([unw_save_loc_t],[],[],[
#ifdef HAVE_LIBUNWIND_H
# include <libunwind.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "The conftest for unw_save_loc_t wants to include <libunwind.h>"
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_LIBUNWIND_H */
])
AC_CHECK_TYPES([unw_accessors_t],[],[],[
#ifdef HAVE_LIBUNWIND_H
# include <libunwind.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "A conftest for unw_accessors_t wants to include <libunwind.h>"
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_LIBUNWIND_H */
])
AC_CHECK_TYPES([kern_return_t],[],[],[
#ifdef HAVE_MACH_KERN_RETURN_H
# include <mach/kern_return.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "This conftest for kern_return_t wants <mach/kern_return.h>"
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_MACH_KERN_RETURN_H */
])
AC_CHECK_HEADERS([mach/mach_vm.h])
AC_CHECK_TYPES([mach_vm_address_t],[],[],[
#ifdef HAVE_MACH_MACH_VM_H
# include <mach/mach_vm.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "This conftest for mach_vm_address_t wants <mach/mach_vm.h>"
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_MACH_MACH_VM_H */
])
AC_CHECK_TYPES([mach_msg_header_t],[
  # Found the type, so check a member:
  AC_CHECK_MEMBER([mach_msg_header_t.msgh_seqno],[],[],[
#ifdef HAVE_MACH_MESSAGE_H
# include <mach/message.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "This conftest for member msgh_seqno wants <mach/message.h>"
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_MACH_MESSAGE_H */
  ])
  ],[],[
#ifdef HAVE_MACH_MESSAGE_H
# include <mach/message.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "This conftest for mach_msg_header_t wants <mach/message.h>"
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_MACH_MESSAGE_H */
])
AC_CHECK_TYPES([mach_port_t],[],[],[
#ifdef HAVE_MACH_PORT_H
# include <mach/port.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "This conftest for mach_port_t wants to include <mach/port.h>"
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_MACH_PORT_H */
])
dnl# The "x86_debug_state32_t" type is checked for in an APPLE_LOCAL
dnl# section below.

# ------------------------------------- #
# Checks for compiler characteristics.  #
# ------------------------------------- #

AC_MSG_CHECKING([compiler characteristics])
AC_MSG_RESULT([ ])

AC_PROG_GCC_TRADITIONAL
AC_C_BIGENDIAN([
  AC_CHECK_DECLS([__BIG_ENDIAN])
],[
  AC_CHECK_DECLS([__LITTLE_ENDIAN])
],[],[
  AC_CHECK_DECLS([__BIG_ENDIAN])
  AC_CHECK_DECLS([__LITTLE_ENDIAN])
])
AC_C_PROTOTYPES
AC_C_VOLATILE
AC_DEFUN([AC_C_DUPES],[
  AC_REQUIRE([AC_C_CONST])
  AC_REQUIRE([AC_C_INLINE])
  AC_REQUIRE([AC_C_RESTRICT])
])
AC_C_DUPES

#FIXME: This check can sometimes say that the compiler does not support the
# -fpascal-strings flag, even when it does actually support it:
AX_CHECK_COMPILE_FLAG([-fpascal-strings])

# ------------------------------ #
# Checks for library functions.  #
# ------------------------------ #

AC_MSG_CHECKING([functions])
AC_MSG_RESULT([ ])

if test -f Makefile.in; then
  if test ! -z "`grep -i alloca Makefile.in`"; then
    if test "x${ac_cv_working_alloca_h}" = "x"; then
      stat Makefile.in >/dev/null
      test -z "${ac_cv_working_alloca_h}"
      test -n "${ac_cv_func_alloca_works}" && unset ac_cv_func_alloca_works
      AC_FUNC_ALLOCA
    else
      test -n "${ac_cv_working_alloca_h}"
    fi
  fi
fi
AC_FUNC_CLOSEDIR_VOID
AC_FUNC_ERROR_AT_LINE
AC_FUNC_FORK
AC_FUNC_GETPGRP
AC_FUNC_LSTAT
AC_FUNC_MALLOC
AC_FUNC_MEMCMP
AM_WITH_DMALLOC
AC_FUNC_OBSTACK
AC_FUNC_REALLOC
AC_FUNC_SELECT_ARGTYPES
AC_FUNC_STAT
AC_FUNC_STRFTIME
AC_FUNC_STRNLEN
AC_FUNC_STRTOD
AC_FUNC_VPRINTF
AC_FUNC_WAIT3
AC_CHECK_FUNCS([canonicalize_file_name realpath])
AC_CHECK_FUNCS([getrusage])
AC_CHECK_FUNCS([getuid getgid])
AC_CHECK_FUNCS([pipe poll])
AC_CHECK_FUNCS([pread pread64])
AC_CHECK_FUNCS([pwrite pwrite64])
AC_CHECK_FUNCS([sbrk])
AC_CHECK_FUNCS([setpgid setpgrp setsid])
AC_CHECK_FUNCS([sigaction sigprocmask sigsetmask])
AC_CHECK_FUNCS([socketpair])
AC_CHECK_FUNCS([syscall])
AC_CHECK_FUNCS([ttrace])
AC_CHECK_FUNCS([wborder wresize])
if test "x${ac_cv_func_setlocale}" = "x"; then
  test -z "${ac_cv_func_setlocale}"
  AC_CHECK_FUNCS([setlocale])
fi
AC_CHECK_FUNCS([iconvlist libiconvlist btowc])
AC_CHECK_FUNCS([setrlimit getrlimit])
AC_CHECK_FUNCS([posix_madvise])
AC_CHECK_FUNCS([waitpid])
AC_CHECK_FUNCS([ptrace ptrace64])
m4_ifdef([AM_LANGINFO_CODESET],[
AM_LANGINFO_CODESET
],[
AC_CHECK_FUNCS([langinfo codeset])
])

# Other functions (that autoscan complains about missing):
unset ac_cv_func_getcwd
unset ac_cv_func_gethostbyname
unset ac_cv_func_strcasecmp
unset ac_cv_func_strchr
AC_CHECK_FUNCS([alarm assert atexit bzero dup2 exit fflush fprintf fwrite \
                getcwd gethostbyaddr gethostbyname gethostname getpass \
                gettimeofday inet_ntoa isascii mach_error_string \
                memchr memcmp memcpy memmove mempcpy memset mkdir \
                netname_look_up nl_langinfo pathconf pow \
                re_comp regcomp select socket strcasecmp strcat strchr \
                strcpy strcspn strdup strerror strlen strncasecmp \
                strncat strncmp strncpy strndup strpbrk strrchr \
                strspn strstr strtok strtol strtoul uname wcwidth])

# ----- #
# Other #
# ----- #

AC_MSG_CHECKING([miscellaneous things that require custom checks])
AC_MSG_RESULT([ ])

# Check the return and argument types of ptrace. No canned test for
# this, so roll our own.
gdb_ptrace_headers='
#if HAVE_SYS_TYPES_H
# include <sys/types.h>
#endif /* HAVE_SYS_TYPES_H */
#if HAVE_SYS_PTRACE_H
# include <sys/ptrace.h>
#endif /* HAVE_SYS_PTRACE_H */
#if HAVE_UNISTD_H
# include <unistd.h>
#endif /* HAVE_UNISTD_H */
'
# There is no point in checking if we do not have a prototype.
AC_CHECK_DECLS([ptrace],[],[
  : ${gdb_cv_func_ptrace_ret='int'}
  : ${gdb_cv_func_ptrace_args='int,int,long,long'}
],[${gdb_ptrace_headers}])
# Check return type:
AC_CACHE_CHECK([return type of ptrace],[gdb_cv_func_ptrace_ret],
  [AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
${gdb_ptrace_headers}
   ]],[[
extern int ptrace();
   ]])],[gdb_cv_func_ptrace_ret='int'],[gdb_cv_func_ptrace_ret='long'])
])
AC_DEFINE_UNQUOTED([PTRACE_TYPE_RET],[${gdb_cv_func_ptrace_ret}],
                   [Define as the return type of ptrace.])
# Check argument types:
AC_CACHE_CHECK([types of arguments for ptrace],[gdb_cv_func_ptrace_args],[
for gdb_arg1 in 'int' 'long'; do
 for gdb_arg2 in 'pid_t' 'int' 'long'; do
  for gdb_arg3 in 'int *' 'caddr_t' 'int' 'long'; do
   for gdb_arg4 in 'int' 'long'; do
     AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[${gdb_ptrace_headers}]],[[
extern ${gdb_cv_func_ptrace_ret}
  ptrace(${gdb_arg1}, ${gdb_arg2}, ${gdb_arg3}, ${gdb_arg4});
]])],[gdb_cv_func_ptrace_args="${gdb_arg1},${gdb_arg2},${gdb_arg3},${gdb_arg4}";
    break 4;],[])
    for gdb_arg5 in 'int *' 'int' 'long'; do
     AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[${gdb_ptrace_headers}]],[[
extern ${gdb_cv_func_ptrace_ret}
  ptrace(${gdb_arg1}, ${gdb_arg2}, ${gdb_arg3}, ${gdb_arg4}, ${gdb_arg5});
]])],[
gdb_cv_func_ptrace_args="${gdb_arg1},${gdb_arg2},${gdb_arg3},${gdb_arg4},${gdb_arg5}";
    break 5;],[])
    done
   done
  done
 done
done
# Provide a safe default value:
: ${gdb_cv_func_ptrace_args='int,int,long,long'}
])
ac_save_IFS=${IFS}; IFS=','
set dummy `echo "${gdb_cv_func_ptrace_args}" | sed 's/\*/\*/g'`
IFS=${ac_save_IFS}
shift
AC_DEFINE_UNQUOTED([PTRACE_TYPE_ARG3],$[3],
  [Define to the type of arg 3 for ptrace.])
if test -n "$[5]"; then
  AC_DEFINE_UNQUOTED([PTRACE_TYPE_ARG5],$[5],
    [Define to the type of arg 5 for ptrace.])
fi

dnl# AC_FUNC_SETPGRP does not work when cross compiling
dnl# Instead, assume we will have a prototype for setpgrp
dnl# if we are cross-compiling:
if test "x${cross_compiling}" = "xno"; then
  AC_FUNC_SETPGRP
else
  AC_CACHE_CHECK([whether setpgrp takes no argument],
                 [ac_cv_func_setpgrp_void],
    [AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <unistd.h>
]],[[
  if (setpgrp(1, 1) == -1) {
      exit(0);
  } else {
      exit(1);
  }
]])],[ac_cv_func_setpgrp_void=no],[ac_cv_func_setpgrp_void=yes])])
if test "x${ac_cv_func_setpgrp_void}" = "xyes"; then
  AC_DEFINE([SETPGRP_VOID],[1],
            [Define to 1 if setgrp takes no argument (i.e. void)])
fi
fi

# Check if sigsetjmp is available. Using AC_CHECK_FUNCS will not do
# since sigsetjmp might only be defined as a macro.
AC_CACHE_CHECK([for sigsetjmp],[gdb_cv_func_sigsetjmp],
[AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <setjmp.h>
]],[[sigjmp_buf env; while (! sigsetjmp (env, 1)) siglongjmp (env, 1);]])],[gdb_cv_func_sigsetjmp=yes],[gdb_cv_func_sigsetjmp=no])])
if test "x${gdb_cv_func_sigsetjmp}" = "xyes"; then
  AC_DEFINE([HAVE_SIGSETJMP],[1],[Define if sigsetjmp is available. ])
fi

# Assume we shall default to using the included libiberty regex:
gdb_use_included_regex=yes

# However, if the system regex is GNU regex, then default to *not*
# using the included regex.
AC_CACHE_CHECK([for GNU regex],
  [gdb_cv_have_gnu_regex],
  [AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[#include <gnu-versions.h>]],[[
#define REGEX_INTERFACE_VERSION 1
#if _GNU_REGEX_INTERFACE_VERSION != REGEX_INTERFACE_VERSION
# error "Version mismatch"
#endif]])],[gdb_cv_have_gnu_regex=yes],[gdb_cv_have_gnu_regex=no])])
if test "x${gdb_cv_have_gnu_regex}" = "xyes"; then
  gdb_use_included_regex=no
fi

AC_ARG_WITH([included-regex],
  [AS_HELP_STRING([--without-included-regex],[Do NOT use included regex; this is the default on systems with version 2 of the GNU C library (use with caution on other system)])],
  [gdb_with_regex=${withval}],
  [gdb_with_regex=${gdb_use_included_regex}])
if test "x${gdb_with_regex}" = "xyes"; then
  AC_DEFINE([USE_INCLUDED_REGEX],[1],
    [Define to 1 if the regex included in libiberty should be used.])
fi

# Check if <sys/proc.h> defines `struct thread' with a td_pcb member:
AC_CHECK_MEMBERS([struct thread.td_pcb],[],[],[
#include <sys/param.h>
#include <sys/proc.h>
])

# See if <sys/lwp.h> defines `struct lwp`.
AC_CACHE_CHECK([for struct lwp],[gdb_cv_struct_lwp],
[AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <sys/param.h>
#include <sys/lwp.h>
]],[[struct lwp l;]])],[gdb_cv_struct_lwp=yes],[gdb_cv_struct_lwp=no])])
if test "x${gdb_cv_struct_lwp}" = "xyes"; then
  AC_DEFINE([HAVE_STRUCT_LWP],[1],
            [Define to 1 if your system has struct lwp.])
fi

# See if <machine/reg.h> defines `struct reg'.
AC_CACHE_CHECK([for struct reg in machine/reg.h],[gdb_cv_struct_reg],
[AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <sys/types.h>
#include <machine/reg.h>
]],[[struct reg r;]])],[gdb_cv_struct_reg=yes],[gdb_cv_struct_reg=no])])
if test "x${gdb_cv_struct_reg}" = "xyes"; then
  AC_DEFINE([HAVE_STRUCT_REG],[1],
            [Define to 1 if your system has struct reg in <machine/reg.h>])
fi

# See if <machine/reg.h> supports the %fs and %gs i386 segment registers.
# Older i386 BSD's do NOT have the r_fs and r_gs members of `struct reg'.
AC_CHECK_MEMBERS([struct reg.r_fs, struct reg.r_gs],[],[],
                 [#include <machine/reg.h>])

# See if <sys/user.h> supports the %fs_base and %gs_bas amd64 segment
# registers. Older amd64 Linuxes do NOT have the fs_base and gs_base
# members of `struct user_regs_struct'.
AC_CHECK_MEMBERS([struct user_regs_struct.fs_base, struct user_regs_struct.gs_base],
		 [],[],[#include <sys/user.h>])

# See if <sys/ptrace.h> provides the PTRACE_GETREGS request.
AC_MSG_CHECKING([for PTRACE_GETREGS])
AC_CACHE_VAL([gdb_cv_have_ptrace_getregs],
[AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <sys/ptrace.h>
 ]],[[
PTRACE_GETREGS;
 ]])],[gdb_cv_have_ptrace_getregs=yes],[gdb_cv_have_ptrace_getregs=no])
])
AC_MSG_RESULT([${gdb_cv_have_ptrace_getregs}])
if test "x${gdb_cv_have_ptrace_getregs}" = "xyes"; then
  AC_DEFINE([HAVE_PTRACE_GETREGS],[1], 
  [Define if sys/ptrace.h defines the PTRACE_GETREGS request.])
fi

# See if <sys/ptrace.h> provides the PTRACE_GETFPXREGS request:
AC_MSG_CHECKING([for PTRACE_GETFPXREGS])
AC_CACHE_VAL([gdb_cv_have_ptrace_getfpxregs],
[AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <sys/ptrace.h>
 ]],[[
PTRACE_GETFPXREGS;
 ]])],[gdb_cv_have_ptrace_getfpxregs=yes],
      [gdb_cv_have_ptrace_getfpxregs=no])
])
AC_MSG_RESULT([${gdb_cv_have_ptrace_getfpxregs}])
if test "x${gdb_cv_have_ptrace_getfpxregs}" = "xyes"; then
  AC_DEFINE([HAVE_PTRACE_GETFPXREGS],[1],
  [Define if sys/ptrace.h defines the PTRACE_GETFPXREGS request.])
fi

# See if <sys/ptrace.h> provides the PT_GETDBREGS request.
AC_MSG_CHECKING([for PT_GETDBREGS])
AC_CACHE_VAL([gdb_cv_have_pt_getdbregs],
[AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <sys/types.h>
#include <sys/ptrace.h>
 ]],[[
PT_GETDBREGS;
 ]])],[gdb_cv_have_pt_getdbregs=yes],[gdb_cv_have_pt_getdbregs=no])
])
AC_MSG_RESULT([${gdb_cv_have_pt_getdbregs}])
if test "x${gdb_cv_have_pt_getdbregs}" = "xyes"; then
  AC_DEFINE([HAVE_PT_GETDBREGS],[1],
  [Define if sys/ptrace.h defines the PT_GETDBREGS request.])
fi

# See if <sys/ptrace.h> provides the PT_GETXMMREGS request:
AC_MSG_CHECKING([for PT_GETXMMREGS])
AC_CACHE_VAL([gdb_cv_have_pt_getxmmregs],
[AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <sys/types.h>
#include <sys/ptrace.h>
 ]],[[
PT_GETXMMREGS;
 ]])],[gdb_cv_have_pt_getxmmregs=yes],[gdb_cv_have_pt_getxmmregs=no])
])
AC_MSG_RESULT([${gdb_cv_have_pt_getxmmregs}])
if test "x${gdb_cv_have_pt_getxmmregs}" = "xyes"; then
  AC_DEFINE([HAVE_PT_GETXMMREGS],[1],
  [Define if sys/ptrace.h defines the PT_GETXMMREGS request.])
fi

# If we are configured native on GNU/Linux, work around problems with
# sys/procfs.h
# Also detect which type of /proc is in use, such as for Unixware
# or for Solaris.

if test "${target}" = "${host}"; then
  case "${host}" in
  i[[3456]]86-*-linux*)
	AC_DEFINE([START_INFERIOR_TRAPS_EXPECTED],[2],
        [Define on a GNU/Linux system to fix problems in sys/procfs.h])
	AC_DEFINE([sys_quotactl],[1],
        [Define on a GNU/Linux system to fix problems in sys/procfs.h])
	;;
  *-*-unixware* | *-*-sysv4.2* | *-*-sysv5* | *-*-interix* )
      AC_DEFINE([NEW_PROC_API],[1],
      [Define if you want to use new multi-fd /proc interface
       (replaces HAVE_MULTIPLE_PROC_FDS as well as other macros).])
      ;;
  *-*-solaris2.[[6789]] | *-*-solaris2.1[[0-9]])
      AC_DEFINE([NEW_PROC_API],[1],
      [Define if you want to use new multi-fd /proc interface
       (replaces HAVE_MULTIPLE_PROC_FDS as well as other macros).])
      ;;
  mips-sgi-irix5*)
      # Work around <sys/proc.h> needing _KMEMUSER problem on IRIX 5.
      AC_DEFINE([_KMEMUSER],[1],
      [Define to 1 so <sys/proc.h> gets a definition of anon_hdl. Works
       around a <sys/proc.h> problem on IRIX 5.])
      ;;
  esac
fi

# APPLE LOCAL begin again:
AC_MSG_NOTICE([doing some further APPLE_LOCAL checks])
case "${host}" in
  *-apple-darwin* | *-apple-macos*)
    gdb_cv_printf_has_long_long=yes
    gdb_cv_printf_has_long_double=yes
    gdb_cv_scanf_has_long_double=yes
  ;;
esac

case "${host}" in
  *-apple-darwin* | *-apple-macos*)
    AC_ARG_ENABLE(carbon-framework,
    [AS_HELP_STRING([--disable-carbon-framework],
                    [disable Carbon framework use even when available])],
    [case ${enableval} in
      yes | no) 
        enable_carbon_framework="${enableval}" ;;
      *)
        AC_MSG_ERROR([bad value '${enableval}' for --disable-carbon-framework]) ;;
    esac],
    [enable_carbon_framework=no])

    AC_MSG_CHECKING([for Carbon framework])
    if test x"${enable_carbon_framework}" = xyes; then
      if test -d "${SDKROOT}/System/Library/Frameworks/Carbon.framework"
      then
        AC_DEFINE([USE_CARBON_FRAMEWORK],[1],
           [Define if Carbon framework is available and we should use it.])
        CARBON_LIBS="-framework Carbon"
        AC_MSG_RESULT([yes])
      else
        AC_MSG_RESULT([no])
      fi
    else
      AC_MSG_RESULT([(disabled) no])
    fi
    ;;
esac

AC_SUBST([CARBON_LIBS])

case "${host}" in
  arm-apple-*)
  # We will NOT be using DebugSymbols.framework to locate dSYM files for
  # ARM yet.
  ;;
  *-apple-darwin* | *-apple-macos*)
    AC_ARG_ENABLE([debug-symbols-framework],
    [AS_HELP_STRING([--disable-debug-symbols-framework],
                    [disable DebugSymbols framework use even if present])],
    [case ${enableval} in
      yes | no) 
        enable_debug_symbols_framework="${enableval}" ;;
      *)
        AC_MSG_ERROR([bad value '${enableval}' for --disable-debug-symbols-framework])
        ;;
    esac],
    [enable_debug_symbols_framework=yes])

    AC_MSG_CHECKING([for DebugSymbols framework])
    if test x"${enable_debug_symbols_framework}" = x"yes"; then
      if test -d "/System/Library/PrivateFrameworks/DebugSymbols.framework"
      then
        AC_DEFINE([USE_DEBUG_SYMBOLS_FRAMEWORK],[1],
     [Define if DebugSymbols framework is available and we should use it.])
        DEBUG_SYMBOLS_LIBS="-F/System/Library/PrivateFrameworks -framework DebugSymbols"
        AC_MSG_RESULT([yes])
      else
        AC_MSG_RESULT([no])
      fi
    else
      AC_MSG_RESULT([(disabled) no])
    fi
  ;;
esac

AC_SUBST([DEBUG_SYMBOLS_LIBS])

# APPLE LOCAL: Enable MIG checking for macosx/macosx.defs.
MIG_CHECKING=-D__MigTypeCheck=1
AC_SUBST([MIG_CHECKING])
# APPLE LOCAL end

# APPLE LOCAL: See if we have the i386 debug register structs, i.e.
# whether we can use the i386 hardware registers for watchpoints or if
# we will need to use the old page-protection method.

AC_CHECK_TYPES([x86_debug_state32_t],[],[],
               [#include <mach/mach_types.h>])

AC_CHECK_TYPES([thread_identifier_info_data_t],[],[],
               [#include <mach/thread_info.h>])

AC_MSG_NOTICE([done with second batch of APPLE_LOCAL checks])

AC_CHECK_FUNCS([pthread_setname_np])

if test "x${ac_cv_header_sys_procfs_h}" = "xyes"; then
  BFD_HAVE_SYS_PROCFS_TYPE([pstatus_t])
  BFD_HAVE_SYS_PROCFS_TYPE([prrun_t])
  BFD_HAVE_SYS_PROCFS_TYPE([gregset_t])
  BFD_HAVE_SYS_PROCFS_TYPE([fpregset_t])
  BFD_HAVE_SYS_PROCFS_TYPE([prgregset_t])
  BFD_HAVE_SYS_PROCFS_TYPE([prfpregset_t])
  BFD_HAVE_SYS_PROCFS_TYPE([prgregset32_t])
  BFD_HAVE_SYS_PROCFS_TYPE([prfpregset32_t])
  BFD_HAVE_SYS_PROCFS_TYPE([lwpid_t])
  BFD_HAVE_SYS_PROCFS_TYPE([psaddr_t])
  BFD_HAVE_SYS_PROCFS_TYPE([prsysent_t])
  BFD_HAVE_SYS_PROCFS_TYPE([pr_sigset_t])
  BFD_HAVE_SYS_PROCFS_TYPE([pr_sigaction64_t])
  BFD_HAVE_SYS_PROCFS_TYPE([pr_siginfo64_t])


  dnl# Check for broken prfpregset_t type

  dnl# For Linux/i386, glibc 2.1.3 was released with a bogus
  dnl# prfpregset_t type (it was a typedef for the pointer to a struct
  dnl# instead of the struct itself). We detect this here, and work
  dnl# around it in gdb_proc_service.h.

  if test "x${bfd_cv_have_sys_procfs_type_prfpregset_t}" = "xyes"; then
    AC_MSG_CHECKING([whether prfpregset_t type is broken])
    AC_CACHE_VAL([gdb_cv_prfpregset_t_broken],
      [AC_RUN_IFELSE([AC_LANG_SOURCE([[#include <sys/procfs.h>
       int main(void)
       {
         if (sizeof(prfpregset_t) == sizeof(void *)) {
           return 1;
         }
         return 0;
       }
       ]])],[gdb_cv_prfpregset_t_broken=no],
            [gdb_cv_prfpregset_t_broken=yes],
            [gdb_cv_prfpregset_t_broken=yes])
    ])
    AC_MSG_RESULT([${gdb_cv_prfpregset_t_broken}])
    if test "x${gdb_cv_prfpregset_t_broken}" = "xyes"; then
      AC_DEFINE([PRFPREGSET_T_BROKEN],[1],
                [Define if the prfpregset_t type is broken.])
    fi
  fi

  dnl# Check for PIOCSET ioctl entry:

  AC_MSG_CHECKING([for PIOCSET ioctl entry in sys/procfs.h])
  AC_CACHE_VAL([gdb_cv_have_procfs_piocset],
  [AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <unistd.h>
#include <sys/types.h>
#include <sys/procfs.h>
  ]],[[
    int dummy;;
    dummy = ioctl(0, PIOCSET, &dummy);
  ]])],[gdb_cv_have_procfs_piocset=yes],[gdb_cv_have_procfs_piocset=no])])
  AC_MSG_RESULT([${gdb_cv_have_procfs_piocset}])
  if test "x${gdb_cv_have_procfs_piocset}" = "xyes"; then
    AC_DEFINE([HAVE_PROCFS_PIOCSET],[1],
              [Define if ioctl argument PIOCSET is available.])
  fi
else
  AC_MSG_NOTICE([skipping checks that require a working procfs])
fi

dnl# For native ports (host == target), check to see what kind of
dnl# legacy link.h support is needed.  (See solib-legacy.c.)
if test "x${host}" = "x${target}"; then
  dnl# Check for struct link_map with l_ members which are indicative
  dnl# of SVR4-like shared libraries:

  AC_MSG_CHECKING([for member l_addr in struct link_map])
  AC_CACHE_VAL([gdb_cv_have_struct_link_map_with_l_members],
    [AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <link.h>
     ]],[[
struct link_map lm; (void)lm.l_addr;
     ]])],[gdb_cv_have_struct_link_map_with_l_members=yes],
          [gdb_cv_have_struct_link_map_with_l_members=no])
  ])
  AC_MSG_RESULT([${gdb_cv_have_struct_link_map_with_l_members}])
  if test "x${gdb_cv_have_struct_link_map_with_l_members}" = "xyes"; then
    AC_DEFINE([HAVE_STRUCT_LINK_MAP_WITH_L_MEMBERS],[1],
    [Define if <link.h> exists and defines struct link_map which has
     members with an ``l_'' prefix.  (For Solaris, SVR4, and
     SVR4-like systems.)])
  fi

  dnl# Check for struct link_map with lm_ members which are indicative
  dnl# of SunOS-like shared libraries

  AC_MSG_CHECKING([for member lm_addr in struct link_map])
  AC_CACHE_VAL([gdb_cv_have_struct_link_map_with_lm_members],
    [AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <sys/types.h>
#include <link.h>
     ]],[[
struct link_map lm; (void)lm.lm_addr;
     ]])],[gdb_cv_have_struct_link_map_with_lm_members=yes],
          [gdb_cv_have_struct_link_map_with_lm_members=no])
  ])
  AC_MSG_RESULT([${gdb_cv_have_struct_link_map_with_lm_members}])
  if test "x${gdb_cv_have_struct_link_map_with_lm_members}" = "xyes"; then
    AC_DEFINE([HAVE_STRUCT_LINK_MAP_WITH_LM_MEMBERS],[1],
    [Define if <link.h> exists and defines struct link_map which has
     members with an ``lm_'' prefix.  (For SunOS.)])
  fi

  dnl# Check for struct so_map with som_ members which are found on 
  dnl# some *BSD systems.

  AC_MSG_CHECKING([for member som_addr in struct so_map])
  AC_CACHE_VAL([gdb_cv_have_struct_so_map_with_som_members],
    [AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <sys/types.h>
#ifdef HAVE_NLIST_H
# include <nlist.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "conftest for member som_addr in struct so_map wants <nlist.h>"
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_NLIST_H */
#include <link.h>
     ]],[[
struct so_map lm; (void) lm.som_addr;
     ]])],[gdb_cv_have_struct_so_map_with_som_members=yes],
          [gdb_cv_have_struct_so_map_with_som_members=no])
  ])
  AC_MSG_RESULT([${gdb_cv_have_struct_so_map_with_som_members}])
  if test "x${gdb_cv_have_struct_so_map_with_som_members}" = "xyes"; then
    AC_DEFINE([HAVE_STRUCT_SO_MAP_WITH_SOM_MEMBERS],[1],
    [Define if <link.h> exists and defines a struct so_map which has
     members with an ``som_'' prefix.  (Found on older *BSD systems.)])
  fi

  dnl# Check for struct link_map32 type, which allows a 64-bit Solaris
  dnl# debugger to debug a 32-bit Solaris app with 32-bit shared libraries.

  AC_MSG_CHECKING([for struct link_map32 in sys/link.h])
  AC_CACHE_VAL([gdb_cv_have_struct_link_map32], 
    [AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#define _SYSCALL32
#include <sys/link.h>
     ]],[[
struct link_map32 l;
     ]])],[gdb_cv_have_struct_link_map32=yes],
          [gdb_cv_have_struct_link_map32=no])
  ])
  AC_MSG_RESULT([${gdb_cv_have_struct_link_map32}])
  if test "x${gdb_cv_have_struct_link_map32}" = "xyes"; then
    AC_DEFINE([HAVE_STRUCT_LINK_MAP32],[1],
    [Define if <sys/link.h> has struct link_map32])
    AC_DEFINE([_SYSCALL32],[1],
    [Define if <sys/link.h> has link_map32 (solaris sparc-64 target)])
  fi
else
  AC_MSG_NOTICE([skipping some checks that require a native build])
fi

# Check if the compiler supports the `long long' type.

AC_CACHE_CHECK([for long long support in compiler],[gdb_cv_c_long_long],
               [AC_COMPILE_IFELSE([AC_LANG_PROGRAM(
[[extern long long foo;]],
[[switch (foo & 2) { case 0: return 1; }]])],
                                  [gdb_cv_c_long_long=yes],
                                  [gdb_cv_c_long_long=no])])
if test "x${gdb_cv_c_long_long}" = "xyes"; then
  AC_DEFINE([CC_HAS_LONG_LONG],[1],
            [Define to 1 if the compiler supports long long.])
elif "x${gdb_cv_c_long_long}" != "xyes"; then
  # libdecnumber requires long long.
  AC_MSG_WARN([Compiler should support long long for using GDB])
fi

# Check if the compiler and runtime support printing long longs:
AC_CACHE_CHECK([for long long support in printf],
               [gdb_cv_printf_has_long_long],
               [AC_RUN_IFELSE([AC_LANG_PROGRAM([AC_INCLUDES_DEFAULT],
[[char buf[32];
  long long l = 0;
  l = (l << 16) + 0x0123;
  l = (l << 16) + 0x4567;
  l = (l << 16) + 0x89ab;
  l = (l << 16) + 0xcdef;
  sprintf(buf, "0x%016llx", l);
  return (strcmp("0x0123456789abcdef", buf));]])],
                              [gdb_cv_printf_has_long_long=yes],
                              [gdb_cv_printf_has_long_long=no],
                              [gdb_cv_printf_has_long_long=no])])
# (note: Apple forces this value to be cached ahead of time; do NOT worry
# about it printing that it is cached here)
if test "x${gdb_cv_printf_has_long_long}" = "xyes"; then
  AC_DEFINE([PRINTF_HAS_LONG_LONG],[1],
            [Define to 1 if the "%ll" format works to print long longs.])
fi

# Check if the compiler and runtime support printing decfloats.

AC_CACHE_CHECK([for decfloat support in printf],
               [gdb_cv_printf_has_decfloat],
               [AC_RUN_IFELSE([AC_LANG_PROGRAM([AC_INCLUDES_DEFAULT],
[[char buf[64];
  _Decimal32 d32 = 1.2345df;
  _Decimal64 d64 = 1.2345dd;
  _Decimal128 d128 = 1.2345dl;
  sprintf(buf, "Decimal32: %H\nDecimal64: %D\nDecimal128: %DD", d32, d64, d128);
  return (strcmp("Decimal32: 1.2345\nDecimal64: 1.2345\nDecimal128: 1.2345", buf));]])],
                              [gdb_cv_printf_has_decfloat=yes],
                              [gdb_cv_printf_has_decfloat=no],
                              [gdb_cv_printf_has_decfloat=no])])
if test "x${gdb_cv_printf_has_decfloat}" = "xyes"; then
  AC_DEFINE([PRINTF_HAS_DECFLOAT],[1],
    [Define to 1 if the "%H, %D and %DD" formats work to print decfloats.])
fi

# Check if the compiler supports the `long double' type. We cannot use
# AC\_C\_LONG\_DOUBLE because that one does additional checks on the
# constants defined in <float.h> that fail on some systems,
# e.g. FreeBSD/i386 4.7 and OpenBSD/i386 3.6.

AC_CACHE_CHECK([for long double support in compiler],
               [gdb_cv_c_long_double],
               [AC_COMPILE_IFELSE([AC_LANG_SOURCE([[long double foo;]])],
                                  [gdb_cv_c_long_double=yes],
                                  [gdb_cv_c_long_double=no])])
if test "x${gdb_cv_c_long_double}" = "xyes"; then
  AC_DEFINE([HAVE_LONG_DOUBLE],[1],
            [Define to 1 if the compiler supports long double.])
fi

# Check if the compiler and runtime support printing long doubles.

AC_CACHE_CHECK([for long double support in printf],
               [gdb_cv_printf_has_long_double],
               [AC_RUN_IFELSE([AC_LANG_PROGRAM([AC_INCLUDES_DEFAULT],
[[char buf[16];
  long double f = 3.141592653;
  sprintf (buf, "%Lg", f);
  return (strncmp ("3.14159", buf, 7));]])],
                              [gdb_cv_printf_has_long_double=yes],
                              [gdb_cv_printf_has_long_double=no],
                              [gdb_cv_printf_has_long_double=no])])
# (note: Apple forces this value to be cached ahead of time; do NOT worry
# about it printing that it is cached here)
if test "x${gdb_cv_printf_has_long_double}" = "xyes"; then
  AC_DEFINE([PRINTF_HAS_LONG_DOUBLE],[1],
            [Define to 1 if the "%Lg" format works to print long doubles.])
fi

# Check if the compiler and runtime support scanning long doubles.

AC_CACHE_CHECK([for long double support in scanf], 
               [gdb_cv_scanf_has_long_double],
               [AC_RUN_IFELSE([AC_LANG_PROGRAM(
[[#include <stdio.h>]],
[[char *buf = "3.141592653";
  long double f = 0;
  sscanf (buf, "%Lg", &f);
  return !(f > 3.14159 && f < 3.14160);]])],
                              [gdb_cv_scanf_has_long_double=yes],
                              [gdb_cv_scanf_has_long_double=no],
                              [gdb_cv_scanf_has_long_double=no])])
# (note: Apple forces this value to be cached ahead of time; do NOT worry
# about it printing that it is cached here)
if test "x${gdb_cv_scanf_has_long_double}" = "xyes"; then
  AC_DEFINE([SCANF_HAS_LONG_DOUBLE],[1],
            [Define to 1 if the "%Lg" format works to scan long doubles.])
fi

case ${host_os} in
aix*)
  AC_CACHE_CHECK([for -bbigtoc option],[gdb_cv_bigtoc],[
    SAVE_LDFLAGS=${LDFLAGS}

    case ${GCC} in
    yes) gdb_cv_bigtoc=-Wl,-bbigtoc ;;
    *) gdb_cv_bigtoc=-bbigtoc ;;
    esac

    # why escape the space instead of just using quotes?
    LDFLAGS=${LDFLAGS}\ ${gdb_cv_bigtoc}
    AC_LINK_IFELSE([AC_LANG_PROGRAM([[]],[[int i;]])],[],[gdb_cv_bigtoc=])
    LDFLAGS="${SAVE_LDFLAGS}"
  ])
  CONFIG_LDFLAGS="${CONFIG_LDFLAGS} ${gdb_cv_bigtoc}"
  ;;
esac

AC_MSG_CHECKING([for the dynamic export flag])
dynamic_list=false
if test "x${gdb_native}" = "xyes"; then
   # The dynamically loaded libthread_db needs access to symbols in the gdb
   # executable. Older GNU ld supports --export-dynamic but --dynamic-list
   # may not be supported there.
   old_LDFLAGS="${LDFLAGS}"
   # Older GNU ld supports --export-dynamic but --dynamic-list it does not.
   RDYNAMIC="-Wl,--dynamic-list=${srcdir}/proc-service.list"
   LDFLAGS="${LDFLAGS} ${RDYNAMIC}"
   if test "x${have_libpython}" = "xno"; then
     AC_LINK_IFELSE([AC_LANG_PROGRAM([[]],[[]])],[dynamic_list=true],[])
   elif test -n "${PYTHON_CFLAGS}" && test -n "${PYTHON_LIBS}" && test -n "${have_libpython}"; then
     # Workaround http://bugs.python.org/issue4434 where static
     # libpythonX.Y.a would get its symbols required for
     # pythonX.Y/lib-dynload/*.so modules hidden by -Wl,--dynamic-list.
     # Problem does not happen for the recommended libpythonX.Y.so linkage.
     old_CFLAGS="${CFLAGS}"
     CFLAGS="${CFLAGS} ${PYTHON_CFLAGS}"
     old_LIBS="${LIBS}"
     LIBS="${LIBS} ${PYTHON_LIBS}"
     AC_RUN_IFELSE([
       AC_LANG_PROGRAM([#include "]${have_libpython}[/Python.h"],[[
          int err;
          Py_Initialize();
          err = PyRun_SimpleString("import itertools\n");
          Py_Finalize();
          return ((err == 0) ? 0 : 1);]])],
       [dynamic_list=true],[],[true])
     LIBS="${old_LIBS}"
     CFLAGS="${old_CFLAGS}"
   fi
   LDFLAGS="${old_LDFLAGS}"
fi
if ${dynamic_list}; then
  found="-Wl,--dynamic-list"
  RDYNAMIC='-Wl,--dynamic-list=$(srcdir)/proc-service.list'
else
  found="-rdynamic"
  RDYNAMIC="-rdynamic"
fi
AC_SUBST([RDYNAMIC])
AC_MSG_RESULT([${found}])


dnl# For certain native configurations, we need to check whether thread
dnl# support can be built in or not.
dnl#
dnl# Note that we only want this if we are both native (host == target),
dnl# and not doing a canadian cross build (build == host).

if test "x${build}" = "x${host}" -a "x${host}" = "x${target}"; then
   case ${host_os} in
   hpux*)
      AC_MSG_CHECKING([for HPUX/OSF thread support])
      if test -f /usr/include/dce/cma_config.h; then
         if test "x${GCC}" = "xyes"; then
            AC_MSG_RESULT([yes])
            AC_DEFINE([HAVE_HPUX_THREAD_SUPPORT],[1],
	              [Define if you have HPUX threads])
            CONFIG_OBS="${CONFIG_OBS} hpux-thread.o"
            CONFIG_SRCS="${CONFIG_SRCS} hpux-thread.c"
         else
            AC_MSG_RESULT([no (suppressed because you are not using GCC)])
         fi
      else
         AC_MSG_RESULT([no])
      fi
      ;;
   solaris*)
      # See if thread_db library is around for Solaris thread debugging.
      # Note that we must explicitly test for version 1 of the library
      # because version 0 (present on Solaris 2.4 or earlier) does NOT have
      # the same API.
      AC_MSG_CHECKING([for Solaris thread debugging library])
      if test -f /usr/lib/libthread_db.so.1 ; then
         AC_MSG_RESULT([yes])
         AC_DEFINE([HAVE_THREAD_DB_LIB],[1],
                   [Define if using Solaris thread debugging.])
         CONFIG_OBS="${CONFIG_OBS} sol-thread.o"
         CONFIG_SRCS="${CONFIG_SRCS} sol-thread.c"
         AC_CHECK_LIB([dl],[dlopen])
         if test "x${GCC}" = "xyes" ; then
            # The GNU linker requires the -export-dynamic option to make
            # all symbols visible in the dynamic symbol table.
            hold_ldflags=${LDFLAGS}
            AC_MSG_CHECKING([for the ld -export-dynamic flag])
            LDFLAGS="${LDFLAGS} -Wl,-export-dynamic"
            AC_LINK_IFELSE([AC_LANG_PROGRAM([[]],[[int i;]])],
                           [found=yes],[found=no])
            LDFLAGS=${hold_ldflags}
            AC_MSG_RESULT([${found}])
            if test "x${found}" = "xyes"; then
               CONFIG_LDFLAGS="${CONFIG_LDFLAGS} -Wl,-export-dynamic"
            fi
         fi
	 # Sun randomly tweaked the prototypes in <proc_service.h>
	 # at one point.
	 AC_MSG_CHECKING([if <proc_service.h> is old])
	 AC_CACHE_VAL([gdb_cv_proc_service_is_old],[
	    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
		#include <proc_service.h>
		ps_err_e ps_pdwrite
		    (struct ps_prochandle*, psaddr_t, const void*, size_t);
	    ]],[[]])],[gdb_cv_proc_service_is_old=no],
                      [gdb_cv_proc_service_is_old=yes])
	 ])
	 AC_MSG_RESULT([${gdb_cv_proc_service_is_old}])
	 if test "x${gdb_cv_proc_service_is_old}" = "xyes"; then
	    AC_DEFINE([PROC_SERVICE_IS_OLD],[1],
            [Define if <proc_service.h> on solaris uses int instead of
             size_t, and assorted other type changes.])
	 fi
      else
         AC_MSG_RESULT([no])
      fi
      ;;
   aix*)
      AC_MSG_CHECKING([for AiX thread debugging library])
      AC_CACHE_VAL([gdb_cv_have_aix_thread_debug],
                   [AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <sys/pthdebug.h>
                    ]],[[
#ifndef PTHDB_VERSION_3
# error thread debugging library unavailable
#endif /* !PTHDB_VERSION_3 */
                    ]])],[gdb_cv_have_aix_thread_debug=yes],
                         [gdb_cv_have_aix_thread_debug=no])
      ])
      AC_MSG_RESULT([${gdb_cv_have_aix_thread_debug}])
      if test ${gdb_cv_have_aix_thread_debug} = yes; then
         CONFIG_SRCS="${CONFIG_SRCS} aix-thread.c"
         CONFIG_OBS="${CONFIG_OBS} aix-thread.o"
         CONFIG_LDFLAGS="${CONFIG_LDFLAGS} -lpthdebug"
      fi
      ;;
   esac
   AC_SUBST([CONFIG_LDFLAGS])
else
   AC_MSG_NOTICE([skipping some checks that require a native, non-Canadian build])
fi

dnl# See if we have a thread_db header file that has TD_NOTALLOC.
if test "x${ac_cv_header_thread_db_h}" = "xyes"; then
   AC_CACHE_CHECK([whether <thread_db.h> has TD_NOTALLOC],
                  [gdb_cv_thread_db_h_has_td_notalloc],
     [AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <thread_db.h>
      ]],[[
int i = TD_NOTALLOC;
      ]])],[gdb_cv_thread_db_h_has_td_notalloc=yes],
           [gdb_cv_thread_db_h_has_td_notalloc=no])
   ])
   AC_CACHE_CHECK([whether <thread_db.h> has TD_VERSION],
                  [gdb_cv_thread_db_h_has_td_version],
     [AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <thread_db.h>
      ]],[[
int i = TD_VERSION;
      ]])],[gdb_cv_thread_db_h_has_td_version=yes],
           [gdb_cv_thread_db_h_has_td_version=no])
   ])
   AC_CACHE_CHECK([whether <thread_db.h> has TD_NOTLS],
                  [gdb_cv_thread_db_h_has_td_notls],
     [AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <thread_db.h>
      ]],[[
int i = TD_NOTLS;
      ]])],[gdb_cv_thread_db_h_has_td_notls=yes],
           [gdb_cv_thread_db_h_has_td_notls=no])
   ])
else
   AC_MSG_NOTICE([skipping some checks that require a thread_db header])
fi
if test "x${gdb_cv_thread_db_h_has_td_notalloc}" = "xyes"; then
  AC_DEFINE([THREAD_DB_HAS_TD_NOTALLOC],[1],
            [Define if <thread_db.h> has the TD_NOTALLOC error code.])
fi
if test "x${gdb_cv_thread_db_h_has_td_version}" = "xyes"; then
  AC_DEFINE([THREAD_DB_HAS_TD_VERSION],[1],
            [Define if <thread_db.h> has the TD_VERSION error code.])
fi
if test "x${gdb_cv_thread_db_h_has_td_notls}" = "xyes"; then
  AC_DEFINE([THREAD_DB_HAS_TD_NOTLS],[1],
            [Define if <thread_db.h> has the TD_NOTLS error code.])
fi

dnl# See if we have a sys/syscall header file that has __NR_tkill.
if test "x${ac_cv_header_sys_syscall_h}" = "xyes"; then
   AC_CACHE_CHECK([whether <sys/syscall.h> has __NR_tkill],
                  [gdb_cv_sys_syscall_h_has_tkill],
     [AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <sys/syscall.h>
      ]],[[
int i = __NR_tkill;
      ]])],[gdb_cv_sys_syscall_h_has_tkill=yes],
           [gdb_cv_sys_syscall_h_has_tkill=no])
   ])
fi
dnl# See if we can issue tkill syscall.
if test "x${gdb_cv_sys_syscall_h_has_tkill}" = "xyes" && test "x${ac_cv_func_syscall}" = "xyes"; then
  AC_DEFINE([HAVE_TKILL_SYSCALL],[1],
            [Define if you support the tkill syscall.])
fi

dnl# Check if we can disable the virtual address space randomization.
dnl# The functionality of setarch -R.
AC_CHECK_DECLS([ADDR_NO_RANDOMIZE],[],[],[#include <sys/personality.h>])
define([PERSONALITY_TEST],[AC_LANG_PROGRAM([#include <sys/personality.h>],[
#      if !HAVE_DECL_ADDR_NO_RANDOMIZE
#       define ADDR_NO_RANDOMIZE 0x0040000
#      endif /* !HAVE_DECL_ADDR_NO_RANDOMIZE */
       /* Test the flag could be set and stays set.  */
       personality (personality (0xffffffff) | ADDR_NO_RANDOMIZE);
       if (!(personality (personality (0xffffffff)) & ADDR_NO_RANDOMIZE))
	   return 1])])
AC_RUN_IFELSE([PERSONALITY_TEST],
	      [have_personality=true],
	      [have_personality=false],
	      [AC_LINK_IFELSE([PERSONALITY_TEST],
			      [have_personality=true],
			      [have_personality=false])])
if ${have_personality}
then
    AC_DEFINE([HAVE_PERSONALITY],[1],
	      [Define if you support the personality syscall.])
fi

dnl# Set the host's .gdbinit filename:
case ${host_os} in
  go32* | *djgpp*)
    gdbinit=gdb.ini
    ;;
  *)
    gdbinit=.gdbinit
    ;;
esac
AC_DEFINE_UNQUOTED([GDBINIT],["${gdbinit}"],[The .gdbinit filename.])

dnl# Handle optional features that can be enabled.

AC_ARG_WITH([sysroot],
[AS_HELP_STRING([--with-sysroot[=DIR]],
                [Search for usr/lib et al within DIR.])],
[
 case ${with_sysroot} in
 yes) TARGET_SYSTEM_ROOT='${exec_prefix}/${target_alias}/sys-root' ;;
 *) TARGET_SYSTEM_ROOT=${with_sysroot} ;;
 esac

 TARGET_SYSTEM_ROOT_DEFINE='-DTARGET_SYSTEM_ROOT=\"$(TARGET_SYSTEM_ROOT)\"'

 if test "x${exec_prefix}" = "xNONE"; then
  if test "x${prefix}" = "xNONE"; then
   test_prefix=/usr/local
  else
   test_prefix=${prefix}
  fi
 else
  test_prefix=${exec_prefix}
 fi
 case ${TARGET_SYSTEM_ROOT} in
 "${test_prefix}"|"${test_prefix}/"*|\
 '${exec_prefix}'|'${exec_prefix}/'*)
   t="${TARGET_SYSTEM_ROOT_DEFINE} -DTARGET_SYSTEM_ROOT_RELOCATABLE"
   TARGET_SYSTEM_ROOT_DEFINE="${t}"
   ;;
 esac
],[
 TARGET_SYSTEM_ROOT=""
 TARGET_SYSTEM_ROOT_DEFINE='-DTARGET_SYSTEM_ROOT=\"\"'
])
AC_SUBST([TARGET_SYSTEM_ROOT])
AC_SUBST([TARGET_SYSTEM_ROOT_DEFINE])

# NOTE: Do NOT add -Wall or -Wunused, they both include
# -Wunused-parameter which reports bogus warnings.
# NOTE: If you add to this list, remember to update
# gdb/doc/gdbint.texinfo.
build_warnings="-Wimplicit -Wreturn-type -Wcomment -Wtrigraphs \
-Wparentheses -Wdeclaration-after-statement -Wpointer-arith \
-Wpointer-sign \
-Wno-unused -Wunused-value -Wunused-label -Wunused-function \
-Wunused-macros -Wundef \
-Wempty-body -Wmissing-paramater-type \
-Wno-format-security -Wno-format \
-Wold-style-declaration -Wold-style-definition"

case "${host}" in
  *-apple-darwin* | *-apple-macos*)
    # -Wmost is APPLE_LOCAL, as are -Wextra-tokens and -Wnewline-eof
    build_warnings="${build_warnings} -Wmost -Wformat-nonliteral -Wextra-tokens -Wnewline-eof -std=gnu99"
  ;;
esac

# GCC supports -Wuninitialized only with -O or -On, n != 0.
# and also -Winit-self only with -Wuninitialized
if test x${CFLAGS+set} = xset; then
  case "${CFLAGS}" in
    *"-O0"* ) ;;
    *"-O"* )
      build_warnings="${build_warnings} -Wuninitialized -Winit-self"
    ;;
  esac
else
  build_warnings="${build_warnings} -Wuninitialized -Winit-self"
fi

# Up for debate: -Wswitch -Wcomment -trigraphs -Wtrigraphs
# -Wunused-function -Wunused-variable -Wunused-value
# -Wchar-subscripts -Wtraditional -Wshadow -Wcast-qual
# -Wcast-align -Wwrite-strings -Wconversion -Wstrict-prototypes
# -Wmissing-prototypes -Wmissing-declarations -Wredundant-decls
# -Woverloaded-virtual -Winline -Wformat -Werror
AC_ARG_ENABLE([build-warnings],
[AS_HELP_STRING([--enable-build-warnings],
                [Enable build-time compiler warnings if gcc is used])],
[case "${enableval}" in
  yes)	;;
  no)	build_warnings="-w";;
  ,*)   t=`echo "${enableval}" | sed -e "s/,/ /g"`
        build_warnings="${build_warnings} ${t}";;
  *,)   t=`echo "${enableval}" | sed -e "s/,/ /g"`
        build_warnings="${t} ${build_warnings}";;
  *)    build_warnings=`echo "${enableval}" | sed -e "s/,/ /g"`;;
esac
if test x"${silent}" != x"yes" && test x"${build_warnings}" != x""; then
  echo "Setting compiler warning flags = ${build_warnings}" 6>&1
fi])dnl
AC_ARG_ENABLE([gdb-build-warnings],
[AS_HELP_STRING([--enable-gdb-build-warnings],
       [Enable GDB-specific build-time compiler warnings if gcc is used])],
[case "${enableval}" in
  yes)	;;
  no)	build_warnings="-w";;
  ,*)   t=`echo "${enableval}" | sed -e "s/,/ /g"`
        build_warnings="${build_warnings} ${t}";;
  *,)   t=`echo "${enableval}" | sed -e "s/,/ /g"`
        build_warnings="${t} ${build_warnings}";;
  *)    build_warnings=`echo "${enableval}" | sed -e "s/,/ /g"`;;
esac
if test x"${silent}" != x"yes" && test x"${build_warnings}" != x""; then
  echo "Setting GDB specific compiler warning flags = ${build_warnings}" 6>&1
fi])dnl
WARN_CFLAGS=""
WERROR_CFLAGS=""
if test "x${build_warnings}" != x -a "x${GCC}" = "xyes"
then
    AC_MSG_CHECKING([compiler warning flags])
    # Separate out the -Werror flag as some files just cannot be
    # compiled with it enabled.
    for w in ${build_warnings}; do
	case ${w} in
	*) # Check that GCC accepts it
	    saved_CFLAGS="${CFLAGS}"
	    CFLAGS="${CFLAGS} ${w}"
	    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[]],[[]])],
                              [WARN_CFLAGS="${WARN_CFLAGS} ${w}"],[])
	    CFLAGS="${saved_CFLAGS}"
	esac
    done
    AC_MSG_RESULT([${WARN_CFLAGS}${WERROR_CFLAGS}])
fi
AC_SUBST([WARN_CFLAGS])
AC_SUBST([WERROR_CFLAGS])

MMALLOC_CFLAGS=""
MMALLOC=""
AC_SUBST([MMALLOC_CFLAGS])
AC_SUBST([MMALLOC])

# APPLE LOCAL: We (used to) need to use mmalloc all the time on our platform.
case "${host}" in
  *-apple-darwin* | *-apple-macos*)
    mmalloc_default_value="true" ;;
  *)
    mmalloc_default_value="false" ;;
esac

AC_ARG_WITH([mmalloc],
[AS_HELP_STRING([--with-mmalloc],[Use memory mapped malloc package])],
[case "${withval}" in
  yes)  want_mmalloc=true ;;
  no)   want_mmalloc=false;;
  *)    AC_MSG_ERROR([bad value ${withval} for GDB with-mmalloc option]) ;;
esac],[want_mmalloc=${mmalloc_default_value}])dnl

if test "x${want_mmalloc}" = "xtrue"; then
   AC_DEFINE([USE_MMALLOC],[1],[Use mmalloc])
   AC_DEFINE([MMCHECK_FORCE],[1],[Force mmcheck])
   MMALLOC_CFLAGS="-I${srcdir}/../mmalloc -I../mmalloc"
   MMALLOC='../mmalloc/libmmalloc.a'
fi

# In the Cygwin environment, we need some additional flags.
AC_CACHE_CHECK([for cygwin],[gdb_cv_os_cygwin],
[AC_EGREP_CPP([you_lose],[
#if defined (__CYGWIN__) || defined (__CYGWIN32__)
you_lose
#endif],[gdb_cv_os_cygwin=yes],[gdb_cv_os_cygwin=no])])


dnl# Figure out which of the many generic ser-*.c files the _host_
dnl# supports.
SER_HARDWIRE="ser-base.o ser-unix.o ser-pipe.o ser-tcp.o"
case ${host} in
  *go32* ) SER_HARDWIRE=ser-go32.o ;;
  *djgpp* ) SER_HARDWIRE=ser-go32.o ;;
  *mingw32*) SER_HARDWIRE="ser-base.o ser-tcp.o" ;;
esac
AC_SUBST([SER_HARDWIRE])

# libreadline needs libuser32.a in a cygwin environment
WIN32LIBS=""
if test "x${gdb_cv_os_cygwin}" = "xyes"; then
    WIN32LIBS="-luser32"
    case "${target}" in
	*cygwin*) WIN32LIBS="${WIN32LIBS} -limagehlp"
	;;
    esac
fi

# The ser-tcp.c module requires sockets.
case ${host} in
  *mingw32*)
    AC_DEFINE([USE_WIN32API],[1],
              [Define if we should use the Windows API, instead of the 
	       POSIX API. On Windows, we use the Windows API when 
	       building for MinGW, but the POSIX API when building 
	       for Cygwin.])
    WIN32LIBS="${WIN32LIBS} -lws2_32"
    ;;
esac	    
AC_SUBST([WIN32LIBS])

if test "x${gdb_host_obs}" != "x"; then
  # Add any host-specific objects to GDB.
  test -n "${gdb_host_obs}" && export CONFIG_OBS="${CONFIG_OBS} ${gdb_host_obs}"
fi

LIBGUI="../libgui/src/libgui.a"
GUI_CFLAGS_X="-I${srcdir}/../libgui/src"
AC_SUBST([LIBGUI])
AC_SUBST([GUI_CFLAGS_X])

WIN32LDAPP=""
AC_SUBST([WIN32LIBS])
AC_SUBST([WIN32LDAPP])

case "${host}" in
*-*-cygwin* | *-*-mingw* )
    configdir="win"
    ;;
*)
    configdir="unix"
    ;;
esac

GDBTKLIBS=""
if test "x${enable_gdbtk}" = "xyes"; then
    # Gdbtk must have an absolute path to srcdir in order to run
    # properly when not installed.
    here=`pwd`
    cd ${srcdir}
    GDBTK_SRC_DIR=`pwd`
    cd ${here}

    CY_AC_PATH_TCLCONFIG
    if test -z "${no_tcl}"; then
	CY_AC_LOAD_TCLCONFIG
	CY_AC_PATH_TKCONFIG

	# now look for Tcl library stuff:
	tcldir="../tcl/${configdir}/"

	TCL_DEPS="${tcldir}${TCL_LIB_FILE}"

	# If ${no_tk} is nonempty, then we cannot do Tk, and there is no
	# point to doing Tcl.
	if test -z "${no_tk}"; then
	   CY_AC_LOAD_TKCONFIG
	   CY_AC_PATH_TCLH
	   CY_AC_PATH_TKH
	   CY_AC_PATH_ITCLH
	   CY_AC_PATH_ITKH

	   # now look for Tk library stuff:
	   tkdir="../tk/${configdir}/"

	   TK_DEPS="${tkdir}${TK_LIB_FILE}"

	   # now look for Itcl library stuff:
	   CY_AC_PATH_ITCLCONFIG
           if test -z "${no_itcl}"; then
             CY_AC_LOAD_ITCLCONFIG 

	     ITCLLIB="${ITCL_BUILD_LIB_SPEC}"
	     ITCL_DEPS="${ITCL_LIB_FULL_PATH}"
           fi

	   # now look for Itk library stuff:
	   CY_AC_PATH_ITKCONFIG
           if test -z "${no_itcl}"; then
             CY_AC_LOAD_ITKCONFIG

	     ITKLIB="${ITK_BUILD_LIB_SPEC}"
	     ITK_DEPS="${ITK_LIB_FULL_PATH}"
           fi

           ENABLE_CFLAGS="${ENABLE_CFLAGS} \$(SUBDIR_GDBTK_CFLAGS)"

	   # Include some libraries that Tcl and Tk want.
	   TCL_LIBS='$(LIBGUI) $(ITCL) $(ITK) $(TK) $(TCL) $(X11_LDFLAGS) $(X11_LIBS)'
	   # Yes, the ordering seems wrong here.  But it is NOT.
	   # TK_LIBS is the list of libraries that need to be linked
	   # after Tcl/Tk. Note that this is NOT put into LIBS. If it
	   # were in LIBS then any link tests after this point would
	   # try to include things like `$(LIBGUI)', which would NOT work.
	   GDBTKLIBS="${TCL_LIBS} ${TK_LIBS}"

           CONFIG_OBS="${CONFIG_OBS} \$(SUBDIR_GDBTK_OBS)"
           CONFIG_DEPS="${CONFIG_DEPS} \$(SUBDIR_GDBTK_DEPS)"
           CONFIG_SRCS="${CONFIG_SRCS} \$(SUBDIR_GDBTK_SRCS)"
	   CONFIG_ALL="${CONFIG_ALL} all-gdbtk"
	   CONFIG_CLEAN="${CONFIG_CLEAN} clean-gdbtk"
	   CONFIG_INSTALL="${CONFIG_INSTALL} install-gdbtk"
	   CONFIG_UNINSTALL="${CONFIG_UNINSTALL} uninstall-gdbtk"

	   if test x$gdb_cv_os_cygwin = xyes; then
	      WIN32LIBS="${WIN32LIBS} -lshell32 -lgdi32 -lcomdlg32 -ladvapi32"
	      WIN32LDAPP="-Wl,--subsystem,console"
	      CONFIG_OBS="${CONFIG_OBS} gdbres.o"
	   fi
	fi
    fi

    AC_CONFIG_SUBDIRS([gdbtk])
else
    AC_MSG_NOTICE([skipping gdbtk tests])
fi

AC_SUBST([X_CFLAGS])
AC_SUBST([X_LDFLAGS])
AC_SUBST([X_LIBS])
AC_SUBST([TCL_DEPS])
AC_SUBST([TK_DEPS])
AC_SUBST([ITCLLIB])
AC_SUBST([ITCL_DEPS])
AC_SUBST([ITKLIB])
AC_SUBST([ITK_DEPS])
AC_SUBST([GDBTKLIBS])
AC_SUBST([GDBTK_CFLAGS])
AC_SUBST([GDBTK_SRC_DIR])

AC_PATH_XTRA dnl# calls AC_PATH_X as well

# Check whether we should enable the TUI, but only do so if we really
# can.
if test x"${enable_tui}" = x"yes"; then
  if test -d ${srcdir}/tui; then
    if test "x${ac_cv_search_waddstr}" != "xno" -a "x${ac_cv_search_initscr}" != "xno" -a "x${ac_cv_search_wborder}" != "xno"; then
      CONFIG_OBS="${CONFIG_OBS} \$(SUBDIR_TUI_OBS)"
      CONFIG_DEPS="${CONFIG_DEPS} \$(SUBDIR_TUI_DEPS)"
      CONFIG_SRCS="${CONFIG_SRCS} \$(SUBDIR_TUI_SRCS)"
      CONFIG_INITS="${CONFIG_INITS} \$(SUBDIR_TUI_INITS)"
      ENABLE_CFLAGS="${ENABLE_CFLAGS} \$(SUBDIR_TUI_CFLAGS)"
      ENABLE_LDFLAGS="${ENABLE_LDFLAGS} \$(SUBDIR_TUI_LDFLAGS)"
      CONFIG_ALL="${CONFIG_ALL} all-tui"
      CONFIG_CLEAN="${CONFIG_CLEAN} clean-tui"
      CONFIG_INSTALL="${CONFIG_INSTALL} install-tui"
      CONFIG_UNINSTALL="${CONFIG_UNINSTALL} uninstall-tui"
    else
      AC_MSG_WARN([no enhanced curses library found; disabling TUI])
    fi
  fi
fi

# Unlike the sim directory, whether a simulator is linked is controlled by 
# presence of a SIM= and a SIM_OBS= definition in the target '.mt' file.  
# This code just checks for a few cases where we would like to ignore those
# definitions, even when they are present in the '.mt' file. These cases
# are when --disable-sim is specified, or if the simulator directory is
# not part of the source tree.
#
AC_ARG_ENABLE([sim],
[AS_HELP_STRING([--enable-sim],[Link gdb with simulator])],
[echo "enable_sim = ${enable_sim}";
 echo "enableval = ${enableval}";
 case "${enableval}" in
  yes) ignore_sim=false ;;
  no)  ignore_sim=true ;;
  *)   ignore_sim=false ;;
 esac],
[ignore_sim=false])

if test ! -d "${srcdir}/../sim"; then
  ignore_sim=true
fi

if test "x${ignore_sim}" = "xtrue"; then
    IGNORE_SIM="SIM="
    IGNORE_SIM_OBS="SIM_OBS="
else
    DEFAULT_SIM_OB="remote-sim.o"
    AC_SUBST([DEFAULT_SIM_OB])
    IGNORE_SIM=""
    IGNORE_SIM_OBS=""
    AC_DEFINE([WITH_SIM],[1],[Define if the simulator is being linked in.])
fi
AC_SUBST([IGNORE_SIM])
AC_SUBST([IGNORE_SIM_OBS])

AC_SUBST([ENABLE_CFLAGS])
AC_SUBST([ENABLE_LDFLAGS])

AC_SUBST([PROFILE_CFLAGS])

AC_SUBST([CONFIG_OBS])
AC_SUBST([CONFIG_DEPS])
AC_SUBST([CONFIG_SRCS])
AC_SUBST([CONFIG_ALL])
AC_SUBST([CONFIG_CLEAN])
AC_SUBST([CONFIG_INSTALL])
AC_SUBST([CONFIG_UNINSTALL])

# List of host floatformats.
AC_DEFINE_UNQUOTED([GDB_HOST_FLOAT_FORMAT],[${gdb_host_float_format}],
                   [Host float floatformat])
AC_DEFINE_UNQUOTED([GDB_HOST_DOUBLE_FORMAT],[${gdb_host_double_format}],
                   [Host double floatformat])
AC_DEFINE_UNQUOTED([GDB_HOST_LONG_DOUBLE_FORMAT],
                   [${gdb_host_long_double_format}],
                   [Host long double floatformat])

# target_subdir is used by the testsuite to find the target libraries.
target_subdir=""
if test "${host}" != "${target}"; then
    target_subdir="${target_alias}/"
fi
AC_SUBST([target_subdir])

frags=""
if test "${target}" = "${host}"; then
  host_makefile_frag=${srcdir}/config/${gdb_host_cpu}/${gdb_host}.mh
  if test ! -f ${host_makefile_frag}; then
    AC_MSG_ERROR([*** Gdb does not support native target ${host}])
  fi
  frags="${frags} ${host_makefile_frag}"
else
  host_makefile_frag=/dev/null
fi

target_makefile_frag=${srcdir}/config/${gdb_target_cpu}/${gdb_target}.mt
if test ! -f ${target_makefile_frag}; then
  AC_MSG_ERROR([*** Gdb does not support target ${target}])
fi
frags="${frags} ${target_makefile_frag}"

AC_SUBST_FILE([host_makefile_frag])
AC_SUBST_FILE([target_makefile_frag])
AC_SUBST([frags])

changequote(,)dnl
hostfile=`sed -n '
s/XM_FILE[ 	]*=[ 	]*\([^ 	]*\)/\1/p
' ${host_makefile_frag}`

targetfile=`sed -n '
s/DEPRECATED_TM_FILE[ 	]*=[ 	]*\([^ 	]*\)/\1/p
' ${target_makefile_frag}`

if test "${target}" = "${host}"; then
# We pick this up from the host configuration file (.mh) because we
# do not have a native configuration Makefile fragment.
nativefile=`sed -n '
s/NAT_FILE[ 	]*=[ 	]*\([^ 	]*\)/\1/p
' ${host_makefile_frag}`
fi
changequote([,])

if test x"${gdb_osabi}" != x"" ; then
    AC_DEFINE_UNQUOTED([GDB_OSABI_DEFAULT],[${gdb_osabi}],
		    [Define to the default OS ABI for this configuration.])
fi

# Enable multi-ice-gdb-server.
AC_ARG_ENABLE([multi-ice],
[AS_HELP_STRING([--enable-multi-ice],[build the multi-ice-gdb-server])],
  [case ${enableval} in
    yes | no)
      ;;
    *) AC_MSG_ERROR([bad value ${enableval} for --enable-multi-ice]) ;;
  esac
])

if test "x${enable_multi_ice}" = "xyes"; then
   if test -d multi-ice; then
dnl# uncomment once it is ready:
dnl#     AC_CONFIG_SUBDIRS([multi-ice])
     :
   else
     AC_MSG_WARN([multi-ice is requested but the subdir for it is missing])
   fi
fi

# We only build gdbserver automatically if host and target are the same.
if test "x${target}" = "x${host}"; then
  AC_MSG_CHECKING([whether gdbserver is supported on this host])
  AC_ARG_VAR([build_gdbserver],
        [Try to override the decision to build the gdbserver
         (unsupported, might not work)])
  if test "x${build_gdbserver}" = "xyes"; then
    AC_MSG_RESULT([yes])
    AC_CONFIG_SUBDIRS([gdbserver])
  else
    AC_MSG_RESULT([no])
  fi
fi

# We build rdi-share on ARM-based targets, as instructed by configure.tgt.
if test "x${build_rdi_share}" = "xyes" -o "x${target_cpu}" = "xarm"; then
  AC_ARG_VAR([build_rdi_share],
             [Try to override the decision to build in the rdi-share subdirectory
              (unsupported, might not work)])
  AC_CONFIG_SUBDIRS([rdi-share])
fi

# We configure the nlm subdirectory on netware targets, as instructed
# by configure.tgt.
if test "x${build_nlm}" = "xyes"; then
  AC_ARG_VAR([build_nlm],
             [Try to override the decision to build in the nlm subdirectory
              (unsupported, might not work)])
  AC_CONFIG_SUBDIRS([nlm])
fi

# If hostfile (XM_FILE) and/or targetfile (DEPRECATED_TM_FILE) and/or
# nativefile (NAT_FILE) is not set in config/*/*.m[ht] files, we link
# to an empty version.

files=""
links=""

rm -f xm.h
xm_h=""
if test "${hostfile}" != ""; then
    xm_h=xm.h
    case "${hostfile}" in
      xm-*.h ) GDB_XM_FILE="config/${gdb_host_cpu}/${hostfile}" ;;
      * ) GDB_XM_FILE="${hostfile}"
    esac
    files="${files} ${GDB_XM_FILE}"
    links="${links} xm.h"
    AC_DEFINE_UNQUOTED([GDB_XM_FILE],["${GDB_XM_FILE}"],[hostfile])
fi
AC_SUBST([xm_h])

rm -f tm.h
tm_h=""
if test "${targetfile}" != ""; then
    tm_h=tm.h
    case "${targetfile}" in
      tm-*.h ) GDB_TM_FILE="config/${gdb_target_cpu}/${targetfile}" ;;
      * ) GDB_TM_FILE="${targetfile}"
    esac
    files="${files} ${GDB_TM_FILE}"
    links="${links} tm.h"
    AC_DEFINE_UNQUOTED([GDB_TM_FILE],["${GDB_TM_FILE}"],[targetfile])
fi
AC_SUBST([tm_h])

rm -f nm.h
nm_h=""
if test "${nativefile}" != ""; then
    nm_h=nm.h
    case "${nativefile}" in
      nm-*.h ) GDB_NM_FILE="config/${gdb_host_cpu}/${nativefile}" ;;
      * ) GDB_NM_FILE="${nativefile}"
    esac
    files="${files} ${GDB_NM_FILE}"
    links="${links} nm.h"
    AC_DEFINE_UNQUOTED([GDB_NM_FILE],["${GDB_NM_FILE}"],[nativefile])
fi
AC_SUBST([nm_h])

if test "x${files}" = "x"; then
  test -z "${files}"
  AC_MSG_WARN([the files variable is empty; not making symlinks.])
else
  test ! -z "${files}"
  AC_MSG_NOTICE([files should be "${files}"])
fi

if test "x${links}" = "x"; then
  test -z "${links}"
  AC_MSG_WARN([the links variable is empty; not making symlinks.])
else
  test ! -z "${links}"
  AC_MSG_NOTICE([links should be "${links}"])
fi

AC_DIAGNOSE([obsolete],[It is technically impossible to `autoupdate'
cleanly from AC\_LINK\_FILES to AC_CONFIG_LINKS. `autoupdate' provides a
functional but inelegant update, you should probably tune the result
yourself.])dnl
ac_sources="${files}"
ac_dests="${links}"
while test -n "${ac_sources}"; do
  set ${ac_dests}; ac_dest=${1}; shift; ac_dests=$*
  set ${ac_sources}; ac_source=${1}; shift; ac_sources=$*
  export ac_config_links_1="${ac_config_links_1} ${ac_dest}:${ac_source}"
done
AC_CONFIG_LINKS([${ac_config_links_1}])
AC_SUBST([ac_config_links1])
echo "symlinks to be made should be \"${ac_config_links1}\""

dnl# Check for exe extension set on certain hosts (e.g. Win32)
AC_EXEEXT

dnl#  Detect the character set used by this host.

dnl#  At the moment, we just assume it is ISO-8859-1 (which is a
dnl#  superset of ASCII containing the characters needed for French,
dnl#  German, Spanish, Italian, and possibly others), but if were
dnl#  *were* to support any host character sets other than ISO-8859-1,
dnl#  here is where we would detect it.

case "${host}" in
  *-apple-darwin* | *-apple-macos*)
  ;;
  *)
AC_DEFINE([GDB_DEFAULT_HOST_CHARSET],["ISO-8859-1"],
          [Define to be a string naming the default host character set.])
  ;;
esac

AM_ICONV

transform=`echo "${program_transform_name}" | sed -e 's/[\\$][\\$]/\\$/g'`
GDB_TRANSFORM_NAME=`echo gdb | sed -e "${transform}"`
if test "x${GDB_TRANSFORM_NAME}" = "x"; then
  export GDB_TRANSFORM_NAME=gdb
fi
AC_SUBST([GDB_TRANSFORM_NAME])
GCORE_TRANSFORM_NAME=`echo gcore | sed -e "${transform}"`
if test "x${GCORE_TRANSFORM_NAME}" = "x"; then
  test -z "${GCORE_TRANSFORM_NAME}" && export GCORE_TRANSFORM_NAME=gcore
fi
AC_SUBST([GCORE_TRANSFORM_NAME])
AC_ARG_VAR([GDB_DATADIR],[where to put gdb's data files])
if test -z "${GDB_DATADIR}"; then
    export GDB_DATADIR=${datadir}/gdb
fi
AC_SUBST([GDB_DATADIR])

AC_CONFIG_FILES([Makefile \
                 data-directory/Makefile \
                 .gdbinit:gdbinit.in \
                 gnulib/Makefile \
                 gdb-gdb.gdb])
AC_CONFIG_FILES([gcore],[chmod +x gcore])
AC_CONFIG_COMMANDS([default],[
dnl# Autoconf does NOT provide a mechanism for modifying definitions 
dnl# provided by makefile fragments.
dnl#

changequote(,)dnl
sed -e '/^DEPRECATED_TM_FILE[ 	]*=/s,^DEPRECATED_TM_FILE[ 	]*=[ 	]*,&config/'"${gdb_target_cpu}"'/,
/^XM_FILE[ 	]*=/s,^XM_FILE[ 	]*=[ 	]*,&config/'"${gdb_host_cpu}"'/,
/^NAT_FILE[ 	]*=/s,^NAT_FILE[ 	]*=[ 	]*,&config/'"${gdb_host_cpu}"'/,' <Makefile >Makefile.tmp
mv -f Makefile.tmp Makefile
changequote([,])dnl


case x${CONFIG_HEADERS} in
xconfig.h:config.in)
echo > stamp-h ;;
esac
],[
gdb_host_cpu=${gdb_host_cpu}
gdb_target_cpu=${gdb_target_cpu}
nativefile=${nativefile}
])

AC_CONFIG_COMMANDS([config_status_vpath_build_fix],[
if test "${srcdir}" != "."; then
  if test -e ${srcdir}/config.status -a ! -e `pwd`/config.status; then
    cp -v ${srcdir}/config.status `pwd`/config.status || echo ""
  elif test -e `pwd`/config.status -a ! -e ${srcdir}/config.status; then
    cp -v `pwd`/config.status ${srcdir}/config.status || echo ""
  fi
fi
],[])

AC_OUTPUT

echo ""

exit 0
