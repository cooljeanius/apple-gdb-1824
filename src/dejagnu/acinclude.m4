dnl# dejagnu/acinclude.m4                                -*- Autoconf -*-

sinclude(../config/warnings.m4)

#  AC_PROG_CXX_OLD

# FIXME: We temporarily define our own version of AC_PROG_CXX.
# This is copied from autoconf 2.13, but does not call AC_PROG_CXX_WORKS.
# We do not need a C++ compiler in order to build dejagnu, so
# if we cannot find a working compiler it is not the end of
# the world.  This is fixed in later versions of autoconf where
# there is different macro which will skip the link test entirely.

AC_DEFUN([AC_PROG_CXX_OLD],
[AC_BEFORE([$0],[AC_PROG_CXXCPP])dnl
AC_CHECK_PROGS([CXX],[${CCC} c++ g++ gcc CC cxx cc++ cl],[gcc])dnl

AC_REQUIRE([AC_PROG_CXX_GNU])dnl
AC_REQUIRE([AC_PROG_RANLIB])dnl

if test "x${ac_cv_prog_gxx_variable}" = "xyes"; then
  GXX=yes
else
  GXX=""
fi

dnl# Check whether -g works, even if CXXFLAGS is set, in case the package
dnl# plays around with CXXFLAGS (such as to build both debugging and
dnl# normal versions of a library), tasteless as that idea is.
ac_test_CXXFLAGS="${CXXFLAGS+set}"
ac_save_CXXFLAGS="${CXXFLAGS}"
CXXFLAGS=""
AC_REQUIRE([AC_PROG_CXX_G])
if test "x${ac_test_CXXFLAGS}" = "xset"; then
  CXXFLAGS="$ac_save_CXXFLAGS"
elif test "x${ac_cv_prog_cxx_g}" = "xyes"; then
  if test "x${GXX}" = "xyes"; then
    CXXFLAGS="-g -O2"
  else
    CXXFLAGS="-g"
  fi
else
  if test "x${GXX}" = "xyes"; then
    CXXFLAGS="-O2"
  else
    CXXFLAGS=""
  fi
fi
])dnl

AC_DEFUN([AC_PROG_CXX_GNU],
[AC_CACHE_CHECK([whether we are using GNU C++],[ac_cv_prog_gxx_variable],
[dnl# The semicolon is to pacify NeXT's syntax-checking cpp.
cat > conftest.C <<EOF
#ifdef __GNUC__
  yes;
#endif /* __GNUC__ */
EOF
AC_REQUIRE([AC_PROG_EGREP])
if AC_TRY_COMMAND([${CXX-g++} -E conftest.C]) | egrep yes >/dev/null 2>&1; then
  ac_cv_prog_gxx_variable=yes
else
  ac_cv_prog_gxx_variable=no
fi])])dnl

AC_DEFUN([AC_PROG_CXX_G],
[AC_CACHE_CHECK([whether ${CXX-g++} accepts -g],[ac_cv_prog_cxx_g],
[echo 'void f(){}' > conftest.cc
if test -z "`${CXX-g++} -g -c conftest.cc 2>&1`"; then
  ac_cv_prog_cxx_g=yes
else
  ac_cv_prog_cxx_g=no
fi
rm -rf conftest*
])])dnl

AC_DEFUN([DJ_AC_STL],[
AC_MSG_CHECKING([for STL versions])
AC_CACHE_VAL([ac_cv_stl],[
  AC_LANG_PUSH([C++])
  AC_COMPILE_IFELSE([AC_LANG_SOURCE([[#include <iostream>]],[[
  using namespace std;
  char bbuuff[5120];
  cout.rdbuf()->pubsetbuf(bbuuff, 5120); ]])],[
  ac_cv_stl=v3
  ],[
  ac_cv_stl=v2
  ])
  AC_LANG_POP
  AC_LANG([C])dnl
],[
  :
])dnl

if test x"${ac_cv_stl}" != x"v2"; then  
  AC_MSG_RESULT([v3])
  AC_DEFINE([HAVE_STL3],[3],[Define to 3 if we have STL3])
else
  AC_MSG_RESULT([v2])
fi
])dnl

AC_DEFUN([DJ_AC_PATH_TCLSH],[
dirlist=".. ../../ ../../../ ../../../../ ../../../../../ ../../../../../../ ../
../../../../../.. ../../../../../../../.. ../../../../../../../../.. ../../../..
/../../../../../.."
no_itcl=true
AC_MSG_CHECKING([for the tclsh program])
AC_ARG_WITH([tclinclude],
            [AS_HELP_STRING([--with-tclinclude],
                            [directory where tcl headers are])],
            [with_tclinclude=${withval}])
AC_CACHE_VAL([ac_cv_path_tclsh],[
dnl# first check to see if --with-itclinclude was specified:
if test x"${with_tclinclude}" != x""; then
  if test -f ${with_tclinclude}/tclsh ; then
    ac_cv_path_tclsh=`(cd ${with_tclinclude}; pwd)`
  elif test -f ${with_tclinclude}/src/tclsh ; then
    ac_cv_path_tclsh=`(cd ${with_tclinclude}/src; pwd)`
  else
    AC_MSG_ERROR([${with_tclinclude} directory does NOT contain tclsh])
  fi
fi
])dnl

dnl# next check in private source directory
dnl# since ls returns lowest version numbers first, reverse its output
if test x"${ac_cv_path_tclsh}" = x""; then
    dnl find the top level Itcl source directory:
    for i in ${dirlist}; do
        if test -n "`ls -dr ${srcdir}/${i}/tcl* 2>/dev/null`"; then
            tclpath=${srcdir}/${i}
            break
        fi
    done

    dnl# find the exact Itcl source dir. We do it this way, because there
    dnl# might be multiple version of Itcl, and we want the most recent
    dnl# one.
    for i in `ls -dr $tclpath/tcl* 2>/dev/null `; do
        if test -f $i/src/tclsh ; then
          ac_cv_path_tclsh=`(cd $i/src; pwd)`/tclsh
          break
        fi
    done
fi

dnl# see if one is installed:
if test x"${ac_cv_path_tclsh}" = x""; then
   AC_MSG_RESULT([none])
   AC_PATH_PROG([TCLSH],[tclsh])
else
   AC_MSG_RESULT([${ac_cv_path_tclsh}])
fi
TCLSH="${ac_cv_path_tclsh}"
AC_SUBST([TCLSH])dnl
])dnl


AC_DEFUN([DJ_AC_PATH_DOCBOOK],[
dirlist=".. ../../ ../../.. ../../../.. ../../../../.. ../../../../../.. ../../../../../../.. ../../../../../../../.. ../../../../../../../../.. ../../../../../../../../../.."
AC_MSG_CHECKING([for docbook tools])
AC_ARG_WITH([oskith],
            [AS_HELP_STRING([--with-docbook],
                            [directory where the db2 sgml tools are])],
            [with_docbook=${withval}])
AC_CACHE_VAL([ac_cv_c_docbook],[
dnl# first check to see if --with-docbook was specified:
if test x"${with_docbook}" != x""; then
  if test -f ${with_docbook}/db2html; then
    ac_cv_c_docbook=`(cd ${with_docbook}; pwd)`
  else
    AC_MSG_ERROR([${with_docbook} directory does NOT contain SGML tools])
  fi
fi
])
if test x"${ac_cv_c_docbook}" = x""; then
    for i in ${ac_default_prefix}/bin ${prefix}/bin /usr/local/bin ${OSKITHDIR}/../bin /usr/bin /bin /opt /opt/local/bin /sw/bin /opt/sw/bin /home; do
	dnl# See is we have an SGML tool in that directory.
	if test -f ${i}/db2html; then
	    ac_cv_c_docbook=${i}
	    break
	fi
    done
fi

if test x"${ac_cv_c_docbook}" = x""; then
    AC_MSG_RESULT([none])
else
    DOCBOOK="${ac_cv_c_docbook}"
    AC_MSG_RESULT([${ac_cv_c_docbook}])
fi

AC_SUBST([DOCBOOK])dnl
])dnl
