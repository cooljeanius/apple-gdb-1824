dnl#                                               -*- Autoconf -*-
dnl#	This file is an input file used by the GNU "autoconf" program to
dnl#	generate the file "configure", which is run to configure the
dnl#	Makefile in this directory.

AC_PREREQ([2.13])
AC_INIT([tcl-tools],[8.4],[tromey@cygnus.com])
AC_CONFIG_SRCDIR([man2tcl.c])
# RCS: @(#) configure.ac,v 1.5 2003/01/21 19:40:17 hunt Exp

# Recover information that Tcl computed with its configure script.

#--------------------------------------------------------------------
#      See if there was a command-line option for where Tcl is;  if
#      not, assume that its top-level directory is a sibling of ours.
#--------------------------------------------------------------------

DEF_VER=8.4

AC_ARG_WITH([tcl],[AS_HELP_STRING([--with-tcl=DIR],[use Tcl ${DEF_VER} binaries from DIR])],[TCL_BIN_DIR=${withval}],[TCL_BIN_DIR=`cd ../../tcl${DEF_VER}${TCL_PATCH_LEVEL}/unix 2>/dev/null; pwd`])
if test ! -d ${TCL_BIN_DIR}; then
    AC_MSG_ERROR([Tcl directory ${TCL_BIN_DIR} does NOT exist])
fi
if test ! -f ${TCL_BIN_DIR}/tclConfig.sh; then
    AC_MSG_WARN([There is no tclConfig.sh in ${TCL_BIN_DIR}; perhaps you failed to specify the Tcl *build* directory (not the toplevel Tcl directory) or you forgot to configure Tcl? (this should be a hard error, but we are ignoring it to facilitate work on testing this configure script)])
else
    AC_MSG_NOTICE([sourcing ${TCL_BIN_DIR}/tclConfig.sh])
    . $TCL_BIN_DIR/tclConfig.sh
fi

TCL_WIN_VERSION="${TCL_MAJOR_VERSION}${TCL_MINOR_VERSION}"
AC_SUBST([TCL_WIN_VERSION])
AC_PROG_CC
AC_CHECK_HEADERS([stdio.h])
AC_CHECK_FUNCS([strerror])
AC_SUBST([TCL_VERSION])
AC_SUBST([TCL_PATCH_LEVEL])
AC_SUBST([TCL_SRC_DIR])
AC_SUBST([TCL_BIN_DIR])

AC_CONFIG_FILES([Makefile tcl.hpj])
AC_OUTPUT
