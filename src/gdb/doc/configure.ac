dnl#                                               -*- Autoconf -*-
dnl# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.59])
AC_INIT([gdb-doc],[1824],[jmolenda@apple.com])
dnl# bug report address is email address of last person to touch this
dnl# directory, according to the Changelog-Apple in this directory
AC_CONFIG_AUX_DIR([../..])
AC_REQUIRE_AUX_FILE([mkinstalldirs])
AC_CONFIG_MACRO_DIR([../m4])
AC_CONFIG_SRCDIR([refcard.tex])
m4_ifdef([ACX_PKGVERSION],[
ACX_PKGVERSION([GDBDOC])
if test "x${PKGVERSION}" != "x"; then
  test -n "${PKGVERSION}"
  AC_DEFINE_UNQUOTED([PKGVERSION],["${PKGVERSION}"],
                     [Additional package description])
fi
],[
if test "x${PKGVERSION}" = "x"; then
  test -z "${PKGVERSION}" && PKGVERSION=GDBDOC
fi
AC_SUBST([PKGVERSION])
])
m4_ifdef([ACX_BUGURL],[
ACX_BUGURL([http://www.gnu.org/software/gdb/bugs/])
if test "x${REPORT_BUGS_TO}" != "x"; then
  test -n "${REPORT_BUGS_TO}"
  AC_DEFINE_UNQUOTED([REPORT_BUGS_TO],["${REPORT_BUGS_TO}"],
                     [Bug reporting address])
fi
],[
if test "x${REPORT_BUGS_TO}" = "x"; then
  test -z "${REPORT_BUGS_TO}" && REPORT_BUGS_TO="/dev/null"
fi
AC_SUBST([REPORT_BUGS_TO])
if test "x${REPORT_BUGS_TEXI}" = "x"; then
  test -z "${REPORT_BUGS_TEXI}" && REPORT_BUGS_TEXI=""
fi
AC_SUBST([REPORT_BUGS_TEXI])
])
AM_MAINTAINER_MODE

# Checks for programs.
AC_PROG_FGREP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MKDIR_P
AC_PROG_SED

# Normally our parent configure script would subst these for us, but it
# would be too much of a pain to rework it for this subdir, so just use
# dummy values:
frags=""
if test "x${host_makefile_frag}" = "x"; then
  test -z "${host_makefile_frag}" && host_makefile_frag=/dev/null
fi
frags="${frags} ${host_makefile_frag}"
AC_SUBST_FILE([host_makefile_frag])
AC_SUBST([frags])

if test "x${HAVE_NATIVE_GCORE_TARGET}" = "x"; then
  test -z "${HAVE_NATIVE_GCORE_TARGET}" && HAVE_NATIVE_GCORE_TARGET="0"
fi
AC_SUBST([HAVE_NATIVE_GCORE_TARGET])

if test "x${SYSTEM_GDBINIT}" = "x"; then
  test -z "${SYSTEM_GDBINIT}" && SYSTEM_GDBINIT=""
fi
AC_SUBST([SYSTEM_GDBINIT])

PACKAGE=gdb-doc
AC_DEFINE_UNQUOTED([PACKAGE],["${PACKAGE}"],[Name of this package. ])
AC_SUBST([PACKAGE])

# We never need to detect it in this sub-configure.
# But preserve it for config.status --recheck.
AC_ARG_VAR([MAKEINFO],
  [Parent configure detects if it is of sufficient version.])
AC_ARG_VAR([MAKEINFOFLAGS],
  [Parameters for MAKEINFO.])

AM_MISSING_PROG([MAKEINFO],[makeinfo])
AC_SUBST([MAKEINFO])

if test "x${MAKEINFO_EXTRA_FLAGS}" = "x"; then
  test -z "${MAKEINFO_EXTRA_FLAGS}" && MAKEINFO_EXTRA_FLAGS=""
fi
AC_SUBST([MAKEINFO_EXTRA_FLAGS])

if test "x${READLINE_TEXI_INCFLAG}" = "x"; then
  test -z "${READLINE_TEXI_INCFLAG}" && READLINE_TEXI_INCFLAG="-I ../../readline/doc"
fi
AC_SUBST([READLINE_TEXI_INCFLAG])

AM_MISSING_PROG([HELP2MAN],[help2man])
AC_SUBST([HELP2MAN])

#TODO: convert all these "missing prog" checks to real checks:
AM_MISSING_PROG([TEX],[tex])
AM_MISSING_PROG([TEXI2DVI],[texi2dvi])
AM_MISSING_PROG([TEXI2HTML],[texi2html])
AM_MISSING_PROG([TEXI2ROFF],[texi2roff])
AM_MISSING_PROG([PDFTEX],[pdftex])
AM_MISSING_PROG([DVIPS],[dvips])
AM_MISSING_PROG([PERL],[perl])
AM_MISSING_PROG([POD2MAN],[pod2man])
AM_MISSING_PROG([DOXYGEN],[doxygen])
AM_MISSING_PROG([INSTALL_INFO],[install-info])

# Output:
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
