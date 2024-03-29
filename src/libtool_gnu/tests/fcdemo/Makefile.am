## Makefile.am -- Process this file with automake to produce Makefile.in
##
##   Copyright (C) 2003, 2004, 2005 Free Software Foundation
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

lib_LTLIBRARIES = libfoo.la libmix.la libfoo2.la libfoo3.la

libfoo_la_SOURCES = foof.f90
libfoo_la_LDFLAGS = -no-undefined
libfoo_la_LIBADD = libfoo2.la

libfoo2_la_SOURCES = foof2.f90
libfoo2_la_LDFLAGS = -no-undefined

libfoo3_la_SOURCES = foof3.f90
libfoo3_la_LDFLAGS = -no-undefined

libmix_la_SOURCES = foof.f90 foof2.f90 fooc.c
libmix_la_LDFLAGS = -no-undefined

noinst_HEADERS = foo.h

bin_PROGRAMS = fprogram cprogram

fprogram_SOURCES = fprogram.f90
fprogram_LDADD = libfoo.la libfoo3.la

cprogram_SOURCES = cprogram.c
cprogram_LDADD = libmix.la $(FCLIBS)

libtool: $(LIBTOOL_DEPS)
	$(SHELL) ./config.status --recheck

# work around Automake bug not providing FCFLAGS_f90:
.f90.o:
	$(FCCOMPILE) -c -o $@ $(FCFLAGS_f90) $<

.f90.obj:
	$(FCCOMPILE) -c -o $@ $(FCFLAGS_f90) `$(CYGPATH_W) '$<'`

.f90.lo:
	$(LTFCCOMPILE) -c -o $@ $(FCFLAGS_f90) $<

# Workaround a bug in Autoconf-2.61 and earlier that do NOT clean up
# file droppings left by many compilers:
distclean-local:
	ac_files="a.out.* a.exe.* a_out.exe.* b.out.* conftest.*"; \
	for ac_file in $$ac_files; do \
	  case $$ac_file in \
	    *.xcoff | *.tds | *.d | *.pdb | *.xSYM | *.bb | *.bbg | *.map | *.inf | *.dSYM ) rm -rf $$ac_file ;; \
	  esac; \
	done
