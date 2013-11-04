dnl#                                               -*- Autoconf -*-
dnl# Process this file with autoconf to produce a configure script.
AC_PREREQ([2.13])
AC_INIT([dejagnu],[1.4.2],[rob@welcomehome.org])
AC_CONFIG_SRCDIR([runtest.exp])
AC_CONFIG_AUX_DIR([..])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])
AC_CANONICAL_TARGET
AC_EXEEXT

dnl# These are required by automake
AM_INIT_AUTOMAKE([1.4 dejagnu -Wno-portability])
dnl# Yes, that is a "1.4" and not a "1.14" there.
dnl# (the aclocal.m4 I found in here was really old)
AM_MAINTAINER_MODE
AM_MAKE_INCLUDE
AM_DEP_TRACK

AC_PROG_SED

if test -e ./config.cache; then
	sed -i "s|ac_cv_env_CFLAGS_value|bad_CFLAGS|g" ./config.cache
	sed -i "s|ac_cv_env_CXXFLAGS_value|bad_CXXFLAGS|g" ./config.cache
	sed -i "s|ac_cv_exeext|bad_exeext|g" ./config.cache
fi

# Checks for programs.
rm -rf *.dSYM
AM_PROG_AS
rm -rf *.dSYM
if test "x$CC" = "x"; then
	test -z "$CC"
	AC_PROG_CC
else
	test ! -z "$CC"
fi
rm -rf *.dSYM
AC_PROG_CXX
CXXFLAGSOLD=${CXXFLAGS}
AC_PROG_CXX_OLD
CXXFLAGS=${CXXFLAGSOLD}
AC_PROG_CXXCPP
AC_PROG_CXX_C_O
AC_PROG_INSTALL
AC_EXEEXT

AC_PROG_YACC
AM_PROG_LEX

# xorg-util-macros checks
XORG_PROG_RAWCPP
XORG_LD_WRAP
XORG_CHANGELOG
XORG_ENABLE_INTEGRATION_TESTS

# Checks for libraries.
AC_LANG([C])
AC_CHECK_LIB([c],[printf])

# Checks for header files.
AC_HEADER_STDBOOL dnl# calls AC_CHECK_HEADER_STDBOOL
AC_HEADER_SYS_WAIT
AC_CHECK_DECLS([wait])
AC_CHECK_HEADERS([regex.h])
AC_CHECK_HEADERS([dejagnu.h],[],[],[
#ifdef HAVE_DECL_WAIT
# undef HAVE_STDLIB_H
# undef HAVE_SYS_WAIT_H
#endif /* HAVE_DECL_WAIT */
])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_BIGENDIAN
AC_C_INLINE
AC_C_PROTOTYPES

# Checks for library functions.
AC_CHECK_FUNCS([atexit regcomp strncmp])
AC_CHECK_DECLS([strncmp])

dnl# check STL version
AC_LANG([C++])
DJ_AC_STL

dnl# we need the path to Docbook so we can build packages.
DJ_AC_PATH_DOCBOOK

dnl# we need the path to the tcl shell to build a release
DJ_AC_PATH_TCLSH

dnl# Level of indirection for automake macro (baseboards:boards_DATA)
BOARDS='$(boards)'
AC_SUBST([BOARDS])
CONFIG='$(config)'
AC_SUBST([CONFIG])

AC_CONFIG_SUBDIRS([doc example/calc])

if test -d contrib/bluegnu2.0.3; then
  test -e contrib/bluegnu2.0.3/configure
  AC_CONFIG_SUBDIRS([contrib/bluegnu2.0.3])
fi

AC_CONFIG_FILES([Makefile \
                 doc/Makefile \
                 testsuite/Makefile \
                 example/Makefile \
                 testsuite/libdejagnu/Makefile])
AC_OUTPUT

