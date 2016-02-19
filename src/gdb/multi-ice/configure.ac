dnl#                                               -*- Autoconf -*-
dnl# Process this file with autoconf to produce a configure script.

dnl# original value, in case autoupdate changes it: 2.13
AC_PREREQ([2.13])dnl

dnl# I found this dir via eCos, and their bug list is read-only, so use the
dnl# next best thing:
AC_INIT([multi-ice-server],[1.0],[ecos-devel@ecos.sourceware.org])
AC_CONFIG_SRCDIR([server-main.c])
AC_CONFIG_AUX_DIR([..]) dnl# (maybe one level further up?)
AC_CONFIG_MACRO_DIR([../m4])dnl

# architecture stuff:
AC_CANONICAL_TARGET
case ${host_os} in
  *cygwin* ) CYGWIN=yes;;
	 * ) CYGWIN=no;;
esac

if test -z "${HOST_ARCHITECTURE}" -a -n "${host_cpu}"; then
  export HOST_ARCHITECTURE="${host_cpu}"
  AC_SUBST([HOST_ARCHITECTURE])
fi
if test -z "${TARGET_ARCHITECTURE}" -a -n "${target_cpu}"; then
  export TARGET_ARCHITECTURE="${target_cpu}"
  AC_SUBST([TARGET_ARCHITECTURE])
fi

# Automake:
AM_INIT_AUTOMAKE([1.11.6 foreign dejagnu])
AM_MAINTAINER_MODE

AC_CONFIG_HEADERS([config.h])
AH_TOP([
#ifndef HAVE_CONFIG_H
# define HAVE_CONFIG_H 1
#endif /* !HAVE_CONFIG_H */
])dnl
AH_VERBATIM([00_CONFIG_H_CHECK],
[/* Check that config.h is #included before system headers
  * (this works only for glibc, but that should be enough).  */
#if defined(__GLIBC__) && !defined(__FreeBSD_kernel__) && !defined(__CONFIG_H__)
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "config.h should be #included before system headers"
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* __GLIBC__ && !__FreeBSD_kernel__ && !__CONFIG_H__ */
#ifndef __CONFIG_H__
# define __CONFIG_H__ 1
# ifndef __MULTI_ICE_CONFIG_H__
#  define __MULTI_ICE_CONFIG_H__ 1
# endif /* __MULTI_ICE_CONFIG_H__ */])dnl
AH_BOTTOM([
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  if defined(__GDB_CONFIG_H__)
#   warning "the gdb-general config.h might have already been included"
#  elif defined(__MULTI_ICE_CONFIG_H__)
#   warning "this config.h might have already been included"
#  else
#   warning "a different config.h might have already been included, not sure which though"
#  endif /* __GDB_CONFIG_H__ || __MULTI_ICE_CONFIG_H__ */
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* __CONFIG_H__ */
])dnl

AC_PROG_CC
AC_PROG_CPP

AC_DEFUN([AC_REQUIRE_GREPS],[
  AC_REQUIRE([AC_PROG_GREP])dnl
  AC_REQUIRE([AC_PROG_EGREP])dnl
  AC_REQUIRE([AC_PROG_FGREP])dnl
])dnl
AC_REQUIRE_GREPS

AC_EXEEXT
AC_OBJEXT
WINDRES=${WINDRES-windres}
AC_SUBST([WINDRES])dnl

# In the cygwin32 environment, we need some additional flags.
AC_CACHE_CHECK([for cygwin32],[multi_ice_cv_os_cygwin32],[
AC_EGREP_CPP([you_lose],[
#ifdef __CYGWIN32__
you_lose
#endif /* __CYGWIN32__ */
],[multi_ice_cv_os_cygwin32=yes],[multi_ice_cv_os_cygwin32=no])])
if test "x${multi_ice_cv_os_cygwin32}" = "xyes"; then
  WIN32LIBS="-lshell32 -ladvapi32 -lgdi32 -luser32"
  WIN32LDAPP="-Wl,--subsystem,console"
  WIN32RES="MICE-gdbserverres.o"
  MULTI_ICE_PLATFORM_SOURCE=server-win.c
else
  WIN32LIBS=""
  WIN32LDAPP=""
  WIN32RES=""
  MULTI_ICE_PLATFORM_SOURCE=""
fi
AC_SUBST([WIN32LIBS])dnl
AC_SUBST([WIN32LDAPP])dnl
AC_SUBST([WIN32RES])dnl
AC_SUBST([MULTI_ICE_PLATFORM_SOURCE])dnl

# This is the location of the ARM Multi-Ice header files, 
# and the toolconf library file (for Jim Ingham at least).
MULTI_ICE_INCLUDES="-I/home/jingham/arm/multi-ice"
MULTI_ICE_LIBS="-L/home/jingham/arm/multi-ice -L. -ltoolconf"
AC_SUBST([MULTI_ICE_INCLUDES])dnl
AC_SUBST([MULTI_ICE_LIBS])dnl

# This is where I am putting them:
M_I_INCLUDE_DIR="${srcdir}/include/arm/multi-ice"
MULTI_ICE_INCLUDES="${MULTI_ICE_INCLUDES} -I${M_I_INCLUDE_DIR}"
AC_SUBST([M_I_INCLUDE_DIR])dnl

AM_CONDITIONAL([WINDOWS],[test "x${multi_ice_cv_os_cygwin32}" = "xyes"])dnl

dnl# List of object files added by configure.
CONFIG_OBS=""
CONFIG_DEPS=""
CONFIG_SRCS=""

dnl# Checks for programs.
AC_PROG_CXX
AC_PROG_GCC_TRADITIONAL
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_RANLIB

dnl# Checks for libraries.
if test "x${acl_cv_wl}" = "x"; then
  test -z "${acl_cv_wl}" && export acl_cv_wl='-Wl,'
fi
AC_SUBST([acl_cv_wl])dnl

AC_MSG_CHECKING([for linker flag to verify linked libraries have the correct architecture])
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
AC_CHECK_LIB([iberty],[xmalloc],[],[
  PRE_LIBERTY_LDFLAGS="${LDFLAGS}"
  LDFLAGS="${LDFLAGS} ${LD_ARCHFLAGS}"
  AC_CHECK_LIB([iberty],[main])dnl
  ## reset:
  LDFLAGS="${PRE_LIBERTY_LDFLAGS}"
])dnl

dnl# Checks for header files.
AC_DEFUN([AC_REQUIRE_HEADER_STDC],[
  AC_REQUIRE([AC_HEADER_STDC])dnl
])dnl
AC_REQUIRE_HEADER_STDC
AC_HEADER_STDBOOL dnl# also "_CHECK"s it
AC_HEADER_TIME
AC_HEADER_TIOCGWINSZ
AC_SYS_POSIX_TERMIOS
unset ac_cv_header_sys_types_h
AC_CHECK_HEADERS([arpa/inet.h basetsd.h errno.h fcntl.h getopt.h \
                  intsafe.h limits.h locale.h mach/vm_types.h \
                  netdb.h netinet/in.h netinet/tcp.h \
                  signal.h stdarg.h stddef.h stdio.h \
                  sys/file.h sys/ioctl.h sys/signal.h sys/time.h \
                  sys/types.h termios.h termio.h unix.h \
                  wincon.h windef.h windows.h winnt.h])dnl
  ## needs other headers first on some systems:
AC_CHECK_HEADERS([mach/kmod.h],[],[],[[
#ifdef HAVE_MACH_VM_TYPES_H
# include <mach/vm_types.h>
#endif /* HAVE_MACH_VM_TYPES_H */
]])dnl
  ## for "../config.h":
EXTRA_DEFS=""
if test "x${ac_cv_header_sys_types_h}" = "xyes"; then
  # this has to go separate from the configure header, because we check
  # for it to be defined at the top of it (for caddr_t for ptrace):
  export DEFS="${DEFS} -DHAVE_SYS_TYPES_H"
  export EXTRA_DEFS="-DHAVE_SYS_TYPES_H"
fi
AC_SUBST([EXTRA_DEFS])dnl

dnl# Checks for typedefs, structures, and compiler characteristics.
AC_C_BIGENDIAN
AC_C_CONST
AC_C_PROTOTYPES
AC_C_RESTRICT
AC_TYPE_INT8_T
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_INTMAX_T
AC_TYPE_INTPTR_T
AC_TYPE_UINTMAX_T
AC_TYPE_UINTPTR_T
AC_TYPE_SIZE_T
dnl# begin what used to be AC\_TYPE\_SIGNAL
AC_CACHE_CHECK([return type of signal handlers],[ac_cv_type_signal],[
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <sys/types.h>
#include <signal.h>
]],[[return *(signal(0, 0)) (0) == 1;]])],
		   [ac_cv_type_signal=int],
		   [ac_cv_type_signal=void])])
AC_DEFINE_UNQUOTED([RETSIGTYPE],[${ac_cv_type_signal}],
                   [Define as the return type of signal handlers
		    (`int' or `void').])dnl
dnl# end what used to be AC\_TYPE\_SIGNAL
AC_CHECK_TYPES([caddr_t])dnl
AC_CHECK_TYPES([socklen_t],[],[],[
#include <sys/types.h>
#include <sys/socket.h>
])dnl

dnl# Checks for library functions.
AC_FUNC_ALLOCA
AC_FUNC_MALLOC
AC_FUNC_SELECT_ARGTYPES
AC_FUNC_STRTOD
AC_FUNC_VPRINTF
AC_CHECK_FUNCS([gethostbyaddr getpeername inet_ntoa low_stop memset \
                select signal socket strchr strerror strstr strtol])dnl
dnl# ones that might actually be from this source tree itself:
AC_CHECK_FUNCS([disable_async_io enable_async_io])dnl

dnl# Checks for declarations...
dnl# ...for things that look like macros:
AC_CHECK_DECLS([REGISTER_BYTE, REGISTER_BYTES, REGISTER_RAW_SIZE])dnl
dnl# ...for functions that might actually be from this source tree:
AC_CHECK_DECLS([check_for_SIGIO, disable_async_io, enable_async_io])dnl
dnl# ...for other functions:
AC_CHECK_DECLS([basename, getopt, strerror, strstr])dnl
dnl# for some signal things:
AC_CHECK_DECLS([SIGINT, SIGTERM, SIGQUIT])dnl

dnl# Checks for compiler warning flags:
build_warnings="-Wimplicit -Wreturn-type -Wcomment -Wtrigraphs \
-Wparentheses -Wdeclaration-after-statement \
-Wpointer-arith -Wpointer-sign \
-Wno-unused -Wunused-label -Wunused-macros -Wswitch \
-Wempty-body -Wmissing-parameter-type -Wmissing-declarations \
-Wold-style-declaration -Wold-style-definition -Wstrict-prototypes \
-Wabi -Wmissing-noreturn -Waddress -Warray-bounds -Wattributes \
-Wswitch -Wclobbered -Wcoverage-mismatch -Wdiv-by-zero \
-Wdouble-promotion -Wenum-compare -Wfree-nonheap-object \
-Winvalid-memory-model -Winvalid-pch -Wmain -Wmissing-braces \
-Wmissing-include-dirs -Woverflow -Woverlength-strings -Woverride-init \
-Wpragmas -Wreturn-local-addr -Wsequence-point -Wsizeof-pointer-memaccess \
-Wtrampolines -Wunknown-pragmas -Wnormalized=nfkc -Wnull-dereference \
-Wunsuffixed-float-constants -Wstrict-overflow -Wjump-misses-init \
-Wmisleading-indentation -Wno-format-security -Wno-format"

case "${host}" in
  *-apple-darwin* | *-apple-macos*)
    # APPLE_LOCAL
    build_warnings="${build_warnings} -Wextra-tokens -Wnewline-eof \
-Wshorten-64-to-32 -std=gnu89"
  ;;
esac

# GCC supports -Wuninitialized only with -O or -On, n != 0.
# and also -Winit-self only with -Wuninitialized
if test x${CFLAGS+set} = xset; then
  case "${CFLAGS}" in
    *"-O0"* ) ;;
    *"-O"* )
      build_warnings="${build_warnings} -Wuninitialized -Winit-self"
    ;;
  esac
else
  build_warnings="${build_warnings} -Wuninitialized -Winit-self"
fi

WARN_CFLAGS=""
BAD_WARN_CFLAGS=""
WERROR_CFLAGS=""
if test "x${build_warnings}" != "x" -a "x${GCC}" = "xyes"
then
    AC_MSG_CHECKING([compiler warning flags])
    # Separate out the -Werror flag as some files just cannot be
    # compiled with it enabled.
    for w in ${build_warnings}; do
	case ${w} in
	*) # Check that GCC accepts it:
	    saved_CFLAGS="${CFLAGS}"
	    CFLAGS="${CFLAGS} ${w}"
	    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[]],[[]])],
                              [WARN_CFLAGS="${WARN_CFLAGS} ${w}"],
                              [BAD_WARN_CFLAGS="${BAD_WARN_CFLAGS} ${w}"])
	    CFLAGS="${saved_CFLAGS}"
	esac
    done
    AC_MSG_RESULT([using: ${WARN_CFLAGS}${WERROR_CFLAGS}])
    if test "x${BAD_WARN_CFLAGS}" != "x"; then
      test -n "${BAD_WARN_CFLAGS}"
      AC_MSG_WARN([compiler failed to accept: ${BAD_WARN_CFLAGS}])
    fi
fi
AC_SUBST([WARN_CFLAGS])dnl
AC_SUBST([WERROR_CFLAGS])dnl

dnl# Output:
AC_CONFIG_FILES([Makefile])dnl
AC_OUTPUT
