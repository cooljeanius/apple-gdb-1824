dnl#                                               -*- Autoconf -*-
dnl# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.12])
AC_INIT([amd-udi],[2.2],[/dev/null])
AC_CONFIG_SRCDIR([mondfe/bkpt.c])
AC_CONFIG_AUX_DIR([../..])
AC_CONFIG_HEADERS([config.h])
AC_CANONICAL_TARGET
AC_EXEEXT
AC_OBJEXT
AM_INIT_AUTOMAKE([gnits dejagnu])
AM_MAINTAINER_MODE
AC_MSG_NOTICE([host is "${host}"])
dnl# TODO: fix this test:
case "${host}" in
  *-*-msdosdjgpp* | *-moss-msdos)
    if test ! -z "${DEFS}"; then
      if test -z "`echo ${DEFS} | grep MSDOS`"; then
        AC_MSG_WARN([You are on a DOS system but do not appear to define MSDOS])
      fi
    fi
    ;;
  *-*-*)
    if test ! -z "${DEFS}"; then
      AC_MSG_NOTICE([DEFS are ${DEFS}])
    fi
    ;;
esac
AC_MSG_NOTICE([target is "${target}"])
case "${target}" in
  a29k-amd-udi | udi29k | a29k-*-vxworks | a29k-none-*)
    export TARGET_IS_29K=yes_this_is_true
    ;;
  *-*-*)
    true
    ;;
esac

# Checks for programs.
MY_CC=${CC-cc}
AC_SUBST([MY_CC])dnl
AC_PROG_AR
AC_PROG_CC
AC_PROG_CPP
AC_PROG_CXX
AC_PROG_EGREP
AC_PROG_INSTALL
AC_PROG_MKDIR_P
AC_PROG_RANLIB
AM_PROG_AS
AM_CONDITIONAL([LINKER_IS_COMPILER],[test "x${LD}" = "x${CC}"])dnl

# Checks for header files.
m4_ifdef([AC_HEADER_TIME],[
  AC_HEADER_TIME dnl# (technically obsolete now, but keep it anyways)
])dnl
AC_SYS_POSIX_TERMIOS
AC_HEADER_TIOCGWINSZ
AC_CHECK_HEADERS([arpa/inet.h bios.h conio.h ctype.h dos.h errno.h \
                  fcntl.h io.h malloc.h malloc/malloc.h \
                  netdb.h netinet/in.h  setjmp.h sgtty.h signal.h \
                  socket.h stdio.h sys/fcntl.h sys/file.h sys/ioctl.h \
                  sys/ioctl_compat.h sys/socket.h sys/sockio.h \
                  sys/time.h sys/timeb.h term.h termio.h termios.h])dnl

if test "x${ac_cv_header_strings_h}" = "x"; then
  test -z "${ac_cv_header_strings_h}" || unset ac_cv_header_strings_h
  AC_CHECK_HEADERS([strings.h])
fi
if test "x${ac_cv_header_unistd_h}" = "x"; then
  test -z "${ac_cv_header_unistd_h}" || unset ac_cv_header_unistd_h
  AC_CHECK_HEADERS([unistd.h])
fi

# Checks for typedefs, structures, and compiler characteristics.
AC_C_BIGENDIAN
AC_C_PROTOTYPES
AC_TYPE_OFF_T
AC_TYPE_SIZE_T
SC_SERIAL_PORT
BASH_SYS_SIGNAL_VINTAGE
BASH_SYS_REINSTALL_SIGHANDLERS
BASH_SYS_JOB_CONTROL_MISSING
BASH_HAVE_TIOCGWINSZ
BASH_HAVE_TIOCSTAT
BASH_HAVE_FIONREAD
BASH_STRUCT_TERMIOS_LDISC
BASH_STRUCT_TERMIO_LDISC
BASH_STRUCT_WINSIZE
BASH_CHECK_SPEED_T
AC_EXPECT_TCGETS_OR_TCGETA
AC_EXPECT_TIOCGWINSZ
AC_CHECK_DECLS([sys_errlist])dnl
AC_CHECK_DECLS([sys_nerr])dnl
AC_CHECK_DECLS([sys_siglist],[],[],
[#include <signal.h>
/* NetBSD declares sys_siglist in <unistd.h>: */
#ifdef HAVE_UNISTD_H
# include <unistd.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
 #  warning "the conftest for sys_siglist wants to include <unistd.h>"
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_UNISTD_H */
])dnl

# Checks for library functions.
AC_FUNC_ALLOCA
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_CHECK_FUNCS([atoi bcopy close execlp exit fprintf free ftime \
                gethostbyname gethostname getpid inet_addr ioctl \
                memmove memset printf read setpgrp sleep socket \
                strcasecmp strchr strcmp strlen strpbrk strrchr \
                system unlink write])dnl

# Output.
AC_CONFIG_FILES([Makefile Makefile_orig \
                 mondfe/Makefile mondfe/Makefile_orig \
                 montip/Makefile montip/Makefile_orig \
                 udi/Makefile udi/Makefile_orig])dnl
AC_OUTPUT
