dnl#                                               -*- Autoconf -*-
dnl#--------------------------------------------------------------------
dnl# configure.ac --- GNU autoconf source for toplevel directory.
dnl#
dnl# Time-stamp:      "2012-02-12 09:59:48 bkorb"
dnl# Author:           Bruce Korb <bkorb@gnu.org>
dnl#
dnl#  This file is part of AutoGen.
dnl#  AutoGen Copyright (c) 1992-2013 by Bruce Korb - all rights reserved
dnl#
dnl# AutoGen is free software: you can redistribute it and/or modify it
dnl# under the terms of the GNU General Public License as published by the
dnl# Free Software Foundation, either version 3 of the License, or
dnl# (at your option) any later version.
dnl#
dnl# AutoGen is distributed in the hope that it will be useful, but
dnl# WITHOUT ANY WARRANTY; without even the implied warranty of
dnl# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
dnl# See the GNU General Public License for more details.
dnl#
dnl# You should have received a copy of the GNU GPL along
dnl# with this program.  If not, see <http://www.gnu.org/licenses/>.
dnl#
AC_INIT([GNU AutoGen],[5.15],[autogen-users@lists.sourceforge.net])
AC_CONFIG_SRCDIR([agen5/autogen.c])
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_MACRO_DIR([config])
dnl# need to run under guile18-build under some circumstances
if test -z "${GUILE}" -a -z "${GUILE_CONFIG}" -a -z "${GUILE_TOOLS}"; then
  AC_PATH_PROG([GUILE_EIGHTTEEN_BUILD],[guile18-build])
  if test -n "${GUILE_EIGHTTEEN_BUILD}" && test -x "${GUILE_EIGHTTEEN_BUILD}"; then
    AC_MSG_NOTICE([you might want to run this configure script under ${GUILE_EIGHTTEEN_BUILD}])
    sleep 1
  fi
fi
AC_CANONICAL_TARGET
AC_MSG_NOTICE([sourcing ${srcdir}/VERSION])
[. ${srcdir}/VERSION
d=`dirname ${0}`
ag_top_srcdir=`cd ${d} && pwd`
ag_top_builddir=`pwd`]
AM_INIT_AUTOMAKE([gnits dejagnu check-news 1.9 dist-xz -Wno-obsolete])dnl
AM_MAINTAINER_MODE
gl_USE_SYSTEM_EXTENSIONS
LT_INIT([dlopen win32-dll])dnl
m4_ifdef([AC_REVISION],[AC_REVISION([$Revision: 4.34 $])],[])dnl
[config_start_time=`date +%s 2>/dev/null`]
# ----------------------------------------------------------------------
# Substitute VERSION vars here, so that they can be used by the Makefile
# ----------------------------------------------------------------------
AC_SUBST([AG_VERSION])dnl
AC_SUBST([AG_MAJOR_VERSION])dnl
AC_SUBST([AG_MINOR_VERSION])dnl
AC_SUBST([AO_CURRENT])dnl
AC_SUBST([AO_REVISION])dnl
AC_SUBST([AO_AGE])dnl
[AO_TEMPLATE_VERSION=`expr '(' ${AO_CURRENT}   '*' 4096 ')' + ${AO_REVISION}`
]dnl
AC_SUBST([AO_TEMPLATE_VERSION])dnl
AC_SUBST([GO_CURRENT])dnl
AC_SUBST([GO_REVISION])dnl
AC_SUBST([GO_AGE])dnl
AC_DEFINE_UNQUOTED([AO_CURRENT],[${AO_CURRENT}],
                   [Define this to the autoopts current interface number])
AC_DEFINE_UNQUOTED([AO_REVISION],[${AO_REVISION}],
                   [Define this to the autoopts interface revision number])
AC_DEFINE_UNQUOTED([AO_AGE],[${AO_AGE}],
                   [Define this to the autoopts interface age number])
# ----------------------------------------------------------------------
# Set up the environment to configure the snprintv subpackage using
# this version of AutoGen (as opposed to any installed version).
# ----------------------------------------------------------------------
[ag_srcdir=`\cd ${srcdir} && pwd`
if test "x${ag_srcdir}" != "x" && test -d ${ag_srcdir}; then
  :
else
  ag_srcdir=..
fi

# ----------------------------------------------------------------------
# If `configure' is invoked (in)directly via `make', ensure that it
# encounters no `make' conflicts. Ignore error if shell does not have
# unset, but at least set these to empty values.
# ----------------------------------------------------------------------
MFLAGS=""
MAKEFLAGS=""
MAKELEVEL=""
unset MFLAGS MAKEFLAGS MAKELEVEL 2>/dev/null]

AM_WITH_DMALLOC

# ----------------------------------------------------------------------
# check for various programs used during the build.
# ----------------------------------------------------------------------
AC_MSG_CHECKING([programs])
AC_MSG_RESULT([ ])
if test "x${ac_cv_prog_AR}" = "x"; then
  test -z "${ac_cv_prog_AR}" || unset ac_cv_prog_AR
  AC_PROG_AR
fi
AM_PROG_CC_C_O
AC_PROG_CC_STDC
AC_PROG_CXX
gl_FUNC_GLIBC_UNLOCKED_IO
AC_EXEEXT
AC_PROG_INSTALL
AC_PROG_MKDIR_P
dnl# libtool is checked above
AC_CHECK_PROG([TEXI2HTML],[texi2html],[texi2html],[:])dnl
AC_C_CONST
AC_C_INLINE
# ----------------------------------------------------------------------
# check for various libraries
# ----------------------------------------------------------------------
AC_MSG_CHECKING([libraries])
AC_MSG_RESULT([ ])
dnl# libtool already checks dlopen, so use a different symbol here:
AC_CHECK_LIB([dl],[dlsym])dnl
dnl# libiconv symbol prefixing can vary:
AC_CHECK_LIB([iconv],[iconv],[],[
  AC_CHECK_LIB([iconv],[libiconv],[],[
    AC_CHECK_LIB([iconv],[main],[],[])dnl
  ])dnl
])dnl
LT_LIB_M
AC_CHECK_LIB([m],[sqrt])dnl
AC_CHECK_LIB([xml2],[xmlParseFile])dnl
AC_CHECK_LIB([z],[gzopen])dnl
dnl# other libraries to add checks for later:
dnl# gmp, guile, ltdl, pthread
# ----------------------------------------------------------------------
# Checks for header files.
# ----------------------------------------------------------------------
AC_MSG_CHECKING([headers])
AC_MSG_RESULT([ ])
AC_DEFUN([AC_REQUIRE_HEADER_STDC],[
  AC_REQUIRE([AC_HEADER_STDC])dnl
])dnl
AC_REQUIRE_HEADER_STDC
AC_HEADER_DIRENT
AC_HEADER_STAT
AC_HEADER_STDBOOL dnl# also "_CHECK"s it
AC_HEADER_SYS_WAIT
AC_CHECK_HEADERS([float.h libintl.h locale.h stddef.h stdio.h \
                  sys/ioctl.h sys/systeminfo.h])dnl
dnl# (might need an extra section for keeping autoscan happy)
# ----------------------------------------------------------------------
# Checks for typedefs, structures, and compiler characteristics.
# ----------------------------------------------------------------------
AC_MSG_CHECKING([typedefs, structures, and compiler characteristics])
AC_MSG_RESULT([ ])
AC_C_BIGENDIAN
AC_C_PROTOTYPES
AC_STRUCT_TIMEZONE dnl# also does the "_TM" one
AC_TYPE_MODE_T
AC_TYPE_OFF_T
AC_TYPE_PID_T
AC_TYPE_SSIZE_T
AC_TYPE_UID_T
dnl# Begin what used to be AC\_TYPE\_SIGNAL
AC_CACHE_CHECK([return type of signal handlers],[ac_cv_type_signal],[
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <sys/types.h>
#include <signal.h>
]],[[
return *(signal(0, 0))(0) == 1;
]])],[ac_cv_type_signal=int],[ac_cv_type_signal=void])
])
AC_DEFINE_UNQUOTED([RETSIGTYPE],[${ac_cv_type_signal}],
         [Define as the return type of signal handlers (`int' or `void').])
dnl# End what used to be AC\_TYPE\_SIGNAL
AC_TYPE_LONG_DOUBLE_WIDER
ac_cv_c_long_double=${ac_cv_type_long_double_wider}
if test "x${ac_cv_c_long_double}" = "xyes"; then
  AC_DEFINE([HAVE_LONG_DOUBLE],[1],
        [Define to 1 if the type `long double' works and has more range or
	 precision than `double'.])
fi
  
[if test "x${ac_cv_type_long_double}" = "xno"; then
  snv_long_double=double
else
  snv_long_double='long double'
fi]
AC_DEFINE_UNQUOTED([SNV_LONG_DOUBLE],[${snv_long_double}],
                   [Define this to the long+double type])
m4_ifdef([AC_TYPE_LONG_LONG_INT],[
AC_TYPE_LONG_LONG_INT dnl# also calls the "_UNSIGNED" version
],[
AC_CHECK_TYPES([long long])dnl
])dnl
m4_ifdef([AC_TYPE_UINTMAX_T],[
AC_TYPE_UINTMAX_T
],[
AC_CHECK_TYPES([uintmax_t])dnl
])dnl
m4_ifdef([AC_TYPE_SIZE_T],[
AC_TYPE_SIZE_T
],[
AC_CHECK_TYPES([size_t])dnl
])dnl
AC_CHECK_TYPES([ptrdiff_t])dnl
AC_TYPE_INT8_T
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_UINT8_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_CHECK_TYPES([wchar_t])dnl
AC_CHECK_SIZEOF([char*],[4])dnl
AC_CHECK_ALIGNOF([char*])dnl
AC_CHECK_SIZEOF([int],[4])dnl
AC_CHECK_ALIGNOF([int])dnl
AC_CHECK_SIZEOF([long],[4])dnl
AC_CHECK_ALIGNOF([long])dnl
AC_CHECK_SIZEOF([short],[2])dnl
AC_CHECK_ALIGNOF([short])dnl

# ----------------------------------------------------------------------
# Checks for library functions.
# ----------------------------------------------------------------------
AC_MSG_CHECKING([functions])
AC_MSG_RESULT([ ])
AC_FUNC_CLOSEDIR_VOID
AC_FUNC_MALLOC
AC_FUNC_MEMCMP
AC_FUNC_MKTIME
AC_FUNC_MMAP
AC_FUNC_REALLOC
AC_FUNC_SELECT_ARGTYPES
m4_ifdef([AC_FUNC_SETVBUF_REVERSED],[
AC_FUNC_SETVBUF_REVERSED
],[
AC_CHECK_FUNCS([setvbuf])dnl
])dnl
AC_FUNC_STAT
AC_FUNC_STRFTIME
AC_FUNC_UTIME_NULL
AC_FUNC_VPRINTF
AC_CHECK_FUNCS_ONCE([dlopen])dnl
AC_CHECK_FUNCS([atexit dup2 getcwd localtime_r mbrlen memmove \
                memset mkfifo munmap realpath regcomp select socket \
                strcasecmp strchr strcspn strerror strlcpy strpbrk strspn \
                strstr strtol strtoul snprintf sysinfo tzset uname utime])
# keep autoscan happy with this impossible condition:
if test "x" = "Y0"; then
  AC_CHECK_FUNCS([alarm getpagesize])
fi
AC_SEARCH_LIBS([copysign],[m],
               [AC_DEFINE([HAVE_COPYSIGN],[1],
                          [Define if you have the `copysign' function])dnl
])dnl
AC_SEARCH_LIBS([copysignl],[m],
               [AC_DEFINE([HAVE_COPYSIGNL],[1],
                          [Define if you have the `copysignl' function])dnl
])dnl
AC_SEARCH_LIBS([modfl],[m],
               [AC_DEFINE([HAVE_MODFL],[1],
                          [Define if you have the `modfl' function])dnl
])dnl

# ----------------------------------------------------------------------
# Gettext
# ----------------------------------------------------------------------
AC_MSG_CHECKING([gettext])
AC_MSG_RESULT([ ])
AM_GNU_GETTEXT([external])dnl
AM_GNU_GETTEXT_VERSION([0.19.2])dnl

# ----------------------------------------------------------------------
# Check for the functions needed from libgen and libdl
# ----------------------------------------------------------------------

AM_CONDITIONAL([NEED_PATHFIND],[test "X${ac_cv_func_pathfind}" = "Xyes"])
[if test "X${ac_cv_func_dlopen}" = "Xyes"
then DYNAMIC_AG=-export-dynamic
else DYNAMIC_AG=""
fi]
AC_SUBST([DYNAMIC_AG])dnl

# ----------------------------------------------------------------------
# Do all our own macros
# ----------------------------------------------------------------------
AC_MSG_CHECKING([some autogen-specific things])
AC_MSG_RESULT([ ])
# might need to unset some variables out here instead of in the macro
# itself because of how "AC_REQUIRE" works:
unset ac_cv_prog_ranlib
unset ac_cv_prog_ac_ct_RANLIB
INVOKE_AG_MACROS
[
if ! test "x${ag_cv_sys_siglist}" = "xyes"
then
  if ! test "x${ac_cv_func_strsignal}" = "xyes"
  then
    echo "WARNING:  strsignal will use POSIX names and Linux signal numbers"
  fi
fi >&2
if test "X${ag_cv_link_sigsetjmp}" = "Xno"
then]
  AC_MSG_ERROR([AutoGen requires sigsetjmp(3)])[
fi
]
# ----------------------------------------------------------------------
# Do SNPRINTFV macros
# ----------------------------------------------------------------------
INVOKE_SNPRINTFV_MACROS
# ----------------------------------------------------------------------
# Generate the make files.
# ----------------------------------------------------------------------
AC_PROG_LN_S
[
test -f ./snprintfv/snprintfv/snprintfv.h \
  && rm -f ./snprintfv/snprintfv.h \
  && ${LN_S} `pwd`/snprintfv/snprintfv/snprintfv.h ./snprintfv/

AGnam=autogen${ac_exeext}
GDnam=getdefs${ac_exeext}
CLnam=columns${ac_exeext}
AGexe=${ag_top_builddir}/agen5/${AGnam}
GDexe=${ag_top_builddir}/getdefs/${GDnam}
CLexe=${ag_top_builddir}/columns/${CLnam}
M4_SRC=`cd ${srcdir}/config; echo [a-z]*.m4`
ENABLE_STATIC=${enable_static}
config_end_time=`date +%s 2>/dev/null`
time_delta=`expr ${config_end_time} - ${config_start_time} 2>/dev/null`

if test -z "${time_delta}"
then time_delta=10
elif test ${time_delta} -lt 5
then time_delta=5; fi

AG_TIMEOUT=${time_delta}
]
AC_DEFINE_UNQUOTED([AG_DEFAULT_TIMEOUT],[${time_delta}],
                   [define to suitable timeout limit for shell command])dnl
AC_SUBST([M4_SRC])dnl
AC_SUBST([AGnam])dnl
AC_SUBST([GDnam])dnl
AC_SUBST([CLnam])dnl
AC_SUBST([AGexe])dnl
AC_SUBST([GDexe])dnl
AC_SUBST([CLexe])dnl
AC_SUBST([AG_TIMEOUT])dnl
AC_SUBST([ac_aux_dir])dnl
AC_SUBST([LIBS])dnl
AC_SUBST([DEBUG_ENABLED])dnl
AC_SUBST([ENABLE_STATIC])dnl
[
if test "${ag_top_srcdir}" = "${ag_top_builddir}"
then
 INCLIST='-I${top_builddir} -I${top_srcdir}/autoopts'
else
 INCLIST='-I${top_builddir} -I${top_srcdir}'
 INCLIST="${INCLIST} -I\${top_builddir}/autoopts -I\${top_srcdir}/autoopts"
fi
]
AC_SUBST([INCLIST])dnl
[
test ${GUILE_VERSION} -ge 108000 \
  -a "X${WERROR}" = "Xyes" \
  -a "X${GCC}" = "Xyes" && \
    CFLAGS=`echo ${CFLAGS} -Wall -Werror -Wcast-align \
       -Wmissing-prototypes -Wpointer-arith -Wshadow -Wstrict-prototypes \
       -Wwrite-strings -Wno-format-contains-nul -fno-strict-aliasing \
       -Wstrict-aliasing=2 -Wextra`
]dnl# left out: -Wconversion -Wsign-conversion -Wstrict-overflow
AC_CONFIG_HEADERS([config.h:config-h.in])dnl

AH_TOP([#ifndef AUTOGEN_CONFIG_H])dnl
AH_TOP([#define AUTOGEN_CONFIG_H 1])dnl
AH_BOTTOM([#endif /* AUTOGEN_CONFIG_H */])dnl

AC_CONFIG_FILES([doc/mk-agen-texi.sh:doc/mk-agen-texi-in],
                [chmod +x doc/mk-agen-texi.sh])dnl
AC_CONFIG_FILES([autoopts/mk-autoopts-pc],
                [chmod +x autoopts/mk-autoopts-pc])dnl
AC_CONFIG_FILES([autoopts/autoopts-config],
                [chmod +x autoopts/autoopts-config])dnl

AC_CONFIG_FILES([autoopts/tpl/tpl-config.tlib:autoopts/tpl/tpl-config-tlib.in])dnl
AC_CONFIG_FILES([doc/auto_gen.tpl:doc/auto_gen-tpl.in])dnl
AC_CONFIG_FILES([
    autoopts/test/defs     pkg/pkg-env            Makefile
    agen5/Makefile         agen5/test/Makefile    autoopts/Makefile
    autoopts/test/Makefile columns/Makefile       compat/Makefile
    doc/Makefile           getdefs/Makefile       getdefs/test/Makefile
    pkg/Makefile           snprintfv/Makefile     xml2ag/Makefile
    xml2ag/test/Makefile   po/Makefile.in         config/mk-shdefs])dnl

AC_CONFIG_COMMANDS([stamp-h],
[test -z "${CONFIG_HEADERS}" || echo timestamp > stamp-h])
[CONFIG_SHELL=${CONFIG_SHELL-/bin/sh}]
AC_SUBST([CONFIG_SHELL])dnl
AC_DEFINE_UNQUOTED([CONFIG_SHELL],[${CONFIG_SHELL}],
                   [Define this to a working Bourne shell])dnl
AC_OUTPUT

echo \
"------------------------------------------------------------------------
Configuration:

  Source code location:   ${srcdir}
  Compiler:               ${CC}
  Compiler flags:         ${CFLAGS}
  Host System Type:       ${host}
  Install path:           ${prefix}

  See config.h for further configuration information.
------------------------------------------------------------------------"
