#gnulib-use-manually.m4 serial 2 -*- Autoconf -*-
dnl# (the latest revbump in serial is actually 1 commit too late...)
dnl# Instead of using gnulib-tool, copy the macros we need from modules
dnl# manually.  This macro is dependent on the exact set of modules used
dnl# here, and as such, would not make much sense elsewhere.
AC_DEFUN([gl_INIT_MANUALLY],[
AC_REQUIRE([AC_PROG_LN_S])dnl
AC_REQUIRE([AC_PROG_RANLIB])dnl
AC_REQUIRE([AC_PROG_SED])dnl
AC_REQUIRE([AC_HEADER_STDBOOL]) dnl# also calls the "_CHECK" version of it
AC_REQUIRE([AC_HEADER_TIME])dnl
AC_REQUIRE([gl_BIGENDIAN])dnl
AC_REQUIRE([AC_C_CONST])dnl
AC_REQUIRE([AC_C_RESTRICT])dnl
AC_REQUIRE([AC_TYPE_SIZE_T])dnl
AC_REQUIRE([AC_FUNC_ALLOCA])dnl
AC_REQUIRE([AC_FUNC_MEMCMP])dnl
dnl# from gnulib module alloca-opt:
AC_REQUIRE([gl_FUNC_ALLOCA])dnl
dnl# from gnulib module autobuild:
AC_REQUIRE([AB_INIT])dnl
dnl# from gnulib module configmake:
AC_REQUIRE([gl_CONFIGMAKE_PREP])dnl
dnl# from gnulib module dirent:
AC_REQUIRE([gl_DIRENT_H])dnl
dnl# from gnulib module dirfd:
AC_REQUIRE([gl_FUNC_DIRFD])dnl
  ## set up libobj if needed:
if test "x${ac_cv_func_dirfd}" = "xno" && test "x${gl_cv_func_dirfd_macro}" = "xno"; then
  AC_LIBOBJ([dirfd])dnl
  gl_PREREQ_DIRFD
fi
gl_DIRENT_MODULE_INDICATOR([dirfd])dnl
dnl# from gnulib module double-slash-root:
AC_REQUIRE([gl_DOUBLE_SLASH_ROOT])dnl
dnl# from gnulib module errno:
AC_REQUIRE([gl_HEADER_ERRNO_H])dnl
dnl# from gnulib module error:
AC_REQUIRE([gl_ERROR])dnl
  ## set up libobj if needed:
if test "x${ac_cv_lib_error_at_line}" = "xno"; then
  AC_LIBOBJ([error])dnl
  gl_PREREQ_ERROR
fi
dnl# (skip the pointless gettext stuff for it)
dnl# from gnulib module extensions:
AC_REQUIRE([gl_USE_SYSTEM_EXTENSIONS])dnl
dnl# from gnulib module extern-inline:
AC_REQUIRE([gl_EXTERN_INLINE])dnl
dnl# from gnulib module fileblocks:
AC_REQUIRE([gl_FILEBLOCKS])dnl
  ## set up libobj if needed:
if test "x${ac_cv_member_struct_stat_st_blocks}" = "xno"; then
  AC_LIBOBJ([fileblocks])dnl
  gl_PREREQ_FILEBLOCKS
fi
dnl# from gnulib module float:
AC_REQUIRE([gl_FLOAT_H])dnl
  ## set up libobjs if needed:
if test ${REPLACE_FLOAT_LDBL} = 1; then
  AC_LIBOBJ([float])dnl
  ## end first libobj from "float"
fi
if test ${REPLACE_ITOLD} = 1; then
  AC_LIBOBJ([itold])dnl
  ## end second libobj from "float"
fi
dnl# from gnulib module fnmatch:
AC_REQUIRE([gl_FUNC_FNMATCH_POSIX])
if test -n "${FNMATCH_H}"; then
  AC_LIBOBJ([fnmatch])dnl
  gl_PREREQ_FNMATCH
fi
dnl# from gnulib module fnmatch-gnu:
AC_REQUIRE([gl_FUNC_FNMATCH_GNU])dnl
  ## set up libobj if needed:
if test -n "${FNMATCH_H}"; then
  AC_LIBOBJ([fnmatch])dnl
  gl_PREREQ_FNMATCH
fi
dnl# from gnulib module fpieee:
AC_REQUIRE([gl_FP_IEEE])dnl
dnl# from gnulib module frexp:
AC_REQUIRE([gl_FUNC_FREXP])dnl
  ## set up libobj if needed:
if test "x${gl_func_frexp}" != "xyes"; then
  AC_LIBOBJ([frexp])dnl
  ## end libobj
fi
gl_MATH_MODULE_INDICATOR([frexp])dnl
dnl# from gnulib module frexpl:
AC_REQUIRE([gl_FUNC_FREXPL])dnl
  ## set up libobj if needed:
if test ${HAVE_DECL_FREXPL} = 0 || test "x${gl_func_frexpl}" = "xno"; then
  AC_LIBOBJ([frexpl])dnl
  ## end libobj
fi
gl_MATH_MODULE_INDICATOR([frexpl])dnl
dnl# from gnulib module gettext-h:
AC_SUBST([LIBINTL])dnl
AC_SUBST([LTLIBINTL])dnl
dnl# from gnulib module gettimeofday:
AC_REQUIRE([gl_FUNC_GETTIMEOFDAY])
if test ${HAVE_GETTIMEOFDAY} = 0 || test ${REPLACE_GETTIMEOFDAY} = 1; then
  AC_LIBOBJ([gettimeofday])dnl
  gl_PREREQ_GETTIMEOFDAY
fi
gl_SYS_TIME_MODULE_INDICATOR([gettimeofday])dnl
dnl# from gnulib module gnu-make:
AC_REQUIRE([gl_GNU_MAKE])dnl
dnl# from gnulib module host-cpu-c-abl:
AC_REQUIRE([gl_HOST_CPU_C_ABI])dnl
dnl# from gnulib module host-os:
AC_REQUIRE([gl_HOST_OS])dnl
dnl# from gnulib module inline:
AC_REQUIRE([gl_INLINE])dnl
dnl# from gnulib module inttypes:
AC_REQUIRE([gl_INTTYPES_H])dnl
dnl# from gnulib module inttypes-incomplete:
AC_REQUIRE([gl_INTTYPES_INCOMPLETE])dnl
dnl# from gnulib module isnand-nolibm:
AC_REQUIRE([gl_FUNC_ISNAND_NO_LIBM])dnl
  ## set up libobj if needed:
if test "x${gl_func_isnand_no_libm}" != "xyes"; then
  AC_LIBOBJ([isnand])dnl
  gl_PREREQ_ISNAND
fi
dnl# from gnulib module isnanl-nolibm:
AC_REQUIRE([gl_FUNC_ISNANL_NO_LIBM])dnl
  ## set up libobj if needed:
if test "x${gl_func_isnanl_no_libm}" != "xyes"; then
  AC_LIBOBJ([isnanl])dnl
  gl_PREREQ_ISNANL
fi
dnl# from gnulib module iswctype:
AC_REQUIRE([gl_FUNC_ISWCTYPE])dnl
  ## set up libobj if needed:
if test ${HAVE_WCTYPE_T} = 0; then
  AC_LIBOBJ([iswctype])dnl
  ## end libobj
fi
gl_WCTYPE_MODULE_INDICATOR([iswctype])dnl
dnl# from gnulib module largefile:
AC_REQUIRE([AC_SYS_LARGEFILE])dnl
AC_REQUIRE([gl_LARGEFILE])dnl
dnl# from gnulib module ldd:
AC_REQUIRE([gl_LDD])dnl
dnl# from gnulib module link-warning:
AC_REQUIRE([gl_FEATURES_H])dnl
dnl# from gnulib module localcharset:
AC_REQUIRE([gl_LOCALCHARSET])dnl
  ## set up envvar if needed:
if test "x${LOCALCHARSET_TESTS_ENVIRONMENT}" = "x"; then
  if test -n "${gl_source_base}"; then
    test -z "${LOCALCHARSET_TESTS_ENVIRONMENT}" && export LOCALCHARSET_TESTS_ENVIRONMENT="CHARSETALIASDIR=\"\$(abs_top_builddir)/${gl_source_base}\""
  else
    test -z "${LOCALCHARSET_TESTS_ENVIRONMENT}" && export LOCALCHARSET_TESTS_ENVIRONMENT="CHARSETALIASDIR=\"\$(abs_top_builddir)\""
  fi
fi
AC_SUBST([LOCALCHARSET_TESTS_ENVIRONMENT])dnl
dnl# from gnulib module lstat:
AC_REQUIRE([gl_FUNC_LSTAT])dnl
  ## set up libobj if needed:
if test ${REPLACE_LSTAT} = 1; then
  AC_LIBOBJ([lstat])dnl
  gl_PREREQ_LSTAT
fi
gl_SYS_STAT_MODULE_INDICATOR([lstat])dnl
dnl# from gnulib module malloc-gnu:
AC_REQUIRE([gl_FUNC_MALLOC_GNU])dnl
  ## set up libobj if needed:
if test ${REPLACE_MALLOC} = 1; then
  AC_LIBOBJ([malloc])dnl
  ## end libobj
fi
gl_MODULE_INDICATOR([malloc-gnu])dnl
dnl# from gnulib module malloc-posix:
AC_REQUIRE([gl_FUNC_MALLOC_POSIX])dnl
dnl# (libobj is already set above)
gl_STDLIB_MODULE_INDICATOR([malloc-posix])dnl
dnl# from gnulib module math:
AC_REQUIRE([gl_MATH_H])dnl
dnl# from gnulib module mbrtowc:
AC_REQUIRE([gl_FUNC_MBRTOWC])dnl
  ## set up libobj if needed:
if test ${HAVE_MBRTOWC} = 0 || test ${REPLACE_MBRTOWC} = 1; then
  AC_LIBOBJ([mbrtowc])dnl
  gl_PREREQ_MBRTOWC
fi
gl_WCHAR_MODULE_INDICATOR([mbrtowc])dnl
dnl# from gnulib module mbsinit:
AC_REQUIRE([gl_FUNC_MBSINIT])dnl
  ## set up libobj if needed:
if test ${HAVE_MBSINIT} = 0 || test ${REPLACE_MBSINIT} = 1; then
  AC_LIBOBJ([mbsinit])dnl
  gl_PREREQ_MBSINIT
fi
gl_WCHAR_MODULE_INDICATOR([mbsinit])dnl
dnl# from gnulib module mbsrtowcs:
AC_REQUIRE([gl_FUNC_MBSRTOWCS])dnl
  ## set up libobj if needed:
if test ${HAVE_MBSRTOWCS} = 0 || test ${REPLACE_MBSRTOWCS} = 1; then
  AC_LIBOBJ([mbsrtowcs])dnl
  AC_LIBOBJ([mbsrtowcs-state])dnl
  gl_PREREQ_MBSRTOWCS
fi
gl_WCHAR_MODULE_INDICATOR([mbsrtowcs])dnl
dnl# from gnulib module memchr:
AC_REQUIRE([gl_FUNC_MEMCHR])dnl
  ## set up libobj if needed:
if test ${HAVE_MEMCHR} = 0 || test ${REPLACE_MEMCHR} = 1; then
  AC_LIBOBJ([memchr])dnl
  gl_PREREQ_MEMCHR
fi
gl_STRING_MODULE_INDICATOR([memchr])dnl
dnl# from gnulib module memcmp:
AC_REQUIRE([gl_FUNC_MEMCMP])dnl
  ## set up libobj if needed:
if test "x${gl_func_memcmp}" = "xno"; then
  AC_LIBOBJ([memcmp])dnl
  gl_PREREQ_MEMCMP
fi
dnl# from gnulib module memmem:
AC_REQUIRE([gl_FUNC_MEMMEM])dnl
  ## set up libobj if needed:
if test ${HAVE_MEMMEM} = 0 || test ${REPLACE_MEMMEM} = 1; then
  AC_LIBOBJ([memmem])dnl
  ## end libobj
fi
dnl# from gnulib module memmem-simple:
AC_REQUIRE([gl_FUNC_MEMMEM_SIMPLE])dnl
dnl# (libobj is already set above)
gl_STRING_MODULE_INDICATOR([memmem])dnl
dnl# from gnulib module mempcpy:
AC_REQUIRE([gl_FUNC_MEMPCPY])dnl
  ## set up libobj if needed:
if test ${HAVE_MEMPCPY} = 0; then
  AC_LIBOBJ([mempcpy])dnl
  gl_PREREQ_MEMPCPY
fi
gl_STRING_MODULE_INDICATOR([mempcpy])dnl
dnl# from gnulib module multiarch:
AC_REQUIRE([gl_MULTIARCH])dnl
dnl# from gnulib module no-c++:
AC_REQUIRE([gt_NO_CXX])dnl
dnl# from gnulib module obstack:
AC_REQUIRE([AC_FUNC_OBSTACK])dnl
dnl# from gnulib module openmp:
AC_REQUIRE([AC_OPENMP])dnl
dnl# from gnulib module pathmax:
AC_REQUIRE([gl_PATHMAX])dnl
dnl# from gnulib module realloc-gnu:
AC_REQUIRE([gl_FUNC_REALLOC_GNU])dnl
  ## set up libobj if needed:
if test ${REPLACE_REALLOC} = 1; then
  AC_LIBOBJ([realloc])dnl
  ## end libobj
fi
gl_MODULE_INDICATOR([realloc-gnu])dnl
dnl# from gnulib module realloc-posix:
AC_REQUIRE([gl_FUNC_REALLOC_POSIX])dnl
dnl# (libobj is already set above)
gl_STDLIB_MODULE_INDICATOR([realloc-posix])dnl
dnl# from gnulib module snippet/link-warning:
AC_REQUIRE([gl_FEATURES_H])dnl
dnl# from gnulib module ssize_t:
AC_REQUIRE([gt_TYPE_SSIZE_T])dnl
dnl# from gnulib module stat:
AC_REQUIRE([gl_FUNC_STAT])dnl
  ## set up libobj if needed:
if test ${REPLACE_STAT} = 1; then
  AC_LIBOBJ([stat])dnl
  gl_PREREQ_STAT
fi
gl_SYS_STAT_MODULE_INDICATOR([stat])dnl
dnl# from gnulib module stat-size:
AC_REQUIRE([gl_STAT_SIZE])dnl
dnl# from gnulib module stat-time:
AC_REQUIRE([gl_STAT_TIME])dnl
AC_REQUIRE([gl_STAT_BIRTHTIME])dnl
dnl# from gnulib module stdbool:
AC_REQUIRE([AM_STDBOOL_H])dnl
dnl# from gnulib module stddef:
AC_REQUIRE([gl_STDDEF_H])dnl
dnl# from gnulib module stdint:
AC_REQUIRE([gl_STDINT_H])dnl
dnl# ...and additionally:
GNULIB_IMPORT_DIR=import
# For Makefile dependencies:
GNULIB_STDINT_H=""
if test x"${STDINT_H}" != x""; then
  test -n "${STDINT_H}" && test -n "${GNULIB_IMPORT_DIR}"
  export GNULIB_STDINT_H=${GNULIB_IMPORT_DIR}/${STDINT_H}
fi
AC_SUBST([GNULIB_STDINT_H])dnl
dnl# from gnulib module strerror:
AC_REQUIRE([gl_FUNC_STRERROR])dnl
  ## set up libobj if needed:
if test ${REPLACE_STRERROR} = 1; then
  AC_LIBOBJ([strerror])dnl
  ## end libobj
fi
gl_MODULE_INDICATOR([strerror])dnl
gl_STRING_MODULE_INDICATOR([strerror])dnl
dnl# from gnulib module strerror_r-posix:
AC_REQUIRE([gl_FUNC_STRERROR_R])dnl
  ## set up libobj if needed:
if test ${HAVE_DECL_STRERROR_R} = 0 || test ${REPLACE_STRERROR_R} = 1; then
  AC_LIBOBJ([strerror_r])dnl
  gl_PREREQ_STRERROR_R
fi
gl_STRING_MODULE_INDICATOR([strerror_r])dnl
dnl# from gnulib module strerror-override:
AC_REQUIRE([gl_HEADER_ERRNO_H])dnl
AC_REQUIRE([gl_FUNC_STRERROR_0])dnl
  ## set up libobj if needed:
if test -n "${ERRNO_H}" || test ${REPLACE_STRERROR_0} = 1; then
  AC_LIBOBJ([strerror-override])dnl
  gl_PREREQ_SYS_H_WINSOCK2
fi
dnl# from gnulib module string:
AC_REQUIRE([gl_HEADER_STRING_H])dnl
dnl# from gnulib module strnlen:
AC_REQUIRE([gl_FUNC_STRNLEN])dnl
  ## set up libobj if needed:
if test ${HAVE_DECL_STRNLEN} = 0 || test ${REPLACE_STRNLEN} = 1; then
  AC_LIBOBJ([strnlen])dnl
  gl_PREREQ_STRNLEN
fi
gl_STRING_MODULE_INDICATOR([strnlen])dnl
dnl# from gnulib module strstr:
AC_REQUIRE([gl_FUNC_STRSTR])dnl
  ## set up libobj if needed:
if test ${REPLACE_STRSTR} = 1; then
  AC_LIBOBJ([strstr])dnl
  ## end libobj
fi
dnl# from gnulib module strstr-simple:
AC_REQUIRE([gl_FUNC_STRSTR_SIMPLE])dnl
dnl# (libobj is already set above)
gl_STRING_MODULE_INDICATOR([strstr])dnl
dnl# from gnulib module sys_stat:
AC_REQUIRE([gl_HEADER_SYS_STAT_H])dnl
AC_REQUIRE([AC_PROG_MKDIR_P])dnl
dnl# from gnulib module sys_time:
AC_REQUIRE([gl_HEADER_SYS_TIME_H])dnl
dnl# from gnulib module sys_types:
AC_REQUIRE([gl_SYS_TYPES_H])dnl
dnl# from gnulib module time:
AC_REQUIRE([gl_HEADER_TIME_H])dnl
dnl# from gnulib module unistd:
AC_REQUIRE([gl_UNISTD_H])dnl
dnl# from gnulib module wchar:
AC_REQUIRE([gl_WCHAR_H])dnl
AC_REQUIRE([AC_FUNC_MBRTOWC])dnl
AC_REQUIRE([AC_TYPE_MBSTATE_T])dnl
dnl# from gnulib module wcsncasecmp:
AC_REQUIRE([gl_FUNC_WCSNCASECMP])dnl
  ## set up libobj if needed:
if test ${HAVE_WCSNCASECMP} = 0; then
  AC_LIBOBJ([wcsncasecmp])dnl
  ## end libobj
fi
gl_WCHAR_MODULE_INDICATOR([wcsncasecmp])dnl
dnl# from gnulib module wctype-h:
AC_REQUIRE([gl_WCTYPE_H])dnl
AC_REQUIRE([gl_WCTYPE_H_DEFAULTS])dnl
AC_REQUIRE([gl_FUNC_TOWCTRANS])dnl
AC_REQUIRE([gl_FUNC_WCTRANS])dnl
AC_REQUIRE([gl_FUNC_WCTYPE])dnl
dnl# other:
if test "x${gl_LIBOBJS}" = "x"; then
  if test "x${LIBOBJS}" != "x"; then
    test -z "${gl_LIBOBJS}" && test -n "${LIBOBJS}" && export gl_LIBOBJS="${LIBOBJS}"
  fi
fi
AC_SUBST([gl_LIBOBJS])dnl
])dnl
