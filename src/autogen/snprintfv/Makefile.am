##  -*- Mode: Makefile -*-
## ---------------------------------------------------------------------
## Makefile.am -- process this file with automake to produce Makefile.in
## Copyright (C) 1998, 1999, 2000, 2002 Gary V. Vaughan
## Originally by Gary V. Vaughan, 1998
## This file is part of Snprintfv
##
## Snprintfv is free software; you can redistribute it and/or
## modify it under the terms of the GNU General Public License as
## published by the Free Software Foundation; either version 2 of the
## License, or (at your option) any later version.
##
## Snprintfv program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program.  If not, write to the Free Software
## Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
##
## As a special exception to the GNU General Public License, if you
## distribute this file as part of a program that also links with and
## uses the libopts library from AutoGen, you may include it under
## the same distribution terms used by the libopts library.

## Code:

AM_CPPFLAGS             = -I$(top_srcdir)
noinst_HEADERS          = mem.h filament.h stream.h printf.h compat.h
dist_noinst_DATA        = filament.stamp stream.stamp printf.stamp
noinst_LTLIBRARIES      = libsnprintfv.la
libsnprintfv_la_LDFLAGS = -no-undefined
CSRC = filament.c format.c printf.c mem.c stream.c custom.c

nodist_libsnprintfv_la_SOURCES = snv.c

# These files are the raw sources used to generate similarly named
# header files after extracting the prototypes from the sources
#
EXTRA_DIST = filament.in printf.in stream.in $(CSRC)

snv.c : $(CSRC)
	for f in $(CSRC) ; do echo "#include \"$$f\"" ; done > $@

.NOTPARALLEL:

# Makefile.am ends here
