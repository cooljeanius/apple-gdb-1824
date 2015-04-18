dnl# Common configure.ac fragment

AC_DEFUN([AM_BINUTILS_WARNINGS],[
# "-W" and "-Wextra" are redundant.
WARN_CFLAGS="-Wall -Wstrict-prototypes -Wmissing-prototypes \
-Wmissing-declarations -Wimplicit -Wparentheses -Wextra -Wc++-compat \
-Wundef -Wold-style-declaration -Wold-style-definition -Wnested-externs \
-Wmissing-parameter-type -Wshadow -Wabi -Wmissing-include-dirs \
-Wconversion -Wfloat-conversion -Wsign-conversion -Wsign-compare \
-Wshorten-64-to-32 -Wshadow -Wdouble-promotion"
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
# ...and also -Winit-self only works when -Wuninitialized is also used.
# -Wmissing-noreturn requires -O or higher, or -fipa-pure-const to work.
# Others in this section are technically supported at other optimization
# levels, but really only make sense with it on.
opt_warnings="-Wuninitialized -Winit-self -Wmissing-noreturn \
-Wvolatile-register-var -Wdisabled-optimization \
-Waggressive-loop-optimizations -Wvector-operation-performance \
-Wmaybe-uninitialized -Wunsafe-loop-optimizations"
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
    ## avoid warnings about "long long" when we are using it:
    if test "x${want64}" = "xtrue" && test "x${bfd_cv_has_long_long}" = "xyes" && test "x${BFD_HOST_64BIT_LONG_LONG}" = "x1"; then
      ## add something else to make up for the removed warnings:
      WARN_CFLAGS="${WARN_CFLAGS} -Wno-long-long -Wpointer-arith"
    fi
    ## fake the '-ansi' flag; actually using it has additional effects:
    WARN_CFLAGS="${WARN_CFLAGS} -D__STRICT_ANSI__"
    WARN_DEFS="${WARN_DEFS} -D__STRICT_ANSI__"
fi

AC_REQUIRE([gl_UNKNOWN_WARNINGS_ARE_ERRORS])dnl
		   
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
	    CFLAGS="${CFLAGS} ${w} ${gl_unknown_warnings_are_errors}"
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

dnl# copied from the gnulib warnings.m4:

# gl_AS_VAR_APPEND(VAR, VALUE)
# ----------------------------
# Provide the functionality of AS_VAR_APPEND if Autoconf does not have it.
m4_ifdef([AS_VAR_APPEND],
[m4_copy([AS_VAR_APPEND], [gl_AS_VAR_APPEND])],
[m4_define([gl_AS_VAR_APPEND],
[AS_VAR_SET([$1], [AS_VAR_GET([$1])$2])])])


# gl_COMPILER_OPTION_IF(OPTION, [IF-SUPPORTED], [IF-NOT-SUPPORTED],
#                       [PROGRAM = AC_LANG_PROGRAM()])
# -----------------------------------------------------------------
# Check if the compiler supports OPTION when compiling PROGRAM.
#
# FIXME: gl_Warn must be used unquoted until we can assume Autoconf
# 2.64 or newer.
AC_DEFUN([gl_COMPILER_OPTION_IF],
[AS_VAR_PUSHDEF([gl_Warn], [gl_cv_warn_[]_AC_LANG_ABBREV[]_$1])dnl
AS_VAR_PUSHDEF([gl_Flags], [_AC_LANG_PREFIX[]FLAGS])dnl
AS_LITERAL_IF([$1],
  [m4_pushdef([gl_Positive], m4_bpatsubst([$1], [^-Wno-], [-W]))],
  [gl_positive="$1"
case $gl_positive in
  -Wno-*) gl_positive=-W`expr "X$gl_positive" : 'X-Wno-\(.*\)'` ;;
esac
m4_pushdef([gl_Positive], [$gl_positive])])dnl
AC_CACHE_CHECK([whether _AC_LANG compiler handles $1], m4_defn([gl_Warn]), [
  gl_save_compiler_FLAGS="$gl_Flags"
  gl_AS_VAR_APPEND(m4_defn([gl_Flags]),
    [" $gl_unknown_warnings_are_errors ]m4_defn([gl_Positive])["])
  AC_LINK_IFELSE([m4_default([$4], [AC_LANG_PROGRAM([])])],
                 [AS_VAR_SET(gl_Warn, [yes])],
                 [AS_VAR_SET(gl_Warn, [no])])
  gl_Flags="$gl_save_compiler_FLAGS"
])
AS_VAR_IF(gl_Warn, [yes], [$2], [$3])
m4_popdef([gl_Positive])dnl
AS_VAR_POPDEF([gl_Flags])dnl
AS_VAR_POPDEF([gl_Warn])dnl
])

# gl_UNKNOWN_WARNINGS_ARE_ERRORS
# ------------------------------
# Clang doesn't complain about unknown warning options unless one also
# specifies -Wunknown-warning-option -Werror.  Detect this.
AC_DEFUN([gl_UNKNOWN_WARNINGS_ARE_ERRORS],
[gl_COMPILER_OPTION_IF([-Werror -Wunknown-warning-option],
   [gl_unknown_warnings_are_errors='-Wunknown-warning-option -Werror'],
   [gl_unknown_warnings_are_errors=])])

# gl_WARN_ADD(OPTION, [VARIABLE = WARN_CFLAGS],
#             [PROGRAM = AC_LANG_PROGRAM()])
# ---------------------------------------------
# Adds parameter to WARN_CFLAGS if the compiler supports it when
# compiling PROGRAM.  For example, gl_WARN_ADD([-Wparentheses]).
#
# If VARIABLE is a variable name, AC_SUBST it.
AC_DEFUN([gl_WARN_ADD],
[AC_REQUIRE([gl_UNKNOWN_WARNINGS_ARE_ERRORS])
gl_COMPILER_OPTION_IF([$1],
  [gl_AS_VAR_APPEND(m4_if([$2], [], [[WARN_CFLAGS]], [[$2]]), [" $1"])],
  [],
  [$3])
m4_ifval([$2],
         [AS_LITERAL_IF([$2], [AC_SUBST([$2])])],
         [AC_SUBST([WARN_CFLAGS])])dnl
])

# Local Variables:
# mode: autoconf
# End:
