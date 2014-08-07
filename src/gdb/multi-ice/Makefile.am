## automake Makefile for multi-ice
## Process this file with automake to produce Makefile.in

AUTOMAKE_OPTIONS = foreign dejagnu
AUTOMAKE_DEPRECATED_OPTIONS = cygnus
ACLOCAL_AMFLAGS = -I../m4 --install
MULTI_ICE_INCLUDES = @MULTI_ICE_INCLUDES@
MULTI_ICE_LIBS = @MULTI_ICE_LIBS@
MULTI_ICE_PLATFORM_SOURCE = @MULTI_ICE_PLATFORM_SOURCE@

WARN_CFLAGS = @WARN_CFLAGS@
AM_CFLAGS = $(WARN_CFLAGS)
AM_CPPFLAGS = -I./.. -I$(srcdir)/../../include \
         -I$(srcdir)/.. -I$(srcdir)/../config -I../../bfd \
         -I$(srcdir)/../macosx \
         $(MULTI_ICE_INCLUDES) @MAINT@ -I$(srcdir)/../rdi-share

INCLUDES_PLUS_OLD_INCLUDES = $(AM_CPPFLAGS) $(INCLUDES)

LIBS=-L../../libiberty @MAINT@ -liberty $(WIN32LIBS) $(MULTI_ICE_LIBS)

bin_PROGRAMS = multi-ice-gdb-server
multi_ice_gdb_server_SOURCES = server-main.c rdi150-low.c low.h \
    remote-utils.c remote-utils.h utils.c server-win.c server.h \
    arm-singlestep.c

noinst_HEADERS = low.h remote-utils.h server.h

# unfortunately some of the binaries need to be distributed, because they
# only come that way:
EXTRA_DIST = libtoolconf.a MICE-gdbserver.ico MICE-gdbserver.rc \
    multi-ice-gdb-server.rc Multi-ICE.def stamp-h.in \
    toolconf.def toolconf.dll

## Dependencies:
multi_ice_gdb_server_LDFLAGS = $(WIN32LDAPP)
multi_ice_gdb_server_DEPENDENCIES = $(WIN32RES)
multi_ice_gdb_server_LDADD = $(WIN32RES)

MICE-gdbserverres.o: MICE-gdbserver.rc
	$(WINDRES) --include $(srcdir) $(srcdir)/MICE-gdbserver.rc MICE-gdbserverres.o

MOSTLYCLEANFILES = config.h.in~

DISTCLEANFILES = *~ autoscan.log

distclean-local:
	rm -rfv autom4te.cache || rmdir autom4te.cache
.PHONY: distclean-local

dist-hook:
	@test -d .. && echo "avoid making distributions in subdirs" >&2
	false
.PHONY: dist-hook

MAINTAINERCLEANFILES = configure.scan missing_symbols.txt

configure.scan:
	autoscan || touch configure.scan

missing_symbols.txt: $(multi_ice_gdb_server_SOURCES) @MAINT@ Makefile
	$(MAKE) multi-ice-gdb-server 2>&1 | $(GREP) "referenced from" | cut -d\, -f1 | tr -d [:punct:] | tr -d [:blank:] | sort | uniq | tee $@

maintainer-clean-local:
	@test -d .. || echo "this should be a subdir"
.PHONY: maintainer-clean-local
