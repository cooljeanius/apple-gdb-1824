## automake conversion of Remote_Makefile
#########################################
#    Makefile for the remote server for GDB, the GNU debugger.
#    Copyright (C) 1986, 1989 Free Software Foundation, Inc.
# 
# This file is part of GDB.
# 
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

ACLOCAL_AMFLAGS = -I../m4 --install
AM_CPPFLAGS = -I. -I.. -I${srcdir}/.. -I${srcdir}/../.. -I${srcdir}/../common -I${srcdir}/../config -I${srcdir}/../macosx -I${srcdir}/../signals -I${srcdir}/../../config -I${srcdir}/../../include -I${srcdir}/../../bfd -I${srcdir}/../../intl
DEFS = @DEFS@
AM_CFLAGS = -ggdb -fno-strict-aliasing -std=gnu99 -Wall -Wextra -Wold-style-definition -Wtrampolines -Wwrite-strings -Wformat=2 -Wmissing-include-dirs @MAINT@ -Wstrict-prototypes -Wnested-externs
REMOTE_CFLAGS_COMBINED = ${AM_CPPFLAGS} ${DEFS} ${AM_CFLAGS}
AM_LDFLAGS = -L. -L..
CC = @CC@

bin_PROGRAMS = serve

serve_SOURCES = remote_server.c \
		remote_inflow.c \
		remote_utils.c \
		remote_gutils.c

SERVER = remote_server.o \
	 remote_inflow.o \
	 remote_utils.o \
	 remote_gutils.o

all: prog
.PHONY: all

prog: $(SERVER) Remote_Makefile
	$(CC) -ggdb -o serve $(SERVER) ${AM_LDFLAGS}
.PHONY: prog

clean-local:
	rm -fv $(SERVER) serve prog
.PHONY: clean-local

DISTCLEANFILES = autoscan.log @MAINT@ *~

distclean-local: clean-local
	rm -rfv autom4te.cache || rmdir autom4te.cache
.PHONY: distclean-local

EXTRA_DIST = Remote_Makefile

dist-hook:
	@test -d .. && echo "avoid making distributions in subdirs" >&2
	@exit 1 || false
.PHONY: dist-hook
