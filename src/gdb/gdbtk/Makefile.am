## Process this file with automake to produce Makefile.in

ACLOCAL_AMFLAGS = -I../m4 --install
AUTOMAKE_OPTIONS = foreign dejagnu
AUTOMAKE_DEPRECATED_OPTIONS = cygnus
SUBDIRS = plugins

EXTRA_DIST = generic/gdbtk.h

DISTCLEANFILES = autoscan.log

distclean-local:
	rm -rfv autom4te.cache || rmdir autom4te.cache
.PHONY: distclean-local

pdf-local:
.PHONY: pdf-local

dist-hook:
	@test -d .. && echo "avoid making distributions in subdirs" >&2
	@exit 1 || false
.PHONY: dist-hook
