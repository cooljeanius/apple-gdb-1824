#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

# while Expect is in alpha/beta, disable caching so as not to confuse
# people trying to fix configure bugs
dnl# CYGNUS LOCAL: allow caching
define([AC_CACHE_LOAD],[])
define([AC_CACHE_SAVE],[])
dnl# END CYGNUS LOCAL

m4_include([../config/warnings.m4])dnl

AC_PREREQ([2.69])
AC_INIT([expect],[5.26],[libes@nist.gov])
AC_CONFIG_SRCDIR([expect.h])dnl

dnl# CYGNUS LOCAL: find aux files in ..
AC_CONFIG_AUX_DIR([..])
dnl# END CYGNUS LOCAL

# note when updating version numbers here, also update pkgIndex.in (see
# comments in Makefile)
EXP_MAJOR_VERSION=5
EXP_MINOR_VERSION=26
EXP_MICRO_VERSION=0
EXP_VERSION="${EXP_MAJOR_VERSION}.${EXP_MINOR_VERSION}"
EXP_VERSION_NODOTS="${EXP_MAJOR_VERSION}${EXP_MINOR_VERSION}"
EXP_VERSION_FULL="${EXP_VERSION}.${EXP_MICRO_VERSION}"

# Tcl's handling of shared_lib_suffix requires this symbol exist
VERSION="${EXP_MAJOR_VERSION}.${EXP_MINOR_VERSION}"

# Too many people send me configure output without identifying the version.
# This forced identification should reduce my pain significantly.
echo "configuring Expect ${EXP_MAJOR_VERSION}.${EXP_MINOR_VERSION}.${EXP_MICRO_VERSION}"

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

AC_USE_SYSTEM_EXTENSIONS
AC_EXEEXT

if test -n "${HOST_ARCHITECTURE}"; then
  ACX_PROG_CC_WARNING_OPTS(["-arch ${HOST_ARCHITECTURE}"],[C_ARCHFLAGS])
  AC_SUBST([C_ARCHFLAGS])
fi

ACX_PROG_CC_WARNING_OPTS([-Wreturn-type -Wfatal-errors])

AC_CONFIG_HEADERS([expect_cf.h])dnl

AC_PROG_SED

if test -e `pwd`/config.cache; then
  sed -i.bak -e "s|ac_cv_env_CFLAGS_value|bad_CFLAGS|g" `pwd`/config.cache
  sed -i.bak -e "s|ac_cv_prog_CC|bad_CC|g" `pwd`/config.cache
  sed -i.bak -e "s|ac_cv_prog_CPP|bad_CPP|g" `pwd`/config.cache
  sed -i.bak -e "s|ac_cv_env_build_alias|bad_build_alias|g" `pwd`/config.cache
  sed -i.bak -e "s|ac_cv_env_host_alias|bad_host_alias|g" `pwd`/config.cache
  sed -i.bak -e "s|ac_cv_env_target_alias|bad_target_alias|g" `pwd`/config.cache
elif test -e ./config.cache; then
  sed -i.bak -e "s|ac_cv_env_CFLAGS_value|bad_CFLAGS|g" ./config.cache
  sed -i.bak -e "s|ac_cv_prog_CC|bad_CC|g" ./config.cache
  sed -i.bak -e "s|ac_cv_prog_CPP|bad_CPP|g" ./config.cache
  sed -i.bak -e "s|ac_cv_env_build_alias|bad_build_alias|g" ./config.cache
  sed -i.bak -e "s|ac_cv_env_host_alias|bad_host_alias|g" ./config.cache
  sed -i.bak -e "s|ac_cv_env_target_alias|bad_target_alias|g" ./config.cache
else
  echo "config.cache is missing, which means we can skip attempts to hack it"
fi

# /bin/sh on some systems is too deficient (in particular, Ultrix 4.3
# sh lacks unset and we *need* that), but all these systems come with
# alternatives, so take user's choice or whatever we are using here and
# allow it to be seen by Make.
AC_SYS_INTERPRETER
AC_MSG_CHECKING([shell to use within Make])
EXP_CONFIG_SHELL=${CONFIG_SHELL-/bin/sh}
AC_MSG_RESULT([${CONFIG_SHELL}])
AC_SYS_LARGEFILE

# If `configure' is invoked (in)directly via `make', ensure that it
# encounters no `make' conflicts.
#
unset MFLAGS MAKEFLAGS
MFLAGS=""
MAKEFLAGS=""

CY_AC_PATH_TCLCONFIG
CY_AC_LOAD_TCLCONFIG
EXP_AND_TCL_LIBS="${EXP_AND_TCL_LIBS} ${TCL_LIBS}"
CY_AC_PATH_TKCONFIG
CY_AC_LOAD_TKCONFIG
EXP_AND_TK_LIBS="${EXP_AND_TK_LIBS} ${TK_LIBS}"

# CYGNUS LOCAL dj/cygwin
AC_CANONICAL_HOST
EXP_CFLAGS=""
# If we built a cygwin-specific tcl, use it here.
case "${host}" in
*-*-cygwin*)
  if test -d ${srcdir}/../tcl/cygwin/.
  then
    TCL_BUILD_LIB_SPEC="../tcl/cygwin/libtcl_cygwin.a"
    # Use the same static lib for installed_expect
    TCL_LIB_SPEC="../tcl/cygwin/libtcl_cygwin.a"
    EXP_CYGWIN_ALTTCL='-DCYGWIN_ALTTCL'
  fi
  touch ac$$.c
  if ${CC} -c -mno-win32 ac$$.c >/dev/null 2>&1; then
      case "$EXP_CFLAGS" in
	  *-mno-win32*) ;;
	  *) EXP_CFLAGS="-mno-win32 ${EXP_CFLAGS}" ;;
      esac
  fi
  rm -f ac$$.o ac$$.c
  ;;
esac

# An explanation is in order for the strange things going on with the
# various LIBS. There are three separate definitions for LIBS. The
# reason is that some systems require shared libraries include
# references to their dependent libraries, i.e., any additional
# libraries that must be linked to. And some systems get upset if the
# references are repeated on the link line. So therefore, we create
# one for Expect and Tk (EXP_AND_TK_LIBS), one for Expect and Tcl
# (EXP_AND_TCL_LIBS), and finally, one for building Expect's own
# shared library. Tcl's tclConfig.sh insists that any shared libs
# that it "helps" build must pass the libraries as LIBS (see comment
# near end of this configure file). I would do but since we are close
# to hitting config's max symbols, we take one short cut and pack the
# LIBS into EXP_SHLIB_LD_LIBS (which is basically what Tcl wants to do
# for us). The point, however, is that there is no separate LIBS or
# EXP_LIBS symbol passed out of configure. One additional point for
# confusion is that LIBS is what configure uses to do all library
# tests, so we have to swap definitions of LIBS peridically. When we
# are swapping out the one for Expect's shared library, we save it in
# EXP_LIBS. Sigh.

dnl# AC_PROG_CC insists on sticking crap -g and -O in CFLAGS
dnl# but I want to control it. Cannot just throw it out at the
dnl# end alas, because the user might have defined CFLAGS.
dnl# CYGNUS LOCAL: just use AC_PROG_CC; we do NOT care.
OLD_CFLAGS=${CFLAGS}
if test "x${CC}" = "x"; then
	test -z "${CC}"
	AC_PROG_CC
else
	test ! -z "${CC}"
	AC_PROG_GCC_TRADITIONAL
	AC_PROG_CPP
	CY_AC_C_WORKS
fi
CFLAGS=${OLD_CFLAGS}
dnl# END CYGNUS LOCAL

# this will use a BSD compatible install or our included install-sh
AC_PROG_INSTALL

# Tcl sets TCL_RANLIB appropriately for shared library if --enable-shared
AC_PROG_RANLIB
UNSHARED_RANLIB=${RANLIB}

# Other programs
if test "x${CPP}" = "x"; then
	test -z "${CPP}"
	AC_PROG_CPP
else
	test ! -z "${CPP}" && export CPP
	AC_SUBST([CPP])
fi
AC_PROG_CXX
AC_PROG_LN_S

# these are the other subdirectories we need to configure
if test -z "${build_alias}" -a -z "${host_alias}" -a -z "${target_alias}"; then
  AC_MSG_NOTICE([will recurse into the testsuite for Expect])
  AC_CONFIG_SUBDIRS([testsuite])
else
  test -d testsuite
  AC_MSG_NOTICE([the testsuite for Expect will have to be configured and run manually])
fi

# This is for LynxOS, which needs a flag to force true POSIX when
# building. The flag varies depending how old the compiler is.
# -X is for the old "cc" and "gcc" (based on 1.42)
# -mposix is for the new gcc (at least 2.5.8)
# This modifies the value of $CC to have the POSIX flag added
# so it will configure correctly.
CY_AC_TCL_LYNX_POSIX

AC_TYPE_PID_T
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

AC_HEADER_TIME
AC_HEADER_SYS_WAIT

EXP_CFLAGS="${EXP_CFLAGS} ${EXP_CYGWIN_ALTTCL}"

AC_MSG_CHECKING([if running Mach])
mach=0
case "${host}" in
  # Both Next and pure Mach behave identically with respect
  # to a few things, so just lump them together as "mach"
  *-*-mach*)		mach=1 ;;
  *-*-next*)		mach=1 ; next=1 ;;
esac

if test $mach -eq 1 ; then
  AC_MSG_RESULT([yes])
else
  AC_MSG_RESULT([no])
fi

AC_MSG_CHECKING([if running MachTen])
# yet another Mach clone (for Classic MacOS)
if test -r /MachTen -a "${cross_compiling}" != "yes" ; then
  AC_MSG_RESULT([yes])
  mach=1
else
  AC_MSG_RESULT([no])
fi

AC_MSG_CHECKING([if on Pyramid])
if test -r /bin/pyr -a "${cross_compiling}" != "yes" ; then
  AC_MSG_RESULT([yes])
  pyr=1
else
  AC_MSG_RESULT([no])
  pyr=0
fi

AC_MSG_CHECKING([if on Apollo])
if test -r /usr/apollo/bin -a "${cross_compiling}" != "yes" ; then
  AC_MSG_RESULT([yes])
  apollo=1
else
  AC_MSG_RESULT([no])
  apollo=0
fi

AC_MSG_CHECKING([if on Interactive])
if test "x`(uname -s) 2>/dev/null`" = xIUNIX; then
  AC_MSG_RESULT([yes])
  iunix=1
else
  AC_MSG_RESULT([no])
  iunix=0
fi

AC_MSG_CHECKING([if stty reads stdout])

# On some systems stty cannot be run in the background (svr4) or get it
# wrong because they fail to complain (NeXT, mach), so do NOT attempt
# the test on some systems.

stty_reads_stdout=""
case "${host}" in
  *-*-solaris*)		stty_reads_stdout=0 ;;
  *-*-irix*)		stty_reads_stdout=0 ;;
  *-*-sco3.2v[[45]]*)	stty_reads_stdout=1 ;;
  i[[3456]]86-*-sysv4.2MP)	stty_reads_stdout=0 ;;
  i[[3456]]86-*-linux*)	stty_reads_stdout=0 ;;
  # Not sure about old convex but 5.2 definitely reads from stdout
  c[[12]]-*-*)		stty_reads_stdout=1 ;;
  *-*-aix[[34]]*)	stty_reads_stdout=0 ;;
  *-*-hpux9*)		stty_reads_stdout=0 ;;
  *-*-hpux10*)		stty_reads_stdout=0 ;;
  *-*-hpux11*)		stty_reads_stdout=0 ;;
  *-*-osf[[234]]*)	stty_reads_stdout=0 ;;
  *-*-ultrix4.4)	stty_reads_stdout=0 ;;
  *-*-dgux*)		stty_reads_stdout=0 ;;
  *-*-cygwin*)		stty_reads_stdout=0 ;;
esac

if test ${mach} -eq 1 ; then
  stty_reads_stdout=1
fi
if test ${apollo} -eq 1 ; then
  stty_reads_stdout=1
fi
if test ${pyr} -eq 1 ; then
  stty_reads_stdout=1
fi

# if we still do NOT know, test
if test x"${stty_reads_stdout}" = x"" ; then
  /bin/stty > /dev/null 2> /dev/null
  if test $? -ne 0 ; then
    stty_reads_stdout=1
  else
    stty_reads_stdout=0
  fi
fi

if test ${stty_reads_stdout} -eq 1 ; then
  AC_MSG_RESULT([yes])
  AC_DEFINE([STTY_READS_STDOUT],[1],[Define to 1 if stty reads stdout])
else
  AC_MSG_RESULT([no])
fi

# Solaris 2.4 and later requires __EXTENSIONS__ in order to see all sorts
# of traditional but nonstandard stuff in header files.
# (see also AC_USE_SYSTEM_EXTENSIONS macro used above)
AC_MSG_CHECKING([if running Solaris])
solaris=0
case "${host}" in
  *-*-solaris*)		solaris=1;;
esac

if test ${solaris} -eq 1 ; then
  AC_MSG_RESULT([yes])
  AC_DEFINE([SOLARIS],[1],[Define to 1 for Solaris])
else
  AC_MSG_RESULT([no])
fi

# Check for a regex library:
AC_SEARCH_LIBS([regcomp],[iberty regex gregex gnuregex rx grx gnurx gnu c])
AC_SEARCH_LIBS([regexec],[iberty regex gregex gnuregex rx grx gnurx gnu c])

# On a few systems, libm.a is the same as libc.a
# Do NOT bother to test against Tcl and Tk libs, they always include -lm
AC_CHECK_LIB([m],[sqrt])
AC_CHECK_FUNC([sin],[],[LIBS="${LIBS} -lm"])

# On Interactive UNIX, -Xp must be added to LIBS in order to find strftime.
# This test should really be done by Tcl. So just check Tcl's definition.
# If defective, add to all three LIBS. (It is not actually necessary for
# EXP_LIBS since -Xp will just be ignored the way that EXP_LIBS is used in
# the Makefile, but we include it for consistency.)
if test ${iunix} -eq 1 ; then
  EXP_LIBS=${LIBS}
  LIBS=${EXP_AND_TCL_LIBS}
  AC_CHECK_FUNC([strftime],[],[
	EXP_LIBS="${LIBS} -Xp" 
	EXP_AND_TCL_LIBS="${LIBS} -Xp" 
	EXP_AND_TK_LIBS="${LIBS} -Xp" 
	])
  LIBS=EXP_LIBS
fi

#
# Ok, let us find the tcl source trees so we can use the headers
#
CY_AC_PATH_TCLH
if test x"${no_tcl}" = x"true" ; then
    echo "	ERROR: Cannot find Tcl headers or library."
    echo "	See README for information on how to obtain Tcl."
    echo "	If Tcl is installed, see INSTALL on how to tell"
    echo "	configure where Tcl is installed."
    exit 1
fi
CY_AC_PATH_ITCLH

# have to whether we are generating shared libs before configuring debugger
AC_MSG_CHECKING([if generating shared or nonshared library])
AC_ARG_ENABLE([shared],
	[AS_HELP_STRING([--enable-shared],
                        [build libexpect as a shared library])],
	[enable_shared=${enableval}],[enable_shared=no])
if test "x${enable_shared}" = "xyes" && test "x${TCL_SHLIB_SUFFIX}" != "x"; then
  AC_MSG_RESULT([both shared and unshared])
else
  AC_MSG_RESULT([unshared])
fi

# Now that we have found the Tcl sources, configure the debugger.
# This is a little tricky because it has its own configure script
# which produces a Makefile and cf file. We only want the cf file,
# so switch to a temporary directory and run the debugger's configure.
# Then save the cf file and delete the rest.
#
# Incidentally, the debugger cannot depend on Expect's cf file, because
# the debugger is designed to be independent of Expect.
#

export verbose=1

test -n "${verbose}" && echo "configuring Tcl debugger"
tmpdir=./Dbg$$
mkdir ${tmpdir}

if test "${enable_shared}" = "yes"; then
  dbg_config_flags='--enable-shared'
else
  dbg_config_flags='--disable-shared'
fi
# (cd;pwd) in next several commands converts relative dirs to absolute.
# This is required because the debugger src is at a different level in
# the filesystem than Expect src (where we are presently), thereby
# making the relative pathnames incorrect.
if test "x${with_tclconfig}" != "x"; then
  dbg_config_flags="${dbg_config_flags} --with-tclconfig=`(cd ${with_tclconfig}; pwd)`"
fi
if test "x${with_tcllibdir}" != "x"; then
  dbg_config_flags="${dbg_config_flags} --with-tcllibdir=`(cd ${with_tcllibdir}; pwd)`"
fi
if test "x${with_tcllib}" != "x"; then
  dbg_config_flags="${dbg_config_flags} --with-tcllib=`(cd ${with_tcllib}; pwd)`"
fi
if test "x${with_tclinclude}" != "x"; then
  dbg_config_flags="${dbg_config_flags} --with-tclinclude=`(cd ${with_tclinclude}; pwd)`"
fi
case "$cache_file" in
 /*)
    dbg_config_flags="${dbg_config_flags} --cache-file=${cache_file}"
    ;;
 *)
    dbg_config_flags="${dbg_config_flags} --cache-file=../${cache_file}"
    ;;
esac

cp -v ${srcdir}/Dbgconfigure ${srcdir}/Dbgconfigure.sh ${srcdir}/Dbg.h ${srcdir}/Dbg_cf.h.in ${srcdir}/install-sh ${tmpdir}
cp -v ${srcdir}/DbgMkfl.in ${tmpdir}/Makefile.in
cp -v ${srcdir}/DbgMkfl.in ${tmpdir}
cp -v ${srcdir}/DbgpkgInd.in ${tmpdir}
OLD_CPP="${CPP}"
unset CPP
test -n "${verbose}" && echo "(cd ${tmpdir}; ${CONFIG_SHELL-/bin/sh} Dbgconfigure --with-tclinclude=`echo ${TCLHDIR} | sed -e 's/-I//'` ${dbg_config_flags})"
  (cd ${tmpdir}; ${CONFIG_SHELL-/bin/sh} Dbgconfigure --with-tclinclude=`echo ${TCLHDIR} | sed -e 's/-I//'` ${dbg_config_flags})
CPP="${OLD_CPP}"
test -e ${tmpdir}/config.status && cp -v ${tmpdir}/config.status ./Dbgconfig.status
test -e ${tmpdir}/Dbg_cf.h && cp -v ${tmpdir}/Dbg_cf.h .
test -e ${tmpdir}/DbgMkfl && cp -v ${tmpdir}/DbgMkfl .
rm -rf ${tmpdir}
test -n "${verbose}" && echo "configured Tcl debugger"

# some people would complain if this explanation had not been provided...

echo "Begin tests for function/library dependencies. Tests may be repeated"
echo "up to three times. 1st test is for building Expect's shared library."
echo "2nd set is for building with Tcl. 3rd is for building with Tk."

######################################################################
# required by Sequent ptx2
unset ac_cv_func_gethostname
AC_CHECK_FUNC([gethostname],[gethostname=1],[gethostname=0])
if test ${gethostname} -eq 0 ; then
  unset ac_cv_lib_inet_gethostname
  AC_CHECK_LIB([inet],[gethostname],[LIBS="$LIBS -linet"],[
    AC_CHECK_LIB([inet],[main])
  ])
fi
# save results and retry for Tcl
EXP_LIBS=${LIBS}
LIBS=${EXP_AND_TCL_LIBS}
unset ac_cv_func_gethostname
AC_CHECK_FUNC([gethostname],[gethostname=1],[gethostname=0])
if test ${gethostname} -eq 0 ; then
  unset ac_cv_lib_inet_gethostname
  AC_CHECK_LIB([inet],[gethostname],[LIBS="$LIBS -linet"],[
    AC_CHECK_LIB([inet],[main])
  ])
fi
# save Tcl results and retry for Tk
EXP_AND_TCL_LIBS=${LIBS}
LIBS=${EXP_AND_TK_LIBS}
unset ac_cv_func_gethostname
AC_CHECK_FUNC([gethostname],[gethostname=1],[gethostname=0])
if test $gethostname -eq 0 ; then
  unset ac_cv_lib_inet_gethostname
  AC_CHECK_LIB([inet],[gethostname],[LIBS="$LIBS -linet"],[
    AC_CHECK_LIB([inet],[main])
  ])
fi
# save Tk results and reset for Expect
EXP_AND_TK_LIBS=${LIBS}
LIBS=${EXP_LIBS}

######################################################################
# required by Fischman's ISC 4.0
unset ac_cv_func_socket
AC_CHECK_FUNC([socket],[socket=1],[socket=0])
if test ${socket} -eq 0 ; then
  unset ac_cv_lib_inet_socket
  AC_CHECK_LIB([inet],[socket],[LIBS="$LIBS -linet"],[
    AC_CHECK_LIB([socket],[socket],[LIBS="$LIBS -lsocket"],[
      AC_CHECK_LIB([socket],[main])
      AC_CHECK_LIB([inet],[main])
    ])
  ])
fi
# save results and retry for Tcl
EXP_LIBS=${LIBS}
LIBS=${EXP_AND_TCL_LIBS}
unset ac_cv_func_socket
AC_CHECK_FUNC([socket],[socket=1],[socket=0])
if test ${socket} -eq 0 ; then
  unset ac_cv_lib_inet_socket
  AC_CHECK_LIB([inet],[socket],[LIBS="$LIBS -linet"],[
    AC_CHECK_LIB([socket],[socket],[LIBS="$LIBS -lsocket"],[
      AC_CHECK_LIB([socket],[main])
      AC_CHECK_LIB([inet],[main])
    ])
  ])
fi
# save Tcl results and retry for Tk
EXP_AND_TCL_LIBS=${LIBS}
LIBS=${EXP_AND_TK_LIBS}
unset ac_cv_func_socket
AC_CHECK_FUNC([socket],[socket=1],[socket=0])
if test ${socket} -eq 0 ; then
  unset ac_cv_lib_inet_socket
  AC_CHECK_LIB([inet],[socket],[LIBS="$LIBS -linet"],[
    AC_CHECK_LIB([socket],[socket],[LIBS="$LIBS -lsocket"],[
      AC_CHECK_LIB([socket],[main])
      AC_CHECK_LIB([inet],[main])
    ])
  ])
fi
# save Tk results and reset for Expect
EXP_AND_TK_LIBS=${LIBS}
LIBS=${EXP_LIBS}

######################################################################
unset ac_cv_func_select
AC_CHECK_FUNC([select],[select=1],[select=0])
if test ${select} -eq 0 ; then
  unset ac_cv_lib_inet_select
  AC_CHECK_LIB([inet],[select],[LIBS="$LIBS -linet"],[
    AC_CHECK_LIB([inet],[main])
  ])
fi
# save results and retry for Tcl
EXP_LIBS=${LIBS}
LIBS=${EXP_AND_TCL_LIBS}
unset ac_cv_func_select
AC_CHECK_FUNC([select],[select=1],[select=0])
if test ${select} -eq 0 ; then
  unset ac_cv_lib_inet_select
  AC_CHECK_LIB([inet],[select],[LIBS="$LIBS -linet"],[
    AC_CHECK_LIB([inet],[main])
  ])
fi
# save Tcl results and retry for Tk
EXP_AND_TCL_LIBS=${LIBS}
LIBS=${EXP_AND_TK_LIBS}
unset ac_cv_func_select
AC_CHECK_FUNC([select],[select=1],[select=0])
if test ${select} -eq 0 ; then
  unset ac_cv_lib_inet_select
  AC_CHECK_LIB([inet],[select],[LIBS="$LIBS -linet"],[
    AC_CHECK_LIB([inet],[main])
  ])
fi
# save Tk results and reset for Expect
EXP_AND_TK_LIBS=${LIBS}
LIBS=${EXP_LIBS}

######################################################################
unset ac_cv_func_getpseudotty
AC_CHECK_FUNC([getpseudotty],[getpseudotty=1],[getpseudotty=0])
if test ${getpseudotty} -eq 0 ; then
  unset ac_cv_lib_seq_getpseudotty
  AC_CHECK_LIB([seq],[getpseudotty],[],[
    unset ac_cv_lib_seq_main
    AC_CHECK_LIB([seq],[main])
  ])
fi
# save results and retry for Tcl
EXP_LIBS=${LIBS}
LIBS=${EXP_AND_TCL_LIBS}
unset ac_cv_func_getpseudotty
AC_CHECK_FUNC([getpseudotty],[getpseudotty=1],[getpseudotty=0])
if test ${getpseudotty} -eq 0 ; then
  unset ac_cv_lib_seq_getpseudotty
  AC_CHECK_LIB([seq],[getpseudotty],[],[
    unset ac_cv_lib_seq_main
    AC_CHECK_LIB([seq],[main])
  ])
fi
# save Tcl results and retry for Tk
EXP_AND_TCL_LIBS=${LIBS}
LIBS=${EXP_AND_TK_LIBS}
unset ac_cv_func_getpseudotty
AC_CHECK_FUNC([getpseudotty],[getpseudotty=1],[getpseudotty=0])
if test ${getpseudotty} -eq 0 ; then
  unset ac_cv_lib_seq_getpseudotty
  AC_CHECK_LIB([seq],[getpseudotty],[],[
    unset ac_cv_lib_seq_main
    AC_CHECK_LIB([seq],[main])
  ])
fi
# save Tk results and reset for Expect
EXP_AND_TK_LIBS=${LIBS}
LIBS=${EXP_LIBS}

######################################################################
# Check for FreeBSD/NetBSD openpty()
# CYGNUS LOCAL: Do NOT do this on Linux. Alpha Linux Red Hat 4.2 has
# openpty, but it does NOT work correctly.
case "${host}" in
*-*-linux*) ;;
*-*-cygwin*)
  EXP_AND_TCL_LIBS="${EXP_AND_TCL_LIBS} -luser32" ;;
*)
  unset ac_cv_func_openpty
  AC_CHECK_FUNC([openpty],[openpty=1],[openpty=0])
  if test $openpty -eq 0 ; then
    unset ac_cv_lib_util_openpty
    AC_CHECK_LIB([util],[openpty],[
	  # we only need to define OPENPTY once, but since we override
	  # the default behavior, we must also handle augment LIBS too.
	  # This need NOT be done in the 2nd and 3rd tests.
	  AC_DEFINE([HAVE_OPENPTY],[1],
                    [Define to 1 if you have the openpty function.])
	  LIBS="${LIBS} -lutil"
    ],[
	  AC_CHECK_LIB([util],[main])
    ])
  fi
  # save results and retry for Tcl
  EXP_LIBS=${LIBS}
  LIBS=${EXP_AND_TCL_LIBS}
  unset ac_cv_func_openpty
  AC_CHECK_FUNC([openpty],[openpty=1],[openpty=0])
  if test $openpty -eq 0 ; then
    unset ac_cv_lib_util_openpty
    AC_CHECK_LIB([util],[openpty],[
        AC_DEFINE([HAVE_OPENPTY],[1],
                  [Define to 1 if you have the openpty function.])
        LIBS="${LIBS} -lutil"
    ],[
	  AC_CHECK_LIB([util],[main])
    ])
  fi
  # save Tcl results and retry for Tk
  EXP_AND_TCL_LIBS=${LIBS}
  LIBS=${EXP_AND_TK_LIBS}
  unset ac_cv_func_openpty
  AC_CHECK_FUNC([openpty],[openpty=1],[openpty=0])
  if test $openpty -eq 0 ; then
    unset ac_cv_lib_util_openpty
    AC_CHECK_LIB([util],[openpty],[
        AC_DEFINE([HAVE_OPENPTY])
        LIBS="$LIBS -lutil"
    ],[
	  AC_CHECK_LIB([util],[main])
    ])
  fi
  # save Tk results and reset for Expect
  EXP_AND_TK_LIBS=${LIBS}
  LIBS=${EXP_LIBS}
;;
esac

######################################################################
# End of library/func checking
######################################################################

######################################################################
#
# Look for various header files
#
AC_HEADER_MAJOR
dnl# AC_HEADER_SYS_WAIT dnl# (already done above)
AC_CHECK_HEADERS([sys/sysmacros.h],[
  AC_DEFINE([HAVE_SYSMACROS_H],[1],
            [Define to 1 if you have the sys/sysmacros.h header file.])dnl
],[])
unset ac_cv_header_stdlib_h
AC_CHECK_HEADERS([stdlib.h],[],[
  AC_DEFINE([NO_STDLIB_H],[1],
            [Define to 1 if there is no stdlib.h header file.])dnl
])dnl

AC_CHECK_HEADERS([ctype.h errno.h fcntl.h ioctl.h malloc.h malloc/malloc.h math.h \
                  mem/memory.h poll.h setjmp.h sgtty.h signal.h stdarg.h \
                  stdio.h sys/file.h sys/ioctl.h sys/poll.h sys/time.h \
                  termio.h termios.h time.h utmp.h varargs/varargs.h])dnl

#
# Look for inttypes.h. Sometimes there are data type conflicts
# with sys/types.h, so we have to use our own special test.
#
CY_AC_INTTYPES_H

# Oddly, some systems have stdarg but do NOT support prototypes
# Tcl avoids the whole issue by not using stdarg on UNIX at all!
AC_C_PROTOTYPES

AC_CHECK_HEADERS([sys/fcntl.h sys/ptem.h sys/ptyio.h sys/select.h \
                  sys/stropts.h sys/strpty.h sys/strredir.h \
                  sys/sysconfig.h sys/vty.h varargs.h])dnl

AC_MSG_CHECKING([for sys/bsdtypes.h])
if test "ISC_${ISC}" = "ISC_1"; then
   AC_MSG_RESULT([yes])
   # if on ISC 1, we need <sys/bsdtypes.h> to get FD_SET macros
   AC_CHECK_HEADERS([sys/bsdtypes.h])
else
   AC_MSG_RESULT([no])
fi

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T
AC_CHECK_TYPES([caddr_t])dnl
AC_CHECK_TYPES([mblk_t])dnl
AC_CHECK_MEMBERS([struct stat.st_rdev])dnl

#
# Look for functions that may be missing
#
if test "x${GITHUB_ACTIONS}" = "xtrue" && test "x${CODEQL_ACTION_FEATURE_WILL_UPLOAD}" = "xtrue"; then
  export ac_cv_func_chown_works=yes
fi
AC_FUNC_CHOWN
if test "x${GITHUB_ACTIONS}" = "xtrue" && test "x${CODEQL_ACTION_FEATURE_WILL_UPLOAD}" = "xtrue"; then
  export ac_cv_func_fork_works=yes
  export ac_cv_func_vfork_works=yes
fi
AC_FUNC_FORK
AC_FUNC_GETPGRP
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_FUNC_SELECT_ARGTYPES
AC_FUNC_SETPGRP
AC_FUNC_STAT
AC_FUNC_STRFTIME
AC_FUNC_VPRINTF
unset ac_cv_func_gethostname
unset ac_cv_func_select
AC_CHECK_FUNCS([dup2 exit gethostname gettimeofday ioctl isascii isspace \
                log memmove memset pow re_comp regcomp regexec select \
                strchr strcpy strcspn strerror strlen strrchr strstr \
                sysconf system TclGetRegError time timezone])dnl

dnl# check for memcpy by hand
# because Unixware 2.0 handles it specially and refuses to compile
# autoconf's automatic test that is a call with no arguments
AC_MSG_CHECKING([for memcpy])
AC_LINK_IFELSE([AC_LANG_PROGRAM([[
#include <string.h>
]],[[
char *s1, *s2;
memcpy(s1,s2,0);
]])],[AC_MSG_RESULT([yes])
	AC_DEFINE([HAVE_MEMCPY],[1],[Define to 1 if we have memcpy])
	],[AC_MSG_RESULT([no])
])dnl

# Some systems only define WNOHANG if _POSIX_SOURCE is defined
# The following merely tests that sys/wait.h can be included
# and if so that WNOHANG is not defined.  The only place I've
# seen this is ISC.
AC_MSG_CHECKING([if WNOHANG requires _POSIX_SOURCE])
AC_RUN_IFELSE([AC_LANG_SOURCE([[
#include <sys/wait.h>
int main(void) {
#ifndef WNOHANG
	return 0;
#else
	return 1;
#endif /* !WNOHANG */
}]])],[AC_MSG_RESULT([yes])
	AC_DEFINE([WNOHANG_REQUIRES_POSIX_SOURCE],[1],[Define to 1 if WNOHANG requires _POSIX_SOURCE])
],[AC_MSG_RESULT([no])
],[case "${host}" in
	 *-*-cygwin*) AC_MSG_RESULT([no]) ;;
	 *) AC_MSG_ERROR([Expect cannot be cross compiled]) ;;
	 esac
])dnl

AC_MSG_CHECKING([if any value exists for WNOHANG])
rm -rf wnohang
AC_RUN_IFELSE([AC_LANG_SOURCE([[
#include <stdio.h>
#include <sys/wait.h>
int main(void) {
#ifdef WNOHANG
	FILE *fp = fopen("wnohang","w");
	fprintf(fp,"%d",WNOHANG);
	fclose(fp);
	return 0;
#else
	return 1;
#endif /* WNOHANG */
}]])],[AC_MSG_RESULT([yes])
	AC_DEFINE_UNQUOTED([WNOHANG_BACKUP_VALUE],[`cat wnohang`],[The backup value for WNOHANG])
	rm -f wnohang
],[AC_MSG_RESULT([no])
	AC_DEFINE([WNOHANG_BACKUP_VALUE],[1],
	          [The backup value for WNOHANG])
],[case "${host}" in
	 *-*-cygwin*) AC_MSG_RESULT([yes])
			AC_DEFINE([WNOHANG_BACKUP_VALUE],[1],[The backup value for WNOHANG]) ;;
	 *) AC_MSG_ERROR([Expect cannot be cross compiled]) ;;
	 esac
])dnl

#
# check how signals work
#

# Check for the data type of the mask used in select().
# This picks up HP braindamage which defines fd_set and then 
# proceeds to ignore it and use int.
# Pattern matching on int could be loosened.
# Cannot use ac_header_egrep since that does NOT see prototypes with
# K&R cpp (although that is really old by now).
AC_MSG_CHECKING([mask type of select])
if egrep "select\(size_t, int" /usr/include/sys/time.h >/dev/null 2>&1; then
  AC_MSG_RESULT([int])
  AC_DEFINE([SELECT_MASK_TYPE],[int],
            [Define to int if it is the mask type for select])
else
  AC_MSG_RESULT([none])
fi

AC_CHECK_FUNCS([alarm])
AC_MSG_CHECKING([if signals need to be re-armed])
AC_RUN_IFELSE([AC_LANG_SOURCE([[
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#ifdef RETSIGTYPE
# undef RETSIGTYPE
#endif
#define RETSIGTYPE $retsigtype

int signal_rearms = 0;

RETSIGTYPE
child_sigint_handler(n)
int n;
{
}

RETSIGTYPE
parent_sigint_handler(n)
int n;
{
signal_rearms++;
}

int main(void)
{
	signal(SIGINT,parent_sigint_handler);

	if (0 == fork()) {
		signal(SIGINT,child_sigint_handler);
		kill(getpid(),SIGINT);
		kill(getpid(),SIGINT);
		kill(getppid(),SIGINT);
	} else {
		int status;

		wait(&status);
		unlink("core");
		exit(signal_rearms);
	}
}]])],[AC_MSG_RESULT([yes])
	AC_DEFINE([REARM_SIG],[1],
                  [Define to 1 if signals need to be rearmed.])
],[AC_MSG_RESULT([no])
],[case "${host}" in
	 *-*-cygwin*) AC_MSG_RESULT([no]) ;;
	 *) AC_MSG_WARN([Expect cannot be cross compiled]) ;;
	 esac
])dnl

# HPUX7 has trouble with the big cat so split it
# Owen Rees <rtor@ansa.co.uk> 29Mar93 
SEDDEFS="${SEDDEFS}CONFEOF
cat >> conftest.sed <<CONFEOF
"
#

# There are multiple versions of getpty, alas.
# I do NOT remember who has the first one, but Convex just added one
# so check for it. Unfortunately, there is no header so the only
# reasonable way to make sure is to look it we are on a Convex.
AC_MSG_CHECKING([if on Convex])
convex=0
case "${host}" in
  c[[12]]-*-*)		convex=1;;
esac

if test ${convex} -eq 1 ; then
  AC_MSG_RESULT([yes])
  AC_DEFINE([CONVEX],[1],[Define to 1 if we are on Convex])
else
  AC_MSG_RESULT([no])
fi

EXP_LDFLAGS=

AC_MSG_CHECKING([if on NeXT])
if test -r /NextApps -a "x${cross_compiling}" != "xyes"; then
  AC_MSG_RESULT([yes])
  # "-m" flag suppresses complaints about multiple strtod:
  EXP_LDFLAGS="${EXP_LDFLAGS} -m"
else
  AC_MSG_RESULT([no])
fi


AC_MSG_CHECKING([if on HP])
if test "x`(uname) 2>/dev/null`" = xHP-UX; then
  AC_MSG_RESULT([yes])
  hp=1
else
  AC_MSG_RESULT([no])
  hp=0
fi

AC_MSG_CHECKING([sane default stty arguments])
DEFAULT_STTY_ARGS="sane"

if test ${mach} -eq 1 ; then
	DEFAULT_STTY_ARGS="cooked"
fi

if test ${hp} -eq 1 ; then
	DEFAULT_STTY_ARGS="sane kill "
fi

AC_MSG_RESULT([${DEFAULT_STTY_ARG}])

# Look for various features to determine what kind of pty
# we have. For some weird reason, ac_compile_check would not
# work, but ac_test_program does.
#
AC_MSG_CHECKING([for HP style pty allocation])
# following test fails on DECstations and other things that do NOT grok -c
# but that is ok, since they do NOT have PTYMs anyway
if test -r /dev/ptym/ptyp0 2>/dev/null -a "x${cross_compiling}" != "xyes"; then
    AC_MSG_RESULT([yes])
    AC_DEFINE([HAVE_PTYM],[1],[Define to 1 for HP style pty allocation])
else
    AC_MSG_RESULT([no])
fi

AC_MSG_CHECKING([for HP style pty trapping])
AC_EGREP_HEADER([struct.*request_info],[sys/ptyio.h],[AC_MSG_RESULT([yes])
	AC_DEFINE([HAVE_PTYTRAP],[1],
                  [Define to 1 for HP style pty trapping])
],[AC_MSG_RESULT([no])
])dnl

AC_MSG_CHECKING([for AIX new-style pty allocation])
if test -r /dev/ptc -a -r /dev/pts -a "x${cross_compiling}" != "xyes"; then
    AC_MSG_RESULT([yes])
    AC_DEFINE([HAVE_PTC_PTS],[1],
              [Define to 1 for AIX new-style pty allocation])
else
    AC_MSG_RESULT([no])
fi

AC_MSG_CHECKING([for SGI old-style pty allocation])
if test -r /dev/ptc -a ! -r /dev/pts -a "x${cross_compiling}" != "xyes"; then
    AC_MSG_RESULT(yes)
    AC_DEFINE([HAVE_PTC],[1],
              [Define to 1 for SGI old-style pty allocation])
else
    AC_MSG_RESULT([no])
fi

# On SCO OpenServer, two types of ptys are available: SVR4 streams and
# c-list. The library routines to open the SVR4 ptys are broken on certain
# systems and the SCO command to increase the number of ptys only configure
# c-list ones anyway. So we chose these, which have a special numbering
# scheme.
#
AC_MSG_CHECKING([for SCO style pty allocation])
sco_ptys=""
case "${host}" in
  *-sco3.2v[[45]]*)	sco_clist_ptys=1 svr4_ptys_broken=1;;
esac

if test x"${sco_clist_ptys}" != x""; then
  AC_MSG_RESULT([yes])
  AC_DEFINE([HAVE_SCO_CLIST_PTYS],[1],
            [Define to 1 for SCO-style pty allocation])
else
  AC_MSG_RESULT([no])
fi

AC_MSG_CHECKING([for SVR4 style pty allocation])
if test -r /dev/ptmx -a "x${svr4_ptys_broken}" = "x" -a "x${cross_compiling}" != "xyes"; then
  AC_MSG_RESULT([yes])
  AC_DEFINE([HAVE_PTMX],[1],[Define to 1 if we have ptmx])
  # aargg. Some systems need libpt.a to use /dev/ptmx
  AC_CHECK_FUNC([ptsname],[],[LIBS="${LIBS} -lpt"])
  # I have never seen Tcl/Tk include -lpt so do NOT bother w/explicit test
  unset ac_cv_func_ptsname
  AC_CHECK_FUNC([ptsname],[],[EXP_AND_TCL_LIBS="${EXP_AND_TCL_LIBS} -lpt"])
  unset ac_cv_func_ptsname
  AC_CHECK_FUNC([ptsname],[],[EXP_AND_TK_LIBS="${EXP_AND_TK_LIBS} -lpt"])
  if test "x${ac_cv_header_features_h}" = "x"; then
    test -z "${ac_cv_header_features_h}"
    AC_CHECK_HEADERS([features.h])
  fi
  case "${host}" in
    ia64-*-*linux*)
    # CYGNUS LOCAL: IA-64
    # On some systems (e.g., IA-64 Linux), we need to define _XOPEN_SOURCE
    # in order to get a declaration for ptsname. It is safe to simply
    # define that whenever we are using ptsname.
    AC_MSG_CHECKING([for NSIG definition with _XOPEN_SOURCE])
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
/* _XOPEN_SOURCE can remove NSIG on UnixWare or OSF/1. */
#define _XOPEN_SOURCE 1
#include <signal.h>
#ifndef NSIG
# define NSIG _NSIG
#endif /* !NSIG */
char junk[NSIG];]],[[]])],[AC_DEFINE([_XOPEN_SOURCE],[1],
                                     [Some systems need this defined])
	  AC_MSG_RESULT([yes])
      ],[AC_MSG_RESULT([no])
      ])
    # END CYGNUS LOCAL
  ;;
  esac
else
  AC_MSG_RESULT([no])
fi

# In OSF/1 case, SVR4 are somewhat different.
# Gregory Depp <depp@osf.org> 17Aug93
AC_MSG_CHECKING([for OSF/1 style pty allocation])
if test -r /dev/ptmx_bsd -a "$cross_compiling" != "yes" ; then
    AC_DEFINE([HAVE_PTMX_BSD],[1],[Define to 1 for BSD-style ptmx])
    AC_MSG_RESULT([yes])
else
    AC_MSG_RESULT([no])
fi

# Set the pty handling for Cygwin
case "${host}" in
  *-*-cygwin*) AC_DEFINE([HAVE_PTMX],[1],[Define to 1 if we have ptmx]) ;;
  *) ;;
esac

tcgetattr=0
tcsetattr=0
AC_CHECK_FUNC([tcgetattr],[tcgetattr=1])
AC_CHECK_FUNC([tcsetattr],[tcsetattr=1])
if test ${tcgetattr} -eq 1 -a ${tcsetattr} -eq 1 ; then
    AC_DEFINE([HAVE_TCSETATTR],[1],[Define to 1 if we have tcsetattr])
    AC_DEFINE([POSIX],[1],[Define to 1 for POSIX])
fi

# first check for the pure bsd
AC_MSG_CHECKING([for struct sgttyb])
AC_RUN_IFELSE([AC_LANG_SOURCE([[
#include <sgtty.h>
#include <stdlib.h>
int main(void)
{
  struct sgttyb tmp;
  exit(0);
}]])],[AC_MSG_RESULT([yes])
        AC_DEFINE([HAVE_SGTTYB],[1],
                  [Define to 1 if you have the struct sgttyb])
        PTY_TYPE=sgttyb
],[AC_MSG_RESULT([no])
],[case "${host}" in
	 *-*-cygwin*) AC_MSG_RESULT([no]) ;;
	 *) AC_MSG_ERROR([Expect cannot be cross compiled]) ;;
	 esac
])dnl

AC_SYS_POSIX_TERMIOS

# mach systems have include files for unimplemented features
# so avoid doing following test on those systems
if test ${mach} -eq 0 ; then

  # next check for the older style ttys
  # note that if we detect termio.h (only), we still set PTY_TYPE=termios
  # since that just controls which of pty_XXXX.c file is use and
  # pty_termios.c is set up to handle pty_termio.
  AC_MSG_CHECKING([for struct termio])
  AC_RUN_IFELSE([AC_LANG_SOURCE([[#include <stdlib.h>
  #include <termio.h>
  int main(void)
  {
    struct termio tmp;
    exit(0);
  }]])],[AC_DEFINE([HAVE_TERMIO],[1],
                   [Define to 1 if you have the struct termio])
        PTY_TYPE=termios
        AC_MSG_RESULT([yes])
],[AC_MSG_RESULT([no])
],[case "${host}" in
	 *-*-cygwin*) AC_MSG_RESULT([no]) ;;
	 *) AC_MSG_ERROR([Expect cannot be cross compiled]) ;;
	 esac
])dnl

  # now check for the new style ttys (not yet posix)
  AC_MSG_CHECKING([for struct termios])
  AC_RUN_IFELSE([AC_LANG_SOURCE([[
# ifdef HAVE_INTTYPES_H
#  include <inttypes.h>
# else
#  if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#   warning "this conftest expects <inttypes.h> to be included."
#  endif /* __GNUC__ && !__STRICT_ANSI__ */
# endif /* HAVE_INTTYPES_H */
#  include <stdlib.h>
#  include <termios.h>
  int main(void)
  {
    struct termios tmp;
    exit(0);
  }]])],[AC_DEFINE([HAVE_TERMIOS],[1],[Define to 1 if you have the struct termios])
        PTY_TYPE=termios
        AC_MSG_RESULT([yes])
  ],[AC_MSG_RESULT([no])
  ],[case "${host}" in
	 *-*-cygwin*) AC_DEFINE([HAVE_TERMIOS],[1],[Define to 1 if you have the struct termios])
			PTY_TYPE=termios
			AC_MSG_RESULT([yes]) ;;
	 *) AC_MSG_ERROR([Expect cannot be cross compiled]) ;;
	 esac
  ])
fi

AC_MSG_CHECKING([if TCGETS or TCGETA is in termios.h])
AC_RUN_IFELSE([AC_LANG_SOURCE([[
/* including termios.h on Solaris 5.6 fails unless inttypes.h included: */
#ifdef HAVE_INTTYPES_H
# include <inttypes.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "this conftest expects <inttypes.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_INTTYPES_H */
#include <termios.h>
int main(void) {
#if defined(TCGETS) || defined(TCGETA)
	return 0;
#else
	return 1;
#endif /* TCGETS || TCGETA */
}]])],[AC_DEFINE([HAVE_TCGETS_OR_TCGETA_IN_TERMIOS_H],[1],[Define to 1 if TCGETS or TCGETA is in termios.h])
	AC_MSG_RESULT([yes])
],[AC_MSG_RESULT([no])
],[case "${host}" in
	 *-*-cygwin*) AC_DEFINE([HAVE_TCGETS_OR_TCGETA_IN_TERMIOS_H],[1],[Define to 1 if TCGETS or TCGETA is in termios.h])
			AC_MSG_RESULT([yes]) ;;
	 *) AC_MSG_ERROR([Expect cannot be cross compiled]) ;;
	 esac
])dnl

AC_HEADER_TIOCGWINSZ

AC_MSG_CHECKING([if TIOCGWINSZ is in termios.h])
AC_RUN_IFELSE([AC_LANG_SOURCE([[
/* including termios.h on Solaris 5.6 fails unless inttypes.h included: */
#ifdef HAVE_INTTYPES_H
# include <inttypes.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "this conftest expects <inttypes.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_INTTYPES_H */
#include <termios.h>
main() {
#ifdef TIOCGWINSZ
	return 0;
#else
	return 1;
#endif /* TIOCGWINSZ */
}]])],[AC_DEFINE([HAVE_TIOCGWINSZ_IN_TERMIOS_H],[1],[Define to 1 if TIOCGWINSZ is in termios.h])
	AC_MSG_RESULT([yes])
],[AC_MSG_RESULT([no])
],[case "${host}" in
	 *-*-cygwin*) AC_DEFINE([HAVE_TIOCGWINSZ_IN_TERMIOS_H],[1],[Define to 1 if TIOCGWINSZ is in termios.h])
			AC_MSG_RESULT([yes]) ;;
	 *) AC_MSG_ERROR([Expect cannot be cross compiled]) ;;
	 esac
])dnl

# finally check for Cray style ttys
AC_MSG_CHECKING([for Cray-style ptys])
SETUID=":"
AC_RUN_IFELSE([AC_LANG_SOURCE([[
int main(void) {
#ifdef CRAY
	return 0;
#else
	return 1;
#endif /* CRAY */
}
]])],[PTY_TYPE=unicos
	SETUID="chmod u+s"
	AC_MSG_RESULT([yes])
],[AC_MSG_RESULT([no])
],[case "${host}" in
	 *-*-cygwin*) AC_MSG_RESULT([no]) ;;
	 *) AC_MSG_ERROR([Expect cannot be cross compiled]) ;;
	 esac
])dnl

if test "x${PTY_TYPE}" = "x"; then
  test -z "${PTY_TYPE}" && echo "PTY_TYPE is empty!" >&2
  AC_MSG_ERROR([no working PTY_TYPE found])
fi

#
# Check for select and/or poll. If both exist, we prefer select.
# if neither exists, define SIMPLE_EVENT.
#
select=0
poll=0
unset ac_cv_func_select
AC_CHECK_FUNC([select],[select=1])
AC_CHECK_FUNC([poll],[poll=1])
AC_MSG_CHECKING([event handling])
if test ${select} -eq 1 ; then
  EVENT_TYPE=select
  EVENT_ABLE=event
  AC_MSG_RESULT([via select])
elif test ${poll} -eq 1 ; then
  EVENT_TYPE=poll
  EVENT_ABLE=event
  AC_MSG_RESULT([via poll])
else
  EVENT_TYPE=simple
  EVENT_ABLE=noevent
  AC_MSG_RESULT([none])
  AC_DEFINE([SIMPLE_EVENT],[1],[Define to 1 if event handling is simple])
fi

AC_CHECK_FUNCS([_getpty])dnl
AC_CHECK_FUNCS([getpty])dnl

#
# check for timezones
#
AC_MSG_CHECKING([for SV-style timezone])
AC_RUN_IFELSE([AC_LANG_SOURCE([[
#include <stdlib.h>
extern char *tzname[2];
extern int daylight;
int main(void)
{
  int *x = &daylight;
  char **y = tzname;

  exit(0);
}]])],[AC_DEFINE([HAVE_SV_TIMEZONE],[1],
                 [Define to 1 for an SV-style timezone])
	AC_MSG_RESULT([yes])],[AC_MSG_RESULT([no])
],[case "${host}" in
	 *-*-cygwin*) AC_MSG_RESULT([no]) ;;
	 *) AC_MSG_ERROR([Expect cannot be cross compiled]) ;;
	 esac
])dnl

dnl# compare with the autoconf pre-written one:
AC_STRUCT_TIMEZONE dnl# also does the "_TM" one

# only look for Tk stuff if we have X11 and user does NOT say not to do so:
AC_ARG_WITH([x],[AS_HELP_STRING([--with-x],[whether or not to use X (default yes)])],[],[with_x=yes])
if test "x${with_x}" = "xno"; then
   no_tk=true
else
   AC_MSG_NOTICE([Doing X-related checks for Tk])
   CY_AC_PATH_TKH
   _AC_PATH_X_XMKMF
fi
if test x"${no_tk}" != x"true"; then
# (libexpectk no longer exists)
  X_PROGS=expectk
# should really generate following symbol, but I am hitting configure's
# limit on substs...
  X_PROGS_INSTALLED=expectk_installed
else
  X_PROGS="# no X support on this system"
  AC_MSG_WARN([No X based programs will be built])
  echo "	WARNING: Cannot find Tk headers or library. You can still"
  echo "	build expect, but not expectk. See Expect's README for"
  echo "	information on how to obtain Tk. If Tk is installed, see"
  echo "	Expect's INSTALL on how to tell configure where Tk is"
  echo "	installed."
fi

# consume these flags so that user can invoke Expect's configure with
# the same command as Tcl's configure
AC_ARG_ENABLE([load],
	[AS_HELP_STRING([--disable-load],[disallow dynamic loading])],
	[disable_dl=${enableval}],[disable_dl=no])dnl

AC_ARG_ENABLE([gcc],
	[AS_HELP_STRING([--enable-gcc],[allow use of gcc if available])],
	[enable_gcc=${enableval}],[enable_gcc=no])dnl


# Following comment stolen from Tcl's configure.ac:
#   Note: in the following variable, it is important to use the absolute
#   path name of the Tcl directory rather than "..":  this is because
#   AIX remembers this path and will attempt to use it at run-time to look
#   up the Tcl library.

if test "${TCL_LIB_VERSIONS_OK}" = "ok"; then
    EXP_LIB_VERSION=${EXP_VERSION}
else
    EXP_LIB_VERSION=${EXP_VERSION_NODOTS}
fi
if test ${iunix} -eq 1 ; then
    EXP_LIB_VERSION=${EXP_VERSION_NODOTS}
fi

# also remove dots on systems that do NOT support filenames > 14
# (are there really systems which support shared libs and restrict
# filename lengths!?)
AC_SYS_LONG_FILE_NAMES
if test "x${ac_cv_sys_long_file_names}" = "xno"; then
    EXP_LIB_VERSION=${EXP_VERSION_NODOTS}
fi

EXP_BUILD_LIB_SPEC="-L`pwd` -lexpect${EXP_LIB_VERSION}"
EXP_LIB_SPEC="-L\${exec_prefix}/lib -lexpect${EXP_LIB_VERSION}"
EXP_UNSHARED_LIB_FILE=libexpect${EXP_LIB_VERSION}.a
EXP_BUILD_LIB_SPEC=${EXP_UNSHARED_LIB_FILE}

AC_MSG_CHECKING([for type of library to build])
if test "${enable_shared}" = "yes" && test "x${TCL_SHLIB_SUFFIX}" != "x"; then
  EXP_SHLIB_CFLAGS=${TCL_SHLIB_CFLAGS}
  eval "EXP_SHARED_LIB_FILE=libexpect${TCL_SHARED_LIB_SUFFIX}"
  EXP_LIB_FILE=${EXP_SHARED_LIB_FILE}
  EXP_LIB_FILES="${EXP_SHARED_LIB_FILE} ${EXP_UNSHARED_LIB_FILE}"
  AC_MSG_RESULT([both shared and unshared])
else
  EXP_SHLIB_CFLAGS=""
  EXP_SHARED_LIB_FILE="reconfigure_Tcl_for_shared_library"
  EXP_LIB_FILE=${EXP_UNSHARED_LIB_FILE}
  EXP_LIB_FILES="${EXP_UNSHARED_LIB_FILE}"
  AC_MSG_RESULT([unshared])
fi

# now broken out into EXP_AND_TCL_LIBS and EXP_AND_TK_LIBS. Had to do this
# in order to avoid repeating lib specs to which some systems object.
EXP_AND_TCL_LIBS="${EXP_AND_TCL_LIBS} ${TCL_LD_SEARCH_FLAGS}"
EXP_AND_TK_LIBS="${EXP_AND_TK_LIBS} ${TCL_LD_SEARCH_FLAGS}"

# Sigh - Tcl defines SHLIB_LD_LIBS to be either empty or ${LIBS} and
# LIBS is intended to be expanded by Make. But since we are too close
# to hitting config's max symbols, pack everything together here and
# do test ourselves. Ugh.  
#
if test "x${TCL_SHLIB_LD_LIBS}" = "x"; then
  EXP_SHLIB_LD_LIBS=""
else
  # It seems a little strange to build in Tcl's build-lib, but
  # that is what Tk does:
  EXP_SHLIB_LD_LIBS="${TCL_BUILD_LIB_SPEC} ${TCL_DL_LIBS} ${LIBS} -lc"
fi

dnl# CYGNUS LOCAL
dnl# check for win32 dependencies:
CY_AC_CYGWIN
CY_AC_EXEEXT

dnl# other:
AC_C_CONST
AC_CHECK_DECLS([I_PUSH])dnl

#--------------------------------------------------------------------
# This section is based on analogous thing in Tk installation. - DEL
#       Various manipulations on the search path used at runtime to
#       find shared libraries:
#       2. On systems such as AIX and Ultrix that use "-L" as the
#          search path option, colons cannot be used to separate
#          directories from each other. Change colons to " -L".
#       3. Create two sets of search flags, one for use in cc lines
#          and the other for when the linker is invoked directly.  In
#          the second case, '-Wl,' must be stripped off and commas must
#          be replaced by spaces.
#--------------------------------------------------------------------

LIB_RUNTIME_DIR='${LIB_RUNTIME_DIR}'

# If Tcl and Expect are installed in different places, adjust the library
# search path to reflect this.

if test "${TCL_EXEC_PREFIX}" != "${exec_prefix}"; then
    LIB_RUNTIME_DIR="${LIB_RUNTIME_DIR}:${TCL_EXEC_PREFIX}"
fi

if test "${TCL_LD_SEARCH_FLAGS}" = '-L${LIB_RUNTIME_DIR}'; then
    LIB_RUNTIME_DIR=`echo ${LIB_RUNTIME_DIR} |sed -e 's/:/ -L/g'`
fi

# The statement below is very tricky! It actually *evaluates* the
# string in TCL_LD_SEARCH_FLAGS, which causes a substitution of the
# variable LIB_RUNTIME_DIR.

eval "EXP_CC_SEARCH_FLAGS=\"${TCL_LD_SEARCH_FLAGS}\""
if test "${GCC}" = yes; then
  true
else
  EXP_LD_SEARCH_FLAGS=`echo ${EXP_CC_SEARCH_FLAGS} |sed -e "s|-Wl,||g" -e "s|,| |g"`
fi

dnl#
dnl# Set up makefile substitutions (at the m4 level)
dnl#
AC_SUBST([EXP_MAJOR_VERSION])dnl
AC_SUBST([EXP_MINOR_VERSION])dnl
AC_SUBST([EXP_MICRO_VERSION])dnl
AC_SUBST([EXP_VERSION_FULL])dnl
AC_SUBST([EXP_VERSION])dnl
AC_SUBST([EXP_CONFIG_SHELL])dnl
AC_SUBST([EXP_SHARED_LIB_FILE])dnl
AC_SUBST([EXP_UNSHARED_LIB_FILE])dnl
AC_SUBST([EXP_SHLIB_CFLAGS])dnl
AC_SUBST([EXP_LIB_FILE])dnl
AC_SUBST([EXP_LIB_FILES])dnl
AC_SUBST([EXP_BUILD_LIB_SPEC])dnl
AC_SUBST([EXP_LIB_SPEC])dnl
AC_SUBST([EXP_CFLAGS])dnl
AC_SUBST([EXP_LDFLAGS])dnl
AC_SUBST([EXP_LD_SEARCH_FLAGS])dnl
AC_SUBST([EXP_AND_TCL_LIBS])dnl
AC_SUBST([EXP_AND_TK_LIBS])dnl
AC_SUBST([EXP_SHLIB_LD_LIBS])dnl
AC_SUBST([X_PROGS])dnl
AC_SUBST([PTY_TYPE])dnl
AC_SUBST([EVENT_TYPE])dnl
AC_SUBST([EVENT_ABLE])dnl
AC_SUBST([SETUID])dnl
AC_SUBST([UNSHARED_RANLIB])dnl
AC_SUBST([DEFAULT_STTY_ARGS])dnl
AC_CONFIG_FILES([Makefile \
	pkgIndex])
AC_CONFIG_COMMANDS([default],[chmod +x ${srcdir}/install-sh ${srcdir}/mkinstalldirs],[])
AC_OUTPUT
