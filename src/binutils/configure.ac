dnl#                                               -*- Autoconf -*-
dnl# Process this file with autoconf to produce a configure script.
dnl#

AC_PREREQ([2.59])
m4_include([../bfd/version.m4])
m4_include([../config/warnings.m4])
dnl# keep version number synced with the one in '../bfd':
AC_INIT([binutils],[2.16.91],[jmolenda@apple.com])
dnl# bug report address is email address of last person to touch this
dnl# directory, according to the Changelog-Apple in this directory.
AC_CONFIG_SRCDIR([ar.c])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([..])
AC_REQUIRE_AUX_FILE([depcomp])dnl
AC_REQUIRE_AUX_FILE([mkinstalldirs])dnl

# target stuff:
AC_MSG_CHECKING([system])
AC_MSG_RESULT([ ])
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
AC_SEARCH_LIBS([strerror],[cposix]) dnl# used to be AC\_ISC\_POSIX
if test -n "${HOST_ARCHITECTURE}"; then
  ACX_PROG_CC_WARNING_OPTS(["-arch ${HOST_ARCHITECTURE}"],[C_ARCHFLAGS])
  AC_SUBST([C_ARCHFLAGS])
fi

# Allow the user to override the flags for our build compiler if desired:
if test x"${build}" = x"${host}"; then
  CFLAGS_FOR_BUILD=${CFLAGS_FOR_BUILD-${CFLAGS}}
  CXXFLAGS_FOR_BUILD=${CXXFLAGS_FOR_BUILD-${CXXFLAGS}}
  LDFLAGS_FOR_BUILD=${LDFLAGS_FOR_BUILD-${LDFLAGS}}
else
  test -n "${CFLAGS_FOR_BUILD}"
  test -n "${CXXFLAGS_FOR_BUILD}"
  test -n "${LDFLAGS_FOR_BUILD}"
fi
AC_SUBST([CFLAGS_FOR_BUILD])dnl
AC_SUBST([CXXFLAGS_FOR_BUILD])dnl
AC_SUBST([LDFLAGS_FOR_BUILD])dnl

# Automake:
AC_MSG_CHECKING([automake stuff])
AC_MSG_RESULT([ ])
if test -f ${srcdir}/config.status; then
  AC_MSG_NOTICE([config.status is already present, moving it aside])
  mv -vf ${srcdir}/config.status ${srcdir}/config.status~
fi
AM_INIT_AUTOMAKE([1.11.6 gnits dejagnu no-dist info-in-builddir -Wno-obsolete])dnl
AM_SANITY_CHECK

# Make sure that sed is checked early:
AC_DEFUN([AC_REQUIRE_SED],[
  AC_REQUIRE([AC_PROG_SED])dnl
])dnl
AC_REQUIRE_SED

if test -e `pwd`/config.cache; then
  echo "hacking `pwd`/config.cache"
  sed -i.bak -e "s|ac_cv_env_CFLAGS_value|bad_CFLAGS|g" `pwd`/config.cache
  sed -i.bak -e "s|ac_cv_env_CXXFLAGS_value|bad_CXXFLAGS|g" `pwd`/config.cache
  sed -i.bak -e "s|ac_cv_exeext|bad_exeext|g" `pwd`/config.cache
elif test -e ./config.cache; then
  echo "hacking ./config.cache"
  sed -i.bak -e "s|ac_cv_env_CFLAGS_value|bad_CFLAGS|g" ./config.cache
  sed -i.bak -e "s|ac_cv_env_CXXFLAGS_value|bad_CXXFLAGS|g" ./config.cache
  sed -i.bak -e "s|ac_cv_exeext|bad_exeext|g" ./config.cache
else
  echo "config.cache is missing, which means we can skip attempts to hack it"
fi

# Libtool checks:
AC_MSG_CHECKING([libtool stuff])
AC_MSG_RESULT([ ])
AC_PROG_CXX
LT_INIT([disable-fast-install disable-shared dlopen static win32-dll])dnl
LT_LANG([C])dnl
LT_LANG([C++])dnl
LT_LANG([Windows Resource])dnl
dnl# temporarily set this while I update everything libtool-related:
LT_PREREQ([2.4.7])dnl
dnl# (largefile check is done farther below)

# flags:
AC_ARG_ENABLE([targets],
[AS_HELP_STRING([--enable-targets],
                [A comma-delimited list of alternate target configurations.
                 Alternatively, whitespace-delimiting it could work, too,
                 if you remember to quote it properly.])],
[case "${enableval}" in
  yes | "")
    AC_MSG_ERROR([enable-targets flag must specify target names or 'all'])
    ;;
  no) enable_targets="" ;;
  *)  enable_targets="${enableval}" ;;
esac])dnl

AC_ARG_ENABLE([commonbfdlib],
[AS_HELP_STRING([--enable-commonbfdlib],
                [build shared BFD/opcodes/libiberty library])],
[case "${enableval}" in
  yes) commonbfdlib=true ;;
  no) commonbfdlib=false ;;
  *) AC_MSG_ERROR([bad value ${enableval} for BFD commonbfdlib flag]) ;;
esac])dnl

AC_ARG_ENABLE([deterministic-archives],
[AS_HELP_STRING([--enable-deterministic-archives],
		[ar and ranlib default to -D behavior])],[
if test "x${enableval}" = "xno"; then
  default_ar_deterministic=0
else
  default_ar_deterministic=1
fi],[default_ar_deterministic=0])dnl

AC_DEFINE_UNQUOTED([DEFAULT_AR_DETERMINISTIC],
                   [${default_ar_deterministic}],
		   [Should ar and ranlib use -D behavior by default?])dnl

AC_ARG_ENABLE([default-strings-all],
[AS_HELP_STRING([--disable-default-strings-all],
		[strings defaults to --data behavior])],[
if test "x${enableval}" = "xno"; then
  default_strings_all=0
else
  default_strings_all=1
fi],[default_strings_all=1])dnl

AC_DEFINE_UNQUOTED([DEFAULT_STRINGS_ALL],[${default_strings_all}],
		   [Should strings use -a behavior by default?])dnl

AM_BINUTILS_WARNINGS
gl_COMPILER_OPTION_IF([-Wstrict-overflow=2],
                      [WARN_CFLAGS="${WARN_CFLAGS} -Wstrict-overflow=2"],
                      [gl_WARN_ADD([-Wstrict-overflow])])dnl
gl_WARN_ADD([-Wsign-compare])dnl
gl_WARN_ADD([-Wfloat-conversion])dnl
gl_WARN_ADD([-Wno-missed-loop-optimizations])dnl

AM_BINUTILS_CLANG_STATIC_ANALYSIS
		   
AC_CONFIG_HEADERS([config.h])dnl
AH_VERBATIM([00_CONFIG_H_CHECK],[
/* Check that config.h is #included before system headers
 * (this works only for glibc, but that should be enough).  */
#if defined(__GLIBC__) && !defined(__FreeBSD_kernel__) && !defined(__CONFIG_H__)
#  error "config.h must be #included before system headers"
#endif /* __GLIBC__ && !__FreeBSD_kernel__ && !__CONFIG_H__ */
#define __CONFIG_H__ 1])dnl
AH_TOP([
#ifndef HAVE_CONFIG_H
# define HAVE_CONFIG_H 1
#endif /* !HAVE_CONFIG_H */
])dnl

# Checks for systems.
if test -z "${target}"; then
  AC_MSG_ERROR([Unrecognized target system type; please check config.sub.])
else
  test -n "${target}"
  AC_SUBST([target])
fi
if test -z "${host}"; then
  AC_MSG_ERROR([Unrecognized host system type; please check config.sub.])
else
  test -n "${host}"
  AC_SUBST([host])
fi
AC_SYS_INTERPRETER
m4_ifdef([ACX_LARGEFILE],[
  ACX_LARGEFILE dnl# calls the one in the other half of the conditional
],[
  AC_SYS_LARGEFILE
])dnl

# Checks for programs.
AC_MSG_CHECKING([programs])
AC_MSG_RESULT([ ])
AM_PROG_AS
if test "x${ac_cv_prog_AR}" = "x"; then
  test -z "${ac_cv_prog_AR}" || unset ac_cv_prog_AR
  AC_PROG_AR
fi
if test "x${CC}" = "x"; then
  test -z "${CC}"
  AC_PROG_CC
else
  test -n "${CC}" && export CC
fi

AC_PROG_YACC
AX_PROG_BISON([
  EXTRA_YFLAGS_SEP=''
  AC_CACHE_CHECK([if your bison is a new enough version of GNU bison],
                 [ax_cv_prog_gnu_bison],[
    AS_IF([test -n "`${YACC} --version 2>/dev/null | grep 'GNU' | grep ' 3.'`"],
      [ax_cv_prog_gnu_bison=yes],[ax_cv_prog_gnu_bison=no])dnl
  ])dnl
  ## one more separator:
  AS_IF([test "x${ax_cv_prog_gnu_bison}" = "xyes"],[
    YFLAGS_SEP1=''
    AC_DEFINE([BISON_IS_V3_OR_LATER],[1],
              [Define to 1 if your bison is a new enough version of GNU bison])
  ],[
    YFLAGS_SEP1='#'
  ])dnl
  ## (hopefully this will be enough)
  AC_MSG_CHECKING([full bison version info])
  BISON_VERSION="`${YACC} --version | head -n 1`"
  AC_MSG_RESULT([${BISON_VERSION}])
  AC_DEFINE_UNQUOTED([BISON_VERSION],["${BISON_VERSION}"],[Version of bison])
  AC_SUBST([BISON_VERSION])dnl
],[
  EXTRA_YFLAGS_SEP='#'
  YFLAGS_SEP1='#'
])dnl##end bison conditional
AC_SUBST([EXTRA_YFLAGS_SEP])dnl
AC_SUBST([YFLAGS_SEP1])dnl
AC_PROG_LEX([yywrap])
AM_PROG_LEX dnl# is required by the next 1, but call it explicitly anyways
AX_PROG_FLEX([
  # (this is not currently used, but it could be)
  EXTRA_LFLAGS_SEP=''
],[
  EXTRA_LFLAGS_SEP='#'
])dnl##end flex conditional
AC_SUBST([EXTRA_LFLAGS_SEP])dnl
AC_PATH_PROGS([ICONV_BIN],[iconv giconv])dnl
AC_SUBST([ICONV_BIN])dnl
AC_PATH_PROG([CONFIGURED_POD2MAN],[pod2man])dnl

AC_PATH_PROG([SHELLCHECK],[shellcheck],[shellcheck])dnl

dnl# for ResourceManager-related stuff:
AC_PATH_PROG([REZ],[Rez],[],[],[${PATH}:/Developer/Tools])dnl
AC_SUBST([REZ])dnl

# Gettext:
if test -d ../intl; then
  if test -z "${LDFLAGS}"; then
    export LDFLAGS="-L. -L../intl"
  else
    export LDFLAGS="${LDFLAGS} -L../intl"
  fi
elif test -d ../libintl; then
  if test -z "${LDFLAGS}"; then
    export LDFLAGS="-L. -L../libintl"
  else
    export LDFLAGS="${LDFLAGS} -L../libintl"
  fi
fi
# keep alphabetical:
ALL_LINGUAS="bg da es fi fr hr id it ja ro ru rw sk sr sv tr uk vi zh_CN zh_TW"
m4_ifdef([CY_GNU_GETTEXT],[
  CY_GNU_GETTEXT dnl# older version
],[
  m4_ifdef([ZW_GNU_GETTEXT_SISTER_DIR],[
    ZW_GNU_GETTEXT_SISTER_DIR dnl# newer version
  ],[
    AC_MSG_NOTICE([skipping some gettext checks])
  ])dnl
  m4_ifdef([AM_PO_SUBDIRS],[
    AM_PO_SUBDIRS dnl# kinda separate from the newer version
  ],[
    AC_MSG_WARN([po subdir might remain unconfigured])
  ])dnl
])dnl

AM_MAINTAINER_MODE
dnl# this should fix the "building texinfos in srcdir" described in the
dnl# Makefile:
AM_CONDITIONAL([GENINSRC_NEVER],[false])dnl
AC_OBJEXT
AC_EXEEXT
if test -n "${EXEEXT}"; then
  AC_DEFINE([HAVE_EXECUTABLE_SUFFIX],[1],
	    [Does the platform use an executable suffix?])
fi
AC_DEFINE_UNQUOTED([EXECUTABLE_SUFFIX],["${EXEEXT}"],
		   [Suffix used for executables, if any.])dnl

# host-specific stuff:
HDEFINES=""

AC_MSG_NOTICE([sourcing ../bfd/configure.host])
. ${srcdir}/../bfd/configure.host

AC_SUBST([HDEFINES])dnl

# More programs:
AR=${AR-ar}
AC_SUBST([AR])dnl
dnl# (skip actually checking for ar, because we build it here)
  ## do check for ranlib if necessary though:
if test "x${RANLIB}" = "x" && test "x${ac_cv_prog_ac_ct_RANLIB}" = "x"; then
  test -z "${RANLIB}" && test -z "${ac_cv_prog_ac_ct_RANLIB}"
  AC_PROG_RANLIB
else
  test ! -z "${RANLIB}"
  test -n "${ac_cv_prog_ac_ct_RANLIB}"
fi
AC_PROG_INSTALL
AM_PROG_INSTALL_STRIP
AC_PROG_LN_S
AC_PROG_MKDIR_P

AC_DEFUN([AM_REQUIRE_EXTRA_AUTOMAKE_CHECKS],[
  AC_PREREQ([2.65])dnl
  dnl# Autoconf wants to disallow AM_ names. We explicitly allow
  dnl# the ones we care about.
  m4_pattern_allow([^AM_[A-Z]+FLAGS$])dnl
  AC_REQUIRE([AM_DEP_TRACK])dnl
  AC_REQUIRE([AM_OUTPUT_DEPENDENCY_COMMANDS])dnl
  AC_REQUIRE([AM_MISSING_HAS_RUN])dnl
  AC_REQUIRE([AM_MAINTAINER_MODE])dnl
  AC_REQUIRE([AC_PROG_GREP])dnl
  AC_REQUIRE([AC_PROG_EGREP])dnl
  AC_REQUIRE([AC_PROG_FGREP])dnl
  AC_REQUIRE([AM_MAKE_INCLUDE])dnl
  AC_REQUIRE([AM_SANITY_CHECK])dnl
  AC_REQUIRE([AC_PROG_AWK])dnl
  AC_REQUIRE([AC_PROG_CPP])dnl
])dnl
AM_REQUIRE_EXTRA_AUTOMAKE_CHECKS

BFD_CC_FOR_BUILD

DEMANGLER_NAME=c++filt
case "${host}" in
  *-*-go32* | *-*-msdos*)
    DEMANGLER_NAME=cxxfilt
esac
AC_SUBST([DEMANGLER_NAME])dnl

# Checks for libraries.
AC_MSG_CHECKING([libraries])
AC_MSG_RESULT([ ])
AC_MSG_CHECKING([for linker flag to verify linked libraries have the correct architecture])
# this is also done in warning.m4, but make sure it is done up here first:
if test "x${acl_cv_wl}" = "x"; then
  test -z "${acl_cv_wl}" && export acl_cv_wl='-Wl,'
fi
pre_saved_LDFLAGS="${LDFLAGS}"
LDFLAGS="${LDFLAGS} ${acl_cv_wl}-arch_errors_fatal"
AC_LINK_IFELSE([AC_LANG_PROGRAM([[]],[[]])],
               [LD_ARCHFLAGS="${acl_cv_wl}-arch_errors_fatal"],[])
LDFLAGS="${pre_saved_LDFLAGS}"
if test "x${LD_ARCHFLAGS}" != "x"; then
  test -n "${LD_ARCHFLAGS}"
  AC_MSG_RESULT([${LD_ARCHFLAGS}])
  if test "x${ERROR_ON_WARNING}" = "xyes"; then
    if test "x${WARN_LDFLAGS}" != "x"; then
      test -n "${WARN_LDFLAGS}" && WARN_LDFLAGS="${WARN_LDFLAGS} ${LD_ARCHFLAGS}"
    else
      test -z "${WARN_LDFLAGS}" && WARN_LDFLAGS="${LD_ARCHFLAGS}"
    fi
  fi
  AC_SUBST([LD_ARCHFLAGS])dnl
  ## that should be all we need to do with them for now...
else
  test -z "${LD_ARCHFLAGS}"
  AC_MSG_RESULT([none])
fi
LT_LIB_M
# ('../intl' is added to LDFLAGS above)
AC_SEARCH_LIBS([dcgettext],[intl])dnl
AC_SEARCH_LIBS([bindtextdomain],[intl])dnl
AC_SEARCH_LIBS([textdomain],[intl])dnl
AC_CHECK_LIB([intl],[gettext],[],[
  # check arch when "main()" is the only function for which we check:
  PRE_INTL_LDFLAGS="${LDFLAGS}"
  LDFLAGS="${LDFLAGS} ${LD_ARCHFLAGS}"
  AC_CHECK_LIB([intl],[main],[],[])dnl
  # reset:
  LDFLAGS="${PRE_INTL_LDFLAGS}"
])dnl
AC_DEFUN([AC_GETTEXT_SUBCHECKS],[
  AC_REQUIRE([gt_INTL_MACOSX])dnl
  AC_REQUIRE([AM_ICONV])dnl
  dnl# this next bit used to be lower below:
  m4_ifdef([AM_LC_MESSAGES],[
    test -z "${am_cv_val_LC_MESSAGES}"
    AC_REQUIRE([AM_LC_MESSAGES])dnl
  ],[
    if test "x${am_cv_val_LC_MESSAGES}" = "x"; then
      test -z "${am_cv_val_LC_MESSAGES}"
      AC_MSG_WARN([might have skipped check for LC_MESSAGES])
    elif test "x${am_cv_val_LC_MESSAGES}" != "xyes"; then
      AC_MSG_WARN([we might be missing support for LC_MESSAGES])
    fi
  ])dnl
  AC_CHECK_LIB([iconv],[iconv],[],[
    AC_CHECK_LIB([iconv],[libiconv],[],[
      # check arch when "main()" is the only function for which we check:
      PRE_ICONV_LDFLAGS="${LDFLAGS}"
      LDFLAGS="${LDFLAGS} ${LD_ARCHFLAGS}"
      AC_CHECK_LIB([iconv],[main],[],[])dnl
      # reset:
      LDFLAGS="${PRE_ICONV_LDFLAGS}"
    ])dnl
  ])dnl
])dnl
AC_GETTEXT_SUBCHECKS
dnl# I forget, why am I not just using AM\_GNU\_GETTEXT?
dnl# Anyways... continuing with library checks:
AC_CHECK_LIB([c],[printf])
AC_CHECK_LIB([ssp],[__chk_fail],[],[
  AC_CHECK_LIB([ssp],[__memcpy_chk],[],[
    AC_CHECK_LIB([ssp],[__strcpy_chk],[],[])
  ])
])
# check arch when "main()" is the only function for which we check:
ARCHLESS_LDFLAGS="${LDFLAGS}"
LDFLAGS="${LDFLAGS} ${LD_ARCHFLAGS}"
# FIXME: Replace `main' with a function in `-lcc_dynamic':
AC_CHECK_LIB([cc_dynamic],[main])
# FIXME: Replace `main' with a function in `-lgcc':
AC_CHECK_LIB([gcc],[main])
# FIXME: Replace `main' with a function in `-lcurses':
AC_CHECK_LIB([curses],[main])
  ## reset these:
LDFLAGS="${ARCHLESS_LDFLAGS}"
if test -d ../readline -a -e ../readline/Makefile; then
  if test -z "${LDFLAGS}"; then
    export LDFLAGS="-L. -L../readline"
  else
    export LDFLAGS="${LDFLAGS} -L../readline"
  fi
fi
AC_CHECK_LIB([history],[add_history],[],[
  # check arch when "main()" is the only function for which we check:
  PRE_HISTORY_LDFLAGS="${LDFLAGS}"
  LDFLAGS="${LDFLAGS} ${LD_ARCHFLAGS}"
  AC_CHECK_LIB([history],[main])dnl
  # reset:
  LDFLAGS="${PRE_HISTORY_LDFLAGS}"
])dnl
AC_CHECK_LIB([readline],[rl_initialize])
AC_CHECK_LIB([iberty],[xmalloc],[],[
  # check arch when "main()" is the only function for which we check:
  PRE_LIBERTY_LDFLAGS="${LDFLAGS}"
  LDFLAGS="${LDFLAGS} ${LD_ARCHFLAGS}"
  AC_CHECK_LIB([iberty],[main])dnl
  # reset:
  LDFLAGS="${PRE_LIBERTY_LDFLAGS}"
])dnl
AC_CHECK_LIB([gnu],[gl_dummy_symbol],[],[
  # check arch when "main()" is the only function for which we check:
  PRE_GNULIB_LDFLAGS="${LDFLAGS}"
  LDFLAGS="${LDFLAGS} ${LD_ARCHFLAGS}"
  AC_CHECK_LIB([gnu],[main],[],[])dnl
  # reset:
  LDFLAGS="${PRE_GNULIB_LDFLAGS}"
])dnl

# Checks for header files.
AC_MSG_CHECKING([headers])
AC_MSG_RESULT([ ])
AC_HEADER_STDBOOL dnl# also "_CHECK"s it
if test -d ../intl; then
  if test -z "${CPPFLAGS}"; then
    export CPPFLAGS="-Wp,-I. -Wp,-I../intl"
  else
    export CPPFLAGS="${CPPFLAGS} -Wp,-I../intl"
  fi
  if test -z "${INCINTL}"; then
    export INCINTL="-Wp,-I../intl"
    AC_SUBST([INCINTL])dnl
    ## ok
  fi
elif test -d ../libintl; then
  if test -z "${CPPFLAGS}"; then
    export CPPFLAGS="-Wp,-I. -Wp,-I../libintl"
  else
    export CPPFLAGS="${CPPFLAGS} -Wp,-I../libintl"
  fi
  if test -z "${INCINTL}"; then
    export INCINTL="-Wp,-I../libintl"
    AC_SUBST([INCINTL])dnl
    ## similar to above
  fi
fi
unset ac_cv_header_libintl_h
unset ac_cv_header_limits_h
unset ac_cv_header_locale_h
unset ac_cv_header_malloc_h
unset ac_cv_header_stdlib_h
unset ac_cv_header_sys_param_h
unset ac_cv_header_sys_time_h
unset ac_cv_header_unistd_h
unset ac_cv_header_utime_h
AC_DEFUN([AC_REQUIRE_HEADER_STDC],[
  AC_REQUIRE([AC_HEADER_STDC])dnl
])dnl
AC_REQUIRE_HEADER_STDC dnl# should be called anyways, but whatever...
AC_CHECK_HEADERS([AvailabilityMacros.h assert.h binary-io.h errno.h \
                  fcntl.h gettext.h iconv.h langinfo.h libgen.h libgettext.h \
                  libintl.h limits.h locale.h mach/i386/thread_status.h \
                  mach/mach.h mach-o/loader.h malloc.h malloc/malloc.h \
                  math.h mingw/fcntl.h mingw/sys/types.h stdarg.h stddef.h \
                  stdio.h sys/file.h sys/ioctl.h sys/param.h sys/time.h \
                  utime.h varargs.h wchar.h wctype.h])dnl
AC_CHECK_HEADERS_ONCE([time.h])dnl
AC_HEADER_SYS_WAIT
AC_HEADER_STAT
AC_HEADER_TIME
m4_ifdef([ACX_HEADER_STRING],[
  ACX_HEADER_STRING
],[
  if test "x${ac_cv_header_string_h}" = "x" && test "x${ac_cv_header_strings_h}" = "x"; then
    test -z "${ac_cv_header_string_h}" && test -z "${ac_cv_header_strings_h}"
    AC_MSG_WARN([looks like we skipped some checks for string headers])
  elif test "x${ac_cv_header_string_h}" != "xyes" && test "x${ac_cv_header_strings_h}" != "xyes"; then
    AC_MSG_WARN([we might be missing some string headers])
  fi
])dnl

dnl# Keep autoscan happy:
if test "x${ac_cv_header_inttypes_h}" = "x"; then
  test -z ${ac_cv_header_inttypes_h} || unset ac_cv_header_inttypes_h
  AC_CHECK_HEADERS([inttypes.h])
fi
if test "x${ac_cv_header_stdint_h}" = "x"; then
  test -z ${ac_cv_header_stdint_h} || unset ac_cv_header_stdint_h
  AC_CHECK_HEADERS([stdint.h])
fi
if test "x${ac_cv_header_strings_h}" = "x"; then
  test -z ${ac_cv_header_strings_h} || unset ac_cv_header_strings_h
  AC_CHECK_HEADERS([strings.h])
fi
if test "x${ac_cv_header_unistd_h}" = "x"; then
  test -z ${ac_cv_header_unistd_h} || unset ac_cv_header_stdint_h
  AC_CHECK_HEADERS([unistd.h])
fi

dnl# if we are using internal include paths, see if renaming has occurred:
AC_CHECK_HEADERS([mach-o/codesign.h],[],[
  AC_CHECK_HEADERS([mach-o-gnu/codesign.h])dnl
])dnl

# Checks for typedefs, structures, and compiler characteristics.
AC_MSG_CHECKING([compiler characteristics])
AC_MSG_RESULT([ ])
AC_DEFUN([AC_REQUIRE_C_CONST],[
  AC_REQUIRE([AC_C_CONST])dnl
])dnl
AC_REQUIRE_C_CONST
AC_C_BIGENDIAN
AC_C_PROTOTYPES
AC_C_RESTRICT
AC_MSG_CHECKING([types])
AC_MSG_RESULT([ ])
AC_CHECK_TYPES([FILE])dnl
AC_TYPE_INT8_T
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_DEFUN([AC_REQUIRE_LONG_LONG_TYPES],[
  m4_ifdef([AC_TYPE_LONG_LONG_INT],[
    AC_REQUIRE([AC_TYPE_LONG_LONG_INT])dnl
  ],[
    if test "x${ac_cv_type_long_long_int}" = "x"; then
      test -z "${ac_cv_type_long_long_int}"
      AC_MSG_WARN([looks like we skipped checking long long])
    fi
  ])dnl
  m4_ifdef([AC_TYPE_UNSIGNED_LONG_LONG_INT],[
    AC_REQUIRE([AC_TYPE_UNSIGNED_LONG_LONG_INT])dnl
  ],[
    if test "x${ac_cv_type_unsigned_long_long_int}" = "x"; then
      test -z "${ac_cv_type_unsigned_long_long_int}"
      AC_MSG_WARN([looks like we skipped checking unsigned long long])
    fi
  ])dnl
])dnl
AC_REQUIRE_LONG_LONG_TYPES dnl# used to compare against later
if test "x${ac_cv_type_long_long}" = "xyes"; then
  AC_CHECK_SIZEOF([long long])dnl
  AC_CHECK_ALIGNOF([long long])dnl
  ## ok
fi
if test "x${ac_cv_type_unsigned_long_long}" = "x"; then
  test -z "${ac_cv_type_unsigned_long_long}" && ac_cv_type_unsigned_long_long="${ac_cv_type_unsigned_long_long_int}"
fi
if test "x${ac_cv_type_unsigned_long_long}" = "xyes"; then
  AC_CHECK_SIZEOF([unsigned long long])dnl
  AC_CHECK_ALIGNOF([unsigned long long])dnl
  ## ok
fi
AC_TYPE_MODE_T
if test "x${ac_cv_type_off_t}" = "x"; then
  test -z "${ac_cv_type_off_t}"
  AC_TYPE_OFF_T
elif test "x${ac_cv_type_off_t}" = "xyes"; then
  AC_DEFINE([HAVE_OFF_T],[1],
            [Define to 1 if you have the off_t type])
fi
## keep separate from previous, as this value might have been updated:
if test "x${ac_cv_type_off_t}" = "xyes"; then
  AC_CHECK_SIZEOF([off_t])dnl
  AC_CHECK_ALIGNOF([off_t])dnl
  ## ok
fi
if test "x${ac_cv_sizeof_off_t}" != "x${ac_cv_sizeof_unsigned_long_long}"; then
  AC_CHECK_TYPES([off64_t],[
    test -z "${ac_cv_sizeof_off64_t}"
    test -z "${ac_cv_alignof_off64_t}"
    AC_CHECK_SIZEOF([off64_t])dnl
    AC_CHECK_ALIGNOF([off64_t])dnl
  ],[
    unset ac_cv_type_off64_t
    if test "x${ac_cv_type_off_t}" = "xyes"; then
      _AC_CHECK_TYPE_OLD([off64_t],[off_t])dnl
      ## hope that is okay...
    elif test "x${ac_cv_type_unsigned_long_long}" = "xyes"; then
      _AC_CHECK_TYPE_OLD([off64_t],[unsigned long long])dnl
    else
      _AC_CHECK_TYPE_OLD([off64_t],[unsigned long])dnl
    fi
  ])dnl
  ## maybe use it as a replacement, idk...
elif test "x${ac_cv_sizeof_off_t}" = "x${ac_cv_sizeof_unsigned_long_long}";  then
  AC_MSG_NOTICE([it seems that your off_t is already 64-bit; in that case, there is no need to check for an off64_t type...])
else
  test -z "${ac_cv_sizeof_off_t}"
  test -z "${ac_cv_sizeof_unsigned_long_long}"
  AC_MSG_WARN([unsure if off_t is 64-bit or not...])
fi
AC_CHECK_TYPES([ptrdiff_t])dnl
AC_TYPE_SIGNAL dnl# (the copy in ./m4/signal.4 overrides the obsoleteness)
if test "x${ac_cv_type_size_t}" = "x"; then
  test -z "${ac_cv_type_size_t}"
  AC_TYPE_SIZE_T
elif test "x${ac_cv_type_size_t}" = "xyes"; then
  AC_DEFINE([HAVE_SIZE_T],[1],
            [Define to 1 if you have the size_t type])
fi
## keep separate from previous, as this value might have been updated:
if test "x${ac_cv_type_size_t}" = "xyes"; then
  AC_CHECK_SIZEOF([size_t])dnl
  AC_CHECK_ALIGNOF([size_t])dnl
  ## ok
fi
AC_TYPE_SSIZE_T
AC_CHECK_MEMBERS([struct stat.st_blksize])dnl
AC_STRUCT_ST_BLOCKS
AC_CHECK_MEMBERS([struct stat.st_rdev])dnl
AC_STRUCT_TIMEZONE dnl# also does the "_TM" one
AC_TYPE_UINT8_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_CHECK_TYPES([iconv_t])dnl
dnl# one hand-rolled one:
AC_CACHE_CHECK([if mbstate_t compiles],[ac_cv_have_mbstate_t],[
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <wchar.h>
]],[[mbstate_t teststate;]])],
[ac_cv_have_mbstate_t=yes],[ac_cv_have_mbstate_t=no])
])dnl
  ## dunno if this should go inside or outside the cache check...
if test x"${ac_cv_have_mbstate_t}" = x"yes"; then
  AC_DEFINE([HAVE_MBSTATE_T],[1],[Define if mbstate_t exists in wchar.h.])
fi

# Checks for library functions.
AC_MSG_CHECKING([functions])
AC_MSG_RESULT([ ])
AM_WITH_DMALLOC
AC_DEFUN([AC_REQUIRE_ALLOCA],[
  AC_REQUIRE([AC_FUNC_ALLOCA])dnl
])dnl
AC_REQUIRE_ALLOCA
AS_IF([test "x${host_os}" = "xmingw32" || test "x${target_os}" = "xmingw32"],[
  AS_IF([false && test -z "`grep \"define uid_t int\" config.h`"],[
    AC_CHECK_TYPE([uid_t],[int])
    AC_CHECK_TYPE([gid_t],[int])
  ],[
    AC_CHECK_TYPES([uid_t])
    AC_CHECK_TYPES([gid_t])
  ])dnl
  ## if that failed, try looking in other headers:
  AS_IF([test "x${ac_cv_type_uid_t}" != "xyes"],[
    unset ac_cv_type_uid_t
    AC_CACHE_CHECK([for uid_t in unistd.h],[ac_cv_type_uid_t],[
      AC_EGREP_HEADER([uid_t],[unistd.h],[ac_cv_type_uid_t=yes],
                      [ac_cv_type_uid_t=maybe])dnl
    ])dnl
  ])dnl
  ## one more try:
  AS_IF([test "x${ac_cv_type_uid_t}" != "xyes" \
         && test "x${ac_cv_header_mingw_sys_types_h}" = "xyes"],[
    unset ac_cv_type_uid_t
    AC_CACHE_CHECK([for uid_t in mingw/sys/types.h],[ac_cv_type_uid_t],[
      AC_EGREP_HEADER([uid_t],[mingw/sys/types.h],[ac_cv_type_uid_t=yes],
                      [ac_cv_type_uid_t=maybe])dnl
    ])dnl
  ])dnl
  ## if none of that works then I have no clue where else to look...
],[
  echo "assuming default test for uid_t will work with host ${host_os} and target ${target_os}..."
])dnl
  ## AC\_FUNC\_CHOWN calls checks for uid_t/gid_t, which can mess up config.h:
AS_IF([test "x${host_os}" != "xmingw32" \
       && test "x${target_os}" != "xmingw32"],[
  AC_FUNC_CHOWN
],[
  AC_CHECK_FUNCS([chown])dnl
])dnl
  ## (FIXME: might need to do the above differently...)
AC_FUNC_CLOSEDIR_VOID
AC_FUNC_ERROR_AT_LINE
AC_FUNC_FNMATCH dnl# avoid the "REPLACE" version of it though
AC_FUNC_FORK
AC_FUNC_LSTAT dnl# calls the _FOLLOWS_SLASHED_SYMLINK version of it
AC_FUNC_MALLOC
AC_FUNC_MEMCMP dnl# old
AC_FUNC_MKTIME
AC_FUNC_OBSTACK
AC_FUNC_REALLOC
m4_ifdef([AC_FUNC_SETVBUF_REVERSED],[
  AC_FUNC_SETVBUF_REVERSED dnl# (the one in ./m4 overrides the obsoletion)
],[
  AC_CHECK_FUNCS([setvbuf])dnl
])dnl
AC_FUNC_STAT
m4_ifdef([AC_FUNC_STRCOLL],[
  AC_FUNC_STRCOLL dnl# should be there, but just in case...
],[
  AC_CHECK_FUNCS([strcoll])dnl
])dnl
AC_FUNC_STRFTIME
AC_FUNC_STRNLEN
AC_FUNC_STRTOD
AC_FUNC_UTIME_NULL
AC_FUNC_VPRINTF
unset ac_cv_func_setlocale
AC_CHECK_FUNCS([atexit bindtextdomain ctime dcgettext dirname dup2 expandargv \
                fclose fcntl fctrl floor fopen fprintf fputs fread free \
                getc_unlocked gettimeofday isascii ioctlsocket localeconv \
                mblen memchr memcmp memcpy memmove memset modf pow re_comp \
                regcomp rint rmdir sbrk setlocale setmode sqrt strcspn \
                strdup strerror strncasecmp strpbrk strrchr strspn strstr \
                strtol strtoul strverscmp textdomain utime utimes])dnl
AC_SEARCH_LIBS([fcntl],[fcntl posix unix compat gcc gcc_s crt iberty c gnu])dnl
AC_SEARCH_LIBS([rpl_realloc],[iberty c gnu mingwex msvcrt ucrt ucrtbase])dnl
AC_SEARCH_LIBS([rpl_malloc],[iberty c gnu mingwex msvcrt ucrt ucrtbase])dnl
AC_SEARCH_LIBS([__chk_fail],[ssp gcc iberty c gnu])dnl
AC_SEARCH_LIBS([__memcpy_chk],[ssp gcc iberty c gnu])dnl
AC_SEARCH_LIBS([__strcpy_chk],[ssp gcc iberty c gnu])dnl

dnl# possible alternatives to mktemp():
AC_CHECK_FUNC([mkstemp],
	      [AC_DEFINE([HAVE_MKSTEMP],[1],
	                 [Define to 1 if you have the `mkstemp' function.])
])dnl
AC_CHECK_FUNC([mkdtemp],
              [AC_DEFINE([HAVE_MKDTEMP],[1],
                         [Define to 1 if you have the `mkdtemp' function.])
])dnl

# Check whether fopen64 is available and whether _LARGEFILE64_SOURCE
# needs to be defined for it:
AC_MSG_CHECKING([for fopen64])
AC_CACHE_VAL([bu_cv_have_fopen64],[
AC_LINK_IFELSE([AC_LANG_PROGRAM([[
#include <stdio.h>
]],[[
FILE *f = fopen64("/tmp/foo","r");
]])],[bu_cv_have_fopen64=yes],
     [saved_CPPFLAGS="${CPPFLAGS}"
      CPPFLAGS="${CPPFLAGS} -D_LARGEFILE64_SOURCE"
      AC_LINK_IFELSE([AC_LANG_PROGRAM([[
#include <stdio.h>
      ]],[[
FILE *f = fopen64("/tmp/foo","r");
      ]])],[bu_cv_have_fopen64="need -D_LARGEFILE64_SOURCE"],
           [bu_cv_have_fopen64=no])dnl
      ## restore:
      CPPFLAGS="${saved_CPPFLAGS}"])dnl
])dnl
  ## print:
AC_MSG_RESULT([${bu_cv_have_fopen64}])
if test "${bu_cv_have_fopen64}" != no; then
  AC_DEFINE([HAVE_FOPEN64],[1],
	    [Define to 1 if fopen64 is available])
fi
AC_MSG_CHECKING([for stat64])
AC_CACHE_VAL([bu_cv_have_stat64],[
AC_LINK_IFELSE([AC_LANG_PROGRAM([[
#include <sys/stat.h>
#ifdef HAVE_AVAILABILITYMACROS_H
# include <AvailabilityMacros.h>
#endif /* HAVE_AVAILABILITYMACROS_H */
]],[[
struct stat64 st;
stat64("/tmp/foo", &st);
]])],[bu_cv_have_stat64=yes],
     [saved_CPPFLAGS="${CPPFLAGS}"
      CPPFLAGS="${CPPFLAGS} -D_LARGEFILE64_SOURCE"
      AC_LINK_IFELSE([AC_LANG_PROGRAM([[
#include <sys/stat.h>
      ]],[[
struct stat64 st;
stat64("/tmp/foo", &st);
      ]])],[bu_cv_have_stat64="need -D_LARGEFILE64_SOURCE"],
           [bu_cv_have_stat64=no])dnl
      ## restore:
      CPPFLAGS="${saved_CPPFLAGS}"])dnl
])dnl
  ## print:
AC_MSG_RESULT([${bu_cv_have_stat64}])
if test "x${bu_cv_have_stat64}" != "xno"; then
  AC_DEFINE([HAVE_STAT64],[1],
	    [Define to 1 if stat64 is available])
fi
if test "${bu_cv_have_fopen64}" = "need -D_LARGEFILE64_SOURCE" \
   || test "${bu_cv_have_stat64}" = "need -D_LARGEFILE64_SOURCE"; then
  AC_DEFINE([_LARGEFILE64_SOURCE],[1],
	    [Define to 1 to enable LFS])
  CPPFLAGS="${CPPFLAGS} -D_LARGEFILE64_SOURCE"
fi

# Some systems have frexp only in -lm, not in -lc.
AC_SEARCH_LIBS([frexp],[m c])dnl
dnl# libm should now already be checked above, but leave this here
dnl# temporarily just in case:
AC_DEFUN([AC_REQUIRE_LT_LIB_M],[
  AC_REQUIRE([LT_LIB_M])dnl
])dnl
AC_REQUIRE_LT_LIB_M

dnl# (check for LC_MESSAGES has been moved so that it is now done
dnl# with the other gettext checks)

# other misc. (time-related) checks:
AC_MSG_CHECKING([for time_t in time.h])
AC_CACHE_VAL([bu_cv_decl_time_t_time_h],[
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <time.h>
]],[[
time_t i;
]])],[bu_cv_decl_time_t_time_h=yes],[bu_cv_decl_time_t_time_h=no])])dnl
  ## print:
AC_MSG_RESULT([${bu_cv_decl_time_t_time_h}])
if test "x${bu_cv_decl_time_t_time_h}" = "xyes"; then
  AC_DEFINE([HAVE_TIME_T_IN_TIME_H],[1],
	    [Is the type time_t defined in <time.h>?])
fi

AC_MSG_CHECKING([for time_t in sys/types.h])
AC_CACHE_VAL([bu_cv_decl_time_t_types_h],[
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <sys/types.h>
]],[[
time_t i;
]])],[bu_cv_decl_time_t_types_h=yes],[bu_cv_decl_time_t_types_h=no])])dnl
  ## print:
AC_MSG_RESULT([${bu_cv_decl_time_t_types_h}])
if test "x${bu_cv_decl_time_t_types_h}" = "xyes"; then
  AC_DEFINE([HAVE_TIME_T_IN_TYPES_H],[1],
	    [Is the type time_t defined in <sys/types.h>?])
fi

AC_MSG_CHECKING([for a known getopt prototype in unistd.h])
AC_CACHE_VAL([bu_cv_decl_getopt_unistd_h],[
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <unistd.h>
]],[[
extern int getopt(int, char *const*, const char *);
]])],[bu_cv_decl_getopt_unistd_h=yes],[bu_cv_decl_getopt_unistd_h=no])])dnl
  ## print:
AC_MSG_RESULT([${bu_cv_decl_getopt_unistd_h}])
if test "x${bu_cv_decl_getopt_unistd_h}" = "xyes"; then
  AC_DEFINE([HAVE_DECL_GETOPT],[1],
	    [Define to 1 if prototype for getopt in <unistd.h>
             is in expected format])
fi

# Under Next 3.2 <utime.h> apparently does not define struct utimbuf
# by default.
AC_MSG_CHECKING([for a good utime.h])
AC_CACHE_VAL([bu_cv_header_utime_h],[
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <sys/types.h>
#ifdef HAVE_TIME_H
# include <time.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "this conftest expects <time.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_TIME_H */
#include <utime.h>
]],[[
struct utimbuf s;
]])],[bu_cv_header_utime_h=yes],[bu_cv_header_utime_h=no])])dnl
  ## print:
AC_MSG_RESULT([${bu_cv_header_utime_h}])
if test "x${bu_cv_header_utime_h}" = "xyes"; then
  AC_DEFINE([HAVE_GOOD_UTIME_H],[1],
            [Does <utime.h> define struct utimbuf?])
fi

# checks for declarations (keep alphabetized please):
AC_MSG_CHECKING([declarations])
AC_MSG_RESULT([ ])
unset ac_cv_have_decl_getenv
AC_CHECK_DECLS([basename, dirname, environ, fprintf, getc_unlocked, getenv,
		sbrk, snprintf, stpcpy, strnlen, strstr, vsnprintf])dnl

# Link in zlib if we can. This allows us to read compressed debug
# sections. This is used only by readelf.c (objdump uses bfd for
# reading compressed sections).
m4_ifdef([AM_ZLIB],[
  AM_ZLIB
],[
  AC_CHECK_LIB([z],[zlibVersion],[
    AC_CHECK_HEADERS([zlib.h])dnl
  ],[
    AC_CHECK_LIB([z],[main],[
      AC_CHECK_HEADERS([zlib.h])dnl
    ])dnl
  ])dnl
])dnl

BFD_BINARY_FOPEN

dnl# (iconv checks are done above)

# target-specific stuff:

# Canonicalize the secondary target names.
if test -n "${enable_targets}"; then
    for targ in `echo ${enable_targets} | sed 's/,/ /g'`
    do
	result=`${ac_config_sub} ${targ} 2>/dev/null`
	if test -n "${result}"; then
	    canon_targets="${canon_targets} ${result}"
	else
	    # Allow targets that config.sub does NOT recognize, like "all".
	    canon_targets="${canon_targets} ${targ}"
	fi
    done
fi

AC_SUBST([ac_config_sub])dnl

all_targets=false
BUILD_NLMCONV=""
NLMCONV_DEFS=""
BUILD_SRCONV=""
BUILD_DLLTOOL=""
DLLTOOL_DEFS=""
DLLTOOL_DEFAULT=""
BUILD_WINDRES=""
BUILD_WINDMC=""
BUILD_DLLWRAP=""
BUILD_MISC=""
BUILD_INSTALL_MISC=""
OBJDUMP_DEFS=""
OBJDUMP_PRIVATE_VECTORS=""
OBJDUMP_PRIVATE_OFILES=""
od_vectors=""

AC_MSG_CHECKING([which targets to look up])
AC_MSG_RESULT([plain ones are '${target}' and canonical ones are '${canon_targets}'])
for targ in ${target} ${canon_targets}
do
    if test "x${targ}" = "xall"; then
        all_targets=true
	BUILD_NLMCONV='$(NLMCONV_PROG)$(EXEEXT)'
	BUILD_SRCONV='$(SRCONV_PROG)'
	NLMCONV_DEFS="-DNLMCONV_I386 -DNLMCONV_ALPHA -DNLMCONV_POWERPC -DNLMCONV_SPARC"
	BUILD_MISC="${BUILD_MISC} "'bin2c$(EXEEXT_FOR_BUILD)'
	BUILD_WINDRES='$(WINDRES_PROG)$(EXEEXT)'
	BUILD_WINDMC='$(WINDMC_PROG)$(EXEEXT)'
	BUILD_DLLTOOL='$(DLLTOOL_PROG)$(EXEEXT)'
	if test -z "${DLLTOOL_DEFAULT}"; then
	  DLLTOOL_DEFAULT="-DDLLTOOL_DEFAULT_I386"
	fi
	DLLTOOL_DEFS="${DLLTOOL_DEFS} -DDLLTOOL_I386"
	BUILD_DLLWRAP='$(DLLWRAP_PROG)$(EXEEXT)'
	od_vectors="${od_vectors} objdump_private_desc_xcoff"
    else
	case ${targ} in
changequote(,)dnl
	i[3-7]86*-*-netware*) 
changequote([,])dnl
	  BUILD_NLMCONV='$(NLMCONV_PROG)$(EXEEXT)'
	  NLMCONV_DEFS="${NLMCONV_DEFS} -DNLMCONV_I386"
	  ;;
	alpha*-*-netware*)
	  BUILD_NLMCONV='$(NLMCONV_PROG)$(EXEEXT)'
	  NLMCONV_DEFS="${NLMCONV_DEFS} -DNLMCONV_ALPHA"
	  ;;
	powerpc*-*-netware*)
	  BUILD_NLMCONV='$(NLMCONV_PROG)$(EXEEXT)'
	  NLMCONV_DEFS="${NLMCONV_DEFS} -DNLMCONV_POWERPC"
	  ;;
	sparc*-*-netware*)
	  BUILD_NLMCONV='$(NLMCONV_PROG)$(EXEEXT)'
	  NLMCONV_DEFS="${NLMCONV_DEFS} -DNLMCONV_SPARC"
	  ;;
	esac

	case ${targ} in
	*-*-hms*) BUILD_SRCONV='$(SRCONV_PROG)' ;;
	esac

	case ${targ} in
	arm-epoc-pe*)
  	  BUILD_DLLTOOL='$(DLLTOOL_PROG)$(EXEEXT)'
	  if test -z "${DLLTOOL_DEFAULT}"; then
	    DLLTOOL_DEFAULT="-DDLLTOOL_DEFAULT_ARM_EPOC"
	  fi
	  DLLTOOL_DEFS="${DLLTOOL_DEFS} -DDLLTOOL_ARM_EPOC -DDLLTOOL_ARM"
	  BUILD_WINDRES='$(WINDRES_PROG)$(EXEEXT)'
	  BUILD_WINDMC='$(WINDMC_PROG)$(EXEEXT)'
	  ;;
	arm-wince-pe* | arm-*-wince | arm*-*-cegcc* | arm*-*-mingw32ce*)
  	  BUILD_DLLTOOL='$(DLLTOOL_PROG)$(EXEEXT)'
	  if test -z "${DLLTOOL_DEFAULT}"; then
	    DLLTOOL_DEFAULT="-DDLLTOOL_DEFAULT_ARM_WINCE"
	  fi
	  DLLTOOL_DEFS="${DLLTOOL_DEFS} -DDLLTOOL_ARM_WINCE -DDLLTOOL_ARM"
	  BUILD_WINDRES='$(WINDRES_PROG)$(EXEEXT)'
	  BUILD_WINDMC='$(WINDMC_PROG)$(EXEEXT)'
	  ;;
	arm-*-pe*)
  	  BUILD_DLLTOOL='$(DLLTOOL_PROG)$(EXEEXT)'
	  if test -z "${DLLTOOL_DEFAULT}"; then
	    DLLTOOL_DEFAULT="-DDLLTOOL_DEFAULT_ARM"
	  fi
	  DLLTOOL_DEFS="${DLLTOOL_DEFS} -DDLLTOOL_ARM"
	  BUILD_WINDRES='$(WINDRES_PROG)$(EXEEXT)'
	  BUILD_WINDMC='$(WINDMC_PROG)$(EXEEXT)'
	  ;;
	thumb-*-pe*)
  	  BUILD_DLLTOOL='$(DLLTOOL_PROG)$(EXEEXT)'
	  if test -z "${DLLTOOL_DEFAULT}"; then
	    DLLTOOL_DEFAULT="-DDLLTOOL_DEFAULT_ARM"
	  fi
	  DLLTOOL_DEFS="${DLLTOOL_DEFS} -DDLLTOOL_ARM -DDLLTOOL_THUMB"
	  BUILD_WINDRES='$(WINDRES_PROG)$(EXEEXT)'
	  BUILD_WINDMC='$(WINDMC_PROG)$(EXEEXT)'
	  ;;
  	arm*-* | xscale-* | strongarm-* | d10v-*)
	  OBJDUMP_DEFS="-DDISASSEMBLER_NEEDS_RELOCS"
	  ;;
	x86_64-*-mingw* | x86_64-*-cygwin*)
  	  BUILD_DLLTOOL='$(DLLTOOL_PROG)$(EXEEXT)'
	  if test -z "${DLLTOOL_DEFAULT}"; then
	    DLLTOOL_DEFAULT="-DDLLTOOL_DEFAULT_MX86_64"
	  fi
	  DLLTOOL_DEFS="${DLLTOOL_DEFS} -DDLLTOOL_MX86_64"
	  BUILD_WINDRES='$(WINDRES_PROG)$(EXEEXT)'
	  BUILD_WINDMC='$(WINDMC_PROG)$(EXEEXT)'
	  BUILD_DLLWRAP='$(DLLWRAP_PROG)$(EXEEXT)'
	  ;;
changequote(,)dnl
	i[3-7]86-*-pe* | i[3-7]86-*-cygwin* | i[3-7]86-*-mingw32** | i[3-7]86-*-netbsdpe*)
changequote([,])dnl
  	  BUILD_DLLTOOL='$(DLLTOOL_PROG)$(EXEEXT)'
	  if test -z "${DLLTOOL_DEFAULT}"; then
	    DLLTOOL_DEFAULT="-DDLLTOOL_DEFAULT_I386"
	  fi
	  DLLTOOL_DEFS="${DLLTOOL_DEFS} -DDLLTOOL_I386"
	  BUILD_WINDRES='$(WINDRES_PROG)$(EXEEXT)'
	  BUILD_WINDMC='$(WINDMC_PROG)$(EXEEXT)'
	  BUILD_DLLWRAP='$(DLLWRAP_PROG)$(EXEEXT)'
	  ;;
changequote(,)dnl
	i[3-7]86-*-interix)
changequote([,])dnl
	  BUILD_DLLTOOL='$(DLLTOOL_PROG)'
	  if test -z "${DLLTOOL_DEFAULT}"; then
	    DLLTOOL_DEFAULT="-DDLLTOOL_DEFAULT_I386"
	  fi
	  DLLTOOL_DEFS="${DLLTOOL_DEFS} -DDLLTOOL_I386"
	  ;;
changequote(,)dnl
	powerpc*-aix5.[01])
changequote([,])dnl
	  ;;
changequote(,)dnl
	powerpc*-aix[5-9].* \
changequote([,])dnl
	| powerpc-*-macos)
	  OBJDUMP_DEFS="-DAIX_WEAK_SUPPORT"
	  ;;
	powerpc*-*-pe* | powerpc*-*-cygwin*)
  	  BUILD_DLLTOOL='$(DLLTOOL_PROG)$(EXEEXT)'
	  if test -z "${DLLTOOL_DEFAULT}"; then
	    DLLTOOL_DEFAULT="-DDLLTOOL_DEFAULT_PPC"
	  fi
	  DLLTOOL_DEFS="${DLLTOOL_DEFS} -DDLLTOOL_PPC"
	  BUILD_WINDRES='$(WINDRES_PROG)$(EXEEXT)'
	  BUILD_WINDMC='$(WINDMC_PROG)$(EXEEXT)'
	  ;;
	powerpc*-*-linux* | powerpc*-*-elf* | powerpc*-*-eabi*)
	  case "${BUILD_INSTALL_MISC}" in
	    *embedspu*) ;;
	    *) BUILD_INSTALL_MISC="${BUILD_INSTALL_MISC} embedspu"
	  esac
	  ;;
	sh*-*-pe)
  	  BUILD_DLLTOOL='$(DLLTOOL_PROG)$(EXEEXT)'
	  if test -z "${DLLTOOL_DEFAULT}"; then
	    DLLTOOL_DEFAULT="-DDLLTOOL_DEFAULT_SH"
	  fi
	  DLLTOOL_DEFS="${DLLTOOL_DEFS} -DDLLTOOL_SH"
	  BUILD_WINDRES='$(WINDRES_PROG)$(EXEEXT)'
	  BUILD_WINDMC='$(WINDMC_PROG)$(EXEEXT)'
	  ;;
	spu-*-*)
	  BUILD_MISC="${BUILD_MISC} "'bin2c$(EXEEXT_FOR_BUILD)'
	  ;;
	mips*-*-pe)
  	  BUILD_DLLTOOL='$(DLLTOOL_PROG)$(EXEEXT)'
	  if test -z "${DLLTOOL_DEFAULT}"; then
	    DLLTOOL_DEFAULT="-DDLLTOOL_DEFAULT_MIPS"
	  fi
	  DLLTOOL_DEFS="${DLLTOOL_DEFS} -DDLLTOOL_MIPS"
	  BUILD_WINDRES='$(WINDRES_PROG)$(EXEEXT)'
	  BUILD_WINDMC='$(WINDMC_PROG)$(EXEEXT)'
	  ;;
	mcore-*-pe)
  	  BUILD_DLLTOOL='$(DLLTOOL_PROG)$(EXEEXT)'
	  if test -z "${DLLTOOL_DEFAULT}"; then
	    DLLTOOL_DEFAULT="-DDLLTOOL_DEFAULT_MCORE"
	  fi
	  DLLTOOL_DEFS="${DLLTOOL_DEFS} -DDLLTOOL_MCORE"
	  BUILD_WINDRES='$(WINDRES_PROG)$(EXEEXT)'
	  BUILD_WINDMC='$(WINDMC_PROG)$(EXEEXT)'
	  ;;
	mcore-*-elf)
  	  BUILD_DLLTOOL='$(DLLTOOL_PROG)$(EXEEXT)'
	  if test -z "${DLLTOOL_DEFAULT}"; then
	    DLLTOOL_DEFAULT="-DDLLTOOL_DEFAULT_MCORE_ELF"
	  fi
	  DLLTOOL_DEFS="${DLLTOOL_DEFS} -DDLLTOOL_MCORE_ELF"
	  ;;
  	mep-*)
	  OBJDUMP_DEFS="-DSKIP_ZEROES=256 -DSKIP_ZEROES_AT_END=0"
	  ;;
	esac

	# Add objdump private vectors.
	case ${targ} in
        avr-*-*)
          od_vectors="${od_vectors} objdump_private_desc_elf32_avr"
          ;;
	powerpc-*-aix*)
	  od_vectors="${od_vectors} objdump_private_desc_xcoff"
	  ;;
        *-*-darwin*)
	  od_vectors="${od_vectors} objdump_private_desc_mach_o"
	  ;;
	esac
    fi
done

dnl# saw the word "uniq" coming up, so that reminded me:
AC_PATH_PROGS([UNIQ_BIN],[uniq guniq],[])
# Uniq objdump private vector, build objdump target ofiles.
od_files=""
f=""
for i in ${od_vectors} ; do
    case " ${f} " in
    *" ${i} "*) ;;
    *)
	f="${f} ${i}"
	OBJDUMP_PRIVATE_VECTORS="${OBJDUMP_PRIVATE_VECTORS} &${i},"
	case ${i} in
        objdump_private_desc_elf32_avr)
            od_files="$od_files od-elf32_avr" ;;
	objdump_private_desc_xcoff)
	    od_files="${od_files} od-xcoff" ;;
	objdump_private_desc_mach_o)
	    od_files="${od_files} od-macho" ;;
	*) AC_MSG_ERROR([*** unknown private vector ${i}]) ;;
	esac
	;;
    esac
done

# Uniq objdump target ofiles:
f=""
for i in ${od_files} ; do
    case " ${f} " in
    *" ${i} "*) ;;
    *)
	f="${f} ${i}"
	OBJDUMP_PRIVATE_OFILES="${OBJDUMP_PRIVATE_OFILES} ${i}.${objext}"
	;;
    esac
done

DLLTOOL_DEFS="${DLLTOOL_DEFS} ${DLLTOOL_DEFAULT}"

if test "${with_windres+set}" = set; then
  BUILD_WINDRES='$(WINDRES_PROG)$(EXEEXT)'
fi

if test "${with_windmc+set}" = set; then
  BUILD_WINDMC='$(WINDMC_PROG)$(EXEEXT)'
fi

OBJDUMP_DEFS="${OBJDUMP_DEFS} -DOBJDUMP_PRIVATE_VECTORS=\"${OBJDUMP_PRIVATE_VECTORS}\""

AC_SUBST([NLMCONV_DEFS])dnl
AC_SUBST([BUILD_NLMCONV])dnl
AC_SUBST([BUILD_SRCONV])dnl
AC_SUBST([BUILD_DLLTOOL])dnl
AC_SUBST([DLLTOOL_DEFS])dnl
AC_SUBST([BUILD_WINDRES])dnl
AC_SUBST([BUILD_WINDMC])dnl
AC_SUBST([BUILD_DLLWRAP])dnl
AC_SUBST([BUILD_MISC])dnl
AC_SUBST([BUILD_INSTALL_MISC])dnl
AC_SUBST([OBJDUMP_DEFS])dnl
AC_SUBST([OBJDUMP_PRIVATE_OFILES])dnl

AC_DEFINE_UNQUOTED([TARGET],["${target}"],[Configured target name.])dnl

targ=${target}
AC_MSG_NOTICE([sourcing ../bfd/config.bfd])
. ${srcdir}/../bfd/config.bfd
if test "x${targ_underscore}" = "xyes"; then
    UNDERSCORE=1
else
    UNDERSCORE=0
fi
AC_DEFINE_UNQUOTED([TARGET_PREPENDS_UNDERSCORE],[${UNDERSCORE}],
[Define to 1 if user symbol names have a leading underscore, 0 if not.])dnl

# Emulation:
for targ_alias in `echo ${target} ${enable_targets} | sed 's/,/ /g'`
do
  # Canonicalize the secondary target names.
  result=`${ac_config_sub} ${targ_alias} 2>/dev/null`
  if test -n "${result}"; then
    targ="${result}"
  else
    targ="${targ_alias}"
  fi

  AC_MSG_NOTICE([sourcing ./configure.tgt for target '${targ}'])
  . ${srcdir}/configure.tgt

  export EMULATION="${targ_emul}"
  export EMULATION_VECTOR="${targ_emul_vector}"
  AC_MSG_NOTICE([EMULATION is '${EMULATION}' for target '${targ}'])
  AC_MSG_NOTICE([EMULATION_VECTOR is '${EMULATION_VECTOR}' for target '${targ}'])
done

dnl# finally, some output:
AC_SUBST([EMULATION])dnl
AC_SUBST([EMULATION_VECTOR])dnl
AC_SUBST([INTLLIBS])dnl
AC_SUBST([INTLDEPS])dnl
AC_SUBST([SYSTEM_FRAMEWORK_LIBS])dnl

dnl# Required for html and install-html:
AC_SUBST([datarootdir])dnl
AC_SUBST([docdir])dnl
AC_SUBST([htmldir])dnl
AC_SUBST([pdfdir])dnl

dnl# the files to actually output:
AC_CONFIG_FILES([Makefile \
                 doc/Makefile \
                 po/Makefile.in:po/Make-in])dnl

AC_CONFIG_COMMANDS([default],[
if test "x${SED}" = "x"; then
  test -z "${SED}" && export SED=sed
fi
case "x${CONFIG_FILES}" in
*) ${SED} -e '/POTFILES =/r po/POTFILES' po/Makefile.in > po/Makefile ;;
esac
],[])dnl

dnl# While there may be subdirs with configure scripts present, they don't actually
dnl# contain the sources necessary to run, so don't listen to autoscan's requests
dnl# to put them in AC\_CONFIG\_SUBDIRS. (I'm thinking the scripts may have only
dnl# gotten there by mistake in the first place)
AC_CONFIG_COMMANDS([subdirs],[
if test "x${CLICOLOR}" = "x1"; then unset CLICOLOR; fi
ls -m bfd binutils etc gas gprof ld opcodes || echo "never mind about subdirs"
],[])dnl

AC_OUTPUT

if test "${srcdir}" != "."; then
  if test -e ${srcdir}/config.status -a ! -e `pwd`/config.status; then
    cp -v ${srcdir}/config.status `pwd`/config.status || echo "1: pwd = `pwd`"
  elif test -e `pwd`/config.status -a ! -e ${srcdir}/config.status; then
    cp -v `pwd`/config.status ${srcdir}/config.status || echo "2: pwd = `pwd`"
  fi
fi
