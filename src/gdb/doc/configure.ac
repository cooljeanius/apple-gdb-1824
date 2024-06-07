dnl#                                               -*- Autoconf -*-
dnl# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.59])dnl
AC_INIT([gdb-doc],[1824],[jmolenda@apple.com])
dnl# bug report address is email address of last person to touch this
dnl# directory, according to the Changelog-Apple in this directory
AC_CONFIG_AUX_DIR([../..])
AC_REQUIRE_AUX_FILE([mkinstalldirs])
AC_CONFIG_MACRO_DIR([../m4])
AC_CONFIG_SRCDIR([refcard.tex])
m4_ifdef([ACX_PKGVERSION],[
ACX_PKGVERSION([GDBDOC])dnl
  ## make sure it is defined:
if test "x${PKGVERSION}" != "x"; then
  test -n "${PKGVERSION}"
  AC_DEFINE_UNQUOTED([PKGVERSION],["${PKGVERSION}"],
                     [Additional package description])dnl
  ## 2...
fi
],[
if test "x${PKGVERSION}" = "x"; then
  test -z "${PKGVERSION}" && PKGVERSION=GDBDOC
fi
AC_SUBST([PKGVERSION])dnl
])dnl
m4_ifdef([ACX_BUGURL],[
ACX_BUGURL([http://www.gnu.org/software/gdb/bugs/])
if test "x${REPORT_BUGS_TO}" != "x"; then
  test -n "${REPORT_BUGS_TO}"
  AC_DEFINE_UNQUOTED([REPORT_BUGS_TO],["${REPORT_BUGS_TO}"],
                     [Bug reporting address])dnl
  ## 2...
fi
],[
if test "x${REPORT_BUGS_TO}" = "x"; then
  test -z "${REPORT_BUGS_TO}" && REPORT_BUGS_TO="/dev/null"
fi
AC_SUBST([REPORT_BUGS_TO])dnl
  ## this too:
if test "x${REPORT_BUGS_TEXI}" = "x"; then
  test -z "${REPORT_BUGS_TEXI}" && REPORT_BUGS_TEXI=""
fi
AC_SUBST([REPORT_BUGS_TEXI])dnl
])dnl
AM_MAINTAINER_MODE
AM_SILENT_RULES
#### check arg val for silent rules:
if test "x${enable_silent_rules}" = "xno"; then
  : ${V=1}
  : ${AM_DEFAULT_VERBOSITY=1}
elif test "x${enable_silent_rules}" = "xyes"; then
  : ${V=0}
  : ${AM_DEFAULT_VERBOSITY=0}
else
  : ${V=0}
  : ${AM_DEFAULT_VERBOSITY=1}
fi
: ${AM_DEFAULT_VERBOSITY=1}
: ${AM_V=${AM_DEFAULT_VERBOSITY}}
: ${AM_DEFAULT_V=${AM_DEFAULT_VERBOSITY}}
dnl# now subst:
AC_SUBST([V])dnl
AM_SUBST_NOTMAKE([V])dnl
AC_SUBST([AM_V])dnl
AM_SUBST_NOTMAKE([AM_V])dnl
AC_SUBST([AM_DEFAULT_V])dnl
AM_SUBST_NOTMAKE([AM_DEFAULT_V])dnl
AC_SUBST([AM_DEFAULT_VERBOSITY])dnl

# Checks for programs.
AC_PROG_CPP
AC_PROG_FGREP
AC_PROG_INSTALL
AC_PROG_LN_S
if test "x${ac_cv_prog_make_make_set}" = "x"; then
  test -z "${ac_cv_prog_make_make_set}"
  AC_PROG_MAKE_SET
fi
AC_PROG_MKDIR_P
AC_PROG_SED

# Hack:
if test -e ../config.cache; then
  echo "hacking CPPFLAGS out of ../config.cache"
  sed -i.bak -e "s|ac_cv_env_CPPFLAGS_set|bad_CPPFLAGS_status|g" ../config.cache
  sed -i.bak -e "s|ac_cv_env_CPPFLAGS_value|bad_CPPFLAGS|g" ../config.cache
elif test -r .././config.cache -a -w .././config.cache; then
  echo "hacking CPPFLAGS out of .././config.cache"
  sed -i.bak -e "s|ac_cv_env_CPPFLAGS_set|bad_CPPFLAGS_status|g" .././config.cache
  sed -i.bak -e "s|ac_cv_env_CPPFLAGS_value|bad_CPPFLAGS|g" .././config.cache
else
  echo "config.cache is missing, which means we can skip attempts to hack it"
fi

# Normally our parent configure script would subst these for us, but it
# would be too much of a pain to rework it for this subdir, so just use
# dummy values:
frags=""
if test "x${host_makefile_frag}" = "x"; then
  test -z "${host_makefile_frag}" && host_makefile_frag=/dev/null
fi
frags="${frags} ${host_makefile_frag}"
AC_SUBST_FILE([host_makefile_frag])dnl
AC_SUBST([frags])dnl

if test "x${HAVE_NATIVE_GCORE_TARGET}" = "x"; then
  test -z "${HAVE_NATIVE_GCORE_TARGET}" && HAVE_NATIVE_GCORE_TARGET="0"
fi
AC_SUBST([HAVE_NATIVE_GCORE_TARGET])dnl

if test "x${SYSTEM_GDBINIT}" = "x"; then
  test -z "${SYSTEM_GDBINIT}" && SYSTEM_GDBINIT=""
fi
AC_SUBST([SYSTEM_GDBINIT])dnl

PACKAGE=gdb-doc
AC_DEFINE_UNQUOTED([PACKAGE],["${PACKAGE}"],[Name of this package. ])dnl
AC_SUBST([PACKAGE])dnl

# We never need to detect it in this sub-configure.
# But preserve it for config.status --recheck.
AC_ARG_VAR([MAKEINFO],
  [Parent configure detects if it is of sufficient version.])dnl
AC_ARG_VAR([MAKEINFOFLAGS],
  [Parameters for MAKEINFO.])dnl

AM_MISSING_PROG([MAKEINFO],[makeinfo])dnl
AC_SUBST([MAKEINFO])dnl

if test "x${MAKEINFO_EXTRA_FLAGS}" = "x"; then
  test -z "${MAKEINFO_EXTRA_FLAGS}" && MAKEINFO_EXTRA_FLAGS=""
fi
AC_SUBST([MAKEINFO_EXTRA_FLAGS])dnl

if test "x${READLINE_TEXI_INCFLAG}" = "x"; then
  test -z "${READLINE_TEXI_INCFLAG}" && READLINE_TEXI_INCFLAG="-I ../../readline/doc"
fi
AC_SUBST([READLINE_TEXI_INCFLAG])dnl

AM_MISSING_PROG([HELP2MAN],[help2man])dnl
AC_SUBST([HELP2MAN])dnl

#TODO: convert all these "missing prog" checks to real checks:
AM_MISSING_PROG([TEX],[tex])dnl
AM_MISSING_PROG([TEXI2DVI],[texi2dvi])dnl
AM_MISSING_PROG([TEXI2HTML],[texi2html])dnl
AM_MISSING_PROG([TEXI2ROFF],[texi2roff])dnl
AM_MISSING_PROG([PDFTEX],[pdftex])dnl
AM_MISSING_PROG([DVIPS],[dvips])dnl
AM_MISSING_PROG([PERL],[perl])dnl
AM_MISSING_PROG([POD2MAN],[pod2man])dnl
AM_MISSING_PROG([DOXYGEN],[doxygen])dnl
AM_MISSING_PROG([INSTALL_INFO],[install-info])dnl

# Output:
AC_CONFIG_FILES([Makefile])dnl
AC_OUTPUT
