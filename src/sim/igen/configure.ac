dnl#                                               -*- Autoconf -*-
dnl# Process this file with autoconf to produce a configure script.

sinclude(../common/acinclude.m4)
AC_PREREQ([2.5])dnl
AC_INIT([igen],[0.0.1],[/dev/null])
AC_CONFIG_SRCDIR([table.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_YACC

SIM_AC_OPTION_WARNINGS

# Put a plausible default for CC_FOR_BUILD in Makefile.
if test "x${cross_compiling}" = "xno" -a "x${host}" != "xi386-windows"; then
  AR_FOR_BUILD='$(AR)'
  AR_FLAGS_FOR_BUILD='$(AR_FLAGS)'
  CC_FOR_BUILD='$(CC)'
  CFLAGS_FOR_BUILD='$(CFLAGS)'
  RANLIB_FOR_BUILD='$(RANLIB)'
  LIBIBERTY_LIB=../../libiberty/libiberty.a
else
  AR_FOR_BUILD=${AR_FOR_BUILD-ar}
  AR_FLAGS_FOR_BUILD=${AR_FLAGS_FOR_BUILD-rc}
  CC_FOR_BUILD=${CC_FOR_BUILD-gcc}
  CFLAGS_FOR_BUILD=${CFLAGS_FOR_BUILD-"-g"}
  RANLIB_FOR_BUILD=${RANLIB_FOR_BUILD-ranlib}
  LIBIBERTY_LIB=
fi

AC_CONFIG_AUX_DIR([../..])
AC_CANONICAL_TARGET
AC_ARG_PROGRAM

AC_MSG_NOTICE([sourcing ${srcdir}/../../bfd/configure.host])
. ${srcdir}/../../bfd/configure.host

AC_CONFIG_HEADERS([config.h])

# Checks for header files.
AC_DEFUN([AC_REQUIRE_HEADER_STDC],[
  AC_REQUIRE([AC_HEADER_STDC])dnl
])dnl
AC_REQUIRE_HEADER_STDC
AC_CHECK_HEADERS([ctype.h fcntl.h getopt.h stdarg.h stdio.h varargs.h])
AC_HEADER_DIRENT

# Checks for typedefs, structures, and compiler characteristics.
AC_C_BIGENDIAN
AC_C_INLINE
AC_C_PROTOTYPES
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_FUNC_MALLOC
AC_CHECK_FUNCS([memset strchr strdup])dnl

# Checks for declarations.
AC_CHECK_DECLS([NULL])

dnl# Substitutions:
AC_SUBST([AR_FOR_BUILD])dnl
AC_SUBST([AR_FLAGS_FOR_BUILD])dnl
AC_SUBST([CC_FOR_BUILD])dnl
AC_SUBST([CFLAGS_FOR_BUILD])dnl
AC_SUBST([RANLIB_FOR_BUILD])dnl
AC_SUBST([LIBIBERTY_LIB])dnl

AC_SUBST([AR])dnl
AC_SUBST([CFLAGS])dnl
AC_PROG_RANLIB

# Output.
AC_CONFIG_FILES([Makefile])
AC_CONFIG_COMMANDS([default],[case x${CONFIG_HEADERS} in xconfig.h:config.in) echo > stamp-h ;; esac],[])
AC_OUTPUT
