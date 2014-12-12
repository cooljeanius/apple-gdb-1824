dnl# Process this file with autoconf to create configure. -*- autoconf -*-
#
# Copyright (C) 2001, 2003, 2004, 2005, 2007 Free Software Foundation, Inc.
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
AC_INIT([mdemo2],[1.0],[bug-libtool@gnu.org])
AC_CONFIG_SRCDIR([main.c])
AC_CONFIG_AUX_DIR([../../libltdl/config])dnl


## ------------------------ ##
## Automake Initialization. ##
## ------------------------ ##
AM_INIT_AUTOMAKE([foreign])
AM_MAINTAINER_MODE


## ------------------ ##
## C compiler checks. ##
## ------------------ ##
AC_PROG_CC


## ----------------------- ##
## Libtool initialization. ##
## ----------------------- ##
LT_INIT([dlopen win32-dll])
AC_SUBST([LIBTOOL_DEPS])dnl

STATIC=""
test "X${enable_static}" = "Xyes" && STATIC="-static"
AC_SUBST([STATIC])dnl


## -------- ##
## Outputs. ##
## -------- ##
AC_CONFIG_FILES([Makefile])dnl
AC_OUTPUT
