#!/bin/bash -norc
dnl#	This file is an input file used by the GNU "autoconf" program to
dnl#	generate the file "configure", which is run during Tcl installation
dnl#	to configure the system for the local environment.
#
# RCS: @(#) configure.in,v 1.9 2003/01/21 19:40:19 hunt Exp
# -*- Autoconf -*-

AC_INIT([Tcl-unix],[8.4.1],[])
AC_CONFIG_SRCDIR([../generic/tcl.h])
AC_PREREQ([2.13])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])
AC_CANONICAL_TARGET
AM_SANITY_CHECK
AM_MAINTAINER_MODE

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
AC_CHECK_HEADERS_ONCE([float.h limits.h unistd.h])

# Some more programs
AC_PROG_AWK
AC_PROG_INSTALL
AM_PROG_INSTALL_STRIP
AM_PROG_INSTALL_SH
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_YACC
AC_PATH_PROG([GDB_BIN],[gdb])
AC_PATH_PROG([DDD_BIN],[ddd])

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
AC_CHECK_FUNCS([getcwd],[],[AC_DEFINE([USEGETWD],[1],[Define to 1 if getcwd is missing and we have to use getwd as a fallback])])
# Nb: if getcwd uses popen and pwd(1) (like SunOS 4) we should really
# define USEGETWD even if the posix getcwd exists. Add a test?

AC_REPLACE_FUNCS([opendir strstr])

AC_REPLACE_FUNCS([strtol strtoll strtoull tmpnam waitpid])
AC_CHECK_FUNCS([strerror],[],[AC_DEFINE([NO_STRERROR],[1],[Define to 1 if we are missing strerror])])
AC_CHECK_FUNCS([getwd],[],[AC_DEFINE([NO_GETWD],[1],[Define to 1 if we are missing getwd])])
AC_CHECK_FUNCS([wait3],[],[AC_DEFINE([NO_WAIT3],[1],[Define to 1 if we are missing wait3])])
AC_CHECK_FUNCS([uname],[],[AC_DEFINE([NO_UNAME],[1],[Define to 1 if we are missing uname])])
AC_CHECK_FUNCS([realpath],[],[AC_DEFINE([NO_REALPATH],[1],[Define to 1 if we are missing realpath])])

# Some more functions
AC_FUNC_CHOWN
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_CHECK_FUNCS([alarm atexit dup2 endgrent endpwent gethostbyaddr \
                gethostname inet_ntoa isxdigit memset mkdir mkfifo \
                nl_langinfo rmdir select setlocale socket strcasecmp \
                strchr strrchr strstr strtol strtoull tzset utime])

AC_CHECK_FUNCS([execl execle execlp])

#--------------------------------------------------------------------
#	Supply substitutes for missing POSIX header files. Special
#	notes:
#	    - stdlib.h does NOT define strtol, strtoul, or
#	      strtod insome versions of SunOS
#	    - some versions of string.h do NOT declare procedures such
#	      as strstr
#--------------------------------------------------------------------

SC_MISSING_POSIX_HEADERS

# More headers
AC_CHECK_HEADERS([CoreFoundation/CoreFoundation.h arpa/inet.h \
                  fcntl.h locale.h netdb.h netinet/in.h sgtty.h stdarg.h \
                  sys/file.h sys/socket.h sys/statfs.h termio.h termios.h \
                  utime.h])

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
	AC_EGREP_HEADER([fd_mask],[sys/select.h],[tcl_cv_grep_fd_mask=present],[tcl_cv_grep_fd_mask=missing]))
    AC_MSG_RESULT([${tcl_cv_grep_fd_mask}])
    if test ${tcl_cv_grep_fd_mask} = present; then
	AC_DEFINE([HAVE_SYS_SELECT_H],[1],[Define to 1 if we have a working sys/select.h])
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
  AC_REQUIRE([AC_HEADER_TIME])
  AC_REQUIRE([AC_STRUCT_TM])
  AC_REQUIRE([AC_STRUCT_TIMEZONE])
])
AC_REQUIRE_TIME_CHECKS

#--------------------------------------------------------------------
#	Some systems (e.g., IRIX 4.0.5) lack the st_blksize field
#	in struct stat.  But we might be able to use fstatfs instead.
#--------------------------------------------------------------------
AC_CHECK_MEMBERS([struct stat.st_blksize],[AC_DEFINE([HAVE_ST_BLKSIZE],[1],
			    [Define to 1 if your `struct stat' has
			     `st_blksize'. Deprecated, use
			     `HAVE_STRUCT_STAT_ST_BLKSIZE' instead.])])

AC_CHECK_FUNCS([fstatfs],[],[AC_DEFINE([NO_FSTATFS],[1],[Define to 1 if there is no fstatfs])])

# Another member check
AC_CHECK_MEMBERS([struct stat.st_rdev])

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
  AC_DEFINE([NO_MEMMOVE],[1],[Define to 1 if there is no memmove])
  AC_DEFINE([NO_STRING_H],[1],[Define to 1 if there is no working string.h])
])

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
if test $tcl_ok = yes; then
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
AC_CHECK_SIZEOF([int])
AC_CHECK_ALIGNOF([int])
AC_CHECK_SIZEOF([double])
AC_CHECK_ALIGNOF([double])
AC_CHECK_SIZEOF([char])
AC_CHECK_ALIGNOF([char])
AC_CHECK_TYPES([regoff_t])

#--------------------------------------------------------------------
#	If a system does NOT have an opendir function (man, is that old!)
#	then we have to supply a different version of dirent.h which
#	is compatible with the substitute version of opendir that is
#	provided. This version only works with V7-style directories.
#--------------------------------------------------------------------

AC_CHECK_FUNCS([opendir],[],[
  AC_DEFINE([USE_DIRENT2_H],[2],[Define to 2 to use a different version of dirent.h])
])

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
    [AC_DEFINE([HAVE_BSDGETTIMEOFDAY],[1],[Define to 1 if we have the BSDgettimeofday function])],[
    AC_CHECK_FUNCS([gettimeofday],[],[AC_DEFINE([NO_GETTOD],[1],[Define to 1 if there is no gettimeofday function])])
])
AC_MSG_CHECKING([for gettimeofday declaration])
AC_CACHE_VAL([tcl_cv_grep_gettimeofday],
    [AC_EGREP_HEADER([gettimeofday],[sys/time.h],
	[tcl_cv_grep_gettimeofday=present], [tcl_cv_grep_gettimeofday=missing])])
AC_MSG_RESULT([${tcl_cv_grep_gettimeofday}])
if test ${tcl_cv_grep_gettimeofday} = missing ; then
    AC_DEFINE([GETTOD_NOT_DECLARED],[1],[Define to 1 if gettimeofday is not declared])
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
    AC_DEFINE([HAVE_SIGNED_CHAR],[1],[Define to 1 if we have a signed char declaration])
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
    AC_DEFINE([HAVE_PUTENV_THAT_COPIES],[1],[Define to 1 if we have a putenv that copies])
fi

#--------------------------------------------------------------------
# Check for support of nl_langinfo function
#--------------------------------------------------------------------

SC_ENABLE_LANGINFO

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

#--------------------------------------------------------------------
#	The statements below define a collection of symbols related to
#	building libtcl as a shared library instead of a static library.
#--------------------------------------------------------------------

TCL_UNSHARED_LIB_SUFFIX=${UNSHARED_LIB_SUFFIX}
TCL_SHARED_LIB_SUFFIX=${SHARED_LIB_SUFFIX}
eval "TCL_LIB_FILE=libtcl${LIB_SUFFIX}"

SC_ENABLE_FRAMEWORK

# tclConfig.sh needs a version of the _LIB_SUFFIX that has been eval'ed
# so that the backslashes quoting the DBX braces are dropped.

# Trick to replace DBGX with TCL_DBGX
DBGX='${TCL_DBGX}'
eval "TCL_LIB_FILE=${TCL_LIB_FILE}"

# Note: in the following variable, it is important to use the absolute
# path name of the Tcl directory rather than "..":  this is because
# AIX remembers this path and will attempt to use it at run-time to look
# up the Tcl library.

if test "${FRAMEWORK_BUILD}" = "1" ; then
    TCL_BUILD_LIB_SPEC="-F`pwd` -framework Tcl"
    TCL_LIB_SPEC="-framework Tcl"
    TCL_LIB_FILE="Tcl"
elif test "${SHARED_BUILD}" = "0" || test "${TCL_NEEDS_EXP_FILE}" = "0"; then
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

if test "${FRAMEWORK_BUILD}" = "1" ; then
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

AC_SUBST([ACLOCAL])
AC_SUBST([TCL_VERSION])
AC_SUBST([TCL_MAJOR_VERSION])
AC_SUBST([TCL_MINOR_VERSION])
AC_SUBST([TCL_PATCH_LEVEL])

AC_SUBST([TCL_LIB_FILE])
AC_SUBST([TCL_LIB_FLAG])
AC_SUBST([TCL_LIB_SPEC])
AC_SUBST([TCL_STUB_LIB_FILE])
AC_SUBST([TCL_STUB_LIB_FLAG])
AC_SUBST([TCL_STUB_LIB_SPEC])
AC_SUBST([TCL_STUB_LIB_PATH])
AC_SUBST([TCL_INCLUDE_SPEC])
AC_SUBST([TCL_BUILD_STUB_LIB_SPEC])
AC_SUBST([TCL_BUILD_STUB_LIB_PATH])

AC_SUBST([TCL_SRC_DIR])
AC_SUBST([TCL_DBGX])
AC_SUBST([CFG_TCL_SHARED_LIB_SUFFIX])
AC_SUBST([CFG_TCL_UNSHARED_LIB_SUFFIX])
AC_SUBST([CFG_TCL_EXPORT_FILE_SUFFIX])

AC_SUBST([TCL_SHARED_BUILD])
AC_SUBST([LD_LIBRARY_PATH_VAR])

AC_SUBST([TCL_BUILD_LIB_SPEC])
AC_SUBST([TCL_NEEDS_EXP_FILE])
AC_SUBST([TCL_BUILD_EXP_FILE])
AC_SUBST([TCL_EXP_FILE])

AC_SUBST([TCL_LIB_VERSIONS_OK])
AC_SUBST([TCL_SHARED_LIB_SUFFIX])
AC_SUBST([TCL_UNSHARED_LIB_SUFFIX])

AC_SUBST([TCL_HAS_LONGLONG])

AC_SUBST([BUILD_DLTEST])
AC_SUBST([TCL_PACKAGE_PATH])

AC_CONFIG_FILES([Makefile dltest/Makefile tclConfig.sh])
AC_OUTPUT
