# gnulib-use-manually.m4 serial 3                          -*- Autoconf -*-
dnl# Instead of using gnulib-tool, copy the macros we need from modules
dnl# manually.  This macro is dependent on the exact set of modules used
dnl# here, and as such, would not make much sense elsewhere.
AC_DEFUN([gl_INIT_MANUALLY],[
AC_REQUIRE([AC_PROG_CXX])dnl
AC_REQUIRE([AC_PROG_LN_S])dnl
AC_REQUIRE([AC_PROG_RANLIB])dnl
AC_REQUIRE([AC_PROG_SED])dnl
AC_REQUIRE([AC_HEADER_STDBOOL]) dnl# also calls the "_CHECK" version of it
m4_ifdef([AC_HEADER_TIME],[
AC_REQUIRE([AC_HEADER_TIME])dnl
])dnl
AC_REQUIRE([gl_BIGENDIAN])dnl
AC_REQUIRE([AC_C_CONST])dnl
AC_REQUIRE([AC_C_RESTRICT])dnl
AC_REQUIRE([AC_TYPE_SIZE_T])dnl
AC_REQUIRE([AC_FUNC_ALLOCA])dnl
AC_REQUIRE([AC_FUNC_MEMCMP])dnl
AC_REQUIRE([AC_FUNC_MKTIME])dnl
dnl# from gnulib module alloca-opt:
AC_REQUIRE([gl_FUNC_ALLOCA])dnl
dnl# from gnulib module assert:
AC_REQUIRE([gl_ASSERT])dnl
dnl# from gnulib module assert-h:
AC_REQUIRE([gl_ASSERT_H])dnl
dnl# from gnulib module autobuild:
AC_REQUIRE([AB_INIT])dnl
dnl# from gnulib module bison-i18n:
m4_ifdef([AM_GNU_GETTEXT],[
  if test "x${USE_NLS}" = "x"; then
    test -z "${USE_NLS}" && export USE_NLS="no"
  fi
  AC_REQUIRE([BISON_I18N])dnl
])dnl
dnl# from gnulib module btowc:
AC_REQUIRE([gl_FUNC_BTOWC])dnl
  ## set up btowc libobj if needed:
if test ${HAVE_BTOWC} = 0 || test ${REPLACE_BTOWC} = 1; then
  AC_LIBOBJ([btowc])dnl
  gl_PREREQ_BTOWC
fi
gl_WCHAR_MODULE_INDICATOR([btowc])dnl
dnl# from gnulib module builtin-expect:
AC_REQUIRE([gl___BUILTIN_EXPECT])dnl
dnl# from gnulib module chdir-long:
AC_REQUIRE([gl_FUNC_CHDIR_LONG])dnl
  ## set up chdir-long libobj if needed:
if test "x${gl_cv_have_arbitrary_file_name_length_limit}" = "xyes"; then
  AC_LIBOBJ([chdir-long])dnl
  gl_PREREQ_CHDIR_LONG
fi
dnl# from gnulib module configmake:
AC_REQUIRE([gl_CONFIGMAKE_PREP])dnl
dnl# from gnulib module connect:
m4_ifdef([gl_HEADER_SYS_SOCKET],[
  AC_REQUIRE([gl_HEADER_SYS_SOCKET])dnl
],[
  m4_ifdef([gl_SYS_SOCKET_H],[
    AC_REQUIRE([gl_SYS_SOCKET_H])dnl
  ],[
    AC_CHECK_HEADERS_ONCE([sys/socket.h winsock2.h])dnl
  ])dnl
])dnl
  ## set up connect libobj if needed:
if test "x${ac_cv_header_winsock2_h}" = "xyes"; then
  AC_LIBOBJ([connect])dnl
  ## end connect libobj
fi
gl_SYS_SOCKET_MODULE_INDICATOR([connect])dnl
dnl# from gnulib module closedir:
AC_REQUIRE([gl_FUNC_CLOSEDIR])dnl
  ## set up closedir libobj if needed:
if test ${HAVE_CLOSEDIR} = 0 || test ${REPLACE_CLOSEDIR} = 1; then
  AC_LIBOBJ([closedir])dnl
  ## end closedir libobj
fi
gl_DIRENT_MODULE_INDICATOR([closedir])dnl
dnl# from gnulib module ctime:
AC_REQUIRE([gl_FUNC_CTIME])dnl
  ## set up ctime libobj if needed:
if test ${REPLACE_CTIME} = 1; then
  AC_LIBOBJ([ctime])dnl
  ## end ctime libobj
fi
gl_TIME_MODULE_INDICATOR([ctime])
dnl# from gnulib module dirent:
AC_REQUIRE([gl_DIRENT_H])dnl
dnl# from gnulib module dirent-safer:
AC_REQUIRE([gl_DIRENT_SAFER])dnl
gl_MODULE_INDICATOR([dirent-safer])dnl
dnl# from gnulib module dirfd:
AC_REQUIRE([gl_FUNC_DIRFD])dnl
  ## set up dirfd libobj if needed:
if test "x${ac_cv_func_dirfd}" = "xno" && test "x${gl_cv_func_dirfd_macro}" = "xno"; then
  AC_LIBOBJ([dirfd])dnl
  gl_PREREQ_DIRFD
fi
gl_DIRENT_MODULE_INDICATOR([dirfd])dnl
dnl# from gnulib module double-slash-root:
AC_REQUIRE([gl_DOUBLE_SLASH_ROOT])dnl
dnl# from gnulib module dup2:
AC_REQUIRE([gl_FUNC_DUP2])dnl
m4_ifdef([gl_PREREQ_DUP2],[
  ## set up dup2 libobj if needed:
if test "x${HAVE_DUP2}" != "x" && test "x${REPLACE_DUP2}" != "x"; then
  test -n "${HAVE_DUP2}" && test -n "${REPLACE_DUP2}"
  if test ${HAVE_DUP2} = 0 || test ${REPLACE_DUP2} = 1; then
    AC_LIBOBJ([dup2])dnl
    gl_PREREQ_DUP2
  fi
fi
])dnl
gl_UNISTD_MODULE_INDICATOR([dup2])dnl
dnl# from gnulib module environ:
AC_REQUIRE([gl_ENVIRON])dnl
gl_UNISTD_MODULE_INDICATOR([environ])dnl
dnl# from gnulib module errno:
AC_REQUIRE([gl_HEADER_ERRNO_H])dnl
dnl# from gnulib module error:
AC_REQUIRE([gl_ERROR])dnl
  ## set up error libobj if needed:
if test "x${ac_cv_lib_error_at_line}" = "xno"; then
  AC_LIBOBJ([error])dnl
  gl_PREREQ_ERROR
fi
dnl# (skip the pointless gettext stuff for it)
dnl# from gnulib module euidaccess:
AC_REQUIRE([gl_FUNC_EUIDACCESS])dnl
  ## set up euidaccess libobj if needed:
if test ${HAVE_EUIDACCESS} = 0; then
  AC_LIBOBJ([euidaccess])dnl
  gl_PREREQ_EUIDACCESS
fi
gl_UNISTD_MODULE_INDICATOR([euidaccess])dnl
dnl# from gnulib module extensions:
AC_REQUIRE([gl_USE_SYSTEM_EXTENSIONS])dnl
dnl# from gnulib module extern-inline:
AC_REQUIRE([gl_EXTERN_INLINE])dnl
dnl# from gnulib module fclose:
AC_REQUIRE([gl_FUNC_FCLOSE])dnl
  ## set up fclose libobj if needed:
if test ${REPLACE_FCLOSE} = 1; then
  AC_LIBOBJ([fclose])dnl
  ## end fclose libobj
fi
gl_STDIO_MODULE_INDICATOR([fclose])dnl
dnl# from gnulib module fcntl:
AC_REQUIRE([gl_FUNC_FCNTL])dnl
  ## set up fcntl libobj if needed:
if test ${HAVE_FCNTL} = 0 || test ${REPLACE_FCNTL} = 1; then
  AC_LIBOBJ([fcntl])dnl
  ## end fcntl libobj
fi
gl_FCNTL_MODULE_INDICATOR([fcntl])dnl
dnl# from gnulib module fcntl-h:
AC_REQUIRE([gl_FCNTL_H])dnl
dnl# from gnulib module fcntl-safer:
AC_REQUIRE([gl_FCNTL_SAFER])dnl
gl_MODULE_INDICATOR([fcntl-safer])dnl
dnl# from gnulib module fileblocks:
AC_REQUIRE([gl_FILEBLOCKS])dnl
  ## set up fileblocks libobj if needed:
if test "x${ac_cv_member_struct_stat_st_blocks}" = "xno"; then
  AC_LIBOBJ([fileblocks])dnl
  gl_PREREQ_FILEBLOCKS
fi
dnl# from gnulib module flexmember:
AC_REQUIRE([AC_C_FLEXIBLE_ARRAY_MEMBER])dnl
dnl# from gnulib module float:
AC_REQUIRE([gl_FLOAT_H])dnl
  ## set up float and itold libobjs if needed:
if test ${REPLACE_FLOAT_LDBL} = 1; then
  AC_LIBOBJ([float])dnl
  ## end first libobj (float) from "float"
fi
if test ${REPLACE_ITOLD} = 1; then
  AC_LIBOBJ([itold])dnl
  ## end second libobj (itold) from "float"
fi
dnl# from gnulib module fnmatch:
AC_REQUIRE([gl_FUNC_FNMATCH_POSIX])
if test -n "${FNMATCH_H}"; then
  AC_LIBOBJ([fnmatch])dnl
  gl_PREREQ_FNMATCH
fi
dnl# from gnulib module fnmatch-gnu:
AC_REQUIRE([gl_FUNC_FNMATCH_GNU])dnl
  ## set up fnmatch libobj if needed:
if test -n "${FNMATCH_H}"; then
  AC_LIBOBJ([fnmatch])dnl
  gl_PREREQ_FNMATCH
fi
dnl# from gnulib module fopen:
AC_REQUIRE([gl_FUNC_FOPEN])dnl
  ## set up fopen libobj if needed:
if test ${REPLACE_FOPEN} = 1; then
  AC_LIBOBJ([fopen])dnl
  gl_PREREQ_FOPEN
fi
gl_STDIO_MODULE_INDICATOR([fopen])dnl
dnl# from gnulib module fpieee:
AC_REQUIRE([gl_FP_IEEE])dnl
dnl# from gnulib module frexp:
AC_REQUIRE([gl_FUNC_FREXP])dnl
  ## set up frexp libobj if needed:
if test "x${gl_func_frexp}" != "xyes"; then
  AC_LIBOBJ([frexp])dnl
  ## end frexp libobj
fi
gl_MATH_MODULE_INDICATOR([frexp])dnl
dnl# from gnulib module frexpl:
AC_REQUIRE([gl_FUNC_FREXPL])dnl
  ## set up frexpl libobj if needed:
if test ${HAVE_DECL_FREXPL} = 0 || test "x${gl_func_frexpl}" = "xno"; then
  AC_LIBOBJ([frexpl])dnl
  ## end frexpl libobj
fi
gl_MATH_MODULE_INDICATOR([frexpl])dnl
dnl# from gnulib module fstatat:
AC_REQUIRE([gl_FUNC_FSTATAT])dnl
  ## set up fstatat libobj if needed:
if test ${HAVE_FSTATAT} = 0 || test ${REPLACE_FSTATAT} = 1; then
  AC_LIBOBJ([fstatat])dnl
  ## end fstatat libobj
fi
gl_SYS_STAT_MODULE_INDICATOR([fstatat])dnl
dnl# from gnulib module getcwd:
AC_REQUIRE([gl_FUNC_GETCWD])dnl
  ## set up getcwd libobj if needed:
if test ${REPLACE_GETCWD} = 1; then
  AC_LIBOBJ([getcwd])dnl
  gl_PREREQ_GETCWD
fi
gl_MODULE_INDICATOR([getcwd])dnl
gl_UNISTD_MODULE_INDICATOR([getcwd])dnl
dnl# from gnulib module getcwd-lgpl:
AC_REQUIRE([gl_FUNC_GETCWD_LGPL])dnl
  ## set up getcwd-lgpl libobj if needed:
if test ${REPLACE_GETCWD} = 1; then
  AC_LIBOBJ([getcwd-lgpl])dnl
  ## end getcwd-lgpl libobj
fi
dnl# from gnulib module getpagesize:
AC_REQUIRE([gl_FUNC_GETPAGESIZE])dnl
  ## set up getpagesize libobj if needed:
if test ${REPLACE_GETPAGESIZE} = 1; then
  AC_LIBOBJ([getpagesize])dnl
  ## end getpagesize libobj
fi
gl_UNISTD_MODULE_INDICATOR([getpagesize])dnl
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
  ## set up isnand libobj if needed:
if test "x${gl_func_isnand_no_libm}" != "xyes"; then
  AC_LIBOBJ([isnand])dnl
  gl_PREREQ_ISNAND
fi
dnl# from gnulib module isnanl-nolibm:
AC_REQUIRE([gl_FUNC_ISNANL_NO_LIBM])dnl
  ## set up isnanl libobj if needed:
if test "x${gl_func_isnanl_no_libm}" != "xyes"; then
  AC_LIBOBJ([isnanl])dnl
  gl_PREREQ_ISNANL
fi
dnl# from gnulib module iswctype:
AC_REQUIRE([gl_FUNC_ISWCTYPE])dnl
  ## set up iswctype libobj if needed:
if test ${HAVE_WCTYPE_T} = 0; then
  AC_LIBOBJ([iswctype])dnl
  ## end iswctype libobj
fi
gl_WCTYPE_MODULE_INDICATOR([iswctype])dnl
dnl# from gnulib module largefile:
AC_REQUIRE([AC_SYS_LARGEFILE])dnl
AC_REQUIRE([gl_LARGEFILE])dnl
dnl# from gnulib module ldd:
AC_REQUIRE([gl_LDD])dnl
dnl# from gnulib module ldexpl:
AC_REQUIRE([gl_FUNC_LDEXPL])dnl
  ## set up ldexpl libobj if needed:
if test ${HAVE_DECL_LDEXPL} = 0 || test "x${gl_func_ldexpl}" = "xno"; then
  AC_LIBOBJ([ldexpl])dnl
  ## end ldexpl libobj
fi
gl_MATH_MODULE_INDICATOR([ldexpl])dnl
dnl# from gnulib module limits-h:
AC_REQUIRE([gl_LIMITS_H])dnl
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
dnl# from gnulib module locale:
AC_REQUIRE([gl_LOCALE_H])dnl
dnl# from gnulib module localtime:
AC_REQUIRE([gl_FUNC_LOCALTIME])dnl
  ## set up localtime libobj if needed:
if test ${REPLACE_LOCALTIME} = 1; then
  AC_LIBOBJ([localtime])dnl
  ## end localtime libobj
fi
gl_TIME_MODULE_INDICATOR([localtime])dnl
dnl# from gnulib module localtime-buffer:
AC_REQUIRE([gl_LOCALTIME_BUFFER_DEFAULTS])dnl
  ## maybe set up localtime-buffer libobj:
if test -f localtime-buffer.c; then
  AC_LIBOBJ([localtime-buffer])dnl
  ## end localtime-buffer libobj
fi
dnl# from gnulib module longlong:
AC_REQUIRE([AC_TYPE_LONG_LONG_INT])dnl
AC_REQUIRE([AC_TYPE_UNSIGNED_LONG_LONG_INT])dnl
dnl# from gnulib module lstat:
AC_REQUIRE([gl_FUNC_LSTAT])dnl
  ## set up lstat libobj if needed:
if test ${REPLACE_LSTAT} = 1; then
  AC_LIBOBJ([lstat])dnl
  gl_PREREQ_LSTAT
fi
gl_SYS_STAT_MODULE_INDICATOR([lstat])dnl
dnl# from gnulib module malloc-gnu:
AC_REQUIRE([AC_FUNC_MALLOC])dnl
AC_REQUIRE([gl_FUNC_MALLOC_GNU])dnl
  ## set up malloc libobj if needed:
if test "x${REPLACE_MALLOC}" != "x"; then
  test -n "${REPLACE_MALLOC}" || echo "did we check for malloc properly?"
  if test ${REPLACE_MALLOC} = 1; then
    AC_LIBOBJ([malloc])dnl
    ## end malloc libobj
  fi
fi
gl_MODULE_INDICATOR([malloc-gnu])dnl
dnl# from gnulib module malloc-posix:
AC_REQUIRE([gl_FUNC_MALLOC_POSIX])dnl
dnl# (libobj is already set above)
gl_STDLIB_MODULE_INDICATOR([malloc-posix])dnl
dnl# from gnulib module malloca:
AC_REQUIRE([gl_MALLOCA])dnl
dnl# from gnulib module math:
AC_REQUIRE([gl_MATH_H])dnl
dnl# from gnulib module mbrtowc:
AC_REQUIRE([gl_FUNC_MBRTOWC])dnl
  ## set up mbrtowc libobj if needed:
if test ${HAVE_MBRTOWC} = 0 || test ${REPLACE_MBRTOWC} = 1; then
  AC_LIBOBJ([mbrtowc])dnl
  gl_PREREQ_MBRTOWC
fi
gl_WCHAR_MODULE_INDICATOR([mbrtowc])dnl
dnl# from gnulib module mbsinit:
AC_REQUIRE([gl_FUNC_MBSINIT])dnl
  ## set up mbsinit libobj if needed:
if test ${HAVE_MBSINIT} = 0 || test ${REPLACE_MBSINIT} = 1; then
  AC_LIBOBJ([mbsinit])dnl
  gl_PREREQ_MBSINIT
fi
gl_WCHAR_MODULE_INDICATOR([mbsinit])dnl
dnl# from gnulib module mbsrtowcs:
AC_REQUIRE([gl_FUNC_MBSRTOWCS])dnl
  ## set up mbsrtowcs libobj(s) if needed:
if test ${HAVE_MBSRTOWCS} = 0 || test ${REPLACE_MBSRTOWCS} = 1; then
  AC_LIBOBJ([mbsrtowcs])dnl
  AC_LIBOBJ([mbsrtowcs-state])dnl
  gl_PREREQ_MBSRTOWCS
fi
gl_WCHAR_MODULE_INDICATOR([mbsrtowcs])dnl
dnl# from gnulib module memchr:
AC_REQUIRE([gl_FUNC_MEMCHR])dnl
m4_ifdef([gl_PREREQ_MEMCHR],[
  ## set up memchr libobj if needed:
if test "x${HAVE_MEMCHR}" != "x" && test "x${REPLACE_MEMCHR}" != "x"; then
  test -n "${HAVE_MEMCHR}" && test -n "${REPLACE_MEMCHR}"
  if test ${HAVE_MEMCHR} = 0 || test ${REPLACE_MEMCHR} = 1; then
    AC_LIBOBJ([memchr])dnl
    gl_PREREQ_MEMCHR
  fi
fi
])dnl
gl_STRING_MODULE_INDICATOR([memchr])dnl
dnl# from gnulib module memcmp:
AC_REQUIRE([gl_FUNC_MEMCMP])dnl
  ## set up memcmp libobj if needed:
if test "x${gl_func_memcmp}" = "xno"; then
  AC_LIBOBJ([memcmp])dnl
  gl_PREREQ_MEMCMP
fi
dnl# from gnulib module memmem:
AC_REQUIRE([gl_FUNC_MEMMEM])dnl
  ## set memmem up libobj if needed:
if test ${HAVE_MEMMEM} = 0 || test ${REPLACE_MEMMEM} = 1; then
  AC_LIBOBJ([memmem])dnl
  ## end memmem libobj
fi
dnl# from gnulib module memmem-simple:
AC_REQUIRE([gl_FUNC_MEMMEM_SIMPLE])dnl
dnl# (libobj is already set above)
gl_STRING_MODULE_INDICATOR([memmem])dnl
dnl# from gnulib module mempcpy:
AC_REQUIRE([gl_FUNC_MEMPCPY])dnl
  ## set up mempcpy libobj if needed:
if test ${HAVE_MEMPCPY} = 0; then
  AC_LIBOBJ([mempcpy])dnl
  gl_PREREQ_MEMPCPY
fi
gl_STRING_MODULE_INDICATOR([mempcpy])dnl
dnl# from gnulib module memrchr
AC_REQUIRE([gl_FUNC_MEMRCHR])dnl
  ## set up memrchr libobj if needed:
if test "x${ac_cv_func_memrchr}" = "xno"; then
  AC_LIBOBJ([memrchr])dnl
  gl_PREREQ_MEMRCHR
fi
gl_STRING_MODULE_INDICATOR([memrchr])dnl
dnl# from gnulib module mkdir:
AC_REQUIRE([gl_FUNC_MKDIR])dnl
  ## set up mkdir libobj if needed:
if test ${REPLACE_MKDIR} = 1; then
  AC_LIBOBJ([mkdir])dnl
  ## end mkdir libobj
fi
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
dnl# from gnulib module pclose:
AC_REQUIRE([gl_FUNC_PCLOSE])dnl
  ## set up pclose libobj if needed:
if test ${HAVE_PCLOSE} = 0; then
  AC_LIBOBJ([pclose])dnl
  gl_PREREQ_PCLOSE
fi
gl_STDIO_MODULE_INDICATOR([pclose])dnl
dnl# from gnulib module perror:
AC_REQUIRE([gl_FUNC_PERROR])dnl
  ## set up perror libobj if needed:
if test ${REPLACE_PERROR} = 1; then
  AC_LIBOBJ([perror])dnl
  ## end perror libobj
fi
gl_STRING_MODULE_INDICATOR([perror])dnl
dnl# from gnulib module popen:
AC_REQUIRE([gl_FUNC_POPEN])dnl
  ## set up popen libobj if needed:
if test ${HAVE_POPEN} = 0 || test ${REPLACE_POPEN} = 1; then
  AC_LIBOBJ([popen])dnl
  gl_PREREQ_POPEN
fi
gl_STDIO_MODULE_INDICATOR([popen])dnl
dnl# from gnulib module putenv:
AC_REQUIRE([gl_FUNC_PUTENV])dnl
  ## set up putenv libobj if needed:
if test ${REPLACE_PUTENV} = 1; then
  AC_LIBOBJ([putenv])dnl
  gl_PREREQ_PUTENV
fi
gl_STDLIB_MODULE_INDICATOR([putenv])dnl
dnl# from gnulib module readdir:
AC_REQUIRE([gl_FUNC_READDIR])dnl
  ## set up readdir libobj if needed:
if test ${HAVE_READDIR} = 0; then
  AC_LIBOBJ([readdir])dnl
  ## end readdir libobj
fi
gl_DIRENT_MODULE_INDICATOR([readdir])dnl
dnl# from gnulib module realloc-gnu:
AC_REQUIRE([AC_FUNC_REALLOC])dnl
AC_REQUIRE([gl_FUNC_REALLOC_GNU])dnl
  ## set up realloc libobj if needed:
if test "x${REPLACE_REALLOC}" != "x"; then
  test -n "${REPLACE_REALLOC}" || echo "did we check for realloc properly?"
  if test ${REPLACE_REALLOC} = 1; then
    AC_LIBOBJ([realloc])dnl
    ## end realloc libobj
  fi
fi
gl_MODULE_INDICATOR([realloc-gnu])dnl
dnl# from gnulib module realloc-posix:
AC_REQUIRE([gl_FUNC_REALLOC_POSIX])dnl
dnl# (libobj is already set above)
gl_STDLIB_MODULE_INDICATOR([realloc-posix])dnl
dnl# from gnulib module recv:
dnl# (already required header check above)
if test "x${ac_cv_header_winsock2_h}" = "xyes"; then
  AC_LIBOBJ([recv])dnl
  ## end recv libobj
fi
gl_SYS_SOCKET_MODULE_INDICATOR([recv])dnl
dnl# from gnulib module regex:
AC_REQUIRE([gl_REGEX])dnl
  ## set up regex libobj if needed:
if test "x${ac_use_included_regex}" = "xyes"; then
  AC_LIBOBJ([regex])dnl
  gl_PREREQ_REGEX
fi
dnl# from gnulib module secure_getenv:
AC_REQUIRE([gl_FUNC_SECURE_GETENV])dnl
  ## set up secure_getenv libobj if needed:
if test ${HAVE_SECURE_GETENV} = 0; then
  AC_LIBOBJ([secure_getenv])dnl
  gl_PREREQ_SECURE_GETENV
fi
gl_STDLIB_MODULE_INDICATOR([secure_getenv])dnl
dnl# from gnulib module send:
dnl# (already required header check above)
if test "x${ac_cv_header_winsock2_h}" = "xyes"; then
  AC_LIBOBJ([send])dnl
  ## end send libobj
fi
gl_SYS_SOCKET_MODULE_INDICATOR([send])dnl
dnl# from gnulib module setenv:
AC_REQUIRE([gl_FUNC_SETENV])dnl
  ## set up setenv libobj if needed:
if test ${HAVE_SETENV} = 0 || test ${REPLACE_SETENV} = 1; then
  AC_LIBOBJ([setenv])dnl
  ## end setenv libobj
fi
gl_STDLIB_MODULE_INDICATOR([setenv])dnl
dnl# from gnulib module sh-filename:
AC_REQUIRE([gl_SH_FILENAME])dnl
dnl# from gnulib module sig2str:
AC_REQUIRE([gl_FUNC_SIG2STR])dnl
  ## set up sig2str libobj if needed:
if test "x${ac_cv_func_sig2str}" = "xno"; then
  AC_LIBOBJ([sig2str])dnl
  gl_PREREQ_SIG2STR
fi
dnl# from gnulib module sigaction:
AC_REQUIRE([gl_SIGACTION])dnl
  ## set up sigaction libobj if needed:
if test ${HAVE_SIGACTION} = 0; then
  AC_LIBOBJ([sigaction])dnl
  gl_PREREQ_SIGACTION
fi
gl_SIGNAL_MODULE_INDICATOR([sigaction])dnl
dnl# from gnulib module signal-h:
AC_REQUIRE([gl_SIGNAL_H])dnl
dnl# from gnulib module sigpipe:
AC_REQUIRE([gl_SIGNAL_SIGPIPE])dnl
dnl## Define the C macro GNULIB_SIGPIPE to 1:
gl_MODULE_INDICATOR([sigpipe])dnl
dnl## Define the substituted variable GNULIB_SIGNAL_H_SIGPIPE to 1:
AC_REQUIRE([gl_SIGNAL_H_DEFAULTS])dnl
  ## ...and export it:
export GNULIB_SIGNAL_H_SIGPIPE=1
dnl## Define the substituted variable GNULIB_STDIO_H_SIGPIPE to 1:
AC_REQUIRE([gl_STDIO_H_DEFAULTS])dnl
AC_REQUIRE([gl_ASM_SYMBOL_PREFIX])dnl
  ## ...and export it:
export GNULIB_STDIO_H_SIGPIPE=1
dnl## Define the substituted variable GNULIB_UNISTD_H_SIGPIPE to 1:
AC_REQUIRE([gl_UNISTD_H_DEFAULTS])dnl
  ## ...and export it:
export GNULIB_UNISTD_H_SIGPIPE=1
dnl# from gnulib module sigprocmask:
AC_REQUIRE([gl_SIGNALBLOCKING])dnl
  ## set up sigprocmask libobj if needed:
if test ${HAVE_POSIX_SIGNALBLOCKING} = 0; then
  AC_LIBOBJ([sigprocmask])dnl
  gl_PREREQ_SIGPROCMASK
fi
gl_SIGNAL_MODULE_INDICATOR([sigprocmask])dnl
dnl# from gnulib module snippet/link-warning:
AC_REQUIRE([gl_FEATURES_H])dnl
dnl# from gnulib module snippet/warn-on-use:
AC_REQUIRE([gl_WARN_ON_USE_PREPARE])dnl
dnl# from gnulib module ssize_t:
AC_REQUIRE([gt_TYPE_SSIZE_T])dnl
dnl# from gnulib module stat:
AC_REQUIRE([gl_FUNC_STAT])dnl
  ## set up stat libobj if needed:
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
m4_ifdef([AM_STDBOOL_H],[
  AC_REQUIRE([AM_STDBOOL_H])dnl
],[
  m4_ifdef([gl_C_BOOL],[
    AC_REQUIRE([gl_C_BOOL])dnl
  ],[
    m4_ifdef([gl_STDBOOL_H],[
      AC_REQUIRE([gl_STDBOOL_H])dnl
    ],[
      : dnl# (regular stdbool check should have already been done)
    ])dnl
  ])dnl
])dnl
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
  ## set up strerror libobj if needed:
if test ${REPLACE_STRERROR} = 1; then
  AC_LIBOBJ([strerror])dnl
  ## end strerror libobj
fi
gl_MODULE_INDICATOR([strerror])dnl
gl_STRING_MODULE_INDICATOR([strerror])dnl
dnl# from gnulib module strerror_r-posix:
AC_REQUIRE([gl_FUNC_STRERROR_R])dnl
  ## set up strerror_r libobj if needed:
if test ${HAVE_DECL_STRERROR_R} = 0 || test ${REPLACE_STRERROR_R} = 1; then
  AC_LIBOBJ([strerror_r])dnl
  gl_PREREQ_STRERROR_R
fi
gl_STRING_MODULE_INDICATOR([strerror_r])dnl
dnl# from gnulib module strerror-override:
AC_REQUIRE([gl_HEADER_ERRNO_H])dnl
AC_REQUIRE([gl_FUNC_STRERROR_0])dnl
  ## set up strerror-override libobj if needed:
if test -n "${ERRNO_H}" || test ${REPLACE_STRERROR_0} = 1; then
  AC_LIBOBJ([strerror-override])dnl
  gl_PREREQ_SYS_H_WINSOCK2
fi
dnl# from gnulib module strftime-fixes:
AC_REQUIRE([gl_FUNC_STRFTIME])dnl
  ## set up strftime-fixes libobj if needed:
if test ${REPLACE_STRFTIME} = 1; then
  AC_LIBOBJ([strftime-fixes])dnl
  ## end strftime-fixes libobj
fi
gl_TIME_MODULE_INDICATOR([strftime])dnl
dnl# from gnulib module string:
m4_ifdef([gl_HEADER_STRING_H],[
  AC_REQUIRE([gl_HEADER_STRING_H])dnl
],[
  m4_ifdef([gl_STRING_H],[
    AC_REQUIRE([gl_STRING_H])dnl
  ],[
    AC_CHECK_HEADERS_ONCE([string.h])dnl
  ])dnl
])dnl
dnl# from gnulib module strncat:
AC_REQUIRE([gl_FUNC_STRNCAT])dnl
  ## set up strncat libobj if needed:
if test ${REPLACE_STRNCAT} = 1; then
  AC_LIBOBJ([strncat])dnl
  gl_PREREQ_STRNCAT
fi
gl_STRING_MODULE_INDICATOR([strncat])dnl
dnl# from gnulib module strndup:
AC_REQUIRE([gl_FUNC_STRNDUP])dnl
  ## set up strndup libobj if needed:
if test ${HAVE_STRNDUP} = 0 || test ${REPLACE_STRNDUP} = 1; then
  AC_LIBOBJ([strndup])dnl
  ## end strndup libobj
fi
gl_STRING_MODULE_INDICATOR([strndup])dnl
dnl# from gnulib module strnlen:
AC_REQUIRE([gl_FUNC_STRNLEN])dnl
  ## set up strnlen libobj if needed:
if test ${HAVE_DECL_STRNLEN} = 0 || test ${REPLACE_STRNLEN} = 1; then
  AC_LIBOBJ([strnlen])dnl
  gl_PREREQ_STRNLEN
fi
gl_STRING_MODULE_INDICATOR([strnlen])dnl
dnl# from gnulib module strpbrk:
m4_ifdef([gl_FUNC_STRPBRK],[
  AC_REQUIRE([gl_FUNC_STRPBRK])dnl
  ## set up strpbrk libobj if needed:
  if test ${HAVE_STRPBRK} = 0; then
    AC_LIBOBJ([strpbrk])dnl
    gl_PREREQ_STRPBRK
  fi
  gl_STRING_MODULE_INDICATOR([strpbrk])dnl
])dnl
dnl# from gnulib module strstr:
AC_REQUIRE([gl_FUNC_STRSTR])dnl
  ## set up strstr libobj if needed:
if test ${REPLACE_STRSTR} = 1; then
  AC_LIBOBJ([strstr])dnl
  ## end strstr libobj
fi
dnl# from gnulib module strstr-simple:
AC_REQUIRE([gl_FUNC_STRSTR_SIMPLE])dnl
dnl# (libobj is already set above)
gl_STRING_MODULE_INDICATOR([strstr])dnl
dnl# from gnulib module strtok_r:
AC_REQUIRE([gl_FUNC_STRTOK_R])dnl
  ## set up strtok_r libobj if needed:
if test ${HAVE_STRTOK_R} = 0 || test ${REPLACE_STRTOK_R} = 1; then
  AC_LIBOBJ([strtok_r])dnl
  gl_PREREQ_STRTOK_R
fi
gl_STRING_MODULE_INDICATOR([strtok_r])dnl
dnl# from gnulib module sys_select:
m4_ifdef([gl_HEADER_SYS_SELECT],[
  AC_REQUIRE([gl_HEADER_SYS_SELECT])dnl
],[
  m4_ifdef([gl_SYS_SELECT_H],[
    AC_REQUIRE([gl_SYS_SELECT_H])dnl
  ],[
    AC_CHECK_HEADERS_ONCE([sys/select.h])dnl
  ])dnl
])dnl
AC_REQUIRE([AC_PROG_MKDIR_P])dnl
dnl# from gnulib module sys_stat:
m4_ifdef([gl_HEADER_SYS_STAT_H],[
  AC_REQUIRE([gl_HEADER_SYS_STAT_H])dnl
],[
  m4_ifdef([gl_SYS_STAT_H],[
    AC_REQUIRE([gl_SYS_STAT_H])dnl
  ],[
    AC_CHECK_HEADERS_ONCE([sys/stat.h])dnl
  ])dnl
])dnl
dnl# from gnulib module sys_time:
m4_ifdef([gl_HEADER_SYS_TIME_H],[
  AC_REQUIRE([gl_HEADER_SYS_TIME_H])dnl
],[
  m4_ifdef([gl_SYS_TIME_H],[
    AC_REQUIRE([gl_SYS_TIME_H])dnl
  ],[
    AC_CHECK_HEADERS_ONCE([sys/time.h])dnl
  ])dnl
])dnl
dnl# from gnulib module sys_types:
AC_REQUIRE([gl_SYS_TYPES_H])dnl
dnl# from gnulib module sys_wait:
AC_REQUIRE([gl_SYS_WAIT_H])dnl
dnl# from gnulib module time:
m4_ifdef([gl_HEADER_TIME_H],[
  AC_REQUIRE([gl_HEADER_TIME_H])dnl
],[
  m4_ifdef([gl_FUNC_TIME],[
    AC_REQUIRE([gl_FUNC_TIME])dnl
  ],[
    AC_CHECK_HEADERS_ONCE([time.h])dnl
    AC_CHECK_FUNCS_ONCE([time])dnl
  ])dnl
])dnl
dnl# from gnulib module time_r:
AC_REQUIRE([gl_TIME_R])dnl
  ## set up time_r libobj if needed:
if test ${HAVE_LOCALTIME_R} = 0 || test ${REPLACE_LOCALTIME_R} = 1; then
  AC_LIBOBJ([time_r])dnl
  gl_PREREQ_TIME_R
fi
gl_TIME_MODULE_INDICATOR([time_r])dnl
dnl# from gnulib module time_rz:
AC_REQUIRE([gl_TIME_RZ])dnl
  ## set up time_rz libobj if needed:
if test "${HAVE_TIMEZONE_T}" = 0; then
  AC_LIBOBJ([time_rz])dnl
  ## end time_rz libobj
fi
gl_TIME_MODULE_INDICATOR([time_rz])dnl
dnl# from gnulib module timegm:
AC_REQUIRE([gl_FUNC_TIMEGM])dnl
  ## set up timegm libobj if needed:
if test ${HAVE_TIMEGM} = 0 || test ${REPLACE_TIMEGM} = 1; then
  AC_LIBOBJ([timegm])dnl
  gl_PREREQ_TIMEGM
fi
gl_TIME_MODULE_INDICATOR([timegm])dnl
dnl# from gnulib module tzset:
AC_REQUIRE([gl_FUNC_TZSET])dnl
  ## set up tzset libobj if needed:
if test "x${HAVE_TZSET}" != "x" && test "x${REPLACE_TZSET}" = "x"; then
  test -n "${HAVE_TZSET}" && test -n "${REPLACE_TZSET}"
  if test ${HAVE_TZSET} = 0 || test ${REPLACE_TZSET} = 1; then
    AC_LIBOBJ([tzset])dnl
    ## end tzset libobj
  fi
fi
gl_TIME_MODULE_INDICATOR([tzset])dnl
dnl# from gnulib module unistd:
AC_REQUIRE([gl_UNISTD_H])dnl
dnl# from gnulib module unistd-safer:
AC_REQUIRE([gl_UNISTD_SAFER])dnl
dnl# from gnulib module unsetenv:
AC_REQUIRE([gl_FUNC_UNSETENV])dnl
  ## set up unsetenv libobj if needed:
if test ${HAVE_UNSETENV} = 0 || test ${REPLACE_UNSETENV} = 1; then
  AC_LIBOBJ([unsetenv])dnl
  gl_PREREQ_UNSETENV
fi
gl_STDLIB_MODULE_INDICATOR([unsetenv])dnl
dnl# from gnulib module usleep:
AC_REQUIRE([gl_FUNC_USLEEP])dnl
  ## set up usleep libobj if needed:
if test ${HAVE_USLEEP} = 0 || test ${REPLACE_USLEEP} = 1; then
  AC_LIBOBJ([usleep])dnl
  ## end usleep libobj
fi
gl_UNISTD_MODULE_INDICATOR([usleep])dnl
dnl# from gnulib module wchar:
AC_REQUIRE([gl_WCHAR_H])dnl
AC_REQUIRE([AC_FUNC_MBRTOWC])dnl
AC_REQUIRE([AC_TYPE_MBSTATE_T])dnl
dnl# from gnulib module wcsncasecmp:
AC_REQUIRE([gl_FUNC_WCSNCASECMP])dnl
  ## set up wcsncasecmp libobj if needed:
if test ${HAVE_WCSNCASECMP} = 0; then
  AC_LIBOBJ([wcsncasecmp])dnl
  ## end wcsncasecmp libobj
fi
gl_WCHAR_MODULE_INDICATOR([wcsncasecmp])dnl
dnl# from gnulib module wctype-h:
AC_REQUIRE([gl_WCTYPE_H])dnl
AC_REQUIRE([gl_WCTYPE_H_DEFAULTS])dnl
AC_REQUIRE([gl_FUNC_TOWCTRANS])dnl
AC_REQUIRE([gl_FUNC_WCTRANS])dnl
AC_REQUIRE([gl_FUNC_WCTYPE])dnl
dnl# from gnulib module winsz-ioctl:
AC_REQUIRE([gl_HEADER_TIOCGWINSZ_NEEDS_SYS_IOCTL])dnl
dnl# from gnulib module winsz-termios:
AC_REQUIRE([gl_HEADER_TIOCGWINSZ_IN_TERMIOS_H])dnl
AC_REQUIRE([gl_WINSIZE_IN_PTEM])dnl
dnl# from gnulib module xalloc:
AC_REQUIRE([gl_XALLOC])dnl
dnl# from gnulib module xmemdup0:
AC_LIBOBJ([xmemdup0])dnl
dnl# from gnulib module xstrndup:
AC_REQUIRE([gl_XSTRNDUP])dnl
dnl# other:
AC_REQUIRE([gl_INCLUDE_NEXT])dnl
  ## need this variable:
if test "x${gl_LIBOBJS}" = "x"; then
  if test "x${LIBOBJS}" != "x"; then
    test -z "${gl_LIBOBJS}" && test -n "${LIBOBJS}" && export gl_LIBOBJS="${LIBOBJS}"
  fi
fi
AC_SUBST([gl_LIBOBJS])dnl
])dnl
