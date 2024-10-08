dnl# Process this file with autoconf to create configure. -*- autoconf -*-
dnl# (only try that via the boostrap script though; avoid doing so directly)

#   Copyright (C) 2001, 2004, 2005, 2008, 2010 Free Software Foundation,
#   Inc.
#   Written by Gary V. Vaughan, 2001
#
#   This file is part of GNU Libtool.
#
# GNU Libtool is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of
# the License, or (at your option) any later version.
#
# GNU Libtool is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GNU Libtool; see the file COPYING. If not, a copy
# can be downloaded from <http://www.gnu.org/licenses/gpl.html>,
# or obtained by writing to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
####

m4_include([../config/warnings.m4])
AC_PREREQ([2.62])dnl# Some m4sh scripts use ${as_echo}
dnl# Oldest automake required for bootstrap is below in AM_INIT_AUTOMAKE.


## ------------------------ ##
## Autoconf initialisation. ##
## ------------------------ ##
AC_INIT([GNU Libtool],[2.4.2],[bug-libtool@gnu.org])
m4_ifndef([AC_PACKAGE_URL],
	  [AC_SUBST([PACKAGE_URL],[https://www.gnu.org/software/libtool/])])
AC_CONFIG_HEADERS([config.h:config-h.in])
AC_CONFIG_SRCDIR([libtoolize.in])
LT_CONFIG_LTDL_DIR([libltdl],[nonrecursive])
AC_CONFIG_AUX_DIR([libltdl/config])
AC_CONFIG_MACRO_DIR([libltdl/m4])
AC_CONFIG_LIBOBJ_DIR([libltdl])dnl


## ------------------------ ##
## Autotest initialization. ##
## ------------------------ ##

AC_CONFIG_TESTDIR([tests])dnl
AC_PATH_PROGS([AUTOM4TE],[autom4te gautom4te],[autom4te])dnl
AC_PROG_GNU_M4
AC_SUBST([AUTOM4TE],[${AUTOM4TE=autom4te}])dnl
AC_SUBST([AUTOTEST],['$(AUTOM4TE) --language=autotest'])dnl


## ---------------------------------------- ##
## Display a configure time version banner. ##
## ---------------------------------------- ##

# Calculating the previous version is somewhat involved, so do it once
# here, and SUBST the result for the maintainer release rules. The
# versioning scheme we use is described here:
#	https://www.gnu.org/software/libtool/contribute.html

# First we break the version number up into its constituent parts.
lt_major=`echo "AC_PACKAGE_VERSION" | sed ['s,\..*$,,g']`
lt_minor=`echo "AC_PACKAGE_VERSION" | sed ['s,^[0-9]*\.\([0-9]*\).*$,\1,']`
lt_micro=`echo "AC_PACKAGE_VERSION" | sed ['s,^[0-9]*\.[0-9]*\(\.[0-9]*\).*$,\1,']`
lt_alpha=`echo "AC_PACKAGE_VERSION" | sed ['s,^[0-9.]*,,']`

test "x${lt_minor}" = "AC_PACKAGE_VERSION" && lt_minor=0
test "x${lt_micro}" = "AC_PACKAGE_VERSION" && lt_micro=""

# Then we try to work out what the release before this one would have been
# numbered. The only time we come unstuck is when this is the 1st release
# from a stable branch (lt_micro=lt_alpha=""), so LASTRELEASE is the last
# release from the old stable branch, and we don't know what version that
# was, so leave it unset so that the Makefile can complain.
case ${lt_alpha} in
  [[ab]])
    case ${lt_micro} in
      [.[01]])
	LASTRELEASE="${lt_major}.${lt_minor}" ;;
      *)
	LASTRELEASE="${lt_major}.${lt_minor}${lt_micro}" ;;
    esac
    ;;
  [[cegikmoqsuwy]])
    LASTRELEASE=`echo "AC_PACKAGE_VERSION" | tr 'c-y' 'b-w'` ;;
  [[dfhjlnprtvxz]])
    LASTRELEASE=`echo "AC_PACKAGE_VERSION" | tr 'd-z' 'b-x'` ;;
  *) # No lt_alpha component
    case ${lt_micro} in
      .0)
	LASTRELEASE="" ;;
      .1)
        LASTRELEASE="${lt_major}.${lt_minor}" ;;
      .*)
	lt_micro_value=`echo ${lt_micro} | sed 's,^\.,,'`
	LASTRELEASE="${lt_major}.${lt_minor}.`expr ${lt_micro_value} - 1`" ;;
      *)
	LASTRELEASE="" ;;
    esac
    ;;
esac
AC_SUBST([LASTRELEASE])dnl

# This is a sanity check so we can see which ver. is used in bug reports.
# It is assumed that we only want to see the date extension for cvs libtool
# versions (i.e. "odd" letters) and not actual alpha releases.
TIMESTAMP=`${CONFIG_SHELL} ${ac_aux_dir}/mkstamp ${srcdir}`
package_revision=`( set ${TIMESTAMP}; echo $1; )`
case ${lt_alpha} in
  ""|[[bdfhjlnprtvxz]])
    TIMESTAMP=""
    ;;
esac
sync && timestamp_string="${TIMESTAMP:+ (Build:$TIMESTAMP)}" && sync
AS_BOX([Configuring AC_PACKAGE_TARNAME${timestamp_string} AC_PACKAGE_VERSION])
sync && echo ""
AC_SUBST([TIMESTAMP])dnl
AC_SUBST([package_revision])dnl


## -------------------- ##
## architecture checks. ##
## -------------------- ##

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

if test -n "${HOST_ARCHITECTURE}"; then
  ACX_PROG_CC_WARNING_OPTS(["-arch ${HOST_ARCHITECTURE}"],[C_ARCHFLAGS])
  test -n "${C_ARCHFLAGS}" && export C_ARCHFLAGS
  AC_SUBST([C_ARCHFLAGS])
fi
if test "x${CFLAGS}" = "x"; then
  test -z "${CFLAGS}" && export CFLAGS="${C_ARCHFLAGS}"
else
  test -n "${CFLAGS}" && export CFLAGS="${CFLAGS} ${C_ARCHFLAGS}"
fi

## ------------------------ ##
## Automake Initialization. ##
## ------------------------ ##

dnl# These are bootstrap requirements! Once built, libtool may work with
dnl# much older releases of autoconf and automake. See release notes.
dnl# 1.11 is needed for color-tests, 1.11.6 fixes security issues.
AM_INIT_AUTOMAKE([1.11.6 gnits subdir-objects dist-xz color-tests parallel-tests])
AM_MAINTAINER_MODE


## ------------------------- ##
## Autobuild initialization. ##
## ------------------------- ##

# You can set autobuild_mode at configure time to specify a "unique"
# string for this build.
: ${autobuild_mode=default}
AB_VERSION="AC_PACKAGE_VERSION (${TIMESTAMP})"
AB_INIT([${autobuild_mode}])dnl


dnl# We abuse m4sh to generate libtool's portable shell scripts:
AC_SUBST([M4SH],['$(AUTOM4TE) --language=m4sh'])dnl

dnl# Make sure config.status is regenerated when the ver. timestamp changes
AC_SUBST([CONFIG_STATUS_DEPENDENCIES],['$(top_srcdir)/ChangeLog'])dnl


## ------------------------------- ##
## Libtool specific configuration. ##
## ------------------------------- ##

AC_SUBST([aclocaldir],["\${datadir}/aclocal"])dnl
AC_SUBST([pkgdatadir],["\${datadir}/${PACKAGE}"])dnl

AC_ARG_ENABLE([ltdl-install],
    [AS_HELP_STRING([--disable-ltdl-install],[do not install libltdl])])
if test x"${enable_ltdl_install+set}" != x"set"; then
  enable_ltdl_install=yes
fi

# All subdirectories that are configured on demand, but that must be
# included in the distribution.
CONF_SUBDIRS="tests/cdemo tests/demo tests/depdemo tests/f77demo  \
	tests/fcdemo tests/mdemo tests/mdemo2 tests/pdemo tests/tagdemo"
AC_SUBST([CONF_SUBDIRS])dnl

DIST_MAKEFILE_LIST=""
for dir in ${CONF_SUBDIRS}; do
  DIST_MAKEFILE_LIST="${DIST_MAKEFILE_LIST}${dir}/Makefile "
done
AC_SUBST([DIST_MAKEFILE_LIST])dnl

## ---------------- ##
## compiler checks. ##
## ---------------- ##

# Need to be sure that we have a working C compiler, otherwise even with
# a good C compiler: if the following C++ tests fail, the whole libtool
# build fails!
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_CPP
AC_PROG_CXX
AC_PROG_CXXCPP

## ----------------------- ##
## Libtool initialization. ##
## ----------------------- ##
LT_INIT([dlopen win32-dll])
_LT_SET_OPTION([LTDL_INIT],[nonrecursive])
_LTDL_SETUP

# Enable all the language support we can:
LT_LANG([C])dnl
LT_LANG([C++])dnl
LT_LANG([Fortran 77])dnl
LT_LANG([Fortran])dnl
LT_LANG([Go])dnl
  ## gcj is slow:
if test "x${GO_SLOWLY}" = "xyes"; then
  test -n "${GO_SLOWLY}" && echo "Going slowly."
  LT_LANG([Java])dnl
  ## OK
fi
LT_LANG([Windows Resource])dnl

# Ensure the correct file name (and path) conversion function
# is available to the test suite.
AC_SUBST([to_host_file_cmd])dnl
AC_SUBST([to_tool_file_cmd])dnl

## --------------------------- ##
## Work out which tests to run ##
## --------------------------- ##
AM_CONDITIONAL([HAVE_CXX],[test -n "[$]_LT_TAGVAR(compiler, CXX)"])dnl
AM_CONDITIONAL([HAVE_F77],[test -n "[$]_LT_TAGVAR(compiler, F77)"])dnl
AM_CONDITIONAL([HAVE_FC],[test -n "[$]_LT_TAGVAR(compiler, FC)"])dnl
AM_CONDITIONAL([HAVE_GCJ],[test -n "[$]_LT_TAGVAR(compiler, GCJ)"])dnl
AM_CONDITIONAL([HAVE_RC],[test -n "[$]_LT_TAGVAR(compiler, RC)"])dnl

## Whether `make' supports order-only prerequisites.
AC_CACHE_CHECK([whether ${MAKE-make} supports order-only prerequisites],
  [lt_cv_make_order_only],
  [mkdir conftest.dir
   cd conftest.dir
   touch b
   touch a
cat >confmk << 'END'
a: b | c
a b c:
	touch $[]@
END
   touch c
   if ${MAKE-make} -s -q -f confmk >/dev/null 2>&1; then
     lt_cv_make_order_only=yes
   else
     lt_cv_make_order_only=no
   fi
   cd ..
   rm -rf conftest.dir
])
if test "x${lt_cv_make_order_only}" = "xyes"; then
  ORDER='|'
else
  ORDER=''
fi
AC_SUBST([ORDER])dnl

## ------------- ##
## Documentation ##
## ------------- ##
AM_MISSING_PROG([HELP2MAN],[help2man])dnl

if test "x${HELP2MAN}" = "x"; then
  test -z "${HELP2MAN}" && echo "error: help2man is required for documentation!" >&2
  exit 1
fi
AC_CACHE_CHECK([if ${HELP2MAN} supports --no-discard-stderr],
               [ac_cv_help2man_supports_no_discard_stderr],
  [AC_PATH_PROGS_FEATURE_CHECK([HELP2MAN],
                               [${HELP2MAN} help2man ghelp2man `which help2man`],
    [[echo "checking ${ac_path_HELP2MAN}"
      ${ac_path_HELP2MAN} --no-discard-stderr ${ac_path_HELP2MAN} >/dev/null 2>&1 \
      ac_cv_path_HELP2MAN=${ac_path_HELP2MAN}
      ac_path_HELP2MAN_found=:
      ac_cv_help2man_supports_no_discard_stderr=yes
      export NO_DISCARD_STDERR="--no-discard-stderr"]],
    [AC_MSG_WARN([help2man should support the --no-discard-stderr flag])
     ac_cv_help2man_supports_no_discard_stderr=no
     export NO_DISCARD_STDERR=""],
    [${PATH}])dnl# end program check
  ])dnl# end cache check
  ## (need this extra line here)
if test "x${ac_path_HELP2MAN_found}" = "x:" && test "x${ac_cv_help2man_supports_no_discard_stderr}" != "x"; then
  test -n "${ac_cv_help2man_supports_no_discard_stderr}" && echo "ok, good: ${ac_cv_help2man_supports_no_discard_stderr}"
elif test "x${ac_path_HELP2MAN_found}" != "x"; then
  test -n "${ac_path_HELP2MAN_found}" && echo "ac_path_HELP2MAN_found is ${ac_path_HELP2MAN_found}"
elif test "x${ac_cv_path_HELP2MAN}" != "x"; then
  if test "x${ac_cv_help2man_supports_no_discard_stderr}" != "x"; then
    test -n "${ac_cv_path_HELP2MAN}" && test -n "${ac_cv_help2man_supports_no_discard_stderr}" && echo "ac_cv_path_HELP2MAN is ${ac_cv_path_HELP2MAN} (${ac_cv_help2man_supports_no_discard_stderr})"
  else
    test -n "${ac_cv_path_HELP2MAN}" && test -z "${ac_cv_help2man_supports_no_discard_stderr}" && echo "ac_cv_path_HELP2MAN is ${ac_cv_path_HELP2MAN} (other cache value unset)"
  fi
  if test "x${NO_DISCARD_STDERR}" != "x"; then
    test -n "${NO_DISCARD_STDERR}" && echo "NO_DISCARD_STDERR is ${NO_DISCARD_STDERR}"
  else
    test -z "${NO_DISCARD_STDERR}" && echo "NO_DISCARD_STDERR is unset"
  fi
elif test "x${ac_path_HELP2MAN}" != "x"; then
  test -n "${ac_path_HELP2MAN}" && echo "ac_path_HELP2MAN is ${ac_path_HELP2MAN}"
elif test "x${HELP2MAN}" != "x"; then
  test -n "${HELP2MAN}" && echo "HELP2MAN is ${HELP2MAN}"
else
  AC_MSG_WARN([something weird happened here?])
fi
AC_SUBST([ac_cv_help2man_supports_no_discard_stderr])dnl
AC_SUBST([HELP2MAN],[${ac_cv_path_HELP2MAN-${HELP2MAN-help2man}}])dnl
AC_SUBST([NO_DISCARD_STDERR])dnl

## -------- ##
## Outputs. ##
## -------- ##
AC_CONFIG_FILES([Makefile])dnl
AC_OUTPUT
