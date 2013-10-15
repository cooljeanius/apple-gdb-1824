dnl#                                               -*- Autoconf -*-
dnl# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([apple-gdb],[1824],[jmolenda@apple.com])
AC_CONFIG_SRCDIR([template.h])
AC_CONFIG_HEADERS([config.h])
AC_CANONICAL_TARGET
AC_SYS_INTERPRETER

# Automake checks, separated out
AM_MAINTAINER_MODE
AM_SANITY_CHECK
AM_PROG_INSTALL_SH
AM_PROG_INSTALL_STRIP
AM_MAKE_INCLUDE
_AM_PROG_TAR([pax])

# Checks for programs.
AM_PROG_AR
AC_PROG_AWK
if test "x$CC" = "x"; then
    test -z "$CC"
    AC_PROG_CC
else
    test ! -z "$CC"
    AC_PROG_GCC_TRADITIONAL
fi
AC_PROG_CPP
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB
AC_PROG_SED
AC_PROG_YACC
AC_PATH_PROG([BUILDIT],[buildit])
AC_SUBST([BUILDIT])

# Checks for libraries.
AC_CHECK_LIB([intl],[gettext])

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h mach/mach.h stddef.h stdio.h \
                  sys/file.h sys/ioctl.h sys/param.h termios.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_BIGENDIAN
AC_C_PROTOTYPES
AC_TYPE_INT32_T

# Checks for library functions.
AC_FUNC_FORK
AC_FUNC_LSTAT dnl# calls AC_FUNC_LSTAT_FOLLOWS_SLASHED_SYMLINK
AC_FUNC_MALLOC
AC_CHECK_FUNCS([bzero memmove memset setenv strchr strerror strrchr \
                strstr strtol strtoull])

AC_CONFIG_SUBDIRS([src])
AC_OUTPUT

# The name "configure" is also a make target in Apple's
# hand-written Makefile, so rename the configure file once
# we are done with it so that `make(1)` will not think that the
# target has already been made.
# Use a separate script for this.
if test -e rename-configure.sh; then
	echo "Running a separate script to rename this configure script..."
	sh ./rename-configure.sh
else
	echo "Warning: The Makefile might think there is nothing to be done."
fi
