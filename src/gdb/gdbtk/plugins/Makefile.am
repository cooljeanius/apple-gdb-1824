## Process this file with automake to produce Makefile.in

ACLOCAL_AMFLAGS = -I../../m4 --install
AUTOMAKE_DEPRECATED_OPTIONS = cygnus
SUBDIRS = @make_subdirs@

DISTCLEANFILES = autoscan.log

distclean-local:
	rm -rfv autom4te.cache || rmdir autom4te.cache
.PHONY: distclean-local

dist-hook:
	@test -d .. && echo "avoid making distributions in subdirs" >&2
	@exit 1 || false
.PHONY: dist-hook
