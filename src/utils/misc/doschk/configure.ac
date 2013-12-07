dnl#                                               -*- Autoconf -*-
dnl# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([doschk],[unknown],[/dev/null])
AC_CONFIG_SRCDIR([doschk.c])
AC_CONFIG_AUX_DIR([../../../])
AC_CONFIG_HEADERS([config.h])
AC_CANONICAL_TARGET
AC_SYS_LARGEFILE
AC_SYS_LONG_FILE_NAMES

# Checks for programs.
my_CC=${CC-cc}
AC_SUBST([my_CC])

if test "x${CC}" = "x"; then
	test -z "${CC}"
	AC_PROG_CC
else
	test ! -z "${CC}" && export CC
fi
AC_PROG_LN_S
AC_PROG_INSTALL
AM_PROG_INSTALL_SH

# Checks for header files.
AC_CHECK_HEADERS([ctype.h malloc.h malloc/malloc.h stdio.h])

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([exit fgets fprintf memset printf putchar qsort \
                strcmp strcpy strlen strncmp strncpy strrchr xmalloc])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
