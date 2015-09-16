dnl#                                               -*- Autoconf -*-
dnl# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([doschk],[unknown],[/dev/null])
AC_CONFIG_SRCDIR([doschk.c])
AC_CONFIG_AUX_DIR([../../../])
AC_CONFIG_HEADERS([config.h:config_h.in])
AC_CANONICAL_TARGET
AC_SYS_LARGEFILE
AC_SYS_LONG_FILE_NAMES
AM_MAINTAINER_MODE

# Checks for programs.
my_CC=${CC-cc}
AC_SUBST([my_CC])dnl

if test "x${CC}" = "x"; then
  test -z "${CC}"
  AC_PROG_CC
else
  test ! -z "${CC}" && export CC
fi
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
AC_PROG_LN_S
AC_PROG_MKDIR_P
AC_PROG_INSTALL
AM_PROG_INSTALL_SH

# Checks for header files.
AC_CHECK_HEADERS([ctype.h malloc.h malloc/malloc.h stdio.h])dnl

# Checks for typedefs, structures, and compiler characteristics.
AC_C_BIGENDIAN
AC_C_PROTOTYPES
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([exit fgets fprintf memset printf putchar qsort \
                strcmp strcpy strlen strncmp strncpy strrchr xmalloc])dnl

# Output.
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
