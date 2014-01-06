#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([zlib],[1.2.8],[jloup@gzip.org])
AC_CONFIG_SRCDIR([adler32.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
AC_CANONICAL_TARGET
AC_EXEEXT
AC_SYS_LARGEFILE

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
AC_PROG_SED
if test -e ./config.cache; then
    sed -i "s|ac_cv_prog_AR|bad_AR|g" ./config.cache
    sed -i "s|am_cv_ar_interface|bad_ar_interface|g" ./config.cache
fi
AM_PROG_AR
AM_PROG_AS
AC_PROG_AWK
if test "x${CC}" = "x"; then
	test -z "${CC}"
	AC_PROG_CC
else
	test ! -z "${CC}"
	AC_SUBST([CC])
fi
AC_PROG_CPP
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_MKDIR_P
AC_PROG_RANLIB
AC_PROG_OBJC
CMAKE_FIND_BINARY
AC_SYS_INTERPRETER

# Checks for libraries.
AC_CHECK_LIB([c],[printf])
AC_CHECK_LIB([z],[gzopen])

# Checks for header files.
AC_HEADER_DIRENT
AC_HEADER_STDBOOL dnl# also "_CHECK"s it
AC_HEADER_TIME
AC_CHECK_HEADERS([Carbon/Carbon.h CHUD/CHUD.h direct.h fcntl.h io.h \
                  limits.h mach/host_info.h mach/mach.h mach/mach_error.h \
                  mach/mach_syscalls.h mach/mach_time.h mach/mach_types.h \
                  mach/message.h malloc.h malloc/malloc.h math.h \
                  stdarg.h stddef.h stdio.h sys/sysctl.h sys/time.h \
                  utime.h windows.h])

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
AC_CHECK_TYPES([kern_return_t])
AC_CHECK_TYPES([ptrdiff_t])
AC_CHECK_TYPES([off64_t])
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T

# Checks for library functions.
AC_FUNC_CHOWN
AC_FUNC_ERROR_AT_LINE
AC_FUNC_FSEEKO
AC_FUNC_MALLOC
AC_FUNC_MMAP
AC_FUNC_REALLOC
AC_CHECK_FUNCS([access atexit chmod exit fprintf fseeko ftruncate \
                gettimeofday memchr memmove memset mkdir munmap \
                snprintf sprintf strchr strdup strerror strrchr strtol \
                system utime vsnprintf vsprintf])

AC_CONFIG_FILES([Makefile])
AC_CONFIG_COMMANDS([original],[
test -e configure.sh && sh configure.sh
],[])
AC_CONFIG_SUBDIRS([contrib/minizip])
AC_OUTPUT
