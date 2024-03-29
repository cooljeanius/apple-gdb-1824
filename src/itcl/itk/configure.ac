#--------------------------------------------------------------------
# Sample configure.ac for Tcl Extensions.  The only places you should
# need to modify this file are marked by the string __CHANGE__
#--------------------------------------------------------------------

sinclude(acinclude.m4)

#--------------------------------------------------------------------
# __CHANGE__
# This very first macro is used to verify that the configure script can 
# find the sources.  The arguments to AC_INIT should be as found in the
# configure.scan generated by autoscan:
#
dnl# AC_INIT([FULL-PACKAGE-NAME],[VERSION],[BUG-REPORT-ADDRESS])
dnl# AC_CONFIG_SRCDIR([../generic/tcl.h])
#--------------------------------------------------------------------

AC_PREREQ([2.61])
AC_INIT([itk],[3.2.1],[mmclennan@lucent.com])
AC_CONFIG_SRCDIR([generic/itk.h])
# CYGNUS LOCAL
AC_CONFIG_AUX_DIR([../..])
# END CYGNUS LOCAL
AC_CONFIG_MACRO_DIR([..])dnl

#--------------------------------------------------------------------
# __CHANGE__
# Set your package name and version numbers here.  The NODOT_VERSION is
# required for constructing the library name on systems that do not like
# dots in library names (Windows).  The VERSION variable is used on the
# other systems.
#--------------------------------------------------------------------

PACKAGE=itk

MAJOR_VERSION=3
MINOR_VERSION=2
PATCHLEVEL=.1

VERSION=${MAJOR_VERSION}.${MINOR_VERSION}
NODOT_VERSION=${MAJOR_VERSION}${MINOR_VERSION}

AC_SUBST([PACKAGE])dnl
AC_SUBST([VERSION])dnl

#--------------------------------------------------------------------
# We put this here so that you can compile with -DVERSION="1.2" to
# encode the package version directly into the source files.
#--------------------------------------------------------------------

eval AC_DEFINE_UNQUOTED([VERSION],["${VERSION}"],[Version])

#------------------------------------------------------------------------
# Handle the --prefix=... option
#------------------------------------------------------------------------

if test "${prefix}" = "NONE"; then
    prefix=/usr/local
fi
if test "${exec_prefix}" = "NONE"; then
    exec_prefix=$prefix
fi

#--------------------------------------------------------------------
# Check whether --enable-gcc or --disable-gcc was given.  Do this 
# before AC\_CYGWIN is called so the compiler can 
# be fully tested by built-in autoconf tools.
# This macro also calls AC_PROG_CC to set the compiler if --enable-gcc
# was not used.
#--------------------------------------------------------------------

# CYGNUS LOCAL
m4_ifdef([SC_ENABLE_GCC],[
  SC_ENABLE_GCC
])dnl
  ## generic autoconf version:
AC_PROG_CC
# END CYGNUS LOCAL
AC_PROG_INSTALL

#--------------------------------------------------------------------
# Checks to see if the make program sets the $MAKE variable.
#--------------------------------------------------------------------

AC_PROG_MAKE_SET

#--------------------------------------------------------------------
# Find ranlib
#--------------------------------------------------------------------

AC_PROG_RANLIB

#--------------------------------------------------------------------
# This macro performs additional compiler tests.
#--------------------------------------------------------------------

AC_CANONICAL_HOST
case $host_os in
  *cygwin* ) CYGWIN=yes;;
	 * ) CYGWIN=no;;
esac


#--------------------------------------------------------------------
# Determines the correct binary file extension (.o, .obj, .exe etc.)
#--------------------------------------------------------------------

AC_OBJEXT
AC_EXEEXT

#--------------------------------------------------------------------
# "cygpath" is used on windows to generate native path names for include
# files.
# These variables should only be used with the compiler and linker since
# they generate native path names.
#
# Unix tclConfig.sh points SRC_DIR at the top-level directory of
# the Tcl sources, while the Windows tclConfig.sh points SRC_DIR at
# the win subdirectory.  Hence the different usages of SRC_DIR below.
#
# This must be done before calling SC_PUBLIC_TCL_HEADERS
#--------------------------------------------------------------------

case "${host}" in
    *mingw32* | *windows32*)
	AC_CHECK_PROG(CYGPATH, cygpath, cygpath -w, echo)
	RELPATH=".. .. bin"
    ;;
    *)
	CYGPATH=echo
	RELPATH=..
    ;;
esac

AC_SUBST([CYGPATH])dnl
AC_SUBST([RELPATH])dnl

#--------------------------------------------------------------------
# Includes for this package
#--------------------------------------------------------------------

# CYGNUS LOCAL
srcdir=`cd ${srcdir} ; pwd`
ITK_SRC_DIR_NATIVE=`${CYGPATH} ${srcdir}`
# END CYGNUS LOCAL
ITK_GENERIC_DIR_NATIVE=`${CYGPATH} ${srcdir}/generic`
ITK_WIN_DIR_NATIVE=`${CYGPATH} ${srcdir}/win`
ITK_UNIX_DIR_NATIVE=`${CYGPATH} ${srcdir}/unix`

case "${host}" in
    *cygwin* | *mingw32* | *windows32*)
	ITK_PLATFORM_DIR_NATIVE=${ITK_WIN_DIR_NATIVE}
    ;;
    *)
	ITK_PLATFORM_DIR_NATIVE=${ITK_UNIX_DIR_NATIVE}
    ;;
esac

ITK_INCLUDES="-I\"${ITK_GENERIC_DIR_NATIVE}\" -I\"${ITK_PLATFORM_DIR_NATIVE}\""

AC_SUBST([ITK_GENERIC_DIR_NATIVE])dnl
AC_SUBST([ITK_WIN_DIR_NATIVE])dnl
AC_SUBST([ITK_UNIX_DIR_NATIVE])dnl
AC_SUBST([ITK_INCLUDES])dnl

#--------------------------------------------------------------------
# Load the tclConfig.sh file
#--------------------------------------------------------------------

SC_PATH_TCLCONFIG
SC_LOAD_TCLCONFIG

#--------------------------------------------------------------------
# Load the tkConfig.sh file
#--------------------------------------------------------------------

SC_PATH_TKCONFIG
SC_LOAD_TKCONFIG

AC_ARG_WITH([itcl],
        [AS_HELP_STRING([--with-itclinclude=DIR],[use Itcl headers from DIR])],
        [ITCL_LIB_DIR=${withval}],[ITCL_LIB_DIR=`cd ${srcdir}/../itcl; pwd`])

if test ! -r "${ITCL_LIB_DIR}/generic/itclInt.h" ; then
    AC_MSG_ERROR([Cannot find Itcl private header files.  Use --with-itclinclude to specify the directory containing the Itcl private headers on your system.])
fi

ITCL_INCLUDES=-I\"`${CYGPATH} ${ITCL_LIB_DIR}/generic`\"
AC_SUBST([ITCL_INCLUDES])dnl

# CYGNUS LOCAL
CYG_AC_PATH_ITCLCONFIG
CYG_AC_LOAD_ITCLCONFIG

itclstub_LIB_FILE=${ITCL_STUB_LIB_FILE}
AC_SUBST([itclstub_LIB_FILE])dnl

tkstub_LIB_SPEC=${TK_STUB_LIB_SPEC}
AC_SUBST([tkstub_LIB_SPEC])dnl
# END CYGNUS LOCAL

#--------------------------------------------------------------------
# __CHANGE__
# Choose which headers you need.  Extension authors should try very
# hard to only rely on the Tcl public header files.  Internal headers
# contain private data structures and are subject to change without
# notice.
# This must be done AFTER calling SC_PATH_TCLCONFIG/SC_LOAD_TCLCONFIG
# so that we can extract TCL_SRC_DIR from the config file (in the case
# of private headers
#--------------------------------------------------------------------

m4_ifdef([SC_PUBLIC_TCL_HEADERS],[
  SC_PUBLIC_TCL_HEADERS
])dnl
  ## just these:
SC_PRIVATE_TCL_HEADERS

m4_ifdef([SC_PUBLIC_TK_HEADERS],[
  SC_PUBLIC_TK_HEADERS
])dnl
  ## just these:
SC_PRIVATE_TK_HEADERS

#--------------------------------------------------------------------
# __CHANGE__
# A few miscellaneous platform-specific items:
#
# Define a special symbol for Windows (BUILD_itk in this case) so
# that we create the export library with the dll.  See sha1.h on how
# to use this.
#
# Windows creates a few extra files that need to be cleaned up.
# You can add more files to clean if your extension creates any extra
# files.
#
# Define any extra compiler flags in the PACKAGE_CFLAGS variable.
# These will be appended to the current set of compiler flags for
# your system.
#--------------------------------------------------------------------

case "${host}" in
    *cygwin* | *mingw32* | *windows32*)
	AC_DEFINE_UNQUOTED(BUILD_${PACKAGE})
	CLEANFILES="*.lib *.dll *.exp *.ilk *.pdb vc50.pch"
	PLATFORM_SOURCES='$(WIN_SOURCES)'
	PLATFORM_OBJECTS='$(WIN_OBJECTS)'
	PLATFORM_DIR='$(WIN_DIR)'
    ;;
    *)
	CLEANFILES=""
	PLATFORM_SOURCES='$(UNIX_SOURCES)'
	PLATFORM_OBJECTS='$(UNIX_OBJECTS)'
	PLATFORM_DIR='$(UNIX_DIR)'
    ;;
esac

AC_SUBST([CLEANFILES])dnl
AC_SUBST([PLATFORM_SOURCES])dnl
AC_SUBST([PLATFORM_OBJECTS])dnl
AC_SUBST([PLATFORM_DIR])dnl

#--------------------------------------------------------------------
# Check whether --enable-threads or --disable-threads was given.
# So far only Tcl responds to this one.
#--------------------------------------------------------------------

SC_ENABLE_THREADS

#--------------------------------------------------------------------
# The statement below defines a collection of symbols related to
# building as a shared library instead of a static library.
#--------------------------------------------------------------------

SC_ENABLE_SHARED

#--------------------------------------------------------------------
# This macro figures out what flags to use with the compiler/linker
# when building shared/static debug/optimized objects.  This information
# is all taken from the tclConfig.sh file.
#--------------------------------------------------------------------

CFLAGS_DEBUG=${TCL_CFLAGS_DEBUG}
CFLAGS_OPTIMIZE=${TCL_CFLAGS_OPTIMIZE}
LDFLAGS_DEBUG=${TCL_LDFLAGS_DEBUG}
LDFLAGS_OPTIMIZE=${TCL_LDFLAGS_OPTIMIZE}
SHLIB_LD=${TCL_SHLIB_LD}
STLIB_LD=${TCL_STLIB_LD}
SHLIB_CFLAGS=${TCL_SHLIB_CFLAGS}

AC_SUBST([CFLAGS_DEBUG])dnl
AC_SUBST([CFLAGS_OPTIMIZE])dnl
AC_SUBST([STLIB_LD])dnl
AC_SUBST([SHLIB_LD])dnl
AC_SUBST([SHLIB_CFLAGS])dnl
AC_SUBST([SHLIB_LDFLAGS])dnl

#--------------------------------------------------------------------
# Set the default compiler switches based on the --enable-symbols 
# option.
#--------------------------------------------------------------------

SC_ENABLE_SYMBOLS

if test "${SHARED_BUILD}" = "1" ; then
    CFLAGS='${CFLAGS_DEFAULT} ${CFLAGS_WARNING} ${SHLIB_CFLAGS}'
else
    CFLAGS='${CFLAGS_DEFAULT} ${CFLAGS_WARNING}'
fi

#--------------------------------------------------------------------
# Everyone should be linking against the Tcl stub library.  If you
# cannot for some reason, remove this definition.  If you are not using
# stubs, you also need to modify the SHLIB_LD_LIBS setting below to
# link against the non-stubbed Tcl library.
#--------------------------------------------------------------------

if test "${SHARED_BUILD}" = "1" ; then
    AC_DEFINE([USE_TCL_STUBS])
    AC_DEFINE([USE_TK_STUBS])
    AC_DEFINE([USE_ITCL_STUBS])
fi

#--------------------------------------------------------------------
# This macro generates a line to use when building a library.  It
# depends on values set by the SC_ENABLE_SHARED, SC_ENABLE_SYMBOLS,
# and SC_LOAD_TCLCONFIG macros above.
#--------------------------------------------------------------------

SC_MAKE_LIB

#--------------------------------------------------------------------
# eval these two values to dereference the ${DBGX} variable.
#--------------------------------------------------------------------

eval "SHARED_LIB_SUFFIX=${TCL_SHARED_LIB_SUFFIX}"
eval "UNSHARED_LIB_SUFFIX=${TCL_UNSHARED_LIB_SUFFIX}"

#--------------------------------------------------------------------
# Shared libraries and static libraries have different names.
#--------------------------------------------------------------------

# CYGNUS LOCAL

if test "${SHARED_BUILD}" = "1" ; then
  # FIXME: Need to devise a TCL_TOOL macro to deal with this!
  case "${host}" in
    *mingw32* | *windows32*)
        SHLIB_LD_LIBS="${TCL_BUILD_STUB_LIB_SPEC} ${TCL_SHLIB_LD_LIBS} \
            ${ITCL_BUILD_STUB_LIB_SPEC} ${TK_BUILD_STUB_LIB_SPEC}"
        # Need to link to the .a or .lib not the .dll!
        TCL_TOOL_SHARED_LIB_LONGNAME(ITK_SHLIB_FILE, ${PACKAGE}, ${SHARED_LIB_SUFFIX})
        TCL_TOOL_STATIC_LIB_LONGNAME(ITK_LIB_FILE, ${PACKAGE}, ${UNSHARED_LIB_SUFFIX})
        ITK_TARGET_FILE=${ITK_SHLIB_FILE}
     ;;
    *)
        SHLIB_LD_LIBS="${TK_BUILD_STUB_LIB_SPEC} ${TCL_BUILD_STUB_LIB_SPEC} \
            ${ITCL_BUILD_STUB_LIB_SPEC}"
        TCL_TOOL_SHARED_LIB_LONGNAME(ITK_LIB_FILE, ${PACKAGE}, ${SHARED_LIB_SUFFIX})
        ITK_TARGET_FILE=${ITK_LIB_FILE}
     ;;
  esac
else
  TCL_TOOL_STATIC_LIB_LONGNAME(ITK_LIB_FILE, ${PACKAGE}, ${UNSHARED_LIB_SUFFIX})
  ITK_TARGET_FILE=${ITK_LIB_FILE}
fi

TCL_TOOL_STATIC_LIB_LONGNAME(ITK_STUB_LIB_FILE, ${PACKAGE}stub, ${UNSHARED_LIB_SUFFIX})

AC_SUBST([ITK_LIB_FILE])dnl
AC_SUBST([ITK_STUB_LIB_FILE])dnl

TCL_TOOL_LIB_SHORTNAME(ITK_LIB_FLAG, ${PACKAGE}, ${VERSION})
TCL_TOOL_LIB_SHORTNAME(ITK_STUB_LIB_FLAG, ${PACKAGE}stub, ${VERSION})

TCL_TOOL_LIB_SPEC(ITK_BUILD_LIB_SPEC, `pwd`, ${ITK_LIB_FLAG})
TCL_TOOL_LIB_SPEC(ITK_BUILD_STUB_LIB_SPEC, `pwd`, ${ITK_STUB_LIB_FLAG})
AC_SUBST([ITK_BUILD_LIB_SPEC])dnl
AC_SUBST([ITK_BUILD_STUB_LIB_SPEC])dnl

TCL_TOOL_LIB_SPEC(ITK_LIB_SPEC, ${exec_prefix}/lib, ${ITK_LIB_FLAG})
TCL_TOOL_LIB_SPEC(ITK_STUB_LIB_SPEC, ${exec_prefix}/lib, ${ITK_STUB_LIB_FLAG})
AC_SUBST([ITK_LIB_SPEC])dnl
AC_SUBST([ITK_STUB_LIB_SPEC])dnl

TCL_TOOL_LIB_PATH(ITK_LIB_FULL_PATH, `pwd`, ${ITK_LIB_FILE})
TCL_TOOL_LIB_PATH(ITK_STUB_LIB_FULL_PATH, `pwd`, ${ITK_STUB_LIB_FILE})
AC_SUBST([ITK_LIB_FULL_PATH])dnl
AC_SUBST([ITK_STUB_LIB_FULL_PATH])dnl

itkstub_LIB_FILE=${ITK_STUB_LIB_FILE}
itk_LIB_FILE=${ITK_TARGET_FILE}

# END CYGNUS LOCAL

#--------------------------------------------------------------------
# __CHANGE__
# Change the name from exampeA_LIB_FILE to match your package name.
# Use the stub_LIB_FILE substitution if your package creates a stub
# library.
#--------------------------------------------------------------------

AC_SUBST([itkstub_LIB_FILE])dnl
AC_SUBST([itk_LIB_FILE])dnl
AC_SUBST([SHLIB_LD_LIBS])dnl

#--------------------------------------------------------------------
# Find tclsh so that we can run pkg_mkIndex to generate the pkgIndex.tcl
# file during the install process.  Do not run the TCLSH_PROG through
# ${CYGPATH} because it is being used directly by make.
# Require that we use a tclsh shell version 8.2 or later since earlier
# versions have bugs in the pkg_mkIndex routine.
#--------------------------------------------------------------------

# CYGNUS LOCAL
# A Tcl shell is not available when bootstrapping!
if test "x${bootstrapping}" != "xyes"; then
    m4_ifdef([SC_PROG_TCLSH],[
      SC_PROG_TCLSH
    ],[
      AC_PATH_PROGS([TCLSH_PROG],[tclsh tclsh8.4],[],[])dnl
      ## just in case:
      if test "x${TCLSH_PROG}" = "x"; then
        test -z "${TCLSH_PROG}" && test -x /usr/bin/false && export TCLSH_PROG="/usr/bin/false"
      fi
      AC_SUBST([TCLSH_PROG])dnl
    ])dnl
    ## enough?
fi
# END CYGNUS LOCAL

#--------------------------------------------------------------------
# A wish shell is needed to run the test suite.
#--------------------------------------------------------------------

# CYGNUS LOCAL
# A Wish shell is not available when bootstrapping!
if test "x${bootstrapping}" != "xyes"; then
    m4_ifdef([SC_PROG_WISH],[
      SC_PROG_WISH
    ],[
      AC_PATH_PROGS([WISH_PROG],[wish],[],[])dnl
      ## just in case:
      if test "x${WISH_PROG}" = "x"; then
        test -z "${WISH_PROG}" && test -x /usr/bin/false && export WISH_PROG="/usr/bin/false"
      fi
      AC_SUBST([WISH_PROG])dnl
    ])dnl
    ## enough?
fi
# END CYGNUS LOCAL

#--------------------------------------------------------------------
# Finally, substitute all of the various values into the Makefile.
#--------------------------------------------------------------------

if test "x${IWIDGETS_SRC_DIR}" = "x"; then
  test -z "${IWIDGETS_SRC_DIR}" && export IWIDGETS_SRC_DIR=../iwidgets
fi
AC_SUBST([IWIDGETS_SRC_DIR])dnl

# CYGNUS LOCAL

# Note: The itkConfig.sh file below is not included in the net release.
# We subst these variables to retain compatibility with the previous
# version of itkConfig.sh.

ITCL_VERSION=${VERSION}
AC_SUBST([ITCL_VERSION])dnl

ITCL_MAJOR_VERSION=${MAJOR_VERSION}
AC_SUBST([ITCL_MAJOR_VERSION])dnl

ITCL_MINOR_VERSION=${MINOR_VERSION}
AC_SUBST([ITCL_MINOR_VERSION])dnl

ITCL_RELEASE_LEVEL=${PATCHLEVEL}
AC_SUBST([ITCL_RELEASE_LEVEL])dnl

ITK_SRC_DIR=${ITK_SRC_DIR_NATIVE}
AC_SUBST([ITK_SRC_DIR])dnl

AC_CONFIG_FILES([Makefile \
	itkConfig.sh \
	pkgIndex.tcl])
AC_OUTPUT

# END CYGNUS LOCAL
