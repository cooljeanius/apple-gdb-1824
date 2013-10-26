# Makefile for directory with message catalog handling in GNU NLS Utilities.
# Copyright (C) 1995, 1996, 1997, 1998 Free Software Foundation, Inc.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA 02110-1301, USA.

ACLOCAL_AMFLAGS = -I m4 --install

SUBDIRS = . po

PACKAGE = @PACKAGE@
VERSION = @VERSION@

SHELL = /bin/sh

srcdir = @srcdir@
top_srcdir = @top_srcdir@
top_builddir = .
VPATH = @srcdir@

prefix = @prefix@
exec_prefix = @exec_prefix@
transform = @program_transform_name@
libdir = $(exec_prefix)/lib
includedir = $(prefix)/include
datadir = $(prefix)/@DATADIRNAME@
localedir = $(datadir)/locale
gnulocaledir = $(datadir)/locale
gettextsrcdir = @datadir@/gettext/intl
aliaspath = $(localedir):.
subdir = intl

INSTALL = @INSTALL@
INSTALLdir = @INSTALLdir@
INSTALL_DATA = @INSTALL_DATA@
MKINSTALLDIRS = @MKINSTALLDIRS@

l = @l@

AR = @AR@
CC = @CC@
LIBTOOL = @LIBTOOL@
RANLIB = @RANLIB@

DEFS = -DLOCALEDIR=\"$(localedir)\" -DGNULOCALEDIR=\"$(gnulocaledir)\" \
-DLOCALE_ALIAS_PATH=\"$(aliaspath)\" @DEFS@
CPPFLAGS = @CPPFLAGS@
CFLAGS = @CFLAGS@
LDFLAGS = @LDFLAGS@

AM_CPPFLAGS = -I$(srcdir)
AM_WARNFLAGS = -Wall
AM_LDFLAGS = -framework CoreFoundation

COMPILE = $(CC) -c $(DEFS) $(INCLUDES) $(AM_WARNFLAGS) $(CPPFLAGS) $(CFLAGS) $(XCFLAGS) # $(AM_CPPFLAGS)
.PHONY: -c -m 644 
# The above `.PHONY:` is a hack; `-c` and `-m` and `644` should not even
# have been treated as targets in the first place...

include_HEADERS = $(COMHDRS) libgettext.h loadinfo.h
COMHDRS = gettext.h gettextP.h hash-string.h
SOURCES = $(COMSRCS) intl-compat.c cat-compat.c
COMSRCS = bindtextdom.c dcgettext.c dgettext.c gettext.c \
finddomain.c loadmsgcat.c localealias.c textdomain.c l10nflist.c \
explodename.c
OBJECTS = @INTLOBJS@ bindtextdom.o dcgettext.o dgettext.o gettext.o \
finddomain.o loadmsgcat.o localealias.o textdomain.o l10nflist.o \
explodename.o
LTOBJECTS = @INTLOBJS@ bindtextdom.lo dcgettext.lo dgettext.lo gettext.lo \
finddomain.lo loadmsgcat.lo localealias.lo textdomain.lo l10nflist.lo \
explodename.lo
CATOBJS = cat-compat.o ../po/cat-id-tbl.o
LTCATOBJS = cat-compat.lo ../po/cat-id-tbl.lo
GETTOBJS = intl-compat.o
GETTLTOBJS = intl-compat.lo
DISTFILES.common = ChangeLog Makefile.in linux-msg.sed po2tbl.sed.in \
xopen-msg.sed $(HEADERS) $(SOURCES)
DISTFILES.normal = VERSION
DISTFILES.gettext = libintl.glibc intlh.inst.in

SUFFIXES = .c .o .lo
.c.o:
	$(COMPILE) $<
.c.lo:
	$(LIBTOOL) --mode=compile $(COMPILE) $<

all: config.status all-@USE_INCLUDED_LIBINTL@
.PHONY: all-@USE_INCLUDED_LIBINTL@

all-yes: libintl.la intlh.inst libintl.a config.status
	@test -e Makefile.orig && make -f Makefile.orig
all-no: config.status
	@test -e Makefile.orig && make -f Makefile.orig
install-info-local:

libintl.a: $(OBJECTS) $(LTOBJECTS)
	rm -f $@
	$(AR) cru $@ $(OBJECTS)
	$(RANLIB) $@

libintl.la: $(LTOBJECTS) $(OBJECTS)
	$(LIBTOOL) --mode=link $(CC) $(LDFLAGS) $(AM_LDFLAGS) -o $@ $(LTOBJECTS) \
		   -version-info 1:0 -rpath $(libdir)

../po/cat-id-tbl.lo: ../po/cat-id-tbl.c $(top_srcdir)/po/$(PACKAGE).pot
	cd ../po && $(MAKE) cat-id-tbl.lo

../po/cat-id-tbl.o: ../po/cat-id-tbl.c $(top_srcdir)/po/$(PACKAGE).pot
	cd ../po && $(MAKE) cat-id-tbl.o

check: all config.status

# This installation goal is only used in GNU gettext. Packages which
# only use the library should use install instead.

# We must not install the libintl.h/libintl.a files if we are on a
# system which has the gettext() function in its C library or in a
# separate library or use the catgets interface.  A special case is
# where configure found a previously installed GNU gettext library.
# If you want to use the one which comes with this version of the
# package, you have to use `configure --with-included-gettext'.
install-libintl: install-exec-local install-data-local
install-exec-local: all config.status
	if test "$(PACKAGE)" = "gettext" \
	   && test '@INTLOBJS@' = '$(GETTOBJS)'; then \
	  if test -r $(MKINSTALLDIRS); then \
	    $(MKINSTALLDIRS) $(libdir) $(includedir); \
	  else \
	    $(top_srcdir)/mkinstalldirs $(libdir) $(includedir); \
	  fi; \
	  $(INSTALL_DATA) intlh.inst $(includedir)/libintl.h; \
	  $(INSTALL_DATA) libintl.a $(libdir)/libintl.a; \
	else \
	  : ; \
	fi
install-data-local: all config.status
	if test "$(PACKAGE)" = "gettext"; then \
	  if test -r $(MKINSTALLDIRS); then \
	    $(MKINSTALLDIRS) $(gettextsrcdir); \
	  else \
	    $(top_srcdir)/mkinstalldirs $(gettextsrcdir); \
	  fi; \
	  $(INSTALL_DATA) VERSION $(gettextsrcdir)/VERSION; \
	  dists="$(DISTFILES.common)"; \
	  for file in $$dists; do \
	    $(INSTALL_DATA) $(srcdir)/$$file $(gettextsrcdir)/$$file; \
	  done; \
	else \
	  : ; \
	fi

# Define this as empty until I found a useful application.
installcheck-local:

uninstall-local:
	dists="$(DISTFILES.common)"; \
	for file in $$dists; do \
	  rm -f $(gettextsrcdir)/$$file; \
	done

info dvi:

$(OBJECTS): config.h libgettext.h
$(LTOBJECTS): config.h libgettext.h

bindtextdom.o finddomain.o loadmsgcat.o: gettextP.h gettext.h loadinfo.h
bindtextdom.lo finddomain.lo loadmsgcat.lo: gettextP.h gettext.h loadinfo.h

dcgettext.o: gettextP.h gettext.h hash-string.h loadinfo.h
dcgettext.lo: gettextP.h gettext.h hash-string.h loadinfo.h

tags-lowercase: TAGS-uppercase

TAGS-uppercase: $(HEADERS) $(SOURCES)
	here=`pwd`; cd $(srcdir) && etags -o $$here/TAGS $(HEADERS) $(SOURCES)

id: ID-uppercase

ID-uppercase: $(HEADERS) $(SOURCES)
	here=`pwd`; cd $(srcdir) && mkid -f$$here/ID $(HEADERS) $(SOURCES)

mostlyclean-local: config.status
	rm -f *.a *.o *.lo core core.*

clean-local: mostlyclean-local config.status

distclean-local: clean-local config.status
	rm -f Makefile ID TAGS po2msg.sed po2tbl.sed libintl.h config.log

maintainer-clean-local: distclean-local config.status
	@echo "This command is intended for maintainers to use;"
	@echo "it deletes files that may require special tools to rebuild."


# GNU gettext needs not contain the file `VERSION' but contains some
# other files which should not be distributed in other packages.
mydistdir = ../$(PACKAGE)-$(VERSION)/$(subdir)
dist_mydistdir: Makefile $(DISTFILES)
	if test "$(PACKAGE)" = gettext; then \
	  additional="$(DISTFILES.gettext)"; \
	else \
	  additional="$(DISTFILES.normal)"; \
	fi; \
	for file in $(DISTFILES.common) $$additional; do \
	  ln $(srcdir)/$$file $(mydistdir) 2> /dev/null \
	    || cp -p $(srcdir)/$$file $(mydistdir); \
	done

dist-libc:
	tar zcvf intl-glibc.tar.gz $(COMSRCS) $(COMHDRS) libintl.h.glibc

Makefile-original: Makefile.in config.status
	CONFIG_FILES=$@ CONFIG_HEADERS= $(SHELL) ./config.status

# The dependency for intlh.inst is different in gettext and all other
# packages. Because we cannot you GNU make features we have to solve
# the problem while rewriting Makefile.in.
@GT_YES@intlh.inst: intlh.inst.in ../config.status
@GT_YES@	cd .. \
@GT_YES@	&& CONFIG_FILES=$(subdir)/$@ CONFIG_HEADERS= \
@GT_YES@	  $(SHELL) ./config.status
@GT_NO@.PHONY: intlh.inst
@GT_NO@intlh.inst:

