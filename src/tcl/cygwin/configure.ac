dnl#                                               -*- Autoconf -*-
dnl# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.13])dnl

# initialize autoconf stuff:
AC_INIT([libtcl_cygwin],[8.4],[tromey@cygnus.com])
AC_CONFIG_SRCDIR([Makefile.in])
AC_CONFIG_HEADERS([config.h])
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE([1.4 foreign -Wno-obsolete])
AM_MAINTAINER_MODE
AC_EXEEXT

# FIXME: We temporarily define our own version of AC_PROG_CC. This is
# copied from autoconf 2.12, but does not call AC_PROG_CC_WORKS. We
# are probably using a cross compiler, which will not be able to fully
# link an executable. This should really be fixed in autoconf
# itself.

AC_DEFUN([LIB_AC_PROG_CC],
[AC_BEFORE([$0],[AC_PROG_CPP])dnl
AC_CHECK_PROG([CC],[gcc],[gcc])
if test -z "${CC}"; then
  AC_CHECK_PROG([CC],[cc],[cc],[],[],[/usr/ucb/cc])
  test -z "${CC}" && AC_MSG_ERROR([no acceptable cc found in \$PATH])
fi

AC_PROG_CC_GNU

if test ${ac_cv_prog_gcc_var} = yes; then
  GCC=yes
dnl# Check whether -g works, even if CFLAGS is set, in case the package
dnl# plays around with CFLAGS (such as to build both debugging and
dnl# normal versions of a library), tasteless as that idea is.
  ac_test_CFLAGS="${CFLAGS+set}"
  ac_save_CFLAGS="${CFLAGS}"
  CFLAGS=""
  AC_PROG_CC_G
  if test "${ac_test_CFLAGS}" = set; then
    CFLAGS="${ac_save_CFLAGS}"
  else
    CFLAGS="-O2"
  fi
else
  GCC=""
fi
])dnl

LIB_AC_PROG_CC

case "${host}" in
*-*-cygwin*)
	touch ac$$.c
	if ${CC} -c -mno-win32 ac$$.c >/dev/null 2>&1; then
	    case "${EXTRA_CFLAGS}" in
		*-mno-win32*) ;;
		*) EXTRA_CFLAGS="-mno-win32 $EXTRA_CFLAGS" ;;
	    esac
	fi
	rm -f ac$$.o ac$$.c
	;;
esac

AM_PROG_AS
m4_ifdef([AM_PROG_AR],[
  AM_PROG_AR
],[
  AC_CHECK_TOOL([AR],[ar],[ar])dnl
  AC_SUBST([AR])dnl
])dnl
AC_SUBST([EXTRA_CFLAGS])dnl
AC_PROG_RANLIB
AC_PROG_INSTALL
AC_HEADER_TIME
AC_STRUCT_TIMEZONE dnl# calls AC\_STRUCT\_TM
AC_CHECK_HEADERS([locale.h sys/param.h sys/select.h])dnl
AC_CHECK_DECLS([LC_TIME])dnl

#--------------------------------------------------------------------
# The statements below define the symbol TCL_PACKAGE_PATH, which
# gives a list of directories that may contain packages. The list
# consists of one directory for machine-dependent binaries and
# another for platform-independent scripts.
#--------------------------------------------------------------------

if test "${prefix}" != "${exec_prefix}"; then
    TCL_PACKAGE_PATH="${exec_prefix}/lib ${prefix}/lib"
else
    TCL_PACKAGE_PATH="${prefix}/lib"
fi

TCL_LIBRARY=@datadir@/tcl${VERSION}

cross_cache=${srcdir}/cygwin-cache
AC_MSG_NOTICE([sourcing ${cross_cache}])
. ${cross_cache}

AC_SUBST([TCL_PACKAGE_PATH])dnl
AC_SUBST([TCL_LIBRARY])dnl

# These variables are needed to get expect working with
# the tclConfig.sh script in the cygwin directory.
LD_FLAGS=""
AC_SUBST([LD_FLAGS])
TCL_LD_SEARCH_FLAGS=""
AC_SUBST([TCL_LD_SEARCH_FLAGS])
DL_LIBS=""
AC_SUBST([DL_LIBS])
MATH_LIBS=""
AC_SUBST([MATH_LIBS])
SHLIB_LD='${CC} -shared ${CFLAGS}'
AC_SUBST([SHLIB_LD])
TCL_BUILD_LIB_SPEC=`pwd`/libtcl_cygwin.a
AC_SUBST([TCL_BUILD_LIB_SPEC])

# Output.
AC_CONFIG_FILES([Makefile tclConfig.sh])dnl
AC_OUTPUT
