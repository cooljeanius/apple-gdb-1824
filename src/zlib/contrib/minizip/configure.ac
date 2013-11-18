#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_INIT([minizip],[1.2.8],[bugzilla.redhat.com])
AC_CONFIG_SRCDIR([minizip.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_AUX_DIR([../..])
AC_CONFIG_MACRO_DIR([../../m4])
AC_CANONICAL_TARGET
AC_SYS_LARGEFILE
AM_INIT_AUTOMAKE([foreign])
AM_MAINTAINER_MODE
AC_PROG_MKDIR_P

# libtool
AC_PROG_CXX
AC_DEFUN([AC_REQUIRE_RANLIB],[
  AC_REQUIRE([AC_PROG_RANLIB])
])
AC_REQUIRE_RANLIB
LT_INIT([win32-dll])
LT_LANG([C])
LT_LANG([Windows Resource])

AC_MSG_CHECKING([whether to build example programs])
AC_ARG_ENABLE([demos],[AS_HELP_STRING([--enable-demos],[build example programs])])
AM_CONDITIONAL([COND_DEMOS],[test "$enable_demos" = yes])
if test "${enable_demos}" = yes
then
	AC_MSG_RESULT([yes])
else
	AC_MSG_RESULT([no])
fi

case "${host}" in
	*-mingw* | mingw*)
		WIN32="yes"
		;;
	*)
		;;
esac
AM_CONDITIONAL([WIN32],[test "${WIN32}" = "yes"])
AC_EXEEXT

# Checks for libraries.
AC_CHECK_LIB([z],[gzopen])

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h stddef.h stdio.h utime.h])
AC_SUBST([HAVE_UNISTD_H],[0])
unset ac_cv_header_unistd_h
AC_CHECK_HEADERS([unistd.h],[HAVE_UNISTD_H=1],[])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_BIGENDIAN
AC_C_PROTOTYPES
AC_TYPE_OFF_T
AC_TYPE_SIZE_T
AC_TYPE_UINT64_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_MKTIME
AC_CHECK_FUNCS([memset mkdir utime])

# Output
AC_CONFIG_FILES([Makefile minizip.pc])
AC_OUTPUT
