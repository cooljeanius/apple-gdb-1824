#!/bin/bash -norc
dnl#	This file is an input file used by the GNU "autoconf" program to
dnl#	generate the file "configure", which is run during Tcl installation
dnl#	to configure the system for the local environment.
#
# RCS: @(#) configure.ac,v 1.9 2003/01/21 19:40:19 hunt Exp
# -*- Autoconf -*-

AC_INIT([Tcl-unix],[8.4.1],[tromey@cygnus.com])
AC_CONFIG_SRCDIR([../generic/tcl.h])
AC_PREREQ([2.59])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h:../unix/config.h.in])
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
AM_SANITY_CHECK
AM_MAINTAINER_MODE
m4_ifdef([AM_INIT_AUTOMAKE],[
  if test -f ${srcdir}/config.status; then
    AC_MSG_NOTICE([config.status is already present, removing it.])
    rm -f ${srcdir}/config.status
  fi
  AM_INIT_AUTOMAKE([gnits dejagnu])
  AC_CONFIG_COMMANDS([automake_post_configure],[
  if test "${srcdir}" != "."; then
    if test -e ${srcdir}/config.status -a ! -e `pwd`/config.status; then
      cp -v ${srcdir}/config.status `pwd`/config.status || echo ""
    elif test -e `pwd`/config.status -a ! -e ${srcdir}/config.status; then
      cp -v `pwd`/config.status ${srcdir}/config.status || echo ""
    fi
  fi
  ],[])dnl
],[
  AC_PROG_AWK
  AC_PROG_MAKE_SET
])dnl
m4_ifdef([AC_REQUIRE_AUX_FILE],[
  AC_REQUIRE_AUX_FILE([compile])dnl
  AC_REQUIRE_AUX_FILE([missing])dnl
])dnl

if test -z "${datarootdir}"; then
  export datarootdir=${prefix}/share
  AC_SUBST([datarootdir])
fi

dnl# This is only used when included from macosx/configure.ac
m4_ifdef([SC_USE_CONFIG_HEADERS],[
    AC_CONFIG_HEADERS([tclConfig.h:../unix/tclConfig.h.in])
    AC_CONFIG_COMMANDS_PRE([DEFS="-DHAVE_TCL_CONFIG_H  -imacros tclConfig.h"])
    AH_TOP([
    #ifndef _TCLCONFIG
    #define _TCLCONFIG])
    AH_BOTTOM([
    /* Undef unused package specific autoheader defines so that we can
     * include both tclConfig.h and tkConfig.h at the same time: */
    /* override */ #undef PACKAGE_NAME
    /* override */ #undef PACKAGE_STRING
    /* override */ #undef PACKAGE_TARNAME
    #endif /* _TCLCONFIG */])
])dnl

TCL_VERSION=8.4
TCL_MAJOR_VERSION=8
TCL_MINOR_VERSION=4
TCL_PATCH_LEVEL=".1"
VERSION=${TCL_VERSION}

#------------------------------------------------------------------------
# Handle the --prefix=... option
#------------------------------------------------------------------------

if test "${prefix}" = "NONE"; then
    prefix=/usr/local
fi
if test "${exec_prefix}" = "NONE"; then
    exec_prefix=${prefix}
fi
# libdir must be a fully qualified path and (not ${exec_prefix}/lib)
eval libdir="${libdir}"
TCL_SRC_DIR=`cd ${srcdir}/..; pwd`

#------------------------------------------------------------------------
# Compress and/or soft link the manpages?
#------------------------------------------------------------------------
SC_CONFIG_MANPAGES

#------------------------------------------------------------------------
# Standard compiler checks
#------------------------------------------------------------------------

# If the user did not set CFLAGS, set it now to keep
# the AC_PROG_CC macro from adding "-g -O2".
if test "${CFLAGS+set}" != "set" ; then
    CFLAGS=""
fi

AC_PROG_CC
AC_PROG_CXX
AC_CHECK_HEADERS_ONCE([float.h limits.h unistd.h])dnl

# Some more programs:
AC_PROG_INSTALL
AM_PROG_INSTALL_STRIP
AM_PROG_INSTALL_SH
AC_PROG_LN_S
AC_PROG_SED
AC_PROG_YACC
AC_PATH_PROG([GDB_BIN],[gdb])dnl
AC_PATH_PROG([DDD_BIN],[ddd])dnl

#------------------------------------------------------------------------
# Threads support
#------------------------------------------------------------------------

SC_ENABLE_THREADS

#------------------------------------------------------------------------
# If we use GCC, see if the compiler understands -pipe. If so, use it.
# It makes compiling go faster. (This is only a performance feature.)
#------------------------------------------------------------------------

AC_ARG_VAR([no_pipe],[Set to try to keep the compiler from using -pipe])
if test -z "${no_pipe}"; then
 if test -n "${GCC}"; then
  AC_MSG_CHECKING([if the compiler understands -pipe])
  OLDCC="${CC}"
  CC="${CC} -pipe"
  AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[]],[[]])],[AC_MSG_RESULT([yes])],[CC="${OLDCC}"
    AC_MSG_RESULT([no])])
 fi
fi

dnl#
#--------------------------------------------------------------------
# The statements below define some defines.
#--------------------------------------------------------------------
AC_DEFINE([TCL_TOMMATH],[1],[Build libtommath?])dnl
AC_DEFINE([MP_PREC],[4],[Default libtommath precision.])dnl

dnl#
#--------------------------------------------------------------------
#	Detect what compiler flags to set for 64-bit support.
#--------------------------------------------------------------------

SC_TCL_EARLY_FLAGS

SC_TCL_64BIT_FLAGS

#--------------------------------------------------------------------
#	Check endianness because we can optimize comparisons of
#	Tcl_UniChar strings to memcmp on big-endian systems.
#--------------------------------------------------------------------

AC_C_BIGENDIAN

#--------------------------------------------------------------------
#	Supply substitutes for missing POSIX library procedures, or
#	set flags so Tcl uses alternate procedures.
#--------------------------------------------------------------------

# Check if Posix compliant getcwd exists, if not we will use getwd.
AC_CHECK_FUNCS([getcwd],[],
               [AC_DEFINE([USEGETWD],[1],
                          [Define to 1 if getcwd is missing and we have to use getwd as a fallback])])dnl
# Nb: if getcwd uses popen and pwd(1) (like SunOS 4) we should really
# define USEGETWD even if the posix getcwd exists. Add a test?

AC_REPLACE_FUNCS([opendir strstr])dnl

AC_REPLACE_FUNCS([strtol strtoll strtoull tmpnam waitpid])
AC_CHECK_FUNCS([strerror],[],
               [AC_DEFINE([NO_STRERROR],[1],
                          [Define to 1 if we are missing strerror])])dnl
AC_CHECK_FUNCS([getwd],[],
               [AC_DEFINE([NO_GETWD],[1],
                          [Define to 1 if we are missing getwd])])dnl
AC_CHECK_FUNCS([wait3],[],
               [AC_DEFINE([NO_WAIT3],[1],
                          [Define to 1 if we are missing wait3])])dnl
AC_CHECK_FUNCS([uname],[],
               [AC_DEFINE([NO_UNAME],[1],
                          [Define to 1 if we are missing uname])])dnl

if test "`uname -s`" = "Darwin" && test "${TCL_THREADS}" = 1 && \
	test "`uname -r | awk -F. '{print [$]1}'`" -lt 7; then
    # prior to Darwin 7, realpath is not threadsafe, so do NOT
    # use it when threads are enabled, c.f. bug # 711232
    export ac_cv_func_realpath=no
fi
AC_CHECK_FUNCS([realpath],[],
               [AC_DEFINE([NO_REALPATH],[1],
                          [Define to 1 if we are missing realpath])])dnl

# Some more functions:
AC_FUNC_CHOWN
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_CHECK_FUNCS([alarm atexit bzero dup2 endgrent endpwent gethostbyaddr \
                gethostname inet_ntoa isxdigit memset mkdir mkfifo \
                nl_langinfo rmdir select setlocale socket strcasecmp \
                strchr strrchr strstr strtol strtoull tzset utime])dnl

AC_CHECK_FUNCS([execl execle execlp])dnl

#--------------------------------------------------------------------
#	Supply substitutes for missing POSIX header files. Special
#	notes:
#	    - stdlib.h does NOT define strtol, strtoul, or
#	      strtod insome versions of SunOS
#	    - some versions of string.h do NOT declare procedures such
#	      as strstr
#--------------------------------------------------------------------

SC_MISSING_POSIX_HEADERS

# More headers:
AC_HEADER_STDBOOL dnl# also "_CHECK"s it
AC_CHECK_HEADERS([arpa/inet.h fcntl.h floatingpoint.h locale.h math.h \
                  netdb.h netinet/in.h sgtty.h stdarg.h sys/file.h \
                  sys/socket.h sys/statfs.h termio.h termios.h utime.h])dnl

#--------------------------------------------------------------------------
#	Determine which interface to use to talk to the serial port.
#	Note that #include lines must begin in leftmost column for
#	some compilers to recognize them as preprocessor directives.
#--------------------------------------------------------------------------

SC_SERIAL_PORT

#--------------------------------------------------------------------
#	Include sys/select.h if it exists and if it supplies things
#	that appear to be useful and are NOT already in sys/types.h.
#	This appears to be true only on the RS/6000 under AIX. Some
#	systems like OSF/1 have a sys/select.h that is of no use, and
#	other systems like SCO UNIX have a sys/select.h that is
#	pernicious. If "fd_set" is NOT defined anywhere, then set a
#	special flag.
#--------------------------------------------------------------------

AC_MSG_CHECKING([for fd_set in sys/types])
AC_CACHE_VAL([tcl_cv_type_fd_set],
    [AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[#include <sys/types.h>]], [[fd_set readMask, writeMask;]])],[tcl_cv_type_fd_set=yes],[tcl_cv_type_fd_set=no])])
AC_MSG_RESULT([${tcl_cv_type_fd_set}])
tk_ok=${tcl_cv_type_fd_set}
if test ${tcl_cv_type_fd_set} = no; then
    AC_MSG_CHECKING([for fd_mask in sys/select])
    AC_CACHE_VAL([tcl_cv_grep_fd_mask],
	[AC_EGREP_HEADER([fd_mask],[sys/select.h],[tcl_cv_grep_fd_mask=present],[tcl_cv_grep_fd_mask=missing])])
    AC_MSG_RESULT([${tcl_cv_grep_fd_mask}])
    if test ${tcl_cv_grep_fd_mask} = present; then
	AC_DEFINE([HAVE_SYS_SELECT_H],[1],
                  [Define to 1 if we have a working sys/select.h])
	tk_ok=yes
    fi
fi
if test ${tk_ok} = no; then
    AC_DEFINE([NO_FD_SET],[1],[Define to 1 if there is no fd_set])
fi

#--------------------------------------------------------------------------
#       Find out all about time handling differences.
#--------------------------------------------------------------------------

SC_TIME_HANDLER
AC_DEFUN([AC_REQUIRE_TIME_CHECKS],[
  AC_REQUIRE([AC_HEADER_TIME])dnl
  AC_REQUIRE([AC_STRUCT_TM])dnl
  AC_REQUIRE([AC_STRUCT_TIMEZONE])dnl
])dnl
AC_REQUIRE_TIME_CHECKS

#--------------------------------------------------------------------
#	Some systems (e.g., IRIX 4.0.5) lack the st_blksize field
#	in struct stat.  But we might be able to use fstatfs instead.
#--------------------------------------------------------------------
AC_CHECK_MEMBERS([struct stat.st_blksize],[AC_DEFINE([HAVE_ST_BLKSIZE],[1],
			    [Define to 1 if your `struct stat' has
			     `st_blksize'. Deprecated, use
			     `HAVE_STRUCT_STAT_ST_BLKSIZE' instead.])])dnl

AC_CHECK_FUNCS([fstatfs],[],
               [AC_DEFINE([NO_FSTATFS],[1],
                          [Define to 1 if there is no fstatfs])])dnl

# Another member check:
AC_CHECK_MEMBERS([struct stat.st_rdev])dnl

#--------------------------------------------------------------------
#       Some system have no memcmp or it does not work with 8 bit
#       data, this checks it and add memcmp.o to LIBOBJS if needed
#--------------------------------------------------------------------
AC_FUNC_MEMCMP

#--------------------------------------------------------------------
#       Some system like SunOS 4 and other BSD like systems
#       have no memmove (we assume they have bcopy instead).
#       {The replacement define is in compat/string.h}
#--------------------------------------------------------------------
AC_CHECK_FUNCS([memmove],[],[
  AC_DEFINE([NO_MEMMOVE],[1],[Define to 1 if there is no memmove])dnl
  AC_DEFINE([NO_STRING_H],[1],
            [Define to 1 if there is no working string.h])dnl
])dnl

#--------------------------------------------------------------------
#	On some systems strstr is broken: it returns a pointer even
#	even if the original string is empty.
#--------------------------------------------------------------------

AC_MSG_CHECKING([proper strstr implementation])
AC_RUN_IFELSE([AC_LANG_SOURCE([[
extern int strstr();
int main()
{
    exit(strstr("\0test", "test") ? 1 : 0);
}
]])],[tcl_ok=yes],[tcl_ok=no],[tcl_ok=no])
if test ${tcl_ok} = yes; then
    AC_MSG_RESULT([yes])
else
    AC_MSG_RESULT([broken, using substitute])
    LIBOBJS="${LIBOBJS} strstr.o"
fi

#--------------------------------------------------------------------
#	Check for strtoul function. This is tricky because under some
#	versions of AIX strtoul returns an incorrect terminator
#	pointer for the string "0".
#--------------------------------------------------------------------

AC_CHECK_FUNCS([strtoul],[tcl_ok=1],[tcl_ok=0])
AC_RUN_IFELSE([AC_LANG_SOURCE([[
extern int strtoul();
int main()
{
    char *string = "0";
    char *term;
    int value;
    value = strtoul(string, &term, 0);
    if ((value != 0) || (term != (string+1))) {
        exit(1);
    }
    exit(0);
}]])],[],[tcl_ok=0],[tcl_ok=0])
if test "${tcl_ok}" = 0; then
    test -n "${verbose}" && echo "	Adding strtoul.o."
    LIBOBJS="${LIBOBJS} strtoul.o"
fi

#---------------------------------------------------------------------
#	Check for the strtod function. This is tricky because in some
#	versions of Linux strtod mis-parses strings starting with "+".
#---------------------------------------------------------------------

AC_CHECK_FUNCS([strtod],[tcl_ok=1],[tcl_ok=0])
AC_RUN_IFELSE([AC_LANG_SOURCE([[
extern double strtod();
int main()
{
    char *string = " +69";
    char *term;
    double value;
    value = strtod(string, &term);
    if ((value != 69) || (term != (string+4))) {
	exit(1);
    }
    exit(0);
}]])],[],[tcl_ok=0],[tcl_ok=0])
if test "${tcl_ok}" = 0; then
    test -n "${verbose}" && echo "	Adding strtod.o."
    LIBOBJS="${LIBOBJS} strtod.o"
fi

#--------------------------------------------------------------------
#	Under Solaris 2.4, strtod returns the wrong value for the
#	terminating character under some conditions.  Check for this
#	and if the problem exists use a substitute procedure
#	"fixstrtod" that corrects the error.
#--------------------------------------------------------------------

SC_BUGGY_STRTOD
AC_FUNC_STRTOD

#--------------------------------------------------------------------
#	Check for various typedefs and provide substitutes if
#	they do not exist.
#--------------------------------------------------------------------

AC_TYPE_MODE_T
AC_TYPE_OFF_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UID_T
AC_TYPE_UINT32_T

AC_MSG_CHECKING([for socklen_t])
AC_CACHE_VAL([ac_cv_type_socklen_t],[AC_EGREP_CPP(changequote(<<,>>)dnl
<<(^|[^a-zA-Z_0-9])socklen_t[^a-zA-Z_0-9]>>dnl
changequote([,]),[
    #include <sys/types.h>
    #include <sys/socket.h>
    #if STDC_HEADERS
    # include <stdlib.h>
    # include <stddef.h>
    #else
    # warning this conftest expects you to have STDC_HEADERS
    #endif /* STDC_HEADERS */
    ],[ac_cv_type_socklen_t=yes],[ac_cv_type_socklen_t=no])])
AC_MSG_RESULT([${ac_cv_type_socklen_t}])
if test ${ac_cv_type_socklen_t} = no; then
    AC_DEFINE([socklen_t],[unsigned],[Define to unsigned if it needs to be])
fi

AC_TYPE_INTMAX_T
AC_TYPE_UINTMAX_T
AC_CHECK_SIZEOF([int])dnl
AC_CHECK_ALIGNOF([int])dnl
AC_CHECK_SIZEOF([double])dnl
AC_CHECK_ALIGNOF([double])dnl
AC_CHECK_SIZEOF([char])dnl
AC_CHECK_ALIGNOF([char])dnl
AC_CHECK_TYPES([regoff_t])dnl

#--------------------------------------------------------------------
#	If a system does NOT have an opendir function (man, is that old!)
#	then we have to supply a different version of dirent.h which
#	is compatible with the substitute version of opendir that is
#	provided. This version only works with V7-style directories.
#--------------------------------------------------------------------

AC_CHECK_FUNCS([opendir],[],[
  AC_DEFINE([USE_DIRENT2_H],[2],
            [Define to 2 to use a different version of dirent.h])dnl
])dnl

#--------------------------------------------------------------------
#	The check below checks whether <sys/wait.h> defines the type
#	"union wait" correctly. It is needed because of weirdness in
#	HP-UX where "union wait" is defined in both the BSD and SYS-V
#	environments.  Checking the usability of WIFEXITED seems to do
#	the trick.
#--------------------------------------------------------------------

AC_HEADER_SYS_WAIT
AC_MSG_CHECKING([union wait])
AC_CACHE_VAL([tcl_cv_union_wait],
    AC_LINK_IFELSE([AC_LANG_PROGRAM([[#include <sys/types.h>
#include <sys/wait.h>]],[[
union wait x;
WIFEXITED(x);		/* Generates compiler error if WIFEXITED
			 * uses an int. */
    ]])],[tcl_cv_union_wait=yes],[tcl_cv_union_wait=no]))
AC_MSG_RESULT([${tcl_cv_union_wait}])
if test ${tcl_cv_union_wait} = no; then
    AC_DEFINE([NO_UNION_WAIT],[1],[Define to 1 if there is no union wait])
fi

#---------------------------------------------------------------------
#	Check whether there is an strncasecmp function on this system.
#	This is a bit tricky because under SCO it is in -lsocket and
#	under Sequent Dynix it is in -linet.
#---------------------------------------------------------------------

AC_CHECK_FUNCS([strncasecmp],[tcl_ok=1],[tcl_ok=0])
if test "${tcl_ok}" = 0; then
    AC_CHECK_LIB([socket],[strncasecmp],[tcl_ok=1],[tcl_ok=0])
fi
if test "${tcl_ok}" = 0; then
    AC_CHECK_LIB([inet],[strncasecmp],[tcl_ok=1],[tcl_ok=0])
fi
if test "${tcl_ok}" = 0; then
    LIBOBJS="${LIBOBJS} strncasecmp.o"
fi

#--------------------------------------------------------------------
#	The code below deals with several issues related to gettimeofday:
#	1. Some systems do NOT provide a gettimeofday function at all
#	   (set NO_GETTOD if this is the case).
#	2. SGI systems do NOT use the BSD form of the gettimeofday
#	   function, but they have a BSDgettimeofday function that can be
#	   used instead.
#	3. See if gettimeofday is declared in the <sys/time.h> header file.
#	   if not, set the GETTOD_NOT_DECLARED flag so that tclPort.h can
#	   declare it.
#--------------------------------------------------------------------

AC_CHECK_FUNCS([BSDgettimeofday],
    [AC_DEFINE([HAVE_BSDGETTIMEOFDAY],[1],
               [Define to 1 if we have the BSDgettimeofday function])],
    [AC_CHECK_FUNCS([gettimeofday],[],
                [AC_DEFINE([NO_GETTOD],[1],
                    [Define to 1 if there is no gettimeofday function])dnl
    ])dnl
])dnl
AC_MSG_CHECKING([for gettimeofday declaration])
AC_CACHE_VAL([tcl_cv_grep_gettimeofday],
    [AC_EGREP_HEADER([gettimeofday],[sys/time.h],
	[tcl_cv_grep_gettimeofday=present], [tcl_cv_grep_gettimeofday=missing])])
AC_MSG_RESULT([${tcl_cv_grep_gettimeofday}])
if test ${tcl_cv_grep_gettimeofday} = missing ; then
    AC_DEFINE([GETTOD_NOT_DECLARED],[1],
              [Define to 1 if gettimeofday is not declared])
fi

#---------------------------------------------------------------------
#	The following code checks to see whether it is possible to get
#	signed chars on this platform.  This is needed in order to
#	properly generate sign-extended ints from character values.
#---------------------------------------------------------------------

AC_C_CHAR_UNSIGNED
AC_MSG_CHECKING([signed char declarations])
AC_CACHE_VAL([tcl_cv_char_signed],
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[]],[[
	signed char *p;
	p = 0;
	]])],[tcl_cv_char_signed=yes],[tcl_cv_char_signed=no]))
AC_MSG_RESULT([${tcl_cv_char_signed}])
if test ${tcl_cv_char_signed} = yes; then
    AC_DEFINE([HAVE_SIGNED_CHAR],[1],
              [Define to 1 if we have a signed char declaration])
fi

#--------------------------------------------------------------------
#  Does putenv() copy or not? We need to know to avoid memory leaks.
#--------------------------------------------------------------------

AC_MSG_CHECKING([for a putenv() that copies the buffer])
AC_CACHE_VAL([tcl_cv_putenv_copy],
    [AC_RUN_IFELSE([AC_LANG_SOURCE([[
	#include <stdlib.h>
	#define OURVAR "havecopy=yes"
	int main (int argc, char *argv[])
	{
	    char *foo, *bar;
	    foo = (char *)strdup(OURVAR);
	    putenv(foo);
	    strcpy((char *)(strchr(foo, '=') + 1), "no");
	    bar = getenv("havecopy");
	    if (!strcmp(bar, "no")) {
		/* doesnt copy */
		return 0;
	    } else {
		/* does copy */
		return 1;
	    }
	}
    ]])],[tcl_cv_putenv_copy=no],[tcl_cv_putenv_copy=yes],[tcl_cv_putenv_copy=no])
])
AC_MSG_RESULT([${tcl_cv_putenv_copy}])
if test ${tcl_cv_putenv_copy} = yes; then
    AC_DEFINE([HAVE_PUTENV_THAT_COPIES],[1],
              [Define to 1 if we have a putenv that copies])
fi

dnl#
#--------------------------------------------------------------------
# Check for support of nl_langinfo function
#--------------------------------------------------------------------

SC_ENABLE_LANGINFO

dnl#
#--------------------------------------------------------------------
# Check for support of chflags function
#--------------------------------------------------------------------

AC_CHECK_FUNCS([chflags])

#--------------------------------------------------------------------
# Check for support of isnan() function or macro
#--------------------------------------------------------------------

AC_CACHE_CHECK([isnan],[tcl_cv_isnan],[
    AC_LINK_IFELSE([AC_LANG_SOURCE([[#include <math.h>]],[[
isnan(0.0);		/* Generates an error if isnan is missing */
]])],[tcl_cv_isnan=yes],[tcl_cv_isnan=no])])
if test ${tcl_cv_isnan} = no; then
    AC_DEFINE([NO_ISNAN],[1],[Do we have a usable 'isnan'?])
fi

#--------------------------------------------------------------------
# Darwin specific API checks and defines
#--------------------------------------------------------------------

if test "`uname -s`" = "Darwin" ; then
    AC_CHECK_FUNCS([getattrlist])
    AC_CHECK_HEADERS([copyfile.h])
    AC_CHECK_FUNCS([copyfile])
    if test "x${tcl_corefoundation}" = "xyes"; then
        AC_ARG_VAR([tcl_corefoundation],
                   [Set to try to build CoreFoundation support in Tcl
                    (might not work)])
	AC_CHECK_HEADERS([CoreFoundation/CoreFoundation.h \
                          libkern/OSAtomic.h])
	AC_CHECK_FUNCS([OSSpinLockLock])
	AC_CHECK_FUNCS([pthread_atfork])
    else
        test -z "${tcl_corefoundation}"
        AC_MSG_NOTICE([CoreFoundation support is disabled])
    fi
    AC_DEFINE([USE_VFORK],[1],[Should we use vfork() instead of fork()?])
    AC_DEFINE([TCL_DEFAULT_ENCODING],["utf-8"],
	[Are we to override what our default encoding is?])
    AC_DEFINE([TCL_LOAD_FROM_MEMORY],[1],
	[Can this platform load code from memory?])
    AC_DEFINE([TCL_WIDE_CLICKS],[1],
	[Does this platform have wide high-resolution clicks?])
    AC_CHECK_HEADERS([AvailabilityMacros.h])
    if test "x${ac_cv_header_AvailabilityMacros_h}" = "xyes"; then
        AC_CHECK_FUNCS_ONCE([rand])
	AC_CACHE_CHECK([if weak import is available],[tcl_cv_cc_weak_import],[
	    hold_cflags=${CFLAGS}; CFLAGS="${CFLAGS} -Werror"
	    AC_LINK_IFELSE([AC_LANG_SOURCE([[
	#ifdef __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__
	# if __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ < 1020
	#  error __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ < 1020
	# endif
	#elif MAC_OS_X_VERSION_MIN_REQUIRED < 1020
	# error MAC_OS_X_VERSION_MIN_REQUIRED < 1020
	#endif
	int rand(void) __attribute__((weak_import));
		]],[[rand();]])],
		[tcl_cv_cc_weak_import=yes],[tcl_cv_cc_weak_import=no])
	    CFLAGS=${hold_cflags}])
	if test ${tcl_cv_cc_weak_import} = yes; then
	    AC_DEFINE([HAVE_WEAK_IMPORT],[1],[Is weak import available?])
	fi
        AC_CHECK_HEADERS_ONCE([sys/cdefs.h])
	AC_CACHE_CHECK([if Darwin SUSv3 extensions are available],
	    [tcl_cv_cc_darwin_c_source],[
	    hold_cflags=${CFLAGS}; CFLAGS="${CFLAGS} -Werror"
	    AC_COMPILE_IFELSE([AC_LANG_SOURCE([[
	#ifdef __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__
	# if __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ < 1050
	#  error __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ < 1050
	# endif
	#elif MAC_OS_X_VERSION_MIN_REQUIRED < 1050
	# error MAC_OS_X_VERSION_MIN_REQUIRED < 1050
	#endif
	#define _DARWIN_C_SOURCE 1
	#include <sys/cdefs.h>
		]],[[]])],[tcl_cv_cc_darwin_c_source=yes],[tcl_cv_cc_darwin_c_source=no])
	    CFLAGS=${hold_cflags}])
	if test ${tcl_cv_cc_darwin_c_source} = yes; then
	    AC_DEFINE([_DARWIN_C_SOURCE],[1],
		      [Are Darwin SUSv3 extensions available?])
	fi
    fi
    # Build .bundle dltest binaries in addition to .dylib
    DLTEST_LD='${CC} -bundle -Wl,-w ${CFLAGS} ${LDFLAGS}'
    DLTEST_SUFFIX=".bundle"
else
    DLTEST_LD='${SHLIB_LD}'
    DLTEST_SUFFIX=""
fi

#--------------------------------------------------------------------
# Check for support of fts functions (readdir replacement)
#--------------------------------------------------------------------

AC_CHECK_HEADERS_ONCE([fts.h])
AC_CACHE_CHECK([for fts],[tcl_cv_api_fts],[
    AC_LINK_IFELSE([AC_LANG_SOURCE([[
	   #include <sys/param.h>
	   #include <sys/stat.h>
	   #include <fts.h>
	]],[[
	   char*const p[2] = {"/", NULL};
	   FTS *f = fts_open(p, FTS_PHYSICAL|FTS_NOCHDIR|FTS_NOSTAT, NULL);
	   FTSENT *e = fts_read(f); fts_close(f);
	]])],[tcl_cv_api_fts=yes],[tcl_cv_api_fts=no])])
if test ${tcl_cv_api_fts} = yes; then
    AC_DEFINE([HAVE_FTS],[1],[Do we have fts functions?])
fi

dnl#
#--------------------------------------------------------------------
# Look for libraries that we will need when compiling the Tcl shell
#--------------------------------------------------------------------

SC_TCL_LINK_LIBS

# Add the threads support libraries

LIBS="${LIBS}${THREADS_LIBS}"

SC_ENABLE_SHARED

#--------------------------------------------------------------------
# The statements below define a collection of compile flags.  This
# macro depends on the value of SHARED_BUILD, and should be called
# after SC_ENABLE_SHARED checks the configure switches.
#--------------------------------------------------------------------

SC_CONFIG_CFLAGS

SC_ENABLE_SYMBOLS

TCL_DBGX="${DBGX}"

#--------------------------------------------------------------------
#	The statements below check for systems where POSIX-style
#	non-blocking I/O (O_NONBLOCK) does not work or is unimplemented.
#	On these systems (mostly older ones), use the old BSD-style
#	FIONBIO approach instead.
#--------------------------------------------------------------------

SC_BLOCKING_STYLE

#------------------------------------------------------------------------

AC_MSG_CHECKING([whether to use dll unloading])
AC_ARG_ENABLE([dll-unloading],
    [AS_HELP_STRING([--enable-dll-unloading],
	[enable the 'unload' command (default: on)])],
    [tcl_ok=${enableval}],[tcl_ok=yes])
if test ${tcl_ok} = yes; then
    AC_DEFINE([TCL_UNLOAD_DLLS],[1],
              [Do we allow unloading of shared libraries?])
fi
AC_MSG_RESULT([${tcl_ok}])

#------------------------------------------------------------------------
#	Check whether the timezone data is supplied by the OS or has
#	to be installed by Tcl. The default is autodetection, but can
#	be overriden on the configure command line either way.
#------------------------------------------------------------------------

AC_MSG_CHECKING([for timezone data])
AC_ARG_WITH([tzdata],
    [AS_HELP_STRING([--with-tzdata],
	[install timezone data (default: autodetect)])],
    [tcl_ok=${withval}],[tcl_ok=auto])dnl
#
# Any directories that get added here must also be added to the
# search path in ::tcl::clock::Initialize (library/clock.tcl).
#
case ${tcl_ok} in
    no)
	AC_MSG_RESULT([supplied by OS vendor])
    ;;
    yes)
	# nothing to do here
    ;;
    auto*)
	AC_CACHE_VAL([tcl_cv_dir_zoneinfo],[
	for dir in /usr/share/zoneinfo \
		/usr/share/lib/zoneinfo \
		/usr/lib/zoneinfo
	do
		if test -f ${dir}/UTC -o -f ${dir}/GMT
		then
			tcl_cv_dir_zoneinfo="${dir}"
			break
		fi
	done])
	if test -n "${tcl_cv_dir_zoneinfo}"; then
	    tcl_ok=no
	    AC_MSG_RESULT([${dir}])
	else
	    tcl_ok=yes
	fi
    ;;
    *)
	AC_MSG_ERROR([invalid argument: ${tcl_ok}])
    ;;
esac
if test ${tcl_ok} = yes
then
    AC_MSG_RESULT([supplied by Tcl])
    INSTALL_TZDATA=install-tzdata
fi

#--------------------------------------------------------------------
#	DTrace support
#--------------------------------------------------------------------

AC_ARG_ENABLE([dtrace],
    [AS_HELP_STRING([--enable-dtrace],
	[build with DTrace support (default: off)])],
    [tcl_ok=${enableval}],[tcl_ok=no])
if test ${tcl_ok} = yes; then
    AC_CHECK_HEADER([sys/sdt.h],[tcl_ok=yes],[tcl_ok=no])
fi
if test ${tcl_ok} = yes; then
    AC_PATH_PROG([DTRACE],[dtrace],[],[${PATH}:/usr/sbin])
    test -z "${ac_cv_path_DTRACE}" && tcl_ok=no
fi
AC_MSG_CHECKING([whether to enable DTrace support])
MAKEFILE_SHELL='/bin/sh'
if test ${tcl_ok} = yes; then
    AC_DEFINE([USE_DTRACE],[1],[Are we building with DTrace support?])
    DTRACE_SRC="\${DTRACE_SRC}"
    DTRACE_HDR="\${DTRACE_HDR}"
    if test "`uname -s`" != "Darwin" ; then
	DTRACE_OBJ="\${DTRACE_OBJ}"
	if test "`uname -s`" = "SunOS" -a "${SHARED_BUILD}" = "0" ; then
	    # Need to create an intermediate obj file to ensure tclDTrace.o
	    # gets included when linking against the static tcl library.
	    STLIB_LD='stlib_ld () { /usr/ccs/bin/ld -r -o $${1%.a}.o "$${@:2}" && '"${STLIB_LD}"' $${1} $${1%.a}.o ; } && stlib_ld'
	    MAKEFILE_SHELL='/bin/bash'
	    # Force use of Sun ar and ranlib, the GNU versions choke on
	    # tclDTrace.o and the combined object file above.
	    AR='/usr/ccs/bin/ar'
	    RANLIB='/usr/ccs/bin/ranlib'
	fi
    fi
fi
AC_MSG_RESULT([${tcl_ok}])dnl

#--------------------------------------------------------------------
#  Does the C stack grow upwards or downwards? Or cross-compiling?
#--------------------------------------------------------------------

AC_CACHE_CHECK([if the C stack grows upwards in memory],[tcl_cv_stack_grows_up],[
    AC_RUN_IFELSE([AC_LANG_SOURCE([[
	int StackGrowsUp(int *parent) {
	    int here;
	    return (&here < parent);
	}
	int main (int argc, char *argv[]) {
	    int foo;
	    return StackGrowsUp(&foo);
	}
    ]])],[tcl_cv_stack_grows_up=yes],[tcl_cv_stack_grows_up=no],
    [tcl_cv_stack_grows_up=unknown])])
if test ${tcl_cv_stack_grows_up} = unknown; then
    AC_DEFINE([TCL_CROSS_COMPILE],[1],[Are we cross-compiling?])
elif test ${tcl_cv_stack_grows_up} = yes; then
    AC_DEFINE([TCL_STACK_GROWS_UP],[1],
              [The C stack grows upwards in memory.])
fi

#--------------------------------------------------------------------
#	The statements below define a collection of symbols related to
#	building libtcl as a shared library instead of a static library.
#--------------------------------------------------------------------

TCL_UNSHARED_LIB_SUFFIX=${UNSHARED_LIB_SUFFIX}
TCL_SHARED_LIB_SUFFIX=${SHARED_LIB_SUFFIX}
eval "TCL_LIB_FILE=libtcl${LIB_SUFFIX}"

# tclConfig.sh needs a version of the _LIB_SUFFIX that has been eval'ed
# so that the backslashes quoting the DBX braces are dropped.

# Trick to replace DBGX with TCL_DBGX
DBGX='${TCL_DBGX}'
eval "TCL_LIB_FILE=${TCL_LIB_FILE}"

# Note: in the following variable, it is important to use the absolute
# path name of the Tcl directory rather than "..":  this is because
# AIX remembers this path and will attempt to use it at run-time to look
# up the Tcl library.

if test "`uname -s`" = "Darwin"; then
    SC_ENABLE_FRAMEWORK
    TCL_SHLIB_LD_EXTRAS="-compatibility_version ${TCL_VERSION} -current_version ${TCL_VERSION}`echo ${TCL_PATCH_LEVEL} | awk ['{match($0, "\\\.[0-9]+"); print substr($0,RSTART,RLENGTH)}']`"
    TCL_SHLIB_LD_EXTRAS="${TCL_SHLIB_LD_EXTRAS}"' -install_name "${DYLIB_INSTALL_DIR}"/${TCL_LIB_FILE}'
    echo "$LDFLAGS " | grep -q -- '-prebind ' && TCL_SHLIB_LD_EXTRAS="${TCL_SHLIB_LD_EXTRAS}"' -seg1addr 0xa000000'
    TCL_SHLIB_LD_EXTRAS="${TCL_SHLIB_LD_EXTRAS}"' -sectcreate __TEXT __info_plist Tcl-Info.plist'
    EXTRA_TCLSH_LIBS='-sectcreate __TEXT __info_plist Tclsh-Info.plist'
    EXTRA_APP_CC_SWITCHES='-mdynamic-no-pic'
    AC_CONFIG_FILES([Tcl-Info.plist:../macosx/Tcl-Info.plist.in Tclsh-Info.plist:../macosx/Tclsh-Info.plist.in])
    TCL_YEAR="`date +%Y`"
fi

if test "${FRAMEWORK_BUILD}" = "1"; then
    AC_DEFINE([TCL_FRAMEWORK],[1],[Is Tcl built as a framework?])
    # Construct a fake local framework structure to make linking with
    # '-framework Tcl' and running of tcltest work
    AC_CONFIG_COMMANDS([Tcl.framework], [n=Tcl &&
        f=${n}.framework && v="Versions/${VERSION}" &&
        rm -rf ${f} && mkdir -p ${f}/${v}/Resources &&
        ln -s ${v}/${n} ${v}/Resources ${f} && ln -s ../../../${n} ${f}/${v} &&
        ln -s ../../../../${n}-Info.plist ${f}/${v}/Resources/Info.plist &&
        unset n f v
    ],[VERSION=${TCL_VERSION}])
    LD_LIBRARY_PATH_VAR="DYLD_FRAMEWORK_PATH"
    if test "${libdir}" = '${exec_prefix}/lib'; then
        # override libdir default
        libdir="/Library/Frameworks"
    fi
    TCL_LIB_FILE="Tcl"
    TCL_LIB_FLAG="-framework Tcl"
    TCL_BUILD_LIB_SPEC="-F`pwd` -framework Tcl"
    TCL_LIB_SPEC="-framework Tcl"
    libdir="${libdir}/Tcl.framework/Versions/\${VERSION}"
    TCL_LIBRARY="${libdir}/Resources/Scripts"
    includedir="${libdir}/Headers"
    PRIVATE_INCLUDE_DIR="${libdir}/PrivateHeaders"
    HTML_DIR="${libdir}/Resources/Documentation/Reference/Tcl"
    EXTRA_INSTALL="install-private-headers html-tcl"
    EXTRA_BUILD_HTML='@ln -fs contents.htm "$(HTML_INSTALL_DIR)"/TclTOC.html'
    EXTRA_INSTALL_BINARIES='@echo "Installing Info.plist to $(LIB_INSTALL_DIR)/Resources" && mkdir -p "$(LIB_INSTALL_DIR)/Resources" && $(INSTALL_DATA) Tcl-Info.plist "$(LIB_INSTALL_DIR)/Resources/Info.plist"'
    EXTRA_INSTALL_BINARIES="${EXTRA_INSTALL_BINARIES}"' && echo "Installing license.terms to $(LIB_INSTALL_DIR)/Resources" && $(INSTALL_DATA) "$(TOP_DIR)/license.terms" "$(LIB_INSTALL_DIR)/Resources"'
    EXTRA_INSTALL_BINARIES="$EXTRA_INSTALL_BINARIES"' && echo "Finalizing Tcl.framework" && rm -f "$(LIB_INSTALL_DIR)/../Current" && ln -s "$(VERSION)" "$(LIB_INSTALL_DIR)/../Current" && for f in "$(LIB_FILE)" tclConfig.sh Resources Headers PrivateHeaders; do rm -f "$(LIB_INSTALL_DIR)/../../$$f" && ln -s "Versions/Current/$$f" "$(LIB_INSTALL_DIR)/../.."; done && f="$(STUB_LIB_FILE)" && rm -f "$(LIB_INSTALL_DIR)/../../$$f" && ln -s "Versions/$(VERSION)/$$f" "$(LIB_INSTALL_DIR)/../.."'
    # Do NOT use AC_DEFINE for the following, as the framework version
    # define needs to go into the Makefile even when using autoheader,
    # so that we can pick up a potential make override of VERSION. Also,
    # do NOT put this into CFLAGS as it should not go into tclConfig.sh
    EXTRA_CC_SWITCHES='-DTCL_FRAMEWORK_VERSION=\"$(VERSION)\"'
elif test "${SHARED_BUILD}" = "0" || test "${TCL_NEEDS_EXP_FILE}" = "0"; then
    # libdir must be a fully qualified path and not ${exec_prefix}/lib
    eval libdir="${libdir}"
    if test "${TCL_LIB_VERSIONS_OK}" = "ok"; then
        TCL_LIB_FLAG="-ltcl${TCL_VERSION}\${TCL_DBGX}"
    else
        TCL_LIB_FLAG="-ltcl`echo ${TCL_VERSION} | tr -d .`\${TCL_DBGX}"
    fi
    TCL_BUILD_LIB_SPEC="-L`pwd` ${TCL_LIB_FLAG}"
    TCL_LIB_SPEC="-L${libdir} ${TCL_LIB_FLAG}"
else
    TCL_BUILD_EXP_FILE="lib.exp"
    eval "TCL_EXP_FILE=libtcl${TCL_EXPORT_FILE_SUFFIX}"

    # Replace DBGX with TCL_DBGX
    eval "TCL_EXP_FILE=\"${TCL_EXP_FILE}\""

    if test "${GCC}" = "yes" ; then
	TCL_BUILD_LIB_SPEC="-Wl,-bI:`pwd`/${TCL_BUILD_EXP_FILE} -L`pwd`"
	TCL_LIB_SPEC="-Wl,-bI:${libdir}/${TCL_EXP_FILE} -L`pwd`"
    else
	TCL_BUILD_LIB_SPEC="-bI:`pwd`/${TCL_BUILD_EXP_FILE}"
	TCL_LIB_SPEC="-bI:${libdir}/${TCL_EXP_FILE}"
    fi
fi
VERSION='${VERSION}'
eval "CFG_TCL_SHARED_LIB_SUFFIX=${TCL_SHARED_LIB_SUFFIX}"
eval "CFG_TCL_UNSHARED_LIB_SUFFIX=${TCL_UNSHARED_LIB_SUFFIX}"
eval "CFG_TCL_EXPORT_FILE_SUFFIX=${TCL_EXPORT_FILE_SUFFIX}"
VERSION=${TCL_VERSION}

#--------------------------------------------------------------------
#	The statements below define the symbol TCL_PACKAGE_PATH, which
#	gives a list of directories that may contain packages.  The list
#	consists of one directory for machine-dependent binaries and
#	another for platform-independent scripts.
#--------------------------------------------------------------------

if test "${FRAMEWORK_BUILD}" = "1"; then
    TCL_PACKAGE_PATH="${libdir}/Resources/Scripts"
elif test "${prefix}" != "${exec_prefix}"; then
    TCL_PACKAGE_PATH="${libdir} ${prefix}/lib"
else
    TCL_PACKAGE_PATH="${prefix}/lib"
fi

#--------------------------------------------------------------------
#       The statements below define various symbols relating to Tcl
#       stub support.
#--------------------------------------------------------------------

# Replace ${VERSION} with contents of ${TCL_VERSION}
eval "TCL_STUB_LIB_FILE=libtclstub${TCL_UNSHARED_LIB_SUFFIX}"
# Replace DBGX with TCL_DBGX
eval "TCL_STUB_LIB_FILE=\"${TCL_STUB_LIB_FILE}\""

if test "${TCL_LIB_VERSIONS_OK}" = "ok"; then
    TCL_STUB_LIB_FLAG="-ltclstub${TCL_VERSION}\${TCL_DBGX}"
else
    TCL_STUB_LIB_FLAG="-ltclstub`echo ${TCL_VERSION} | tr -d .`\${TCL_DBGX}"
fi

TCL_BUILD_STUB_LIB_SPEC="-L`pwd` ${TCL_STUB_LIB_FLAG}"
TCL_STUB_LIB_SPEC="-L${libdir} ${TCL_STUB_LIB_FLAG}"
TCL_BUILD_STUB_LIB_PATH="`pwd`/${TCL_STUB_LIB_FILE}"
TCL_STUB_LIB_PATH="${libdir}/${TCL_STUB_LIB_FILE}"

# Install time header dir can be set via --includedir
eval "TCL_INCLUDE_SPEC=\"-I${includedir}\""

#------------------------------------------------------------------------
# tclConfig.sh refers to this by a different name
#------------------------------------------------------------------------

TCL_SHARED_BUILD="${SHARED_BUILD}"
if test "${TCL_SHARED_BUILD}" = "1"; then
  test ! -z "${TCL_SHARED_BUILD}"
  AC_CONFIG_SUBDIRS([dltest])
fi

AC_SUBST([ACLOCAL])dnl
AC_SUBST([TCL_VERSION])dnl
AC_SUBST([TCL_MAJOR_VERSION])dnl
AC_SUBST([TCL_MINOR_VERSION])dnl
AC_SUBST([TCL_PATCH_LEVEL])dnl
AC_SUBST([TCL_YEAR])dnl

AC_SUBST([TCL_LIB_FILE])dnl
AC_SUBST([TCL_LIB_FLAG])dnl
AC_SUBST([TCL_LIB_SPEC])dnl
AC_SUBST([TCL_STUB_LIB_FILE])dnl
AC_SUBST([TCL_STUB_LIB_FLAG])dnl
AC_SUBST([TCL_STUB_LIB_SPEC])dnl
AC_SUBST([TCL_STUB_LIB_PATH])dnl
AC_SUBST([TCL_INCLUDE_SPEC])dnl
AC_SUBST([TCL_BUILD_STUB_LIB_SPEC])dnl
AC_SUBST([TCL_BUILD_STUB_LIB_PATH])dnl

AC_SUBST([TCL_SRC_DIR])dnl
AC_SUBST([TCL_DBGX])dnl
AC_SUBST([CFG_TCL_SHARED_LIB_SUFFIX])dnl
AC_SUBST([CFG_TCL_UNSHARED_LIB_SUFFIX])dnl
AC_SUBST([CFG_TCL_EXPORT_FILE_SUFFIX])dnl

AC_SUBST([TCL_SHARED_BUILD])dnl
AC_SUBST([LD_LIBRARY_PATH_VAR])dnl

AC_SUBST([TCL_BUILD_LIB_SPEC])dnl
AC_SUBST([TCL_NEEDS_EXP_FILE])dnl
AC_SUBST([TCL_BUILD_EXP_FILE])dnl
AC_SUBST([TCL_EXP_FILE])dnl

AC_SUBST([TCL_LIB_VERSIONS_OK])dnl
AC_SUBST([TCL_SHARED_LIB_SUFFIX])dnl
AC_SUBST([TCL_UNSHARED_LIB_SUFFIX])dnl

AC_SUBST([TCL_HAS_LONGLONG])dnl

AC_SUBST([INSTALL_TZDATA])dnl

AC_SUBST([DTRACE_SRC])dnl
AC_SUBST([DTRACE_HDR])dnl
AC_SUBST([DTRACE_OBJ])dnl
AC_SUBST([MAKEFILE_SHELL])dnl

AC_SUBST([BUILD_DLTEST])dnl
AC_SUBST([TCL_PACKAGE_PATH])dnl
AC_SUBST([TCL_MODULE_PATH])dnl

AC_SUBST([TCL_LIBRARY])dnl
AC_SUBST([PRIVATE_INCLUDE_DIR])dnl
AC_SUBST([HTML_DIR])dnl

AC_SUBST([EXTRA_CFLAGS])dnl
AC_SUBST([EXTRA_CC_SWITCHES])dnl
AC_SUBST([EXTRA_APP_CC_SWITCHES])dnl
AC_SUBST([EXTRA_INSTALL])dnl
AC_SUBST([EXTRA_INSTALL_BINARIES])dnl
AC_SUBST([EXTRA_BUILD_HTML])dnl
AC_SUBST([EXTRA_TCLSH_LIBS])dnl

AC_SUBST([DLTEST_LD])dnl
AC_SUBST([DLTEST_SUFFIX])dnl

dnl# Disable the automake-friendly normalization of LIBOBJS
dnl# performed by autoconf 2.53 and later. It is not correct for us.
define([_AC_LIBOBJS_NORMALIZE],[])
AC_CONFIG_FILES([
    Makefile:../unix/Makefile.in
    dltest/Makefile:../unix/dltest/Makefile.in
    tclConfig.sh:../unix/tclConfig.sh.in
])dnl
AC_OUTPUT

