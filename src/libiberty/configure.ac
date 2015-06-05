dnl#                                               -*- Autoconf -*-
dnl# Process this file with autoconf to produce a configure script

AC_PREREQ([2.64])
AC_INIT([libiberty],[1.0],[jmolenda@apple.com])
dnl# bug report address is email address of last person to touch this
dnl# directory, according to the Changelog-Apple in this directory
AC_CONFIG_SRCDIR([xmalloc.c])dnl

# This works around the fact that libtool configuration may change LD
# for this particular configuration, but some shells, instead of
# keeping the changes in LD private, export them just because LD is
# exported. We do NOT use libtool yet, but some day we might, so...
ORIGINAL_LD_FOR_MULTILIBS=${LD}

dnl# We use these options to decide which functions to include.
AC_ARG_WITH([target-subdir],
[AS_HELP_STRING([--with-target-subdir=SUBDIR],
                [Flag for configuring in a subdirectory for target])])
AC_ARG_WITH([build-subdir],
[AS_HELP_STRING([--with-build-subdir=SUBDIR],
                [Flag for configuring in a subdirectory for build])])
AC_ARG_WITH([cross-host],
[AS_HELP_STRING([--with-cross-host=HOST],
                [Flag for configuring with a cross compiler])])
AC_ARG_WITH([newlib],
[AS_HELP_STRING([--with-newlib],[Flag for configuring with newlib])])dnl

if test "x${srcdir}" = "x."; then
  if test -n "${with_build_subdir}"; then
    libiberty_topdir="${srcdir}/../.."
    with_target_subdir=""
  elif test -z "${with_target_subdir}"; then
    libiberty_topdir="${srcdir}/.."
  else
    if test "x${with_target_subdir}" != "x."; then
      libiberty_topdir="${srcdir}/${with_multisrctop}../.."
    else
      libiberty_topdir="${srcdir}/${with_multisrctop}.."
    fi
  fi
else
  libiberty_topdir="${srcdir}/.."
fi
AC_SUBST([libiberty_topdir])dnl
AC_CONFIG_AUX_DIR([config])dnl
AC_CONFIG_MACRO_DIR([m4])dnl
AC_CONFIG_LIBOBJ_DIR([.])dnl

dnl# Very limited version of automake's enable-maintainer-mode:
AC_MSG_CHECKING([whether to enable maintainer-specific portions of Makefiles])
  dnl# maintainer-mode is disabled by default
  AC_ARG_ENABLE([maintainer-mode],
[AS_HELP_STRING([--enable-maintainer-mode],
                [enable make rules and dependencies not useful (and sometimes confusing) to the casual installer])],
      [maintainer_mode=${enableval}],
      [maintainer_mode=no])dnl

AC_MSG_RESULT([${maintainer_mode}])dnl

if test "x${maintainer_mode}" = "xyes"; then
  MAINT=''
  NOTMAINT='#'
else
  MAINT='#'
  NOTMAINT=''
fi
AC_SUBST([MAINT])dnl
AC_SUBST([NOTMAINT])dnl

# Do we have a single-tree copy of texinfo? Even if we do, we cannot
# rely on it - libiberty is built before texinfo.
AC_CHECK_PROG([MAKEINFO],[makeinfo],[makeinfo],[])
if test "x${MAKEINFO}" = "x"; then
    MAKEINFO="@echo makeinfo missing; true"
    BUILD_INFO=""
else
    BUILD_INFO=info
    case "${MAKEINFO}" in
      */missing\ makeinfo*)
	BUILD_INFO=""
	AC_MSG_WARN([
*** Makeinfo is missing. Info documentation will not be built.])
        ;;
      *)
	AC_PROG_GREP
	case x"`${MAKEINFO} --version | grep 'GNU texinfo'`" in
	  x*\ [[1-3]].* )
	    MAKEINFO="@echo ${MAKEINFO} is too old, 4.0 or newer required; true"
	    BUILD_INFO=""
	    AC_MSG_WARN([
*** Makeinfo is too old. Info documentation will not be built.])
	  ;;
	esac
	;;
    esac
fi
AC_SUBST([MAKEINFO])dnl
AC_SUBST([BUILD_INFO])dnl

AC_CHECK_PROG([PERL],[perl],[perl],[])dnl
  ## have it enable/disable certain parts of the Makefile:
if test x"${PERL}" = x""; then
  HAVE_PERL='#'
else
  HAVE_PERL=''
fi
AC_SUBST([HAVE_PERL])dnl

# architectures:
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

# libtoolize will delete this, because we do not actually use automake:
if test "x${ac_aux_dir}" != "x"; then
  test -n "${ac_aux_dir}" && test -d "${ac_aux_dir}"
  if test ! -e ${ac_aux_dir}/missing && test -w "${ac_aux_dir}"; then
    if test -e ./missing && test "x${ac_aux_dir}" != "x."; then
      cp ./missing ${ac_aux_dir}
    elif test -e ../missing && test "x${ac_aux_dir}" != "x.."; then
      cp ../missing ${ac_aux_dir}
    elif test -e ../config/missing && test "x${ac_aux_dir}" != "x../config"; then
      cp ../config/missing ${ac_aux_dir}
    fi
  fi
else
  test -z "${ac_aux_dir}"
fi

dnl# When we start using automake with a legit Makefile.am; un-comment and
dnl# un-escape this:
dnl#AM\_INIT\_AUTOMAKE\(\[gnu\ dejagnu\ \-Wno\-obsolete\]\)
AM_MISSING_PROG([AUTOCONF],[autoconf])dnl

dnl# until then, though, use this as a substitute:
AC_DEFUN([AM_INIT_MY_AUTOMAKE_LITE],[
  AC_PREREQ([2.65])dnl
  dnl# Autoconf wants to disallow AM_ names. We explicitly allow
  dnl# the ones we care about.
  m4_pattern_allow([^AM_[A-Z]+FLAGS$])dnl
  AC_REQUIRE([AM_SET_CURRENT_AUTOMAKE_VERSION])dnl
  AC_REQUIRE([AC_PROG_SED])dnl

  # test whether we have cygpath:
  if test -z "${CYGPATH_W}"; then
    if (cygpath --version) >/dev/null 2>/dev/null; then
      CYGPATH_W='cygpath -w'
    else
      CYGPATH_W=echo
    fi
  fi
  AC_SUBST([CYGPATH_W])dnl

  # Some tools Automake needs.
  AC_REQUIRE([AM_SANITY_CHECK])dnl
  AC_REQUIRE([AC_ARG_PROGRAM])dnl
  AC_REQUIRE([AM_MISSING_HAS_RUN])dnl
  AM_MISSING_PROG([ACLOCAL],[aclocal])dnl
  AM_MISSING_PROG([AUTOHEADER],[autoheader])dnl
  AM_MISSING_PROG([AUTOMAKE],[automake])dnl
  AC_REQUIRE([AM_PROG_INSTALL_SH])dnl
  dnl# For better backward compatibility. To be removed once Automake 1.9.x
  dnl# dies out for good.
  AC_SUBST([mkdir_p],['$(MKDIR_P)'])dnl
  dnl# We need awk for the "check" target. The system "awk" is bad on
  dnl# some platforms.
  AC_REQUIRE([AM_SET_LEADING_DOT])dnl
  AC_REQUIRE([AM_SILENT_RULES])dnl
  AC_REQUIRE([AC_PROG_GREP])dnl
  AC_REQUIRE([AM_SET_DEPDIR])dnl
  AC_REQUIRE([AM_OUTPUT_DEPENDENCY_COMMANDS])dnl
  AC_REQUIRE([AM_MAKE_INCLUDE])dnl
  AC_REQUIRE([AM_DEP_TRACK])dnl

  dnl######################################################################
  dnl# Port to Automake 1.11.
  dnl# This section can be removed once we assume Automake 1.14 or later.
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

  dnl## from configmake prep:
  dnl# Added in autoconf 2.70:
  if test "x${runstatedir}" = "x"; then
    AC_SUBST([runstatedir],['${localstatedir}/run'])
  fi
  dnl# Automake 1.9.6 only lacks pkglibexecdir; and since 1.11 merely
  dnl# provides it without AC_SUBST, this blind use of AC_SUBST is safe.
  AC_SUBST([pkglibexecdir],['${libexecdir}/${PACKAGE}'])dnl
])dnl
AM_INIT_MY_AUTOMAKE_LITE

if test -e ./config.cache; then
  sed -i "s|ac_cv_env_CFLAGS_value|bad_CFLAGS|g" ./config.cache
fi

m4_ifdef([AC_REQUIRE_AUX_FILE],[
  AC_REQUIRE_AUX_FILE([compile])dnl
  AC_REQUIRE_AUX_FILE([missing])dnl
])dnl

dnl# These must be called before LT_INIT, because it may want
dnl# to call AC_CHECK_PROG.
GCC_NO_EXECUTABLES
AC_PROG_GCC_TRADITIONAL
AC_USE_SYSTEM_EXTENSIONS
m4_ifdef([AM_PROG_AR],[
  if test "x${AR}" = "x" && test "x${am_cv_ar_interface}" = "x"; then
    test -z "${AR}" && test -z "${am_cv_ar_interface}"
    AM_PROG_AR
  elif test "x${ac_ct_AR}" = "x" && test "x${ac_cv_prog_AR}" = "x"; then
    test -z "${ac_ct_AR}" && test -z "${ac_cv_prog_AR}"
    AC_CHECK_TOOLS([AR],[ar gar])
  fi
],[
  AC_CHECK_TOOLS([AR],[ar gar])dnl
])dnl
m4_ifdef([AC_PROG_RANLIB],[
if test "x${RANLIB}" = "x"; then
  test -z "${RANLIB}"
  AC_PROG_RANLIB
fi
],[
if test "x${ac_cv_prog_ac_ct_RANLIB}" = "x"; then
  test -z "${ac_cv_prog_ac_ct_RANLIB}"
  AC_CHECK_TOOLS([RANLIB],[ranlib granlib],[:])
fi
])dnl

dnl# When switching to automake, replace the following block with
dnl# the AM_ENABLE_MULTILIB macro:
# Add --enable-multilib to configure.
# Default to --enable-multilib
AC_ARG_ENABLE([multilib],
[AS_HELP_STRING([--enable-multilib],
                [try to build many library versions (default off)])],
[case "${enableval}" in
  yes) multilib=yes ;;
  no)  multilib=no ;;
  *)   AC_MSG_ERROR([bad value ${enableval} for multilib option]) ;;
 esac],
	      [multilib=no])dnl

# Even if the default multilib is not a cross compilation,
# it may be that some of the other multilibs are.
if test "x${cross_compiling}" = "xno" && test "x${multilib}" = "xyes" \
   && test "x${with_multisubdir}" != "x"; then
   cross_compiling=maybe
fi

# The flag for installing the target headers is further below.
test -d .

# More checks for programs:
AM_PROG_AS
AC_SYS_LARGEFILE
AC_PROG_CPP_WERROR
AC_PROG_MKDIR_P

m4_ifdef([ACX_PROG_CC_WARNING_OPTS],[
  dnl# since we actually check the flags in this case, we can try more of
  dnl# them:
  ACX_PROG_CC_WARNING_OPTS([-Wall -Wextra -Wwrite-strings -Wc++-compat \
-Wodr -Wstrict-prototypes -Wmissing-prototypes -Wmissing-parameter-type \
-Wmissing-declarations -Wnested-externs -Wundef -Wshadow -Wformat=2 \
-Wold-style-definition -Wold-style-declaration -Wfloat-conversion -Wabi \
-Wc99-c11-compat],[ac_libiberty_warn_cflags])dnl
  m4_ifdef([ACX_PROG_CC_WARNING_ALMOST_PEDANTIC],[
    ACX_PROG_CC_WARNING_ALMOST_PEDANTIC([],[ac_libiberty_warn_cflags])dnl
  ])dnl
  ##FIXME: old versions of the above macros omitted the second argument:
  if test "x${ac_libiberty_warn_cflags}" = "x"; then
    test -z "${ac_libiberty_warn_cflags}"
    if test "x${WARN_CFLAGS}" != "x"; then
      test -n "${WARN_CFLAGS}" && ac_libiberty_warn_cflags="${WARN_CFLAGS}"
    elif test "x${WARN_PEDANTIC}" != "x"; then
      test -n "${ac_libiberty_warn_cflags}" && ac_libiberty_warn_cflags="${WARN_PEDANTIC}"
    elif test "x${LIBIBERTY_WARN_CFLAGS}" != "x"; then
      test -n "${LIBIBERTY_WARN_CFLAGS}" && ac_libiberty_warn_cflags="${LIBIBERTY_WARN_CFLAGS}"
    fi
  fi
],[
AC_MSG_CHECKING([warning flags to use])
if test "x${GCC}" = "xyes"; then
  ac_libiberty_warn_cflags='-Wall -Wextra -pedantic -Wwrite-strings -Wc++-compat -Wmissing-prototypes -Wstrict-prototypes -Wold-style-definition -Wundef'
else
  ac_libiberty_warn_cflags=''
fi
AC_MSG_RESULT([${ac_libiberty_warn_cflags}])
])dnl
AC_SUBST([ac_libiberty_warn_cflags])dnl

AC_PROG_CC_C_O
# autoconf is lame and does not give us any substitution variable for this:
if eval "test \"`echo '$ac_cv_prog_cc_'${ac_cc}_c_o`\" = no"; then
  NO_MINUS_C_MINUS_O=yes
else
  OUTPUT_OPTION='-o $@'
fi
AC_SUBST([NO_MINUS_C_MINUS_O])dnl
AC_SUBST([OUTPUT_OPTION])dnl

# C compiler characteristics:
AC_C_CONST
AC_C_INLINE
AC_C_BIGENDIAN
AC_C_PROTOTYPES
AC_C_VOLATILE

dnl# When we start using libtool:
dnl# Default to a non shared library. This may be overridden by the
dnl# configure option --enable-shared.
AC_DISABLE_SHARED

dnl# When we start using libtool:
AC_DEFUN([AC_EXTRA_THINGS_LIBTOOL_WANTS],[
  AC_REQUIRE([AC_PROG_AWK])dnl
  AC_REQUIRE([AC_PROG_CC])dnl
  AC_REQUIRE([AC_PROG_CPP])dnl
  AC_REQUIRE([AC_PROG_CXX])dnl
  AC_REQUIRE([AC_PROG_MAKE_SET])dnl
])dnl
AC_EXTRA_THINGS_LIBTOOL_WANTS
LT_INIT([disable-shared pic-only])dnl
LT_PREREQ([2.4.6])dnl

AC_CONFIG_HEADERS([config.h])dnl

dnl# When we start using automake:
AM_MAINTAINER_MODE

dnl# back to non-automake-requiring things:
AC_EXEEXT
AC_OBJEXT

dnl# When we start using automake:
AM_PROG_INSTALL_STRIP

dnl# back to non-automake-requiring things:
AC_PROG_INSTALL
AC_PROG_LN_S

# Do NOT build the shared library for build.
if [[ -n "${with_build_subdir}" ]]; then
  enable_shared=no
fi

frag=""
case "${host}" in
  rs6000-ibm-aix3.1 | rs6000-ibm-aix)
                        frag=mh-aix ;;
  *-*-cxux7*)		frag=mh-cxux7 ;;
  *-*-freebsd2.1.*)	frag=mh-fbsd21 ;;
  *-*-freebsd2.2.[[012]])	frag=mh-fbsd21 ;;
  i370-*-opened*)       frag=mh-openedition ;;
  i[[34567]]86-*-windows*)	frag=mh-windows ;;
  # APPLE LOCAL - Allow more than 256 streams to be open at a time.
  arm-apple-darwin*) frag=mh-arm-macosx;;
  *-apple-darwin* | *-macos*) frag=mh-macosx;;
esac

if [[ -n "${frag}" ]]; then
  frags=${libiberty_topdir}/libiberty/config/${frag}
else
  frags=""
fi

m4_ifdef([GCC_PICFLAG],[
  GCC_PICFLAG
],[
if test "x${PICFLAG}" = "x"; then
  test -z "${PICFLAG}" && PICFLAG=""
fi
if test "x${NOPICFLAG}" = "x"; then
  test -z "${NOPICFLAG}" && NOPICFLAG=""
fi
case "${host}" in
  i[[34567]]86-*-darwin* | powerpc-*-darwin*)
    NOPICFLAG=-mdynamic-no-pic ;;
  *) ;;
esac
])dnl

# If they did NOT specify --enable-shared, do NOT generate shared libs.
case "${enable_shared}" in
  yes) shared=yes ;;
  no) shared=no ;;
  "") shared=no ;;
  *) shared=yes ;;
esac
# ...unless --enable-host-shared was passed from top-level config:
if [[ "x${enable_host_shared}" = "xyes" ]]; then
  shared=yes
fi
if [[ "x${shared}" = "xyes" ]]; then
  frag=""
  case "${host}" in
    *-*-cygwin*)	;;
    alpha*-*-linux*)	frag=mh-elfalphapic ;;
    arm*-*-*)		frag=mh-armpic ;;
    *-apple-darwin*)    frag=mh-darwinpic ;;
    hppa*-*-*)		frag=mh-papic ;;
    i[[34567]]86-*-* | x86_64-*-*)
			frag=mh-x86pic ;;
    powerpc*-*-aix*)	;;
    powerpc*-*-*)	frag=mh-ppcpic ;;
    sparc*-*-*)		frag=mh-sparcpic ;;
    s390*-*-*)		frag=mh-s390pic ;;
    *)			frag=mh-${host_cpu}pic ;;
  esac
  if [[ -n "${frag}" ]]; then
    frags="${frags} ${libiberty_topdir}/config/${frag}"
  fi
elif [[ "x${shared}" != "xyes" ]]; then
  if test "x${PICFLAG}" != "x"; then
    test -n "${PICFLAG}"
    AC_MSG_WARN([libraries are unshared; overriding user-supplied PICFLAG])
  fi
  PICFLAG=""
  NOPICFLAG=""
fi
AC_SUBST([PICFLAG])dnl
AC_SUBST([NOPICFLAG])dnl

NOASANFLAG=""
case " ${CFLAGS} " in
  *\ -fsanitize=address\ *) NOASANFLAG=-fno-sanitize=address ;;
esac
AC_SUBST([NOASANFLAG])dnl

AC_MSG_CHECKING([if we need to modify your Makefile fragment at all])
AC_MSG_RESULT([ ])
echo "# Warning: this fragment is automatically generated" > temp-frag

for frag in ${frags}; do
  if [[ -f ${frag} ]]; then
    echo "Appending ${frag} to xhost-mkfrag"
    echo "# Following fragment copied from ${frag}" >> temp-frag
    cat ${frag} >> temp-frag
  fi
done

# record if we want to build shared libs:
if [[ "x${shared}" = "xyes" ]]; then
  echo "enable_shared = yes" >> temp-frag
else
  echo "enable_shared = no" >> temp-frag
fi

frag=xhost-mkfrag
${CONFIG_SHELL-/bin/sh} ${libiberty_topdir}/move-if-change temp-frag xhost-mkfrag

host_makefile_frag=${frag}
AC_SUBST_FILE([host_makefile_frag])dnl

# It is OK to check for header files. Although the compiler may not be
# able to link anything, it had better be able to at least
# compile (or at least preprocess) something.
AC_MSG_CHECKING([headers])
AC_MSG_RESULT([ ])
AC_DEFUN([AC_REQUIRE_HEADER_STDC],[
  AC_REQUIRE([AC_HEADER_STDC])dnl
])dnl
AC_REQUIRE_HEADER_STDC
AC_HEADER_STAT
AC_HEADER_STDBOOL dnl# also "_CHECK"s it
AC_CHECK_HEADERS([alloca.h assert.h ctype.h fcntl.h float.h getopt.h \
  langinfo.h libgen.h libintl.h limits.h locale.h \
  locale/coll-lookup.h locale/elem-hash.h locale/localeinfo.h \
  locale/weight.h locale/weightwc.h \
  mach/mach.h machine/hal_sysinfo.h math.h malloc.h malloc/malloc.h \
  process.h regex.h \
  stdarg.h stddef.h stdio.h stdio_ext.h \
  sys/file.h sys/mman.h sys/param.h sys/prctl.h sys/pstat.h \
  sys/resource.h sys/sysctl.h sys/sysinfo.h sys/sysmp.h sys/systemcfg.h \
  sys/table.h sys/time.h \
  time.h wchar.h wctype.h])dnl
AC_HEADER_SYS_WAIT
AC_HEADER_TIME

libiberty_AC_DECLARE_ERRNO

# It is also okay to check for types.
AC_MSG_CHECKING([types])
AC_MSG_RESULT([ ])dnl

# Determine sizes of some types.
AC_CHECK_SIZEOF([int])dnl
AC_CHECK_ALIGNOF([int])dnl
AC_CHECK_SIZEOF([long])dnl
AC_CHECK_ALIGNOF([long])dnl

# Check for presense of the 'long long' type:
AC_CHECK_TYPE([long long],
  [AC_DEFINE([HAVE_LONG_LONG],[1],
             [Define if you have the `long long' type.])dnl
   AC_CHECK_SIZEOF([long long])dnl
   AC_CHECK_ALIGNOF([long long])dnl
  ],[])dnl

# Look for a 64-bit type.
AC_MSG_CHECKING([for a 64-bit type])
AC_CACHE_VAL([liberty_cv_uint64],
[AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#ifdef HAVE_STDINT_H
# include <stdint.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "This conftest expects <stdint.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_STDINT_H */
]],[[extern uint64_t foo;]])],[liberty_cv_uint64=uint64_t],[
AC_COMPILE_IFELSE([AC_LANG_SOURCE([[
#ifdef HAVE_LIMITS_H
# include <limits.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "This conftest expects <limits.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_LIMITS_H */
#ifndef CHAR_BIT
# define CHAR_BIT 8
#endif /* !CHAR_BIT */
]],[[
extern char foo[sizeof(long) * CHAR_BIT >= 64 ? 1 : -1];
]])],
[liberty_cv_uint64="unsigned long"],
[AC_COMPILE_IFELSE([AC_LANG_SOURCE([[
#ifdef HAVE_LIMITS_H
# include <limits.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "This conftest expects <limits.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_LIMITS_H */
#ifndef CHAR_BIT
# define CHAR_BIT 8
#endif /* !CHAR_BIT */
]],[[
extern char foo[sizeof(long long) * CHAR_BIT >= 64 ? 1 : -1];
]])],
[liberty_cv_uint64="unsigned long long"],[liberty_cv_uint64=none])])])])
AC_MSG_RESULT([${liberty_cv_uint64}])
if test "x${liberty_cv_uint64}" != "xnone"; then
  AC_DEFINE_UNQUOTED([UNSIGNED_64BIT_TYPE],[${liberty_cv_uint64}],
             [Define to an unsigned 64-bit type available in the compiler])
fi

AC_TYPE_INTPTR_T
m4_ifdef([AC_TYPE_UINTPTR_T],[
  AC_TYPE_UINTPTR_T
],[
  AC_CHECK_TYPE([uintptr_t],[unsigned long])dnl
])dnl

# Given the above check, we always have uintptr_t or a fallback
# definition. So define HAVE_UINTPTR_T in case any imported code
# relies on it.
AC_DEFINE([HAVE_UINTPTR_T],[1],[Define if you have the \`uintptr_t' type.])

# other types and structs:
AC_CHECK_TYPES([clock_t])dnl
AC_TYPE_OFF_T
AC_TYPE_PID_T
AC_CHECK_TYPES([ptrdiff_t])dnl
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_INT32_T
AC_TYPE_UID_T
AC_TYPE_UINT8_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_CHECK_TYPES([regex_t],[],[],[
#ifdef HAVE_REGEX_H
# include <regex.h>
#endif /* HAVE_REGEX_H */
])dnl
AC_CHECK_TYPES([regmatch_t],[],[],[
#ifdef HAVE_REGEX_H
# include <regex.h>
#endif /* HAVE_REGEX_H */
])dnl
AC_STRUCT_TIMEZONE dnl# also does the "_TM" one

# This is the list of functions which libiberty will provide if they
# are not available on the host:
funcs="asprintf"
funcs="${funcs} atexit"
funcs="${funcs} basename"
funcs="${funcs} bcmp"
funcs="${funcs} bcopy"
funcs="${funcs} bsearch"
funcs="${funcs} bzero"
funcs="${funcs} calloc"
funcs="${funcs} clock"
funcs="${funcs} ffs"
funcs="${funcs} getcwd"
funcs="${funcs} getpagesize"
funcs="${funcs} gettimeofday"
funcs="${funcs} index"
funcs="${funcs} insque"
funcs="${funcs} memchr"
funcs="${funcs} memcmp"
funcs="${funcs} memcpy"
funcs="${funcs} memmem"
funcs="${funcs} memmove"
funcs="${funcs} mempcpy"
funcs="${funcs} memset"
funcs="${funcs} mkstemps"
funcs="${funcs} putenv"
funcs="${funcs} random"
funcs="${funcs} rename"
funcs="${funcs} rindex"
funcs="${funcs} setenv"
funcs="${funcs} snprintf"
funcs="${funcs} sigsetmask"
funcs="${funcs} stpcpy"
funcs="${funcs} stpncpy"
funcs="${funcs} strcasecmp"
funcs="${funcs} strchr"
funcs="${funcs} strdup"
funcs="${funcs} strncasecmp"
funcs="${funcs} strndup"
funcs="${funcs} strnlen"
funcs="${funcs} strrchr"
funcs="${funcs} strstr"
funcs="${funcs} strtod"
funcs="${funcs} strtol"
funcs="${funcs} strtoll"
funcs="${funcs} strtoul"
funcs="${funcs} strtoull"
funcs="${funcs} strverscmp"
funcs="${funcs} tmpnam"
funcs="${funcs} vasprintf"
funcs="${funcs} vfprintf"
funcs="${funcs} vprintf"
funcs="${funcs} vsnprintf"
funcs="${funcs} vsprintf"
funcs="${funcs} waitpid"
funcs="${funcs} setproctitle"

# Also in the old function.def file: alloca, vfork, getopt.

vars="sys_errlist sys_nerr sys_siglist"

checkfuncs="__fsetlocking btowc bzero canonicalize_file_name dup3"
checkfuncs="${checkfuncs} getrlimit getrusage getsysinfo gettimeofday"
checkfuncs="${checkfuncs} on_exit psignal pstat_getdynamic pstat_getstatic"
checkfuncs="${checkfuncs} realpath setrlimit sbrk spawnve spawnvpe"
checkfuncs="${checkfuncs} strerror strsignal sysconf sysctl sysmp table"
checkfuncs="${checkfuncs} times wait3 wait4"

# These are neither executed nor required, but they help keep
# autoheader happy without adding a bunch of text to acconfig.h.
# They also keep autoscan happy as well.
if test "x" = "Y0"; then
  AC_MSG_WARN([this condition should not actually be reached])
  AC_FUNC_ALLOCA
  AC_FUNC_CHOWN
  AC_FUNC_ERROR_AT_LINE
  AC_FUNC_LSTAT dnl# also checks whether it "_FOLLOWS_SLASHED_SYMLINK"
  AC_FUNC_MALLOC
  AC_FUNC_MEMCMP
  AC_FUNC_MBRTOWC
  AC_DEFUN([AC_FUNC_MMAP],[AC_CHECK_FUNCS([mmap])])dnl
  AC_FUNC_MMAP
  AC_FUNC_OBSTACK
  AC_FUNC_REALLOC
  AC_FUNC_STAT
  AC_FUNC_STRTOD
  AC_FUNC_STRNLEN
  AC_FUNC_UTIME_NULL
  AC_FUNC_VPRINTF
  AC_REPLACE_FNMATCH
  AC_CHECK_FUNCS([abort asprintf atexit \
    basename bcmp bcopy bsearch btowc bzero \
    calloc canonicalize_file_name clock dup2 dup3 ffs __fsetlocking \
    getcwd getpagesize getrlimit getrusage getsysinfo \
    gettext gettimeofday getwd index insque memchr memcmp memcpy memmem \
    memmove mempcpy memset mkdir mkstemps munmap \
    on_exit pathconf psignal pstat_getdynamic pstat_getstatic putenv \
    random realpath re_comp regcomp rename rindex rmdir \
    sbrk setenv setproctitle setrlimit sigsetmask snprintf \
    spawnve spawnvpe stpcpy stpncpy strcasecmp strchr strcspn \
    strdup strerror strlen strncasecmp strndup strnlen strpbrk strrchr \
    strsignal strspn strstr strtod strtol strtoll strtoul strtoull \
    strverscmp sysconf sysctl sysmp system \
    table times tmpnam unlink utime \
    vasprintf vfprintf vprintf vsnprintf vsprintf wait3 wait4 waitpid])dnl
  dnl# actual checks for decls are done later than this:
  AC_CHECK_DECLS([basename(char *), strtol, strtoul, strtoll, strtoull])dnl
  AC_CHECK_DECLS([basename, ffs, asprintf, vasprintf, snprintf, vsnprintf])
  AC_CHECK_DECLS([sys_siglist])dnl
  AC_DEFINE([HAVE_SYS_ERRLIST],[1],
            [Define if you have the sys_errlist variable.])dnl
  AC_DEFINE([HAVE_SYS_NERR],[1],
            [Define if you have the sys_nerr variable.])dnl
  AC_DEFINE([HAVE_SYS_SIGLIST],[1],
            [Define if you have the sys_siglist variable.])dnl
  ## that should be enough for now...
else
  ## APPLE LOCAL: mmap() always works right on Darwin. Take our word for it
  case "${host}" in
    *-apple-darwin* | *-apple-macos*)
      ac_cv_func_mmap_fixed_mapped=yes
      AC_DEFINE([HAVE_MMAP],[1],
                [Define to 1 if you have the `mmap' function.])dnl
      ## skip actually checking for it though...
      ;;
    *)
      if test "x${gcc_no_link}" = "xyes"; then
        if test "x${ac_cv_func_mmap_fixed_mapped+set}" != "xset"; then
          ac_cv_func_mmap_fixed_mapped=no
        fi
      fi
      ;;
  esac
fi

# For each of these functions, if the host does not provide the
# function we want to put FN.o in LIBOBJS, and if the host does
# provide the function, we want to define HAVE_FN in config.h.

setobjs=""
CHECK=""
target_header_dir=""
if test -n "${with_target_subdir}"; then

  # We are being configured as a target library.  AC_REPLACE_FUNCS
  # may not work correctly, because the compiler may not be able to
  # link executables.  Note that we may still be being configured
  # native.

  # If we are being configured for newlib, we know which functions
  # newlib provide and which ones we will be expected to provide.

  if test "x${with_newlib}" = "xyes"; then
    AC_LIBOBJ([asprintf])dnl
    AC_LIBOBJ([basename])dnl
    AC_LIBOBJ([insque])dnl
    AC_LIBOBJ([random])dnl
    AC_LIBOBJ([strdup])dnl
    AC_LIBOBJ([vasprintf])dnl

    for f in ${funcs}; do
      case "${f}" in
	asprintf | basename | insque | random | strdup | vasprintf)
	  ;;
	*)
	  n=HAVE_`echo ${f} | tr 'abcdefghijklmnopqrstuvwxyz' 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'`
	  AC_DEFINE_UNQUOTED([${n}],[1],
                     [Define to 1 if the result for finding ${f} is "yes"])
	  ;;
      esac
    done

    # newlib does NOT provide any of the variables in ${vars}, so we
    # do not have to check them here.

    # Of the functions in ${checkfuncs}, newlib only has strerror.
    AC_DEFINE([HAVE_STRERROR],[1],[Define to 1 if we have strerror])
    setobjs=yes
  fi

  # If we are being configured for Mingw, we know which functions
  # Mingw provides and which ones we will be expected to provide.

  case "${host}" in
  *-*-mingw*)
    AC_LIBOBJ([asprintf])dnl
    AC_LIBOBJ([basename])dnl
    AC_LIBOBJ([bcmp])dnl
    AC_LIBOBJ([bcopy])dnl
    AC_LIBOBJ([bzero])dnl
    AC_LIBOBJ([clock])dnl
    AC_LIBOBJ([ffs])dnl
    AC_LIBOBJ([getpagesize])dnl
    AC_LIBOBJ([index])dnl
    AC_LIBOBJ([insque])dnl
    AC_LIBOBJ([mempcpy])dnl
    AC_LIBOBJ([mkstemps])dnl
    AC_LIBOBJ([random])dnl
    AC_LIBOBJ([rindex])dnl
    AC_LIBOBJ([sigsetmask])dnl
    AC_LIBOBJ([stpcpy])dnl
    AC_LIBOBJ([stpncpy])dnl
    AC_LIBOBJ([strndup])dnl
    AC_LIBOBJ([strnlen])dnl
    AC_LIBOBJ([strverscmp])dnl
    AC_LIBOBJ([vasprintf])dnl
    AC_LIBOBJ([waitpid])dnl

    for f in ${funcs}; do
      case "${f}" in
	asprintf | basename | bcmp | bcopy | bzero | clock | ffs | getpagesize | index | insque | mempcpy | mkstemps | random | rindex | sigsetmask | stpcpy | stpncpy | strdup | strndup | strnlen | strverscmp | vasprintf | waitpid)
	  ;;
	*)
	  n=HAVE_`echo ${f} | tr 'abcdefghijklmnopqrstuvwxyz' 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'`
	  AC_DEFINE_UNQUOTED([${n}],[1],
                     [Define to 1 if the result for finding ${f} is "yes"])
	  ;;
      esac
    done

    # Mingw does NOT provide any of the variables in ${vars}, so we
    # dont have to check them here.

    # Of the functions in ${checkfuncs}, Mingw only has strerror.
    AC_DEFINE([HAVE_STRERROR],[1],[Define to 1 if we have strerror])
    setobjs=yes
    ;;

  *-*-msdosdjgpp)
    AC_LIBOBJ([vasprintf])dnl
    AC_LIBOBJ([vsnprintf])dnl
    AC_LIBOBJ([snprintf])dnl
    AC_LIBOBJ([asprintf])dnl

    for f in atexit basename bcmp bcopy bsearch bzero calloc clock ffs \
             getcwd getpagesize getrusage gettimeofday \
             index insque memchr memcmp memcpy memmove memset psignal \
             putenv random rename rindex sbrk setenv stpcpy strcasecmp \
             strchr strdup strerror strncasecmp strrchr strstr strtod \
             strtol strtoul sysconf times tmpnam vfprintf vprintf \
             vsprintf waitpid
    do
      n=HAVE_`echo ${f} | tr 'abcdefghijklmnopqrstuvwxyz' 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'`
      AC_DEFINE_UNQUOTED([${n}],[1],
                     [Define to 1 if the result for finding ${f} is "yes"])

    done

    setobjs=yes
    ;;

  esac

  # We may wish to install the target headers somewhere:
  AC_ARG_ENABLE([install-libiberty],
  [AS_HELP_STRING([--enable-install-libiberty],
                  [Install headers for end users])],
  [enable_install_libiberty=${enableval}],
  [enable_install_libiberty=no])dnl
  
  # Option parsed, now set things appropriately:
  case x"${enable_install_libiberty}" in
    xyes|x)
      target_header_dir=libiberty
      ;;
    xno)   
      target_header_dir=""
      ;;
    *) 
      # This could be sanity-checked in various ways...
      target_header_dir="${enable_install_libiberty}"
      ;;
  esac
else
   # Not a target library, so we set things up to run the test suite:
   CHECK=really-check
fi

AC_SUBST([CHECK])dnl
AC_SUBST([target_header_dir])dnl

case "${host}" in
  *-*-cygwin* | *-*-mingw*)
    AC_DEFINE([HAVE_SYS_ERRLIST],[1],
              [Define if you have the sys_errlist variable.])dnl
    AC_DEFINE([HAVE_SYS_NERR],[1],
              [Define if you have the sys_nerr variable.])dnl
    ## hope those assumptions stay correct...
    ;;
esac

if test -z "${setobjs}"; then
  case "${host}" in
    *-*-vxworks*)
      # Handle VxWorks configuration specially, since on VxWorks the
      # libraries are actually on the target board, not in the file
      # system.
      AC_LIBOBJ([basename])dnl
      AC_LIBOBJ([getpagesize])dnl
      AC_LIBOBJ([insque])dnl
      AC_LIBOBJ([random])dnl
      AC_LIBOBJ([strcasecmp])dnl
      AC_LIBOBJ([strncasecmp])dnl
      AC_LIBOBJ([strdup])dnl
      AC_LIBOBJ([vfork])dnl
      AC_LIBOBJ([waitpid])dnl
      AC_LIBOBJ([vasprintf])dnl

      for f in ${funcs}; do
        case "${f}" in
	      basename | getpagesize | insque | random | strcasecmp)
	        ;;
	      strncasecmp | strdup | vfork | waitpid | vasprintf)
	        ;;
	      *)
	        n=HAVE_`echo ${f} | tr 'abcdefghijklmnopqrstuvwxyz' 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'`
	        AC_DEFINE_UNQUOTED([${n}],[1],
                     [Define to 1 if the result for finding ${f} is "yes"])
	        ;;
        esac
      done

      # VxWorks does NOT provide any of the variables in ${vars}, so we
      # do NOT have to check them here.

      # Of the functions in ${checkfuncs}, VxWorks only has strerror.
      AC_DEFINE([HAVE_STRERROR],[1],[Define to 1 if we have strerror])dnl

      setobjs=yes
      ;;

    *)
      test -z "${setobjs}"
      ;;
  esac
fi

if test -z "${setobjs}"; then
  if test "x${SED}" = "x" -a "x${ac_cv_path_SED}" = "x"; then
    test -z "${SED}" && test -z "${ac_cv_path_SED}"
    AC_PROG_SED
  fi
  case "${host}" in
    *-*-cygwin*)
      # The Cygwin library actually uses a couple of files from
      # libiberty when it is built. If we are building a native
      # Cygwin, and we run the tests, we will appear to have these
      # files. However, when we go on to build winsup, we will wind up
      # with a library which does not have the files, since they should
      # have come from libiberty.

      # We handle this by removing the functions the winsup library
      # provides from our shell variables, so that they appear to be
      # missing.

      #DJ: only if we are *building* cygwin, not just building *with* it
  
      if test -n "${with_target_subdir}"
      then
        funcs="`echo ${funcs} | sed -e 's/random//'`"
        AC_LIBOBJ([random])
        vars="`echo ${vars} | sed -e 's/sys_siglist//'`"
        checkfuncs="`echo ${checkfuncs} | sed -e 's/strsignal//' -e 's/psignal//'`"
      fi
      ;;
    *-*-mingw32*)
      # Under mingw32, sys_nerr and sys_errlist exist, but they are
      # macros, so the test below will not find them.
      libiberty_cv_var_sys_nerr=yes
      libiberty_cv_var_sys_errlist=yes
      AC_SUBST([libiberty_cv_var_sys_nerr])
      AC_SUBST([libiberty_cv_var_sys_errlist])
      ;;
    *-*-msdosdjgpp*)
      # vfork and fork are stubs.
      ac_cv_func_vfork_works=no
      AC_SUBST([ac_cv_func_vfork_works])
      ;;
    *-*-uwin*)
      # Under some versions of uwin, vfork is buggy and the test 
      # can hang configure; on other versions, vfork exists just as a stub.
      # FIXME: This can be removed once vfork in uwin's runtime is fixed.
      ac_cv_func_vfork_works=no
      AC_SUBST([ac_cv_func_vfork_works])
      # Under uwin 2.0+, sys_nerr and sys_errlist exist, but they are
      # macros (actually, these are imported from a DLL, but the end effect 
      # is the same), so the test below won't find them.
      libiberty_cv_var_sys_nerr=yes
      libiberty_cv_var_sys_errlist=yes
      AC_SUBST([libiberty_cv_var_sys_nerr])
      AC_SUBST([libiberty_cv_var_sys_errlist])
      ;;
    *-*-*vms*)
      # Under VMS, vfork works differently than on Unix. The standard test 
      # will not work, and it is not easily adapted. It makes more sense to
      # just force it.
      ac_cv_func_vfork_works=yes
      AC_SUBST([ac_cv_func_vfork_works])
      ;;
  esac

  # We have NOT set the list of objects yet. Use the standard autoconf
  # tests. This will only work if the compiler works.
  AC_SEARCH_LIBS([strerror],[cposix]) dnl# used to be AC\_ISC\_POSIX
  AC_MSG_CHECKING([which functions we need to replace])
  AC_MSG_RESULT([ ])
  AC_REPLACE_FUNCS([${funcs}])
  libiberty_AC_FUNC_C_ALLOCA
  AC_FUNC_FORK dnl# why is this macro giving me warnings here?
               dnl# It does not normally do so...
               dnl# (see ../config/acx.m4 for why)
  if test "x${ac_cv_func_vfork_works}" = "xno"; then
    AC_LIBOBJ([vfork])
  fi
  # We only need _doprnt if we might use it to implement v*printf.
  if test "x${ac_cv_func_vprintf}" != "xyes" \
     || test "x${ac_cv_func_vfprintf}" != "xyes" \
     || test "x${ac_cv_func_vsprintf}" != "xyes"; then
    AC_REPLACE_FUNCS([_doprnt])
  else
    AC_CHECK_FUNCS([_doprnt])
  fi

  for v in ${vars}; do
    AC_MSG_CHECKING([for var ${v}])
    AC_CACHE_VAL(libiberty_cv_var_$v,
      [AC_LINK_IFELSE([AC_LANG_PROGRAM([[int *p;]],[[
extern int ${v} []; p = ${v};
                      ]])],[eval "libiberty_cv_var_$v=yes"],
		           [eval "libiberty_cv_var_$v=no"])
                      ])
    if eval "test \"`echo '$libiberty_cv_var_'$v`\" = yes"; then
      AC_MSG_RESULT([yes])
      n=HAVE_`echo ${v} | tr 'abcdefghijklmnopqrstuvwxyz' 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'`
      AC_DEFINE_UNQUOTED([${n}],[1],
                     [Define to 1 if the result for finding ${v} is "yes"])
    else
      AC_MSG_RESULT([no])
    fi
  done

  # special check for _system_configuration because AIX <4.3.2 do not
  # contain the `physmem' member.
  AC_MSG_CHECKING([for external symbol _system_configuration])
  AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#ifdef HAVE_SYS_SYSTEMCFG_H
# include <sys/systemcfg.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "The conftest for _system_configuration wants <sys/systemcfg.h>"
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_SYS_SYTEMCFG_H */
    ]],[[
double x = _system_configuration.physmem;
    ]])],
    [AC_MSG_RESULT([yes])
     AC_DEFINE([HAVE__SYSTEM_CONFIGURATION],[1],
	       [Define if you have the _system_configuration variable.])],
    [AC_MSG_RESULT([no])])dnl

  # More function checks:
  AC_MSG_CHECKING([for functions that only need to be checked and not necessarily replaced])
  AC_MSG_RESULT([ ])
  unset ac_cv_func_bzero
  unset ac_cv_func_gettimeofday
  AC_CHECK_FUNCS([${checkfuncs}])dnl

  # (more) declaration checks:
  AC_MSG_CHECKING([declarations])
  AC_MSG_RESULT([ ])
  AC_CHECK_DECLS([basename],[],[],[
#ifdef HAVE_LIBGEN_H
# include <libgen.h>
#endif /* HAVE_LIBGEN_H */
  ])dnl
  AC_CHECK_DECLS([ffs, asprintf, vasprintf, snprintf, vsnprintf])dnl
  AC_CHECK_DECLS([calloc, getenv, getopt, malloc, realloc, sbrk])dnl
  AC_CHECK_DECLS([memmem, mempcpy, stpncpy, strndup, strnlen])dnl
  AC_CHECK_DECLS([strverscmp, btowc, bzero, mkstemps])dnl
  libiberty_NEED_DECLARATION([canonicalize_file_name])
fi

# Figure out which version of pexecute to use.
# objext is now attached in the Makefile instead of here.
case "${host}" in
     *-*-mingw* | *-*-winnt*)	pexecute=pex-win32  ;;
     *-*-msdosdjgpp*)		pexecute=pex-djgpp  ;;
     *-*-msdos*)		pexecute=pex-msdos  ;;
     *)				pexecute=pex-unix   ;;
esac
AC_SUBST([pexecute])dnl

libiberty_AC_FUNC_STRNCMP

# Install a library built with a cross compiler in $(tooldir) rather
# than $(libdir).
if test -z "${with_cross_host}"; then
  INSTALL_DEST=libdir
else
  INSTALL_DEST=tooldir
fi
AC_SUBST([INSTALL_DEST])dnl

m4_pattern_allow([LIBOBJS])
L=""
for l in x ${LIBOBJS}; do
  case ${l} in
    x) ;;
    *) L="${L} ./${l}" ;;
  esac
done
LIBOBJS="${L}"

AC_ARG_VAR([PICFLAG],
           [If you know what flags to set for compiling with PIC, you can try setting them with this variable (unsupported)])dnl
AC_SUBST([PICFLAG])dnl# (did we not already subst this once?)

dnl# Required by html and install-html:
AC_SUBST([datarootdir])dnl
AC_SUBST([docdir])dnl
AC_SUBST([htmldir])dnl

# Section for mmalloc support:
MMALLOC_CFLAGS=""
MMALLOC=""
AC_SUBST([MMALLOC_CFLAGS])dnl
AC_SUBST([MMALLOC])dnl

AC_ARG_WITH([mmalloc],
[AS_HELP_STRING([--with-mmalloc],[Use memory mapped malloc package])],
[case "${withval}" in
  yes) want_mmalloc=true ;;
  no) want_mmalloc=false;;
  *) AC_MSG_ERROR([bad value ${withval} for libiberty with-mmalloc flag])
     ;;
esac],[want_mmalloc=false])dnl

if test "x${want_mmalloc}" = "xtrue"; then
   AC_DEFINE([USE_MMALLOC],[1],[Define to 1 to use mmalloc])
   MMALLOC_CFLAGS="-I${srcdir}/../mmalloc"
   MMALLOC='../mmalloc/libmmalloc.a'
   MMALLOC_OK=''
else
   MMALLOC_OK='#'
fi
AC_SUBST([MMALLOC_OK])dnl

# Output.
# We need multilib support, but only if configuring for the target.
AC_CONFIG_FILES([Makefile testsuite/Makefile])
AC_CONFIG_COMMANDS([default],
  [[test -z "${CONFIG_HEADERS}" || echo "timestamp: `date`" > stamp-h
if test -n "${CONFIG_FILES}"; then
  if test -n "${with_target_subdir}"; then
    # FIXME: We should NOT need to set ac_file here:
    ac_file=Makefile
    LD="${ORIGINAL_LD_FOR_MULTILIBS}"
    echo "sourcing '${libiberty_topdir}/config-ml.in'..."
    . ${libiberty_topdir}/config-ml.in
  fi
fi]],
[[srcdir=${srcdir}
host=${host}
target=${target}
with_target_subdir=${with_target_subdir}
with_multisubdir=${with_multisubdir}
ac_multi_configure_args="--enable-multilib ${ac_configure_args}"
ac_configure_args="--disable-multilib ${ac_configure_args}"
CONFIG_SHELL=${CONFIG_SHELL-/bin/sh}
ORIGINAL_LD_FOR_MULTILIBS="${ORIGINAL_LD_FOR_MULTILIBS}"
libiberty_topdir=${libiberty_topdir}
]])dnl
AC_OUTPUT
