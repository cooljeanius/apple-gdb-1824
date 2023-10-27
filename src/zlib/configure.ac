dnl#                                               -*- Autoconf -*-
dnl# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
dnl# This is a hack because autoconf finds the AC\_INIT from
dnl# ../config/override.m4 first if it is present, which is annoying.
dnl# Feel free to re-define to whichever version of autoconf
dnl# that you are using:
m4_define([_GCC_AUTOCONF_VERSION],[2.71])
AC_INIT([zlib],[1.2.8],[jloup@gzip.org])
AC_CONFIG_SRCDIR([adler32.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

if test -n "${with_target_subdir}" && test -d "${with_target_subdir}"; then
  AM_ENABLE_MULTILIB([],[..])
fi

# architecture stuff:
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

# This works around an automake problem.
mkinstalldirs="`cd ${ac_aux_dir} && ${PWDCMD-pwd}`/mkinstalldirs"
AC_SUBST([mkinstalldirs])dnl

AC_REQUIRE_AUX_FILE([mkinstalldirs])dnl
AC_REQUIRE_AUX_FILE([depcomp])dnl

AM_INIT_AUTOMAKE([1.11.6 foreign dejagnu no-dist no-installinfo])dnl

AM_MAINTAINER_MODE

dnl# We use these options to decide which functions to include:
AC_ARG_WITH([target-subdir],
[AS_HELP_STRING([--with-target-subdir=SUBDIR],
                [Configuring in a subdirectory for target])])dnl

if test -z "${with_target_subdir}" || test "${with_target_subdir}" = "."; then
   COMPPATH=.
else
   COMPPATH=..
fi
AC_SUBST([COMPPATH])dnl

AC_ARG_WITH([cross-host],
[AS_HELP_STRING([--with-cross-host=HOST],
                [configuring with a cross compiler])])dnl

dnl# Default to --enable-multilib
AC_ARG_ENABLE([multilib],
[AS_HELP_STRING([--enable-multilib],
                [build many library versions (default)])],
[case "${enableval}" in
  yes) multilib=yes ;;
  no)  multilib=no ;;
  *)   AC_MSG_ERROR([bad value ${enableval} for multilib option]) ;;
 esac],[test -z "${with_target_subdir}" && multilib=no || multilib=yes])dnl

AC_ARG_WITH([system-zlib],
[AS_HELP_STRING([--with-system-zlib],[use installed libz])])dnl

# Make sure we do NOT test executables when making cross-tools:
GCC_NO_EXECUTABLES

AC_EXEEXT
AC_USE_SYSTEM_EXTENSIONS
AC_SYS_LARGEFILE
if test -n "${BUILD_ARCHITECTURE}"; then
  ACX_PROG_CC_WARNING_OPTS(["-arch ${BUILD_ARCHITECTURE}"],
                           [C_BUILDARCHFLAGS])
  AC_SUBST([C_BUILDARCHFLAGS])
fi
if test -n "${HOST_ARCHITECTURE}"; then
  ACX_PROG_CC_WARNING_OPTS(["-arch ${HOST_ARCHITECTURE}"],
                           [C_ARCHFLAGS])
  AC_SUBST([C_ARCHFLAGS])
fi
if test -n "${TARGET_ARCHITECTURE}"; then
  ACX_PROG_CC_WARNING_OPTS(["-arch ${TARGET_ARCHITECTURE}"],
                           [C_TARGARCHFLAGS])
  AC_SUBST([C_TARGARCHFLAGS])
fi

# Checks for programs.
if test "${ARFLAGS}" = "-o"; then
    if test "${AR}" = "ar"; then
        unset ARFLAGS
    elif test "${ac_cv_prog_AR}" = "ar"; then
        unset ac_cv_prog_AR
        unset ARFLAGS
    elif test "${am_cv_ar_interface}" = "ar"; then
        unset am_cv_ar_interface
        unset ARFLAGS
    fi
fi
if test "x${ac_cv_path_SED}" = "x"; then
  test -z "${ac_cv_path_SED}"
  AC_PROG_SED
fi
if test -e ./config.cache; then
    sed -i "s|ac_cv_prog_AR|bad_AR|g" ./config.cache
    sed -i "s|am_cv_ar_interface|bad_ar_interface|g" ./config.cache
fi
AM_PROG_AR
AM_PROG_AS
if test "x${ac_cv_prog_AWK}" = "x"; then
  test -z "${ac_cv_prog_AWK}"
  AC_PROG_AWK
fi
if test "x${CC}" = "x"; then
  test -z "${CC}"
  # The same as in boehm-gc and libstdc++. Have to borrow it from there.
  # We must force CC to /not/ be precious variables; otherwise
  # the wrong, non-multilib-adjusted value will be used in multilibs.
  # As a side effect, we have to subst CFLAGS ourselves.

  m4_rename([_AC_ARG_VAR_PRECIOUS],[real_PRECIOUS])
  m4_define([_AC_ARG_VAR_PRECIOUS],[])
  AC_PROG_CC
  m4_rename_force([real_PRECIOUS],[_AC_ARG_VAR_PRECIOUS])
  AC_SUBST([CFLAGS])dnl
  ## end compiler check
else
  test ! -z "${CC}"
  AC_SUBST([CC])dnl
  ## need to do something here...
fi

AC_LANG([C])dnl

# libtool:
LT_INIT([win32-dll])dnl
LT_LANG([C])dnl
LT_PREREQ([2.4.6])dnl

# Find CPP now so that any conditional tests below will NOT do it and
# thereby make the resulting definitions conditional:
AC_PROG_CPP

# More checks for programs:
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_LN_S
if test "x${ac_cv_prog_make_make_set}" = "x"; then
  test -z "${ac_cv_prog_make_make_set}"
  AC_PROG_MAKE_SET
fi
AC_PROG_MKDIR_P
if test "x${RANLIB}" = "x" && test "x${ac_cv_prog_ac_ct_RANLIB}" = "x"; then
  test -z "${RANLIB}" && test -z "${ac_cv_prog_ac_ct_RANLIB}"
  AC_PROG_RANLIB
fi
AC_PROG_OBJC
CMAKE_FIND_BINARY
AC_SYS_INTERPRETER

if test -n "${with_cross_host}"; then
   dnl# We are being configured with a cross compiler.  AC_REPLACE_FUNCS
   dnl# may not work correctly, because the compiler may not be able to
   dnl# link executables.

   # We assume newlib.  This lets us hard-code the functions we know
   # we will have.
   AC_DEFINE([HAVE_MEMCPY])dnl
   AC_DEFINE([HAVE_STRERROR])dnl

   # We ignore --with-system-zlib in this case:
   target_all=libzgcj.la
else
   # check stuff:
   AC_FUNC_MMAP
   # other functions:
   AC_CHECK_FUNCS([memcpy strerror])dnl

   if test "x${with_system_zlib}" = "xyes"; then
      AC_CHECK_LIB([z],[deflate],[target_all=""],[target_all=libzgcj.la])
   else
      target_all=libzgcj.la
   fi
fi

AC_SUBST([target_all])dnl

# Checks for libraries.
AC_CHECK_LIB([c],[printf])dnl
AC_CHECK_LIB([z],[gzopen])dnl

# Checks for header files.
AC_HEADER_DIRENT
AC_HEADER_STDBOOL dnl# also "_CHECK"s it
AC_HEADER_TIME
AC_CHECK_HEADERS([Carbon/Carbon.h CHUD/CHUD.h direct.h fcntl.h io.h \
                  limits.h locale.h mach/host_info.h mach/mach.h \
                  mach/mach_error.h mach/mach_syscalls.h mach/mach_time.h \
                  mach/mach_types.h mach/message.h malloc.h \
                  malloc/malloc.h math.h stdarg.h stddef.h stdio.h \
                  sys/sysctl.h sys/time.h utime.h windows.h])dnl
  ## probably unnecessary:
if test "x${ac_cv_header_unistd_h}" = "x"; then
  test -z "${ac_cv_header_unistd_h}"
  AC_CHECK_HEADERS([unistd.h])
fi

dnl# using ${host_os} instead of just ${host} leaves out the architecture
AC_MSG_CHECKING([cpu-specific header for ${host}])
case ${host} in
  *86*-apple-darwin*)
    AC_MSG_RESULT([ ])
    AC_CHECK_HEADERS([System/i386/cpu_capabilities.h])
    ;;
  *pc*-apple-darwin*)
    AC_MSG_RESULT([ ])
    AC_CHECK_HEADERS([System/ppc/cpu_capabilities.h])
    ;;
  *arm*-apple-darwin*)
    AC_MSG_RESULT([ ])
    AC_CHECK_HEADERS([arm/arch.h])
    ;;
  *darwin*)
    AC_MSG_RESULT([ ])
    AC_CHECK_HEADERS([cpu_capabilities.h])
    ;;
  *)
    AC_MSG_RESULT([not applicable])
    ;;
esac

# Checks for typedefs, structures, and compiler characteristics.
AC_C_BIGENDIAN
AC_C_INLINE
AC_C_PROTOTYPES
AC_C_VOLATILE
AC_TYPE_INT32_T
AC_TYPE_OFF_T
AC_CHECK_TYPES([kern_return_t])dnl
AC_CHECK_TYPES([ptrdiff_t])dnl
AC_CHECK_TYPES([off64_t])dnl
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T

# Checks for library functions.
AC_FUNC_CHOWN
AC_FUNC_ERROR_AT_LINE
AC_FUNC_FSEEKO
AC_FUNC_MALLOC
if test "x${ac_cv_func_mmap}" = "x" && test "x${ac_cv_func_mmap_fixed_mapped}" = "x"; then
  test -z "${ac_cv_func_mmap}" && test -z "${ac_cv_func_mmap_fixed_mapped}"
  AC_FUNC_MMAP
fi
AC_FUNC_REALLOC
AC_CHECK_FUNCS([access atexit chmod exit fprintf fseeko ftruncate \
                gettimeofday localeconv memchr memmove memset mkdir \
                munmap snprintf sprintf strchr strdup strrchr strtol \
                system utime vsnprintf vsprintf])dnl
  ## just in case:
if test "x${ac_cv_func_strerror}" = "x"; then
  test -z "${ac_cv_func_strerror}"
  AC_CHECK_FUNCS([strerror])
fi

# Output.
if test -n "${with_cross_host}" &&
   test x"${with_cross_host}" != x"no"; then
  toolexecdir='$(exec_prefix)/$(target_alias)'
  toolexeclibdir='$(toolexecdir)/lib'
else
  toolexecdir='$(libdir)/gcc-lib/$(target_alias)'
  toolexeclibdir='$(libdir)'
fi
if test "x${GCC}" = "xyes" && ${CC} -print-multi-os-directory > /dev/null 2>&1; then
  multiosdir=/`${CC} -print-multi-os-directory`
  case ${multiosdir} in
    /.) multiosdir="" ;; # Avoid trailing /.
  esac
else
  multiosdir=""
fi
toolexeclibdir="${toolexeclibdir}${multiosdir}"
AC_SUBST([toolexecdir])dnl
AC_SUBST([toolexeclibdir])dnl

AM_CONDITIONAL([TARGET_LIBRARY],
   [test -n "${with_target_subdir}" && test -d "${with_target_subdir}"])dnl

if test "x${multilib}" = "xyes"; then
  multilib_arg="--enable-multilib"
else
  multilib_arg=""
fi

AC_ARG_ENABLE([host-shared],
[AS_HELP_STRING([--enable-host-shared],
		[build host code as shared libraries])],[
case "${host}" in
*-apple-darwin*) PIC_FLAG="-DPIC -fPIC -fno-common" ;;
*) PIC_FLAG=-fPIC ;;
esac
],[PICFLAG=""])dnl
AC_SUBST([PICFLAG])dnl

ACX_PROG_CC_WARNING_OPTS([-Wall -Wextra -Wwrite-strings -Wpointer-arith \
-Wfloat-conversion -Wstrict-prototypes -Wmissing-prototypes -Wimplicit \
-Wnested-externs -Wmissing-parameter-type -Wabi -Wswitch -Wparentheses \
-Wzero-length-bounds -Wfatal-errors])
ACX_PROG_CC_WARNING_ALMOST_PEDANTIC([-Wno-long-long])dnl

# Only enable with --enable-werror-always until existing warnings are
# corrected.
ACX_PROG_CC_WARNINGS_ARE_ERRORS([manual])dnl

# Final output.
AC_CONFIG_FILES([Makefile Makefile_orig])
AC_CONFIG_COMMANDS([original],[
if test ! -e Makefile.am || test -e Makefile_orig.in; then
  (test -x configure.sh && sh configure.sh) || echo "No original script"
fi
],[])
AC_CONFIG_COMMANDS([cleanup],[
rm -rf ztest*.dSYM || rmdir ztest*.dSYM || echo "No ztest junk to clean"
],[])
AC_CONFIG_SUBDIRS([contrib/minizip])
AC_OUTPUT
