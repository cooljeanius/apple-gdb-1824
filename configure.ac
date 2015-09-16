dnl#                                               -*- Autoconf -*-
dnl# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([apple-gdb],[1824],[jmolenda@apple.com])
AC_CONFIG_SRCDIR([template.h])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([build-aux])dnl

if test -x `which sw_vers`; then
  if test ! -z "$(sw_vers -productVersion | cut -d\. -f2 2>/dev/null)"; then
    if test "$(sw_vers -productVersion | cut -d\. -f2 2>/dev/null)" = "6"; then
      if test -e ./.profile_snowleopard; then
        AC_MSG_NOTICE([sourcing ./.profile_snowleopard])
        . ./.profile_snowleopard
      else
        AC_MSG_WARN([there are some environment variables that you will have to set manually for this to work properly.])
      fi
    elif test "$(sw_vers -productVersion | cut -d\. -f2 2>/dev/null)" = "5"; then
      if test -e ./.profile_leopard; then
        AC_MSG_NOTICE([sourcing ./.profile_leopard])
        . ./.profile_leopard
      else
        AC_MSG_WARN([there are some environment variables that you will have to set manually for this to work properly.])
      fi
    else
      AC_MSG_WARN([you will have to find out what to do for this platform yourself.])
    fi
  else
    AC_MSG_WARN([you will have to find out what to do for this platform yourself.])
  fi
else
  if test -e ./.profile_generic; then
    AC_MSG_NOTICE([sourcing ./.profile_generic])
    . ./.profile_generic
  else
    AC_MSG_WARN([you will have to find out what to do for this platform yourself.])
  fi
fi

AC_CANONICAL_TARGET
AC_SYS_INTERPRETER

# Automake is just for one subdir; switching any other subdirs to use
# automake could potentially be disastrous:
AC_MSG_CHECKING([automake things])
AC_MSG_RESULT([ ])
AM_INIT_AUTOMAKE([foreign no-dist dejagnu no-define no-dependencies std-options tar-pax])dnl
AM_MAINTAINER_MODE
AM_SANITY_CHECK
AM_PROG_INSTALL_SH
AM_PROG_INSTALL_STRIP
AM_MAKE_INCLUDE
AC_ARG_ENABLE([opts-test],
[AS_HELP_STRING([--disable-opts-test],
                [Do not try to run a test AutoOpts program
                 (only relevant in src subdir)])dnl
])dnl# (for the autoconf macro for checking for autogen)

# Checks for programs.
AC_MSG_CHECKING([programs])
AC_MSG_RESULT([ ])
AM_PROG_AR
if test "x${AWK}" = "x" && test "x${ac_cv_prog_AWK}" = "x"; then
  test -z "${AWK}" && test -z "${ac_cv_prog_AWK}"
  AC_PROG_AWK
fi
if test "x${CC}" = "x"; then
  test -z "${CC}"
  AC_PROG_CC
else
  test ! -z "${CC}"
  AC_PROG_GCC_TRADITIONAL
fi
AC_PROG_CPP
AC_PROG_CXX
AC_PROG_INSTALL
AM_PROG_LEX
AC_PROG_LN_S
if test "x${ac_cv_prog_make_make_set}" = "x"; then
  test -z "${ac_cv_prog_make_make_set}"
  AC_PROG_MAKE_SET
fi
AC_PROG_MKDIR_P
AC_PROG_OBJC
AC_PROG_OBJCPP
AC_PROG_OBJCXX
AC_PROG_OBJCXXCPP
AC_PROG_RANLIB
AC_PROG_SED
AC_PROG_YACC
AC_PATH_PROG([BUILDIT],[buildit])dnl
AC_SUBST([BUILDIT])dnl

# Checks for libraries.
AC_MSG_CHECKING([libraries])
AC_MSG_RESULT([ ])
AC_MSG_CHECKING([for linker flag to verify linked libraries have the correct architecture])
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
    if test "x${WARN_LDFLAGS}" = "x"; then
      test -z "${WARN_LDFLAGS}" && WARN_LDFLAGS="${LD_ARCHFLAGS}"
    fi
  fi
  AC_SUBST([LD_ARCHFLAGS])dnl
  ## that should be all we need to do with them for now...
else
  test -z "${LD_ARCHFLAGS}"
  AC_MSG_RESULT([none])
fi
AC_CHECK_LIB([c],[printf])dnl
AC_CHECK_LIB([dl],[dlopen])dnl
  ## might want to pick a better symbol to check for:
AC_CHECK_LIB([gmp],[gmp_version],[],[
  PRE_GMP_LDFLAGS="${LDFLAGS}"
  LDFLAGS="${LDFLAGS} ${LD_ARCHFLAGS}"
  AC_CHECK_LIB([gmp],[main])dnl
  ## reset:
  LDFLAGS="${PRE_GMP_LDFLAGS}"
])dnl
  ## might want to ensure we find the in-tree one:
AC_CHECK_LIB([iberty],[xmalloc],[],[
  PRE_LIBERTY_LDFLAGS="${LDFLAGS}"
  LDFLAGS="${LDFLAGS} ${LD_ARCHFLAGS}"
  AC_CHECK_LIB([iberty],[main])dnl
  ## reset:
  LDFLAGS="${PRE_LIBERTY_LDFLAGS}"
])dnl
AC_CHECK_LIB([iconv],[iconv],[],[
  AC_CHECK_LIB([iconv],[libiconv],[],[
    PRE_ICONV_LDFLAGS="${LDFLAGS}"
    LDFLAGS="${LDFLAGS} ${LD_ARCHFLAGS}"
    AC_CHECK_LIB([iconv],[main])dnl
    ## reset:
    LDFLAGS="${PRE_ICONV_LDFLAGS}"
  ])dnl
])dnl
  ## might want to ensure we find the in-tree one:
AC_CHECK_LIB([intl],[gettext],[],[
  PRE_INTL_LDFLAGS="${LDFLAGS}"
  LDFLAGS="${LDFLAGS} ${LD_ARCHFLAGS}"
  AC_CHECK_LIB([intl],[main],[],[])dnl
  ## reset:
  LDFLAGS="${PRE_INTL_LDFLAGS}"
])dnl
AC_CHECK_LIB([m],[sqrt])
m4_ifdef([LT_LIB_M],[
  LT_LIB_M
],[
  LIBM=""
  AC_CHECK_LIB([m],[cos],[LIBM=-lm])dnl
  AC_SUBST([LIBM])dnl
])dnl
  ## might want to pick a better symbol to check for:
AC_CHECK_LIB([mpfr],[mpfr_get_version],[],[
  PRE_MPFR_LDFLAGS="${LDFLAGS}"
  LDFLAGS="${LDFLAGS} ${LD_ARCHFLAGS}"
  AC_CHECK_LIB([mpfr],[main])dnl
  ## reset:
  LDFLAGS="${PRE_MPFR_LDFLAGS}"
])dnl
  ## might want to ensure we find the in-tree one:
AC_CHECK_LIB([readline],[rl_initialize])dnl
AC_CHECK_LIB([sqlite3],[sqlite3_initialize])dnl
  ## might want to ensure we find the in-tree one:
AC_CHECK_LIB([z],[gzopen])dnl

# Checks for header files.
AC_MSG_CHECKING([headers])
AC_MSG_RESULT([ ])
AC_HEADER_STAT
AC_HEADER_STDBOOL dnl# also "_CHECK"s it
AC_CHECK_HEADERS([ApplicationServices/ApplicationServices.h argz.h \
                  fcntl.h float.h gettext.h langinfo.h libgettext.h \
                  libintl.h limits.h locale.h mach/mach.h \
                  malloc.h malloc/malloc.h netinet/in.h nl_types.h \
                  sgtty.h stddef.h stdio.h stdio_ext.h sys/file.h \
                  sys/ioctl.h sys/socket.h sys/time.h \
                  termio.h termios.h wchar.h wctype.h])
if test "x${ac_cv_header_sys_param_h}" = "x"; then
  test -z "${ac_cv_header_sys_param_h}"
  AC_CHECK_HEADERS([sys/param.h])dnl
  ## should have been checked anyways, but whatever...
fi

# Checks for typedefs, structures, and compiler characteristics.
AC_MSG_CHECKING([C compiler charactersistics])
AC_MSG_RESULT([ ])
AC_C_CONST
AC_C_BIGENDIAN
AC_C_INLINE
AC_C_PROTOTYPES
AC_C_RESTRICT
AC_MSG_CHECKING([types])
AC_MSG_RESULT([ ])
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_OFF_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UINT8_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_CHECK_TYPES([ptrdiff_t])dnl
AC_STRUCT_TIMEZONE dnl# also does the "_TM" one
dnl# Begin what used to be AC\_TYPE\_SIGNAL
AC_CACHE_CHECK([return type of signal handlers],[ac_cv_type_signal],[
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <sys/types.h>
#include <signal.h>
]],[[
return *(signal (0, 0)) (0) == 1;
]])],[ac_cv_type_signal=int],[ac_cv_type_signal=void])dnl
])dnl
  ## corresponding define:
AC_DEFINE_UNQUOTED([RETSIGTYPE],[${ac_cv_type_signal}],
                   [Define as the return type of signal handlers
                    (`int' or `void').])
dnl# End what used to be AC\_TYPE\_SIGNAL

# Checks for library functions.
AC_MSG_CHECKING([functions])
AC_MSG_RESULT([ ])
AC_FUNC_ALLOCA
AC_FUNC_ERROR_AT_LINE
AC_FUNC_FORK
AC_FUNC_FSEEKO
AC_FUNC_LSTAT dnl# also checks whether it "_FOLLOWS_SLASHED_SYMLINK"
AC_FUNC_MALLOC
AC_FUNC_MEMCMP
AC_FUNC_MBRTOWC
AC_FUNC_MMAP
AC_FUNC_OBSTACK
AC_FUNC_REALLOC
AC_FUNC_STAT
AC_FUNC_VPRINTF
AC_CHECK_FUNCS([__argz_count __argz_next __argz_stringify bzero \
                dcgettext getcwd gettimeofday localeconv mblen mbrlen \
                memchr memmove mempcpy memset munmap nl_langinfo \
                re_comp regcomp setenv setlocale stpcpy strcasecmp \
                strchr strcspn strdup strerror strpbrk strrchr strstr \
                strtol strtoul strtoull strverscmp])dnl

# Gettext checks:
AC_MSG_CHECKING([gettext])
AC_MSG_RESULT([ ])
AM_GNU_GETTEXT([external])
AM_GNU_GETTEXT_VERSION([0.19.4])dnl

# Output:
AC_CONFIG_FILES([libcheckpoint/Makefile])
AC_CONFIG_SUBDIRS([src])dnl
AC_OUTPUT

# The name "configure" is also a make target in Apple's
# hand-written Makefile, so rename the configure file once
# we are done with it so that `make(1)` will not think that the
# target has already been made.
# Use a separate script for this.
if test -e rename-configure.sh; then
  echo "Running a separate script to rename this configure script..."
  ${SHELL} ./rename-configure.sh
else
  echo "Warning: The Makefile might think there is nothing to be done."
fi
