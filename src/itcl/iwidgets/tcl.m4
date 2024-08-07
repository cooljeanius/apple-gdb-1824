# tcl.m4 --
#
#	This file provides a set of autoconf macros to help TEA-enable
#	a Tcl extension.
#
# Copyright (c) 1999 Scriptics Corporation.
#
# See the file "license.terms" for information on usage and redistribution
# of this file, and for a DISCLAIMER OF ALL WARRANTIES.

#------------------------------------------------------------------------
# SC_PATH_TCLCONFIG --
#
#	Locate the tclConfig.sh file and perform a sanity check on
#	the Tcl compile flags
#
# Arguments:
#	none
#
# Results:
#
#	Adds the following arguments to configure:
#		--with-tcl=...
#
#	Defines the following vars:
#		TCL_BIN_DIR	Full path to the directory containing
#				the tclConfig.sh file
#------------------------------------------------------------------------

AC_DEFUN([SC_PATH_TCLCONFIG],[
    #
    # Ok, let us find the tcl configuration
    # First, look for one uninstalled.
    # the alternative search directory is invoked by --with-tcl
    #

    if test x"${no_tcl}" = x""; then
	# we reset no_tcl in case something fails here
	no_tcl=true
	AC_ARG_WITH([tcl],
	            [AS_HELP_STRING([--with-tcl],
	                            [directory containing tcl configuration (tclConfig.sh)])],
	            [with_tclconfig=${withval}])
	AC_MSG_CHECKING([for Tcl configuration])
	AC_CACHE_VAL([ac_cv_c_tclconfig],[

	    # First check to see if --with-tcl was specified.
	    if test x"${with_tclconfig}" != x""; then
		if test -f "${with_tclconfig}/tclConfig.sh"; then
		    ac_cv_c_tclconfig=`(cd ${with_tclconfig}; pwd)`
		else
		    AC_MSG_ERROR([${with_tclconfig} directory does NOT contain tclConfig.sh])
		fi
	    fi

	    # then check for a private Tcl installation
	    if test x"${ac_cv_c_tclconfig}" = x""; then
		for i in \
			../tcl \
			`ls -dr ../tcl[[8-9]].[[0-9]]* 2>/dev/null` \
			../../tcl \
			`ls -dr ../../tcl[[8-9]].[[0-9]]* 2>/dev/null` \
			../../../tcl \
			`ls -dr ../../../tcl[[8-9]].[[0-9]]* 2>/dev/null` \
			../../dist/tcl; do
		    if test -f "${i}/unix/tclConfig.sh"; then
			ac_cv_c_tclconfig=`(cd ${i}/unix; pwd)`
			break
		    elif test -f "${i}/unix/tclConfig.sh.in"; then
			AC_MSG_NOTICE([file from which tclConfig.sh is generated was found in ${i}, but not the actual script itself])
			AC_MSG_NOTICE([perhaps you still need to configure and/or build there?])
		    fi
		done
	    fi

	    # check in a few common install locations
	    if test x"${ac_cv_c_tclconfig}" = x""; then
		for i in `ls -d ${libdir} 2>/dev/null` \
			`ls -d /usr/local/lib 2>/dev/null` \
			`ls -d /usr/contrib/lib 2>/dev/null` \
			`ls -d /usr/lib 2>/dev/null` \
			`ls -d /opt/local/lib 2>/dev/null` \
			`ls -d /opt/sw/lib 2>/dev/null`; do
		    if test -f "${i}/tclConfig.sh"; then
			ac_cv_c_tclconfig=`(cd ${i}; pwd)`
			break
		    fi
		done
	    fi

	    # check in a few other private locations
	    if test x"${ac_cv_c_tclconfig}" = x""; then
		for i in \
			${srcdir}/../tcl \
			`ls -dr ${srcdir}/../tcl[[8-9]].[[0-9]]* 2>/dev/null` \
			${srcdir}/../../tcl; do
		    if test -f "${i}/unix/tclConfig.sh"; then
		    ac_cv_c_tclconfig=`(cd ${i}/unix; pwd)`
		    break
		fi
		done
	    fi
	])

	if test x"${ac_cv_c_tclconfig}" = x""; then
	    TCL_BIN_DIR="# no Tcl configs found"
	    AC_MSG_WARN([Cannot find Tcl configuration definitions])
	    exit 0
	else
	    no_tcl=""
	    TCL_BIN_DIR=${ac_cv_c_tclconfig}
	    AC_MSG_RESULT([found ${TCL_BIN_DIR}/tclConfig.sh])
	fi
    fi
])

#------------------------------------------------------------------------
# SC_PATH_TKCONFIG --
#
#	Locate the tkConfig.sh file
#
# Arguments:
#	none
#
# Results:
#
#	Adds the following arguments to configure:
#		--with-tk=...
#
#	Defines the following vars:
#		TK_BIN_DIR	Full path to the directory containing
#				the tkConfig.sh file
#------------------------------------------------------------------------

AC_DEFUN([SC_PATH_TKCONFIG],[
    #
    # Ok, lets find the tk configuration
    # First, look for one uninstalled.
    # the alternative search directory is invoked by --with-tk
    #

    if test x"${no_tk}" = x ; then
	# we reset no_tk in case something fails here
	no_tk=true
	AC_ARG_WITH(tk, [  --with-tk               directory containing tk configuration (tkConfig.sh)], with_tkconfig=${withval})
	AC_MSG_CHECKING([for Tk configuration])
	AC_CACHE_VAL(ac_cv_c_tkconfig,[

	    # First check to see if --with-tkconfig was specified.
	    if test x"${with_tkconfig}" != x ; then
		if test -f "${with_tkconfig}/tkConfig.sh" ; then
		    ac_cv_c_tkconfig=`(cd ${with_tkconfig}; pwd)`
		else
		    AC_MSG_ERROR([${with_tkconfig} directory doesn't contain tkConfig.sh])
		fi
	    fi

	    # then check for a private Tk library
	    if test x"${ac_cv_c_tkconfig}" = x ; then
		for i in \
			../tk \
			`ls -dr ../tk[[8-9]].[[0-9]]* 2>/dev/null` \
			../../tk \
			`ls -dr ../../tk[[8-9]].[[0-9]]* 2>/dev/null` \
			../../../tk \
			`ls -dr ../../../tk[[8-9]].[[0-9]]* 2>/dev/null` ; do
		    if test -f "$i/unix/tkConfig.sh" ; then
			ac_cv_c_tkconfig=`(cd $i/unix; pwd)`
			break
		    fi
		done
	    fi
	    # check in a few common install locations
	    if test x"${ac_cv_c_tkconfig}" = x ; then
		for i in `ls -d ${prefix}/lib 2>/dev/null` \
			`ls -d /usr/local/lib 2>/dev/null` ; do
		    if test -f "$i/tkConfig.sh" ; then
			ac_cv_c_tkconfig=`(cd $i; pwd)`
			break
		    fi
		done
	    fi
	    # check in a few other private locations
	    if test x"${ac_cv_c_tkconfig}" = x ; then
		for i in \
			${srcdir}/../tk \
			`ls -dr ${srcdir}/../tk[[8-9]].[[0-9]]* 2>/dev/null` ; do
		    if test -f "$i/unix/tkConfig.sh" ; then
			ac_cv_c_tkconfig=`(cd $i/unix; pwd)`
			break
		    fi
		done
	    fi
	])
	if test x"${ac_cv_c_tkconfig}" = x ; then
	    TK_BIN_DIR="# no Tk configs found"
	    AC_MSG_WARN(Can't find Tk configuration definitions)
	    exit 0
	else
	    no_tk=
	    TK_BIN_DIR=${ac_cv_c_tkconfig}
	    AC_MSG_RESULT(found $TK_BIN_DIR/tkConfig.sh)
	fi
    fi

])

#------------------------------------------------------------------------
# SC_LOAD_TCLCONFIG --
#
#	Load the tclConfig.sh file
#
# Arguments:
#	
#	Requires the following vars to be set:
#		TCL_BIN_DIR
#
# Results:
#
#	Subst the following vars:
#		TCL_BIN_DIR
#		TCL_SRC_DIR
#		TCL_LIB_FILE
#
#------------------------------------------------------------------------

AC_DEFUN([SC_LOAD_TCLCONFIG],[
    AC_MSG_CHECKING([for existence of $TCL_BIN_DIR/tclConfig.sh])

    if test -f "$TCL_BIN_DIR/tclConfig.sh" ; then
        AC_MSG_RESULT([loading])
	. $TCL_BIN_DIR/tclConfig.sh
    else
        AC_MSG_RESULT([file not found])
    fi

    #
    # The eval is required to do the TCL_DBGX substitution in the
    # TCL_LIB_FILE variable
    #

    eval TCL_LIB_FILE=${TCL_LIB_FILE}
    eval TCL_LIB_FLAG=${TCL_LIB_FLAG}

    AC_SUBST(TCL_DBGX)
    AC_SUBST(TCL_BIN_DIR)
    AC_SUBST(TCL_SRC_DIR)
    AC_SUBST(TCL_LIB_FILE)
    AC_SUBST(TCL_LIBS)
    AC_SUBST(TCL_DEFS)
    AC_SUBST(TCL_SHLIB_LD_LIBS)
    AC_SUBST(TCL_EXTRA_CFLAGS)
    AC_SUBST(TCL_LD_FLAGS)
    AC_SUBST(TCL_LIB_FILE)
    AC_SUBST(TCL_STUB_LIB_FILE)
    AC_SUBST(TCL_LIB_SPEC)
    AC_SUBST(TCL_BUILD_LIB_SPEC)
    AC_SUBST(TCL_STUB_LIB_SPEC)
    AC_SUBST(TCL_BUILD_STUB_LIB_SPEC)
])

#------------------------------------------------------------------------
# SC_LOAD_TKCONFIG --
#
#	Load the tkConfig.sh file
#
# Arguments:
#	
#	Requires the following vars to be set:
#		TK_BIN_DIR
#
# Results:
#
#	Sets the following vars that should be in tkConfig.sh:
#		TK_BIN_DIR
#------------------------------------------------------------------------

AC_DEFUN([SC_LOAD_TKCONFIG],[
    AC_MSG_CHECKING([for existence of $TK_BIN_DIR/tkConfig.sh])

    if test -f "$TK_BIN_DIR/tkConfig.sh" ; then
        AC_MSG_RESULT([loading])
	. $TK_BIN_DIR/tkConfig.sh
    else
        AC_MSG_RESULT([could not find $TK_BIN_DIR/tkConfig.sh])
    fi

    AC_SUBST(TK_BIN_DIR)
    AC_SUBST(TK_SRC_DIR)
    AC_SUBST(TK_LIB_FILE)
    AC_SUBST(TK_XINCLUDES)
])

#------------------------------------------------------------------------
# SC_ENABLE_GCC --
#
#	Allows the use of GCC if available
#
# Arguments:
#	none
#	
# Results:
#
#	Adds the following arguments to configure:
#		--enable-gcc
#
#	Sets the following vars:
#		CC	Command to use for the compiler
#------------------------------------------------------------------------

AC_DEFUN([SC_ENABLE_GCC],[
    AC_ARG_ENABLE(gcc, [  --enable-gcc            allow use of gcc if available [--disable-gcc]],
	[ok=$enableval], [ok=no])
    if test "$ok" = "yes"; then
	CC=gcc
    else
	case "`uname -s`" in
	    *win32* | *WIN32* | *CYGWIN_NT* | *CYGWIN_98* | *CYGWIN_95*)
		CC=cl
	    ;;
	    *)
		CC=${CC-cc}
	    ;;
	esac
    fi
    AC_PROG_CC
])

#------------------------------------------------------------------------
# SC_ENABLE_SHARED --
#
#	Allows the building of shared libraries
#
# Arguments:
#	none
#	
# Results:
#
#	Adds the following arguments to configure:
#		--enable-shared=yes|no
#
#	Defines the following vars:
#		STATIC_BUILD	Used for building import/export libraries
#				on Windows.
#
#	Sets the following vars:
#		SHARED_BUILD	Value of 1 or 0
#------------------------------------------------------------------------

AC_DEFUN([SC_ENABLE_SHARED],[
    AC_MSG_CHECKING([how to build libraries])
    AC_ARG_ENABLE(shared,
	[  --enable-shared         build and link with shared libraries [--enable-shared]],
	[tcl_ok=$enableval], [tcl_ok=yes])

    if test "${enable_shared+set}" = set; then
	enableval="$enable_shared"
	tcl_ok=$enableval
    else
	tcl_ok=yes
    fi

    if test "$tcl_ok" = "yes" ; then
	AC_MSG_RESULT([shared])
	SHARED_BUILD=1
    else
	AC_MSG_RESULT([static])
	SHARED_BUILD=0
	AC_DEFINE(STATIC_BUILD)
    fi
])

#------------------------------------------------------------------------
# SC_ENABLE_THREADS --
#
#	Specify if thread support should be enabled
#
# Arguments:
#	none
#	
# Results:
#
#	Adds the following arguments to configure:
#		--enable-threads
#
#	Sets the following vars:
#		THREADS_LIBS	Thread library(s)
#
#	Defines the following vars:
#		TCL_THREADS
#		_REENTRANT
#
#------------------------------------------------------------------------

AC_DEFUN([SC_ENABLE_THREADS],[
    AC_MSG_CHECKING(for building with threads)
    AC_ARG_ENABLE(threads, [  --enable-threads        build with threads],
	[tcl_ok=$enableval], [tcl_ok=no])

    if test "$tcl_ok" = "yes"; then
	TCL_THREADS=1
	AC_DEFINE(TCL_THREADS)
	AC_DEFINE(_REENTRANT)

	case "`uname -s`" in
	    *win32* | *WIN32* | *CYGWIN_NT* | *CYGWIN_98* | *CYGWIN_95*)
		    AC_MSG_RESULT(yes)
		;;
	    *)
		AC_CHECK_LIB(pthread,pthread_mutex_init,tcl_ok=yes,tcl_ok=no)
		if test "$tcl_ok" = "yes"; then
		    # The space is needed
		    THREADS_LIBS=" -lpthread"
		    AC_MSG_RESULT(yes)
		else
		    TCL_THREADS=0
		    AC_MSG_RESULT(no)
		    AC_MSG_WARN("Don t know how to find pthread lib on your system - you must disable thread support or edit the LIBS in the Makefile...")
		fi
		;;
	esac
    else
	TCL_THREADS=0
	AC_MSG_RESULT(no (default))
    fi

])

#------------------------------------------------------------------------
# SC_ENABLE_SYMBOLS --
#
#	Specify if debugging symbols should be used
#
# Arguments:
#	none
#	
#	Requires the following vars to be set:
#		CFLAGS_DEBUG
#		CFLAGS_OPTIMIZE
#		LDFLAGS_DEBUG
#		LDFLAGS_OPTIMIZE
#	
# Results:
#
#	Adds the following arguments to configure:
#		--enable-symbols
#
#	Defines the following vars:
#		CFLAGS_DEFAULT	Sets to CFLAGS_DEBUG if true
#				Sets to CFLAGS_OPTIMIZE if false
#		LDFLAGS_DEFAULT	Sets to LDFLAGS_DEBUG if true
#				Sets to LDFLAGS_OPTIMIZE if false
#		DBGX		Debug library extension
#
#------------------------------------------------------------------------

AC_DEFUN([SC_ENABLE_SYMBOLS],[
    case "`uname -s`" in
	*win32* | *WIN32* | *CYGWIN_NT* | *CYGWIN_98* | *CYGWIN_95*)
	    tcl_dbgx=d
	;;
	*)
	    tcl_dbgx=g
	;;
    esac

    AC_MSG_CHECKING([for build with symbols])
    AC_ARG_ENABLE(symbols, [  --enable-symbols        build with debugging symbols [--disable-symbols]],    [tcl_ok=$enableval], [tcl_ok=no])
    if test "$tcl_ok" = "yes"; then
	CFLAGS_DEFAULT="${CFLAGS_DEBUG}"
	LDFLAGS_DEFAULT="${LDFLAGS_DEBUG}"
	DBGX=${tcl_dbgx}
	TCL_DBGX=${tcl_dbgx}
	AC_MSG_RESULT([yes])
    else
	CFLAGS_DEFAULT="${CFLAGS_OPTIMIZE}"
	LDFLAGS_DEFAULT="${LDFLAGS_OPTIMIZE}"
	DBGX=""
	TCL_DBGX=""
	AC_MSG_RESULT([no])
    fi

    AC_SUBST(TCL_DBGX)
    AC_SUBST(CFLAGS_DEFAULT)
    AC_SUBST(LDFLAGS_DEFAULT)
])

#--------------------------------------------------------------------
# SC_CONFIG_CFLAGS
#
#	Try to determine the proper flags to pass to the compiler
#	for building shared libraries and other such nonsense.
#
# Arguments:
#	none
#
# Results:
#
#	Defines the following vars:
#
#       DL_OBJS -       Name of the object file that implements dynamic
#                       loading for Tcl on this system.
#       DL_LIBS -       Library file(s) to include in tclsh and other base
#                       applications in order for the "load" command to work.
#       LDFLAGS -      Flags to pass to the compiler when linking object
#                       files into an executable application binary such
#                       as tclsh.
#       LD_SEARCH_FLAGS-Flags to pass to ld, such as "-R /usr/local/tcl/lib",
#                       that tell the run-time dynamic linker where to look
#                       for shared libraries such as libtcl.so.  Depends on
#                       the variable LIB_RUNTIME_DIR in the Makefile.
#       MAKE_LIB -      Command to execute to build the Tcl library;
#                       differs depending on whether or not Tcl is being
#                       compiled as a shared library.
#       SHLIB_CFLAGS -  Flags to pass to cc when compiling the components
#                       of a shared library (may request position-independent
#                       code, among other things).
#       SHLIB_LD -      Base command to use for combining object files
#                       into a shared library.
#       SHLIB_LD_LIBS - Dependent libraries for the linker to scan when
#                       creating shared libraries.  This symbol typically
#                       goes at the end of the "ld" commands that build
#                       shared libraries. The value of the symbol is
#                       "${LIBS}" if all of the dependent libraries should
#                       be specified when creating a shared library.  If
#                       dependent libraries should not be specified (as on
#                       SunOS 4.x, where they cause the link to fail, or in
#                       general if Tcl and Tk aren't themselves shared
#                       libraries), then this symbol has an empty string
#                       as its value.
#       SHLIB_SUFFIX -  Suffix to use for the names of dynamically loadable
#                       extensions.  An empty string means we don't know how
#                       to use shared libraries on this platform.
#       TCL_LIB_FILE -  Name of the file that contains the Tcl library, such
#                       as libtcl7.8.so or libtcl7.8.a.
#       TCL_LIB_SUFFIX -Specifies everything that comes after the "libtcl"
#                       in the shared library name, using the $VERSION variable
#                       to put the version in the right place.  This is used
#                       by platforms that need non-standard library names.
#                       Examples:  ${VERSION}.so.1.1 on NetBSD, since it needs
#                       to have a version after the .so, and ${VERSION}.a
#                       on AIX, since the Tcl shared library needs to have
#                       a .a extension whereas shared objects for loadable
#                       extensions have a .so extension.  Defaults to
#                       ${VERSION}${SHLIB_SUFFIX}.
#       TCL_NEEDS_EXP_FILE -
#                       1 means that an export file is needed to link to a
#                       shared library.
#       TCL_EXP_FILE -  The name of the installed export / import file which
#                       should be used to link to the Tcl shared library.
#                       Empty if Tcl is unshared.
#       TCL_BUILD_EXP_FILE -
#                       The name of the built export / import file which
#                       should be used to link to the Tcl shared library.
#                       Empty if Tcl is unshared.
#	CFLAGS_DEBUG -
#			Flags used when running the compiler in debug mode
#	CFLAGS_OPTIMIZE -
#			Flags used when running the compiler in optimize mode
#
#	EXTRA_CFLAGS
#
#	Subst's the following vars:
#		DL_LIBS
#		CFLAGS_DEBUG
#		CFLAGS_OPTIMIZE
#--------------------------------------------------------------------

AC_DEFUN([SC_CONFIG_CFLAGS],[

    # Step 0: Enable 64 bit support?

    AC_MSG_CHECKING([if 64bit support is enabled])
    AC_ARG_ENABLE(64bit,[  --enable-64bit          enable 64bit support],,enableval="no")

    if test "$enableval" = "yes"; then
	AC_MSG_RESULT(Will compile with 64bit support)
	do64bit=yes
    else
	do64bit=no
    fi
    AC_MSG_RESULT($do64bit)
 
    # Step 1: set the variable "system" to hold the name and version number
    # for the system.  This can usually be done via the "uname" command, but
    # there are a few systems, like Next, where this doesn't work.

    AC_MSG_CHECKING([system version (for dynamic loading)])
    if test -f /usr/lib/NextStep/software_version; then
	system=NEXTSTEP-`awk '/3/,/3/' /usr/lib/NextStep/software_version`
    else
	system=`uname -s`-`uname -r`
	if test "$?" -ne 0 ; then
	    AC_MSG_RESULT([unknown (can't find uname command)])
	    system=unknown
	else
	    # Special check for weird MP-RAS system (uname returns weird
	    # results, and the version is kept in special file).
	
	    if test -r /etc/.relid -a "X`uname -n`" = "X`uname -s`" ; then
		system=MP-RAS-`awk '{print $3}' /etc/.relid`
	    fi
	    if test "`uname -s`" = "AIX" ; then
		system=AIX-`uname -v`.`uname -r`
	    fi
	    AC_MSG_RESULT($system)
	fi
    fi

    # Step 2: check for existence of -ldl library.  This is needed because
    # Linux can use either -ldl or -ldld for dynamic loading.

    AC_CHECK_LIB(dl, dlopen, have_dl=yes, have_dl=no)

    # Step 3: set configuration options based on system name and version.

    do64bit_ok=no
    fullSrcDir=`cd $srcdir; pwd`
    EXTRA_CFLAGS=""
    TCL_EXPORT_FILE_SUFFIX=""
    UNSHARED_LIB_SUFFIX=""
    TCL_TRIM_DOTS='`echo ${VERSION} | tr -d .`'
    ECHO_VERSION='`echo ${VERSION}`'
    TCL_LIB_VERSIONS_OK=ok
    CFLAGS_DEBUG=-g
    CFLAGS_OPTIMIZE=-O
    TCL_NEEDS_EXP_FILE=0
    TCL_BUILD_EXP_FILE=""
    TCL_EXP_FILE=""
    STLIB_LD="ar cr"
    case $system in
	AIX-4.[[2-9]])
	    SHLIB_CFLAGS=""
	    SHLIB_LD="$fullSrcDir/ldAix /bin/ld -bhalt:4 -bM:SRE -bE:lib.exp -H512 -T512 -bnoentry"
	    SHLIB_LD_LIBS='${LIBS}'
	    SHLIB_SUFFIX=".so"
	    DL_OBJS="tclLoadDl.o"
	    DL_LIBS="-ldl"
	    LDFLAGS=""
	    LD_SEARCH_FLAGS='-L${LIB_RUNTIME_DIR}'
	    TCL_NEEDS_EXP_FILE=1
	    TCL_EXPORT_FILE_SUFFIX='${VERSION}\$\{DBGX\}.exp'
	    ;;
	AIX-*)
	    SHLIB_CFLAGS=""
	    SHLIB_LD="$fullSrcDir/ldAix /bin/ld -bhalt:4 -bM:SRE -bE:lib.exp -H512 -T512"
	    SHLIB_LD_LIBS='${LIBS}'
	    SHLIB_SUFFIX=".so"
	    DL_OBJS="tclLoadDl.o"
	    LIBOBJS="$LIBOBJS tclLoadAix.o"
	    DL_LIBS="-lld"
	    LDFLAGS=""
	    LD_SEARCH_FLAGS='-L${LIB_RUNTIME_DIR}'
	    TCL_NEEDS_EXP_FILE=1
	    TCL_EXPORT_FILE_SUFFIX='${VERSION}\$\{DBGX\}.exp'
	    ;;
	BSD/OS-2.1*|BSD/OS-3*)
	    SHLIB_CFLAGS=""
	    SHLIB_LD="shlicc -r"
	    SHLIB_LD_LIBS='${LIBS}'
	    SHLIB_SUFFIX=".so"
	    DL_OBJS="tclLoadDl.o"
	    DL_LIBS="-ldl"
	    LDFLAGS=""
	    LD_SEARCH_FLAGS=""
	    ;;
	BSD/OS-4.*)
	    SHLIB_CFLAGS="-export-dynamic -fPIC"
	    SHLIB_LD="cc -shared"
	    SHLIB_LD_LIBS='${LIBS}'
	    SHLIB_SUFFIX=".so"
	    DL_OBJS="tclLoadDl.o"
	    DL_LIBS="-ldl"
	    LDFLAGS="-export-dynamic"
	    LD_SEARCH_FLAGS=""
	    ;;
	*win32*|*WIN32*|CYGWIN_NT*|cygwin_nt*|*CYGWIN_98*|*CYGWIN_95*)
	    CFLAGS_DEBUG="-nologo -Z7 -Od -WX ${runtime}d"
	    CFLAGS_OPTIMIZE="-nologo -Oti -Gs -GD ${runtime}"
	    LDFLAGS_CONSOLE="-subsystem:console"
	    LDFLAGS_WINDOW="-subsystem:windows"
	    LDFLAGS_DEBUG="-debug:full -debugtype:cv"
	    LDFLAGS_OPTIMIZE="-release"
	    EXTRA_CFLAGS="-YX"
	    PATHTYPE=-w
	    STLIB_LD="lib -nologo"
	    SHLIB_LD="link -dll -nologo"
	    SHLIB_LD_LIBS="user32.lib advapi32.lib"
	    RC="rc"
	    ;;
	dgux*)
	    SHLIB_CFLAGS="-K PIC"
	    SHLIB_LD="cc -G"
	    SHLIB_LD_LIBS=""
	    SHLIB_SUFFIX=".so"
	    DL_OBJS="tclLoadDl.o"
	    DL_LIBS="-ldl"
	    LDFLAGS=""
	    LD_SEARCH_FLAGS=""
	    ;;
	HP-UX-*.08.*|HP-UX-*.09.*|HP-UX-*.10.*|HP-UX-*.11.*)
	    SHLIB_SUFFIX=".sl"
	    AC_CHECK_LIB(dld, shl_load, tcl_ok=yes, tcl_ok=no)
	    if test "$tcl_ok" = yes; then
		SHLIB_CFLAGS="+z"
		SHLIB_LD="ld -b"
		SHLIB_LD_LIBS=""
		DL_OBJS="tclLoadShl.o"
		DL_LIBS="-ldld"
		LDFLAGS="-Wl,-E"
		LD_SEARCH_FLAGS='-Wl,+s,+b,${LIB_RUNTIME_DIR}:.'
	    fi
	    ;;
	IRIX-4.*)
	    SHLIB_CFLAGS="-G 0"
	    SHLIB_SUFFIX=".a"
	    SHLIB_LD="echo tclLdAout $CC \{$SHLIB_CFLAGS\} | `pwd`/tclsh -r -G 0"
	    SHLIB_LD_LIBS='${LIBS}'
	    DL_OBJS="tclLoadAout.o"
	    DL_LIBS=""
	    LDFLAGS="-Wl,-D,08000000"
	    LD_SEARCH_FLAGS='-L${LIB_RUNTIME_DIR}'
	    SHARED_LIB_SUFFIX='${VERSION}\$\{DBGX\}.a'
	    ;;
	IRIX-5.*|IRIX-6.*|IRIX64-6.5*)
	    SHLIB_CFLAGS=""
	    SHLIB_LD="ld -n32 -shared -rdata_shared"
	    SHLIB_LD_LIBS=""
	    SHLIB_SUFFIX=".so"
	    DL_OBJS="tclLoadDl.o"
	    DL_LIBS=""
	    LD_SEARCH_FLAGS='-Wl,-rpath,${LIB_RUNTIME_DIR}'
	    if test "$CC" = "gcc" -o `$CC -v 2>&1 | grep -c gcc` != "0" ; then
		EXTRA_CFLAGS="-mabi=n32"
		LDFLAGS="-mabi=n32"
	    else
		case $system in
		    IRIX-6.3)
			# Use to build 6.2 compatible binaries on 6.3.
			EXTRA_CFLAGS="-n32 -D_OLD_TERMIOS"
			;;
		    *)
			EXTRA_CFLAGS="-n32"
			;;
		esac
		LDFLAGS="-n32"
	    fi
	    ;;
	IRIX64-6.*)
	    SHLIB_CFLAGS=""
	    SHLIB_LD="ld -32 -shared -rdata_shared"
	    SHLIB_LD_LIBS='${LIBS}'
	    SHLIB_SUFFIX=".so"
	    DL_OBJS="tclLoadDl.o"
	    DL_LIBS=""
	    LDFLAGS=""
	    LD_SEARCH_FLAGS='-Wl,-rpath,${LIB_RUNTIME_DIR}'
	    ;;
	Linux*)
	    SHLIB_CFLAGS="-fPIC"
	    SHLIB_LD_LIBS='${LIBS}'
	    SHLIB_SUFFIX=".so"

	    # egcs-2.91.66 on Redhat Linux 6.0 generates lots of warnings 
	    # when you inline the string and math operations.  Turn this off to
	    # get rid of the warnings.

	    CFLAGS_OPTIMIZE="${CFLAGS_OPTIMIZE} -D__NO_STRING_INLINES -D__NO_MATH_INLINES"

	    if test "$have_dl" = yes; then
		SHLIB_LD="${CC} -shared"
		DL_OBJS="tclLoadDl.o"
		DL_LIBS="-ldl"
		LDFLAGS="-rdynamic"
		LD_SEARCH_FLAGS='-Wl,-rpath,${LIB_RUNTIME_DIR}'
	    else
		AC_CHECK_HEADER(dld.h, [
		    SHLIB_LD="ld -shared"
		    DL_OBJS="tclLoadDld.o"
		    DL_LIBS="-ldld"
		    LDFLAGS=""
		    LD_SEARCH_FLAGS=""])
	    fi
	    if test "`uname -m`" = "alpha" ; then
		EXTRA_CFLAGS="-mieee"
	    fi
	    ;;
	MP-RAS-02*)
	    SHLIB_CFLAGS="-K PIC"
	    SHLIB_LD="cc -G"
	    SHLIB_LD_LIBS=""
	    SHLIB_SUFFIX=".so"
	    DL_OBJS="tclLoadDl.o"
	    DL_LIBS="-ldl"
	    LDFLAGS=""
	    LD_SEARCH_FLAGS=""
	    ;;
	MP-RAS-*)
	    SHLIB_CFLAGS="-K PIC"
	    SHLIB_LD="cc -G"
	    SHLIB_LD_LIBS=""
	    SHLIB_SUFFIX=".so"
	    DL_OBJS="tclLoadDl.o"
	    DL_LIBS="-ldl"
	    LDFLAGS="-Wl,-Bexport"
	    LD_SEARCH_FLAGS=""
	    ;;
	NetBSD-*|FreeBSD-[[12]].*|OpenBSD-*)
	    # Not available on all versions:  check for include file.
	    AC_CHECK_HEADER(dlfcn.h, [
		SHLIB_CFLAGS="-fpic"
		SHLIB_LD="ld -Bshareable -x"
		SHLIB_LD_LIBS=""
		SHLIB_SUFFIX=".so"
		DL_OBJS="tclLoadDl.o"
		DL_LIBS=""
		LDFLAGS=""
		LD_SEARCH_FLAGS=""
		SHARED_LIB_SUFFIX='${TCL_TRIM_DOTS}\$\{DBGX\}.so.1.0'
	    ], [
		SHLIB_CFLAGS=""
		SHLIB_LD="echo tclLdAout $CC \{$SHLIB_CFLAGS\} | `pwd`/tclsh -r"
		SHLIB_LD_LIBS='${LIBS}'
		SHLIB_SUFFIX=".a"
		DL_OBJS="tclLoadAout.o"
		DL_LIBS=""
		LDFLAGS=""
		LD_SEARCH_FLAGS='-L${LIB_RUNTIME_DIR}'
		SHARED_LIB_SUFFIX='${TCL_TRIM_DOTS}\$\{DBGX\}.a'
	    ])

	    # FreeBSD doesn't handle version numbers with dots.

	    UNSHARED_LIB_SUFFIX='${TCL_TRIM_DOTS}\$\{DBGX\}.a'
	    TCL_LIB_VERSIONS_OK=nodots
	    ;;
	FreeBSD-*)
	    # FreeBSD 3.* and greater have ELF.
	    SHLIB_CFLAGS="-fpic"
	    SHLIB_LD="ld -Bshareable -x"
	    SHLIB_LD_LIBS=""
	    SHLIB_SUFFIX=".so"
	    DL_OBJS="tclLoadDl.o"
	    DL_LIBS=""
	    LDFLAGS=""
	    LD_SEARCH_FLAGS=""
	    ;;
	NEXTSTEP-*)
	    SHLIB_CFLAGS=""
	    SHLIB_LD="cc -nostdlib -r"
	    SHLIB_LD_LIBS=""
	    SHLIB_SUFFIX=".so"
	    DL_OBJS="tclLoadNext.o"
	    DL_LIBS=""
	    LDFLAGS=""
	    LD_SEARCH_FLAGS=""
	    ;;
	OS/390-*)
	    CFLAGS_OPTIMIZE=""      # Optimizer is buggy
	    AC_DEFINE(_OE_SOCKETS)  # needed in sys/socket.h
	    ;;      
	OSF1-1.0|OSF1-1.1|OSF1-1.2)
	    # OSF/1 1.[012] from OSF, and derivatives, including Paragon OSF/1
	    SHLIB_CFLAGS=""
	    # Hack: make package name same as library name
	    SHLIB_LD='ld -R -export $@:'
	    SHLIB_LD_LIBS=""
	    SHLIB_SUFFIX=".so"
	    DL_OBJS="tclLoadOSF.o"
	    DL_LIBS=""
	    LDFLAGS=""
	    LD_SEARCH_FLAGS=""
	    ;;
	OSF1-1.*)
	    # OSF/1 1.3 from OSF using ELF, and derivatives, including AD2
	    SHLIB_CFLAGS="-fpic"
	    SHLIB_LD="ld -shared"
	    SHLIB_LD_LIBS=""
	    SHLIB_SUFFIX=".so"
	    DL_OBJS="tclLoadDl.o"
	    DL_LIBS=""
	    LDFLAGS=""
	    LD_SEARCH_FLAGS=""
	    ;;
	OSF1-V*)
	    # Digital OSF/1
	    SHLIB_CFLAGS=""
	    SHLIB_LD='ld -shared -expect_unresolved "*"'
	    SHLIB_LD_LIBS=""
	    SHLIB_SUFFIX=".so"
	    DL_OBJS="tclLoadDl.o"
	    DL_LIBS=""
	    LDFLAGS=""
	    LD_SEARCH_FLAGS='-Wl,-rpath,${LIB_RUNTIME_DIR}'
	    ;;
	RISCos-*)
	    SHLIB_CFLAGS="-G 0"
	    SHLIB_LD="echo tclLdAout $CC \{$SHLIB_CFLAGS\} | `pwd`/tclsh -r -G 0"
	    SHLIB_LD_LIBS='${LIBS}'
	    SHLIB_SUFFIX=".a"
	    DL_OBJS="tclLoadAout.o"
	    DL_LIBS=""
	    LDFLAGS="-Wl,-D,08000000"
	    LD_SEARCH_FLAGS='-L${LIB_RUNTIME_DIR}'
	    ;;
	SCO_SV-3.2*)
	    # Note, dlopen is available only on SCO 3.2.5 and greater.  However,
	    # this test works, since "uname -s" was non-standard in 3.2.4 and
	    # below.
	    SHLIB_CFLAGS="-Kpic -belf"
	    SHLIB_LD="ld -G"
	    SHLIB_LD_LIBS=""
	    SHLIB_SUFFIX=".so"
	    DL_OBJS="tclLoadDl.o"
	    DL_LIBS=""
	    LDFLAGS="-belf -Wl,-Bexport"
	    LD_SEARCH_FLAGS=""
	    ;;
	SINIX*5.4*)
	    SHLIB_CFLAGS="-K PIC"
	    SHLIB_LD="cc -G"
	    SHLIB_LD_LIBS=""
	    SHLIB_SUFFIX=".so"
	    DL_OBJS="tclLoadDl.o"
	    DL_LIBS="-ldl"
	    LDFLAGS=""
	    LD_SEARCH_FLAGS=""
	    ;;
	SunOS-4*)
	    SHLIB_CFLAGS="-PIC"
	    SHLIB_LD="ld"
	    SHLIB_LD_LIBS=""
	    SHLIB_SUFFIX=".so"
	    DL_OBJS="tclLoadDl.o"
	    DL_LIBS="-ldl"
	    LDFLAGS=""
	    LD_SEARCH_FLAGS='-L${LIB_RUNTIME_DIR}'

	    # SunOS can't handle version numbers with dots in them in library
	    # specs, like -ltcl7.5, so use -ltcl75 instead.  Also, it
	    # requires an extra version number at the end of .so file names.
	    # So, the library has to have a name like libtcl75.so.1.0

	    SHARED_LIB_SUFFIX='${TCL_TRIM_DOTS}\$\{DBGX\}.so.1.0'
	    UNSHARED_LIB_SUFFIX='${TCL_TRIM_DOTS}\$\{DBGX\}.a'
	    TCL_LIB_VERSIONS_OK=nodots
	    ;;
	SunOS-5.[[0-6]]*)
	    SHLIB_CFLAGS="-KPIC"
	    SHLIB_LD="/usr/ccs/bin/ld -G -z text"

	    # Note: need the LIBS below, otherwise Tk won't find Tcl's
	    # symbols when dynamically loaded into tclsh.

	    SHLIB_LD_LIBS='${LIBS}'
	    SHLIB_SUFFIX=".so"
	    DL_OBJS="tclLoadDl.o"
	    DL_LIBS="-ldl"
	    LDFLAGS=""
	    LD_SEARCH_FLAGS='-Wl,-R,${LIB_RUNTIME_DIR}'
	    ;;
	SunOS-5*)
	    SHLIB_CFLAGS="-KPIC"
	    SHLIB_LD="/usr/ccs/bin/ld -G -z text"
	    LDFLAGS=""
    
	    do64bit_ok=no
	    if test "$do64bit" = "yes" ; then
	    arch=`isainfo`
	    if test "$arch" = "sparcv9 sparc" ; then
		if test "$CC" != "gcc" -a `$CC -v 2>&1 | grep -c gcc` = "0" ; then
		do64bit_ok=yes
		EXTRA_CFLAGS="-xarch=v9"
		LDFLAGS="-xarch=v9"
		else 
		AC_MSG_WARN("64bit mode not supported using GCC on $system")
		fi
	    else
		AC_MSG_WARN("64bit mode only supported sparcv9 system")
	    fi
	    fi
	    
	    # Note: need the LIBS below, otherwise Tk won't find Tcl's
	    # symbols when dynamically loaded into tclsh.

	    SHLIB_LD_LIBS='${LIBS}'
	    SHLIB_SUFFIX=".so"
	    DL_OBJS="tclLoadDl.o"
	    DL_LIBS="-ldl"
	    if test "$CC" = "gcc" -o `$CC -v 2>&1 | grep -c gcc` != "0" ; then
		LD_SEARCH_FLAGS='-Wl,-R,${LIB_RUNTIME_DIR}'
	    else
		LD_SEARCH_FLAGS='-R ${LIB_RUNTIME_DIR}'
	    fi
	    ;;
	ULTRIX-4.*)
	    SHLIB_CFLAGS="-G 0"
	    SHLIB_SUFFIX=".a"
	    SHLIB_LD="echo tclLdAout $CC \{$SHLIB_CFLAGS\} | `pwd`/tclsh -r -G 0"
	    SHLIB_LD_LIBS='${LIBS}'
	    DL_OBJS="tclLoadAout.o"
	    DL_LIBS=""
	    LDFLAGS="-Wl,-D,08000000"
	    LD_SEARCH_FLAGS='-L${LIB_RUNTIME_DIR}'
	    ;;
	UNIX_SV* | UnixWare-5*)
	    SHLIB_CFLAGS="-KPIC"
	    SHLIB_LD="cc -G"
	    SHLIB_LD_LIBS=""
	    SHLIB_SUFFIX=".so"
	    DL_OBJS="tclLoadDl.o"
	    DL_LIBS="-ldl"
	    # Some UNIX_SV* systems (unixware 1.1.2 for example) have linkers
	    # that don't grok the -Bexport option.  Test that it does.
	    hold_ldflags=$LDFLAGS
	    AC_MSG_CHECKING(for ld accepts -Bexport flag)
	    LDFLAGS="${LDFLAGS} -Wl,-Bexport"
	    AC_TRY_LINK(, [int i;], found=yes, found=no)
	    LDFLAGS=$hold_ldflags
	    AC_MSG_RESULT($found)
	    if test $found = yes; then
	    LDFLAGS="-Wl,-Bexport"
	    else
	    LDFLAGS=""
	    fi
	    LD_SEARCH_FLAGS=""
	    ;;
    esac

    if test "$do64bit" = "yes" -a "$do64bit_ok" = "no" ; then
    AC_MSG_WARN("64bit support being disabled -- not supported on this platform")
    fi

    # Step 4: If pseudo-static linking is in use (see K. B. Kenny, "Dynamic
    # Loading for Tcl -- What Became of It?".  Proc. 2nd Tcl/Tk Workshop,
    # New Orleans, LA, Computerized Processes Unlimited, 1994), then we need
    # to determine which of several header files defines the a.out file
    # format (a.out.h, sys/exec.h, or sys/exec_aout.h).  At present, we
    # support only a file format that is more or less version-7-compatible. 
    # In particular,
    #	- a.out files must begin with `struct exec'.
    #	- the N_TXTOFF on the `struct exec' must compute the seek address
    #	  of the text segment
    #	- The `struct exec' must contain a_magic, a_text, a_data, a_bss
    #	  and a_entry fields.
    # The following compilation should succeed if and only if either sys/exec.h
    # or a.out.h is usable for the purpose.
    #
    # Note that the modified COFF format used on MIPS Ultrix 4.x is usable; the
    # `struct exec' includes a second header that contains information that
    # duplicates the v7 fields that are needed.

    if test "x$DL_OBJS" = "xtclLoadAout.o" ; then
	AC_MSG_CHECKING(sys/exec.h)
	AC_TRY_COMPILE([#include <sys/exec.h>],[
	    struct exec foo;
	    unsigned long seek;
	    int flag;
#if defined(__mips) || defined(mips)
	    seek = N_TXTOFF (foo.ex_f, foo.ex_o);
#else
	    seek = N_TXTOFF (foo);
#endif
	    flag = (foo.a_magic == OMAGIC);
	    return foo.a_text + foo.a_data + foo.a_bss + foo.a_entry;
    ], tcl_ok=usable, tcl_ok=unusable)
	AC_MSG_RESULT($tcl_ok)
	if test $tcl_ok = usable; then
	    AC_DEFINE(USE_SYS_EXEC_H)
	else
	    AC_MSG_CHECKING(a.out.h)
	    AC_TRY_COMPILE([#include <a.out.h>],[
		struct exec foo;
		unsigned long seek;
		int flag;
#if defined(__mips) || defined(mips)
		seek = N_TXTOFF (foo.ex_f, foo.ex_o);
#else
		seek = N_TXTOFF (foo);
#endif
		flag = (foo.a_magic == OMAGIC);
		return foo.a_text + foo.a_data + foo.a_bss + foo.a_entry;
	    ], tcl_ok=usable, tcl_ok=unusable)
	    AC_MSG_RESULT($tcl_ok)
	    if test $tcl_ok = usable; then
		AC_DEFINE(USE_A_OUT_H)
	    else
		AC_MSG_CHECKING(sys/exec_aout.h)
		AC_TRY_COMPILE([#include <sys/exec_aout.h>],[
		    struct exec foo;
		    unsigned long seek;
		    int flag;
#if defined(__mips) || defined(mips)
		    seek = N_TXTOFF (foo.ex_f, foo.ex_o);
#else
		    seek = N_TXTOFF (foo);
#endif
		    flag = (foo.a_midmag == OMAGIC);
		    return foo.a_text + foo.a_data + foo.a_bss + foo.a_entry;
		], tcl_ok=usable, tcl_ok=unusable)
		AC_MSG_RESULT($tcl_ok)
		if test $tcl_ok = usable; then
		    AC_DEFINE(USE_SYS_EXEC_AOUT_H)
		else
		    DL_OBJS=""
		fi
	    fi
	fi
    fi

    # Step 5: disable dynamic loading if requested via a command-line switch.

    AC_ARG_ENABLE(load, [  --disable-load          disallow dynamic loading and "load" command],
	[tcl_ok=$enableval], [tcl_ok=yes])
    if test "$tcl_ok" = "no"; then
	DL_OBJS=""
    fi

    if test "x$DL_OBJS" != "x" ; then
	BUILD_DLTEST="\$(DLTEST_TARGETS)"
    else
	echo "Can't figure out how to do dynamic loading or shared libraries"
	echo "on this system."
	SHLIB_CFLAGS=""
	SHLIB_LD=""
	SHLIB_SUFFIX=""
	DL_OBJS="tclLoadNone.o"
	DL_LIBS=""
	LDFLAGS=""
	LD_SEARCH_FLAGS=""
	BUILD_DLTEST=""
    fi

    # If we're running gcc, then change the C flags for compiling shared
    # libraries to the right flags for gcc, instead of those for the
    # standard manufacturer compiler.

    if test "$DL_OBJS" != "tclLoadNone.o" ; then
	if test "$CC" = "gcc" -o `$CC -v 2>&1 | grep -c gcc` != "0" ; then
	    case $system in
		AIX-*)
		    ;;
		BSD/OS*)
		    ;;
		IRIX*)
		    ;;
		NetBSD-*|FreeBSD-*|OpenBSD-*)
		    ;;
		RISCos-*)
		    ;;
		ULTRIX-4.*)
		    ;;
		*)
		    SHLIB_CFLAGS="-fPIC"
		    ;;
	    esac
	fi
    fi

    if test "$SHARED_LIB_SUFFIX" = "" ; then
	SHARED_LIB_SUFFIX='${VERSION}\$\{DBGX\}${SHLIB_SUFFIX}'
    fi
    if test "$UNSHARED_LIB_SUFFIX" = "" ; then
	UNSHARED_LIB_SUFFIX='${VERSION}\$\{DBGX\}.a'
    fi

    AC_SUBST(STLIB_LD)
    AC_SUBST(SHLIB_LD)
    AC_SUBST(SHLIB_CFLAGS)
    AC_SUBST(SHLIB_LDFLAGS)
    AC_SUBST(DL_LIBS)
    AC_SUBST(CFLAGS_DEBUG)
    AC_SUBST(CFLAGS_OPTIMIZE)
    AC_SUBST(LDFLAGS_DEBUG)
    AC_SUBST(LDFLAGS_OPTIMIZE)
])

#--------------------------------------------------------------------
# SC_SERIAL_PORT
#
#	Determine which interface to use to talk to the serial port.
#	Note that #include lines must begin in leftmost column for
#	some compilers to recognize them as preprocessor directives.
#
# Arguments:
#	none
#	
# Results:
#
#	Defines only one of the following vars:
#		USE_TERMIOS
#		USE_TERMIO
#		USE_SGTTY
#
#--------------------------------------------------------------------

AC_DEFUN([SC_SERIAL_PORT],[
    AC_MSG_CHECKING([termios vs. termio vs. sgtty])

    AC_TRY_RUN([
#include <termios.h>

main()
{
    struct termios t;
    if (tcgetattr(0, &t) == 0) {
	cfsetospeed(&t, 0);
	t.c_cflag |= PARENB | PARODD | CSIZE | CSTOPB;
	return 0;
    }
    return 1;
}], tk_ok=termios, tk_ok=no, tk_ok=no)

    if test $tk_ok = termios; then
	AC_DEFINE(USE_TERMIOS)
    else
	AC_TRY_RUN([
#include <termio.h>

main()
{
    struct termio t;
    if (ioctl(0, TCGETA, &t) == 0) {
	t.c_cflag |= CBAUD | PARENB | PARODD | CSIZE | CSTOPB;
	return 0;
    }
    return 1;
    }], tk_ok=termio, tk_ok=no, tk_ok=no)

    if test $tk_ok = termio; then
	AC_DEFINE(USE_TERMIO)
    else
	AC_TRY_RUN([
#include <sgtty.h>

main()
{
    struct sgttyb t;
    if (ioctl(0, TIOCGETP, &t) == 0) {
	t.sg_ospeed = 0;
	t.sg_flags |= ODDP | EVENP | RAW;
	return 0;
    }
    return 1;
}], tk_ok=sgtty, tk_ok=none, tk_ok=none)
    if test $tk_ok = sgtty; then
	AC_DEFINE(USE_SGTTY)
    fi
    fi
    fi
    AC_MSG_RESULT($tk_ok)
])

#--------------------------------------------------------------------
# SC_MISSING_POSIX_HEADERS
#
#	Supply substitutes for missing POSIX header files.  Special
#	notes:
#	    - stdlib.h doesn't define strtol, strtoul, or
#	      strtod insome versions of SunOS
#	    - some versions of string.h don't declare procedures such
#	      as strstr
#
# Arguments:
#	none
#	
# Results:
#
#	Defines some of the following vars:
#		NO_DIRENT_H
#		NO_ERRNO_H
#		NO_VALUES_H
#		NO_LIMITS_H
#		NO_STDLIB_H
#		NO_STRING_H
#		NO_SYS_WAIT_H
#		NO_DLFCN_H
#		HAVE_UNISTD_H
#		HAVE_SYS_PARAM_H
#
#		HAVE_STRING_H ?
#
#--------------------------------------------------------------------

AC_DEFUN([SC_MISSING_POSIX_HEADERS],[

    AC_MSG_CHECKING(dirent.h)
    AC_TRY_LINK([#include <sys/types.h>
#include <dirent.h>], [
#ifndef _POSIX_SOURCE
#   ifdef __Lynx__
	/*
	 * Generate compilation error to make the test fail:  Lynx headers
	 * are only valid if really in the POSIX environment.
	 */

	missing_procedure();
#   endif
#endif
DIR *d;
struct dirent *entryPtr;
char *p;
d = opendir("foobar");
entryPtr = readdir(d);
p = entryPtr->d_name;
closedir(d);
], tcl_ok=yes, tcl_ok=no)

    if test $tcl_ok = no; then
	AC_DEFINE(NO_DIRENT_H)
    fi

    AC_MSG_RESULT($tcl_ok)
    AC_CHECK_HEADER(errno.h, , AC_DEFINE(NO_ERRNO_H))
    AC_CHECK_HEADER(float.h, , AC_DEFINE(NO_FLOAT_H))
    AC_CHECK_HEADER(values.h, , AC_DEFINE(NO_VALUES_H))
    AC_CHECK_HEADER(limits.h, , AC_DEFINE(NO_LIMITS_H))
    AC_CHECK_HEADER(stdlib.h, tcl_ok=1, tcl_ok=0)
    AC_EGREP_HEADER(strtol, stdlib.h, , tcl_ok=0)
    AC_EGREP_HEADER(strtoul, stdlib.h, , tcl_ok=0)
    AC_EGREP_HEADER(strtod, stdlib.h, , tcl_ok=0)
    if test $tcl_ok = 0; then
	AC_DEFINE(NO_STDLIB_H)
    fi
    AC_CHECK_HEADER(string.h, tcl_ok=1, tcl_ok=0)
    AC_EGREP_HEADER(strstr, string.h, , tcl_ok=0)
    AC_EGREP_HEADER(strerror, string.h, , tcl_ok=0)

    # See also memmove check below for a place where NO_STRING_H can be
    # set and why.

    if test $tcl_ok = 0; then
	AC_DEFINE(NO_STRING_H)
    fi

    AC_CHECK_HEADER(sys/wait.h, , AC_DEFINE(NO_SYS_WAIT_H))
    AC_CHECK_HEADER(dlfcn.h, , AC_DEFINE(NO_DLFCN_H))

    # OS/390 lacks sys/param.h (and doesn't need it, by chance).

    AC_HAVE_HEADERS(unistd.h sys/param.h)

])

#--------------------------------------------------------------------
# SC_PATH_X
#
#	Locate the X11 header files and the X11 library archive.  Try
#	the ac_path_x macro first, but if it doesn't find the X stuff
#	(e.g. because there's no xmkmf program) then check through
#	a list of possible directories.  Under some conditions the
#	autoconf macro will return an include directory that contains
#	no include files, so double-check its result just to be safe.
#
# Arguments:
#	none
#	
# Results:
#
#	Sets the the following vars:
#		XINCLUDES
#		XLIBSW
#
#--------------------------------------------------------------------

AC_DEFUN([SC_PATH_X],[
    AC_PATH_X
    not_really_there=""
    if test "$no_x" = ""; then
	if test "$x_includes" = ""; then
	    AC_TRY_CPP([#include <X11/XIntrinsic.h>], , not_really_there="yes")
	else
	    if test ! -r $x_includes/X11/Intrinsic.h; then
		not_really_there="yes"
	    fi
	fi
    fi
    if test "$no_x" = "yes" -o "$not_really_there" = "yes"; then
	AC_MSG_CHECKING(for X11 header files)
	XINCLUDES="# no special path needed"
	AC_TRY_CPP([#include <X11/Intrinsic.h>], , XINCLUDES="nope")
	if test "$XINCLUDES" = nope; then
	    dirs="/usr/unsupported/include /usr/local/include /usr/X386/include /usr/X11R6/include /usr/X11R5/include /usr/include/X11R5 /usr/include/X11R4 /usr/openwin/include /usr/X11/include /usr/sww/include"
	    for i in $dirs ; do
		if test -r $i/X11/Intrinsic.h; then
		    AC_MSG_RESULT($i)
		    XINCLUDES=" -I$i"
		    break
		fi
	    done
	fi
    else
	if test "$x_includes" != ""; then
	    XINCLUDES=-I$x_includes
	else
	    XINCLUDES="# no special path needed"
	fi
    fi
    if test "$XINCLUDES" = nope; then
	AC_MSG_RESULT(couldn't find any!)
	XINCLUDES="# no include files found"
    fi

    if test "$no_x" = yes; then
	AC_MSG_CHECKING(for X11 libraries)
	XLIBSW=nope
	dirs="/usr/unsupported/lib /usr/local/lib /usr/X386/lib /usr/X11R6/lib /usr/X11R5/lib /usr/lib/X11R5 /usr/lib/X11R4 /usr/openwin/lib /usr/X11/lib /usr/sww/X11/lib"
	for i in $dirs ; do
	    if test -r $i/libX11.a -o -r $i/libX11.so -o -r $i/libX11.sl; then
		AC_MSG_RESULT($i)
		XLIBSW="-L$i -lX11"
		x_libraries="$i"
		break
	    fi
	done
    else
	if test "$x_libraries" = ""; then
	    XLIBSW=-lX11
	else
	    XLIBSW="-L$x_libraries -lX11"
	fi
    fi
    if test "$XLIBSW" = nope ; then
	AC_CHECK_LIB(Xwindow, XCreateWindow, XLIBSW=-lXwindow)
    fi
    if test "$XLIBSW" = nope ; then
	AC_MSG_RESULT(couldn't find any!  Using -lX11.)
	XLIBSW=-lX11
    fi
])
#--------------------------------------------------------------------
# SC_BLOCKING_STYLE
#
#	The statements below check for systems where POSIX-style
#	non-blocking I/O (O_NONBLOCK) doesn't work or is unimplemented. 
#	On these systems (mostly older ones), use the old BSD-style
#	FIONBIO approach instead.
#
# Arguments:
#	none
#	
# Results:
#
#	Defines some of the following vars:
#		HAVE_SYS_IOCTL_H
#		HAVE_SYS_FILIO_H
#		USE_FIONBIO
#		O_NONBLOCK
#
#--------------------------------------------------------------------

AC_DEFUN([SC_BLOCKING_STYLE],[
    AC_CHECK_HEADERS(sys/ioctl.h)
    AC_CHECK_HEADERS(sys/filio.h)
    AC_MSG_CHECKING([FIONBIO vs. O_NONBLOCK for nonblocking I/O])
    if test -f /usr/lib/NextStep/software_version; then
	system=NEXTSTEP-`awk '/3/,/3/' /usr/lib/NextStep/software_version`
    else
	system=`uname -s`-`uname -r`
	if test "$?" -ne 0 ; then
	    system=unknown
	else
	    # Special check for weird MP-RAS system (uname returns weird
	    # results, and the version is kept in special file).
	
	    if test -r /etc/.relid -a "X`uname -n`" = "X`uname -s`" ; then
		system=MP-RAS-`awk '{print $3}' /etc/.relid`
	    fi
	    if test "`uname -s`" = "AIX" ; then
		system=AIX-`uname -v`.`uname -r`
	    fi
	fi
    fi
    case $system in
	# There used to be code here to use FIONBIO under AIX.  However, it
	# was reported that FIONBIO doesn't work under AIX 3.2.5.  Since
	# using O_NONBLOCK seems fine under AIX 4.*, I removed the FIONBIO
	# code (JO, 5/31/97).

	OSF*)
	    AC_DEFINE(USE_FIONBIO)
	    AC_MSG_RESULT(FIONBIO)
	    ;;
	SunOS-4*)
	    AC_DEFINE(USE_FIONBIO)
	    AC_MSG_RESULT(FIONBIO)
	    ;;
	ULTRIX-4.*)
	    AC_DEFINE(USE_FIONBIO)
	    AC_MSG_RESULT(FIONBIO)
	    ;;
	*)
	    AC_MSG_RESULT(O_NONBLOCK)
	    ;;
    esac
])

#--------------------------------------------------------------------
# SC_HAVE_VFORK
#
#	Check to see whether the system provides a vfork kernel call.
#	If not, then use fork instead.  Also, check for a problem with
#	vforks and signals that can cause core dumps if a vforked child
#	resets a signal handler.  If the problem exists, then use fork
#	instead of vfork.
#
# Arguments:
#	none
#	
# Results:
#
#	Defines some of the following vars:
#		vfork (=fork)
#
#--------------------------------------------------------------------

AC_DEFUN([SC_HAVE_VFORK],[
    AC_TYPE_SIGNAL()
    AC_CHECK_FUNC(vfork, tcl_ok=1, tcl_ok=0)
    if test "$tcl_ok" = 1; then
	AC_MSG_CHECKING([vfork/signal bug]);
	AC_TRY_RUN([
#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>
int gotSignal = 0;
sigProc(sig)
    int sig;
{
    gotSignal = 1;
}
main()
{
    int pid, sts;
    (void) signal(SIGCHLD, sigProc);
    pid = vfork();
    if (pid <  0) {
	exit(1);
    } else if (pid == 0) {
	(void) signal(SIGCHLD, SIG_DFL);
	_exit(0);
    } else {
	(void) wait(&sts);
    }
    exit((gotSignal) ? 0 : 1);
}], tcl_ok=1, tcl_ok=0, tcl_ok=0)

	if test "$tcl_ok" = 1; then
	    AC_MSG_RESULT(ok)
	else
	    AC_MSG_RESULT([buggy, using fork instead])
	fi
    fi
    rm -f core
    if test "$tcl_ok" = 0; then
	AC_DEFINE(vfork, fork)
    fi
])

#--------------------------------------------------------------------
# SC_TIME_HANLDER
#
#	Checks how the system deals with time.h, what time structures
#	are used on the system, and what fields the structures have.
#
# Arguments:
#	none
#	
# Results:
#
#	Defines some of the following vars:
#		USE_DELTA_FOR_TZ
#		HAVE_TM_GMTOFF
#		HAVE_TM_TZADJ
#		HAVE_TIMEZONE_VAR
#
#--------------------------------------------------------------------

AC_DEFUN([SC_TIME_HANDLER],[
    AC_CHECK_HEADERS(sys/time.h)
    AC_HEADER_TIME
    AC_STRUCT_TIMEZONE

    AC_MSG_CHECKING([tm_tzadj in struct tm])
    AC_TRY_COMPILE([#include <time.h>], [struct tm tm; tm.tm_tzadj;],
	    [AC_DEFINE(HAVE_TM_TZADJ)
	    AC_MSG_RESULT(yes)],
	    AC_MSG_RESULT(no))

    AC_MSG_CHECKING([tm_gmtoff in struct tm])
    AC_TRY_COMPILE([#include <time.h>], [struct tm tm; tm.tm_gmtoff;],
	    [AC_DEFINE(HAVE_TM_GMTOFF)
	    AC_MSG_RESULT(yes)],
	    AC_MSG_RESULT(no))

    #
    # Its important to include time.h in this check, as some systems
    # (like convex) have timezone functions, etc.
    #
    have_timezone=no
    AC_MSG_CHECKING([long timezone variable])
    AC_TRY_COMPILE([#include <time.h>],
	    [extern long timezone;
	    timezone += 1;
	    exit (0);],
	    [have_timezone=yes
	    AC_DEFINE(HAVE_TIMEZONE_VAR)
	    AC_MSG_RESULT(yes)],
	    AC_MSG_RESULT(no))

    #
    # On some systems (eg IRIX 6.2), timezone is a time_t and not a long.
    #
    if test "$have_timezone" = no; then
    AC_MSG_CHECKING([time_t timezone variable])
    AC_TRY_COMPILE([#include <time.h>],
	    [extern time_t timezone;
	    timezone += 1;
	    exit (0);],
	    [AC_DEFINE(HAVE_TIMEZONE_VAR)
	    AC_MSG_RESULT(yes)],
	    AC_MSG_RESULT(no))
    fi

    #
    # AIX does not have a timezone field in struct tm. When the AIX bsd
    # library is used, the timezone global and the gettimeofday methods are
    # to be avoided for timezone deduction instead, we deduce the timezone
    # by comparing the localtime result on a known GMT value.
    #

    if test "`uname -s`" = "AIX" ; then
	AC_CHECK_LIB(bsd, gettimeofday, libbsd=yes)
	if test $libbsd = yes; then
	    AC_DEFINE(USE_DELTA_FOR_TZ)
	fi
    fi
])

#--------------------------------------------------------------------
# SC_BUGGY_STRTOD
#
#	Under Solaris 2.4, strtod returns the wrong value for the
#	terminating character under some conditions.  Check for this
#	and if the problem exists use a substitute procedure
#	"fixstrtod" (provided by Tcl) that corrects the error.
#
# Arguments:
#	none
#	
# Results:
#
#	Might defines some of the following vars:
#		strtod (=fixstrtod)
#
#--------------------------------------------------------------------

AC_DEFUN([SC_BUGGY_STRTOD],[
    AC_CHECK_FUNC(strtod, tk_strtod=1, tk_strtod=0)
    if test "$tk_strtod" = 1; then
	AC_MSG_CHECKING([for Solaris 2.4 strtod bug])
	AC_TRY_RUN([
	    extern double strtod();
	    int main()
	    {
		char *string = "NaN";
		char *term;
		strtod(string, &term);
		if ((term != string) && (term[-1] == 0)) {
		    exit(1);
		}
		exit(0);
	    }], tk_ok=1, tk_ok=0, tk_ok=0)
	if test "$tk_ok" = 1; then
	    AC_MSG_RESULT(ok)
	else
	    AC_MSG_RESULT(buggy)
	    AC_DEFINE(strtod, fixstrtod)
	fi
    fi
])

#--------------------------------------------------------------------
# SC_TCL_LINK_LIBS
#
#	Search for the libraries needed to link the Tcl shell.
#	Things like the math library (-lm) and socket stuff (-lsocket vs.
#	-lnsl) are dealt with here.
#
# Arguments:
#	Requires the following vars to be set in the Makefile:
#		DL_LIBS
#		LIBS
#		MATH_LIBS
#	
# Results:
#
#	Subst's the following var:
#		TCL_LIBS
#		MATH_LIBS
#
#	Might append to the following vars:
#		LIBS
#
#	Might define the following vars:
#		HAVE_NET_ERRNO_H
#
#--------------------------------------------------------------------

AC_DEFUN([SC_TCL_LINK_LIBS],[
    #--------------------------------------------------------------------
    # On a few very rare systems, all of the libm.a stuff is
    # already in libc.a.  Set compiler flags accordingly.
    # Also, Linux requires the "ieee" library for math to work
    # right (and it must appear before "-lm").
    #--------------------------------------------------------------------

    AC_CHECK_FUNC(sin, MATH_LIBS="", MATH_LIBS="-lm")
    AC_CHECK_LIB(ieee, main, [MATH_LIBS="-lieee $MATH_LIBS"])

    #--------------------------------------------------------------------
    # On AIX systems, libbsd.a has to be linked in to support
    # non-blocking file IO.  This library has to be linked in after
    # the MATH_LIBS or it breaks the pow() function.  The way to
    # insure proper sequencing, is to add it to the tail of MATH_LIBS.
    # This library also supplies gettimeofday.
    #--------------------------------------------------------------------

    libbsd=no
    if test "`uname -s`" = "AIX" ; then
	AC_CHECK_LIB(bsd, gettimeofday, libbsd=yes)
	if test $libbsd = yes; then
	    MATH_LIBS="$MATH_LIBS -lbsd"
	fi
    fi


    #--------------------------------------------------------------------
    # Interactive UNIX requires -linet instead of -lsocket, plus it
    # needs net/errno.h to define the socket-related error codes.
    #--------------------------------------------------------------------

    AC_CHECK_LIB(inet, main, [LIBS="$LIBS -linet"])
    AC_CHECK_HEADER(net/errno.h, AC_DEFINE(HAVE_NET_ERRNO_H))

    #--------------------------------------------------------------------
    #	Check for the existence of the -lsocket and -lnsl libraries.
    #	The order here is important, so that they end up in the right
    #	order in the command line generated by make.  Here are some
    #	special considerations:
    #	1. Use "connect" and "accept" to check for -lsocket, and
    #	   "gethostbyname" to check for -lnsl.
    #	2. Use each function name only once:  can't redo a check because
    #	   autoconf caches the results of the last check and won't redo it.
    #	3. Use -lnsl and -lsocket only if they supply procedures that
    #	   aren't already present in the normal libraries.  This is because
    #	   IRIX 5.2 has libraries, but they aren't needed and they're
    #	   bogus:  they goof up name resolution if used.
    #	4. On some SVR4 systems, can't use -lsocket without -lnsl too.
    #	   To get around this problem, check for both libraries together
    #	   if -lsocket doesn't work by itself.
    #--------------------------------------------------------------------

    tcl_checkBoth=0
    AC_CHECK_FUNC(connect, tcl_checkSocket=0, tcl_checkSocket=1)
    if test "$tcl_checkSocket" = 1; then
	AC_CHECK_LIB(socket, main, LIBS="$LIBS -lsocket", tcl_checkBoth=1)
    fi
    if test "$tcl_checkBoth" = 1; then
	tk_oldLibs=$LIBS
	LIBS="$LIBS -lsocket -lnsl"
	AC_CHECK_FUNC(accept, tcl_checkNsl=0, [LIBS=$tk_oldLibs])
    fi
    AC_CHECK_FUNC(gethostbyname, , AC_CHECK_LIB(nsl, main,
	    [LIBS="$LIBS -lnsl"]))
    
    # Don't perform the eval of the libraries here because DL_LIBS
    # won't be set until we call SC_CONFIG_CFLAGS

    TCL_LIBS='${DL_LIBS} ${LIBS} ${MATH_LIBS}'
    AC_SUBST(TCL_LIBS)
    AC_SUBST(MATH_LIBS)
])

#------------------------------------------------------------------------
# SC_MAKE_LIB --
#
#	Generate a line that can be used to build a shared/unshared library
#	in a platform independent manner.
#
# Arguments:
#	none
#
#	Requires:
#
# Results:
#
#	Defines the following vars:
#		MAKE_LIB	Makefile rule for building a library
#		MAKE_SHARED_LIB	Makefile rule for building a shared library
#		MAKE_UNSHARED_LIB	Makefile rule for building a static
#				library
#------------------------------------------------------------------------

AC_DEFUN([SC_MAKE_LIB],[
    case "`uname -s`" in
	*win32* | *WIN32* | *CYGWIN_NT* |*CYGWIN_98*|*CYGWIN_95*)
	    if test "${CC-cc}" = "cl"; then
		MAKE_STATIC_LIB="\${STLIB_LD} -out:\[$]@ \$(\[$]@_OBJECTS) "
		MAKE_SHARED_LIB="\${SHLIB_LD} \${SHLIB_LDFLAGS} \${SHLIB_LD_LIBS} \$(LDFLAGS) -out:\[$]@ \$(\[$]@_OBJECTS) "
	    fi
	    ;;
	*)
	    MAKE_STATIC_LIB="\${STLIB_LD} \[$]@ \$(\[$]@_OBJECTS)"
	    MAKE_SHARED_LIB="\${SHLIB_LD} -o \[$]@ \$(\[$]@_OBJECTS) \${SHLIB_LDFLAGS} \${SHLIB_LD_LIBS}"
	    ;;
    esac

    if test "${SHARED_BUILD}" = "1" ; then
	MAKE_LIB=${MAKE_SHARED_LIB}
    else
	MAKE_LIB=${MAKE_STATIC_LIB}
    fi

    AC_SUBST(MAKE_LIB)
    AC_SUBST(MAKE_SHARED_LIB)
    AC_SUBST(MAKE_STATIC_LIB)
])

#------------------------------------------------------------------------
# SC_LIB_SPEC --
#
#	Compute the name of an existing object library located in libdir
#	from the given base name and produce the appropriate linker flags.
#
# Arguments:
#	basename	The base name of the library without version
#			numbers, extensions, or "lib" prefixes.
#	extra_dir	Extra directory in which to search for the
#			library.  This location is used first, then
#			$prefix/$exec-prefix, then some defaults.
#
# Requires:
#	CYGPATH		command used to generate native style paths
#
# Results:
#
#	Defines the following vars:
#		${basename}_LIB_NAME	The computed library name.
#		${basename}_LIB_SPEC	The computed linker flags.
#------------------------------------------------------------------------

AC_DEFUN([SC_LIB_SPEC],[
    AC_MSG_CHECKING(for $1 library)

    # Look in exec-prefix and prefix for the library.  If neither of
    # these were specified, look in libdir.  It doesn't matter if libdir
    # wasn't specified since a search in the unspecified directory will
    # fail (NONE/lib)

    if test x"${exec_prefix}" != x"NONE" ; then
	sc_lib_name_dir="${exec_prefix}/lib"
    elif test x"${prefix}" != "NONE" ; then
	sc_lib_name_dir="${prefix}/lib"
    else
	eval "sc_lib_name_dir=${libdir}"
    fi

    if test x"$2" != x ; then
	sc_extra_lib_dir=$2
    else
	sc_extra_lib_dir=NONE
    fi

    for i in \
	    `ls -dr ${sc_extra_lib_dir}/$1[[0-9]]*.lib 2>/dev/null ` \
	    `ls -dr ${sc_extra_lib_dir}/lib$1[[0-9]]* 2>/dev/null ` \
	    `ls -dr ${sc_lib_name_dir}/$1[[0-9]]*.lib 2>/dev/null ` \
	    `ls -dr ${sc_lib_name_dir}/lib$1[[0-9]]* 2>/dev/null ` \
	    `ls -dr /usr/lib/$1[[0-9]]*.lib 2>/dev/null ` \
	    `ls -dr /usr/lib/lib$1[[0-9]]* 2>/dev/null ` \
	    `ls -dr /usr/local/lib/$1[[0-9]]*.lib 2>/dev/null ` \
	    `ls -dr /usr/local/lib/lib$1[[0-9]]* 2>/dev/null ` ; do
	if test -f "$i" ; then

	    sc_lib_name_dir=`dirname $i`
	    $1_LIB_NAME=`basename $i`
	    $1_LIB_PATH_NAME=$i
	    break
	fi
    done

    case "`uname -s`" in
	*win32* | *WIN32* | *CYGWIN_NT* |*CYGWIN_98*|*CYGWIN_95*)
	    $1_LIB_SPEC=\"`${CYGPATH} ${$1_LIB_PATH_NAME}`\"
	    ;;
	*)
	    # Strip off the leading "lib" and trailing ".a" or ".so"

	    sc_lib_name_lib=`echo ${$1_LIB_NAME}|sed -e 's/^lib//' -e 's/\.[[^.]]*$//'`
	    $1_LIB_SPEC="-L${sc_lib_name_dir} -l${sc_lib_name_lib}"
	    ;;
    esac
    if test "x$1_LIB_NAME" = x ; then
	AC_MSG_ERROR(not found)
    else
	AC_MSG_RESULT(${$1_LIB_SPEC})
    fi
])

#------------------------------------------------------------------------
# SC_PRIVATE_TCL_HEADERS --
#
#	Locate the private Tcl include files
#
# Arguments:
#
#	Requires:
#		TCL_SRC_DIR	Assumes that SC_LOAD_TCLCONFIG has
#				 already been called.
#
# Results:
#
#	Substs the following vars:
#		TCL_TOP_DIR_NATIVE
#		TCL_GENERIC_DIR_NATIVE
#		TCL_UNIX_DIR_NATIVE
#		TCL_WIN_DIR_NATIVE
#		TCL_BMAP_DIR_NATIVE
#		TCL_TOOL_DIR_NATIVE
#		TCL_PLATFORM_DIR_NATIVE
#		TCL_BIN_DIR_NATIVE
#		TCL_INCLUDES
#------------------------------------------------------------------------

AC_DEFUN([SC_PRIVATE_TCL_HEADERS],[
    AC_MSG_CHECKING(for Tcl private include files)

    case "`uname -s`" in
	*win32* | *WIN32* | *CYGWIN_NT* |*CYGWIN_98*|*CYGWIN_95*)
	    TCL_TOP_DIR_NATIVE=\"`${CYGPATH} ${TCL_SRC_DIR}`\"
	    TCL_GENERIC_DIR_NATIVE=\"`${CYGPATH} ${TCL_SRC_DIR}/generic`\"
	    TCL_UNIX_DIR_NATIVE=\"`${CYGPATH} ${TCL_SRC_DIR}/unix`\"
	    TCL_WIN_DIR_NATIVE=\"`${CYGPATH} ${TCL_SRC_DIR}/win`\"
	    TCL_BMAP_DIR_NATIVE=\"`${CYGPATH} ${TCL_SRC_DIR}/bitmaps`\"
	    TCL_TOOL_DIR_NATIVE=\"`${CYGPATH} ${TCL_SRC_DIR}/tools`\"
	    TCL_COMPAT_DIR_NATIVE=\"`${CYGPATH} ${TCL_SRC_DIR}/compat`\"
	    TCL_PLATFORM_DIR_NATIVE=${TCL_WIN_DIR_NATIVE}
	;;
	*)
	    TCL_TOP_DIR_NATIVE='$(TCL_SRC_DIR)'
	    TCL_GENERIC_DIR_NATIVE='$(TCL_TOP_DIR_NATIVE)/generic'
	    TCL_UNIX_DIR_NATIVE='$(TCL_TOP_DIR_NATIVE)/unix'
	    TCL_WIN_DIR_NATIVE='$(TCL_TOP_DIR_NATIVE)/win'
	    TCL_BMAP_DIR_NATIVE='$(TCL_TOP_DIR_NATIVE)/bitmaps'
	    TCL_TOOL_DIR_NATIVE='$(TCL_TOP_DIR_NATIVE)/tools'
	    TCL_COMPAT_DIR_NATIVE='$(TCL_TOP_DIR_NATIVE)/compat'
	    TCL_PLATFORM_DIR_NATIVE=${TCL_UNIX_DIR_NATIVE}
	;;
    esac

    AC_SUBST(TCL_TOP_DIR_NATIVE)
    AC_SUBST(TCL_GENERIC_DIR_NATIVE)
    AC_SUBST(TCL_UNIX_DIR_NATIVE)
    AC_SUBST(TCL_WIN_DIR_NATIVE)
    AC_SUBST(TCL_BMAP_DIR_NATIVE)
    AC_SUBST(TCL_TOOL_DIR_NATIVE)
    AC_SUBST(TCL_PLATFORM_DIR_NATIVE)

    TCL_INCLUDES="-I${TCL_GENERIC_DIR_NATIVE} -I${TCL_PLATFORM_DIR_NATIVE}"
    AC_SUBST(TCL_INCLUDES)
    AC_MSG_RESULT(Using srcdir found in tclConfig.sh: ${TCL_SRC_DIR})
])

#------------------------------------------------------------------------
# SC_PUBLIC_TCL_HEADERS --
#
#	Locate the installed public Tcl header files
#
# Arguments:
#	None.
#
# Requires:
#	CYGPATH must be set
#
# Results:
#
#	Adds a --with-tclinclude switch to configure.
#	Result is cached.
#
#	Substs the following vars:
#		TCL_INCLUDES
#------------------------------------------------------------------------

AC_DEFUN([SC_PUBLIC_TCL_HEADERS],[
    AC_MSG_CHECKING(for Tcl public headers)

    AC_ARG_WITH(tclinclude, [ --with-tclinclude      directory containing the public Tcl header files.], with_tclinclude=${withval})

    if test x"${with_tclinclude}" != x ; then
	if test -f "${with_tclinclude}/tcl.h" ; then
	    ac_cv_c_tclh=${with_tclinclude}
	else
	    AC_MSG_ERROR([${with_tclinclude} directory does not contain Tcl public header file tcl.h])
	fi
    else
	AC_CACHE_VAL(ac_cv_c_tclh, [
	    # Use the value from --with-tclinclude, if it was given

	    if test x"${with_tclinclude}" != x ; then
		ac_cv_c_tclh=${with_tclinclude}
	    else
		# Check in the includedir, if --prefix was specified

		eval "temp_includedir=${includedir}"
		for i in \
			`ls -d ${temp_includedir} 2>/dev/null` \
			/usr/local/include /usr/include ; do
		    if test -f "$i/tcl.h" ; then
			ac_cv_c_tclh=$i
			break
		    fi
		done
	    fi
	])
    fi

    # Print a message based on how we determined the include path

    if test x"${ac_cv_c_tclh}" = x ; then
	AC_MSG_ERROR(tcl.h not found.  Please specify its location with --with-tclinclude)
    else
	AC_MSG_RESULT(${ac_cv_c_tclh})
    fi

    # Convert to a native path and substitute into the output files.

    INCLUDE_DIR_NATIVE=`${CYGPATH} ${ac_cv_c_tclh}`

    TCL_INCLUDES=-I\"${INCLUDE_DIR_NATIVE}\"

    AC_SUBST(TCL_INCLUDES)
])

#------------------------------------------------------------------------
# SC_PRIVATE_TK_HEADERS --
#
#	Locate the private Tk include files
#
# Arguments:
#
#	Requires:
#		TK_SRC_DIR	Assumes that SC_LOAD_TKCONFIG has
#				 already been called.
#
# Results:
#
#	Substs the following vars:
#		TK_INCLUDES
#------------------------------------------------------------------------

AC_DEFUN([SC_PRIVATE_TK_HEADERS],[
    AC_MSG_CHECKING(for Tk private include files)

    case "`uname -s`" in
	*win32* | *WIN32* | *CYGWIN_NT* |*CYGWIN_98*|*CYGWIN_95*)
	    TK_UNIX_DIR_NATIVE=\"`${CYGPATH} ${TK_SRC_DIR}/unix`\"
	    TK_WIN_DIR_NATIVE=\"`${CYGPATH} ${TK_SRC_DIR}/win`\"
	    TK_GENERIC_DIR_NATIVE=\"`${CYGPATH} ${TK_SRC_DIR}/generic`\"
	    TK_XLIB_DIR_NATIVE=\"`${CYGPATH} ${TK_SRC_DIR}/xlib`\"
	    TK_PLATFORM_DIR_NATIVE=${TK_WIN_DIR_NATIVE}

	    TK_INCLUDES="-I${TK_GENERIC_DIR_NATIVE} -I${TK_PLATFORM_DIR_NATIVE} -I${TK_XLIB_DIR_NATIVE}"
	;;
	*)
	    TK_GENERIC_DIR_NATIVE='$(TK_TOP_DIR_NATIVE)/generic'
	    TK_UNIX_DIR_NATIVE='$(TK_TOP_DIR_NATIVE)/unix'
	    TK_WIN_DIR_NATIVE='$(TK_TOP_DIR_NATIVE)/win'
	    TK_PLATFORM_DIR_NATIVE=${TK_UNIX_DIR_NATIVE}

	    TK_INCLUDES="-I${TK_GENERIC_DIR_NATIVE} -I${TK_PLATFORM_DIR_NATIVE}"
	;;
    esac

    AC_SUBST(TK_UNIX_DIR_NATIVE)
    AC_SUBST(TK_WIN_DIR_NATIVE)
    AC_SUBST(TK_GENERIC_DIR_NATIVE)
    AC_SUBST(TK_XLIB_DIR_NATIVE)
    AC_SUBST(TK_PLATFORM_DIR_NATIVE)

    AC_SUBST(TK_INCLUDES)
    AC_MSG_RESULT(Using srcdir found in tkConfig.sh)
])

#------------------------------------------------------------------------
# SC_PUBLIC_TK_HEADERS --
#
#	Locate the installed public Tk header files
#
# Arguments:
#	None.
#
# Requires:
#	CYGPATH must be set
#
# Results:
#
#	Adds a --with-tkinclude switch to configure.
#	Result is cached.
#
#	Substs the following vars:
#		TK_INCLUDES
#------------------------------------------------------------------------

AC_DEFUN([SC_PUBLIC_TK_HEADERS],[
    AC_MSG_CHECKING(for Tk public headers)

    AC_ARG_WITH(tkinclude, [ --with-tkinclude      directory containing the public Tk header files.], with_tkinclude=${withval})

    if test x"${with_tkinclude}" != x ; then
	if test -f "${with_tkinclude}/tk.h" ; then
	    ac_cv_c_tkh=${with_tkinclude}
	else
	    AC_MSG_ERROR([${with_tkinclude} directory does not contain Tk public header file tk.h])
	fi
    else
	AC_CACHE_VAL(ac_cv_c_tkh, [
	    # Use the value from --with-tkinclude, if it was given

	    if test x"${with_tkinclude}" != x ; then
		ac_cv_c_tkh=${with_tkinclude}
	    else
		# Check in the includedir, if --prefix was specified

		eval "temp_includedir=${includedir}"
		for i in \
			`ls -d ${temp_includedir} 2>/dev/null` \
			/usr/local/include /usr/include ; do
		    if test -f "$i/tk.h" ; then
			ac_cv_c_tkh=$i
			break
		    fi
		done
	    fi
	])
    fi

    # Print a message based on how we determined the include path

    if test x"${ac_cv_c_tkh}" = x ; then
	AC_MSG_ERROR(tk.h not found.  Please specify its location with --with-tkinclude)
    else
	AC_MSG_RESULT(${ac_cv_c_tkh})
    fi

    # Convert to a native path and substitute into the output files.

    INCLUDE_DIR_NATIVE=`${CYGPATH} ${ac_cv_c_tkh}`

    TK_INCLUDES=-I\"${INCLUDE_DIR_NATIVE}\"

    AC_SUBST(TK_INCLUDES)
])

#------------------------------------------------------------------------
# SC_SIMPLE_EXEEXT
#	Select the executable extension based on the host type.  This
#	is a lightweight replacement for AC_EXEEXT that doesn't require
#	a compiler.
#
# Arguments
#	none
#
# Results
#	Subst's the following values:
#		EXEEXT
#------------------------------------------------------------------------

AC_DEFUN([SC_SIMPLE_EXEEXT],[
    AC_MSG_CHECKING(executable extension based on host type)

    case "`uname -s`" in
	*win32* | *WIN32* | *CYGWIN_NT* |*CYGWIN_98*|*CYGWIN_95*)
	    EXEEXT=".exe"
	;;
	*)
	    EXEEXT=""
	;;
    esac

    AC_MSG_RESULT(${EXEEXT})
    AC_SUBST(EXEEXT)
])

#------------------------------------------------------------------------
# SC_PROG_TCLSH
#	Locate a tclsh shell in the following directories:
#		${exec_prefix}/bin
#		${prefix}/bin
#		${TCL_BIN_DIR}
#		${TCL_BIN_DIR}/../bin
#		${PATH}
#
# Arguments
#	none
#
# Results
#	Subst's the following values:
#		TCLSH_PROG
#------------------------------------------------------------------------

AC_DEFUN([SC_PROG_TCLSH],[
    AC_MSG_CHECKING([for tclsh])

    AC_CACHE_VAL(ac_cv_path_tclsh, [
	search_path=`echo ${exec_prefix}/bin:${prefix}/bin:${TCL_BIN_DIR}:${TCL_BIN_DIR}/../bin:${PATH} | sed -e 's/:/ /g'`
	for dir in $search_path ; do
	    for j in `ls -r $dir/tclsh[[8-9]]*${EXEEXT} 2> /dev/null` \
		    `ls -r $dir/tclsh*${EXEEXT} 2> /dev/null` ; do
		if test x"$ac_cv_path_tclsh" = x ; then
		    if test -f "$j" ; then
			ac_cv_path_tclsh=$j
			break
		    fi
		fi
	    done
	done
    ])

    if test -f "$ac_cv_path_tclsh" ; then
	TCLSH_PROG=$ac_cv_path_tclsh
	AC_MSG_RESULT($TCLSH_PROG)
    else
	AC_MSG_ERROR(No tclsh found in PATH:  $search_path)
    fi
    AC_SUBST(TCLSH_PROG)
])

#------------------------------------------------------------------------
# SC_PROG_WISH
#	Locate a wish shell in the following directories:
#		${exec_prefix}/bin
#		${prefix}/bin
#		${TCL_BIN_DIR}
#		${TCL_BIN_DIR}/../bin
#		${PATH}
#
# Arguments
#	none
#
# Results
#	Subst's the following values:
#		WISH_PROG
#------------------------------------------------------------------------

AC_DEFUN([SC_PROG_WISH],[
    AC_MSG_CHECKING([for wish])

    AC_CACHE_VAL(ac_cv_path_wish, [
	search_path=`echo ${exec_prefix}/bin:${prefix}/bin:${TCL_BIN_DIR}:${TCL_BIN_DIR}/../bin:${PATH} | sed -e 's/:/ /g'`
	for dir in $search_path ; do
	    for j in `ls -r $dir/wish[[8-9]]*${EXEEXT} 2> /dev/null` \
		    `ls -r $dir/wish*${EXEEXT} 2> /dev/null` ; do
		if test x"$ac_cv_path_wish" = x ; then
		    if test -f "$j" ; then
			ac_cv_path_wish=$j
			break
		    fi
		fi
	    done
	done
    ])

    if test -f "$ac_cv_path_wish" ; then
	WISH_PROG=$ac_cv_path_wish
	AC_MSG_RESULT($WISH_PROG)
    else
	AC_MSG_ERROR(No wish found in PATH:  $search_path)
    fi
    AC_SUBST(WISH_PROG)
])

