dnl#                                               -*- Autoconf -*-
dnl# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])dnl
AC_INIT([electric-fence],[2.0.5],[Bruce@Pixar.com])
AC_CONFIG_SRCDIR([efence.c])dnl
AC_CONFIG_HEADERS([config.h])dnl

# pieces of automake that make sense here:
AM_MAINTAINER_MODE
AM_SANITY_CHECK

# Checks for programs.
AC_PROG_CC
AC_PROG_CPP
AC_ARG_VAR([CLANG_ANALYZER],[Path to the clang static analyzer])dnl
AC_CACHE_CHECK([for the clang static analyzer],[ac_cv_path_CLANG_ANALYZER],
  [AC_PATH_PROGS_FEATURE_CHECK([CLANG_ANALYZER],
    [clang clang++ clang-mp-3.5 clang-mp-3.4 clang-mp-3.3 clang-mp-3.2],
    [[${ac_path_CLANG_ANALYZER} --analyze /dev/null > /dev/null 2>&1 && \
      ac_cv_path_CLANG_ANALYZER=${ac_path_CLANG_ANALYZER}
      ac_path_CLANG_ANALYZER_found=:]],
    [AC_MSG_WARN([we will not be able to do static analysis with clang])],
    [${PATH}])dnl# end program check
  ])dnl# end cache check
  ## (need this extra line here)
AC_SUBST([CLANG_ANALYZER],[${ac_cv_path_CLANG_ANALYZER}])dnl
AC_PROG_INSTALL
AC_PROG_LN_S
AM_MISSING_PROG([MAKEINFO],[makeinfo])dnl
AC_PATH_PROG([CONFIGURED_TEX],[tex])dnl
AC_PROG_MKDIR_P
m4_ifdef([AM_PROG_AR],[
  AM_PROG_AR
],[
  AC_CHECK_TOOL([AR],[ar])dnl
])dnl
m4_ifdef([AC_PROG_RANLIB],[
  AC_PROG_RANLIB
],[
  AC_CHECK_TOOL([RANLIB],[ranlib],[:])dnl
])dnl
AC_PATH_PROG([NROFF_PROG],[nroff])dnl
AC_PATH_PROG([SHAR_PROG],[shar])dnl

# Checks for libraries.
AC_CHECK_LIB([pthread],[pthread_exit])dnl

# Checks for library functions.
AC_FUNC_MMAP

# Checks for header files.
AC_DEFUN([AC_REQUIRE_HEADER_STDC],[
  AC_REQUIRE([AC_HEADER_STDC])dnl
])dnl
AC_REQUIRE_HEADER_STDC
AC_CHECK_HEADERS([errno.h fcntl.h limits.h mach/mach.h malloc.h \
                  malloc/malloc.h math.h pthread.h setjmp.h signal.h \
                  stddef.h stdio.h])dnl

# Checks for declarations.
AC_CHECK_DECLS([sys_nerr])dnl
AC_CHECK_DECLS([sys_errlist])dnl

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
AC_ARG_PROGRAM
AC_EXEEXT
AC_SUBST([EXEEXT])dnl

case "${host}" in
*-apple-rhapsody*|*-apple-macos10*|*-apple-darwin*) PAGEFILE=nspage ;;
*-*-winnt*|*-*-cygwin*) PAGEFILE=ntpage ;;
*) PAGEFILE=page ;;
esac

# Checks for typedefs, structures, and compiler characteristics.
AC_SYS_LARGEFILE
AC_C_BIGENDIAN
AC_C_CONST
AC_C_PROTOTYPES
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_CHECK_TYPES([caddr_t])dnl
AC_CHECK_TYPES([kern_return_t])dnl
AC_CHECK_TYPES([long long, unsigned long long],
               [if test "x${ac_cv_sizeof_long_long}" = "x"; then
                  test -z "${ac_cv_sizeof_long_long}"
                  AC_CHECK_SIZEOF([long long])
                fi
                if test "x${ac_cv_alignof_long_long}" = "x"; then
                  test -z "${ac_cv_alignof_long_long}"
                  AC_CHECK_ALIGNOF([long long])
                fi
                if test "x${ac_cv_sizeof_unsigned_long_long}" = "x"; then
                  test -z "${ac_cv_sizeof_unsigned_long_long}"
                  AC_CHECK_SIZEOF([unsigned long long])
                fi
                if test "x${ac_cv_alignof_unsigned_long_long}" = "x"; then
                  test -z "${ac_cv_alignof_unsigned_long_long}"
                  AC_CHECK_ALIGNOF([unsigned long long])
                fi
                AC_DEFINE([USE_LONG_LONG],[1],
                          [Define to 1 on 64-bit systems])
                export DEFS="${DEFS} -DUSE_LONG_LONG"])dnl

# Checks for library functions not checked above.
unset ac_cv_header_stdlib_h
AC_FUNC_MALLOC
unset ac_cv_func_getpagesize
AC_CHECK_FUNCS([atoi calloc exit _exit free getpagesize getpid memset \
                mprotect munmap strerror write])dnl

# Output.
AC_SUBST([PAGEFILE])dnl
AC_CONFIG_FILES([Makefile])dnl
AC_OUTPUT
