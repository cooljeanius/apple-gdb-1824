# tcl.m4
# unix-specific version

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

    if test x"${no_tcl}" = x ; then
	# we reset no_tcl in case something fails here
	no_tcl=true
	AC_ARG_WITH([tcl],[AS_HELP_STRING([--with-tcl],[directory containing tcl configuration (tclConfig.sh)])],[with_tclconfig=${withval}])
	AC_MSG_CHECKING([for Tcl configuration])
	AC_CACHE_VAL([ac_cv_c_tclconfig],[

	    # First check to see if --with-tcl was specified.
	    if test x"${with_tclconfig}" != x ; then
		if test -f "${with_tclconfig}/tclConfig.sh" ; then
		    ac_cv_c_tclconfig=`(cd ${with_tclconfig}; pwd)`
		else
		    AC_MSG_ERROR([${with_tclconfig} directory does NOT contain tclConfig.sh])
		fi
	    fi

	    # then check for a private Tcl installation
	    if test x"${ac_cv_c_tclconfig}" = x ; then
		for i in \
			../tcl \
			`ls -dr ../tcl[[8-9]].[[0-9]]* 2>/dev/null` \
			../../tcl \
			`ls -dr ../../tcl[[8-9]].[[0-9]]* 2>/dev/null` \
			../../../tcl \
			`ls -dr ../../../tcl[[8-9]].[[0-9]]* 2>/dev/null` ; do
		    if test -f "$i/unix/tclConfig.sh" ; then
			ac_cv_c_tclconfig=`(cd $i/unix; pwd)`
			break
		    fi
		done
	    fi

	    # check in a few common install locations
	    if test x"${ac_cv_c_tclconfig}" = x ; then
		for i in `ls -d ${libdir} 2>/dev/null` \
			`ls -d /usr/local/lib 2>/dev/null` \
			`ls -d /usr/contrib/lib 2>/dev/null` \
			`ls -d /usr/lib 2>/dev/null` \
			; do
		    if test -f "$i/tclConfig.sh" ; then
			ac_cv_c_tclconfig=`(cd $i; pwd)`
			break
		    fi
		done
	    fi

	    # check in a few other private locations
	    if test x"${ac_cv_c_tclconfig}" = x ; then
		for i in \
			${srcdir}/../tcl \
			`ls -dr ${srcdir}/../tcl[[8-9]].[[0-9]]* 2>/dev/null` ; do
		    if test -f "$i/unix/tclConfig.sh" ; then
		    ac_cv_c_tclconfig=`(cd $i/unix; pwd)`
		    break
		fi
		done
	    fi
	])

	if test x"${ac_cv_c_tclconfig}" = x ; then
	    TCL_BIN_DIR="# no Tcl configs found"
	    AC_MSG_WARN([Cannot find Tcl configuration definitions])
	    exit 0
	else
	    no_tcl=
	    TCL_BIN_DIR=${ac_cv_c_tclconfig}
	    AC_MSG_RESULT([found $TCL_BIN_DIR/tclConfig.sh])
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
    # Ok, let us find the tk configuration
    # First, look for one uninstalled.
    # the alternative search directory is invoked by --with-tk
    #

    if test x"${no_tk}" = x""; then
	# we reset no_tk in case something fails here
	no_tk=true
	AC_ARG_WITH([tk],[AS_HELP_STRING([--with-tk],[directory containing tk configuration (tkConfig.sh)])],[with_tkconfig=${withval}])
	AC_MSG_CHECKING([for Tk configuration])
	AC_CACHE_VAL([ac_cv_c_tkconfig],[

	    # First check to see if --with-tkconfig was specified.
	    if test x"${with_tkconfig}" != x""; then
		if test -f "${with_tkconfig}/tkConfig.sh" ; then
		    ac_cv_c_tkconfig=`(cd ${with_tkconfig}; pwd)`
		else
		    AC_MSG_ERROR([${with_tkconfig} directory does NOT contain tkConfig.sh])
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
		for i in `ls -d ${libdir} 2>/dev/null` \
			`ls -d /usr/local/lib 2>/dev/null` \
			`ls -d /usr/contrib/lib 2>/dev/null` \
			`ls -d /usr/lib 2>/dev/null` \
			; do
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
	    AC_MSG_WARN([Cannot find Tk configuration definitions])
	    exit 0
	else
	    no_tk=
	    TK_BIN_DIR=${ac_cv_c_tkconfig}
	    AC_MSG_RESULT([found $TK_BIN_DIR/tkConfig.sh])
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
        AC_MSG_RESULT([loading $TCL_BIN_DIR/tclConfig.sh])
	. $TCL_BIN_DIR/tclConfig.sh
    else
        AC_MSG_RESULT([file not found])
    fi

    #
    # If the TCL_BIN_DIR is the build directory (not the install directory),
    # then set the common variable name to the value of the build variables.
    # For example, the variable TCL_LIB_SPEC will be set to the value
    # of TCL_BUILD_LIB_SPEC. An extension should make use of TCL_LIB_SPEC
    # instead of TCL_BUILD_LIB_SPEC since it will work with both an
    # installed and uninstalled version of Tcl.
    #

    if test -f $TCL_BIN_DIR/Makefile ; then
        TCL_LIB_SPEC=${TCL_BUILD_LIB_SPEC}
        TCL_STUB_LIB_SPEC=${TCL_BUILD_STUB_LIB_SPEC}
        TCL_STUB_LIB_PATH=${TCL_BUILD_STUB_LIB_PATH}
    fi

    #
    # eval is required to do the TCL_DBGX substitution
    #

    eval "TCL_LIB_FILE=\"${TCL_LIB_FILE}\""
    eval "TCL_LIB_FLAG=\"${TCL_LIB_FLAG}\""
    eval "TCL_LIB_SPEC=\"${TCL_LIB_SPEC}\""

    eval "TCL_STUB_LIB_FILE=\"${TCL_STUB_LIB_FILE}\""
    eval "TCL_STUB_LIB_FLAG=\"${TCL_STUB_LIB_FLAG}\""
    eval "TCL_STUB_LIB_SPEC=\"${TCL_STUB_LIB_SPEC}\""

    AC_SUBST([TCL_VERSION])
    AC_SUBST([TCL_BIN_DIR])
    AC_SUBST([TCL_SRC_DIR])

    AC_SUBST([TCL_LIB_FILE])
    AC_SUBST([TCL_LIB_FLAG])
    AC_SUBST([TCL_LIB_SPEC])

    AC_SUBST([TCL_STUB_LIB_FILE])
    AC_SUBST([TCL_STUB_LIB_FLAG])
    AC_SUBST([TCL_STUB_LIB_SPEC])
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
        AC_MSG_RESULT([loading $TK_BIN_DIR/tkConfig.sh])
	. $TK_BIN_DIR/tkConfig.sh
    else
        AC_MSG_RESULT([could not find $TK_BIN_DIR/tkConfig.sh])
    fi

    AC_SUBST([TK_VERSION])
    AC_SUBST([TK_BIN_DIR])
    AC_SUBST([TK_SRC_DIR])
    AC_SUBST([TK_LIB_FILE])
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
    AC_ARG_ENABLE([shared],
	[AS_HELP_STRING([--enable-shared],[build and link with shared libraries [--enable-shared]])],
	[tcl_ok=$enableval],[tcl_ok=no])

    if test "${enable_shared+set}" = set; then
	enableval="$enable_shared"
	tcl_ok=$enableval
    else
	tcl_ok=no
    fi

    if test "$tcl_ok" = "yes" ; then
	AC_MSG_RESULT([shared])
	SHARED_BUILD=1
    else
	AC_MSG_RESULT([static])
	SHARED_BUILD=0
	AC_DEFINE([STATIC_BUILD],[1],[Define to 1 if doing a static build])
    fi
])

#------------------------------------------------------------------------
# SC_ENABLE_FRAMEWORK --
#
#	Allows the building of shared libraries into frameworks
#
# Arguments:
#	none
#	
# Results:
#
#	Adds the following arguments to configure:
#		--enable-framework=yes|no
#
#	Sets the following vars:
#		FRAMEWORK_BUILD	Value of 1 or 0
#------------------------------------------------------------------------

AC_DEFUN([SC_ENABLE_FRAMEWORK],[
    AC_MSG_CHECKING([how to package libraries])
    AC_ARG_ENABLE([framework],
	[AS_HELP_STRING([--enable-framework],[package shared libraries in frameworks [--disable-framework]])],
	[tcl_ok=$enableval],[tcl_ok=no])

    if test "${enable_framework+set}" = set; then
	enableval="$enable_framework"
	tcl_ok=$enableval
    else
	tcl_ok=no
    fi

    if test "$tcl_ok" = "yes" ; then
	AC_MSG_RESULT([framework])
	FRAMEWORK_BUILD=1
	if test "${SHARED_BUILD}" = "0" ; then
	    AC_MSG_WARN(["Frameworks can only be built if --enable-shared is yes"])
	    FRAMEWORK_BUILD=0
	fi
    else
	AC_MSG_RESULT([standard shared library])
	FRAMEWORK_BUILD=0
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
#		_THREAD_SAFE
#
#------------------------------------------------------------------------

AC_DEFUN([SC_ENABLE_THREADS],[
    AC_MSG_CHECKING([for building with threads])
    AC_ARG_ENABLE([threads],[AS_HELP_STRING([--enable-threads],[build with threads])],
	[tcl_ok=$enableval],[tcl_ok=no])

    if test "$tcl_ok" = "yes"; then
	AC_MSG_RESULT([yes])
	TCL_THREADS=1
	AC_DEFINE([TCL_THREADS],[1],[Define to 1 for Tcl threads])
	# USE_THREAD_ALLOC tells us to try the special thread-based
	# allocator that significantly reduces lock contention
	AC_DEFINE([USE_THREAD_ALLOC],[1],[Define to 1 to try the special thread-based allocator])
	AC_DEFINE([_REENTRANT],[1],[If this is not defined, then Solaris will not define thread-safe library routines.])
	AC_DEFINE([_THREAD_SAFE],[1],[Define to 1 if thread safe])
	AC_CHECK_LIB([pthread],[pthread_mutex_init],[tcl_ok=yes],[tcl_ok=no])
	if test "$tcl_ok" = "no"; then
	    # Check a little harder for __pthread_mutex_init in the same
	    # library, as some systems hide it there until pthread.h is
	    # defined. We could alternatively do an AC_COMPILE_IFELSE with
	    # pthread.h, but that will work with a libpthread that really does
	    # NOT exist, like AIX 4.2.  [Bug: 4359]
	    AC_CHECK_LIB([pthread],[__pthread_mutex_init],[tcl_ok=yes],[tcl_ok=no])
	fi

	if test "$tcl_ok" = "yes"; then
	    # The space is needed
	    THREADS_LIBS=" -lpthread"
	else
	    AC_CHECK_LIB([pthreads],[pthread_mutex_init],[tcl_ok=yes],[tcl_ok=no])
	    if test "$tcl_ok" = "yes"; then
		# The space is needed
		THREADS_LIBS=" -lpthreads"
	    else
		AC_CHECK_LIB([c],[pthread_mutex_init],[tcl_ok=yes],[tcl_ok=no])
	    	if test "$tcl_ok" = "no"; then
		    AC_CHECK_LIB([c_r],[pthread_mutex_init],[tcl_ok=yes],[tcl_ok=no])
		    if test "$tcl_ok" = "yes"; then
			# The space is needed
			THREADS_LIBS=" -pthread"
		    else
			TCL_THREADS=0
			AC_MSG_WARN(["Do not know how to find pthread lib on your system - you must disable thread support or edit the LIBS in the Makefile..."])
		    fi
	    	fi
	    fi
	fi

	# Does the pthread-implementation provide
	# 'pthread_attr_setstacksize' ?

	AC_CHECK_FUNCS([pthread_attr_setstacksize])
	AC_CHECK_FUNCS([readdir_r])
    else
	TCL_THREADS=0
	AC_MSG_RESULT([no (default)])
    fi
    AC_SUBST([TCL_THREADS])
])

#------------------------------------------------------------------------
# SC_ENABLE_SYMBOLS --
#
#	Specify if debugging symbols should be used.
#	Memory (TCL_MEM_DEBUG) and compile (TCL_COMPILE_DEBUG) debugging
#	can also be enabled.
#
# Arguments:
#	none
#	
#	Requires the following vars to be set in the Makefile:
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
#		CFLAGS_DEFAULT	Sets to $(CFLAGS_DEBUG) if true
#				Sets to $(CFLAGS_OPTIMIZE) if false
#		LDFLAGS_DEFAULT	Sets to $(LDFLAGS_DEBUG) if true
#				Sets to $(LDFLAGS_OPTIMIZE) if false
#		DBGX		Debug library extension
#
#------------------------------------------------------------------------

AC_DEFUN([SC_ENABLE_SYMBOLS],[
    AC_MSG_CHECKING([for build with symbols])
    AC_ARG_ENABLE([symbols],[AS_HELP_STRING([--enable-symbols],[build with debugging symbols [--disable-symbols]])],[tcl_ok=$enableval],[tcl_ok=no])
# FIXME: Currently, LDFLAGS_DEFAULT is not used, it should work like CFLAGS_DEFAULT.
    if test "$tcl_ok" = "no"; then
	CFLAGS_DEFAULT='$(CFLAGS_OPTIMIZE)'
	LDFLAGS_DEFAULT='$(LDFLAGS_OPTIMIZE)'
	DBGX=""
	AC_MSG_RESULT([no])
    else
	CFLAGS_DEFAULT='$(CFLAGS_DEBUG)'
	LDFLAGS_DEFAULT='$(LDFLAGS_DEBUG)'
	DBGX=g
	if test "$tcl_ok" = "yes"; then
	    AC_MSG_RESULT([yes (standard debugging)])
	fi
    fi
    AC_SUBST([CFLAGS_DEFAULT])
    AC_SUBST([LDFLAGS_DEFAULT])

    if test "$tcl_ok" = "mem" -o "$tcl_ok" = "all"; then
	AC_DEFINE([TCL_MEM_DEBUG],[1],[Define to 1 for debugging Tcl memory])
    fi

    if test "$tcl_ok" = "compile" -o "$tcl_ok" = "all"; then
	AC_DEFINE([TCL_COMPILE_DEBUG],[1],[Define to 1 for compilation debugging])
	AC_DEFINE([TCL_COMPILE_STATS],[1],[Define to 1 for compilation statistics])
    fi

    if test "$tcl_ok" != "yes" -a "$tcl_ok" != "no"; then
	if test "$tcl_ok" = "all"; then
	    AC_MSG_RESULT([enabled symbols mem compile debugging])
	else
	    AC_MSG_RESULT([enabled $tcl_ok debugging])
	fi
    fi
])

#------------------------------------------------------------------------
# SC_ENABLE_LANGINFO --
#
#	Allows use of modern nl_langinfo check for better l10n.
#	This is only relevant for Unix.
#
# Arguments:
#	none
#	
# Results:
#
#	Adds the following arguments to configure:
#		--enable-langinfo=yes|no (default is yes)
#
#	Defines the following vars:
#		HAVE_LANGINFO	Triggers use of nl_langinfo if defined.
#
#------------------------------------------------------------------------

AC_DEFUN([SC_ENABLE_LANGINFO],[
    AC_ARG_ENABLE([langinfo],
	[AS_HELP_STRING([--enable-langinfo],[use nl_langinfo if possible to determine encoding at startup, otherwise use old heuristic])],
	[langinfo_ok=$enableval], [langinfo_ok=yes])

    HAVE_LANGINFO=0
    if test "$langinfo_ok" = "yes"; then
	if test "$langinfo_ok" = "yes"; then
	    AC_CHECK_HEADERS([langinfo.h],[langinfo_ok=yes],[langinfo_ok=no])
	fi
    fi
    AC_MSG_CHECKING([whether to use nl_langinfo])
    if test "$langinfo_ok" = "yes"; then
	AC_COMPILE_IFELSE([AC_LANG_SOURCE([[#include <langinfo.h>]],
		[[nl_langinfo(CODESET);]])],[langinfo_ok=yes],[langinfo_ok=no])
	if test "$langinfo_ok" = "no"; then
	    langinfo_ok="no (could not compile with nl_langinfo)";
	fi
	if test "$langinfo_ok" = "yes"; then
	    AC_DEFINE([HAVE_LANGINFO],[1],[Define to 1 if we can compile with nl_langinfo])
	fi
    fi
    AC_MSG_RESULT([$langinfo_ok])
])

#--------------------------------------------------------------------
# SC_CONFIG_MANPAGES
#	
#	Decide whether to use symlinks for linking the manpages and
#	whether to compress the manpages after installation.
#
# Arguments:
#	none
#
# Results:
#
#	Adds the following arguments to configure:
#		--enable-man-symlinks
#		--enable-man-compression=PROG
#
#	Defines the following variable:
#
#	MKLINKS_FLAGS -		The apropriate flags for mkLinks
#				according to the user's selection.
#
#--------------------------------------------------------------------
AC_DEFUN([SC_CONFIG_MANPAGES],[

	AC_MSG_CHECKING([whether to use symlinks for manpages])
	AC_ARG_ENABLE([man-symlinks],
		[AS_HELP_STRING([--enable-man-symlinks],[use symlinks for the manpages])],
		test "$enableval" != "no" && MKLINKS_FLAGS="$MKLINKS_FLAGS --symlinks",
		enableval="no")
	AC_MSG_RESULT([$enableval])

	AC_MSG_CHECKING([compression for manpages])
	AC_ARG_ENABLE([man-compression],
		[AS_HELP_STRING([--enable-man-compression=PROG],
                          [compress the manpages with PROG])],
		test "$enableval" = "yes" && echo && AC_MSG_ERROR([missing argument to --enable-man-compression])
		test "$enableval" != "no" && MKLINKS_FLAGS="$MKLINKS_FLAGS --compress $enableval",
		enableval="no")
	AC_MSG_RESULT([$enableval])

	AC_SUBST([MKLINKS_FLAGS])
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
#	Defines and substitutes the following vars:
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
#                       the variable LIB_RUNTIME_DIR in the Makefile. Could
#                       be the same as CC_SEARCH_FLAGS if ${CC} is used to link.
#       CC_SEARCH_FLAGS-Flags to pass to ${CC}, such as "-Wl,-rpath,/usr/local/tcl/lib",
#                       that tell the run-time dynamic linker where to look
#                       for shared libraries such as libtcl.so.  Depends on
#                       the variable LIB_RUNTIME_DIR in the Makefile.
#       MAKE_LIB -      Command to execute to build the a library;
#                       differs when building shared or static.
#       MAKE_STUB_LIB -
#                       Command to execute to build a stub library.
#       INSTALL_LIB -   Command to execute to install a library;
#                       differs when building shared or static.
#       INSTALL_STUB_LIB -
#                       Command to execute to install a stub library.
#       STLIB_LD -      Base command to use for combining object files
#                       into a static library.
#       SHLIB_CFLAGS -  Flags to pass to cc when compiling the components
#                       of a shared library (may request position-independent
#                       code, among other things).
#       SHLIB_LD -      Base command to use for combining object files
#                       into a shared library.
#       SHLIB_LD_FLAGS -Flags to pass when building a shared library. This
#                       differes from the SHLIB_CFLAGS as it is not used
#                       when building object files or executables.
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
# TCL_SHLIB_LD_EXTRAS - Additional element which are added to SHLIB_LD_LIBS
#  TK_SHLIB_LD_EXTRAS   for the build of Tcl and Tk, but not recorded in the
#                       tclConfig.sh, since they are only used for the build
#                       of Tcl and Tk. 
#                       Examples: MacOS X records the library version and
#                       compatibility version in the shared library.  But
#                       of course the Tcl version of this is only used for Tcl.
#       LIB_SUFFIX -    Specifies everything that comes after the "libfoo"
#                       in a static or shared library name, using the $VERSION variable
#                       to put the version in the right place.  This is used
#                       by platforms that need non-standard library names.
#                       Examples:  ${VERSION}.so.1.1 on NetBSD, since it needs
#                       to have a version after the .so, and ${VERSION}.a
#                       on AIX, since a shared library needs to have
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
#	CFLAGS_WARNING -
#			Flags used to make the compiler print warnings
#	EXTRA_CFLAGS
#
#--------------------------------------------------------------------

AC_DEFUN([SC_CONFIG_CFLAGS],[

    # Step 0.a: Enable 64 bit support?

    AC_MSG_CHECKING([if 64bit support is requested])
    AC_ARG_ENABLE([64bit],[AS_HELP_STRING([--enable-64bit],[enable 64bit support (where applicable)])],[],[enableval="no"])

    if test "$enableval" = "yes"; then
	do64bit=yes
    else
	do64bit=no
    fi
    AC_MSG_RESULT([$do64bit])

    # Step 0.b: Enable Solaris 64 bit VIS support?

    AC_MSG_CHECKING([if 64bit Sparc VIS support is requested])
    AC_ARG_ENABLE([64bit-vis],[AS_HELP_STRING([--enable-64bit-vis],[enable 64bit Sparc VIS support])],[],[enableval="no"])

    if test "$enableval" = "yes"; then
	# Force 64bit on with VIS
	do64bit=yes
	do64bitVIS=yes
    else
	do64bitVIS=no
    fi
    AC_MSG_RESULT([$do64bitVIS])

    # Step 1: set the variable "system" to hold the name and version number
    # for the system. This can usually be done via the "uname" command, but
    # there are a few systems, like Next, where this does NOT work.

    AC_REQUIRE([AC_PROG_AWK])dnl

    AC_MSG_CHECKING([system version (for dynamic loading)])
    if test -f /usr/lib/NextStep/software_version; then
	system=NEXTSTEP-`awk '/3/,/3/' /usr/lib/NextStep/software_version`
    else
	system=`uname -s`-`uname -r`
	if test "$?" -ne 0 ; then
	    AC_MSG_RESULT([unknown (cannot find uname command)])
	    system=unknown
	else
	    # Special check for weird MP-RAS system (uname returns weird
	    # results, and the version is kept in special file).
	
	    if test -r /etc/.relid -a "X`uname -n`" = "X`uname -s`" ; then
		system=MP-RAS-`awk '{print $3}' /etc/.relid'`
	    fi
	    if test "`uname -s`" = "AIX" ; then
		system=AIX-`uname -v`.`uname -r`
	    fi
	    AC_MSG_RESULT([$system])
	fi
    fi

    # Step 2: check for existence of -ldl library.  This is needed because
    # Linux can use either -ldl or -ldld for dynamic loading:
    AC_CHECK_LIB([dl],[dlopen],[have_dl=yes],[have_dl=no])dnl

    dnl# Require ranlib early so we can override it in special cases below:
    AC_REQUIRE([AC_PROG_RANLIB])dnl

    # Step 3: set configuration options based on system name and version.

    do64bit_ok=no
    EXTRA_CFLAGS=""
    TCL_EXPORT_FILE_SUFFIX=""
    UNSHARED_LIB_SUFFIX=""
    TCL_TRIM_DOTS='`echo ${VERSION} | tr -d .`'
    ECHO_VERSION='`echo ${VERSION}`'
    TCL_LIB_VERSIONS_OK=ok
    CFLAGS_DEBUG=-ggdb
    CFLAGS_OPTIMIZE=-O1
    if test "x${GCC}" = "xyes"; then
	# FIXME: '-Wconversion' changed semantics in newer gcc versions.
	# '-Wtraditional-conversion' is the new name for the old
	# '-Wconversion'.
	# We need to choose which one to add based on gcc version.
	# Also we should be checking these flags to see if they are
	# actually supported or not:
	CFLAGS_WARNING="-Wall -Wno-implicit-int"
	CFLAGS_WARNING="${CFLAGS_WARNING} -Wno-deprecated-declarations"
    else
	CFLAGS_WARNING=""
    fi
    TCL_NEEDS_EXP_FILE=0
    TCL_BUILD_EXP_FILE=""
    TCL_EXP_FILE=""
dnl# FIXME: Replace AC_CHECK_PROG with AC_CHECK_TOOL once cross compiling
dnl# is fixed.
    AC_CHECK_PROG([AR],[ar],[ar])
    STLIB_LD='${AR} cr'
    LD_LIBRARY_PATH_VAR="LD_LIBRARY_PATH"
    PLAT_OBJS=""
    case $system in
	AIX-5.*)
	    if test "${TCL_THREADS}" = "1" -a "$GCC" != "yes" ; then
		# AIX requires the _r compiler when gcc isn't being used
		if test "${CC}" != "cc_r" ; then
		    CC=${CC}_r
		fi
		AC_MSG_RESULT([Using $CC for compiling with threads])
	    fi
	    LIBS="$LIBS -lc"
	    # AIX-5 uses ELF style dynamic libraries
	    SHLIB_CFLAGS=""
	    SHLIB_LD_LIBS='${LIBS}'
	    SHLIB_SUFFIX=".so"
	    if test "`uname -m`" = "ia64" ; then
		# AIX-5 uses ELF style dynamic libraries on IA-64, but not PPC
		SHLIB_LD="/usr/ccs/bin/ld -G -z text"
		# AIX-5 has dl* in libc.so
		DL_LIBS=""
		if test "$GCC" = "yes" ; then
		    CC_SEARCH_FLAGS='-Wl,-R,${LIB_RUNTIME_DIR}'
		else
		    CC_SEARCH_FLAGS='-R${LIB_RUNTIME_DIR}'
		fi
		LD_SEARCH_FLAGS='-R ${LIB_RUNTIME_DIR}'
	    else
		SHLIB_LD="${TCL_SRC_DIR}/unix/ldAix /bin/ld -bhalt:4 -bM:SRE -bE:lib.exp -H512 -T512 -bnoentry"
		DL_LIBS="-ldl"
		CC_SEARCH_FLAGS='-L${LIB_RUNTIME_DIR}'
		LD_SEARCH_FLAGS=${CC_SEARCH_FLAGS}
		TCL_NEEDS_EXP_FILE=1
		TCL_EXPORT_FILE_SUFFIX='${VERSION}\$\{DBGX\}.exp'
	    fi

	    # Note: need the LIBS below, otherwise Tk won't find Tcl's
	    # symbols when dynamically loaded into tclsh.

	    DL_OBJS="tclLoadDl.o"
	    LDFLAGS=""

	    LD_LIBRARY_PATH_VAR="LIBPATH"

	    # Check to enable 64-bit flags for compiler/linker
	    if test "$do64bit" = "yes" ; then
		if test "$GCC" = "yes" ; then
		    AC_MSG_WARN(["64bit mode not supported with GCC on $system"])
		else 
		    do64bit_ok=yes
		    EXTRA_CFLAGS="-q64"
		    LDFLAGS="-q64"
		    RANLIB="${RANLIB} -X64"
		    AR="${AR} -X64"
		    SHLIB_LD_FLAGS="-b64"
		fi
	    fi
	    ;;
	AIX-*)
	    if test "${TCL_THREADS}" = "1" -a "$GCC" != "yes" ; then
		# AIX requires the _r compiler when gcc isn't being used
		if test "${CC}" != "cc_r" ; then
		    CC=${CC}_r
		fi
		AC_MSG_RESULT([Using $CC for compiling with threads])
	    fi
	    LIBS="$LIBS -lc"
	    SHLIB_CFLAGS=""
	    SHLIB_LD="${TCL_SRC_DIR}/unix/ldAix /bin/ld -bhalt:4 -bM:SRE -bE:lib.exp -H512 -T512 -bnoentry"
	    SHLIB_LD_LIBS='${LIBS}'
	    SHLIB_SUFFIX=".so"
	    DL_OBJS="tclLoadDl.o"
	    DL_LIBS="-ldl"
	    LDFLAGS=""
	    CC_SEARCH_FLAGS='-L${LIB_RUNTIME_DIR}'
	    LD_SEARCH_FLAGS=${CC_SEARCH_FLAGS}
	    LD_LIBRARY_PATH_VAR="LIBPATH"
	    TCL_NEEDS_EXP_FILE=1
	    TCL_EXPORT_FILE_SUFFIX='${VERSION}\$\{DBGX\}.exp'

	    # AIX v<=4.1 has some different flags than 4.2+
	    if test "$system" = "AIX-4.1" -o "`uname -v`" -lt "4" ; then
		LIBOBJS="$LIBOBJS tclLoadAix.o"
		DL_LIBS="-lld"
	    fi

	    # On AIX <=v4 systems, libbsd.a has to be linked in to support
	    # non-blocking file IO.  This library has to be linked in after
	    # the MATH_LIBS or it breaks the pow() function.  The way to
	    # insure proper sequencing, is to add it to the tail of MATH_LIBS.
	    # This library also supplies gettimeofday.
	    #
	    # AIX does not have a timezone field in struct tm. When the AIX
	    # bsd library is used, the timezone global and the gettimeofday
	    # methods are to be avoided for timezone deduction instead, we
	    # deduce the timezone by comparing the localtime result on a
	    # known GMT value.

	    AC_CHECK_LIB([bsd],[gettimeofday],[libbsd=yes],[libbsd=no])
	    if test $libbsd = yes; then
	    	MATH_LIBS="$MATH_LIBS -lbsd"
	    	AC_DEFINE([USE_DELTA_FOR_TZ],[1],[Define to 1 because AIX needs it])
	    fi

	    # Check to enable 64-bit flags for compiler/linker
	    if test "$do64bit" = "yes" ; then
		if test "$GCC" = "yes" ; then
		    AC_MSG_WARN(["64bit mode not supported with GCC on $system"])
		else 
		    do64bit_ok=yes
		    EXTRA_CFLAGS="-q64"
		    LDFLAGS="-q64"
		    RANLIB="${RANLIB} -X64"
		    AR="${AR} -X64"
		    SHLIB_LD_FLAGS="-b64"
		fi
	    fi
	    ;;
	BSD/OS-2.1*|BSD/OS-3*)
	    SHLIB_CFLAGS=""
	    SHLIB_LD="shlicc -r"
	    SHLIB_LD_LIBS='${LIBS}'
	    SHLIB_SUFFIX=".so"
	    DL_OBJS="tclLoadDl.o"
	    DL_LIBS="-ldl"
	    LDFLAGS=""
	    CC_SEARCH_FLAGS=""
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
	    CC_SEARCH_FLAGS=""
	    LD_SEARCH_FLAGS=""
	    ;;
	dgux*)
	    SHLIB_CFLAGS="-K PIC"
	    SHLIB_LD="cc -G"
	    SHLIB_LD_LIBS=""
	    SHLIB_SUFFIX=".so"
	    DL_OBJS="tclLoadDl.o"
	    DL_LIBS="-ldl"
	    LDFLAGS=""
	    CC_SEARCH_FLAGS=""
	    LD_SEARCH_FLAGS=""
	    ;;
	HP-UX-*.11.*)
	    # Use updated header definitions where possible
	    AC_DEFINE([_XOPEN_SOURCE_EXTENDED],[1],[Define to 1 to use updated header definitions where possible])

	    SHLIB_SUFFIX=".sl"
	    AC_CHECK_LIB([dld],[shl_load],[tcl_ok=yes],[tcl_ok=no])
	    if test "$tcl_ok" = yes; then
		SHLIB_CFLAGS="+z"
		SHLIB_LD="ld -b"
		SHLIB_LD_LIBS='${LIBS}'
		DL_OBJS="tclLoadShl.o"
		DL_LIBS="-ldld"
		LDFLAGS="-Wl,-E"
		CC_SEARCH_FLAGS='-Wl,+s,+b,${LIB_RUNTIME_DIR}:.'
		LD_SEARCH_FLAGS='+s +b ${LIB_RUNTIME_DIR}:.'
		LD_LIBRARY_PATH_VAR="SHLIB_PATH"
	    fi

	    # Users may want PA-RISC 1.1/2.0 portable code - needs HP cc
	    EXTRA_CFLAGS="+DAportable"

	    # Check to enable 64-bit flags for compiler/linker
	    if test "$do64bit" = "yes" ; then
		if test "$GCC" = "yes" ; then
		    hpux_arch=`gcc -dumpmachine`
		    case $hpux_arch in
			hppa64*)
			    # 64-bit gcc in use.  Fix flags for GNU ld.
			    do64bit_ok=yes
			    SHLIB_LD="gcc -shared"
			    SHLIB_LD_LIBS=""
			    LD_SEARCH_FLAGS=''
			    CC_SEARCH_FLAGS=''
			    ;;
			*)
			    AC_MSG_WARN(["64bit mode not supported with GCC on $system"])
			    ;;
		    esac
		else
		    do64bit_ok=yes
		    EXTRA_CFLAGS="+DA2.0W"
		    LDFLAGS="+DA2.0W $LDFLAGS"
		fi
	    fi
	    ;;
	HP-UX-*.08.*|HP-UX-*.09.*|HP-UX-*.10.*)
	    SHLIB_SUFFIX=".sl"
	    AC_CHECK_LIB([dld],[shl_load],[tcl_ok=yes],[tcl_ok=no])
	    if test "$tcl_ok" = yes; then
		SHLIB_CFLAGS="+z"
		SHLIB_LD="ld -b"
		SHLIB_LD_LIBS=""
		DL_OBJS="tclLoadShl.o"
		DL_LIBS="-ldld"
		LDFLAGS="-Wl,-E"
		CC_SEARCH_FLAGS='-Wl,+s,+b,${LIB_RUNTIME_DIR}:.'
		LD_SEARCH_FLAGS='+s +b ${LIB_RUNTIME_DIR}:.'
		LD_LIBRARY_PATH_VAR="SHLIB_PATH"
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
	    CC_SEARCH_FLAGS='-L${LIB_RUNTIME_DIR}'
	    LD_SEARCH_FLAGS=${CC_SEARCH_FLAGS}
	    SHARED_LIB_SUFFIX='${VERSION}\$\{DBGX\}.a'
	    ;;
	IRIX-5.*)
	    SHLIB_CFLAGS=""
	    SHLIB_LD="ld -shared -rdata_shared"
	    SHLIB_LD_LIBS='${LIBS}'
	    SHLIB_SUFFIX=".so"
	    DL_OBJS="tclLoadDl.o"
	    DL_LIBS=""
	    CC_SEARCH_FLAGS='-Wl,-rpath,${LIB_RUNTIME_DIR}'
	    LD_SEARCH_FLAGS='-rpath ${LIB_RUNTIME_DIR}'
	    EXTRA_CFLAGS=""
	    LDFLAGS=""
	    ;;
	IRIX-6.*|IRIX64-6.5*)
	    SHLIB_CFLAGS=""
	    SHLIB_LD="ld -n32 -shared -rdata_shared"
	    SHLIB_LD_LIBS='${LIBS}'
	    SHLIB_SUFFIX=".so"
	    DL_OBJS="tclLoadDl.o"
	    DL_LIBS=""
	    CC_SEARCH_FLAGS='-Wl,-rpath,${LIB_RUNTIME_DIR}'
	    LD_SEARCH_FLAGS='-rpath ${LIB_RUNTIME_DIR}'
	    if test "$GCC" = "yes" ; then
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
	    SHLIB_LD="ld -n32 -shared -rdata_shared"
	    SHLIB_LD_LIBS='${LIBS}'
	    SHLIB_SUFFIX=".so"
	    DL_OBJS="tclLoadDl.o"
	    DL_LIBS=""
	    LDFLAGS=""
	    CC_SEARCH_FLAGS='-Wl,-rpath,${LIB_RUNTIME_DIR}'
	    LD_SEARCH_FLAGS='-rpath ${LIB_RUNTIME_DIR}'

	    # Check to enable 64-bit flags for compiler/linker

	    if test "$do64bit" = "yes" ; then
	        if test "$GCC" = "yes" ; then
	            AC_MSG_WARN([64bit mode not supported by gcc])
	        else
	            do64bit_ok=yes
	            SHLIB_LD="ld -64 -shared -rdata_shared"
	            EXTRA_CFLAGS="-64"
	            LDFLAGS="-64"
	        fi
	    fi
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
		CC_SEARCH_FLAGS='-Wl,-rpath,${LIB_RUNTIME_DIR}'
		LD_SEARCH_FLAGS=${CC_SEARCH_FLAGS}
	    else
		AC_CHECK_HEADERS([dld.h],[
		    SHLIB_LD="ld -shared"
		    DL_OBJS="tclLoadDld.o"
		    DL_LIBS="-ldld"
		    LDFLAGS=""
		    CC_SEARCH_FLAGS=""
		    LD_SEARCH_FLAGS=""])
	    fi
	    if test "`uname -m`" = "alpha" ; then
		EXTRA_CFLAGS="-mieee"
	    fi

	    # The combo of gcc + glibc has a bug related
	    # to inlining of functions like strtod(). The
	    # -fno-builtin flag should address this problem
	    # but it does not work. The -fno-inline flag
	    # is kind of overkill but it works.
	    # Disable inlining only when one of the
	    # files in compat/*.c is being linked in.
	    if test x"${LIBOBJS}" != x ; then
	        EXTRA_CFLAGS="${EXTRA_CFLAGS} -fno-inline"
	    fi

	    # XIM peeking works under XFree86.
	    AC_DEFINE([PEEK_XCLOSEIM],[1],[Define to 1 if XIM peeking works under XFree86.])

	    ;;
	GNU*)
	    SHLIB_CFLAGS="-fPIC"
	    SHLIB_LD_LIBS='${LIBS}'
	    SHLIB_SUFFIX=".so"

	    if test "$have_dl" = yes; then
		SHLIB_LD="${CC} -shared"
		DL_OBJS=""
		DL_LIBS="-ldl"
		LDFLAGS="-rdynamic"
		CC_SEARCH_FLAGS=""
		LD_SEARCH_FLAGS=""
	    else
		AC_CHECK_HEADERS([dld.h],[
		    SHLIB_LD="ld -shared"
		    DL_OBJS=""
		    DL_LIBS="-ldld"
		    LDFLAGS=""
		    CC_SEARCH_FLAGS=""
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
	    CC_SEARCH_FLAGS=""
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
	    CC_SEARCH_FLAGS=""
	    LD_SEARCH_FLAGS=""
	    ;;
	NetBSD-*|FreeBSD-[[1-2]].*|OpenBSD-*)
	    # Not available on all versions:  check for include file.
	    AC_CHECK_HEADERS([dlfcn.h],[
		# NetBSD/SPARC needs -fPIC, -fpic will not do.
		SHLIB_CFLAGS="-fPIC"
		SHLIB_LD="ld -Bshareable -x"
		SHLIB_LD_LIBS=""
		SHLIB_SUFFIX=".so"
		DL_OBJS="tclLoadDl.o"
		DL_LIBS=""
		LDFLAGS=""
		CC_SEARCH_FLAGS='-Wl,-rpath,${LIB_RUNTIME_DIR}'
		LD_SEARCH_FLAGS='-rpath ${LIB_RUNTIME_DIR}'
		AC_MSG_CHECKING([for ELF])
		AC_EGREP_CPP([yes],[
#ifdef __ELF__
	yes
#endif /* __ELF__ */
		],[
		    AC_MSG_RESULT([yes])
		    SHARED_LIB_SUFFIX='${TCL_TRIM_DOTS}\$\{DBGX\}.so',
		    AC_MSG_RESULT([no])
		    SHARED_LIB_SUFFIX='${TCL_TRIM_DOTS}\$\{DBGX\}.so.1.0'
		])
	    ],[
		SHLIB_CFLAGS=""
		SHLIB_LD="echo tclLdAout $CC \{$SHLIB_CFLAGS\} | `pwd`/tclsh -r"
		SHLIB_LD_LIBS='${LIBS}'
		SHLIB_SUFFIX=".a"
		DL_OBJS="tclLoadAout.o"
		DL_LIBS=""
		LDFLAGS=""
		CC_SEARCH_FLAGS='-L${LIB_RUNTIME_DIR}'
		LD_SEARCH_FLAGS=${CC_SEARCH_FLAGS}
		SHARED_LIB_SUFFIX='${TCL_TRIM_DOTS}\$\{DBGX\}.a'
	    ])

	    # FreeBSD does NOT handle version numbers with dots.

	    UNSHARED_LIB_SUFFIX='${TCL_TRIM_DOTS}\$\{DBGX\}.a'
	    TCL_LIB_VERSIONS_OK=nodots
	    ;;
	FreeBSD-*)
	    # FreeBSD 3.* and greater have ELF.
	    SHLIB_CFLAGS="-fPIC"
	    SHLIB_LD="ld -Bshareable -x"
	    SHLIB_LD_LIBS='${LIBS}'
	    SHLIB_SUFFIX=".so"
	    DL_OBJS="tclLoadDl.o"
	    DL_LIBS=""
	    LDFLAGS="-export-dynamic"
	    CC_SEARCH_FLAGS='-Wl,-rpath,${LIB_RUNTIME_DIR}'
	    LD_SEARCH_FLAGS='-rpath ${LIB_RUNTIME_DIR}'
	    if test "${TCL_THREADS}" = "1" ; then
		AC_REQUIRE([AC_PROG_SED])
		# The -pthread needs to go in the CFLAGS, not LIBS
		LIBS=`echo $LIBS | sed s/-pthread//`
		EXTRA_CFLAGS="-pthread"
	    	LDFLAGS="$LDFLAGS -pthread"
	    fi
	    case $system in
	    FreeBSD-3.*)
	    	# FreeBSD-3 does NOT handle version numbers with dots.
	    	UNSHARED_LIB_SUFFIX='${TCL_TRIM_DOTS}\$\{DBGX\}.a'
	    	SHARED_LIB_SUFFIX='${TCL_TRIM_DOTS}\$\{DBGX\}.so'
	    	TCL_LIB_VERSIONS_OK=nodots
		;;
	    esac
	    ;;
	Rhapsody-*|Darwin-*)
	    SHLIB_CFLAGS="-fno-common"
	    SHLIB_LD="cc -dynamiclib \${LDFLAGS}"
	    TCL_SHLIB_LD_EXTRAS="-compatibility_version ${TCL_VERSION} -current_version \${VERSION} -install_name \${DYLIB_INSTALL_DIR}/\${TCL_LIB_FILE} -prebind -seg1addr 0xa000000"
	    TK_SHLIB_LD_EXTRAS="-compatibility_version ${TK_VERSION} -current_version \${VERSION} -install_name \${DYLIB_INSTALL_DIR}/\${TK_LIB_FILE} -prebind -seg1addr 0xb000000"
	    SHLIB_LD_LIBS='${LIBS}'
	    SHLIB_SUFFIX=".dylib"
	    DL_OBJS="tclLoadDyld.o"
	    PLAT_OBJS="tclMacOSXBundle.o"
	    DL_LIBS=""
	    LDFLAGS="-prebind"
	    CC_SEARCH_FLAGS=""
	    LD_SEARCH_FLAGS=""
	    CFLAGS_OPTIMIZE="-Os"
	    LD_LIBRARY_PATH_VAR="DYLD_LIBRARY_PATH"
	    # for compatibility with autoconf vers 2.13 :
	    HACK=""
	    if test "x${tcl_corefoundation}" = "xyes"; then
	        EXTRA_CFLAGS="-DMA${HACK}C_OSX_TCL -DHAVE_CFBUNDLE -DTCL_DEFAULT_ENCODING=\\\"utf-8\\\""
	    else
	        EXTRA_CFLAGS="-DMA${HACK}C_OSX_TCL -DTCL_DEFAULT_ENCODING=\\\"utf-8\\\""
	    fi
	    if test "x${tcl_corefoundation}" != "xno"; then
	        LIBS="${LIBS} -framework CoreFoundation"
	    fi
	    ;;
	NEXTSTEP-*)
	    SHLIB_CFLAGS=""
	    SHLIB_LD="cc -nostdlib -r"
	    SHLIB_LD_LIBS=""
	    SHLIB_SUFFIX=".so"
	    DL_OBJS="tclLoadNext.o"
	    DL_LIBS=""
	    LDFLAGS=""
	    CC_SEARCH_FLAGS=""
	    LD_SEARCH_FLAGS=""
	    ;;
	OS/390-*)
	    CFLAGS_OPTIMIZE=""      # Optimizer is buggy
	    AC_DEFINE([_OE_SOCKETS],[1],[needed in sys/socket.h])  # needed in sys/socket.h
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
	    CC_SEARCH_FLAGS=""
	    LD_SEARCH_FLAGS=""
	    ;;
	OSF1-1.*)
	    # OSF/1 1.3 from OSF using ELF, and derivatives, including AD2
	    SHLIB_CFLAGS="-fPIC"
	    if test "$SHARED_BUILD" = "1" ; then
	        SHLIB_LD="ld -shared"
	    else
	        SHLIB_LD="ld -non_shared"
	    fi
	    SHLIB_LD_LIBS=""
	    SHLIB_SUFFIX=".so"
	    DL_OBJS="tclLoadDl.o"
	    DL_LIBS=""
	    LDFLAGS=""
	    CC_SEARCH_FLAGS=""
	    LD_SEARCH_FLAGS=""
	    ;;
	OSF1-V*)
	    # Digital OSF/1
	    SHLIB_CFLAGS=""
	    if test "$SHARED_BUILD" = "1" ; then
	        SHLIB_LD='ld -shared -expect_unresolved "*"'
	    else
	        SHLIB_LD='ld -non_shared -expect_unresolved "*"'
	    fi
	    SHLIB_LD_LIBS=""
	    SHLIB_SUFFIX=".so"
	    DL_OBJS="tclLoadDl.o"
	    DL_LIBS=""
	    LDFLAGS=""
	    CC_SEARCH_FLAGS='-Wl,-rpath,${LIB_RUNTIME_DIR}'
	    LD_SEARCH_FLAGS='-rpath ${LIB_RUNTIME_DIR}'
	    if test "$GCC" != "yes" ; then
		EXTRA_CFLAGS="-DHAVE_TZSET -std1"
	    fi
	    # see pthread_intro(3) for pthread support on osf1, k.furukawa
	    if test "${TCL_THREADS}" = "1" ; then
		EXTRA_CFLAGS="${EXTRA_CFLAGS} -DHAVE_PTHREAD_ATTR_SETSTACKSIZE"
		EXTRA_CFLAGS="${EXTRA_CFLAGS} -DTCL_THREAD_STACK_MIN=PTHREAD_STACK_MIN*64"
		LIBS=`echo $LIBS | sed s/-lpthreads//`
		if test "$GCC" = "yes" ; then
		    LIBS="$LIBS -lpthread -lmach -lexc"
		else
		    EXTRA_CFLAGS="${EXTRA_CFLAGS} -pthread"
		    LDFLAGS="-pthread"
		fi
	    fi

	    ;;
	QNX-6*)
	    # QNX RTP
	    # This may work for all QNX, but it was only reported for v6.
	    SHLIB_CFLAGS="-fPIC"
	    SHLIB_LD="ld -Bshareable -x"
	    SHLIB_LD_LIBS=""
	    SHLIB_SUFFIX=".so"
	    DL_OBJS="tclLoadDl.o"
	    # dlopen is in -lc on QNX
	    DL_LIBS=""
	    LDFLAGS=""
	    CC_SEARCH_FLAGS=""
	    LD_SEARCH_FLAGS=""
	    ;;
	RISCos-*)
	    SHLIB_CFLAGS="-G 0"
	    SHLIB_LD="echo tclLdAout $CC \{$SHLIB_CFLAGS\} | `pwd`/tclsh -r -G 0"
	    SHLIB_LD_LIBS='${LIBS}'
	    SHLIB_SUFFIX=".a"
	    DL_OBJS="tclLoadAout.o"
	    DL_LIBS=""
	    LDFLAGS="-Wl,-D,08000000"
	    CC_SEARCH_FLAGS='-L${LIB_RUNTIME_DIR}'
	    LD_SEARCH_FLAGS=${CC_SEARCH_FLAGS}
	    ;;
	SCO_SV-3.2*)
	    # Note, dlopen is available only on SCO 3.2.5 and greater. However,
	    # this test works, since "uname -s" was non-standard in 3.2.4 and
	    # below.
	    if test "$GCC" = "yes" ; then
	    	SHLIB_CFLAGS="-fPIC -melf"
	    	LDFLAGS="-melf -Wl,-Bexport"
	    else
	    	SHLIB_CFLAGS="-Kpic -belf"
	    	LDFLAGS="-belf -Wl,-Bexport"
	    fi
	    SHLIB_LD="ld -G"
	    SHLIB_LD_LIBS=""
	    SHLIB_SUFFIX=".so"
	    DL_OBJS="tclLoadDl.o"
	    DL_LIBS=""
	    CC_SEARCH_FLAGS=""
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
	    CC_SEARCH_FLAGS=""
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
	    CC_SEARCH_FLAGS='-L${LIB_RUNTIME_DIR}'
	    LD_SEARCH_FLAGS=${CC_SEARCH_FLAGS}

	    # SunOS canno handle version numbers with dots in them in library
	    # specs, like -ltcl7.5, so use -ltcl75 instead. Also, it
	    # requires an extra version number at the end of .so file names.
	    # So, the library has to have a name like libtcl75.so.1.0

	    SHARED_LIB_SUFFIX='${TCL_TRIM_DOTS}\$\{DBGX\}.so.1.0'
	    UNSHARED_LIB_SUFFIX='${TCL_TRIM_DOTS}\$\{DBGX\}.a'
	    TCL_LIB_VERSIONS_OK=nodots
	    ;;
	SunOS-5.[[0-6]]*)

	    # Note: If _REENTRANT is NOT defined, then Solaris
	    # will NOT define thread-safe library routines.

	    AC_DEFINE([_REENTRANT],[1],[If this is not defined, then Solaris will not define thread-safe library routines.])
	    AC_DEFINE([_POSIX_PTHREAD_SEMANTICS],[1],[Define to 1 to use POSIX pthread semantics])

	    SHLIB_CFLAGS="-KPIC"

	    # Note: need the LIBS below, otherwise Tk will NOT find Tcl's
	    # symbols when dynamically loaded into tclsh.

	    SHLIB_LD_LIBS='${LIBS}'
	    SHLIB_SUFFIX=".so"
	    DL_OBJS="tclLoadDl.o"
	    DL_LIBS="-ldl"
	    LDFLAGS=""
	    if test "$GCC" = "yes" ; then
		SHLIB_LD="$CC -shared"
		CC_SEARCH_FLAGS='-Wl,-R,${LIB_RUNTIME_DIR}'
		LD_SEARCH_FLAGS=${CC_SEARCH_FLAGS}
	    else
		SHLIB_LD="/usr/ccs/bin/ld -G -z text"
		CC_SEARCH_FLAGS='-R ${LIB_RUNTIME_DIR}'
		LD_SEARCH_FLAGS=${CC_SEARCH_FLAGS}
	    fi
	    ;;
	SunOS-5*)

	    # Note: If _REENTRANT is NOT defined, then Solaris
	    # will NOT define thread-safe library routines.

	    AC_DEFINE([_REENTRANT],[1],[If this is not defined, then Solaris will not define thread-safe library routines.])
	    AC_DEFINE([_POSIX_PTHREAD_SEMANTICS],[1],[Define to 1 to use POSIX pthread semantics])

	    SHLIB_CFLAGS="-KPIC"
	    LDFLAGS=""
    
	    # Check to enable 64-bit flags for compiler/linker
	    if test "$do64bit" = "yes" ; then
		arch=`isainfo`
		if test "$arch" = "sparcv9 sparc" ; then
			if test "$GCC" = "yes" ; then
			    AC_MSG_WARN(["64bit mode not supported with GCC on $system"])
			else
			    do64bit_ok=yes
			    if test "$do64bitVIS" = "yes" ; then
				EXTRA_CFLAGS="-xarch=v9a"
			    	LDFLAGS="-xarch=v9a"
			    else
				EXTRA_CFLAGS="-xarch=v9"
			    	LDFLAGS="-xarch=v9"
			    fi
			fi
		else
		    AC_MSG_WARN(["64bit mode only supported sparcv9 system"])
		fi
	    fi
	    
	    # Note: need the LIBS below, otherwise Tk will NOT find Tcl's
	    # symbols when dynamically loaded into tclsh.

	    SHLIB_LD_LIBS='${LIBS}'
	    SHLIB_SUFFIX=".so"
	    DL_OBJS="tclLoadDl.o"
	    DL_LIBS="-ldl"
	    if test "$GCC" = "yes" ; then
		SHLIB_LD="$CC -shared"
		CC_SEARCH_FLAGS='-Wl,-R,${LIB_RUNTIME_DIR}'
		LD_SEARCH_FLAGS=${CC_SEARCH_FLAGS}
	    else
		SHLIB_LD="/usr/ccs/bin/ld -G -z text"
		CC_SEARCH_FLAGS='-Wl,-R,${LIB_RUNTIME_DIR}'
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
	    CC_SEARCH_FLAGS='-L${LIB_RUNTIME_DIR}'
	    LD_SEARCH_FLAGS=${CC_SEARCH_FLAGS}
	    if test "$GCC" != "yes" ; then
		EXTRA_CFLAGS="-DHAVE_TZSET -std1"
	    fi
	    ;;
	UNIX_SV* | UnixWare-5*)
	    SHLIB_CFLAGS="-KPIC"
	    SHLIB_LD="cc -G"
	    SHLIB_LD_LIBS=""
	    SHLIB_SUFFIX=".so"
	    DL_OBJS="tclLoadDl.o"
	    DL_LIBS="-ldl"
	    # Some UNIX_SV* systems (unixware 1.1.2 for example) have linkers
	    # that do NOT grok the -Bexport option. Test that it does.
	    hold_ldflags=$LDFLAGS
	    AC_MSG_CHECKING([for ld accepts -Bexport flag])
	    LDFLAGS="${LDFLAGS} -Wl,-Bexport"
	    AC_LINK_IFELSE([AC_LANG_SOURCE([[]],[[int i;]])],[found=yes],[found=no])
	    LDFLAGS=$hold_ldflags
	    AC_MSG_RESULT([$found])
	    if test $found = yes; then
	    LDFLAGS="-Wl,-Bexport"
	    else
	    LDFLAGS=""
	    fi
	    CC_SEARCH_FLAGS=""
	    LD_SEARCH_FLAGS=""
	    ;;
    esac

    if test "$do64bit" = "yes" -a "$do64bit_ok" = "no" ; then
    AC_MSG_WARN(["64bit support being disabled -- do NOT know magic for this platform"])
    fi

    # Step 4: If pseudo-static linking is in use (see K. B. Kenny, "Dynamic
    # Loading for Tcl -- What Became of It?". Proc. 2nd Tcl/Tk Workshop,
    # New Orleans, LA, Computerized Processes Unlimited, 1994), then we need
    # to determine which of several header files defines the a.out file
    # format (a.out.h, sys/exec.h, or sys/exec_aout.h). At present, we
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
	AC_MSG_CHECKING([sys/exec.h])
	AC_COMPILE_IFELSE([AC_LANG_SOURCE([[#include <sys/exec.h>]],[[
	    struct exec foo;
	    unsigned long seek;
	    int flag;
#if defined(__mips) || defined(mips)
	    seek = N_TXTOFF (foo.ex_f, foo.ex_o);
#else
	    seek = N_TXTOFF (foo);
#endif /* __mips || mips */
	    flag = (foo.a_magic == OMAGIC);
	    return foo.a_text + foo.a_data + foo.a_bss + foo.a_entry;
    ]])],[tcl_ok=usable],[tcl_ok=unusable])
	AC_MSG_RESULT([$tcl_ok])
	if test $tcl_ok = usable; then
	    AC_DEFINE([USE_SYS_EXEC_H],[1],[Define to 1 to use the sys/exec.h header])
	else
	    AC_MSG_CHECKING([a.out.h])
	    AC_COMPILE_IFELSE([AC_LANG_SOURCE([[#include <a.out.h>]],[[
		struct exec foo;
		unsigned long seek;
		int flag;
#if defined(__mips) || defined(mips)
		seek = N_TXTOFF (foo.ex_f, foo.ex_o);
#else
		seek = N_TXTOFF (foo);
#endif /* __mips || mips */
		flag = (foo.a_magic == OMAGIC);
		return foo.a_text + foo.a_data + foo.a_bss + foo.a_entry;
	    ]])],[tcl_ok=usable],[tcl_ok=unusable])
	    AC_MSG_RESULT([$tcl_ok])
	    if test $tcl_ok = usable; then
		AC_DEFINE([USE_A_OUT_H],[1],[Define to 1 to use a.out.h])
	    else
		AC_MSG_CHECKING([sys/exec_aout.h])
		AC_COMPILE_IFELSE([AC_LANG_SOURCE([[#include <sys/exec_aout.h>]],[[
		    struct exec foo;
		    unsigned long seek;
		    int flag;
#if defined(__mips) || defined(mips)
		    seek = N_TXTOFF (foo.ex_f, foo.ex_o);
#else
		    seek = N_TXTOFF (foo);
#endif /* __mips || mips */
		    flag = (foo.a_midmag == OMAGIC);
		    return foo.a_text + foo.a_data + foo.a_bss + foo.a_entry;
		]])],[tcl_ok=usable],[tcl_ok=unusable])
		AC_MSG_RESULT([$tcl_ok])
		if test $tcl_ok = usable; then
		    AC_DEFINE([USE_SYS_EXEC_AOUT_H],[1],[Define to 1 to use sys/exec_aout.h])
		else
		    DL_OBJS=""
		fi
	    fi
	fi
    fi

    # Step 5: disable dynamic loading if requested via a command-line switch.

    AC_ARG_ENABLE([load],[AS_HELP_STRING([--disable-load],[disallow dynamic loading and "load" command])],
	[tcl_ok=$enableval],[tcl_ok=yes])
    if test "$tcl_ok" = "no"; then
	DL_OBJS=""
    fi

    if test "x$DL_OBJS" != "x" ; then
	BUILD_DLTEST="\$(DLTEST_TARGETS)"
    else
	echo "Cannot figure out how to do dynamic loading or shared libraries"
	echo "on this system."
	SHLIB_CFLAGS=""
	SHLIB_LD=""
	SHLIB_SUFFIX=""
	DL_OBJS="tclLoadNone.o"
	DL_LIBS=""
	LDFLAGS=""
	CC_SEARCH_FLAGS=""
	LD_SEARCH_FLAGS=""
	BUILD_DLTEST=""
    fi

    # If we are running gcc, then change the C flags for compiling shared
    # libraries to the right flags for gcc, instead of those for the
    # standard manufacturer compiler.

    if test "$DL_OBJS" != "tclLoadNone.o" ; then
	if test "$GCC" = "yes" ; then
	    case $system in
		AIX-*)
		    ;;
		BSD/OS*)
		    ;;
		IRIX*)
		    ;;
		NetBSD-*|FreeBSD-*|OpenBSD-*)
		    ;;
		Rhapsody-*|Darwin-*)
		    ;;
		RISCos-*)
		    ;;
		SCO_SV-3.2*)
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

    if test "${SHARED_BUILD}" = "1" && test "${SHLIB_SUFFIX}" != "" ; then
        LIB_SUFFIX=${SHARED_LIB_SUFFIX}
        MAKE_LIB='${SHLIB_LD} -o [$]@ ${SHLIB_LD_FLAGS} ${OBJS} ${SHLIB_LD_LIBS} ${TCL_SHLIB_LD_EXTRAS} ${TK_SHLIB_LD_EXTRAS} ${LD_SEARCH_FLAGS}'
        INSTALL_LIB='$(INSTALL_LIBRARY) $(LIB_FILE) $(LIB_INSTALL_DIR)/$(LIB_FILE)'
    else
        LIB_SUFFIX=${UNSHARED_LIB_SUFFIX}

        if test "$RANLIB" = "" ; then
            MAKE_LIB='$(STLIB_LD) [$]@ ${OBJS}'
            INSTALL_LIB='$(INSTALL_LIBRARY) $(LIB_FILE) $(LIB_INSTALL_DIR)/$(LIB_FILE)'
        else
            MAKE_LIB='${STLIB_LD} [$]@ ${OBJS} ; ${RANLIB} [$]@'
            INSTALL_LIB='$(INSTALL_LIBRARY) $(LIB_FILE) $(LIB_INSTALL_DIR)/$(LIB_FILE) ; (cd $(LIB_INSTALL_DIR) ; $(RANLIB) $(LIB_FILE))'
        fi
    fi


    # Stub lib does not depend on shared/static configuration
    if test "$RANLIB" = "" ; then
        MAKE_STUB_LIB='${STLIB_LD} [$]@ ${STUB_LIB_OBJS}'
        INSTALL_STUB_LIB='$(INSTALL_LIBRARY) $(STUB_LIB_FILE) $(LIB_INSTALL_DIR)/$(STUB_LIB_FILE)'
    else
        MAKE_STUB_LIB='${STLIB_LD} [$]@ ${STUB_LIB_OBJS} ; ${RANLIB} [$]@'
        INSTALL_STUB_LIB='$(INSTALL_LIBRARY) $(STUB_LIB_FILE) $(LIB_INSTALL_DIR)/$(STUB_LIB_FILE) ; (cd $(LIB_INSTALL_DIR) ; $(RANLIB) $(STUB_LIB_FILE))'
    fi

    AC_SUBST([DL_LIBS])dnl

    AC_SUBST([DL_OBJS])dnl
    AC_SUBST([PLAT_OBJS])dnl
    AC_SUBST([CFLAGS])dnl
    AC_SUBST([CFLAGS_DEBUG])dnl
    AC_SUBST([CFLAGS_OPTIMIZE])dnl
    AC_SUBST([CFLAGS_WARNING])dnl
    AC_SUBST([EXTRA_CFLAGS])dnl

    AC_SUBST([LDFLAGS])dnl
    AC_SUBST([LDFLAGS_DEBUG])dnl
    AC_SUBST([LDFLAGS_OPTIMIZE])dnl
    AC_SUBST([CC_SEARCH_FLAGS])dnl
    AC_SUBST([LD_SEARCH_FLAGS])dnl

    AC_SUBST([STLIB_LD])dnl
    AC_SUBST([SHLIB_LD])dnl
    AC_SUBST([TCL_SHLIB_LD_EXTRAS])dnl
    AC_SUBST([TK_SHLIB_LD_EXTRAS])dnl
    AC_SUBST([SHLIB_LD_FLAGS])dnl
    AC_SUBST([SHLIB_LD_LIBS])dnl
    AC_SUBST([SHLIB_CFLAGS])dnl
    AC_SUBST([SHLIB_SUFFIX])dnl

    AC_SUBST([MAKE_LIB])dnl
    AC_SUBST([MAKE_STUB_LIB])dnl
    AC_SUBST([INSTALL_LIB])dnl
    AC_SUBST([INSTALL_STUB_LIB])dnl
    AC_SUBST([RANLIB])dnl
])dnl

#--------------------------------------------------------------------
# SC_SERIAL_PORT
#
#	Determine which interface to use to talk to the serial port.
#	Note that #include lines must begin in leftmost column for
#	some compilers to recognize them as preprocessor directives,
#	and some build environments have stdin not pointing at a
#	pseudo-terminal (usually /dev/null instead.)
#
# Arguments:
#	none
#	
# Results:
#
#	Defines only one of the following vars:
#		HAVE_SYS_MODEM_H
#		USE_TERMIOS
#		USE_TERMIO
#		USE_SGTTY
#
#--------------------------------------------------------------------

AC_DEFUN([SC_SERIAL_PORT],[
    AC_CHECK_HEADERS([sys/modem.h])
    AC_MSG_CHECKING([termios vs. termio vs. sgtty])
    AC_CACHE_VAL([tcl_cv_api_serial],[
    AC_RUN_IFELSE([AC_LANG_SOURCE([[
#include <termios.h>

int main() {
    struct termios t;
    if (tcgetattr(0, &t) == 0) {
	cfsetospeed(&t, 0);
	t.c_cflag |= PARENB | PARODD | CSIZE | CSTOPB;
	return 0;
    }
    return 1;
}]])],[tcl_cv_api_serial=termios],[tcl_cv_api_serial=no],[tcl_cv_api_serial=no])
    if test $tcl_cv_api_serial = no ; then
	AC_RUN_IFELSE([AC_LANG_SOURCE([[
#include <termio.h>

int main() {
    struct termio t;
    if (ioctl(0, TCGETA, &t) == 0) {
	t.c_cflag |= CBAUD | PARENB | PARODD | CSIZE | CSTOPB;
	return 0;
    }
    return 1;
}]])],[tcl_cv_api_serial=termio],[tcl_cv_api_serial=no],[tcl_cv_api_serial=no])
    fi
    if test $tcl_cv_api_serial = no ; then
	AC_RUN_IFELSE([AC_LANG_SOURCE([[
#include <sgtty.h>

int main() {
    struct sgttyb t;
    if (ioctl(0, TIOCGETP, &t) == 0) {
	t.sg_ospeed = 0;
	t.sg_flags |= ODDP | EVENP | RAW;
	return 0;
    }
    return 1;
}]])],[tcl_cv_api_serial=sgtty],[tcl_cv_api_serial=no],[tcl_cv_api_serial=no])
    fi
    if test $tcl_cv_api_serial = no ; then
	AC_RUN_IFELSE([AC_LANG_SOURCE([[
#include <termios.h>
#include <errno.h>

int main() {
    struct termios t;
    if (tcgetattr(0, &t) == 0
	|| errno == ENOTTY || errno == ENXIO || errno == EINVAL) {
	cfsetospeed(&t, 0);
	t.c_cflag |= PARENB | PARODD | CSIZE | CSTOPB;
	return 0;
    }
    return 1;
}]])],[tcl_cv_api_serial=termios],[tcl_cv_api_serial=no],[tcl_cv_api_serial=no])
    fi
    if test $tcl_cv_api_serial = no; then
	AC_RUN_IFELSE([AC_LANG_SOURCE([[
#include <termio.h>
#include <errno.h>

int main() {
    struct termio t;
    if (ioctl(0, TCGETA, &t) == 0
	|| errno == ENOTTY || errno == ENXIO || errno == EINVAL) {
	t.c_cflag |= CBAUD | PARENB | PARODD | CSIZE | CSTOPB;
	return 0;
    }
    return 1;
    }]])],[tcl_cv_api_serial=termio],[tcl_cv_api_serial=no],[tcl_cv_api_serial=no])
    fi
    if test $tcl_cv_api_serial = no; then
	AC_RUN_IFELSE([AC_LANG_SOURCE([[
#include <sgtty.h>
#include <errno.h>

int main() {
    struct sgttyb t;
    if (ioctl(0, TIOCGETP, &t) == 0
	|| errno == ENOTTY || errno == ENXIO || errno == EINVAL) {
	t.sg_ospeed = 0;
	t.sg_flags |= ODDP | EVENP | RAW;
	return 0;
    }
    return 1;
}]])],[tcl_cv_api_serial=sgtty],[tcl_cv_api_serial=none],[tcl_cv_api_serial=none])
    fi])
    case $tcl_cv_api_serial in
	termios) AC_DEFINE([USE_TERMIOS],[1],[Define to 1 to use termios]);;
	termio)  AC_DEFINE([USE_TERMIO],[1],[Define to 1 to use termio]);;
	sgtty)   AC_DEFINE([USE_SGTTY],[1],[Define to 1 to use sgtty]);;
    esac
    AC_MSG_RESULT([$tcl_cv_api_serial])
])

#--------------------------------------------------------------------
# SC_MISSING_POSIX_HEADERS
#
#	Supply substitutes for missing POSIX header files. Special
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
    m4_ifdef([AC_HEADER_STDC],[
      AC_REQUIRE([AC_HEADER_STDC])dnl
    ])dnl
    AC_MSG_CHECKING([dirent.h])
    AC_LINK_IFELSE([AC_LANG_SOURCE([[#include <sys/types.h>
#include <dirent.h>]],[[
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
]])],[tcl_ok=yes],[tcl_ok=no])

    if test $tcl_ok = no; then
	AC_DEFINE([NO_DIRENT_H],[1],[Define to 1 if there is no working dirent.h header])
    fi

    AC_MSG_RESULT([$tcl_ok])
    AC_CHECK_HEADERS([errno.h],[],[AC_DEFINE([NO_ERRNO_H],[1],[Define to 1 if there is no errno.h])])
    AC_CHECK_HEADERS([float.h],[],[AC_DEFINE([NO_FLOAT_H],[1],[Define to 1 if there is no float.h])])
    AC_CHECK_HEADERS([values.h],[],[AC_DEFINE([NO_VALUES_H],[1],[Define to 1 if there is no values.h])])
    AC_CHECK_HEADERS([limits.h],[],[AC_DEFINE([NO_LIMITS_H],[1],[Define to 1 if there is no limits.h])])
    AC_CHECK_HEADERS([stdlib.h],[tcl_ok=1],[tcl_ok=0])
    AC_EGREP_HEADER([strtol],[stdlib.h],[],[tcl_ok=0])
    AC_EGREP_HEADER([strtoul],[stdlib.h],[],[tcl_ok=0])
    AC_EGREP_HEADER([strtod],[stdlib.h],[],[tcl_ok=0])
    if test $tcl_ok = 0; then
	AC_DEFINE([NO_STDLIB_H],[1],[Define to 1 if there is no stdlib.h])
    fi
    AC_CHECK_HEADERS([string.h],[tcl_ok=1],[tcl_ok=0])
    AC_REQUIRE([AC_PROG_EGREP])
    AC_EGREP_HEADER([strstr],[string.h],[],[tcl_ok=0])
    AC_EGREP_HEADER([strerror],[string.h],[],[tcl_ok=0])

    # See also memmove check below for a place where NO_STRING_H can be
    # set and why.

    if test $tcl_ok = 0; then
	AC_DEFINE([NO_STRING_H],[1],[Define to 1 if there is no string.h])
    fi

    AC_CHECK_HEADER([sys/wait.h],[],[AC_DEFINE([NO_SYS_WAIT_H],[1],[Define to 1 if there is no sys/wait.h])])
    AC_CHECK_HEADER([dlfcn.h],[],[AC_DEFINE([NO_DLFCN_H],[1],[Define to 1 if there is no dlfcn.h])])

    # OS/390 lacks sys/param.h (and does NOT need it, by chance).

    AC_CHECK_HEADERS([unistd.h sys/param.h])
])

#--------------------------------------------------------------------
# SC_PATH_X
#
#	Locate the X11 header files and the X11 library archive. Try
#	the ac_path_x macro first, but if it does NOT find the X stuff
#	(e.g. because there is no xmkmf program) then check through
#	a list of possible directories. Under some conditions the
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
    AC_REQUIRE([AC_PATH_XTRA])
    not_really_there=""
    if test "$no_x" = ""; then
	if test "$x_includes" = ""; then
	    AC_PREPROC_IFELSE([AC_LANG_SOURCE([[#include <X11/XIntrinsic.h>]])],[],[not_really_there="yes"])
	else
	    if test ! -r $x_includes/X11/Intrinsic.h; then
		not_really_there="yes"
	    fi
	fi
    fi
    if test "$no_x" = "yes" -o "$not_really_there" = "yes"; then
	AC_MSG_CHECKING([for X11 header files])
	found_xincludes="no"
	AC_PREPROC_IFELSE([AC_LANG_SOURCE([[#include <X11/Intrinsic.h>]])],[found_xincludes="yes"],[found_xincludes="no"])
	if test "$found_xincludes" = "no"; then
	    dirs="/usr/unsupported/include /usr/local/include /usr/X386/include /usr/X11R6/include /usr/X11R5/include /usr/include/X11R5 /usr/include/X11R4 /usr/openwin/include /usr/X11/include /usr/sww/include"
	    for i in $dirs ; do
		if test -r $i/X11/Intrinsic.h; then
		    AC_MSG_RESULT([$i])
		    XINCLUDES=" -I$i"
		    found_xincludes="yes"
		    break
		fi
	    done
	fi
    else
	if test "$x_includes" != ""; then
	    XINCLUDES="-I$x_includes"
	    found_xincludes="yes"
	fi
    fi
    if test found_xincludes = "no"; then
	AC_MSG_RESULT([could NOT find any!])
    fi

    if test "$no_x" = yes; then
	AC_MSG_CHECKING([for X11 libraries])
	XLIBSW=nope
	dirs="/usr/unsupported/lib /usr/local/lib /usr/X386/lib /usr/X11R6/lib /usr/X11R5/lib /usr/lib/X11R5 /usr/lib/X11R4 /usr/openwin/lib /usr/X11/lib /usr/sww/X11/lib"
	for i in $dirs ; do
	    if test -r $i/libX11.a -o -r $i/libX11.so -o -r $i/libX11.sl; then
		AC_MSG_RESULT([$i])
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
	AC_CHECK_LIB([Xwindow],[XCreateWindow],[XLIBSW=-lXwindow])
    fi
    if test "$XLIBSW" = nope ; then
	AC_MSG_RESULT([could NOT find any! Using -lX11.])
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
    AC_REQUIRE([AC_PROG_AWK])
    AC_CHECK_HEADERS([sys/ioctl.h])
    AC_CHECK_HEADERS([sys/filio.h])
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
		system=MP-RAS-`awk '{print $3}' /etc/.relid'`
	    fi
	    if test "`uname -s`" = "AIX" ; then
		system=AIX-`uname -v`.`uname -r`
	    fi
	fi
    fi
    case $system in
	# There used to be code here to use FIONBIO under AIX. However, it
	# was reported that FIONBIO does NOT work under AIX 3.2.5. Since
	# using O_NONBLOCK seems fine under AIX 4.*, I removed the FIONBIO
	# code (JO, 5/31/97).

	OSF*)
	    AC_DEFINE([USE_FIONBIO],[1],[Define to 1 to use FIONBIO])
	    AC_MSG_RESULT([FIONBIO])
	    ;;
	SunOS-4*)
	    AC_DEFINE([USE_FIONBIO],[1],[Define to 1 to use FIONBIO])
	    AC_MSG_RESULT([FIONBIO])
	    ;;
	ULTRIX-4.*)
	    AC_DEFINE([USE_FIONBIO],[1],[Define to 1 to use FIONBIO])
	    AC_MSG_RESULT([FIONBIO])
	    ;;
	*)
	    AC_MSG_RESULT([O_NONBLOCK])
	    ;;
    esac
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
    AC_CHECK_HEADERS([sys/time.h time.h])
    m4_ifdef([AC_HEADER_TIME],[
      AC_REQUIRE([AC_HEADER_TIME])dnl
    ])dnl
    AC_REQUIRE([AC_STRUCT_TIMEZONE])dnl
    AC_REQUIRE([AC_STRUCT_TM])dnl
    AC_REQUIRE([AC_FUNC_SELECT_ARGTYPES])dnl
    AC_REQUIRE([AC_FUNC_STRFTIME])dnl
    AC_REQUIRE([AC_FUNC_UTIME_NULL])dnl
    AC_CHECK_TYPES([time_t])dnl

    AC_CHECK_FUNCS([gmtime_r localtime_r])dnl

    AC_MSG_CHECKING([tm_tzadj in struct tm])
    AC_CACHE_VAL([tcl_cv_member_tm_tzadj],
	[AC_COMPILE_IFELSE([AC_LANG_SOURCE([[
#ifdef TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#elif defined(HAVE_TIME_H)
# include <time.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "This conftest expects <time.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* TIME_WITH_SYS_TIME || HAVE_TIME_H */
]],[[struct tm tm; tm.tm_tzadj;]])],
	    [tcl_cv_member_tm_tzadj=yes],[tcl_cv_member_tm_tzadj=no])])
    AC_MSG_RESULT([$tcl_cv_member_tm_tzadj])
    if test $tcl_cv_member_tm_tzadj = yes ; then
	AC_DEFINE([HAVE_TM_TZADJ],[1],[Define to 1 if we have tm_tzadj in struct tm])
    else
        AC_MSG_WARN([Your struct tm should really have tm_tzadj in it.])
    fi

    AC_MSG_CHECKING([tm_gmtoff in struct tm])
    AC_CACHE_VAL([tcl_cv_member_tm_gmtoff],
	[AC_COMPILE_IFELSE([AC_LANG_SOURCE([[#include <time.h>]],[[struct tm tm; tm.tm_gmtoff;]])],
	    [tcl_cv_member_tm_gmtoff=yes],[tcl_cv_member_tm_gmtoff=no])])
    AC_MSG_RESULT([$tcl_cv_member_tm_gmtoff])
    if test $tcl_cv_member_tm_gmtoff = yes ; then
	AC_DEFINE([HAVE_TM_GMTOFF],[1],[Define to 1 if we have tm_gmtoff in struct tm])
    fi

    #
    # It is important to include time.h in this check, as some systems
    # (like convex) have timezone functions, etc.
    #
    AC_MSG_CHECKING([long timezone variable])
    AC_CACHE_VAL([tcl_cv_var_timezone],
	[AC_COMPILE_IFELSE([AC_LANG_SOURCE([[#include <stdlib.h>
	    #include <time.h>]],
	    [[extern long timezone;
	    timezone += 1;
	    exit (0);]])],
	    [tcl_cv_timezone_long=yes],[tcl_cv_timezone_long=no])])
    AC_MSG_RESULT([$tcl_cv_timezone_long])
    if test $tcl_cv_timezone_long = yes ; then
	AC_DEFINE([HAVE_TIMEZONE_VAR],[1],[Define to 1 if we have a time_t timezone variable])
    else
	#
	# On some systems (eg IRIX 6.2), timezone is a time_t and not a long.
	#
	AC_MSG_CHECKING([time_t timezone variable])
	AC_CACHE_VAL([tcl_cv_timezone_time],
	    [AC_COMPILE_IFELSE([AC_LANG_SOURCE([[#include <stdlib.h>
	    #include <time.h>]],
		[[extern time_t timezone;
		timezone += 1;
		exit (0);]])],
		[tcl_cv_timezone_time=yes],[tcl_cv_timezone_time=no])])
	AC_MSG_RESULT([$tcl_cv_timezone_time])
	if test $tcl_cv_timezone_time = yes ; then
	    AC_DEFINE([HAVE_TIMEZONE_VAR],[1],[Define to 1 if we have a time_t timezone variable])
	fi
    fi
])

#--------------------------------------------------------------------
# SC_BUGGY_STRTOD
#
#	Under Solaris 2.4, strtod returns the wrong value for the
#	terminating character under some conditions. Check for this
#	and if the problem exists use a substitute procedure
#	"fixstrtod" (provided by Tcl) that corrects the error.
#	Also, on Compaq's Tru64 Unix 5.0,
#	strtod(" ") returns 0.0 instead of a failure to convert.
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
    AC_CHECK_FUNCS([strtod],[tcl_strtod=1],[tcl_strtod=0])
    if test "$tcl_strtod" = 1; then
	AC_MSG_CHECKING([for Solaris2.4/Tru64 strtod bugs])
	AC_CACHE_VAL([tcl_cv_strtod_buggy],[
	    AC_RUN_IFELSE([AC_LANG_SOURCE([[
	    #if defined(HAVE_STDLIB_H) || defined(STDC_HEADERS) || defined(__STDC__)
	    # include <stdlib.h>
	    #else
	    # if !defined(NO_STDLIB_H) && defined(__GNUC__) && !defined(__STRICT_ANSI__)
	    #  warning "this conftest for strtod expects <stdlib.h> to be included."
	    # endif /* !NO_STDLIB_H && __GNUC__ && !__STRICT_ANSI__ */
	    #endif /* HAVE_STDLIB_H || STDC_HEADERS || __STDC__ */
		extern double strtod();
		int main(void) {
		    char *infString="Inf", *nanString="NaN", *spaceString=" ";
		    char *term;
		    double value;
		    value = strtod(infString, &term);
		    if ((term != infString) && (term[-1] == 0)) {
			exit(1);
		    }
		    value = strtod(nanString, &term);
		    if ((term != nanString) && (term[-1] == 0)) {
			exit(1);
		    }
		    value = strtod(spaceString, &term);
		    if (term == (spaceString+1)) {
			exit(1);
		    }
		    exit(0);
		}]])],[tcl_cv_strtod_buggy=1],[tcl_cv_strtod_buggy=0],[tcl_cv_strtod_buggy=0])])
	if test "$tcl_cv_strtod_buggy" = 1; then
	    AC_MSG_RESULT([ok])
	else
	    AC_MSG_RESULT([buggy; redefining as fixstrtod])
	    LIBOBJS="$LIBOBJS fixstrtod.o"
	    AC_DEFINE([strtod],[fixstrtod],[Define to fixstrtod if the original strtod is buggy])
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
    # already in libc.a. Set compiler flags accordingly.
    # Also, Linux requires the "ieee" library for math to work
    # right (and it must appear before "-lm").
    #--------------------------------------------------------------------

    AC_CHECK_FUNCS([sin],[MATH_LIBS=""],[MATH_LIBS="-lm"])
    AC_CHECK_LIB([ieee],[main],[MATH_LIBS="-lieee $MATH_LIBS"])

    #--------------------------------------------------------------------
    # Interactive UNIX requires -linet instead of -lsocket, plus it
    # needs net/errno.h to define the socket-related error codes.
    #--------------------------------------------------------------------

    AC_CHECK_LIB([inet],[main],[LIBS="$LIBS -linet"])
    AC_CHECK_HEADERS([net/errno.h],[AC_DEFINE([HAVE_NET_ERRNO_H],[1],[Define to 1 if we have the net/errno.h header])])

    #--------------------------------------------------------------------
    #	Check for the existence of the -lsocket and -lnsl libraries.
    #	The order here is important, so that they end up in the right
    #	order in the command line generated by make. Here are some
    #	special considerations:
    #	1. Use "connect" and "accept" to check for -lsocket, and
    #	   "gethostbyname" to check for -lnsl.
    #	2. Use each function name only once: cannot redo a check because
    #	   autoconf caches the results of the last check and will NOT redo it.
    #	3. Use -lnsl and -lsocket only if they supply procedures that
    #	   are NOT already present in the normal libraries. This is because
    #	   IRIX 5.2 has libraries, but they are NOT needed and they are
    #	   bogus: they goof up name resolution if used.
    #	4. On some SVR4 systems, cannot use -lsocket without -lnsl too.
    #	   To get around this problem, check for both libraries together
    #	   if -lsocket does NOT work by itself.
    #--------------------------------------------------------------------

    tcl_checkBoth=0
    AC_CHECK_FUNCS([connect],[tcl_checkSocket=0],[tcl_checkSocket=1])
    if test "$tcl_checkSocket" = 1; then
	AC_CHECK_FUNCS([setsockopt],[],[AC_CHECK_LIB([socket],[setsockopt],
	    [LIBS="$LIBS -lsocket"],[tcl_checkBoth=1])])
    fi
    if test "$tcl_checkBoth" = 1; then
	tk_oldLibs=$LIBS
	LIBS="$LIBS -lsocket -lnsl"
	AC_CHECK_FUNCS([accept],[tcl_checkNsl=0],[LIBS=$tk_oldLibs])
    fi
    AC_CHECK_FUNCS([gethostbyname],[],[AC_CHECK_LIB([nsl],[gethostbyname],
	    [LIBS="$LIBS -lnsl"])])
    
    # Do NOT perform the eval of the libraries here because DL_LIBS
    # will NOT be set until we call SC_CONFIG_CFLAGS

    TCL_LIBS='${DL_LIBS} ${LIBS} ${MATH_LIBS}'
    AC_SUBST([TCL_LIBS])
    AC_SUBST([MATH_LIBS])
])

#--------------------------------------------------------------------
# SC_TCL_EARLY_FLAGS
#
#	Check for what flags are needed to be passed so the correct OS
#	features are available.
#
# Arguments:
#	None
#	
# Results:
#
#	Might define the following vars:
#		_ISOC99_SOURCE
#		_LARGEFILE64_SOURCE
#
#--------------------------------------------------------------------

AC_DEFUN([SC_TCL_EARLY_FLAG],[
    AC_CACHE_VAL([tcl_cv_flag_]translit($1,[A-Z],[a-z]),
	AC_TRY_COMPILE([$2],[$3],[tcl_cv_flag_]translit($1,[A-Z],[a-z])=no,
	    AC_TRY_COMPILE([[#define ]$1[ 1
]$2],[$3],
		[tcl_cv_flag_]translit($1,[A-Z],[a-z])=yes,
		[tcl_cv_flag_]translit($1,[A-Z],[a-z])=no)))
    if test ["x${tcl_cv_flag_]translit($1,[A-Z],[a-z])[}" = "xyes"] ; then
	AC_DEFINE([$1],[1],[Define to 1 because ???])
	tcl_flags="$tcl_flags $1"
    fi])

AC_DEFUN([SC_TCL_EARLY_FLAGS],[
    AC_MSG_CHECKING([for required early compiler flags])
    tcl_flags=""
    SC_TCL_EARLY_FLAG([_ISOC99_SOURCE],[#include <stdlib.h>],
	[char *p = (char *)strtoll; char *q = (char *)strtoull;])
    SC_TCL_EARLY_FLAG([_LARGEFILE64_SOURCE],[#include <sys/stat.h>],
	[struct stat64 buf; int i = stat64("/", &buf);])
    if test "x${tcl_flags}" = "x" ; then
	AC_MSG_RESULT([none])
    else
	AC_MSG_RESULT([${tcl_flags}])
    fi
])

#--------------------------------------------------------------------
# SC_TCL_64BIT_FLAGS
#
#	Check for what is defined in the way of 64-bit features.
#
# Arguments:
#	None
#	
# Results:
#
#	Might define the following vars:
#		TCL_WIDE_INT_IS_LONG
#		TCL_WIDE_INT_TYPE
#		HAVE_STRUCT_DIRENT64
#		HAVE_STRUCT_STAT64
#		HAVE_TYPE_OFF64_T
#
#--------------------------------------------------------------------

AC_DEFUN([SC_TCL_64BIT_FLAGS],[
    AC_MSG_CHECKING([for 64-bit integer type])
    AC_CACHE_VAL([tcl_cv_type_64bit],[
	tcl_cv_type_64bit=none
	# See if the compiler knows natively about __int64
	AC_COMPILE_IFELSE([AC_LANG_SOURCE([[]],[[__int64 value = (__int64) 0;]])],
	    [tcl_type_64bit=__int64],[tcl_type_64bit="long long"])
	# See if we should use long anyway  Note that we substitute in the
	# type that is our current guess for a 64-bit type inside this check
	# program, so it should be modified only carefully...
	AC_RUN_IFELSE([AC_LANG_SOURCE([[#include <stdlib.h>
	    #include <unistd.h>
	    int main(void) {exit(!(sizeof(]${tcl_type_64bit}[) > sizeof(long)));}
	    ]])],[tcl_cv_type_64bit=${tcl_type_64bit}],[:],[:])])
    if test "${tcl_cv_type_64bit}" = none ; then
	AC_DEFINE([TCL_WIDE_INT_IS_LONG],[1],[Define to 1 if wide int is long])
	AC_MSG_RESULT([using long])
    else
	AC_DEFINE_UNQUOTED([TCL_WIDE_INT_TYPE],[${tcl_cv_type_64bit}],[The wide int type])
	AC_MSG_RESULT([${tcl_cv_type_64bit}])

	# Now check for auxiliary declarations
	AC_MSG_CHECKING([for struct dirent64])
	AC_CACHE_VAL([tcl_cv_struct_dirent64],[
	    AC_COMPILE_IFELSE([AC_LANG_SOURCE([[#include <sys/types.h>
#include <sys/dirent.h>]],[[struct dirent64 p;]])],
		[tcl_cv_struct_dirent64=yes],[tcl_cv_struct_dirent64=no])])
	if test "x${tcl_cv_struct_dirent64}" = "xyes" ; then
	    AC_DEFINE([HAVE_STRUCT_DIRENT64],[1],[Define to 1 if we have the struct dirent64])
	fi
	AC_MSG_RESULT([${tcl_cv_struct_dirent64}])

	AC_MSG_CHECKING([for struct stat64])
	AC_CACHE_VAL([tcl_cv_struct_stat64],[
	    AC_COMPILE_IFELSE([AC_LANG_SOURCE([[#include <sys/stat.h>]],[[struct stat64 p;
]])],
		[tcl_cv_struct_stat64=yes],[tcl_cv_struct_stat64=no])])
	if test "x${tcl_cv_struct_stat64}" = "xyes" ; then
	    AC_DEFINE([HAVE_STRUCT_STAT64],[1],[Define to 1 if we have the struct stat64])
	fi
	AC_MSG_RESULT([${tcl_cv_struct_stat64}])

	AC_MSG_CHECKING([for off64_t])
	AC_CACHE_VAL([tcl_cv_type_off64_t],[
	    AC_COMPILE_IFELSE([AC_LANG_SOURCE([[#include <sys/types.h>]],[[off64_t offset;
]])],
		[tcl_cv_type_off64_t=yes],[tcl_cv_type_off64_t=no])])
	if test "x${tcl_cv_type_off64_t}" = "xyes" ; then
	    AC_DEFINE([HAVE_TYPE_OFF64_T],[1],[Define to 1 if we have the off64_t type])
	fi
	AC_MSG_RESULT([${tcl_cv_type_off64_t}])
    fi
])
