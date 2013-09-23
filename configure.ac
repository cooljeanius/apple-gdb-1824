dnl#                                               -*- Autoconf -*-
dnl# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([apple-gdb],[1824],[jmolenda@apple.com])
AC_CONFIG_SRCDIR([template.h])
AC_CONFIG_HEADERS([config.h])

# automake
AM_MAINTAINER_MODE
AM_SANITY_CHECK
AM_PROG_INSTALL_STRIP

# Checks for programs.
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CPP
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB

# Checks for libraries.
AC_CHECK_LIB([intl],[gettext])

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h mach/mach.h stddef.h stdio.h \
                  sys/file.h sys/ioctl.h sys/param.h termios.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_BIGENDIAN
AC_C_PROTOTYPES

# Checks for library functions.
AC_FUNC_LSTAT dnl# calls AC_FUNC_LSTAT_FOLLOWS_SLASHED_SYMLINK
AC_CHECK_FUNCS([memmove memset setenv strchr strerror strrchr strstr \
                strtol strtoull])

AC_CONFIG_SUBDIRS([src])
AC_OUTPUT
