## automake Makefile for libopcodes source
## Process this file with automake to generate Makefile.in
#
#   Copyright (C) 2012-2014 Free Software Foundation, Inc.
#
# This file is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; see the file COPYING3.  If not see
# <http://www.gnu.org/licenses/>.
#

## "-Wportability" warns about rules that we just stole from automake
## itself, so disable those warnings for being hypocritical:
AUTOMAKE_OPTIONS = 1.11.6 gnits no-dist dejagnu -Wall -Wno-portability
## "cygnus" == "foreign no-dist" (we hardened from foreign to gnits though)
AUTOMAKE_DEPRECATED_OPTIONS = cygnus

ACLOCAL_AMFLAGS = -I m4 --install

# APPLE LOCAL: dynamiclib building method:
NSLIBTOOL = cc -dynamiclib

SUBDIRS = po

INCDIR = $(srcdir)/../include
BFDDIR = $(srcdir)/../bfd
INTLDIR = $(srcdir)/../intl
MKDEP = gcc -MM

PATH_TO_MOVEIFCHANGE = $(srcdir)/../move-if-change
RUN_MOVEIFCHANGE = $(SHELL) $(PATH_TO_MOVEIFCHANGE)

## also needed:
HOST_ARCHITECTURE = @HOST_ARCHITECTURE@
## not strictly necessary, but goes with the previous:
TARGET_ARCHITECTURE = @TARGET_ARCHITECTURE@
## use the host one, because that is the one that matters:
C_ARCHFLAGS = -arch $(HOST_ARCHITECTURE)

## APPLE LOCAL: WARN_CFLAGS not handled by automake?
## (yes they are, actually... not sure what was going wrong for Apple...)
AM_WARN_CFLAGS = -Wall
WARN_CFLAGS = @WARN_CFLAGS@
NO_WERROR = @NO_WERROR@
AM_CFLAGS = $(WARN_CFLAGS) $(C_ARCHFLAGS)
AM_WARNFLAGS = $(AM_WARN_CFLAGS)
AM_LIBTOOLFLAGS = --no-silent --verbose 
AM_LIBTOOL_DEBUG_FLAGS = @MAINT@ --debug

bfdlibdir = @bfdlibdir@
bfdincludedir = @bfdincludedir@

bfdlib_LTLIBRARIES = libopcodes.la

# This is where bfd.h lives:
BFD_H = ../bfd/bfd.h

# This is where libiberty lives:
LIBIBERTY = ../libiberty/libiberty.a

AM_LDFLAGS = -L../libiberty -L../bfd -L../intl -L../mmalloc

# Header files.
HFILES = \
	cgen-ops.h cgen-types.h \
	fr30-desc.h fr30-opc.h \
	frv-desc.h frv-opc.h \
	h8500-opc.h \
	i386-opc.h \
	ia64-asmtab.h \
	ia64-opc.h \
	ip2k-desc.h ip2k-opc.h \
	iq2000-desc.h iq2000-opc.h \
	m32c-desc.h m32c-opc.h \
	m32r-desc.h m32r-opc.h \
	mcore-opc.h \
	ms1-desc.h ms1-opc.h \
	openrisc-desc.h openrisc-opc.h \
	sh-opc.h \
	sh64-opc.h \
	sysdep.h \
	w65-opc.h \
	xstormy16-desc.h xstormy16-opc.h \
	z8k-opc.h

# C source files that correspond to .o's.
CFILES = \
	a29k-dis.c \
	alpha-dis.c \
	alpha-opc.c \
	arc-dis.c \
	arc-opc.c \
	arc-ext.c \
	arm-dis.c \
	avr-dis.c \
	cgen-asm.c \
	cgen-dis.c \
	cgen-opc.c \
	cris-dis.c \
	cris-opc.c \
	crx-dis.c \
	crx-opc.c \
	d10v-dis.c \
	d10v-opc.c \
	d30v-dis.c \
	d30v-opc.c \
	dlx-dis.c \
	dis-buf.c \
	dis-init.c \
	disassemble.c \
	fr30-asm.c \
	fr30-desc.c \
	fr30-dis.c \
	fr30-ibld.c \
	fr30-opc.c \
	frv-asm.c \
	frv-desc.c \
	frv-dis.c \
	frv-ibld.c \
	frv-opc.c \
	h8300-dis.c \
	h8500-dis.c \
	hppa-dis.c \
	i370-dis.c \
	i370-opc.c \
	i386-dis.c \
	i386-opc.c \
	i860-dis.c \
	i960-dis.c \
	ia64-dis.c \
	ia64-opc-a.c \
	ia64-opc-b.c \
	ia64-opc-f.c \
	ia64-opc-i.c \
	ia64-opc-m.c \
	ia64-opc-d.c \
	ia64-opc.c \
	ia64-gen.c \
	ia64-asmtab.c \
	ip2k-asm.c \
	ip2k-desc.c \
	ip2k-dis.c \
	ip2k-ibld.c \
	ip2k-opc.c \
	iq2000-asm.c \
	iq2000-desc.c \
	iq2000-dis.c \
	iq2000-ibld.c \
	iq2000-opc.c \
	m32c-asm.c \
	m32c-desc.c \
	m32c-dis.c \
	m32c-ibld.c \
	m32c-opc.c \
	m32r-asm.c \
	m32r-desc.c \
	m32r-dis.c \
	m32r-ibld.c \
	m32r-opc.c \
	m32r-opinst.c \
	m68hc11-dis.c \
	m68hc11-opc.c \
	m68k-dis.c \
	m68k-opc.c \
	m88k-dis.c \
	maxq-dis.c \
	mcore-dis.c \
	mips-dis.c \
	mips-opc.c \
	mips16-opc.c \
	m10200-dis.c \
	m10200-opc.c \
	m10300-dis.c \
	m10300-opc.c \
	mmix-dis.c \
	mmix-opc.c \
	ms1-asm.c \
	ms1-desc.c \
	ms1-dis.c \
	ms1-ibld.c \
	ms1-opc.c \
	ns32k-dis.c \
	openrisc-asm.c \
	openrisc-desc.c \
	openrisc-dis.c \
	openrisc-ibld.c \
	openrisc-opc.c \
	or32-dis.c \
	or32-opc.c \
	pdp11-dis.c \
	pdp11-opc.c \
	pj-dis.c \
	pj-opc.c \
	ppc-dis.c \
	ppc-opc.c \
	s390-mkopc.c \
	s390-opc.c \
	s390-dis.c \
	sh-dis.c \
	sh64-dis.c \
	sh64-opc.c \
	sparc-dis.c \
	sparc-opc.c \
	tic30-dis.c \
	tic4x-dis.c \
	tic54x-dis.c \
	tic54x-opc.c \
	tic80-dis.c \
	tic80-opc.c \
	v850-dis.c \
	v850-opc.c \
	vax-dis.c \
	w65-dis.c \
	xstormy16-asm.c \
	xstormy16-desc.c \
	xstormy16-dis.c \
	xstormy16-ibld.c \
	xstormy16-opc.c \
	xtensa-dis.c \
	z8k-dis.c \
	z8kgen.c

ALL_MACHINES = \
	a29k-dis.lo \
	alpha-dis.lo \
	alpha-opc.lo \
	arc-dis.lo \
	arc-opc.lo \
	arc-ext.lo \
	arm-dis.lo \
	avr-dis.lo \
	cgen-asm.lo \
	cgen-dis.lo \
	cgen-opc.lo \
	cris-dis.lo \
	cris-opc.lo \
	crx-dis.lo \
	crx-opc.lo \
	d10v-dis.lo \
	d10v-opc.lo \
	d30v-dis.lo \
	d30v-opc.lo \
	dlx-dis.lo \
	fr30-asm.lo \
	fr30-desc.lo \
	fr30-dis.lo \
	fr30-ibld.lo \
	fr30-opc.lo \
	frv-asm.lo \
	frv-desc.lo \
	frv-dis.lo \
	frv-ibld.lo \
	frv-opc.lo \
	h8300-dis.lo \
	h8500-dis.lo \
	hppa-dis.lo \
	i386-dis.lo \
	i386-opc.lo \
	i370-dis.lo \
	i370-opc.lo \
	i860-dis.lo \
	i960-dis.lo \
	ia64-dis.lo \
	ia64-opc.lo \
	ip2k-asm.lo \
	ip2k-desc.lo \
	ip2k-dis.lo \
	ip2k-ibld.lo \
	ip2k-opc.lo \
	iq2000-asm.lo \
	iq2000-desc.lo \
	iq2000-dis.lo \
	iq2000-ibld.lo \
	iq2000-opc.lo \
	m32c-asm.lo \
	m32c-desc.lo \
	m32c-dis.lo \
	m32c-ibld.lo \
	m32c-opc.lo \
	m32r-asm.lo \
	m32r-desc.lo \
	m32r-dis.lo \
	m32r-ibld.lo \
	m32r-opc.lo \
	m32r-opinst.lo \
	m68hc11-dis.lo \
	m68hc11-opc.lo \
	m68k-dis.lo \
	m68k-opc.lo \
	m88k-dis.lo \
	m10200-dis.lo \
	m10200-opc.lo \
	m10300-dis.lo \
	m10300-opc.lo \
	maxq-dis.lo \
	mcore-dis.lo \
	mips-dis.lo \
	mips-opc.lo \
	mips16-opc.lo \
	mmix-dis.lo \
	mmix-opc.lo \
	ms1-asm.lo \
	ms1-desc.lo \
	ms1-dis.lo \
	ms1-ibld.lo \
	ms1-opc.lo \
	msp430-dis.lo \
	ns32k-dis.lo \
	openrisc-asm.lo \
	openrisc-desc.lo \
	openrisc-dis.lo \
	openrisc-ibld.lo \
	openrisc-opc.lo \
	or32-dis.lo \
	or32-opc.lo \
	pdp11-dis.lo \
	pdp11-opc.lo \
	pj-dis.lo \
	pj-opc.lo \
	ppc-dis.lo \
	ppc-opc.lo \
	s390-dis.lo \
	s390-opc.lo \
	sh-dis.lo \
	sh64-dis.lo \
	sh64-opc.lo \
	sparc-dis.lo \
	sparc-opc.lo \
	tic30-dis.lo \
	tic4x-dis.lo \
	tic54x-dis.lo \
	tic54x-opc.lo \
	tic80-dis.lo \
	tic80-opc.lo \
	v850-dis.lo \
	v850-opc.lo \
	vax-dis.lo \
	w65-dis.lo \
	xstormy16-asm.lo \
	xstormy16-desc.lo \
	xstormy16-dis.lo \
	xstormy16-ibld.lo \
	xstormy16-opc.lo \
	xtensa-dis.lo \
	z8k-dis.lo

OFILES = @BFD_MACHINES@

# We should reconfigure whenever bfd/configure.ac changes, because
# that is where the version number in Makefile comes from.
CONFIG_STATUS_DEPENDENCIES = $(BFDDIR)/configure.ac

AM_CPPFLAGS = -D_GNU_SOURCE -I. -I$(srcdir) -I../bfd -I$(INCDIR) \
-I$(BFDDIR) @HDEFINES@ -I$(INTLDIR) -I../intl

## hacks to extend the verbosity control:
AM_V_redir = $(am__v_redir_$(V))
am__v_redir_ = $(am__v_redir_$(AM_DEFAULT_VERBOSITY))
am__v_redir_0 = >
am__v_redir_1 = | tee

AM_V_redir_a = $(am__v_redir_a_$(V))
am__v_redir_a_ = $(am__v_redir_a_$(AM_DEFAULT_VERBOSITY))
am__v_redir_a_0 = >>
am__v_redir_a_1 = | tee -a

AM_V_v = $(am__v_v_$(V))
am__v_v_ = $(am__v_v_$(AM_DEFAULT_VERBOSITY))
am__v_v_0 =
am__v_v_1 = -v

disassemble.lo: disassemble.c $(INCDIR)/dis-asm.h
	$(AM_V_CC)$(LIBTOOL) --mode=compile $(COMPILE) -c @archdefs@ \
	  $(srcdir)/disassemble.c
disassemle.o: disassemble.lo

libopcodes_la_SOURCES =  dis-buf.c disassemble.c dis-init.c i386-opc.c
# It is desirable to list ../bfd/libbfd.la in DEPENDENCIES and LIBADD.
# Unfortunately this causes libtool to add -L$(libdir), referring to the
# planned install directory of libbfd.  This can cause us to pick up an
# old version of libbfd, or to pick up libbfd for the wrong architecture
# if host != build.
libopcodes_la_DEPENDENCIES = $(OFILES) $(ALL_MACHINES)
libopcodes_la_LIBADD = $(OFILES) @WIN32LIBADD@
libopcodes_la_LDFLAGS = -release $(VERSION) @WIN32LDFLAGS@

# libtool will build .libs/libopcodes.a. We create libopcodes.a in
# the build directory so that we do NOT have to convert all the
# programs that use libopcodes.a simultaneously. This is a hack which
# should be removed if everything else starts using libtool. FIXME.

noinst_LIBRARIES = libopcodes.a

stamp-lib: libopcodes.la @MAINT@ config.status $(PATH_TO_MOVEIFCHANGE)
	libtooldir=`$(LIBTOOL) --config | $(SED) -n -e 's/^objdir=//p'`; \
	if test -z "$${libtooldir}"; then \
	  echo "" && echo "error: libtooldir is unset" >&2 && exit 1; \
	else \
	  echo "libtooldir is $${libtooldir}"; \
	fi; \
	if [ -f $${libtooldir}/libopcodes.a ]; then \
	  cp $(AM_V_v) $${libtooldir}/libopcodes.a libopcodes.tmp; \
	  $(RANLIB) libopcodes.tmp; \
	  $(RUN_MOVEIFCHANGE) libopcodes.tmp libopcodes.a; \
	else echo "no need to check in $${libtooldir} for libopcodes.a"; fi
	$(AM_V_at)touch stamp-lib

libopcodes.a: stamp-lib

nonlibtool_libopcodes.a: stamp-lib
	@echo "target for $@ only calls its dependency, $<"
.PHONY: nonlibtool_libopcodes.a

$(BFD_H): ../bfd/Makefile
	(cd ../bfd && $(MAKE) bfd.h)
	test -e $(BFD_H)

# APPLE LOCAL: begin framework building:
SYSTEM_FRAMEWORK = -framework System @SYSTEM_FRAMEWORK_LIBS@

FRAMEWORK = opcodes
FRAMEWORK_ADDRESS = 0x0
FRAMEWORK_PREFIX =
FRAMEWORK_SUFFIX =
FRAMEWORK_VERSION_SUFFIX =
FRAMEWORK_VERSION = A
FRAMEWORK_OFILES = `echo $(libopcodes_la_OBJECTS) $(libopcodes_la_LIBADD) | $(SED) -e 's/\.lo/\.o/g'`
FRAMEWORK_LIB = -F../bfd -L../bfd -framework bfd -F../libiberty -L../libiberty -framework liberty -F../mmalloc -L../mmalloc -framework mmalloc -L../intl -lintl $(SYSTEM_FRAMEWORK) $(LDFLAGS)

stamp-framework-headers:
	rm -f stamp-framework stamp-framework-headers
	rm -rf $(AM_V_v) $(FRAMEWORK).framework
	$(MKDIR_P) $(AM_V_v) $(FRAMEWORK).framework/Versions/$(FRAMEWORK_VERSION)/Headers
	$(MKDIR_P) $(AM_V_v) $(FRAMEWORK).framework/Versions/$(FRAMEWORK_VERSION)/PrivateHeaders
	$(LN_S) $(AM_V_v) Versions/Current/Headers $(FRAMEWORK).framework/Headers
	$(LN_S) $(AM_V_v) Versions/Current/PrivateHeaders $(FRAMEWORK).framework/PrivateHeaders
	$(LN_S) $(AM_V_v) A $(FRAMEWORK).framework/Versions/Current
	cp -p $(AM_V_v) $(srcdir)/*.h $(FRAMEWORK).framework/Versions/Current/Headers/
	cp -p $(AM_V_v) *.h $(FRAMEWORK).framework/Versions/Current/Headers/
	$(AM_V_at)touch stamp-framework-headers

stamp-framework: $(libopcodes_la_OBJECTS) $(libopcodes_la_DEPENDENCIES)
	$(MAKE) $(FRAMEWORK_OFILES)
	rm -f stamp-framework-headers
	$(MAKE) stamp-framework-headers
	$(MKDIR_P) $(AM_V_v) $(FRAMEWORK).framework/Versions/$(FRAMEWORK_VERSION)/Resources
	$(LN_S) $(AM_V_v) Versions/Current/Resources $(FRAMEWORK).framework/Resources
	$(MKDIR_P) $(AM_V_v) $(FRAMEWORK).framework/Versions/Current/Resources/English.lproj

	set -ex; \
	if [ "x$(FRAMEWORK_SUFFIX)" != "x" ]; then \
	    test -n "$(FRAMEWORK_SUFFIX)" && echo "$(FRAMEWORK_SUFFIX)"; \
	   $(NSLIBTOOL) -compatibility_version 1 -current_version 1 -install_name /System/Library/PrivateFrameworks/$(FRAMEWORK).framework/Versions/$(FRAMEWORK_VERSION)/$(FRAMEWORK_PREFIX)$(FRAMEWORK)$(FRAMEWORK_VERSION_SUFFIX) -o $(FRAMEWORK).framework/Versions/Current/$(FRAMEWORK_PREFIX)$(FRAMEWORK)$(FRAMEWORK_VERSION_SUFFIX) $(FRAMEWORK_OFILES) $(FRAMEWORK_LIB); \
	    $(LN_S) $(AM_V_v) $(FRAMEWORK_PREFIX)$(FRAMEWORK)$(FRAMEWORK_VERSION_SUFFIX) $(FRAMEWORK).framework/Versions/Current/$(FRAMEWORK_PREFIX)$(FRAMEWORK)$(FRAMEWORK_SUFFIX); \
	    $(LN_S) $(AM_V_v) Versions/Current/$(FRAMEWORK_PREFIX)$(FRAMEWORK)$(FRAMEWORK_VERSION_SUFFIX) $(FRAMEWORK).framework/$(FRAMEWORK_PREFIX)$(FRAMEWORK)$(FRAMEWORK_VERSION_SUFFIX); \
	    $(LN_S) $(AM_V_v) Versions/Current/$(FRAMEWORK_PREFIX)$(FRAMEWORK)$(FRAMEWORK_SUFFIX) $(FRAMEWORK).framework/$(FRAMEWORK_PREFIX)$(FRAMEWORK)$(FRAMEWORK_SUFFIX); \
	    $(LN_S) $(AM_V_v) $(FRAMEWORK_PREFIX)$(FRAMEWORK)$(FRAMEWORK_SUFFIX) $(FRAMEWORK).framework/$(FRAMEWORK); \
	else \
	    echo "making un-suffixed framework"; \
	    $(NSLIBTOOL) -compatibility_version 1 -current_version 1 -install_name /System/Library/PrivateFrameworks/$(FRAMEWORK).framework/Versions/$(FRAMEWORK_VERSION)/$(FRAMEWORK_PREFIX)$(FRAMEWORK) -o $(FRAMEWORK).framework/Versions/Current/$(FRAMEWORK_PREFIX)$(FRAMEWORK) $(FRAMEWORK_OFILES) $(FRAMEWORK_LIB); \
	    $(LN_S) $(AM_V_v) Versions/Current/$(FRAMEWORK_PREFIX)$(FRAMEWORK) $(FRAMEWORK).framework/$(FRAMEWORK_PREFIX)$(FRAMEWORK); \
	fi
	$(AM_V_at)touch stamp-framework
# APPLE LOCAL: end framework building

POTFILES = $(HFILES) $(CFILES)
po/POTFILES.in: @MAINT@ Makefile
	$(AM_V_GEN)for f in $(POTFILES); do echo $${f}; done \
	  | LC_ALL=C LC_COLLATE="" sort $(AM_V_redir) tmp \
	  && mv $(AM_V_v) tmp $(srcdir)/po/POTFILES.in

## NORMAL_INSTALL is a special automake variable:
install-bfdlibLTLIBRARIES: @INSTALL_LIBBFD_TRUE@install_libopcodes
	$(AM_V_at)$(NORMAL_INSTALL)

## NORMAL_UNINSTALL is a special automake variable:
uninstall-bfdlibLTLIBRARIES: @INSTALL_LIBBFD_TRUE@uninstall_libopcodes
	$(AM_V_at)$(NORMAL_UNINSTALL)

.PHONY: install_libopcodes uninstall_libopcodes
install_libopcodes: $(bfdlib_LTLIBRARIES)
	$(mkinstalldirs) $(DESTDIR)$(bfdlibdir)
	$(mkinstalldirs) $(DESTDIR)$(bfdincludedir)
	$(AM_V_at)list='$(bfdlib_LTLIBRARIES)'; for p in $${list}; do \
	  if test -f $${p}; then \
	    echo "$(LIBTOOL) --mode=install $(INSTALL) $${p} $(DESTDIR)$(bfdlibdir)/$${p}"; \
	    $(LIBTOOL) --mode=install $(INSTALL) $${p} $(DESTDIR)$(bfdlibdir)/$${p}; \
	  else echo "$${p} does not exist to install"; fi; \
	done
	$(INSTALL_DATA) $(INCDIR)/dis-asm.h $(DESTDIR)$(bfdincludedir)/dis-asm.h

uninstall_libopcodes:
	list='$(bfdlib_LTLIBRARIES)'; for p in $${list}; do \
	  $(LIBTOOL) --mode=uninstall rm -f $(DESTDIR)$(bfdlibdir)/$${p}; \
	done
	rm -f $(AM_V_v) $(DESTDIR)$(bfdincludedir)/dis-asm.h

CLEANFILES = \
	stamp-ip2k stamp-m32c stamp-m32r stamp-fr30 stamp-frv \
	stamp-openrisc stamp-iq2000 stamp-ms1 stamp-xstormy16 \
	libopcodes.a stamp-lib dep.sed DEP DEPA DEP1 DEP2

DISTCLEANFILES = stamp-lib .DS_Store *~

distclean-local:
	rm -rf autom4te.cache || rmdir autom4te.cache

CGENDIR = @cgendir@
CPUDIR = $(CGENDIR)/cpu
CGEN = "`if test -f ../guile/libguile/guile; then echo ../guile/libguile/guile; else echo guile; fi` -l ${cgendir}/guile.scm -s"
CGENFLAGS = -v

CGENDEPS = \
	$(CGENDIR)/desc.scm $(CGENDIR)/desc-cpu.scm \
	$(CGENDIR)/opcodes.scm $(CGENDIR)/opc-asmdis.scm \
	$(CGENDIR)/opc-ibld.scm $(CGENDIR)/opc-itab.scm \
	$(CGENDIR)/opc-opinst.scm \
	cgen-asm.in cgen-dis.in cgen-ibld.in

CGEN_CPUS = fr30 frv ip2k m32c m32r ms1 openrisc xstormy16

if CGEN_MAINT
IP2K_DEPS = stamp-ip2k
M32C_DEPS = stamp-m32c
M32R_DEPS = stamp-m32r
FR30_DEPS = stamp-fr30
FRV_DEPS = stamp-frv
MS1_DEPS = stamp-ms1
OPENRISC_DEPS = stamp-openrisc
IQ2000_DEPS = stamp-iq2000
XSTORMY16_DEPS = stamp-xstormy16
else
IP2K_DEPS =
M32C_DEPS =
M32R_DEPS =
FR30_DEPS =
FRV_DEPS =
MS1_DEPS =
OPENRISC_DEPS = 
IQ2000_DEPS = 
XSTORMY16_DEPS = 
endif

run-cgen: config.status
	$(SHELL) $(srcdir)/cgen.sh opcodes $(srcdir) $(CGEN) \
	  $(CGENDIR) "$(CGENFLAGS)" $(arch) $(prefix) $(archfile) $(opcfile) \
	  "$(options)" "$(extrafiles)"
	touch stamp-${prefix}
.PHONY: run-cgen

# Maintainer utility rule to regenerate all cgen files.
run-cgen-all: config.status
	for c in $(CGEN_CPUS); \
	do \
		$(MAKE) stamp-$${c} || exit 1; \
	done
.PHONY: run-cgen-all

# For now, require developers to configure with --enable-cgen-maint.
$(srcdir)/ip2k-desc.h $(srcdir)/ip2k-desc.c $(srcdir)/ip2k-opc.h $(srcdir)/ip2k-opc.c $(srcdir)/ip2k-ibld.c $(srcdir)/ip2k-asm.c $(srcdir)/ip2k-dis.c: $(IP2K_DEPS)
	@echo "ensuring $@ is up-to-date"
stamp-ip2k: $(CGENDEPS) $(CPUDIR)/ip2k.cpu $(CPUDIR)/ip2k.opc
	$(MAKE) run-cgen arch=ip2k prefix=ip2k options="" \
		archfile=$(CPUDIR)/ip2k.cpu opcfile=$(CPUDIR)/ip2k.opc extrafiles=""

$(srcdir)/m32c-desc.h $(srcdir)/m32c-desc.c $(srcdir)/m32c-opc.h $(srcdir)/m32c-opc.c $(srcdir)/m32c-ibld.c $(srcdir)/m32c-asm.c $(srcdir)/m32c-dis.c: $(M32C_DEPS)
	@echo "ensuring $@ is up-to-date" || true
stamp-m32c: $(CGENDEPS) $(srcdir)/../cpu/m32c.cpu $(srcdir)/../cpu/m32c.opc
	$(MAKE) run-cgen arch=m32c prefix=m32c options="" \
		archfile=$(srcdir)/../cpu/m32c.cpu \
		opcfile=$(srcdir)/../cpu/m32c.opc extrafiles=""
m32c_opc_h = m32c-opc.h cgen-types.h cgen-ops.h
m32c-asm.lo: m32c-asm.c sysdep.h config.h $(BFD_H) \
  $(INCDIR)/ansidecl.h $(INCDIR)/symcat.h m32c-desc.h \
  $(INCDIR)/opcode/cgen.h $(m32c_opc_h) opintl.h
m32c-asm.o: m32c-asm.lo
m32c-desc.lo: m32c-desc.c sysdep.h config.h $(BFD_H) \
  $(INCDIR)/ansidecl.h $(INCDIR)/symcat.h m32c-desc.h \
  $(INCDIR)/opcode/cgen.h $(m32c_opc_h) opintl.h
m32c-desc.o: m32c-desc.lo
m32c-dis.lo: m32c-dis.c sysdep.h config.h $(INCDIR)/dis-asm.h \
  $(BFD_H) $(INCDIR)/ansidecl.h \
  $(INCDIR)/symcat.h m32c-desc.h $(INCDIR)/opcode/cgen.h \
  $(m32c_opc_h) opintl.h
m32c-dis.o: m32c-dis.lo
m32c-ibld.lo: m32c-ibld.c sysdep.h config.h $(INCDIR)/dis-asm.h \
  $(BFD_H) $(INCDIR)/ansidecl.h \
  $(INCDIR)/symcat.h m32c-desc.h $(INCDIR)/opcode/cgen.h \
  $(m32c_opc_h) opintl.h
m32c-ibld.o: m32c-ibld.lo
m32c-opc.lo: m32c-opc.c sysdep.h config.h $(BFD_H) \
  $(INCDIR)/ansidecl.h $(INCDIR)/symcat.h m32c-desc.h \
  $(INCDIR)/opcode/cgen.h $(m32c_opc_h)
m32c-opc.o: m32c-opc.lo

$(srcdir)/m32r-desc.h $(srcdir)/m32r-desc.c $(srcdir)/m32r-opc.h $(srcdir)/m32r-opc.c $(srcdir)/m32r-ibld.c $(srcdir)/m32r-opinst.c $(srcdir)/m32r-asm.c $(srcdir)/m32r-dis.c: $(M32R_DEPS)
	@echo "ensuring $@ is up-to-date"
stamp-m32r: $(CGENDEPS) $(srcdir)/../cpu/m32r.cpu $(srcdir)/../cpu/m32r.opc
	$(MAKE) run-cgen arch=m32r prefix=m32r options=opinst \
		archfile=$(srcdir)/../cpu/m32r.cpu \
		opcfile=$(srcdir)/../cpu/m32r.opc extrafiles=opinst

$(srcdir)/fr30-desc.h $(srcdir)/fr30-desc.c $(srcdir)/fr30-opc.h $(srcdir)/fr30-opc.c $(srcdir)/fr30-ibld.c $(srcdir)/fr30-asm.c $(srcdir)/fr30-dis.c: $(FR30_DEPS)
	@echo "ensuring $@ is up-to-date"
stamp-fr30: $(CGENDEPS) $(CPUDIR)/fr30.cpu $(CPUDIR)/fr30.opc
	$(MAKE) run-cgen arch=fr30 prefix=fr30 options="" \
		archfile=$(CPUDIR)/fr30.cpu opcfile=$(CPUDIR)/fr30.opc extrafiles=""

$(srcdir)/frv-desc.h $(srcdir)/frv-desc.c $(srcdir)/frv-opc.h $(srcdir)/frv-opc.c $(srcdir)/frv-ibld.c $(srcdir)/frv-asm.c $(srcdir)/frv-dis.c: $(FRV_DEPS)
	@echo "ensuring $@ is up-to-date"
stamp-frv: $(CGENDEPS) $(srcdir)/../cpu/frv.cpu $(srcdir)/../cpu/frv.opc
	$(MAKE) run-cgen arch=frv prefix=frv options="" \
		archfile=$(srcdir)/../cpu/frv.cpu opcfile=$(srcdir)/../cpu/frv.opc extrafiles=""

$(srcdir)/ms1-desc.h $(srcdir)/ms1-desc.c $(srcdir)/ms1-opc.h $(srcdir)/ms1-opc.c $(srcdir)/ms1-ibld.c $(srcdir)/ms1-asm.c $(srcdir)/ms1-dis.c: $(MS1_DEPS)
	@echo "ensuring $@ is up-to-date"
stamp-ms1: $(CGENDEPS) $(srcdir)/../cpu/ms1.cpu $(srcdir)/../cpu/ms1.opc
	$(MAKE) run-cgen arch=ms1 prefix=ms1 options="" \
		archfile=$(srcdir)/../cpu/ms1.cpu \
		opcfile=$(srcdir)/../cpu/ms1.opc extrafiles=""

$(srcdir)/openrisc-desc.h $(srcdir)/openrisc-desc.c $(srcdir)/openrisc-opc.h $(srcdir)/openrisc-opc.c $(srcdir)/openrisc-ibld.c $(srcdir)/openrisc-asm.c $(srcdir)/openrisc-dis.c: $(OPENRISC_DEPS)
	@echo "ensuring $@ is up-to-date"
stamp-openrisc: $(CGENDEPS) $(CPUDIR)/openrisc.cpu $(CPUDIR)/openrisc.opc
	$(MAKE) run-cgen arch=openrisc prefix=openrisc options="" \
		archfile=$(CPUDIR)/openrisc.cpu opcfile=$(CPUDIR)/openrisc.opc extrafiles=""

$(srcdir)/iq2000-desc.h $(srcdir)/iq2000-desc.c $(srcdir)/iq2000-opc.h $(srcdir)/iq2000-opc.c $(srcdir)/iq2000-ibld.c $(srcdir)/iq2000-asm.c $(srcdir)/iq2000-dis.c: $(IQ2000_DEPS)
	@echo "ensuring $@ is up-to-date"
stamp-iq2000: $(CGENDEPS) $(srcdir)/../cpu/iq2000.cpu \
		$(srcdir)/../cpu/iq2000.opc $(srcdir)/../cpu/iq2000m.cpu \
		$(srcdir)/../cpu/iq10.cpu
	$(MAKE) run-cgen arch=iq2000 prefix=iq2000 options="" \
		archfile=$(srcdir)/../cpu/iq2000.cpu \
		opcfile=$(srcdir)/../cpu/iq2000.opc extrafiles=""

$(srcdir)/xstormy16-desc.h $(srcdir)/xstormy16-desc.c $(srcdir)/xstormy16-opc.h $(srcdir)/xstormy16-opc.c $(srcdir)/xstormy16-ibld.c $(srcdir)/xstormy16-asm.c $(srcdir)/xstormy16-dis.c: $(XSTORMY16_DEPS)
	@echo "ensuring $@ is up-to-date"
stamp-xstormy16: $(CGENDEPS) $(CPUDIR)/xstormy16.cpu $(CPUDIR)/xstormy16.opc
	$(MAKE) run-cgen arch=xstormy16 prefix=xstormy16 options="" \
		archfile=$(CPUDIR)/xstormy16.cpu opcfile=$(CPUDIR)/xstormy16.opc extrafiles=""

i386-gen: i386-gen.o @MAINT@ $(LIBIBERTY)
	$(AM_V_CCLD)$(LINK) i386-gen.o $(LIBIBERTY)

I386_COMMON_SRCS = i386-opc.c i386-opc.h sysdep.h $(INCDIR)/libiberty.h
i386-gen.o: i386-gen.c $(I386_COMMON_SRCS) \
  $(INCDIR)/getopt.h @MAINT@ $(INCDIR)/hashtab.h $(INCDIR)/safe-ctype.h 

i386-asmtab.c: @MAINT@ i386-gen i386-opc.tbl i386-reg.tbl
	$(AM_V_GEN)./i386-gen --srcdir $(srcdir) > $(srcdir)/i386-asmtab.c

I386_OPC_O_DEPS = $(I386_COMMON_SRCS) config.h \
  $(INCDIR)/ansidecl.h @MAINT@ $(INCDIR)/opcode/i386.h
i386-opc.o: $(I386_OPC_O_DEPS)
i386-opc.lo: $(I386_OPC_O_DEPS)
  
ia64-gen: ia64-gen.o @MAINT@ $(LIBIBERTY) @MAINT@ ia64-gen.lo
	$(AM_V_CCLD)$(LINK) ia64-gen.o $(LIBIBERTY)

IA64_GEN_O_DEPS = $(INCDIR)/ansidecl.h $(INCDIR)/libiberty.h \
  $(INCDIR)/safe-ctype.h sysdep.h config.h $(INCDIR)/getopt.h \
  $(INCDIR)/opcode/ia64.h $(BFD_H) $(INCDIR)/symcat.h ia64-opc.c \
  ia64-opc-a.c ia64-opc-b.c ia64-opc-f.c ia64-opc-i.c ia64-opc-m.c \
  ia64-opc-d.c ia64-opc.h @MAINT@ $(INTLDIR)/libintl.h

ia64-gen.o: ia64-gen.c @MAINT@# $(IA64_GEN_O_DEPS)

##FIXME: building ia64-gen.o is currently broken, so always disable
## rebuilding this file:
ia64-asmtab.c: @MAINT@# ia64-gen ia64-ic.tbl ia64-raw.tbl ia64-waw.tbl ia64-war.tbl
	$(AM_V_GEN)./ia64-gen --srcdir $(srcdir) > $(srcdir)/ia64-asmtab.c

s390-mkopc: s390-mkopc.c
	$(AM_V_CCLD)$(CC_FOR_BUILD) -o s390-mkopc $(srcdir)/s390-mkopc.c

s390-opc.tab: s390-mkopc s390-opc.txt
	$(AM_V_GEN)./s390-mkopc < $(srcdir)/s390-opc.txt > s390-opc.tab

sh-dis.lo: sh-dis.c sh-opc.h sysdep.h config.h
	$(AM_V_CC)$(LIBTOOL) --mode=compile $(COMPILE) -c @archdefs@ $<
sh-dis.o: sh-dis.lo

# This dependency stuff is copied from BFD.

# Have to get rid of any pre-existing DEP1 here, so that "$?" later
# includes all of $(CFILES).
DEP: dep.sed $(CFILES) $(HFILES) config.h
	rm -f $(AM_V_v) DEP1
	$(MAKE) MKDEP="$(MKDEP)" DEP1
	$(SED) -f dep.sed < DEP1 $(AM_V_redir) DEPA
	echo '# IF YOU PUT ANYTHING HERE IT WILL GO AWAY' >> DEPA
	if $(GREP) ' /' DEPA > /dev/null 2> /dev/null; then \
	  echo 'make DEP failed; it produced bad directories!'>&2; \
	  exit 1; \
	else \
	  mv -f $(AM_V_v) DEPA $@; \
	$(AM_V_at)fi

# Not sure if AM_CPPFLAGS works perfectly as a 1-for-1 replacement
# for INCLUDES here in this next one:
DEP1: $(CFILES)
	echo '# DO NOT DELETE THIS LINE -- mkdep uses it.' > DEP2
	echo '# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.' >> DEP2
	$(MKDEP) $(AM_CPPFLAGS) $(CFLAGS) $? | tee -a DEP2
	mv -f $(AM_V_v) DEP2 $@
	$(AM_V_at)touch $@

#FIXME: this sed script is broken and hack-ish:
dep.sed: dep-in.sed @MAINT@ config.status
	$(SED) <$(srcdir)/dep-in.sed >dep.sed	\
	  -e 's!@BFD_H@!$(BFD_H)!'	\
	  -e 's!@INCDIR@!$(INCDIR)!'	\
	  -e 's!@BFDDIR@!$(BFDDIR)!'	\
	  -e 's!@SRCDIR@!$(srcdir)!'	\
	  -e 's!@TOPDIR@!'`echo $(srcdir) | $(SED) -e s,/opcodes$$,,`'!'

#FIXME: this target relies on dep.sed, which is broken (see above)
dep: DEP @MAINT@ $(PATH_TO_MOVEIFCHANGE)
	$(SED) -e '/^..DO NOT DELETE THIS LINE/,$$d' < Makefile > tmp-Makefile
	cat DEP >> tmp-Makefile
	$(RUN_MOVEIFCHANGE) tmp-Makefile Makefile
	$(AM_V_at)touch Makefile

#FIXME: this target relies on dep.sed, which is broken (see above)
dep-in: DEP @MAINT@ $(PATH_TO_MOVEIFCHANGE)
	$(SED) -e '/^..DO NOT DELETE THIS LINE/,$$d' < $(srcdir)/Makefile.in > tmp-Makefile.in
	cat DEP >> tmp-Makefile.in
	$(RUN_MOVEIFCHANGE) tmp-Makefile.in $(srcdir)/Makefile.in
	$(AM_V_at)touch $(srcdir)/Makefile.in

#FIXME: this target relies on dep.sed, which is broken (see above)
dep-am: DEP @MAINT@ $(PATH_TO_MOVEIFCHANGE)
	@echo "this target is broken/dangerous; not letting you use it" >&2
	@echo "edit Makefile.am if you have fixed things"
	exit 1
	$(SED) -e '/^..DO NOT DELETE THIS LINE/,$$d' < $(srcdir)/Makefile.am > tmp-Makefile.am
	cat DEP >> tmp-Makefile.am
	$(RUN_MOVEIFCHANGE) tmp-Makefile.am $(srcdir)/Makefile.am
	$(AM_V_at)touch $(srcdir)/Makefile.am

.PHONY: dep dep-in dep-am

# What appears below is generated by a hacked mkdep using gcc -MM.
# FIXME: the sed script that generates this currently screws up some stuff
# such as incorrectly using the $(BFDDIR) variable, and removing letters
# and directory separators when they do not need to be removed
# (for example, the 't' in 'aout', the 2nd 'f' in 'coff', the 'f' in 'elf')
# Can edit manually until the sed script is fixed (?)

# DO NOT DELETE THIS LINE -- mkdep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.
a29k-dis.lo: a29k-dis.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/dis-asm.h $(BFD_H) $(INCDIR)/symcat.h $(INCDIR)/opcode/a29k.h
alpha-dis.lo: alpha-dis.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/dis-asm.h $(BFD_H) $(INCDIR)/symcat.h $(INCDIR)/opcode/alpha.h
alpha-opc.lo: alpha-opc.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/opcode/alpha.h $(BFD_H) $(INCDIR)/symcat.h \
  opintl.h $(INTLDIR)/libintl.h
arc-dis.lo: arc-dis.c $(INCDIR)/ansidecl.h $(INCDIR)/libiberty.h \
  $(INCDIR)/dis-asm.h $(BFD_H) $(INCDIR)/symcat.h $(INCDIR)/opcode/arc.h \
  $(BFDDIR)/elf-bfd.h $(INCDIR)/elf/common.h $(INCDIR)/elf/internal.h \
  $(INCDIR)/elf/external.h $(INCDIR)/bfdlink.h $(INCDIR)/elf/arc.h \
  $(INCDIR)/elf/reloc-macros.h opintl.h arc-dis.h arc-ext.h
arc-opc.lo: arc-opc.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(BFD_H) $(INCDIR)/symcat.h $(INCDIR)/opcode/arc.h \
  opintl.h $(INTLDIR)/libintl.h
arc-ext.lo: arc-ext.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(BFD_H) $(INCDIR)/symcat.h arc-ext.h $(INCDIR)/libiberty.h
arm-dis.lo: arm-dis.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/dis-asm.h $(BFD_H) $(INCDIR)/symcat.h $(INCDIR)/opcode/arm.h \
  opintl.h $(INCDIR)/safe-ctype.h $(INTLDIR)/libintl.h \
  $(INCDIR)/floatformat.h $(INCDIR)/ansidecl.h $(INCDIR)/coff/internal.h \
  $(BFDDIR)/libcoff.h $(INCDIR)/bfdlink.h $(BFDDIR)/elf-bfd.h \
  $(INCDIR)/elf/common.h $(INCDIR)/elf/internal.h $(INCDIR)/elf/external.h \
  $(INCDIR)/elf/arm.h $(INCDIR)/elf/reloc-macros.h
avr-dis.lo: avr-dis.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/dis-asm.h $(BFD_H) $(INCDIR)/symcat.h opintl.h \
  $(INCDIR)/libiberty.h $(INCDIR)/opcode/avr.h $(INTLDIR)/libintl.h
cgen-asm.lo: cgen-asm.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/libiberty.h $(INCDIR)/safe-ctype.h $(BFD_H) \
  $(INCDIR)/symcat.h $(INCDIR)/opcode/cgen.h opintl.h $(INTLDIR)/libintl.h
cgen-dis.lo: cgen-dis.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/libiberty.h $(BFD_H) $(INCDIR)/symcat.h $(INCDIR)/opcode/cgen.h
cgen-opc.lo: cgen-opc.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/libiberty.h $(INCDIR)/safe-ctype.h $(BFD_H) \
  $(INCDIR)/symcat.h $(INCDIR)/opcode/cgen.h
cris-dis.lo: cris-dis.c $(INCDIR)/dis-asm.h $(BFD_H) \
  $(INCDIR)/ansidecl.h $(INCDIR)/symcat.h sysdep.h config.h \
  $(INCDIR)/opcode/cris.h $(INCDIR)/libiberty.h
cris-opc.lo: cris-opc.c $(INCDIR)/opcode/cris.h
crx-dis.lo: crx-dis.c $(INCDIR)/dis-asm.h $(BFD_H) \
  $(INCDIR)/ansidecl.h $(INCDIR)/symcat.h sysdep.h config.h \
  $(INCDIR)/opcode/crx.h
crx-opc.lo: crx-opc.c $(INCDIR)/libiberty.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/symcat.h $(INCDIR)/opcode/crx.h
d10v-dis.lo: d10v-dis.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/opcode/d10v.h $(INCDIR)/dis-asm.h $(BFD_H) \
  $(INCDIR)/symcat.h
d10v-opc.lo: d10v-opc.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/opcode/d10v.h
d30v-dis.lo: d30v-dis.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/opcode/d30v.h $(INCDIR)/dis-asm.h $(BFD_H) \
  $(INCDIR)/symcat.h opintl.h $(INTLDIR)/libintl.h
d30v-opc.lo: d30v-opc.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/opcode/d30v.h
dlx-dis.lo: dlx-dis.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/dis-asm.h $(BFD_H) $(INCDIR)/symcat.h $(INCDIR)/opcode/dlx.h
dis-buf.lo: dis-buf.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/dis-asm.h $(BFD_H) $(INCDIR)/symcat.h opintl.h \
  $(INTLDIR)/libintl.h
dis-init.lo: dis-init.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/dis-asm.h $(BFD_H) $(INCDIR)/symcat.h
disassemble.lo: disassemble.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/dis-asm.h $(BFD_H) $(INCDIR)/symcat.h
fr30-asm.lo: fr30-asm.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(BFD_H) $(INCDIR)/symcat.h fr30-desc.h $(INCDIR)/opcode/cgen.h \
  fr30-opc.h opintl.h $(INTLDIR)/libintl.h \
  $(INCDIR)/libiberty.h $(INCDIR)/safe-ctype.h
fr30-desc.lo: fr30-desc.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(BFD_H) $(INCDIR)/symcat.h fr30-desc.h $(INCDIR)/opcode/cgen.h \
  fr30-opc.h opintl.h $(INTLDIR)/libintl.h $(INCDIR)/libiberty.h
fr30-dis.lo: fr30-dis.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/dis-asm.h $(BFD_H) $(INCDIR)/symcat.h $(INCDIR)/libiberty.h \
  fr30-desc.h $(INCDIR)/opcode/cgen.h fr30-opc.h opintl.h \
  $(INTLDIR)/libintl.h
fr30-ibld.lo: fr30-ibld.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/dis-asm.h $(BFD_H) $(INCDIR)/symcat.h fr30-desc.h \
  $(INCDIR)/opcode/cgen.h fr30-opc.h opintl.h $(INTLDIR)/libintl.h \
  $(INCDIR)/safe-ctype.h
fr30-opc.lo: fr30-opc.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(BFD_H) $(INCDIR)/symcat.h fr30-desc.h $(INCDIR)/opcode/cgen.h \
  fr30-opc.h $(INCDIR)/libiberty.h
frv-asm.lo: frv-asm.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(BFD_H) $(INCDIR)/symcat.h frv-desc.h $(INCDIR)/opcode/cgen.h \
  frv-opc.h opintl.h $(INTLDIR)/libintl.h \
  $(INCDIR)/libiberty.h $(INCDIR)/safe-ctype.h
frv-desc.lo: frv-desc.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(BFD_H) $(INCDIR)/symcat.h frv-desc.h $(INCDIR)/opcode/cgen.h \
  frv-opc.h opintl.h $(INTLDIR)/libintl.h $(INCDIR)/libiberty.h
frv-dis.lo: frv-dis.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/dis-asm.h $(BFD_H) $(INCDIR)/symcat.h $(INCDIR)/libiberty.h \
  frv-desc.h $(INCDIR)/opcode/cgen.h frv-opc.h opintl.h \
  $(INTLDIR)/libintl.h
frv-ibld.lo: frv-ibld.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/dis-asm.h $(BFD_H) $(INCDIR)/symcat.h frv-desc.h \
  $(INCDIR)/opcode/cgen.h frv-opc.h opintl.h $(INTLDIR)/libintl.h \
  $(INCDIR)/safe-ctype.h
frv-opc.lo: frv-opc.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(BFD_H) $(INCDIR)/symcat.h frv-desc.h $(INCDIR)/opcode/cgen.h \
  frv-opc.h $(INCDIR)/libiberty.h $(INCDIR)/elf/frv.h $(INCDIR)/elf/reloc-macros.h
h8300-dis.lo: h8300-dis.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/opcode/h8300.h $(INCDIR)/dis-asm.h $(BFD_H) $(INCDIR)/symcat.h \
  opintl.h $(INTLDIR)/libintl.h $(INCDIR)/libiberty.h
h8500-dis.lo: h8500-dis.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  h8500-opc.h $(INCDIR)/dis-asm.h $(BFD_H) $(INCDIR)/symcat.h \
  opintl.h $(INTLDIR)/libintl.h
hppa-dis.lo: hppa-dis.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/dis-asm.h $(BFD_H) $(INCDIR)/symcat.h $(BFDDIR)/libhppa.h \
  $(INCDIR)/opcode/hppa.h
i370-dis.lo: i370-dis.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/dis-asm.h $(BFD_H) $(INCDIR)/symcat.h $(INCDIR)/opcode/i370.h
i370-opc.lo: i370-opc.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/opcode/i370.h
i386-dis.lo: i386-dis.c $(INCDIR)/dis-asm.h $(BFD_H) \
  $(INCDIR)/ansidecl.h $(INCDIR)/symcat.h sysdep.h config.h \
  opintl.h $(INTLDIR)/libintl.h $(INCDIR)/opcode/i386.h
i386-opc.lo: i386-opc.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/libiberty.h i386-opc.h $(INCDIR)/opcode/i386.h
i860-dis.lo: i860-dis.c $(INCDIR)/dis-asm.h $(BFD_H) \
  $(INCDIR)/ansidecl.h $(INCDIR)/symcat.h $(INCDIR)/opcode/i860.h
i960-dis.lo: i960-dis.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/dis-asm.h $(BFD_H) $(INCDIR)/symcat.h
ia64-dis.lo: ia64-dis.c $(INCDIR)/dis-asm.h $(BFD_H) \
  $(INCDIR)/ansidecl.h $(INCDIR)/symcat.h $(INCDIR)/opcode/ia64.h
ia64-opc-a.lo: ia64-opc-a.c ia64-opc.h $(INCDIR)/opcode/ia64.h \
  $(BFD_H) $(INCDIR)/ansidecl.h $(INCDIR)/symcat.h
ia64-opc-b.lo: ia64-opc-b.c ia64-opc.h $(INCDIR)/opcode/ia64.h \
  $(BFD_H) $(INCDIR)/ansidecl.h $(INCDIR)/symcat.h
ia64-opc-f.lo: ia64-opc-f.c ia64-opc.h $(INCDIR)/opcode/ia64.h \
  $(BFD_H) $(INCDIR)/ansidecl.h $(INCDIR)/symcat.h
ia64-opc-i.lo: ia64-opc-i.c ia64-opc.h $(INCDIR)/opcode/ia64.h \
  $(BFD_H) $(INCDIR)/ansidecl.h $(INCDIR)/symcat.h
ia64-opc-m.lo: ia64-opc-m.c ia64-opc.h $(INCDIR)/opcode/ia64.h \
  $(BFD_H) $(INCDIR)/ansidecl.h $(INCDIR)/symcat.h
ia64-opc-d.lo: ia64-opc-d.c
ia64-opc.lo: ia64-opc.c $(INCDIR)/ansidecl.h sysdep.h \
  config.h $(INCDIR)/libiberty.h ia64-asmtab.h $(INCDIR)/opcode/ia64.h \
  $(BFD_H) $(INCDIR)/symcat.h ia64-asmtab.c
ia64-gen.lo: ia64-gen.c $(INCDIR)/ansidecl.h $(INCDIR)/libiberty.h \
  $(INCDIR)/safe-ctype.h sysdep.h config.h $(INCDIR)/getopt.h \
  ia64-opc.h $(INCDIR)/opcode/ia64.h $(BFD_H) $(INCDIR)/symcat.h \
  ia64-opc-a.c ia64-opc-i.c ia64-opc-m.c ia64-opc-b.c \
  ia64-opc-f.c ia64-opc-x.c ia64-opc-d.c $(INTLDIR)/libintl.h
ia64-asmtab.lo: ia64-asmtab.c
ip2k-asm.lo: ip2k-asm.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(BFD_H) $(INCDIR)/symcat.h ip2k-desc.h $(INCDIR)/opcode/cgen.h \
  ip2k-opc.h opintl.h $(INTLDIR)/libintl.h \
  $(INCDIR)/libiberty.h $(INCDIR)/safe-ctype.h
ip2k-desc.lo: ip2k-desc.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(BFD_H) $(INCDIR)/symcat.h ip2k-desc.h $(INCDIR)/opcode/cgen.h \
  ip2k-opc.h opintl.h $(INTLDIR)/libintl.h $(INCDIR)/libiberty.h
ip2k-dis.lo: ip2k-dis.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/dis-asm.h $(BFD_H) $(INCDIR)/symcat.h $(INCDIR)/libiberty.h \
  ip2k-desc.h $(INCDIR)/opcode/cgen.h ip2k-opc.h opintl.h \
  $(INTLDIR)/libintl.h
ip2k-ibld.lo: ip2k-ibld.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/dis-asm.h $(BFD_H) $(INCDIR)/symcat.h ip2k-desc.h \
  $(INCDIR)/opcode/cgen.h ip2k-opc.h opintl.h $(INTLDIR)/libintl.h \
  $(INCDIR)/safe-ctype.h
ip2k-opc.lo: ip2k-opc.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(BFD_H) $(INCDIR)/symcat.h ip2k-desc.h $(INCDIR)/opcode/cgen.h \
  ip2k-opc.h $(INCDIR)/libiberty.h $(INCDIR)/safe-ctype.h
iq2000-asm.lo: iq2000-asm.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(BFD_H) $(INCDIR)/symcat.h iq2000-desc.h $(INCDIR)/opcode/cgen.h \
  iq2000-opc.h opintl.h $(INTLDIR)/libintl.h \
  $(INCDIR)/libiberty.h $(INCDIR)/safe-ctype.h
iq2000-desc.lo: iq2000-desc.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(BFD_H) $(INCDIR)/symcat.h iq2000-desc.h $(INCDIR)/opcode/cgen.h \
  iq2000-opc.h opintl.h $(INTLDIR)/libintl.h $(INCDIR)/libiberty.h
iq2000-dis.lo: iq2000-dis.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/dis-asm.h $(BFD_H) $(INCDIR)/symcat.h $(INCDIR)/libiberty.h \
  iq2000-desc.h $(INCDIR)/opcode/cgen.h iq2000-opc.h \
  opintl.h $(INTLDIR)/libintl.h
iq2000-ibld.lo: iq2000-ibld.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/dis-asm.h $(BFD_H) $(INCDIR)/symcat.h iq2000-desc.h \
  $(INCDIR)/opcode/cgen.h iq2000-opc.h opintl.h $(INTLDIR)/libintl.h \
  $(INCDIR)/safe-ctype.h
iq2000-opc.lo: iq2000-opc.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(BFD_H) $(INCDIR)/symcat.h iq2000-desc.h $(INCDIR)/opcode/cgen.h \
  iq2000-opc.h $(INCDIR)/libiberty.h
m32c-asm.lo: m32c-asm.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(BFD_H) $(INCDIR)/symcat.h m32c-desc.h $(INCDIR)/opcode/cgen.h \
  m32c-opc.h cgen-types.h cgen-ops.h opintl.h $(INTLDIR)/libintl.h \
  $(INCDIR)/libiberty.h $(INCDIR)/safe-ctype.h
m32c-desc.lo: m32c-desc.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(BFD_H) $(INCDIR)/symcat.h m32c-desc.h $(INCDIR)/opcode/cgen.h \
  m32c-opc.h cgen-types.h cgen-ops.h opintl.h $(INTLDIR)/libintl.h \
  $(INCDIR)/libiberty.h
m32c-dis.lo: m32c-dis.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/dis-asm.h $(BFD_H) $(INCDIR)/symcat.h $(INCDIR)/libiberty.h \
  m32c-desc.h $(INCDIR)/opcode/cgen.h m32c-opc.h cgen-types.h \
  cgen-ops.h opintl.h $(INTLDIR)/libintl.h $(INCDIR)/elf/m32c.h \
  $(INCDIR)/elf/reloc-macros.h $(BFDDIR)/elf-bfd.h $(INCDIR)/elf/common.h \
  $(INCDIR)/elf/internal.h $(INCDIR)/elf/external.h $(INCDIR)/bfdlink.h
m32c-ibld.lo: m32c-ibld.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/dis-asm.h $(BFD_H) $(INCDIR)/symcat.h m32c-desc.h \
  $(INCDIR)/opcode/cgen.h m32c-opc.h cgen-types.h cgen-ops.h \
  opintl.h $(INTLDIR)/libintl.h $(INCDIR)/safe-ctype.h
m32c-opc.lo: m32c-opc.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(BFD_H) $(INCDIR)/symcat.h m32c-desc.h $(INCDIR)/opcode/cgen.h \
  m32c-opc.h cgen-types.h cgen-ops.h $(INCDIR)/libiberty.h
m32r-asm.lo: m32r-asm.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(BFD_H) $(INCDIR)/symcat.h m32r-desc.h $(INCDIR)/opcode/cgen.h \
  m32r-opc.h opintl.h $(INTLDIR)/libintl.h \
  $(INCDIR)/libiberty.h $(INCDIR)/safe-ctype.h
m32r-desc.lo: m32r-desc.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(BFD_H) $(INCDIR)/symcat.h m32r-desc.h $(INCDIR)/opcode/cgen.h \
  m32r-opc.h opintl.h $(INTLDIR)/libintl.h $(INCDIR)/libiberty.h
m32r-dis.lo: m32r-dis.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/dis-asm.h $(BFD_H) $(INCDIR)/symcat.h $(INCDIR)/libiberty.h \
  m32r-desc.h $(INCDIR)/opcode/cgen.h m32r-opc.h opintl.h \
  $(INTLDIR)/libintl.h
m32r-ibld.lo: m32r-ibld.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/dis-asm.h $(BFD_H) $(INCDIR)/symcat.h m32r-desc.h \
  $(INCDIR)/opcode/cgen.h m32r-opc.h opintl.h $(INTLDIR)/libintl.h \
  $(INCDIR)/safe-ctype.h
m32r-opc.lo: m32r-opc.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(BFD_H) $(INCDIR)/symcat.h m32r-desc.h $(INCDIR)/opcode/cgen.h \
  m32r-opc.h $(INCDIR)/libiberty.h
m32r-opinst.lo: m32r-opinst.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(BFD_H) $(INCDIR)/symcat.h m32r-desc.h $(INCDIR)/opcode/cgen.h \
  m32r-opc.h
m68hc11-dis.lo: m68hc11-dis.c $(INCDIR)/ansidecl.h \
  $(INCDIR)/opcode/m68hc11.h $(INCDIR)/dis-asm.h $(BFD_H) \
  $(INCDIR)/symcat.h
m68hc11-opc.lo: m68hc11-opc.c $(INCDIR)/ansidecl.h \
  $(INCDIR)/opcode/m68hc11.h
m68k-dis.lo: m68k-dis.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/dis-asm.h $(BFD_H) $(INCDIR)/symcat.h $(INCDIR)/floatformat.h \
  $(INCDIR)/libiberty.h opintl.h $(INTLDIR)/libintl.h \
  $(INCDIR)/opcode/m68k.h
m68k-opc.lo: m68k-opc.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/opcode/m68k.h
m88k-dis.lo: m88k-dis.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/dis-asm.h $(BFD_H) $(INCDIR)/symcat.h $(INCDIR)/opcode/m88k.h \
  opintl.h $(INTLDIR)/libintl.h $(INCDIR)/libiberty.h
maxq-dis.lo: maxq-dis.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/dis-asm.h $(BFD_H) $(INCDIR)/symcat.h $(INCDIR)/opcode/maxq.h
mcore-dis.lo: mcore-dis.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  mcore-opc.h $(INCDIR)/dis-asm.h $(BFD_H) $(INCDIR)/symcat.h
mips-dis.lo: mips-dis.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/dis-asm.h $(BFD_H) $(INCDIR)/symcat.h $(INCDIR)/libiberty.h \
  $(INCDIR)/opcode/mips.h opintl.h $(INTLDIR)/libintl.h $(BFDDIR)/elf-bfd.h \
  $(INCDIR)/elf/common.h $(INCDIR)/elf/internal.h \
  $(INCDIR)/elf/external.h $(INCDIR)/bfdlink.h $(INCDIR)/elf/mips.h \
  $(INCDIR)/elf/reloc-macros.h
mips-opc.lo: mips-opc.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/opcode/mips.h
mips16-opc.lo: mips16-opc.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/opcode/mips.h
m10200-dis.lo: m10200-dis.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/opcode/mn10200.h $(INCDIR)/dis-asm.h $(BFD_H) \
  $(INCDIR)/symcat.h opintl.h $(INTLDIR)/libintl.h
m10200-opc.lo: m10200-opc.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/opcode/mn10200.h
m10300-dis.lo: m10300-dis.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/opcode/mn10300.h $(INCDIR)/dis-asm.h $(BFD_H) \
  $(INCDIR)/symcat.h opintl.h $(INTLDIR)/libintl.h
m10300-opc.lo: m10300-opc.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/opcode/mn10300.h
mmix-dis.lo: mmix-dis.c $(INCDIR)/opcode/mmix.h $(INCDIR)/dis-asm.h \
  $(BFD_H) $(INCDIR)/ansidecl.h $(INCDIR)/symcat.h $(INCDIR)/libiberty.h \
  opintl.h
mmix-opc.lo: mmix-opc.c $(INCDIR)/opcode/mmix.h $(INCDIR)/symcat.h
ms1-asm.lo: ms1-asm.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(BFD_H) $(INCDIR)/symcat.h ms1-desc.h $(INCDIR)/opcode/cgen.h \
  ms1-opc.h opintl.h $(INTLDIR)/libintl.h \
  $(INCDIR)/libiberty.h $(INCDIR)/safe-ctype.h
ms1-desc.lo: ms1-desc.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(BFD_H) $(INCDIR)/symcat.h ms1-desc.h $(INCDIR)/opcode/cgen.h \
  ms1-opc.h opintl.h $(INTLDIR)/libintl.h $(INCDIR)/libiberty.h
ms1-dis.lo: ms1-dis.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/dis-asm.h $(BFD_H) $(INCDIR)/symcat.h $(INCDIR)/libiberty.h \
  ms1-desc.h $(INCDIR)/opcode/cgen.h ms1-opc.h opintl.h \
  $(INTLDIR)/libintl.h
ms1-ibld.lo: ms1-ibld.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/dis-asm.h $(BFD_H) $(INCDIR)/symcat.h ms1-desc.h \
  $(INCDIR)/opcode/cgen.h ms1-opc.h opintl.h $(INTLDIR)/libintl.h \
  $(INCDIR)/safe-ctype.h
ms1-opc.lo: ms1-opc.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(BFD_H) $(INCDIR)/symcat.h ms1-desc.h $(INCDIR)/opcode/cgen.h \
  ms1-opc.h $(INCDIR)/libiberty.h $(INCDIR)/safe-ctype.h
ns32k-dis.lo: ns32k-dis.c $(BFD_H) $(INCDIR)/ansidecl.h \
  $(INCDIR)/symcat.h sysdep.h config.h $(INCDIR)/dis-asm.h \
  $(INCDIR)/opcode/ns32k.h opintl.h $(INTLDIR)/libintl.h
openrisc-asm.lo: openrisc-asm.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(BFD_H) $(INCDIR)/symcat.h openrisc-desc.h $(INCDIR)/opcode/cgen.h \
  openrisc-opc.h opintl.h $(INTLDIR)/libintl.h \
  $(INCDIR)/libiberty.h $(INCDIR)/safe-ctype.h
openrisc-desc.lo: openrisc-desc.c sysdep.h config.h \
  $(INCDIR)/ansidecl.h $(BFD_H) $(INCDIR)/symcat.h openrisc-desc.h \
  $(INCDIR)/opcode/cgen.h openrisc-opc.h opintl.h $(INTLDIR)/libintl.h \
  $(INCDIR)/libiberty.h
openrisc-dis.lo: openrisc-dis.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/dis-asm.h $(BFD_H) $(INCDIR)/symcat.h $(INCDIR)/libiberty.h \
  openrisc-desc.h $(INCDIR)/opcode/cgen.h openrisc-opc.h \
  opintl.h $(INTLDIR)/libintl.h
openrisc-ibld.lo: openrisc-ibld.c sysdep.h config.h \
  $(INCDIR)/ansidecl.h $(INCDIR)/dis-asm.h $(BFD_H) $(INCDIR)/symcat.h \
  openrisc-desc.h $(INCDIR)/opcode/cgen.h openrisc-opc.h \
  opintl.h $(INTLDIR)/libintl.h $(INCDIR)/safe-ctype.h
openrisc-opc.lo: openrisc-opc.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(BFD_H) $(INCDIR)/symcat.h openrisc-desc.h $(INCDIR)/opcode/cgen.h \
  openrisc-opc.h $(INCDIR)/libiberty.h
or32-dis.lo: or32-dis.c $(INCDIR)/dis-asm.h $(BFD_H) \
  $(INCDIR)/ansidecl.h $(INCDIR)/symcat.h $(INCDIR)/opcode/or32.h \
  $(INCDIR)/safe-ctype.h
or32-opc.lo: or32-opc.c $(INCDIR)/safe-ctype.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/opcode/or32.h
pdp11-dis.lo: pdp11-dis.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/dis-asm.h $(BFD_H) $(INCDIR)/symcat.h $(INCDIR)/opcode/pdp11.h
pdp11-opc.lo: pdp11-opc.c $(INCDIR)/opcode/pdp11.h
pj-dis.lo: pj-dis.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/opcode/pj.h $(INCDIR)/dis-asm.h $(BFD_H) \
  $(INCDIR)/symcat.h
pj-opc.lo: pj-opc.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/opcode/pj.h
ppc-dis.lo: ppc-dis.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/dis-asm.h $(BFD_H) $(INCDIR)/symcat.h \
  $(INCDIR)/opcode/ppc.h
ppc-opc.lo: ppc-opc.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/opcode/ppc.h opintl.h $(INTLDIR)/libintl.h
s390-mkopc.lo: s390-mkopc.c
s390-opc.lo: s390-opc.c $(INCDIR)/ansidecl.h $(INCDIR)/opcode/s390.h \
  s390-opc.tab
s390-dis.lo: s390-dis.c $(INCDIR)/ansidecl.h sysdep.h \
  config.h $(INCDIR)/dis-asm.h $(BFD_H) $(INCDIR)/symcat.h \
  $(INCDIR)/opcode/s390.h
sh-dis.lo: sh-dis.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  sh-opc.h $(BFD_H) $(INCDIR)/symcat.h $(INCDIR)/dis-asm.h
sh64-dis.lo: sh64-dis.c $(INCDIR)/dis-asm.h $(BFD_H) \
  $(INCDIR)/ansidecl.h $(INCDIR)/symcat.h sysdep.h config.h \
  sh64-opc.h $(INCDIR)/libiberty.h $(BFDDIR)/elf-bfd.h \
  $(INCDIR)/elf/common.h $(INCDIR)/elf/internal.h \
  $(INCDIR)/elf/external.h $(INCDIR)/bfdlink.h $(INCDIR)/elf/sh.h \
  $(INCDIR)/elf/reloc-macros.h $(BFDDIR)/elf32-sh64.h
sh64-opc.lo: sh64-opc.c sh64-opc.h
sparc-dis.lo: sparc-dis.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/opcode/sparc.h $(INCDIR)/dis-asm.h $(BFD_H) \
  $(INCDIR)/symcat.h $(INCDIR)/libiberty.h opintl.h $(INTLDIR)/libintl.h
sparc-opc.lo: sparc-opc.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/opcode/sparc.h
tic30-dis.lo: tic30-dis.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/dis-asm.h $(BFD_H) $(INCDIR)/symcat.h $(INCDIR)/opcode/tic30.h
tic4x-dis.lo: tic4x-dis.c $(INCDIR)/libiberty.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/dis-asm.h $(BFD_H) $(INCDIR)/symcat.h $(INCDIR)/opcode/tic4x.h
tic54x-dis.lo: tic54x-dis.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/dis-asm.h $(BFD_H) $(INCDIR)/symcat.h $(INCDIR)/opcode/tic54x.h \
  $(INCDIR)/coff/tic54x.h $(INCDIR)/coff/ti.h
tic54x-opc.lo: tic54x-opc.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/dis-asm.h $(BFD_H) $(INCDIR)/symcat.h $(INCDIR)/opcode/tic54x.h
tic80-dis.lo: tic80-dis.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/opcode/tic80.h $(INCDIR)/dis-asm.h $(BFD_H) \
  $(INCDIR)/symcat.h
tic80-opc.lo: tic80-opc.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/opcode/tic80.h
v850-dis.lo: v850-dis.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/opcode/v850.h $(INCDIR)/dis-asm.h $(BFD_H) \
  $(INCDIR)/symcat.h opintl.h $(INTLDIR)/libintl.h
v850-opc.lo: v850-opc.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/opcode/v850.h opintl.h $(INTLDIR)/libintl.h
vax-dis.lo: vax-dis.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/opcode/vax.h $(INCDIR)/dis-asm.h $(BFD_H) \
  $(INCDIR)/symcat.h
w65-dis.lo: w65-dis.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  w65-opc.h $(INCDIR)/dis-asm.h $(BFD_H) $(INCDIR)/symcat.h
xstormy16-asm.lo: xstormy16-asm.c sysdep.h config.h \
  $(INCDIR)/ansidecl.h $(BFD_H) $(INCDIR)/symcat.h xstormy16-desc.h \
  $(INCDIR)/opcode/cgen.h xstormy16-opc.h opintl.h $(INTLDIR)/libintl.h \
  $(INCDIR)/libiberty.h $(INCDIR)/safe-ctype.h
xstormy16-desc.lo: xstormy16-desc.c sysdep.h config.h \
  $(INCDIR)/ansidecl.h $(BFD_H) $(INCDIR)/symcat.h xstormy16-desc.h \
  $(INCDIR)/opcode/cgen.h xstormy16-opc.h opintl.h $(INCDIR)/libiberty.h \
  $(INTLDIR)/libintl.h
xstormy16-dis.lo: xstormy16-dis.c sysdep.h config.h \
  $(INCDIR)/ansidecl.h $(INCDIR)/dis-asm.h $(BFD_H) $(INCDIR)/symcat.h \
  $(INCDIR)/libiberty.h xstormy16-desc.h $(INCDIR)/opcode/cgen.h \
  xstormy16-opc.h opintl.h $(INTLDIR)/libintl.h
xstormy16-ibld.lo: xstormy16-ibld.c sysdep.h config.h \
  $(INCDIR)/ansidecl.h $(INCDIR)/dis-asm.h $(BFD_H) $(INCDIR)/symcat.h \
  xstormy16-desc.h $(INCDIR)/opcode/cgen.h xstormy16-opc.h \
  opintl.h $(INTLDIR)/libintl.h $(INCDIR)/safe-ctype.h
xstormy16-opc.lo: xstormy16-opc.c sysdep.h config.h \
  $(INCDIR)/ansidecl.h $(BFD_H) $(INCDIR)/symcat.h xstormy16-desc.h \
  $(INCDIR)/opcode/cgen.h xstormy16-opc.h $(INCDIR)/libiberty.h
xtensa-dis.lo: xtensa-dis.c $(INCDIR)/xtensa-isa.h \
  $(INCDIR)/ansidecl.h $(INCDIR)/libiberty.h sysdep.h \
  config.h $(INCDIR)/dis-asm.h $(BFD_H) $(INCDIR)/symcat.h
z8k-dis.lo: z8k-dis.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/dis-asm.h $(BFD_H) $(INCDIR)/symcat.h z8k-opc.h
z8kgen.lo: z8kgen.c sysdep.h config.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/libiberty.h
# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
