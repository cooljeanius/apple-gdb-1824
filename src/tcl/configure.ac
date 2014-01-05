dnl#                                               -*- Autoconf -*-
dnl# This file is used as input to autoconf to generate configure.
dnl# The only reason we need this is that the Tcl directory structure
dnl# changed in 7.5, and this change lets us avoid changing the
dnl# configuration superstructure.
dnl# Tom Tromey <tromey@cygnus.com>

AC_PREREQ([2.5])

AC_INIT([Tcl],[8.4.1],[tromey@cygnus.com])
AC_CONFIG_SRCDIR([generic/tcl.h])
AC_CONFIG_HEADERS([config.h])

AC_CANONICAL_TARGET

case "${host}" in
*cygwin*)
	CONFIGDIR="win"
	AC_SUBST([CONFIGDIR])
	CONFIGDIR2="cygwin"
	AC_SUBST([CONFIGDIR2])
	AC_CONFIG_SUBDIRS([cygwin])
	;;
*mingw32* | *windows32*)
	CONFIGDIR="win"
	AC_SUBST([CONFIGDIR])
	AC_CONFIG_SUBDIRS([win])
	;;
*)
	CONFIGDIR="unix"
	AC_SUBST([CONFIGDIR])
	AC_CONFIG_AUX_DIR([unix])
	AC_CONFIG_SUBDIRS([unix])
	;;
esac

# Other system-related checks
AC_USE_SYSTEM_EXTENSIONS
AC_SYS_LARGEFILE
AC_OBJEXT
AC_EXEEXT

# Checks for programs.
if test "x${CC}" = "x"; then
	test -z "${CC}"
	AC_PROG_CC
else
	test ! -z "${CC}" && export CC
	AC_SUBST([CC])
fi
AC_PROG_AWK
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_MKDIR_P
AC_PROG_RANLIB
AC_PROG_YACC

# Checks for header files.
AC_DEFUN([AC_REQUIRE_HEADER_STDC],[
  AC_REQUIRE([AC_PROG_CPP])
  AC_REQUIRE([AC_HEADER_STDC])
])
AC_REQUIRE_HEADER_STDC
AC_HEADER_STDBOOL dnl# calls AC_CHECK_HEADER_STDBOOL
AC_CHECK_HEADERS([fcntl.h float.h limits.h locale.h math.h \
                  stddef.h stdio.h sys/file.h sys/param.h \
                  sys/timeb.h utime.h values.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_BIGENDIAN
AC_C_PROTOTYPES
AC_TYPE_UID_T
AC_TYPE_OFF_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_CHECK_MEMBERS([struct stat.st_blksize])
AC_STRUCT_ST_BLOCKS
AC_CHECK_MEMBERS([struct stat.st_rdev])

# Checks for library functions.
AC_FUNC_ALLOCA
AC_FUNC_CHOWN
AC_FUNC_FORK
AC_FUNC_GETGROUPS
AC_FUNC_MALLOC
AC_FUNC_MKTIME
AC_FUNC_REALLOC
AC_FUNC_STRTOD
AC_FUNC_WAIT3
AC_CHECK_FUNCS([dup2 ftime ftruncate getcwd gettimeofday getwd memchr \
                memmove memset putenv re_comp regcomp setlocale sqrt \
                strcasecmp strchr strcspn strdup strerror strncasecmp \
                strpbrk strrchr strspn strstr strtol strtoul strtoull \
                tzset utime])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
