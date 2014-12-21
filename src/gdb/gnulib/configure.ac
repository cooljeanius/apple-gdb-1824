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
AC_MSG_CHECKING([system extensions])
AC_MSG_RESULT([ ])
gl_USE_SYSTEM_EXTENSIONS
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
AM_INIT_AUTOMAKE([1.11.1 gnits no-define subdir-objects])dnl
  ## do this part again, to set a default:
if test "x${am_cv_make_support_nested_variables}" = "x"; then
  test -z "${am_cv_make_support_nested_variables}"
  m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES([yes])])dnl
  ## (might want to pass it the flag anyways, just in case)
fi

# --------------------- #
# Checks for programs.  #
# --------------------- #

AC_PROG_INSTALL
AC_PROG_LN_S
if test "x${ac_cv_prog_ac_ct_RANLIB}" = "x" -a "x${RANLIB}" = "x"; then
  test -z "${ac_cv_prog_ac_ct_RANLIB}" && test -z "${RANLIB}"
  AC_PROG_RANLIB
fi

m4_ifdef([AM_PROG_AR],[
  AM_PROG_AR
],[
  AC_CHECK_TOOLS([AR],[ar gar])dnl
])dnl

# Check for the 'make' the user wants to use:
AC_CHECK_PROGS([MAKE],[make gmake gnumake remake])
AC_SUBST([MAKE])
MAKE_IS_GNU=""
case "`${MAKE} --version 2>&1 | sed 1q`" in
  *GNU*)
    MAKE_IS_GNU=yes
    ;;
esac
AC_SUBST([MAKE_IS_GNU])dnl

# ---------------------- #
# Checks for libraries.  #
# ---------------------- #
AC_CHECK_LIB([c],[printf])dnl

# Checks for header files:
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
AC_HEADER_SYS_WAIT
if test "x${ac_cv_header_time}" = "x"; then
  test -z "${ac_cv_header_time}"
  AC_HEADER_TIME
fi
AC_CHECK_HEADERS([OS.h crtdefs.h ctype.h errno.h fcntl.h float.h \
                  langinfo.h libintl.h limits.h locale.h \
                  malloc.h malloc/malloc.h netdb.h nl_types.h \
                  shlib-compat.h stddef.h stdio.h time.h])dnl
  ## dummies to keep autoscan happy:
if test "x${ac_cv_header_sys_param_h}" = "x"; then
  test -z "${ac_cv_header_sys_param_h}"
  AC_CHECK_HEADERS([sys/param.h])
fi
if test "x${ac_cv_header_sys_time_h}" = "x"; then
  test -z "${ac_cv_header_sys_time_h}"
  AC_CHECK_HEADERS([sys/time.h])
fi
if test "x${ac_cv_header_wchar_h}" = "x"; then
  test -z "${ac_cv_header_wchar_h}"
  AC_CHECK_HEADERS([wchar.h])
fi
if test "x${ac_cv_header_wctype_h}" = "x"; then
  test -z "${ac_cv_header_wctype_h}"
  AC_CHECK_HEADERS([wctype.h])
fi

# Checks for typedefs, structures, and compiler characteristics:
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
AC_TYPE_INT32_T
if test "x${ac_cv_type_mode_t}" = "x"; then
  test -z "${ac_cv_type_mode_t}"
  AC_TYPE_MODE_T
fi
AC_TYPE_OFF_T
AC_CHECK_TYPES([ptrdiff_t])dnl
AC_TYPE_SSIZE_T
AC_TYPE_UID_T
AC_TYPE_UINT32_T
AC_CHECK_MEMBERS([struct stat.st_blksize])dnl
 ## check cache value for this:
if test "x${ac_cv_member_struct_stat_st_blocks}" = "x"; then
  test -z "${ac_cv_member_struct_stat_st_blocks}"
  AC_STRUCT_ST_BLOCKS
fi
AC_STRUCT_TIMEZONE dnl# also does the "_TM" one

# Checks for library functions:
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
AC_CHECK_FUNCS([getpagesize memset nl_langinfo setlocale stpcpy strchr \
                strcmp strcspn strdup strerror strndup strpbrk strrchr \
                strspn strstr tzset wcwidth])dnl
  ## dummies to keep autoscan happy:
if test "x${ac_cv_func_gettimeofday}" = "x"; then
  test -z "${gettimeofday}"
  AC_CHECK_FUNCS([gettimeofday])
fi
if test "x${ac_cv_func_memchr}" = "x"; then
  test -z "${ac_cv_func_memchr}"
  AC_CHECK_FUNCS([memchr])
fi
if test "x${ac_cv_func_mempcpy}" = "x"; then
  test -z "${ac_cv_func_mempcpy}"
  AC_CHECK_FUNCS([mempcpy])
fi

if test "x${NEXT_STRING_H}" = "x"; then
  test -z "${NEXT_STRING_H}" && export NEXT_STRING_H="<string.h>"
  AC_SUBST([NEXT_STRING_H])
fi

# Output:
AC_CONFIG_FILES([Makefile Makefile_orig import/Makefile])dnl
AC_CONFIG_COMMANDS([default],[
case x${CONFIG_HEADERS} in
xconfig.h:config.in)
echo "" > stamp-h ;;
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

exit 0
