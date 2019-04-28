dnl#                                               -*- Autoconf -*-
dnl# Autoconf configure script for gnulib for GDB, the GNU debugger.
dnl# Copyright (C) 1995-2014 Free Software Foundation, Inc.
dnl#
dnl# This file is part of GDB.
dnl#
dnl# This program is free software; you can redistribute it and/or modify
dnl# it under the terms of the GNU General Public License as published by
dnl# the Free Software Foundation; either version 3 of the License, or
dnl# (at your option) any later version.
dnl#
dnl# This program is distributed in the hope that it will be useful,
dnl# but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
dnl# GNU General Public License for more details.
dnl#
dnl# You should have received a copy of the GNU General Public License
dnl# along with this program. If not, see <http://www.gnu.org/licenses/>.

dnl# Process this file with autoconf to produce a configure script.

# Setup:
AC_PREREQ([2.64])dnl
AC_INIT([libgnu],[2013.11.14],[bug-gnulib@gnu.org])
AC_CONFIG_SRCDIR([import/memmem.c])
AC_CONFIG_LIBOBJ_DIR([import])
AC_CONFIG_HEADER([config.h])dnl

# Some initial checks:
AM_MAINTAINER_MODE

AC_PROG_CC
if test "x${COMPILER}" = "x"; then
  test -z "${COMPILER}" && export COMPILER='$(CC)'
fi
AC_SUBST([COMPILER])dnl

AC_MSG_CHECKING([system extensions])
AC_MSG_RESULT([ ])
gl_USE_SYSTEM_EXTENSIONS
if test "x${ac_cv_search_strerror}" = "x"; then
  test -z "${ac_cv_search_strerror}"
  AC_SEARCH_LIBS([strerror],[cposix]) dnl# used to be AC\_ISC\_POSIX
  ## (maybe move below?)
fi
AC_MSG_CHECKING([things to check early before really initializing gnulib])
AC_MSG_RESULT([ ])
gl_EARLY
AC_MSG_NOTICE([done with early gnulib pre-initialization checks])
AC_PROG_CPP
am_cv_prog_cc_stdc="${ac_cv_prog_cc_stdc}"

AC_CONFIG_AUX_DIR([../..])
AC_CONFIG_MACRO_DIR([m4])
AC_CANONICAL_TARGET
if test -z "${HOST_ARCHITECTURE}" -a -n "${host_cpu}"; then
  export HOST_ARCHITECTURE="${host_cpu}"
  AC_SUBST([HOST_ARCHITECTURE])
fi
if test -z "${TARGET_ARCHITECTURE}" -a -n "${target_cpu}"; then
  export TARGET_ARCHITECTURE="${target_cpu}"
  AC_SUBST([TARGET_ARCHITECTURE])
fi

m4_ifdef([BISON_I18N],[
  if test "x${USE_NLS}" = "x"; then
    test -z "${USE_NLS}" && export USE_NLS="no"
  fi
])dnl

## avoid message from one configure test:
export gl_cv_func_localtime_infloop_bug=yes

AC_MSG_NOTICE([initializing gnulib for real now])
gl_INIT
AC_MSG_NOTICE([doing some extra gnulib initialization checks])
gl_INIT_MANUALLY
AC_MSG_NOTICE([done with gnulib initialization checks])dnl

# We do NOT use automake, but gnulib does (well, actually, we _are_ in
# gnulib here, and we _are_ using automake here, but this comment still
# applies for how things originally were...). This part lets us generate
# the Makefile.in for the imported gnulib:
if test -f ${srcdir}/config.status; then
  AC_MSG_NOTICE([config.status is already present, removing it.])
  rm -f ${srcdir}/config.status
fi
AM_INIT_AUTOMAKE([1.11.6 gnits dejagnu no-define subdir-objects])dnl
  ## do this part again, to set a default:
if test "x${am_cv_make_support_nested_variables}" = "x"; then
  test -z "${am_cv_make_support_nested_variables}"
  m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES([yes])])dnl
  ## (might want to pass it the flag anyways, just in case)
fi
AM_CONDITIONAL([CAN_MAKE_IN_IMPORT],[test -e import/Makefile])dnl

# --------------------- #
# Checks for programs.  #
# --------------------- #
AC_MSG_CHECKING([programs for gnulib])
AC_MSG_RESULT([ ])
AC_PROG_INSTALL
AC_PROG_LN_S
if test "x${ac_cv_prog_ac_ct_RANLIB}" = "x" -a "x${RANLIB}" = "x"; then
  test -z "${ac_cv_prog_ac_ct_RANLIB}" && test -z "${RANLIB}"
  AC_PROG_RANLIB
fi

m4_ifdef([AM_PROG_AR],[
  AM_PROG_AR dnl# for libgnu.a
],[
  AC_CHECK_TOOLS([AR],[ar gar])dnl
])dnl

AM_PROG_AS
AC_PROG_CXXCPP
AC_REQUIRE_GREPS dnl# (just in case)

AC_PROG_YACC

# Check for the 'make' the user wants to use:
AC_CHECK_PROGS([MAKE],[make gmake gnumake remake])
AC_SUBST([MAKE])dnl
dnl# first we need to make sure that sed has been checked though:
AC_REQUIRE_PROG_SED dnl# (just in case)
dnl# ok, back to checking 'make' stuff:
MAKE_IS_GNU=""
case "`${MAKE} --version 2>&1 | sed 1q`" in
  *GNU*)
    MAKE_IS_GNU=yes
    ;;
esac
AC_SUBST([MAKE_IS_GNU])dnl

AM_CONDITIONAL([GMAKE],[test "x${MAKE_IS_GNU}" = "xyes"])dnl

# Do the same thing another way:
AX_CHECK_GNU_MAKE

# ---------------------- #
# Checks for libraries.  #
# ---------------------- #

AC_MSG_CHECKING([libraries for gnulib])
AC_MSG_RESULT([ ])
if test "x${acl_cv_wl}" = "x"; then
  test -z "${acl_cv_wl}" && export acl_cv_wl='-Wl,'
fi
AC_SUBST([acl_cv_wl])dnl

AC_CHECK_LIB([c],[printf])dnl

# Solaris requires -lintl if you want strerror (which calls dgettext)
# to return localized messages.
if test "x${ac_cv_func_strerror}" = "xyes" || test "x${ac_cv_search_strerror}" != "x"; then
  test -n "${ac_cv_func_strerror}" || test -n "${ac_cv_search_strerror}"
  AC_CHECK_LIB([intl],[dgettext],[:],[
    AC_CHECK_LIB([intl],[libintl_dgettext])dnl
  ])dnl
  ## should be enough here
fi
unset ac_cv_lib_intl_gettext

# ------------------------- #
# Checks for header files.  #
# ------------------------- #
AC_MSG_CHECKING([headers for gnulib])
AC_MSG_RESULT([ ])
gl_STDINT_H
AC_SUBST([STDINT_H])dnl
gl_HEADER_STRING_H
AC_HEADER_ASSERT
AC_HEADER_DIRENT
if test "x${ac_cv_header_stat_broken}" = "x"; then
  test -z "${ac_cv_header_stat_broken}"
  AC_HEADER_STAT
fi
if test "x${ac_cv_header_stdbool_h}" = "x"; then
  test -z "${ac_cv_header_stdbool_h}"
  AC_HEADER_STDBOOL dnl# also "_CHECK"s it
fi
if test "x${ac_cv_header_sys_wait_h}" = "x"; then
  test -z "${ac_cv_header_sys_wait_h}"
  AC_HEADER_SYS_WAIT
fi
if test "x${ac_cv_header_time}" = "x"; then
  test -z "${ac_cv_header_time}"
  AC_HEADER_TIME
fi
AC_CHECK_HEADERS([OS.h ctype.h errno.h fcntl.h float.h locale.h \
                  malloc/malloc.h nl_types.h shlib-compat.h stddef.h stdio.h \
                  stdio_ext.h time.h _wctype.h])dnl
  ## dummies to keep autoscan happy:
if test "x${ac_cv_header_crtdefs_h}" = "x"; then
  test -z "${ac_cv_header_crtdefs_h}"
  AC_CHECK_HEADERS([crtdefs.h])
fi
if test "x${ac_cv_header_langinfo_h}" = "x"; then
  test -z "${ac_cv_header_langinfo_h}"
  AC_CHECK_HEADERS([langinfo.h])
fi
if test "x${ac_cv_header_libintl_h}" = "x"; then
  test -z "${ac_cv_header_libintl_h}"
  AC_CHECK_HEADERS([libintl.h])
fi
if test "x${ac_cv_header_limits_h}" = "x"; then
  test -z "${ac_cv_header_limits_h}"
  AC_CHECK_HEADERS([limits.h])
fi
if test "x${ac_cv_header_malloc_h}" = "x"; then
  test -z "${ac_cv_header_malloc_h}"
  AC_CHECK_HEADERS([malloc.h])
fi
if test "x${ac_cv_header_netdb_h}" = "x"; then
  test -z "${ac_cv_header_netdb_h}"
  AC_CHECK_HEADERS([netdb.h])
fi
if test "x${ac_cv_header_sys_ioctl_h}" = "x"; then
  test -z "${ac_cv_header_sys_ioctl_h}"
  AC_CHECK_HEADERS([sys/ioctl.h])
fi
if test "x${ac_cv_header_sys_param_h}" = "x"; then
  test -z "${ac_cv_header_sys_param_h}"
  AC_CHECK_HEADERS([sys/param.h])
fi
if test "x${ac_cv_header_sys_socket_h}" = "x"; then
  test -z "${ac_cv_header_sys_socket_h}"
  AC_CHECK_HEADERS([sys/socket.h])
fi
if test "x${ac_cv_header_sys_time_h}" = "x"; then
  test -z "${ac_cv_header_sys_time_h}"
  AC_CHECK_HEADERS([sys/time.h])
fi
if test "x${ac_cv_header_sys_vfs_h}" = "x"; then
  test -z "${ac_cv_header_sys_vfs_h}"
  AC_CHECK_HEADERS([sys/vfs.h])
fi
if test "x${ac_cv_header_termios_h}" = "x"; then
  test -z "${ac_cv_header_termios_h}"
  AC_CHECK_HEADERS([termios.h])
fi
if test "x${ac_cv_header_wchar_h}" = "x"; then
  test -z "${ac_cv_header_wchar_h}"
  AC_CHECK_HEADERS([wchar.h])
fi
if test "x${ac_cv_header_wctype_h}" = "x"; then
  test -z "${ac_cv_header_wctype_h}"
  AC_CHECK_HEADERS([wctype.h])
fi
## mirror our parent:
EXTRA_DEFS=""
if test "x${ac_cv_header_sys_types_h}" = "xyes"; then
  # this has to go separate from the configure header for gdb:
  export DEFS="${DEFS} -DHAVE_SYS_TYPES_H"
  export EXTRA_DEFS="-DHAVE_SYS_TYPES_H"
fi
AC_SUBST([EXTRA_DEFS])dnl

# --------------------------------------------------------------- #
# Checks for typedefs, structures, and compiler characteristics.  #
# --------------------------------------------------------------- #
  ## 1
AC_MSG_CHECKING([compiler characteristics for gnulib])
AC_MSG_RESULT([ ])
if test "x${ac_cv_c_bigendian}" = "x"; then
  test -z "${ac_cv_c_bigendian}"
  AC_C_BIGENDIAN
fi
if test "x${ac_cv_c_const}" = "x"; then
  test -z "${ac_cv_c_const}"
  AC_C_CONST
fi
if test "x${ac_cv_c_inline}" = "x"; then
  test -z "${ac_cv_c_inline}"
  AC_C_INLINE
fi
AC_C_PROTOTYPES
AC_C_VOLATILE
  ## 2
AC_MSG_CHECKING([types for gnulib])
AC_MSG_RESULT([ ])
AC_TYPE_INT32_T
if test "x${ac_cv_type_mbstate_t}" = "x"; then
  test -z "${ac_cv_type_mbstate_t}"
  AC_TYPE_MBSTATE_T
fi
AC_CHECK_TYPES([__darwin_mbstate_t])
if test "x${ac_cv_type_mode_t}" = "x"; then
  test -z "${ac_cv_type_mode_t}"
  AC_TYPE_MODE_T
fi
AC_TYPE_OFF_T
if test "x${ac_cv_type_pid_t}" = "x"; then
  test -z "${ac_cv_type_pid_t}"
  AC_TYPE_PID_T
fi
if test "x${ac_cv_type_ptrdiff_t}" = "x"; then
  test -z "${ac_cv_type_ptrdiff_t}"
  AC_CHECK_TYPES([ptrdiff_t])dnl
  ## keeps autoscan happy
fi
AC_TYPE_SSIZE_T
if test "x${ac_cv_type_uid_t}" = "x"; then
  test -z "${ac_cv_type_uid_t}"
  AC_TYPE_UID_T
fi
AC_TYPE_UINT8_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_CHECK_TYPES([wctype_t],[
export HAVE_WCTYPE_T=1
export DEFS="${DEFS} -DGNULIB_defined_wctype_t=1"
export EXTRA_DEFS="${EXTRA_DEFS} -DGNULIB_defined_wctype_t=1"
],[
export HAVE_WCTYPE_T=0
],[
#ifdef HAVE_WCTYPE_H
# include <wctype.h>
#else
# ifdef HAVE__WCTYPE_H
#  include <_wctype.h>
# else
#  if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#   warning "This conftest for wctype_t wants to include a header for it."
#  endif /* __GNUC__ && !__STRICT_ANSI__ */
# endif /* HAVE__WCTYPE_H */
#endif /* HAVE_WCTYPE_H */
])dnl
AC_CHECK_TYPES([__darwin_wctype_t])dnl
AC_CHECK_TYPES([wctrans_t],[
export HAVE_WCTRANS_T=1
export DEFS="${DEFS} -DGNULIB_defined_wctrans_t=1"
export EXTRA_DEFS="${EXTRA_DEFS} -DGNULIB_defined_wctrans_t=1"
],[
export HAVE_WCTRANS_T=0
],[
#ifdef HAVE_WCTYPE_H
# include <wctype.h>
#else
# ifdef HAVE__WCTYPE_H
#  include <_wctype.h>
# else
#  if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#   warning "This conftest for wctrans_t wants to include a header for it."
#  endif /* __GNUC__ && !__STRICT_ANSI__ */
# endif /* HAVE__WCTYPE_H */
#endif /* HAVE_WCTYPE_H */
])dnl
AC_CHECK_TYPES([__darwin_wctrans_t])dnl
  ## 3
AC_MSG_CHECKING([structures for gnulib])
AC_MSG_RESULT([ ])
AC_CHECK_MEMBERS([struct stat.st_blksize])dnl
AC_CHECK_MEMBERS([struct stat.st_rdev])dnl
 ## check cache value for this:
if test "x${ac_cv_member_struct_stat_st_blocks}" = "x"; then
  test -z "${ac_cv_member_struct_stat_st_blocks}"
  AC_STRUCT_ST_BLOCKS
fi
 ## and this:
if test "x${ac_cv_member_struct_tm_tm_zone}" = "x"; then
  test -z "${ac_cv_member_struct_tm_tm_zone}"
  AC_STRUCT_TIMEZONE dnl# also does the "_TM" one
elif test "x${ac_cv_struct_tm}" = "x"; then
  test -z "${ac_cv_struct_tm}"
  AC_STRUCT_TM
fi

# ------------------------------ #
# Checks for library functions.  #
# ------------------------------ #
AC_MSG_CHECKING([functions for gnulib])
AC_MSG_RESULT([ ])
if test "x${ac_cv_lib_error_at_line}" = "x"; then
  test -z "${ac_cv_lib_error_at_line}"
  AC_FUNC_ERROR_AT_LINE dnl# (might create libobj)
fi
AC_FUNC_FNMATCH dnl# (avoid replacing it here though)
  ## also checks whether it "_FOLLOWS_SLASHED_SYMLINK":
if test "x${ac_cv_func_lstat}" = "x"; then
  test -z "${ac_cv_func_lstat}"
  AC_FUNC_LSTAT dnl# (might create libobj)
fi
if test "x${ac_cv_func_malloc_0_nonnull}" = "x"; then
  test -z "${ac_cv_func_malloc_0_nonnull}"
  AC_FUNC_MALLOC dnl# (might create libobj)
fi
if test "x${ac_cv_func_memcmp_working}" = "x"; then
  test -z "${ac_cv_func_memcmp_working}"
  AC_FUNC_MEMCMP dnl# (might create libobj)
fi
if test "x${ac_cv_func_mbrtowc}" = "x"; then
  test -z "${ac_cv_func_mbrtowc}"
  AC_FUNC_MBRTOWC
fi
if test "x${ac_cv_func_obstack}" = "x"; then
  test -z "${ac_cv_func_obstack}"
  AC_FUNC_OBSTACK dnl# (might create libobj)
fi
if test "x${ac_cv_func_realloc_0_nonnull}" = "x"; then
  test -z "${ac_cv_func_realloc_0_nonnull}"
  AC_FUNC_REALLOC dnl# (might create libobj)
fi
AC_FUNC_STAT dnl# (might create libobj)
  ## two cache values to check for this one:
if test "x${ac_cv_func_strerror_r}" = "x" && test "x${ac_cv_func_strerror_r_char_p}" = "x"; then
  test -z "${ac_cv_func_strerror_r}" && test -z "${ac_cv_func_strerror_r_char_p}"
  AC_FUNC_STRERROR_R
fi
AC_FUNC_STRNLEN dnl# (might create libobj)
AC_FUNC_VPRINTF
AC_CHECK_FUNCS([getcwd isascii iswprint localeconv mblen mbrlen memset mkdir \
                putenv re_comp regcomp rmdir setlocale stpcpy strcasecmp \
                strchr strcmp strcspn strerror strncasecmp strrchr strspn \
                strstr tzunset uname])dnl
  ## dummies to keep autoscan happy:
if test "x${ac_cv_func_atexit}" = "x"; then
  test -z "${ac_cv_func_atexit}"
  AC_CHECK_FUNCS([atexit])
fi
if test "x${ac_cv_func_btowc}" = "x"; then
  test -z "${ac_cv_func_btowc}"
  AC_CHECK_FUNCS([btowc])
fi
if test "x${ac_cv_func_dup2}" = "x"; then
  test -z "${ac_cv_func_dup2}"
  AC_CHECK_FUNCS([dup2])
fi
if test "x${ac_cv_func_fchdir}" = "x"; then
  test -z "${ac_cv_func_fchdir}"
  AC_CHECK_FUNCS([fchdir])
fi
if test "x${ac_cv_func_getpagesize}" = "x"; then
  test -z "${ac_cv_func_getpagesize}"
  AC_CHECK_FUNCS([getpagesize])
fi
if test "x${ac_cv_func_gettimeofday}" = "x"; then
  test -z "${ac_cv_func_gettimeofday}"
  AC_CHECK_FUNCS([gettimeofday])
fi
if test "x${ac_cv_func_gmtime}" = "x"; then
  test -z "${ac_cv_func_gmtime}"
  AC_CHECK_FUNCS([gmtime])
fi
if test "x${ac_cv_func_localtime}" = "x"; then
  test -z "${ac_cv_func_localtime}"
  AC_CHECK_FUNCS([localtime])
fi
if test "x${ac_cv_func_localtime_r}" = "x"; then
  test -z "${ac_cv_func_localtime_r}"
  AC_CHECK_FUNCS([localtime_r])
fi
if test "x${ac_cv_func_memchr}" = "x"; then
  test -z "${ac_cv_func_memchr}"
  AC_CHECK_FUNCS([memchr])
fi
if test "x${ac_cv_func_memmove}" = "x"; then
  test -z "${ac_cv_func_memmove}"
  AC_CHECK_FUNCS([memmove])
fi
if test "x${ac_cv_func_mempcpy}" = "x"; then
  test -z "${ac_cv_func_mempcpy}"
  AC_CHECK_FUNCS([mempcpy])
fi
if test "x${ac_cv_func_memrchr}" = "x"; then
  test -z "${ac_cv_func_memrchr}"
  AC_CHECK_FUNCS([memrchr])
fi
if test "x${ac_cv_func_nl_langinfo}" = "x"; then
  test -z "${ac_cv_func_nl_langinfo}"
  AC_CHECK_FUNCS([nl_langinfo])
fi
if test "x${ac_cv_func_setenv}" = "x"; then
  test -z "${ac_cv_func_setenv}"
  AC_CHECK_FUNCS([setenv])
fi
if test "x${ac_cv_func_strdup}" = "x"; then
  test -z "${ac_cv_func_strdup}"
  AC_CHECK_FUNCS([strdup])
fi
if test "x${ac_cv_func_strndup}" = "x"; then
  test -z "${ac_cv_func_strndup}"
  AC_CHECK_FUNCS([strndup])
fi
if test "x${ac_cv_func_strpbrk}" = "x"; then
  test -z "${ac_cv_func_strpbrk}"
  AC_CHECK_FUNCS([strpbrk])
fi
if test "x${ac_cv_func_strtol}" = "x"; then
  test -z "${ac_cv_func_strtol}"
  AC_CHECK_FUNCS([strtol])
fi
if test "x${ac_cv_func_tzset}" = "x"; then
  test -z "${ac_cv_func_tzset}"
  AC_CHECK_FUNCS([tzset])
fi
if test "x${ac_cv_func_wcwidth}" = "x"; then
  test -z "${ac_cv_func_wcwidth}"
  AC_CHECK_FUNCS([wcwidth])
fi
AC_CHECK_DECLS_ONCE([gmtime])dnl
AC_CHECK_DECLS_ONCE([localtime])dnl
AC_CHECK_DECLS_ONCE([localtime_r])dnl
AC_CHECK_DECLS_ONCE([strerror])dnl
  ## force this check to be done again:
unset ac_cv_have_decl_wctype
AC_CHECK_DECLS([wctype],[
export HAVE_DECL_WCTYPE=1
export GNULIB_WCTYPE=0
],[
export HAVE_DECL_WCTYPE=0
],[
#ifdef HAVE_WCTYPE_H
# include <wctype.h>
#else
# ifdef HAVE__WCTYPE_H
#  include <_wctype.h>
# else
#  ifdef HAVE_WCHAR_H
#   include <wchar.h>
#  else
#   if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#    warning "This conftest for wctype wants to include a header for it."
#   endif /* __GNUC__ && !__STRICT_ANSI__ */
#  endif /* HAVE_WCHAR_H */
# endif /* HAVE__WCTYPE_H */
#endif /* HAVE_WCTYPE_H */
])dnl

# One last header thing:
if test "x${NEXT_STRING_H}" = "x"; then
  test -z "${NEXT_STRING_H}" && export NEXT_STRING_H="<string.h>"
  AC_SUBST([NEXT_STRING_H])
fi

# -------- #
# Output.  #
# -------- #
AC_CONFIG_FILES([Makefile Makefile_orig import/Makefile])dnl
AC_CONFIG_COMMANDS([default],[
case x${CONFIG_HEADERS} in
xconfig.h:config.in)
echo "warning: using wrong config header" | tee stamp-h ;;
xconfig.h)
echo "stamp" > stamp-h ;;
esac
],[])dnl

AC_CONFIG_COMMANDS([config_status_vpath_build_fix],[
if test "${srcdir}" != "."; then
  if test -e ${srcdir}/config.status -a ! -e `pwd`/config.status; then
    cp -v ${srcdir}/config.status `pwd`/config.status || echo ""
  elif test -e `pwd`/config.status -a ! -e ${srcdir}/config.status; then
    cp -v `pwd`/config.status ${srcdir}/config.status || echo ""
  fi
fi
],[])dnl

AC_OUTPUT

echo "Done with the gnulib configure script."

exit 0
