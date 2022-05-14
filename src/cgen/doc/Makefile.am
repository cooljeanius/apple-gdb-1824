## automake Makefile for cgen documentation
## Process this file with automake to generate Makefile.in

AUTOMAKE_OPTIONS = 1.9 gnits no-dist info-in-builddir
## "cygnus" == "foreign no-dist" (we hardened from foreign to gnits though)
AUTOMAKE_DEPRECATED_OPTIONS = cygnus

# List of files that go into cgen.info.
DOCFILES = app.texi cgen.texi credits.texi glossary.texi intro.texi \
	notes.texi opcodes.texi pmacros.texi porting.texi \
	rtl.texi running.texi sim.texi

info_TEXINFOS = cgen.texi cgenint.texi

# version.texi is handled by autoconf/automake
cgen_TEXINFOS = $(DOCFILES)

# versionint.texi is handled by autoconf/automake
cgenint_TEXINFOS = cgenint.texi glossary.texi

# These are NOT ready for prime time yet. Not even a little bit:
noinst_TEXINFOS = cgen.texi cgenint.texi

# Maintenance:
MAINTAINERCLEANFILES = cgen.info cgenint.info

# Automake 1.9 will only build info files in the objdir if they are
# mentioned in DISTCLEANFILES. It does NOT have to be unconditional,
# though, so we use a bogus condition (this is a hack).
# NOTE: This is copied from gas.
if GENINSRC_NEVER
DISTCLEANFILES = cgen.info cgenint.info
endif
