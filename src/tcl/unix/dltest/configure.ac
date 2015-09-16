dnl#                                               -*- Autoconf -*-
dnl# This file is an input file used by the GNU "autoconf" program to
dnl# generate the file "configure", which is run to configure the
dnl# Makefile in this directory.

dnl# CYGNUS LOCAL, need 2.5 or higher for --bindir et al
AC_PREREQ([2.5])
dnl# END CYGNUS LOCAL

AC_INIT([dltest],[1.4],[tromey@cygnus.com])
AC_CONFIG_SRCDIR([pkga.c])
AC_CONFIG_MACRO_DIR([../m4])
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

# RCS: @(#) configure.ac,v 1.4 2002/11/26 19:48:05 hunt Exp

# Recover information that Tcl computed with its configure script.

if test -r ../tclConfig.sh; then
  AC_MSG_NOTICE([sourcing ../tclConfig.sh])
  . ../tclConfig.sh
fi

## pre-assigned compiler:
MY_TCL_CC=${TCL_CC}
AC_SUBST([MY_TCL_CC])dnl
  ## autoconf way to check compiler:
if test -z "${CC}"; then
  AC_PROG_CC
else
  test ! -z "${CC}"
  AC_SUBST([CC])
fi
## for dynamic linking:
SHLIB_CFLAGS=${TCL_SHLIB_CFLAGS}
AC_SUBST([SHLIB_CFLAGS])dnl
  ## for compiling:
EXTRA_CFLAGS=${TCL_EXTRA_CFLAGS}
AC_SUBST([EXTRA_CFLAGS])dnl
  ## linker for dynamic libraries:
SHLIB_LD=${TCL_SHLIB_LD}
AC_SUBST([SHLIB_LD])dnl
  ## dynamic libraries to link against:
SHLIB_LD_LIBS=${TCL_SHLIB_LD_LIBS}
AC_SUBST([SHLIB_LD_LIBS])dnl
  ## file extension for shared libraries:
SHLIB_SUFFIX=${TCL_SHLIB_SUFFIX}
AC_SUBST([SHLIB_SUFFIX])dnl
  ## version of shared libraries:
SHLIB_VERSION=${TCL_SHLIB_VERSION}
AC_SUBST([SHLIB_VERSION])dnl
AC_SUBST([TCL_BUILD_STUB_LIB_SPEC])dnl
  ## libraries to link against for Tcl:
TCL_LIBS=${TCL_LIBS}
AC_SUBST([TCL_LIBS])dnl
  ## version of Tcl library:
TCL_VERSION=${TCL_VERSION}
AC_SUBST([TCL_VERSION])dnl
  ## gets added to the just-plain 'CFLAGS' in the Makefile:
if test -z "${CFLAGS_DEFAULT}" && test -z "${CFLAGS}"; then
  export CFLAGS_DEFAULT="-g -O2"
fi
AC_SUBST([CFLAGS_DEFAULT])dnl
  ## for debugging:
if test -z "${CFLAGS_DEBUG}"; then
  export CFLAGS_DEBUG="-ggdb"
fi
AC_SUBST([CFLAGS_DEBUG])dnl
  ## for optimizing:
if test -z "${CFLAGS_OPTIMIZE}"; then
  export CFLAGS_OPTIMIZE="-O1"
fi
AC_SUBST([CFLAGS_OPTIMIZE])dnl
AC_SUBST([DL_LIBS])dnl
AC_SUBST([MATH_LIBS])dnl
AC_SUBST([TCL_DBGX])dnl

# Tcl8.1 requires writable strings for gcc:
if test "x${GCC}" = "xyes"; then
   TCL_EXTRA_CFLAGS="-fwritable-strings"
   ##FIXME: it is bad to require this.
else
   TCL_EXTRA_CFLAGS=""
fi
AC_SUBST([TCL_EXTRA_CFLAGS])dnl

AM_SANITY_CHECK
AM_MAINTAINER_MODE
test -e ${srcdir}/missing
AM_MISSING_PROG([AUTOMAKE114],[automake-1.14])dnl
AM_INIT_AUTOMAKE([1.14 foreign dejagnu])dnl
AM_PROG_INSTALL_STRIP
LT_INIT([dlopen])dnl
LT_LIB_M

# Output.
AC_CONFIG_FILES([Makefile Makefile_orig])dnl
AC_OUTPUT

