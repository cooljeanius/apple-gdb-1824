dnl# This file is used as input to autoconf to generate configure.
dnl# The only reason we need this is that the Tk directory structure
dnl# changed in 7.5, and this change lets us avoid changing the
dnl# configuration superstructure.
dnl# Tom Tromey <tromey@cygnus.com>

m4_include([../config/warnings.m4])
AC_PREREQ([2.5])

AC_INIT([tk],[8.4.1],[http://tcl.sourceforge.net/])
AC_CONFIG_SRCDIR([generic/tk.h])
AC_CONFIG_HEADERS([config.h])

# architecture stuff:
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

# an automake bit:
AM_MAINTAINER_MODE

case "${host}" in
*cygwin* | *mingw32* | *windows32*)
	CONFIGDIR="win"
	AC_CONFIG_SUBDIRS([win])
	;;
*)
	CONFIGDIR="unix"
	AC_CONFIG_SUBDIRS([unix])
	AC_CONFIG_AUX_DIR([unix])	
	;;
esac

AC_SUBST([CONFIGDIR])dnl

# Other system-related checks:
AC_USE_SYSTEM_EXTENSIONS
AC_SYS_LARGEFILE
AC_OBJEXT
AC_EXEEXT
if test -n "${HOST_ARCHITECTURE}"; then
  ACX_PROG_CC_WARNING_OPTS(["-arch ${HOST_ARCHITECTURE}"],[C_ARCHFLAGS])
  AC_SUBST([C_ARCHFLAGS])
fi

# Checks for programs.
AC_PROG_CC
AC_PROG_GCC_TRADITIONAL
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AC_PROG_SED

# Checks for header files.
AC_DEFUN([AC_REQUIRE_HEADER_STDC],[
  AC_REQUIRE([AC_HEADER_STDC])dnl
])dnl
AC_REQUIRE_HEADER_STDC
AC_HEADER_STDBOOL dnl# also "_CHECK"s it
AC_HEADER_TIME
AC_PATH_X
AC_CHECK_HEADERS([fcntl.h limits.h stddef.h stdio.h sys/file.h \
                  sys/ioctl.h sys/time.h])dnl

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_UID_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_STRUCT_ST_BLOCKS

# Checks for library functions.
if test "x${GITHUB_ACTIONS}" = "xtrue" && test "x${CODEQL_ACTION_FEATURE_WILL_UPLOAD}" = "xtrue"; then
  export ac_cv_func_chown_works=yes
fi
AC_FUNC_CHOWN
if test "x${GITHUB_ACTIONS}" = "xtrue" && test "x${CODEQL_ACTION_FEATURE_WILL_UPLOAD}" = "xtrue"; then
  export ac_cv_func_fork_works=yes
  export ac_cv_func_vfork_works=yes
fi
AC_FUNC_FORK
AC_FUNC_GETGROUPS
AC_FUNC_MALLOC
AC_FUNC_MEMCMP
AC_FUNC_REALLOC
AC_FUNC_SELECT_ARGTYPES
AC_FUNC_STRTOD
AC_CHECK_FUNCS([bzero dup2 endpwent floor ftruncate getcwd memmove \
                memset pow select sqrt strcasecmp strchr strncasecmp \
                strrchr strstr strtol strtoul])dnl

# Output.
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
