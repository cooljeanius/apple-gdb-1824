# Copyright (C) 1992 - 2001 Free Software Foundation, Inc.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 1, or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#

AUTOMAKE_OPTIONS = 1.11.6 dejagnu foreign

SUBDIRS = .

noinst_PROGRAMS = calc
calc_SOURCES = calc.c
calc_CFLAGS = $(CFLAGS) $(AM_CFLAGS) -Wall -Wextra -pedantic -Wno-unused

# renamed because the old variable overrode the default Automake one:
RUNTESTDEFAULTFLAGSLOCAL = --tool calc CALC=`pwd`/calc --srcdir $$srcdir/testsuite
AM_RUNTESTFLAGS = $(RUNTESTDEFAULTFLAGSLOCAL)

CLEANFILES = *~ *.o .DS_Store testsuite/.DS_Store

clean-local:
	-rm -rf *.dSYM || rmdir *.dSYM

distclean-local:
	-rm -rf autom4te.cache || rmdir autom4te.cache
