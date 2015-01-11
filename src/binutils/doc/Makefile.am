## automake Makefile for binutils documentation
## Process this file with automake to generate Makefile.in

AUTOMAKE_OPTIONS = 1.9.6 gnits no-dist
## "cygnus" == "foreign no-dist" (we hardened from foreign to gnits though)
AUTOMAKE_DEPRECATED_OPTIONS = cygnus

# What version of the manual you want; "all" includes everything
CONFIG=all

# Options to extract the man page from as.texinfo
MANCONF = -Dman

TEXI2POD = perl $(top_srcdir)/../etc/texi2pod.pl

POD2MAN = pod2man --center="GNU Development Tools" \
	 --release="binutils-$(VERSION)" --section=1

# List of man pages generated from binutils.texi
man_MANS = \
	addr2line.1 \
	ar.1 \
	dlltool.1 \
	nlmconv.1 \
	nm.1 \
	objcopy.1 \
	objdump.1 \
	ranlib.1 \
	readelf.1 \
	size.1 \
	strings.1 \
	strip.1 \
	elfedit.1 \
	windres.1 \
	windmc.1 \
	$(DEMANGLER_NAME).1

info_TEXINFOS = binutils.texi
binutils_TEXINFOS = config.texi
binutils_TEXI = $(srcdir)/binutils.texi

config.texi: $(srcdir)/../../bfd/configure $(binutils_TEXI)
	rm -f config.texi
	eval `$(GREP) '^ *VERSION=' $(srcdir)/../../bfd/configure`; \
	  echo "@set VERSION $$VERSION" > $@
	echo "@set UPDATED `date "+%B %Y"`" >> config.texi

AM_MAKEINFOFLAGS = -I "$(srcdir)" -I "$(top_srcdir)/../libiberty" \
		   -I "$(top_srcdir)/../bfd/doc" -I ../../bfd/doc
AM_TEXI2DVI = texi2dvi -I "$(srcdir)" -I "$(top_srcdir)/../libiberty" \
		    -I "$(top_srcdir)/../bfd/doc" -I ../../bfd/doc

# Man page generation from texinfo
addr2line.1:	$(binutils_TEXI) $(binutils_TEXINFOS)
	touch $@
	-$(TEXI2POD) $(MANCONF) -Daddr2line < $(binutils_TEXI) > addr2line.pod
	-($(POD2MAN) addr2line.pod | $(SED) -e '/^.if n .na/d' > $@.T$$$$ && \
		mv -fv $@.T$$$$ $@) || (rm -f $@.T$$$$ && exit 1)
	rm -f addr2line.pod

ar.1:	$(binutils_TEXI) $(binutils_TEXINFOS)
	touch $@
	-$(TEXI2POD) $(MANCONF) -Dar < $(binutils_TEXI) > ar.pod
	-($(POD2MAN) ar.pod | $(SED) -e '/^.if n .na/d' > $@.T$$$$ && \
		mv -fv $@.T$$$$ $@) || (rm -f $@.T$$$$ && exit 1)
	rm -f ar.pod

dlltool.1:	$(binutils_TEXI) $(binutils_TEXINFOS)
	touch $@
	-$(TEXI2POD) $(MANCONF) -Ddlltool < $(binutils_TEXI) > dlltool.pod
	-($(POD2MAN) dlltool.pod | $(SED) -e '/^.if n .na/d' > $@.T$$$$ && \
		mv -fv $@.T$$$$ $@) || (rm -f $@.T$$$$ && exit 1)
	rm -f dlltool.pod

nlmconv.1:	$(binutils_TEXI) $(binutils_TEXINFOS)
	touch $@
	-$(TEXI2POD) $(MANCONF) -Dnlmconv < $(binutils_TEXI) > nlmconv.pod
	-($(POD2MAN) nlmconv.pod | $(SED) -e '/^.if n .na/d' > $@.T$$$$ && \
		mv -fv $@.T$$$$ $@) || (rm -f $@.T$$$$ && exit 1)
	rm -f nlmconv.pod

nm.1:	$(binutils_TEXI) $(binutils_TEXINFOS)
	touch $@
	-$(TEXI2POD) $(MANCONF) -Dnm < $(binutils_TEXI) > nm.pod
	-($(POD2MAN) nm.pod | $(SED) -e '/^.if n .na/d' > $@.T$$$$ && \
		mv -fv $@.T$$$$ $@) || (rm -f $@.T$$$$ && exit 1)
	rm -f nm.pod

objcopy.1:	$(binutils_TEXI) $(binutils_TEXINFOS)
	touch $@
	-$(TEXI2POD) $(MANCONF) -Dobjcopy < $(binutils_TEXI) > objcopy.pod
	-($(POD2MAN) objcopy.pod | $(SED) -e '/^.if n .na/d' > $@.T$$$$ && \
		mv -fv $@.T$$$$ $@) || (rm -f $@.T$$$$ && exit 1)
	rm -f objcopy.pod

objdump.1:	$(binutils_TEXI) $(binutils_TEXINFOS)
	touch $@
	-$(TEXI2POD) $(MANCONF) -Dobjdump < $(binutils_TEXI) > objdump.pod
	-($(POD2MAN) objdump.pod | $(SED) -e '/^.if n .na/d' > $@.T$$$$ && \
		mv -fv $@.T$$$$ $@) || (rm -f $@.T$$$$ && exit 1)
	rm -f objdump.pod

ranlib.1:	$(binutils_TEXI) $(binutils_TEXINFOS)
	touch $@
	-$(TEXI2POD) $(MANCONF) -Dranlib < $(binutils_TEXI) > ranlib.pod
	-($(POD2MAN) ranlib.pod | $(SED) -e '/^.if n .na/d' > $@.T$$$$ && \
		mv -fv $@.T$$$$ $@) || (rm -f $@.T$$$$ && exit 1)
	rm -f ranlib.pod

readelf.1:	$(binutils_TEXI) $(binutils_TEXINFOS)
	touch $@
	-$(TEXI2POD) $(MANCONF) -Dreadelf < $(binutils_TEXI) > readelf.pod
	-($(POD2MAN) readelf.pod | $(SED) -e '/^.if n .na/d' > $@.T$$$$ && \
		mv -fv $@.T$$$$ $@) || (rm -f $@.T$$$$ && exit 1)
	rm -f readelf.pod

size.1:	$(binutils_TEXI) $(binutils_TEXINFOS)
	touch $@
	-$(TEXI2POD) $(MANCONF) -Dsize < $(binutils_TEXI) > size.pod
	-($(POD2MAN) size.pod | $(SED) -e '/^.if n .na/d' > $@.T$$$$ && \
		mv -fv $@.T$$$$ $@) || (rm -f $@.T$$$$ && exit 1)
	rm -f size.pod

strings.1:	$(binutils_TEXI) $(binutils_TEXINFOS)
	touch $@
	-$(TEXI2POD) $(MANCONF) -Dstrings < $(binutils_TEXI) > strings.pod
	-($(POD2MAN) strings.pod | $(SED) -e '/^.if n .na/d' > $@.T$$$$ && \
		mv -fv $@.T$$$$ $@) || (rm -f $@.T$$$$ && exit 1)
	rm -f strings.pod

strip.1:	$(binutils_TEXI) $(binutils_TEXINFOS)
	touch $@
	-$(TEXI2POD) $(MANCONF) -Dstrip < $(binutils_TEXI) > strip.pod
	-($(POD2MAN) strip.pod | $(SED) -e '/^.if n .na/d' > $@.T$$$$ && \
		mv -fv $@.T$$$$ $@) || (rm -f $@.T$$$$ && exit 1)
	rm -f strip.pod

elfedit.1:	$(binutils_TEXI) $(binutils_TEXINFOS)
	touch $@
	-$(TEXI2POD) $(MANCONF) -Delfedit < $(binutils_TEXI) > elfedit.pod
	-($(POD2MAN) elfedit.pod | $(SED) -e '/^.if n .na/d' > $@.T$$$$ && \
		mv -fv $@.T$$$$ $@) || (rm -f $@.T$$$$ && exit 1)
	rm -f elfedit.pod

windres.1:	$(binutils_TEXI) $(binutils_TEXINFOS)
	touch $@
	-$(TEXI2POD) $(MANCONF) -Dwindres < $(binutils_TEXI) > windres.pod
	-($(POD2MAN) windres.pod | $(SED) -e '/^.if n .na/d' > $@.T$$$$ && \
		mv -fv $@.T$$$$ $@) || (rm -f $@.T$$$$ && exit 1)
	rm -f windres.pod

windmc.1:	$(binutils_TEXI) $(binutils_TEXINFOS)
	touch $@
	-$(TEXI2POD) $(MANCONF) -Dwindmc < $(binutils_TEXI) > windmc.pod
	-($(POD2MAN) windmc.pod | $(SED) -e '/^.if n .na/d' > $@.T$$$$ && \
		mv -fv $@.T$$$$ $@) || (rm -f $@.T$$$$ && exit 1)
	rm -f windmc.pod

cxxfilt.man:	$(binutils_TEXI) $(binutils_TEXINFOS)
	touch $@
	-$(TEXI2POD) $(MANCONF) -Dcxxfilt < $(binutils_TEXI) > $(DEMANGLER_NAME).pod
	-($(POD2MAN) $(DEMANGLER_NAME).pod | $(SED) -e '/^.if n .na/d' > $@.T$$$$ && \
		mv -fv $@.T$$$$ $@) || (rm -f $@.T$$$$ && exit 1)
	rm -f $(DEMANGLER_NAME).pod

MAINTAINERCLEANFILES = config.texi
MOSTLYCLEANFILES     = $(DEMANGLER_NAME).1

$(DEMANGLER_NAME).1: cxxfilt.man Makefile
	if test -f cxxfilt.man; then \
	  man=cxxfilt.man; \
	else \
	  man=$(srcdir)/cxxfilt.man; \
	fi; \
	$(SED) -e 's/@PROGRAM@/$(DEMANGLER_NAME)/' \
	    -e 's/cxxfilt/$(DEMANGLER_NAME)/' < $${man} \
		> $(DEMANGLER_NAME).1

# We want install to imply install-info as per GNU standards, despite the
# (deprecated) cygnus option.
install-data-local: install-info

## This is already set above, but append to it here (instead of above)
## to match the order of newer FSF versions of this Makefile:
MAINTAINERCLEANFILES += binutils.info

## Automake 1.9 will only build info files in the objdir if they are
## mentioned in DISTCLEANFILES. It does NOT have to be unconditional,
## though, so we use a bogus condition (i.e. this will always be false):
if GENINSRC_NEVER
DISTCLEANFILES = binutils.info
endif
## (end GENINSRC_NEVER dummy condition)

# Maintenance

# We need it for the taz target in ../../Makefile.in.
info-local: $(MANS)
