dnl#                                               -*- Autoconf -*-
dnl# Process this file with autoconf to produce a configure script.
dnl# libintl/configure.ac

AC_PREREQ([2.69])
AC_INIT([libintl],[apple-gdb-1824],[jmolenda@apple.com])
dnl# bug report address is email address of last person to touch this
dnl# directory, according to the Changelog-Apple in this directory
AC_CONFIG_SRCDIR([dgettext.c])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])

AC_CANONICAL_TARGET
AC_OBJEXT
AC_EXEEXT
AC_USE_SYSTEM_EXTENSIONS
if test -f ${srcdir}/config.status; then
  AC_MSG_NOTICE([config.status is already present, removing it.])
  rm -f ${srcdir}/config.status
fi
AM_INIT_AUTOMAKE([1.7 gnu dejagnu])
ifdef([AM_MAINTAINER_MODE],[
  test -e "."
  if test -e "Makefile"; then
    AC_MSG_NOTICE([A Makefile is already present; removing it.])
    test -e "Makefile" && rm -f "Makefile"
  else
    AC_MSG_NOTICE([No Makefile present...])
  fi
  AM_MAINTAINER_MODE
])
ifdef([AM_SANITY_CHECK],[
  test -e "."
  if test -e "Makefile"; then
    AC_MSG_NOTICE([A Makefile is already present; removing it.])
    test -e "Makefile" && rm -f "Makefile"
  else
    AC_MSG_NOTICE([No Makefile present, the sanity check should pass next...])
  fi
  AM_SANITY_CHECK
])

AC_SYS_LARGEFILE
AC_SYS_LONG_FILE_NAMES

# Checks for programs.
AM_PROG_AR
AC_PROG_CXX
AC_DISABLE_SHARED
LT_INIT([shared static win32-dll])
LT_LANG([C])
AM_PROG_INSTALL_STRIP
AC_PROG_INSTALL
AC_SUBST([INSTALLdir])
AC_PROG_LN_S
if test "x$RANLIB" = "x"; then
	test -z "$RANLIB"
	AC_PROG_RANLIB
else
	test ! -z "$RANLIB"
fi
AC_DEFUN([AC_REQUIRE_PROG_SED],[
  AC_REQUIRE([AC_PROG_SED])
])
AC_REQUIRE_PROG_SED
AC_PATH_PROG([ICONV_BIN],[iconv])
AC_SUBST([ICONV_BIN])

# Checks for libraries.
AC_CHECK_LIB([c],[printf])
# FIXME: Replace `main' with a function in `-liberty':
AC_CHECK_LIB([iberty],[main])
# FIXME: Replace `main' with a function in `-lgnu':
AC_CHECK_LIB([gnu],[main])

# Checks for header files.
AC_DEFUN([AC_REQUIRE_HEADER_STDC],[
  AC_REQUIRE([AC_HEADER_STDC])
])
AC_REQUIRE_HEADER_STDC
AC_CHECK_HEADERS([argz.h bits/libc-lock.h byteswap.h ctype.h errno.h \
                  fcntl.h iconv.h libintl.h limits.h locale.h \
                  malloc.h malloc/malloc.h nl_types.h \
                  stddef.h stdio.h sys/mman.h sys/param.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_BIGENDIAN
AC_C_INLINE
AC_C_PROTOTYPES
AC_TYPE_SIZE_T
AC_TYPE_UINTMAX_T
AC_CHECK_TYPES([iconv_t])

# Checks for library functions.
AM_GNU_GETTEXT([external])
AC_SUBST([GT_YES])
AC_SUBST([GT_NO])
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
export DATADIRNAME=share
AC_SUBST([DATADIRNAME])
export MKINSTALLDIRS=../mkinstalldirs
AC_SUBST([MKINSTALLDIRS])
AC_SUBST([l])
AC_SUBST([INTLOBJS])
export USE_INCLUDED_LIBINTL=yes
AC_SUBST([USE_INCLUDED_LIBINTL])
AM_WITH_DMALLOC
AC_FUNC_ALLOCA
AC_FUNC_MALLOC
AC_FUNC_MMAP
AC_FUNC_REALLOC
AC_FUNC_STRNLEN
AC_CHECK_FUNCS([__argz_count __argz_next __argz_stringify \
                dcgettext__ dgettext__ errno free fgets getcwd \
                getenv getwd memcpy mempcpy munmap putenv setenv \
                setlocale stpcpy strcasecmp strchr strdup \
                strlen tolower])
AC_CHECK_DECLS([dcgettext__])
AC_CHECK_DECLS([dgettext__])
AC_CHECK_DECLS([free])
AC_CHECK_DECLS([index])
AC_CHECK_DECLS([NULL])
AC_CHECK_DECLS([malloc])
AC_CHECK_DECLS([realloc])
AC_SEARCH_LIBS([rpl_realloc],[iberty c gnu])
AC_SEARCH_LIBS([rpl_malloc],[iberty c gnu])

# Output
AC_DIAGNOSE([obsolete],[It is technically impossible to `autoupdate'
cleanly from AC_LINK_FILES to AC_CONFIG_LINKS. `autoupdate' provides a
functional but inelegant update, you should probably tune the result
yourself.])dnl
ac_sources="$nls_cv_header_libgt"
ac_dests="$nls_cv_header_intl"
while test -n "$ac_sources"; do
  set $ac_dests; ac_dest=$1; shift; ac_dests=$*
  set $ac_sources; ac_source=$1; shift; ac_sources=$*
  ac_config_links_1="$ac_config_links_1 $ac_dest:$ac_source"
done
AC_CONFIG_LINKS([$ac_config_links_1])
AH_TOP([
#define _INCLUDING_CONFIG_H
])

if test -e libgettext.h -a ! -e libintl.h; then
  ln -sv libgettext.h libintl.h
elif test -e ${srcdir}/libgettext.h -a ! -e ${srcdir}/libintl.h; then
  ln -sv ${srcdir}/libgettext.h ${srcdir}/libintl.h
elif test -e ${srcdir}/libgettext.h -a ! -e libintl.h; then
  ln -sv ${srcdir}/libgettext.h libintl.h
elif test -e libgettext.h -a ! -e ${srcdir}/libintl.h; then
  ln -sv libgettext.h ${srcdir}/libintl.h
fi

AC_CONFIG_FILES([Makefile \
                 Makefile.orig \
                 po/Makefile.in \
                 po/Makevars])
AC_OUTPUT

if test "${srcdir}" != "."; then
  if test -e ${srcdir}/config.status -a ! -e `pwd`/config.status; then
    cp -v ${srcdir}/config.status `pwd`/config.status || echo ""
  elif test -e `pwd`/config.status -a ! -e ${srcdir}/config.status; then
    cp -v `pwd`/config.status ${srcdir}/config.status || echo ""
  fi
fi
