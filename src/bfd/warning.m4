dnl# Common configure.ac fragment

AC_DEFUN([AM_BINUTILS_WARNINGS],[
WARN_CFLAGS="-W -Wall -Wstrict-prototypes -Wmissing-prototypes -Wimplicit -Wparentheses -Wextra"

case "${host}" in
  *-apple-darwin* | *-apple-macos*)
    # -Wmost is APPLE_LOCAL
    WARN_CFLAGS="${WARN_CFLAGS} -Wmost"
  ;;
esac

# GCC supports -Wuninitialized only with -O or -On, n != 0.
if test x${CFLAGS+set} = xset; then
  case "${CFLAGS}" in
    *"-O0"* ) ;;
    *"-O"* )
      WARN_CFLAGS="${WARN_CFLAGS} -Wuninitialized"
    ;;
  esac
else
  WARN_CFLAGS="${WARN_CFLAGS} -Wuninitialized"
fi

AC_ARG_ENABLE([werror],
  [AS_HELP_STRING([--enable-werror],[treat compile warnings as errors])],
  [case "${enableval}" in
     yes | y) ERROR_ON_WARNING="yes" ;;
     no | n)  ERROR_ON_WARNING="no" ;;
     *) AC_MSG_ERROR([bad value ${enableval} for --enable-werror]) ;;
   esac])

AC_ARG_ENABLE([pedantic],
  [AS_HELP_STRING([--enable-pedantic],[enable pedantic warnings])],
  [case "${enableval}" in
     yes | y) PEDANTIC_WARNINGS="yes" ;;
     no | n)  PEDANTIC_WARNINGS="no" ;;
     *) AC_MSG_ERROR([bad value ${enableval} for --enable-werror]) ;;
   esac])dnl

AC_REQUIRE([AC_PROG_CC])
# Enable -Werror by default when using gcc
if test "${GCC}" = yes -a -z "${ERROR_ON_WARNING}" ; then
    ERROR_ON_WARNING=yes
fi

NO_WERROR=""
if test "x${ERROR_ON_WARNING}" = "xyes"; then
    WARN_CFLAGS="${WARN_CFLAGS} -Werror"
    NO_WERROR="-Wno-error"
fi

if test "x${PEDANTIC_WARNINGS}" = "xyes" ; then
    WARN_CFLAGS="${WARN_CFLAGS} -pedantic"
fi
		   
AC_ARG_ENABLE([build-warnings],
[AS_HELP_STRING([--enable-build-warnings],[Enable build-time compiler warnings])],
[case "${enableval}" in
  yes)	;;
  no)	WARN_CFLAGS="-w";;
  ,*)   t=`echo "${enableval}" | sed -e "s/,/ /g"`
        WARN_CFLAGS="${WARN_CFLAGS} ${t}";;
  *,)   t=`echo "${enableval}" | sed -e "s/,/ /g"`
        WARN_CFLAGS="${t} ${WARN_CFLAGS}";;
  *)    WARN_CFLAGS=`echo "${enableval}" | sed -e "s/,/ /g"`;;
esac])

if test "x${WARN_CFLAGS}" != "x" -a "x${GCC}" = "xyes"
then
    AC_MSG_CHECKING([compiler warning flags])
    for w in ${WARN_CFLAGS}; do
	case ${w} in
	*) # Check that GCC accepts it
	    saved_CFLAGS="${CFLAGS}"
	    CFLAGS="${CFLAGS} ${w}"
	    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[]],[[]])],[MY_WARN_CFLAGS="${MY_WARN_CFLAGS} ${w}"],[])
	    CFLAGS="${saved_CFLAGS}"
	esac
    done
    # Have to use second variable to avoid overwriting and/or duplicates:
    WARN_CFLAGS="${MY_WARN_CFLAGS}"
    AC_MSG_RESULT([${WARN_CFLAGS}])
fi

if test x"${silent}" != x"yes" && test x"${WARN_CFLAGS}" != x""; then
  echo "Setting warning flags = ${WARN_CFLAGS}" 6>&1
fi

AC_SUBST([WARN_CFLAGS])
AC_SUBST([NO_WERROR])
])
