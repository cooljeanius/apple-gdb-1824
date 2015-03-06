dnl#                                               -*- Autoconf -*-
dnl# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.3])dnl
AC_INIT([sparclite],[???],[/dev/null])
AC_CONFIG_SRCDIR([aload.c])
AC_CONFIG_HEADERS([config.h])dnl

my_CC=${CC-cc}
AC_SUBST([my_CC])dnl
AC_PROG_CC
AC_PROG_GCC_TRADITIONAL
AC_USE_SYSTEM_EXTENSIONS
AC_SEARCH_LIBS([strerror],[cposix])dnl

AC_PROG_INSTALL

AC_CONFIG_AUX_DIR([../..])
AC_CANONICAL_TARGET
AC_ARG_PROGRAM

# Checks for libraries.
#FIXME: verify that libraries are the correct architecture; like other dirs
AC_CHECK_LIB([bfd],[main])
AC_SEARCH_LIBS([bfd_check_format],[bfd])
AC_SEARCH_LIBS([bfd_get_section_contents],[bfd])
AC_SEARCH_LIBS([bfd_openr],[bfd])dnl

# Checks for header files.
AC_HEADER_TIME
AC_CHECK_HEADERS([errno.h fcntl.h netdb.h netinet/in.h stdarg.h stdio.h \
                  sys/socket.h sys/time.h varargs.h])dnl
AC_CHECK_HEADERS([termios.h],
                 [AC_DEFINE([HAVE_TERMIOS],[1],
                            [Define to 1 if we have termios.h])
                  termio=termios])dnl
AC_CHECK_HEADERS([termio.h],
                 [AC_DEFINE([HAVE_TERMIO],[1],
                            [Define to 1 if we have termio])
                  termio=termio])dnl
AC_CHECK_HEADERS([sgtty.h],
                 [AC_DEFINE([HAVE_SGTTY],[1],
                            [Define to 1 if we have sgtty])
                  termio=sgtty])dnl

# disable standalone load programs on DOS/GO32 hosts:
changequote(,)dnl
PROGS="aload eload"
case "${host}" in
i[3456]86-*-go32*)	
	PROGS=""
	;;
*)
esac
changequote([,])dnl
AC_SUBST([PROGS])dnl

# check for networking libraries.
NETLIBS=""
AC_CHECK_LIB([nsl],[t_accept],[NETLIBS="${NETLIBS} -lnsl"])dnl

# -lsocket is required on some systems, but is broken on IRIX.
if test "`(uname) 2> /dev/null`" != "IRIX"; then
	AC_CHECK_LIB([socket],[socket],[NETLIBS="${NETLIBS} -lsocket"])
fi
AC_SUBST([NETLIBS])dnl

# Checks for typedefs, structures, and compiler characteristics.
AC_C_BIGENDIAN
AC_C_CONST
AC_C_PROTOTYPES
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_CHECK_SIZEOF([int])dnl
AC_CHECK_ALIGNOF([int])dnl

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_FUNC_SELECT_ARGTYPES
AC_FUNC_VPRINTF
AC_CHECK_FUNCS([connect exit fprintf fputc gethostbyname isupper memcpy \
                recv select send socket strerror tolower vfprintf])dnl
AC_CHECK_DECLS_ONCE([basename])dnl

# Output.
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
