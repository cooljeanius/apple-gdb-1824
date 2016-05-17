#
# Makefile for utils/amd-udi/udi libraries
#   Copyright (C) 1993 Free Software Foundation
#
# This file is free software; you can redistribute it and/or modify
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
#
# 	$Id: Makefile.in,v 1.3 1995/09/20 17:47:45 ian Exp $
#
# Written by Jeffrey Wheat (cassidy@cygnus.com)
#

## Automake-specific things:
AUTOMAKE_OPTIONS = foreign dejagnu no-dist


#### Start of system configuration section. ####

srcdir = @srcdir@
VPATH = @srcdir@

CC = @CC@

INSTALL_PROG = @INSTALL@
MY_INSTALL_OLD_DATA_OF_MINE = @INSTALL_DATA@
MY_INSTALL_PROGRAM = @INSTALL_PROGRAM@

DEFS = @DEFS@
LIBS = @LIBS@
RANLIB = @RANLIB@

AR = ar
AR_FLAGS = cq

AM_CFLAGS = -ggdb -Wall -Wextra -Wno-unused -Wno-format -Wimplicit

LD = ld
# Darwin linker (ld64) fails to recognize this flag, so conditionalize:
if LINKER_IS_COMPILER
AM_LDFLAGS = -g
endif

MV = mv
RM = rm

prefix = @prefix@
my_exec_prefix = $(prefix)

my_bindir = $(exec_prefix)/bin
my_libdir = $(exec_prefix)/lib

my_mandir = $(prefix)/man
man1dir = $(mandir)/man1
man2dir = $(mandir)/man2
man3dir = $(mandir)/man3
man4dir = $(mandir)/man4
man5dir = $(mandir)/man5
man6dir = $(mandir)/man6
man7dir = $(mandir)/man7
man8dir = $(mandir)/man8

MY_SHELL = /bin/sh

INSTALL_HARDCODED = install -c
INSTALL_DATA_HARDCODED = $(INSTALL)
INSTALL_PROGRAM_HARDCODED = $(INSTALL)

#### End of system configuration section. ####

INCLUDE = -I$(srcdir)/../udi -I$(srcdir)/../include -I$(srcdir)/..

LIBDFE_OBJS = udip2soc.o
LIBTIP_OBJS = soc2udip.o
SHARED_OBJS = udr.o

CLEANFILES = *.o lib*.a

DISTCLEANFILES = Makefile Makefile*orig *.status

all:	libtip.a libdfe.a

libdfe.a: $(SHARED_OBJS) $(LIBDFE_OBJS)
	$(AR) $(AR_FLAGS) $@ $(SHARED_OBJS) $(LIBDFE_OBJS)
	$(RANLIB) $@

libtip.a: $(SHARED_OBJS) $(LIBTIP_OBJS)
	@$(RM) -f $@
	$(AR) $(AR_FLAGS) $@ $(SHARED_OBJS) $(LIBTIP_OBJS)
	$(RANLIB) $@

.c.o:
	$(CC) $(INCLUDE) $(DEFS) $(CFLAGS) $(AM_CFLAGS) -c $<

install_udi: all
	@true
.PHONY: install_udi

mostlyclean-local clean-local:
	$(RM) -f *.o libdfe.a libtip.a
.PHONY: mostlyclean-local clean-local

distclean-local maintainer-clean-local realclean-local: clean-local
	$(RM) -f Makefile config.status
.PHONY: distclean-local maintainer-clean-local realclean-local

check installcheck:
	@true
.PHONY: check installcheck

info install-info clean-info dvi:
	@true
.PHONY: info install-info clean-info dvi
