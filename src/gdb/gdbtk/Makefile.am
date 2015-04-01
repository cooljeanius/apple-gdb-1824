## automake Makefile for gdbtk
## Process this file with automake to produce Makefile.in

ACLOCAL_AMFLAGS = -I../m4 --install
AUTOMAKE_OPTIONS = foreign dejagnu no-dist
## "cygnus" == "foreign no-dist" (we hardened from foreign to gnits though)
AUTOMAKE_DEPRECATED_OPTIONS = cygnus
SUBDIRS = plugins

## extend the automake verbosity control:
AM_V_v = $(am__v_v_@AM_V@)
am__v_v_ = $(am__v_v_@AM_DEFAULT_V@)
am__v_v_0 =
am__v_v_1 = -v

EXTRA_DIST = generic/gdbtk.h

DISTCLEANFILES = autoscan.log .DS_Store

distclean-local:
	rm -rf $(AM_V_v) autom4te.cache || rmdir autom4te.cache
.PHONY: distclean-local

pdf-local:
.PHONY: pdf-local

dist-hook:
	@test -d .. && echo "avoid making distributions in subdirs" >&2
	@exit 1 || false
.PHONY: dist-hook
