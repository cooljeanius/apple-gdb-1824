# Makefile for directory with message catalog handling in GNU NLS Utilities
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
# Foundation, 51 Franklin Street - 5th Floor, Boston, MA 02110-1301, USA.

ACLOCAL_AMFLAGS = -I m4 --install

AUTOMAKE_OPTIONS = 1.11.6 gnits dejagnu -Wno-obsolete

SUBDIRS = . @MAINT@ po

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

## extend the automake verbosity control:
AM_V_v = $(am__v_v_@AM_V@)
am__v_v_ = $(am__v_v_@AM_DEFAULT_V@)
am__v_v_0 =
am__v_v_1 = -v

AM_V_MFLAG = $(am__v_MFLAG_@AM_V@)
am__v_MFLAG_ = $(am__v_MFLAG_@AM_DEFAULT_V@)
am__v_MFLAG_0 = @MAINT@ -s
am__v_MFLAG_1 = -w

## architecture flags:
C_ARCHFLAGS = @C_ARCHFLAGS@
XC_ARCHFLAGS = @XC_ARCHFLAGS@
## (the variables should be automatically subst-ed by autoconf/automake)

DEFS = -DLOCALEDIR=\"$(localedir)\" -DGNULOCALEDIR=\"$(gnulocaledir)\" \
-DLOCALE_ALIAS_PATH=\"$(aliaspath)\" -DLIBDIR="\"$(libdir)\"" -DINSTALLDIR="\"$(libdir)\"" @DEFS@
PICFLAG = @PICFLAG@
CPPFLAGS = @CPPFLAGS@
CFLAGS = @CFLAGS@ 
LIBCFLAGS = $(CFLAGS) $(C_ARCHFLAGS)
## FIXME: only Apple compilers handle multiple '-arch' flags correctly:
XCFLAGS = -DXCOMPILING @MAINT@ $(CFLAGS) # $(XC_ARCHFLAGS)
LDFLAGS = @LDFLAGS@

AM_CPPFLAGS = -Wp,-I. -Wp,-I$(srcdir)
AM_ANSIFICATION_FLAGS = -Wstrict-prototypes -Wmissing-prototypes -Wconversion -Wold-style-definition -Wshadow -Wundef -Wextra -Wno-unused @MAINT@ -ansi -pedantic -Wno-error @EXTRA_ANSIFICATION_FLAGS@
AM_COMPILE_DEBUGFLAGS = -ggdb @MAINT@ -save-temps -fverbose-asm
AM_WARNFLAGS = -Wall @MAINT@ $(AM_ANSIFICATION_FLAGS)
AM_DEBUG_CFLAGS = $(AM_WARNFLAGS) @MAINT@ $(AM_COMPILE_DEBUGFLAGS)
AM_LDFLAGS = -framework CoreFoundation
AM_YFLAGS = -d -t $(AM_V_v) @MAINT@@EXTRA_YFLAGS_SEP@ -k -Wall -Wyacc

COMPILE = $(CC) -c $(DEFS) $(AM_CPPFLAGS) $(AM_DEBUG_CFLAGS) $(CPPFLAGS) $(CFLAGS) $(PICFLAG) $(C_ARCHFLAGS) $(XCFLAGS) @MAINT@ $(INCLUDES)
.PHONY: -c -m 644 
## The above `.PHONY:` is a hack; `-c` and `-m` and `644` should not even
## have been treated as targets in the first place...

include_HEADERS = $(COMHDRS) gmo.h libgettext.h loadinfo.h plural-exp.h \
eval-plural.h localcharset.h relocatable.h libgnuintl.h
COMHDRS = gettext.h gettextP.h hash-string.h
SOURCES = $(COMSRCS) intl-compat.c cat-compat.c
COMSRCS = bindtextdom.c dcgettext.c dgettext.c gettext.c \
finddomain.c loadmsgcat.c localealias.c textdomain.c l10nflist.c \
explodename.c dcigettext.c dcngettext.c dngettext.c ngettext.c plural.y \
plural-exp.c localcharset.c relocatable.c localename.c log.c osdep.c
EXTRA_DIST = $(SOURCES)
OBJECTS = @INTLOBJS@ bindtextdom.o dcgettext.o dgettext.o gettext.o \
finddomain.o loadmsgcat.o localealias.o textdomain.o l10nflist.o \
explodename.o dcigettext.o dcngettext.o dngettext.o ngettext.o plural.o \
plural-exp.o localcharset.o relocatable.o localename.o log.o osdep.o \
$(GETTOBJS)
LTOBJECTS = @INTLOBJS@ bindtextdom.lo dcgettext.lo dgettext.lo gettext.lo \
finddomain.lo loadmsgcat.lo localealias.lo textdomain.lo l10nflist.lo \
explodename.lo dcigettext.lo dcngettext.lo dngettext.lo ngettext.lo \
plural.lo plural-exp.lo localcharset.lo relocatable.lo localename.lo \
log.lo osdep.lo $(GETTLTOBJS)
CATOBJS = cat-compat.o ../po/cat-id-tbl.o
LTCATOBJS = cat-compat.lo ../po/cat-id-tbl.lo
GETTOBJS = intl-compat.o
GETTLTOBJS = intl-compat.lo

DISTCLEANFILES = *.cache.bak autoscan.log

DISTFILES.common = ChangeLog Makefile.in linux-msg.sed po2tbl.sed.in \
xopen-msg.sed $(HEADERS) $(SOURCES)
DISTFILES.normal = VERSION
DISTFILES.gettext = libintl.glibc intlh.inst.in

SUFFIXES = .c .y .o .lo
.c.o:
	$(COMPILE) $<
.c.lo:
	$(LIBTOOL) --tag=CC --mode=compile $(COMPILE) $<

.y.c:
@ifGNUmake@@BISON3_YES@	echo '#define USE_BISON3' > $(patsubst %.c,%-config.h,$@)
@BISON3_YES@	sed 's,%pure_parser,,;s,^/\* BISON3 \(.*\) \*/$$,\1,' $< > $@.y
@BISON3_YES@	$(YACC) $(YFLAGS) $(AM_YFLAGS) --output $@.c $@.y
@BISON3_YES@	sed 's/\.c\.y"/.y"/' $@.c > $@
@BISON3_YES@	rm -f $@.c $@.y $@.h
@ifGNUmake@@BISON3_NO@	echo '/* #define USE_BISON3 */' > $(patsubst %.c,%-config.h,$@)
@BISON3_NO@	$(YACC) $(YFLAGS) --output $@ $<
	rm -f $*.h

EXTRA_DIST += plural.c

AM_INCLUDES = -Wp,-I. -Wp,-I$(srcdir)

## Leave the actual rules for this target empty, because if we put any here, that
## would be overriding the default:
all: all-@USE_INCLUDED_LIBINTL@ @MAINT@ config.status po/Makefile
.PHONY: all all-@USE_INCLUDED_LIBINTL@

all-local:
	@echo "Use included libintl? @USE_INCLUDED_LIBINTL@"
.PHONY: all-local

all-yes: libintl.la intlh.inst libintl.a libintl.h config.intl @MAINT@ config.status
	@test -e Makefile_orig && make -f Makefile_orig
all-no: @MAINT@ config.status
	@test -e Makefile_orig && make -f Makefile_orig
install-info-local:
.PHONY: install-info-local

libintl.h: @MAINT@ config.status
	./config.status libintl_h_compat

libintl.a: $(OBJECTS) $(LTOBJECTS)
	$(AM_V_at)rm -f $@
	$(AR) cru $@ $(OBJECTS)
	$(RANLIB) $@

libintl.la: $(LTOBJECTS) $(OBJECTS)
	$(LIBTOOL) --tag=CC --mode=link $(CC) $(LDFLAGS) $(AM_LDFLAGS) \
		   -o $@ $(LTOBJECTS) -version-info 1:0 -rpath $(libdir)

../po/cat-id-tbl.lo: ../po/cat-id-tbl.c $(top_srcdir)/po/$(PACKAGE).pot
	$(AM_V_at)cd ../po && $(MAKE) $(AM_V_MFLAG) cat-id-tbl.lo

../po/cat-id-tbl.o: ../po/cat-id-tbl.c $(top_srcdir)/po/$(PACKAGE).pot
	$(AM_V_at)cd ../po && $(MAKE) $(AM_V_MFLAG) cat-id-tbl.o

EXTRA_DIST += po/Makevars.in

check: all @MAINT@ config.status
	-(ls ./*.i || ls ./*.s || ls ./*.bc || ls)
.PHONY: check

check-local:
	if test -L libintl.h; then readlink libintl.h; \
	elif test -e libintl.h; then stat libintl.h; \
	else echo "libintl.h is missing"; fi

# This installation goal is only used in GNU gettext. Packages which
# only use the library should use install instead.

# We must not install the libintl.h/libintl.a files if we are on a
# system which has the gettext() function in its C library or in a
# separate library or use the catgets interface.  A special case is
# where configure found a previously installed GNU gettext library.
# If you want to use the one which comes with this version of the
# package, you have to use `configure --with-included-gettext'.
install-libintl: install-exec-local install-data-local
.PHONY: install-libintl
install-exec-local: all @MAINT@ config.status
	if test "x$(PACKAGE)" = "xgettext" \
	   && test 'x@INTLOBJS@' = 'x$(GETTOBJS)'; then \
	  if test -r $(MKINSTALLDIRS); then \
	    $(MKINSTALLDIRS) $(libdir) $(includedir); \
	  else \
	    $(top_srcdir)/mkinstalldirs $(libdir) $(includedir); \
	  fi; \
	  $(INSTALL_DATA) intlh.inst $(includedir)/libintl.h; \
	  $(INSTALL_DATA) libintl.a $(libdir)/libintl.a; \
	else \
	  echo "no extra exec things to install"; \
	fi
.PHONY: install-exec-local

install-data-local: all @MAINT@ config.status
	if test "x$(PACKAGE)" = "xgettext"; then \
	  if test -r $(MKINSTALLDIRS); then \
	    $(MKINSTALLDIRS) $(gettextsrcdir); \
	  else \
	    $(top_srcdir)/mkinstalldirs $(gettextsrcdir); \
	  fi; \
	  $(INSTALL_DATA) VERSION $(gettextsrcdir)/VERSION; \
	  dists="$(DISTFILES.common)"; \
	  for file in $${dists}; do \
	    $(INSTALL_DATA) $(srcdir)/$${file} $(gettextsrcdir)/$${file}; \
	  done; \
	else \
	  echo "no extra data to install"; \
	fi
.PHONY: install-data-local

install-strip-local: install
.PHONY: install-strip-local

# Define this as empty until I found a useful application.
installcheck-local:
.PHONY: installcheck-local

uninstall-local:
	dists="$(DISTFILES.common)"; \
	for file in $${dists}; do \
	  rm -f $(AM_V_v) $(gettextsrcdir)/$${file}; \
	done
.PHONY: uninstall-local

.PHONY: info dvi ps pdf html
.PHONY: install-info install-dvi install-ps install-pdf install-html

info dvi ps pdf html:
install-info install-dvi install-ps install-pdf install-html:

$(OBJECTS): config.h libgettext.h
$(LTOBJECTS): config.h libgettext.h

bindtextdom.o finddomain.o loadmsgcat.o: gettextP.h gettext.h loadinfo.h
bindtextdom.lo finddomain.lo loadmsgcat.lo: gettextP.h gettext.h loadinfo.h

dcgettext.o: gettextP.h gettext.h hash-string.h loadinfo.h
dcgettext.lo: gettextP.h gettext.h hash-string.h loadinfo.h

dcigettext.o: gettextP.h plural-exp.h hash-string.h libgnuintl.h \
  eval-plural.h
dcigettext.lo: gettextP.h plural-exp.h hash-string.h libgnuintl.h \
  eval-plural.h

dcngettext.o dngettext.o ngettext.o: gettextP.h libgnuintl.h
dcngettext.lo dngettext.lo ngettext.lo: gettextP.h libgnuintl.h

explodename.o l10nflist.o: loadinfo.h
explodename.lo l10nflist.lo: loadinfo.h

localcharset.o: localcharset.h
localcharset.lo: localcharset.h

localealias.o relocatable.o: relocatable.h
localealias.lo relocatable.lo: relocatable.h

plural-exp.o: plural-exp.h
plural-exp.lo: plural-exp.h

plural.c: plural.y @MAINT@@BISON3_NO@ Makefile
plural.lo: plural.c
	$(LIBTOOL) --tag=CC --mode=compile $(COMPILE) $<; \
	if [[ $$? -eq 1 ]]; then \
	  if test -d ../../.git && test -n "${GIT_BIN}" && test -x "${GIT_BIN}"; then \
	    if test -n "`${GIT_BIN} status -s plural.c | grep M`"; then \
	      echo "plural.c appears to have been changed; attempting to restore it..."; \
	      ${GIT_BIN} restore $<; \
	      $(LIBTOOL) --tag=CC --mode=compile $(COMPILE) $<; \
            else \
              echo "error: plural.c does NOT seem to have been changed, so therefore building it ought to succeed; dunno what happened here..." >&2; \
	      exit 1; \
	    fi; \
	  else \
	    echo "error: bison 3 may have been used to re-yacc plural.c; you may need to either restore the previous version, or try re-yaccing with bison 2 instead." >&2; \
	    exit 1; \
	  fi; \
	else \
	  echo "compilation returned code $$?"; \
	fi

tags-lowercase: TAGS-uppercase
.PHONY: tags-lowercase

TAGS-uppercase: $(HEADERS) $(SOURCES)
	here=`pwd`; cd $(srcdir) && etags -o $${here}/TAGS $(HEADERS) $(SOURCES)
.PHONY: TAGS-uppercase

id: ID-uppercase
.PHONY: id

ID-uppercase: $(HEADERS) $(SOURCES)
	here=`pwd`; cd $(srcdir) && \
		mkid -f$${here}/ID $(HEADERS) $(SOURCES)
.PHONY: ID-uppercase

mostlyclean-local: @MAINT@ config.status
	rm -f $(AM_V_v) *.a *.la *.o *.obj *.lo core.*
	-rm -f core
.PHONY: mostlyclean-local

clean-local: mostlyclean-local @MAINT@ config.status
	rm -f $(AM_V_v) *.s *.i *.bc
	-if test ! -e po/Makefile; then $(MAKE) $(AM_V_MFLAG) po/Makefile; fi
.PHONY: clean-local

distclean-local: clean-local @MAINT@ config.status
	rm -f po2msg.sed po2tbl.sed @MAINT@ po/Makevars
	if test -e config.status && test -x config.status; then \
	  if test -e po; then ./config.status po/Makevars; fi \
	fi
	rm -f config.log config.intl config.h
	-if test -L libintl.h && test "x${IN_MAINT_MODE}" = "xyes"; then \
	  if test -n "$(FILECMD)" && test -n "$(GREP)"; then \
	    if test -n "`$(FILECMD) libintl.h | $(GREP) broken`"; then \
	      (unlink libintl.h || readlink libintl.h || stat libintl.h); \
	    elif test -x "${GIT_BIN}" && test -n "`${GIT_BIN} status -s libintl.h`"; then \
	      echo "remember you can use ${GIT_BIN} to restore libintl.h if it gets deleted"; \
	    else \
	      echo "$@ in `pwd`: leaving libintl.h alone"; \
	    fi; \
	  elif test -x "`which sync`"; then \
	    sync && echo "take care of libintl.h yourself"; \
	  fi; \
	elif test -e libintl.h; then \
	  echo "$@: leaving libintl.h alone in `pwd`"; \
	fi
	rm -f Makefile ID TAGS @MAINT@ stamp-h1
	if test ! -d po && (test -z $(SUBDIRS) || (test "x$(SUBDIRS)" != "xpo" && test "x$(SUBDIRS)" != "x. po")); then \
	  test "x${IN_MAINT_MODE}" != "xyes" && rm -f $(AM_V_v) *.status; \
	else \
	  echo "SUBDIRS of `pwd` are $(SUBDIRS); leaving status files alone"; \
	fi
	rm -f $(AM_V_v) *.output Makefile*orig *~ m4/*~ po/*~
	rm -rf $(AM_V_v) autom4te.cache || rmdir autom4te.cache
	-rm -f .DS_Store
	-if test ! -e po/Makefile; then $(MAKE) $(AM_V_MFLAG) po/Makefile; fi
.PHONY: distclean-local

maintainer-clean-local: distclean-local @MAINT@ config.status
	@echo "This command is intended for maintainers to use;"
	@echo "it deletes files that may require special tools to rebuild."
	rm -f libintl.h
.PHONY: maintainer-clean-local

# GNU gettext needs not contain the file `VERSION' but contains some
# other files which should not be distributed in other packages.
mydistdir = ../$(PACKAGE)-$(VERSION)/$(subdir)
dist_mydistdir: Makefile $(DISTFILES)
	if test "x$(PACKAGE)" = "xgettext"; then \
	  additional="$(DISTFILES.gettext)"; \
	else \
	  additional="$(DISTFILES.normal)"; \
	fi; \
	for file in $(DISTFILES.common) $${additional}; do \
	  ln $(srcdir)/$${file} $(mydistdir) 2>/dev/null \
	    || cp -p $(AM_V_v) $(srcdir)/$${file} $(mydistdir); \
	done
.PHONY: dist_mydistdir

dist-libc:
	tar zcvf intl-glibc.tar.gz $(COMSRCS) $(COMHDRS) libintl.h.glibc
.PHONY: dist-libc

Makefile-original: Makefile.in config.status
	CONFIG_FILES=$@ CONFIG_HEADERS="" $(SHELL) ./config.status
.PHONY: Makefile-original

po/Makefile: po/Makefile.in.in po/Makevars $(top_builddir)/config.status @POMAKEFILEDEPS@
	$(AM_V_at)cd $(top_builddir) \
	  && @SHELL@ ./config.status $(subdir)/$@.in po-directories

# The dependency for intlh.inst is different in gettext and all other
# packages. Because we cannot you GNU make features we have to solve
# the problem while rewriting Makefile.in.
@GT_YES@intlh.inst: intlh.inst.in ../config.status
@GT_YES@	cd .. \
@GT_YES@	&& CONFIG_FILES=$(subdir)/$@ CONFIG_HEADERS="" \
@GT_YES@	  $(SHELL) ./config.status
@GT_NO@.PHONY: intlh.inst
@GT_NO@intlh.inst:

EXTRA_DIST += intlh.inst.in
