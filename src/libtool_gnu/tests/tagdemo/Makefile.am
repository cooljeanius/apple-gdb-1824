## Makefile.am -- Process this file with automake to produce Makefile.in
##
##   Copyright (C) 2003, 2004, 2005, 2011 Free Software Foundation
##   Written by Gary V. Vaughan, 2003
##
##   This file is part of GNU Libtool.
##
## GNU Libtool is free software; you can redistribute it and/or
## modify it under the terms of the GNU General Public License as
## published by the Free Software Foundation; either version 2 of
## the License, or (at your option) any later version.
##
## GNU Libtool is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with GNU Libtool; see the file COPYING.  If not, a copy
## can be downloaded from  http://www.gnu.org/licenses/gpl.html,
## or obtained by writing to the Free Software Foundation, Inc.,
## 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
#####

AUTOMAKE_OPTIONS = 1.11.6 no-dependencies foreign
## cannot use '--install'; that will trash our local (newer) copies of
## libtool.m4 and friends:
ACLOCAL_AMFLAGS = -I ../../libltdl/m4
AM_CPPFLAGS = -I$(top_srcdir)/../..
DISTCLEANFILES = *~

noinst_LTLIBRARIES = libconv.la
lib_LTLIBRARIES = libfoo.la libbaz.la

libfoo_la_SOURCES = foo.cpp
libfoo_la_LDFLAGS = -no-undefined
libfoo_la_LIBADD = libconv.la $(LIBM)

# Test some of the ILD support when using tagged configurations.
libbaz_la_SOURCES = baz.cpp
libbaz_la_LDFLAGS = -no-undefined
libbaz_la_LIBADD = libfoo.la

# Test convenience libraries.
libconv_la_SOURCES = conv.cpp
libconv_la_LDFLAGS = -no-undefined

noinst_HEADERS = foo.h baz.h conv.h

bin_PROGRAMS = tagdemo

tagdemo_SOURCES = main.cpp
tagdemo_LDADD = libbaz.la libfoo.la

libtool: $(LIBTOOL_DEPS)
	$(SHELL) ./config.status --recheck

# Workaround a bug in Autoconf-2.61 and earlier that do NOT clean up
# file droppings left by many compilers:
distclean-local:
	ac_files="a.out.* a.exe.* a_out.exe.* b.out.* conftest.*"; \
	for ac_file in $$ac_files; do \
	  case $$ac_file in \
	    *.xcoff | *.tds | *.d | *.pdb | *.xSYM | *.bb | *.bbg | *.map | *.inf | *.dSYM ) rm -rf $$ac_file ;; \
	  esac; \
	done
