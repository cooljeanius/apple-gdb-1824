## Process this file with automake to generate Makefile.in

AUTOMAKE_OPTIONS = 1.11.6 dejagnu gnits

AM_CFLAGS = -I$(top_srcdir) -ggdb -Wall -Wextra -Wabi
AM_CPPFLAGS = -I$(top_srcdir)
AM_CXXFLAGS = -I$(top_srcdir) -ggdb -Wall -Wextra -Wabi -Wsign-promo \
  -Wstrict-null-sentinel -Woverloaded-virtual -Wctor-dtor-privacy \
  -Wuninitialized -Wshadow -Wconversion -pedantic

noinst_PROGRAMS = unit

unit_SOURCES = unit.cc
unit_CXXFLAGS = $(CXXSTDLIBFLAGS)
## unit_CXXFLAGS are also used when linking, so no need for a separate
## unit_LDFLAGS

clean-local: Makefile
	-rm -rf *.dSYM || rmdir *.dSYM
.PHONY: clean-local

distclean-local: $(top_srcdir)/config.status clean-local
	-if test -d .deps; then (rm -rf .deps || rmdir .deps); fi
.PHONY: distclean-local
