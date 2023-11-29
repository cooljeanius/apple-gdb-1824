dnl#                                               -*- Autoconf -*-
dnl# Process this file file with autoconf to produce a configure script.
dnl# This file is a shell script fragment that supplies the information
dnl# necessary to tailor a template configure script into the configure
dnl# script appropriate for this directory.  For more information, check
dnl# any existing configure script.

AC_PREREQ([2.70])dnl
AC_INIT
AC_CONFIG_SRCDIR([pthreads.exp])dnl
AC_CONFIG_HEADERS([config.h:config_h.in])dnl

MY_CC=${CC-cc}
AC_SUBST([MY_CC])dnl

AC_CONFIG_AUX_DIR([../../..])
AC_CANONICAL_TARGET

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL

# Checks for header files.
AC_CHECK_HEADERS([pthread.h stddef.h sys/time.h])dnl

if test "x${ac_cv_header_stdint_h}" = "x"; then
  test -z "${ac_cv_header_stdint_h}"
  AC_CHECK_HEADERS([stdint.h])
fi
if test "x${ac_cv_header_unistd_h}" = "x"; then
  test -z "${ac_cv_header_unistd_h}"
  AC_CHECK_HEADERS([unistd.h])
fi

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_PID_T
AC_TYPE_SIZE_T
define([AC_PTHREAD_INCLUDES],[[
#if !defined(__has_include)
# define __has_include(foo) 0
#endif /* !__has_include */
#if defined(HAVE_PTHREAD_H) || __has_include(<pthread.h>)
# include <pthread.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning \"conftest for pthread_t wants to include <pthread.h>.\"
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_PTHREAD_H */]])dnl
  ## pthread_t may require special includes:
AC_CHECK_TYPES([pthread_t],[
  AC_CHECK_SIZEOF([pthread_t],[],[AC_PTHREAD_INCLUDES])
  AC_CHECK_ALIGNOF([pthread_t],[
AC_PTHREAD_INCLUDES
#if defined(HAVE_STDDEF_H) || __has_include(<stddef.h>) || defined(__clang__)
# include <stddef.h>
#endif /* HAVE_STDDEF_H || __clang__ */])dnl
],[],[AC_PTHREAD_INCLUDES])dnl

# Checks for library functions.
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_CHECK_FUNCS([alarm atexit clock_gettime getpid memset pthread_self select \
                sleep strchr strerror strtol])dnl

# Output.
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
