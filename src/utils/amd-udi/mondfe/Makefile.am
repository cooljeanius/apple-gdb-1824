#
# Makefile for utils/amd-udi/mondfe
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
# 	$Id: Makefile.in,v 1.3 1995/09/20 17:47:38 ian Exp $
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

AM_CFLAGS = -ggdb -Wall -Wno-unused -Wno-format

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

OBJS = main.o monitor.o error.o help.o commands.o asm.o \
	opcodes.o regs.o bkpt.o dasm.o fill.o move.o \
	set.o xcmd.o yank.o dump.o getdata.o io.o icmd.o mini2udi.o

CLEANFILES = $(OBJS) mondfe

DISTCLEANFILES = Makefile Makefile*orig *.status

LIBDFE = ../udi/libdfe.a

LIBIBERTY = ../../../libiberty/libiberty.a

INCLUDE = -I$(srcdir)/../udi -I$(srcdir)/../include -I$(srcdir)/..

.c.o:
	$(CC) $(INCLUDE) $(DEFS) $(CFLAGS) $(AM_CFLAGS) -c $<

all:	mondfe

just_objs: $(OBJS)
.PHONY: just_objs

mondfe: $(LIBUDIDFE) $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LIBDFE) $(LIBIBERTY)

install_mondfe: all
	@true
.PHONY: install_mondfe

mostlyclean-local clean-local:
	$(RM) -f *.o mondfe
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
