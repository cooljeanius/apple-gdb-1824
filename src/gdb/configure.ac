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

dnl# in case autoupdate changes this, the number is supposed to be "2.61":
AC_PREREQ([2.61])dnl
AC_INIT([gdb],[1824],[jmolenda@apple.com])
m4_include([../config/warnings.m4])
dnl# bug report address is email address of last person to touch this
dnl# directory, according to the Changelog-Apple in this directory
AC_CONFIG_SRCDIR([main.c])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADER([config.h])
AM_MAINTAINER_MODE

## Oops I had this backwards at first:
export APPLE_MAINT='#'
if test "x${APPLE_INTERNAL_DIR}" = "x/AppleInternal"; then
  test -d "${APPLE_INTERNAL_DIR}" && export APPLE_MAINT=""
fi
AC_SUBST([APPLE_MAINT])dnl

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
# ifndef __GDB_CONFIG_H__
#  define __GDB_CONFIG_H__ 1
# endif /* __GDB_CONFIG_H__ */])dnl
AH_BOTTOM([
#endif /* __CONFIG_H__ */
])dnl

if test -e ${srcdir}/development.sh; then
  # Set the 'development' global.
  AC_MSG_NOTICE([sourcing ${srcdir}/development.sh for gdb])
  . ${srcdir}/development.sh
fi

AC_PROG_CC
AC_PROG_CXX
AC_USE_SYSTEM_EXTENSIONS
AC_SEARCH_LIBS([strerror],[cposix]) dnl# used to be AC\_ISC\_POSIX
AM_PROG_CC_STDC

AC_CONFIG_AUX_DIR([..])
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

m4_ifdef([ACX_NONCANONICAL_BUILD],[
  ACX_NONCANONICAL_BUILD
])dnl
m4_ifdef([ACX_NONCANONICAL_HOST],[
  ACX_NONCANONICAL_HOST
])dnl
m4_ifdef([ACX_NONCANONICAL_TARGET],[
  ACX_NONCANONICAL_TARGET
])dnl

m4_ifdef([gl_HOST_OS],[
  gl_HOST_OS
])dnl
m4_ifdef([gl_HOST_CPU_C_ABI],[
  gl_HOST_CPU_C_ABI
])dnl

if test -n "${BUILD_ARCHITECTURE}"; then
  ACX_PROG_CC_WARNING_OPTS(["-arch ${BUILD_ARCHITECTURE}"],
                           [C_BUILDARCHFLAGS])
  AC_SUBST([C_BUILDARCHFLAGS])
fi
if test -n "${HOST_ARCHITECTURE}"; then
  ACX_PROG_CC_WARNING_OPTS(["-arch ${HOST_ARCHITECTURE}"],
                           [C_ARCHFLAGS])
  AC_SUBST([C_ARCHFLAGS])
fi
if test -n "${TARGET_ARCHITECTURE}"; then
  ACX_PROG_CC_WARNING_OPTS(["-arch ${TARGET_ARCHITECTURE}"],
                           [C_TARGARCHFLAGS])
  AC_SUBST([C_TARGARCHFLAGS])
fi

# Check for the 'make' the user wants to use:
AC_CHECK_PROGS([MAKE],[make gmake gnumake remake])dnl
AC_SUBST([MAKE])
MAKE_IS_GNU=""
case "`${MAKE} --version 2>&1 | sed 1q`" in
  *GNU*)
    MAKE_IS_GNU=yes
    AC_SUBST([MAKE_IS_GNU])
    ;;
esac
AM_CONDITIONAL([GMAKE],[test "x${MAKE_IS_GNU}" = "xyes"])dnl

# Do the same thing another way:
AX_CHECK_GNU_MAKE

# Automake (we can use this now that gnulib has a Makefile.am, even if
# srcdir itself still uses just a Makefile.in)
if test -f ${srcdir}/config.status; then
  AC_MSG_NOTICE([config.status is already present, removing it.])
  rm -f ${srcdir}/config.status
fi
AM_INIT_AUTOMAKE([1.11.6 gnits dejagnu \
                  dist-bzip2 dist-xz dist-zip dist-tarZ])dnl
  ## make sure this is in the right place:
if test -x ${srcdir}/missing; then
  test -e missing
  AM_SANITY_CHECK dnl# might as well put these here, too:
  m4_ifdef([AC_REQUIRE_AUX_FILE],[
    AC_REQUIRE_AUX_FILE([compile])dnl
    AC_REQUIRE_AUX_FILE([depcomp])dnl
    AC_REQUIRE_AUX_FILE([missing])dnl
  ])dnl
  ## should now have it...
fi

##FIXME: this is probably a bad idea:
if test -d ../intl; then
  if test -z "${LDFLAGS}"; then
    export LDFLAGS="-L. -L../intl"
  else
    export LDFLAGS="${LDFLAGS} -L../intl"
  fi
elif test -d ../libintl; then
  if test -z "${LDFLAGS}"; then
    export LDFLAGS="-L. -L../libintl"
  else
    export LDFLAGS="${LDFLAGS} -L../libintl"
  fi
fi

if test "x${recurse_into_gnulib}" = "xyes"; then
  # Configure gnulib.  We need to build gnulib under some other
  # directory not "gnulib", to avoid the problem of both GDB and
  # GDBserver wanting to build it in the same directory, when building
  # in the source dir.
  ACX_CONFIGURE_DIR(["gnulib"],["build-gnulib"])dnl
  ##FIXME: broken
fi

dnl# List of object files and targets accumulated by configure:
CONFIG_OBS=""
CONFIG_DEPS=""
CONFIG_SRCS=""
ENABLE_CFLAGS=""
ENABLE_LDFLAGS=""

CONFIG_ALL=""
CONFIG_CLEAN=""
CONFIG_INSTALL=""
CONFIG_UNINSTALL=""

dnl# Handle 64-bit BFD:
AC_ARG_ENABLE([64-bit-bfd],
[AS_HELP_STRING([--enable-64-bit-bfd],
                [64-bit bfd support (on hosts with narrower word sizes)])],
[case "${enableval}" in
  yes)  want64=true  ;;
  no)   want64=false ;;
  *)    AC_MSG_ERROR([bad value ${enableval} for 64-bit-bfd option]) ;;
esac],[want64=false])dnl
AC_SUBST([want64])dnl

if test "x${want64}" = "xtrue"; then
  enable_64_bit_bfd=yes
fi

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
    AC_SUBST([MACH_EXC_SERVER_PREFACE])dnl
    ## TODO: come back to this after working on aarch64 support
    ;;
  *-apple-darwin* | *-apple-macosx*)
    AC_MSG_CHECKING([whether we can use 64 bit mach exceptions])
    if test -r "${SDKROOT}/usr/include/mach/mach_exc.defs"; then
      AC_DEFINE([HAVE_64_BIT_MACH_EXCEPTIONS],[1],
                [Define to 1 if you have 64 bit mach exceptions.])
      MACH_EXC_SERVER_PREFACE='mach_'
      AC_MSG_RESULT([yes])
    elif test "x${SDKROOT}/usr" != "x/usr" && test -r "/usr/include/mach/mach_exc.defs"; then
      AC_MSG_WARN([SDK is unusable, having to fall back to system])
      AC_DEFINE([HAVE_64_BIT_MACH_EXCEPTIONS],[2],
                [Define to 1 if you have 64 bit mach exceptions in the SDK,
                 and to 2 if you have them, but NOT from your SDK.])
      MACH_EXC_SERVER_PREFACE='mach_'
    else
      MACH_EXC_SERVER_PREFACE=""
      AC_MSG_RESULT([no])
    fi
    AC_SUBST([MACH_EXC_SERVER_PREFACE])dnl
    ## might also want to use this elsewhere...
    ;;
esac

dnl# APPLE LOCAL check for posix_spawn and the "arch" program. Note that we
dnl# do NOT trust the "arch" program if the posix_spawn headers are NOT
dnl# present, since Tiger has an "arch" program, but it only echoes the
dnl# current system architecture.
case "${host}" in
  *-apple-darwin* | *-apple-macosx*)
    dnl# we are inside a conditional; no "_ONCE" allowed:
    AC_CHECK_HEADERS([spawn.h])dnl
    AC_CHECK_DECLS([POSIX_SPAWN_SETEXEC])
    AC_CHECK_FUNCS([posix_spawnp])
    AC_CHECK_TYPES([posix_spawnattr_t])
    AC_MSG_CHECKING([whether posix_spawn supports SETEXEC])
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <spawn.h>
    ]],[[
int foo = POSIX_SPAWN_SETEXEC;
    ]])],[gdb_cv_posix_spawn=yes],
         [gdb_cv_posix_spawn=no])
    AC_MSG_RESULT([${gdb_cv_posix_spawn}])dnl

    if test "x${gdb_cv_posix_spawn}" = "xyes"; then
      AC_DEFINE([USE_POSIX_SPAWN],[1],
                [Define to 1 to use posix_spawn instead of execvp.])dnl

      AC_ARG_ENABLE([arch-for-exec],
      	[AS_HELP_STRING([--disable-arch-for-exec],
                        [disable use of arch for exec])],
      	[case ${enableval} in
          yes | no) 
            enable_arch_exec="${enableval}" ;;
          *)
          AC_MSG_ERROR([bad val '${enableval}' to --disable-arch-for-exec])
            ;;
      	 esac],
      	[enable_arch_exec=yes])dnl
  
      AC_MSG_CHECKING([whether we should use arch for exec])
      if test x"${enable_arch_exec}" = x"yes"; then
        if test -r "/usr/bin/arch" && test -x "/usr/bin/arch"; then
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
    AC_CHECK_FUNCS([__mach_stack_logging_enumerate_records])
    AC_MSG_CHECKING([whether 64 bit stack logging is supported])
    AC_LINK_IFELSE([AC_LANG_PROGRAM([[
extern int __mach_stack_logging_enumerate_records();
    ]],[[
__mach_stack_logging_enumerate_records();
    ]])],[gdb_cv_stack_logging_64=yes],
         [gdb_cv_stack_logging_64=no])dnl
    ## corresponding define:
    if test "x${gdb_cv_stack_logging_64}" = "xyes"; then
      AC_DEFINE([HAVE_64_BIT_STACK_LOGGING],[1],
                [Define to 1 to use 64 bit stack logging functions.])
    fi
    AC_MSG_RESULT([${gdb_cv_stack_logging_64}])dnl

    if test "x${gdb_cv_stack_logging_64}" = "xno"; then
        AC_CHECK_FUNCS([stack_logging_enumerate_records])
    	AC_MSG_CHECKING([whether 32 bit stack logging is supported])
    	AC_LINK_IFELSE([AC_LANG_PROGRAM([[
extern int stack_logging_enumerate_records();
        ]],[[
stack_logging_enumerate_records();
        ]])],[gdb_cv_stack_logging_32=yes],
      	     [gdb_cv_stack_logging_32=no])dnl
        ## corresponding define:
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
    dnl# we are inside a conditional; no "_ONCE" allowed:
    AC_CHECK_HEADERS([mach/task.h])
    AC_MSG_CHECKING([whether task_set_state is available])
    AC_LINK_IFELSE([AC_LANG_PROGRAM([[
#include <mach/mach.h>
#include <mach/task.h>
    ]],
[[task_t task;]
 [thread_state_flavor_t flavor;]
 [thread_state_t new_state;]
 [mach_msg_type_number_t new_stateCnt;]
 [task_set_state(task, flavor, new_state, new_stateCnt);]])],
      [gdb_cv_task_set_state=yes],
      [gdb_cv_task_set_state=no])dnl
    ## corresponding define:
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
    ])dnl
    ## maybe also try it with MACH_EXC_SERVER_PREFACE in front of it...
    ;;
esac

if test "x${host_vendor}" = "xapple" || test "x${host_cpu}" = "xppc"; then
  AX_CHECK_COMPILE_FLAG([-force_cpusubtype_ALL],
                        [CFLAGS="${CFLAGS} -force_cpusubtype_ALL"],
                        [:])dnl
  ## might not actually be necessary, but whatever...
fi

if test "x${acl_cv_wl}" = "x"; then
  test -z "${acl_cv_wl}" && export acl_cv_wl='-Wl,'
fi
AC_SUBST([acl_cv_wl])dnl

AC_MSG_CHECKING([for linker flag to verify linked libraries have the correct architecture])
pre_saved_LDFLAGS="${LDFLAGS}"
LDFLAGS="${LDFLAGS} ${acl_cv_wl}-arch_errors_fatal"
AC_LINK_IFELSE([AC_LANG_PROGRAM([[]],[[]])],
               [LD_ARCHFLAGS="${acl_cv_wl}-arch_errors_fatal"],[])
LDFLAGS="${pre_saved_LDFLAGS}"
if test "x${LD_ARCHFLAGS}" != "x"; then
  test -n "${LD_ARCHFLAGS}"
  AC_MSG_RESULT([${LD_ARCHFLAGS}])
  if test "x${ERROR_ON_WARNING}" = "xyes"; then
    if test "x${WARN_LDFLAGS}" != "x"; then
      test -n "${WARN_LDFLAGS}" && WARN_LDFLAGS="${WARN_LDFLAGS} ${LD_ARCHFLAGS}"
    else
      test -z "${WARN_LDFLAGS}" && WARN_LDFLAGS="${LD_ARCHFLAGS}"
    fi
  fi
  AC_SUBST([LD_ARCHFLAGS])dnl
  ## that should be all we need to do with them for now...
else
  test -z "${LD_ARCHFLAGS}"
  AC_MSG_RESULT([none])
fi

AC_MSG_NOTICE([done with first batch of APPLE_LOCAL checks])dnl

dnl# Set up for gettext. PACKAGE is used when we call bindtextdomain.
AC_MSG_CHECKING([gettext-related things (Cygnus-style)])
AC_MSG_RESULT([ ])
ALL_LINGUAS=""
CY_GNU_GETTEXT
AC_CHECK_FUNCS([gettext _gettext])dnl

# Solaris requires -lintl if you want strerror (which calls dgettext)
# to return localized messages.
if test "x${ac_cv_func_strerror}" = "xyes" || test "x${ac_cv_search_strerror}" != "x"; then
  test -n "${ac_cv_func_strerror}" || test -n "${ac_cv_search_strerror}"
  AC_CHECK_LIB([intl],[dgettext],[:],[
    if test -x "`which ld`" && test "x${LD}" != "xld-136"; then
      AC_CHECK_LIB([intl],[libintl_dgettext])dnl
      ## cache value is also used below...
    fi
  ])dnl
  ## should be enough here
fi
unset ac_cv_lib_intl_gettext
# Also check for -lintl anyways, but with different functions:
AC_CHECK_LIB([intl],[gettext],[:],[
  AC_CHECK_LIB([intl],[libintl_gettext],[:],[
    if test -x "`which ld`" && test "x${LD}" != "xld-136"; then
      if test "x${ac_cv_lib_intl_main}" = "x" && test "x${LD}" = "Y0"; then
        test -z "${ac_cv_lib_intl_main}"
        ## neither worked:
        PRE_INTL_LDFLAGS="${LDFLAGS}"
        LDFLAGS="${LDFLAGS} ${LD_ARCHFLAGS}"
        ##FIXME: the linker sometimes crashes here:
        AC_CHECK_LIB([intl],[main])dnl
        ## reset:
        LDFLAGS="${PRE_INTL_LDFLAGS}"
      fi
    fi
  ])dnl#end case where symbols are prefixed with "libintl_"
])dnl#end case where symbols are unprefixed
  ## just in case:
if test -x "`which ld`" && test "x${LD}" != "xld-136"; then
  if test "x${ac_cv_lib_intl_libintl_dgettext}" = "x"; then
    test -z "${ac_cv_lib_intl_libintl_dgettext}"
    AC_SEARCH_LIBS([libintl_dgettext],[intl asprintf gettextlib gettextpo \
                                       gettextsrc gettext libintl lintl \
                                       gnu c iberty iconv])dnl
    ## ...or is it here where the linker crashes?
  fi
fi

localedir='${datadir}/locale'
AC_SUBST([localedir])dnl

if test "x${POSUB}" != "x"; then
  ## stuff for ${srcdir}/po:
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
  AC_MSG_RESULT([${LINGUAS}])dnl

  CONFIG_ALL="${CONFIG_ALL} all-po"
  CONFIG_CLEAN="${CONFIG_CLEAN} clean-po"		   
  CONFIG_INSTALL="${CONFIG_INSTALL} install-po"
  CONFIG_UNINSTALL="${CONFIG_UNINSTALL} uninstall-po"
elif test x"${USE_NLS}" = x"yes"; then
  CONFIG_ALL="${CONFIG_ALL} all-po"
  CONFIG_CLEAN="${CONFIG_CLEAN} clean-po"
  CONFIG_INSTALL="${CONFIG_INSTALL} install-po"
  CONFIG_UNINSTALL="${CONFIG_UNINSTALL} uninstall-po"
else
  CONFIG_ALL="${CONFIG_ALL}"
  CONFIG_CLEAN="${CONFIG_CLEAN}"
  CONFIG_INSTALL="${CONFIG_INSTALL}"
  CONFIG_UNINSTALL="${CONFIG_UNINSTALL}"
fi

GNULIB_IMPORT_DIR=build-gnulib/import

# For Makefile dependencies.
GNULIB_STDINT_H=""
if test x"${STDINT_H}" != x""; then
  test -n "${STDINT_H}" && test -n "${GNULIB_IMPORT_DIR}"
  export GNULIB_STDINT_H=${GNULIB_IMPORT_DIR}/${STDINT_H}
fi
AC_SUBST([GNULIB_STDINT_H])dnl

PACKAGE=gdb
AC_DEFINE_UNQUOTED([PACKAGE],["${PACKAGE}"],[Name of this package. ])dnl
AC_SUBST([PACKAGE])dnl

AC_MSG_NOTICE([done with Cygnus gettext checks])dnl

# We never need to detect it in this sub-configure.
# But preserve it for config.status --recheck.
AC_ARG_VAR([MAKEINFO],
           [Parent configure detects if it is of sufficient version.])dnl
AC_ARG_VAR([MAKEINFOFLAGS],
           [Parameters for MAKEINFO.])dnl

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
  AC_SUBST([MAKEINFO_EXTRA_FLAGS])dnl
  ## end makeinfo stuff
fi

# debugdir setting:
debugdir=${libdir}/debug
	 
AC_ARG_WITH([separate-debug-dir],
[AS_HELP_STRING([--with-separate-debug-dir=path],
       [Look for global separate debug info in this path [LIBDIR/debug]])],
[debugdir="${withval}"])dnl
	
AC_DEFINE_DIR([DEBUGDIR],[debugdir],
              [Global directory for separate debug files. ])dnl
  ## in case the above failed:
if test -z "${DEBUGDIR}" -a ! -z "${debugdir}" -a "x${GCC}" = "xno"; then
  test -d "${debugdir}"
  AC_DEFINE_UNQUOTED([DEBUGDIR],["${debugdir}"],
                     [Global directory for separate debug files. ])dnl
  ## 2...
fi

AC_CONFIG_SUBDIRS([doc testsuite])dnl

unused_subdirs=""

## this version of gdb does not actually use the gnulib sources yet,
## so make building them optional here:
AC_ARG_WITH([gnulib],
            [AS_HELP_STRING([--with-gnulib],
                            [Build the gnulib sources here])],
            [case ${withval} in
               yes|no) ;;
               *)
                 AC_MSG_ERROR([bad value '${withval}' for --with-gnulib])
                 ;;
             esac
             recurse_into_gnulib="${withval}"],
            [recurse_into_gnulib=no])dnl
  ## now use that flag:
if test -d ${srcdir}/gnulib && test "x${recurse_into_gnulib}" = "xyes"; then
  test -n "${recurse_into_gnulib}"
  AC_MSG_NOTICE([using gnulib])
  AC_CONFIG_SUBDIRS([gnulib])dnl
  ## (this way we can avoid any build failures associated with it)
else
  if test -d gnulib; then
    AC_MSG_NOTICE([gnulib subdir present, but we are not using it.])
    export unused_subdirs="${unused_subdirs} gnulib"
  fi
fi
AM_CONDITIONAL([MAKE_SHOULD_RECURSE_INTO_GNULIB],
  [test -d ${srcdir}/gnulib && test "x${recurse_into_gnulib}" = "xyes"])dnl

if test "x${target}" = "x${host}"; then
  gdb_native=yes
else
  gdb_native=no
fi

# Make sure shell scripts work:
AC_SYS_INTERPRETER

if test "x${SHELL}" != "x"; then
  test -n "${SHELL}" && export SHELL
  AC_SUBST([SHELL])dnl
  ## 2...
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
  AC_MSG_NOTICE([targ_archs are now '${targ_archs}'])
  AC_DEFINE_UNQUOTED([DEFAULT_BFD_ARCH],[${targ_archs}],
    [Define to BFD's default architecture. ])dnl
  ## end def
fi
if test "x${targ_defvec}" != "x"; then
  AC_MSG_NOTICE([targ_defvec is now '${targ_defvec}'])
  AC_DEFINE_UNQUOTED([DEFAULT_BFD_VEC],[${targ_defvec}],
    [Define to BFD's default target vector. ])dnl
  ## end def
fi

AC_ARG_PROGRAM

# See if we are building with C++, and substitute COMPILER:
GDB_AC_BUILD_WITH_CXX

# Dependency checking.
m4_ifdef([ZW_CREATE_DEPDIR],[
ZW_CREATE_DEPDIR
])dnl
m4_ifdef([ZW_PROG_COMPILER_DEPENDENCIES],[
ZW_PROG_COMPILER_DEPENDENCIES([CC])dnl
])dnl

# Some system checks.
m4_ifdef([ACX_LARGEFILE],[
  ACX_LARGEFILE
],[
  AC_SYS_LARGEFILE
])dnl
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
       AC_MSG_ERROR([bad value '${enableval}' for --enable-gdbcli]) ;;
   esac],
  [enable_gdbcli=yes])dnl
  ## set corresponding variables:
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
  [enable_gdbmi=yes])dnl
  ## set corresponding variables:
if test x"${enable_gdbmi}" = x"yes"; then
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
   esac],[enable_tui=no])dnl

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
   fi])dnl
  # We unconditionally disable gdbtk tests on selected platforms:
case ${host_os} in
  go32* | windows*)
    AC_MSG_WARN([gdbtk is NOT supported on ${host}; disabling])
    enable_gdbtk=no ;;
  darwin*)
    AC_MSG_WARN([gdbtk is broken on ${host}; disabling])
    enable_gdbtk=no
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
  AC_CHECK_HEADERS([libunwind.h])dnl
  AC_CHECK_HEADERS([libunwind-ia64.h])dnl
  ## make sure we found both of them:
  if test x"${ac_cv_header_libunwind_h}" = x"yes" -a x"${ac_cv_header_libunwind_ia64_h}" = x"yes"; then
    enable_libunwind=yes;
  fi
])dnl
   ## now actually do the libunwind checks:
if test x"${enable_libunwind}" = x"yes"; then
  AC_CHECK_HEADERS([libunwind.h])dnl
  AC_CHECK_HEADERS([libunwind-ia64.h])dnl
  AC_CHECK_DECLS([acc])dnl
  AC_CHECK_FUNCS([unw_create_addr_space_p unw_get_saveloc_p \
                  unw_search_unwind_table_p])dnl
  ## if all those checks succeeded, assume we can define:
  AC_DEFINE([HAVE_LIBUNWIND],[1],
            [Define if libunwind library is being used.])dnl
  ## ...and that we can also build these objects:
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
 [enable_profiling=no])dnl

AC_CHECK_FUNCS([monstartup _mcleanup])
AC_CACHE_CHECK([for _etext],[ac_cv_var__etext],
[AC_LINK_IFELSE([AC_LANG_PROGRAM([[
#include <stdlib.h>
extern char _etext;
 ]],[[
free(&_etext);
 ]])],[ac_cv_var__etext=yes],[ac_cv_var__etext=no])dnl
])dnl
  ## the corresponding define:
if test "x${ac_cv_var__etext}" = "xyes"; then
  AC_DEFINE([HAVE__ETEXT],[1],
            [Define to 1 if your system has the _etext variable. ])
fi
if test "x${enable_profiling}" = "xyes"; then
  if test "x${ac_cv_func_monstartup}" = "xno" || test "x${ac_cv_func__mcleanup}" = "xno"; then
    AC_MSG_ERROR([--enable-profiling requires monstartup and _mcleanup])
  fi
  PROFILE_CFLAGS=-pg
  OLD_CFLAGS="${CFLAGS}"
  CFLAGS="${CFLAGS} ${PROFILE_CFLAGS}"

  AC_CACHE_CHECK([whether ${CC} supports -pg],[ac_cv_cc_supports_pg],
    [AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[]],[[int x;]])],
                       [ac_cv_cc_supports_pg=yes],
                       [ac_cv_cc_supports_pg=no])])dnl

  if test "x${ac_cv_cc_supports_pg}" = "xno"; then
   AC_MSG_ERROR([--enable-profiling needs a ${CC} supporting the -pg flag])
  fi

  CFLAGS="${OLD_CFLAGS}"
fi

m4_ifdef([ACX_PKGVERSION],[
ACX_PKGVERSION([GDB])dnl
  ## corresponding define:
if test "x${PKGVERSION}" != "x"; then
  test -n "${PKGVERSION}"
  AC_DEFINE_UNQUOTED([PKGVERSION],["${PKGVERSION}"],
                     [Additional package description])dnl
  ## end definition
fi
],[
  test -z "${PKGVERSION}"
])dnl
m4_ifdef([ACX_BUGURL],[
ACX_BUGURL([http://www.gnu.org/software/gdb/bugs/])dnl
  ## corresponding define:
if test "x${REPORT_BUGS_TO}" != "x"; then
  test -n "${REPORT_BUGS_TO}"
  AC_DEFINE_UNQUOTED([REPORT_BUGS_TO],["${REPORT_BUGS_TO}"],
                     [Bug reporting address])dnl
  ## end definition
fi
],[
  test -z "${REPORT_BUGS_TO}"
])dnl

# --------------------- #
# Checks for programs.  #
# --------------------- #

AC_MSG_CHECKING([programs])
AC_MSG_RESULT([ ])
AN_PROGRAM([ar],[AM_PROG_AR])dnl
AN_MAKEVAR([AR],[AM_PROG_AR])dnl
  ## automake has a better check for the proper archiver:
if test -x `which automake`; then
  test -z "${AR}"
  AM_PROG_AR
else
  test -z "${AR}"
  AC_CHECK_TOOLS([AR],[ar gar],[],[${PATH}:./../binutils])
fi
if test -e ./config.cache; then
  echo "hacking ./config.cache"
  sed -i "s|ac_cv_env_CFLAGS_value|bad_CFLAGS|g" ./config.cache
  sed -i "s|ac_cv_env_CPPFLAGS_set|bad_CPPFLAGS_status|g" ./config.cache
  sed -i "s|ac_cv_env_CPPFLAGS_value|bad_CPPFLAGS|g" ./config.cache
  sed -i "s|ac_cv_env_CXXFLAGS_value|bad_CXXFLAGS|g" ./config.cache
  sed -i "s|ac_cv_exeext|bad_exeext|g" ./config.cache
fi
AN_PROGRAM([as],[AM_PROG_AS])dnl
AN_MAKEVAR([AS],[AM_PROG_AS])dnl
AM_PROG_AS
m4_ifdef([NCN_STRICT_CHECK_TOOLS],[
  NCN_STRICT_CHECK_TOOLS([ASSEMBLER],[as gas],[],[${PATH}:./../binutils])
])dnl
AC_PROG_CPP

AC_MSG_CHECKING([libtool things])
AC_MSG_RESULT([ ])
LT_INIT([disable-fast-install dlopen])dnl
LT_LANG([C])dnl
dnl# temporarily set this while I update everything libtool-related:
LT_PREREQ([2.4.6])dnl
dnl# even though gdb is written only in C, it has tests written
dnl# for debugging the following languages, so check them, too:
LT_LANG([C++])dnl
LT_LANG([Go])dnl
  ## gcj is slow:
if test "x${GO_SLOWLY}" = "xyes"; then
  test -n "${GO_SLOWLY}" && echo "Going slowly."
  LT_LANG([Java])dnl
  ## OK
fi
LT_LANG([Fortran 77])dnl
LT_LANG([Fortran])dnl
LT_LANG([Windows Resource])dnl
dnl# not exactly a "language" per se, but fits here in a similar spirit:
AC_OPENMP

AC_MSG_CHECKING([other programs])
AC_MSG_RESULT([ ])
if test "x${AWK}" = "x"; then
  test -z "${AWK}"
  AC_PROG_AWK
else
  test ! -z "${AWK}" && export AWK
  AC_SUBST([AWK])dnl
  ## gotta do something here...
fi
if test -d ./../fixincludes; then
  ## not really sure which of the programs it has to actually check for:
  AC_PATH_PROG([FIXINCL],[fixincl],[],[],[${PATH}:./../fixincludes])dnl
  ## this one is just a shell script, so nvm:
  test -x ./../fixincludes/fixinc.sh
fi
AC_PROG_INSTALL
AM_PROG_INSTALL_STRIP
AM_PROG_LEX
AM_PATH_LISPDIR
AC_PROG_LN_S
AC_PROG_MKDIR_P
AM_PATH_PYTHON
AC_PROG_OBJC
if test "x${OBJCPP}" = "x" && test -e /lib/cpp; then
  test -z "${OBJCPP}" && test -x /lib/cpp
  AC_PROG_OBJCPP
else
  test -n "${OBJCPP}" && test -x "${OBJCPP}" && stat "${OBJCPP}"
  AC_SUBST([OBJCPP])dnl
  ## just in case... I do something further here? idk...
fi
AC_PROG_OBJCXX
AC_PROG_OBJCXXCPP
if test "x${RANLIB}" = "x"; then
  test -z "${RANLIB}"
  AC_PROG_RANLIB
else
  test ! -z "${RANLIB}" && export RANLIB
  AC_SUBST([RANLIB])dnl
  ## gotta do something here...
fi
AC_PATH_PROG([RUNTEST_PROG],[runtest],[],[],[${PATH}:./../dejagnu])dnl
AC_BAKEFILE_METROWERKS_EXTO
AX_COMPILER_VENDOR

LINT_PATH_DIR=""
if test -d /usr/5bin; then
  LINT_PATH_DIR=/usr/5bin
elif test -d ../utils/lint; then
  LINT_PATH_DIR=../utils/lint
else
  LINT_PATH_DIR=.
fi
AC_PATH_PROGS([LINT],[lint splint lclint tdlint alint],[],[],
              [${PATH}:${LINT_PATH_DIR}])dnl
AC_SUBST([LINT_OPTS])dnl

AC_PATH_PROGS([RELPATH],[relpath],[],[],
              [${PATH}:./../utils/relpath.tproj])dnl

m4_ifdef([AM_BINUTILS_CLANG_STATIC_ANALYSIS],[
AM_BINUTILS_CLANG_STATIC_ANALYSIS
])dnl

AC_PATH_PROG([XCODEBUILD],[xcodebuild])dnl

# ResourceManager-related tools:
AC_PATH_PROG([REZ],[Rez],[],[],[${PATH}:/Developer/Tools])dnl
AC_PATH_PROG([DEREZ],[DeRez],[],[],[${PATH}:/Developer/Tools])dnl
AC_PATH_PROG([REZDET],[RezDet],[],[],[${PATH}:/Developer/Tools])dnl
AC_PATH_PROG([REZWACK],[RezWack],[],[],[${PATH}:/Developer/Tools])dnl
AC_PATH_PROG([RESMERGER],[ResMerger],[],[],[${PATH}:/Developer/Tools])dnl
AC_SUBST([REZ])dnl
AC_SUBST([DEREZ])dnl
AC_SUBST([REZDET])dnl
AC_SUBST([REZWACK])dnl
AC_SUBST([RESMERGER])dnl

# Other Mac-specific tools:
AC_PATH_PROG([LD_CLASSIC],[ld_classic])dnl
AC_PATH_PROG([MERGE_PEF],[MergePef])dnl
AC_PATH_PROG([CODESIGN],[codesign])dnl
AC_PATH_PROG([CODESIGN_ALLOCATE],[codesign_allocate])dnl
dnl#FIXME: also add a test to make sure that the codesign_allocate found
dnl# matches the version of the cctools and linker used by the compiler

# A few more compiler-y things
AM_PROG_VALAC([],[],[])
AC_PROG_YACC
AX_PROG_BISON([
  # (might want to check the bison version first before warning)
  AC_MSG_NOTICE([Actually using bison in this case.])
  EXTRA_YFLAGS_SEP=''
  BISON_CXX_FLAG=''
  if test "x${enable_build_with_cxx}" = "xyes"; then
    BISON_CXX_FLAG='--language=c++'
  elif test "x${enable_build_with_cxx}" = "xno"; then
    BISON_CXX_FLAG='--language=c'
  fi
  AC_SUBST([BISON_CXX_FLAG])dnl
],[
  # (might want to be more specific with this warning message)
  AC_MSG_WARN([Using something other than bison is untested.])
  EXTRA_YFLAGS_SEP='#'
])dnl##end bison conditional
AC_SUBST([EXTRA_YFLAGS_SEP])dnl

m4_ifdef([BISON_I18N],[
  m4_ifdef([AM_GNU_GETTEXT],[
    if test "x${USE_NLS}" = "x"; then
      test -z "${USE_NLS}" && export USE_NLS="no"
    fi
    BISON_I18N
  ])dnl
])dnl

AC_CHECK_TOOLS([DLLTOOL],[dlltool gdlltool],[],[${PATH}:./../binutils])dnl
  ## duplicate that:
if test "x${ac_ct_DLLTOOL}" = "x" && test "x${ac_cv_prog_DLLTOOL}" = "x" && test "x${ac_cv_prog_ac_ct_DLLTOOL}" = "x" && test "x${DLLTOOL}" = "x"; then
  test -z "${ac_ct_DLLTOOL}" && test -z "${ac_cv_prog_DLLTOOL}" && test -z "${ac_cv_prog_ac_ct_DLLTOOL}" && test -z "${DLLTOOL}"
  dnl# in case the differences introduced by the pluralized version of the
  dnl# macro caused this check to get skipped:
  AC_CHECK_TOOL([DLLTOOL],[dlltool],[],[${PATH}:./../binutils])dnl
  ## (the orignal)
else
  if test "x${DLLTOOL}" = "xdlltool"; then
    test -n "${DLLTOOL}" && echo "configure: ${DLLTOOL} already checked"
  elif test "x${DLLTOOL}" != "x"; then
    test -n "${DLLTOOL}" && test -x "${DLLTOOL}" && ${DLLTOOL} --version
    AC_MSG_NOTICE([dlltool already checked. Value is ${DLLTOOL}.])
  else
    test -z "${DLLTOOL}"
    AC_MSG_NOTICE([dlltool already checked])
  fi
fi
AC_CHECK_TOOLS([WINDRES],[windres gwindres],[],[${PATH}:./../binutils])dnl

# Needed for GNU/Hurd hosts.
# (and for separate but similar reasons for OS X)
AC_CHECK_TOOL([MIG],[mig])dnl

case ${host_os} in
  gnu*)
    if test x"${MIG}" = x""; then
      AC_MSG_ERROR([MIG not found but required for ${host} hosts])
    fi
    ;;
  darwin*)
    if test x"${MIG}" = x""; then
      AC_MSG_WARN([MIG not found but needed on ${host} hosts to regenerate sources made from .defs files])
    fi
    ;;
esac

# Tools for docs:
AC_PATH_PROGS([ROFF],[nroff groff troff roff])dnl
AC_PATH_PROG([CXREF],[cxref])dnl

dnl# We could not originally use AM\_INIT\_AUTOMAKE here (I have since 
dnl# added it above since then), but we might still need some of its
dnl# checks separately, so define this lite version of the macro
dnl# (only the AC\_REQUIRE'd parts, to prevent duplication):
AC_DEFUN([AM_INIT_MY_AUTOMAKE_LITE],[
  dnl# in case autoupdate changes this, the value is supposed to be "2.65":
  AC_PREREQ([2.65])dnl
  dnl# Autoconf wants to disallow AM_ names. We explicitly allow
  dnl# the ones we care about.
  m4_pattern_allow([^AM_[A-Z]+FLAGS$])dnl
  AC_REQUIRE([AM_SET_CURRENT_AUTOMAKE_VERSION])dnl
  AC_REQUIRE([AC_PROG_SED])dnl

  # test whether we have cygpath:
  if test -z "${CYGPATH_W}"; then
    if (cygpath --version) >/dev/null 2>/dev/null; then
      CYGPATH_W='cygpath -w'
    else
      CYGPATH_W=echo
    fi
  fi
  AC_SUBST([CYGPATH_W])dnl

  # Some tools that Automake needs:
  AC_REQUIRE([AM_SANITY_CHECK])dnl
  AC_REQUIRE([AM_MISSING_HAS_RUN])dnl
  AM_MISSING_PROG([AUTOCONF],[autoconf])dnl
  AM_MISSING_PROG([AUTOHEADER],[autoheader])dnl
  AM_MISSING_PROG([MAKEINFO],[makeinfo])dnl
  AC_REQUIRE([AM_PROG_INSTALL_SH])dnl
  dnl# For better backward compatibility. To be removed once Automake 1.9.x
  dnl# dies out for good.
  AC_SUBST([mkdir_p],['$(MKDIR_P)'])dnl
  dnl# We need awk for the "check" target. The system "awk" is bad on
  dnl# some platforms.
  AC_REQUIRE([AC_PROG_AWK])dnl
  AC_REQUIRE([AC_PROG_MAKE_SET])dnl
  AC_REQUIRE([AM_SET_LEADING_DOT])dnl
  AC_REQUIRE([AM_SILENT_RULES])dnl
  AC_REQUIRE([AC_PROG_GREP])dnl
  AC_REQUIRE([AM_SET_DEPDIR])dnl
  AC_REQUIRE([AM_OUTPUT_DEPENDENCY_COMMANDS])dnl
  AC_REQUIRE([AM_MAKE_INCLUDE])dnl
  AC_REQUIRE([AM_DEP_TRACK])dnl

  dnl######################################################################
  dnl# Port to Automake 1.11.
  dnl# This section can be removed once we assume Automake 1.14 or later.
  #### check arg val for silent rules:
  if test "x${enable_silent_rules}" = "xno"; then
    : ${V=1}
    : ${AM_DEFAULT_VERBOSITY=1}
  elif test "x${enable_silent_rules}" = "xyes"; then
    : ${V=0}
    : ${AM_DEFAULT_VERBOSITY=0}
  else
    : ${V=0}
    : ${AM_DEFAULT_VERBOSITY=1}
  fi
  : ${AM_DEFAULT_VERBOSITY=1}
  : ${AM_V=${AM_DEFAULT_VERBOSITY}}
  : ${AM_DEFAULT_V=${AM_DEFAULT_VERBOSITY}}
  dnl# now subst:
  AC_SUBST([V])dnl
  AM_SUBST_NOTMAKE([V])dnl
  AC_SUBST([AM_V])dnl
  AM_SUBST_NOTMAKE([AM_V])dnl
  AC_SUBST([AM_DEFAULT_V])dnl
  AM_SUBST_NOTMAKE([AM_DEFAULT_V])dnl
  AC_SUBST([AM_DEFAULT_VERBOSITY])dnl

  dnl## from configmake prep:
  dnl# Added in autoconf 2.70:
  if test "x${runstatedir}" = "x"; then
    AC_SUBST([runstatedir],['${localstatedir}/run'])
  fi
  dnl# Automake 1.9.6 only lacks pkglibexecdir; and since 1.11 merely
  dnl# provides it without AC_SUBST, this blind use of AC_SUBST is safe.
  AC_SUBST([pkglibexecdir],['${libexecdir}/${PACKAGE}'])dnl
])dnl
AM_INIT_MY_AUTOMAKE_LITE

# ---------------------- #
# Checks for libraries.  #
# ---------------------- #

AC_MSG_CHECKING([libraries])
AC_MSG_RESULT([ ])dnl

# We might need to link with -lm; most simulators need it.
LT_LIB_M
PRE_M_LDFLAGS="${LDFLAGS}"
LDFLAGS="${LDFLAGS} ${LD_ARCHFLAGS}"
AC_CHECK_LIB([m],[main])dnl
  ## reset:
LDFLAGS="${PRE_M_LDFLAGS}"
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
AC_CHECK_LIB([c],[printf])dnl

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
  if test "x${CPPFLAGS}" = "x"; then
    test -z "${CPPFLAGS}" && CPPFLAGS="-I. ${LIBXML2_INCLUDES}"
  else
    test -n "${CPPFLAGS}" && CPPFLAGS="${CPPFLAGS} ${LIBXML2_INCLUDES}"
  fi
  AC_CHECK_HEADERS([libxml/parser.h libxml/tree.h])
  AC_CHECK_LIB([xml2],[xmlParseFile])
fi
if test "x${ac_cv_header_libxml_tree_h}" = "xyes" -a \
     "x${ac_cv_header_libxml_parser_h}" = "xyes" -a \
     "x${ac_cv_lib_xml2_xmlParseFile}" = "xyes"
then
  AC_DEFINE([LIBXML2_IS_USABLE],[1],[libxml2 on this system is usable])
fi
AC_SUBST([LIBXML2_INCLUDES])dnl
AC_SUBST([LIBXML2_LIBS])dnl
# APPLE LOCAL end: Get the libxml2 headers and libraries

UNIX03_UGH_CFLAGS=-D__MAYBE_UNIX03
if test "x${COMMAND_MODE}" = "xlegacy"; then
  # APPLE LOCAL: Do NOT bother trying to make this copy of gdb work with
  # the "new" standards-conformant header environment:
  UNIX03_UGH_CFLAGS=-D__DARWIN_UNIX03=0
elif test "x${COMMAND_MODE}" = "xunix2003"; then
  # '1' is the opposite of '0':
  UNIX03_UGH_CFLAGS=-D__DARWIN_UNIX03=1
fi
AC_SUBST([UNIX03_UGH_CFLAGS])dnl

# We need to link with -lw to get `wctype' on Solaris before Solaris
# 2.6.  Solaris 2.6 and beyond have this function in libc, and have a
# libw that some versions of the GNU linker cannot hanle (GNU ld 2.9.1
# is known to have this problem).  Therefore we avoid libw if we can.
AC_CHECK_FUNC([wctype],[],
  [AC_CHECK_LIB([w],[wctype])])dnl

# Some systems (e.g. Solaris) have `gethostbyname' in libnsl.
# Others have it in libresolv or libc.
AC_SEARCH_LIBS([gethostbyname],[nsl resolv c])dnl

# Some systems (e.g. Solaris) have `socketpair' in libsocket.
AC_SEARCH_LIBS([socketpair],[socket c])dnl

# Link in zlib if we can. This allows us to read compressed debug sections:
m4_ifdef([AM_ZLIB],[
  AM_ZLIB dnl# from ../config/zlib.m4
],[
AC_MSG_CHECKING([if there is a zlib sister directory in this source tree])
if test -d ../zlib -o -d ${srcdir}/../zlib; then
  if test "x${zlibdir}" = "x"; then
    if test -d ../zlib; then
      test -z "${zlibdir}" && zlibdir=-L../zlib
    elif test -d ${srcdir}/../zlib; then
      test -z "${zlibdir}" && zlibdir=-L${srcdir}/../zlib
    fi
  fi
  if test "x${zlibinc}" = "x"; then
    test -z "${zlibinc}" && zlibinc="-I\$(srcdir)/../zlib"
  fi
  export HAVE_ZLIB_SOURCE=1
  AC_SUBST([HAVE_ZLIB_SOURCE])dnl
  ## print:
  AC_MSG_RESULT([yes])
else
  zlibdir=""
  zlibinc=""
  AC_MSG_RESULT([no])
fi
AC_SEARCH_LIBS([zlibVersion],[z],[AC_CHECK_HEADERS([zlib.h])])dnl
AC_SUBST([zlibdir])dnl
AC_SUBST([zlibinc])dnl
])dnl

# On FreeBSD we may need libutil for kinfo_getvmmap (used by fbsd-nat.c).
# On GNU/kFreeBSD systems, FreeBSD libutil is renamed to libutil-freebsd.
AC_SEARCH_LIBS([kinfo_getvmmap],[util util-freebsd bsd kern kinfo kmod],
   [AC_DEFINE([HAVE_KINFO_GETVMMAP],[1],
              [Define to 1 if your system has the kinfo_getvmmap func])])

# On HP/UX we may need libxpdl for dlgetmodinfo (used by solib-pa64.c).
AC_SEARCH_LIBS([dlgetmodinfo],[dl xpdl dlcompat dyld ltdl zrldsrl])dnl

# For the TUI, we need enhanced curses functionality.
#
# FIXME: kettenis/20040905: We prefer ncurses over the vendor-supplied
# curses library because the latter might not provide all the
# functionality we need.  However, this leads to problems on systems
# where the linker searches /usr/local/lib, but the compiler does NOT
# search /usr/local/include, if ncurses is installed in /usr/local. A
# default installation of ncurses on alpha*-dec-osf* will lead to such
# a situation.
AC_SEARCH_LIBS([waddstr],[ncurses cursesX curses])dnl

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
  darwin*)
    test -d ${srcdir}/libtermcap && stat ../libtermcap
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

# Other misc. libraries that autoscan added to configure.scan, and such:
AC_CHECK_LIB([asprintf],[asprintf],[
  if test "x${INTLLIBS}" != "x"; then
    test -n "${INTLLIBS}" && export INTLLIBS="${INTLLIBS} -lasprintf"
  fi
])dnl
  ## might want to ensure we find the in-tree one:
AC_CHECK_LIB([bfd],[bfd_set_reloc],[],[
  PRE_BFD_LDFLAGS="${LDFLAGS}"
  LDFLAGS="${LDFLAGS} ${LD_ARCHFLAGS}"
  AC_CHECK_LIB([bfd],[main])dnl
  ## reset:
  LDFLAGS="${PRE_BFD_LDFLAGS}"
])dnl
AC_CHECK_LIB([gnu],[gl_dummy_symbol],[],[
  PRE_GNU_LDFLAGS="${LDFLAGS}"
  LDFLAGS="${LDFLAGS} ${LD_ARCHFLAGS}"
  AC_CHECK_LIB([gnu],[main],[],[])dnl
  ## reset:
  LDFLAGS="${PRE_GNU_LDFLAGS}"
])dnl
AC_CHECK_LIB([iberty],[xmalloc],[],[
  PRE_LIBERTY_LDFLAGS="${LDFLAGS}"
  LDFLAGS="${LDFLAGS} ${LD_ARCHFLAGS}"
  AC_CHECK_LIB([iberty],[main])dnl
  ## reset:
  LDFLAGS="${PRE_LIBERTY_LDFLAGS}"
])dnl
AC_SEARCH_LIBS([gettext],
          [intl c gettext gnu asprintf gettextlib gettextpo gettextsrc])dnl
  ## might want to ensure we find the in-tree one:
unset ac_cv_lib_intl_gettext
AC_CHECK_LIB([intl],[gettext],[],[
  unset ac_cv_lib_intl_main
  PRE_INTL_LDFLAGS="${LDFLAGS}"
  LDFLAGS="${LDFLAGS} ${LD_ARCHFLAGS}"
  AC_CHECK_LIB([intl],[main],[],[])dnl
  ## reset:
  LDFLAGS="${PRE_INTL_LDFLAGS}"
])dnl
  ## might want to ensure we find the in-tree one:
AC_CHECK_LIB([opcodes],[op_cmp],[],[
  PRE_OPCODES_LDFLAGS="${LDFLAGS}"
  LDFLAGS="${LDFLAGS} ${LD_ARCHFLAGS}"
  AC_CHECK_LIB([opcodes],[main])dnl
  ## reset:
  LDFLAGS="${PRE_OPCODES_LDFLAGS}"
])dnl
  ## might want to ensure we find the in-tree one:
AC_CHECK_LIB([readline],[rl_initialize])dnl
AC_CHECK_LIB([sqlite3],[sqlite3_initialize])dnl
  ## might want to ensure we find the in-tree one:
AC_CHECK_LIB([z],[gzread])dnl

# ------------------------- #
# Checks for header files.  #
# ------------------------- #

AC_MSG_CHECKING([headers])
AC_MSG_RESULT([ ])dnl

AC_HEADER_DIRENT
AC_HEADER_STAT
AC_DEFUN([AC_REQUIRE_HEADER_STDC],[
  AC_REQUIRE([AC_HEADER_STDC])dnl
])dnl
AC_REQUIRE_HEADER_STDC
AC_HEADER_SYS_WAIT
AC_CHECK_HEADERS([nlist.h])dnl
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
])dnl
AC_CHECK_HEADERS([machine/reg.h])dnl
AC_CHECK_HEADERS([poll.h sys/poll.h])dnl
AC_CHECK_HEADERS([proc_service.h thread_db.h gnu/libc-version.h])dnl
AC_CHECK_HEADERS([stddef.h])dnl
AC_CHECK_HEADERS([stdio.h])dnl
AC_CHECK_HEADERS([sys/fault.h])dnl
AC_CHECK_HEADERS([sys/file.h])dnl
AC_CHECK_HEADERS([sys/filio.h])dnl
AC_CHECK_HEADERS([sys/ioctl.h])dnl
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
])dnl
AC_CHECK_HEADERS([sys/ptrace.h ptrace.h])dnl
AC_CHECK_HEADERS([sys/resource.h])dnl
AC_CHECK_HEADERS([sys/reg.h sys/debugreg.h])dnl
if test "x${ac_cv_header_sys_select_h}" = "x"; then
  test -z "${ac_cv_header_sys_select_h}"
  AC_CHECK_HEADERS([sys/select.h])
fi
AC_CHECK_HEADERS([sys/syscall.h])dnl
AC_CHECK_HEADERS([sys/user.h],[],[],[
#if HAVE_SYS_PARAM_H
# include <sys/param.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "This conftest for sys/user.h wants to include <sys/param.h>"
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_SYS_PARAM_H */
])dnl
  ## in case this failed to be checked:
if test "x${ac_cv_header_sys_wait_h}" = "x"; then
  test -z "${ac_cv_header_sys_wait_h}"
  AC_CHECK_HEADERS([sys/wait.h])
fi
AC_CHECK_HEADERS([wait.h])dnl
AC_CHECK_HEADERS([termios.h termio.h sgtty.h sys/termios.h])dnl
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
AC_CHECK_HEADERS([curses.h cursesX.h ncurses.h ncurses/ncurses.h])dnl
AC_CHECK_HEADERS([ncurses/term.h])dnl
AC_CHECK_HEADERS([term.h],[],[],[
#if HAVE_CURSES_H
# include <curses.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "This conftest for term.h expects <curses.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_CURSES_H */
])dnl

# FIXME: kettenis/20030102: In most cases we include these
# unconditionally, so what is the point in checking these?
# ^How about "why not?"?
AC_CHECK_HEADERS([ctype.h time.h])dnl

# Other headers that autoscan complains about:
AC_HEADER_STDBOOL dnl# also "_CHECK"s it
AC_HEADER_TIME
if test -d ../intl; then
  if test -z "${CPPFLAGS}"; then
    CPPFLAGS="-I. -I../intl"
  else
    CPPFLAGS="${CPPFLAGS} -I../intl"
  fi
elif test -d ../libintl; then
  if test -z "${CPPFLAGS}"; then
    CPPFLAGS="-I. -I../libintl"
  else
    CPPFLAGS="${CPPFLAGS} -I../libintl"
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
AC_CHECK_HEADERS([arpa/inet.h langinfo.h libintl.h limits.h locale.h \
                  mach/mach.h malloc.h malloc/malloc.h netdb.h \
                  netinet/in.h nl_types.h paths.h sys/param.h \
                  sys/socket.h sys/sysctl.h sys/time.h utmp.h \
                  wchar.h wctype.h])dnl

# The Makefile says this could be in /usr/include/sys if the OS violates
# POSIX, so check both places:
FCNTL_H_CFLAGS=""
AC_CHECK_HEADERS([fcntl.h],[
  : # (OK, no special flags needed)
],[
  AC_CHECK_HEADERS([sys/fcntl.h],[
    FCNTL_H_CFLAGS="-I/usr/include/sys"
  ],[
    AC_MSG_WARN([fcntl.h is in neither of the places where we might expect it to be])
  ])dnl
])dnl
AC_SUBST([FCNTL_H_CFLAGS])dnl

# Other headers needed for things that I have ifdef-ed:
unset ac_cv_header_dlfcn_h
unset ac_cv_header_memory_h
unset ac_cv_header_sys_types_h
AC_CHECK_HEADERS([a.out.h dlfcn.h errno.h regex.h sys/types.h])dnl
  ## special variable that depends on result of <sys/types.h> check:
EXTRA_DEFS=""
if test "x${ac_cv_header_sys_types_h}" = "xyes"; then
  # this has to go separate from the configure header, because we check
  # for it to be defined at the top of it (for caddr_t for ptrace):
  export DEFS="${DEFS} -DHAVE_SYS_TYPES_H"
  export EXTRA_DEFS="-DHAVE_SYS_TYPES_H"
fi
AC_SUBST([EXTRA_DEFS])dnl

# check these two headers together:
AC_CHECK_HEADERS([stdarg.h varargs.h])dnl
# even more header checks:
AC_HEADER_ASSERT
AC_CHECK_HEADERS([Availability.h AvailabilityMacros.h \
                  CoreFoundation/CoreFoundation.h \
                  CoreFoundation/CFPropertyList.h \
                  CoreFoundation/CFURLAccess.h Security/Security.h \
                  alloca.h assert.h architecture/byte_order.h \
                  bits/endian.h crt_externs.h dl.h libgen.h libgettext.h \
                  linux/byteorder/big_endian.h \
                  linux/byteorder/little_endian.h mach.h mach_error.h \
                  mach/kern_return.h mach/mach_error.h mach/mach_init.h \
                  mach/mach_port.h mach/message.h mach/port.h \
                  mach/thread_info.h mach/thread_status.h mach/vm_types.h \
                  mach-o/loader.h mach-o/nlist.h machine/save_state.h \
                  math.h memory.h pc.h pthread.h rpc/rpc.h \
                  servers/netname.h servers/netname_defs.h setjmp.h \
                  sys/uio.h sys/utsname.h uuid/uuid.h])dnl
  ## needs other headers first on some systems:
AC_CHECK_HEADERS([mach/kmod.h],[],[],[[
#ifdef HAVE_MACH_VM_TYPES_H
# include <mach/vm_types.h>
#endif /* HAVE_MACH_VM_TYPES_H */
]])dnl
  ## likewise:
AC_CHECK_HEADERS([rpc/clnt.h],[],[],[
#ifdef HAVE_RPC_RPC_H
# include <rpc/rpc.h>
#endif /* HAVE_RPC_RPC_H */
])dnl
  ## be careful in case we find the in-tree one:
AC_CHECK_HEADERS([gettext.h],[],[],[
#ifndef HAVE_CONFDEFS_H
# define HAVE_CONFDEFS_H 1
#endif /* !HAVE_CONFDEFS_H */
])dnl
# The gnulib subdirectory needs this:
gl_LIMITS_H
gl_STDINT_H
AC_SUBST([STDINT_H])dnl
gl_HEADER_STRING_H
# Just in case:
if test "x${NEXT_STRING_H}" = "x"; then
  test -z "${NEXT_STRING_H}" && export NEXT_STRING_H="<string.h>"
  AC_SUBST([NEXT_STRING_H])dnl
  ## (gnulib should be happy now)
fi
if test "x${enable_gdbtk}" = "xyes"; then
  if test "x${ac_cv_header_X11_Xlib_h}" = "x"; then
    test -z "${ac_cv_header_X11_Xlib_h}"
    AC_CHECK_HEADERS([X11/Xlib.h])
  else
    test -n "${ac_cv_header_X11_Xlib_h}" && test -f /usr/X11/include/X11/Xlib.h && test -r /usr/X11/include/X11/Xlib.h && stat /usr/X11/include/X11/Xlib.h
  fi
fi

# ------------------------- #
# Checks for declarations.  #
# ------------------------- #

AC_MSG_CHECKING([declarations])
AC_MSG_RESULT([ ])dnl

AC_CHECK_DECLS([atof],[],[],[
#ifdef HAVE_STDLIB_H
# include <stdlib.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "This conftest for atof wants to include <stdlib.h>"
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_STDLIB_H */
])dnl

unset ac_cv_have_decl_strpbrk
AC_CHECK_DECLS([strchr, strpbrk, strlcpy, strlcat],[],[],[
#ifdef HAVE_STRING_H
# include <string.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "This conftest expects <string.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_STRING_H */
])dnl

AC_CHECK_DECLS([strcasecmp, strncasecmp],[],[],[
#ifdef HAVE_STRINGS_H
# include <strings.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "This conftest expects <strings.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_STRINGS_H */
])dnl

m4_ifdef([libiberty_INIT],[
  libiberty_INIT
],[
  AC_MSG_NOTICE([skipping extra checks for libiberty things])dnl
])dnl

if test "x${ac_cv_have_decl_close}" = "x"; then
  test -z "${ac_cv_have_decl_close}"
  AC_CHECK_DECLS([close],[],[],[
#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif /* HAVE_UNISTD_H */
  ])dnl
  ## OK
fi

dnl# just skip the first one; getting different results from the two
dnl# conftests can lead to warnings:
if test "x${ac_cv_have_decl_sys_siglist}" = "x" && test "x" = "Y0"; then
  test -z "${ac_cv_have_decl_sys_siglist}"
  AC_CHECK_DECLS([sys_siglist])dnl
  ## end bad check for sys_siglist
fi
# just a plain check for sys_syglist might be wrong, so redo it:
dnl# using the autoupdated version of AC\_DECL\_SYS\_SIGLIST here:
if test "x${ac_cv_have_decl_sys_siglist}" != "xyes"; then
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
  ])dnl
  ## end better check for sys_siglist
fi
AC_CHECK_DECLS([bcopy, bzero, qsort_r, realpath])dnl
AC_CHECK_DECLS([gettimeofday, gmtime, localtime, localtime_r, setitimer],[
],[],[
#ifdef TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# ifdef HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  ifdef HAVE_TIME_H
#   include <time.h>
#  endif /* HAVE_TIME_H */
# endif /* HAVE_SYS_TIME_H */
#endif /* TIME_WITH_SYS_TIME */
])dnl
AC_CHECK_DECLS([free, malloc, realloc])dnl
AC_CHECK_DECLS([strerror, strstr, getopt])dnl
  ## the libiberty macro might have already checked these for us:
unset ac_cv_have_decl_asprintf
unset ac_cv_have_decl_snprintf
unset ac_cv_have_decl_vasprintf
unset ac_cv_have_decl_vsnprintf
AC_CHECK_DECLS([asprintf, snprintf, vasprintf, vsnprintf])dnl
dnl# these should probably be done above with the rest of the libunwind
dnl# checks:
AC_CHECK_DECLS([UNW_SLT_NONE],[],[],[
#ifdef HAVE_LIBUNWIND_H
# include <libunwind.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "This conftest for UNW_SLT_NONE wants to include <libunwind.h>"
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_LIBUNWIND_H */
])dnl
AC_CHECK_DECLS([UNW_SLT_MEMORY],[],[],[
#ifdef HAVE_LIBUNWIND_H
# include <libunwind.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "A conftest for UNW_SLT_MEMORY wants to include <libunwind.h>"
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_LIBUNWIND_H */
])dnl
AC_CHECK_DECLS([UNW_SLT_REG],[],[],[
#ifdef HAVE_LIBUNWIND_H
# include <libunwind.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "This conftest for UNW_SLT_REG wants to include <libunwind.h>"
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_LIBUNWIND_H */
])dnl
m4_ifdef([AM_LC_MESSAGES],[
  unset am_cv_val_LC_MESSAGES
  AM_LC_MESSAGES
],[
  if test "x${am_cv_val_LC_MESSAGES}" = "x"; then
    test -z "${am_cv_val_LC_MESSAGES}"
    AC_MSG_WARN([might have skipped check for LC_MESSAGES])
  elif test "x${am_cv_val_LC_MESSAGES}" != "xyes"; then
    AC_MSG_WARN([we might be missing support for LC_MESSAGES])
  fi
])dnl
  ## the libiberty macro might have already checked this for us, but avoid
  ## un-setting the cache value, though, as that can lead to redefinition
  ## warnings in config.log:
if test "x${ac_cv_have_decl_basename}" = "x" && test "x${ac_cv_have_decl_basename_char_p_}" = "x"; then
  test -z "${ac_cv_have_decl_basename}" && test -z "${ac_cv_have_decl_basename_char_p_}"
  AC_CHECK_DECLS([basename],[],[],[
#ifdef HAVE_LIBGEN_H
# include <libgen.h>
#endif /* HAVE_LIBGEN_H */
  ])dnl
  ## blah... darn it, basename...
fi
AC_CHECK_DECLS([name_server_port],[],[],[
#ifdef HAVE_MACH_MACH_INIT_H
# include <mach/mach_init.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "This conftest for name_server_port wants <mach/mach_init.h>"
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_MACH_MACH_INIT_H */
])dnl
AC_CHECK_DECLS([timeradd, timerclear, timersub],[],[],[
#ifdef HAVE_SYS_TIME_H
# include <sys/time.h>
#endif /* HAVE_SYS_TIME_H */
])dnl

# ----------------------- #
# Checks for structures.  #
# ----------------------- #

AC_MSG_CHECKING([structures])
AC_MSG_RESULT([ ])dnl

AC_STRUCT_ST_BLOCKS
AC_CHECK_MEMBERS([struct stat.st_blksize])dnl
AC_CHECK_MEMBERS([struct stat.st_rdev])dnl
dnl# checks for struct reg members are done later in a separate macro
AC_STRUCT_TIMEZONE dnl# also does the "_TM" one

# ------------------ #
# Checks for types.  #
# ------------------ #

AC_MSG_CHECKING([types])
AC_MSG_RESULT([ ])dnl

dnl# Begin what used to be AC\_TYPE\_SIGNAL
AC_CACHE_CHECK([return type of signal handlers],[ac_cv_type_signal],[
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <sys/types.h>
#include <signal.h>
]],[[
return *(signal (0, 0)) (0) == 1;
]])],[ac_cv_type_signal=int],[ac_cv_type_signal=void])dnl
])dnl
  ## corresponding define:
AC_DEFINE_UNQUOTED([RETSIGTYPE],[${ac_cv_type_signal}],
         [Define as the return type of signal handlers (`int' or `void').])
dnl# End what used to be AC\_TYPE\_SIGNAL
AC_CHECK_TYPES([socklen_t],[],[],[
#include <sys/types.h>
#include <sys/socket.h>
])dnl
AC_CHECK_TYPES([uintptr_t],[],[],[
#ifdef HAVE_STDINT_H
# include <stdint.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "This conftest for uintptr_t expects <stdint.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_STDINT_H */
])dnl
AC_CHECK_TYPES([ptrdiff_t])dnl
AC_TYPE_INT8_T
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_INTMAX_T
if test "x${ac_cv_type_mbstate_t}" = "x"; then
  test -z "${ac_cv_type_mbstate_t}"
  AC_TYPE_MBSTATE_T
fi
AC_TYPE_MODE_T
AC_TYPE_PID_T
AC_TYPE_SSIZE_T
AC_TYPE_UID_T
AC_CHECK_TYPES([uuid_t],[],[],[
#ifdef HAVE_UNISTD_H
# include <unistd.h>
#else
# ifdef HAVE_UUID_UUID_H
#  include <uuid/uuid.h>
# else
#  if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#   warning "Missing a header that might be needed for uuid_t"
#  endif /* __GNUC__ && !__STRICT_ANSI__ */
# endif /* HAVE_UUID_UUID_H */
#endif /* HAVE_UNISTD_H */
])dnl
AC_TYPE_UINT8_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINTMAX_T
AC_CHECK_SIZEOF([char])dnl
AC_CHECK_ALIGNOF([char])dnl
AC_CHECK_SIZEOF([short])dnl
AC_CHECK_ALIGNOF([short])dnl
AC_CHECK_SIZEOF([int])dnl
AC_CHECK_ALIGNOF([int])dnl
AC_CHECK_TYPES([wchar_t],[
  AC_CHECK_SIZEOF([wchar_t])dnl
  AC_CHECK_ALIGNOF([wchar_t])dnl
],[],[])dnl
AC_CHECK_SIZEOF([long])dnl
AC_CHECK_ALIGNOF([long])dnl
AC_CHECK_TYPES([caddr_t])dnl
AC_CHECK_TYPES([thread_t])dnl
AC_CHECK_TYPES([sa_family_t],[],[],[
#ifdef HAVE_SYS_SOCKET_H
# include <sys/socket.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "This conftest for sa_family_t wants <sys/socket.h> included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_SYS_SOCKET_H */
])dnl
AC_CHECK_TYPES([regex_t],[],[],[
#ifdef HAVE_REGEX_H
# include <regex.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "This conftest for regex_t expects <regex.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_REGEX_H */
])dnl
dnl# these should probably be done above with the rest of the libunwind
dnl# checks:
AC_CHECK_TYPES([unw_dyn_info_t],[],[],[
#ifdef HAVE_LIBUNWIND_H
# include <libunwind.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "The conftest for unw_dyn_info_t wants to include <libunwind.h>"
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_LIBUNWIND_H */
])dnl
AC_CHECK_TYPES([unw_save_loc_t],[],[],[
#ifdef HAVE_LIBUNWIND_H
# include <libunwind.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "The conftest for unw_save_loc_t wants to include <libunwind.h>"
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_LIBUNWIND_H */
])dnl
AC_CHECK_TYPES([unw_accessors_t],[],[],[
#ifdef HAVE_LIBUNWIND_H
# include <libunwind.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "A conftest for unw_accessors_t wants to include <libunwind.h>"
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_LIBUNWIND_H */
])dnl
dnl# some mach-specific ones:
AC_CHECK_TYPES([kern_return_t],[],[],[
#ifdef HAVE_MACH_KERN_RETURN_H
# include <mach/kern_return.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "This conftest for kern_return_t wants <mach/kern_return.h>"
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_MACH_KERN_RETURN_H */
])dnl
AC_CHECK_HEADERS([mach/mach_vm.h])
AC_CHECK_TYPES([mach_vm_address_t],[],[],[
#ifdef HAVE_MACH_MACH_VM_H
# include <mach/mach_vm.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "This conftest for mach_vm_address_t wants <mach/mach_vm.h>"
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_MACH_MACH_VM_H */
])dnl
AC_CHECK_TYPES([vm_address_t],[],[],[
#ifdef HAVE_MACH_MACH_VM_H
# include <mach/mach_vm.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "This conftest for vm_address_t wants <mach/mach_vm.h>"
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_MACH_MACH_VM_H */
])dnl
AC_CHECK_TYPES([mach_msg_header_t],[
  # Found the type, so check a member:
  AC_CHECK_MEMBER([mach_msg_header_t.msgh_seqno],[
  ##FIXME: choose a good define to put here
  ],[],[
#ifdef HAVE_MACH_MESSAGE_H
# include <mach/message.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "This conftest for member msgh_seqno wants <mach/message.h>"
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_MACH_MESSAGE_H */
  ])dnl
  ## might also want to check some other members here...
  ],[],[
#ifdef HAVE_MACH_MESSAGE_H
# include <mach/message.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "This conftest for mach_msg_header_t wants <mach/message.h>"
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_MACH_MESSAGE_H */
])dnl
AC_CHECK_TYPES([mach_port_t],[],[],[
#ifdef HAVE_MACH_PORT_H
# include <mach/port.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "This conftest for mach_port_t wants to include <mach/port.h>"
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_MACH_PORT_H */
])dnl
  ## if we have that type, then check for this function, as well:
if test "x${ac_cv_type_mach_port_t}" = "xyes"; then
  test -n "${ac_cv_type_mach_port_t}"
  AC_CHECK_FUNCS([mach_port_get_receive_status])dnl
  ## (for macosx-nat-inferior-debug.c)
fi
AC_CHECK_TYPES([save_state_t],[],[],[
#ifdef HAVE_MACHINE_SAVE_STATE_H
# include <machine/save_state.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "This conftest for save_state_t wants <machine/save_state.h>"
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_MACHINE_SAVE_STATE_H */
])dnl
dnl# The "x86_debug_state32_t" type is checked for in an APPLE_LOCAL
dnl# section below.
dnl# "long long" support is also checked farther below.

dnl### A common CoreFoundation type that we use:
AC_CHECK_TYPES([CFStringRef],[],[],[
#ifdef HAVE_CORE_FOUNDATION_CORE_FOUNDATION_H
# include <CoreFoundation/CoreFoundation.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "This conftest for CFStringRef needs the main CF header"
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_CORE_FOUNDATION_CORE_FOUNDATION_H */
])dnl
dnl## FIXME: there are many others that we might want to check as well...

# ------------------------------------- #
# Checks for compiler characteristics.  #
# ------------------------------------- #

AC_MSG_CHECKING([compiler characteristics])
AC_MSG_RESULT([ ])dnl

AC_PROG_GCC_TRADITIONAL
AC_C_BACKSLASH_A dnl# "\a" is used in a few files
AC_C_BIGENDIAN([
  AC_CHECK_DECLS([__BIG_ENDIAN])dnl
],[
  AC_CHECK_DECLS([__LITTLE_ENDIAN])dnl
],[
  AC_MSG_WARN([cannot check for additional endianness decls
 when the endianness is unknown;
 pre-setting ac_cv_c_bigendian=no (or yes) will help])
],[
  AC_CHECK_DECLS([__BIG_ENDIAN])dnl
  AC_CHECK_DECLS([__LITTLE_ENDIAN])dnl
])dnl
AC_C_CHAR_UNSIGNED dnl# can help with conversion warnings eventually
AC_C_FLEXIBLE_ARRAY_MEMBER dnl# struct hack in at least 1 header (symtab.h)
AC_C_PROTOTYPES
AC_C_STRINGIZE
AC_C_VARARRAYS
AC_C_VOLATILE
AC_DEFUN([AC_C_DUPES],[
  AC_REQUIRE([AC_C_CONST])dnl
  AC_REQUIRE([AC_C_INLINE])dnl
  AC_REQUIRE([AC_C_RESTRICT])dnl
  m4_ifdef([gl_MULTIARCH],[
    AC_REQUIRE([gl_MULTIARCH])dnl
  ])dnl
])dnl
AC_C_DUPES
gl_EXTERN_INLINE dnl# ensures config.h is okay for use with gnulib includes

#FIXME: This check can sometimes say that the compiler does NOT support the
# -fpascal-strings flag, even when it does actually support it:
AX_CHECK_COMPILE_FLAG([-fpascal-strings],
                      [CFLAGS="${CFLAGS} -fpascal-strings"],
                      [:])dnl

# ------------------------------ #
# Checks for library functions.  #
# ------------------------------ #

AC_MSG_CHECKING([functions])
AC_MSG_RESULT([ ])dnl

if test -f Makefile.in; then
  if test ! -z "`grep -i alloca Makefile.in`"; then
    if test "x${ac_cv_working_alloca_h}" = "x"; then
      stat Makefile.in >/dev/null
      test -z "${ac_cv_working_alloca_h}"
      test -n "${ac_cv_func_alloca_works}" && unset ac_cv_func_alloca_works
      AC_FUNC_ALLOCA
      if test "x${ac_cv_func_alloca_works}" = "xno"; then
        AC_SEARCH_LIBS([alloca],[iberty gnu gcc c alloca])
      else
        echo "alloca should work fine as it is"
      fi
    else
      test -n "${ac_cv_working_alloca_h}" && echo "Is alloca working? ${ac_cv_working_alloca_h}"
    fi
  else
    AC_MSG_NOTICE([skipping check for alloca])
  fi
else
  AC_MSG_WARN([cannot check Makefile.in to see if we need to check for alloca because Makefile.in seems to be missing])
fi
AC_FUNC_CLOSEDIR_VOID
AC_FUNC_ERROR_AT_LINE
AC_FUNC_FORK
AC_FUNC_GETPGRP
AC_FUNC_LSTAT
AC_FUNC_MALLOC
if test "x${ac_cv_func_mbrtowc}" = "x"; then
  test -z "${ac_cv_func_mbrtowc}"
  AC_FUNC_MBRTOWC
fi
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
m4_ifdef([AC_FUNC_WAIT3],[
  if test "x${GO_SLOWLY}" = "xyes"; then
    test -n "${GO_SLOWLY}" && echo "Going slowly."
    AC_FUNC_WAIT3
  else
    if test "x${ac_cv_func_wait3}" = "x"; then
      test -z "${ac_cv_func_wait3}"
      AC_CHECK_FUNCS([wait3])
    fi
  fi
])dnl
dnl# (why is 'realpath' up here, instead of in its alphabetical location?)
AC_CHECK_FUNCS([canonicalize_file_name realpath])dnl
AC_CHECK_FUNCS([getrusage])dnl
AC_CHECK_FUNCS([getuid getgid])dnl
AC_CHECK_FUNCS([pipe poll])dnl
AC_CHECK_FUNCS([pread pread64])dnl
AC_CHECK_FUNCS([pwrite pwrite64])dnl
AC_CHECK_FUNCS([sbrk])dnl
AC_CHECK_FUNCS([setpgid setpgrp setsid])dnl
AC_CHECK_FUNCS([sigaction sigprocmask sigsetmask])dnl
AC_CHECK_FUNCS([socketpair])dnl
AC_CHECK_FUNCS([syscall])dnl
AC_CHECK_FUNCS([ttrace])dnl
AC_CHECK_FUNCS([wborder wresize])dnl
  ## might have already been checked:
if test "x${ac_cv_func_setlocale}" = "x"; then
  test -z "${ac_cv_func_setlocale}"
  AC_CHECK_FUNCS([setlocale])dnl
  ## ok
fi
AC_CHECK_FUNCS([iconvlist libiconvlist btowc])dnl
AC_CHECK_FUNCS([setrlimit getrlimit])dnl
AC_CHECK_FUNCS([posix_madvise])dnl
AC_CHECK_FUNCS([waitpid])dnl
AC_CHECK_FUNCS([ptrace ptrace64])dnl
m4_ifdef([AM_LANGINFO_CODESET],[
  AM_LANGINFO_CODESET
],[
  AC_CHECK_FUNCS([langinfo codeset])dnl
])dnl

# Other functions (that autoscan might complain about missing):
unset ac_cv_func_getcwd
unset ac_cv_func_gethostbyname
unset ac_cv_func_strcasecmp
unset ac_cv_func_strchr
AC_CHECK_FUNCS([atof alarm assert atexit bcopy bsearch bzero \
                clnt_pcreateerror dup2 exit fflush fgets fprintf fwrite \
                getcwd gethostbyaddr gethostbyname gethostname getpass \
                gets gettimeofday gmtime inet_addr inet_ntoa isascii \
                ishexnumber isnumber isspace localtime localtime_r \
                mach_error_string memchr memcmp memcpy memmove mempcpy \
                memset mkdir netname_look_up nl_langinfo pathconf pclose \
                popen pow qsort qsort_r readdir recv re_comp regcomp \
                select send setitimer socket strcasecmp strcat strchr \
                strcpy strcspn strdup strerror strlcat strlcpy strlen \
                strncasecmp strncat strncmp strncpy strndup strpbrk \
                strrchr strspn strstr strtok strtok_r strtol strtoul \
                timeradd timerclear timersub uname vasprintf wcwidth])dnl

dnl# Use as a replacement for just-plain sprintf:
if test "x${ac_cv_func_snprintf}" = "x"; then
  test -z "${ac_cv_func_snprintf}"
  AC_CHECK_FUNCS([snprintf])
fi
dnl# might be checked elsewhere?
if test "x${ac_cv_func_connect}" = "x"; then
  test -z "${ac_cv_func_connect}"
  AC_CHECK_FUNCS([connect])
fi

dnl# CoreFoundation:
AC_CHECK_FUNCS([CFPropertyListCreateWithData])dnl

# ----- #
# Other #
# ----- #

AC_MSG_CHECKING([miscellaneous things that require custom checks])
AC_MSG_RESULT([ ])dnl

dnl# moved to a separate macro file, so subdirs can use it:
GDB_CHECK_PTRACE

dnl# likewise:
GDB_FUNC_SETPGRP

# Check if sigsetjmp is available. Using AC_CHECK_FUNCS will not do
# since sigsetjmp might only be defined as a macro.
AC_CACHE_CHECK([for sigsetjmp],[gdb_cv_func_sigsetjmp],[
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <setjmp.h>
]],[[
sigjmp_buf env;
while (! sigsetjmp(env, 1)) {
  siglongjmp(env, 1);
}
]])],[gdb_cv_func_sigsetjmp=yes],[gdb_cv_func_sigsetjmp=no])])
if test "x${gdb_cv_func_sigsetjmp}" = "xyes"; then
  AC_DEFINE([HAVE_SIGSETJMP],[1],[Define if sigsetjmp is available. ])
fi

# Assume we shall default to using the included libiberty regex:
gdb_use_included_regex=yes

dnl# for the next conftest:
AC_CHECK_HEADERS([gnu-versions.h])dnl

# However, if the system regex is GNU regex, then default to *not*
# using the included regex.
AC_CACHE_CHECK([for GNU regex],
  [gdb_cv_have_gnu_regex],
  [AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <gnu-versions.h>
   ]],[[
#define REGEX_INTERFACE_VERSION 1
#if _GNU_REGEX_INTERFACE_VERSION != REGEX_INTERFACE_VERSION
# error "Version mismatch between regex interfaces"
#endif /* _GNU_REGEX_INTERFACE_VERSION != REGEX_INTERFACE_VERSION */
   ]])],[gdb_cv_have_gnu_regex=yes],[gdb_cv_have_gnu_regex=no])])
if test "x${gdb_cv_have_gnu_regex}" = "xyes"; then
  gdb_use_included_regex=no
fi

AC_ARG_WITH([included-regex],
  [AS_HELP_STRING([--without-included-regex],
                  [Do NOT use included regex; this is the default
                   on systems with version 2 of the GNU C library
                   (use with caution on other systems)])],
  [gdb_with_regex=${withval}],
  [gdb_with_regex=${gdb_use_included_regex}])dnl
  ## corresponding define:
if test "x${gdb_with_regex}" = "xyes"; then
  AC_DEFINE([USE_INCLUDED_REGEX],[1],
    [Define to 1 if the regex included in libiberty should be used.])
fi

# Check if <sys/proc.h> defines `struct thread' with a td_pcb member:
AC_CHECK_MEMBERS([struct thread.td_pcb],[],[],[
#include <sys/param.h>
#include <sys/proc.h>
])dnl

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
AC_CACHE_CHECK([for struct reg in machine/reg.h],[gdb_cv_struct_reg],[
dnl# (machine/reg.h header is already checked-for above)
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
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
                 [#include <machine/reg.h>])dnl

# See if <sys/user.h> supports the %fs_base and %gs_bas amd64 segment
# registers. Older amd64 Linuxes do NOT have the fs_base and gs_base
# members of `struct user_regs_struct'.
AC_CHECK_MEMBERS([struct user_regs_struct.fs_base, struct user_regs_struct.gs_base],
		 [],[],[#include <sys/user.h>])dnl

# See if <sys/ptrace.h> provides the PTRACE_GETREGS request.
AC_MSG_CHECKING([for PTRACE_GETREGS])
AC_CACHE_VAL([gdb_cv_have_ptrace_getregs],[
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <sys/ptrace.h>
]],[[
PTRACE_GETREGS;
]])],[gdb_cv_have_ptrace_getregs=yes],[gdb_cv_have_ptrace_getregs=no])dnl
])dnl
  ## print:
AC_MSG_RESULT([${gdb_cv_have_ptrace_getregs}])
if test "x${gdb_cv_have_ptrace_getregs}" = "xyes"; then
  AC_DEFINE([HAVE_PTRACE_GETREGS],[1], 
            [Define if sys/ptrace.h defines the PTRACE_GETREGS request.])
fi

# See if <sys/ptrace.h> provides the PTRACE_GETFPXREGS request:
AC_MSG_CHECKING([for PTRACE_GETFPXREGS])
AC_CACHE_VAL([gdb_cv_have_ptrace_getfpxregs],[
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <sys/ptrace.h>
]],[[
PTRACE_GETFPXREGS;
]])],[gdb_cv_have_ptrace_getfpxregs=yes],
     [gdb_cv_have_ptrace_getfpxregs=no])dnl
])dnl
  ## print:
AC_MSG_RESULT([${gdb_cv_have_ptrace_getfpxregs}])
if test "x${gdb_cv_have_ptrace_getfpxregs}" = "xyes"; then
  AC_DEFINE([HAVE_PTRACE_GETFPXREGS],[1],
            [Define if sys/ptrace.h defines the PTRACE_GETFPXREGS request])
fi

# See if <sys/ptrace.h> provides the PT_GETDBREGS request.
AC_MSG_CHECKING([for PT_GETDBREGS])
AC_CACHE_VAL([gdb_cv_have_pt_getdbregs],[
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <sys/types.h>
#include <sys/ptrace.h>
]],[[
PT_GETDBREGS;
]])],[gdb_cv_have_pt_getdbregs=yes],[gdb_cv_have_pt_getdbregs=no])dnl
])dnl
  ## print:
AC_MSG_RESULT([${gdb_cv_have_pt_getdbregs}])
if test "x${gdb_cv_have_pt_getdbregs}" = "xyes"; then
  AC_DEFINE([HAVE_PT_GETDBREGS],[1],
            [Define if sys/ptrace.h defines the PT_GETDBREGS request.])dnl
  ## end definition
fi

# See if <sys/ptrace.h> provides the PT_GETXMMREGS request:
AC_MSG_CHECKING([for PT_GETXMMREGS])
AC_CACHE_VAL([gdb_cv_have_pt_getxmmregs],[
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <sys/types.h>
#include <sys/ptrace.h>
]],[[
PT_GETXMMREGS;
]])],[gdb_cv_have_pt_getxmmregs=yes],[gdb_cv_have_pt_getxmmregs=no])dnl
])dnl
  ## print:
AC_MSG_RESULT([${gdb_cv_have_pt_getxmmregs}])
if test "x${gdb_cv_have_pt_getxmmregs}" = "xyes"; then
  AC_DEFINE([HAVE_PT_GETXMMREGS],[1],
            [Define if sys/ptrace.h defines the PT_GETXMMREGS request.])dnl
  ## end definition
fi

# If we are configured native on GNU/Linux, work around problems with
# <sys/procfs.h>
# Also detect which type of /proc is in use, such as for Unixware
# or for Solaris:
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
                 (replaces HAVE_MULTIPLE_PROC_FDS and other macros)])
      ;;
  *-*-solaris2.[[6789]] | *-*-solaris2.1[[0-9]])
      AC_DEFINE([NEW_PROC_API],[1],
                [Define if you want to use new multi-fd /proc interface
                 (replaces HAVE_MULTIPLE_PROC_FDS and other macros)])
      ;;
  mips-sgi-irix5*)
      # Work around <sys/proc.h> needing _KMEMUSER problem on IRIX 5.
      AC_DEFINE([_KMEMUSER],[1],
                [Define to 1 so <sys/proc.h> gets a definition of anon_hdl.
                 Works around a <sys/proc.h> problem on IRIX 5.])
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
    AC_ARG_ENABLE([carbon-framework],
    [AS_HELP_STRING([--disable-carbon-framework],
                    [disable Carbon framework use even when available])],
    [case ${enableval} in
      yes | no) 
       enable_carbon_framework="${enableval}" ;;
      *)
       AC_MSG_ERROR([bad val '${enableval}' to --disable-carbon-framework])
       ;;
     esac],
    [enable_carbon_framework=no])dnl

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

AC_SUBST([CARBON_LIBS])dnl

case "${host}" in
  arm-apple-*)
    # We will NOT be using DebugSymbols.framework to locate dSYM files for
    # ARM yet.
    AC_MSG_NOTICE([DebugSymbols.framework unavailable on ARM])
  ;;
  *-apple-darwin* | *-apple-macos*)
    AC_ARG_ENABLE([debug-symbols-framework],
    [AS_HELP_STRING([--disable-debug-symbols-framework],
                    [disable DebugSymbols framework use even if present])],
    [case ${enableval} in
       yes | no) 
         enable_debug_symbols_framework="${enableval}" ;;
       *)
         AC_MSG_ERROR([bad val '${enableval}' to --disable-debug-symbols-framework])
         ;;
     esac],
    [enable_debug_symbols_framework=yes])dnl

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

AC_SUBST([DEBUG_SYMBOLS_LIBS])dnl

# APPLE LOCAL: Enable MIG checking for macosx/macosx.defs.
MIG_CHECKING=-D__MigTypeCheck=1
AC_SUBST([MIG_CHECKING])dnl
# APPLE LOCAL end

# APPLE LOCAL: See if we have the i386 debug register structs, i.e.
# whether we can use the i386 hardware registers for watchpoints or if
# we will need to use the old page-protection method.

AC_CHECK_TYPES([x86_debug_state_t, x86_debug_state32_t],[],[],
               [#include <mach/mach_types.h>])dnl

AC_CHECK_TYPES([thread_identifier_info_data_t],[],[],
               [#include <mach/thread_info.h>])dnl

AC_MSG_NOTICE([done with second batch of APPLE_LOCAL checks])dnl

AC_CHECK_FUNCS([pthread_setname_np])dnl

if test "x${ac_cv_header_sys_procfs_h}" = "xyes"; then
  BFD_HAVE_SYS_PROCFS_TYPE([pstatus_t])dnl
  BFD_HAVE_SYS_PROCFS_TYPE([prrun_t])dnl
  BFD_HAVE_SYS_PROCFS_TYPE([gregset_t])dnl
  BFD_HAVE_SYS_PROCFS_TYPE([fpregset_t])dnl
  BFD_HAVE_SYS_PROCFS_TYPE([prgregset_t])dnl
  BFD_HAVE_SYS_PROCFS_TYPE([prfpregset_t])dnl
  BFD_HAVE_SYS_PROCFS_TYPE([prgregset32_t])dnl
  BFD_HAVE_SYS_PROCFS_TYPE([prfpregset32_t])dnl
  BFD_HAVE_SYS_PROCFS_TYPE([lwpid_t])dnl
  BFD_HAVE_SYS_PROCFS_TYPE([psaddr_t])dnl
  BFD_HAVE_SYS_PROCFS_TYPE([prsysent_t])dnl
  BFD_HAVE_SYS_PROCFS_TYPE([pr_sigset_t])dnl
  BFD_HAVE_SYS_PROCFS_TYPE([pr_sigaction64_t])dnl
  BFD_HAVE_SYS_PROCFS_TYPE([pr_siginfo64_t])dnl

  dnl# Check for broken prfpregset_t type.

  dnl# For Linux/i386, glibc 2.1.3 was released with a bogus
  dnl# prfpregset_t type (it was a typedef for the pointer to a struct
  dnl# instead of the struct itself). We detect this here, and work
  dnl# around it in gdb_proc_service.h.

  if test "x${bfd_cv_have_sys_procfs_type_prfpregset_t}" = "xyes"; then
    AC_MSG_CHECKING([whether prfpregset_t type is broken])
    AC_CACHE_VAL([gdb_cv_prfpregset_t_broken],
      [AC_RUN_IFELSE([AC_LANG_SOURCE([[
#include <sys/procfs.h>
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
    AC_CHECK_SIZEOF([prfpregset_t],[],[#include <sys/procfs.h>])
    AC_CHECK_SIZEOF([void *])
    if test "x${gdb_cv_prfpregset_t_broken}" = "xyes"; then
      AC_DEFINE([PRFPREGSET_T_BROKEN],[1],
                [Define if the prfpregset_t type is broken.])
    fi
  fi

  dnl# Check for PIOCSET ioctl entry:
  AC_MSG_CHECKING([for PIOCSET ioctl entry in sys/procfs.h])
  AC_CACHE_VAL([gdb_cv_have_procfs_piocset],[
  AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
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
  AC_CACHE_VAL([gdb_cv_have_struct_link_map_with_l_members],[
     AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <link.h>
     ]],[[
struct link_map lm; (void)lm.l_addr;
     ]])],[gdb_cv_have_struct_link_map_with_l_members=yes],
          [gdb_cv_have_struct_link_map_with_l_members=no])dnl
  ])dnl
  ## print:
  AC_MSG_RESULT([${gdb_cv_have_struct_link_map_with_l_members}])
  if test "x${gdb_cv_have_struct_link_map_with_l_members}" = "xyes"; then
    AC_DEFINE([HAVE_STRUCT_LINK_MAP_WITH_L_MEMBERS],[1],
              [Define if <link.h> exists and defines struct link_map
               which has members with an ``l_'' prefix.
               (For Solaris, SVR4, and SVR4-like systems.)])
  fi

  dnl# Check for struct link_map with lm_ members which are indicative
  dnl# of SunOS-like shared libraries:
  AC_MSG_CHECKING([for member lm_addr in struct link_map])
  AC_CACHE_VAL([gdb_cv_have_struct_link_map_with_lm_members],[
     AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <sys/types.h>
#include <link.h>
     ]],[[
struct link_map lm; (void)lm.lm_addr;
     ]])],[gdb_cv_have_struct_link_map_with_lm_members=yes],
          [gdb_cv_have_struct_link_map_with_lm_members=no])dnl
  ])dnl
  ## print:
  AC_MSG_RESULT([${gdb_cv_have_struct_link_map_with_lm_members}])
  if test "x${gdb_cv_have_struct_link_map_with_lm_members}" = "xyes"; then
    AC_DEFINE([HAVE_STRUCT_LINK_MAP_WITH_LM_MEMBERS],[1],
              [Define if <link.h> exists and defines struct link_map
               which has members with an ``lm_'' prefix. (For SunOS.)])
  fi

  dnl# Check for struct so_map with som_ members which are found on 
  dnl# some *BSD systems:
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
          [gdb_cv_have_struct_so_map_with_som_members=no])dnl
  ])dnl
  ## print:
  AC_MSG_RESULT([${gdb_cv_have_struct_so_map_with_som_members}])
  if test "x${gdb_cv_have_struct_so_map_with_som_members}" = "xyes"; then
    AC_DEFINE([HAVE_STRUCT_SO_MAP_WITH_SOM_MEMBERS],[1],
              [Define if <link.h> exists and defines a struct so_map
               which has members with an ``som_'' prefix.
               (Found on older *BSD systems.)])dnl
    ## 4...
  fi

  dnl# Check for struct link_map32 type, which allows a 64-bit Solaris
  dnl# debugger to debug a 32-bit Solaris app with 32-bit shared libraries:
  AC_MSG_CHECKING([for struct link_map32 in sys/link.h])
  AC_CACHE_VAL([gdb_cv_have_struct_link_map32],[
     AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#define _SYSCALL32
#include <sys/link.h>
     ]],[[
struct link_map32 l;
     ]])],[gdb_cv_have_struct_link_map32=yes],
          [gdb_cv_have_struct_link_map32=no])dnl
  ])dnl
  ## print:
  AC_MSG_RESULT([${gdb_cv_have_struct_link_map32}])
  if test "x${gdb_cv_have_struct_link_map32}" = "xyes"; then
    AC_DEFINE([HAVE_STRUCT_LINK_MAP32],[1],
              [Define if <sys/link.h> has struct link_map32])dnl
    AC_DEFINE([_SYSCALL32],[1],
              [Define if <sys/link.h> has link_map32
               (solaris sparc-64 target)])dnl
    ## should be enough definitions
  fi
else
  AC_MSG_NOTICE([skipping some checks that require a native build])
fi

# Check if the compiler supports the `long long' type:
AC_CACHE_CHECK([for long long support in compiler],[gdb_cv_c_long_long],[
                AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
extern long long foo;
                ]],[[
switch (foo & 2) { case 0: return 1; }
                ]])],[gdb_cv_c_long_long=yes],
                     [gdb_cv_c_long_long=no])])
if test "x${gdb_cv_c_long_long}" = "xyes"; then
  AC_DEFINE([CC_HAS_LONG_LONG],[1],
            [Define to 1 if the compiler supports long long.])
elif "x${gdb_cv_c_long_long}" != "xyes"; then
  # libdecnumber requires long long.
  AC_MSG_WARN([Compiler should support long long for using GDB])
fi

dnl# compare against some pre-written macros for "long long":
AC_DEFUN([AC_REQUIRE_LONG_LONG_TYPES],[
  m4_ifdef([AC_TYPE_LONG_LONG_INT],[
    AC_REQUIRE([AC_TYPE_LONG_LONG_INT])dnl
  ],[
    test -z "${ac_cv_type_long_long_int}"
  ])dnl
  m4_ifdef([AC_TYPE_UNSIGNED_LONG_LONG_INT],[
    AC_REQUIRE([AC_TYPE_UNSIGNED_LONG_LONG_INT])dnl
  ],[
    test -z "${ac_cv_type_unsigned_long_long_int}"
  ])dnl
])dnl
AC_REQUIRE_LONG_LONG_TYPES dnl# might actually get done earlier...

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
  snprintf(buf, sizeof(buf), "0x%016llx", l);
  return (strcmp("0x0123456789abcdef", buf));]])],
                              [gdb_cv_printf_has_long_long=yes],
                              [gdb_cv_printf_has_long_long=no],
                              [gdb_cv_printf_has_long_long=no])])dnl
  ### ok
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
  snprintf(buf, sizeof(buf), "Decimal32: %H\nDecimal64: %D\nDecimal128: %DD", d32, d64, d128);
  return (strcmp("Decimal32: 1.2345\nDecimal64: 1.2345\nDecimal128: 1.2345", buf));]])],
                              [gdb_cv_printf_has_decfloat=yes],
                              [gdb_cv_printf_has_decfloat=no],
                              [gdb_cv_printf_has_decfloat=no])])dnl
  ## now define based on that:
if test "x${gdb_cv_printf_has_decfloat}" = "xyes"; then
  AC_DEFINE([PRINTF_HAS_DECFLOAT],[1],
    [Define to 1 if the "%H, %D and %DD" formats work to print decfloats.])
fi

# Check if the compiler supports the `long double' type. We cannot use
# AC\_C\_LONG\_DOUBLE because that one does additional checks on the
# constants defined in <float.h> that fail on some systems,
# e.g. FreeBSD/i386 4.7 and OpenBSD/i386 3.6.
# Anyways:
AC_CACHE_CHECK([for long double support in compiler],
               [gdb_cv_c_long_double],
               [AC_COMPILE_IFELSE([AC_LANG_SOURCE([[long double foo;]])],
                                  [gdb_cv_c_long_double=yes],
                                  [gdb_cv_c_long_double=no])])dnl
  ## now define based on that:
if test "x${gdb_cv_c_long_double}" = "xyes"; then
  AC_DEFINE([HAVE_LONG_DOUBLE],[1],
            [Define to 1 if the compiler supports long double.])dnl
  ## end def
fi

dnl# While we cannot use the macro mentioned above, we can use this one
dnl# separately; the resulting define should be non-conflicting:
m4_ifdef([AC_TYPE_LONG_DOUBLE_WIDER],[
  AC_TYPE_LONG_DOUBLE_WIDER
])dnl

# Check if the compiler and runtime support printing long doubles:
AC_CACHE_CHECK([for long double support in printf],
               [gdb_cv_printf_has_long_double],
               [AC_RUN_IFELSE([AC_LANG_PROGRAM([AC_INCLUDES_DEFAULT],
[[char buf[16];
  long double f = 3.141592653;
  snprintf(buf, sizeof(buf), "%Lg", f);
  return (strncmp("3.14159", buf, 7));]])],
                              [gdb_cv_printf_has_long_double=yes],
                              [gdb_cv_printf_has_long_double=no],
                              [gdb_cv_printf_has_long_double=no])])dnl
  ### ok
  # (note: Apple forces this value to be cached ahead of time; do NOT worry
  # about it printing that it is cached here)
if test "x${gdb_cv_printf_has_long_double}" = "xyes"; then
  AC_DEFINE([PRINTF_HAS_LONG_DOUBLE],[1],
            [Define to 1 if the "%Lg" format works to print long doubles.])
fi

# Check if the compiler and runtime support scanning long doubles:
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
                              [gdb_cv_scanf_has_long_double=no])])dnl
  ### ok
  # (note: Apple forces this value to be cached ahead of time; do NOT worry
  # about it printing that it is cached here)
if test "x${gdb_cv_scanf_has_long_double}" = "xyes"; then
  AC_DEFINE([SCANF_HAS_LONG_DOUBLE],[1],
            [Define to 1 if the "%Lg" format works to scan long doubles.])
fi

case ${host_os} in
aix*)
  AC_CACHE_CHECK([for -bbigtoc option],[gdb_cv_bigtoc],[
    SAVE_LDFLAGS="${LDFLAGS}"

    case ${GCC} in
    yes) gdb_cv_bigtoc=-Wl,-bbigtoc ;;
    *) gdb_cv_bigtoc=-bbigtoc ;;
    esac

    # why escape the space instead of just using quotes?
    LDFLAGS=${LDFLAGS}\ ${gdb_cv_bigtoc}
    AC_LINK_IFELSE([AC_LANG_PROGRAM([[]],[[int i;]])],
                   [],[gdb_cv_bigtoc=""])dnl
    ## restore:
    LDFLAGS="${SAVE_LDFLAGS}"
  ])dnl
  ## might look like we are restoring, but we are actually appending here:
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
AC_MSG_RESULT([${found}])
AC_SUBST([RDYNAMIC])dnl

dnl# For certain native configurations, we need to check whether thread
dnl# support can be built in or not.
dnl#
dnl# Note that we only want this if we are both native (host == target),
dnl# and not doing a canadian cross build (build == host).
dnl# Anyways:
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
                      [gdb_cv_proc_service_is_old=yes])dnl
	 ])dnl
         ## print:
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
                         [gdb_cv_have_aix_thread_debug=no])dnl
      ])dnl
      ## print:
      AC_MSG_RESULT([${gdb_cv_have_aix_thread_debug}])
      if test ${gdb_cv_have_aix_thread_debug} = yes; then
         CONFIG_SRCS="${CONFIG_SRCS} aix-thread.c"
         CONFIG_OBS="${CONFIG_OBS} aix-thread.o"
         CONFIG_LDFLAGS="${CONFIG_LDFLAGS} -lpthdebug"
      fi
      ;;
   darwin*)
      AC_MSG_NOTICE([skipping checks for thread library on darwin])
      ;;
   esac
   AC_SUBST([CONFIG_LDFLAGS])dnl
   ## 3...
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
           [gdb_cv_thread_db_h_has_td_notalloc=no])dnl
   ])dnl
   ## follow up with:
   AC_CACHE_CHECK([whether <thread_db.h> has TD_VERSION],
                  [gdb_cv_thread_db_h_has_td_version],
     [AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <thread_db.h>
      ]],[[
int i = TD_VERSION;
      ]])],[gdb_cv_thread_db_h_has_td_version=yes],
           [gdb_cv_thread_db_h_has_td_version=no])dnl
   ])dnl
   ## also this:
   AC_CACHE_CHECK([whether <thread_db.h> has TD_NOTLS],
                  [gdb_cv_thread_db_h_has_td_notls],
     [AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <thread_db.h>
      ]],[[
int i = TD_NOTLS;
      ]])],[gdb_cv_thread_db_h_has_td_notls=yes],
           [gdb_cv_thread_db_h_has_td_notls=no])dnl
   ])dnl
   ## should be enough
else
   AC_MSG_NOTICE([skipping some checks that require a thread_db header])
fi
if test "x${gdb_cv_thread_db_h_has_td_notalloc}" = "xyes"; then
  AC_DEFINE([THREAD_DB_HAS_TD_NOTALLOC],[1],
            [Define if <thread_db.h> has the TD_NOTALLOC error code.])dnl
  ## end def
fi
if test "x${gdb_cv_thread_db_h_has_td_version}" = "xyes"; then
  AC_DEFINE([THREAD_DB_HAS_TD_VERSION],[1],
            [Define if <thread_db.h> has the TD_VERSION error code.])dnl
  ## end def
fi
if test "x${gdb_cv_thread_db_h_has_td_notls}" = "xyes"; then
  AC_DEFINE([THREAD_DB_HAS_TD_NOTLS],[1],
            [Define if <thread_db.h> has the TD_NOTLS error code.])dnl
  ## end def
fi

dnl# See if we have a sys/syscall header file that has __NR_tkill.
if test "x${ac_cv_header_sys_syscall_h}" = "xyes"; then
   AC_CACHE_CHECK([whether <sys/syscall.h> has __NR_tkill],
                  [gdb_cv_sys_syscall_h_has_tkill],[
      AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <sys/syscall.h>
      ]],[[
int i = __NR_tkill;
      ]])],[gdb_cv_sys_syscall_h_has_tkill=yes],
           [gdb_cv_sys_syscall_h_has_tkill=no])dnl
   ])dnl
   ## end check
fi
dnl# See if we can issue tkill syscall.
if test "x${gdb_cv_sys_syscall_h_has_tkill}" = "xyes" && test "x${ac_cv_func_syscall}" = "xyes"; then
  AC_DEFINE([HAVE_TKILL_SYSCALL],[1],
            [Define if you support the tkill syscall.])dnl
  ## end def...
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
			      [have_personality=false])])dnl
  ## corresponding define:
if ${have_personality}
then
    AC_DEFINE([HAVE_PERSONALITY],[1],
	      [Define if you support the personality syscall.])dnl
    ## 4...
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
AC_DEFINE_UNQUOTED([GDBINIT],["${gdbinit}"],[The .gdbinit filename.])dnl

dnl# Handle optional features that can be enabled.

AC_ARG_WITH([sysroot],
            [AS_HELP_STRING([--with-sysroot[=DIR]],
                            [Search for usr/lib et al within DIR.])],[
 case ${with_sysroot} in
 yes) TARGET_SYSTEM_ROOT='${exec_prefix}/${target_alias}/sys-root' ;;
 *) TARGET_SYSTEM_ROOT=${with_sysroot} ;;
 esac

 TARGET_SYSTEM_ROOT_DEFINE='-DTARGET_SYSTEM_ROOT=\"$(TARGET_SYSTEM_ROOT)\"'

 if test "x${exec_prefix}" = "xNONE"; then
  if test "x${prefix}" = "xNONE"; then
   test_prefix=/usr/local
  else
   test_prefix="${prefix}"
  fi
 else
  test_prefix="${exec_prefix}"
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
])dnl
AC_SUBST([TARGET_SYSTEM_ROOT])dnl
AC_SUBST([TARGET_SYSTEM_ROOT_DEFINE])dnl

###########################################################################
#                      COMPILER WARNINGS SECTION                          #
###########################################################################

# NOTE: Do NOT add -Wall or -Wunused, they both include
# -Wunused-parameter which reports bogus warnings.
## (they are easy enough to fix though; just put ATTRIBUTE_UNUSED
##  everywhere...)
# NOTE: If you add to this list, remember to update
# gdb/doc/gdbint.texinfo.
build_warnings="-Wreturn-type -Wcomment -Wtrigraphs \
-Wparentheses -Wpointer-arith -Wchar-subscripts \
-Wunused-value -Wunused-label -Wunused-result \
-Wunused-variable -Wunused-but-set-variable -Wunused-but-set-parameter \
-Wunused-function -Wempty-body -Wmissing-declarations \
-Wignored-qualifiers -Wmissing-format-attribute -Waddress -Wattributes \
-Wignored-attributes -Wswitch -Wswitch-default -Wclobbered \
-Wcoverage-mismatch -Wdiv-by-zero -Wdouble-promotion \
-Wfree-nonheap-object -Wrestrict -Wsign-compare -Wpointer-compare \
-Winvalid-memory-model -Winvalid-pch -Wmain -Wmissing-braces \
-Wmissing-include-dirs -Woverflow -Woverlength-strings -Wpragmas \
-Wreturn-local-addr -Wsequence-point \
-Wsizeof-pointer-div -Wsizeof-pointer-memaccess \
-Wtrampolines -Wunknown-pragmas -Wnormalized=nfkc -Wlogical-op \
-Wfloat-conversion -Wmissing-field-initializers \
-Wno-deprecated-declarations -Wbool-compare -Wlogical-not-parentheses \
-Wsizeof-array-argument -Wswitch-bool -Wshift-count-negative \
-Wshift-count-overflow -Wshift-negative-value -Wshift-overflow=2 \
-Wmisleading-indentation -Wlto-type-mismatch -Wtautological-compare \
-Wduplicated-cond -Wwrite-strings -Wenum-compare -Wnonnull-compare \
-Wdangling-else -Wmemset-elt-size -Wswitch-unreachable -Whsa \
-Wscalar-storage-order -Wimplicit-fallthrough=3 \
-Wformat-overflow=2 -Wformat-truncation=2 \
-Wexpansion-to-defined -Wint-in-bool-context -Wbool-operation \
-Wduplicated-branches -Wstringop-overflow=2 \
-Wenum-conversion -Wbuiltin-declaration-mismatch \
-Walloc-zero -Walloc-size-larger-than=2147483647 \
-Wmultistatement-macros -Wpacked-not-aligned"
link_warnings=""

if test "x${enable_build_with_cxx}" = "xyes"; then
  ## Special warnings only for C++ compilations:
  cxx_warnings="-Wabi -Wabi-tag -Woverloaded-virtual -Wsign-promo \
-Wsubobject-linkage -Wctor-dtor-privacy -Wterminate -Wc++11-compat \
-Wdelete-incomplete -Wdelete-non-virtual-dtor -Wglobal-constructors \
-Winherited-variadic-ctor -Winvalid-offsetof -Wliteral-suffix -Wnoexcept \
-Wnon-template-friend -Wnon-virtual-dtor -Wreorder -Wstrict-null-sentinel \
-Wmultiple-inheritance -Wvirtual-inheritance -Wpmf-conversions \
-Wtemplates -Wvirtual-move-assign -Wconversion-null -Weffc++ \
-Wsized-deallocation -Wplacement-new=2 -Wshadow-compatible-local \
-Wbuiltin-function-redefined -Waligned-new=all -Wsynth -Wcatch-value \
-Wclass-memaccess -Wnon-pod-varargs"
  build_warnings="${build_warnings} ${cxx_warnings} -fpermissive"
else
  c_warnings="-Wimplicit -Wunsuffixed-float-constants -Wjump-misses-init \
-Wpointer-sign -Wmissing-prototypes -Wdeclaration-after-statement \
-Woverride-init -Woverride-init-side-effects -Wduplicate-decl-specifier \
-Wmissing-parameter-type -Wold-style-declaration -Wold-style-definition \
-Wstrict-prototypes -Wnested-externs -Wc99-c11-compat -fno-common"
  if test -f /usr/X11/include/X11/Xlib.h && test "x${enable_gdbtk}" = "xyes"; then
    AC_MSG_WARN([Disabling C++ compatibility warnings for gdbtk])
  else
    c_warnings="${c_warnings} -Wc++-compat"
  fi
  build_warnings="${build_warnings} ${c_warnings}"
fi

# If compiler does NOT support the long long data type, see if it can
# at least warn about it:
if test "x${gdb_cv_c_long_long}" != "xyes"; then
  build_warnings="${build_warnings} -Wlong-long"
fi
# similarly with VLAs:
if test "x${ac_cv_c_vararrays}" != "xyes" || test "x${enable_build_with_cxx}" = "xyes"; then
  build_warnings="${build_warnings} -Wvla"
elif test "x${ac_cv_c_vararrays}" = "xyes"; then
  build_warnings="${build_warnings} -Wvla-larger-than=9999"
fi
# ...and alloca:
if test "x${ac_cv_func_alloca_works}" = "xno"; then
  build_warnings="${build_warnings} -Walloca"
elif test "x${ac_cv_func_alloca_works}" = "xyes"; then
  if test "x${AVOID_LARGE_ALLOCAS}" = "xyes"; then
    # 4032 is MAX_ALLOCA_SIZE in "libiberty.h":
    build_warnings="${build_warnings} -Walloca-larger-than=4032"
  fi
fi

# clang-specific warnings are now moved to after the host-specific ones.

dnl# (we now ensure that 'acl_cv_wl' is set elsewhere above)

case "${host}" in
  *-apple-darwin* | *-apple-macos*)
    stdflag=""
    if test "x${enable_build_with_cxx}" = "xyes"; then
      stdflag="-std=gnu++03"
    else
      stdflag="-std=gnu99"
    fi
    # -Wmost is APPLE_LOCAL, as are -Wextra-tokens and -Wnewline-eof
    # (-Wmost might have to be removed for the same reason as -Wall)
    # -Wnonportable-cfstrings is actually a flag for newer versions of the
    # FSF gcc, but it only makes sense on Apple platforms.
    build_warnings="${build_warnings} -Wmost -Wextra-tokens -Wnewline-eof \
-Wendif-labels -Wformat -Wformat-y2k -Wformat-security -Wnonnull \
-Wnonportable-cfstrings -Wno-unused-const-variable ${stdflag}"
    case "${host}" in
      *64*-apple-darwin9* | *64*-apple-darwin1[[0123456789]].*.*)
        build_warnings="${build_warnings} -Wshorten-64-to-32"
      ;;
    esac
    link_warnings="${link_warnings} ${acl_cv_wl}-warn_compact_unwind \
${acl_cv_wl}-warn_weak_exports ${acl_cv_wl}-warn_stabs \
${acl_cv_wl}-warn_commons"
    # also consider: "${acl_cv_wl}-why_load"
  ;;
  *-gnu*-* | gnu*-*-* | *-*-gnu*)
    #FIXME: these flags are untested, and are only based on my
    # my reading of the GNU ld manpage:
    link_warnings="${link_warnings} ${acl_cv_wl}--warn-alternate-em \
${acl_cv_wl}--warn-common ${acl_cv_wl}--warn-constructors \
${acl_cv_wl}--warn-multiple-gp ${acl_cv_wl}--warn-section-align"
  ;;
  *-*-mingw32*)
    # Enable -Wno-format by default when using gcc on mingw since many
    # GCC versions complain about %I64:
    build_warnings="${build_warnings} -Wno-format"
  ;;
  *)
    build_warnings="${build_warnings} -Wformat -Wformat-nonliteral"
  ;;
esac

if test "x${enable_64_bit_bfd}" = "xyes"; then
  build_warnings="${build_warnings} -Wtype-limits"
fi

# clang-specific warnings:
if test "x${ax_cv_c_compiler_vendor}" = "xclang"; then
  build_warnings="${build_warnings} -Wgcc-compat -Wasm \
-Wunneeded-internal-declaration -Wstring-plus-int -Wstring-conversion \
-Wbool-conversion -Wconstant-logical-operand -Wunused-comparison \
-Wabsolute-value -Wtautological-constant-out-of-range-compare \
-Wunevaluated-expression -Wextra-semi -Wunsequenced -Wself-assign \
-Wshift-sign-overflow -Wstatic-in-inline -Wparentheses-equality \
-Wincompatible-pointer-types-discards-qualifiers -Winvalid-noreturn"
  if test "x${enable_build_with_cxx}" = "xyes"; then
    build_warnings="${build_warnings} -Wmismatched-tags \
-Wmissing-exception-spec -Wpessimizing-move -Wredundant-move \
-Wnon-literal-null-conversion -Wpredefined-identifier-outside-function \
-Wvexing-parse -Wreturn-stack-address -Wshadow-field-in-constructor \
-Wunused-private-field -Wvla-extension"
  fi
  ## mig typechecks cause paramaters that would otherwise be unused to be
  ## used, triggering this:
  if test "x${MIG_CHECKING}" = "x"; then
    test -z "${MIG_CHECKING}"
    build_warnings="${build_warnings} -Wused-but-marked-unused"
  fi
  ## the clang version of this warning is un-pluralized, unlike in GCC:
  build_warnings="${build_warnings} -Wno-unused-local-typedef"
else
  ## really only noisy with clang:
  build_warnings="${build_warnings} -Wcast-align"
fi

# GCC supports -Wuninitialized only with -O or -On, n != 0.
# ...and also -Winit-self only works when -Wuninitialized is also used.
# -Wmissing-noreturn requires -O or higher, or -fipa-pure-const to work.
# Others in this section are technically supported at other optimization
# levels, but really only make sense with it on.
opt_warnings="-Wuninitialized -Winit-self -Wmissing-noreturn \
-Wvolatile-register-var -Wdisabled-optimization -Winline \
-Waggressive-loop-optimizations -Wvector-operation-performance \
-Wmaybe-uninitialized -Wstrict-overflow=1 -fno-strict-aliasing \
-Wnull-dereference -Wframe-address -Wframe-larger-than=262144"
if test "x${ax_cv_c_compiler_vendor}" != "xclang"; then
  opt_warnings="${opt_warnings} -Wsuggest-attribute=const \
-Wsuggest-attribute=cold"
fi
# -Warray-bounds is only for -O2 and up, or when -ftree-vrp is active, so
# use a separate variable for it:
if test "x${enable_build_with_cxx}" = "xyes"; then
  opt_warnings_plus="${opt_warnings} -Warray-bounds"
else
  opt_warnings_plus="${opt_warnings} -Warray-bounds=2"
fi
if test x${CFLAGS+set} = xset; then
  case "${CFLAGS}" in
    *"-O0"* )
      ## we will miss out on warnings about signed integer overflow in this
      ## case, so turn them into runtime errors instead, for debugging:
      build_warnings="${build_warnings} -ftrapv"
      AC_DEFINE([OPTIMIZATION_LEVEL],[0],
                [0 is none, 1 is some, and 2 is anything higher])
    ;;
    *"-O2"* | *"-O3"* | *"-Ofast"* | *"-ftree-vrp"* )
      build_warnings="${build_warnings} ${opt_warnings_plus}"
      AC_DEFINE([OPTIMIZATION_LEVEL],[2],
                [0 is none, 1 is some, and 2 is anything higher])
      case "${CFLAGS}" in
        *"-Ofast"* )
          ## Work around GCC bug 71009:
          build_warnings="${build_warnings} -fmath-errno"
          ## Not warnings, but from GCC testsuite:
          build_warnings="${build_warnings} -funroll-loops -ftracer"
          ## Other optimizations:
          build_warnings="${build_warnings} -fgcse-sm -fgcse-las -fipa-pta"
        ;;
      esac
    ;;
    *"-O "* | *"-O1"* | *"-Og"* )
      build_warnings="${build_warnings} ${opt_warnings}"
      AC_DEFINE([OPTIMIZATION_LEVEL],[1],
                [0 is none, 1 is some, and 2 is anything higher])
    ;;
    *"-Os"* | *"-Oz"* )
      build_warnings="${build_warnings} -Wlarger-than=262144"
      ## Not all of these are warnings, but since we care about code size here:
      build_warnings="${build_warnings} -Wframe-larger-than=65536 \
-finline-limit=4 -Winline -fno-keep-static-functions -fno-keep-static-consts \
-fno-keep-inline-functions -finline-small-functions -flimit-function-alignment"
      AC_DEFINE([OPTIMIZATION_LEVEL],[1],
                [0 is none, 1 is some, and 2 is anything higher])
    ;;
    *"-fstack-protector"* )
      build_warnings="${build_warnings} -Wstack-protector"
      AC_DEFINE([HAVE_STACK_PROTECTION],[1],[1 if the stack is protected])
    ;;
    *"-flto-odr-type-merging"* )
      build_warnings="${build_warnings} -Wodr"
      AC_DEFINE([HAVE_LTO_ODR_TYPE_MERGING],[1],[1 if... yeah.])
    ;;
  esac
else
  build_warnings="${build_warnings} ${opt_warnings}"
fi

# Up for debate (i.e. for debating adding to existing flags): 
#  * -trigraphs (only assuming we leave -Wtrigraphs in, see below)
#  * -Wshadow (noisy, but usually easy enough to fix if you use an IDE)
#  * -Wcast-qual (casting can silence other warnings, so this can conflict)
#  * -Wbad-function-cast (likewise) (C-only, not C++)
#  * -Wconversion (would require a lot of casting)
#  * -Wsign-conversion (likewise)
#  * -Wtraditional-conversion (likewise) (C-only, not C++)
#  * -Wredundant-decls (I like redundant declarations & dislike this flag)
#  * -Wundef (only if we can figure out how to fix the issue with caddr_t)
#  * -Wunused-macros (if we also add back -Wundef as per above)
#  * -Wunsafe-loop-optimizations (lex/yacc generate code that triggers it)
#  * -Waggregate-return (too noisy and hard to avoid)
#  * -Wformat-signedness (too noisy)
#  * -Wpadded (warnings in headers out of our control)
#  * -Wsuggest-attribute=pure (less obvious than its const counterpart)
#  * -Wstack-usage=262144 (as in bfd/binutils)
#  * -Wconditionally-supported (for C++)
#  * -Wunused-local-typedefs (`mig` sometimes generates code triggering
#    this, but it is inconsistent though)
# Up for debate (i.e. for debating removing from existing flags):
#  * -Wcomment (if we just switch to -Wall instead)
#  * -Wtrigraphs (might need -trigraphs to be added to stay in, see above)
#  * -Wunused-value (dummy workaround method for this is dumb)
#  * -Wunused-but-set-variable (likewise)
#  * -Wstrict-prototypes (would already do similar checks anyways)
#  * -Wmissing-noreturn (might want to use the newer version instead)
#  * -Wvolatile-register-var (kinda pointless)
#  * -Wswitch (I like this one, but others might not)
#  * -Wc++-compat (noisy in some headers, but upstream is moving to C++)
#  * -Wunsuffixed-float-constants (broken for regular doubles, which
#    require a non-standard suffix)
#  * -Wno-deprecated-declarations (I recently marked a bunch of functions
#    as deprecated, but that made things too noisy... will eventually want
#    the warnings back though.)
#  * -Wformat (we used to explicitly disable this; but it was required for
#    others such as -Wformat-nonliteral, so I added it back)
#  * -Wnested-externs (if it gets redundant with -Wimplicit again)
#  * -Wwrite-strings (in case all the const-ifying gets to be too much)
# Up for debate in some other way:
#  * -Wstrict-overflow (i.e., could change the value assigned to it. We use
#    "1" now because that is the default for "-Wall", but "2" is the
#    default when no value is used with it with an "=", and the max is "5")
#  * -Wabi (currently only used for C++, but sometimes accepted for just C)
#  * -Wdouble-promotion (could be moved to the same place as -Wlong-long if
#    we get rid of -Wunsuffixed-float-constants some time...)
#  * -Wformat-nonliteral (as to the conditions in which we enable it)
#  * -Wcast-align (as to whether to un-disable for clang as well)
#  * -Wtype-limits (as to whether to also use it for 32-bit BFD)
# NOT actually up for debate:
#  * -Werror (never actually add this, even though upstream GDB does...)
#  * -pedantic-errors (likewise, never add this)
#  * -Wunused-parameter (despite the existence of clang's
#    -Wused-but-marked-unused flag, upstream still insists on leaving this
#    one out, presumably because gcc does not support the other one yet...)
#  * -Wall (contains -Wunused-parameter)
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
  echo "Setting compiler warning flags to attempt to use = ${build_warnings}" 6>&1
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
  echo "Setting GDB specific compiler warning flags to try to use = ${build_warnings}" 6>&1
fi])dnl

# The set of warnings supported by a C++ compiler is not the same as
# of the C compiler:
if test "x${enable_build_with_cxx}" = "xyes"; then
    AC_LANG_PUSH([C++])
fi

WARN_CFLAGS=""
BAD_WARN_CFLAGS=""
WERROR_CFLAGS=""
if test "x${build_warnings}" != "x" -a "x${GCC}" = "xyes"
then
    AC_MSG_CHECKING([compiler warning flags])
    AC_MSG_RESULT([(will report full results at end)])
    # Separate out the -Werror flag as some files just cannot be
    # compiled with it enabled.
    # Do need to use it for these conftests though.
    for w in ${build_warnings}; do
        AC_MSG_CHECKING([if compiler supports ${w}])
	case ${w} in
          -Wold-style-definition|-Wstrict-prototypes|-Wsuggest-attribute=const)
	    saved_CFLAGS="${CFLAGS}"
	    CFLAGS="${CFLAGS} ${w} -Werror"
            dnl# Use "_SOURCE" instead of "_PROGRAM" here to keep us from
            dnl# having a bad "main()" function:
	    AC_COMPILE_IFELSE([AC_LANG_SOURCE([[]])],
                              [AC_MSG_RESULT([yes])
                               WARN_CFLAGS="${WARN_CFLAGS} ${w}"],
                              [AC_MSG_RESULT([no])
                               BAD_WARN_CFLAGS="${BAD_WARN_CFLAGS} ${w}"])
	    CFLAGS="${saved_CFLAGS}"
            ;;
          -Wformat-nonliteral|-Wformat-signedness|-Wformat-y2k|-Wformat-security)
            ## (These would be ignored without also using -Wformat)
	    saved_CFLAGS="${CFLAGS}"
	    CFLAGS="${CFLAGS} ${w} -Werror -Wformat"
	    saved_CXXFLAGS="${CXXFLAGS}"
	    CXXFLAGS="${CXXFLAGS} ${w} -Werror -Wformat"
	    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[]],[[]])],
                              [AC_MSG_RESULT([yes])
                               WARN_CFLAGS="${WARN_CFLAGS} ${w}"],
                              [AC_MSG_RESULT([no])
                               BAD_WARN_CFLAGS="${BAD_WARN_CFLAGS} ${w}"])
	    CFLAGS="${saved_CFLAGS}"
	    CXXFLAGS="${saved_CXXFLAGS}"
            ;;
	  *) # Check that GCC accepts it:
	    saved_CFLAGS="${CFLAGS}"
	    CFLAGS="${CFLAGS} ${w} -Werror"
	    saved_CXXFLAGS="${CXXFLAGS}"
	    CXXFLAGS="${CXXFLAGS} ${w} -Werror"
	    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[]],[[]])],
                              [AC_MSG_RESULT([yes])
                               WARN_CFLAGS="${WARN_CFLAGS} ${w}"],
                              [AC_MSG_RESULT([no])
                               BAD_WARN_CFLAGS="${BAD_WARN_CFLAGS} ${w}"])
	    CFLAGS="${saved_CFLAGS}"
	    CXXFLAGS="${saved_CXXFLAGS}"
            ;;
	esac
    done
    AC_MSG_RESULT([using: ${WARN_CFLAGS}${WERROR_CFLAGS}])
    if test "x${BAD_WARN_CFLAGS}" != "x"; then
      test -n "${BAD_WARN_CFLAGS}"
      AC_MSG_WARN([compiler failed to accept: ${BAD_WARN_CFLAGS}])
    fi
fi
AC_SUBST([WARN_CFLAGS])dnl
AC_SUBST([WERROR_CFLAGS])dnl

if test "x${enable_build_with_cxx}" = "xyes"; then
   AC_LANG_POP([C++])
fi

WARN_LDFLAGS=""
if test "x${link_warnings}" != "x" -a "x${GCC}" = "xyes"
then
    AC_MSG_CHECKING([linker warning flags])
    # Leave in -Werror just for these conftests.
    for w in ${link_warnings}; do
	case ${w} in
	*) # Check that the compiler successfully passes it to the linker.
	    saved_LDFLAGS="${LDFLAGS}"
	    LDFLAGS="${LDFLAGS} ${w} -Werror"
	    AC_LINK_IFELSE([AC_LANG_PROGRAM([[]],[[]])],
                           [WARN_LDFLAGS="${WARN_LDFLAGS} ${w}"],[])
	    LDFLAGS="${saved_LDFLAGS}"
	esac
    done
    if test "x${WERROR_CFLAGS}" != "x"; then
      test -n "${WERROR_CFLAGS}"
      if test "x${LD_ARCHFLAGS}" != "x"; then
        test -n "${LD_ARCHFLAGS}"
        if test "x${WARN_LDFLAGS}" != "x"; then
          test -n "${WARN_LDFLAGS}" && WARN_LDFLAGS="${WARN_LDFLAGS} ${LD_ARCHFLAGS}"
        else
          test -z "${WARN_LDFLAGS}" && WARN_LDFLAGS="${LD_ARCHFLAGS}"
        fi
      fi
    fi
    AC_MSG_RESULT([${WARN_LDFLAGS}])
fi
AC_SUBST([WARN_LDFLAGS])dnl

###########################################################################
## section for checking mmalloc usage:
MMALLOC_CFLAGS=""
MMALLOC=""
AC_SUBST([MMALLOC_CFLAGS])dnl
AC_SUBST([MMALLOC])dnl

# APPLE LOCAL: We (used to) need to use mmalloc all the time on our
# platform.
case "${host}" in
  *-apple-darwin[[0156789]][[,.]]* | *-apple-macos*)
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

if test "x${want_mmalloc}" = "xtrue" && test -d ../mmalloc; then
   AC_DEFINE([USE_MMALLOC],[1],[Define to 1 to use mmalloc])
   AC_DEFINE([MMCHECK_FORCE],[1],[Define to 1 to force mmcheck])
   MMALLOC_CFLAGS="-I${srcdir}/../mmalloc -I../mmalloc"
   MMALLOC='../mmalloc/libmmalloc.a'
   if test ! -e ${MMALLOC}; then
     AC_MSG_WARN([${MMALLOC} is wanted, but missing])
   fi
fi

# In the Cygwin environment, we need some additional flags.
AC_CACHE_CHECK([for cygwin],[gdb_cv_os_cygwin],
[AC_EGREP_CPP([you_lose],[
#if defined(__CYGWIN__) || defined(__CYGWIN32__)
you_lose
#endif /* __CYGWIN__ || __CYGWIN32__ */
],[gdb_cv_os_cygwin=yes],[gdb_cv_os_cygwin=no])])dnl

dnl# Figure out which of the many generic ser-*.c files the _host_
dnl# supports.
SER_HARDWIRE="ser-base.o ser-unix.o ser-pipe.o ser-tcp.o"
case ${host} in
  *go32* ) SER_HARDWIRE=ser-go32.o ;;
  *djgpp* ) SER_HARDWIRE=ser-go32.o ;;
  *mingw32*) SER_HARDWIRE="ser-base.o ser-tcp.o" ;;
  *e7kpc*|*e7000pc*) SER_HARDWIRE=ser-e7kpc.o ;;
esac
AC_SUBST([SER_HARDWIRE])dnl

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
AC_SUBST([WIN32LIBS])dnl

m4_ifdef([GDB_AC_CHECK_BFD],[
  # Add ELF support to GDB, but only if BFD includes ELF support:
  GDB_AC_CHECK_BFD([for ELF support in BFD],[gdb_cv_var_elf],
                   [bfd_get_elf_phdr_upper_bound(NULL)],[elf-bfd.h])
  if test "x${gdb_cv_var_elf}" = "xyes"; then
    CONFIG_OBS="${CONFIG_OBS} elfread.o stap-probe.o"
    AC_DEFINE([HAVE_ELF],[1],
  	      [Define if ELF support should be included.])
    # -ldl is provided by bfd/Makfile.am (LIBDL) <PLUGINS>.
    if test "x${plugins}" = "xyes" && test "x${ac_cv_search_dlopen}" = "x"; then
      test -z "${ac_cv_search_dlopen}"
      AC_SEARCH_LIBS([dlopen],[dl zrldsrl])
    fi
  fi

  # Add macho support to GDB, but only if BFD includes it:
  GDB_AC_CHECK_BFD([for Mach-O support in BFD],[gdb_cv_var_macho],
                   [bfd_mach_o_lookup_command(NULL, 0, NULL)],[mach-o.h])
  if test "x${gdb_cv_var_macho}" = "xyes"; then
    if test -e ./machoread.c; then
      CONFIG_OBS="${CONFIG_OBS} machoread.o"
      AC_DEFINE([HAVE_MACH_O_IN_BFD],[1],
                [Define if mach-o support should be included.])
    else
      AC_MSG_NOTICE([want to build machoread.o, but no machoread.c in current directory])
    fi
  fi

  # Add SOM support to GDB, but only if BFD includes it:
  GDB_AC_CHECK_BFD([for SOM support in BFD],[gdb_cv_var_som],
                   [bfd_som_attach_aux_hdr(NULL, 0, NULL)],[som.h])
  if test "x${gdb_cv_var_som}" = "xyes"; then
    CONFIG_OBS="${CONFIG_OBS} somread.o"
  fi
],[
  test -z "${gdb_cv_var_elf}"
  test -z "${gdb_cv_var_macho}"
  test -z "${gdb_cv_var_som}"
  AC_MSG_WARN([skipping checks for what formats bfd supports.])dnl
])dnl##end ifdef

if test "x${gdb_host_obs}" != "x"; then
  # Add any host-specific objects to GDB.
  test -n "${gdb_host_obs}" && export CONFIG_OBS="${CONFIG_OBS} ${gdb_host_obs}"
fi

# If building on ELF, look for lzma support for embedded compressed debug
# info:
if test "x${gdb_cv_var_elf}" = "xyes"; then
  AC_ARG_WITH([lzma],
    [AS_HELP_STRING([--with-lzma],
                    [support lzma compression (auto/yes/no)])],
    [],[with_lzma=auto])dnl
  ## see what we were passed:
  AC_MSG_CHECKING([whether to use lzma])
  AC_MSG_RESULT([${with_lzma}])dnl

  if test "x${with_lzma}" != "xno"; then
    dnl# C source code is only singly-m4-quoted in args to this macro:
    AC_LIB_HAVE_LINKFLAGS([lzma],[],[#include "lzma.h"],
			  [lzma_index_iter iter;
			   lzma_index_iter_init(&iter, 0);
			   lzma_mf_is_supported(LZMA_MF_HC3);])
    if test "x${HAVE_LIBLZMA}" != "xyes"; then
      if test "x${with_lzma}" = "xyes"; then
        AC_MSG_ERROR([missing liblzma for --with-lzma])
      fi
    fi
  fi
fi

LIBGUI="../libgui/src/libgui.a"
GUI_CFLAGS_X="-I${srcdir}/../libgui/src"
AC_SUBST([LIBGUI])dnl
AC_SUBST([GUI_CFLAGS_X])dnl

WIN32LDAPP=""
AC_SUBST([WIN32LIBS])dnl
AC_SUBST([WIN32LDAPP])dnl

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
           else
             AC_MSG_WARN([cannot load itclConfig.sh; it is missing])
           fi

	   # now look for Itk library stuff:
	   CY_AC_PATH_ITKCONFIG
           if test -z "${no_itk}"; then
             CY_AC_LOAD_ITKCONFIG

	     ITKLIB="${ITK_BUILD_LIB_SPEC}"
	     ITK_DEPS="${ITK_LIB_FULL_PATH}"
           else
             AC_MSG_WARN([cannot load itkConfig.sh; it is missing])
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

	   if test "x${gdb_cv_os_cygwin}" = "xyes"; then
	      WIN32LIBS="${WIN32LIBS} -lshell32 -lgdi32 -lcomdlg32 -ladvapi32"
	      WIN32LDAPP="-Wl,--subsystem,console"
	      CONFIG_OBS="${CONFIG_OBS} gdbres.o"
	   fi
        else
           AC_MSG_WARN([skipping any further checks for tk, because it is missing (gdbtk will therefore not be built)])
	fi
    else
        AC_MSG_WARN([skipping any further checks for tcltk, because tcl is missing (gdbtk will therefore not be built)])
    fi

    AC_CONFIG_SUBDIRS([gdbtk])
else
    AC_MSG_NOTICE([skipping gdbtk tests])
    if test -d gdbtk; then
        export unused_subdirs="${unused_subdirs} gdbtk"
    fi
fi

AC_SUBST([X_CFLAGS])dnl
AC_SUBST([X_LDFLAGS])dnl
AC_SUBST([X_LIBS])dnl
AC_SUBST([TCL_DEPS])dnl
AC_SUBST([TK_DEPS])dnl
AC_SUBST([ITCLLIB])dnl
AC_SUBST([ITCL_DEPS])dnl
AC_SUBST([ITKLIB])dnl
AC_SUBST([ITK_DEPS])dnl
AC_SUBST([GDBTKLIBS])dnl
AC_SUBST([GDBTK_CFLAGS])dnl
AC_SUBST([GDBTK_SRC_DIR])dnl

unset ac_cv_func_gethostbyname
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
[ignore_sim=false])dnl

if test ! -d "${srcdir}/../sim" || test ! -d "${srcdir}/../sim/${target_cpu}"; then
  if test "x${ignore_sim}" = "xfalse"; then
    AC_MSG_WARN([disabling simulator support])
  fi
  ignore_sim=true
fi

if test "x${ignore_sim}" = "xtrue"; then
    IGNORE_SIM="SIM="
    IGNORE_SIM_OBS="SIM_OBS="
else
    if test x"${gdb_sim}" != x""; then
      GDB_SIM="${gdb_sim}"
    fi
    DEFAULT_SIM_OB="remote-sim.o"
    AC_SUBST([DEFAULT_SIM_OB])dnl
    ## clear:
    IGNORE_SIM=""
    IGNORE_SIM_OBS=""
    AC_DEFINE([WITH_SIM],[1],[Define if the simulator is being linked in.])
fi
AC_SUBST([GDB_SIM])dnl
AC_SUBST([IGNORE_SIM])dnl
AC_SUBST([IGNORE_SIM_OBS])dnl

AC_SUBST([ENABLE_CFLAGS])dnl
AC_SUBST([ENABLE_LDFLAGS])dnl

AC_SUBST([PROFILE_CFLAGS])dnl

AC_SUBST([CONFIG_OBS])dnl
AC_SUBST([CONFIG_DEPS])dnl
AC_SUBST([CONFIG_SRCS])dnl
AC_SUBST([CONFIG_ALL])dnl
AC_SUBST([CONFIG_CLEAN])dnl
AC_SUBST([CONFIG_INSTALL])dnl
AC_SUBST([CONFIG_UNINSTALL])dnl

# List of host floatformats.
AC_DEFINE_UNQUOTED([GDB_HOST_FLOAT_FORMAT],[${gdb_host_float_format}],
                   [Host float floatformat])dnl
AC_DEFINE_UNQUOTED([GDB_HOST_DOUBLE_FORMAT],[${gdb_host_double_format}],
                   [Host double floatformat])dnl
AC_DEFINE_UNQUOTED([GDB_HOST_LONG_DOUBLE_FORMAT],
                   [${gdb_host_long_double_format}],
                   [Host long double floatformat])dnl

# target_subdir is used by the testsuite to find the target libraries.
target_subdir=""
if test "${host}" != "${target}"; then
    target_subdir="${target_alias}/"
fi
AC_SUBST([target_subdir])dnl

frags=""
if test "${target}" = "${host}"; then
  host_makefile_frag=${srcdir}/config/${gdb_host_cpu}/${gdb_host}.mh
  if test ! -f ${host_makefile_frag}; then
    AC_MSG_WARN([failed to find ${host_makefile_frag}])
    AC_MSG_ERROR([*** Gdb does not support native target ${host}])
  fi
  frags="${frags} ${host_makefile_frag}"
else
  host_makefile_frag=/dev/null
fi

target_makefile_frag=${srcdir}/config/${gdb_target_cpu}/${gdb_target}.mt
if test ! -f ${target_makefile_frag}; then
  AC_MSG_WARN([failed to find ${target_makefile_frag}])
  AC_MSG_ERROR([*** Gdb does not support target ${target}])
fi
frags="${frags} ${target_makefile_frag}"

AC_SUBST_FILE([host_makefile_frag])dnl
AC_SUBST_FILE([target_makefile_frag])dnl
AC_SUBST([frags])dnl

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

if test x"${gdb_osabi}" != x""; then
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
])dnl

if test "x${enable_multi_ice}" = "xyes"; then
   if test -d multi-ice; then
     AC_CONFIG_SUBDIRS([multi-ice])
     test -r multi-ice && test -w multi-ice && stat multi-ice
   else
     AC_MSG_WARN([multi-ice is requested but the subdir for it is missing])
   fi
else
   if test -d multi-ice; then
     AC_MSG_NOTICE([multi-ice subdir present, but we are not using it.])
     export unused_subdirs="${unused_subdirs} multi-ice"
   fi
fi

if test "x${enable_multi_ice}" != "xno"; then
   if test -d rem-multi_old; then
     AC_CONFIG_SUBDIRS([rem-multi_old])
     test -r rem-multi_old && test -w rem-multi_old && stat rem-multi_old
   elif test -d rem-multi; then
     AC_MSG_WARN([rem-multi remains un-renamed])
   fi
else
   if test -d rem-multi_old; then
     export unused_subdirs="${unused_subdirs} rem-multi_old"
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
    ##FIXME: sometimes we fail to recurse into it?
  else
    AC_MSG_RESULT([no])
    if test -d gdbserver; then
      export unused_subdirs="${unused_subdirs} gdbserver"
    fi
  fi
else
  if test -d gdbserver; then
    AC_MSG_NOTICE([gdbserver subdir present, but we are not using it.])
    export unused_subdirs="${unused_subdirs} gdbserver"
  fi
fi

# If the user explicitly request the gdbserver to be built, verify that
# we were in fact able to enable it.
if test "x${enable_gdbserver}" = "xyes" -a "x${gdbserver_build_enabled}" != "yes"; then
  AC_MSG_WARN([Automatic gdbserver build is not supported for this configuration])
fi

# Check for babeltrace and babeltrace-ctf:
AC_ARG_WITH([babeltrace],
  [AS_HELP_STRING([--with-babeltrace],
                  [include babeltrace support (auto/yes/no)])],
  [],[with_babeltrace=auto])dnl
  ## see what we were passed:
AC_MSG_CHECKING([whether to use babeltrace])
AC_MSG_RESULT([${with_babeltrace}])dnl

if test "x${with_babeltrace}" = "xno"; then
  # Upstream libbabeltrace is hosted on a Linux-specific website:
  case "${host}" in
    *-*-linux*)
      AC_MSG_WARN([babeltrace support disabled; GDB is unable to read CTF data.])
      ;;
    *)
      AC_MSG_WARN([babeltrace support disabled; please port libbabeltrace to your platform (${host}).])
      ;;
  esac
else
  # Append -Werror to CFLAGS so that configure can catch the warning
  # "assignment from incompatible pointer type", which is related to
  # the babeltrace change from 1.0.3 to 1.1.0.  Babeltrace 1.1.0 works
  # in GDB, while babeltrace 1.0.3 is broken.
  # AC_LIB_HAVE_LINKFLAGS may modify CPPFLAGS in it, so it should be
  # safe to save and restore CFLAGS here.
  saved_CFLAGS="${CFLAGS}"
  CFLAGS="${CFLAGS} -Werror"
  dnl# C source code is only singly-m4-quoted in args to this macro:
  AC_LIB_HAVE_LINKFLAGS([babeltrace],[babeltrace-ctf],[
#include <babeltrace/babeltrace.h>
#include <babeltrace/ctf/events.h>
#include <babeltrace/ctf/iterator.h>
  ],[
struct bt_iter_pos *pos = bt_iter_get_pos(bt_ctf_get_iter(NULL));
struct bt_ctf_event *event = NULL;
const struct bt_definition *scope;

bt_iter_set_pos(bt_ctf_get_iter(NULL), pos);
scope = bt_ctf_get_top_level_scope(event,
			      	   BT_STREAM_EVENT_HEADER);
bt_ctf_get_uint64(bt_ctf_get_field(event, scope, "id"));
  ])dnl
  ## restore:
  CFLAGS="${saved_CFLAGS}"

  if test "x${HAVE_LIBBABELTRACE}" != "xyes"; then
     if test "x${with_babeltrace}" = "xyes"; then
       AC_MSG_ERROR([babeltrace requested, but it is missing or unusable])
     else
       case "${host}" in
         *-*-linux*)
           AC_MSG_WARN([babeltrace is missing or unusable; GDB is unable to read CTF data.])
           ;;
         *)
           AC_MSG_WARN([babeltrace is unavailable; please port libbabeltrace to your platform (${host}).])
           ;;
       esac
     fi
  fi
fi

# We build rdi-share on ARM-based targets, as instructed by configure.tgt.
if test "x${build_rdi_share}" = "xyes" -o "x${target_cpu}" = "xarm"; then
  AC_ARG_VAR([build_rdi_share],
             [Try to override decision to build the rdi-share subdirectory
              (unsupported, might not work)])
  AC_CONFIG_SUBDIRS([rdi-share])
else
  if test -d rdi-share; then
    AC_MSG_NOTICE([rdi-share subdir present, but we are not using it])
    export unused_subdirs="${unused_subdirs} rdi-share"
  fi
fi

# We configure the nlm subdirectory on netware targets, as instructed
# by configure.tgt.
if test "x${build_nlm}" = "xyes"; then
  AC_ARG_VAR([build_nlm],
             [Try to override the decision to build in the nlm subdirectory
              (unsupported, might not work)])
  AC_CONFIG_SUBDIRS([nlm])
else
  if test -d nlm; then
    AC_MSG_NOTICE([nlm subdir present, but we are not using it])
    export unused_subdirs="${unused_subdirs} nlm"
  fi
fi
AC_SUBST([unused_subdirs])dnl

# If hostfile (XM_FILE) and/or targetfile (DEPRECATED_TM_FILE) and/or
# nativefile (NAT_FILE) is not set in config/*/*.m[ht] files, then we link
# to an empty version:
files=""
links=""

rm -f xm.h
xm_h=""
if test "x${hostfile}" != "x"; then
    test -n "${hostfile}"
    xm_h=xm.h
    case "${hostfile}" in
      xm-*.h ) GDB_XM_FILE="config/${gdb_host_cpu}/${hostfile}" ;;
      * ) GDB_XM_FILE="${hostfile}"
    esac
    files="${files} ${GDB_XM_FILE}"
    links="${links} xm.h"
    AC_DEFINE_UNQUOTED([GDB_XM_FILE],["${GDB_XM_FILE}"],[hostfile])
else
    test -z "${hostfile}"
    AC_MSG_WARN([unsure what to use for hostfile])
fi
AC_SUBST([xm_h])dnl

rm -f tm.h
tm_h=""
if test "x${targetfile}" != "x"; then
    test -n "${targetfile}"
    tm_h=tm.h
    case "${targetfile}" in
      tm-*.h ) GDB_TM_FILE="config/${gdb_target_cpu}/${targetfile}" ;;
      * ) GDB_TM_FILE="${targetfile}"
    esac
    files="${files} ${GDB_TM_FILE}"
    links="${links} tm.h"
    AC_DEFINE_UNQUOTED([GDB_TM_FILE],["${GDB_TM_FILE}"],[targetfile])
else
    test -z "${targetfile}"
    AC_MSG_WARN([unsure what to use for targetfile])
fi
AC_SUBST([tm_h])dnl

rm -f nm.h
nm_h=""
if test "x${nativefile}" != "x"; then
    test -n "${nativefile}"
    nm_h=nm.h
    case "${nativefile}" in
      nm-*.h ) GDB_NM_FILE="config/${gdb_host_cpu}/${nativefile}" ;;
      * ) GDB_NM_FILE="${nativefile}"
    esac
    files="${files} ${GDB_NM_FILE}"
    links="${links} nm.h"
    AC_DEFINE_UNQUOTED([GDB_NM_FILE],["${GDB_NM_FILE}"],[nativefile])
else
    test -z "${nativefile}"
    AC_MSG_WARN([unsure what to use for nativefile])
fi
AC_SUBST([nm_h])dnl

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

dnl# keep this warning in here just in case:
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
AC_SUBST([ac_config_links1])dnl
  ## forget why I use echo here instead of an autoconf macro, but whatever:
echo "symlinks to be made should be \"${ac_config_links1}\""

dnl# Check for exe extension set on certain hosts (e.g. Win32)
AC_EXEEXT

dnl#  Detect the character set used by this host.

dnl#  At the moment, we just assume it is ISO-8859-1 (which is a
dnl#  superset of ASCII containing the characters needed for French,
dnl#  German, Spanish, Italian, and possibly others), but if were
dnl#  *were* to support any host character sets other than ISO-8859-1,
dnl#  here is where we would detect it.
dnl#  Newer versions of gdb have switched to UTF-8.

AH_TEMPLATE([GDB_DEFAULT_HOST_CHARSET],
            [Define to be a string naming the default host character set.])
case "${host}" in
  *-apple-darwin* | *-apple-macos*)
  ;;
  *)
AC_DEFINE([GDB_DEFAULT_HOST_CHARSET],["ISO-8859-1"],
          [Define to be a string naming the default host character set.])
  ;;
esac

AM_ICONV

# Undo the ${ec_script} escaping suitable for Makefile:
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
AC_SUBST([GCORE_TRANSFORM_NAME])dnl
AC_ARG_VAR([GDB_DATADIR],[where to put gdb's data files])
if test -z "${GDB_DATADIR}"; then
    export GDB_DATADIR=${datadir}/gdb
fi
AC_SUBST([GDB_DATADIR])dnl

dnl# for the gnulib Makefile:
if test "x${gl_LIBOBJS}" = "x"; then
  if test "x${LIBOBJS}" != "x"; then
    test -z "${gl_LIBOBJS}" && test -n "${LIBOBJS}" && export gl_LIBOBJS="${LIBOBJS}"
  fi
fi
AC_SUBST([gl_LIBOBJS])dnl

AC_CONFIG_FILES([Makefile \
                 data-directory/Makefile \
                 .gdbinit:gdbinit.in \
                 gnulib/Makefile \
                 gdb-gdb.gdb])dnl
  ##FIXME: add build-gnulib/Makefile:gnulib/Makefile.in once it works
AC_CONFIG_FILES([gcore],[chmod +x gcore])dnl
AC_CONFIG_COMMANDS([default],[
dnl# Autoconf does NOT provide a mechanism for modifying definitions 
dnl# provided by makefile fragments.
dnl# So instead do this:

changequote(,)dnl
sed -e '/^DEPRECATED_TM_FILE[ 	]*=/s,^DEPRECATED_TM_FILE[ 	]*=[ 	]*,&config/'"${gdb_target_cpu}"'/,
/^XM_FILE[ 	]*=/s,^XM_FILE[ 	]*=[ 	]*,&config/'"${gdb_host_cpu}"'/,
/^NAT_FILE[ 	]*=/s,^NAT_FILE[ 	]*=[ 	]*,&config/'"${gdb_host_cpu}"'/,' <Makefile >Makefile.tmp
mv -f Makefile.tmp Makefile
changequote([,])dnl

echo "CONFIG_HEADERS are ${CONFIG_HEADERS}"

case x${CONFIG_HEADERS} in
xconfig.h:config.in)
echo "warning: using wrong config header" | tee stamp-h ;;
xconfig.h)
echo "stamp" > stamp-h ;;
esac
],[
gdb_host_cpu=${gdb_host_cpu}
gdb_target_cpu=${gdb_target_cpu}
nativefile=${nativefile}
])dnl

AC_CONFIG_COMMANDS([config_status_vpath_build_fix],[
if test "${srcdir}" != "."; then
  if test -e ${srcdir}/config.status -a ! -e `pwd`/config.status; then
    cp -v ${srcdir}/config.status `pwd`/config.status || echo ""
  elif test -e `pwd`/config.status -a ! -e ${srcdir}/config.status; then
    cp -v `pwd`/config.status ${srcdir}/config.status || echo ""
  fi
fi
],[])dnl

AC_OUTPUT

echo ""
echo "=== entering . (`pwd`)"
# display some configured variables for the user to see:
echo ""
echo "Done configuring the gdb subdir with the following overall settings:"
echo "CC: '${CC}'"
echo "CFLAGS: '${CFLAGS}'"
echo "CPPFLAGS: '${CPPFLAGS}'"
echo "DEFS: '${DEFS}'"
echo "LD: '${LD}'"
echo "LDFLAGS: '${LDFLAGS}'"
echo "LIBS: '${LIBS}'"
echo "subdirs: '${subdirs}'"
echo "unused_subdirs: '${unused_subdirs}'"
echo ""

exit 0
