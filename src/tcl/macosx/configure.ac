#!/bin/bash -norc
dnl#	This file is an input file used by the GNU "autoconf" program to
dnl#	generate the file "configure", which is run during Tcl installation
dnl#	to configure the system for the local environment.
#
# RCS: @(#) $Id: configure.ac,v 1.1 2005/11/27 02:33:49 das Exp $

dnl#	Ensure that the config (auto)headers support is used, then just 
dnl#	include the configure sources from ../unix:

m4_include(../unix/aclocal.m4)
m4_define(SC_USE_CONFIG_HEADERS)
m4_include(../unix/configure.ac)

if test "${srcdir}" = "." -a -d "${srcdir}/build" -a ! -d "${srcdir}/build/Development.build/Tcl.build/Configure_Copied.build/.."; then
  mkdir -pv "${srcdir}/build/Development.build/Tcl.build/Configure_Copied.build/.."
fi

if test "${srcdir}" = "." -a -d "${srcdir}/build" -a ! -d "${srcdir}/build/Development.build/Tcl.build/Configure_Copied.build"; then
  mkdir -pv "${srcdir}/build/Development.build/Tcl.build/Configure_Copied.build"
fi

if test "${srcdir}" = "." -a -d "${srcdir}/build" -a ! -d "${srcdir}/build/Development.build/Tcl.build/Make_Copied.build/.."; then
  mkdir -pv "${srcdir}/build/Development.build/Tcl.build/Make_Copied.build/.."
fi

if test "${srcdir}" = "." -a -d "${srcdir}/build" -a ! -d "${srcdir}/build/Development.build/Tcl.build/Make_Copied.build"; then
  mkdir -pv "${srcdir}/build/Development.build/Tcl.build/Make_Copied.build"
fi
