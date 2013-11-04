# copied from ../../../../expect/acinclude.m4
# (I hope that is the right number of directories to go up)

AC_DEFUN([CY_AC_PATH_TCLH],[
#
# Ok, let us find the tcl source trees so we can use the headers
# Warning: transition of version 9 to 10 will break this algorithm
# because 10 sorts before 9. We also look for just tcl. We have to
# be careful that we do NOT match stuff like tclX by accident.
# the alternative search directory is involked by --with-tclinclude
#
no_tcl=true
AC_MSG_CHECKING([for Tcl private headers])
AC_MSG_RESULT([ ])
AC_ARG_WITH([tclinclude],[AS_HELP_STRING([--with-tclinclude],[directory where tcl private headers are])],[with_tclinclude=${withval}])
AC_CACHE_VAL([ac_cv_c_tclh],[
# first check to see if --with-tclinclude was specified
if test x"${with_tclinclude}" != x ; then
  if test -f ${with_tclinclude}/tclInt.h ; then
    ac_cv_c_tclh=`(cd ${with_tclinclude}; pwd)`
  elif test -f ${with_tclinclude}/generic/tclInt.h ; then
    ac_cv_c_tclh=`(cd ${with_tclinclude}/generic; pwd)`
  else
    AC_MSG_ERROR([${with_tclinclude} directory does NOT contain private headers])
  fi
fi

# next check if it came with Tcl configuration file
if test x"${ac_cv_c_tclconfig}" != x ; then
  if test -f $ac_cv_c_tclconfig/../generic/tclInt.h ; then
    ac_cv_c_tclh=`(cd $ac_cv_c_tclconfig/../generic; pwd)`
  fi
fi

# next check in private source directory
#
# since ls returns lowest version numbers first, reverse its output
changequote(,)
if test x"${ac_cv_c_tclh}" = x ; then
  for i in \
		${srcdir}/../tcl \
		`ls -dr ${srcdir}/../tcl[7-9].[0-9] 2>/dev/null` \
		${srcdir}/../../tcl \
		`ls -dr ${srcdir}/../../tcl[7-9].[0-9] 2>/dev/null` \
		${srcdir}/../../../tcl \
		`ls -dr ${srcdir}/../../../tcl[7-9].[0-9] 2>/dev/null ` \
		${srcdir}/../../../../tcl \
		`ls -dr ${srcdir}/../../../../tcl[7-9].[0-9] 2>/dev/null ` ; do
    if test -f $i/generic/tclInt.h ; then
      ac_cv_c_tclh=`(cd $i/generic; pwd)`
      break
    fi
  done
fi
changequote([,])
# finally check in a few common install locations
#
# since ls returns lowest version numbers first, reverse its output
changequote(,)
if test x"${ac_cv_c_tclh}" = x ; then
  for i in \
		`ls -dr /usr/local/src/tcl[7-9].[0-9] 2>/dev/null` \
		`ls -dr /usr/local/lib/tcl[7-9].[0-9] 2>/dev/null` \
		/usr/local/src/tcl \
		/usr/local/lib/tcl \
		${prefix}/include ; do
    if test -f $i/generic/tclInt.h ; then
      ac_cv_c_tclh=`(cd $i/generic; pwd)`
      break
    fi
  done
fi
changequote([,])
dnl# see if one is installed
if test x"${ac_cv_c_tclh}" = x ; then
   AC_CHECK_HEADERS([tclInt.h],[ac_cv_c_tclh=installed],[ac_cv_c_tclh=""])
fi
])
if test x"${ac_cv_c_tclh}" = x ; then
  TCLHDIR="# no Tcl private headers found"
  TCLHDIRDASHI="# no Tcl private headers found"
  AC_MSG_ERROR([Cannot find Tcl private headers])
fi
if test x"${ac_cv_c_tclh}" != x ; then
  no_tcl=""
  if test x"${ac_cv_c_tclh}" = x"installed" ; then
    AC_MSG_RESULT([Tcl private headers are installed])
    TCLHDIR=""
    TCLHDIRDASHI=""
    TCL_LIBRARY=""
  else
    AC_MSG_RESULT([found in ${ac_cv_c_tclh}])
    # this hack is because the TCLHDIR will not print if there is a "-I"
    # in it.
    TCLHDIR="${ac_cv_c_tclh}"
    TCLHDIRDASHI="-I${ac_cv_c_tclh}"
    TCL_LIBRARY=`echo $TCLHDIR | sed -e 's/generic//'`library
  fi
fi

AC_SUBST([TCLHDIR])
AC_SUBST([TCLHDIRDASHI])
AC_SUBST([TCL_LIBRARY])
])

