#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])

AC_INIT([gdb-utils],[???],[/dev/null])
AC_CONFIG_SRCDIR([Makefile.in])
AC_CONFIG_AUX_DIR([..])
AM_SANITY_CHECK

# Checks for programs.
AC_PROG_INSTALL
AC_CANONICAL_TARGET
AC_PROG_CC

# Checks for header files.
AC_CHECK_HEADERS([libc.h sys/param.h])

# Checks for library functions.
AC_FUNC_LSTAT dnl# also checks whether it "_FOLLOWS_SLASHED_SYMLINK"
AC_CHECK_FUNCS([exit fopen fprintf getc getc_unlocked getwd getcwd \
                perror printf putchar putchar_unlocked strcat strcpy \
                unsetenv])

# Autogen
sinclude(../config/autoopts.m4)
AG_PATH_AUTOOPTS([5.17.4])

if test -d ${srcdir}/spu ; then
  configdirs="spu"
fi
if test -d ${srcdir}/misc ; then
  configdirs="${configdirs} misc"
fi

case "${target}" in
  sparclite-*-*)
    if test -d ${srcdir}/sparclite ; then
      configdirs="${configdirs} sparclite"
    fi
    ;;
  *mips*-*-pe* | *sh*-*-pe* | *arm*-*-wince | *arm*-wince-pe* )
    if test -d ${srcdir}/wince ; then
      configdirs="${configdirs} wince"
    fi
    ;;
  *-*-msdosdjgpp* | *-moss-msdos)
    if test -d ${srcdir}/msdos ; then
      configdirs="${configdirs} msdos"
    fi
    ;;
  a29k-amd-udi | udi29k | a29k-*-vxworks | a29k-none-*)
    if test -d ${srcdir}/amd-udi ; then
      configdirs="${configdirs} amd-udi"
    fi
    ;;
esac

all_subdirs=""
install_subdirs=""
mostlyclean_subdirs=""
clean_subdirs=""
distclean_subdirs=""
maintainer_clean_subdirs=""

for dir in ${configdirs} ; do
    all_subdirs="${all_subdirs} all-${dir}"
    install_subdirs="${install_subdirs} install-${dir}"
    mostlyclean_subdirs="${mostlyclean_subdirs} mostlyclean-${dir}"
    clean_subdirs="${clean_subdirs} clean-${dir}"
    distclean_subdirs="${distclean_subdirs} distclean-${dir}"
    maintainer_clean_subdirs="${maintainer_clean_subdirs} maintainer-clean-${dir}"
done

AC_SUBST([all_subdirs])
AC_SUBST([install_subdirs])
AC_SUBST([mostlyclean_subdirs])
AC_SUBST([clean_subdirs])
AC_SUBST([distclean_subdirs])
AC_SUBST([maintainer_clean_subdirs])

case "${srcdir}" in
  [[\\/]]* | ?:[[\\/]]* ) # Absolute
    abs_srcdir=${srcdir}
    ;;
  *) 
    abs_srcdir="../${srcdir}"
    ;;
esac
AC_SUBST([abs_srcdir])

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
AC_SUBST([subconfigure_args])

AC_CONFIG_FILES([Makefile \
                 decomment.tproj/Makefile \
                 relpath.tproj/Makefile])
AC_OUTPUT

