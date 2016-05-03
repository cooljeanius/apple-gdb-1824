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
AUTOMAKE_OPTIONS = 1.11.6 foreign no-dist
## "cygnus" == "foreign no-dist"
AUTOMAKE_DEPRECATED_OPTIONS = cygnus
upsrcdir = ${srcdir}/..
upupsrcdir = ${upsrcdir}/..
AM_CPPFLAGS = -I. -I.. -I${upsrcdir} -I${upupsrcdir} -I${upsrcdir}/common \
-I${upsrcdir}/config -I${upsrcdir}/macosx -I${upsrcdir}/signals \
-I${upupsrcdir}/config -I${upupsrcdir}/include -I${upupsrcdir}/bfd \
-I${upupsrcdir}/intl -I${upsrcdir}/attic
DEFS = @DEFS@ @EXTRA_DEFS@
C_ARCHFLAGS = -arch $(HOST_ARCHITECTURE)
AM_CFLAGS = $(C_ARCHFLAGS) -ggdb -fno-strict-aliasing -std=gnu99 -Wall \
-Wextra -Wold-style-definition -Wwrite-strings -Wformat=2 \
-Wmissing-include-dirs @MAINT@ -Wstrict-prototypes -Wnested-externs
## put these in an unused variable, just to record what I tried:
AM_EXTRA_WARN_CFLAGS = -Wmissing-format-attribute -Wc++-compat \
-Wmissing-parameter-type -Wundef -Wshadow -Wold-style-declaration \
-Wc99-c11-compat -pedantic -Wfloat-conversion -Wsign-conversion \
-Wmisleading-indentation
REMOTE_CFLAGS_COMBINED = ${AM_CPPFLAGS} ${DEFS} ${AM_CFLAGS}
AM_LDFLAGS = -L. -L.. $(C_ARCHFLAGS)
CC = @CC@

## extend the verbosity control:
AM_V_v = $(am__v_v_@AM_V@)
am__v_v_ = $(am__v_v_@AM_DEFAULT_V@)
am__v_v_0 =
am__v_v_1 = -v

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

prog: $(SERVER) @MAINT@ Remote_Makefile
	@if test ! -e serve || test ! -x serve; then \
	  $(CCLD) -ggdb -o serve $(SERVER) ${AM_LDFLAGS}; \
	else \
	  echo "$@ (i.e. 'serve') is already good to go."; \
	fi
.PHONY: prog

pdf-local:
.PHONY: pdf-local

clean-local:
	rm -f $(AM_V_v) $(SERVER) serve prog
.PHONY: clean-local

DISTCLEANFILES = autoscan.log @MAINT@ .DS_Store *~

distclean-local: clean-local
	rm -rf $(AM_V_v) autom4te.cache || rmdir autom4te.cache
	-rm -rf $(AM_V_v) *.dSYM || rmdir *.dSYM
	-rm -f config.h.in~
.PHONY: distclean-local

EXTRA_DIST = Remote_Makefile

dist-hook:
	@test -d .. && echo "avoid making distributions in subdirs" >&2
	@exit 1 || false
.PHONY: dist-hook
