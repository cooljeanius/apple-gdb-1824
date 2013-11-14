dnl#                                               -*- Autoconf -*-
dnl# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([apple-gdb],[1824],[jmolenda@apple.com])
AC_CONFIG_SRCDIR([template.h])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([build-aux])

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
  fi
fi

AC_CANONICAL_TARGET
AC_SYS_INTERPRETER

# Automake checks, separated out
AM_MAINTAINER_MODE
AM_SANITY_CHECK
AM_PROG_INSTALL_SH
AM_PROG_INSTALL_STRIP
AM_MAKE_INCLUDE
_AM_PROG_TAR([pax])

# Checks for programs.
AM_PROG_AR
AC_PROG_AWK
if test "x$CC" = "x"; then
    test -z "$CC"
    AC_PROG_CC
else
    test ! -z "$CC"
    AC_PROG_GCC_TRADITIONAL
fi
AC_PROG_CPP
AC_PROG_CXX
AC_PROG_INSTALL
AM_PROG_LEX
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_MKDIR_P
AC_PROG_OBJC
AC_PROG_OBJCXX
AC_PROG_RANLIB
AC_PROG_SED
AC_PROG_YACC
AC_PATH_PROG([BUILDIT],[buildit])
AC_SUBST([BUILDIT])

# Checks for libraries.
AC_CHECK_LIB([c],[printf])
AC_CHECK_LIB([dl],[dlopen])
AC_CHECK_LIB([iberty],[xmalloc])
AC_CHECK_LIB([iconv],[iconv])
AC_CHECK_LIB([intl],[gettext])
AC_CHECK_LIB([m],[sqrt])
AC_CHECK_LIB([readline],[rl_initialize])
AC_CHECK_LIB([sqlite3],[sqlite3_initialize])
AC_CHECK_LIB([z],[gzopen])

# Checks for header files.
AC_HEADER_STDBOOL dnl# calls AC_CHECK_HEADER_STDBOOL
AC_CHECK_HEADERS([ApplicationServices/ApplicationServices.h argz.h \
                  fcntl.h float.h gettext.h langinfo.h libgettext.h \
                  libintl.h limits.h locale.h mach/mach.h \
                  malloc.h malloc/malloc.h netinet/in.h nl_types.h \
                  sgtty.h stddef.h stdio.h stdio_ext.h sys/file.h \
                  sys/ioctl.h sys/param.h sys/socket.h sys/time.h \
                  termio.h termios.h wchar.h wctype.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_BIGENDIAN
AC_C_INLINE
AC_C_PROTOTYPES
AC_C_RESTRICT
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
AC_CHECK_TYPES([ptrdiff_t])

# Checks for library functions.
AC_FUNC_ALLOCA
AC_FUNC_ERROR_AT_LINE
AC_FUNC_FORK
AC_FUNC_FSEEKO
AC_FUNC_LSTAT dnl# calls AC_FUNC_LSTAT_FOLLOWS_SLASHED_SYMLINK
AC_FUNC_MALLOC
AC_FUNC_MBRTOWC
AC_FUNC_MMAP
AC_FUNC_OBSTACK
AC_FUNC_REALLOC
AC_CHECK_FUNCS([__argz_count __argz_next __argz_stringify bzero \
                dcgettext getcwd gettimeofday localeconv mblen mbrlen \
                memchr memmove mempcpy memset munmap nl_langinfo \
                re_comp regcomp setenv setlocale stpcpy strcasecmp \
                strchr strcspn strdup strerror strpbrk strrchr strstr \
                strtol strtoul strtoull strverscmp])

# Gettext checks
AM_GNU_GETTEXT([external])
AM_GNU_GETTEXT_VERSION([0.18.3])

AC_CONFIG_SUBDIRS([src])
AC_OUTPUT

# The name "configure" is also a make target in Apple's
# hand-written Makefile, so rename the configure file once
# we are done with it so that `make(1)` will not think that the
# target has already been made.
# Use a separate script for this.
if test -e rename-configure.sh; then
	echo "Running a separate script to rename this configure script..."
	sh ./rename-configure.sh
else
	echo "Warning: The Makefile might think there is nothing to be done."
fi
