## -*- text -*- ##
# Master Makefile for the GNU readline library.
# Copyright (C) 1994, 1998 Free Software Foundation, Inc.

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111 USA.
ACLOCAL_AMFLAGS = -I m4 --install

AUTOMAKE_OPTIONS = 1.11.6 dejagnu foreign std-options no-dist -Wno-obsolete

SUBDIRS = . doc shlib examples @MAINT@ examples/rlfe.dir

RL_LIBRARY_VERSION = @LIBVERSION@
RL_LIBRARY_NAME = readline

srcdir = @srcdir@
VPATH = @srcdir@
top_srcdir = @top_srcdir@
BUILD_DIR = @BUILD_DIR@

INSTALL = @INSTALL@
INSTALL_PROGRAM = @INSTALL_PROGRAM@
INSTALL_DATA = @INSTALL_DATA@
INSTALLdir = @INSTALLdir@

CC = @CC@
RANLIB = @RANLIB@
AR = @AR@
ARFLAGS = @ARFLAGS@
RM = rm -f
CP = cp
MV = mv

@SET_MAKE@
SHELL = @MAKE_SHELL@

prefix = @prefix@
exec_prefix = @exec_prefix@

bindir = @bindir@
libdir = @libdir@
mandir = @mandir@
includedir = @includedir@

infodir = @infodir@

man3dir = $(mandir)/man3

# Support an alternate destination root directory for package building:
DESTDIR =

# Programs to make tags files.
my_ETAGS = etags -tw
my_CTAGS = ctags -tw

CFLAGS = @CFLAGS@
SHOBJ_CFLAGS = @SHOBJ_CFLAGS@

VERS_FLAG = -DRL_LIBRARY_VERSION='"$(RL_LIBRARY_VERSION)"'
LOCAL_CFLAGS = @LOCAL_CFLAGS@ $(VERS_FLAG)
AM_CFLAGS = $(LOCAL_CFLAGS)
CPPFLAGS = @CPPFLAGS@

DEFS = @DEFS@
LOCAL_DEFS = @LOCAL_DEFS@

XMALLOCOBJ = xmalloc.o

TERMCAP_LIB = @TERMCAP_LIB@

# For libraries which include headers from other libraries.
AM_CPPFLAGS = -I. -I$(srcdir)

## architecture flags:
C_ARCHFLAGS = @C_ARCHFLAGS@

# now combine all the flags:
XCCFLAGS = $(DEFS) $(LOCAL_DEFS) $(CPPFLAGS) $(INCLUDES) $(AM_CPPFLAGS)
JUST_CFLAGS = $(LOCAL_CFLAGS) $(CFLAGS) $(SHOBJ_CFLAGS)
CCFLAGS = $(XCCFLAGS) $(JUST_CFLAGS) $(C_ARCHFLAGS)

GCC_LINT_FLAGS = @GCC_LINT_FLAGS@
# could(?) add -Werror here
GCC_HARDCODED_LINT_FLAGS = -ansi -Wall -Wshadow -Wpointer-arith \
		 -Wcast-qual -Wwrite-strings -Wtype-limits \
		 -Wstrict-prototypes -Wmissing-prototypes -Wno-implicit \
		 -pedantic -Wold-style-definition -Wc++-compat \
		 -Wmissing-declarations
GCC_LINT_CFLAGS = $(XCCFLAGS) $(GCC_LINT_FLAGS) @CFLAGS@ @LOCAL_CFLAGS@

@ifGNUmake@ifneq (.@OBJEXT@,.o)
@ifGNUmake@.c.@OBJEXT@:
@ifGNUmake@	$(AM_V_at)${RM} $@
@ifGNUmake@	$(AM_V_CC)$(CC) -c $(CCFLAGS) $<
@ifGNUmake@endif

# The name of the main library target.
LIBRARY_NAME = libreadline.a
STATIC_LIBS = libreadline.a libhistory.a
noinst_LIBRARIES = libreadline_am.a libhistory_am.a
noinst_LTLIBRARIES = libreadline_lt.la libhistory_lt.la

WCWIDTH_OBJ = @WCWIDTH_OBJ@

# The C code source files for this library.
CSOURCES = $(srcdir)/readline.c $(srcdir)/funmap.c $(srcdir)/keymaps.c \
	   $(srcdir)/vi_mode.c $(srcdir)/parens.c $(srcdir)/rltty.c \
	   $(srcdir)/complete.c $(srcdir)/bind.c $(srcdir)/isearch.c \
	   $(srcdir)/display.c $(srcdir)/signals.c \
	   $(srcdir)/emacs_keymap.c $(srcdir)/vi_keymap.c \
	   $(srcdir)/util.c $(srcdir)/kill.c $(srcdir)/undo.c \
	   $(srcdir)/macro.c $(srcdir)/input.c $(srcdir)/callback.c \
	   $(srcdir)/terminal.c $(srcdir)/xmalloc.c $(srcdir)/history.c \
	   $(srcdir)/histsearch.c $(srcdir)/histexpand.c \
	   $(srcdir)/histfile.c $(srcdir)/nls.c $(srcdir)/search.c \
	   $(srcdir)/shell.c $(srcdir)/savestring.c $(srcdir)/tilde.c \
	   $(srcdir)/text.c $(srcdir)/misc.c $(srcdir)/compat.c \
	   $(srcdir)/realloc.c $(srcdir)/mbutil.c $(srcdir)/malloc.c \
	   $(srcdir)/support/wcwidth.c

libreadline_am_a_SOURCES = $(CSOURCES)
libreadline_am_a_CFLAGS = $(AM_CFLAGS) @MAINT@ $(GTOGGLE)
libreadline_am_a_CPPFLAGS = -I..

libreadline_lt_la_SOURCES = $(CSOURCES)
libreadline_lt_la_CFLAGS = $(AM_CFLAGS) @MAINT@ $(GTOGGLE)
libreadline_lt_la_CPPFLAGS = -I..

# The header files for this library.
HSOURCES = readline.h rldefs.h chardefs.h keymaps.h history.h histlib.h \
	   posixstat.h posixdir.h posixjmp.h tilde.h rlconf.h rltty.h \
	   ansi_stdlib.h tcap.h rlstdc.h xmalloc.h rlprivate.h rlshell.h \
	   rltypedefs.h rlmbutil.h

HISTSOURCES = history.c histexpand.c histfile.c histsearch.c \
	  malloc.c realloc.c shell.c mbutil.c
libhistory_am_a_SOURCES = $(HISTSOURCES)
libhistory_am_a_CFLAGS = $(AM_CFLAGS) @MAINT@ $(GTOGGLE)
libhistory_lt_la_SOURCES = $(HISTSOURCES)
libhistory_lt_la_CFLAGS = $(AM_CFLAGS) @MAINT@ $(GTOGGLE)
HISTOBJ = history.o histexpand.o histfile.o histsearch.o \
	  malloc.o realloc.o shell.o mbutil.o
TILDEOBJ = tilde.o
OBJECTS = readline.o vi_mode.o funmap.o keymaps.o parens.o search.o \
	  rltty.o complete.o bind.o isearch.o display.o signals.o \
	  util.o kill.o undo.o macro.o input.o callback.o terminal.o \
	  text.o nls.o misc.o compat.o \
	  $(XMALLOCOBJ) $(HISTOBJ) $(TILDEOBJ) $(WCWIDTH_OBJ)

# The texinfo files which document this library.
DOCSOURCE = doc/rlman.texinfo doc/rltech.texinfo doc/rluser.texinfo
DOCOBJECT = doc/readline.info
DOCSUPPORT = doc/Makefile
DOCUMENTATION = $(DOCSOURCE) $(DOCOBJECT) $(DOCSUPPORT)

CREATED_MAKEFILES = Makefile doc/Makefile examples/Makefile shlib/Makefile
CREATED_CONFIGURE = config.status config.h config.cache config.log \
		    stamp-config stamp-h
CREATED_TAGS = TAGS tags

INSTALLED_HEADERS = readline.h chardefs.h keymaps.h history.h tilde.h \
		    rlstdc.h rlconf.h rltypedefs.h
INSTALLEDdir = @INSTALLEDdir@

## extend the automake verbosity control:
AM_V_v = $(am__v_v_@AM_V@)
am__v_v_ = $(am__v_v_@AM_DEFAULT_V@)
am__v_v_0 =
am__v_v_1 = -v

AM_V_MFLAG = $(am__v_MFLAG_@AM_V@)
am__v_MFLAG_ = $(am__v_MFLAG_@AM_DEFAULT_V@)
am__v_MFLAG_0 = -s
am__v_MFLAG_1 = -w

##########################################################################
TARGETS = @STATIC_TARGET@ @SHARED_TARGET@
INSTALL_TARGETS = @STATIC_INSTALL_TARGET@ @SHARED_INSTALL_TARGET@

all: config.status $(TARGETS)

# Hack: should not be treated as targets in the first place
.PHONY: -c -m 644

everything: config.status all @MAINT@ examples

static: $(STATIC_LIBS)
	$(AM_V_at)for mylib in $(STATIC_LIBS) $(noinst_LIBRARIES); do \
	  if test -e $${mylib} && test $${mylib} -nt Makefile; then \
	    if test "x$(AM_V_P)" != "xfalse"; then \
	      stat $${mylib}; \
	    else \
	      test -r $${mylib}; \
	    fi; \
	  elif test -e Makefile; then \
	    echo "$@ target: using '$${CCFLAGS}' for CCFLAGS for $${mylib}"; \
	    $(MAKE) $(AM_V_MFLAG) $(MFLAGS) "CCFLAGS=$${CCFLAGS}" $${mylib}; \
	  else \
	    echo "unsure how to make $${mylib}"; \
	  fi; \
	done

libreadline.a: $(OBJECTS) $(HISTOBJ) $(TILDEOBJ) $(XMALLOCOBJ) ${LIBOBJDIR}compat$U.o ${LIBOBJDIR}malloc$U.o ${LIBOBJDIR}realloc$U.o
	$(AM_V_at)$(RM) $@
	$(AM_V_at)test -e ./Makefile && $(MAKE) $(AM_V_MFLAG) ./compat.o ./malloc.o ./realloc.o
	$(AM_V_at)if test -e $(srcdir)/compat.o -a ! -e compat.o; then \
	  cp $(AM_V_v) $(srcdir)/compat.o compat.o; \
	fi
	$(AM_V_at)if test -e $(srcdir)/malloc.o -a ! -e malloc.o; then \
	  cp $(AM_V_v) $(srcdir)/malloc.o malloc.o; \
	fi
	$(AM_V_at)if test -e $(srcdir)/realloc.o -a ! -e realloc.o; then \
	  cp $(AM_V_v) $(srcdir)/realloc.o realloc.o; \
	fi
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(OBJECTS)
	$(AM_V_at)-test -n "$(RANLIB)" && $(RANLIB) $@

libhistory.a: $(HISTOBJ) $(XMALLOCOBJ) ${LIBOBJDIR}malloc$U.o ${LIBOBJDIR}realloc$U.o @MAINT@ Makefile
	$(AM_V_at)$(RM) $@
	$(AM_V_at)test -e Makefile && test -w . && $(MAKE) $(AM_V_MFLAG) ./malloc.o ./realloc.o
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(HISTOBJ) $(XMALLOCOBJ)
	$(AM_V_at)-test -n "$(RANLIB)" && $(RANLIB) $@

wcwidth.o: $(srcdir)/support/wcwidth.c
	$(AM_V_at)$(RM) $@
	$(AM_V_CC)$(CC) $(CCFLAGS) -c $(srcdir)/support/wcwidth.c

# Since tilde.c is shared between readline and bash, make sure we compile
# it with the right flags when it is built as part of readline
tilde.o: tilde.c
	$(AM_V_at)rm -rf $@
	$(AM_V_CC)$(CC) $(CCFLAGS) -DREADLINE_LIBRARY -c $(srcdir)/tilde.c

readline: $(OBJECTS) readline.h rldefs.h chardefs.h examples/rl.c ./libreadline.a
	$(AM_V_CCLD)$(CC) $(CCFLAGS) -o $@ ./examples/rl.c ./libreadline.a \
	  ${TERMCAP_LIB}

lint lint-local: force
	export CFLAGS='$(GCC_LINT_CFLAGS)'; \
	export CCFLAGS='$(GCC_LINT_CFLAGS)'; \
	$(MAKE) $(AM_V_MFLAG) $(MFLAGS) CCFLAGS='$(GCC_LINT_CFLAGS)' \
	  CFLAGS='$(GCC_LINT_CFLAGS)' static
.PHONY: lint lint-local

am_Makefile am_makefile: config.status $(srcdir)/Makefile.in
	CONFIG_FILES=Makefile CONFIG_HEADERS="" $(SHELL) ./config.status
.PHONY: am_Makefile am_makefile

doc/Makefile: config.status $(srcdir)/Makefile.in
	CONFIG_FILES=doc/Makefile CONFIG_HEADERS="" $(SHELL) ./config.status

examples/Makefile: config.status $(srcdir)/Makefile.in
	CONFIG_FILES=examples/Makefile CONFIG_HEADERS="" $(SHELL) ./config.status

Makefiles makefiles: config.status $(srcdir)/Makefile.in
	$(AM_V_at)for mf in $(CREATED_MAKEFILES); do \
	  CONFIG_FILES=$${mf} CONFIG_HEADERS="" $(SHELL) ./config.status; \
	done

am_config.status: configure config.status
	$(SHELL) ./config.status --recheck
.PHONY: am_config.status

am_config.h: config.status stamp-h
.PHONY: am_config.h

stamp-h: config.status $(srcdir)/config.h.in
	CONFIG_FILES="" CONFIG_HEADERS=config.h ./config.status
	echo "" > $@

shared: force shlib/Makefile
	-test -d shlib || mkdir shlib
	-( cd shlib; $(MAKE) $(AM_V_MFLAG) ${MFLAGS} all )

documentation: force doc/Makefile
	-test -d doc || mkdir doc
	-( cd doc && $(MAKE) $(AM_V_MFLAG) $(MFLAGS) )

examples: force examples/Makefile
	-test -d examples || mkdir examples
	-(cd examples && $(MAKE) $(AM_V_MFLAG) ${MFLAGS} all )

force:

install-headers: installdirs ${INSTALLED_HEADERS}
	for f in ${INSTALLED_HEADERS}; do \
	  $(INSTALL_DATA) $(srcdir)/$${f} $(DESTDIR)$(includedir)/readline; \
	done

uninstall-headers:
	-test -n "$(includedir)" && cd $(DESTDIR)$(includedir)/readline \
	  && ${RM} ${INSTALLED_HEADERS}

maybe-uninstall-headers: uninstall-headers

## CYGNUS LOCAL
## Do NOT mess with people's installed readline versions.
## This tries to install this version of readline over whatever
## version is already installed on the system (which could be a
## newer version). There is no real reason for us to install
## readline along with GDB. GDB links statically against readline,
## so it does NOT depend on us installing it on the system.

install-generic:
.PHONY: install-generic

install-static: installdirs-local $(STATIC_LIBS) install-headers
	-$(MV) $(DESTDIR)$(libdir)/libreadline.a $(DESTDIR)$(libdir)/libreadline.old
	$(INSTALL_DATA) libreadline.a $(DESTDIR)$(libdir)/libreadline.a
	-test -n "$(RANLIB)" && $(RANLIB) $(DESTDIR)$(libdir)/libreadline.a
	-$(MV) $(DESTDIR)$(libdir)/libhistory.a $(DESTDIR)$(libdir)/libhistory.old
	$(INSTALL_DATA) libhistory.a $(DESTDIR)$(libdir)/libhistory.a
	-test -n "$(RANLIB)" && $(RANLIB) $(DESTDIR)$(libdir)/libhistory.a
	-( if test -d doc ; then \
		cd doc && \
		${MAKE} ${MFLAGS} infodir=$(infodir) DESTDIR=${DESTDIR} install; \
	  fi )

installdirs-local: $(srcdir)/support/mkdirs
	-$(SHELL) $(srcdir)/support/mkdirs $(DESTDIR)$(includedir) \
		$(DESTDIR)$(includedir)/readline $(DESTDIR)$(libdir) \
		$(DESTDIR)$(infodir) $(DESTDIR)$(man3dir)
.PHONY: installdirs-local

uninstall-local: uninstall-headers
	-test -n "$(DESTDIR)$(libdir)" && cd $(DESTDIR)$(libdir) && \
		${RM} libreadline.a libreadline.old libhistory.a libhistory.old $(SHARED_LIBS)
	-( if test -d doc ; then \
	     cd doc && \
	     ${MAKE} ${MFLAGS} infodir=$(infodir) DESTDIR=${DESTDIR} $@; \
	  fi )
	-( cd shlib; ${MAKE} ${MFLAGS} DESTDIR=${DESTDIR} uninstall )

install-shared: installdirs install-headers shared
	-( cd shlib ; ${MAKE} ${MFLAGS} DESTDIR=${DESTDIR} install )

uninstall-shared: maybe-uninstall-headers
	-( cd shlib; ${MAKE} ${MFLAGS} DESTDIR=${DESTDIR} uninstall )

my_TAGS: force
	$(my_ETAGS) $(CSOURCES) $(HSOURCES)
.PHONY: my_TAGS

tags-local: force
	$(my_CTAGS) $(CSOURCES) $(HSOURCES)
.PHONY: tags-local

clean-local: config.status force
	$(RM) $(OBJECTS) $(STATIC_LIBS)
	$(RM) readline readline.exe
	-( if test -e shlib/Makefile; then cd shlib && $(MAKE) $(AM_V_MFLAG) $(MFLAGS) $@ ; fi )
	-( if test -e doc/Makefile; then cd doc && $(MAKE) $(AM_V_MFLAG) $(MFLAGS) $@ ; fi )
	-( if test -e examples/Makefile; then cd examples && $(MAKE) $(AM_V_MFLAG) $(MFLAGS) $@ ; fi )
	-( if test -e examples/rlfe.dir/Makefile; then cd examples/rlfe.dir && $(MAKE) $(AM_V_MFLAG) $(MFLAGS) $@ ; fi )

mostlyclean-local: clean-local config.status
	-( if test -e shlib/Makefile; then cd shlib && $(MAKE) $(AM_V_MFLAG) $(MFLAGS) $@ ; fi )
	-( if test -e doc/Makefile; then cd doc && $(MAKE) $(AM_V_MFLAG) $(MFLAGS) $@ ; fi )
	-( if test -e examples/Makefile; then cd examples && $(MAKE) $(AM_V_MFLAG) $(MFLAGS) $@ ; fi )
	-( if test -e examples/rlfe.dir/Makefile; then cd examples/rlfe.dir && $(MAKE) $(AM_V_MFLAG) $(MFLAGS) $@ ; fi )

distclean-local: clean-local config.status
	-( if test -e shlib/Makefile; then cd shlib && $(MAKE) $(AM_V_FLAG) $(MFLAGS) $@ ; fi )
	-( if test -e doc/Makefile; then cd doc && cp $(AM_V_v) Makefile Makefile.bak && $(MAKE) $(AM_V_FLAG) $(MFLAGS) $@ && mv $(AM_V_v) Makefile.bak Makefile; fi )
	-( if test -e examples/Makefile; then cd examples && $(MAKE) $(AM_V_FLAG) $(MFLAGS) $@ ; fi )
	-( if test -e examples/rlfe.dir/Makefile && test -x examples/rlfe.dir/config.status; then cd examples/rlfe.dir && cp $(AM_V_v) Makefile Makefile.bak && $(MAKE) $(AM_V_MFLAG) $(MFLAGS) $@ && mv $(AM_V_v) Makefile.bak Makefile; fi )
	$(RM) $(CREATED_TAGS)
	$(RM) -r $(AM_V_v) .deps/*.*o || rmdir .deps
	$(RM) -r autom4te.cache || rmdir autom4te.cache

maintainer-clean-local: distclean-local
	-( if test -e shlib/Makefile; then cd shlib && $(MAKE) $(AM_V_MFLAG) $(MFLAGS) $@ ; fi )
	-( if test -e doc/Makefile; then cd doc && $(MAKE) $(AM_V_MFLAG) $(MFLAGS) $@ ; fi )
	-( if test -e examples/Makefile; then cd examples && $(MAKE) $(AM_V_MFLAG) $(MFLAGS) $@ ; fi )
	-( if test -e examples/rlfe.dir/Makefile; then cd examples/rlfe.dir && $(MAKE) $(AM_V_MFLAG) $(MFLAGS) $@ ; fi )
	if test -e config.h; then $(RM) config.h; \
	elif test -e config.h.in; then $(RM) config.h.in; fi
	if test -e Makefile_orig; then $(RM) Makefile_orig; \
	elif test -e Makefile.in; then $(RM) Makefile.in; fi

CLEANFILES = *~ config/*~ m4/*~ support/*~
DISTCLEANFILES = .DS_Store stamp-h* config.cache

info dvi: doc/Makefile
	-( cd doc && $(MAKE) $(AM_V_MFLAG) $(MFLAGS) $@ )

install-info-local:
check: @MAINT@ lint
check-local: @MAINT@ lint-local
installcheck-local:

dist-local: force
	@echo "Readline distributions are created using $(srcdir)/support/mkdist."
	@echo "Here is a sample of the necessary commands:"
	@echo "bash $(srcdir)/support/mkdist -m $(srcdir)/MANIFEST -s $(srcdir) -r $(RL_LIBRARY_NAME) $(RL_LIBRARY_VERSION)"
	@echo "tar cf $(RL_LIBRARY_NAME)-${RL_LIBRARY_VERSION}.tar ${RL_LIBRARY_NAME}-$(RL_LIBRARY_VERSION)"
	@echo "gzip $(RL_LIBRARY_NAME)-$(RL_LIBRARY_VERSION).tar"

NSLIBTOOL = cc -dynamiclib
SYSTEM_FRAMEWORK = -framework System -lcurses

FRAMEWORK = readline
FRAMEWORK_PREFIX =
FRAMEWORK_SUFFIX =
FRAMEWORK_VERSION_SUFFIX =
FRAMEWORK_VERSION = A
FRAMEWORK_OFILES = $(OBJECTS)
BINUTILS_FRAMEWORKS = ../bfd
LIBIBERTY_FRAMEWORKS = ../libiberty
MMALLOC_FRAMEWORKS = ../mmalloc
FRAMEWORK_LIB = -F$(BINUTILS_FRAMEWORKS) -L$(BINUTILS_FRAMEWORKS) -F$(LIBIBERTY_FRAMEWORKS) -L$(LIBIBERTY_FRAMEWORKS) -F$(MMALLOC_FRAMEWORKS) -L$(MMALLOC_FRAMEWORKS) -framework liberty -framework mmalloc $(SYSTEM_FRAMEWORK)

stamp-framework-headers:
	$(AM_V_at)$(RM) -f stamp-framework stamp-framework-headers
	$(RM) -rf $(AM_V_v) $(FRAMEWORK).framework
	$(MKDIR_P) $(AM_V_v) $(FRAMEWORK).framework/Versions/$(FRAMEWORK_VERSION)/Headers
	$(MKDIR_P) $(AM_V_v) $(FRAMEWORK).framework/Versions/$(FRAMEWORK_VERSION)/PrivateHeaders
	$(LN_S) $(AM_V_v) Versions/Current/Headers $(FRAMEWORK).framework/Headers
	$(LN_S) $(AM_V_v) Versions/Current/PrivateHeaders $(FRAMEWORK).framework/PrivateHeaders
	$(LN_S) $(AM_V_v) A $(FRAMEWORK).framework/Versions/Current
	cp -p $(AM_V_v) $(srcdir)/*.h $(FRAMEWORK).framework/Versions/Current/Headers/
	cp -p $(AM_V_v) *.h $(FRAMEWORK).framework/Versions/Current/Headers/
	$(AM_V_at)touch stamp-framework-headers

stamp-framework: $(FRAMEWORK_OFILES)
	$(AM_V_at)$(RM) -f stamp-framework-headers
	$(MAKE) $(AM_V_MFLAG) stamp-framework-headers
	$(MKDIR_P) $(AM_V_v) $(FRAMEWORK).framework/Versions/$(FRAMEWORK_VERSION)/Resources
	$(LN_S) $(AM_V_v) Versions/Current/Resources $(FRAMEWORK).framework/Resources
	$(MKDIR_P) $(AM_V_v) $(FRAMEWORK).framework/Versions/Current/Resources/English.lproj
	set -ex; \
	if [ "x$(FRAMEWORK_SUFFIX)" != "x" ]; then \
	    $(NSLIBTOOL) -arch $(HOST_ARCHITECTURE) -compatibility_version 1 -current_version 1 -install_name /System/Library/PrivateFrameworks/$(FRAMEWORK).framework/Versions/$(FRAMEWORK_VERSION)/$(FRAMEWORK_PREFIX)$(FRAMEWORK)$(FRAMEWORK_VERSION_SUFFIX) -o $(FRAMEWORK).framework/Versions/Current/$(FRAMEWORK_PREFIX)$(FRAMEWORK)$(FRAMEWORK_VERSION_SUFFIX) $(FRAMEWORK_OFILES) $(FRAMEWORK_LIB); \
	    $(LN_S) $(AM_V_v) $(FRAMEWORK_PREFIX)$(FRAMEWORK)$(FRAMEWORK_VERSION_SUFFIX) $(FRAMEWORK).framework/Versions/Current/$(FRAMEWORK_PREFIX)$(FRAMEWORK)$(FRAMEWORK_SUFFIX); \
	    $(LN_S) $(AM_V_v) Versions/Current/$(FRAMEWORK_PREFIX)$(FRAMEWORK)$(FRAMEWORK_VERSION_SUFFIX) $(FRAMEWORK).framework/$(FRAMEWORK_PREFIX)$(FRAMEWORK)$(FRAMEWORK_VERSION_SUFFIX); \
	    $(LN_S) $(AM_V_v) Versions/Current/$(FRAMEWORK_PREFIX)$(FRAMEWORK)$(FRAMEWORK_SUFFIX) $(FRAMEWORK).framework/$(FRAMEWORK_PREFIX)$(FRAMEWORK)$(FRAMEWORK_SUFFIX); \
	    $(LN_S) $(AM_V_v) $(FRAMEWORK_PREFIX)$(FRAMEWORK)$(FRAMEWORK_SUFFIX) $(FRAMEWORK).framework/$(FRAMEWORK); \
	else \
	    $(NSLIBTOOL) -arch $(HOST_ARCHITECTURE)  -compatibility_version 1 -current_version 1 -install_name /System/Library/PrivateFrameworks/$(FRAMEWORK).framework/Versions/$(FRAMEWORK_VERSION)/$(FRAMEWORK_PREFIX)$(FRAMEWORK) -o $(FRAMEWORK).framework/Versions/Current/$(FRAMEWORK_PREFIX)$(FRAMEWORK) $(FRAMEWORK_OFILES) $(FRAMEWORK_LIB); \
	    $(LN_S) $(AM_V_v) Versions/Current/$(FRAMEWORK_PREFIX)$(FRAMEWORK) $(FRAMEWORK).framework/$(FRAMEWORK_PREFIX)$(FRAMEWORK); \
	fi
	$(AM_V_at)touch stamp-framework

# Dependencies:
bind.o: ansi_stdlib.h posixstat.h
bind.o: rldefs.h ${BUILD_DIR}/config.h rlconf.h
bind.o: readline.h keymaps.h rltypedefs.h chardefs.h tilde.h rlstdc.h
bind.o: history.h
callback.o: rlconf.h
callback.o: rldefs.h ${BUILD_DIR}/config.h rlconf.h
callback.o: readline.h keymaps.h rltypedefs.h chardefs.h tilde.h rlstdc.h
compat.o: rlstdc.h
complete.o: ansi_stdlib.h posixdir.h posixstat.h
complete.o: rldefs.h ${BUILD_DIR}/config.h rlconf.h
complete.o: readline.h keymaps.h rltypedefs.h chardefs.h tilde.h rlstdc.h
display.o: ansi_stdlib.h posixstat.h
display.o: rldefs.h ${BUILD_DIR}/config.h rlconf.h
display.o: tcap.h
display.o: readline.h keymaps.h rltypedefs.h chardefs.h tilde.h
display.o: history.h rlstdc.h
funmap.o: readline.h keymaps.h rltypedefs.h chardefs.h tilde.h
funmap.o: rlconf.h ansi_stdlib.h rlstdc.h
funmap.o: ${BUILD_DIR}/config.h
histexpand.o: ansi_stdlib.h
histexpand.o: history.h histlib.h rlstdc.h rltypedefs.h
histexpand.o: ${BUILD_DIR}/config.h
histfile.o: ansi_stdlib.h
histfile.o: history.h histlib.h rlstdc.h rltypedefs.h
histfile.o: ${BUILD_DIR}/config.h
history.o: ansi_stdlib.h
history.o: history.h histlib.h rlstdc.h rltypedefs.h
history.o: ${BUILD_DIR}/config.h
histsearch.o: ansi_stdlib.h
histsearch.o: history.h histlib.h rlstdc.h rltypedefs.h
histsearch.o: ${BUILD_DIR}/config.h
input.o: ansi_stdlib.h
input.o: rldefs.h ${BUILD_DIR}/config.h rlconf.h
input.o: readline.h keymaps.h rltypedefs.h chardefs.h tilde.h rlstdc.h
isearch.o: rldefs.h ${BUILD_DIR}/config.h rlconf.h
isearch.o: readline.h keymaps.h rltypedefs.h chardefs.h tilde.h
isearch.o: ansi_stdlib.h history.h rlstdc.h
keymaps.o: emacs_keymap.c vi_keymap.c
keymaps.o: keymaps.h rltypedefs.h chardefs.h rlconf.h ansi_stdlib.h
keymaps.o: readline.h keymaps.h rltypedefs.h chardefs.h tilde.h
keymaps.o: ${BUILD_DIR}/config.h rlstdc.h
kill.o: ansi_stdlib.h
kill.o: rldefs.h ${BUILD_DIR}/config.h rlconf.h
kill.o: readline.h keymaps.h rltypedefs.h chardefs.h tilde.h
kill.o: history.h rlstdc.h
macro.o: ansi_stdlib.h
macro.o: rldefs.h ${BUILD_DIR}/config.h rlconf.h
macro.o: readline.h keymaps.h rltypedefs.h chardefs.h tilde.h
macro.o: history.h rlstdc.h
mbutil.o: rldefs.h ${BUILD_DIR}/config.h rlconf.h
mbutil.o: readline.h keymaps.h rltypedefs.h chardefs.h rlstdc.h
misc.o: readline.h keymaps.h rltypedefs.h chardefs.h tilde.h
misc.o: rldefs.h ${BUILD_DIR}/config.h rlconf.h
misc.o: history.h rlstdc.h ansi_stdlib.h
nls.o: ansi_stdlib.h
nls.o: rldefs.h ${BUILD_DIR}/config.h rlconf.h
nls.o: readline.h keymaps.h rltypedefs.h chardefs.h tilde.h  
nls.o: history.h rlstdc.h  
parens.o: rlconf.h
parens.o: ${BUILD_DIR}/config.h
parens.o: readline.h keymaps.h rltypedefs.h chardefs.h tilde.h rlstdc.h
readline.o: readline.h keymaps.h rltypedefs.h chardefs.h tilde.h
readline.o: rldefs.h ${BUILD_DIR}/config.h rlconf.h
readline.o: history.h rlstdc.h
readline.o: posixstat.h ansi_stdlib.h posixjmp.h
rltty.o: rldefs.h ${BUILD_DIR}/config.h rlconf.h
rltty.o: rltty.h
rltty.o: readline.h keymaps.h rltypedefs.h chardefs.h tilde.h rlstdc.h
search.o: rldefs.h ${BUILD_DIR}/config.h rlconf.h
search.o: readline.h keymaps.h rltypedefs.h chardefs.h tilde.h
search.o: ansi_stdlib.h history.h rlstdc.h
shell.o: ${BUILD_DIR}/config.h
shell.o: ansi_stdlib.h
signals.o: rldefs.h ${BUILD_DIR}/config.h rlconf.h
signals.o: readline.h keymaps.h rltypedefs.h chardefs.h tilde.h
signals.o: history.h rlstdc.h
terminal.o: rldefs.h ${BUILD_DIR}/config.h rlconf.h
terminal.o: tcap.h
terminal.o: readline.h keymaps.h rltypedefs.h chardefs.h tilde.h
terminal.o: history.h rlstdc.h
text.o: readline.h keymaps.h rltypedefs.h chardefs.h tilde.h
text.o: rldefs.h ${BUILD_DIR}/config.h rlconf.h
text.o: history.h rlstdc.h ansi_stdlib.h
tilde.o: ansi_stdlib.h
tilde.o: ${BUILD_DIR}/config.h
tilde.o: tilde.h
undo.o: ansi_stdlib.h
undo.o: rldefs.h ${BUILD_DIR}/config.h rlconf.h
undo.o: readline.h keymaps.h rltypedefs.h chardefs.h tilde.h
undo.o: history.h rlstdc.h
util.o: posixjmp.h ansi_stdlib.h
util.o: rldefs.h ${BUILD_DIR}/config.h rlconf.h
util.o: readline.h keymaps.h rltypedefs.h chardefs.h tilde.h rlstdc.h
vi_mode.o: rldefs.h ${BUILD_DIR}/config.h rlconf.h
vi_mode.o: readline.h keymaps.h rltypedefs.h chardefs.h tilde.h
vi_mode.o: history.h ansi_stdlib.h rlstdc.h
xmalloc.o: ${BUILD_DIR}/config.h
xmalloc.o: ansi_stdlib.h

bind.o: rlshell.h
histfile.o: rlshell.h
nls.o: rlshell.h
readline.o: rlshell.h
shell.o: rlshell.h
terminal.o: rlshell.h
histexpand.o: rlshell.h

bind.o: rlprivate.h
callback.o: rlprivate.h
complete.o: rlprivate.h
display.o: rlprivate.h
input.o: rlprivate.h
isearch.o: rlprivate.h
kill.o: rlprivate.h
macro.o: rlprivate.h
mbutil.o: rlprivate.h
misc.o: rlprivate.h
nls.o: rlprivate.h   
parens.o: rlprivate.h
readline.o: rlprivate.h
rltty.o: rlprivate.h 
search.o: rlprivate.h
signals.o: rlprivate.h
terminal.o: rlprivate.h
text.o: rlprivate.h
undo.o: rlprivate.h
util.o: rlprivate.h
vi_mode.o: rlprivate.h

bind.o: xmalloc.h
complete.o: xmalloc.h
display.o: xmalloc.h
funmap.o: xmalloc.h
histexpand.o: xmalloc.h
histfile.o: xmalloc.h
history.o: xmalloc.h
input.o: xmalloc.h
isearch.o: xmalloc.h
keymaps.o: xmalloc.h
kill.o: xmalloc.h
macro.o: xmalloc.h
mbutil.o: xmalloc.h
misc.o: xmalloc.h
readline.o: xmalloc.h
savestring.o: xmalloc.h
search.o: xmalloc.h
shell.o: xmalloc.h
terminal.o: xmalloc.h
text.o: xmalloc.h
tilde.o: xmalloc.h
undo.o: xmalloc.h
util.o: xmalloc.h
vi_mode.o: xmalloc.h
xmalloc.o: xmalloc.h

complete.o: rlmbutil.h
display.o: rlmbutil.h
histexpand.o: rlmbutil.h
input.o: rlmbutil.h    
isearch.o: rlmbutil.h
mbutil.o: rlmbutil.h
misc.o: rlmbutil.h
readline.o: rlmbutil.h
search.o: rlmbutil.h 
text.o: rlmbutil.h
vi_mode.o: rlmbutil.h

bind.o: $(srcdir)/bind.c
callback.o: $(srcdir)/callback.c
compat.o: $(srcdir)/compat.c
complete.o: $(srcdir)/complete.c
display.o: $(srcdir)/display.c
funmap.o: $(srcdir)/funmap.c
input.o: $(srcdir)/input.c
isearch.o: $(srcdir)/isearch.c
keymaps.o: $(srcdir)/keymaps.c $(srcdir)/emacs_keymap.c $(srcdir)/vi_keymap.c
kill.o: $(srcdir)/kill.c
macro.o: $(srcdir)/macro.c
malloc.o: $(srcdir)/malloc.c
mbutil.o: $(srcdir)/mbutil.c
misc.o: $(srcdir)/misc.c
nls.o: $(srcdir)/nls.c
parens.o: $(srcdir)/parens.c
readline.o: $(srcdir)/readline.c
realloc.o: $(srcdir)/realloc.c
rltty.o: $(srcdir)/rltty.c
savestring.o: $(srcdir)/savestring.c
search.o: $(srcdir)/search.c
shell.o: $(srcdir)/shell.c
signals.o: $(srcdir)/signals.c
terminal.o: $(srcdir)/terminal.c
text.o: $(srcdir)/text.c
tilde.o: $(srcdir)/tilde.c
undo.o: $(srcdir)/undo.c
util.o: $(srcdir)/util.c
vi_mode.o: $(srcdir)/vi_mode.c
xmalloc.o: $(srcdir)/xmalloc.c

histexpand.o: $(srcdir)/histexpand.c
histfile.o: $(srcdir)/histfile.c
history.o: $(srcdir)/history.c
histsearch.o: $(srcdir)/histsearch.c

bind.o: bind.c
callback.o: callback.c
compat.o: compat.c
complete.o: complete.c
display.o: display.c
funmap.o: funmap.c
input.o: input.c
isearch.o: isearch.c
keymaps.o: keymaps.c emacs_keymap.c vi_keymap.c
kill.o: kill.c
macro.o: macro.c
malloc.o: malloc.c
mbutil.o: mbutil.c
misc.o: misc.c
nls.o: nls.c
parens.o: parens.c
readline.o: readline.c
realloc.o: realloc.c
rltty.o: rltty.c
savestring.o: savestring.c
search.o: search.c
shell.o: shell.c
signals.o: signals.c
terminal.o: terminal.c
text.o: text.c
tilde.o: tilde.c
undo.o: undo.c
util.o: util.c
vi_mode.o: vi_mode.c
xmalloc.o: xmalloc.c

histexpand.o: histexpand.c
histfile.o: histfile.c
history.o: history.c
histsearch.o: histsearch.c
