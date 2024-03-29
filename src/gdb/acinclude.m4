dnl# gdb/acinclude.m4                                      -*- Autoconf -*-
dnl# written by Rob Savoye <rob@cygnus.com> for Cygnus Support
dnl# major rewriting for Tcl 7.5 by Don Libes <libes@nist.gov>

sinclude(acx_configure_dir.m4)

dnl# gdb/configure.ac uses BFD_NEED_DECLARATION, so get its definition.
sinclude(../bfd/bfd.m4)

dnl# This gets the standard macros, like the TCL, TK, etc. ones:
sinclude(../config/acinclude.m4)

dnl# In case any were left out from the previous, such as the ACX_* ones:
sinclude(../config/acx.m4)

dnl# for zlib macro:
sinclude(../config/zlib.m4)

dnl# For libiberty_INIT.
m4_include(../libiberty/m4/libiberty.m4)

dnl# For --enable-build-with-cxx and COMPILER.
m4_include(m4/build-with-cxx.m4)

dnl# For AM_LANGINFO_CODESET.
m4_include(m4/codeset.m4)

dnl# gettext stuff:
sinclude(../lcmessage.m4)
sinclude(../gettext.m4)
dnl# The lines below arrange for aclocal not to bring gettext.m4's
dnl# CY_GNU_GETTEXT into aclocal.m4.
ifelse([yes],[no],[
AC_DEFUN([CY_GNU_GETTEXT],[echo "The Cygnus gettext macro was called."])dnl
])dnl

dnl# CYGNUS LOCAL: This gets the right posix flag for gcc:
AC_DEFUN([CY_AC_TCL_LYNX_POSIX],[
AC_REQUIRE([AC_PROG_CC])dnl
AC_REQUIRE([AC_PROG_CPP])dnl
AC_REQUIRE([AC_PROG_EGREP])dnl
# Do the actual checking for LynxOS now:
AC_MSG_CHECKING([if running LynxOS])
AC_CACHE_VAL([ac_cv_os_lynx],
[AC_EGREP_CPP([yes_this_is_lynx],
[/*
 * The old Lynx "cc" only defines "Lynx", but the newer one uses "__Lynx__"
 */
#if defined(__Lynx__) || defined(Lynx)
yes_this_is_lynx
#endif /* __Lynx__ || Lynx */
],[ac_cv_os_lynx=yes],[ac_cv_os_lynx=no])])
#
if test "x${ac_cv_os_lynx}" = "xyes"; then
  AC_MSG_RESULT([yes])
  AC_DEFINE([LYNX],[1],[Define to 1 for LynxOS])
  AC_MSG_CHECKING([whether -mposix or -X is available])
  AC_CACHE_VAL([ac_cv_c_posix_flag],
  [AC_COMPILE_IFELSE([AC_LANG_SOURCE([[]],[[
  /*
   * This flag varies depending on how old the compiler is.
   * -X is for the old "cc" and "gcc" (based on 1.42).
   * -mposix is for the new gcc (at least 2.5.8).
   */
  #if defined(__GNUC__) && (__GNUC__ >= 2)
  choke me
  #endif /* gcc 2+ */
  ]])],[ac_cv_c_posix_flag=" -mposix"],[ac_cv_c_posix_flag=" -X"])])
  CC="${CC} ${ac_cv_c_posix_flag}"
  AC_MSG_RESULT([${ac_cv_c_posix_flag}])
else
  AC_MSG_RESULT([no])
fi
])dnl

#
# Sometimes the native compiler is a bogus stub for gcc or /usr/ucb/cc.
# This makes configure think it is cross compiling. If the --target flag
# was NOT used, then we cannot configure, so something is wrong. We do NOT
# use the cache here cause if somebody fixes their compiler install, we
# want this to work.
AC_DEFUN([CY_AC_C_WORKS],[
# If we cannot compile and link a trivial program, then we cannot expect
# anything to work:
AC_MSG_CHECKING([whether the compiler (${CC}) actually works])
AC_COMPILE_IFELSE([AC_LANG_SOURCE([[]],[[/* do NOT need anything here */]])],
        [c_compiles=yes],[c_compiles=no])

AC_LINK_IFELSE([AC_LANG_SOURCE([[]],[[/* do NOT need anything here */]])],
        [c_links=yes],[c_links=no])

if test x"${c_compiles}" = x"no"; then
  # error out:
  AC_MSG_ERROR([the native compiler is broken and will NOT compile.])
fi

if test x"${c_links}" = x"no"; then
  AC_MSG_ERROR([the native compiler is broken and will NOT link.])
fi
AC_MSG_RESULT([yes])
])dnl

AC_DEFUN([CY_AC_PATH_TCLH],[
#
# Ok, let us find the tcl source trees so we can use the headers
# Warning: transition of version 9 to 10 will break this algorithm
# because 10 sorts before 9. We also look for just tcl. We have to
# be careful that we don't match stuff like tclX by accident.
# the alternative search directory is involked by --with-tclinclude
#

no_tcl=true
AC_MSG_CHECKING([for Tcl private headers. dir=${configdir}])
AC_ARG_WITH([tclinclude],
            [AS_HELP_STRING([--with-tclinclude=DIR],
                            [Directory where tcl private headers are])],
            [with_tclinclude=${withval}])
AC_CACHE_VAL([ac_cv_c_tclh],[
# first check to see if --with-tclinclude was specified:
if test x"${with_tclinclude}" != x""; then
  if test -f ${with_tclinclude}/tclInt.h; then
    ac_cv_c_tclh=`(cd ${with_tclinclude}; pwd)`
  elif test -f ${with_tclinclude}/generic/tclInt.h; then
    ac_cv_c_tclh=`(cd ${with_tclinclude}/generic; pwd)`
  else
    AC_MSG_ERROR([${with_tclinclude} directory does NOT contain private headers])
  fi
fi

# next check if it came with Tcl configuration file:
if test x"${ac_cv_c_tclconfig}" = x""; then
  if test -f ${ac_cv_c_tclconfig}/../generic/tclInt.h; then
    ac_cv_c_tclh=`(cd ${ac_cv_c_tclconfig}/..; pwd)`
  fi
fi

# next check in private source directory
#
# since ls returns lowest version numbers first, reverse its output:
if test x"${ac_cv_c_tclh}" = x""; then
  for i in \
		${srcdir}/../tcl \
		`ls -dr ${srcdir}/../tcl[[7-9]]* 2>/dev/null` \
		${srcdir}/../../tcl \
		`ls -dr ${srcdir}/../../tcl[[7-9]]* 2>/dev/null` \
		${srcdir}/../../../tcl \
		`ls -dr ${srcdir}/../../../tcl[[7-9]]* 2>/dev/null ` ; do
    if test -f ${i}/generic/tclInt.h; then
      ac_cv_c_tclh=`(cd ${i}/generic; pwd)`
      break
    fi
  done
fi
# finally check in a few common install locations
#
# since ls returns lowest version numbers first, reverse its output:
if test x"${ac_cv_c_tclh}" = x""; then
  for i in \
		`ls -dr /usr/local/src/tcl[[7-9]]* 2>/dev/null` \
		`ls -dr /usr/local/lib/tcl[[7-9]]* 2>/dev/null` \
		/usr/local/src/tcl \
		/usr/local/lib/tcl \
		${prefix}/include; do
    if test -f ${i}/generic/tclInt.h; then
      ac_cv_c_tclh=`(cd ${i}/generic; pwd)`
      break
    fi
  done
fi
# see if one is installed
if test x"${ac_cv_c_tclh}" = x""; then
   AC_CHECK_HEADER([tclInt.h],[ac_cv_c_tclh=installed],[ac_cv_c_tclh=""])
fi
])
if test x"${ac_cv_c_tclh}" = x""; then
  TCLHDIR="# no Tcl private headers found"
  AC_MSG_ERROR([Cannot find Tcl private headers])
fi
if test x"${ac_cv_c_tclh}" != x""; then
  no_tcl=""
  if test x"${ac_cv_c_tclh}" = x"installed"; then
    AC_MSG_RESULT([is installed])
    TCLHDIR=""
  else
    AC_MSG_RESULT([found in ${ac_cv_c_tclh}])
    # this hack is because the TCLHDIR will not print if there is a "-I" in
    # it.
    TCLHDIR="-idirafter ${ac_cv_c_tclh}"
  fi
fi

AC_SUBST([TCLHDIR])dnl
])dnl


AC_DEFUN([CY_AC_PATH_TCLCONFIG],[
#
# Ok, let us find the tcl configuration
# First, look for one uninstalled.  
# the alternative search directory is invoked by --with-tclconfig
#

if test x"${no_tcl}" = x""; then
  # we reset no_tcl in case something fails here
  no_tcl=true
  AC_ARG_WITH([tclconfig],
              [AS_HELP_STRING([--with-tclconfig=DIR],
                 [Directory containing tcl configuration (tclConfig.sh)])],
         [with_tclconfig=${withval}])
  AC_MSG_CHECKING([for Tcl configuration])
  AC_CACHE_VAL([ac_cv_c_tclconfig],[

  # First check to see if --with-tclconfig was specified.
  if test x"${with_tclconfig}" != x""; then
    if test -f "${with_tclconfig}/tclConfig.sh"; then
      ac_cv_c_tclconfig=`(cd ${with_tclconfig}; pwd)`
    else
      AC_MSG_ERROR([${with_tclconfig} directory does NOT contain tclConfig.sh])
    fi
  fi

  # then check for a private Tcl installation:
  if test x"${ac_cv_c_tclconfig}" = x""; then
    for i in \
		../tcl \
		`ls -dr ../tcl[[7-9]]* 2>/dev/null` \
		../../tcl \
		`ls -dr ../../tcl[[7-9]]* 2>/dev/null` \
		../../../tcl \
		`ls -dr ../../../tcl[[7-9]]* 2>/dev/null`; do
      if test -f "${i}/${configdir}/tclConfig.sh" ; then
        ac_cv_c_tclconfig=`(cd ${i}/${configdir}; pwd)`
	break
      fi
    done
  fi
  # check in a few common install locations:
  if test x"${ac_cv_c_tclconfig}" = x""; then
    for i in `ls -d ${prefix}/lib /usr/local/lib 2>/dev/null`; do
      if test -f "${i}/tclConfig.sh"; then
        ac_cv_c_tclconfig=`(cd ${i}; pwd)`
	break
      fi
    done
  fi
  # check in a few other private locations:
  if test x"${ac_cv_c_tclconfig}" = x""; then
    for i in \
		${srcdir}/../tcl \
		`ls -dr ${srcdir}/../tcl[[7-9]]* 2>/dev/null` ; do
      if test -f "${i}/${configdir}/tclConfig.sh"; then
        ac_cv_c_tclconfig=`(cd ${i}/${configdir}; pwd)`
	break
      fi
    done
  fi
  ])
  if test x"${ac_cv_c_tclconfig}" = x""; then
    TCLCONFIG="# no Tcl configs found"
    AC_MSG_WARN([Cannot find Tcl configuration definitions])
  else
    no_tcl=""
    TCLCONFIG=${ac_cv_c_tclconfig}/tclConfig.sh
    AC_MSG_RESULT([found ${TCLCONFIG}])
  fi
fi
])dnl

# Defined as a separate macro so we do not have to cache the values
# from PATH_TCLCONFIG (because this can also be cached).
AC_DEFUN([CY_AC_LOAD_TCLCONFIG],[
    AC_REQUIRE([CY_AC_PATH_TCLCONFIG])
    AC_MSG_NOTICE([sourcing ${TCLCONFIG}])
    . ${TCLCONFIG}

    AC_SUBST([TCL_VERSION])dnl
    AC_SUBST([TCL_MAJOR_VERSION])dnl
    AC_SUBST([TCL_MINOR_VERSION])dnl
    AC_SUBST([TCL_CC])dnl
    AC_SUBST([TCL_DEFS])dnl
    AC_SUBST([TCL_SHLIB_CFLAGS])dnl
    AC_SUBST([TCL_SHLIB_LD])dnl
    AC_SUBST([TCL_SHLIB_LD_LIBS])dnl
    AC_SUBST([TCL_SHLIB_SUFFIX])dnl
    AC_SUBST([TCL_DL_LIBS])dnl
    AC_SUBST([TCL_LD_FLAGS])dnl
    AC_SUBST([TCL_LD_SEARCH_FLAGS])dnl
    AC_SUBST([TCL_CC_SEARCH_FLAGS])dnl
    AC_SUBST([TCL_COMPAT_OBJS])dnl
    AC_SUBST([TCL_RANLIB])dnl
    AC_SUBST([TCL_BUILD_LIB_SPEC])dnl
    AC_SUBST([TCL_LIB_SPEC])dnl
    AC_SUBST([TCL_LIB_VERSIONS_OK])dnl
])dnl

# Warning: Tk definitions are very similar to Tcl definitions but
# are not precisely the same.  There are a couple of differences,
# so don't do changes to Tcl thinking you can cut and paste it do 
# the Tk differences and later simply substitute "Tk" for "Tcl".
# Known differences:
#  - Acceptable Tcl major version #s is 7-9 while Tk is 4-9
#  - Searching for Tcl includes looking for tclInt.h, Tk looks for tk.h
#  - Computing major/minor versions is different because Tk depends on
#    headers to Tcl, Tk, and X.
#  - Symbols in tkConfig.sh are different than tclConfig.sh
#  - Acceptable for Tk to be missing but not Tcl.

AC_DEFUN([CY_AC_PATH_TKH],[
#
# Ok, let us find the tk source trees so we can use the headers.
# If the directory (presumably symlink) named "tk" exists, use that one
# in preference to any others. Same logic is used when choosing library
# and again with Tcl. The search order is the best place to look first,
# then in decreasing significance. The loop breaks if the trigger file is
# found.
# Note the gross little conversion here of srcdir by cd'ing to the found
# directory. This converts the path from a relative to an absolute, so
# recursive cache variables for the path will work right. We check all
# the possible paths in one loop rather than many seperate loops to speed
# things up.
# The alternative search directory is involked by --with-tkinclude
#
no_tk=true
AC_MSG_CHECKING([for Tk private headers])
AC_ARG_WITH([tkinclude],
            [AS_HELP_STRING([--with-tkinclude=DIR],
                            [Directory where tk private headers are])],
            [with_tkinclude=${withval}])
AC_CACHE_VAL([ac_cv_c_tkh],[
# first check to see if --with-tkinclude was specified:
if test x"${with_tkinclude}" != x""; then
  if test -f ${with_tkinclude}/tk.h; then
    ac_cv_c_tkh=`(cd ${with_tkinclude}; pwd)`
  elif test -f ${with_tkinclude}/generic/tk.h; then
    ac_cv_c_tkh=`(cd ${with_tkinclude}/generic; pwd)`
  else
    AC_MSG_ERROR([${with_tkinclude} directory does NOT contain private headers])
  fi
fi

# next check if it came with Tk configuration file:
if test x"${ac_cv_c_tkconfig}" = x""; then
  if test -f ${ac_cv_c_tkconfig}/../generic/tk.h; then
    ac_cv_c_tkh=`(cd ${ac_cv_c_tkconfig}/..; pwd)`
  fi
fi

# next check in private source directory
#
# since ls returns lowest version numbers first, reverse its output:
if test x"${ac_cv_c_tkh}" = x""; then
  for i in \
		${srcdir}/../tk \
		`ls -dr ${srcdir}/../tk[[4-9]]* 2>/dev/null` \
		${srcdir}/../../tk \
		`ls -dr ${srcdir}/../../tk[[4-9]]* 2>/dev/null` \
		${srcdir}/../../../tk \
		`ls -dr ${srcdir}/../../../tk[[4-9]]* 2>/dev/null `; do
    if test -f ${i}/generic/tk.h ; then
      ac_cv_c_tkh=`(cd ${i}/generic; pwd)`
      break
    fi
  done
fi
# finally check in a few common install locations
#
# since ls returns lowest version numbers first, reverse its output:
if test x"${ac_cv_c_tkh}" = x""; then
  for i in \
		`ls -dr /usr/local/src/tk[[4-9]]* 2>/dev/null` \
		`ls -dr /usr/local/lib/tk[[4-9]]* 2>/dev/null` \
		/usr/local/src/tk \
		/usr/local/lib/tk \
		${prefix}/include; do
    if test -f ${i}/generic/tk.h ; then
      ac_cv_c_tkh=`(cd ${i}/generic; pwd)`
      break
    fi
  done
fi
# see if one is installed
if test x"${ac_cv_c_tkh}" = x""; then
   AC_CHECK_HEADER([tk.h],[ac_cv_c_tkh=installed],[ac_cv_c_tkh=""])
fi
])
if test x"${ac_cv_c_tkh}" != x""; then
  no_tk=""
  if test x"${ac_cv_c_tkh}" = x"installed"; then
    AC_MSG_RESULT([is installed])
    TKHDIR=""
  else
    AC_MSG_RESULT([found in ${ac_cv_c_tkh}])
    # this hack is because the TKHDIR will NOT print if there is a "-I" in
    # it.
    TKHDIR="-idirafter ${ac_cv_c_tkh}"
  fi
else
  TKHDIR="# no Tk directory found"
  AC_MSG_WARN([Cannot find Tk private headers])
  no_tk=true
fi

AC_SUBST([TKHDIR])dnl
])dnl


AC_DEFUN([CY_AC_PATH_TKCONFIG],[
#
# Ok, let us find the tk configuration
# First, look for one uninstalled.  
# the alternative search directory is invoked by --with-tkconfig
#

if test x"${no_tk}" = x""; then
  # we reset no_tk in case something fails here:
  no_tk=true
  AC_ARG_WITH([tkconfig],
              [AS_HELP_STRING([--with-tkconfig=DIR],
                   [Directory containing tk configuration (tkConfig.sh)])],
         [with_tkconfig=${withval}])
  AC_MSG_CHECKING([for Tk configuration])
  AC_CACHE_VAL([ac_cv_c_tkconfig],[

  # First check to see if --with-tkconfig was specified.
  if test x"${with_tkconfig}" != x""; then
    if test -f "${with_tkconfig}/tkConfig.sh"; then
      ac_cv_c_tkconfig=`(cd ${with_tkconfig}; pwd)`
    else
      AC_MSG_ERROR([${with_tkconfig} directory does NOT contain tkConfig.sh])
    fi
  fi

  # then check for a private Tk library:
  if test x"${ac_cv_c_tkconfig}" = x""; then
    for i in \
		../tk \
		`ls -dr ../tk[[4-9]]* 2>/dev/null` \
		../../tk \
		`ls -dr ../../tk[[4-9]]* 2>/dev/null` \
		../../../tk \
		`ls -dr ../../../tk[[4-9]]* 2>/dev/null`; do
      if test -f "${i}/${configdir}/tkConfig.sh"; then
        ac_cv_c_tkconfig=`(cd ${i}/${configdir}; pwd)`
	break
      fi
    done
  fi
  # check in a few common install locations:
  if test x"${ac_cv_c_tkconfig}" = x""; then
    for i in `ls -d ${prefix}/lib /usr/local/lib 2>/dev/null`; do
      if test -f "${i}/tkConfig.sh" ; then
        ac_cv_c_tkconfig=`(cd ${i}; pwd)`
	break
      fi
    done
  fi
  # check in a few other private locations:
  if test x"${ac_cv_c_tkconfig}" = x""; then
    for i in \
		${srcdir}/../tk \
		`ls -dr ${srcdir}/../tk[[4-9]]* 2>/dev/null`; do
      if test -f "${i}/${configdir}/tkConfig.sh"; then
        ac_cv_c_tkconfig=`(cd ${i}/${configdir}; pwd)`
	break
      fi
    done
  fi
  ])
  if test x"${ac_cv_c_tkconfig}" = x""; then
    TKCONFIG="# no Tk configs found"
    AC_MSG_WARN([Cannot find Tk configuration definitions])
  else
    no_tk=""
    TKCONFIG=${ac_cv_c_tkconfig}/tkConfig.sh
    AC_MSG_RESULT([found ${TKCONFIG}])
  fi
fi
])dnl

# Defined as a separate macro so we do NOT have to cache the values
# from PATH_TKCONFIG (because this can also be cached).
AC_DEFUN([CY_AC_LOAD_TKCONFIG],[
    AC_REQUIRE([CY_AC_PATH_TKCONFIG])
    if test -f "${TKCONFIG}"; then
      AC_MSG_NOTICE([sourcing ${TKCONFIG}])
      . ${TKCONFIG}
    fi

    AC_SUBST([TK_VERSION])dnl
    AC_SUBST([TK_DEFS])dnl
    AC_SUBST([TK_BUILD_INCLUDES])dnl
    AC_SUBST([TK_XINCLUDES])dnl
    AC_SUBST([TK_XLIBSW])dnl
    AC_SUBST([TK_BUILD_LIB_SPEC])dnl
    AC_SUBST([TK_LIB_SPEC])dnl
])dnl

# check for Itcl headers. 

AC_DEFUN([CY_AC_PATH_ITCLCONFIG],[
#
# Ok, let us find the itcl configuration
# First, look for one uninstalled.  
# the alternative search directory is invoked by --with-itclconfig
#

if test x"${no_itcl}" = x""; then
  # we reset no_itcl in case something fails here:
  no_itcl=true
  AC_ARG_WITH([itclconfig],
              [AS_HELP_STRING([--with-itclconfig],
               [Directory containing itcl configuration (itclConfig.sh)])],
         [with_itclconfig=${withval}])
  AC_MSG_CHECKING([for Itcl configuration])
  AC_CACHE_VAL([ac_cv_c_itclconfig],[

  # First check to see if --with-itclconfig was specified.
  if test x"${with_itclconfig}" != x""; then
    if test -f "${with_itclconfig}/itclConfig.sh" ; then
      ac_cv_c_itclconfig=`(cd ${with_itclconfig}; pwd)`
    else
      AC_MSG_ERROR([${with_itclconfig} directory does NOT contain itclConfig.sh])
    fi
  fi

  # then check for a private Itcl library:
  if test x"${ac_cv_c_itclconfig}" = x""; then
    for i in \
		../itcl/itcl \
		`ls -dr ../itcl[[4-9]]*/itcl 2>/dev/null` \
		../../itcl \
		`ls -dr ../../itcl[[4-9]]*/itcl 2>/dev/null` \
		../../../itcl \
		`ls -dr ../../../itcl[[4-9]]*/itcl 2>/dev/null`; do
      if test -f "$i/itclConfig.sh" ; then
        ac_cv_c_itclconfig=`(cd $i; pwd)`
	break
      fi
    done
  fi
  # check in a few common install locations:
  if test x"${ac_cv_c_itclconfig}" = x""; then
    for i in `ls -d ${prefix}/lib /usr/local/lib 2>/dev/null`; do
      if test -f "${i}/itclConfig.sh"; then
        ac_cv_c_itclconfig=`(cd ${i}; pwd)`
	break
      fi
    done
  fi
  # check in a few other private locations:
  if test x"${ac_cv_c_itclconfig}" = x""; then
    for i in \
		${srcdir}/../itcl/itcl \
		`ls -dr ${srcdir}/../itcl[[4-9]]*/itcl 2>/dev/null`; do
      if test -f "${i}/itclConfig.sh" ; then
        ac_cv_c_itclconfig=`(cd ${i}; pwd)`
	break
      fi
    done
  fi
  ])
  if test x"${ac_cv_c_itclconfig}" = x""; then
    ITCLCONFIG="# no Itcl configs found"
    AC_MSG_WARN([Cannot find Itcl configuration definitions])
  else
    no_itcl=""
    ITCLCONFIG=${ac_cv_c_itclconfig}/itclConfig.sh
    AC_MSG_RESULT([found ${ITCLCONFIG}])
  fi
fi
])dnl

# Defined as a separate macro so we don't have to cache the values
# from PATH_ITCLCONFIG (because this can also be cached).
AC_DEFUN([CY_AC_LOAD_ITCLCONFIG],[
    AC_REQUIRE([CY_AC_PATH_ITCLCONFIG])
    if test -f "${ITCLCONFIG}"; then
      AC_MSG_NOTICE([sourcing ${ITCLCONFIG}])
      . $ITCLCONFIG
    fi

    AC_SUBST([ITCL_VERSION])dnl
    AC_SUBST([ITCL_DEFS])dnl
    AC_SUBST([ITCL_BUILD_INCLUDES])dnl
    AC_SUBST([ITCL_BUILD_LIB_SPEC])dnl
    AC_SUBST([ITCL_LIB_SPEC])dnl
])dnl

# check for Itcl headers. 

AC_DEFUN([CY_AC_PATH_ITCLH],[
AC_MSG_CHECKING([for Itcl private headers. srcdir=${srcdir}])
if test x"${ac_cv_c_itclh}" = x""; then
  for i in ${srcdir}/../itcl ${srcdir}/../../itcl ${srcdir}/../../../itcl ${srcdir}/../itcl/itcl; do
    if test -f ${i}/generic/itcl.h ; then
      ac_cv_c_itclh=`(cd ${i}/generic; pwd)`
      break
    fi
  done
fi
if test x"${ac_cv_c_itclh}" = x""; then
  ITCLHDIR="# no Itcl private headers found"
  AC_MSG_ERROR([Cannot find Itcl private headers])
fi
if test x"${ac_cv_c_itclh}" != x""; then
     AC_MSG_RESULT([found in ${ac_cv_c_itclh}])
     ITCLHDIR="-idirafter ${ac_cv_c_itclh}"
fi
AC_SUBST([ITCLHDIR])dnl
])dnl


AC_DEFUN([CY_AC_PATH_ITKCONFIG],[
#
# Ok, let us find the itk configuration
# First, look for one uninstalled.  
# the alternative search directory is invoked by --with-itkconfig
#

if test x"${no_itk}" = x""; then
  # we reset no_itk in case something fails here:
  no_itk=true
  AC_ARG_WITH([itkconfig],
              [AS_HELP_STRING([--with-itkconfig],
                 [Directory containing itk configuration (itkConfig.sh)])],
         [with_itkconfig=${withval}])
  AC_MSG_CHECKING([for Itk configuration])
  AC_CACHE_VAL([ac_cv_c_itkconfig],[

  # First check to see if --with-itkconfig was specified.
  if test x"${with_itkconfig}" != x""; then
    if test -f "${with_itkconfig}/itkConfig.sh"; then
      ac_cv_c_itkconfig=`(cd ${with_itkconfig}; pwd)`
    else
      AC_MSG_ERROR([${with_itkconfig} directory does NOT contain itkConfig.sh])
    fi
  fi

  # then check for a private Itk library:
  if test x"${ac_cv_c_itkconfig}" = x""; then
    for i in \
		../itcl/itk \
		`ls -dr ../itcl[[4-9]]*/itk 2>/dev/null` \
		../../itk \
		`ls -dr ../../itcl[[4-9]]*/itk 2>/dev/null` \
		../../../itk \
		`ls -dr ../../../itcl[[4-9]]*/itk 2>/dev/null` ; do
      if test -f "${i}/itkConfig.sh" ; then
        ac_cv_c_itkconfig=`(cd ${i}; pwd)`
	break
      fi
    done
  fi
  # check in a few common install locations:
  if test x"${ac_cv_c_itkconfig}" = x""; then
    for i in `ls -d ${prefix}/lib /usr/local/lib 2>/dev/null`; do
      if test -f "${i}/itkConfig.sh"; then
        ac_cv_c_itkconfig=`(cd ${i}; pwd)`
	break
      fi
    done
  fi
  # check in a few other private locations:
  if test x"${ac_cv_c_itkconfig}" = x""; then
    for i in \
		${srcdir}/../itcl/itk \
		`ls -dr ${srcdir}/../itcl[[4-9]]*/itk 2>/dev/null`; do
      if test -f "${i}/itkConfig.sh"; then
        ac_cv_c_itkconfig=`(cd ${i}; pwd)`
	break
      fi
    done
  fi
  ])
  if test x"${ac_cv_c_itkconfig}" = x""; then
    ITKCONFIG="# no Itk configs found"
    AC_MSG_WARN([Cannot find Itk configuration definitions])
  else
    no_itk=""
    ITKCONFIG=${ac_cv_c_itkconfig}/itkConfig.sh
    AC_MSG_RESULT([found ${ITKCONFIG}])
  fi
fi
])dnl

# Defined as a separate macro so we do NOT have to cache the values
# from PATH_ITKCONFIG (because this can also be cached).
AC_DEFUN([CY_AC_LOAD_ITKCONFIG],[
    AC_REQUIRE([CY_AC_PATH_ITKCONFIG])
    if test -f "${ITKCONFIG}"; then
      AC_MSG_NOTICE([sourcing ${ITKCONFIG}])
      . ${ITKCONFIG}
    fi

    AC_SUBST([ITK_VERSION])dnl
    AC_SUBST([ITK_DEFS])dnl
    AC_SUBST([ITK_BUILD_INCLUDES])dnl
    AC_SUBST([ITK_BUILD_LIB_SPEC])dnl
    AC_SUBST([ITK_LIB_SPEC])dnl
])dnl

AC_DEFUN([CY_AC_PATH_ITKH],[
AC_MSG_CHECKING([for Itk private headers. srcdir=${srcdir}])
if test x"${ac_cv_c_itkh}" = x""; then
  for i in ${srcdir}/../itcl ${srcdir}/../../itcl ${srcdir}/../../../itcl ${srcdir}/../itcl/itk; do
    if test -f ${i}/generic/itk.h ; then
      ac_cv_c_itkh=`(cd ${i}/generic; pwd)`
      break
    fi
  done
fi
if test x"${ac_cv_c_itkh}" = x""; then
  ITKHDIR="# no Itk private headers found"
  AC_MSG_ERROR([Cannot find Itk private headers])
fi
if test x"${ac_cv_c_itkh}" != x""; then
     AC_MSG_RESULT([found in ${ac_cv_c_itkh}])
     ITKHDIR="-idirafter ${ac_cv_c_itkh}"
fi
AC_SUBST([ITKHDIR])dnl
])dnl

# check for Tix headers. 

AC_DEFUN([CY_AC_PATH_TIXH],[
AC_MSG_CHECKING([for Tix private headers. srcdir=${srcdir}])
if test x"${ac_cv_c_tixh}" = x""; then
  for i in ${srcdir}/../tix ${srcdir}/../../tix ${srcdir}/../../../tix; do
    if test -f ${i}/generic/tix.h ; then
      ac_cv_c_tixh=`(cd ${i}/generic; pwd)`
      break
    fi
  done
fi
if test x"${ac_cv_c_tixh}" = x""; then
  TIXHDIR="# no Tix private headers found"
  AC_MSG_ERROR([Cannot find Tix private headers])
fi
if test x"${ac_cv_c_tixh}" != x""; then
     TIXHDIR="-I${ac_cv_c_tixh}"
fi
AC_SUBST([TIXHDIR])dnl
])dnl

AC_DEFUN([CY_AC_PATH_TIXCONFIG],[
#
# Ok, let us find the tix configuration
# First, look for one uninstalled.  
# the alternative search directory is invoked by --with-itkconfig
#

if test x"${no_tix}" = x""; then
  # we reset no_tix in case something fails here
  no_tix=true
  AC_ARG_WITH([tixconfig],
              [AS_HELP_STRING([--with-tixconfig],
                              [Directory containing tix configuration (tixConfig.sh)])],
              [with_tixconfig=${withval}])
  AC_MSG_CHECKING([for Tix configuration])
  AC_CACHE_VAL([ac_cv_c_tixconfig],[

  # First check to see if --with-tixconfig was specified.
  if test x"${with_tixconfig}" != x""; then
    if test -f "${with_tixconfig}/tixConfig.sh"; then
      ac_cv_c_tixconfig=`(cd ${with_tixconfig}; pwd)`
    else
      AC_MSG_ERROR([${with_tixconfig} directory does NOT contain tixConfig.sh])
    fi
  fi

  # then check for a private Tix library
  if test x"${ac_cv_c_tixconfig}" = x""; then
    for i in \
		../tix \
		`ls -dr ../tix 2>/dev/null` \
		../../tix \
		`ls -dr ../../tix 2>/dev/null` \
		../../../tix \
		`ls -dr ../../../tix 2>/dev/null`; do
      echo "**** Looking at ${i} - with ${configdir}"
      if test -f "${i}/tixConfig.sh" ; then
        ac_cv_c_tixconfig=`(cd ${i}; pwd)`
	break
      fi
    done
  fi
  # check in a few common install locations
  if test x"${ac_cv_c_tixconfig}" = x""; then
    for i in `ls -d ${prefix}/lib /usr/local/lib 2>/dev/null`; do
      echo "**** Looking at $i"
      if test -f "${i}/tixConfig.sh" ; then
        ac_cv_c_tixconfig=`(cd ${i}; pwd)`
	break
      fi
    done
  fi
  # check in a few other private locations
  echo "**** Other private locations"
  if test x"${ac_cv_c_tixconfig}" = x""; then
    for i in \
		${srcdir}/../tix \
		`ls -dr ${srcdir}/../tix 2>/dev/null`; do
      echo "**** Looking at ${i} - with ${configdir}"
      if test -f "${i}/${configdir}/tixConfig.sh"; then
        ac_cv_c_tixconfig=`(cd ${i}/${configdir}; pwd)`
	break
      fi
    done
  fi
  ])
  if test x"${ac_cv_c_tixconfig}" = x""; then
    TIXCONFIG="# no Tix configs found"
    AC_MSG_WARN([Cannot find Tix configuration definitions])
  else
    no_tix=""
    TIXCONFIG=${ac_cv_c_tixconfig}/tixConfig.sh
    AC_MSG_RESULT([found ${TIXCONFIG}])
  fi
fi
])dnl

# Defined as a separate macro so we do NOT have to cache the values
# from PATH_TIXCONFIG (because this can also be cached).
AC_DEFUN([CY_AC_LOAD_TIXCONFIG],[
    if test -f "${TIXCONFIG}"; then
      . ${TIXCONFIG}
    fi

    AC_SUBST([TIX_VERSION])dnl
    AC_SUBST([TIX_BUILD_LIB_SPEC])dnl
])dnl

dnl# the macro definition that used to be here has moved to:
sinclude(m4/ccstdc.m4)

dnl# From Bruno Haible.

AC_DEFUN([AM_ICONV],[
  AC_REQUIRE([AC_HEADER_STDC])dnl
  AC_REQUIRE([AC_PROG_CC])dnl
  AC_REQUIRE([AC_PROG_CPP])dnl
  AC_REQUIRE([AC_PROG_CXX])dnl

  dnl# Some systems have iconv in libc, some have it in libiconv (OSF/1 and
  dnl# those with the standalone portable GNU libiconv installed).

  AC_ARG_WITH([libiconv-prefix],
              [AS_HELP_STRING([--with-libiconv-prefix=DIR],
                      [search for libiconv in DIR/include and DIR/lib])],[
    for dir in `echo "${withval}" | tr : ' '`; do
      if test -d ${dir}/include; then CPPFLAGS="${CPPFLAGS} -I${dir}/include"; fi
      if test -d ${dir}/lib; then LDFLAGS="${LDFLAGS} -L${dir}/lib"; fi
    done
   ])dnl

  AC_CACHE_CHECK([for iconv],[am_cv_func_iconv],[
    am_cv_func_iconv="no, consider installing GNU libiconv"
    am_cv_lib_iconv=no
    AC_LINK_IFELSE([AC_LANG_SOURCE([[
#include <stdlib.h>
#include <iconv.h>]],[[
       iconv_t cd = iconv_open("","");
       iconv(cd, NULL, NULL, NULL, NULL);
       iconv_close(cd);]])],
      [am_cv_func_iconv=yes])
    if test "x${am_cv_func_iconv}" != "xyes"; then
      am_save_LIBS="${LIBS}"
      LIBS="${LIBS} -liconv"
      AC_LINK_IFELSE([AC_LANG_SOURCE([[
#include <stdlib.h>
#include <iconv.h>]],[[
         iconv_t cd = iconv_open("","");
         iconv(cd, NULL, NULL, NULL, NULL);
         iconv_close(cd);]])],
        [am_cv_lib_iconv=yes]
        [am_cv_func_iconv=yes])
      LIBS="${am_save_LIBS}"
    fi
  ])
  if test "x${am_cv_func_iconv}" = "xyes"; then
    AC_DEFINE([HAVE_ICONV],[1],[Define if you have the iconv() function.])
    AC_MSG_CHECKING([for iconv declaration])
    AC_CACHE_VAL([am_cv_proto_iconv],[
      AC_COMPILE_IFELSE([AC_LANG_SOURCE([[
#include <stdlib.h>
#include <iconv.h>
extern
#ifdef __cplusplus
"C"
#endif /* __cplusplus */
#if defined(__STDC__) || defined(__cplusplus)
size_t iconv (iconv_t cd, char * *inbuf, size_t *inbytesleft, char * *outbuf, size_t *outbytesleft);
#else
size_t iconv();
#endif /* __STDC__ || __cplusplus */
]],[[]])],[am_cv_proto_iconv_arg1=""],[am_cv_proto_iconv_arg1="const"])
      am_cv_proto_iconv="extern size_t iconv (iconv_t cd, $am_cv_proto_iconv_arg1 char * *inbuf, size_t *inbytesleft, char * *outbuf, size_t *outbytesleft);"])
    am_cv_proto_iconv=`echo "[$]am_cv_proto_iconv" | tr -s ' ' | sed -e 's/( /(/'`
    AC_MSG_RESULT([$]{ac_t:-
         }[$]am_cv_proto_iconv)
    AC_DEFINE_UNQUOTED([ICONV_CONST],[${am_cv_proto_iconv_arg1}],
      [Define as const if the declaration of iconv() needs const.])
  fi
  LIBICONV=""
  if test "x${am_cv_lib_iconv}" = "xyes"; then
    LIBICONV="-liconv"
  fi
  AC_SUBST([LIBICONV])dnl
])dnl

dnl# written by Guido Draheim <guidod@gmx.de>, original by Alexandre Oliva 
dnl# Version 1.3 (2001/03/02)
dnl# source:
# <http://www.gnu.org/software/ac-archive/Miscellaneous/ac_define_dir.html>

AC_DEFUN([AC_DEFINE_DIR],[
  test "x${prefix}" = "xNONE" && prefix="$ac_default_prefix"
  test "x${exec_prefix}" = "xNONE" && exec_prefix='${prefix}'
  ac_define_dir=`eval echo [$]$2`
  ac_define_dir=`eval echo [$]ac_define_dir`
  ifelse([$3],[],[
    AC_DEFINE_UNQUOTED([$1],["${ac_define_dir}"])
  ],[
    AC_DEFINE_UNQUOTED([$1],["${ac_define_dir}"],[$3])
  ])dnl
])dnl

dnl# See whether we need a declaration for a function.
dnl# The result is highly dependent on the INCLUDES passed in, so make sure
dnl# to use a different cache variable name in this macro if it is invoked
dnl# in a different context somewhere else.
dnl# gcc_AC_CHECK_DECL([SYMBOL],
dnl# 	[ACTION-IF-FOUND],[ACTION-IF-NOT-FOUND],[INCLUDES])
AC_DEFUN([gcc_AC_CHECK_DECL],
[AC_MSG_CHECKING([whether $1 is declared])
AC_CACHE_VAL([gcc_cv_have_decl_$1],
[AC_COMPILE_IFELSE([AC_LANG_SOURCE([[$4]],
[[#ifndef $1
char *(*pfn) = (char *(*)) $1 ;
#endif]])],[eval "gcc_cv_have_decl_$1=yes"],[eval "gcc_cv_have_decl_$1=no"])])
if eval "test \"`echo '$gcc_cv_have_decl_'$1`\" = yes"; then
  AC_MSG_RESULT([yes]) ; ifelse([$2],[],[:],[$2])
else
  AC_MSG_RESULT([no]) ; ifelse([$3],[],[:],[$3])
fi
])dnl

dnl# Check multiple functions to see whether each needs a declaration.
dnl# Arrange to define HAVE_DECL_<FUNCTION> to 0 or 1 as appropriate.
dnl# gcc_AC_CHECK_DECLS([SYMBOLS],
dnl# 	[ACTION-IF-NEEDED],[ACTION-IF-NOT-NEEDED],[INCLUDES])
AC_DEFUN([gcc_AC_CHECK_DECLS],
[for ac_func in $1
do
changequote(, )dnl
  ac_tr_decl=HAVE_DECL_`echo $ac_func | tr 'abcdefghijklmnopqrstuvwxyz' 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'`
changequote([, ])dnl
gcc_AC_CHECK_DECL([${ac_func}],
  [AC_DEFINE_UNQUOTED([${ac_tr_decl}],[1],[Define to 1 if ???]) $2],
  [AC_DEFINE_UNQUOTED([${ac_tr_decl}],[0],[Define to 0 if ???]) $3],[
dnl# It is possible that the include files passed in here are local headers
dnl# which supply a backup declaration for the relevant prototype based on
dnl# the definition of (or lack of) the HAVE_DECL_ macro. If so, this test
dnl# will always return success.  E.g. see libiberty.h's handling of
dnl# `basename'. To avoid this, we define the relevant HAVE_DECL_ macro to
dnl# 1 so that any local headers used do not provide their own prototype
dnl# during this test.
#undef ${ac_tr_decl}
#define ${ac_tr_decl} 1
  $4
])
done
dnl# Automatically generate config.h entries via autoheader.
if test "x" = "Y0"; then
  patsubst([translit([$1],[a-z],[A-Z])],[\w+],
    [AC_DEFINE([HAVE_DECL_\&],[1],
      [Define to 1 if we found this declaration otherwise define to 0.])])dnl
fi
])dnl

dnl# Find the location of the private Tcl headers
dnl# When Tcl is installed, this is TCL_INCLUDE_SPEC/tcl-private/generic
dnl# When Tcl is in the build tree, this is not needed.
dnl#
dnl# Note: you must use first use SC_LOAD_TCLCONFIG!
AC_DEFUN([CY_AC_TCL_PRIVATE_HEADERS],[
  AC_REQUIRE([AC_PROG_SED])dnl

  AC_MSG_CHECKING([for Tcl private headers])
  private_dir=""
  dir=`echo ${TCL_INCLUDE_SPEC}/tcl-private/generic | sed -e s/-I//`
  if test -f ${dir}/tclInt.h ; then
    private_dir=${dir}
  fi

  if test x"${private_dir}" = x""; then
    AC_ERROR([could not find private Tcl headers])
  else
    TCL_PRIVATE_INCLUDE="-idirafter ${private_dir}"
    AC_MSG_RESULT([${private_dir}])
  fi
])dnl

dnl# Find the location of the private Tk headers
dnl# When Tk is installed, this is TK_INCLUDE_SPEC/tk-private/generic
dnl# When Tk is in the build tree, this not needed.
dnl#
dnl# Note: you must first use SC_LOAD_TKCONFIG
AC_DEFUN([CY_AC_TK_PRIVATE_HEADERS],[
  AC_REQUIRE([AC_PROG_SED])dnl

  AC_MSG_CHECKING([for Tk private headers])
  private_dir=""
  dir=`echo ${TK_INCLUDE_SPEC}/tk-private/generic | sed -e s/-I//`
  if test -f ${dir}/tkInt.h; then
    private_dir=${dir}
  fi

  if test x"${private_dir}" = x""; then
    AC_ERROR([could not find Tk private headers])
  else
    TK_PRIVATE_INCLUDE="-idirafter ${private_dir}"
    AC_MSG_RESULT([${private_dir}])
  fi
])dnl

dnl# GDB_AC_DEFINE_RELOCATABLE([VARIABLE],[ARG-NAME],[SHELL-VARIABLE])
dnl# For use in processing directory values for --with-foo.
dnl# If the path in SHELL_VARIABLE is relative to the prefix, then the
dnl# result is relocatable, then this will define the C macro
dnl VARIABLE_RELOCATABLE to 1; otherwise it is defined as 0.
AC_DEFUN([GDB_AC_DEFINE_RELOCATABLE], [
  if test "x${exec_prefix}" = "xNONE" || test "x${exec_prefix}" = 'x${prefix}'; then
     if test "x${prefix}" = "xNONE"; then
     	test_prefix=/usr/local
     else
	test_prefix="${prefix}"
     fi
  else
     test_prefix="${exec_prefix}"
  fi
  value=0
  case [$3] in
     "${test_prefix}"|"${test_prefix}/"*|\
	'${exec_prefix}'|'${exec_prefix}/'*)
     value=1
     ;;
  esac
  AC_DEFINE_UNQUOTED([$1]_RELOCATABLE,[${value}],
     [Define if the $2 directory should be relocated when GDB is moved.])
])dnl

dnl# GDB_AC_WITH_DIR([VARIABLE],[ARG-NAME],[HELP],[DEFAULT])
dnl# Add a new --with option that defines a directory.
dnl# The result is stored in VARIABLE.  AC_DEFINE_DIR is called
dnl# on this variable, as is AC_SUBST.
dnl# ARG-NAME is the base name of the argument (without "--with").
dnl# HELP is the help text to use.
dnl# If the user's choice is relative to the prefix, then the
dnl# result is relocatable, then this will define the C macro
dnl# VARIABLE_RELOCATABLE to 1; otherwise it is defined as 0.
dnl# DEFAULT is the default value, which is used if the user
dnl# does not specify the argument.
AC_DEFUN([GDB_AC_WITH_DIR],[
  AC_ARG_WITH([$2],[AS_HELP_STRING([--with-][$2][=PATH],[$3])],[
    [$1]=${withval}],[[$1]=[$4]])
  AC_DEFINE_DIR([$1],[$1],[$3])dnl
  AC_SUBST([$1])dnl
  GDB_AC_DEFINE_RELOCATABLE([$1],[$2],[${ac_define_dir}])dnl
])dnl

dnl# GDB_AC_CHECK_BFD([MESSAGE],[CV],[CODE],[HEADER])
dnl# Check whether BFD provides a feature.
dnl# MESSAGE is the "checking" message to display.
dnl# CV is the name of the cache variable where the result is stored.
dnl# The result will be "yes" or "no".
dnl# CODE is some code to compile that checks for the feature.
dnl# A link test is run.
dnl# HEADER is the name of an extra BFD header to include.
AC_DEFUN([GDB_AC_CHECK_BFD],[
  OLD_CFLAGS="${CFLAGS}"
  OLD_LDFLAGS="${LDFLAGS}"
  OLD_LIBS="${LIBS}"
  # Put the old CFLAGS/LDFLAGS last, in case the user's (C|LD)FLAGS
  # points somewhere with bfd, with -I/foo/lib and -L/foo/lib.  We
  # always want our bfd.
  CFLAGS="-I${srcdir}/../include -I../bfd -I${srcdir}/../bfd ${CFLAGS}"
  LDFLAGS="-L../bfd -L../libiberty ${zlibdir} ${LDFLAGS}"
  intl=`echo ${LIBINTL} | sed 's,${top_builddir}/,,g'`
  if test -f ../zlib/Makefile; then
    LIBS="-lbfd -L../zlib -lz -liberty ${intl} ${LIBS}"
  else
    LIBS="-lbfd -lz -liberty ${intl} ${LIBS}"
  fi
  AC_CACHE_CHECK([$1],[$2],
  [AC_TRY_LINK(
  [#include <stdlib.h>
  #include "bfd.h"
  #include "$4"
  ],
  [return $3;],[[$2]=yes],[[$2]=no])])
  CFLAGS="${OLD_CFLAGS}"
  LDFLAGS="${OLD_LDFLAGS}"
  LIBS="${OLD_LIBS}"
])dnl

dnl# GDB_GUILE_PROGRAM_NAMES([PKG-CONFIG],[VERSION])
dnl#
dnl# Define and substitute 'GUILD' to contain the absolute file name of
dnl# the 'guild' command for VERSION, using PKG-CONFIG.  (This is
dnl# similar to Guile's 'GUILE_PROGS' macro.)
AC_DEFUN([GDB_GUILE_PROGRAM_NAMES],[
  AC_CACHE_CHECK([for the absolute file name of the 'guild' command],
    [ac_cv_guild_program_name],
    [ac_cv_guild_program_name="`$1 --variable guild $2`"

     # In Guile up to 2.0.11 included, guile-2.0.pc would not define
     # the 'guild' and 'bindir' variables.  In that case, try to guess
     # what the program name is, at the risk of getting it wrong if
     # Guile was configured with '--program-suffix' or similar.
     if test "x${ac_cv_guild_program_name}" = "x"; then
       guile_exec_prefix="`$1 --variable exec_prefix $2`"
       ac_cv_guild_program_name="$guile_exec_prefix/bin/guild"
     fi
  ])dnl

  if ! "${ac_cv_guild_program_name}" --version >&AS_MESSAGE_LOG_FD 2>&AS_MESSAGE_LOG_FD; then
    AC_MSG_ERROR(['${ac_cv_guild_program_name}' appears to be unusable])
  fi

  GUILD="${ac_cv_guild_program_name}"
  AC_SUBST([GUILD])dnl
])dnl

dnl# GDB_GUILD_TARGET_FLAG
dnl#
dnl# Compute the value of GUILD_TARGET_FLAG.
dnl# For native builds this is empty.
dnl# For cross builds this is --target=<host>.
AC_DEFUN([GDB_GUILD_TARGET_FLAG],[
  if test "x${cross_compiling}" = "xno"; then
    GUILD_TARGET_FLAG=""
  else
    GUILD_TARGET_FLAG="--target=${host}"
  fi
  AC_SUBST([GUILD_TARGET_FLAG])dnl
])dnl

dnl# GDB_TRY_GUILD([SRC-FILE])
dnl#
dnl# We precompile the .scm files and install them with gdb, so make sure
dnl# guild works for this host.
dnl# The .scm files are precompiled for several reasons:
dnl# 1) To silence Guile during gdb startup (Guile's auto-compilation
dnl#    output is unnecessarily verbose).
dnl# 2) Make gdb developers see compilation errors/warnings during the
dnl#    build, and not leave it to later when the user runs gdb.
dnl# 3) As a convenience for the user, so that one copy of the files is
dnl#    built instead of one copy per user.
dnl#
dnl# Make sure guild can handle this host by trying to compile SRC-FILE,
dnl# and setting ac_cv_guild_ok to yes or no.
dnl# Note that guild can handle cross-compilation.
dnl# It could happen that guild cannot handle the host, but guile would
dnl# still work.  For the time being we are conservative, and if guild
dnl# fails on us, we then punt.
AC_DEFUN([GDB_TRY_GUILD],[
  AC_REQUIRE([GDB_GUILD_TARGET_FLAG])dnl
  ## actual check now:
  AC_CACHE_CHECK([whether guild supports this host],
    [ac_cv_guild_ok],
    [echo "${ac_cv_guild_program_name} compile ${GUILD_TARGET_FLAG} -o conftest.go $1" >&AS_MESSAGE_LOG_FD
     if "${ac_cv_guild_program_name}" compile ${GUILD_TARGET_FLAG} -o conftest.go "$1" >&AS_MESSAGE_LOG_FD 2>&AS_MESSAGE_LOG_FD; then
       ac_cv_guild_ok=yes
     else
       ac_cv_guild_ok=no
     fi])dnl
])dnl

dnl# taken from ../bfd/warning.m4:
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
