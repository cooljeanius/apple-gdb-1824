## automake Makefile for gdbtk plugins
## Process this file with automake to produce Makefile.in

ACLOCAL_AMFLAGS = -I../../m4 --install
AUTOMAKE_OPTIONS = 1.11.6 foreign no-dist dejagnu
## "cygnus" == "foreign no-dist"
AUTOMAKE_DEPRECATED_OPTIONS = cygnus
SUBDIRS = @make_subdirs@

## extend the automake verbosity control:
AM_V_v = $(am__v_v_@AM_V@)
am__v_v_ = $(am__v_v_@AM_DEFAULT_V@)
am__v_v_0 =
am__v_v_1 = -v

DISTCLEANFILES = autoscan.log .DS_Store

distclean-local:
	rm -rf $(AM_V_v) autom4te.cache || rmdir autom4te.cache
.PHONY: distclean-local

dist-hook:
	@test -d .. && echo "avoid making distributions in subdirs" >&2
	@exit 1 || false
.PHONY: dist-hook
