dnl#	This file is an input file used by the GNU "autoconf" program to
dnl#	generate the file "configure", which is run during [incr Tcl]
dnl#    installation to configure the system for the local environment.
dnl#  RCS: $Id: configure.ac,v 1.12.150.3 2001/05/18 02:21:42 mdejong Exp $

# CYGNUS LOCAL
AC_PREREQ([2.5])dnl

AC_INIT([itcl],[3.2],[http://www.tcltk.com/itcl/])
AC_CONFIG_SRCDIR([itcl/generic/itcl.h])
AC_CANONICAL_TARGET

# Note, we do NOT use the files in the config subdirectory!
AC_CONFIG_AUX_DIR([..])
AC_PROG_MAKE_SET
# END CYGNUS LOCAL

m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES([yes])])dnl

if test "x${V}" = "x"; then
  test -z "${V}" && export V=0
fi
AC_SUBST([V])dnl

dnl########################################################################
dnl# Port to Automake 1.11.
dnl# This section can be removed once we assume Automake 1.14 or later.
if test "x${V}" != "x"; then
  test -n "${V}" || export V=0
  : ${AM_DEFAULT_VERBOSITY=${V}}
else
  : ${AM_DEFAULT_VERBOSITY=0}
fi
: ${AM_V=${AM_DEFAULT_VERBOSITY}}
: ${AM_DEFAULT_V=${AM_DEFAULT_VERBOSITY}}
dnl# now subst:
AC_SUBST([AM_V])dnl
m4_ifdef([AM_SUBST_NOTMAKE],[
AM_SUBST_NOTMAKE([AM_V])dnl
])dnl
AC_SUBST([AM_DEFAULT_V])dnl
m4_ifdef([AM_SUBST_NOTMAKE],[
AM_SUBST_NOTMAKE([AM_DEFAULT_V])dnl
])dnl
AC_SUBST([AM_DEFAULT_VERBOSITY])dnl

# -----------------------------------------------------------------------
#
#   Set up a new default --prefix.  If a previous installation of
#   [incr Tcl] can be found searching $PATH use that directory.
#
# -----------------------------------------------------------------------

AC_PREFIX_DEFAULT([/usr/local])
AC_PREFIX_PROGRAM([itclsh])dnl

# Output.
AC_CONFIG_SUBDIRS([itcl itk])
# Source-Navigator does not use the iwidgets package:
if test ! -d ${srcdir}/../snavigator || test -d ${srcdir}/../gdb/gdbtk; then
    AC_CONFIG_SUBDIRS([iwidgets])
fi

AC_CONFIG_FILES([Makefile])
AC_CONFIG_COMMANDS([default],[chmod +x ${srcdir}/config/install-sh ${srcdir}/config/mkinstalldirs],[])
AC_OUTPUT
