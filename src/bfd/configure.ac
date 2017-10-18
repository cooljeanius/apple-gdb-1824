dnl#                                               -*- Autoconf -*-
dnl# Process this file with autoconf to produce a configure script.
dnl#

AC_PREREQ([2.59])
m4_include([version.m4])
m4_include([../config/warnings.m4])
AC_INIT([bfd],[2.16.91],[jingham@apple.com])
dnl# bug report address is email address of last person to touch this
dnl# directory, according to the Changelog-Apple in this directory.
AC_CONFIG_SRCDIR([libbfd.c])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])dnl

# target stuff:
AC_CANONICAL_TARGET
AC_SEARCH_LIBS([strerror],[cposix]) dnl# used to be AC\_ISC\_POSIX
if test -z "${TARGET_ARCHITECTURE}" -a -n "${target_cpu}"; then
  export TARGET_ARCHITECTURE="${target_cpu}"
  AC_SUBST([TARGET_ARCHITECTURE])dnl
  ## I guess target is first in this subdir because it is more important...
fi

# host stuff:
if test "x${CC}" = "x"; then
  test -z "${CC}"
   AC_PROG_CC
else
  test ! -z "${CC}"
  AC_PROG_GCC_TRADITIONAL
fi
AC_USE_SYSTEM_EXTENSIONS
if test -z "${HOST_ARCHITECTURE}" -a -n "${host_cpu}"; then
  export HOST_ARCHITECTURE="${host_cpu}"
  AC_SUBST([HOST_ARCHITECTURE])dnl
  ## might also need build; do that next:
  if test -z "${BUILD_ARCHITECTURE}" -a -n "${build_cpu}"; then
    export BUILD_ARCHITECTURE="${build_cpu}"
    AC_SUBST([BUILD_ARCHITECTURE])dnl
    ## (just in case)
  fi
fi
if test -n "${HOST_ARCHITECTURE}"; then
  ACX_PROG_CC_WARNING_OPTS(["-arch ${HOST_ARCHITECTURE}"],[C_ARCHFLAGS])
  AC_SUBST([C_ARCHFLAGS])
fi

# Automake:
AC_MSG_CHECKING([automake things])
AC_MSG_RESULT([ ])
if test -f ${srcdir}/config.status; then
  AC_MSG_NOTICE([${srcdir}/config.status is already present, removing it.])
  rm -f ${srcdir}/config.status
fi
AM_INIT_AUTOMAKE([1.11.6 no-dist gnits dejagnu -Wall -Wno-portability])dnl
  ## maintainer mode is used kind of hackishly in the Makefiles here:
AM_MAINTAINER_MODE
AM_PROG_INSTALL_STRIP
AM_SANITY_CHECK

dnl# These must be called before LT_INIT, because it may want
dnl# to call AC_CHECK_PROG.
AC_MSG_CHECKING([programs])
AC_MSG_RESULT([ ])
AM_PROG_AS
AM_PROG_AR
if test "x${AR}" = "x"; then
  test -z "${AR}"
  AC_CHECK_TOOLS([AR],[ar gar])dnl
  ## might want to switch around these checks...
else
  test ! -z "${AR}"
fi
AC_CHECK_TOOLS([RANLIB],[ranlib granlib],[:])dnl
  ## keep autoscan happy:
if test "x${RANLIB}" = "x"; then
  test -z "${RANLIB}"
  AC_PROG_RANLIB
else
  test ! -z "${RANLIB}"
fi
AC_PATH_PROG([SANDBOX_EXEC],[sandbox-exec],[],[/usr/bin])dnl
AC_PATH_PROGS([MAKE_BIN],[make gmake gnumake remake "xcrun make"])
if test -n "${SANDBOX_EXEC}" -a -n "${MAKE_BIN}"; then
  if test -e `pwd`/sandbox/build-libbfd.sb; then
    sb_build_profile=`pwd`/sandbox/build-libbfd.sb
  elif test -e ${srcdir}/sandbox/build-libbfd.sb; then
    sb_build_profile=${srcdir}/sandbox/build-libbfd.sb
  fi
  AC_SUBST([sb_build_profile])dnl
  ## now see if it can be used with a Makefile:
  if test -e `pwd`/sandbox/dummy-Makefile; then
    sb_dummy_Makefile=`pwd`/sandbox/dummy-Makefile
  elif test -e ${srcdir}/sandbox/build-libbfd.sb; then
    sb_dummy_Makefile=${srcdir}/sandbox/dummy-Makefile
  fi
  if test -n "${sb_build_profile}" -a -n "${sb_dummy_Makefile}"; then
    AC_MSG_CHECKING([if ${MAKE_BIN} can be run by ${SANDBOX_EXEC} with profile ${sb_build_profile}])
    ##FIXME: make portable to different OSX versions:
    if (${SANDBOX_EXEC} -f ${sb_build_profile} ${MAKE_BIN} --quiet --no-print-directory -f ${sb_dummy_Makefile} nothing 2>&5); then
      AC_MSG_RESULT([yes])
      # this does not actually work yet, so hide it in a dummy condition
      # to prevent it from actually being done:
      if test "x" = "Y0"; then
        export MAKE_BIN="${SANDBOX_EXEC} -f ${sb_build_profile} ${MAKE_BIN}"
        AC_SUBST([MAKE_BIN])
      fi
      export CAN_BUILD_IN_SANDBOX=1
      AC_SUBST([CAN_BUILD_IN_SANDBOX])dnl
      ## might actually not even be necessary any more...
    else
      AC_MSG_RESULT([no])
    fi
  fi
fi
AX_CHECK_GNU_MAKE

dnl# Default to a non shared library. This may be overridden by the
dnl# configure option --enable-shared.
AC_DISABLE_SHARED

# Libtool checks:
AC_PROG_CXX
AC_MSG_CHECKING([libtool things])
AC_MSG_RESULT([ ])
LT_INIT([disable-fast-install disable-shared dlopen static win32-dll])dnl
LT_LANG([C])dnl
dnl# (temporarily) set this while I update everything libtool-related:
LT_PREREQ([2.4.6])dnl

if test -z "${lt_cv_objdir}"; then
  AC_MSG_WARN([lt_cv_objdir somehow was left unset, setting to default])
  export lt_cv_objdir=.libs
  AC_SUBST([lt_cv_objdir])dnl
  ## other possible names for it:
  if test -z "${objdir}"; then
    export objdir=.libs
    AC_SUBST([objdir])
  fi
  if test -z "${libtooldir}"; then
    export libtooldir=.libs
    AC_SUBST([libtooldir])
  fi
elif test "x${lt_cv_objdir}" = "x.."; then
  AC_MSG_WARN([lt_cv_objdir is one level too high, reassigning])
  export lt_cv_objdir=.
  if test -z "${objdir}" -o "x${objdir}" = "x.."; then
    export objdir=.
  fi
  if test -z "${libtooldir}" -o "x${libtooldir}" = "x.."; then
    export libtooldir=.
  fi
elif test "x${lt_cv_objdir}" = "x../.libs"; then
  AC_MSG_WARN([lt_cv_objdir is in one level too high, reassigning])
  export lt_cv_objdir=./.libs
  export objdir=.
  if test -z "${objdir}" -o "x${objdir}" = "x../.libs"; then
    export objdir=./.libs
  fi
  if test -z "${libtooldir}" -o "x${libtooldir}" = "x../.libs"; then
    export libtooldir=./.libs
  fi
elif test -n "${lt_cv_objdir}"; then
  if test -z "${objdir}"; then
    export objdir="${lt_cv_objdir}"
  fi
  if test -z "${libtooldir}"; then
    export libtooldir="${lt_cv_objdir}"
  fi
# This last one should not be reached; the previous cases should have
# covered all the possibilities, but just in case they failed to do so:
elif test -n "${objdir}"; then
  if test -z "${libtooldir}"; then
    AC_MSG_WARN([setting libtooldir to "${objdir}" should be unnecessary])
    export libtooldir="${objdir}"
  fi
fi
if test "x${lt_sysroot}" = "x" -a "x${prefix}" != "x"; then
  test -z "${lt_sysroot}" && test -n "${prefix}"
  AC_MSG_NOTICE([trying to set lt_sysroot to prefix])
  lt_sysroot="${prefix}"
fi
AC_DEFUN([AX_LIBTOOL_FIXES],[
  AC_MSG_WARN([possibly changing how libtool works])
  m4_ifdef([AX_PATCH_LIBTOOL_CHANGING_CMDS_IFS],[
    AC_REQUIRE([AX_PATCH_LIBTOOL_CHANGING_CMDS_IFS])dnl
  ])dnl
  m4_ifdef([AX_MAINTAINER_MODE_AUTO_SILENT],[
    AC_REQUIRE([AX_MAINTAINER_MODE_AUTO_SILENT])dnl
  ])dnl
])dnl
  ## this libtool hack is probably unnecessary now:
if test -x ${srcdir}/libtool; then
  test -r libtool
  if test "x${USE_MAINTAINER_MODE}" = "xyes"; then
    test ! -z "${USE_MAINTAINER_MODE}"
    AX_LIBTOOL_FIXES
  fi
fi

dnl# AC_PLUGINS (setting "${plugins}") is called by ACX_LARGEFILE:
m4_ifdef([ACX_LARGEFILE],[
  ACX_LARGEFILE
],[
  AC_PLUGINS
])dnl

AM_CONDITIONAL([PLUGINS],[test "x${plugins}" = "xyes"])dnl

if test "x${plugins}" = "xyes"; then
  if test "x${enable_dlopen}" != "xyes"; then
    AC_MSG_ERROR([
      Building BFD with plugin support requires a host that supports -ldl])
  fi
  enable_targets="${enable_targets} plugin"
fi

AC_ARG_ENABLE([64-bit-bfd],
[AS_HELP_STRING([--enable-64-bit-bfd],
                [64-bit support (on hosts with narrower word sizes)])],
[case "${enableval}" in
  yes)  want64=true  ;;
  no)   want64=false ;;
  *)    AC_MSG_ERROR([bad value ${enableval} for 64-bit-bfd option]) ;;
esac],[want64=false])dnl
AC_ARG_ENABLE([targets],
[AS_HELP_STRING([--enable-targets],
                [A comma-delimited list of alternate target configurations.
                 Alternatively, whitespace-delimiting it could work, too,
                 if you remember to quote it properly.])],
[case "${enableval}" in
  yes | "") 
      AC_MSG_ERROR([enable-targets flag must specify targ names or 'all'])
      ;;
  no) enable_targets="" ;;
  *)  enable_targets="${enableval}" ;;
esac
test -n "${enable_targets}"])dnl
  ## this flag could be dangerous:
AC_ARG_ENABLE([obsolete],
[AS_HELP_STRING([--enable-obsolete],
                [allow building of obsolete target configurations])],
[case "${enableval}" in
  yes) export enable_obsolete="yes" ;;
  no)  export enable_obsolete="no" ;;
  *)   AC_MSG_ERROR([bad value '${enableval}' for obsolete flag]) ;;
esac
test -n "${enable_obsolete}"])dnl
  ## other subdirs share this flag, too:
AC_ARG_ENABLE([commonbfdlib],
[AS_HELP_STRING([--enable-commonbfdlib],
                [build shared BFD/opcodes/libiberty library])],
[case "${enableval}" in
  yes) commonbfdlib=true ;;
  no)  commonbfdlib=false ;;
  *)   AC_MSG_ERROR([bad value ${enableval} for BFD commonbfdlib flag]) ;;
esac
test -n "${commonbfdlib}"])dnl
AC_ARG_WITH([mmap],
[AS_HELP_STRING([--with-mmap],
                [try using mmap for BFD input files if available])],
[case "${withval}" in
  yes)  want_mmap=true ;;
  no)   want_mmap=false ;;
  *)    AC_MSG_ERROR([bad value ${withval} for BFD with-mmap option]) ;;
esac
test -n "${want_mmap}"],[want_mmap=false])dnl

AC_ARG_ENABLE([secureplt],
[AS_HELP_STRING([--enable-secureplt],
                [Default to creating read-only plt entries])],
[case "${enableval}" in
  yes)  use_secureplt="true"  ;;
  no)   use_secureplt="false" ;;
  *)    AC_MSG_ERROR([bad value ${enableval} for secureplt option]) ;;
esac],[use_secureplt="false"])dnl
  ## corresponding define:
if test "x${use_secureplt}" = "xtrue"; then
  AC_DEFINE([USE_SECUREPLT],[1],
    [Define if we should default to creating read-only plt entries])
fi

AC_ARG_ENABLE([leading-mingw64-underscores],
  [AS_HELP_STRING([--enable-leading-mingw64-underscores],
                  [Enable leading underscores on 64 bit mingw targets])],
  [],[])dnl
  ## corresponding define:
AS_IF([test x"${enable_leading_mingw64_underscores}" = x"yes"],
  [AC_DEFINE([USE_MINGW64_LEADING_UNDERSCORES],[1],
    [Define to 1 if we should use leading underscore on 64 bit mingw
     targets])dnl
])dnl

DEBUGDIR=${libdir}/debug
AC_ARG_WITH([separate-debug-dir],
[AS_HELP_STRING([--with-separate-debug-dir=DIR],
                [Look for global separate debug info in DIR [[default=LIBDIR/debug]]])],
[DEBUGDIR="${withval}"])
AC_SUBST([DEBUGDIR])dnl

# Check to see if we should allow the generation of
# symbols with the ELF standard's STT_COMMON type.
AC_ARG_ENABLE([elf-stt-common],
[AS_HELP_STRING([--enable-elf-stt-common],
          [Allow the generation of ELF symbols with the STT_COMMON type])],
[case "${enableval}" in
  yes) want_elf_stt_common="true"  ;;
  no)  want_elf_stt_common="false" ;;
  *)   AC_MSG_ERROR([bad value ${enableval} for ELF STT_COMMON option]) ;;
 esac],[
# We have to choose a default behaviour. For native builds we could
# test whether the loader supports the STT_COMMON type, but that would
# mean that built binaries could not be exported to older systems where
# the loader does not support it. So by default we always choose to
# disable this feature.
  want_elf_stt_common="false"])dnl
  ## corresponding define:
if test "x${want_elf_stt_common}" = "xtrue"; then
  AC_DEFINE([USE_STT_COMMON],[1],
    [Define if we may generate symbols with ELF's STT_COMMON type])
fi

m4_ifdef([ACX_PKGVERSION],[
  ACX_PKGVERSION([GNU Binutils])dnl
],[
  echo "Package: GNU Binutils"
])dnl
m4_ifdef([ACX_BUGURL],[
  ACX_BUGURL([http://www.sourceware.org/bugzilla/])dnl
],[
  echo "Bug report URL: http://www.sourceware.org/bugzilla/"
])dnl

if test "x${WANT_CONVERSION_WARNS}" = "x"; then
  test -z "${WANT_CONVERSION_WARNS}" && export WANT_CONVERSION_WARNS=1
fi
AM_BINUTILS_WARNINGS
AM_BINUTILS_CLANG_STATIC_ANALYSIS

dnl# add some insurance to our "config.h" header:
AH_TOP([
#ifndef HAVE_CONFIG_H
# define HAVE_CONFIG_H 1
#endif /* !HAVE_CONFIG_H */
])dnl

# PR 14072
AH_VERBATIM([00_CONFIG_H_CHECK],[
/* Check that config.h is #included before system headers
 * (this works only for glibc, but that should be enough).  */
#if defined(__GLIBC__) && !defined(__FreeBSD_kernel__) && !defined(__CONFIG_H__)
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
 #  warning "config.h should be #included before system headers"
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* __GLIBC__ && !__FreeBSD_kernel__ && !__CONFIG_H__ */
#ifndef __CONFIG_H__
# define __CONFIG_H__ 1
# ifndef __BFD_CONFIG_H__
#  define __BFD_CONFIG_H__ 1
# endif /* __BFD_CONFIG_H__ */
])dnl
AH_BOTTOM([
#endif /* __CONFIG_H__ */
])dnl

if test -z "${target}"; then
  AC_MSG_ERROR([Unrecognized target system type; please check config.sub])
fi

AM_INSTALL_LIBBFD
AM_CONDITIONAL([GENINSRC_NEVER],[false])dnl
AC_OBJEXT
AC_EXEEXT

host64=false
target64=false
bfd_default_target_size=32

dnl# APPLE LOCAL begin mmap() always works right on Darwin. Take our word
dnl# for it.
dnl# CY_GNU_GETTEXT will call AC_FUNC_MMAP for us.
case "${host}" in
  *-apple-darwin* | *-apple-macos*)
   ac_cv_func_mmap_fixed_mapped=yes
   AC_SUBST([ac_cv_func_mmap_fixed_mapped])
  ;;
esac
dnl# APPLE LOCAL end mmap() always works right on Darwin. Take our word
dnl# for it.

# host stuff:
# gettext stuff first specifically:
AC_MSG_CHECKING([gettext stuff])
AC_MSG_RESULT([ ])
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
ALL_LINGUAS="fr tr ja es sv da zh_CN ro rw vi fi id ru uk"
CY_GNU_GETTEXT

# Permit host-specific settings:
AC_MSG_NOTICE([sourcing ${srcdir}/configure.host])
. ${srcdir}/configure.host

AC_SUBST([HDEFINES])dnl
AC_PROG_INSTALL
AC_PROG_LN_S

AC_MSG_CHECKING([host types])
AC_MSG_RESULT([ ])
BFD_HOST_64BIT_LONG=0
BFD_HOST_64BIT_LONG_LONG=0
BFD_HOST_LONG_LONG=0
BFD_HOST_64_BIT_DEFINED=0
BFD_HOST_64_BIT=""
BFD_HOST_U_64_BIT=""
BFD_HOSTPTR_T="unsigned long"

AC_MSG_CHECKING([for long long])
AC_CACHE_VAL([bfd_cv_has_long_long],
[AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
]],[[
unsigned long long ll = 18446744073709551615ULL;
]])],[bfd_cv_has_long_long="yes"],
     [bfd_cv_has_long_long="no"])dnl
])dnl
  ## print:
AC_MSG_RESULT([${bfd_cv_has_long_long}])
  ## if so, further checks:
if test "x${bfd_cv_has_long_long}" = "xyes"; then
  BFD_HOST_LONG_LONG=1
  BFD_HOST_UNSIGNED_LONG_LONG=1
  AC_CHECK_SIZEOF([long long])dnl
  AC_CHECK_ALIGNOF([long long])dnl
  AC_CHECK_SIZEOF([unsigned long long])dnl
  AC_CHECK_ALIGNOF([unsigned long long])dnl
  ## ok
fi

AC_CHECK_SIZEOF([void *])dnl
AC_CHECK_ALIGNOF([void *])dnl
AC_CHECK_SIZEOF([long])dnl
AC_CHECK_ALIGNOF([long])dnl
  ## use that to see if we are 64-bit:
if test "x${ac_cv_sizeof_long}" = "x8"; then
  AC_MSG_NOTICE([our host is natively 64-bit])
  host64=true
  BFD_HOST_64BIT_LONG=1
  test -n "${HOST_64BIT_TYPE}" || HOST_64BIT_TYPE="long"
  test -n "${HOST_U_64BIT_TYPE}" || HOST_U_64BIT_TYPE="unsigned long"
elif test "x${ac_cv_sizeof_long_long}" = "x8"; then
  AC_MSG_NOTICE([using long long for 64-bit])
  BFD_HOST_64BIT_LONG_LONG=1
  test -n "${HOST_64BIT_TYPE}" || HOST_64BIT_TYPE="long long"
  test -n "${HOST_U_64BIT_TYPE}" || HOST_U_64BIT_TYPE="unsigned long long"
  if test "x${ac_cv_sizeof_void_p}" = "x8"; then
    BFD_HOSTPTR_T="unsigned long long"
  fi
else
  AC_MSG_WARN([no type of size 8 found])
fi

if test -n "${HOST_64BIT_TYPE}" -a -n "${HOST_U_64BIT_TYPE}"; then
  BFD_HOST_64_BIT_DEFINED=1
  BFD_HOST_64_BIT="${HOST_64BIT_TYPE}"
  BFD_HOST_U_64_BIT="${HOST_U_64BIT_TYPE}"
  AC_MSG_NOTICE([our host 64-bit types will be '${BFD_HOST_64_BIT}' and '${BFD_HOST_U_64_BIT}'])
else
  AC_MSG_WARN([no host 64-bit type found])
fi

AC_SUBST([BFD_HOST_64BIT_LONG])dnl
AC_SUBST([BFD_HOST_64BIT_LONG_LONG])dnl
AC_SUBST([BFD_HOST_LONG_LONG])dnl
AC_SUBST([BFD_HOST_64_BIT_DEFINED])dnl
AC_SUBST([BFD_HOST_64_BIT])dnl
AC_SUBST([BFD_HOST_U_64_BIT])dnl
AC_SUBST([BFD_HOSTPTR_T])dnl

BFD_CC_FOR_BUILD

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
# might need to check here:
if test -d /usr/lib/gcc/darwin/2.95.2; then
  export LDFLAGS="${LDFLAGS} -L/usr/lib/gcc/darwin/2.95.2"
fi
# check arch when "main()" is the only function for which we check:
ARCHLESS_LDFLAGS="${LDFLAGS}"
LDFLAGS="${LDFLAGS} ${LD_ARCHFLAGS}"
# FIXME: Replace `main' with a function in `-lcc_dynamic':
AC_CHECK_LIB([cc_dynamic],[main],
           [SYSTEM_FRAMEWORK_LIBS="${SYSTEM_FRAMEWORK_LIBS} -lcc_dynamic"])
# FIXME: Replace `main' with a function in `-lgcc':
AC_CHECK_LIB([gcc],[main],
             [SYSTEM_FRAMEWORK_LIBS="${SYSTEM_FRAMEWORK_LIBS} -lgcc"])dnl
# FIXME: Replace `main' with a function in `-lSystem':
AC_CHECK_LIB([System],[main],
            [SYSTEM_FRAMEWORK_LIBS="${SYSTEM_FRAMEWORK_LIBS} -lSystem"])dnl
  ## reset these:
LDFLAGS="${ARCHLESS_LDFLAGS}"
  ## TODO: might only want to make this an error depending on platform:
if test -z "${SYSTEM_FRAMEWORK_LIBS}"; then
  AC_MSG_ERROR([Missing some required system libraries.])
fi
AC_SUBST([SYSTEM_FRAMEWORK_LIBS])dnl
dnl#FIXME: the darwin linker sometimes crashes somewhere in these next
dnl# few parts...
AC_CHECK_LIB([c],[printf])dnl
dnl# the libtool checks will already check for dlopen, so use a different
dnl# symbol here:
AC_CHECK_LIB([dl],[dlsym])dnl
# ../intl is added to LDFLAGS above, so this should work:
AC_CHECK_LIB([intl],[gettext],[],[
  if test -x "`which ld`"; then
    PRE_INTL_LDFLAGS="${LDFLAGS}"
    LDFLAGS="${LDFLAGS} ${LD_ARCHFLAGS}"
    AC_CHECK_LIB([intl],[main],[],[])dnl
    ## reset:
    LDFLAGS="${PRE_INTL_LDFLAGS}"
  fi
])dnl
  ## TODO: might want to make sure the in-tree libiberty is found first:
AC_CHECK_LIB([iberty],[rpl_malloc],[],[
  AC_CHECK_LIB([iberty],[xmalloc],[],[
    if test -x "`which ld`"; then
      PRE_LIBERTY_LDFLAGS="${LDFLAGS}"
      LDFLAGS="${LDFLAGS} ${LD_ARCHFLAGS}"
      AC_CHECK_LIB([iberty],[main])dnl
      ## reset:
      LDFLAGS="${PRE_LIBERTY_LDFLAGS}"
    fi
  ])dnl
])dnl
  ## just in case:
AC_CHECK_LIB([gnu],[rpl_malloc],[],[
  AC_CHECK_LIB([gnu],[xmalloc],[],[
    AC_CHECK_LIB([gnu],[gl_dummy_symbol],[],[
      if test -x "`which ld`"; then
        PRE_GNU_LDFLAGS="${LDFLAGS}"
        LDFLAGS="${LDFLAGS} ${LD_ARCHFLAGS}"
        AC_CHECK_LIB([gnu],[main])dnl
        ## reset:
        LDFLAGS="${PRE_GNU_LDFLAGS}"
      fi
    ])dnl
  ])dnl
])dnl

# Checks for header files.
AC_MSG_CHECKING([headers])
AC_MSG_RESULT([ ])
AC_HEADER_STDBOOL dnl# also "_CHECK"s it
if test -d ../intl; then
  if test -z "${CPPFLAGS}"; then
    export CPPFLAGS="-I. -I../intl"
  else
    export CPPFLAGS="${CPPFLAGS} -I../intl"
  fi
elif test -d ../libintl; then
  if test -z "${CPPFLAGS}"; then
    export CPPFLAGS="-I. -I../libintl"
  else
    export CPPFLAGS="${CPPFLAGS} -I../libintl"
  fi
fi
unset ac_cv_header_libintl_h
unset ac_cv_header_limits_h
unset ac_cv_header_stdlib_h
unset ac_cv_header_sys_param_h
AC_CHECK_HEADERS([ctype.h errno.h fcntl.h gettext.h iconv.h libgen.h \
                  libgettext.h libintl.h limits.h mach/mach.h \
                  mach-o/loader.h malloca.h malloc/malloc.h math.h \
                  realloc.h stdarg.h stddef.h stdio.h sys/acl.h \
                  sys/core.h sys/errno.h sys/file.h sys/paccess.h \
                  sys/param.h sys/region.h sys/resource.h sys/socket.h \
                  sys/time.h sys/user.h sys/utsname.h time.h \
                  wchar.h wctype.h windef.h windows.h])dnl
GCC_HEADER_STDINT([bfd_stdint.h],[stdint.h])dnl
AC_HEADER_TIME
AC_HEADER_DIRENT
AC_HEADER_STAT
ACX_HEADER_STRING
# This should have already been checked:
unset ac_cv_header_malloc_h
AC_CHECK_HEADERS([malloc.h])dnl

# Checks for typedefs, structures, and compiler characteristics.
AC_MSG_CHECKING([C language characteristics that your C compiler supports])
AC_MSG_RESULT([ ])
AC_C_BIGENDIAN
AC_C_PROTOTYPES
AC_C_RESTRICT
AC_C_STRINGIZE
AC_C_TYPEOF
AC_C_VOLATILE
AC_MSG_CHECKING([other types that did not fit with the host ones above])
AC_MSG_RESULT([ ])
AC_TYPE_MODE_T
AC_TYPE_PID_T
AC_TYPE_UID_T
AC_CHECK_TYPES([ptrdiff_t])dnl
AC_DEFUN([AC_TYPE_VARIOUS_INT_TYPES],[
  AC_REQUIRE([AC_TYPE_INT32_T])dnl
  AC_REQUIRE([AC_TYPE_UINT16_T])dnl
  AC_REQUIRE([AC_TYPE_UINT32_T])dnl
  AC_REQUIRE([AC_TYPE_UINT64_T])dnl
])dnl
AC_TYPE_VARIOUS_INT_TYPES
AC_STRUCT_TIMEZONE dnl# also does the "_TM" one
AC_CHECK_MEMBERS([struct user.u_dsize, struct user.u_ssize],[],[],[
#if defined(__USLC__) || defined(HAVE_SYS_USER_H)
# include <sys/user.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "The conftest for members of 'struct user' wants <sys/user.h>"
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* __USLC__ || HAVE_SYS_USER_H */
])dnl
AC_CHECK_MEMBERS([struct user.u_comm],[],[
  AC_DEFINE([NO_CORE_COMMAND],[1],
            [Define to 1 if struct user has no u_comm member])dnl
],[
#if defined(__USLC__) || defined(HAVE_SYS_USER_H)
# include <sys/user.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "The conftest for a member of 'struct user' wants <sys/user.h>"
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* __USLC__ || HAVE_SYS_USER_H */
])dnl

# Checks for library functions.
AC_MSG_CHECKING([functions])
AC_MSG_RESULT([ ])
AM_WITH_DMALLOC
AC_CHECK_FUNCS([btowc calloc close exit fabs fabsf fabsl fcntl fdopen \
                feof ferror fileno fopen fprintf fread getgid getrlimit \
                getuid islower isspace isupper mbsrtowcs memchr memmove \
                mempcpy memset open pread qsort rpl_malloc rpl_realloc \
                setitimer strcmp strdup strncasecmp strrchr strspn strstr \
                strtol strtoul strtoull sysconf system uname wmempcpy])dnl
AC_CHECK_FUNCS_ONCE([lt_dlinit])dnl
AC_FUNC_CLOSEDIR_VOID
AC_FUNC_FNMATCH
AC_FUNC_FORK
AC_FUNC_FSEEKO
AC_FUNC_MALLOC
AC_FUNC_MEMCMP
AC_FUNC_REALLOC
AC_FUNC_STAT
AC_FUNC_STRNLEN
AC_FUNC_VPRINTF
AC_SEARCH_LIBS([rpl_realloc],[iberty c gnu])dnl
AC_SEARCH_LIBS([rpl_malloc],[iberty c gnu])dnl

# other misc. stuff:
AC_MSG_CHECKING([stuff that we might have already checked])
AC_MSG_RESULT([ ])
# Hack because AC_REQUIRE cannot be used outside of macro definitions:
AC_DEFUN([AC_POSSIBLE_DUPLICATE_CHECKS],
[AC_REQUIRE([AC_FUNC_ALLOCA])dnl
 AC_REQUIRE([AC_FUNC_MMAP])dnl
 AC_REQUIRE([AC_TYPE_OFF_T])dnl
 AC_REQUIRE([AC_TYPE_SIZE_T])dnl
 AC_REQUIRE([AC_C_CONST])dnl
 AC_REQUIRE([AC_C_INLINE])dnl
 AC_REQUIRE([AM_PROG_CC_C_O])dnl
 AC_REQUIRE([AM_DEP_TRACK])dnl
 AC_REQUIRE([AM_SET_DEPDIR])dnl
 AC_REQUIRE([AM_OUTPUT_DEPENDENCY_COMMANDS])dnl
 AC_REQUIRE([AM_MAKE_INCLUDE])dnl
 AC_REQUIRE([AM_SET_CURRENT_AUTOMAKE_VERSION])dnl
 AC_REQUIRE([AM_PROG_INSTALL_SH])dnl
 AC_REQUIRE([AM_SET_LEADING_DOT])dnl
 AC_REQUIRE([AC_ARG_PROGRAM])dnl
 AC_REQUIRE([AC_SYS_LARGEFILE])dnl
 AC_REQUIRE([AC_SYS_LONG_FILE_NAMES])dnl
 AC_REQUIRE([AC_PROG_AWK])dnl
 AC_REQUIRE([AC_PROG_GREP])dnl
 AC_REQUIRE([AC_PROG_EGREP])dnl
 AC_REQUIRE([AC_PROG_FGREP])dnl
 AC_REQUIRE([AC_PROG_MKDIR_P])dnl
 AC_REQUIRE([AC_PROG_SED])dnl
 AC_REQUIRE([AC_HEADER_SYS_WAIT])dnl
 AC_REQUIRE([AC_TYPE_MBSTATE_T])dnl
 AC_REQUIRE([GCC_STDINT_TYPES])dnl
 m4_ifdef([ZW_GNU_GETTEXT_SISTER_DIR],[
   AC_REQUIRE([ZW_GNU_GETTEXT_SISTER_DIR])dnl
 ])dnl

 dnl#FIXME: require does NOT work with conditionals like this:
 if test "x${GETTEXT_MACRO_VERSION}" = "x"; then
   test -z "${GETTEXT_MACRO_VERSION}"
   AC_REQUIRE([AM_PO_SUBDIRS])dnl
   ## maybe handle differently?
 fi
])dnl
AC_POSSIBLE_DUPLICATE_CHECKS

AC_MSG_CHECKING([declarations])
AC_MSG_RESULT([ ])
AC_CHECK_DECLS([basename],[],[],[
/* declared in this header on darwin at least: */
#ifdef HAVE_LIBGEN_H
# include <libgen.h>
#endif /* HAVE_LIBGEN_H */
])dnl
AC_CHECK_DECLS([close])dnl
AC_CHECK_DECLS([ftello])dnl
AC_CHECK_DECLS([ftello64])dnl
AC_CHECK_DECLS([fseeko])dnl
AC_CHECK_DECLS([fseeko64])dnl

BFD_BINARY_FOPEN

AC_CHECK_DECLS([ffs])dnl
AC_CHECK_DECLS([free])dnl
AC_CHECK_DECLS([getenv])dnl
AC_CHECK_DECLS_ONCE([getopt])dnl
AC_CHECK_DECLS([malloc])dnl
AC_CHECK_DECLS([open])dnl
AC_CHECK_DECLS([pread])dnl
AC_CHECK_DECLS([realloc])dnl
AC_CHECK_DECLS([stpcpy])dnl
AC_CHECK_DECLS([strstr])dnl
AC_CHECK_DECLS([snprintf])dnl
AC_CHECK_DECLS([vsnprintf])dnl
AC_CHECK_DECLS([strnlen])dnl

# Link in zlib if we can. This allows us to read compressed debug sections.
# This is used only by compress.c.
AC_MSG_CHECKING([zlib])
AC_MSG_RESULT([ ])
m4_ifdef([AM_ZLIB],[
  AM_ZLIB dnl# from ../config/zlib.m4
],[
AC_MSG_CHECKING([if there is a zlib sister directory in this source tree])
if test -d ../zlib -o -d ${srcdir}/../zlib; then
  export HAVE_ZLIB_SOURCE=1
  AC_SUBST([HAVE_ZLIB_SOURCE])dnl
  ## print:
  AC_MSG_RESULT([yes])
else
  AC_MSG_RESULT([no])
fi
AC_SEARCH_LIBS([zlibVersion],[z],[AC_CHECK_HEADERS([zlib.h])])dnl
])dnl

# If we are configured native, pick a core file support file.
AC_MSG_CHECKING([more system-specific things])
AC_MSG_RESULT([ ])
COREFILE=""
COREFLAG=""
CORE_HEADER=""
TRAD_HEADER=""
if test "${target}" = "${host}"; then
  case "${host}" in
  alpha*-*-freebsd* | alpha*-*-kfreebsd*-gnu | alpha*-*-*vms*)
	COREFILE=''
	;;
  alpha*-*-linux-*)
	COREFILE=trad-core.lo
	TRAD_HEADER='"hosts/alphalinux.h"'
	;;
  alpha*-*-netbsd* | alpha*-*-openbsd*)
	COREFILE=netbsd-core.lo
	;;
  alpha*-*-*)		COREFILE=osf-core.lo ;;
  arm-*-freebsd* | arm-*-kfreebsd*-gnu)
			COREFILE='' ;;
  arm-*-netbsd* | arm-*-openbsd*)
			COREFILE=netbsd-core.lo ;;
  arm-*-riscix)		COREFILE=trad-core.lo ;;
  hppa*-*-hpux*)	COREFILE=hpux-core.lo ;;
  hppa*-*-hiux*)	COREFILE=hpux-core.lo ;;
  hppa*-*-mpeix*)	COREFILE=hpux-core.lo ;;
  hppa*-*-bsd*)		COREFILE="hpux-core.lo hppabsd-core.lo"
			COREFLAG="-DHPUX_CORE -DHPPABSD_CORE" ;;
  hppa*-*-netbsd* | hppa*-*-openbsd*)
	COREFILE=netbsd-core.lo
	;;

  i370-*-*)
	COREFILE=trad-core.lo
	TRAD_HEADER='"hosts/i370linux.h"'
	;;

changequote(,)dnl
  i[3-7]86-sequent-bsd*)
changequote([,])dnl
	COREFILE=trad-core.lo
	TRAD_HEADER='"hosts/symmetry.h"'
	;;
changequote(,)dnl
  i[3-7]86-sequent-sysv4*) ;;
  i[3-7]86-sequent-sysv*)
changequote([,])dnl
	COREFILE=trad-core.lo
	TRAD_HEADER='"hosts/symmetry.h"'
	;;
changequote(,)dnl
  i[3-7]86-*-bsdi)
changequote([,])dnl
	COREFILE=''
	;;
changequote(,)dnl
  i[3-7]86-*-bsd* | i[3-7]86-*-freebsd[123] | i[3-7]86-*-freebsd[123]\.* | i[3-7]86-*-freebsd4\.[01234] | i[3-7]86-*-freebsd4\.[01234]\.* | i[3-7]86-*-freebsd*aout*)
changequote([,])dnl
	COREFILE=trad-core.lo
	TRAD_HEADER='"hosts/i386bsd.h"'
	;;
changequote(,)dnl
  i[3-7]86-*-freebsd* | i[3-7]86-*-kfreebsd*-gnu | i[3-7]86-*-dragonfly*)
changequote([,])dnl
	COREFILE=''
	TRAD_HEADER='"hosts/i386bsd.h"'
	;;
changequote(,)dnl
  i[3-7]86-*-netbsd* | i[3-7]86-*-knetbsd*-gnu | i[3-7]86-*-openbsd*)
changequote([,])dnl
	COREFILE=netbsd-core.lo
	;;
changequote(,)dnl
  i[3-7]86-esix-sysv3*)
changequote([,])dnl
	COREFILE=trad-core.lo
	TRAD_HEADER='"hosts/esix.h"'
	;;
changequote(,)dnl
  i[3-7]86-*-sco3.2v5*)
changequote([,])dnl
	COREFILE=sco5-core.lo
	;;
changequote(,)dnl
  i[3-7]86-*-sco* | i[3-7]86-*-isc*)
changequote([,])dnl
	COREFILE=trad-core.lo
	TRAD_HEADER='"hosts/i386sco.h"'
	;;
changequote(,)dnl
  i[3-7]86-*-mach3*)
changequote([,])dnl
	COREFILE=trad-core.lo
	TRAD_HEADER='"hosts/i386mach3.h"'
	;;
changequote(,)dnl
  i[3-7]86-*-linux-*)
changequote([,])dnl
	COREFILE=trad-core.lo
	TRAD_HEADER='"hosts/i386linux.h"'
	case "${enable_targets}"-"${want64}" in
	  *x86_64-*linux*|*-true)
	    CORE_HEADER='"hosts/x86-64linux.h"' ;;
	esac
	;;
changequote(,)dnl
  i[3-7]86-*-isc*)	COREFILE=trad-core.lo ;;
  i[3-7]86-*-aix*)	COREFILE=aix386-core.lo ;;
changequote([,])dnl
  i860-*-mach3* | i860-*-osf1*)
	COREFILE=trad-core.lo
	TRAD_HEADER='"hosts/i860mach3.h"'
	;;
  mips-*-netbsd* | mips*-*-openbsd*)
	COREFILE=netbsd-core.lo
	;;
  mips-dec-*)
	COREFILE=trad-core.lo
	TRAD_HEADER='"hosts/decstation.h"'
	;;
  mips-sgi-irix4*)	COREFILE=irix-core.lo ;;
  mips-sgi-irix5*)	COREFILE=irix-core.lo ;;
  mips-sgi-irix6*)	COREFILE=irix-core.lo ;;
  mips-*-sysv4*)	;;
  mips-*-sysv* | mips-*-riscos*)
	COREFILE=trad-core.lo
	TRAD_HEADER='"hosts/riscos.h"'
	;;
  mips-sony-bsd*)
	COREFILE=trad-core.lo
	TRAD_HEADER='"hosts/news-mips.h"'
	;;
  m68*-bull*-sysv*)
	COREFILE=trad-core.lo
	TRAD_HEADER='"hosts/dpx2.h"'
	;;
  m68*-hp-hpux*)	COREFILE=hpux-core.lo ;;
  m68*-hp-bsd*)
	COREFILE=trad-core.lo
	TRAD_HEADER='"hosts/hp300bsd.h"'
	;;
  m68*-*-linux-*)
	COREFILE=trad-core.lo
	TRAD_HEADER='"hosts/m68klinux.h"'
	;;
  m68*-motorola-sysv*)
	COREFILE=trad-core.lo
	TRAD_HEADER='"hosts/delta68.h"'
	;;
  m68*-sony-*)
	COREFILE=trad-core.lo
	TRAD_HEADER='"hosts/news.h"'
	;;
  m68*-*-netbsd* | m68*-*-openbsd*)
	COREFILE=netbsd-core.lo
	;;
  m68*-apple-aux*)
	COREFILE=trad-core.lo
	TRAD_HEADER='"hosts/m68kaux.h"'
	;;
  m88*-*-sysv4*)
	;;
  m88*-motorola-sysv*)
	COREFILE=ptrace-core.lo
	;;
  m88*-*-mach3*)
	COREFILE=trad-core.lo
	TRAD_HEADER='"hosts/m88kmach3.h"'
	;;
  m88*-*-openbsd*)
	COREFILE=netbsd-core.lo
	;;
  ns32k-pc532-mach)
	COREFILE=trad-core.lo
	TRAD_HEADER='"hosts/pc532mach.h"'
	;;
  ns32k-*-netbsd* | ns32k-*-openbsd*)
	COREFILE=netbsd-core.lo
	;;
  rs6000-*-lynx*)
  	COREFILE=lynx-core.lo
	;;
changequote(,)dnl
  rs6000-*-aix[5-9].* | powerpc-*-aix[5-9].*)
changequote([,])dnl
        COREFILE=rs6000-core.lo
	COREFLAG="${COREFLAG} -DAIX_5_CORE -DAIX_CORE_DUMPX_CORE"
	;;
changequote(,)dnl
  rs6000-*-aix4.[3-9]* | powerpc-*-aix4.[3-9]*)
changequote([,])dnl
        COREFILE=rs6000-core.lo
	COREFLAG="${COREFLAG} -DAIX_CORE_DUMPX_CORE"
	# Not all versions of AIX with -DAIX_CORE_DUMPX_CORE
	# have c_impl as a member of struct core_dumpx
	AC_CHECK_HEADERS_ONCE([core.h])
	AC_MSG_CHECKING([for c_impl in struct core_dumpx])
	AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <core.h>
        ]],[[
struct core_dumpx c; c.c_impl = 0;
        ]])],[AC_DEFINE([HAVE_ST_C_IMPL],[1],
	                [Define if struct core_dumpx has member c_impl])
	      AC_MSG_RESULT([yes])],
             [AC_MSG_RESULT([no])
        ])
	;;
  rs6000-*-aix4*)	COREFILE=rs6000-core.lo ;;
  rs6000-*-*)		COREFILE=rs6000-core.lo ;;
  powerpc-*-aix4*)	COREFILE=rs6000-core.lo ;;
  powerpc-*-aix*)	COREFILE=rs6000-core.lo ;;
  powerpc-*-beos*)	;;
  powerpc-*-freebsd* | powerpc-*-kfreebsd*-gnu)
		        COREFILE='' ;;
  powerpc-*-netbsd*)	COREFILE=netbsd-core.lo ;;
  powerpc-*-*bsd*)	COREFILE=netbsd-core.lo ;;
  s390*-*-*)            COREFILE=trad-core.lo ;;
  sh*-*-netbsd* | sh*-*-openbsd*)
	COREFILE=netbsd-core.lo
	;;
  sparc-*-netbsd* | sparc*-*-openbsd*)
	COREFILE=netbsd-core.lo
	;;
  tahoe-*-*)
	COREFILE=trad-core.lo
	TRAD_HEADER='"hosts/tahoe.h"'
	;;
  vax-*-netbsd* | vax-*-openbsd*)
  	COREFILE=netbsd-core.lo
	;;
  vax-*-ultrix2*)
	COREFILE=trad-core.lo
	TRAD_HEADER='"hosts/vaxult2.h"'
	;;
  vax-*-ultrix*)
	COREFILE=trad-core.lo
	TRAD_HEADER='"hosts/vaxult2.h"'
	;;
  vax-*-linux-*)
	COREFILE=trad-core.lo
	TRAD_HEADER='"hosts/vaxlinux.h"'
	;;
  vax-*-*)
	COREFILE=trad-core.lo
	TRAD_HEADER='"hosts/vaxbsd.h"'
	;;
  x86_64-*-linux*)
	CORE_HEADER='"hosts/x86-64linux.h"'
	;;
  x86_64-*-netbsd* | x86_64-*-openbsd*)
	COREFILE=netbsd-core.lo
	;;
  esac

  case "${COREFILE}" in
  aix386-core.lo)	COREFLAG="-DAIX386_CORE" ;;
  hppabsd-core.lo)	COREFLAG="-DHPPABSD_CORE" ;;
  hpux-core.lo)		COREFLAG="-DHPUX_CORE" ;;
  irix-core.lo)		COREFLAG="-DIRIX_CORE" ;;
  lynx-core.lo)		COREFLAG="-DLYNX_CORE" ;;
  netbsd-core.lo)	COREFLAG="-DNETBSD_CORE" ;;
  osf-core.lo)		COREFLAG="-DOSF_CORE" ;;
  ptrace-core.lo)	COREFLAG="-DPTRACE_CORE" ;;
  rs6000-core.lo)	COREFLAG="${COREFLAG} -DAIX_CORE" ;;
  sco5-core.lo)		COREFLAG="${COREFLAG} -DSCO5_CORE" ;;
  trad-core.lo)		COREFLAG="${COREFLAG} -DTRAD_CORE" ;;
  esac

  # ELF corefile support has several flavors, but all of
  # them use something called <sys/procfs.h>, so check it:
  unset ac_cv_header_sys_procfs_h
  AC_CHECK_HEADERS([sys/procfs.h])dnl
  ## these types depend on the header:
  if test "x${ac_cv_header_sys_procfs_h}" = "xyes"; then
    BFD_HAVE_SYS_PROCFS_TYPE([prstatus_t])dnl
    BFD_HAVE_SYS_PROCFS_TYPE([prstatus32_t])dnl
    BFD_HAVE_SYS_PROCFS_TYPE_MEMBER([prstatus_t],[pr_who])dnl
    BFD_HAVE_SYS_PROCFS_TYPE_MEMBER([prstatus32_t],[pr_who])dnl
    BFD_HAVE_SYS_PROCFS_TYPE([pstatus_t])dnl
    BFD_HAVE_SYS_PROCFS_TYPE([pxstatus_t])dnl
    BFD_HAVE_SYS_PROCFS_TYPE([pstatus32_t])dnl
    BFD_HAVE_SYS_PROCFS_TYPE([prpsinfo_t])dnl
    BFD_HAVE_SYS_PROCFS_TYPE_MEMBER([prpsinfo_t],[pr_pid])dnl
    BFD_HAVE_SYS_PROCFS_TYPE([prpsinfo32_t])dnl
    BFD_HAVE_SYS_PROCFS_TYPE_MEMBER([prpsinfo32_t],[pr_pid])dnl
    BFD_HAVE_SYS_PROCFS_TYPE([psinfo_t])dnl
    BFD_HAVE_SYS_PROCFS_TYPE_MEMBER([psinfo_t],[pr_pid])dnl
    BFD_HAVE_SYS_PROCFS_TYPE([psinfo32_t])dnl
    BFD_HAVE_SYS_PROCFS_TYPE_MEMBER([psinfo32_t],[pr_pid])dnl
    BFD_HAVE_SYS_PROCFS_TYPE([lwpstatus_t])dnl
    BFD_HAVE_SYS_PROCFS_TYPE([lwpxstatus_t])dnl
    BFD_HAVE_SYS_PROCFS_TYPE_MEMBER([lwpstatus_t],[pr_context])dnl
    BFD_HAVE_SYS_PROCFS_TYPE_MEMBER([lwpstatus_t],[pr_reg])dnl
    BFD_HAVE_SYS_PROCFS_TYPE_MEMBER([lwpstatus_t],[pr_fpreg])dnl
    BFD_HAVE_SYS_PROCFS_TYPE([win32_pstatus_t])dnl
    ## that should be all of those for now...
  else
    AC_MSG_NOTICE([skipping checks for types in missing header])
  fi
fi
AC_SUBST([COREFILE])dnl
AC_SUBST([COREFLAG])dnl

if test -n "${CORE_HEADER}"; then
  AC_DEFINE_UNQUOTED([CORE_HEADER],[${CORE_HEADER}],
    [Name of host specific core header file to include in elf.c.])
fi
if test -n "${TRAD_HEADER}"; then
  AC_DEFINE_UNQUOTED([TRAD_HEADER],[${TRAD_HEADER}],
    [Name of host specific header file to include in trad-core.c.])
fi

# Check if linker supports --as-needed and --no-as-needed options:
AC_CACHE_CHECK([linker --as-needed support],[bfd_cv_ld_as_needed],
[bfd_cv_ld_as_needed=no
 if ${LD} --help 2>/dev/null | grep as-needed > /dev/null; then
    bfd_cv_ld_as_needed=yes
 fi
])dnl

dnl# check for libm:
LT_LIB_M

# When building a shared libbfd, link against the pic version of libiberty
# so that apps that use libbfd will NOT need libiberty just to satisfy any
# libbfd references.
# We cannot do that if a pic libiberty is unavailable since including
# non-pic code would insert text relocations into libbfd.
# These are also horrible hacks to build DLLs on Windows.
WIN32LDFLAGS=""
WIN32LIBADD=""
SHARED_LIBADD=""
SHARED_LDFLAGS=""
case "${host}" in
*-*-cygwin*)
  if test "x${enable_shared}" = "xyes"; then
    WIN32LDFLAGS="-no-undefined"
    SHARED_LDFLAGS="${WIN32LDFLAGS}"
    WIN32LIBADD="-L`pwd`/../libiberty -liberty -L`pwd`/../intl -lintl -lcygwin -lkernel32"
    SHARED_LIBADD="${WIN32LIBADD}"
  fi
  ;;
*-*-linux*)
  # We borrow WIN32LIBADD so that the shared libbfd will not depend on
  # libiberty.a.
  case "${host}" in
    mips*-*-linux*)
      # Linux/MIPS uses PIC by default.
      if test "x${enable_shared}" = "xyes"; then
        WIN32LIBADD="-L../libiberty -liberty"
      fi
      ;;
    *)
changequote(,)dnl
      x=`sed -n -e 's/^[ 	]*PICFLAG[ 	]*=[ 	]*//p' < ../libiberty/Makefile | sed -n '$p'`
changequote([,])dnl
      if test -n "${x}"; then
        WIN32LIBADD="-L../libiberty/pic -liberty"
        SHARED_LIBADD="-L`pwd`/../libiberty/pic -liberty"
      fi
      ;;
  esac
  ;;
# Hack to build or1k-src on OSX:
or1k*-*-darwin*)
  if test "x${enable_shared}" = "xyes"; then
    SHARED_LIBADD="-L`pwd`/../libiberty/pic -L`pwd`/../intl -liberty -lintl"
  fi
  ;;
*)
  if test "x${enable_shared}" = "xyes"; then
    if test -n "${SHARED_LIBADD}"; then
      if test -n "${LIBM}"; then
        if test x"${bfd_cv_ld_as_needed}" = x"yes"; then
	  # Link against libm only when needed.  Put -lc, -lm inside -Wl
	  # to stop libtool reordering these options.
	  SHARED_LIBADD="${SHARED_LIBADD} -Wl,-lc,--as-needed,`echo ${LIBM} | sed 's/ /,/g'`,--no-as-needed"
        else
	  SHARED_LIBADD="${SHARED_LIBADD} ${LIBM}"
        fi
      fi
    fi
  fi
  ;;
esac
AC_SUBST([WIN32LDFLAGS])dnl
AC_SUBST([WIN32LIBADD])dnl
AC_SUBST([SHARED_LDFLAGS])dnl
AC_SUBST([SHARED_LIBADD])dnl

# target stuff:

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
defvec=""
selvecs=""
assocvecs=""
selarchs=""
TDEFINES=""

AC_MSG_CHECKING([which targets to look up])
AC_MSG_RESULT([plain ones are '${target}' and canonical ones are '${canon_targets}'])
for targ in ${target} ${canon_targets}
do
    if test "x${targ}" = "xall"; then
        AC_MSG_NOTICE([no need to source config.bfd when targ is 'all'])
        all_targets=true
	assocvecs="${assocvecs} ${targ_defvec} ${targ_selvecs}"
    else
	AC_MSG_NOTICE([sourcing ${srcdir}/config.bfd for target '${targ}'])
	. ${srcdir}/config.bfd
        if test -z "${BREAK_MY_BUILD}"; then
            # reassign these to avoid build errors:
            if test "x${targ_defvec}" = "xx86_64_mach_o_vec"; then
                targ_defvec=mach_o_le_vec
            fi
            if test "x${targ_selvecs}" = "xx86_64_mach_o_vec"; then
                unset targ_selvecs
            fi
            if test "x${targ64_selvecs}" = "xx86_64_mach_o_vec"; then
                unset targ64_selvecs
            fi
        fi
	if test "x${targ}" = "x${target}"; then
	    defvec="${targ_defvec}"
            AC_MSG_NOTICE([defvec is now '${defvec}'])
	fi
	selvecs="${selvecs} ${targ_defvec} ${targ_selvecs}"
        AC_MSG_NOTICE([selvecs are now '${selvecs}'])
	selarchs="${selarchs} ${targ_archs}"
        AC_MSG_NOTICE([selarchs are now '${selarchs}'])
	TDEFINES="${TDEFINES} ${targ_cflags}"
        AC_MSG_NOTICE([TDEFINES are now '${TDEFINES}'])
    fi
done
AC_SUBST([selvecs])dnl
AC_SUBST([selarchs])dnl
AC_SUBST([TDEFINES])dnl

# This processing still needs to be done if we are to decide properly
# whether 64-bit support needs to be compiled in. Currently, it will be
# included if the default or any other explicitly requested target requires
# it; it will not be included on a 32-bit host if no 64-bit target is
# requested, and no "--with-64-bit-bfd" option is given, even if
# "--enable-targets=all" is used.

AC_PATH_PROGS([UNIQ_BIN],[uniq guniq],[])dnl
# uniq the default and selected vectors in all the configured targets:
f=""
for i in ${selvecs}; do
    case " ${f} " in
    *" ${i} "*) ;;
    *) f="${f} ${i}" ;;
    esac
done
selvecs="${f}"

# uniq the associated vectors in all the configured targets:
f=""
for i in ${assocvecs}; do
    case " ${f} " in
    *" ${i} "*) ;;
    *) f="${f} ${i}" ;;
    esac
done
assocvecs="${f}"

# uniq the architectures in all the configured targets:
f=""
for i in ${selarchs}; do
    case " ${f} " in
    *" ${i} "*) ;;
    *) f="${f} ${i}" ;;
    esac
done
selarchs="${f}"

# Target backend .o files.
tb=""

elf="elf.lo elflink.lo elf-attrs.lo elf-strtab.lo elf-eh-frame.lo dwarf1.lo"

for vec in ${selvecs}
do
    target_size=32
    case "${vec}" in
    # This list is alphabetized to make it easy to compare
    # with the two vector lists in targets.c. For the same reason,
    # use one entry per line, even though this leads to long lines.
    a29kcoff_big_vec) tb="${tb} coff-a29k.lo cofflink.lo" ;;
    a_out_adobe_vec) tb="${tb} aout-adobe.lo aout32.lo" ;;
    aarch64_elf32_be_vec) tb="${tb} elf32-aarch64.lo elfxx-aarch64.lo elf-ifunc.lo elf32.lo ${elf}"; target_size=64 ;;
    aarch64_elf32_le_vec) tb="${tb} elf32-aarch64.lo elfxx-aarch64.lo elf-ifunc.lo elf32.lo ${elf}"; target_size=64 ;;
    aarch64_elf64_be_vec) tb="${tb} elf64-aarch64.lo elfxx-aarch64.lo elf-ifunc.lo elf64.lo ${elf}"; target_size=64 ;;
    aarch64_elf64_le_vec) tb="${tb} elf64-aarch64.lo elfxx-aarch64.lo elf-ifunc.lo elf64.lo ${elf}"; target_size=64 ;;
    aix5coff64_vec) tb="${tb} coff64-rs6000.lo xcofflink.lo aix5ppc-core.lo"; target_size=64 ;;
    alpha_ecoff_le_vec) tb="${tb} coff-alpha.lo ecoff.lo ecofflink.lo"; target_size=64 ;;
    alpha_elf64_vec) tb="${tb} elf64-alpha.lo elf64.lo $elf"; target_size=64 ;;
    alpha_elf64_fbsd_vec) tb="${tb} elf64-alpha.lo elf64.lo $elf"; target_size=64 ;;
    alpha_nlm32_vec) tb="${tb} nlm32-alpha.lo nlm32.lo nlm.lo"; target_size=64 ;;
    alpha_vms_vec) tb="${tb} vms-alpha.lo vms-misc.lo vms-lib.lo"; target_size=64 ;;
    alpha_vms_lib_txt_vec) tb="${tb} vms-lib.lo vms-misc.lo" ;;
    am33_elf32_linux_vec) tb="${tb} elf32-am33lin.lo elf32.lo ${elf}" ;;
    aout0_be_vec) tb="${tb} aout0.lo aout32.lo" ;;
    aout0_big_vec) tb="${tb} aout0.lo aout32.lo" ;;
    aout64_vec) tb="${tb} demo64.lo aout64.lo"; target_size=64 ;;
    aout_vec) tb="${tb} host-aout.lo aout32.lo" ;;
    aout_adobe_vec) tb="${tb} aout-adobe.lo aout32.lo" ;;
    aout_arm_big_vec) tb="${tb} aout-arm.lo aout32.lo" ;;
    aout_arm_little_vec) tb="${tb} aout-arm.lo aout32.lo" ;;
    apollocoff_vec) tb="${tb} coff-apollo.lo" ;;
    arc_elf32_be_vec) tb="${tb} elf32-arc.lo elf32.lo ${elf}" ;;
    arc_elf32_le_vec) tb="${tb} elf32-arc.lo elf32.lo ${elf}" ;;
    arm_aout_be_vec) tb="${tb} aout-arm.lo aout32.lo" ;;
    arm_aout_le_vec) tb="${tb} aout-arm.lo aout32.lo" ;;
    arm_aout_nbsd_vec) tb="${tb} armnetbsd.lo aout32.lo" ;;
    arm_aout_riscix_vec) tb="${tb} aout32.lo riscix.lo" ;;
    arm_coff_be_vec) tb="${tb} coff-arm.lo cofflink.lo " ;;
    arm_coff_le_vec) tb="${tb} coff-arm.lo cofflink.lo " ;;
    arm_elf32_be_vec) tb="${tb} elf32-arm.lo elf32.lo elf-nacl.lo elf-vxworks.lo ${elf}" ;;
    arm_elf32_le_vec) tb="${tb} elf32-arm.lo elf32.lo elf-nacl.lo elf-vxworks.lo ${elf}" ;;
    arm_elf32_nacl_be_vec) tb="${tb} elf32-arm.lo elf32.lo elf-nacl.lo elf-vxworks.lo ${elf}" ;;
    arm_elf32_nacl_le_vec) tb="${tb} elf32-arm.lo elf32.lo elf-nacl.lo elf-vxworks.lo ${elf}" ;;
    arm_elf32_symbian_be_vec) tb="${tb} elf32-arm.lo elf32.lo elf-nacl.lo elf-vxworks.lo ${elf}" ;;
    arm_elf32_symbian_le_vec) tb="${tb} elf32-arm.lo elf32.lo elf-nacl.lo elf-vxworks.lo ${elf}" ;;
    arm_elf32_vxworks_be_vec) tb="${tb} elf32-arm.lo elf32.lo elf-nacl.lo elf-vxworks.lo ${elf}" ;;
    arm_elf32_vxworks_le_vec) tb="${tb} elf32-arm.lo elf32.lo elf-nacl.lo elf-vxworks.lo ${elf}" ;;
    arm_epoc_pe_big_vec) tb="${tb} epoc-pe-arm.lo peigen.lo cofflink.lo " ;;
    arm_epoc_pe_little_vec) tb="${tb} epoc-pe-arm.lo peigen.lo cofflink.lo " ;;
    arm_epoc_pei_big_vec) tb="${tb} epoc-pei-arm.lo peigen.lo cofflink.lo " ;;
    arm_epoc_pei_little_vec) tb="${tb} epoc-pei-arm.lo peigen.lo cofflink.lo " ;;
    arm_pe_be_vec) tb="${tb} pe-arm.lo peigen.lo cofflink.lo " ;;
    arm_pe_le_vec) tb="${tb} pe-arm.lo peigen.lo cofflink.lo " ;;
    arm_pe_epoc_be_vec) tb="${tb} epoc-pe-arm.lo peigen.lo cofflink.lo " ;;
    arm_pe_epoc_le_vec) tb="${tb} epoc-pe-arm.lo peigen.lo cofflink.lo " ;;
    arm_pe_wince_be_vec) tb="${tb} coff-arm.lo pe-arm-wince.lo pe-arm.lo peigen.lo cofflink.lo " ;;
    arm_pe_wince_le_vec) tb="${tb} coff-arm.lo pe-arm-wince.lo pe-arm.lo peigen.lo cofflink.lo " ;;
    arm_pei_be_vec) tb="${tb} pei-arm.lo peigen.lo cofflink.lo " ;;
    arm_pei_le_vec) tb="${tb} pei-arm.lo peigen.lo cofflink.lo " ;;
    arm_pei_epoc_be_vec) tb="${tb} epoc-pei-arm.lo peigen.lo cofflink.lo " ;;
    arm_pei_epoc_le_vec) tb="${tb} epoc-pei-arm.lo peigen.lo cofflink.lo " ;;
    arm_pei_wince_be_vec) tb="${tb} coff-arm.lo pei-arm-wince.lo pei-arm.lo peigen.lo cofflink.lo " ;;
    arm_pei_wince_le_vec) tb="${tb} coff-arm.lo pei-arm-wince.lo pei-arm.lo peigen.lo cofflink.lo " ;;
    armcoff_big_vec) tb="${tb} coff-arm.lo cofflink.lo " ;;
    armcoff_little_vec) tb="${tb} coff-arm.lo cofflink.lo " ;;
    armnetbsd_vec) tb="${tb} armnetbsd.lo aout32.lo" ;;
    armpe_big_vec) tb="${tb} pe-arm.lo peigen.lo cofflink.lo " ;;
    armpe_little_vec) tb="${tb} pe-arm.lo peigen.lo cofflink.lo " ;;
    armpei_big_vec) tb="${tb} pei-arm.lo peigen.lo cofflink.lo " ;;
    armpei_little_vec) tb="${tb} pei-arm.lo peigen.lo cofflink.lo " ;;
    avr_elf32_vec) tb="${tb} elf32-avr.lo elf32.lo ${elf}" ;;
    b_out_vec_big_host) tb="${tb} bout.lo aout32.lo" ;;
    b_out_vec_little_host) tb="${tb} bout.lo aout32.lo" ;;
    bfd_efi_app_ia32_vec) tb="${tb} efi-app-ia32.lo peigen.lo cofflink.lo" ;;
    bfd_efi_app_ia64_vec) tb="${tb} efi-app-ia64.lo pepigen.lo cofflink.lo"; target_size=64 ;;
    bfd_elf32_am33lin_vec) tb="${tb} elf32-am33lin.lo elf32.lo ${elf}" ;;
    bfd_elf32_avr_vec) tb="${tb} elf32-avr.lo elf32.lo ${elf}" ;;
    bfd_elf32_bfin_vec) tb="${tb} elf32-bfin.lo elf32.lo ${elf}" ;;
    bfd_elf32_big_generic_vec) tb="${tb} elf32-gen.lo elf32.lo ${elf}" ;;
    bfd_elf32_bigarc_vec) tb="${tb} elf32-arc.lo elf32.lo ${elf}" ;;
    bfd_elf32_bigarm_vec) tb="${tb} elf32-arm.lo elf32.lo ${elf}" ;;
    bfd_elf32_bigarm_symbian_vec)	
                                tb="${tb} elf32-arm.lo elf32.lo ${elf}" ;;
    bfd_elf32_bigarm_vxworks_vec)	
                                tb="${tb} elf32-arm.lo elf32.lo ${elf}" ;;
    bfd_elf32_bigmips_vec) tb="${tb} elf32-mips.lo elfxx-mips.lo elf32.lo ${elf} ecofflink.lo" ;;
    bfd_elf32_cr16_vec) tb="${tb} elf32-cr16.lo elf32.lo ${elf}" ;;
    bfd_elf32_cr16c_vec) tb="${tb} elf32-cr16c.lo elf32.lo ${elf}" ;;
    bfd_elf32_cris_vec) tb="${tb} elf32-cris.lo elf32.lo ${elf}" ;;
    bfd_elf32_crx_vec) tb="${tb} elf32-crx.lo elf32.lo ${elf}" ;;    
    bfd_elf32_d10v_vec) tb="${tb} elf32-d10v.lo elf32.lo ${elf}" ;;
    bfd_elf32_d30v_vec) tb="${tb} elf32-d30v.lo elf32.lo ${elf}" ;;
    bfd_elf32_dlx_big_vec) tb="${tb} elf32-dlx.lo elf32.lo ${elf}" ;;
    bfd_elf32_epiphany_vec) tb="${tb} elf32-epiphany.lo elf32.lo ${elf}" ;;
    bfd_elf32_fr30_vec) tb="${tb} elf32-fr30.lo elf32.lo ${elf}" ;;
    bfd_elf32_frv_vec) tb="${tb} elf32-frv.lo elf32.lo ${elf}" ;;
    bfd_elf32_frvfdpic_vec) tb="${tb} elf32-frv.lo elf32.lo ${elf}" ;;
    bfd_elf32_h8300_vec) tb="${tb} elf32-h8300.lo elf32.lo ${elf}" ;;
    bfd_elf32_hppa_linux_vec) tb="${tb} elf32-hppa.lo elf32.lo ${elf}" ;;
    bfd_elf32_hppa_nbsd_vec) tb="${tb} elf32-hppa.lo elf32.lo ${elf}" ;;
    bfd_elf32_hppa_vec) tb="${tb} elf32-hppa.lo elf32.lo ${elf}" ;;
    bfd_elf32_i370_vec) tb="${tb} elf32-i370.lo elf32.lo ${elf}" ;;
    bfd_elf32_i386_freebsd_vec) tb="${tb} elf32-i386.lo elf-vxworks.lo elf32.lo ${elf}" ;;
    bfd_elf32_i386_vxworks_vec) tb="${tb} elf32-i386.lo elf-vxworks.lo elf32.lo ${elf}" ;;
    bfd_elf32_i386_vec) tb="${tb} elf32-i386.lo elf-vxworks.lo elf32.lo ${elf}" ;;
    bfd_elf32_i860_little_vec) tb="${tb} elf32-i860.lo elf32.lo ${elf}" ;;
    bfd_elf32_i860_vec) tb="${tb} elf32-i860.lo elf32.lo ${elf}" ;;
    bfd_elf32_i960_vec) tb="${tb} elf32-i960.lo elf32.lo ${elf}" ;;
    bfd_elf32_ia64_big_vec) tb="${tb} elf32-ia64.lo elf32.lo ${elf}" ;;
    bfd_elf32_ia64_hpux_big_vec) tb="${tb} elf32-ia64.lo elf32.lo ${elf}";;
    bfd_elf32_ip2k_vec) tb="${tb} elf32-ip2k.lo elf32.lo ${elf}" ;;
    bfd_elf32_iq2000_vec) tb="${tb} elf32-iq2000.lo elf32.lo ${elf}" ;;
    bfd_elf32_little_generic_vec) tb="${tb} elf32-gen.lo elf32.lo ${elf}" ;;
    bfd_elf32_littlearc_vec) tb="${tb} elf32-arc.lo elf32.lo ${elf}" ;;
    bfd_elf32_littlearm_symbian_vec)
                                tb="${tb} elf32-arm.lo elf32.lo ${elf}" ;;
    bfd_elf32_littlearm_vxworks_vec)
                                tb="${tb} elf32-arm.lo elf32.lo ${elf}" ;;
    bfd_elf32_littlearm_vec) tb="${tb} elf32-arm.lo elf32.lo ${elf}" ;;
    bfd_elf32_littlemips_vec) tb="${tb} elf32-mips.lo elfxx-mips.lo elf32.lo ${elf} ecofflink.lo" ;;
    bfd_elf32_m32c_vec) tb="${tb} elf32-m32c.lo elf32.lo ${elf}" ;;
    bfd_elf32_m32r_vec) tb="${tb} elf32-m32r.lo elf32.lo ${elf}" ;;
    bfd_elf32_m32rle_vec) tb="${tb} elf32-m32r.lo elf32.lo ${elf}" ;;
    bfd_elf32_m32rlin_vec) tb="${tb} elf32-m32r.lo elf32.lo ${elf}" ;;
    bfd_elf32_m32rlelin_vec) tb="${tb} elf32-m32r.lo elf32.lo ${elf}" ;;
    bfd_elf32_m68hc11_vec) tb="${tb} elf32-m68hc11.lo elf32-m68hc1x.lo elf32.lo ${elf}" ;;
    bfd_elf32_m68hc12_vec) tb="${tb} elf32-m68hc12.lo elf32-m68hc1x.lo elf32.lo ${elf}" ;;
    bfd_elf32_m68k_vec) tb="${tb} elf32-m68k.lo elf32.lo ${elf}" ;;
    bfd_elf32_m88k_vec) tb="${tb} elf32-m88k.lo elf32.lo ${elf}" ;;
    bfd_elf32_mcore_big_vec) tb="${tb} elf32-mcore.lo elf32.lo ${elf}" ;;
    bfd_elf32_mcore_little_vec) tb="${tb} elf32-mcore.lo elf32.lo ${elf}" ;;
    bfd_elf32_mn10200_vec) tb="${tb} elf-m10200.lo elf32.lo ${elf}" ;;
    bfd_elf32_mn10300_vec) tb="${tb} elf-m10300.lo elf32.lo ${elf}" ;;
    bfd_elf32_ms1_vec) tb="${tb} elf32-ms1.lo elf32.lo ${elf}" ;;
    bfd_elf32_msp430_vec) tb="${tb} elf32-msp430.lo elf32.lo ${elf}" ;;
    bfd_elf32_nbigmips_vec) tb="${tb} elfn32-mips.lo elfxx-mips.lo elf32.lo ${elf} ecofflink.lo"; target_size=64 ;;
    bfd_elf32_nlittlemips_vec) tb="${tb} elfn32-mips.lo elfxx-mips.lo elf32.lo ${elf} ecofflink.lo"; target_size=64 ;;
    bfd_elf32_ntradbigmips_vec) tb="${tb} elfn32-mips.lo elfxx-mips.lo elf32.lo ${elf} ecofflink.lo"; target_size=64 ;;
    bfd_elf32_ntradlittlemips_vec) tb="${tb} elfn32-mips.lo elfxx-mips.lo elf32.lo ${elf} ecofflink.lo"; target_size=64 ;;
    bfd_elf32_openrisc_vec) tb="${tb} elf32-openrisc.lo elf32.lo ${elf}" ;;
    bfd_elf32_or32_big_vec) tb="${tb} elf32-or32.lo elf32.lo ${elf}" ;;
    bfd_elf32_pj_vec) tb="${tb} elf32-pj.lo elf32.lo ${elf}";;
    bfd_elf32_pjl_vec) tb="${tb} elf32-pj.lo elf32.lo ${elf}";;
    bfd_elf32_powerpc_vec) tb="${tb} elf32-ppc.lo elf-vxworks.lo elf32.lo ${elf}" ;;
    bfd_elf32_powerpcle_vec) tb="${tb} elf32-ppc.lo elf-vxworks.lo elf32.lo ${elf}" ;;
    bfd_elf32_powerpc_vxworks_vec) tb="${tb} elf32-ppc.lo elf-vxworks.lo elf32.lo ${elf}" ;;
    bfd_elf32_s390_vec) tb="${tb} elf32-s390.lo elf32.lo ${elf}" ;;
    # FIXME: We include cofflink.lo not because it is needed for
    # bfd_elf32_sh64[l]_vec, but because we include bfd_elf32_sh[l]_vec
    # which needs it but does not list it. Should be fixed in right place.
    bfd_elf32_sh64_vec) tb="${tb} elf32-sh64.lo elf32-sh64-com.lo elf32.lo ${elf} cofflink.lo" target_size=64 ;;
    bfd_elf32_sh64l_vec) tb="${tb} elf32-sh64.lo elf32-sh64-com.lo elf32.lo ${elf} cofflink.lo" target_size=64 ;;
    bfd_elf32_sh64lin_vec) tb="${tb} elf32-sh64.lo elf32-sh64-com.lo elf32.lo ${elf} cofflink.lo" target_size=64 ;;
    bfd_elf32_sh64blin_vec) tb="${tb} elf32-sh64.lo elf32-sh64-com.lo elf32.lo ${elf} cofflink.lo" target_size=64 ;;
    bfd_elf32_sh64lnbsd_vec) tb="${tb} elf32-sh64.lo elf32-sh64-com.lo elf32.lo ${elf} cofflink.lo" ;;
    bfd_elf32_sh64nbsd_vec) tb="${tb} elf32-sh64.lo elf32-sh64-com.lo elf32.lo ${elf} cofflink.lo" ;;
    bfd_elf32_sh_vec) tb="${tb} elf32-sh.lo elf32.lo ${elf} coff-sh.lo" ;;
    bfd_elf32_shblin_vec) tb="${tb} elf32-sh.lo elf32.lo ${elf} coff-sh.lo cofflink.lo" ;;
    bfd_elf32_shl_vec) tb="${tb} elf32-sh.lo elf32.lo ${elf} coff-sh.lo" ;;
    bfd_elf32_shl_symbian_vec) tb="${tb} elf32-sh-symbian.lo elf32-sh64-com.lo elf32.lo ${elf} coff-sh.lo" ;;
    bfd_elf32_shlin_vec) tb="${tb} elf32-sh.lo elf32.lo ${elf} coff-sh.lo cofflink.lo" ;;
    bfd_elf32_shlnbsd_vec) tb="${tb} elf32-sh.lo elf32.lo ${elf} coff-sh.lo cofflink.lo" ;;
    bfd_elf32_shnbsd_vec) tb="${tb} elf32-sh.lo elf32.lo ${elf} coff-sh.lo cofflink.lo" ;;
    bfd_elf32_sparc_vec) tb="${tb} elf32-sparc.lo elfxx-sparc.lo elf32.lo ${elf}" ;;
    bfd_elf32_tradbigmips_vec) tb="${tb} elf32-mips.lo elfxx-mips.lo elf32.lo ${elf} ecofflink.lo" ;;
    bfd_elf32_tradlittlemips_vec) tb="${tb} elf32-mips.lo elfxx-mips.lo elf32.lo ${elf} ecofflink.lo" ;;
    bfd_elf32_us_cris_vec) tb="${tb} elf32-cris.lo elf32.lo ${elf}" ;;
    bfd_elf32_v850_vec) tb="${tb} elf32-v850.lo elf32.lo ${elf}" ;;
    bfd_elf32_vax_vec) tb="${tb} elf32-vax.lo elf32.lo ${elf}" ;;
    bfd_elf32_xstormy16_vec) tb="${tb} elf32-xstormy16.lo elf32.lo ${elf}" ;;
    bfd_elf32_xtensa_le_vec) tb="${tb} xtensa-isa.lo xtensa-modules.lo elf32-xtensa.lo elf32.lo ${elf}" ;;
    bfd_elf32_xtensa_be_vec) tb="${tb} xtensa-isa.lo xtensa-modules.lo elf32-xtensa.lo elf32.lo ${elf}" ;;
    bfd_elf64_alpha_freebsd_vec) tb="${tb} elf64-alpha.lo elf64.lo ${elf}"; target_size=64 ;;
    bfd_elf64_alpha_vec) tb="${tb} elf64-alpha.lo elf64.lo ${elf}"; target_size=64 ;;
    bfd_elf64_big_generic_vec) tb="${tb} elf64-gen.lo elf64.lo ${elf}"; target_size=64 ;;
    bfd_elf64_bigmips_vec) tb="${tb} elf64-mips.lo elf64.lo elfxx-mips.lo elf32.lo ${elf} ecofflink.lo"; target_size=64 ;;
    bfd_elf64_hppa_linux_vec) tb="${tb} elf64-hppa.lo elf64.lo ${elf}"; target_size=64 ;;
    bfd_elf64_hppa_vec) tb="${tb} elf64-hppa.lo elf64.lo ${elf}"; target_size=64 ;;
    bfd_elf64_ia64_big_vec) tb="${tb} elf64-ia64.lo elf64.lo ${elf}"; target_size=64 ;;
    bfd_elf64_ia64_hpux_big_vec) tb="${tb} elf64-ia64.lo elf64.lo ${elf}"; target_size=64 ;;
    bfd_elf64_ia64_little_vec) tb="${tb} elf64-ia64.lo elf64.lo ${elf}"; target_size=64 ;;
    bfd_elf64_little_generic_vec) tb="${tb} elf64-gen.lo elf64.lo ${elf}"; target_size=64 ;;
    bfd_elf64_littlemips_vec) tb="${tb} elf64-mips.lo elf64.lo elfxx-mips.lo elf32.lo ${elf} ecofflink.lo"; target_size=64 ;;
    bfd_elf64_mmix_vec) tb="${tb} elf64-mmix.lo elf64.lo ${elf}" target_size=64 ;;
    bfd_elf64_powerpc_vec) tb="${tb} elf64-ppc.lo elf64-gen.lo elf64.lo ${elf}"; target_size=64 ;;
    bfd_elf64_powerpcle_vec) tb="${tb} elf64-ppc.lo elf64-gen.lo elf64.lo ${elf}" target_size=64 ;;
    bfd_elf64_s390_vec) tb="${tb} elf64-s390.lo elf64.lo ${elf}"; target_size=64 ;;
    bfd_elf64_sh64_vec) tb="${tb} elf64-sh64.lo elf64.lo ${elf}" target_size=64 ;;
    bfd_elf64_sh64l_vec) tb="${tb} elf64-sh64.lo elf64.lo ${elf}" target_size=64 ;;
    bfd_elf64_sh64lin_vec) tb="${tb} elf64-sh64.lo elf64.lo ${elf}" target_size=64 ;;
    bfd_elf64_sh64blin_vec) tb="${tb} elf64-sh64.lo elf64.lo ${elf}" target_size=64 ;;
    bfd_elf64_sh64lnbsd_vec) tb="${tb} elf64-sh64.lo elf64.lo ${elf}" target_size=64 ;;
    bfd_elf64_sh64nbsd_vec) tb="${tb} elf64-sh64.lo elf64.lo ${elf}" target_size=64 ;;
    bfd_elf64_sparc_vec) tb="${tb} elf64-sparc.lo elfxx-sparc.lo elf64.lo ${elf}"; target_size=64 ;;
    bfd_elf64_tradbigmips_vec) tb="${tb} elf64-mips.lo elf64.lo elfxx-mips.lo elf32.lo ${elf} ecofflink.lo"; target_size=64 ;;
    bfd_elf64_tradlittlemips_vec) tb="${tb} elf64-mips.lo elf64.lo elfxx-mips.lo elf32.lo ${elf} ecofflink.lo"; target_size=64 ;;
    bfd_elf64_x86_64_vec) tb="${tb} elf64-x86-64.lo elf64.lo ${elf}"; target_size=64 ;;
    bfd_mmo_vec) tb="${tb} mmo.lo" target_size=64 ;;
    bfd_powerpc_pe_vec) tb="${tb} pe-ppc.lo peigen.lo cofflink.lo" ;;
    bfd_powerpc_pei_vec) tb="${tb} pei-ppc.lo peigen.lo cofflink.lo" ;;
    bfd_powerpcle_pe_vec) tb="${tb} pe-ppc.lo peigen.lo cofflink.lo" ;;
    bfd_powerpcle_pei_vec) tb="${tb} pei-ppc.lo peigen.lo cofflink.lo" ;;
    bfin_elf32_vec) tb="${tb} elf32-bfin.lo elf32.lo ${elf}" ;;
    bfin_elf32_fdpic_vec) tb="${tb} elf32-bfin.lo elf32.lo ${elf}" ;;
    bout_be_vec) tb="${tb} bout.lo aout32.lo" ;;
    bout_le_vec) tb="${tb} bout.lo aout32.lo" ;;
    cr16_elf32_vec) tb="${tb} elf32-cr16.lo elf32.lo ${elf}" ;;
    cr16c_elf32_vec) tb="${tb} elf32-cr16c.lo elf32.lo ${elf}" ;;
    cris_aout_vec) tb="${tb} aout-cris.lo" ;;
    cris_elf32_vec) tb="${tb} elf32-cris.lo elf32.lo ${elf}" ;;
    cris_elf32_us_vec) tb="${tb} elf32-cris.lo elf32.lo ${elf}" ;;
    crx_elf32_vec) tb="${tb} elf32-crx.lo elf32.lo ${elf}" ;;
    d10v_elf32_vec) tb="${tb} elf32-d10v.lo elf32.lo ${elf}" ;;
    d30v_elf32_vec) tb="${tb} elf32-d30v.lo elf32.lo ${elf}" ;;
    demo_64_vec) tb="${tb} demo64.lo aout64.lo"; target_size=64 ;;
    dlx_elf32_be_vec) tb="${tb} elf32-dlx.lo elf32.lo ${elf}" ;;
    ecoff_big_vec) tb="${tb} coff-mips.lo ecoff.lo ecofflink.lo" ;;
    ecoff_biglittle_vec) tb="${tb} coff-mips.lo ecoff.lo ecofflink.lo" ;;
    ecoff_little_vec) tb="${tb} coff-mips.lo ecoff.lo ecofflink.lo" ;;
    ecoffalpha_little_vec) tb="${tb} coff-alpha.lo ecoff.lo ecofflink.lo"; target_size=64 ;;
    elf32_be_vec) tb="${tb} elf32-gen.lo elf32.lo ${elf}" ;;
    elf32_le_vec) tb="${tb} elf32-gen.lo elf32.lo ${elf}" ;;
    elf64_be_vec) tb="${tb} elf64-gen.lo elf64.lo $elf"; target_size=64 ;;
    elf64_le_vec) tb="${tb} elf64-gen.lo elf64.lo $elf"; target_size=64 ;;
    epiphany_elf32_vec) tb="${tb} elf32-epiphany.lo elf32.lo ${elf}" ;;
    fr30_elf32_vec) tb="${tb} elf32-fr30.lo elf32.lo ${elf}" ;;
    frv_elf32_vec) tb="${tb} elf32-frv.lo elf32.lo ${elf}" ;;
    frv_elf32_fdpic_vec) tb="${tb} elf32-frv.lo elf32.lo ${elf}" ;;
    go32coff_vec) tb="${tb} coff-go32.lo cofflink.lo" ;;
    go32stubbedcoff_vec) tb="${tb} coff-stgo32.lo cofflink.lo" ;;
    h8300coff_vec) tb="${tb} coff-h8300.lo reloc16.lo" ;;
    h8300_coff_vec) tb="${tb} coff-h8300.lo reloc16.lo" ;;
    h8300_elf32_vec) tb="${tb} elf32-h8300.lo elf32.lo ${elf}" ;;
    h8500coff_vec) tb="${tb} coff-h8500.lo reloc16.lo" ;;
    h8500_coff_vec) tb="${tb} coff-h8500.lo reloc16.lo" ;;
    host_aout_vec) tb="${tb} host-aout.lo aout32.lo" ;;
    hp300bsd_vec) tb="${tb} hp300bsd.lo aout32.lo" ;;
    hp300hpux_vec) tb="${tb} hp300hpux.lo aout32.lo" ;;
    hppa_elf32_vec) tb="${tb} elf32-hppa.lo elf32.lo ${elf}" ;;
    hppa_elf32_linux_vec) tb="${tb} elf32-hppa.lo elf32.lo ${elf}" ;;
    hppa_elf32_nbsd_vec) tb="${tb} elf32-hppa.lo elf32.lo ${elf}" ;;
    hppa_elf64_vec) tb="${tb} elf64-hppa.lo elf64.lo $elf"; target_size=64 ;;
    hppa_elf64_linux_vec) tb="${tb} elf64-hppa.lo elf64.lo $elf"; target_size=64 ;;
    hppa_som_vec) tb="${tb} som.lo" ;;
    i370_elf32_vec) tb="${tb} elf32-i370.lo elf32.lo ${elf}" ;;
    i386_aout_vec)tb="${tb} i386aout.lo aout32.lo" ;;
    i386_aout_bsd_vec) tb="${tb} i386bsd.lo aout32.lo" ;;
    i386_aout_dynix_vec) tb="${tb} i386dynix.lo aout32.lo" ;;
    i386_aout_fbsd_vec) tb="${tb} i386freebsd.lo aout32.lo" ;;
    i386_aout_linux_vec) tb="${tb} i386linux.lo aout32.lo" ;;
    i386_aout_lynx_vec) tb="${tb} i386lynx.lo lynx-core.lo aout32.lo" ;;
    i386_aout_mach3_vec) tb="${tb} i386mach3.lo aout32.lo" ;;
    i386_aout_nbsd_vec) tb="${tb} i386netbsd.lo aout32.lo" ;;
    i386_aout_os9k_vec) tb="${tb} i386os9k.lo aout32.lo" ;;
    i386_coff_vec) tb="${tb} coff-i386.lo cofflink.lo" ;;
    i386_coff_go32_vec) tb="${tb} coff-go32.lo cofflink.lo" ;;
    i386_coff_go32stubbed_vec) tb="${tb} coff-stgo32.lo cofflink.lo" ;;
    i386_coff_lynx_vec) tb="${tb} cf-i386lynx.lo cofflink.lo lynx-core.lo" ;;
    i386_elf32_vec) tb="${tb} elf32-i386.lo elf-ifunc.lo elf-nacl.lo elf-vxworks.lo elf32.lo ${elf}" ;;
    i386_elf32_fbsd_vec) tb="${tb} elf32-i386.lo elf-ifunc.lo elf-nacl.lo elf-vxworks.lo elf32.lo ${elf}" ;;
    i386_elf32_nacl_vec) tb="${tb} elf32-i386.lo elf-ifunc.lo elf-nacl.lo elf-vxworks.lo elf32.lo ${elf}" ;;
    i386_elf32_sol2_vec) tb="${tb} elf32-i386.lo elf-ifunc.lo elf-nacl.lo elf-vxworks.lo elf32.lo ${elf}" ;;
    i386_elf32_vxworks_vec) tb="${tb} elf32-i386.lo elf-ifunc.lo elf-nacl.lo elf-vxworks.lo elf32.lo ${elf}" ;;
    i386_mach_o_vec) tb="${tb} mach-o-i386.lo" ;;
    i386_msdos_vec) tb="${tb} i386msdos.lo" ;;
    i386_nlm32_vec) tb="${tb} nlm32-i386.lo nlm32.lo nlm.lo" ;;
    i386_pe_vec) tb="${tb} pe-i386.lo peigen.lo cofflink.lo" ;;
    i386_pei_vec) tb="${tb} pei-i386.lo peigen.lo cofflink.lo" ;;
    i386aout_vec) tb="${tb} i386aout.lo aout32.lo" ;;
    i386bsd_vec) tb="${tb} i386bsd.lo aout32.lo" ;;
    i386coff_vec) tb="${tb} coff-i386.lo cofflink.lo" ;;
    i386dynix_vec) tb="${tb} i386dynix.lo aout32.lo" ;;
    i386freebsd_vec) tb="${tb} i386freebsd.lo aout32.lo" ;;
    i386linux_vec) tb="${tb} i386linux.lo aout32.lo" ;;
    i386lynx_aout_vec) tb="${tb} i386lynx.lo lynx-core.lo aout32.lo" ;;
    i386lynx_coff_vec) tb="${tb} cf-i386lynx.lo cofflink.lo lynx-core.lo" ;;
    i386mach3_vec) tb="${tb} i386mach3.lo aout32.lo" ;;
    i386msdos_vec) tb="${tb} i386msdos.lo" ;;
    i386netbsd_vec) tb="${tb} i386netbsd.lo aout32.lo" ;;
    i386os9k_vec) tb="${tb} i386os9k.lo aout32.lo" ;;
    i386pe_vec) tb="${tb} pe-i386.lo peigen.lo cofflink.lo" ;;
    i386pei_vec) tb="${tb} pei-i386.lo peigen.lo cofflink.lo" ;;
    i860_coff_vec) tb="${tb} coff-i860.lo cofflink.lo" ;;
    i860_elf32_vec) tb="${tb} elf32-i860.lo elf32.lo ${elf}" ;;
    i860_elf32_le_vec) tb="${tb} elf32-i860.lo elf32.lo ${elf}" ;;
    i860coff_vec) tb="${tb} coff-i860.lo cofflink.lo" ;;
    i960_elf32_vec) tb="${tb} elf32-i960.lo elf32.lo ${elf}" ;;
    ia64_elf32_be_vec) tb="${tb} elf32-ia64.lo elfxx-ia64.lo elf32.lo ${elf}" ;;
    ia64_elf32_hpux_be_vec) tb="${tb} elf32-ia64.lo elfxx-ia64.lo elf32.lo ${elf}" ;;
    ia64_elf64_be_vec) tb="${tb} elf64-ia64.lo elfxx-ia64.lo elf64.lo $elf"; target_size=64 ;;
    ia64_elf64_le_vec) tb="${tb} elf64-ia64.lo elfxx-ia64.lo elf64.lo $elf"; target_size=64 ;;
    ia64_elf64_hpux_be_vec) tb="${tb} elf64-ia64.lo elfxx-ia64.lo elf64.lo $elf"; target_size=64 ;;
    ia64_elf64_vms_vec) tb="${tb} elf64-ia64-vms.lo elf64-ia64.lo elfxx-ia64.lo elf64.lo vms-lib.lo vms-misc.lo $elf"; target_size=64 ;;
    ia64_pei_vec) tb="${tb} pei-ia64.lo pepigen.lo cofflink.lo"; target_size=64 ;;
    icoff_be_vec) tb="${tb} coff-i960.lo cofflink.lo" ;;
    icoff_big_vec) tb="${tb} coff-i960.lo cofflink.lo" ;;
    icoff_le_vec) tb="${tb} coff-i960.lo cofflink.lo" ;;
    icoff_little_vec) tb="${tb} coff-i960.lo cofflink.lo" ;;
    ieee_vec) tb="${tb} ieee.lo" ;;
    ip2k_elf32_vec) tb="${tb} elf32-ip2k.lo elf32.lo ${elf}" ;;
    iq2000_elf32_vec) tb="${tb} elf32-iq2000.lo elf32.lo ${elf}" ;;
    k1om_elf64_vec) tb="${tb} elf64-x86-64.lo elf-ifunc.lo elf-nacl.lo elf64.lo $elf"; target_size=64 ;;
    k1om_elf64_fbsd_vec) tb="${tb} elf64-x86-64.lo elf-ifunc.lo elf-nacl.lo elf64.lo $elf"; target_size=64 ;;
    l1om_elf64_vec) tb="${tb} elf64-x86-64.lo elf-ifunc.lo elf-nacl.lo elf64.lo $elf"; target_size=64 ;;
    l1om_elf64_fbsd_vec) tb="${tb} elf64-x86-64.lo elf-ifunc.lo elf-nacl.lo elf64.lo $elf"; target_size=64 ;;
    lm32_elf32_vec) tb="${tb} elf32-lm32.lo elf32.lo ${elf}" ;;
    lm32_elf32_fdpic_vec) tb="${tb} elf32-lm32.lo elf32.lo ${elf}" ;;
    m32c_elf32_vec) tb="${tb} elf32-m32c.lo elf32.lo ${elf}" ;;
    m32r_elf32_vec) tb="${tb} elf32-m32r.lo elf32.lo ${elf}" ;;
    m32r_elf32_le_vec) tb="${tb} elf32-m32r.lo elf32.lo ${elf}" ;;
    m32r_elf32_linux_vec) tb="${tb} elf32-m32r.lo elf32.lo ${elf}" ;;
    m32r_elf32_linux_le_vec) tb="${tb} elf32-m32r.lo elf32.lo ${elf}" ;;
    m68hc11_elf32_vec) tb="${tb} elf32-m68hc11.lo elf32-m68hc1x.lo elf32.lo ${elf}" ;;
    m68hc12_elf32_vec) tb="${tb} elf32-m68hc12.lo elf32-m68hc1x.lo elf32.lo ${elf}" ;;
    m68k_aout_4knbsd_vec) tb="${tb} m68k4knetbsd.lo aout32.lo" ;;
    m68k_aout_hp300bsd_vec) tb="${tb} hp300bsd.lo aout32.lo" ;;
    m68k_aout_hp300hpux_vec) tb="${tb} hp300hpux.lo aout32.lo" ;;
    m68k_aout_linux_vec) tb="${tb} m68klinux.lo aout32.lo" ;;
    m68k_aout_nbsd_vec) tb="${tb} m68knetbsd.lo aout32.lo" ;;
    m68k_aout_newsos3_vec) tb="${tb} newsos3.lo aout32.lo" ;;
    m68k_coff_vec) tb="${tb} coff-m68k.lo cofflink.lo" ;;
    m68k_coff_apollo_vec) tb="${tb} coff-apollo.lo" ;;
    m68k_coff_aux_vec) tb="${tb} coff-aux.lo coff-m68k.lo cofflink.lo" ;;
    m68k_coff_sysv_vec) tb="${tb} coff-svm68k.lo cofflink.lo" ;;
    m68k_coff_un_vec) tb="${tb} coff-u68k.lo coff-m68k.lo cofflink.lo" ;;
    m68k_elf32_vec) tb="${tb} elf32-m68k.lo elf32.lo ${elf}" ;;
    m68k_versados_vec) tb="${tb} versados.lo" ;;
    m68k4knetbsd_vec) tb="${tb} m68k4knetbsd.lo aout32.lo" ;;
    m68kaux_coff_vec) tb="${tb} coff-aux.lo coff-m68k.lo cofflink.lo" ;;
    m68kcoff_vec) tb="${tb} coff-m68k.lo cofflink.lo" ;;
    m68kcoffun_vec) tb="${tb} coff-u68k.lo coff-m68k.lo cofflink.lo" ;;
    m68klinux_vec) tb="${tb} m68klinux.lo aout32.lo" ;;
    m68knetbsd_vec) tb="${tb} m68knetbsd.lo aout32.lo" ;;
    m68ksysvcoff_vec) tb="${tb} coff-svm68k.lo cofflink.lo" ;;
    m88k_aout_mach3_vec) tb="${tb} m88kmach3.lo aout32.lo" ;;
    m88k_aout_obsd_vec) tb="${tb} m88kopenbsd.lo aout32.lo" ;;
    m88k_coff_bcs_vec) tb="${tb} coff-m88k.lo" ;;
    m88k_elf32_vec) tb="${tb} elf32-m88k.lo elf32.lo ${elf}" ;;
    m88kbcs_vec) tb="${tb} coff-m88k.lo" ;;
    m88kmach3_vec) tb="${tb} m88kmach3.lo aout32.lo" ;;
    m88kopenbsd_vec) tb="${tb} m88kopenbsd.lo aout32.lo" ;;
    maxqcoff_vec) tb="${tb} coff-maxq.lo" ;;
    mach_o_be_vec) tb="${tb} mach-o.lo" ;;
    mach_o_le_vec) tb="${tb} mach-o.lo" ;;
    mach_o_fat_vec) tb="${tb} mach-o.lo" ;;
    mcore_elf32_be_vec) tb="${tb} elf32-mcore.lo elf32.lo ${elf}" ;;
    mcore_elf32_le_vec) tb="${tb} elf32-mcore.lo elf32.lo ${elf}" ;;
    mcore_pe_be_vec) tb="${tb} pe-mcore.lo peigen.lo cofflink.lo" ;;
    mcore_pe_big_vec) tb="${tb} pe-mcore.lo peigen.lo cofflink.lo" ;;
    mcore_pe_le_vec) tb="${tb} pe-mcore.lo peigen.lo cofflink.lo" ;;
    mcore_pe_little_vec) tb="${tb} pe-mcore.lo peigen.lo cofflink.lo" ;;
    mcore_pei_be_vec) tb="${tb} pei-mcore.lo peigen.lo cofflink.lo" ;;
    mcore_pei_big_vec) tb="${tb} pei-mcore.lo peigen.lo cofflink.lo" ;;
    mcore_pei_le_vec) tb="${tb} pei-mcore.lo peigen.lo cofflink.lo" ;;
    mcore_pei_little_vec) tb="${tb} pei-mcore.lo peigen.lo cofflink.lo" ;;
    mep_elf32_vec) tb="${tb} elf32-mep.lo elf32.lo ${elf}" ;;
    mep_elf32_le_vec) tb="${tb} elf32-mep.lo elf32.lo ${elf}" ;;
    metag_elf32_vec) tb="${tb} elf32-metag.lo elf32.lo ${elf}" ;;
    microblaze_elf32_vec) tb="${tb} elf32-microblaze.lo elf32.lo ${elf}" ;;
    microblaze_elf32_le_vec) tb="${tb} elf32-microblaze.lo elf32.lo ${elf}" ;;
    mips_ecoff_be_vec) tb="${tb} coff-mips.lo ecoff.lo ecofflink.lo" ;;
    mips_ecoff_le_vec) tb="${tb} coff-mips.lo ecoff.lo ecofflink.lo" ;;
    mips_ecoff_bele_vec) tb="${tb} coff-mips.lo ecoff.lo ecofflink.lo" ;;
    mips_elf32_be_vec) tb="${tb} elf32-mips.lo elfxx-mips.lo elf-vxworks.lo elf32.lo ${elf} ecofflink.lo" ;;
    mips_elf32_le_vec) tb="${tb} elf32-mips.lo elfxx-mips.lo elf-vxworks.lo elf32.lo ${elf} ecofflink.lo" ;;
    mips_elf32_n_be_vec) tb="${tb} elfn32-mips.lo elfxx-mips.lo elf-vxworks.lo elf32.lo ${elf} ecofflink.lo"; target_size=64 ;;
    mips_elf32_n_le_vec) tb="${tb} elfn32-mips.lo elfxx-mips.lo elf-vxworks.lo elf32.lo ${elf} ecofflink.lo"; target_size=64 ;;
    mips_elf32_ntrad_be_vec) tb="${tb} elfn32-mips.lo elfxx-mips.lo elf-vxworks.lo elf32.lo ${elf} ecofflink.lo"; target_size=64 ;;
    mips_elf32_ntrad_le_vec) tb="${tb} elfn32-mips.lo elfxx-mips.lo elf-vxworks.lo elf32.lo ${elf} ecofflink.lo"; target_size=64 ;;
    mips_elf32_ntradfbsd_be_vec) tb="${tb} elfn32-mips.lo elfxx-mips.lo elf-vxworks.lo elf32.lo ${elf} ecofflink.lo"; target_size=64 ;;
    mips_elf32_ntradfbsd_le_vec) tb="${tb} elfn32-mips.lo elfxx-mips.lo elf-vxworks.lo elf32.lo ${elf} ecofflink.lo"; target_size=64 ;;
    mips_elf32_trad_be_vec) tb="${tb} elf32-mips.lo elfxx-mips.lo elf-vxworks.lo elf32.lo ${elf} ecofflink.lo" ;;
    mips_elf32_trad_le_vec) tb="${tb} elf32-mips.lo elfxx-mips.lo elf-vxworks.lo elf32.lo ${elf} ecofflink.lo" ;;
    mips_elf32_tradfbsd_be_vec) tb="${tb} elf32-mips.lo elfxx-mips.lo elf-vxworks.lo elf32.lo ${elf} ecofflink.lo" ;;
    mips_elf32_tradfbsd_le_vec) tb="${tb} elf32-mips.lo elfxx-mips.lo elf-vxworks.lo elf32.lo ${elf} ecofflink.lo" ;;
    mips_elf32_vxworks_be_vec) tb="${tb} elf32-mips.lo elfxx-mips.lo elf-vxworks.lo elf32.lo ${elf} ecofflink.lo" ;;
    mips_elf32_vxworks_le_vec) tb="${tb} elf32-mips.lo elfxx-mips.lo elf-vxworks.lo elf32.lo ${elf} ecofflink.lo" ;;
    mips_elf64_be_vec) tb="${tb} elf64-mips.lo elf64.lo elfxx-mips.lo elf-vxworks.lo elf32.lo ${elf} ecofflink.lo"; target_size=64 ;;
    mips_elf64_le_vec) tb="${tb} elf64-mips.lo elf64.lo elfxx-mips.lo elf-vxworks.lo elf32.lo ${elf} ecofflink.lo"; target_size=64 ;;
    mips_elf64_trad_be_vec) tb="${tb} elf64-mips.lo elf64.lo elfxx-mips.lo elf-vxworks.lo elf32.lo ${elf} ecofflink.lo"; target_size=64 ;;
    mips_elf64_trad_le_vec) tb="${tb} elf64-mips.lo elf64.lo elfxx-mips.lo elf-vxworks.lo elf32.lo ${elf} ecofflink.lo"; target_size=64 ;;
    mips_elf64_tradfbsd_be_vec) tb="${tb} elf64-mips.lo elf64.lo elfxx-mips.lo elf-vxworks.lo elf32.lo ${elf} ecofflink.lo"; target_size=64 ;;
    mips_elf64_tradfbsd_le_vec) tb="${tb} elf64-mips.lo elf64.lo elfxx-mips.lo elf-vxworks.lo elf32.lo ${elf} ecofflink.lo"; target_size=64 ;;
    mips_pe_le_vec) tb="${tb} pe-mips.lo peigen.lo cofflink.lo" ;;
    mips_pei_le_vec) tb="${tb} pei-mips.lo peigen.lo cofflink.lo" ;;
    mipslpe_vec) tb="${tb} pe-mips.lo peigen.lo cofflink.lo" ;;
    mipslpei_vec) tb="${tb} pei-mips.lo peigen.lo cofflink.lo" ;;
    mmix_elf64_vec) tb="${tb} elf64-mmix.lo elf64.lo ${elf}" target_size=64 ;;
    mmix_mmo_vec) tb="${tb} mmo.lo" target_size=64 ;;
    mn10200_elf32_vec) tb="${tb} elf-m10200.lo elf32.lo ${elf}" ;;
    mn10300_elf32_vec) tb="${tb} elf-m10300.lo elf32.lo ${elf}" ;;
    moxie_elf32_be_vec) tb="${tb} elf32-moxie.lo elf32.lo ${elf}" ;;
    moxie_elf32_le_vec) tb="${tb} elf32-moxie.lo elf32.lo ${elf}" ;;
    msp430_elf32_vec) tb="${tb} elf32-msp430.lo elf32.lo ${elf}" ;;
    msp430_elf32_ti_vec) tb="${tb} elf32-msp430.lo elf32.lo ${elf}" ;;
    mt_elf32_vec) tb="${tb} elf32-mt.lo elf32.lo ${elf}" ;;
    nds32_elf32_be_vec) tb="${tb} elf32-nds32.lo elf32.lo ${elf}" ;;
    nds32_elf32_le_vec) tb="${tb} elf32-nds32.lo elf32.lo ${elf}" ;;
    nds32_elf32_linux_be_vec) tb="${tb} elf32-nds32.lo elf32.lo ${elf}" ;;
    nds32_elf32_linux_le_vec) tb="${tb} elf32-nds32.lo elf32.lo ${elf}" ;;
    newsos3_vec) tb="${tb} newsos3.lo aout32.lo" ;;
    nios2_elf32_be_vec) tb="${tb} elf32-nios2.lo elf32.lo ${elf}" ;;
    nios2_elf32_le_vec) tb="${tb} elf32-nios2.lo elf32.lo ${elf}" ;;
    nlm32_alpha_vec) tb="${tb} nlm32-alpha.lo nlm32.lo nlm.lo"; target_size=64 ;;
    nlm32_i386_vec) tb="${tb} nlm32-i386.lo nlm32.lo nlm.lo" ;;
    nlm32_powerpc_vec) tb="${tb} nlm32-ppc.lo nlm32.lo nlm.lo" ;;
    nlm32_sparc_vec) tb="${tb} nlm32-sparc.lo nlm32.lo nlm.lo" ;;
    ns32k_aout_pc532mach_vec) tb="${tb} pc532-mach.lo aout-ns32k.lo" ;;
    ns32k_aout_pc532nbsd_vec) tb="${tb} ns32knetbsd.lo aout-ns32k.lo" ;;
    or1k_elf32_vec) tb="${tb} elf32-or1k.lo elf32.lo ${elf}" ;;
    or32coff_big_vec) tb="${tb} coff-or32.lo cofflink.lo" ;;
    pc532machaout_vec) tb="${tb} pc532-mach.lo aout-ns32k.lo" ;;
    pc532netbsd_vec) tb="${tb} ns32knetbsd.lo aout-ns32k.lo" ;;
    pdp11_aout_vec) tb="${tb} pdp11.lo" ;;
    pef_vec) tb="${tb} pef.lo" ;;
    pef_xlib_vec) tb="${tb} pef.lo" ;;
    pj_elf32_vec) tb="${tb} elf32-pj.lo elf32.lo ${elf}" ;;
    pj_elf32_le_vec) tb="${tb} elf32-pj.lo elf32.lo ${elf}" ;;
    plugin_vec) tb="${tb} plugin.lo" ;;
    pmac_xcoff_vec) tb="${tb} coff-pmac.lo coff-rs6000.lo xcofflink.lo" ;;
    pmac_xcoff64_vec) tb="${tb} coff-pmac.lo coff64-rs6000.lo xcofflink.lo" ;;
    powerpc_boot_vec) tb="${tb} ppcboot.lo" ;;
    powerpc_elf32_vec) tb="${tb} elf32-ppc.lo elf-vxworks.lo elf32.lo ${elf}" ;;
    powerpc_elf32_le_vec) tb="${tb} elf32-ppc.lo elf-vxworks.lo elf32.lo ${elf}" ;;
    powerpc_elf32_fbsd_vec) tb="${tb} elf32-ppc.lo elf-vxworks.lo elf32.lo ${elf}" ;;
    powerpc_elf32_vxworks_vec) tb="${tb} elf32-ppc.lo elf-vxworks.lo elf32.lo ${elf}" ;;
    powerpc_elf64_vec) tb="${tb} elf64-ppc.lo elf64-gen.lo elf64.lo $elf"; target_size=64 ;;
    powerpc_elf64_le_vec) tb="${tb} elf64-ppc.lo elf64-gen.lo elf64.lo ${elf}" target_size=64 ;;
    powerpc_elf64_fbsd_vec) tb="${tb} elf64-ppc.lo elf64-gen.lo elf64.lo ${elf}" target_size=64 ;;
    powerpc_nlm32_vec) tb="${tb} nlm32-ppc.lo nlm32.lo nlm.lo" ;;
    powerpc_pe_vec) tb="${tb} pe-ppc.lo peigen.lo cofflink.lo" ;;
    powerpc_pe_le_vec) tb="${tb} pe-ppc.lo peigen.lo cofflink.lo" ;;
    powerpc_pei_vec) tb="${tb} pei-ppc.lo peigen.lo cofflink.lo" ;;
    powerpc_pei_le_vec) tb="${tb} pei-ppc.lo peigen.lo cofflink.lo" ;;
    powerpc_xcoff_vec) tb="${tb} coff-rs6000.lo xcofflink.lo" ;;
    powerpc_xcoff64_vec) tb="${tb} coff64-rs6000.lo xcofflink.lo" ;;
    ppcboot_vec) tb="${tb} ppcboot.lo" ;;
    riscix_vec) tb="${tb} aout32.lo riscix.lo" ;;
    rl78_elf32_vec) tb="${tb} elf32-rl78.lo elf32.lo ${elf}" ;;
    rs6000_xcoff64_vec) tb="${tb} coff64-rs6000.lo xcofflink.lo aix5ppc-core.lo"; target_size=64 ;;
    rs6000_xcoff64_aix_vec) tb="${tb} coff64-rs6000.lo xcofflink.lo aix5ppc-core.lo"; target_size=64 ;;
    rs6000_xcoff_vec) tb="${tb} coff-rs6000.lo xcofflink.lo" ;;
    rs6000coff64_vec) tb="${tb} coff64-rs6000.lo xcofflink.lo aix5ppc-core.lo"; target_size=64 ;;
    rs6000coff_vec) tb="${tb} coff-rs6000.lo xcofflink.lo" ;;
    rx_elf32_be_vec) tb="${tb} elf32-rx.lo elf32.lo ${elf}" ;;
    rx_elf32_be_ns_vec)tb="${tb} elf32-rx.lo elf32.lo ${elf}" ;;
    rx_elf32_le_vec) tb="${tb} elf32-rx.lo elf32.lo ${elf}" ;;
    s390_elf32_vec) tb="${tb} elf32-s390.lo elf32.lo ${elf}" ;;
    s390_elf64_vec) tb="${tb} elf64-s390.lo elf64.lo $elf"; target_size=64 ;;
    score_elf32_be_vec) tb="${tb} elf32-score.lo elf32-score7.lo elf32.lo $elf"; want64=true; target_size=64 ;;
    score_elf32_le_vec) tb="${tb} elf32-score.lo elf32-score7.lo elf32.lo $elf"; want64=true; target_size=64 ;;
    # FIXME: We include cofflink.lo not because it is needed for
    # sh64_elf32[_le]_vec, but because we include sh_elf32[_le]_vec
    # which needs it but does not list it.  Should be fixed in right place.
    sh64_elf32_vec) tb="${tb} elf32-sh64.lo elf32-sh64-com.lo elf-vxworks.lo elf32.lo ${elf} cofflink.lo" target_size=64 ;;
    sh64_elf32_le_vec) tb="${tb} elf32-sh64.lo elf32-sh64-com.lo elf-vxworks.lo elf32.lo ${elf} cofflink.lo" target_size=64 ;;
    sh64_elf32_linux_vec) tb="${tb} elf32-sh64.lo elf32-sh64-com.lo elf-vxworks.lo elf32.lo ${elf} cofflink.lo" target_size=64 ;;
    sh64_elf32_linux_be_vec) tb="${tb} elf32-sh64.lo elf32-sh64-com.lo elf-vxworks.lo elf32.lo ${elf} cofflink.lo" target_size=64 ;;
    sh64_elf32_nbsd_vec) tb="${tb} elf32-sh64.lo elf32-sh64-com.lo elf-vxworks.lo elf32.lo ${elf} cofflink.lo" ;;
    sh64_elf32_nbsd_le_vec) tb="${tb} elf32-sh64.lo elf32-sh64-com.lo elf-vxworks.lo elf32.lo ${elf} cofflink.lo" ;;
    sh64_elf64_vec) tb="${tb} elf64-sh64.lo elf64.lo ${elf}" target_size=64 ;;
    sh64_elf64_le_vec) tb="${tb} elf64-sh64.lo elf64.lo ${elf}" target_size=64 ;;
    sh64_elf64_linux_vec) tb="${tb} elf64-sh64.lo elf64.lo ${elf}" target_size=64 ;;
    sh64_elf64_linux_be_vec) tb="${tb} elf64-sh64.lo elf64.lo ${elf}" target_size=64 ;;
    sh64_elf64_nbsd_vec) tb="${tb} elf64-sh64.lo elf64.lo ${elf}" target_size=64 ;;
    sh64_elf64_nbsd_le_vec) tb="${tb} elf64-sh64.lo elf64.lo ${elf}" target_size=64 ;;
    sh_coff_vec) tb="${tb} coff-sh.lo cofflink.lo" ;;
    sh_coff_le_vec) tb="${tb} coff-sh.lo cofflink.lo" ;;
    sh_coff_small_vec) tb="${tb} coff-sh.lo cofflink.lo" ;;
    sh_coff_small_le_vec) tb="${tb} coff-sh.lo cofflink.lo" ;;
    sh_elf32_vec) tb="${tb} elf32-sh.lo elf-vxworks.lo elf32.lo ${elf} coff-sh.lo" ;;
    sh_elf32_le_vec) tb="${tb} elf32-sh.lo elf-vxworks.lo elf32.lo ${elf} coff-sh.lo" ;;
    sh_elf32_fdpic_be_vec) tb="${tb} elf32-sh.lo elf-vxworks.lo elf32.lo ${elf} coff-sh.lo cofflink.lo" ;;
    sh_elf32_fdpic_le_vec) tb="${tb} elf32-sh.lo elf-vxworks.lo elf32.lo ${elf} coff-sh.lo cofflink.lo" ;;
    sh_elf32_linux_vec) tb="${tb} elf32-sh.lo elf-vxworks.lo elf32.lo ${elf} coff-sh.lo cofflink.lo" ;;
    sh_elf32_linux_be_vec) tb="${tb} elf32-sh.lo elf-vxworks.lo elf32.lo ${elf} coff-sh.lo cofflink.lo" ;;
    sh_elf32_nbsd_vec) tb="${tb} elf32-sh.lo elf-vxworks.lo elf32.lo ${elf} coff-sh.lo cofflink.lo" ;;
    sh_elf32_nbsd_le_vec) tb="${tb} elf32-sh.lo elf-vxworks.lo elf32.lo ${elf} coff-sh.lo cofflink.lo" ;;
    sh_elf32_symbian_le_vec) tb="${tb} elf32-sh-symbian.lo elf32-sh64-com.lo elf-vxworks.lo elf32.lo ${elf} coff-sh.lo" ;;
    sh_elf32_vxworks_vec) tb="${tb} elf32-sh.lo elf-vxworks.lo elf32.lo ${elf} coff-sh.lo" ;;
    sh_elf32_vxworks_le_vec) tb="${tb} elf32-sh.lo elf-vxworks.lo elf32.lo ${elf} coff-sh.lo" ;;
    sh_pe_le_vec) tb="${tb} pe-sh.lo coff-sh.lo peigen.lo cofflink.lo" ;;
    sh_pei_le_vec) tb="${tb} pei-sh.lo coff-sh.lo peigen.lo cofflink.lo" ;;
    shcoff_small_vec) tb="${tb} coff-sh.lo cofflink.lo" ;;
    shcoff_vec) tb="${tb} coff-sh.lo cofflink.lo" ;;
    shlcoff_small_vec) tb="${tb} coff-sh.lo cofflink.lo" ;;
    shlcoff_vec) tb="${tb} coff-sh.lo cofflink.lo" ;;
    shlpe_vec) tb="${tb} pe-sh.lo coff-sh.lo peigen.lo cofflink.lo" ;;
    shlpei_vec) tb="${tb} pei-sh.lo coff-sh.lo peigen.lo cofflink.lo" ;;
    som_vec) tb="${tb} som.lo" ;;
    sparc_aout_le_vec)	tb="${tb} aout-sparcle.lo aout32.lo" ;;
    sparc_aout_linux_vec) tb="${tb} sparclinux.lo aout32.lo" ;;
    sparc_aout_lynx_vec) tb="${tb} sparclynx.lo lynx-core.lo aout32.lo" ;;
    sparc_aout_nbsd_vec) tb="${tb} sparcnetbsd.lo aout32.lo" ;;
    sparc_aout_sunos_be_vec) tb="${tb} sunos.lo aout32.lo" ;;
    sparc_coff_vec) tb="${tb} coff-sparc.lo" ;;
    sparc_coff_lynx_vec) tb="${tb} cf-sparclynx.lo lynx-core.lo" ;;
    sparc_elf32_vec) tb="${tb} elf32-sparc.lo elfxx-sparc.lo elf-vxworks.lo elf32.lo ${elf}" ;;
    sparc_elf32_sol2_vec) tb="${tb} elf32-sparc.lo elfxx-sparc.lo elf-vxworks.lo elf32.lo ${elf}" ;;
    sparc_elf32_vxworks_vec) tb="${tb} elf32-sparc.lo elfxx-sparc.lo elf-vxworks.lo elf32.lo ${elf}" ;;
    sparc_elf64_vec) tb="${tb} elf64-sparc.lo elfxx-sparc.lo elf-vxworks.lo elf64.lo $elf"; target_size=64 ;;
    sparc_elf64_fbsd_vec) tb="${tb} elf64-sparc.lo elfxx-sparc.lo elf-vxworks.lo elf64.lo $elf"; target_size=64 ;;
    sparc_elf64_sol2_vec) tb="${tb} elf64-sparc.lo elfxx-sparc.lo elf-vxworks.lo elf64.lo $elf"; target_size=64 ;;
    sparc_nlm32_vec) tb="${tb} nlm32-sparc.lo nlm32.lo nlm.lo" ;;
    sparccoff_vec) tb="${tb} coff-sparc.lo" ;;
    sparcle_aout_vec) tb="${tb} aout-sparcle.lo aout32.lo" ;;
    sparclinux_vec) tb="${tb} sparclinux.lo aout32.lo" ;;
    sparclynx_aout_vec)	tb="${tb} sparclynx.lo lynx-core.lo aout32.lo" ;;
    sparclynx_coff_vec)	tb="${tb} cf-sparclynx.lo lynx-core.lo" ;;
    sparcnetbsd_vec) tb="${tb} sparcnetbsd.lo aout32.lo" ;;
    spu_elf32_vec) tb="${tb} elf32-spu.lo elf32.lo ${elf}" ;;
    sunos_big_vec) tb="${tb} sunos.lo aout32.lo" ;;
    sym_vec) tb="${tb} xsym.lo" ;;
    tic30_aout_vec) tb="${tb} aout-tic30.lo" ;;
    tic30_coff_vec) tb="${tb} coff-tic30.lo" ;;
    tic4x_coff0_vec) tb="${tb} coff-tic4x.lo" ;;
    tic4x_coff0_beh_vec) tb="${tb} coff-tic4x.lo" ;;
    tic4x_coff1_vec) tb="${tb} coff-tic4x.lo" ;;
    tic4x_coff1_beh_vec) tb="${tb} coff-tic4x.lo" ;;
    tic4x_coff2_vec) tb="${tb} coff-tic4x.lo" ;;
    tic4x_coff2_beh_vec) tb="${tb} coff-tic4x.lo" ;;
    tic54x_coff0_beh_vec) tb="${tb} coff-tic54x.lo" ;;
    tic54x_coff0_vec) tb="${tb} coff-tic54x.lo" ;;
    tic54x_coff1_beh_vec) tb="${tb} coff-tic54x.lo" ;;
    tic54x_coff1_vec) tb="${tb} coff-tic54x.lo" ;;
    tic54x_coff2_beh_vec) tb="${tb} coff-tic54x.lo" ;;
    tic54x_coff2_vec) tb="${tb} coff-tic54x.lo" ;;
    tic6x_elf32_be_vec) tb="${tb} elf32-tic6x.lo elf32.lo ${elf}" ;;
    tic6x_elf32_le_vec) tb="${tb} elf32-tic6x.lo elf32.lo ${elf}" ;;
    tic6x_elf32_c6000_be_vec) tb="${tb} elf32-tic6x.lo elf32.lo ${elf}" ;;
    tic6x_elf32_c6000_le_vec) tb="${tb} elf32-tic6x.lo elf32.lo ${elf}" ;;
    tic6x_elf32_linux_be_vec) tb="${tb} elf32-tic6x.lo elf32.lo ${elf}" ;;
    tic6x_elf32_linux_le_vec) tb="${tb} elf32-tic6x.lo elf32.lo ${elf}" ;;
    tic80coff_vec) tb="${tb} coff-tic80.lo cofflink.lo" ;;
    tic80_coff_vec) tb="${tb} coff-tic80.lo cofflink.lo" ;;
    tilegx_elf32_be_vec) tb="${tb} elf32-tilegx.lo elfxx-tilegx.lo elf32.lo ${elf}" ; target_size=32 ;;
    tilegx_elf32_le_vec) tb="${tb} elf32-tilegx.lo elfxx-tilegx.lo elf32.lo ${elf}" ; target_size=32 ;;
    tilegx_elf64_be_vec) tb="${tb} elf64-tilegx.lo elfxx-tilegx.lo elf64.lo ${elf}" ; target_size=64 ;;
    tilegx_elf64_le_vec) tb="${tb} elf64-tilegx.lo elfxx-tilegx.lo elf64.lo ${elf}" ; target_size=64 ;;
    tilepro_elf32_vec) tb="${tb} elf32-tilepro.lo elf32.lo ${elf}" ;;
    v800_elf32_vec) tb="${tb} elf32-v850.lo elf32.lo ${elf}" ;;
    v850_elf32_vec) tb="${tb} elf32-v850.lo elf32.lo ${elf}" ;;
    vax_aout_1knbsd_vec) tb="${tb} vax1knetbsd.lo aout32.lo" ;;
    vax_aout_bsd_vec) tb="${tb} vaxbsd.lo aout32.lo" ;;
    vax_aout_nbsd_vec) tb="${tb} vaxnetbsd.lo aout32.lo" ;;
    vax_elf32_vec) tb="${tb} elf32-vax.lo elf32.lo ${elf}" ;;
    vaxnetbsd_vec) tb="${tb} vaxnetbsd.lo aout32.lo" ;;
    vax1knetbsd_vec) tb="${tb} vax1knetbsd.lo aout32.lo" ;;
    vaxbsd_vec) tb="${tb} vaxbsd.lo aout32.lo" ;;
    versados_vec) tb="${tb} versados.lo" ;;
    vms_alpha_vec) tb="${tb} vms.lo vms-hdr.lo vms-gsd.lo vms-tir.lo vms-misc.lo"; target_size=64 ;;
    vms_vax_vec) tb="${tb} vms.lo vms-hdr.lo vms-gsd.lo vms-tir.lo vms-misc.lo" ;;
    w65_coff_vec) tb="${tb} coff-w65.lo reloc16.lo" ;;
    w65_vec) tb="${tb} coff-w65.lo reloc16.lo" ;;
    we32k_coff_vec) tb="${tb} coff-we32k.lo" ;;
    we32kcoff_vec) tb="${tb} coff-we32k.lo" ;;
    x86_64_coff_vec) tb="${tb} coff-x86_64.lo cofflink.lo"; target_size=64 ;;
    x86_64_elf32_vec) tb="${tb} elf64-x86-64.lo elf-ifunc.lo elf-nacl.lo elf64.lo elf32.lo $elf"; target_size=64 ;;
    x86_64_elf32_nacl_vec) tb="${tb} elf64-x86-64.lo elf-ifunc.lo elf-nacl.lo elf64.lo elf32.lo $elf"; target_size=64 ;;
    x86_64_elf64_vec) tb="${tb} elf64-x86-64.lo elf-ifunc.lo elf-nacl.lo elf64.lo $elf"; target_size=64 ;;
    x86_64_elf64_fbsd_vec) tb="${tb} elf64-x86-64.lo elf-ifunc.lo elf-nacl.lo elf64.lo $elf"; target_size=64 ;;
    x86_64_elf64_nacl_vec) tb="${tb} elf64-x86-64.lo elf-ifunc.lo elf-nacl.lo elf64.lo $elf"; target_size=64 ;;
    x86_64_elf64_sol2_vec) tb="${tb} elf64-x86-64.lo elf-ifunc.lo elf-nacl.lo elf64.lo $elf"; target_size=64 ;;
    x86_64_mach_o_vec) if test -n "${BREAK_MY_BUILD}"; then tb="${tb} mach-o-x86-64.lo"; fi ;;
    x86_64_pe_vec) tb="${tb} pe-x86_64.lo pex64igen.lo cofflink.lo"; target_size=64 ;;
    x86_64_pe_be_vec) tb="${tb} pe-x86_64.lo pex64igen.lo cofflink.lo"; target_size=64 ;;
    x86_64_pei_vec) tb="${tb} pei-x86_64.lo pex64igen.lo cofflink.lo"; target_size=64 ;;
    xc16x_elf32_vec) tb="${tb} elf32-xc16x.lo elf32.lo ${elf}" ;;
    xgate_elf32_vec) tb="${tb} elf32-xgate.lo elf32.lo ${elf}" ;;
    xstormy16_elf32_vec) tb="${tb} elf32-xstormy16.lo elf32.lo ${elf}" ;;
    xtensa_elf32_be_vec) tb="${tb} xtensa-isa.lo xtensa-modules.lo elf32-xtensa.lo elf32.lo ${elf}" ;;
    xtensa_elf32_le_vec) tb="${tb} xtensa-isa.lo xtensa-modules.lo elf32-xtensa.lo elf32.lo ${elf}" ;;
    z80_coff_vec) tb="${tb} coff-z80.lo reloc16.lo" ;;
    z8k_coff_vec) tb="${tb} coff-z8k.lo reloc16.lo cofflink.lo" ;;
    z8kcoff_vec) tb="${tb} coff-z8k.lo reloc16.lo" ;;

    # These appear out of order in targets.c
    srec_vec) tb="${tb} srec.lo" ;;
    symbolsrec_vec) tb="${tb} srec.lo" ;;
    tekhex_vec) tb="${tb} tekhex.lo" ;;
    cisco_core_big_vec) tb="${tb} cisco-core.lo" ;;
    cisco_core_little_vec) tb="${tb} cisco-core.lo" ;;
    core_cisco_be_vec) tb="${tb} cisco-core.lo" ;;
    core_cisco_le_vec) tb="${tb} cisco-core.lo" ;;

    "")			;;
    *) AC_MSG_ERROR([*** unknown target vector ${vec}]) ;;
    esac

    if test "x${target_size}" = "x64"; then
	target64=true
    fi
    if test x"${vec}" = x"${defvec}"; then
	bfd_default_target_size=${target_size}
    fi
done

# Target architecture .o files.
# A couple of CPUs use shorter file names to avoid problems on DOS
# filesystems.
ta=`echo ${selarchs} | sed -e s/bfd_/cpu-/g -e s/_arch/.lo/g -e s/mn10200/m10200/ -e s/mn10300/m10300/`

# Weed out duplicate .o files, first among the target backend ones:
f=""
for i in ${tb}; do
    case " ${f} " in
    *" ${i} "*) ;;
    *) f="${f} ${i}" ;;
    esac
done
tb="${f}"

# Likewise with the duplicate .o file among the target architecture ones:
f=""
for i in ${ta}; do
    case " ${f} " in
    *" ${i} "*) ;;
    *) f="${f} ${i}" ;;
    esac
done
ta="${f}"

bfd_backends="${tb}"
bfd_machines="${ta}"

if test "x${all_targets}" = "xtrue"; then
  bfd_backends="${bfd_backends}"' $(ALL_BACKENDS)'
  bfd_machines="${bfd_machines}"' $(ALL_MACHINES)'
  selvecs=""
  havevecs="-DHAVE_all_vecs"
  selarchs=""
  test -n "${assocvecs}" &&
    assocvecs=`echo ${assocvecs} | sed -e 's/^/\&/' -e 's/ \(.\)/,\&\1/g'`
else	# all_targets is true
  # Only set these if they will be nonempty, for the clever echo.
  havevecs=""
  assocvecs=""
  test -n "${selvecs}" &&
    havevecs=`echo ${selvecs} | sed -e 's/^/-DHAVE_/' -e 's/ \(.\)/ -DHAVE_\1/g'`
  test -n "${selvecs}" &&
    selvecs=`echo ${selvecs} | sed -e 's/^/\&/' -e 's/ \(.\)/,\&\1/g'`
  test -n "${selarchs}" &&
    selarchs=`echo ${selarchs} | sed -e 's/^/\&/' -e 's/ \(.\)/,\&\1/g'`
fi	# all_targets is true

case ${host64}-${target64}-${want64} in
  *true*)
    wordsize=64
    bfd_libs='$(BFD64_LIBS) $(BFD32_LIBS)'
    bfd64_libs='$(BFD64_LIBS)'
    all_backends='$(BFD64_BACKENDS) $(BFD32_BACKENDS)'
    if test ${BFD_HOST_64_BIT_DEFINED} = 0; then
      AC_MSG_WARN([You have requested a 64 bit BFD configuration, but])
      AC_MSG_WARN([your compiler may not have a 64 bit integral type])
    fi
    if test -n "${GCC}"; then
       bad_64bit_gcc="no";
       AC_MSG_CHECKING([for gcc version with buggy 64-bit support])
       # Add more tests for gcc versions with non-working 64-bit support
       # here.
       AC_EGREP_CPP([: 2 : 91 : 1 :],[:__GNUC__:__GNUC_MINOR__:__i386__:],
		    [bad_64bit_gcc="yes";
		     AC_MSG_RESULT([yes: egcs-1.1.2 on ix86 spotted])],
		    [AC_MSG_RESULT([no])])
       if test "x${bad_64bit_gcc}" = "xyes"; then
	 AC_MSG_ERROR([A newer version of gcc is needed for the requested 64-bit BFD configuration])
       fi
    fi
    ;;
  false-false-false)
    wordsize=32
    bfd_libs='$(BFD32_LIBS)'
    all_backends='$(BFD32_BACKENDS)'
    ;;
esac

if test "x${wordsize}" = "x"; then
  test -z "${wordsize}"
  AC_MSG_WARN([wordsize never got set])
fi

AC_SUBST([wordsize])dnl
AC_SUBST([bfd_libs])dnl
AC_SUBST([bfd64_libs])dnl
AC_SUBST([all_backends])dnl
AC_SUBST([bfd_backends])dnl
AC_SUBST([bfd_machines])dnl
AC_SUBST([bfd_default_target_size])dnl

if test "x${plugins}" = "xyes"; then
  supports_plugins=1
else
  supports_plugins=0
fi
AC_SUBST([supports_plugins])dnl
AC_SUBST([lt_cv_dlopen_libs])dnl

# Determine the host-dependent file_ptr a.k.a. off_t type. In order
# prefer: off64_t - if ftello64 and fseeko64, off_t - if ftello and
# fseeko, long. This assumes that sizeof off_t is .ge. sizeof long.
# Hopefully a reasonable assumption since fseeko et.al. should be
# upward compatible.
AC_CHECK_FUNCS([ftello ftello64 fseeko fseeko64 fopen64])dnl
  ## the types we check are based off of which functions we found:
if test x"${ac_cv_func_ftello}" = xyes -a x"${ac_cv_func_fseeko}" = xyes; then
    AC_CHECK_SIZEOF([off_t])dnl
    AC_CHECK_ALIGNOF([off_t])dnl
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
AC_MSG_CHECKING([file_ptr type])
bfd_file_ptr="long"
bfd_ufile_ptr="unsigned long"
if test x"${ac_cv_func_ftello64}" = x"yes" -a x"${ac_cv_func_fseeko64}" = x"yes" \
	-o x"${ac_cv_sizeof_off_t}" = x"8"; then
    bfd_file_ptr=BFD_HOST_64_BIT
    bfd_ufile_ptr=BFD_HOST_U_64_BIT
fi
AC_MSG_RESULT([${bfd_file_ptr}])
AC_SUBST([bfd_file_ptr])dnl
AC_SUBST([bfd_ufile_ptr])dnl

tdefaults=""
test -n "${defvec}" && tdefaults="${tdefaults} -DDEFAULT_VECTOR=${defvec}"
test -n "${selvecs}" && tdefaults="${tdefaults} -DSELECT_VECS='${selvecs}'"
test -n "${assocvecs}" && tdefaults="${tdefaults} -DASSOCIATED_VECS='${assocvecs}'"
test -n "${selarchs}" && tdefaults="${tdefaults} -DSELECT_ARCHITECTURES='${selarchs}'"
test -n "${havevecs}" && tdefaults="${tdefaults} ${havevecs}"
AC_SUBST([tdefaults])dnl
AC_SUBST([havevecs])dnl

# APPLE LOCAL:  mmap() always works right on Darwin.  Take our word for it.
case "${host}" in
  *-apple-darwin* | *-apple-macos*)
    ac_cv_func_mmap_fixed_mapped=yes
  ;;
esac
AC_CHECK_HEADERS_ONCE([sys/mman.h])dnl
AC_CHECK_FUNCS([madvise mprotect])dnl
  ## define based on availability:
case ${want_mmap}+${ac_cv_func_mmap_fixed_mapped} in
  true+yes )  AC_DEFINE([USE_MMAP],[1],[Use mmap if it is available?]) ;;
esac

## kinda like the vpath-related hack, but slightly different:
if test -f doc/config.status; then
  AC_MSG_NOTICE([doc/config.status is already present, removing it.])
  rm -f doc/config.status
fi
## Output.
AC_CONFIG_FILES([Makefile \
                 doc/Makefile \
                 bfd-in3.h:bfd-in2.h \
                 po/Makefile.in:po/Make-in])dnl

AC_CONFIG_COMMANDS([aaa_config_status_init],
[[echo "config.status is working"]],[[]])dnl

AC_CONFIG_COMMANDS([default],
[[echo "dealing with our po-subdir"
sed -e '/SRC-POTFILES =/r po/SRC-POTFILES' -e '/BLD-POTFILES =/r po/BLD-POTFILES' po/Makefile.in > po/Makefile]],[[]])dnl

dnl# We need this duplication, even though we use the standard po-subdirs
dnl# autoconf macro, because of our two separate POTFILES. Yuck.
AC_CONFIG_COMMANDS([default-2],
[[echo "dealing with our two separate POTFILES"
case "${srcdir}" in
  .) srcdirpre='' ;;
  *) srcdirpre='$(srcdir)/' ;;
esac
POFILES=""
GMOFILES=""
for lang in dummy ${OBSOLETE_ALL_LINGUAS}; do
  if test "x${lang}" != "xdummy"; then
    POFILES="${POFILES} ${srcdirpre}${lang}.po"
    GMOFILES="${GMOFILES} ${srcdirpre}${lang}.gmo"
  fi
done
if test ! -e po/Makefile; then
  sed -e '/SRC-POTFILES =/r po/SRC-POTFILES' \
      -e '/BLD-POTFILES =/r po/BLD-POTFILES' \
      -e "s,@POFILES@,${POFILES}," \
      -e "s,@GMOFILES@,${GMOFILES}," \
      po/Makefile.in > po/Makefile
fi]],[[]])dnl

AC_CONFIG_COMMANDS([config_status_vpath_build_fix],[
if test "${srcdir}" != "."; then
  if test -e ${srcdir}/config.status -a ! -e `pwd`/config.status; then
    cp -v ${srcdir}/config.status `pwd`/config.status || echo ""
  elif test -e `pwd`/config.status -a ! -e ${srcdir}/config.status; then
    cp -v `pwd`/config.status ${srcdir}/config.status || echo ""
  fi
fi
],[])dnl

AC_CONFIG_COMMANDS([bfd_h_vpath_fix],[
if test ! -e bfd.h; then
  if test -e doc/bfd.h && test -z "`grep @BFD_HOST_64_BIT_DEFINED@ doc/bfd.h`"; then
    cp -v doc/bfd.h bfd.h || echo ""
  elif test "${srcdir}" != "."; then
    if test -e ${srcdir}/doc/bfd.h && test -z "`grep @BFD_HOST_64_BIT_DEFINED@ ${srcdir}/doc/bfd.h`"; then
      cp -v ${srcdir}/doc/bfd.h bfd.h || echo ""
    elif test -e ${srcdir}/bfd.h -a "${srcdir}" != "."; then
      cp -v ${srcdir}/bfd.h bfd.h || echo ""
    fi
  fi
fi
],[])dnl

dnl# does the reverse of the previous one:
AC_CONFIG_COMMANDS([bfd_h_vpath_fix2],[
if test ! -e ${srcdir}/bfd.h; then
  if test -e ${srcdir}/doc/bfd.h && test -z "`grep @BFD_HOST_64_BIT_DEFINED@ ${srcdir}/doc/bfd.h`"; then
    cp -v ${srcdir}/doc/bfd.h ${srcdir}/bfd.h || echo ""
  elif test "${srcdir}" != "."; then
    if test -e doc/bfd.h && test -z "`grep @BFD_HOST_64_BIT_DEFINED@ doc/bfd.h`"; then
      cp -v doc/bfd.h ${srcdir}/bfd.h || echo ""
    elif test -e bfd.h -a "${srcdir}" != "."; then
      cp -v bfd.h ${srcdir}/bfd.h || echo ""
    fi
  fi
fi
],[])dnl

AC_CONFIG_COMMANDS([libtool_lengthening_hack_fix1],[
# Hack for when the configure script generates a libtool that is too short:
if test -e libtool -a -e libtool.old; then
  stat libtool > /dev/null
  stat libtool.old > /dev/null
changequote(,)
  if test "$(wc -l < libtool.old | tr -d [:blank:])" -gt "$(wc -l < libtool | tr -d [:blank:])"; then
changequote([,])
    echo "libtool.old is longer than libtool"
    echo "that means that it is probably the better one to use"
    cp -vf libtool.old libtool
  else
    echo "libtool.old is NOT longer than libtool"
    echo "leaving it alone..."
  fi
elif test -e libtool.old -a ! -e libtool; then
  stat libtool.old > /dev/null
  echo "libtool is not present, but libtool.old is"
  echo "so use libtool.old instead"
  cp -v libtool.old libtool
elif test -e libtool -a ! -e libtool.old; then
  stat libtool > /dev/null
  echo "using default libtool"
elif test ! -e libtool -a ! -e libtool.old; then
  echo "no libtool found"
else
  echo "this should not happen"
fi
],[])dnl

AC_CONFIG_COMMANDS([libtool_lengthening_hack_fix2],[
# Also try the hack from ../opcodes here as well:
changequote(,)
# The "1000" is just an arbitrarily chosen number that is usually right
if test "$(wc -l < libtool | tr -d [:blank:])" -lt "1000"; then
changequote([,])
  echo "even after all that libtool is still too short"
  echo "trying to create a new one in a different way this time"
  if test -x ../ltconfig; then
    # I have no clue why this is even a thing that I have to check for:
    if test -e /mount/endor/wildenhu/local-x86_64/bin/sed; then
      if test -r ../ltmain.sh; then
        echo "../ltconfig ../ltmain.sh"
        ../ltconfig ../ltmain.sh
      elif test -r ../ltmain-old.sh; then
        echo "../ltconfig ../ltmain-old.sh"
        ../ltconfig ../ltmain-old.sh
      elif test -r ../ltcf-c.sh; then
        echo "../ltconfig ../ltcf-c.sh"
        ../ltconfig ../ltcf-c.sh
      else
        echo "not sure which ltmain.sh to use to do so"
      fi
    else
      echo "actually never mind (for now)"
    fi
  else
    echo "unable to do so: ../ltconfig is missing and/or unusable"
  fi
fi
],[])dnl

AC_CONFIG_COMMANDS([broken_libtool_hack_fix3],[[
if test -x ./config.status; then
  if test -z "${our_libtool_script}"; then
    if test -x ./libtool; then
      our_libtool_script="./libtool"
    elif test -n "${srcdir}" -a -x ${srcdir}/libtool; then
      our_libtool_script="${srcdir}/libtool"
    else
      our_libtool_script=libtool
    fi
  fi
  if test -z "${LIBTOOL}"; then
    if test -x /bin/sh; then
      LIBTOOL="/bin/sh ${our_libtool_script}"
    else
      LIBTOOL="${our_libtool_script}"
    fi
  fi  
  if (${LIBTOOL} --config | ${GREP} "objdir="); then
    my_libtooldir=`${LIBTOOL} --config | ${SED} -n -e 's/^objdir=//p'`
    if test -n "${my_libtooldir}"; then
      echo "${our_libtool_script} already defines objdir: '${my_libtooldir}'"
    else
      echo "error: my_libtooldir should be defined if ${our_libtool_script} already defines objdir" >&2
      exit 1
    fi
  elif test -e ${our_libtool_script}; then
    echo "" >> ${our_libtool_script}
    if test -n "${lt_cv_objdir}"; then
      echo "using lt_cv_objdir ('${lt_cv_objdir}') to define objdir in ${our_libtool_script}"
      echo "objdir=${lt_cv_objdir}" >> ${our_libtool_script}
    elif test -n "${objdir}"; then
      echo "using objdir ('${objdir}') to define objdir in ${our_libtool_script}"
      echo "objdir=${objdir}" >> ${our_libtool_script}
    elif test -n "${libtooldir}"; then
      echo "using libtooldir ('${libtooldir}') to define objdir in ${our_libtool_script}"
      echo "objdir=${libtooldir}" >> ${our_libtool_script}
    elif test -d .libs; then
      echo "using .libs to define objdir in ${our_libtool_script}"
      echo "objdir=.libs" >> ${our_libtool_script}
    elif test -n "${srcdir}" -a -d ${srcdir}/.libs; then
      echo "using ${srcdir}/.libs to define objdir in ${our_libtool_script}"
      echo "objdir=${srcdir}/.libs" >> ${our_libtool_script}
    else
      echo "warning: need to define objdir in ${our_libtool_script}, but cannot figure out how to do so"
    fi
  else
    echo "I hate libtool..."
  fi
fi
]],[])dnl

dnl# Required by html, pdf, install-pdf, and install-html:
AC_SUBST([datarootdir])dnl
AC_SUBST([docdir])dnl
AC_SUBST([htmldir])dnl
AC_SUBST([pdfdir])dnl

if test "x${no_create}" = "xyes"; then
  if test "x${silent}" != "xyes" || test "x${silent}" = "xno"; then
    echo "skipping re-creating configured files"
  fi
fi

# Output happens for real now.
AC_OUTPUT

echo "Done configuring bfd."
