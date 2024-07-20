dnl#                                               -*- Autoconf -*-
dnl# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])dnl

# Autoconf initialization stuff.
AC_INIT([gdb-utils],[???],[/dev/null])
AC_CONFIG_SRCDIR([Makefile.in])
AC_CONFIG_AUX_DIR([..])
AM_SANITY_CHECK
m4_ifdef([AM_MAINTAINER_MODE],[AM_MAINTAINER_MODE])dnl
m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES([yes])])dnl

if test "x${V}" = "x"; then
  test -z "${V}" && export V=0
fi
AC_SUBST([V])dnl

# Checks for programs.
AC_PROG_SED
if test -e `pwd`/config.cache; then
  echo "hacking `pwd`/config.cache"
  sed -i.bak -e "s|ac_cv_env_CPP_set|bad_CPP_set|g" `pwd`/config.cache
  sed -i.bak -e "s|ac_cv_env_CPP_value|bad_CPP_value|g" `pwd`/config.cache
  sed -i.bak -e "s|ac_cv_prog_CPP|bad_CPP|g" `pwd`/config.cache
elif test -e ./config.cache; then
  echo "hacking ./config.cache"
  sed -i.bak -e "s|ac_cv_env_CPP_set|bad_CPP_set|g" ./config.cache
  sed -i.bak -e "s|ac_cv_env_CPP_value|bad_CPP_value|g" ./config.cache
  sed -i.bak -e "s|ac_cv_prog_CPP|bad_CPP|g" ./config.cache
else
  echo "config.cache is missing, which means we can skip attempts to hack it"
fi
AC_PROG_INSTALL
AC_CANONICAL_TARGET
AC_PROG_CC
AC_PROG_CPP
AC_PROG_CXX
AC_PROG_CXXCPP
m4_ifdef([AM_PROG_AR],[
  AM_PROG_AR
  if test "x${AR}" = "x" && test "x${ac_cv_prog_AR}" = "x"; then
    test -z "${AR}" || unset AR
    test -z "${ac_cv_prog_AR}" || unset ac_cv_prog_AR
    m4_ifdef([AC_PROG_AR],[
      AC_PROG_AR
    ],[
      AC_CHECK_TOOL([AR],[ar],[:])
    ])
  fi
],[
  if test "x${AR}" = "x"; then
    test -z "${AR}" || unset AR
    m4_ifdef([AC_PROG_AR],[
      AC_PROG_AR
    ],[
      AC_CHECK_TOOL([AR],[ar],[:])
    ])
  else
    test -n "${AR}" && export AR
  fi
  AC_SUBST([AR])dnl
])dnl
m4_ifdef([AM_PROG_AS],[
  AM_PROG_AS
],[
  test -n "${CCAS}"
  AC_SUBST([CCAS])dnl
])dnl
m4_ifdef([AM_MISSING_PROG],[
  AM_MISSING_PROG([AUTOCONF],[autoconf])dnl
])dnl

sinclude(../config/m4.m4)
AC_PROG_GNU_M4

# Checks for header files.
AC_CHECK_HEADERS([libc.h sys/param.h])dnl

if test "x${ac_cv_header_strings_h}" = "x"; then
  test -z "${ac_cv_header_strings_h}" || unset ac_cv_header_strings_h
  AC_CHECK_HEADERS([strings.h])
fi

# Checks for library functions.
AC_FUNC_LSTAT dnl# also checks whether it "_FOLLOWS_SLASHED_SYMLINK"
AC_CHECK_FUNCS([exit fopen fprintf getc getc_unlocked getwd getcwd \
                perror printf putchar putchar_unlocked strcat strcpy \
                unsetenv])dnl

# Autogen stuff:
sinclude(../config/autoopts.m4)
AG_PATH_AUTOOPTS([5.17])dnl

AC_MSG_CHECKING([if there are any subdirectories we should recurse into])
if test -z "${configdirs}"; then
  configdirs=""
fi
# Choose which subdirs to use:
if test -d ${srcdir}/spu; then
  configdirs="spu"
fi
if test -d ${srcdir}/misc; then
  configdirs="${configdirs} misc"
fi

dnl# begin m4-quoting, to allow the use of brackets in case labels:
[
case "${target}" in
  sparclite-*-* | simso-wrs* | sparc86x-*-elf*)
    if test -d ${srcdir}/sparclite; then
      configdirs="${configdirs} sparclite"
    fi
    ;;
  *mips*-*-pe* | *sh*-*-pe* | *arm*-*-wince | *arm*-wince-pe* | \
  *arm*-*-mingw32ce* | *arm*-*-cegcc*)
    if test -d ${srcdir}/wince; then
      configdirs="${configdirs} wince"
    fi
    ;;
  *-*-msdosdjgpp* | *-moss-msdos | i[3-7]86-*-msdos | i[3-7]86-*-moss* | \
  *-coff-go32* | x86_64-*-rdos*)
    if test -d ${srcdir}/msdos; then
      configdirs="${configdirs} msdos"
    fi
    ;;
  a29k-*-udi* | udi29k | a29k-*-vxworks* | a29k-none-* | a29k-*-ebmon* | \
  a29k-*-coff* | a29k-*-sym1* | a29k-*-sysv* | a29k-*-aout* | a29k-*-bsd* | \
  a29k-*-vsta*)
    if test -d ${srcdir}/amd-udi; then
      configdirs="${configdirs} amd-udi"
    fi
    ;;
  *)
    # (nothing)
    ;;
esac
]
dnl# end m4-quoting
AC_MSG_RESULT([${configdirs}])

all_subdirs=""
install_subdirs=""
mostlyclean_subdirs=""
clean_subdirs=""
distclean_subdirs=""
maintainer_clean_subdirs=""
check_subdirs=""

for dir in ${configdirs}; do
    all_subdirs="${all_subdirs} all-${dir}"
    install_subdirs="${install_subdirs} install-${dir}"
    mostlyclean_subdirs="${mostlyclean_subdirs} mostlyclean-${dir}"
    clean_subdirs="${clean_subdirs} clean-${dir}"
    distclean_subdirs="${distclean_subdirs} distclean-${dir}"
    maintainer_clean_subdirs="${maintainer_clean_subdirs} maintainer-clean-${dir}"
    check_subdirs="${check_subdirs} check-${dir}"
done

AC_SUBST([all_subdirs])dnl
AC_SUBST([install_subdirs])dnl
AC_SUBST([mostlyclean_subdirs])dnl
AC_SUBST([clean_subdirs])dnl
AC_SUBST([distclean_subdirs])dnl
AC_SUBST([maintainer_clean_subdirs])dnl
AC_SUBST([check_subdirs])dnl

case "${srcdir}" in
  [[\\/]]* | ?:[[\\/]]* ) # Absolute
    abs_srcdir=${srcdir}
    ;;
  *) 
    abs_srcdir="../${srcdir}"
    ;;
esac
AC_SUBST([abs_srcdir])dnl

case "${cache_file}" in
  [[\\/]]* | ?:[[\\/]]* ) # Absolute
    abs_cache_file=${cache_file}
    ;;
  *) 
    abs_cache_file="../${cache_file}"
    ;;
esac
subconfigure_args=${ac_configure_args}
subconfigure_args="${subconfigure_args} --disable-option-checking --cache-file=${abs_cache_file} --srcdir=${abs_srcdir}"
AC_SUBST([subconfigure_args])dnl

# Output.
AC_CONFIG_FILES([Makefile \
                 decomment.tproj/Makefile \
                 relpath.tproj/Makefile])dnl
AC_OUTPUT

