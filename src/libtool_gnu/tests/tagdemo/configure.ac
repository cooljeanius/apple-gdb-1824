dnl# Process this file with autoconf to create configure. -*- autoconf -*-
#
#  Copyright (C) 2001, 2003, 2004, 2005, 2007 Free Software Foundation, Inc
#
#   This file is part of GNU Libtool.
#
# GNU Libtool is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of
# the License, or (at your option) any later version.
#
# GNU Libtool is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GNU Libtool; see the file COPYING.  If not, a copy
# can be downloaded from  http://www.gnu.org/licenses/gpl.html,
# or obtained by writing to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
####

AC_PREREQ([2.54])dnl


## ------------------------ ##
## Autoconf initialization. ##
## ------------------------ ##
AC_INIT([tagdemo],[1.0],[bug-libtool@gnu.org])
AC_CONFIG_SRCDIR([foo.cpp])
AC_CONFIG_AUX_DIR([../../libltdl/config])dnl

AC_CANONICAL_TARGET


## ------------------------ ##
## Automake Initialization. ##
## ------------------------ ##
AM_INIT_AUTOMAKE([1.11.6 foreign])
AM_MAINTAINER_MODE


## ---------------- ##
## Compiler checks. ##
## ---------------- ##
AC_PROG_CC
AC_PROG_CPP
AC_PROG_CC_C_O

AC_PROG_CXX
AC_PROG_CXXCPP

# Check for namespace support and new-style headers
AC_LANG_PUSH([C++])
AC_MSG_CHECKING([whether the compiler implements namespaces])
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM([[namespace A { namespace B { int i = 0; }}]],
		     [[using namespace A::B; return i;]])],
    [AC_MSG_RESULT([yes])
     AC_DEFINE([HAVE_NAMESPACES],[1],
	       [define if the compiler implements namespaces])],
    [AC_MSG_RESULT([no])])dnl

AC_MSG_CHECKING([whether the compiler has ISO C++ iostream])
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM([[#include <iostream>
#ifdef HAVE_NAMESPACES
using namespace std;
#endif /* HAVE_NAMESPACES */ ]],[[cout << "bingo\n"; return 0;]])],
    [AC_MSG_RESULT([yes])
     AC_DEFINE([HAVE_IOSTREAM],[1],
	       [define if the compiler has ISO C++ iostream])],
    [AC_MSG_RESULT([no])])dnl
AC_LANG_POP([C++])dnl


## ----------------------- ##
## Libtool initialization. ##
## ----------------------- ##

# Set the test language to C++.
AC_LANG([C++])dnl
LT_INIT([dlopen win32-dll])
LT_LANG([C++])dnl


## ------------------------------ ##
## Libraries required by tagdemo. ##
## ------------------------------ ##
LT_LIB_M


## -------- ##
## Outputs. ##
## -------- ##
AC_CONFIG_FILES([Makefile])dnl
AC_OUTPUT
