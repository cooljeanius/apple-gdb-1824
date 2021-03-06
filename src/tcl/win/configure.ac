#!/bin/bash -norc
# This file is an input file used by the GNU "autoconf" program to
# generate the file "configure", which is run during Tcl installation
# to configure the system for the local environment.
#
# RCS: @(#) configure.in,v 1.12 2003/01/21 19:40:21 hunt Exp
# -*- Autoconf -*-

AC_INIT([Tcl-Win],[8.4.1],[tromey@cygnus.com])
AC_CONFIG_SRCDIR([../generic/tcl.h])
AC_PREREQ([2.13])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])
AM_SANITY_CHECK

TCL_VERSION=8.4
TCL_MAJOR_VERSION=8
TCL_MINOR_VERSION=4
TCL_PATCH_LEVEL=".1"
VER="${TCL_MAJOR_VERSION}${TCL_MINOR_VERSION}"

TCL_DDE_VERSION=1.2
TCL_DDE_MAJOR_VERSION=1
TCL_DDE_MINOR_VERSION=2
TCL_DDE_PATCH_LEVEL=""
DDEVER="${TCL_DDE_MAJOR_VERSION}${TCL_DDE_MINOR_VERSION}"

TCL_REG_VERSION=1.1
TCL_REG_MAJOR_VERSION=1
TCL_REG_MINOR_VERSION=1
TCL_REG_PATCH_LEVEL=""
REGVER="${TCL_REG_MAJOR_VERSION}${TCL_REG_MINOR_VERSION}"

#------------------------------------------------------------------------
# Handle the --prefix=... option
#------------------------------------------------------------------------

if test "${prefix}" = "NONE"; then
    prefix=/usr/local
fi
if test "${exec_prefix}" = "NONE"; then
    exec_prefix=$prefix
fi
# libdir must be a fully qualified path (not ${exec_prefix}/lib)
eval libdir="$libdir"

#------------------------------------------------------------------------
# Standard compiler checks
#------------------------------------------------------------------------

# If the user did not set CFLAGS, set it now to keep
# the AC_PROG_CC macro from adding "-g -O2".
if test "${CFLAGS+set}" != "set" ; then
    CFLAGS=""
fi

AC_PROG_CC

# To properly support cross-compilation, one would
# need to use these tool checks instead of
# the ones below and reconfigure with
# autoconf 2.50. You can also just set
# the CC, AR, RANLIB, and RC environment
# variables if you want to cross compile.
AC_CHECK_TOOL([AR],[ar],[:])
AC_CHECK_TOOL([RANLIB],[ranlib],[:])
AC_CHECK_TOOL([RC],[windres],[:])dnl

if test "${GCC}" = "yes" ; then
    if test "x${AR}" = "x"; then
        test -z "${AR}"
        AC_CHECK_PROG([AR],[ar],[ar])
    fi
    if test "x${RANLIB}" = "x"; then
        test -z "${RANLIB}"
        AC_CHECK_PROG([RANLIB],[ranlib],[ranlib])
    fi
    if test "x${RC}" = "x"; then
        test -z "${RC}"
        AC_CHECK_PROG([RC],[windres],[windres])
    fi
else
    if test "x${AR}" = "x"; then
        test -z "${AR}"
        AM_PROG_AR
    fi
    if test "x${RANLIB}" = "x"; then
        test -z "${RANLIB}"
        AC_PROG_RANLIB
    fi
fi

#--------------------------------------------------------------------
# Checks to see if the make program sets the $MAKE variable.
#--------------------------------------------------------------------

AC_PROG_MAKE_SET

#--------------------------------------------------------------------
# Perform additinal compiler tests.
#--------------------------------------------------------------------

AC_CANONICAL_TARGET
case ${host_os} in
  *cygwin* ) CYGWIN=yes
             ac_cv_cygwin=yes
             ;;
	 * ) CYGWIN=no
             ac_cv_cygwin=no
             ;;
esac

if test "${ac_cv_cygwin}" = "yes" ; then
    AC_MSG_WARN([Compiling with the Cygwin version of gcc is not supported.
    Use the Mingw version of gcc from www.mingw.org instead.])
fi

AC_CHECK_HEADERS([winbase.h windows.h winioctl.h winnt.h winsock.h \
                  winsock2.h wstcpip.h])dnl

AC_CACHE_CHECK([for SEH support in compiler],
   [tcl_cv_seh],
[AC_RUN_IFELSE([AC_LANG_SOURCE([[
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef WIN32_LEAN_AND_MEAN

int main(int argc, char** argv) {
    int a, b = 0;
    __try {
        a = 666 / b;
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        return 0;
    }
    return 1;
}
]])],[tcl_cv_seh=yes],[tcl_cv_seh=no],[tcl_cv_seh=no])
])
if test "x${tcl_cv_seh}" = "xno"; then
    AC_DEFINE([HAVE_NO_SEH],[1],
            [Defined when mingw does not support SEH])
fi

#
# Check to see if the excpt.h include file provided contains the
# definition for EXCEPTION_DISPOSITION; if not, which is the case
# with Cygwin's version as of 2002-04-10, define it to be int, 
# sufficient for getting the current code to work.
#
AC_CHECK_HEADERS([excpt.h])
AC_CACHE_CHECK([for EXCEPTION_DISPOSITION support in include files],
    [tcl_cv_eh_disposition],
[AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef WIN32_LEAN_AND_MEAN
]],[[
  EXCEPTION_DISPOSITION x;
]])],[tcl_cv_eh_disposition=yes],[tcl_cv_eh_disposition=no])
])
if test "${tcl_cv_eh_disposition}" = "no" ; then
    AC_DEFINE([EXCEPTION_DISPOSITION],[int],
        [Defined when cygwin/mingw does not support EXCEPTION DISPOSITION])
fi

#--------------------------------------------------------------------
# Determines the correct binary file extension (.o, .obj, .exe etc.)
#--------------------------------------------------------------------

AC_OBJEXT
AC_EXEEXT

#--------------------------------------------------------------------
# Check whether --enable-threads or --disable-threads was given.
#--------------------------------------------------------------------

SC_ENABLE_THREADS

#--------------------------------------------------------------------
# The statements below define a collection of symbols related to
# building libtcl as a shared library instead of a static library.
#--------------------------------------------------------------------

SC_ENABLE_SHARED

#--------------------------------------------------------------------
# The statements below define a collection of compile flags.  This 
# macro depends on the value of SHARED_BUILD, and should be called
# after SC_ENABLE_SHARED checks the configure switches.
#--------------------------------------------------------------------

SC_CONFIG_CFLAGS

#--------------------------------------------------------------------
# Set the default compiler switches based on the --enable-symbols 
# option.  This macro depends on C flags, and should be called
# after SC_CONFIG_CFLAGS macro is called.
#--------------------------------------------------------------------

SC_ENABLE_SYMBOLS

TCL_DBGX="${DBGX}"

#--------------------------------------------------------------------
# man2tcl needs this so that it can use errno.h
#--------------------------------------------------------------------

AC_CHECK_HEADERS([errno.h],[],[MAN2TCLFLAGS="-DNO_ERRNO_H"])dnl
AC_SUBST([MAN2TCLFLAGS])dnl

#------------------------------------------------------------------------
# tclConfig.sh refers to this by a different name
#------------------------------------------------------------------------

TCL_SHARED_BUILD="${SHARED_BUILD}"

# Checks for header files.
AC_HEADER_STDBOOL dnl# also "_CHECK"s it
AC_HEADER_TIME
AC_CHECK_HEADERS([ddeml.h fcntl.h float.h io.h lmaccess.h locale.h \
                  malloc.h malloc/malloc.h math.h process.h select.h \
                  shlobj.h signal.h stdio.h sys/io.h sys/param.h \
                  sys/select.h sys/timeb.h sys/utime.h tclPort.h \
                  time.h toolhelp.h utime.h])dnl

# Checks for declarations.
AC_CHECK_DECLS([LC_TIME])dnl
AC_CHECK_DECLS([__try])dnl

# Checks for typedefs, structures, and compiler characteristics.
AC_C_BIGENDIAN
AC_C_INLINE
AC_C_PROTOTYPES
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_CHECK_MEMBERS([struct stat.st_rdev])dnl
AC_CHECK_MEMBERS([TclWinProcs.createHardLinkProc])dnl
AC_CHECK_MEMBERS([TclWinProcs.getFileAttributesExProc])dnl
AC_STRUCT_TIMEZONE dnl# calls AC\_STRUCT\_TM
AC_CHECK_SIZEOF([int])dnl
AC_CHECK_ALIGNOF([int])dnl
AC_CHECK_SIZEOF([char])dnl
AC_CHECK_ALIGNOF([char])dnl

# Checks for library functions.
AC_FUNC_ALLOCA
AC_FUNC_MALLOC
AC_FUNC_MKTIME
AC_FUNC_REALLOC
AC_CHECK_FUNCS([floor ftime memset setlocale strchr strpbrk strrchr \
                strspn strstr strtoull tzset])dnl

#--------------------------------------------------------------------
# Perform final evaluations of variables with possible substitutions.
#--------------------------------------------------------------------

TCL_SHARED_LIB_SUFFIX="\${NODOT_VERSION}${DLLSUFFIX}"
TCL_UNSHARED_LIB_SUFFIX="\${NODOT_VERSION}${LIBSUFFIX}"
TCL_EXPORT_FILE_SUFFIX="\${NODOT_VERSION}${LIBSUFFIX}"

eval "TCL_SRC_DIR=\"`cd $srcdir/..; pwd`\""

eval "TCL_DLL_FILE=tcl${VER}${DLLSUFFIX}"

eval "TCL_LIB_FILE=${LIBPREFIX}tcl$VER${LIBSUFFIX}"

TCL_BUILD_LIB_SPEC="-L`pwd` -ltcl${VER}${TCL_DBGX}"

eval "TCL_STUB_LIB_FILE=\"${LIBPREFIX}tclstub${VER}${LIBSUFFIX}\""
eval "TCL_STUB_LIB_FLAG=\"-ltclstub${VER}${TCL_DBGX}\""
eval "TCL_BUILD_STUB_LIB_SPEC=\"-L`pwd` ${TCL_STUB_LIB_FLAG}\""
eval "TCL_STUB_LIB_SPEC=\"-L${libdir} ${TCL_STUB_LIB_FLAG}\""
eval "TCL_BUILD_STUB_LIB_PATH=\"`pwd`/${TCL_STUB_LIB_FILE}\""
eval "TCL_STUB_LIB_PATH=\"${libdir}/${TCL_STUB_LIB_FILE}\""

# Install time header dir can be set via --includedir
eval "TCL_INCLUDE_SPEC=\"-I${includedir}\""

eval "DLLSUFFIX=${DLLSUFFIX}"
eval "LIBPREFIX=${LIBPREFIX}"
eval "LIBSUFFIX=${LIBSUFFIX}"
eval "EXESUFFIX=${EXESUFFIX}"

CFG_TCL_SHARED_LIB_SUFFIX=${TCL_SHARED_LIB_SUFFIX}
CFG_TCL_UNSHARED_LIB_SUFFIX=${TCL_UNSHARED_LIB_SUFFIX}
CFG_TCL_EXPORT_FILE_SUFFIX=${TCL_EXPORT_FILE_SUFFIX}

#--------------------------------------------------------------------
# Adjust the defines for how the resources are built depending
# on symbols and static vs. shared.
#--------------------------------------------------------------------

if test ${SHARED_BUILD} = 0 ; then
    if test "${DBGX}" = "d"; then
        RC_DEFINES="${RC_DEFINE} STATIC_BUILD ${RC_DEFINE} DEBUG"
    else
        RC_DEFINES="${RC_DEFINE} STATIC_BUILD"
    fi
else
    if test "${DBGX}" = "d"; then
        RC_DEFINES="${RC_DEFINE} DEBUG"
    else
        RC_DEFINES=""
    fi
fi

if test ! -z "$(echo ${RC_DEFINES} | grep STATIC_BUILD)"; then
    test ! -z "${STATIC_BUILD}"
    AC_DEFINE([STATIC_BUILD],[1],[Define to 1 for a static build])
fi

AC_SUBST([TCL_VERSION])dnl
AC_SUBST([TCL_MAJOR_VERSION])dnl
AC_SUBST([TCL_MINOR_VERSION])dnl
AC_SUBST([TCL_PATCH_LEVEL])dnl

AC_SUBST([TCL_LIB_FILE])dnl
AC_SUBST([TCL_LIB_FLAG])dnl
dnl# empty on win:
AC_SUBST([TCL_LIB_SPEC])dnl
AC_SUBST([TCL_STUB_LIB_FILE])dnl
AC_SUBST([TCL_STUB_LIB_FLAG])dnl
AC_SUBST([TCL_STUB_LIB_SPEC])dnl
AC_SUBST([TCL_STUB_LIB_PATH])dnl
AC_SUBST([TCL_INCLUDE_SPEC])dnl
AC_SUBST([TCL_BUILD_STUB_LIB_SPEC])dnl
AC_SUBST([TCL_BUILD_STUB_LIB_PATH])dnl
AC_SUBST([TCL_DLL_FILE])dnl

AC_SUBST([TCL_SRC_DIR])dnl
AC_SUBST([TCL_BIN_DIR])dnl
AC_SUBST([TCL_DBGX])dnl
AC_SUBST([CFG_TCL_SHARED_LIB_SUFFIX])dnl
AC_SUBST([CFG_TCL_UNSHARED_LIB_SUFFIX])dnl
AC_SUBST([CFG_TCL_EXPORT_FILE_SUFFIX])dnl

dnl# win/tcl.m4 does NOT set (CFLAGS)
AC_SUBST([CFLAGS_DEFAULT])dnl
AC_SUBST([EXTRA_CFLAGS])dnl
AC_SUBST([CYGPATH])dnl
AC_SUBST([DEPARG])dnl
AC_SUBST([CC_OBJNAME])dnl
AC_SUBST([CC_EXENAME])dnl

dnl# win/tcl.m4 does NOT set (LDFLAGS)
AC_SUBST([LDFLAGS_DEFAULT])dnl
AC_SUBST([LDFLAGS_DEBUG])dnl
AC_SUBST([LDFLAGS_OPTIMIZE])dnl
AC_SUBST([LDFLAGS_CONSOLE])dnl
AC_SUBST([LDFLAGS_WINDOW])dnl
AC_SUBST([AR])dnl
AC_SUBST([RANLIB])dnl

AC_SUBST([STLIB_LD])dnl
AC_SUBST([SHLIB_LD])dnl
AC_SUBST([SHLIB_LD_LIBS])dnl
AC_SUBST([SHLIB_CFLAGS])dnl
AC_SUBST([SHLIB_SUFFIX])dnl
AC_SUBST([TCL_SHARED_BUILD])dnl

AC_SUBST([LIBS])dnl
AC_SUBST([LIBS_GUI])dnl
AC_SUBST([DLLSUFFIX])dnl
AC_SUBST([LIBPREFIX])dnl
AC_SUBST([LIBSUFFIX])dnl
AC_SUBST([EXESUFFIX])dnl
AC_SUBST([LIBRARIES])dnl
AC_SUBST([MAKE_LIB])dnl
AC_SUBST([POST_MAKE_LIB])dnl
AC_SUBST([MAKE_DLL])dnl
AC_SUBST([MAKE_EXE])dnl

dnl# empty on win, but needs subst-ing
AC_SUBST([TCL_BUILD_LIB_SPEC])dnl
AC_SUBST([TCL_LD_SEARCH_FLAGS])dnl
AC_SUBST([TCL_NEEDS_EXP_FILE])dnl
AC_SUBST([TCL_BUILD_EXP_FILE])dnl
AC_SUBST([TCL_EXP_FILE])dnl
AC_SUBST([DL_LIBS])dnl
AC_SUBST([LIBOBJS])dnl
AC_SUBST([TCL_LIB_VERSIONS_OK])dnl
AC_SUBST([TCL_PACKAGE_PATH])dnl

dnl# win only:
AC_SUBST([TCL_DDE_VERSION])dnl
AC_SUBST([TCL_DDE_MAJOR_VERSION])dnl
AC_SUBST([TCL_DDE_MINOR_VERSION])dnl
AC_SUBST([TCL_DDE_PATCH_LEVEL])dnl
AC_SUBST([TCL_REG_VERSION])dnl
AC_SUBST([TCL_REG_MAJOR_VERSION])dnl
AC_SUBST([TCL_REG_MINOR_VERSION])dnl
AC_SUBST([TCL_REG_PATCH_LEVEL])dnl

AC_SUBST([RC])dnl
AC_SUBST([RC_OUT])dnl
AC_SUBST([RC_TYPE])dnl
AC_SUBST([RC_INCLUDE])dnl
AC_SUBST([RC_DEFINE])dnl
AC_SUBST([RC_DEFINES])dnl
AC_SUBST([RES])dnl

# Output.
AC_CONFIG_FILES([Makefile tclConfig.sh tcl.hpj])dnl
AC_OUTPUT
