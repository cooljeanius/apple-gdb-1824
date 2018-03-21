dnl	This file is an input file used by the GNU "autoconf" program to
dnl	generate the file "configure", which is run during [incr Tcl]
dnl installation to configure the system for the local environment.

sinclude(acinclude.m4)

AC_INIT([iwidgets],[4.0.1],[])
AC_CONFIG_SRCDIR([generic/buttonbox.itk])
# RCS: $Id: configure.ac,v 1.2.4.2 2001/05/25 00:11:35 mdejong Exp $

#--------------------------------------------------------------------
# We should really cache the ITCL_VERSION when we configure itcl,
# and pick up the version from the cache here.
#--------------------------------------------------------------------

ITCL_VERSION=3.2
AC_SUBST([ITCL_VERSION])dnl

PACKAGE=iwidgets
MAJOR_VERSION=4
MINOR_VERSION=0
PATCHLEVEL=.1

VERSION=${MAJOR_VERSION}.${MINOR_VERSION}${PATCHLEVEL}

AC_SUBST([PACKAGE])dnl
AC_SUBST([VERSION])dnl

AC_CONFIG_AUX_DIR([../config])
AC_CONFIG_MACRO_DIR([.])
AC_PREREQ([2.0])

AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB

case "`uname -s`" in
    *win32* | *WIN32* | *CYGWIN_NT*|*CYGWIN_98*|*CYGWIN_95*)
	CYGPATH="cygpath -w"
	RELPATH=".. .. bin"
    ;;
    *)
	CYGPATH=echo
	RELPATH=..
    ;;
esac

AC_SUBST([CYGPATH])
AC_SUBST([RELPATH])

#--------------------------------------------------------------------
#   See if there was a command-line option for where Tcl is;  if
#   not, search for Tcl.
#--------------------------------------------------------------------

SC_PATH_TCLCONFIG
SC_LOAD_TCLCONFIG

#--------------------------------------------------------------------
#   See if there was a command-line option for where Tk is;  if
#   not, search for Tk.
#--------------------------------------------------------------------

SC_PATH_TKCONFIG
SC_LOAD_TKCONFIG

#--------------------------------------------------------------------
#   See if there was a command-line option for where Itcl is;  if
#   not, search for Itcl.
#--------------------------------------------------------------------

AC_ARG_WITH([itcl],
        [AS_HELP_STRING([--with-itcl=DIR],[use Itcl 3.0 binaries from DIR])],
        [itcl_search=${withval}],[itcl_search=`cd ${srcdir}/../itcl; pwd`])dnl

ITCL_LIB_DIR=""
for dir in ${exec_prefix}/lib $TCL_LIB_DIR $itcl_search ; do
    if test -r ${dir}/library/itcl.tcl; then
        ITCL_LIB_DIR=$dir
        ITCL_SRC_DIR=$dir
        break
    fi
done

if test -z "$ITCL_LIB_DIR"; then
    AC_MSG_ERROR([Cannot find Itcl libraries.  Use --with-itcl to specify the directory containing itcl.tcl on your system.])
fi

#--------------------------------------------------------------------
#   See if there was a command-line option for where Itk is;  if
#   not, search for Itk.
#--------------------------------------------------------------------

AC_ARG_WITH([itk],
        [AS_HELP_STRING([--with-itk=DIR],[use Itk 3.0 binaries from DIR])],
        [itk_search=${withval}],[itk_search=`cd ${srcdir}/../itk; pwd`])dnl

ITK_LIB_DIR=""
for dir in ${exec_prefix}/lib $TCL_LIB_DIR $itk_search ; do
    if test -r ${dir}/library/itk.tcl; then
        ITK_LIB_DIR=$dir
        ITK_SRC_DIR=$dir
        break
    fi
done

if test -z "$ITK_LIB_DIR"; then
    AC_MSG_ERROR([Cannot find Itk libraries.  Use --with-itk to specify the directory containing itk.tcl on your system.])
fi

#--------------------------------------------------------------------
#   Needed for running the test suite
#--------------------------------------------------------------------

# CYGNUS LOCAL
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
#   Fill in template files with the proper info.
#--------------------------------------------------------------------
AC_SUBST([BUILD_DIR])dnl
AC_SUBST([TCL_LIB_DIR])dnl
AC_SUBST([TCL_SRC_DIR])dnl
AC_SUBST([TK_LIB_DIR])dnl
AC_SUBST([TK_SRC_DIR])dnl
AC_SUBST([ITCL_LIB_DIR])dnl
AC_SUBST([ITCL_SRC_DIR])dnl
AC_SUBST([ITK_LIB_DIR])dnl
AC_SUBST([ITK_SRC_DIR])dnl

if test "x${IWIDGETS_SRC_DIR}" = "x"; then
  test -z "${IWIDGETS_SRC_DIR}" && export IWIDGETS_SRC_DIR="`cd ${srcdir}; pwd`"
fi
AC_SUBST([ITCL_VERSION])dnl
AC_SUBST([IWIDGETS_VERSION])dnl
AC_SUBST([IWIDGETS_LIBRARY])dnl
AC_SUBST([IWIDGETS_SRC_DIR])dnl

AC_CONFIG_FILES([Makefile \
	iwidgets.tcl \
	pkgIndex.tcl])
AC_OUTPUT
