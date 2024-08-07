#! /bin/bash -norc
dnl#	This file is an input file used by the GNU "autoconf" program to
dnl#	generate the file "configure", which is run during Tk installation
dnl#	to configure the system for the local environment.
#
# RCS: @(#) $Id: configure.ac,v 1.82 2002/09/27 18:09:53 dgp Exp $

m4_include([../../config/warnings.m4])
AC_INIT([tcl-unix],[8.4.1],[BUG-REPORT-ADDRESS])
AC_CONFIG_SRCDIR([../generic/tk.h])
AC_PREREQ([2.13])

TK_VERSION=8.4
TK_MAJOR_VERSION=8
TK_MINOR_VERSION=4
TK_PATCH_LEVEL=".1"
VERSION=${TK_VERSION}
LOCALES="cs de el en en_gb es fr it nl ru"

# architecture stuff:
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
# Make sure srcdir is fully qualified!
srcdir=`cd ${srcdir}; pwd`
TK_SRC_DIR=`cd ${srcdir}/..; pwd`

#------------------------------------------------------------------------
# Compress and/or soft link the manpages?
#------------------------------------------------------------------------
SC_CONFIG_MANPAGES

#------------------------------------------------------------------------
# Standard compiler checks
#------------------------------------------------------------------------

# First hack cache:
AC_PROG_SED
if test -e `pwd`/config.cache; then
  echo "hacking `pwd`/config.cache"
  sed -i.bak -e "s|ac_cv_env_CPP_set|bad_CPP_set|g" `pwd`/config.cache
  sed -i.bak -e "s|ac_cv_env_CPP_value|bad_CPP_value|g" `pwd`/config.cache
  sed -i.bak -e "s|ac_cv_prog_CPP|bad_CPP|g" `pwd`/config.cache
elif test -e ./config.cache; then
  echo "hacking ./config.cache"
  sed -i.bak -e "s|ac_cv_env_CPP_set|bad_CPP_set|g" ./config.cache
  sed -i.bak -e "s|ac_cv_env_CPP_value|bad_CPP_value|g" ./config.cache
  sed -i.bak -e "s|ac_cv_prog_CPP|bad_CPP|g" ./config.cache
elif test -e .././config.cache; then
  echo "hacking .././config.cache"
  sed -i.bak -e "s|ac_cv_env_CPP_set|bad_CPP_set|g" .././config.cache
  sed -i.bak -e "s|ac_cv_env_CPP_value|bad_CPP_value|g" .././config.cache
  sed -i.bak -e "s|ac_cv_prog_CPP|bad_CPP|g" .././config.cache
else
  echo "config.cache is missing, which means we can skip attempts to hack it"
fi

# If the user did not set CFLAGS, set it now to keep
# the AC_PROG_CC macro from adding "-g -O2".
if test "${CFLAGS+set}" != "set"; then
    CFLAGS=""
fi

# Actual compiler check(s):
AC_PROG_CC

if test -n "${HOST_ARCHITECTURE}"; then
  ACX_PROG_CC_WARNING_OPTS(["-arch ${HOST_ARCHITECTURE}"],[C_ARCHFLAGS])
  AC_SUBST([C_ARCHFLAGS])
fi

#------------------------------------------------------------------------
# Other programs
#------------------------------------------------------------------------
AC_PROG_AWK
AC_PROG_INSTALL

#------------------------------------------------------------------------
# I am not sure why these need to come before all of the other tests
#------------------------------------------------------------------------

AC_CHECK_HEADERS([arpa/inet.h fcntl.h limits.h netinet/in.h sys/file.h])

#------------------------------------------------------------------------
# Threads support
#------------------------------------------------------------------------

SC_ENABLE_THREADS

#--------------------------------------------------------------------------
# If we are using GCC, then see if the compiler understands -pipe.
# If so, then use it.
# It makes compiling go faster.  (This is only a performance feature.)
#--------------------------------------------------------------------------

if test -z "${no_pipe}"; then
if test -n "${GCC}"; then
  AC_MSG_CHECKING([if the compiler understands -pipe])
  OLDCC="${CC}"  
  CC="${CC} -pipe"
  AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[]],[[]])],
                    [AC_MSG_RESULT([yes])],
                    [CC="${OLDCC}"
                     AC_MSG_RESULT([no])])
fi  
fi

#--------------------------------------------------------------------
#	Detect what compiler flags to set for 64-bit support.
#--------------------------------------------------------------------

SC_TCL_EARLY_FLAGS

SC_TCL_64BIT_FLAGS

#--------------------------------------------------------------------
# Find and load the tclConfig.sh file
#--------------------------------------------------------------------

SC_PATH_TCLCONFIG
SC_LOAD_TCLCONFIG

#--------------------------------------------------------------------
# Recompute the necessary flags to run the compiler
#--------------------------------------------------------------------

SC_ENABLE_SHARED

SC_CONFIG_CFLAGS

SC_ENABLE_SYMBOLS

LIB_RUNTIME_DIR='${LIB_RUNTIME_DIR}'

TK_DBGX=${DBGX}

#------------------------------------------------------------------------
# If Tcl and Tk are installed in different places, adjust the library
# search path to reflect this.
#------------------------------------------------------------------------

if test "${TCL_EXEC_PREFIX}" != "${exec_prefix}"; then
    LIB_RUNTIME_DIR="${LIB_RUNTIME_DIR}:${TCL_EXEC_PREFIX}/lib"
fi

#--------------------------------------------------------------------
# On a few very rare systems, all of the libm.a stuff is
# already in libc.a.  Set compiler flags accordingly.
# Also, Linux requires the "ieee" library for math to work
# right (and it must appear before "-lm").
#--------------------------------------------------------------------

AC_CHECK_FUNC([sin],[MATH_LIBS=""],[MATH_LIBS="-lm"])
AC_CHECK_LIB([ieee],[main],[MATH_LIBS="-lieee ${MATH_LIBS}"])

#--------------------------------------------------------------------
# On AIX systems, libbsd.a has to be linked in to support
# non-blocking file IO.  This library has to be linked in after
# the MATH_LIBS or it breaks the pow() function.  The way to
# insure proper sequencing, is to add it to the tail of MATH_LIBS.
# This library also supplies gettimeofday.
#--------------------------------------------------------------------
libbsd=no
if test "`uname -s`" = "AIX"; then
    AC_CHECK_LIB([bsd],[gettimeofday],[libbsd=yes])
    if test "x${libbsd}" = "xyes"; then
	MATH_LIBS="${MATH_LIBS} -lbsd"
    fi
fi

#--------------------------------------------------------------------
# Supply a substitute for stdlib.h if it fails to define strtol,
# strtoul, or strtod (which happens on some versions of SunOS).
#--------------------------------------------------------------------

AC_MSG_CHECKING([for a usable stdlib.h])
AC_EGREP_HEADER([strtol],[stdlib.h],[tk_ok=yes],[tk_ok=no])
AC_EGREP_HEADER([strtoul],[stdlib.h],[],[tk_ok=no])
AC_EGREP_HEADER([strtod],[stdlib.h],[],[tk_ok=no])
if test "x${tk_ok}" = "xno"; then
    AC_DEFINE([NO_STDLIB_H])
fi
AC_MSG_RESULT([${tk_ok}])

AC_CHECK_FUNCS([strtol strtoul])

#--------------------------------------------------------------------
# Include sys/select.h if it exists and if it supplies things
# that appear to be useful and are NOT already in sys/types.h.
# This appears to be true only on the RS/6000 under AIX.  Some
# systems like OSF/1 have a sys/select.h that is of no use, and
# other systems like SCO UNIX have a sys/select.h that is
# pernicious.  If "fd_set" is NOT defined anywhere then set a
# special flag.
#--------------------------------------------------------------------

AC_MSG_CHECKING([fd_set and sys/select])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[#include <sys/types.h>]],
                                   [[fd_set readMask, writeMask;]])],
                  [tk_ok=yes],[tk_ok=no])
if test "x${tk_ok}" = "xno"; then
    AC_EGREP_HEADER([fd_mask],[sys/select.h],[tk_ok=yes])
    if test "x${tk_ok}" = "xyes"; then
	AC_DEFINE([HAVE_SYS_SELECT_H])
    fi
fi
AC_MSG_RESULT([${tk_ok}])
if test "x${tk_ok}" = "xno"; then
    AC_DEFINE([NO_FD_SET])
fi

#--------------------------------------------------------------------
# Check for various typedefs and provide substitutes if
# they are missing.
#--------------------------------------------------------------------

AC_TYPE_MODE_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_UID_T

#--------------------------------------------------------------------
# Check for various functions.
#--------------------------------------------------------------------
AC_CHECK_FUNCS([memset select strcasecmp strchr strrchr])dnl

#--------------------------------------------------------------------------
#       Find out about time handling differences.
#--------------------------------------------------------------------------

AC_CHECK_HEADERS([sys/time.h])
AC_HEADER_TIME

#-------------------------------------------
#     In OS/390 struct pwd has no pw_gecos field
#-------------------------------------------

AC_MSG_CHECKING([pw_gecos in struct pwd])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[#include <pwd.h>]],
                                   [[struct passwd pwd; pwd.pw_gecos;]])],
                  [tk_ok=yes],[tk_ok=no])
AC_MSG_RESULT([${tk_ok}])
if test "x${tk_ok}" = "xyes"; then
    AC_DEFINE([HAVE_PW_GECOS])
fi

#--------------------------------------------------------------------
#	Locate the X11 header files and the X11 library archive.  Try
#	the ac_path_x macro first, but if it fails to find the X stuff
#	(e.g. because there is no xmkmf program) then check through
#	a list of possible directories.  Under some conditions the
#	autoconf macro will return an include directory that contains
#	no include files, so double-check its result just to be safe.
#--------------------------------------------------------------------

SC_PATH_X

#--------------------------------------------------------------------
#	Various manipulations on the search path used at runtime to
#	find shared libraries:
#	1. If the X library binaries are in a non-standard directory,
#	   add the X library location into that search path.
#	2. On systems such as AIX and Ultrix that use "-L" as the
#	   search path option, colons cannot be used to separate
#	   directories from each other. Change colons to " -L".
#	3. Create two sets of search flags, one for use in cc lines
#	   and the other for when the linker is invoked directly.  In
#	   the second case, '-Wl,' must be stripped off and commas must
#	   be replaced by spaces.
#--------------------------------------------------------------------

if test "x${x_libraries}" != "x"; then
    LIB_RUNTIME_DIR="${LIB_RUNTIME_DIR}:${x_libraries}"
fi
if test "${TCL_LD_SEARCH_FLAGS}" = '-L${LIB_RUNTIME_DIR}'; then
    LIB_RUNTIME_DIR=`echo ${LIB_RUNTIME_DIR} |sed -e 's/:/ -L/g'`
fi

# The statement below is very tricky!  It actually *evaluates* the
# string in TCL_CC_SEARCH_FLAGS and TCL_LD_SEARCH_FLAGS which
# causes a substitution of the variable LIB_RUNTIME_DIR.

eval "CC_SEARCH_FLAGS=\"$TCL_CC_SEARCH_FLAGS\""
eval "LD_SEARCH_FLAGS=\"$TCL_LD_SEARCH_FLAGS\""

#--------------------------------------------------------------------
#	Check for the existence of various libraries.  The order here
#	is important, so that then end up in the right order in the
#	command line generated by make.  The -lsocket and -lnsl libraries
#	require a couple of special tricks:
#	1. Use "connect" and "accept" to check for -lsocket, and
#	   "gethostbyname" to check for -lnsl.
#	2. Use each function name only once:  cannot redo a check because
#	   autoconf caches the results of the last check and will NOT redo it.
#	3. Use -lnsl and -lsocket only if they supply procedures that
#	   are NOT already present in the normal libraries.  This is because
#	   IRIX 5.2 has libraries, but they are NOT needed and they are
#	   bogus: they goof up name resolution if used.
#	4. On some SVR4 systems, cannot use -lsocket without -lnsl too.
#	   To get around this problem, check for both libraries together
#	   if -lsocket fails to work by itself.
#--------------------------------------------------------------------

AC_CHECK_LIB([Xbsd],[main],[LIBS="${LIBS} -lXbsd"])

tk_checkBoth=0
AC_CHECK_FUNC([connect],[tk_checkSocket=0],[tk_checkSocket=1])
if test "${tk_checkSocket}" = 1; then
    AC_CHECK_LIB([socket],[main],[LIBS="${LIBS} -lsocket"],[tk_checkBoth=1])
fi
if test "${tk_checkBoth}" = 1; then
    tk_oldLibs="${LIBS}"
    LIBS="${LIBS} -lsocket -lnsl"
    AC_CHECK_FUNC([accept],[tk_checkNsl=0],[LIBS=${tk_oldLibs}])
fi
AC_CHECK_FUNC([gethostbyname],[],
              [AC_CHECK_LIB([nsl],[main],[LIBS="${LIBS} -lnsl"])])

# Add the threads support libraries

LIBS="${LIBS} ${THREADS_LIBS}"

#--------------------------------------------------------------------
# One more check related to the X libraries.  The standard releases
# of Ultrix fail to support the "xauth" mechanism, so send will fail
# unless TK_NO_SECURITY is defined.  However, there are usually copies
# of the MIT X server available as well, which do support xauth.
# Check for the MIT stuff and use it if it exists.
#
# Note: cannot use ac_check_lib macro (at least, not in Autoconf 2.1)
# because it cannot deal with the "-" in the library name.
#--------------------------------------------------------------------

if test -d /usr/include/mit; then
    AC_MSG_CHECKING([MIT X libraries])
    tk_oldCFlags=$CFLAGS
    CFLAGS="$CFLAGS -I/usr/include/mit"
    tk_oldLibs=$LIBS
    LIBS="$LIBS -lX11-mit"
    AC_LINK_IFELSE([AC_LANG_PROGRAM([[
	#include <X11/Xlib.h>
    ]], [[
	XOpenDisplay(0);
    ]])],[
	AC_MSG_RESULT([yes])
	XLIBSW="-lX11-mit"
	XINCLUDES="-I/usr/include/mit"
    ],[AC_MSG_RESULT([no])])
    CFLAGS="${tk_oldCFlags}"
    LIBS="${tk_oldLibs}"
else
    AC_MSG_NOTICE([skipping MIT X checks])
fi

#--------------------------------------------------------------------
#	On a few very rare systems, all of the libm.a stuff is
#	already in libc.a.  Set compiler flags accordingly.
#	Also, Linux requires the "ieee" library for math to
#	work right (and it must appear before "-lm").
#--------------------------------------------------------------------

MATH_LIBS=""
AC_CHECK_FUNC([sin],[],[MATH_LIBS="-lm"])
AC_CHECK_LIB([ieee],[main],[MATH_LIBS="-lieee ${MATH_LIBS}"])

#--------------------------------------------------------------------
#	Figure out whether "char" is unsigned.  If so, set a
#	#define for __CHAR_UNSIGNED__.
#--------------------------------------------------------------------

AC_C_CHAR_UNSIGNED

#--------------------------------------------------------------------
#	Under Solaris 2.4, strtod returns the wrong value for the
#	terminating character under some conditions.  Check for this
#	and if the problem exists use a substitute procedure
#	"fixstrtod" (provided by Tcl) that corrects the error.
#--------------------------------------------------------------------

SC_BUGGY_STRTOD

#--------------------------------------------------------------------
#	The statements below define a collection of symbols related to
#	building libtk as a shared library instead of a static library.
#--------------------------------------------------------------------

eval eval "TK_SHARED_LIB_SUFFIX=${SHARED_LIB_SUFFIX}"
eval eval "TK_UNSHARED_LIB_SUFFIX=${UNSHARED_LIB_SUFFIX}"

if test "${SHARED_BUILD}" = "1" -a "${SHLIB_SUFFIX}" != ""; then
    SHLIB_LD_LIBS="${SHLIB_LD_LIBS} \${TCL_STUB_LIB_SPEC}"
    TCL_STUB_FLAGS="-DUSE_TCL_STUBS"
else
    TCL_STUB_FLAGS=""
fi
TK_LIB_FILE=libtk${LIB_SUFFIX}
eval "TK_LIB_FILE=${TK_LIB_FILE}"
AC_MSG_NOTICE([TK_LIB_FILE is now ${TK_LIB_FILE}])dnl

# Note:  in the following variable, it is important to use the absolute
# path name of the Tcl directory rather than "..": this is because
# AIX remembers this path and will attempt to use it at run-time to look
# up the Tcl library.

if test "${SHARED_BUILD}" = 0 -o ${TCL_NEEDS_EXP_FILE} = 0; then
    if test "${TCL_LIB_VERSIONS_OK}" = "ok"; then
	eval TK_LIB_FLAG="-ltk${VERSION}\${TK_DBGX}"
    else
	eval TK_LIB_FLAG="-ltk`echo ${VERSION} | tr -d .`\${TK_DBGX}"
    fi
    TK_BUILD_LIB_SPEC="-L`pwd` ${TK_LIB_FLAG}"
    TK_LIB_SPEC="-L${libdir} ${TK_LIB_FLAG}"
    TK_BUILD_EXP_FILE=""
    TK_EXP_FILE=""
else
    TK_BUILD_EXP_FILE="lib.exp"
    eval "TK_EXP_FILE=libtk${TCL_EXPORT_FILE_SUFFIX}"

    if test "${GCC}" = "yes"; then
	TK_BUILD_LIB_SPEC="-Wl,-bI:`pwd`/${TK_BUILD_EXP_FILE} -L`pwd`"
	TK_LIB_SPEC="-Wl,-bI:${libdir}/${TK_EXP_FILE} -L`pwd`"    
    else
	TK_BUILD_LIB_SPEC="-bI:`pwd`/${TK_BUILD_EXP_FILE}"
	TK_LIB_SPEC="-bI:${libdir}/${TK_EXP_FILE}"    
    fi
fi
TK_SHARED_BUILD=${SHARED_BUILD}

#--------------------------------------------------------------------
# The statements below define various symbols relating to creating
# the stubbed version of the Tk library
#
# For now, linking to Tcl stubs is not supported with Tk. It causes
# too many problems with linking.  When Tk is a fully loadable 
# extension, linking the the Tcl stubs will be supported.
#--------------------------------------------------------------------

# Replace ${VERSION} with contents of ${TK_VERSION}
eval "TK_STUB_LIB_FILE=libtkstub${TK_UNSHARED_LIB_SUFFIX}"

if test "${TCL_LIB_VERSIONS_OK}" = "ok"; then
    eval TK_STUB_LIB_FLAG="-ltkstub${TK_VERSION}\${TK_DBGX}"
else
    eval TK_STUB_LIB_FLAG="-ltkstub`echo ${TK_VERSION} | tr -d .`\${TK_DBGX}"
fi

TK_BUILD_STUB_LIB_SPEC="-L`pwd` ${TK_STUB_LIB_FLAG}"
TK_STUB_LIB_SPEC="-L${libdir} ${TK_STUB_LIB_FLAG}"
TK_BUILD_STUB_LIB_PATH="`pwd`/${TK_STUB_LIB_FILE}"
TK_STUB_LIB_PATH="${libdir}/${TK_STUB_LIB_FILE}"

eval "TK_LIB_FILE=${TK_LIB_FILE}"
AC_MSG_NOTICE([...and finally, TK_LIB_FILE is now ${TK_LIB_FILE}])dnl

# A few more things.
AC_C_PROTOTYPES
if test "x${ac_prog_cc_stdc}" = "xno"; then
  PROTO_FLAGS="-DNO_PROTOTYPE"
fi
AC_SUBST([PROTO_FLAGS])dnl

AC_SUBST([TK_VERSION])dnl
AC_SUBST([TK_MAJOR_VERSION])dnl
AC_SUBST([TK_MINOR_VERSION])dnl
AC_SUBST([TK_PATCH_LEVEL])dnl
AC_SUBST([TK_DBGX])dnl

AC_SUBST([TK_STUB_LIB_FILE])dnl
AC_SUBST([TK_STUB_LIB_FLAG])dnl
AC_SUBST([TK_BUILD_STUB_LIB_SPEC])dnl
AC_SUBST([TK_STUB_LIB_SPEC])dnl
AC_SUBST([TK_BUILD_STUB_LIB_PATH])dnl
AC_SUBST([TK_STUB_LIB_PATH])dnl
AC_SUBST([TK_STUB_FLAGS])dnl
AC_SUBST([TK_BUILD_EXP_FILE])dnl
AC_SUBST([TK_EXP_FILE])dnl

AC_SUBST([TCL_STUB_FLAGS])dnl
AC_SUBST([TK_BUILD_EXP_FILE])dnl
AC_SUBST([TK_EXP_FILE])dnl

AC_SUBST([LD_LIBRARY_PATH_VAR])dnl

AC_SUBST([MATH_LIBS])dnl
AC_SUBST([TK_BUILD_LIB_SPEC])dnl
AC_SUBST([TK_LIB_FILE])dnl
AC_SUBST([TK_LIB_FLAG])dnl
AC_SUBST([TK_LIB_SPEC])dnl
AC_SUBST([TK_SRC_DIR])dnl
AC_SUBST([XINCLUDES])dnl
AC_SUBST([XLIBSW])dnl
AC_SUBST([TK_SHARED_BUILD])dnl
AC_SUBST([LOCALES])dnl

# Output.
AC_CONFIG_FILES([Makefile tkConfig.sh])
AC_OUTPUT
