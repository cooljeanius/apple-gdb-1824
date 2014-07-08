dnl# Instead of using gnulib-tool, copy the macros we need from modules
dnl# manually:
AC_DEFUN([gl_INIT_MANUALLY],[
AC_REQUIRE([AC_PROG_LN_S])
AC_REQUIRE([AC_PROG_RANLIB])
AC_REQUIRE([AC_PROG_SED])
AC_REQUIRE([AC_HEADER_STDBOOL]) dnl# also calls the "_CHECK" version of it
AC_REQUIRE([AC_HEADER_TIME])
AC_REQUIRE([gl_BIGENDIAN])
AC_REQUIRE([AC_C_CONST])
AC_REQUIRE([AC_C_RESTRICT])
AC_REQUIRE([AC_TYPE_SIZE_T])
AC_REQUIRE([AC_FUNC_ALLOCA])
AC_REQUIRE([AC_FUNC_MEMCMP])
dnl# from gnulib module alloca-opt:
AC_REQUIRE([gl_FUNC_ALLOCA])
dnl# from gnulib module configmake:
AC_REQUIRE([gl_CONFIGMAKE_PREP])
dnl# from gnulib module dirent:
AC_REQUIRE([gl_DIRENT_H])
dnl# from gnulib module dirfd:
AC_REQUIRE([gl_FUNC_DIRFD])
if test "x${ac_cv_func_dirfd}" = "xno" && test "x${gl_cv_func_dirfd_macro}" = "xno"; then
  AC_LIBOBJ([dirfd])
  gl_PREREQ_DIRFD
fi
gl_DIRENT_MODULE_INDICATOR([dirfd])
dnl# from gnulib module errno:
AC_REQUIRE([gl_HEADER_ERRNO_H])
dnl# from gnulib module float:
AC_REQUIRE([gl_FLOAT_H])
if test ${REPLACE_FLOAT_LDBL} = 1; then
  AC_LIBOBJ([float])
fi
if test ${REPLACE_ITOLD} = 1; then
  AC_LIBOBJ([itold])
fi
dnl# from gnulib module fnmatch:
AC_REQUIRE([gl_FUNC_FNMATCH_POSIX])
if test -n "${FNMATCH_H}"; then
  AC_LIBOBJ([fnmatch])
  gl_PREREQ_FNMATCH
fi
dnl# from gnulib module fnmatch-gnu:
AC_REQUIRE([gl_FUNC_FNMATCH_GNU])
if test -n "${FNMATCH_H}"; then
  AC_LIBOBJ([fnmatch])
  gl_PREREQ_FNMATCH
fi
dnl# from gnulib module frexp:
AC_REQUIRE([gl_FUNC_FREXP])
if test "x${gl_func_frexp}" != "xyes"; then
  AC_LIBOBJ([frexp])
fi
gl_MATH_MODULE_INDICATOR([frexp])
dnl# from gnulib module frexpl:
AC_REQUIRE([gl_FUNC_FREXPL])
if test ${HAVE_DECL_FREXPL} = 0 || test "x${gl_func_frexpl}" = "xno"; then
  AC_LIBOBJ([frexpl])
fi
gl_MATH_MODULE_INDICATOR([frexpl])
dnl# from gnulib module inttypes:
AC_REQUIRE([gl_INTTYPES_H])
dnl# from gnulib module inttypes-incomplete:
AC_REQUIRE([gl_INTTYPES_INCOMPLETE])
dnl# from gnulib module isnand-nolibm:
AC_REQUIRE([gl_FUNC_ISNAND_NO_LIBM])
if test "x${gl_func_isnand_no_libm}" != "xyes"; then
  AC_LIBOBJ([isnand])
  gl_PREREQ_ISNAND
fi
dnl# from gnulib module isnanl-nolibm:
AC_REQUIRE([gl_FUNC_ISNANL_NO_LIBM])
if test "x${gl_func_isnanl_no_libm}" != "xyes"; then
  AC_LIBOBJ([isnanl])
  gl_PREREQ_ISNANL
fi
dnl# from gnulib module iswctype:
AC_REQUIRE([gl_FUNC_ISWCTYPE])
if test ${HAVE_WCTYPE_T} = 0; then
  AC_LIBOBJ([iswctype])
fi
gl_WCTYPE_MODULE_INDICATOR([iswctype])
dnl# from gnulib module link-warning:
AC_REQUIRE([gl_FEATURES_H])
dnl# from gnulib module localcharset:
AC_REQUIRE([gl_LOCALCHARSET])
if test "x${LOCALCHARSET_TESTS_ENVIRONMENT}" = "x"; then
  if test -n "${gl_source_base}"; then
    test -z "${LOCALCHARSET_TESTS_ENVIRONMENT}" && export LOCALCHARSET_TESTS_ENVIRONMENT="CHARSETALIASDIR=\"\$(abs_top_builddir)/${gl_source_base}\""
  else
    test -z "${LOCALCHARSET_TESTS_ENVIRONMENT}" && export LOCALCHARSET_TESTS_ENVIRONMENT="CHARSETALIASDIR=\"\$(abs_top_builddir)\""
  fi
fi
AC_SUBST([LOCALCHARSET_TESTS_ENVIRONMENT])
dnl# from gnulib module math:
AC_REQUIRE([gl_MATH_H])
dnl# from gnulib module mbrtowc:
AC_REQUIRE([gl_FUNC_MBRTOWC])
if test ${HAVE_MBRTOWC} = 0 || test ${REPLACE_MBRTOWC} = 1; then
  AC_LIBOBJ([mbrtowc])
  gl_PREREQ_MBRTOWC
fi
gl_WCHAR_MODULE_INDICATOR([mbrtowc])
dnl# from gnulib module mbsinit:
AC_REQUIRE([gl_FUNC_MBSINIT])
if test ${HAVE_MBSINIT} = 0 || test ${REPLACE_MBSINIT} = 1; then
  AC_LIBOBJ([mbsinit])
  gl_PREREQ_MBSINIT
fi
gl_WCHAR_MODULE_INDICATOR([mbsinit])
dnl# from gnulib module mbsrtowcs:
AC_REQUIRE([gl_FUNC_MBSRTOWCS])
if test ${HAVE_MBSRTOWCS} = 0 || test ${REPLACE_MBSRTOWCS} = 1; then
  AC_LIBOBJ([mbsrtowcs])
  AC_LIBOBJ([mbsrtowcs-state])
  gl_PREREQ_MBSRTOWCS
fi
gl_WCHAR_MODULE_INDICATOR([mbsrtowcs])
dnl# from gnulib module memchr:
AC_REQUIRE([gl_FUNC_MEMCHR])
if test ${HAVE_MEMCHR} = 0 || test ${REPLACE_MEMCHR} = 1; then
  AC_LIBOBJ([memchr])
  gl_PREREQ_MEMCHR
fi
gl_STRING_MODULE_INDICATOR([memchr])
dnl# from gnulib module memcmp:
AC_REQUIRE([gl_FUNC_MEMCMP])
if test "x${gl_func_memcmp}" = "xno"; then
  AC_LIBOBJ([memcmp])
  gl_PREREQ_MEMCMP
fi
dnl# from gnulib module memmem:
AC_REQUIRE([gl_FUNC_MEMMEM])
if test ${HAVE_MEMMEM} = 0 || test ${REPLACE_MEMMEM} = 1; then
  AC_LIBOBJ([memmem])
fi
dnl# from gnulib module memmem-simple:
AC_REQUIRE([gl_FUNC_MEMMEM_SIMPLE])
dnl# (libobj is already set above)
gl_STRING_MODULE_INDICATOR([memmem])
dnl# from gnulib module mempcpy:
AC_REQUIRE([gl_FUNC_MEMPCPY])
if test ${HAVE_MEMPCPY} = 0; then
  AC_LIBOBJ([mempcpy])
  gl_PREREQ_MEMPCPY
fi
gl_STRING_MODULE_INDICATOR([mempcpy])
dnl# from gnulib module pathmax:
AC_REQUIRE([gl_PATHMAX])
dnl# from gnulib module stdbool:
AC_REQUIRE([AM_STDBOOL_H])
dnl# from gnulib module stddef:
AC_REQUIRE([gl_STDDEF_H])
dnl# from gnulib module stdint:
AC_REQUIRE([gl_STDINT_H])
dnl# from gnulib module string:
AC_REQUIRE([gl_HEADER_STRING_H])
dnl# from gnulib module strstr:
AC_REQUIRE([gl_FUNC_STRSTR])
if test ${REPLACE_STRSTR} = 1; then
  AC_LIBOBJ([strstr])
fi
dnl# from gnulib module strstr-simple:
AC_REQUIRE([gl_FUNC_STRSTR_SIMPLE])
dnl# (libobj is already set above)
gl_STRING_MODULE_INDICATOR([strstr])
dnl# from gnulib module sys_stat:
AC_REQUIRE([gl_HEADER_SYS_STAT_H])
AC_REQUIRE([AC_PROG_MKDIR_P])
dnl# from gnulib module sys_types:
AC_REQUIRE([gl_SYS_TYPES_H])
dnl# from gnulib module time:
AC_REQUIRE([gl_HEADER_TIME_H])
dnl# from gnulib module unistd:
AC_REQUIRE([gl_UNISTD_H])
dnl# from gnulib module wchar:
AC_REQUIRE([gl_WCHAR_H])
AC_REQUIRE([AC_FUNC_MBRTOWC])
AC_REQUIRE([AC_TYPE_MBSTATE_T])
dnl# from gnulib module wctype-h:
AC_REQUIRE([gl_WCTYPE_H])
AC_REQUIRE([gl_WCTYPE_H_DEFAULTS])
AC_REQUIRE([gl_FUNC_TOWCTRANS])
AC_REQUIRE([gl_FUNC_WCTRANS])
AC_REQUIRE([gl_FUNC_WCTYPE])
])
