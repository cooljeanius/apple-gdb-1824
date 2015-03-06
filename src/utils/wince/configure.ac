dnl#                                               -*- Autoconf -*-
dnl# Process this file with autoconf to produce a configure script.
AC_PREREQ([2.3])dnl
AC_INIT([wince],[???],[/dev/null])
AC_CONFIG_SRCDIR([cesetup.c])
AC_CONFIG_HEADERS([config.h])dnl

my_CC=${CC-cc}
AC_SUBST([my_CC])dnl
AC_PROG_CC
AC_EXEEXT

AC_PROG_INSTALL

AC_CONFIG_AUX_DIR([../..])
AC_CANONICAL_TARGET
AC_ARG_PROGRAM

PROGS='cesetup$(EXEEXT)'
AC_SUBST([PROGS])dnl

# Checks for libraries.
#FIXME: verify that libraries are the correct architecture; like other dirs
AC_CHECK_LIB([c],[printf])dnl

# Checks for header files.
AC_HEADER_DIRENT
AC_CHECK_HEADERS([ctype.h stdarg.h stdio.h])dnl

# Checks for typedefs, structures, and compiler characteristics.
AC_C_BIGENDIAN
AC_C_INLINE
AC_C_PROTOTYPES
AC_CHECK_SIZEOF([int])dnl
AC_CHECK_ALIGNOF([int])dnl

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_STAT
AC_CHECK_FUNCS([exit fclose fopen fprintf fputs isupper memcmp memset \
                memcpy strcasecmp strchr strncasecmp strstr tolower])dnl

# Output.
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
