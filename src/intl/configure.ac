dnl#                                               -*- Autoconf -*-
dnl# Process this file with autoconf to produce a configure script.
dnl# libintl/configure.ac

AC_PREREQ([2.69])dnl
AC_INIT([libintl],[0.12.1],[jmolenda@apple.com])
dnl# bug report address is email address of last person to touch this
dnl# directory, according to the Changelog-Apple in this directory
AC_CONFIG_SRCDIR([dgettext.c])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])
m4_ifdef([AC_REQUIRE_AUX_FILE],[
  AC_REQUIRE_AUX_FILE([mkinstalldirs])dnl
  AC_REQUIRE_AUX_FILE([ylwrap])dnl
])dnl

echo "hello from the intl subdirectory's configure script (in `pwd`)"

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

AC_OBJEXT
AC_EXEEXT
AC_USE_SYSTEM_EXTENSIONS
if test -n "${HOST_ARCHITECTURE}"; then
  ACX_PROG_CC_WARNING_OPTS(["-arch ${HOST_ARCHITECTURE}"],
                           [C_ARCHFLAGS])
  AC_SUBST([C_ARCHFLAGS])
fi
if test -n "${TARGET_ARCHITECTURE}"; then
  ACX_PROG_CC_WARNING_OPTS(["-arch ${TARGET_ARCHITECTURE}"],
                           [XC_ARCHFLAGS])
  AC_SUBST([XC_ARCHFLAGS])
fi
if test -f ${srcdir}/config.status; then
  AC_MSG_NOTICE([config.status is already present, removing it.])
  rm -f ${srcdir}/config.status
fi
AM_INIT_AUTOMAKE([1.11.6 gnits dejagnu -Wno-obsolete])
m4_ifdef([AM_MAINTAINER_MODE],[
  test -e "." && test -d "." && test -r "." && stat "."
  if test -e "Makefile"; then
    AC_MSG_NOTICE([A Makefile is already present; removing it.])
    test -e "Makefile" && rm -f "Makefile"
  else
    AC_MSG_NOTICE([No Makefile present...])
  fi
  AM_MAINTAINER_MODE
],[
  AC_ARG_ENABLE([maintainer-mode],
                [AS_HELP_STRING([--enable-maintainer-mode],
                                [enable rules only needed by maintainers])],
                [],[enable_maintainer_mode=no])
  if test "x${enable_maintainer_mode}" = "xno"; then
    MAINT='#'
  else
    MAINT=''
  fi
  AC_SUBST([MAINT])dnl
])dnl

export IN_MAINT_MODE="${enable_maintainer_mode}"
AC_SUBST([IN_MAINT_MODE])dnl

m4_ifdef([AM_SANITY_CHECK],[
  (test -e "." && test -d "." && test -r ".") || (echo "error: sanity check failed!" >&2 && exit 1)
  if test -e "Makefile"; then
    AC_MSG_NOTICE([A Makefile is already present; removing it.])
    test -e "Makefile" && rm -f "Makefile"
  else
    AC_MSG_NOTICE([No Makefile present, the sanity check should pass next...])
  fi
  AM_SANITY_CHECK
])dnl

AX_CHECK_GNU_MAKE

AC_SYS_LARGEFILE
AC_SYS_LONG_FILE_NAMES

if test -e `pwd`/config.cache; then
  echo "hacking `pwd`/config.cache"
  sed -i.bak -e "s|ac_cv_env_CPP_set|bad_CPP_status|g" `pwd`/config.cache
  sed -i.bak -e "s|ac_cv_env_CPP_value|bad_CPP_value|g" `pwd`/config.cache
  sed -i.bak -e "s|ac_cv_prog_CPP|bad_CPP|g" `pwd`/config.cache
  sed -i.bak -e "s|ac_cv_env_CXX_set|bad_CXX_status|g" `pwd`/config.cache
  sed -i.bak -e "s|ac_cv_env_CXX_value|bad_CXX_value|g" `pwd`/config.cache
  sed -i.bak -e "s|ac_cv_prog_CXX|bad_CXX|g" `pwd`/config.cache
elif test -e ./config.cache; then
  echo "hacking ./config.cache"
  sed -i.bak -e "s|ac_cv_env_CPP_set|bad_CPP_status|g" ./config.cache
  sed -i.bak -e "s|ac_cv_env_CPP_value|bad_CPP_value|g" ./config.cache
  sed -i.bak -e "s|ac_cv_prog_CPP|bad_CPP|g" ./config.cache
  sed -i.bak -e "s|ac_cv_env_CXX_set|bad_CXX_status|g" ./config.cache
  sed -i.bak -e "s|ac_cv_env_CXX_value|bad_CXX_value|g" ./config.cache
  sed -i.bak -e "s|ac_cv_prog_CXX|bad_CXX|g" ./config.cache
else
  echo "config.cache is missing, which means we can skip attempts to hack it"
fi

# Checks for programs.
AM_PROG_AR
AC_PROG_CPP
AC_PROG_CXX
AC_DEFUN([AC_REQUIRE_GREPS],[
  AC_REQUIRE([AC_PROG_GREP])dnl
  AC_REQUIRE([AC_PROG_EGREP])dnl
  AC_REQUIRE([AC_PROG_FGREP])dnl
])dnl
AC_REQUIRE_GREPS
AC_DISABLE_SHARED
LT_INIT([shared static win32-dll])dnl
LT_LANG([C])dnl
dnl# temporarily set this while I update everything libtool-related:
LT_PREREQ([2.4.6])dnl
AM_PROG_INSTALL_STRIP
AC_PROG_INSTALL
AC_SUBST([INSTALLdir])dnl
AC_PROG_LN_S
AC_PROG_MKDIR_P
if test "x${RANLIB}" = "x"; then
  test -z "${RANLIB}" || unset RANLIB
  AC_PROG_RANLIB
else
  test -n "${RANLIB}" && export RANLIB="${RANLIB}"
fi
AC_DEFUN([AC_REQUIRE_PROG_SED],[
  AC_REQUIRE([AC_PROG_SED])dnl
])dnl
AC_REQUIRE_PROG_SED
AC_PATH_PROG([ICONV_BIN],[iconv])dnl
AC_SUBST([ICONV_BIN])dnl
AC_PROG_YACC
AX_PROG_BISON([
  # (might want to check the bison version first)
  EXTRA_YFLAGS_SEP=''
],[
  EXTRA_YFLAGS_SEP='#'
])dnl##end bison conditional
AC_SUBST([EXTRA_YFLAGS_SEP])dnl
AC_PROG_LEX([yywrap])
AM_PROG_LEX
AC_PATH_PROG([GIT_BIN],[git])dnl

# Checks for libraries.
AC_CHECK_LIB([c],[printf])dnl
AC_CHECK_LIB([gnu],[gl_dummy_symbol],[],[
  AC_CHECK_LIB([gnu],[main],[],[])dnl
])dnl
AC_CHECK_LIB([iberty],[xmalloc],[],[
  AC_CHECK_LIB([iberty],[main])dnl
])dnl

# Checks for header files.
AC_DEFUN([AC_REQUIRE_HEADER_STDC],[
  AC_REQUIRE([AC_HEADER_STDC])dnl
])dnl
AC_REQUIRE_HEADER_STDC
AC_HEADER_STAT
AC_HEADER_STDBOOL dnl# also "_CHECK"s it
AC_CHECK_HEADERS([argz.h bits/libc-lock.h byteswap.h ctype.h errno.h \
                  fcntl.h gcc/sys-types.h iconv.h io.h langinfo.h \
                  libiconv.h libintl.h limits.h locale.h malloc.h \
                  malloc/malloc.h nl_types.h php_iconv.h stddef.h \
                  stdio.h sys/malloc.h sys/mman.h sys/param.h \
                  uniconv.h wchar.h])dnl
AC_CHECK_HEADERS_ONCE([sys/types.h])dnl

# Checks for typedefs, structures, and compiler characteristics.
AC_C_BIGENDIAN
AC_C_CONST
AC_C_INLINE
AC_C_PROTOTYPES
AC_C_VOLATILE
AC_TYPE_OFF_T
AC_TYPE_SIZE_T
AC_TYPE_UINTMAX_T
AC_CHECK_TYPES([u_short])
AC_CHECK_TYPES([u_int64_t])
AC_CHECK_TYPES([iconv_t],[],[],[
#ifdef HAVE_ICONV_H
# include <iconv.h>
#else
# ifdef HAVE_PHP_ICONV_H
#  include <php_iconv.h>
# else
#  ifdef HAVE_UNICONV_H
#   include <uniconv.h>
#  endif /* HAVE_UNICONV_H */
# endif /* HAVE_PHP_ICONV_H */
#endif /* HAVE_ICONV_H */
])dnl

# Checks for compiler flags.
AC_MSG_CHECKING([compiler flags])
AC_MSG_RESULT([ ])
ACX_PROG_CC_WARNING_OPTS([-Wtraditional-conversion -Wnested-externs \
  -Wmissing-parameter-type -Wold-style-declaration -Wfatal-errors],
                         [EXTRA_ANSIFICATION_FLAGS])dnl

# Checks for library functions.
AM_GNU_GETTEXT([external],[need-ngettext])dnl
AM_GNU_GETTEXT_VERSION([0.21.1])dnl
AC_SUBST([GT_YES],['#'])dnl
AC_SUBST([GT_NO])dnl
AC_DEFUN([AC_GETTEXT_SUBCHECKS],[
  AC_REQUIRE([gl_EXTERN_INLINE])dnl
  AC_REQUIRE([gl_FCNTL_O_FLAGS])dnl
  AC_REQUIRE([gt_GLIBC2])dnl
  AC_REQUIRE([gl_GLIBC21])dnl
  AC_REQUIRE([gt_INTDIV0])dnl
  AC_REQUIRE([gt_INTL_MACOSX])dnl
  AC_REQUIRE([gt_TYPE_INTMAX_T])dnl
  AC_REQUIRE([gt_INTTYPES_PRI])dnl
  AC_REQUIRE([gl_AC_HEADER_INTTYPES_H])dnl
  AC_REQUIRE([gt_LC_MESSAGES])dnl
  AC_REQUIRE([AC_TYPE_LONG_LONG_INT])dnl
  AC_REQUIRE([gt_PRINTF_POSIX])dnl
  AC_REQUIRE([gl_SIZE_MAX])dnl
  AC_REQUIRE([gl_AC_HEADER_STDINT_H])dnl
  AC_REQUIRE([gl_AC_TYPE_UINTMAX_T])dnl
  AC_REQUIRE([gl_VISIBILITY])dnl
  AC_REQUIRE([gt_TYPE_WCHAR_T])dnl
  AC_REQUIRE([gt_TYPE_WINT_T])dnl
  AC_REQUIRE([gl_XSIZE])dnl
  AC_REQUIRE([AM_ICONV])dnl
  m4_ifdef([AM_LANGINFO_CODESET],[
    AC_REQUIRE([AM_LANGINFO_CODESET])dnl
  ],[
    AC_CHECK_FUNCS([langinfo codeset])dnl
  ])dnl

  dnl# libiconv symbol prefixing can vary:
  AC_CHECK_LIB([iconv],[iconv],[],[
    AC_CHECK_LIB([iconv],[libiconv],[],[
      AC_CHECK_LIB([iconv],[main],[],[])dnl
    ])dnl
  ])dnl
])dnl
AC_GETTEXT_SUBCHECKS
export DATADIRNAME=share
AC_SUBST([DATADIRNAME])
export MKINSTALLDIRS=../mkinstalldirs
AC_SUBST([MKINSTALLDIRS])dnl
AC_SUBST([l])dnl
AC_SUBST([INTLOBJS])
export USE_INCLUDED_LIBINTL=yes
AC_SUBST([USE_INCLUDED_LIBINTL])dnl
AM_WITH_DMALLOC
AC_FUNC_ALLOCA
AC_FUNC_MALLOC
AC_FUNC_MMAP
AC_FUNC_REALLOC
AC_FUNC_STRNLEN
AC_CHECK_FUNCS_ONCE([gettext _gettext])dnl
AC_CHECK_FUNCS([__argz_count __argz_next __argz_stringify \
                __dcgettext __dgettext dcgettext__ dgettext__ \
                errno free fgets getcwd getenv getwd \
                libintl_dcgettext libintl_dgettext memcpy \
                mempcpy munmap nl_langinfo putenv raise setenv \
                setlocale stpcpy strcasecmp strchr strdup strlen \
                strstr strtoul system tolower])dnl
AC_CHECK_DECLS([__dcgettext])dnl
AC_CHECK_DECLS([__dgettext])dnl
AC_CHECK_DECLS([dcgettext__])dnl
AC_CHECK_DECLS([dgettext__])dnl
AC_CHECK_DECLS([libintl_dcgettext])dnl
AC_CHECK_DECLS([libintl_dgettext])dnl
AC_CHECK_DECLS([free])dnl
AC_CHECK_DECLS([index])dnl
AC_CHECK_DECLS([NULL])dnl
AC_CHECK_DECLS([mempcpy])dnl
AC_CHECK_DECLS([stpcpy])dnl
AC_CHECK_DECLS([malloc])dnl
AC_CHECK_DECLS([realloc])dnl
AC_SEARCH_LIBS([rpl_realloc],[iberty c gnu])dnl
AC_SEARCH_LIBS([rpl_malloc],[iberty c gnu])dnl

# Output:
AC_DIAGNOSE([obsolete],[It is technically impossible to 'autoupdate'
cleanly from AC\_LINK\_FILES to AC_CONFIG_LINKS. 'autoupdate' provides a
functional but inelegant update, so you should probably tune the result
yourself.])dnl
ac_sources="${nls_cv_header_libgt}"
ac_dests="${nls_cv_header_intl}"
while test -n "${ac_sources}"; do
  set ${ac_dests}; ac_dest=$1; shift; ac_dests=$*
  set ${ac_sources}; ac_source=$1; shift; ac_sources=$*
  ac_config_links_1="${ac_config_links_1} ${ac_dest}:${ac_source}"
done
AC_CONFIG_LINKS([${ac_config_links_1}])
AH_TOP([
#ifndef _INCLUDING_CONFIG_H
# define _INCLUDING_CONFIG_H 1
#endif /* !_INCLUDING_CONFIG_H */
])dnl
dnl# This replaces the extensive use of DEFS in the original Makefile.in:
AC_DEFINE([IN_LIBINTL],[1],[Define because this is libintl.])dnl
AC_DEFINE([IN_LIBRARY],[1],[Define because this is a library.])dnl
AC_DEFINE([DEPENDS_ON_LIBICONV],[1],
          [Define to 1 because we depend on libiconv.])dnl
AC_DEFINE([ENABLE_RELOCATABLE],[1],[Define to enable relocation.])dnl
AC_DEFINE([NO_XMALLOC],[1],[Define if there is no xmalloc.])dnl
AC_DEFINE([set_relocation_prefix],[libintl_set_relocation_prefix],
          [Define this entry point correctly.])dnl
AC_DEFINE([relocate],[libintl_relocate],
          [Define this entry point correctly.])dnl

MISSING=missing
if test -d ${ac_aux_dir} && test -e ${ac_aux_dir}/missing; then
  MISSING=`cd ${ac_aux_dir} && ${PWDCMD-pwd}`/missing
elif test -d ${srcdir} -a -e ${srcdir}/missing; then
  MISSING=`cd ${srcdir} && ${PWDCMD-pwd}`/missing
elif test -d . -a -e ./missing; then
  MISSING=`cd . && ${PWDCMD-pwd}`/missing
elif test -d .. -a -e ../missing; then
  MISSING=`cd .. && ${PWDCMD-pwd}`/missing
elif test -d ../config -a -e ../config/missing; then
  MISSING=`cd ../config && ${PWDCMD-pwd}`/missing
fi
AC_CHECK_PROGS([ACLOCAL],[aclocal],[${MISSING} aclocal])
AC_CHECK_PROGS([AUTOCONF],[autoconf],[${MISSING} autoconf])
AC_CHECK_PROGS([AUTOHEADER],[autoheader],[${MISSING} autoheader])dnl

# Additional info for config.intl.
AC_SUBST([LIBINTL_DEP])dnl
AC_SUBST([INCINTL])dnl

LIBINTL_DEP=""
INCINTL=""
if test "x${USE_INCLUDED_LIBINTL}" != "x"; then
  test -n "${USE_INCLUDED_LIBINTL}" || test ! -z "${USE_INCLUDED_LIBINTL}"
  case ${USE_INCLUDED_LIBINTL} in
    yes)
      LIBINTL=`echo ${LIBINTL} | sed 's,${top_builddir},-L&/..,; s,\.\./intl/libintl\.a,../intl -lintl,' `
      ## might be ok to remove LTLIBINTL here, but I dunno:
      LTLIBINTL=`echo ${LTLIBINTL} | sed 's,${top_builddir},&/..,' `
      LIBINTL_DEP='${top_builddir}/../intl/libintl.a'
      INCINTL='-Wp,-I${top_builddir}/../intl'
      ;;
  esac
fi

# intl is sometimes linked into shared libraries even without --enable-shared
# (e.g. gdbsupport's inprocess agent): so always PICify, just in case.
GCC_PICFLAG
AC_SUBST([PICFLAG])dnl

BISON3_YES='#'
BISON3_NO=''
if test -n "${INTLBISON}" && test "${INTLBISON}" != :; then
  ac_bison3=no
  AC_MSG_CHECKING([bison 3 or later])
changequote(<<,>>)dnl
  ac_prog_version=`${INTLBISON} --version 2>&1 | sed -n 's/^.*GNU Bison.* \([0-9]*\.[0-9.]*\).*$/\1/p'`
  case ${ac_prog_version} in
    [3-9].*)
changequote([,])dnl
      ac_prog_version="${ac_prog_version}, bison3"; ac_bison3=yes;;
    *) ac_prog_version="${ac_prog_version}, old";;
  esac
  AC_MSG_RESULT([${ac_prog_version}])
  if test "x${ac_bison3}" = "xyes"; then
    BISON3_YES=''
    BISON3_NO='#'
  fi
fi
AC_SUBST([BISON3_YES])dnl
AC_SUBST([BISON3_NO])dnl

# Output commands.
AC_CONFIG_COMMANDS([libintl_h_compat],[
if test -e libgettext.h -a ! -e libintl.h; then
  ln -sv libgettext.h libintl.h
fi
if test "x${srcdir}" != "x."; then
  if test -e ${srcdir}/libgettext.h -a ! -e libintl.h; then
    ln -sv ${srcdir}/libgettext.h libintl.h
  fi
  if test -w ${srcdir}; then
    if test -e ${srcdir}/libgettext.h -a ! -e ${srcdir}/libintl.h; then
      ln -sv ${srcdir}/libgettext.h ${srcdir}/libintl.h
    fi
    if test -e libgettext.h -a ! -e ${srcdir}/libintl.h; then
      ln -sv libgettext.h ${srcdir}/libintl.h
    fi
  else
    # this can happen during distcheck, so check for that:
    if test "x${srcdir}" != "x" -a ! -w "${srcdir}"; then
      echo "warning: srcdir '${srcdir}' is unwritable"
    elif test "x${srcdir}" != "x.."; then
      echo "warning: unexpected srcdir '${srcdir}'"
    fi
  fi
fi
],[])dnl

AC_CONFIG_COMMANDS([config_status_vpath_build_fix],[
if test "x${srcdir}" != "x."; then
  if test -e ${srcdir}/config.status -a ! -e `pwd`/config.status; then
    cp -v ${srcdir}/config.status `pwd`/config.status || echo ""
  fi
  if test -e `pwd`/config.status -a ! -e ${srcdir}/config.status; then
    if test -w ${srcdir}; then
      cp -v `pwd`/config.status ${srcdir}/config.status || echo ""
    else
      # this can happen during distcheck, so check for that:
      if test "x${srcdir}" != "x" -a ! -w "${srcdir}"; then
        echo "warning: srcdir '${srcdir}' is unwritable"
      elif test "x${srcdir}" != "x.."; then
        echo "warning: unexpected srcdir '${srcdir}'"
      fi
    fi
  fi
fi
],[])dnl

dnl#FIXME: what is supposed to go in this variable?
AC_SUBST([POMAKEFILEDEPS])dnl

# Final output.
AC_CONFIG_FILES([Makefile \
                 Makefile_orig \
                 po/Makefile.in \
                 po/Makevars \
                 po2tbl.sed \
                 config.intl])dnl
AC_OUTPUT
