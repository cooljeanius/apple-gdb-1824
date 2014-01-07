dnl#                                               -*- Autoconf -*-
dnl# Process this file with autoconf to produce a configure script.
dnl#

AC_PREREQ([2.57])
dnl# keep version number synced with the one in ../bfd
AC_INIT([binutils],[2.16.91],[jmolenda@apple.com])
dnl# bug report address is email address of last person to touch this
dnl# directory, according to the Changelog-Apple in this directory
AC_CONFIG_SRCDIR([ar.c])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([..])

AC_CANONICAL_TARGET
AC_SEARCH_LIBS([strerror],[cposix])
AC_USE_SYSTEM_EXTENSIONS

# Automake
if test -f ${srcdir}/config.status; then
  AC_MSG_NOTICE([config.status is already present, removing it.])
  rm -f ${srcdir}/config.status
fi
AM_INIT_AUTOMAKE([1.9.6 gnu dejagnu])
AM_SANITY_CHECK

AC_PROG_SED

if test -e ./config.cache; then
	sed -i "s|ac_cv_env_CFLAGS_value|bad_CFLAGS|g" ./config.cache
	sed -i "s|ac_cv_env_CXXFLAGS_value|bad_CXXFLAGS|g" ./config.cache
	sed -i "s|ac_cv_exeext|bad_exeext|g" ./config.cache
fi

# Libtool checks
AC_PROG_CXX
LT_INIT([disable-fast-install disable-shared dlopen static win32-dll])
LT_LANG([C])
LT_LANG([C++])
LT_LANG([Windows Resource])

AC_ARG_ENABLE([targets],
[AS_HELP_STRING([--enable-targets],[alternative target configurations])],
[case "${enableval}" in
  yes | "") AC_MSG_ERROR([enable-targets option must specify target names or 'all'])
            ;;
  no)       enable_targets= ;;
  *)        enable_targets=$enableval ;;
esac])dnl
AC_ARG_ENABLE([commonbfdlib],
[AS_HELP_STRING([--enable-commonbfdlib],[build shared BFD/opcodes/libiberty library])],
[case "${enableval}" in
  yes) commonbfdlib=true ;;
  no)  commonbfdlib=false ;;
  *)   AC_MSG_ERROR([bad value ${enableval} for BFD commonbfdlib option]) ;;
esac])dnl

AM_BINUTILS_WARNINGS
		   
AC_CONFIG_HEADERS([config.h])
AH_TOP([
#ifndef HAVE_CONFIG_H
# define HAVE_CONFIG_H
#endif /* !HAVE_CONFIG_H */
])

# Checks for systems.
if test -z "${target}" ; then
    AC_MSG_ERROR([Unrecognized target system type; please check config.sub.])
fi
if test -z "${host}" ; then
    AC_MSG_ERROR([Unrecognized host system type; please check config.sub.])
fi
AC_SYS_INTERPRETER
AC_SYS_LARGEFILE

# Checks for programs.
AM_PROG_AS
if test "x${CC}" = "x"; then
    test -z "${CC}"
    AC_PROG_CC
else
    test ! -z "${CC}"
    AC_PROG_GCC_TRADITIONAL
fi

AC_PROG_YACC
AM_PROG_LEX
AC_PATH_PROG([ICONV_BIN],[iconv])
AC_SUBST([ICONV_BIN])

ALL_LINGUAS="fr tr ja es sv da zh_CN ru ro rw zh_TW"
CY_GNU_GETTEXT

AM_MAINTAINER_MODE
AC_OBJEXT
AC_EXEEXT
if test -n "${EXEEXT}"; then
  AC_DEFINE([HAVE_EXECUTABLE_SUFFIX],[1],
	    [Does the platform use an executable suffix?])
fi
AC_DEFINE_UNQUOTED([EXECUTABLE_SUFFIX],["${EXEEXT}"],
		   [Suffix used for executables, if any.])

# host-specific stuff:

HDEFINES=

AC_MSG_NOTICE([sourcing ../bfd/configure.host])
. ${srcdir}/../bfd/configure.host

AC_SUBST([HDEFINES])

# More programs:
AR=${AR-ar}
AC_SUBST([AR])
if test "x${RANLIB}" = "x"; then
	test -z "${RANLIB}"
	AC_PROG_RANLIB
else
	test ! -z "${RANLIB}"
fi
AC_PROG_INSTALL
AM_PROG_INSTALL_STRIP
AC_PROG_LN_S

AC_DEFUN([AM_REQUIRE_EXTRA_AUTOMAKE_CHECKS],[
  AC_PREREQ([2.65])
  dnl# Autoconf wants to disallow AM_ names. We explicitly allow
  dnl# the ones we care about.
  m4_pattern_allow([^AM_[A-Z]+FLAGS$])dnl
  AC_REQUIRE([AM_DEP_TRACK])dnl
  AC_REQUIRE([AM_OUTPUT_DEPENDENCY_COMMANDS])dnl
  AC_REQUIRE([AM_MISSING_HAS_RUN])dnl
  AC_REQUIRE([AM_MAINTAINER_MODE])dnl
  AC_REQUIRE([AC_PROG_GREP])dnl
  AC_REQUIRE([AM_MAKE_INCLUDE])dnl
  AC_REQUIRE([AM_SANITY_CHECK])dnl
  AC_REQUIRE([AC_PROG_AWK])dnl
  AC_REQUIRE([AC_PROG_CPP])dnl
])
AM_REQUIRE_EXTRA_AUTOMAKE_CHECKS

BFD_CC_FOR_BUILD

DEMANGLER_NAME=c++filt
case "${host}" in
  *-*-go32* | *-*-msdos*)
    DEMANGLER_NAME=cxxfilt
esac
AC_SUBST([DEMANGLER_NAME])

# Checks for libraries.
LT_LIB_M
AC_SEARCH_LIBS([dcgettext],[intl])
AC_SEARCH_LIBS([bindtextdomain],[intl])
AC_SEARCH_LIBS([textdomain],[intl])
AC_CHECK_LIB([intl],[gettext],[],[
  AC_CHECK_LIB([intl],[main],[],[])
])
AC_DEFUN([AC_GETTEXT_SUBCHECKS],[
  AC_REQUIRE([gt_INTL_MACOSX])
  AC_REQUIRE([AM_ICONV])
  AC_CHECK_LIB([iconv],[iconv],[],[
    AC_CHECK_LIB([iconv],[libiconv],[],[
      AC_CHECK_LIB([iconv],[main],[],[])
    ])
  ])
])
AC_GETTEXT_SUBCHECKS
dnl# I forget, why am I not just using AM\_GNU\_GETTEXT?
dnl# Anyways... continuing with library checks:
AC_CHECK_LIB([c],[printf])
# FIXME: Replace `main' with a function in `-lcc_dynamic':
AC_CHECK_LIB([cc_dynamic],[main])
# FIXME: Replace `main' with a function in `-lgcc':
AC_CHECK_LIB([gcc],[main])
# FIXME: Replace `main' with a function in `-lcurses':
AC_CHECK_LIB([curses],[main])
# FIXME: Replace `main' with a function in `-lhistory':
AC_CHECK_LIB([history],[main])
# FIXME: Replace `main' with a function in `-lreadline':
AC_CHECK_LIB([readline],[main])
# FIXME: Replace `main' with a function in `-liberty':
AC_CHECK_LIB([iberty],[main])
# FIXME: Replace `main' with a function in `-lgnu':
AC_CHECK_LIB([gnu],[main])

# Checks for header files.
AC_HEADER_STDBOOL dnl# calls AC_CHECK_HEADER_STDBOOL
if test -d ../intl; then
  if test -z "${CPPFLAGS}"; then
    export CPPFLAGS="-I../intl"
  else
    export CPPFLAGS="${CPPFLAGS} -I../intl"
  fi
elif test -d ../libintl; then
  if test -z "${CPPFLAGS}"; then
    export CPPFLAGS="-I../libintl"
  else
    export CPPFLAGS="${CPPFLAGS} -I../libintl"
  fi
fi
unset ac_cv_header_libintl_h
unset ac_cv_header_limits_h
unset ac_cv_header_locale_h
unset ac_cv_header_malloc_h
unset ac_cv_header_stdlib_h
unset ac_cv_header_sys_param_h
unset ac_cv_header_unistd_h
AC_CHECK_HEADERS([assert.h errno.h fcntl.h gettext.h iconv.h langinfo.h \
                  libgettext.h libintl.h limits.h locale.h mach/mach.h \
                  malloc.h malloc/malloc.h stdarg.h stddef.h stdio.h \
                  sys/file.h sys/param.h sys/time.h utime.h \
                  varargs.h wchar.h wctype.h])
AC_CHECK_HEADERS_ONCE([time.h])
AC_HEADER_SYS_WAIT

# Checks for typedefs, structures, and compiler characteristics.
AC_C_BIGENDIAN
AC_C_PROTOTYPES
AC_C_RESTRICT
AC_CHECK_TYPES([FILE])
AC_TYPE_INT8_T
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_SSIZE_T
AC_TYPE_UINT8_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_CHECK_TYPES([iconv_t])

# Checks for library functions.
AM_WITH_DMALLOC
AC_FUNC_ALLOCA
AC_FUNC_CHOWN
AC_FUNC_ERROR_AT_LINE
AC_FUNC_FORK
AC_FUNC_LSTAT dnl# calls the _FOLLOWS_SLASHED_SYMLINK version of it
AC_FUNC_MALLOC
AC_FUNC_OBSTACK
AC_FUNC_REALLOC
AC_FUNC_STRCOLL
AC_FUNC_STRNLEN
unset ac_cv_func_setlocale
AC_CHECK_FUNCS([bindtextdomain ctime dcgettext dup2 fclose fopen \
                fprintf fputs fread free getc_unlocked gettimeofday \
                memchr memcmp memcpy memmove memset re_comp regcomp \
                rmdir sbrk setlocale setmode strdup strerror strncasecmp \
                strrchr strspn strstr strtol strtoul strverscmp \
                textdomain utime utimes])
AC_SEARCH_LIBS([rpl_realloc],[iberty c gnu])
AC_SEARCH_LIBS([rpl_malloc],[iberty c gnu])

# Check whether fopen64 is available and whether _LARGEFILE64_SOURCE
# needs to be defined for it
AC_MSG_CHECKING([for fopen64])
AC_CACHE_VAL([bu_cv_have_fopen64],
[AC_LINK_IFELSE([AC_LANG_PROGRAM([[#include <stdio.h>]],[[FILE *f = fopen64 ("/tmp/foo","r");]])],[bu_cv_have_fopen64=yes],[saved_CPPFLAGS=$CPPFLAGS
 CPPFLAGS="$CPPFLAGS -D_LARGEFILE64_SOURCE"
 AC_LINK_IFELSE([AC_LANG_PROGRAM([[#include <stdio.h>]],[[FILE *f = fopen64 ("/tmp/foo","r");]])],
[bu_cv_have_fopen64="need -D_LARGEFILE64_SOURCE"],
[bu_cv_have_fopen64=no])
 CPPFLAGS=${saved_CPPFLAGS}])])
AC_MSG_RESULT([${bu_cv_have_fopen64}])
if test "${bu_cv_have_fopen64}" != no; then
  AC_DEFINE([HAVE_FOPEN64],[1],
	    [Is fopen64 available?])
fi
AC_MSG_CHECKING([for stat64])
AC_CACHE_VAL([bu_cv_have_stat64],
[AC_LINK_IFELSE([AC_LANG_PROGRAM([[#include <sys/stat.h>]],[[struct stat64 st; stat64 ("/tmp/foo", &st);]])],[bu_cv_have_stat64=yes],[saved_CPPFLAGS=$CPPFLAGS
 CPPFLAGS="$CPPFLAGS -D_LARGEFILE64_SOURCE"
 AC_LINK_IFELSE([AC_LANG_PROGRAM([[#include <sys/stat.h>]],[[struct stat64 st; stat64 ("/tmp/foo", &st);]])],
[bu_cv_have_stat64="need -D_LARGEFILE64_SOURCE"],
[bu_cv_have_stat64=no])
 CPPFLAGS=$saved_CPPFLAGS])])
AC_MSG_RESULT([${bu_cv_have_stat64}])
if test "${bu_cv_have_stat64}" != no; then
  AC_DEFINE([HAVE_STAT64],[1],
	    [Is stat64 available?])
fi
if test "${bu_cv_have_fopen64}" = "need -D_LARGEFILE64_SOURCE" \
   || test "${bu_cv_have_stat64}" = "need -D_LARGEFILE64_SOURCE"; then
  AC_DEFINE([_LARGEFILE64_SOURCE],[1],
	    [Enable LFS])
  CPPFLAGS="${CPPFLAGS} -D_LARGEFILE64_SOURCE"
fi

# Some systems have frexp only in -lm, not in -lc.
AC_SEARCH_LIBS([frexp],[m])
LT_LIB_M

AC_MSG_CHECKING([for time_t in time.h])
AC_CACHE_VAL([bu_cv_decl_time_t_time_h],
[AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[#include <time.h>]],[[time_t i;]])],[bu_cv_decl_time_t_time_h=yes],[bu_cv_decl_time_t_time_h=no])])
AC_MSG_RESULT([${bu_cv_decl_time_t_time_h}])
if test ${bu_cv_decl_time_t_time_h} = yes; then
  AC_DEFINE([HAVE_TIME_T_IN_TIME_H],[1],
	    [Is the type time_t defined in <time.h>?])
fi

AC_MSG_CHECKING([for time_t in sys/types.h])
AC_CACHE_VAL([bu_cv_decl_time_t_types_h],
[AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[#include <sys/types.h>]],[[time_t i;]])],[bu_cv_decl_time_t_types_h=yes],[bu_cv_decl_time_t_types_h=no])])
AC_MSG_RESULT([${bu_cv_decl_time_t_types_h}])
if test ${bu_cv_decl_time_t_types_h} = yes; then
  AC_DEFINE([HAVE_TIME_T_IN_TYPES_H],[1],
	    [Is the type time_t defined in <sys/types.h>?])
fi

AC_MSG_CHECKING([for a known getopt prototype in unistd.h])
AC_CACHE_VAL([bu_cv_decl_getopt_unistd_h],
[AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[#include <unistd.h>]],[[extern int getopt (int, char *const*, const char *);]])],[bu_cv_decl_getopt_unistd_h=yes],[bu_cv_decl_getopt_unistd_h=no])])
AC_MSG_RESULT([${bu_cv_decl_getopt_unistd_h}])
if test ${bu_cv_decl_getopt_unistd_h} = yes; then
  AC_DEFINE([HAVE_DECL_GETOPT],[1],
	    [Is the prototype for getopt in <unistd.h> in the expected format?])
fi

# Under Next 3.2 <utime.h> apparently does not define struct utimbuf
# by default.
AC_MSG_CHECKING([for a good utime.h])
AC_CACHE_VAL([bu_cv_header_utime_h],
[AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[#include <sys/types.h>
#ifdef HAVE_TIME_H
# include <time.h>
#else
# warning this conftest expects <time.h> to be included.
#endif /* HAVE_TIME_H */
#include <utime.h>]],[[struct utimbuf s;]])],[bu_cv_header_utime_h=yes],[bu_cv_header_utime_h=no])])
AC_MSG_RESULT([${bu_cv_header_utime_h}])
if test ${bu_cv_header_utime_h} = yes; then
  AC_DEFINE([HAVE_GOOD_UTIME_H],[1],[Does <utime.h> define struct utimbuf?])
fi

AC_CHECK_DECLS([fprintf, strstr, sbrk, getenv, environ, getc_unlocked])

BFD_BINARY_FOPEN

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

all_targets=false
BUILD_NLMCONV=
NLMCONV_DEFS=
BUILD_SRCONV=
BUILD_DLLTOOL=
DLLTOOL_DEFS=
BUILD_WINDRES=
BUILD_DLLWRAP=
BUILD_MISC=
OBJDUMP_DEFS=

for targ in ${target} ${canon_targets}
do
    if test "x${targ}" = "xall"; then
        all_targets=true
	BUILD_NLMCONV='$(NLMCONV_PROG)$(EXEEXT)'
	BUILD_SRCONV='$(SRCONV_PROG)'
	NLMCONV_DEFS="-DNLMCONV_I386 -DNLMCONV_ALPHA -DNLMCONV_POWERPC -DNLMCONV_SPARC"
    else
	case $targ in
changequote(,)dnl
	i[3-7]86*-*-netware*) 
changequote([,])dnl
	  BUILD_NLMCONV='$(NLMCONV_PROG)$(EXEEXT)'
	  NLMCONV_DEFS="$NLMCONV_DEFS -DNLMCONV_I386"
	  ;;
	alpha*-*-netware*)
	  BUILD_NLMCONV='$(NLMCONV_PROG)$(EXEEXT)'
	  NLMCONV_DEFS="$NLMCONV_DEFS -DNLMCONV_ALPHA"
	  ;;
	powerpc*-*-netware*)
	  BUILD_NLMCONV='$(NLMCONV_PROG)$(EXEEXT)'
	  NLMCONV_DEFS="$NLMCONV_DEFS -DNLMCONV_POWERPC"
	  ;;
	sparc*-*-netware*)
	  BUILD_NLMCONV='$(NLMCONV_PROG)$(EXEEXT)'
	  NLMCONV_DEFS="$NLMCONV_DEFS -DNLMCONV_SPARC"
	  ;;
	esac
	case $targ in
	*-*-hms*) BUILD_SRCONV='$(SRCONV_PROG)' ;;
	esac
	case $targ in
	arm-epoc-pe*)
  	  BUILD_DLLTOOL='$(DLLTOOL_PROG)$(EXEEXT)'
	  DLLTOOL_DEFS="$DLLTOOL_DEFS -DDLLTOOL_ARM_EPOC -DDLLTOOL_ARM"
	  BUILD_WINDRES='$(WINDRES_PROG)$(EXEEXT)'
	  ;;
	arm-*-pe* | arm-*-wince)
  	  BUILD_DLLTOOL='$(DLLTOOL_PROG)$(EXEEXT)'
	  DLLTOOL_DEFS="$DLLTOOL_DEFS -DDLLTOOL_ARM"
	  BUILD_WINDRES='$(WINDRES_PROG)$(EXEEXT)'
	  ;;
	thumb-*-pe*)
  	  BUILD_DLLTOOL='$(DLLTOOL_PROG)$(EXEEXT)'
	  DLLTOOL_DEFS="$DLLTOOL_DEFS -DDLLTOOL_ARM"
	  BUILD_WINDRES='$(WINDRES_PROG)$(EXEEXT)'
	  ;;
  	arm*-* | xscale-* | strongarm-* | d10v-*)
	  OBJDUMP_DEFS="-DDISASSEMBLER_NEEDS_RELOCS"
	  ;;
changequote(,)dnl
	i[3-7]86-*-pe* | i[3-7]86-*-cygwin* | i[3-7]86-*-mingw32** | i[3-7]86-*-netbsdpe*)
changequote([,])dnl
  	  BUILD_DLLTOOL='$(DLLTOOL_PROG)$(EXEEXT)'
	  DLLTOOL_DEFS="$DLLTOOL_DEFS -DDLLTOOL_I386"
	  BUILD_WINDRES='$(WINDRES_PROG)$(EXEEXT)'
	  BUILD_DLLWRAP='$(DLLWRAP_PROG)$(EXEEXT)'
	  ;;
changequote(,)dnl
	i[3-7]86-*-interix)
changequote([,])dnl
	  BUILD_DLLTOOL='$(DLLTOOL_PROG)'
	  DLLTOOL_DEFS="$DLLTOOL_DEFS -DDLLTOOL_I386"
	  ;;
changequote(,)dnl
	powerpc*-aix5.[01])
changequote([,])dnl
	  ;;
	powerpc*-aix5.*)
	  OBJDUMP_DEFS="-DAIX_WEAK_SUPPORT"
	  ;;
	powerpc*-*-pe* | powerpc*-*-cygwin*)
  	  BUILD_DLLTOOL='$(DLLTOOL_PROG)$(EXEEXT)'
	  DLLTOOL_DEFS="$DLLTOOL_DEFS -DDLLTOOL_PPC"
	  BUILD_WINDRES='$(WINDRES_PROG)$(EXEEXT)'
	  ;;
	sh*-*-pe)
  	  BUILD_DLLTOOL='$(DLLTOOL_PROG)$(EXEEXT)'
	  DLLTOOL_DEFS="$DLLTOOL_DEFS -DDLLTOOL_SH"
	  BUILD_WINDRES='$(WINDRES_PROG)$(EXEEXT)'
	  ;;
	mips*-*-pe)
  	  BUILD_DLLTOOL='$(DLLTOOL_PROG)$(EXEEXT)'
	  DLLTOOL_DEFS="$DLLTOOL_DEFS -DDLLTOOL_MIPS"
	  BUILD_WINDRES='$(WINDRES_PROG)$(EXEEXT)'
	  ;;
	mcore-*-pe)
  	  BUILD_DLLTOOL='$(DLLTOOL_PROG)$(EXEEXT)'
	  DLLTOOL_DEFS="$DLLTOOL_DEFS -DDLLTOOL_MCORE"
	  BUILD_WINDRES='$(WINDRES_PROG)$(EXEEXT)'
	  ;;
	mcore-*-elf)
  	  BUILD_DLLTOOL='$(DLLTOOL_PROG)$(EXEEXT)'
	  DLLTOOL_DEFS="$DLLTOOL_DEFS -DDLLTOOL_MCORE_ELF"
	  ;;
	esac
    fi
done

if test "${with_windres+set}" = set; then
	  BUILD_WINDRES='$(WINDRES_PROG)$(EXEEXT)'
fi

AC_SUBST([NLMCONV_DEFS])
AC_SUBST([BUILD_NLMCONV])
AC_SUBST([BUILD_SRCONV])
AC_SUBST([BUILD_DLLTOOL])
AC_SUBST([DLLTOOL_DEFS])
AC_SUBST([BUILD_WINDRES])
AC_SUBST([BUILD_DLLWRAP])
AC_SUBST([BUILD_MISC])
AC_SUBST([OBJDUMP_DEFS])

AC_DEFINE_UNQUOTED([TARGET],["${target}"],[Configured target name.])

targ=${target}
AC_MSG_NOTICE([sourcing ../bfd/config.bfd])
. ${srcdir}/../bfd/config.bfd
if test "x${targ_underscore}" = "xyes"; then
    UNDERSCORE=1
else
    UNDERSCORE=0
fi
AC_DEFINE_UNQUOTED([TARGET_PREPENDS_UNDERSCORE],[${UNDERSCORE}],
 [Define to 1 if user symbol names have a leading underscore, 0 if not.])

# Emulation 
for targ_alias in `echo ${target} ${enable_targets} | sed 's/,/ /g'`
do
  # Canonicalize the secondary target names.
  result=`${ac_config_sub} ${targ_alias} 2>/dev/null`
  if test -n "${result}"; then
    targ=${result}
  else
    targ=${targ_alias}
  fi

  AC_MSG_NOTICE([sourcing ./configure.tgt for ${targ}])
  . ${srcdir}/configure.tgt

  export EMULATION=${targ_emul}
  export EMULATION_VECTOR=${targ_emul_vector}
  AC_MSG_NOTICE([EMULATION is ${EMULATION}])
  AC_MSG_NOTICE([EMULATION_VECTOR is ${EMULATION_VECTOR}])	
done

AC_SUBST([EMULATION])
AC_SUBST([EMULATION_VECTOR])
AC_SUBST([INTLLIBS])
AC_SUBST([INTLDEPS])
AC_SUBST([SYSTEM_FRAMEWORK_LIBS])

AC_CONFIG_FILES([Makefile \
                 doc/Makefile \
                 po/Makefile.in:po/Make-in])

AC_CONFIG_COMMANDS([default],[
case "x${CONFIG_FILES}" in
*) sed -e '/POTFILES =/r po/POTFILES' po/Makefile.in > po/Makefile ;;
esac
],[])

AC_OUTPUT

if test "${srcdir}" != "."; then
  if test -e ${srcdir}/config.status -a ! -e `pwd`/config.status; then
    cp -v ${srcdir}/config.status `pwd`/config.status || echo ""
  elif test -e `pwd`/config.status -a ! -e ${srcdir}/config.status; then
    cp -v `pwd`/config.status ${srcdir}/config.status || echo ""
  fi
fi
