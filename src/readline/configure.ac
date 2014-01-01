dnl#                                               -*- Autoconf -*-
dnl# Configure script for readline library
dnl#
dnl# report bugs to chet@po.cwru.edu
dnl#
dnl# Process this file with autoconf to produce a configure script.
AC_REVISION([for Readline 4.3, version 2.45, from autoconf version] AC_ACVERSION)

AC_INIT([readline],[4.3],[bug-readline@gnu.org])

dnl# make sure we are using a "recent" autoconf version
AC_PREREQ([2.61])

AC_CONFIG_SRCDIR([readline.h])
AC_CONFIG_AUX_DIR([support])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])

dnl# update the value of RL_READLINE_VERSION in readline.h when this
dnl# changes
LIBVERSION=4.3

AC_CANONICAL_TARGET

dnl# configure defaults
opt_curses=no

dnl# arguments to configure
AC_ARG_WITH([curses],[AS_HELP_STRING([--with-curses],[use the curses library instead of the termcap library])],[opt_curses=${withval}])

if test "${opt_curses}" = "yes"; then
	prefer_curses=yes
fi

dnl# option parsing for optional features
opt_static_libs=yes
opt_shared_libs=no

AC_ARG_ENABLE([static],[AS_HELP_STRING([--enable-static],[build static libraries [[default=YES]]])],[opt_static_libs=${enableval}])

echo ""
echo "Beginning configuration for readline-$LIBVERSION for ${host_cpu}-${host_vendor}-${host_os}"
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
	AC_SUBST([CC])
fi
XORG_PROG_RAWCPP
XORG_LD_WRAP

# We want these before the (rest of the) checks, so that the (rest of the)
# checks can modify their values.
test -z "$CFLAGS" && CFLAGS="-g"
test -z "$auto_cflags" && auto_cflags="1"

dnl# BEGIN changes for CYGNUS cross-building for Cygwin
 
dnl# load up the cross-building cache file -- add more cases and cache
dnl# files as necessary
if test "x${cross_compiling}" = "xyes"; then
    case "${host}" in
    *-cygwin*)
	cross_cache=${srcdir}/cross-build/cygwin.cache
	LOCAL_CFLAGS="$LOCAL_CFLAGS -I${srcdir}/../libtermcap"
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
  CROSS_COMPILING_FLAG=
fi
AC_SUBST([CROSS_COMPILING_FLAG])
 
if test -z "${CC_FOR_BUILD}"; then
    if test "x${cross_compiling}" = "xno"; then
        CC_FOR_BUILD='$(CC)'
    else
        CC_FOR_BUILD=gcc
    fi
fi
AC_SUBST([CC_FOR_BUILD])
 
dnl# END changes for CYGNUS cross-building for Cygwin

# If we are using gcc and the user has NOT specified CFLAGS, add -O to
# CFLAGS.
test -n "${GCC}" && test -n "${auto_cflags}" && CFLAGS="${CFLAGS} -O"

# Checks for programs.
AC_PROG_AWK
AC_PROG_GCC_TRADITIONAL
AC_PROG_INSTALL
AM_PROG_INSTALL_STRIP
AC_SUBST([INSTALLdir])
AC_SUBST([INSTALLEDdir])
AC_PROG_LN_S
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
AC_PATH_PROG([TEX_BINARY],[tex])

# Checks for libraries.
AC_CHECK_LIB([c],[printf])
# FIXME: Replace `main' with a function in `-lcurses':
AC_CHECK_LIB([curses],[main])
# FIXME: Replace `main' with a function in `-lhistory':
AC_CHECK_LIB([history],[main])
# FIXME: Replace `main' with a function in `-lreadline':
AC_CHECK_LIB([readline],[main])
# FIXME: Replace `main' with a function in `-liberty':
AC_CHECK_LIB([iberty],[main])

MAKE_SHELL=/bin/sh
AC_SUBST([MAKE_SHELL])

# Checks for typedefs, structures, and compiler characteristics.
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
]])],[ac_cv_type_signal=int],[ac_cv_type_signal=void])
])
AC_DEFINE_UNQUOTED([RETSIGTYPE],[$ac_cv_type_signal],[Define as the return type of signal handlers (`int' or `void').])
dnl# End what used to be AC\_TYPE\_SIGNAL
AC_TYPE_UID_T
AC_TYPE_PID_T
AC_CHECK_TYPES([ptrdiff_t])
AC_TYPE_SIZE_T
AC_CHECK_TYPE([ssize_t],[int])

# Checks for library functions.
AM_WITH_DMALLOC
AC_FUNC_ALLOCA
AC_FUNC_CHOWN
AC_FUNC_ERROR_AT_LINE
AC_FUNC_FORK
AC_FUNC_LSTAT dnl# Calls AC_FUNC_LSTAT_FOLLOWS_SLASHED_SYMLINK
AC_FUNC_MALLOC
AC_FUNC_MBRTOWC
AC_FUNC_MMAP
AC_FUNC_REALLOC
AC_FUNC_STAT
AC_FUNC_STRCOLL
AC_FUNC_STRNLEN
AC_FUNC_STRTOD
AC_DEFUN([gl__ALLOC_CHECKS],[
  AC_REQUIRE([gl_EEALLOC])
  AC_REQUIRE([gl_FUNC_MALLOC_GNU])
  AC_REQUIRE([gl_FUNC_MALLOC_POSIX])
  AC_REQUIRE([gl_FUNC_REALLOC_GNU])
  AC_REQUIRE([gl_FUNC_REALLOC_POSIX])
  AC_REQUIRE([gl_MALLOCA])
  AC_REQUIRE([gl_XALLOC])
])dnl
gl__ALLOC_CHECKS
XORG_CHECK_MALLOC_ZERO
XORG_MEMORY_CHECK_FLAGS
AX_CHECK_PAGE_ALIGNED_MALLOC
AC_CHECK_FUNCS([atoi ctime dup2 endpwent exit fcntl fprintf free \
                ftruncate getcwd getopt getpwent getpwnam getpwuid \
                isascii isxdigit kill lstat mbrlen memmove memset \
                munmap putenv select setenv setlocale strcasecmp \
                strchr strcpy strdup strerror strlen strncpy strpbrk \
                strrchr system tcgetattr vsnprintf wcwidth xrealloc])
AC_REPLACE_FUNCS([xmalloc])
AC_SEARCH_LIBS([xmalloc],[iberty c gnu])
AC_SEARCH_LIBS([xrealloc],[iberty c gnu])
AC_SEARCH_LIBS([rpl_realloc],[iberty c gnu])
AC_SEARCH_LIBS([rpl_malloc],[iberty c gnu])
AC_REPLACE_FUNCS([rpl_malloc])
AC_REPLACE_FUNCS([rpl_realloc])
AC_CHECK_DECL([exit])
AC_CHECK_DECL([free])
AC_CHECK_DECL([strcpy])
AC_CHECK_DECL([strlen])
AC_CHECK_DECL([strncpy])

# Checks for header files.
AC_HEADER_DIRENT
AC_HEADER_STDBOOL dnl# calls AC_CHECK_HEADER_STDBOOL
AC_HEADER_STAT
AC_HEADER_TIOCGWINSZ
AC_CHECK_HEADERS([arpa/inet.h errno.h fcntl.h features.h limits.h \
                  locale.h malloc.h malloca.h malloc/malloc.h netdb.h \
                  netinet/in.h pwd.h readline/readline.h \
                  readline/history.h realloc.h select.h sgtty.h \
                  signal.h stdarg.h stddef.h stdio.h sys/file.h \
                  sys/ioctl.h sys/pte.h sys/ptem.h sys/select.h \
                  sys/socket.h sys/stream.h sys/time.h termcap.h \
                  termio.h termios.h varargs.h wchar.h wctype.h])

# Checks using macros from Bash's configure script
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

# Automake
if test -f ${srcdir}/config.status; then
  AC_MSG_NOTICE([config.status is already present, removing it.])
  rm -f ${srcdir}/config.status
fi
AM_INIT_AUTOMAKE([dejagnu gnits -Wall])
AM_MAINTAINER_MODE

# shared library configuration section
#
# Shared object configuration section. These values are generated by
# ${srcdir}/support/shobj-conf
#
if test -f ${srcdir}/support/shobj-conf; then
        AC_MSG_CHECKING([configuration for building shared libraries])
        eval `${CONFIG_SHELL-/bin/sh} ${srcdir}/support/shobj-conf -C "${CC}" -c ${host_cpu} -o ${host_os} -v ${host_vendor}`
        AC_SUBST([SHOBJ_CC])
        AC_SUBST([SHOBJ_CFLAGS])
        AC_SUBST([SHOBJ_LD])
        AC_SUBST([SHOBJ_LDFLAGS])
	AC_SUBST([SHOBJ_XLDFLAGS])
        AC_SUBST([SHOBJ_LIBS])
        AC_SUBST([SHOBJ_STATUS])
	AC_SUBST([SHLIB_STATUS])
	AC_SUBST([SHLIB_XLDFLAGS])
	AC_SUBST([SHLIB_LIBSUFF])
	AC_SUBST([SHLIB_LIBVERSION])
	AC_SUBST([SHLIB_LIBS])
        AC_MSG_RESULT([$SHLIB_STATUS])

	# SHLIB_STATUS is either `supported' or `unsupported'. If it is
	# `unsupported', turn off any default shared library building
	if test "${SHLIB_STATUS}" = 'unsupported'; then
		opt_shared_libs=no
	fi

	# shared library versioning
	# quoted for m4 so I can use character classes
	SHLIB_MAJOR=[`expr "${LIBVERSION}" : '\([0-9]\)\..*'`]
	SHLIB_MINOR=[`expr "${LIBVERSION}" : '[0-9]\.\([0-9]\).*'`]
	AC_SUBST([SHLIB_MAJOR])
	AC_SUBST([SHLIB_MINOR])
fi

if test "${opt_static_libs}" = "yes"; then
	STATIC_TARGET=static
	STATIC_INSTALL_TARGET=install-static
fi
if test "${opt_shared_libs}" = "yes"; then
	SHARED_TARGET=shared
	SHARED_INSTALL_TARGET=install-shared
fi

# Libtool
LT_INIT([static disable-shared])
LT_LANG([C])

# Some final things
XORG_CHANGELOG

AC_SUBST([STATIC_TARGET])
AC_SUBST([SHARED_TARGET])
AC_SUBST([STATIC_INSTALL_TARGET])
AC_SUBST([SHARED_INSTALL_TARGET])

case "${host_os}" in
msdosdjgpp*)	BUILD_DIR=`pwd.exe` ;;	# to prevent //d/path/file
*)		BUILD_DIR=`pwd` ;;
esac

AC_SUBST([BUILD_DIR])

AC_SUBST([CFLAGS])
AC_SUBST([LOCAL_CFLAGS])
AC_SUBST([LOCAL_LDFLAGS])
AC_SUBST([LOCAL_DEFS])

AC_SUBST([AR])
AC_SUBST([ARFLAGS])

AC_SUBST([host_cpu])
AC_SUBST([host_os])

RL_LIB_READLINE_VERSION
AC_SUBST([LIBVERSION])

AC_SUBST([TERMCAP_LIB])

AC_CONFIG_FILES([Makefile \
                 Makefile.orig \
                 doc/Makefile \
                 doc/Makefile.orig \
                 examples/Makefile \
                 examples/Makefile.orig \
                 shlib/Makefile \
                 shlib/Makefile.orig])
AC_CONFIG_SUBDIRS([examples/rlfe.dir])
AC_CONFIG_COMMANDS([default],[
# Makefile uses this timestamp file to record whether config.h is up to
# date.
echo "" > stamp-h
],[])
AC_OUTPUT

if test "${srcdir}" != "."; then
  if test -e ${srcdir}/config.status -a ! -e `pwd`/config.status; then
    cp -v ${srcdir}/config.status `pwd`/config.status || echo ""
  elif test -e `pwd`/config.status -a ! -e ${srcdir}/config.status; then
    cp -v `pwd`/config.status ${srcdir}/config.status || echo ""
  fi
fi
