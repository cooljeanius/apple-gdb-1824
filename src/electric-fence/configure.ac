dnl#                                               -*- Autoconf -*-
dnl# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])dnl
AC_INIT([electric-fence],[2.0.5],[Bruce@Pixar.com])
AC_CONFIG_SRCDIR([efence.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LN_S
m4_ifdef([AM_PROG_AR],[
AM_PROG_AR
],[
AC_CHECK_TOOL([AR],[ar])
])
m4_ifdef([AC_PROG_RANLIB],[
AC_PROG_RANLIB
],[
AC_CHECK_TOOL([RANLIB],[ranlib],[:])
])
AC_PATH_PROG([NROFF_PROG],[nroff])
AC_PATH_PROG([SHAR_PROG],[shar])

# Checks for libraries.
AC_CHECK_LIB([pthread],[pthread_exit])

# Checks for library functions.
AC_FUNC_MMAP

# Checks for header files.
AC_CHECK_HEADERS([errno.h fcntl.h limits.h mach/mach.h malloc.h \
                  malloc/malloc.h math.h pthread.h setjmp.h signal.h \
                  stddef.h stdio.h])

# Checks for declarations.
AC_CHECK_DECLS([sys_nerr])
AC_CHECK_DECLS([sys_errlist])

AC_CANONICAL_TARGET([])
AC_ARG_PROGRAM
AC_EXEEXT
AC_SUBST([EXEEXT])

case "${host}" in
*-apple-rhapsody*|*-apple-macos10*|*-apple-darwin*) PAGEFILE=nspage ;;
*-*-winnt*|*-*-cygwin*) PAGEFILE=ntpage ;;
*) PAGEFILE=page ;;
esac

# Checks for typedefs, structures, and compiler characteristics.
AC_SYS_LARGEFILE
AC_C_BIGENDIAN
AC_C_PROTOTYPES
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_CHECK_TYPES([caddr_t])
AC_CHECK_TYPES([kern_return_t])

# Checks for library functions not checked above.
unset ac_cv_header_stdlib_h
AC_FUNC_MALLOC
unset ac_cv_func_getpagesize
AC_CHECK_FUNCS([atoi calloc exit _exit free getpagesize getpid memset \
                mprotect munmap strerror write])

AC_SUBST([PAGEFILE])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
