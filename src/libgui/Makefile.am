## automake Makefile for libgui
## Process this file with automake to produce Makefile.in.

AUTOMAKE_OPTIONS = 1.11.6 gnits no-dependencies no-dist dejagnu
## "cygnus" == "foreign no-dist" (we hardened from foreign to gnits though)
AUTOMAKE_DEPRECATED_OPTIONS = cygnus
TEXINFO_TEX = ../texinfo/texinfo.tex
SUBDIRS = library src
DISTCLEANFILES = *~ autoscan*.log .DS_Store *.cache.bak

distclean-local:
	rm -rf autom4te.cache || rmdir autom4te.cache
	if test -d .. && test -w .. && test -r .. && test -s ..; then \
	  rm -f ../*~; \
	fi
