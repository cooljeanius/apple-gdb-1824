# ansi-c++.m4 serial 10
dnl# Copyright (C) 2002-2003, 2005, 2010-2014 Free Software Foundation, Inc
dnl# This file is free software; the Free Software Foundation
dnl# gives unlimited permission to copy and/or distribute it,
dnl# with or without modifications, as long as this notice is preserved.

dnl# From Bruno Haible.

# Sets CXX_CHOICE to 'yes' or 'no', depending on the preferred use of C++.
# The default is 'yes'. If the configure.ac contains a definition of the
# macro gl_CXX_CHOICE_DEFAULT_NO, then the default is 'no'. In both cases,
# the user can change the value by passing the option --disable-cxx or
# --enable-cxx, respectively.

AC_DEFUN([gl_CXX_CHOICE],[
  AC_MSG_CHECKING([whether to use C++])
  dnl# Plus signs are supported in AC_ARG_ENABLE starting with
  dnl# autoconf-2.66:
  m4_version_prereq([2.66],
    [m4_ifdef([gl_CXX_CHOICE_DEFAULT_NO],
       [AC_ARG_ENABLE([c++],
          [AS_HELP_STRING([--enable-c++],[also build C++ sources])],
          [CXX_CHOICE="${enableval}"],
          [CXX_CHOICE=no])],
       [AC_ARG_ENABLE([c++],
          [AS_HELP_STRING([--disable-c++],[do not build C++ sources])],
          [CXX_CHOICE="${enableval}"],
          [CXX_CHOICE=yes])])],
    [m4_ifdef([gl_CXX_CHOICE_DEFAULT_NO],
       [AC_ARG_ENABLE([cxx],
          [AS_HELP_STRING([--enable-cxx],[also build C++ sources])],
          [CXX_CHOICE="${enableval}"],
          [CXX_CHOICE=no])],
       [AC_ARG_ENABLE([cxx],
          [AS_HELP_STRING([--disable-cxx],[do not build C++ sources])],
          [CXX_CHOICE="${enableval}"],
          [CXX_CHOICE=yes])])])
  AC_MSG_RESULT([${CXX_CHOICE}])
  AC_SUBST([CXX_CHOICE])dnl
])dnl

# gl_PROG_ANSI_CXX([ANSICXX_VARIABLE],[ANSICXX_CONDITIONAL])
# Sets ANSICXX_VARIABLE to the name of a sufficiently ANSI C++ compliant
# compiler, or to "no" if none is found.
# Defines the Automake condition ANSICXX_CONDITIONAL to true if such
# a compiler was found, or to false if not.

AC_DEFUN([gl_PROG_ANSI_CXX],[
  AC_REQUIRE([AC_EXEEXT])dnl
  AC_REQUIRE([gl_CXX_CHOICE])dnl
  m4_if([$1],[CXX],[],
    [gl_save_CXX="${CXX}"])dnl
  ## conditionalize based on result from macro that provides this variable:
  if test "x${CXX_CHOICE}" = "xno"; then
    CXX=no
  fi
  if test -z "${CXX}"; then
    if test -n "${CCC}"; then
      CXX="${CCC}"
    else
      AC_CHECK_TOOLS([CXX],
                     [g++ c++ cxx cc++ cl${ac_exeext} clang++ xlC_r xlC],
                     [:])
    fi
  fi
  if test "x${CXX}" != "xno"; then
    dnl# Use a modified version of AC_PROG_CXX_WORKS that does not exit
    dnl# upon failure.
    AC_MSG_CHECKING([whether the C++ compiler (${CXX} ${CXXFLAGS} ${LDFLAGS}) works])
    AC_LANG_PUSH([C++])dnl
    AC_ARG_VAR([CXX],[C++ compiler command])dnl
    AC_ARG_VAR([CXXFLAGS],[C++ compiler flags])dnl
    ## why is this conftest done so hackily?
    echo 'int main () { return 0; }' > conftest.${ac_ext}
    if AC_TRY_EVAL([ac_link]) && test -s conftest${ac_exeext}; then
      gl_cv_prog_ansicxx_works=yes
      if (./conftest; exit) 2>/dev/null; then
        gl_cv_prog_ansicxx_cross=no
      else
        gl_cv_prog_ansicxx_cross=yes
      fi
    else
      gl_cv_prog_ansicxx_works=no
    fi
    rm -fr conftest*
    AC_LANG_POP([C++])
    AC_MSG_RESULT([${gl_cv_prog_ansicxx_works}])
    if test "x${gl_cv_prog_ansicxx_works}" = "xno"; then
      CXX=no
    else
      dnl# Test for namespaces.
      dnl# We do NOT bother supporting pre-ANSI-C++ compilers here:
      AC_MSG_CHECKING([whether the C++ compiler supports namespaces])
      AC_LANG_PUSH([C++])
      cat <<EOF > conftest.${ac_ext}
#include <iostream>
namespace test { using namespace std; }
std::ostream* ptr;
int main () { return 0; }
EOF
      if AC_TRY_EVAL([ac_link]) && test -s conftest${ac_exeext}; then
        gl_cv_prog_ansicxx_namespaces=yes
      else
        gl_cv_prog_ansicxx_namespaces=no
      fi
      rm -fr conftest*
      AC_LANG_POP([C++])
      AC_MSG_RESULT([${gl_cv_prog_ansicxx_namespaces}])
      if test "x${gl_cv_prog_ansicxx_namespaces}" = "xno"; then
        CXX=no
      fi
    fi
  fi
  m4_if([$1],[CXX],[],
    [$1="${CXX}"
     CXX="${gl_save_CXX}"])
  AC_SUBST([$1])dnl

  ## I wish I could ignore all this automake stuff, but...
  AM_CONDITIONAL([$2],[test "$$1" != "no"])dnl

  ## Hack:
  if test "$$1" != "no"; then
    dnl# This macro invocation resolves an automake error:
    dnl# /usr/local/share/automake-1.11/am/depend2.am: am__fastdepCXX does not appear in AM_CONDITIONAL
    dnl# /usr/local/share/automake-1.11/am/depend2.am:   The usual way to define 'am__fastdepCXX' is to add 'AC_PROG_CXX'
    dnl# /usr/local/share/automake-1.11/am/depend2.am:   to 'configure.ac' and run 'aclocal' and 'autoconf' again.
    _AM_DEPENDENCIES([CXX])
  else
    AM_CONDITIONAL([am__fastdepCXX],[false])
  fi
])dnl
