dnl# Common configure.ac fragment

AC_DEFUN([AM_BINUTILS_WARNINGS],[
# "-W" and "-Wextra" are redundant.
WARN_CFLAGS="-Wall -Wstrict-prototypes -Wmissing-prototypes \
-Wmissing-declarations -Wimplicit -Wparentheses -Wextra -Wc++-compat \
-Wundef -Wold-style-declaration -Wold-style-definition -Wnested-externs \
-Wmissing-parameter-type -Wshadow -Wabi -Wmissing-include-dirs \
-Wconversion -Wfloat-conversion -Wsign-conversion -Wsign-compare \
-Wshorten-64-to-32 -Wshadow"
# "-Wconversion" and friends are because of a comment in libbfd.c
WARN_DEFS="-D_FORTIFY_SOURCE=2 -Dlint"
WARN_LDFLAGS=""

AC_REQUIRE([AC_CANONICAL_HOST])dnl

if test "x${acl_cv_wl}" = "x"; then
  test -z "${acl_cv_wl}" && export acl_cv_wl='-Wl,'
fi
AC_SUBST([acl_cv_wl])dnl

case "${host}" in
  *-apple-darwin* | *-apple-macos*)
    # -Wmost is APPLE_LOCAL, as are -Wextra-tokens and -Wnewline-eof:
    WARN_CFLAGS="${WARN_CFLAGS} -Wmost -Wextra-tokens -Wnewline-eof"
    WARN_LDFLAGS="${WARN_LDFLAGS} ${acl_cv_wl}-warn_compact_unwind"
    WARN_LDFLAGS="${WARN_LDFLAGS} ${acl_cv_wl}-warn_weak_exports"
    WARN_LDFLAGS="${WARN_LDFLAGS} ${acl_cv_wl}-warn_stabs"
    WARN_LDFLAGS="${WARN_LDFLAGS} ${acl_cv_wl}-warn_commons"
  ;;
esac

# GCC supports -Wuninitialized only with -O or -On, n != 0.
# and also -Winit-self only with -Wuninitialized
# others in this section are technically supported at other optimization
# levels, but really only make sense with it on.
opt_warnings="-Wuninitialized -Winit-self -Wvolatile-register-var \
-Wdisabled-optimization -Waggressive-loop-optimizations \
-Wvector-operation-performance -Wmaybe-uninitialized \
-Wunsafe-loop-optimizations"
if test x${CFLAGS+set} = xset; then
  case "${CFLAGS}" in
    *"-O0"* ) ;;
    *"-O"* )
      WARN_CFLAGS="${WARN_CFLAGS} ${opt_warnings}"
    ;;
  esac
else
  WARN_CFLAGS="${WARN_CFLAGS} ${opt_warnings}"
fi

AC_ARG_ENABLE([werror],
  [AS_HELP_STRING([--enable-werror],[treat compile warnings as errors])],
  [case "${enableval}" in
     yes | y) ERROR_ON_WARNING="yes" ;;
     no | n)  ERROR_ON_WARNING="no" ;;
     *) AC_MSG_ERROR([bad value ${enableval} for --enable-werror]) ;;
   esac])dnl

AC_ARG_ENABLE([pedantic],
  [AS_HELP_STRING([--enable-pedantic],[enable pedantic warnings])],
  [case "${enableval}" in
     yes | y) PEDANTIC_WARNINGS="yes" ;;
     no | n)  PEDANTIC_WARNINGS="no" ;;
     *) AC_MSG_ERROR([bad value ${enableval} for --enable-pedantic]) ;;
   esac])dnl

AC_REQUIRE([AC_PROG_CC])dnl
# Enable -Werror by default when using gcc:
if test "${GCC}" = yes -a -z "${ERROR_ON_WARNING}"; then
    ERROR_ON_WARNING=yes
fi

NO_WERROR=""
if test "x${ERROR_ON_WARNING}" = "xyes"; then
    WARN_CFLAGS="${WARN_CFLAGS} -Werror"
    NO_WERROR="-Wno-error"
fi

if test "x${PEDANTIC_WARNINGS}" = "xyes"; then
    WARN_CFLAGS="${WARN_CFLAGS} -pedantic -Wdeclaration-after-statement"
    ## fake the '-ansi' flag; actually using it has additional effects:
    WARN_CFLAGS="${WARN_CFLAGS} -D__STRICT_ANSI__"
    WARN_DEFS="${WARN_DEFS} -D__STRICT_ANSI__"
fi
		   
AC_ARG_ENABLE([build-warnings],
[AS_HELP_STRING([--enable-build-warnings],
                [Enable build-time compiler warnings])],
[case "${enableval}" in
  yes)	;;
  no)	WARN_CFLAGS="-w";;
  ,*)   t=`echo "${enableval}" | sed -e "s/,/ /g"`
        WARN_CFLAGS="${WARN_CFLAGS} ${t}";;
  *,)   t=`echo "${enableval}" | sed -e "s/,/ /g"`
        WARN_CFLAGS="${t} ${WARN_CFLAGS}";;
  *)    WARN_CFLAGS=`echo "${enableval}" | sed -e "s/,/ /g"`;;
esac])dnl

if test "x${WARN_DEFS}" != "x" -a "x${GCC}" = "xyes"
then
    AC_MSG_CHECKING([preprocessor definitons to use when warning])
    for w in ${WARN_DEFS}; do
	case ${w} in
	*) # Check its use with the preprocessor.
	    saved_DEFS="${DEFS}"
	    DEFS="${DEFS} ${w}"
            # The above fails to actually get it into the compile command,
            # so do this as well:
	    saved_CFLAGS="${CFLAGS}"
	    CFLAGS="${CFLAGS} -Wp,${w}"
            # Actually that fails as well, so one more try:
            saved_CPPFLAGS="${CPPFLAGS}"
	    CPPFLAGS="${CPPFLAGS} ${w}"
	    AC_PREPROC_IFELSE([AC_LANG_PROGRAM([[]],[[]])],
                              [MY_WARN_DEFS="${MY_WARN_DEFS} ${w}"],[])
	    DEFS="${saved_DEFS}"
	    CFLAGS="${saved_CFLAGS}"
	    CPPFLAGS="${saved_CPPFLAGS}"
	esac
    done
    # Have to use second variable to avoid overwriting and/or duplicates:
    WARN_DEFS="${MY_WARN_DEFS}"
    AC_MSG_RESULT([${WARN_DEFS}])
fi

BAD_WARN_CFLAGS=""
if test "x${WARN_CFLAGS}" != "x" -a "x${GCC}" = "xyes"
then
    AC_MSG_CHECKING([compiler warning flags to use])
    for w in ${WARN_CFLAGS}; do
	case ${w} in
	*) # Check that GCC accepts it:
	    saved_CFLAGS="${CFLAGS}"
	    CFLAGS="${CFLAGS} ${w}"
	    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[]],[[]])],
                              [MY_WARN_CFLAGS="${MY_WARN_CFLAGS} ${w}"],
                              [BAD_WARN_CFLAGS="${BAD_WARN_CFLAGS} ${w}"])
	    CFLAGS="${saved_CFLAGS}"
	esac
    done
    # Have to use second variable to avoid overwriting and/or duplicates:
    WARN_CFLAGS="${MY_WARN_CFLAGS}"
    AC_MSG_RESULT([using: ${WARN_CFLAGS}])
    if test "x${BAD_WARN_CFLAGS}" != "x"; then
      test -n "${BAD_WARN_CFLAGS}"
      AC_MSG_WARN([compiler failed to accept: ${BAD_WARN_CFLAGS}])
    fi
fi

if test "x${WARN_LDFLAGS}" != "x" -a "x${GCC}" = "xyes"
then
    AC_MSG_CHECKING([linker warning flags to use])
    for w in ${WARN_LDFLAGS}; do
	case ${w} in
	*) # Check that the compiler successfully passes it to the linker.
	    saved_LDFLAGS="${LDFLAGS}"
	    LDFLAGS="${LDFLAGS} ${w}"
	    AC_LINK_IFELSE([AC_LANG_PROGRAM([[]],[[]])],
                           [MY_WARN_LDFLAGS="${MY_WARN_LDFLAGS} ${w}"],[])
	    LDFLAGS="${saved_LDFLAGS}"
	esac
    done
    # Have to use second variable to avoid overwriting and/or duplicates:
    WARN_LDFLAGS="${MY_WARN_LDFLAGS}"
    AC_MSG_RESULT([${WARN_LDFLAGS}])
fi

if test x"${silent}" != x"yes" && test x"${WARN_CFLAGS}" != x""; then
  echo "Setting C compiler warning flags = ${WARN_CFLAGS}" 6>&1
fi
if test x"${silent}" != x"yes" && test x"${WARN_LDFLAGS}" != x""; then
  echo "Setting linker warning flags = ${WARN_LDFLAGS}" 6>&1
fi

AC_SUBST([WARN_CFLAGS])dnl
AC_SUBST([WARN_DEFS])dnl
AC_SUBST([WARN_LDFLAGS])dnl
AC_SUBST([NO_WERROR])dnl
])dnl

AC_DEFUN([AM_BINUTILS_CLANG_STATIC_ANALYSIS],[
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
])dnl
