dnl#                                               -*- Autoconf -*-
dnl# Configure script for readline library
dnl#
dnl# report bugs to chet@po.cwru.edu
dnl#
dnl# Process this file with autoconf to produce a configure script.
AC_REVISION([for Readline 4.3, version 2.45, from autoconf version] AC_ACVERSION)

AC_INIT([readline],[4.3],[bug-readline@gnu.org])dnl

dnl# make sure we are using a "recent" autoconf version
AC_PREREQ([2.61])dnl

AC_CONFIG_SRCDIR([readline.h])dnl
AC_CONFIG_AUX_DIR([support])dnl
AC_CONFIG_MACRO_DIR([m4])dnl
AC_CONFIG_HEADERS([config.h])dnl

dnl# update the value of RL_READLINE_VERSION in readline.h when this
dnl# changes:
LIBVERSION=4.3

AC_CANONICAL_TARGET
if test -z "${BUILD_ARCHITECTURE}" -a -n "${build_cpu}"; then
  export BUILD_ARCHITECTURE="${build_cpu}"
  AC_SUBST([BUILD_ARCHITECTURE])
fi
if test -z "${HOST_ARCHITECTURE}" -a -n "${host_cpu}"; then
  export HOST_ARCHITECTURE="${host_cpu}"
  AC_SUBST([HOST_ARCHITECTURE])
fi
if test -z "${TARGET_ARCHITECTURE}" -a -n "${target_cpu}"; then
  export TARGET_ARCHITECTURE="${target_cpu}"
  AC_SUBST([TARGET_ARCHITECTURE])
fi

dnl# configure defaults:
opt_curses=no

dnl# arguments to configure:
AC_ARG_WITH([curses],
            [AS_HELP_STRING([--with-curses],
                [use the curses library instead of the termcap library])],
            [opt_curses=${withval}])dnl

if test "${opt_curses}" = "yes"; then
	prefer_curses=yes
fi

dnl# option parsing for optional features:
opt_static_libs=yes
opt_shared_libs=no

AC_ARG_ENABLE([static],
              [AS_HELP_STRING([--enable-static],
                              [build static libraries [[default=YES]]])],
              [opt_static_libs=${enableval}])dnl

echo ""
echo "Beginning configuration for readline-${LIBVERSION} for ${host_cpu}-${host_vendor}-${host_os}"
echo ""

AM_SANITY_CHECK
AC_OBJEXT
AC_EXEEXT
AC_USE_SYSTEM_EXTENSIONS

AC_PROG_MAKE_SET
if test "x${CC}" = "x"; then
  test -z "${CC}"
  AC_PROG_CC
else
  test ! -z "${CC}" && export CC
  AC_SUBST([CC])dnl
  ## dummy
fi
XORG_PROG_RAWCPP
XORG_LD_WRAP

# We want these before the (rest of the) checks, so that the (rest of the)
# checks can modify their values.
test -z "${CFLAGS}" && CFLAGS="-ggdb"
test -z "${auto_cflags}" && auto_cflags="1"

dnl# BEGIN changes for CYGNUS cross-building for Cygwin
 
dnl# load up the cross-building cache file -- add more cases and cache
dnl# files as necessary
if test "x${cross_compiling}" = "xyes"; then
    case "${host}" in
    *-cygwin*)
	cross_cache=${srcdir}/cross-build/cygwin.cache
	LOCAL_CFLAGS="${LOCAL_CFLAGS} -I${srcdir}/../libtermcap"
	;;
   *-mingw32*)	
	cross_cache=${srcdir}/cross-build/mingw.cache
        ;;
    *)  echo "configure: cross-compiling for a non-cygwin target is not supported" >&2
	;;
    esac

    if test "x${cross_cache}" != "x"; then
	if test -r "${cross_cache}"; then
	    echo "loading cross-build cache file ${cross_cache}"
	    . ${cross_cache}
	fi
	unset cross_cache
    fi
fi
 
if test "x${cross_compiling}" = "xyes"; then
  CROSS_COMPILING_FLAG=-DCROSS_COMPILING
else
  CROSS_COMPILING_FLAG=""
fi
AC_SUBST([CROSS_COMPILING_FLAG])dnl
 
if test -z "${CC_FOR_BUILD}"; then
    if test "x${cross_compiling}" = "xno"; then
        CC_FOR_BUILD='$(CC)'
    else
        CC_FOR_BUILD=gcc
    fi
fi
AC_SUBST([CC_FOR_BUILD])dnl
 
dnl# END changes for CYGNUS cross-building for Cygwin

# If we are using gcc and the user has NOT specified CFLAGS, add -O1 to
# CFLAGS.
test -n "${GCC}" && test -n "${auto_cflags}" && CFLAGS="${CFLAGS} -O1"

# Checks for programs.
AC_MSG_CHECKING([programs])
AC_MSG_RESULT([ ])
AC_PROG_AWK
AC_PROG_GCC_TRADITIONAL
AC_DEFUN([AC_REQUIRE_GREPS],[
  AC_REQUIRE([AC_PROG_GREP])dnl
  AC_REQUIRE([AC_PROG_EGREP])dnl
  AC_REQUIRE([AC_PROG_FGREP])dnl
])dnl
AC_REQUIRE_GREPS
AC_PROG_INSTALL
AM_PROG_INSTALL_STRIP
AC_SUBST([INSTALLdir])dnl
AC_SUBST([INSTALLEDdir])dnl
AC_PROG_LN_S
AC_PROG_MKDIR_P
AC_CHECK_PROG([AR],[ar],[],[ar])
dnl# Set default for ARFLAGS, since autoconf does not have a macro for it.
dnl# This allows people to set it when running configure or make
dnl#TODO: automake does actually have the AM_PROG_AR macro,
dnl# try using that instead?
test -n "${ARFLAGS}" || ARFLAGS="cr"
if test "x${AR}" = "x"; then
  test -z "${AR}"
  AM_PROG_AR
fi
if test "x${RANLIB}" = "x"; then
  test -z "${RANLIB}"
  AC_PROG_RANLIB
else
  test ! -z "${RANLIB}"
fi
AC_DEFUN([AC_REQUIRE_PROG_SED],[
  AC_REQUIRE([AC_PROG_SED])dnl
])dnl
AC_REQUIRE_PROG_SED
AC_PATH_PROG([TEX_BINARY],[tex])dnl

# Checks for libraries.
AC_MSG_CHECKING([libraries])
AC_MSG_RESULT([ ])
AC_MSG_CHECKING([for linker flag to verify linked libraries have the correct architecture])
# Make sure this is done up here first:
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
AC_CHECK_LIB([c],[printf])dnl
# check arch when "main()" is the only function for which we check:
ARCHLESS_LDFLAGS="${LDFLAGS}"
LDFLAGS="${LDFLAGS} ${LD_ARCHFLAGS}"
# FIXME: Replace `main' with a function in `-lcurses':
AC_CHECK_LIB([curses],[main])dnl
# FIXME: Replace `main' with a function in `-lhistory':
AC_CHECK_LIB([history],[main])dnl
# FIXME: Replace `main' with a function in `-lreadline':
AC_CHECK_LIB([readline],[main])dnl
  ## reset these:
LDFLAGS="${ARCHLESS_LDFLAGS}"
AC_CHECK_LIB([iberty],[xmalloc],[],[
  PRE_LIBERTY_LDFLAGS="${LDFLAGS}"
  LDFLAGS="${LDFLAGS} ${LD_ARCHFLAGS}"
  AC_CHECK_LIB([iberty],[main])dnl
  ## reset:
  LDFLAGS="${PRE_LIBERTY_LDFLAGS}"
])dnl

MAKE_SHELL=/bin/sh
AC_SUBST([MAKE_SHELL])dnl

# Checks for typedefs, structures, and compiler characteristics.
AC_MSG_CHECKING([typedefs, structures, and compiler characteristics])
AC_MSG_RESULT([ ])
AC_C_BIGENDIAN
AC_C_CONST
AC_C_PROTOTYPES
AC_C_CHAR_UNSIGNED
AX_GCC_MALLOC_CALL
dnl# Begin what used to be AC\_TYPE\_SIGNAL
AC_CACHE_CHECK([return type of signal handlers],[ac_cv_type_signal],[
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <sys/types.h>
#include <signal.h>
]],[[
return *(signal (0, 0)) (0) == 1;
]])],[ac_cv_type_signal=int],[ac_cv_type_signal=void])dnl
])dnl
  ## corresponding define:
AC_DEFINE_UNQUOTED([RETSIGTYPE],[${ac_cv_type_signal}],
                   [Define as the return type of signal handlers
                    (`int' or `void').])dnl
dnl# End what used to be AC\_TYPE\_SIGNAL
AC_TYPE_UID_T
AC_TYPE_PID_T
AC_CHECK_TYPES([ptrdiff_t])dnl
AC_TYPE_SIZE_T
AC_CHECK_TYPE([ssize_t],[int])dnl

# Checks for library functions.
AC_MSG_CHECKING([functions])
AC_MSG_RESULT([ ])
AM_WITH_DMALLOC
AC_FUNC_ALLOCA
AC_FUNC_CHOWN
AC_FUNC_ERROR_AT_LINE
AC_FUNC_FORK
AC_FUNC_LSTAT dnl# also checks whether it "_FOLLOWS_SLASHED_SYMLINK"
AC_FUNC_MALLOC
AC_FUNC_MBRTOWC
AC_FUNC_MMAP
AC_FUNC_REALLOC
AC_FUNC_STAT
AC_FUNC_STRCOLL
AC_FUNC_STRNLEN
AC_FUNC_STRTOD
AC_DEFUN([gl__ALLOC_CHECKS],[
  AC_REQUIRE([gl_EEALLOC])dnl
  AC_REQUIRE([gl_FUNC_MALLOC_GNU])dnl
  AC_REQUIRE([gl_FUNC_MALLOC_POSIX])dnl
  AC_REQUIRE([gl_FUNC_REALLOC_GNU])dnl
  AC_REQUIRE([gl_FUNC_REALLOC_POSIX])dnl
  AC_REQUIRE([gl_MALLOCA])dnl
  AC_REQUIRE([gl_XALLOC])dnl
])dnl# end gl__ALLOC_CHECKS
dnl# now actually call it:
gl__ALLOC_CHECKS
XORG_CHECK_MALLOC_ZERO
XORG_MEMORY_CHECK_FLAGS
AX_CHECK_PAGE_ALIGNED_MALLOC
AC_CHECK_FUNCS([atoi ctime dup2 endpwent exit fcntl fprintf free \
                ftruncate getcwd getopt getpwent getpwnam getpwuid \
                isascii isxdigit kill lstat mbrlen memmove memset \
                munmap putenv select setenv setlocale strcasecmp \
                strchr strcpy strdup strerror strlen strncpy strpbrk \
                strrchr system tcgetattr vsnprintf wcwidth xrealloc])dnl
AC_REPLACE_FUNCS([xmalloc])dnl
AC_SEARCH_LIBS([xmalloc],[iberty c gnu])dnl
AC_SEARCH_LIBS([xrealloc],[iberty c gnu])dnl
AC_SEARCH_LIBS([rpl_realloc],[iberty c gnu])dnl
AC_SEARCH_LIBS([rpl_malloc],[iberty c gnu])dnl
AC_REPLACE_FUNCS([rpl_malloc])dnl
AC_REPLACE_FUNCS([rpl_realloc])dnl
AC_CHECK_DECLS([exit, free, strchr, strcpy, strlen, strncpy])dnl

# Checks for header files.
AC_MSG_CHECKING([headers])
AC_MSG_RESULT([ ])
AC_HEADER_DIRENT
AC_HEADER_STDBOOL dnl# also "_CHECK"s it.
AC_HEADER_STAT
AC_HEADER_TIME
AC_HEADER_TIOCGWINSZ
AC_CHECK_HEADERS([arpa/inet.h errno.h fcntl.h features.h limits.h \
                  locale.h malloc.h malloca.h malloc/malloc.h netdb.h \
                  netinet/in.h pwd.h readline/readline.h \
                  readline/history.h realloc.h select.h sgtty.h \
                  signal.h stdarg.h stddef.h stdio.h sys/file.h \
                  sys/ioctl.h sys/pte.h sys/ptem.h sys/select.h \
                  sys/socket.h sys/stream.h sys/time.h termcap.h \
                  termio.h termios.h time.h varargs.h wchar.h wctype.h])dnl

# Checks using macros from Bash's configure script
AC_MSG_NOTICE([running checks stolen from Bash's configure script])
BASH_SYS_SIGNAL_VINTAGE
BASH_SYS_REINSTALL_SIGHANDLERS

BASH_FUNC_POSIX_SETJMP
BASH_FUNC_LSTAT
BASH_FUNC_STRCOLL

BASH_CHECK_GETPW_FUNCS

BASH_TYPE_SIGHANDLER
BASH_HAVE_TIOCSTAT
BASH_HAVE_FIONREAD
BASH_CHECK_OFF_T_64
BASH_CHECK_SPEED_T
BASH_STRUCT_WINSIZE
BASH_STRUCT_DIRENT_D_INO
BASH_STRUCT_DIRENT_D_FILENO
BASH_STRUCT_ST_BLOCKS

dnl# yuck
case "${host_os}" in
aix*)   prefer_curses=yes ;;
esac
BASH_CHECK_LIB_TERMCAP
if test "${TERMCAP_LIB}" = "./lib/termcap/libtermcap.a"; then
	if test "${prefer_curses}" = yes; then
		TERMCAP_LIB=-lcurses
	else
		TERMCAP_LIB=-ltermcap	# default
	fi
fi

BASH_CHECK_MULTIBYTE

case "${host_cpu}" in
*cray*)	LOCAL_CFLAGS=-DCRAY ;;
*s390*) LOCAL_CFLAGS=-fsigned-char ;;
esac

case "${host_os}" in
isc*)	LOCAL_CFLAGS=-Disc386 ;;
esac

# Automake:
AC_MSG_CHECKING([automake-related things])
AC_MSG_RESULT([ ])
if test -f ${srcdir}/config.status; then
  AC_MSG_NOTICE([config.status is already present, removing it.])
  rm -f ${srcdir}/config.status
fi
AM_INIT_AUTOMAKE([1.11.6 dejagnu gnits -Wall])
AM_MAINTAINER_MODE
m4_ifdef([AC_REQUIRE_AUX_FILE],[
  AC_REQUIRE_AUX_FILE([compile])dnl
  AC_REQUIRE_AUX_FILE([missing])dnl
])dnl

AH_TOP([
#ifndef HAVE_CONFIG_H
# define HAVE_CONFIG_H 1
#endif /* !HAVE_CONFIG_H */
])dnl
AH_VERBATIM([00_CONFIG_H_CHECK],[
/* Check that config.h is #included before system headers
 * (this works only for glibc, but that should be enough). */
#if defined(__GLIBC__) && !defined(__FreeBSD_kernel__) && !defined(__CONFIG_H__)
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
 #  warning "config.h should be #included before system headers"
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* __GLIBC__ && !__FreeBSD_kernel__ && !__CONFIG_H__ */
#ifndef __CONFIG_H__
# define __CONFIG_H__ 1
# ifndef __READLINE_CONFIG_H__
#  define __READLINE_CONFIG_H__ 1
# endif /* __READLINE_CONFIG_H__ */])dnl
AH_BOTTOM([
#endif /* __CONFIG_H__ */
])dnl

# shared library configuration section
#
# Shared object configuration section. These values are generated by
# ${srcdir}/support/shobj-conf
#
if test -f ${srcdir}/support/shobj-conf; then
        AC_MSG_CHECKING([configuration for building shared libraries])
        eval `${CONFIG_SHELL-/bin/sh} ${srcdir}/support/shobj-conf -C "${CC}" -c ${host_cpu} -o ${host_os} -v ${host_vendor}`
        AC_SUBST([SHOBJ_CC])dnl
        AC_SUBST([SHOBJ_CFLAGS])dnl
        AC_SUBST([SHOBJ_LD])dnl
        AC_SUBST([SHOBJ_LDFLAGS])dnl
	AC_SUBST([SHOBJ_XLDFLAGS])dnl
        AC_SUBST([SHOBJ_LIBS])dnl
        AC_SUBST([SHOBJ_STATUS])dnl
	AC_SUBST([SHLIB_STATUS])dnl
	AC_SUBST([SHLIB_XLDFLAGS])dnl
	AC_SUBST([SHLIB_LIBSUFF])dnl
	AC_SUBST([SHLIB_LIBVERSION])dnl
	AC_SUBST([SHLIB_LIBS])dnl
        AC_MSG_RESULT([${SHLIB_STATUS}])dnl

	# SHLIB_STATUS is either `supported' or `unsupported'. If it is
	# `unsupported', turn off any default shared library building
	if test "${SHLIB_STATUS}" = 'unsupported'; then
		opt_shared_libs=no
	fi

	# shared library versioning
	# quoted for m4 so I can use character classes
	SHLIB_MAJOR=[`expr "${LIBVERSION}" : '\([0-9]\)\..*'`]
	SHLIB_MINOR=[`expr "${LIBVERSION}" : '[0-9]\.\([0-9]\).*'`]
	AC_SUBST([SHLIB_MAJOR])dnl
	AC_SUBST([SHLIB_MINOR])dnl
	## done with shobj-conf
fi

if test "x${opt_static_libs}" = "xyes"; then
  STATIC_TARGET=static
  STATIC_INSTALL_TARGET=install-static
fi
if test "x${opt_shared_libs}" = "xyes"; then
  SHARED_TARGET=shared
  SHARED_INSTALL_TARGET=install-shared
fi

# Libtool:
AC_MSG_CHECKING([libtool-related things])
AC_MSG_RESULT([ ])
LT_INIT([static disable-shared])dnl
LT_LANG([C])dnl
dnl# temporarily set this while I update everything libtool-related:
LT_PREREQ([2.4.6])dnl

# Some final things:
AC_MSG_NOTICE([finishing up...])
XORG_CHANGELOG

AC_SUBST([STATIC_TARGET])dnl
AC_SUBST([SHARED_TARGET])dnl
AC_SUBST([STATIC_INSTALL_TARGET])dnl
AC_SUBST([SHARED_INSTALL_TARGET])dnl

case "${host_os}" in
msdosdjgpp*)	BUILD_DIR=`pwd.exe` ;;	# to prevent //d/path/file
*)		BUILD_DIR=`pwd` ;;
esac

AC_SUBST([BUILD_DIR])dnl

AC_SUBST([CFLAGS])dnl
AC_SUBST([LOCAL_CFLAGS])dnl
AC_SUBST([LOCAL_LDFLAGS])dnl
AC_SUBST([LOCAL_DEFS])dnl

AC_SUBST([AR])dnl
AC_SUBST([ARFLAGS])dnl

AC_SUBST([host_cpu])dnl
AC_SUBST([host_os])dnl

RL_LIB_READLINE_VERSION
AC_SUBST([LIBVERSION])dnl

AC_SUBST([TERMCAP_LIB])dnl

# Output.
AC_CONFIG_FILES([Makefile \
                 Makefile_orig \
                 doc/Makefile \
                 doc/Makefile_orig \
                 examples/Makefile \
                 examples/Makefile_orig \
                 shlib/Makefile \
                 shlib/Makefile_orig])dnl
AC_CONFIG_SUBDIRS([examples/rlfe.dir])dnl
AC_CONFIG_COMMANDS([default],[
# Makefile uses this timestamp file to record whether config.h is up to
# date.
echo "timestamp" > stamp-h
],[])dnl

AC_CONFIG_COMMANDS([config_status_vpath_build_fix],[
if test "${srcdir}" != "."; then
  if test -e ${srcdir}/config.status -a ! -e `pwd`/config.status; then
    cp -v ${srcdir}/config.status `pwd`/config.status || echo ""
  elif test -e `pwd`/config.status -a ! -e ${srcdir}/config.status; then
    cp -v `pwd`/config.status ${srcdir}/config.status || echo ""
  fi
fi
],[])dnl

AC_OUTPUT
