## Process this file with automake to generate Makefile.in

AUTOMAKE_OPTIONS = dejagnu

AM_CFLAGS = -I$(top_srcdir) -ggdb
AM_CPPFLAGS = -I$(top_srcdir)
AM_CXXFLAGS = -I$(top_srcdir) -ggdb

noinst_PROGRAMS = unit

unit_SOURCES = unit.cc

clean-local: Makefile
.PHONY: clean-local

distclean-local: $(top_srcdir)/config.status
.PHONY: distclean-local
