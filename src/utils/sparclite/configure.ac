dnl#                                               -*- Autoconf -*-
dnl# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.3])dnl
AC_INIT([sparclite],[???],[/dev/null])
AC_CONFIG_SRCDIR([aload.c])
AC_CONFIG_HEADERS([config.h])dnl

my_CC=${CC-cc}
AC_SUBST([my_CC])dnl
AC_PROG_CC
AC_USE_SYSTEM_EXTENSIONS
AC_SEARCH_LIBS([strerror],[cposix]) dnl# used to be AC\_ISC\_POSIX

AC_PROG_INSTALL

AC_CONFIG_AUX_DIR([../..])
AC_CANONICAL_TARGET
  ## set these for use by the Makefile:
if test -z "${HOST_ARCHITECTURE}"; then
  if test -n "${host_cpu}"; then
    export HOST_ARCHITECTURE="${host_cpu}"
  elif test -n "`uname -m`"; then
    export HOST_ARCHITECTURE="`uname -m`"
  else
    export HOST_ARCHITECTURE="x86_64"
  fi
  AC_SUBST([HOST_ARCHITECTURE])dnl
fi
if test -z "${TARGET_ARCHITECTURE}"; then
  if test -n "${target_cpu}"; then
    export TARGET_ARCHITECTURE="${target_cpu}"
  else
    export TARGET_ARCHITECTURE="sparclite"
  fi
  AC_SUBST([TARGET_ARCHITECTURE])dnl
  ## TODO: might also want to do likewise with the build arch...
fi
AC_ARG_PROGRAM

# Checks for libraries.
AC_MSG_CHECKING([for linker flag to verify linked libraries have the correct architecture])
# Make sure this is done here:
if test "x${acl_cv_wl}" = "x"; then
  test -z "${acl_cv_wl}" && export acl_cv_wl='-Wl,'
fi
pre_saved_LDFLAGS="${LDFLAGS}"
LDFLAGS="${LDFLAGS} ${acl_cv_wl}-arch_errors_fatal"
AC_LINK_IFELSE([AC_LANG_PROGRAM([[]],[[]])],
               [LD_ARCHFLAGS="${acl_cv_wl}-arch_errors_fatal"],[])
LDFLAGS="${pre_saved_LDFLAGS}"
if test "x${LD_ARCHFLAGS}" != "x"; then
  test -n "${LD_ARCHFLAGS}"
  AC_MSG_RESULT([${LD_ARCHFLAGS}])
  AC_SUBST([LD_ARCHFLAGS])dnl
  ## that should be all we need to do with them for now...
else
  test -z "${LD_ARCHFLAGS}"
  AC_MSG_RESULT([none])
fi
# check arch when "main()" is the only function for which we check:
ARCHLESS_LDFLAGS="${LDFLAGS}"
LDFLAGS="${LDFLAGS} ${LD_ARCHFLAGS}"
AC_CHECK_LIB([bfd],[main])dnl
  ## reset these:
LDFLAGS="${ARCHLESS_LDFLAGS}"
AC_SEARCH_LIBS([bfd_check_format],[bfd])
AC_SEARCH_LIBS([bfd_get_section_contents],[bfd])
AC_SEARCH_LIBS([bfd_openr],[bfd])dnl

# Checks for header files.
AC_HEADER_TIME
AC_CHECK_HEADERS([errno.h fcntl.h netdb.h netinet/in.h stdarg.h stdio.h \
                  sys/socket.h sys/time.h varargs.h])dnl
  ## 1 of 3:
AC_CHECK_HEADERS([termios.h],
                 [AC_DEFINE([HAVE_TERMIOS],[1],
                            [Define to 1 if we have termios.h])
                  termio=termios])dnl
  ## 2 of 3:
AC_CHECK_HEADERS([termio.h],
                 [AC_DEFINE([HAVE_TERMIO],[1],
                            [Define to 1 if we have termio])
                  termio=termio])dnl
  ## 3 of 3:
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
