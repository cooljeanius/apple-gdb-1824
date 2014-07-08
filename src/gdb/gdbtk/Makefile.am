## Process this file with automake to produce Makefile.in

ACLOCAL_AMFLAGS = -I../m4 --install
AUTOMAKE_OPTIONS = foreign dejagnu
AUTOMAKE_DEPRECATED_OPTIONS = cygnus
SUBDIRS = plugins

EXTRA_DIST = generic/gdbtk.h

DISTCLEANFILES = autoscan.log

distclean-local:
	rm -rfv autom4te.cache || rmdir autom4te.cache
