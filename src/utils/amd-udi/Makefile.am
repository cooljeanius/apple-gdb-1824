#
# Makefile for utils/amd-udi/mondfe and montip tools
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
# Written by Jeffrey Wheat (cassidy@cygnus.com)
#

## Automake-specific things:
all-local:
	@echo "TODO: find out what is supposed to be going on here (in `pwd`)."

CLEANFILES = *.o *~

DISTCLEANFILES = Makefile config.status stamp-h1 config.h *.log

SUBDIRS = udi mondfe montip

AUTOMAKE_OPTIONS = gnits dejagnu no-dist -Wno-obsolete

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
AR_FLAGS = rv

AM_CFLAGS = -ggdb -O1 -Wall -Wextra

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

SHELL_HARDCODED = /bin/sh

MY_MAKEINFO = makeinfo
TEXI2DVI = texi2dvi

MY_SUBDIRS = udi mondfe montip

## hacks to extend the verbosity control:
AM_V_v = $(am__v_v_@AM_V@)
am__v_v_ = $(am__v_v_@AM_DEFAULT_V@)
am__v_v_0 =
am__v_v_1 = -v

AM_V_MFLAG = $(am__v_MFLAG_@AM_V@)
am__v_MFLAG_ = $(am__v_MFLAG_@AM_DEFAULT_V@)
am__v_MFLAG_0 = -s
am__v_MFLAG_1 = -w

# These are roughly topologically sorted in order to make porting more
# streamlined.
FLAGS_TO_PASS = \
	"CC=$(CC)" \
	"CFLAGS=$(CFLAGS)" \
	"AR=$(AR)" \
	"RANLIB=$(RANLIB)" \
	"AR_FLAGS=$(AR_FLAGS)" \
	"LD=$(LD)" \
	"LDFLAGS=$(LDFLAGS)" \
	"AS=$(AS)" \
	"MV=$(MV)" \
	"RM=$(RM)" \
	"CROSS_CFLAGS=$(CROSS_CFLAGS)" \
	"TARGET_CFLAGS=$(TARGET_CFLAGS)" \
	"INCLUDES=$(INCLUDES)"

all_old:
	$(AM_V_at)for dir in ${SUBDIRS}; do \
	if [ -d $${dir} ]; then \
	    (rootme=`pwd`/; export rootme; \
	    rootsrc=`cd $(srcdir); pwd`/; export rootsrc ; \
	    cd $${dir}; $(MAKE) $(AM_V_MFLAG) $(FLAGS_TO_PASS)); \
	    fi; \
	done
.PHONY: all_old

install_amd-udi:
	$(AM_V_at)for dir in ${SUBDIRS}; do \
	    echo "doing $@ in $${dir}:"; \
	    if [ -d $${dir} ]; then \
		(cd $${dir}; $(MAKE) $(AM_V_MFLAG) install); \
		fi; \
	    done
.PHONY: install_amd-udi

clean-local mostlyclean-local:
	-rm -f $(AM_V_v) *~ core *.o a.out .DS_Store
	$(AM_V_at)for dir in ${SUBDIRS}; do \
	    echo "doing $@ in $${dir}:"; \
	    if [ -d $${dir} ]; then \
		(cd $${dir}; $(MAKE) $(AM_V_MFLAG) $@); \
	    elif [ -d $${dir}/autom4te.cache ]; then \
		rm -fr $(AM_V_v) $${dir}/autom4te.cache || rmdir $${dir}/autom4te.cache; \
	    fi; \
	done
.PHONY: clean-local mostlyclean-local

distclean-local maintainer-clean-local realclean-local: clean-local
	$(AM_V_at)for dir in ${SUBDIRS}; do \
	    echo "doing $@ in $${dir}:"; \
	    (cd $${dir}; $(MAKE) $(AM_V_MFLAG) $@); \
	done
	-rm -f $(AM_V_v) *~ core
	-rm -fr $(AM_V_v) autom4te.cache || rmdir autom4te.cache
	-rm -f Makefile config.status
.PHONY: distclean-local maintainer-clean-local realclean-local

.c.o:
	$(CC) -c $(CFLAGS) $(INCLUDE) $(CPPFLAGS) $(DEFS) $<

install-info-local: info

installcheck-local:

check: @MAINT@ all-local
.PHONY: check

rebuild_Makefile: Makefile.in configure.ac
	$(SHELL) ./config.status
.PHONY: rebuild_Makefile

rebuild_configure: configure.ac
	autoconf --force --warnings=all,no-obsolete
	-rm -fr $(AM_V_v) autom4te.cache || rmdir autom4te.cache
.PHONY: rebuild_configure

TAGS_e: $(SRCS)
	etags $(SRCS)
.PHONY: TAGS_e

