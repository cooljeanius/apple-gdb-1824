## automake Makefile for bfd source
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

## might have to add back -Wno-portability, if I screwed up:
AUTOMAKE_OPTIONS = 1.11.6 no-dist gnits dejagnu -Wall -Wno-override -Wno-obsolete
## "cygnus" == "foreign no-dist" (we hardened from foreign to gnits though)
AUTOMAKE_DEPRECATED_OPTIONS = cygnus
TEXINFO_TEX = ../texinfo/texinfo.tex

ACLOCAL_AMFLAGS = -I m4 --install

# APPLE LOCAL dynamiclib building method:
NSLIBTOOL = cc -dynamiclib

# Uncomment the following line when doing a release.
#RELEASE=y

srcdir = @srcdir@

INCDIR = $(srcdir)/../include
CSEARCH = -Wp,-I. -Wp,-I$(srcdir) -Wp,-I$(INCDIR)
MKDEP = gcc -MM

PATH_TO_MOVEIFCHANGE = $(srcdir)/../move-if-change
RUN_MOVEIFCHANGE = $(SHELL) $(PATH_TO_MOVEIFCHANGE)

SUBDIRS = doc @MAINT@ po

bfddocdir = doc

sdocdir = $(srcdir)/doc
if INSTALL_LIBBFD
bfdlibdir = @bfdlibdir@
bfdincludedir = @bfdincludedir@
else !INSTALL_LIBBFD
## Empty these so that the respective installation directories will not be
## created:
bfdlibdir =
bfdincludedir =
endif

libbfddir = @bfdlibdir@
libbfdincludedir = @bfdincludedir@

my_bfdlib_LIBTOOL_ARCHIVE = libbfd.la
libbfd_LTLIBRARIES = libbfd.la

## also needed:
HOST_ARCHITECTURE = @HOST_ARCHITECTURE@
## not strictly necessary, but goes with the previous:
TARGET_ARCHITECTURE = @TARGET_ARCHITECTURE@
## use the host one, because that is the one that matters:
C_ARCHFLAGS = @C_ARCHFLAGS@

WARN_CFLAGS = @WARN_CFLAGS@ $(FLEXMEMBER_STRICTNESS_FLAG)
NO_WERROR = @NO_WERROR@
AM_CFLAGS = $(WARN_CFLAGS) $(WARN_WRITE_STRINGS) $(C_ARCHFLAGS)
AM_DEFS = $(WARN_DEFS)
MY_AM_CPPFLAGS_EXTRA = -DBINDIR='"$(bindir)"'
AM_WARNFLAGS = $(WARN_CFLAGS) $(WARN_WRITE_STRINGS)
AM_LIBTOOLFLAGS = --no-silent --verbose
AM_LIBTOOL_DEBUG_FLAGS = @MAINT@ --debug
if PLUGINS
LIBDL = @lt_cv_dlopen_libs@
endif

# bfd.h goes here, for now:
BFD_H = bfd.h

# Jim Kingdon notes:
# Writing S-records should be included in all (or at least most)
# *-*-coff, *-*-aout, etc., configurations, because people will want to
# be able to use objcopy to create S-records. (S-records are not useful
# for the debugger, so if you are downloading things as S-records, then you
# will need 2 copies of the executable: 1 to download & 1 for the debugger)
BFD32_LIBS = \
	archive.lo archures.lo bfd.lo bfdio.lo bfdwin.lo \
	cache.lo coffgen.lo corefile.lo \
	format.lo init.lo libbfd.lo opncls.lo reloc.lo \
	section.lo syms.lo targets.lo hash.lo linker.lo \
	srec.lo binary.lo tekhex.lo ihex.lo stabs.lo stab-syms.lo \
	merge.lo dwarf2.lo simple.lo @MAINT@ # compress.lo verilog.lo

BFD32_PLISTS = \
	archive.plist archures.plist bfd.plist bfdio.plist bfdwin.plist \
	cache.plist coffgen.plist corefile.plist \
	format.plist init.plist libbfd.plist opncls.plist reloc.plist \
	section.plist syms.plist targets.plist hash.plist linker.plist \
	srec.plist binary.plist tekhex.plist ihex.plist stabs.plist \
	stab-syms.plist merge.plist dwarf2.plist simple.plist
## to add to the above:
FUTURE_BFD32_LIBS_EXTRA_PLISTS = compress.plist verilog.plist

BFD64_LIBS = archive64.lo

BFD64_PLISTS = archive64.plist

BFD32_LIBS_CFILES = \
	archive.c archures.c bfd.c bfdio.c bfdwin.c \
	cache.c coffgen.c corefile.c \
	format.c init.c libbfd.c opncls.c reloc.c \
	section.c syms.c targets.c hash.c linker.c \
	srec.c binary.c tekhex.c ihex.c stabs.c stab-syms.c \
	merge.c dwarf2.c simple.c
## to add to the above:
FUTURE_BFD32_LIBS_EXTRA_CFILES = compress.c verilog.c

BFD64_LIBS_CFILES = archive64.c

# This list is alphabetized to make it easier to keep in sync with the
# decls and initializer in archures.c.
ALL_MACHINES = \
	cpu-aarch64.lo \
	cpu-a29k.lo \
	cpu-alpha.lo \
	cpu-arc.lo \
	cpu-arm.lo \
	cpu-avr.lo \
	cpu-bfin.lo \
	cpu-cr16.lo \
	cpu-cr16c.lo \
	cpu-cris.lo \
	cpu-crx.lo \
	cpu-d10v.lo \
	cpu-d30v.lo \
	cpu-dlx.lo \
	cpu-epiphany.lo \
	cpu-fr30.lo \
	cpu-frv.lo \
	cpu-h8300.lo \
	cpu-h8500.lo \
	cpu-hppa.lo \
	cpu-ia64.lo \
	cpu-i370.lo \
	cpu-i386.lo \
	cpu-i860.lo \
	cpu-i960.lo \
	cpu-ip2k.lo \
	cpu-iq2000.lo \
	cpu-k1om.lo \
	cpu-l1om.lo \
	cpu-lm32.lo \
	cpu-m32c.lo \
	cpu-m32r.lo \
	cpu-m68hc11.lo \
	cpu-m68hc12.lo \
	cpu-m68k.lo \
	cpu-m88k.lo \
	cpu-m9s12x.lo \
	cpu-m9s12xg.lo \
	cpu-m10200.lo \
	cpu-m10300.lo \
	cpu-maxq.lo \
	cpu-mcore.lo \
	cpu-mep.lo \
	cpu-metag.lo \
	cpu-microblaze.lo \
	cpu-mips.lo \
	cpu-mmix.lo \
	cpu-moxie.lo \
	cpu-ms1.lo \
	cpu-msp430.lo \
	cpu-mt.lo \
	cpu-nds32.lo \
	cpu-nios2.lo \
	cpu-ns32k.lo \
	cpu-openrisc.lo \
	cpu-or1k.lo \
	cpu-or32.lo \
	cpu-pdp11.lo \
	cpu-pj.lo \
	cpu-plugin.lo \
	cpu-powerpc.lo \
	cpu-rl78.lo \
	cpu-rs6000.lo \
	cpu-rx.lo \
	cpu-s390.lo \
	cpu-score.lo \
	cpu-sh.lo \
	cpu-sparc.lo \
	cpu-spu.lo \
	cpu-tic30.lo \
	cpu-tic4x.lo \
	cpu-tic54x.lo \
	cpu-tic6x.lo \
	cpu-tic80.lo \
	cpu-tilegx.lo \
	cpu-tilepro.lo \
	cpu-v850.lo \
	cpu-v850_rh850.lo \
	cpu-vax.lo \
	cpu-we32k.lo \
	cpu-w65.lo \
	cpu-xc16x.lo \
	cpu-xgate.lo \
	cpu-xstormy16.lo \
	cpu-xtensa.lo \
	cpu-z80.lo \
	cpu-z8k.lo

# Source files corresponding to the previous libtool objects for all:
ALL_MACHINES_CFILES = \
	cpu-aarch64.c \
	cpu-a29k.c \
	cpu-alpha.c \
	cpu-arc.c \
	cpu-arm.c \
	cpu-avr.c \
	cpu-bfin.c \
	cpu-cris.c \
	cpu-cr16.c \
	cpu-cr16c.c \
	cpu-crx.c \
	cpu-d10v.c \
	cpu-d30v.c \
	cpu-dlx.c \
	cpu-epiphany.c \
	cpu-fr30.c \
	cpu-frv.c \
	cpu-h8300.c \
	cpu-h8500.c \
	cpu-hppa.c \
	cpu-ia64.c \
	cpu-i370.c \
	cpu-i386.c \
	cpu-i860.c \
	cpu-i960.c \
	cpu-ip2k.c \
	cpu-iq2000.c \
	cpu-k1om.c \
	cpu-l1om.c \
	cpu-lm32.c \
	cpu-m32c.c \
	cpu-m32r.c \
	cpu-m68hc11.c \
	cpu-m68hc12.c \
	cpu-m68k.c \
	cpu-m88k.c \
	cpu-m9s12x.c \
	cpu-m9s12xg.c \
	cpu-m10200.c \
	cpu-m10300.c \
	cpu-maxq.c \
	cpu-mcore.c \
	cpu-mep.c \
	cpu-metag.c \
	cpu-microblaze.c \
	cpu-mips.c \
	cpu-mmix.c \
	cpu-moxie.c \
	cpu-ms1.c \
	cpu-msp430.c \
	cpu-mt.c \
	cpu-nds32.c \
	cpu-nios2.c \
	cpu-ns32k.c \
	cpu-openrisc.c \
	cpu-or1k.c \
	cpu-or32.c \
	cpu-pdp11.c \
	cpu-pj.c \
	cpu-plugin.c \
	cpu-powerpc.c \
	cpu-rl78.c \
	cpu-rs6000.c \
	cpu-rx.c \
	cpu-s390.c \
	cpu-score.c \
	cpu-sh.c \
	cpu-sparc.c \
	cpu-spu.c \
	cpu-tic30.c \
	cpu-tic4x.c \
	cpu-tic54x.c \
	cpu-tic6x.c \
	cpu-tic80.c \
	cpu-tilegx.c \
	cpu-tilepro.c \
	cpu-v850.c \
	cpu-v850_rh850.c \
	cpu-vax.c \
	cpu-we32k.c \
	cpu-w65.c \
	cpu-xc16x.c \
	cpu-xgate.c \
	cpu-xstormy16.c \
	cpu-xtensa.c \
	cpu-z80.c \
	cpu-z8k.c

# The .o files needed by all of the 32 bit vectors that are configured into
# target_vector in targets.c if configured with --enable-targets=all.
BFD32_BACKENDS = \
	aout-adobe.lo \
	aout-arm.lo \
	aout-cris.lo \
	aout-ns32k.lo \
	aout-sparcle.lo \
	aout-tic30.lo \
	aout0.lo \
	aout32.lo \
	armnetbsd.lo \
	bout.lo \
	cf-i386lynx.lo \
	cf-m68klynx.lo \
	cf-sparclynx.lo \
	coff-a29k.lo \
	coff-apollo.lo \
	coff-arm.lo \
	coff-aux.lo \
	coff-h8300.lo \
	coff-h8500.lo \
	coff-i386.lo \
	coff-go32.lo \
	coff-i860.lo \
	coff-i960.lo \
	coff-m68k.lo \
	coff-m88k.lo \
	coff-maxq.lo \
	coff-mips.lo \
	coff-or32.lo \
	coff-pmac.lo \
	coff-rs6000.lo \
	coff-sh.lo \
	coff-sparc.lo \
	coff-stgo32.lo \
	coff-svm68k.lo \
	coff-tic30.lo \
	coff-tic4x.lo \
	coff-tic54x.lo \
	coff-tic80.lo \
	coff-u68k.lo \
	coff-we32k.lo \
	coff-w65.lo \
	coff-z80.lo \
	coff-z8k.lo \
	cofflink.lo \
	dwarf1.lo \
	ecoff.lo \
	ecofflink.lo \
	efi-app-ia32.lo \
	elf.lo \
	elf32-am33lin.lo \
	elf32-arc.lo \
	elf32-arm.lo \
	elf32-avr.lo \
	elf32-bfin.lo \
	elf32-cr16.lo \
	elf32-cr16c.lo \
	elf32-cris.lo \
	elf32-crx.lo \
	elf32-d10v.lo \
	elf32-d30v.lo \
	elf32-dlx.lo \
	elf32-epiphany.lo \
	elf32-fr30.lo \
	elf32-frv.lo \
	elf32-gen.lo \
	elf32-h8300.lo \
	elf32-hppa.lo \
	elf32-i370.lo \
	elf32-i386.lo \
	elf32-i860.lo \
	elf32-i960.lo \
	elf32-ip2k.lo \
	elf32-iq2000.lo \
	elf32-lm32.lo \
	elf32-m32c.lo \
	elf32-m32r.lo \
	elf32-m68hc11.lo \
	elf32-m68hc12.lo \
	elf32-m68hc1x.lo \
	elf32-m68k.lo \
	elf32-m88k.lo \
	elf-m10200.lo \
	elf-m10300.lo \
	elf32-mcore.lo \
	elf32-mep.lo \
	elf32-metag.lo \
	elf32-microblaze.lo \
	elfxx-mips.lo \
	elf32-mips.lo \
	elf32-moxie.lo \
	elf32-ms1.lo \
	elf32-msp430.lo \
	elf32-mt.lo \
	elf32-nds32.lo \
	elf32-nios2.lo \
	elf32-openrisc.lo \
	elf32-or1k.lo \
	elf32-or32.lo \
	elf32-pj.lo \
	elf32-ppc.lo \
	elf32-rl78.lo \
	elf32-rx.lo \
	elf32-s390.lo \
	elf32-sh.lo \
	elf32-sh-symbian.lo \
	elf32-sh64.lo \
	elf32-sh64-com.lo \
	elfxx-sparc.lo \
	elf32-sparc.lo \
	elf32-spu.lo \
	elf32-tic6x.lo \
	elf32-tilegx.lo \
	elf32-tilepro.lo \
	elf32-v850.lo \
	elf32-vax.lo \
	elf32-xc16x.lo \
	elf32-xgate.lo \
	elf32-xstormy16.lo \
	elf32-xtensa.lo \
	elf32.lo \
	elflink.lo \
	elf-attrs.lo \
	elf-eh-frame.lo \
	elf-ifunc.lo \
	elf-nacl.lo \
	elf-strtab.lo \
	elf-vxworks.lo \
	epoc-pe-arm.lo \
	epoc-pei-arm.lo \
	hp300bsd.lo \
	hp300hpux.lo \
	i386aout.lo \
	i386bsd.lo \
	i386dynix.lo \
	i386freebsd.lo \
	i386linux.lo \
	i386lynx.lo \
	i386msdos.lo \
	i386netbsd.lo \
	i386mach3.lo \
	i386os9k.lo \
	ieee.lo \
	m68k4knetbsd.lo \
	m68klinux.lo \
	m68klynx.lo \
	m68knetbsd.lo \
	m88kmach3.lo \
	m88kopenbsd.lo \
	mach-o.lo \
	mach-o-i386.lo \
	mipsbsd.lo \
	newsos3.lo \
	nlm.lo \
	nlm32-i386.lo \
	nlm32-sparc.lo \
	nlm32-ppc.lo \
	nlm32.lo \
	ns32knetbsd.lo \
	oasys.lo \
	pc532-mach.lo \
	pdp11.lo \
	pef.lo \
	pe-arm.lo \
	pei-arm.lo \
	pe-arm-wince.lo \
	pei-arm-wince.lo \
	pe-i386.lo \
	pei-i386.lo \
	pe-mcore.lo \
	pei-mcore.lo \
	pe-ppc.lo \
	pei-ppc.lo \
	pe-sh.lo \
	pei-sh.lo \
	pe-mips.lo \
	pei-mips.lo \
	peigen.lo \
	plugin.lo \
	ppcboot.lo \
	reloc16.lo \
	riscix.lo \
	som.lo \
	sparclinux.lo \
	sparclynx.lo \
	sparcnetbsd.lo \
	sunos.lo \
	vaxnetbsd.lo \
	vax1knetbsd.lo \
	vaxbsd.lo \
	versados.lo \
	vms.lo \
	vms-gsd.lo \
	vms-hdr.lo \
	vms-lib.lo \
	vms-misc.lo \
	vms-tir.lo \
	xcofflink.lo \
	xsym.lo \
	xtensa-isa.lo \
	xtensa-modules.lo

# Source files corresponding to the previous libtool objects for bfd32:
BFD32_BACKENDS_CFILES = \
	aout-adobe.c \
	aout-arm.c \
	aout-cris.c \
	aout-ns32k.c \
	aout-sparcle.c \
	aout-tic30.c \
	aout0.c \
	aout32.c \
	armnetbsd.c \
	bout.c \
	cf-i386lynx.c \
	cf-m68klynx.c \
	cf-sparclynx.c \
	coff-a29k.c \
	coff-apollo.c \
	coff-arm.c \
	coff-aux.c \
	coff-h8300.c \
	coff-h8500.c \
	coff-i386.c \
	coff-i860.c \
	coff-go32.c \
	coff-i960.c \
	coff-m68k.c \
	coff-m88k.c \
	coff-maxq.c \
	coff-mips.c \
	coff-or32.c \
	coff-pmac.c \
	coff-rs6000.c \
	coff-sh.c \
	coff-sparc.c \
	coff-stgo32.c \
	coff-svm68k.c \
	coff-tic30.c \
	coff-tic4x.c \
	coff-tic54x.c \
	coff-tic80.c \
	coff-u68k.c \
	coff-we32k.c \
	coff-w65.c \
	coff-z80.c \
	coff-z8k.c \
	cofflink.c \
	dwarf1.c \
	ecoff.c \
	ecofflink.c \
	efi-app-ia32.c \
	elf.c \
	elf32-am33lin.c \
	elf32-arc.c \
	elf32-arm.c \
	elf32-avr.c \
	elf32-bfin.c \
	elf32-cr16.c \
	elf32-cr16c.c \
	elf32-cris.c \
	elf32-crx.c \
	elf32-d10v.c \
	elf32-d30v.c \
	elf32-dlx.c \
	elf32-epiphany.c \
	elf32-fr30.c \
	elf32-frv.c \
	elf32-gen.c \
	elf32-h8300.c \
	elf32-hppa.c \
	elf32-i370.c \
	elf32-i386.c \
	elf32-i860.c \
	elf32-i960.c \
	elf32-ip2k.c \
	elf32-iq2000.c \
	elf32-lm32.c \
	elf32-m32c.c \
	elf32-m32r.c \
	elf32-m68k.c \
	elf32-m68hc11.c \
	elf32-m68hc12.c \
	elf32-m68hc1x.c \
	elf32-m88k.c \
	elf-m10200.c \
	elf-m10300.c \
	elf32-mcore.c \
	elf32-mep.c \
	elf32-metag.c \
	elf32-microblaze.c \
	elfxx-mips.c \
	elf32-mips.c \
	elf32-moxie.c \
	elf32-ms1.c \
	elf32-msp430.c \
	elf32-mt.c \
	elf32-nds32.c \
	elf32-nios2.c \
	elf32-openrisc.c \
	elf32-or1k.c \
	elf32-or32.c \
	elf32-pj.c \
	elf32-ppc.c \
	elf32-rl78.c \
	elf32-rx.c \
	elf32-sh64.c \
	elf32-sh64-com.c \
	elf32-s390.c \
	elf32-sh.c \
	elf32-sh-symbian.c \
	elfxx-sparc.c \
	elf32-sparc.c \
	elf32-spu.c \
	elf32-tic6x.c \
	elf32-tilegx.c \
	elf32-tilepro.c \
	elf32-v850.c \
	elf32-vax.c \
	elf32-xc16x.c \
	elf32-xgate.c \
	elf32-xstormy16.c \
	elf32-xtensa.c \
	elf32.c \
	elflink.c \
	elf-attrs.c \
	elf-eh-frame.c \
	elf-ifunc.c \
	elf-nacl.c \
	elf-strtab.c \
	elf-vxworks.c \
	epoc-pe-arm.c \
	epoc-pei-arm.c \
	hp300bsd.c \
	hp300hpux.c \
	i386aout.c \
	i386bsd.c \
	i386dynix.c \
	i386freebsd.c \
	i386linux.c \
	i386lynx.c \
	i386msdos.c \
	i386netbsd.c \
	i386mach3.c \
	i386os9k.c \
	ieee.c \
	m68k4knetbsd.c \
	m68klinux.c \
	m68knetbsd.c \
	m88kmach3.c \
	m88kopenbsd.c \
	mach-o.c \
	mach-o-i386.c \
	mipsbsd.c \
	newsos3.c \
	nlm.c \
	nlm32-i386.c \
	nlm32-sparc.c \
	nlm32-ppc.c \
	nlm32.c \
	ns32knetbsd.c \
	oasys.c \
	pc532-mach.c \
	pdp11.c \
	pef.c \
	pe-arm.c \
	pei-arm.c \
	pe-arm-wince.c \
	pei-arm-wince.c \
	pe-i386.c \
	pei-i386.c \
	pe-mcore.c \
	pei-mcore.c \
	pe-ppc.c \
	pei-ppc.c \
	pe-sh.c \
	pei-sh.c \
	pe-mips.c \
	pei-mips.c \
	plugin.c \
	ppcboot.c \
	reloc16.c \
	riscix.c \
	som.c \
	sparclinux.c \
	sparclynx.c \
	sparcnetbsd.c \
	sunos.c \
	vaxnetbsd.c \
	vax1knetbsd.c \
	vaxbsd.c \
	versados.c \
	vms.c \
	vms-gsd.c \
	vms-hdr.c \
	vms-lib.c \
	vms-misc.c \
	vms-tir.c \
	xcofflink.c \
	xsym.c \
	xtensa-isa.c \
	xtensa-modules.c

# The .o files needed by all of the 64 bit vectors that are configured into
# target_vector in targets.c if configured with --enable-targets=all
# and --enable-64-bit-bfd.
# elf32-ia64.c requires a 64-bit bfd_vma, and hence can not be put in
# BFD32_BACKENDS.
BFD64_BACKENDS = \
	aix5ppc-core.lo \
	aout64.lo \
	coff-alpha.lo \
	coff-x86_64.lo \
	coff64-rs6000.lo \
	demo64.lo \
	efi-app-ia64.lo \
	elf64-x86-64.lo \
	elf32-aarch64.lo \
	elf64-aarch64.lo \
	elfxx-aarch64.lo \
	elf64-alpha.lo \
	elf64-hppa.lo \
	elf32-ia64.lo \
	elf64-ia64.lo \
	elf64-ia64-vms.lo \
	elf64-gen.lo \
	elf32-mips.lo \
	elfn32-mips.lo \
	elf64-mips.lo \
	elf64-mmix.lo \
	elf64-sh64.lo \
	elf64-ppc.lo \
	elf64-s390.lo \
	elf32-score.lo \
	elf32-score7.lo \
	elf64-sh64.lo \
	elf64-sparc.lo \
	elf64-tilegx.lo \
	elf64-x86-64.lo \
	elf64.lo \
	elfxx-ia64.lo \
	elfxx-mips.lo \
	mach-o-x86-64.lo \
	mmo.lo \
	nlm32-alpha.lo \
	nlm64.lo \
	pe-x86_64.lo \
	pei-ia64.lo \
	pei-x86_64.lo \
	pepigen.lo \
	pex64igen.lo \
	vms-alpha.lo

# Source files corresponding to the previous libtool objects for bfd64:
BFD64_BACKENDS_CFILES = \
	aix5ppc-core.c \
	aout64.c \
	coff-alpha.c \
	coff-x86_64.c \
	coff64-rs6000.c \
	demo64.c \
	efi-app-ia64.c \
	elf32-mips.c \
	elf32-score.c \
	elf32-score7.c \
	elf64-x86-64.c \
	elf64-alpha.c \
	elf64-hppa.c \
	elf64-ia64-vms.c \
	elf64-gen.c \
	elfn32-mips.c \
	elf64-mips.c \
	elf64-mmix.c \
	elf64-ppc.c \
	elf64-s390.c \
	elf64-sh64.c \
	elf64-sparc.c \
	elf64-tilegx.c \
	elf64-x86-64.c \
	elf64.c \
	elfxx-aarch64.c \
	elfxx-mips.c \
	mach-o-x86-64.c \
	mmo.c \
	nlm32-alpha.c \
	nlm64.c \
	pe-x86_64.c \
	pei-ia64.c \
	pei-x86_64.c \
	vms-alpha.c
## to add to the above:
FUTURE_BFD64_BACKENDS_EXTRA_CFILES = elfxx-ia64.c

# Other misc. object files:
OPTIONAL_BACKENDS = \
	aix386-core.lo \
	cisco-core.lo \
	hpux-core.lo \
	irix-core.lo \
	lynx-core.lo \
	osf-core.lo \
	rs6000-core.lo \
	sco5-core.lo \
	trad-core.lo

## corresponding plists generated by clang static analysis:
OPTIONAL_BACKENDS_PLISTS = \
	aix386-core.plist \
	cisco-core.plist \
	hpux-core.plist \
	irix-core.plist \
	lynx-core.plist \
	osf-core.plist \
	rs6000-core.plist \
	sco5-core.plist \
	trad-core.plist

# Source files corresponding to the previous optional libtool objects:
OPTIONAL_BACKENDS_CFILES = \
	aix386-core.c \
	cisco-core.c \
	hpux-core.c \
	irix-core.c \
	lynx-core.c \
	osf-core.c \
	rs6000-core.c \
	sco5-core.c \
	trad-core.c

# We want to rerun configure if configure.ac, config.bfd, or
# configure.host change. configure.ac is needed since the version
# number in Makefile comes from configure.ac.
# development.sh is used to determine -Werror default.
CONFIG_STATUS_DEPENDENCIES = \
	$(srcdir)/configure.ac \
	$(srcdir)/config.bfd \
	$(srcdir)/configure.host \
	$(srcdir)/development.sh

# These are defined by configure.ac:
WORDSIZE = @wordsize@
ALL_BACKENDS = @all_backends@
BFD_BACKENDS = @bfd_backends@
BFD_LIBS = @bfd_libs@
BFD_MACHINES = @bfd_machines@
TDEFAULTS = @tdefaults@
HAVEVECS = @havevecs@

INCINTL = @INCINTL@

AM_CPPFLAGS = @HDEFINES@ @COREFLAG@ @TDEFINES@ $(CSEARCH) $(CSWITCHES) \
	$(HAVEVECS) $(INCINTL) $(AM_DEFS)

RUN_CLANG_ANALYZER = $(CLANG_ANALYZER) $(DEFS) $(CPPFLAGS) $(CFLAGS) \
  $(CSEARCH) $(HAVEVECS) $(INCINTL) --analyze @MAINT@ -Weverything -ferror-limit=1

# C source files that correspond to .o's.
SOURCE_CFILES = \
	$(BFD32_LIBS_CFILES) \
	$(BFD64_LIBS_CFILES) \
	$(ALL_MACHINES_CFILES) \
	$(BFD32_BACKENDS_CFILES) \
	$(BFD64_BACKENDS_CFILES) \
	$(OPTIONAL_BACKENDS_CFILES)

# C source files that are generated:
BUILD_CFILES = \
	elf32-aarch64.c elf64-aarch64.c \
	elf32-ia64.c elf64-ia64.c peigen.c pepigen.c pex64igen.c

CFILES = $(SOURCE_CFILES) $(BUILD_CFILES)

## This is a list of all .h files which are in the source tree.
SOURCE_HFILES = \
	aout-target.h aoutf1.h aoutx.h coffcode.h coffswap.h ecoffswap.h \
	elf-bfd.h elf-hppa.h elf-linux-psinfo.h elf32-hppa.h \
	elf64-hppa.h elfcode.h elfcore.h \
	freebsd.h genlink.h go32stub.h \
	libaout.h libbfd.h libcoff.h libecoff.h libhppa.h libieee.h \
	libnlm.h liboasys.h libpei.h libxcoff.h mach-o.h \
	netbsd.h nlm-target.h nlmcode.h nlmswap.h ns32k.h \
	pef.h pef-traceback.h peicode.h som.h trad-user.h version.h \
	vms.h xcoff-target.h xsym.h

## ... and all .h files which are in the build tree.
BUILD_HFILES = \
	bfdver.h elf32-target.h elf64-target.h targmatch.h bfd_stdint.h

# Ensure they are built early:
MY_BUILT_SOURCES_COMMENTED_OUT = $(BUILD_HFILES) $(BUILD_CFILES)
BUILT_SOURCES = $(BUILD_HFILES) $(BUILD_CFILES)

# headers:
HFILES = $(SOURCE_HFILES) $(BUILD_HFILES)
noinst_HEADERS = $(HFILES)

## hacks to extend the verbosity control:
AM_V_redir = $(am__v_redir_@AM_V@)
am__v_redir_ = $(am__v_redir_@AM_DEFAULT_V@)
am__v_redir_0 = >
am__v_redir_1 = | tee

AM_V_redir_a = $(am__v_redir_a_@AM_V@)
am__v_redir_a_ = $(am__v_redir_a_@AM_DEFAULT_V@)
am__v_redir_a_0 = >>
am__v_redir_a_1 = | tee -a

AM_V_v = $(am__v_v_@AM_V@)
am__v_v_ = $(am__v_v_@AM_DEFAULT_V@)
am__v_v_0 =
am__v_v_1 = -v

AM_V_MFLAG = $(am__v_MFLAG_@AM_V@)
am__v_MFLAG_ = $(am__v_MFLAG_@AM_DEFAULT_V@)
am__v_MFLAG_0 = -s
am__v_MFLAG_1 = -w

AM_V_CLANG = $(am__v_CLANG_@AM_V@)
am__v_CLANG_ = $(am__v_CLANG_@AM_DEFAULT_V@)
am__v_CLANG_0 = @echo "  CLANG      " $@;
am__v_CLANG_1 =

# files to check for translations (I think?):
SRC_POTFILES = $(SOURCE_CFILES) $(SOURCE_HFILES)
BLD_POTFILES = $(BUILD_CFILES) $(BUILD_HFILES)

## where to put clang static analysis plists:
analysisdir = $(builddir)/clang_static_analysis

SUFFIXES = .c .o .lo .plist

.c.plist:
	@if test ! -d $(analysisdir); then $(MKDIR_P) $(analysisdir); fi
	$(AM_V_CLANG)$(RUN_CLANG_ANALYZER) -o $(analysisdir)/$@ $<

po/SRC-POTFILES.in: @MAINT@ Makefile $(SRC_POTFILES)
	$(AM_V_GEN)if test ! -e tmp.src; then touch tmp.src; fi; \
	for file in $(SRC_POTFILES); do echo $${file}; done \
	  | LC_ALL=C LC_COLLATE="" sort $(AM_V_redir) tmp.src \
	  && mv $(AM_V_v) tmp.src $(srcdir)/po/SRC-POTFILES.in

po/BLD-POTFILES.in: @MAINT@ Makefile $(BLD_POTFILES)
	$(AM_V_GEN)if test ! -e tmp.bld; then touch tmp.bld; fi; \
	for file in $(BLD_POTFILES); do echo $${file}; done \
	  | LC_ALL=C LC_COLLATE="" sort $(AM_V_redir) tmp.bld \
	  && mv $(AM_V_v) tmp.bld $(srcdir)/po/BLD-POTFILES.in

all: info @MAINT@ config.status
.PHONY: all info info-am

diststuff: info @MAINT@ config.status
	find . -name *.info -print
.PHONY: diststuff

## currently succeeds, both with and without -Werror:
all_machines: $(ALL_MACHINES)
	@echo "this target, '$@' (in `pwd`), just makes its dependencies, '$^'"
.PHONY: all_machines

## currently breaks on elf32-bfin.c, both with and without -Werror:
bfd32_backends: $(BFD32_BACKENDS)
	@echo "this target, '$@' (in `pwd`), just makes its dependencies, '$^'"
.PHONY: bfd32_backends

## currently breaks on elf64-x86-64.c with -Werror, elf32-aarch64.c without:
bfd64_backends: $(BFD64_BACKENDS)
	@echo "this target, '$@' (in `pwd`), just makes its dependencies, '$^'"
.PHONY: bfd64_backends

## currently breaks on sco5-core.c, both with and without -Werror:
optional_backends: $(OPTIONAL_BACKENDS)
	@echo "this target, '$@' (in `pwd`), just makes its dependencies, '$^'"
.PHONY: optional_backends

# dummy target that does nothing, not even an "echo"; for potential future
# use by the configure script:
nothing:
	@true
.PHONY: nothing

# Various kinds of .o files to put in libbfd.a:
# BFD_BACKENDS	Routines the configured targets need.
# BFD_MACHINES	Architecture-specific routines the configured targets need.
# COREFILE	Core file routines for a native configuration
# bfd64_libs	Routines for 64bit support
# first they are libtool objects:
LOFILES = $(BFD_BACKENDS) $(BFD_MACHINES) @COREFILE@ @bfd64_libs@
# so need to transform to normal ones:
OFILES = `echo $(LOFILES) | $(SED) -e 's/\.lo/\.o/g'`
# We can use the same method to get a list of clang static analyzer plists
# to make:
PLISTFILES = `echo $(LOFILES) | $(SED) -e 's/\.lo/\.plist/g'`
# ... and also:
PLISTFILES_FROM_CFILES = `echo $(CFILES) | $(SED) -e 's/\.c/\.plist/g'`

stamp-lofiles: Makefile @MAINT@ $(PATH_TO_MOVEIFCHANGE)
	$(AM_V_at)rm -f $(AM_V_v) tlofiles
	$(AM_V_at)f=""; \
	for i in $(LOFILES); do \
	  case " $${f} " in \
	    *" $${i} "*) ;; \
	    *) f="$${f} $${i}" ;; \
	  esac; \
	done; \
	if test ! -e tlofiles; then touch tlofiles; fi; \
	echo $${f} $(AM_V_redir) tlofiles
	$(RUN_MOVEIFCHANGE) tlofiles lofiles
	$(AM_V_at)if test stamp-lofiles -ot lofiles; then touch stamp-lofiles; fi

stamp-ofiles: Makefile @MAINT@ $(PATH_TO_MOVEIFCHANGE)
	$(AM_V_at)rm -f $(AM_V_v) tofiles
	$(AM_V_at)f=""; \
	for i in $(OFILES); do \
	  case " $${f} " in \
	    *" $${i} "*) ;; \
	    *) f="$${f} $${i}" ;; \
	  esac; \
	done; \
	if test ! -e tofiles; then touch tofiles; fi; \
	echo $${f} $(AM_V_redir) tofiles
	$(RUN_MOVEIFCHANGE) tofiles ofiles
	$(AM_V_at)if test stamp-ofiles -ot ofiles; then touch stamp-ofiles; fi

stamp-plistfiles: Makefile @MAINT@ $(PATH_TO_MOVEIFCHANGE)
	$(AM_V_at)rm -f $(AM_V_v) tplistfiles
	$(AM_V_at)f=""; \
	for i in $(PLISTFILES); do \
	  case " $${f} " in \
	    *" $${i} "*) ;; \
	    *) f="$${f} $${i}" ;; \
	  esac; \
	done; \
	if test ! -e tplistfiles; then touch tplistfiles; fi; \
	echo $${f} $(AM_V_redir) tplistfiles
	$(RUN_MOVEIFCHANGE) tplistfiles plistfiles
	$(AM_V_at)if test stamp-plistfiles -ot plistfiles; then \
	  touch stamp-plistfiles; \
	fi

lofiles: stamp-lofiles
	@echo "this target, '$@' (in `pwd`), just makes its dependency, '$<'"
ofiles: stamp-ofiles
	@echo "this target, '$@' (in `pwd`), just makes its dependency, '$<'"
plistfiles: stamp-plistfiles
	@echo "this target, '$@' (in `pwd`), just makes its dependency, '$<'"

@ifGNUmake@ifneq ($(srcdir),.)
@ifGNUmake@$(srcdir)/lofiles: stamp-lofiles
@ifGNUmake@	if test -e lofiles && test ! -e $(srcdir)/lofiles; then \
@ifGNUmake@	  cp $(AM_V_v) lofiles $(srcdir)/lofiles; \
@ifGNUmake@	fi
@ifGNUmake@$(srcdir)/ofiles: stamp-ofiles
@ifGNUmake@	if test -e ofiles && test ! -e $(srcdir)/ofiles; then \
@ifGNUmake@	  cp $(AM_V_v) ofiles $(srcdir)/ofiles; \
@ifGNUmake@	fi
@ifGNUmake@endif

# Since BFD64_LIBS is optional, and we cannot have substitution in
# libbfd_la_SOURCES, we put BFD64_LIBS in OFILES instead.
# However, list all sources in EXTRA_libbfd_la_SOURCES so the
# dependency tracking fragments are picked up in the Makefile.
libbfd_la_SOURCES = $(BFD32_LIBS_CFILES) $(BFD64_LIBS_CFILES)
MY_EXTRA_libbfd_la_SOURCES_COMMENTED_OUT = $(CFILES)
EXTRA_libbfd_la_SOURCES = $(CFILES)
libbfd_la_DEPENDENCIES = $(LOFILES) stamp-lofiles lofiles $(srcdir)/lofiles
libbfd_la_LIBADD = `cat lofiles` @WIN32LIBADD@ $(LIBDL)
libbfd_la_LDFLAGS = -release $(VERSION) @WIN32LDFLAGS@

SHARED_LIBADD = @SHARED_LIBADD@
SHARED_LDFLAGS = @SHARED_LDFLAGS@
LIBTOOL_SOVERSION = `cat libtool-soversion`

# libtool will build .libs/libbfd.a.  We create libbfd.a in the build
# directory so that we do NOT have to convert all the programs that use
# libbfd.a simultaneously. This is a hack which should be removed if
# everything else starts using libtool. FIXME.

noinst_LIBRARIES = libbfd.a
libbfd_a_SOURCES = libbfd.h $(BFD32_LIBS_CFILES) $(BFD64_LIBS_CFILES)
MY_EXTRA_libbfd_a_SOURCES_COMMENTED_OUT = $(BFD_H) $(CFILES)
EXTRA_libbfd_a_SOURCES = bfd.h $(CFILES)
libbfd_a_DEPENDENCIES = stamp-ofiles ofiles $(srcdir)/ofiles @MAINT@ bfd.h
## '$(OFILES)' does NOT work as a dependency here any more, now that
## it is actually a command, so use 'stamp-staticlib' for it instead:
EXTRA_libbfd_a_DEPENDENCIES = stamp-staticlib @MAINT@ Makefile # $(BFD_H)
libbfd_a_LIBADD = `cat ofiles` @WIN32LIBADD@
## see if this will cause automake to give its objfiles a different name:
libbfd_a_CFLAGS = $(AM_CFLAGS) @MAINT@ $(GTOGGLE)
libbfd_a_LDFLAGS = -static

# FIXME: Even the hack fails now... (does it still? I think we are okay...)
stamp-lib: libbfd.la @MAINT@ ofiles $(PATH_TO_MOVEIFCHANGE) ltmain.sh
	if test ! -e stamp-staticlib; then $(MAKE) $(AM_V_MFLAG) stamp-staticlib; fi
	libtooldir=`$(LIBTOOL) --config | $(SED) -n -e 's/^objdir=//p'`; \
	if test -z "$${libtooldir}"; then \
	  echo "" && echo "$@ target: error: libtooldir is unset" >&2 && exit 1; \
	else \
	  echo "$@ target: libtooldir is $${libtooldir}"; \
	fi; \
	stat $${libtooldir}/libbfd.a; \
	if [ -f $${libtooldir}/libbfd.a ]; then \
	  cp $(AM_V_v) $${libtooldir}/libbfd.a libbfd.tmp; \
	  $(RANLIB) libbfd.tmp; \
	  $(RUN_MOVEIFCHANGE) libbfd.tmp libbfd.a; \
	else \
	  echo "$@ target: $${libtooldir}/libbfd.a does not exist as a file"; \
	  if [ -f libbfd.a ]; then \
	    $(RANLIB) libbfd.a; \
	  fi; \
	fi
	$(AM_V_at)if test $@ -ot stamp-staticlib; then touch stamp-lib; fi

# since OFILES no longer works as a dependency, have to make it explicitly
# as part of a rule instead:
stamp-staticlib: stamp-ofiles @MAINT@ bfd.h
	$(AM_V_at)rm -f $(AM_V_v) stamp-staticlib
	if test ! -e $(BFD_H); then $(MAKE) $(AM_V_MFLAG) $(BFD_H); fi
	$(MAKE) $(AM_V_MFLAG) $(OFILES)
	if test -n "$(libbfd_a_OBJECTS)"; then \
	  $(MAKE) $(AM_V_MFLAG) $(libbfd_a_OBJECTS); \
	fi
	if [ -f libbfd.a ]; then \
	  $(RANLIB) libbfd.a; \
	elif [ -f $(srcdir)/libbfd.a ]; then \
	  cp $(AM_V_v) $(srcdir)/libbfd.a .; \
	elif [ -f $(builddir)/libbfd.a ]; then \
	  cp $(AM_V_v) $(builddir)/libbfd.a .; \
	else \
	  echo "$@ target: warning: missing libbfd.a" >&2; \
	fi
	$(AM_V_at)if test ! -e $@ || test $@ -ot libbfd.a; then touch $@; fi

# likewise with making PLISTFILES when running `make check`:
check-local: Makefile stamp-plistfiles @MAINT@ plistfiles
	for myplist in $(BFD32_PLISTS) $(BFD64_PLISTS) $(PLISTFILES); do \
	  echo "$@: using $(CLANG_ANALYZER) to make $(analysisdir)/$${myplist}"; \
	  $(MAKE) $(AM_V_MFLAG) $${myplist} || stat $(analysisdir)/$${myplist}; \
	done
	@echo "$@ target (in `pwd`): done."
.PHONY: check-local

check-everything: Makefile stamp-plistfiles @MAINT@ plistfiles
	for myplist in $(PLISTFILES_FROM_CFILES); do \
	  export mysubplist="$(analysisdir)/$${myplist}"; \
	  if test ! -e $${mysubplist} || test $${mysubplist} -ot stamp-plistfiles || test $${mysubplist} -ot plistfiles || test $${mysubplist} -ot Makefile; then \
	    printf "\n"; \
	    echo "$@: using $(CLANG_ANALYZER) to make $${mysubplist}..."; \
	    $(MAKE) $(AM_V_MFLAG) $${myplist} || \
	      (if test -d $(analysisdir); then \
	         stat $${mysubplist} || (test -x "`which sync`" && sync); \
	       else \
	         stat $${myplist} || (test -x "`which sync`" && sync); \
	       fi); \
	  elif test "x$(AM_V_P)" != "xfalse"; then \
	    stat $${mysubplist}|| (test -x "`which sync`" && sync); \
	  else \
	    printf "."; \
	  fi; \
	done
	@printf "\n" && echo "$@ target (in `pwd`): done."
.PHONY: check-everything

# stamp-staticlib just makes the OFILES that libbfd.a depends upon, need
# to make the 'stamp-lib' target instead to actually make the static
# library:
libbfd_static_library: stamp-lib
	@echo "this target, '$@' (in `pwd`), just makes its dependency, '$<'"
.PHONY: libbfd_static_library

# hmmm...
libbfd_la: libbfd.la
	@echo "this target, '$@' (in `pwd`), just makes its dependency, '$<'"
.PHONY: libbfd_la
libbfd_a: libbfd.a
	@echo "this target, '$@' (in `pwd`), just makes its dependency, '$<'"
.PHONY: libbfd_a

# This file holds an array associating configuration triplets and
# vector names. It is built from config.bfd. It is not compiled by
# itself, but is included by targets.c.
targmatch.h: config.bfd $(srcdir)/targmatch.sed @MAINT@ config.status
	$(AM_V_at)rm -f $(AM_V_v) targmatch.h
	echo "/* $@ is generated from multiple files */" > targmatch.new
	$(SED) -f $(srcdir)/targmatch.sed < $(srcdir)/config.bfd >> targmatch.new
	mv -f $(AM_V_v) targmatch.new targmatch.h

# When compiling archures.c and targets.c, supply the default target
# info from configure.

targets.lo: targets.c @MAINT@ Makefile ltmain.sh
	$(AM_V_CC)$(LIBTOOL) $(AM_V_lt) --tag=CC $(AM_LIBTOOLFLAGS) \
	  $(LIBTOOLFLAGS) --mode=compile $(COMPILE) -c $(TDEFAULTS) \
	  $(srcdir)/targets.c
targets.o: targets.lo
libbfd_a-targets.o: targets.c @MAINT@ Makefile
	$(AM_V_CC)$(COMPILE) -c $(TDEFAULTS) $(srcdir)/targets.c -o $@

libbfd_a-archive.o: archive.c @MAINT@ Makefile config.h bfd.h
	$(AM_V_CC)$(COMPILE) -c $(TDEFAULTS) $(srcdir)/archive.c -o $@

archures.lo: archures.c @MAINT@ Makefile ltmain.sh
	$(AM_V_CC)$(LIBTOOL) $(AM_V_lt) --tag=CC $(AM_LIBTOOLFLAGS) \
	  $(LIBTOOLFLAGS) --mode=compile $(COMPILE) -c $(TDEFAULTS) \
	  $(srcdir)/archures.c
archures.o: archures.lo
libbfd_a-archures.o: archures.c @MAINT@ Makefile bfd.h
	$(AM_V_CC)$(COMPILE) -c $(TDEFAULTS) $(srcdir)/archures.c -o $@

dwarf2.lo: dwarf2.c @MAINT@ Makefile ltmain.sh
if am__fastdepCC
	$(AM_V_CC)$(LTCOMPILE) -MT $@ -MD -MP -MF $(DEPDIR)/$*.Tpo -c \
	  -o $@ -DDEBUGDIR=\"$(DEBUGDIR)\" $(srcdir)/dwarf2.c
	$(AM_V_at)$(am__mv) $(DEPDIR)/$*.Tpo $(DEPDIR)/$*.Plo
else
if AMDEP
	source='dwarf2.c' object='$@' libtool=yes @AMDEPBACKSLASH@
	DEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) @AMDEPBACKSLASH@
endif
	$(LTCOMPILE) -c -o $@ -DDEBUGDIR=\"$(DEBUGDIR)\" $(srcdir)/dwarf2.c
endif
dwarf2.o: dwarf2.lo
libbfd_a-dwarf2.o: dwarf2.c @MAINT@ Makefile
	$(AM_V_CC)$(COMPILE) -c -o $@ -DDEBUGDIR=\"$(DEBUGDIR)\" \
	  $(srcdir)/dwarf2.c

elf32-target.h : elfxx-target.h
	$(AM_V_at)rm -f $(AM_V_v) $@
	echo "/* DO NOT EDIT!  -*- buffer-read-only: t -*-  */" > elf32-target.new
	echo "/* $@ is generated from $< */" >> elf32-target.new
	$(SED) -e s/NN/32/g < $(srcdir)/elfxx-target.h >> elf32-target.new
	mv -f $(AM_V_v) elf32-target.new $@

elf64-target.h : elfxx-target.h
	$(AM_V_at)rm -f $(AM_V_v) $@
	echo "/* DO NOT EDIT!  -*- buffer-read-only: t -*-  */" > elf64-target.new
	echo "/* $@ is generated from $< */" >> elf64-target.new
	$(SED) -e s/NN/64/g < $(srcdir)/elfxx-target.h >> elf64-target.new
	mv -f $(AM_V_v) elf64-target.new $@

elf32-aarch64.c : elfnn-aarch64.c
	$(AM_V_at)rm -f $(AM_V_v) $@
	echo "#line 1 \"$(srcdir)/elfnn-aarch64.c\"" > elf32-aarch64.new
	$(SED) -e s/NN/32/g < $(srcdir)/elfnn-aarch64.c >>elf32-aarch64.new
	mv -f $(AM_V_v) elf32-aarch64.new $@

elf64-aarch64.c : elfnn-aarch64.c
	$(AM_V_at)rm -f $(AM_V_v) $@
	echo "#line 1 \"$(srcdir)/elfnn-aarch64.c\"" > elf64-aarch64.new
	$(SED) -e s/NN/64/g < $(srcdir)/elfnn-aarch64.c >>elf64-aarch64.new
	mv -f $(AM_V_v) elf64-aarch64.new $@

elf32-ia64.c : elfxx-ia64.c
	$(AM_V_at)rm -f $(AM_V_v) $@
	echo "/* DO NOT EDIT!  -*- buffer-read-only: t -*-  */" > elf32-ia64.new
	echo "/* $@ is generated from $< */" >> elf32-ia64.new
	$(SED) -e s/NN/32/g < $(srcdir)/elfxx-ia64.c >> elf32-ia64.new
	mv -f $(AM_V_v) elf32-ia64.new $@

elf64-ia64.c : elfxx-ia64.c
	$(AM_V_at)rm -f $(AM_V_v) $@
	echo "/* DO NOT EDIT!  -*- buffer-read-only: t -*-  */" > elf64-ia64.new
	echo "/* $@ is generated from $< */" >> elf64-ia64.new
	$(SED) -e s/NN/64/g < $(srcdir)/elfxx-ia64.c >> elf64-ia64.new
	mv -f $(AM_V_v) elf64-ia64.new $@

peigen.c : peXXigen.c
	$(AM_V_at)rm -f $(AM_V_v) $@
	echo "/* DO NOT EDIT!  -*- buffer-read-only: t -*-  */" > peigen.new
	echo "/* $@ is generated from $< */" >> peigen.new
	$(SED) -e s/XX/pe/g < $(srcdir)/peXXigen.c >> peigen.new
	mv -f $(AM_V_v) peigen.new $@

pepigen.c : peXXigen.c
	$(AM_V_at)rm -f $(AM_V_v) $@
	echo "/* DO NOT EDIT!  -*- buffer-read-only: t -*-  */" > pepigen.new
	echo "/* $@ is generated from $< */" >> pepigen.new
	$(SED) -e s/XX/pep/g < $(srcdir)/peXXigen.c >> pepigen.new
	mv -f $(AM_V_v) pepigen.new $@

pex64igen.c : peXXigen.c
	$(AM_V_at)rm -f $(AM_V_v) $@
	echo "/* DO NOT EDIT!  -*- buffer-read-only: t -*-  */" > pex64igen.new
	echo "/* $@ is generated from $< */" >> pex64igen.new
	$(SED) -e s/XX/pex64/g < $(srcdir)/peXXigen.c >> pex64igen.new
	mv -f $(AM_V_v) pex64igen.new pex64igen.c

BFD_H_DEPS = $(INCDIR)/ansidecl.h $(INCDIR)/symcat.h
LOCAL_H_DEPS = libbfd.h sysdep.h config.h
COMMON_BFD_DEPS = $(BFD_H) $(BFD_H_DEPS) $(LOCAL_H_DEPS)

# These targets had all previously been together;
# splitting them up to ease debugging this Makefile
$(BFD32_LIBS): $(COMMON_BFD_DEPS)
$(BFD64_LIBS): $(COMMON_BFD_DEPS)
$(ALL_MACHINES): $(COMMON_BFD_DEPS)
##FIXME: BFD32_BACKENDS might still be broken because of libtool(?)
$(BFD32_BACKENDS): $(COMMON_BFD_DEPS)
$(BFD64_BACKENDS): $(COMMON_BFD_DEPS)
$(OPTIONAL_BACKENDS): $(COMMON_BFD_DEPS)

## NORMAL_INSTALL is a special automake variable:
install_my_bfdlibLTLIBRARIES: @INSTALL_LIBBFD_TRUE@install_libbfd
	$(AM_V_at)$(NORMAL_INSTALL)
.PHONY: install_my_bfdlibLTLIBRARIES

## NORMAL_UNINSTALL is a special automake variable:
uninstall_my_bfdlibLTLIBRARIES: @INSTALL_LIBBFD_TRUE@uninstall_libbfd
	$(AM_V_at)$(NORMAL_UNINSTALL)
.PHONY: uninstall_my_bfdlibLTLIBRARIES

.PHONY: install_libbfd uninstall_libbfd
.PHONY: all-am
install_libbfd: $(bfdlib_LTLIBRARIES) $(BFD_H) @MAINT@ ltmain.sh
	$(mkinstalldirs) $(DESTDIR)$(bfdlibdir)
	$(mkinstalldirs) $(DESTDIR)$(bfdincludedir)
	$(AM_V_at)list='$(bfdlib_LTLIBRARIES)'; for p in $${list}; do \
	  if test -f $${p}; then \
	    echo "$(LIBTOOL) --mode=install $(INSTALL) $${p} $(DESTDIR)$(bfdlibdir)/$${p}"; \
	    $(LIBTOOL) --mode=install $(INSTALL) $${p} $(DESTDIR)$(bfdlibdir)/$${p}; \
	  else echo "$@: $${p} is not an extant file, so not installing"; fi; \
	done
	$(INSTALL_DATA) $(BFD_H) $(DESTDIR)$(bfdincludedir)/bfd.h
	$(INSTALL_DATA) $(INCDIR)/ansidecl.h $(DESTDIR)$(bfdincludedir)/ansidecl.h
	$(INSTALL_DATA) $(INCDIR)/symcat.h $(DESTDIR)$(bfdincludedir)/symcat.h
	$(INSTALL_DATA) $(INCDIR)/bfdlink.h $(DESTDIR)$(bfdincludedir)/bfdlink.h

uninstall_libbfd: @MAINT@ ltmain.sh
	list='$(bfdlib_LTLIBRARIES)'; for p in $${list}; do \
	  $(LIBTOOL) --mode=uninstall rm -f $(AM_V_v) $(DESTDIR)$(bfdlibdir)/$${p}; \
	done
	rm -f $(AM_V_v) $(DESTDIR)$(bfdincludedir)/bfd.h
	rm -f $(AM_V_v) $(DESTDIR)$(bfdincludedir)/ansidecl.h
	rm -f $(AM_V_v) $(DESTDIR)$(bfdincludedir)/symcat.h
	rm -f $(AM_V_v) $(DESTDIR)$(bfdincludedir)/bfdlink.h

# Have to get rid of any pre-existing DEP1 here, so that "$?" later
# includes all of $(CFILES).
DEP: dep.sed $(CFILES) $(HFILES) bfd.h
	$(AM_V_at)rm -f $(AM_V_v) DEP1
	$(MAKE) $(AM_V_MFLAG) MKDEP="$(MKDEP)" DEP1
	$(SED) -f dep.sed < DEP1 $(AM_V_redir) DEPA
	echo '# IF YOU PUT ANYTHING HERE IT WILL GO AWAY' >> DEPA
	if $(GREP) ' /' DEPA > /dev/null 2>/dev/null; then \
	  echo 'make DEP failed; it produced bad directories!'>&2; \
	  exit 1; \
	else \
	  mv -f $(AM_V_v) DEPA $@; \
	fi
	$(AM_V_at)if test $@ -ot dep.sed; then touch $@; fi
# I also want to include a check for variables not being closed properly
# (missing parentheses), but I am not sure how to do that in a way where
# the bad syntax for which we would be grepping does not also get
# evaluated and treated as an error...

# Not sure if AM_CPPFLAGS works perfectly as a 1-for-1 replacement
# for INCLUDES here in this next one:
DEP1: $(CFILES)
	echo '# DO NOT DELETE THIS LINE -- mkdep uses it.' > DEP2
	echo '# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.' >>DEP2
	$(MKDEP) $(AM_CPPFLAGS) $(CFLAGS) $? $(AM_V_redir_a) DEP2
	mv -f $(AM_V_v) DEP2 $@
	$(AM_V_at)if test $@ -ot DEP2; then touch $@; fi

#FIXME: this sed script is broken and hack-ish:
dep.sed: dep-in.sed @MAINT@ config.status
	$(SED) <$(srcdir)/dep-in.sed >dep.sed	\
		-e 's!@BFD_H@!$(BFD_H)!'	\
		-e 's!@SRCDIR@!$(srcdir)!'	\
		-e 's!@TOPDIR@!'`echo $(srcdir) | $(SED) -e s,/bfd$$,,`'!'
	$(AM_V_at)if test $@ -ot dep-in.sed; then touch $@; fi

#FIXME: this target relies on dep.sed, which is broken (see above)
dep: DEP @MAINT@ $(PATH_TO_MOVEIFCHANGE)
	$(SED) -e '/^..DO NOT DELETE THIS LINE/,$$d' < Makefile > tmp-Makefile
	cat DEP >> tmp-Makefile
	$(RUN_MOVEIFCHANGE) tmp-Makefile Makefile
	$(AM_V_at)if test Makefile -ot DEP; then touch Makefile; fi

#FIXME: this target relies on dep.sed, which is broken (see above)
dep-in: DEP @MAINT@ $(PATH_TO_MOVEIFCHANGE)
	$(SED) -e '/^..DO NOT DELETE THIS LINE/,$$d' < $(srcdir)/Makefile.in > tmp-Makefile.in
	cat DEP >> tmp-Makefile.in
	$(RUN_MOVEIFCHANGE) tmp-Makefile.in $(srcdir)/Makefile.in
	$(AM_V_at)if test $(srcdir)/Makefile.in -ot DEP; then \
	  touch $(srcdir)/Makefile.in; \
	fi

#FIXME: this target relies on dep.sed, which is broken (see above)
dep-am: DEP @MAINT@ $(PATH_TO_MOVEIFCHANGE)
	@echo "this target ($@) is broken/dangerous; not letting you use it" >&2
	@echo "edit Makefile.am manually if you have fixed things (in `pwd`)"
	exit 1
	$(SED) -e '/^..DO NOT DELETE THIS LINE/,$$d' < $(srcdir)/Makefile.am > tmp-Makefile.am
	cat DEP >> tmp-Makefile.am
	$(RUN_MOVEIFCHANGE) tmp-Makefile.am $(srcdir)/Makefile.am
	$(AM_V_at)if test $(srcdir)/Makefile.am -ot DEP; then \
	  touch $(srcdir)/Makefile.am; \
	fi

# Some manually-specified dependencies:
archive64.o: archive64.lo
host-aout.lo: Makefile
host-aout.o: host-aout.lo
mach-o.lo: $(BFD_H) bfd.h bfd-in2.h bfd-in3.h mach-o-target.c \
  bfd_stdint.h
mach-o.o: mach-o.lo
pef.o: pef.lo
xsym.o: xsym.lo
cpu-i386.o: cpu-i386.lo
cpu-powerpc.o: cpu-powerpc.lo
cpu-rs6000.o: cpu-rs6000.lo

# The following program can be used to generate a simple config file
# which can be folded into an h-XXX file for a new host, with some editing.
aout-params.h: gen-aout @MAINT@ config.status
	./gen-aout host $(AM_V_redir) aout-params.h
gen-aout: $(srcdir)/gen-aout.c @MAINT@ Makefile
	$(AM_V_CCLD)$(CC) -o gen-aout $(AM_CFLAGS) $(CFLAGS) $(LFLAGS) \
	  $(srcdir)/gen-aout.c

$(BFD_H): stmp-bfd-h $(srcdir)/bfd-in2.h bfd-in3.h $(srcdir)/libbfd.h \
  $(srcdir)/libcoff.h @MAINT@ $(srcdir)/Makefile
	$(AM_V_at)if test -e $(srcdir)/Makefile -a -d $(srcdir)/.deps; then \
	  echo "$@ target: making headers in srcdir ($(srcdir))..."; \
	  (cd $(srcdir) && $(MAKE) $(AM_V_MFLAG) headers $(FLAGS_TO_PASS)); \
	elif test -e ./Makefile -a -d ./.deps; then \
	  echo "$@ target: making headers in the current directory (`pwd`)..."; \
	  (cd . && $(MAKE) $(AM_V_MFLAG) headers $(FLAGS_TO_PASS)); \
	elif test -e $(builddir)/Makefile -a -d $(builddir)/.deps; then \
	  echo "$@ target: making headers in builddir ($(builddir))..."; \
	  (cd $(builddir) && $(MAKE) $(AM_V_MFLAG) headers $(FLAGS_TO_PASS)); \
	else \
	  echo "$@ target: unsure where to make headers..."; \
	fi
	$(AM_V_at)-if test ! -e $(BFD_H); then \
	  if test -e $(srcdir)/Makefile; then \
	    (cd $(srcdir) && $(MAKE) $(AM_V_MFLAG) stmp-bfd-h $(FLAGS_TO_PASS)); \
	  elif test -e ./Makefile; then \
	    (cd . && $(MAKE) $(AM_V_MFLAG) stmp-bfd-h $(FLAGS_TO_PASS)); \
	  elif test -d ../../.git && test -x "`which git`"; then \
	    if test -n "`git status -s $(BFD_H) | grep D`"; then \
	      echo "BFD_H ($(BFD_H)) appears to have been deleted; attempting to restore it..."; \
	      git restore $(BFD_H); \
	    else \
	      git status -s $(BFD_H); \
	    fi; \
	  elif test -e $(builddir)/Makefile; then \
	    (cd $(builddir) && $(MAKE) $(AM_V_MFLAG) stmp-bfd-h $(FLAGS_TO_PASS)); \
	  else \
	    echo "$@ target: unsure how to restore BFD_H ($(BFD_H))..."; \
	  fi; \
	else \
	  echo "checking status of BFD_H ($(BFD_H))..." && stat $(BFD_H); \
	  echo "$@: also checking its length..." && wc -l $(BFD_H); \
	  echo "$@: also checking its disk usage..." && du $(BFD_H); \
	fi

stmp-bfd-h: bfd-in3.h @MAINT@ headers @MAINT@ $(PATH_TO_MOVEIFCHANGE)
	$(AM_V_at)rm -f $(AM_V_v) bfd-tmp.h
	$(AM_V_at)if test ! -e bfd-tmp.h || test bfd-tmp.h -ot bfd-in3.h; then \
	  cp $(AM_V_v) bfd-in3.h bfd-tmp.h; \
	  $(RUN_MOVEIFCHANGE) bfd-tmp.h $(BFD_H); \
	elif test -r bfd-in3.h && test "x$(AM_V_P)" != "xfalse"; then \
	  stat bfd-in3.h; \
	fi
	$(AM_V_at)if test $(BFD_H) -ot $@; then touch $(BFD_H); fi
	$(AM_V_at)rm -f $(AM_V_v) bfd-tmp.h
	$(AM_V_at)if test $@ -ot bfd-in3.h; then touch $@; fi

BFD_H_FILES = bfd-in.h init.c opncls.c libbfd.c section.c archures.c \
	reloc.c syms.c bfd.c bfdio.c bfdwin.c \
	archive.c corefile.c targets.c format.c @MAINT@ compress.c
BFD64_H_FILES = archive64.c
LIBBFD_H_FILES = libbfd-in.h init.c libbfd.c bfdio.c bfdwin.c \
	cache.c reloc.c archures.c elf.c
LIBCOFF_H_FILES = libcoff-in.h coffcode.h

# APPLE LOCAL begin framework building:
SYSTEM_FRAMEWORK = -L../intl -lintl -framework System @SYSTEM_FRAMEWORK_LIBS@

FRAMEWORK = bfd
FRAMEWORK_PREFIX =
FRAMEWORK_SUFFIX =
FRAMEWORK_VERSION_SUFFIX =
FRAMEWORK_VERSION = A
# We now fix ofiles above, but just in case:
FRAMEWORK_OFILES = `echo $(libbfd_la_OBJECTS) | $(SED) -e 's/\.lo/\.o/g'` `(cat lofiles | $(SED) -e 's/\.lo/\.o/g' || cat ofiles)`
FRAMEWORK_LIB = -F../libiberty -framework liberty -F../mmalloc -framework mmalloc $(SYSTEM_FRAMEWORK) $(LDFLAGS)

stamp-framework-headers: bfd.h headers
	$(AM_V_at)rm -f stamp-framework stamp-framework-headers
	rm -rf $(AM_V_v) $(FRAMEWORK).framework
	$(MKDIR_P) $(AM_V_v) $(FRAMEWORK).framework/Versions/$(FRAMEWORK_VERSION)/Headers
	$(MKDIR_P) $(AM_V_v) $(FRAMEWORK).framework/Versions/$(FRAMEWORK_VERSION)/PrivateHeaders
	$(LN_S) $(AM_V_v) Versions/Current/Headers $(FRAMEWORK).framework/Headers
	$(LN_S) $(AM_V_v) Versions/Current/PrivateHeaders $(FRAMEWORK).framework/PrivateHeaders
	$(LN_S) $(AM_V_v) A $(FRAMEWORK).framework/Versions/Current
	cp -p $(AM_V_v) $(srcdir)/*.h $(FRAMEWORK).framework/Versions/Current/Headers/
	cp -p $(AM_V_v) *.h $(FRAMEWORK).framework/Versions/Current/Headers/
	cp -p $(AM_V_v) $(BFD_H_DEPS) $(FRAMEWORK).framework/Versions/Current/Headers/
	$(AM_V_at)if test $@ -ot bfd.h; then touch stamp-framework-headers; fi

stamp-framework: Makefile $(libbfd_la_OBJECTS) $(libbfd_la_DEPENDENCIES)
	$(MAKE) $(AM_V_MFLAG) $(FRAMEWORK_OFILES)
	$(AM_V_at)rm -f stamp-framework-headers
	$(MAKE) $(AM_V_MFLAG) stamp-framework-headers
	$(MKDIR_P) $(AM_V_v) $(FRAMEWORK).framework/Versions/$(FRAMEWORK_VERSION)/Resources
	$(LN_S) $(AM_V_v) Versions/Current/Resources $(FRAMEWORK).framework/Resources
	$(MKDIR_P) $(AM_V_v) $(FRAMEWORK).framework/Versions/Current/Resources/English.lproj
	set -ex; \
	if [ "x$(FRAMEWORK_SUFFIX)" != "x" ]; then \
	    test -n "$(FRAMEWORK_SUFFIX)" && echo "$(FRAMEWORK_SUFFIX)"; \
	    $(NSLIBTOOL) -arch $(HOST_ARCHITECTURE) -compatibility_version 1 -current_version 1 -install_name /System/Library/PrivateFrameworks/$(FRAMEWORK).framework/Versions/$(FRAMEWORK_VERSION)/$(FRAMEWORK_PREFIX)$(FRAMEWORK)$(FRAMEWORK_VERSION_SUFFIX) -o $(FRAMEWORK).framework/Versions/Current/$(FRAMEWORK_PREFIX)$(FRAMEWORK)$(FRAMEWORK_VERSION_SUFFIX) $(FRAMEWORK_OFILES) $(FRAMEWORK_LIB); \
	    $(LN_S) $(AM_V_v) $(FRAMEWORK_PREFIX)$(FRAMEWORK)$(FRAMEWORK_VERSION_SUFFIX) $(FRAMEWORK).framework/Versions/Current/$(FRAMEWORK_PREFIX)$(FRAMEWORK)$(FRAMEWORK_SUFFIX); \
	    $(LN_S) $(AM_V_v) Versions/Current/$(FRAMEWORK_PREFIX)$(FRAMEWORK)$(FRAMEWORK_VERSION_SUFFIX) $(FRAMEWORK).framework/$(FRAMEWORK_PREFIX)$(FRAMEWORK)$(FRAMEWORK_VERSION_SUFFIX); \
	    $(LN_S) $(AM_V_v) Versions/Current/$(FRAMEWORK_PREFIX)$(FRAMEWORK)$(FRAMEWORK_SUFFIX) $(FRAMEWORK).framework/$(FRAMEWORK_PREFIX)$(FRAMEWORK)$(FRAMEWORK_SUFFIX); \
	    $(LN_S) $(AM_V_v) $(FRAMEWORK_PREFIX)$(FRAMEWORK)$(FRAMEWORK_SUFFIX) $(FRAMEWORK).framework/$(FRAMEWORK); \
	else \
	    echo "$@: making un-suffixed framework"; \
	    $(NSLIBTOOL) -arch $(HOST_ARCHITECTURE) -compatibility_version 1 -current_version 1 -install_name /System/Library/PrivateFrameworks/$(FRAMEWORK).framework/Versions/$(FRAMEWORK_VERSION)/$(FRAMEWORK_PREFIX)$(FRAMEWORK) -o $(FRAMEWORK).framework/Versions/Current/$(FRAMEWORK_PREFIX)$(FRAMEWORK) $(FRAMEWORK_OFILES) $(FRAMEWORK_LIB); \
	    $(LN_S) $(AM_V_v) Versions/Current/$(FRAMEWORK_PREFIX)$(FRAMEWORK) $(FRAMEWORK).framework/$(FRAMEWORK_PREFIX)$(FRAMEWORK); \
	fi
	$(AM_V_at)if test $@ -ot stamp-framework-headers; then touch $@; fi
# APPLE LOCAL end framework building.

# Could really use a "copy-if-change"...
headers: $(sdocdir)/Makefile Makefile stmp-bin2-h stmp-lbfd-h stmp-lcoff-h \
  $(srcdir)/version.h elfxx-target.h @MAINT@ $(PATH_TO_MOVEIFCHANGE)
	(cd $(sdocdir); $(MAKE) $(AM_V_MFLAG) protos $(FLAGS_TO_PASS))
	$(AM_V_at)if test ! -e $(sdocdir)/libbfd.h || test $(sdocdir)/libbfd.h -ot $(sdocdir)/Makefile; then \
	  (cd $(sdocdir); $(MAKE) $(AM_V_MFLAG) libbfd.h $(FLAGS_TO_PASS)); \
	elif test -r $(sdocdir)/libbfd.h && test "x$(AM_V_P)" != "xfalse"; then \
	  stat $(sdocdir)/libbfd.h; \
	elif test -x "`which sync`"; then \
	  sync; \
	fi
	$(AM_V_at)if test ! -e $(sdocdir)/libcoff.h || test $(sdocdir)/libcoff.h -ot $(sdocdir)/Makefile; then \
	  (cd $(sdocdir); $(MAKE) $(AM_V_MFLAG) libcoff.h $(FLAGS_TO_PASS)); \
	elif test -r $(sdocdir)/libcoff.h && test "x$(AM_V_P)" != "xfalse"; then \
	  stat $(sdocdir)/libcoff.h; \
	elif test -x "`which sync`"; then \
	  sync; \
	fi
	$(AM_V_at)if test ! -e $(sdocdir)/bfd.h || test $(sdocdir)/bfd.h -ot $(sdocdir)/Makefile; then \
	  (cd $(sdocdir); $(MAKE) $(AM_V_MFLAG) bfd.h $(FLAGS_TO_PASS)); \
	elif test -r $(sdocdir)/bfd.h && test "x$(AM_V_P)" != "xfalse"; then \
	  stat $(sdocdir)/bfd.h; \
	elif test -x "`which sync`"; then \
	  sync; \
	fi
	$(AM_V_at)if test $(srcdir) != "."; then \
	  echo "out-of-tree build detected; checking back in srcdir ($(srcdir)) for $@..."; \
	  (cd $(sdocdir); $(MAKE) $(AM_V_MFLAG) $(srcdir)/libbfd.h $(FLAGS_TO_PASS)); \
	  (cd $(sdocdir); $(MAKE) $(AM_V_MFLAG) $(srcdir)/libcoff.h $(FLAGS_TO_PASS)); \
	  (cd $(sdocdir); $(MAKE) $(AM_V_MFLAG) $(srcdir)/bfd.h $(FLAGS_TO_PASS)); \
	else \
	  echo "$@ target: already in srcdir ($(srcdir)): `pwd`"; \
	fi
	$(AM_V_at)if test ! -e bfd-in2.h-new || test bfd-in2.h-new -ot $(sdocdir)/bfd.h; then \
	  cp $(AM_V_v) $(sdocdir)/bfd.h bfd-in2.h-new; \
	  $(RUN_MOVEIFCHANGE) bfd-in2.h-new $(srcdir)/bfd-in2.h; \
	elif test -r bfd-in2.h-new && test "x$(AM_V_P)" != "xfalse"; then \
	  stat bfd-in2.h-new; \
	elif test -x "`which sync`"; then \
	  sync; \
	fi
	$(AM_V_at)if test $(srcdir)/bfd-in2.h -ot stmp-bin2-h; then \
	  touch $(srcdir)/bfd-in2.h; \
	elif test stmp-bin2-h -ot $(srcdir)/bfd-in2.h; then \
	  touch stmp-bin2-h; \
	elif test -x "`which sync`"; then \
	  sync; \
	fi
	$(AM_V_at)if test ! -e libbfd.h-new || test libbfd.h-new -ot $(sdocdir)/libbfd.h; then \
	  cp $(AM_V_v) $(sdocdir)/libbfd.h libbfd.h-new; \
	  $(RUN_MOVEIFCHANGE) libbfd.h-new $(srcdir)/libbfd.h; \
	elif test -r libbfd.h-new && test "x$(AM_V_P)" != "xfalse"; then \
	  stat libbfd.h-new; \
	elif test -x "`which sync`"; then \
	  sync; \
	fi
	$(AM_V_at)if test $(srcdir)/libbfd.h -ot stmp-lbfd-h; then \
	  touch $(srcdir)/libbfd.h; \
	elif test stmp-lbfd-h -ot $(srcdir)/libbfd.h; then \
	  touch stmp-lbfd-h; \
	elif test -x "`which sync`"; then \
	  sync; \
	fi
	$(AM_V_at)if test ! -e libcoff.h-new || test libcoff.h-new -ot $(sdocdir)/libcoff.h; then \
	  cp $(AM_V_v) $(sdocdir)/libcoff.h libcoff.h-new; \
	  $(RUN_MOVEIFCHANGE) libcoff.h-new $(srcdir)/libcoff.h; \
	elif test -r libcoff.h-new && test "x$(AM_V_P)" != "xfalse"; then \
	  stat libcoff.h-new; \
	elif test -x "`which sync`"; then \
	  sync; \
	fi
	$(AM_V_at)if test $(srcdir)/libcoff.h -ot stmp-lcoff-h; then \
	  touch $(srcdir)/libcoff.h; \
	elif test stmp-lcoff-h -ot $(srcdir)/libcoff.h; then \
	  touch stmp-lcoff-h; \
	elif test -x "`which sync`"; then \
	  sync; \
	fi
	$(AM_V_at)if test ! -e bfdver.h || test bfdver.h -ot $(srcdir)/version.h; then \
	  $(MAKE) $(AM_V_MFLAG) bfdver.h; \
	elif test -r bfdver.h && test "x$(AM_V_P)" != "xfalse"; then \
	  stat bfdver.h; \
	fi
	$(AM_V_at)for nn in 32 64; do \
	  if test ! -e elf$${nn}-target.h || test elf$${nn}-target.h -ot elfxx-target.h; then \
	    $(MAKE) $(AM_V_MFLAG) elf$${nn}-target.h; \
	  elif test -r elf$${nn}-target.h && test "x$(AM_V_P)" != "xfalse"; then \
	    stat elf$${nn}-target.h; \
	  fi; \
	done
	$(AM_V_at)-rm -f $(AM_V_v) *.h-new || echo "done with $@ target in `pwd`"
.PHONY: headers

# Target for the Makefile in the documentation subdirectory:
$(sdocdir)/Makefile: $(sdocdir)/Makefile.in $(top_builddir)/config.status
	$(AM_V_at)if test $(srcdir) != "."; then \
	  echo "out-of-tree build detected; checking back in srcdir ($(srcdir)) for $@..."; \
	  if test -e ./doc/Makefile -a ! -e $(sdocdir)/Makefile; then \
	    cp $(AM_V_v) ./doc/Makefile $(sdocdir)/Makefile; \
	  elif test -e $(srcdir)/config.status; then \
	    echo ' $(SHELL) $(srcdir)/config.status'; \
	    $(SHELL) $(srcdir)/config.status; \
	  fi; \
	else \
	  if test -e $(top_builddir)/config.status; then \
	    echo ' $(SHELL) $(top_builddir)/config.status'; \
	    $(SHELL) $(top_builddir)/config.status; \
	  fi; \
	fi
	$(AM_V_at)if test -e Makefile -a ! -e $(srcdir)/Makefile -a -d .deps; then \
	  $(MAKE) $(AM_V_MFLAG) $(srcdir)/Makefile; \
	fi

@ifGNUmake@ifneq ($(srcdir),.)
@ifGNUmake@$(srcdir)/Makefile: $(srcdir)/Makefile.in $(top_builddir)/config.status
@ifGNUmake@	if test $(srcdir) != "."; then \
@ifGNUmake@	  if test -e ./Makefile -a ! -e $(srcdir)/Makefile; then \
@ifGNUmake@	    cp $(AM_V_v) ./Makefile $(srcdir)/Makefile; \
@ifGNUmake@	  elif test -e $(srcdir)/config.status; then \
@ifGNUmake@	    echo ' $(SHELL) $(srcdir)/config.status'; \
@ifGNUmake@	    $(SHELL) $(srcdir)/config.status; \
@ifGNUmake@	  fi; \
@ifGNUmake@	else \
@ifGNUmake@	  if test -e $(top_builddir)/config.status; then \
@ifGNUmake@	    echo ' $(SHELL) $(top_builddir)/config.status'; \
@ifGNUmake@	    $(SHELL) $(top_builddir)/config.status; \
@ifGNUmake@	  fi; \
@ifGNUmake@	fi
@ifGNUmake@endif

# We only rebuild the header files automatically if we have been
# configured with --enable-maintainer-mode.

$(srcdir)/bfd-in2.h: @MAINT@ stmp-bin2-h
	@echo "this target, '$@' (in `pwd`), just makes its dependency, '$<', when in maintainer mode"
stmp-bin2-h: $(sdocdir)/Makefile $(BFD_H_FILES) $(BFD64_H_FILES) @MAINT@ $(PATH_TO_MOVEIFCHANGE)
	(cd $(sdocdir); $(MAKE) $(AM_V_MFLAG) $(FLAGS_TO_PASS) bfd.h)
	$(AM_V_at)if test $(srcdir) != "."; then \
	  echo "out-of-tree build detected; checking back in srcdir ($(srcdir)) for $@..."; \
	  (cd $(sdocdir); $(MAKE) $(AM_V_MFLAG) $(FLAGS_TO_PASS) $(srcdir)/bfd.h); \
	else \
	  echo "$@ target: already in srcdir ($(srcdir)): `pwd`"; \
	fi
	cp $(AM_V_v) $(sdocdir)/bfd.h bfd-in2.h-new
	$(RUN_MOVEIFCHANGE) bfd-in2.h-new $(srcdir)/bfd-in2.h
	$(AM_V_at)if test $(srcdir)/bfd-in2.h -ot $@; then \
	  touch $(srcdir)/bfd-in2.h; \
	elif test $@ -ot $(srcdir)/bfd-in2.h; then \
	  touch $@; \
	fi

$(srcdir)/libbfd.h: @MAINT@ stmp-lbfd-h
	@echo "this target, '$@' (in `pwd`), just makes its dependency, '$<', when in maintainer mode"
stmp-lbfd-h: $(sdocdir)/Makefile $(LIBBFD_H_FILES) @MAINT@ $(PATH_TO_MOVEIFCHANGE)
	(cd $(sdocdir); $(MAKE) $(AM_V_MFLAG) $(FLAGS_TO_PASS) libbfd.h)
	$(AM_V_at)if test $(srcdir) != "."; then \
	  echo "out-of-tree build detected; checking back in srcdir ($(srcdir)) for $@..."; \
	  (cd $(sdocdir); $(MAKE) $(AM_V_MFLAG) $(FLAGS_TO_PASS) $(srcdir)/libbfd.h); \
	else \
	  echo "$@ target: already in srcdir ($(srcdir)): `pwd`"; \
	fi
	cp $(AM_V_v) $(sdocdir)/libbfd.h libbfd.h-new
	$(RUN_MOVEIFCHANGE) libbfd.h-new $(srcdir)/libbfd.h
	$(AM_V_at)if test $(srcdir)/libbfd.h -ot $@; then \
	  touch $(srcdir)/libbfd.h; \
	elif test $@ -ot $(srcdir)/libbfd.h; then \
	  touch $@; \
	fi

$(srcdir)/libcoff.h: @MAINT@ stmp-lcoff-h
	@echo "this target, '$@' (in `pwd`), just makes its dependency, '$<', when in maintainer mode"
stmp-lcoff-h: $(sdocdir)/Makefile $(LIBCOFF_H_FILES) @MAINT@ $(PATH_TO_MOVEIFCHANGE)
	(cd $(sdocdir); $(MAKE) $(AM_V_MFLAG) $(FLAGS_TO_PASS) libcoff.h)
	$(AM_V_at)if test $(srcdir) != "."; then \
	  echo "out-of-tree build detected; checking back in srcdir ($(srcdir)) for $@..."; \
	  (cd $(sdocdir); $(MAKE) $(AM_V_MFLAG) $(FLAGS_TO_PASS) $(srcdir)/libcoff.h); \
	else \
	  echo "$@ target: already in srcdir ($(srcdir)): `pwd`"; \
	fi
	cp $(AM_V_v) $(sdocdir)/libcoff.h libcoff.h-new
	$(RUN_MOVEIFCHANGE) libcoff.h-new $(srcdir)/libcoff.h
	$(AM_V_at)if test $(srcdir)/libcoff.h -ot $@; then \
	  touch $(srcdir)/libcoff.h; \
	elif test $@ -ot $(srcdir)/libcoff.h; then \
	  touch $@; \
	fi

MOSTLYCLEANFILES = tofiles ofiles stamp-ofiles
MOSTLYCLEANFILES += tlofiles lofiles stamp-lofiles
MOSTLYCLEANFILES += tplistfiles plistfiles stamp-plistfiles

CLEANFILES = dep.sed stmp-bfd-h DEP DEPA DEP1 DEP2 libbfd.a stamp-lib \
  stamp-staticlib stmp-bin2-h stmp-lbfd-h stmp-lcoff-h @MAINT@ bfd.h

DISTCLEANFILES = $(BUILD_CFILES) $(BUILD_HFILES) libtool-soversion *~ *.cache.bak

MAINTAINERCLEANFILES = bfd.h @MAINT@ bfd-in2.h

apple-clean: @MAINT@ $(sdocdir)/Makefile @ifnGNUmake@ mostlyclean
	if test -r $(sdocdir)/Makefile; then \
	  (cd $(sdocdir) && $(MAKE) $(AM_V_MFLAG) apple-clean $(FLAGS_TO_PASS)); \
	elif test -r Makefile && test -d $(sdocdir) && test ! -e $(sdocdir)/Makefile; then \
	  $(MAKE) $(AM_V_MFLAG) $(sdocdir)/Makefile; \
	fi
	rm -rfv $(FRAMEWORK).framework || rmdir $(FRAMEWORK).framework
.PHONY: apple-clean

distclean-local: @MAINT@ $(sdocdir)/Makefile apple-clean
	if test -r Makefile && test -d $(sdocdir) && test ! -e $(sdocdir)/Makefile; then \
	  $(MAKE) $(AM_V_MFLAG) $(sdocdir)/Makefile; \
	fi
	-rm -rf $(AM_V_v) .deps || rmdir .deps
	rm -rf $(AM_V_v) autom4te.cache || rmdir autom4te.cache

bfdver.h: $(srcdir)/version.h @MAINT@ $(srcdir)/development.sh \
  $(srcdir)/Makefile.in
	@echo "creating $@ (in `pwd`)"
	@bfd_version=`echo "$(VERSION)" | $(SED) -e 's/\([^\.]*\)\.*\([^\.]*\)\.*\([^\.]*\)\.*\([^\.]*\)\.*\([^\.]*\).*/\1.00\2.00\3.00\4.00\5/' -e 's/\([^\.]*\)\..*\(..\)\..*\(..\)\..*\(..\)\..*\(..\)$$/\1\2\3\4\5/'`; \
	bfd_version_string="\"$(VERSION)\""; \
	bfd_soversion="$(VERSION)"; \
	bfd_version_package="\"$(PKGVERSION)\""; \
	report_bugs_to="\"$(REPORT_BUGS_TO)\""; \
	. $(srcdir)/development.sh; \
	if test "x$(RELEASE)" = "x" || test "x$${development}" = "xtrue"; then \
	  bfd_version_date=`$(SED) -n -e 's/.*DATE //p' < $(srcdir)/version.h`; \
	  bfd_version_string="\"$(VERSION) $${bfd_version_date}\""; \
	  bfd_soversion="$(VERSION).$${bfd_version_date}"; \
	fi; \
	$(SED) -e "s/@bfd_version@/$${bfd_version}/" \
	    -e "s/@bfd_version_string@/$${bfd_version_string}/" \
	    -e "s,@bfd_version_package@,$${bfd_version_package}," \
	    -e "s,@report_bugs_to@,$${report_bugs_to}," \
	    < $(srcdir)/version.h $(AM_V_redir) $@; \
	echo "$${bfd_soversion}" > libtool-soversion
	$(AM_V_at)if test $@ -ot $(srcdir)/version.h; then touch $@; fi

libtool-soversion: bfdver.h
	$(AM_V_at)stat $@ || (if test $@ -ot $<; then touch $@; fi)

# Disable -Werror, if it has been enabled, since coffswap.h fails
# to compile with gcc 4.5 and above:
coff-tic4x.lo: coff-tic4x.c
@am__fastdepCC_TRUE@	$(AM_V_CC)$(LTCOMPILE) -MT $@ -MD -MP -MF $(DEPDIR)/$*.Tpo -c -o $@ $< $(NO_WERROR)
@am__fastdepCC_TRUE@	$(AM_V_at)$(am__mv) $(DEPDIR)/$*.Tpo $(DEPDIR)/$*.Plo
@AMDEP_TRUE@@am__fastdepCC_FALSE@	$(AM_V_CC)source='$<' object='$@' libtool=yes @AMDEPBACKSLASH@
@AMDEP_TRUE@@am__fastdepCC_FALSE@	DEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) @AMDEPBACKSLASH@
@am__fastdepCC_FALSE@	$(AM_V_CC@am__nodep@)$(LTCOMPILE) -c -o $@ $< $(NO_WERROR)

coff-tic54x.lo: coff-tic54x.c
@am__fastdepCC_TRUE@	$(AM_V_CC)$(LTCOMPILE) -MT $@ -MD -MP -MF $(DEPDIR)/$*.Tpo -c -o $@ $< $(NO_WERROR)
@am__fastdepCC_TRUE@	$(AM_V_at)$(am__mv) $(DEPDIR)/$*.Tpo $(DEPDIR)/$*.Plo
@AMDEP_TRUE@@am__fastdepCC_FALSE@	$(AM_V_CC)source='$<' object='$@' libtool=yes @AMDEPBACKSLASH@
@AMDEP_TRUE@@am__fastdepCC_FALSE@	DEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) @AMDEPBACKSLASH@
@am__fastdepCC_FALSE@	$(AM_V_CC@am__nodep@)$(LTCOMPILE) -c -o $@ $< $(NO_WERROR)

# What appears below is generated by a hacked mkdep using gcc -MM.
# FIXME: the sed script that generates this currently screws up some stuff
# such as removing letters and directory separators when they do not need
# to be removed
# (for example, the 't' in 'aout', the 2nd 'f' in 'coff', the 'f' in 'elf')
# can edit manually until the sed script is fixed

# DO NOT DELETE THIS LINE -- mkdep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.
archive.lo: archive.c $(INCDIR)/filenames.h $(INCDIR)/libiberty.h \
  $(INCDIR)/hashtab.h $(INCDIR)/aout/ar.h $(INCDIR)/aout/ranlib.h \
  $(INCDIR)/safe-ctype.h
archures.lo: archures.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  $(INCDIR)/safe-ctype.h
bfd.lo: bfd.c bfdver.h $(INCDIR)/filenames.h $(INCDIR)/libiberty.h \
  $(INCDIR)/safe-ctype.h $(INCDIR)/bfdlink.h $(INCDIR)/hashtab.h \
  $(INCDIR)/coff/internal.h $(INCDIR)/coff/sym.h libcoff.h \
  libecoff.h $(INCDIR)/coff/ecoff.h elf-bfd.h $(INCDIR)/elf/common.h \
  $(INCDIR)/elf/internal.h $(INCDIR)/elf/external.h
bfdio.lo: bfdio.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
bfdwin.lo: bfdwin.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
cache.lo: cache.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  $(INCDIR)/libiberty.h
coffgen.lo: coffgen.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  $(INCDIR)/coff/internal.h libcoff.h $(INCDIR)/bfdlink.h
corefile.lo: corefile.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
format.lo: format.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
init.lo: init.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
libbfd.lo: libbfd.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  $(INCDIR)/libiberty.h
opncls.lo: opncls.c $(INCDIR)/filenames.h $(INCDIR)/objalloc.h \
  $(INCDIR)/hashtab.h $(INCDIR)/libiberty.h
reloc.lo: reloc.c $(INCDIR)/filenames.h $(INCDIR)/bfdlink.h \
  $(INCDIR)/hashtab.h
section.lo: section.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  $(INCDIR)/bfdlink.h
syms.lo: syms.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  $(INCDIR)/safe-ctype.h $(INCDIR)/bfdlink.h $(INCDIR)/aout/stab_gnu.h \
  $(INCDIR)/aout/stab.def
targets.lo: targets.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  $(INCDIR)/fnmatch.h targmatch.h
hash.lo: hash.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  $(INCDIR)/objalloc.h $(INCDIR)/libiberty.h
linker.lo: linker.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  $(INCDIR)/bfdlink.h genlink.h
srec.lo: srec.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  $(INCDIR)/libiberty.h $(INCDIR)/safe-ctype.h
binary.lo: binary.c $(INCDIR)/filenames.h $(INCDIR)/safe-ctype.h \
  $(INCDIR)/hashtab.h
tekhex.lo: tekhex.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  $(INCDIR)/libiberty.h
ihex.lo: ihex.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  $(INCDIR)/libiberty.h $(INCDIR)/safe-ctype.h
stabs.lo: stabs.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  $(INCDIR)/aout/stab_gnu.h $(INCDIR)/aout/stab.def $(INCDIR)/safe-ctype.h
stab-syms.lo: stab-syms.c libaout.h $(INCDIR)/bfdlink.h \
  $(INCDIR)/aout/aout64.h $(INCDIR)/aout/stab.def
merge.lo: merge.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  $(INCDIR)/libiberty.h
dwarf2.lo: dwarf2.c $(INCDIR)/filenames.h $(INCDIR)/libiberty.h \
  $(INCDIR)/hashtab.h elf-bfd.h $(INCDIR)/elf/common.h \
  $(INCDIR)/elf/internal.h $(INCDIR)/elf/external.h $(INCDIR)/bfdlink.h \
  $(INCDIR)/elf/dwarf2.h
simple.lo: simple.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  $(INCDIR)/bfdlink.h
archive64.lo: archive64.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  $(INCDIR)/aout/ar.h
cpu-aarch64.lo: cpu-aarch64.c $(INCDIR)/filenames.h \
  $(INCDIR)/hashtab.h $(INCDIR)/libiberty.h
cpu-a29k.lo: cpu-a29k.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
cpu-alpha.lo: cpu-alpha.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
cpu-arc.lo: cpu-arc.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
cpu-arm.lo: cpu-arm.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  $(INCDIR)/libiberty.h
cpu-avr.lo: cpu-avr.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
cpu-bfin.lo: cpu-bfin.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
cpu-cris.lo: cpu-cris.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
cpu-cr16.lo: cpu-cr16.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
cpu-cr16c.lo: cpu-cr16c.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
cpu-crx.lo: cpu-crx.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
cpu-d10v.lo: cpu-d10v.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
cpu-d30v.lo: cpu-d30v.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
cpu-dlx.lo: cpu-dlx.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
cpu-epiphany.lo: cpu-epiphany.c $(INCDIR)/filenames.h \
  $(INCDIR)/hashtab.h
cpu-fr30.lo: cpu-fr30.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
cpu-frv.lo: cpu-frv.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
cpu-h8300.lo: cpu-h8300.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
cpu-h8500.lo: cpu-h8500.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
cpu-hppa.lo: cpu-hppa.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
cpu-ia64.lo: cpu-ia64.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  cpu-ia64-opc.c $(srcdir)/../opcodes/ia64-opc.h $(INCDIR)/opcode/ia64.h
cpu-i370.lo: cpu-i370.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
cpu-i386.lo: cpu-i386.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
cpu-i860.lo: cpu-i860.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
cpu-i960.lo: cpu-i960.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
cpu-ip2k.lo: cpu-ip2k.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
cpu-iq2000.lo: cpu-iq2000.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
cpu-k1om.lo: cpu-k1om.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
cpu-l1om.lo: cpu-l1om.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
cpu-lm32.lo: cpu-lm32.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
cpu-m32c.lo: cpu-m32c.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
cpu-m32r.lo: cpu-m32r.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
cpu-m68hc11.lo: cpu-m68hc11.c $(INCDIR)/filenames.h \
  $(INCDIR)/hashtab.h
cpu-m68hc12.lo: cpu-m68hc12.c $(INCDIR)/filenames.h \
  $(INCDIR)/hashtab.h
cpu-m68k.lo: cpu-m68k.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
cpu-m88k.lo: cpu-m88k.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
cpu-m9s12x.lo: cpu-m9s12x.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
cpu-m9s12xg.lo: cpu-m9s12xg.c $(INCDIR)/filenames.h \
  $(INCDIR)/hashtab.h
cpu-m10200.lo: cpu-m10200.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
cpu-m10300.lo: cpu-m10300.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
cpu-maxq.lo: cpu-maxq.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
cpu-mcore.lo: cpu-mcore.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
cpu-mep.lo: cpu-mep.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
cpu-metag.lo: cpu-metag.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
cpu-microblaze.lo: cpu-microblaze.c $(INCDIR)/filenames.h \
  $(INCDIR)/hashtab.h
cpu-mips.lo: cpu-mips.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
cpu-mmix.lo: cpu-mmix.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
cpu-moxie.lo: cpu-moxie.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
cpu-ms1.lo: cpu-ms1.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
cpu-msp430.lo: cpu-msp430.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
cpu-mt.lo: cpu-mt.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
cpu-nds32.lo: cpu-nds32.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  elf-bfd.h $(INCDIR)/elf/common.h $(INCDIR)/elf/internal.h \
  $(INCDIR)/elf/external.h $(INCDIR)/bfdlink.h
cpu-nios2.lo: cpu-nios2.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
cpu-ns32k.lo: cpu-ns32k.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  ns32k.h
cpu-openrisc.lo: cpu-openrisc.c $(INCDIR)/filenames.h \
  $(INCDIR)/hashtab.h
cpu-or1k.lo: cpu-or1k.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
cpu-or32.lo: cpu-or32.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
cpu-pdp11.lo: cpu-pdp11.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
cpu-pj.lo: cpu-pj.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
cpu-plugin.lo: cpu-plugin.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
cpu-powerpc.lo: cpu-powerpc.c $(INCDIR)/filenames.h \
  $(INCDIR)/hashtab.h
cpu-rl78.lo: cpu-rl78.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
cpu-rs6000.lo: cpu-rs6000.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
cpu-rx.lo: cpu-rx.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
cpu-s390.lo: cpu-s390.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
cpu-score.lo: cpu-score.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
cpu-sh.lo: cpu-sh.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  $(srcdir)/../opcodes/sh-opc.h
cpu-sparc.lo: cpu-sparc.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
cpu-spu.lo: cpu-spu.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
cpu-tic30.lo: cpu-tic30.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
cpu-tic4x.lo: cpu-tic4x.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
cpu-tic54x.lo: cpu-tic54x.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
cpu-tic6x.lo: cpu-tic6x.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
cpu-tic80.lo: cpu-tic80.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
cpu-tilegx.lo: cpu-tilegx.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
cpu-tilepro.lo: cpu-tilepro.c $(INCDIR)/filenames.h \
  $(INCDIR)/hashtab.h
cpu-v850.lo: cpu-v850.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  $(INCDIR)/safe-ctype.h
cpu-v850_rh850.lo: cpu-v850_rh850.c $(INCDIR)/filenames.h \
  $(INCDIR)/hashtab.h $(INCDIR)/safe-ctype.h
cpu-vax.lo: cpu-vax.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
cpu-we32k.lo: cpu-we32k.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
cpu-w65.lo: cpu-w65.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
cpu-xc16x.lo: cpu-xc16x.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
cpu-xgate.lo: cpu-xgate.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
cpu-xstormy16.lo: cpu-xstormy16.c $(INCDIR)/filenames.h \
  $(INCDIR)/hashtab.h
cpu-xtensa.lo: cpu-xtensa.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
cpu-z80.lo: cpu-z80.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
cpu-z8k.lo: cpu-z8k.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
aout-adobe.lo: aout-adobe.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  $(INCDIR)/aout/adobe.h $(INCDIR)/aout/stab_gnu.h $(INCDIR)/aout/stab.def \
  libaout.h $(INCDIR)/bfdlink.h
aout-arm.lo: aout-arm.c $(INCDIR)/filenames.h libaout.h \
  $(INCDIR)/bfdlink.h $(INCDIR)/aout/aout64.h aoutx.h \
  $(INCDIR)/safe-ctype.h $(INCDIR)/hashtab.h $(INCDIR)/aout/stab_gnu.h \
  $(INCDIR)/aout/stab.def $(INCDIR)/aout/ar.h aout-target.h
aout-cris.lo: aout-cris.c aout32.c aoutx.h $(INCDIR)/filenames.h \
  $(INCDIR)/safe-ctype.h $(INCDIR)/bfdlink.h libaout.h \
  $(INCDIR)/hashtab.h $(INCDIR)/aout/aout64.h $(INCDIR)/aout/stab_gnu.h \
  $(INCDIR)/aout/stab.def $(INCDIR)/aout/ar.h aout-target.h
aout-ns32k.lo: aout-ns32k.c $(INCDIR)/aout/aout64.h ns32k.h \
  libaout.h $(INCDIR)/bfdlink.h aoutx.h $(INCDIR)/filenames.h \
  $(INCDIR)/safe-ctype.h $(INCDIR)/hashtab.h $(INCDIR)/aout/stab_gnu.h \
  $(INCDIR)/aout/stab.def $(INCDIR)/aout/ar.h
aout-sparcle.lo: aout-sparcle.c $(INCDIR)/bfdlink.h \
  libaout.h aoutf1.h $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  $(INCDIR)/aout/sun4.h $(INCDIR)/aout/aout64.h $(INCDIR)/aout/stab_gnu.h \
  $(INCDIR)/aout/stab.def $(INCDIR)/aout/ar.h aout-target.h
aout-tic30.lo: aout-tic30.c $(INCDIR)/filenames.h libaout.h \
  $(INCDIR)/bfdlink.h $(INCDIR)/aout/aout64.h $(INCDIR)/aout/stab_gnu.h \
  $(INCDIR)/aout/stab.def $(INCDIR)/aout/ar.h aoutx.h \
  $(INCDIR)/safe-ctype.h $(INCDIR)/hashtab.h
aout0.lo: aout0.c aoutf1.h $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  $(INCDIR)/aout/sun4.h libaout.h $(INCDIR)/bfdlink.h \
  $(INCDIR)/aout/aout64.h $(INCDIR)/aout/stab_gnu.h $(INCDIR)/aout/stab.def \
  $(INCDIR)/aout/ar.h aout-target.h
aout32.lo: aout32.c aoutx.h $(INCDIR)/filenames.h $(INCDIR)/safe-ctype.h \
  $(INCDIR)/bfdlink.h libaout.h $(INCDIR)/hashtab.h $(INCDIR)/aout/aout64.h \
  $(INCDIR)/aout/stab_gnu.h $(INCDIR)/aout/stab.def $(INCDIR)/aout/ar.h
armnetbsd.lo: armnetbsd.c netbsd.h $(INCDIR)/filenames.h \
  $(INCDIR)/hashtab.h libaout.h $(INCDIR)/bfdlink.h aout-target.h \
  $(INCDIR)/aout/aout64.h $(INCDIR)/aout/stab_gnu.h $(INCDIR)/aout/stab.def \
  $(INCDIR)/aout/ar.h
bout.lo: bout.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  $(INCDIR)/bfdlink.h genlink.h $(INCDIR)/bout.h $(INCDIR)/libiberty.h \
  $(INCDIR)/aout/stab_gnu.h $(INCDIR)/aout/stab.def libaout.h
cf-i386lynx.lo: cf-i386lynx.c $(INCDIR)/filenames.h \
  coff-i386.c $(INCDIR)/hashtab.h $(INCDIR)/coff/i386.h \
  $(INCDIR)/coff/external.h $(INCDIR)/coff/internal.h \
  libcoff.h $(INCDIR)/bfdlink.h coffcode.h coffswap.h
cf-m68klynx.lo: cf-m68klynx.c coff-m68k.c $(INCDIR)/filenames.h \
  $(INCDIR)/hashtab.h $(INCDIR)/coff/m68k.h $(INCDIR)/coff/external.h \
  $(INCDIR)/coff/internal.h libcoff.h $(INCDIR)/bfdlink.h \
  coffcode.h coffswap.h
cf-sparclynx.lo: cf-sparclynx.c coff-sparc.c $(INCDIR)/filenames.h \
  $(INCDIR)/hashtab.h $(INCDIR)/coff/sparc.h $(INCDIR)/coff/external.h \
  $(INCDIR)/coff/internal.h libcoff.h $(INCDIR)/bfdlink.h \
  coffcode.h coffswap.h
coff-a29k.lo: coff-a29k.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  $(INCDIR)/coff/a29k.h $(INCDIR)/coff/external.h $(INCDIR)/coff/internal.h \
  libcoff.h $(INCDIR)/bfdlink.h coffcode.h coffswap.h
coff-apollo.lo: coff-apollo.c $(INCDIR)/filenames.h \
  $(INCDIR)/hashtab.h $(INCDIR)/coff/apollo.h $(INCDIR)/coff/external.h \
  $(INCDIR)/coff/internal.h libcoff.h $(INCDIR)/bfdlink.h \
  coffcode.h coffswap.h
coff-arm.lo: coff-arm.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  $(INCDIR)/coff/arm.h $(INCDIR)/coff/external.h $(INCDIR)/coff/internal.h \
  libcoff.h $(INCDIR)/bfdlink.h coffcode.h coffswap.h
coff-aux.lo: coff-aux.c $(INCDIR)/filenames.h $(INCDIR)/coff/aux-coff.h \
  $(INCDIR)/coff/internal.h $(INCDIR)/coff/m68k.h $(INCDIR)/coff/external.h \
  coff-m68k.c $(INCDIR)/hashtab.h $(INCDIR)/coff/m68k.h \
  $(INCDIR)/coff/internal.h libcoff.h $(INCDIR)/bfdlink.h \
  coffcode.h coffswap.h
coff-h8300.lo: coff-h8300.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  $(INCDIR)/bfdlink.h genlink.h $(INCDIR)/coff/h8300.h \
  $(INCDIR)/coff/external.h $(INCDIR)/coff/internal.h \
  libcoff.h $(INCDIR)/libiberty.h coffcode.h coffswap.h
coff-h8500.lo: coff-h8500.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  $(INCDIR)/bfdlink.h $(INCDIR)/coff/h8500.h $(INCDIR)/coff/external.h \
  $(INCDIR)/coff/internal.h libcoff.h coffcode.h coffswap.h
coff-i386.lo: coff-i386.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  $(INCDIR)/coff/i386.h $(INCDIR)/coff/external.h $(INCDIR)/coff/internal.h \
  libcoff.h $(INCDIR)/bfdlink.h coffcode.h coffswap.h
coff-i860.lo: coff-i860.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  $(INCDIR)/coff/i860.h $(INCDIR)/coff/external.h $(INCDIR)/coff/internal.h \
  libcoff.h $(INCDIR)/bfdlink.h coffcode.h coffswap.h
coff-go32.lo: coff-go32.c coff-i386.c $(INCDIR)/filenames.h \
  $(INCDIR)/hashtab.h $(INCDIR)/coff/i386.h $(INCDIR)/coff/external.h \
  $(INCDIR)/coff/internal.h libcoff.h $(INCDIR)/bfdlink.h \
  coffcode.h coffswap.h
coff-i960.lo: coff-i960.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  $(INCDIR)/coff/i960.h $(INCDIR)/coff/internal.h libcoff.h \
  $(INCDIR)/bfdlink.h coffcode.h coffswap.h
coff-m68k.lo: coff-m68k.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  $(INCDIR)/coff/m68k.h $(INCDIR)/coff/external.h $(INCDIR)/coff/internal.h \
  libcoff.h $(INCDIR)/bfdlink.h coffcode.h coffswap.h
coff-m88k.lo: coff-m88k.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  $(INCDIR)/coff/m88k.h $(INCDIR)/coff/external.h $(INCDIR)/coff/internal.h \
  libcoff.h $(INCDIR)/bfdlink.h coffcode.h coffswap.h
coff-maxq.lo: coff-maxq.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  $(INCDIR)/coff/maxq.h $(INCDIR)/coff/external.h $(INCDIR)/coff/internal.h \
  libcoff.h $(INCDIR)/bfdlink.h $(INCDIR)/libiberty.h \
  coffcode.h coffswap.h
coff-mips.lo: coff-mips.c $(INCDIR)/filenames.h $(INCDIR)/bfdlink.h \
  $(INCDIR)/hashtab.h $(INCDIR)/coff/internal.h $(INCDIR)/coff/sym.h \
  $(INCDIR)/coff/symconst.h $(INCDIR)/coff/ecoff.h $(INCDIR)/coff/mips.h \
  $(INCDIR)/coff/external.h libcoff.h libecoff.h coffswap.h \
  ecoffswap.h
coff-or32.lo: coff-or32.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  $(INCDIR)/coff/or32.h $(INCDIR)/coff/internal.h libcoff.h \
  $(INCDIR)/bfdlink.h coffcode.h coffswap.h
coff-pmac.lo: coff-pmac.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  $(INCDIR)/coff/internal.h $(INCDIR)/coff/rs6000.h libcoff.h \
  $(INCDIR)/bfdlink.h libxcoff.h $(INCDIR)/coff/xcoff.h \
  xcoff-target.h coffcode.h coffswap.h
coff-rs6000.lo: coff-rs6000.c $(INCDIR)/filenames.h \
  $(INCDIR)/bfdlink.h $(INCDIR)/hashtab.h $(INCDIR)/coff/internal.h \
  $(INCDIR)/coff/xcoff.h $(INCDIR)/coff/rs6000.h libcoff.h \
  libxcoff.h coffcode.h coffswap.h
coff-sh.lo: coff-sh.c $(INCDIR)/filenames.h $(INCDIR)/libiberty.h \
  $(INCDIR)/hashtab.h $(INCDIR)/bfdlink.h $(INCDIR)/coff/sh.h \
  $(INCDIR)/coff/external.h $(INCDIR)/coff/internal.h \
  libcoff.h coffcode.h coffswap.h
coff-sparc.lo: coff-sparc.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  $(INCDIR)/coff/sparc.h $(INCDIR)/coff/external.h $(INCDIR)/coff/internal.h \
  libcoff.h $(INCDIR)/bfdlink.h coffcode.h coffswap.h
coff-stgo32.lo: coff-stgo32.c coff-i386.c $(INCDIR)/filenames.h \
  $(INCDIR)/hashtab.h $(INCDIR)/coff/i386.h $(INCDIR)/coff/external.h \
  $(INCDIR)/coff/internal.h $(INCDIR)/coff/go32exe.h \
  libcoff.h $(INCDIR)/bfdlink.h coffcode.h coffswap.h \
  go32stub.h
coff-svm68k.lo: coff-svm68k.c coff-m68k.c $(INCDIR)/filenames.h \
  $(INCDIR)/hashtab.h $(INCDIR)/coff/m68k.h $(INCDIR)/coff/external.h \
  $(INCDIR)/coff/internal.h libcoff.h $(INCDIR)/bfdlink.h \
  coffcode.h coffswap.h
coff-tic30.lo: coff-tic30.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  $(INCDIR)/bfdlink.h $(INCDIR)/coff/tic30.h $(INCDIR)/coff/external.h \
  $(INCDIR)/coff/internal.h libcoff.h coffcode.h coffswap.h
coff-tic4x.lo: coff-tic4x.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  $(INCDIR)/bfdlink.h $(INCDIR)/coff/tic4x.h $(INCDIR)/coff/ti.h \
  $(INCDIR)/coff/internal.h libcoff.h coffcode.h coffswap.h
coff-tic54x.lo: coff-tic54x.c $(INCDIR)/filenames.h \
  $(INCDIR)/hashtab.h $(INCDIR)/bfdlink.h $(INCDIR)/coff/tic54x.h \
  $(INCDIR)/coff/ti.h $(INCDIR)/coff/internal.h libcoff.h \
  coffcode.h coffswap.h
coff-tic80.lo: coff-tic80.c $(INCDIR)/bfdlink.h $(INCDIR)/filenames.h \
  $(INCDIR)/hashtab.h $(INCDIR)/coff/tic80.h $(INCDIR)/coff/external.h \
  $(INCDIR)/coff/internal.h libcoff.h coffcode.h coffswap.h
coff-u68k.lo: coff-u68k.c coff-m68k.c $(INCDIR)/filenames.h \
  $(INCDIR)/hashtab.h $(INCDIR)/coff/m68k.h $(INCDIR)/coff/external.h \
  $(INCDIR)/coff/internal.h libcoff.h $(INCDIR)/bfdlink.h \
  coffcode.h coffswap.h
coff-we32k.lo: coff-we32k.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  $(INCDIR)/coff/we32k.h $(INCDIR)/coff/external.h $(INCDIR)/coff/internal.h \
  libcoff.h $(INCDIR)/bfdlink.h coffcode.h coffswap.h
coff-w65.lo: coff-w65.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  $(INCDIR)/bfdlink.h $(INCDIR)/coff/w65.h $(INCDIR)/coff/external.h \
  $(INCDIR)/coff/internal.h libcoff.h coffcode.h coffswap.h
coff-z80.lo: coff-z80.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  $(INCDIR)/bfdlink.h $(INCDIR)/coff/z80.h $(INCDIR)/coff/external.h \
  $(INCDIR)/coff/internal.h libcoff.h coffcode.h coffswap.h
coff-z8k.lo: coff-z8k.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  $(INCDIR)/bfdlink.h $(INCDIR)/coff/z8k.h $(INCDIR)/coff/external.h \
  $(INCDIR)/coff/internal.h libcoff.h coffcode.h coffswap.h
cofflink.lo: cofflink.c $(INCDIR)/filenames.h $(INCDIR)/bfdlink.h \
  $(INCDIR)/hashtab.h $(INCDIR)/coff/internal.h libcoff.h \
  $(INCDIR)/safe-ctype.h
dwarf1.lo: dwarf1.c $(INCDIR)/filenames.h $(INCDIR)/libiberty.h \
  $(INCDIR)/hashtab.h elf-bfd.h $(INCDIR)/elf/common.h \
  $(INCDIR)/elf/internal.h $(INCDIR)/elf/external.h $(INCDIR)/bfdlink.h \
  $(INCDIR)/elf/dwarf.h
ecoff.lo: ecoff.c $(INCDIR)/filenames.h $(INCDIR)/bfdlink.h \
  $(INCDIR)/hashtab.h $(INCDIR)/aout/ar.h $(INCDIR)/aout/ranlib.h \
  $(INCDIR)/aout/stab_gnu.h $(INCDIR)/aout/stab.def libaout.h \
  $(INCDIR)/aout/aout64.h $(INCDIR)/coff/internal.h $(INCDIR)/coff/sym.h \
  $(INCDIR)/coff/symconst.h $(INCDIR)/coff/ecoff.h libcoff.h \
  libecoff.h $(INCDIR)/libiberty.h
ecofflink.lo: ecofflink.c $(INCDIR)/filenames.h $(INCDIR)/bfdlink.h \
  $(INCDIR)/hashtab.h $(INCDIR)/objalloc.h $(INCDIR)/aout/stab_gnu.h \
  $(INCDIR)/aout/stab.def $(INCDIR)/coff/internal.h $(INCDIR)/coff/sym.h \
  $(INCDIR)/coff/symconst.h $(INCDIR)/coff/ecoff.h libcoff.h \
  libecoff.h
efi-app-ia32.lo: efi-app-ia32.c $(INCDIR)/filenames.h \
  coff-i386.c $(INCDIR)/hashtab.h $(INCDIR)/coff/i386.h \
  $(INCDIR)/coff/external.h $(INCDIR)/coff/internal.h \
  $(INCDIR)/coff/pe.h libcoff.h $(INCDIR)/bfdlink.h coffcode.h \
  peicode.h libpei.h
elf.lo: elf.c $(INCDIR)/filenames.h $(INCDIR)/bfdlink.h \
  $(INCDIR)/hashtab.h elf-bfd.h $(INCDIR)/elf/common.h \
  $(INCDIR)/elf/internal.h $(INCDIR)/elf/external.h $(INCDIR)/libiberty.h
elf32-am33lin.lo: elf32-am33lin.c elf-m10300.c $(INCDIR)/filenames.h \
  $(INCDIR)/hashtab.h elf-bfd.h $(INCDIR)/elf/common.h \
  $(INCDIR)/elf/internal.h $(INCDIR)/elf/external.h $(INCDIR)/bfdlink.h \
  $(INCDIR)/elf/mn10300.h $(INCDIR)/elf/reloc-macros.h \
  elf32-target.h
elf32-arc.lo: elf32-arc.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  elf-bfd.h $(INCDIR)/elf/common.h $(INCDIR)/elf/internal.h \
  $(INCDIR)/elf/external.h $(INCDIR)/bfdlink.h $(INCDIR)/elf/arc.h \
  $(INCDIR)/elf/reloc-macros.h $(INCDIR)/libiberty.h \
  elf32-target.h
elf32-arm.lo: elf32-arm.c $(INCDIR)/elf/arm.h $(INCDIR)/elf/reloc-macros.h \
  $(INCDIR)/filenames.h $(INCDIR)/hashtab.h elf-bfd.h \
  $(INCDIR)/elf/common.h $(INCDIR)/elf/internal.h $(INCDIR)/elf/external.h \
  $(INCDIR)/bfdlink.h elf32-target.h
elf32-avr.lo: elf32-avr.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  elf-bfd.h $(INCDIR)/elf/common.h $(INCDIR)/elf/internal.h \
  $(INCDIR)/elf/external.h $(INCDIR)/bfdlink.h $(INCDIR)/elf/avr.h \
  $(INCDIR)/elf/reloc-macros.h elf32-target.h
elf32-bfin.lo: elf32-bfin.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  elf-bfd.h $(INCDIR)/elf/common.h $(INCDIR)/elf/internal.h \
  $(INCDIR)/elf/external.h $(INCDIR)/bfdlink.h $(INCDIR)/elf/bfin.h \
  $(INCDIR)/elf/reloc-macros.h $(INCDIR)/dwarf2.h $(INCDIR)/dwarf2.def \
  elf32-target.h
elf32-cr16.lo: elf32-cr16.c $(INCDIR)/filenames.h $(INCDIR)/bfdlink.h \
  $(INCDIR)/hashtab.h $(INCDIR)/libiberty.h elf-bfd.h \
  $(INCDIR)/elf/common.h $(INCDIR)/elf/internal.h $(INCDIR)/elf/external.h \
  $(INCDIR)/elf/cr16.h $(INCDIR)/elf/reloc-macros.h elf32-target.h
elf32-cr16c.lo: elf32-cr16c.c $(INCDIR)/filenames.h \
  $(INCDIR)/hashtab.h $(INCDIR)/bfdlink.h $(INCDIR)/elf/cr16c.h \
  $(INCDIR)/elf/reloc-macros.h elf-bfd.h $(INCDIR)/elf/common.h \
  $(INCDIR)/elf/internal.h $(INCDIR)/elf/external.h elf32-target.h
elf32-cris.lo: elf32-cris.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  elf-bfd.h $(INCDIR)/elf/common.h $(INCDIR)/elf/internal.h \
  $(INCDIR)/elf/external.h $(INCDIR)/bfdlink.h $(INCDIR)/elf/cris.h \
  $(INCDIR)/elf/reloc-macros.h elf32-target.h
elf32-crx.lo: elf32-crx.c $(INCDIR)/filenames.h $(INCDIR)/bfdlink.h \
  $(INCDIR)/hashtab.h elf-bfd.h $(INCDIR)/elf/common.h \
  $(INCDIR)/elf/internal.h $(INCDIR)/elf/external.h $(INCDIR)/elf/crx.h \
  $(INCDIR)/elf/reloc-macros.h elf32-target.h
elf32-d10v.lo: elf32-d10v.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  elf-bfd.h $(INCDIR)/elf/common.h $(INCDIR)/elf/internal.h \
  $(INCDIR)/elf/external.h $(INCDIR)/bfdlink.h $(INCDIR)/elf/d10v.h \
  $(INCDIR)/elf/reloc-macros.h elf32-target.h
elf32-d30v.lo: elf32-d30v.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  elf-bfd.h $(INCDIR)/elf/common.h $(INCDIR)/elf/internal.h \
  $(INCDIR)/elf/external.h $(INCDIR)/bfdlink.h $(INCDIR)/elf/d30v.h \
  $(INCDIR)/elf/reloc-macros.h elf32-target.h
elf32-dlx.lo: elf32-dlx.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  elf-bfd.h $(INCDIR)/elf/common.h $(INCDIR)/elf/internal.h \
  $(INCDIR)/elf/external.h $(INCDIR)/bfdlink.h $(INCDIR)/elf/dlx.h \
  $(INCDIR)/elf/reloc-macros.h elf32-target.h
elf32-fr30.lo: elf32-fr30.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  elf-bfd.h $(INCDIR)/elf/common.h $(INCDIR)/elf/internal.h \
  $(INCDIR)/elf/external.h $(INCDIR)/bfdlink.h $(INCDIR)/elf/fr30.h \
  $(INCDIR)/elf/reloc-macros.h elf32-target.h
elf32-frv.lo: elf32-frv.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  elf-bfd.h $(INCDIR)/elf/common.h $(INCDIR)/elf/internal.h \
  $(INCDIR)/elf/external.h $(INCDIR)/bfdlink.h $(INCDIR)/elf/frv.h \
  $(INCDIR)/elf/reloc-macros.h $(INCDIR)/elf/dwarf2.h \
  elf32-target.h
elf32-gen.lo: elf32-gen.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  elf-bfd.h $(INCDIR)/elf/common.h $(INCDIR)/elf/internal.h \
  $(INCDIR)/elf/external.h $(INCDIR)/bfdlink.h elf32-target.h
elf32-h8300.lo: elf32-h8300.c $(INCDIR)/filenames.h \
  $(INCDIR)/hashtab.h elf-bfd.h $(INCDIR)/elf/common.h \
  $(INCDIR)/elf/internal.h $(INCDIR)/elf/external.h $(INCDIR)/bfdlink.h \
  $(INCDIR)/elf/h8.h $(INCDIR)/elf/reloc-macros.h elf32-target.h
elf32-hppa.lo: elf32-hppa.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  elf-bfd.h $(INCDIR)/elf/common.h $(INCDIR)/elf/internal.h \
  $(INCDIR)/elf/external.h $(INCDIR)/bfdlink.h $(INCDIR)/elf/hppa.h \
  $(INCDIR)/elf/reloc-macros.h libhppa.h elf32-hppa.h \
  elf-hppa.h elf32-target.h
elf32-i370.lo: elf32-i370.c $(INCDIR)/filenames.h $(INCDIR)/bfdlink.h \
  $(INCDIR)/hashtab.h elf-bfd.h $(INCDIR)/elf/common.h \
  $(INCDIR)/elf/internal.h $(INCDIR)/elf/external.h $(INCDIR)/elf/i370.h \
  $(INCDIR)/elf/reloc-macros.h elf32-target.h
elf32-i386.lo: elf32-i386.c $(INCDIR)/filenames.h $(INCDIR)/bfdlink.h \
  $(INCDIR)/hashtab.h elf-bfd.h $(INCDIR)/elf/common.h \
  $(INCDIR)/elf/internal.h $(INCDIR)/elf/external.h elf-vxworks.h \
  $(INCDIR)/elf/i386.h $(INCDIR)/elf/reloc-macros.h elf32-target.h
elf32-i860.lo: elf32-i860.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  elf-bfd.h $(INCDIR)/elf/common.h $(INCDIR)/elf/internal.h \
  $(INCDIR)/elf/external.h $(INCDIR)/bfdlink.h $(INCDIR)/elf/i860.h \
  $(INCDIR)/elf/reloc-macros.h elf32-target.h
elf32-i960.lo: elf32-i960.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  elf-bfd.h $(INCDIR)/elf/common.h $(INCDIR)/elf/internal.h \
  $(INCDIR)/elf/external.h $(INCDIR)/bfdlink.h $(INCDIR)/elf/i960.h \
  $(INCDIR)/elf/reloc-macros.h elf32-target.h
elf32-ip2k.lo: elf32-ip2k.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  elf-bfd.h $(INCDIR)/elf/common.h $(INCDIR)/elf/internal.h \
  $(INCDIR)/elf/external.h $(INCDIR)/bfdlink.h $(INCDIR)/elf/ip2k.h \
  $(INCDIR)/elf/reloc-macros.h elf32-target.h
elf32-iq2000.lo: elf32-iq2000.c $(INCDIR)/filenames.h \
  $(INCDIR)/hashtab.h elf-bfd.h $(INCDIR)/elf/common.h \
  $(INCDIR)/elf/internal.h $(INCDIR)/elf/external.h $(INCDIR)/bfdlink.h \
  $(INCDIR)/elf/iq2000.h $(INCDIR)/elf/reloc-macros.h \
  elf32-target.h
elf32-m32c.lo: elf32-m32c.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  elf-bfd.h $(INCDIR)/elf/common.h $(INCDIR)/elf/internal.h \
  $(INCDIR)/elf/external.h $(INCDIR)/bfdlink.h $(INCDIR)/elf/m32c.h \
  $(INCDIR)/elf/reloc-macros.h $(INCDIR)/libiberty.h \
  elf32-target.h
elf32-m32r.lo: elf32-m32r.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  elf-bfd.h $(INCDIR)/elf/common.h $(INCDIR)/elf/internal.h \
  $(INCDIR)/elf/external.h $(INCDIR)/bfdlink.h $(INCDIR)/elf/m32r.h \
  $(INCDIR)/elf/reloc-macros.h elf32-target.h
elf32-m68k.lo: elf32-m68k.c $(INCDIR)/filenames.h $(INCDIR)/bfdlink.h \
  $(INCDIR)/hashtab.h elf-bfd.h $(INCDIR)/elf/common.h \
  $(INCDIR)/elf/internal.h $(INCDIR)/elf/external.h $(INCDIR)/elf/m68k.h \
  $(INCDIR)/elf/reloc-macros.h elf32-target.h
elf32-m68hc11.lo: elf32-m68hc11.c $(INCDIR)/filenames.h \
  $(INCDIR)/bfdlink.h $(INCDIR)/hashtab.h elf-bfd.h $(INCDIR)/elf/common.h \
  $(INCDIR)/elf/internal.h $(INCDIR)/elf/external.h elf32-m68hc1x.h \
  $(INCDIR)/elf/m68hc11.h $(INCDIR)/elf/reloc-macros.h \
  $(INCDIR)/opcode/m68hc11.h elf32-target.h
elf32-m68hc12.lo: elf32-m68hc12.c $(INCDIR)/filenames.h \
  $(INCDIR)/bfdlink.h $(INCDIR)/hashtab.h elf-bfd.h $(INCDIR)/elf/common.h \
  $(INCDIR)/elf/internal.h $(INCDIR)/elf/external.h elf32-m68hc1x.h \
  $(INCDIR)/elf/m68hc11.h $(INCDIR)/elf/reloc-macros.h \
  $(INCDIR)/opcode/m68hc11.h elf32-target.h
elf32-m68hc1x.lo: elf32-m68hc1x.c $(INCDIR)/filenames.h \
  $(INCDIR)/bfdlink.h $(INCDIR)/hashtab.h elf-bfd.h $(INCDIR)/elf/common.h \
  $(INCDIR)/elf/internal.h $(INCDIR)/elf/external.h elf32-m68hc1x.h \
  $(INCDIR)/elf/m68hc11.h $(INCDIR)/elf/reloc-macros.h \
  $(INCDIR)/opcode/m68hc11.h
elf32-m88k.lo: elf32-m88k.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  elf-bfd.h $(INCDIR)/elf/common.h $(INCDIR)/elf/internal.h \
  $(INCDIR)/elf/external.h $(INCDIR)/bfdlink.h elf32-target.h
elf-m10200.lo: elf-m10200.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  elf-bfd.h $(INCDIR)/elf/common.h $(INCDIR)/elf/internal.h \
  $(INCDIR)/elf/external.h $(INCDIR)/bfdlink.h elf32-target.h
elf-m10300.lo: elf-m10300.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  elf-bfd.h $(INCDIR)/elf/common.h $(INCDIR)/elf/internal.h \
  $(INCDIR)/elf/external.h $(INCDIR)/bfdlink.h $(INCDIR)/elf/mn10300.h \
  $(INCDIR)/elf/reloc-macros.h elf32-target.h
elf32-mcore.lo: elf32-mcore.c $(INCDIR)/filenames.h \
  $(INCDIR)/bfdlink.h $(INCDIR)/hashtab.h elf-bfd.h $(INCDIR)/elf/common.h \
  $(INCDIR)/elf/internal.h $(INCDIR)/elf/external.h $(INCDIR)/elf/mcore.h \
  $(INCDIR)/elf/reloc-macros.h elf32-target.h
elfxx-mips.lo: elfxx-mips.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  $(INCDIR)/libiberty.h elf-bfd.h $(INCDIR)/elf/common.h \
  $(INCDIR)/elf/internal.h $(INCDIR)/elf/external.h $(INCDIR)/bfdlink.h \
  elfxx-mips.h $(INCDIR)/elf/mips.h $(INCDIR)/elf/reloc-macros.h \
  $(INCDIR)/coff/sym.h $(INCDIR)/coff/symconst.h $(INCDIR)/coff/ecoff.h \
  $(INCDIR)/coff/mips.h $(INCDIR)/coff/external.h
elf32-mips.lo: elf32-mips.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  $(INCDIR)/bfdlink.h genlink.h elf-bfd.h $(INCDIR)/elf/common.h \
  $(INCDIR)/elf/internal.h $(INCDIR)/elf/external.h elfxx-mips.h \
  $(INCDIR)/elf/mips.h $(INCDIR)/elf/reloc-macros.h $(INCDIR)/coff/sym.h \
  $(INCDIR)/coff/symconst.h $(INCDIR)/coff/internal.h \
  $(INCDIR)/coff/ecoff.h $(INCDIR)/coff/mips.h $(INCDIR)/coff/external.h \
  ecoffswap.h elf32-target.h
elf32-ms1.lo: elf32-ms1.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  elf-bfd.h $(INCDIR)/elf/common.h $(INCDIR)/elf/internal.h \
  $(INCDIR)/elf/external.h $(INCDIR)/bfdlink.h $(INCDIR)/elf/ms1.h \
  $(INCDIR)/elf/reloc-macros.h elf32-target.h
elf32-msp430.lo: elf32-msp430.c $(INCDIR)/filenames.h \
  $(INCDIR)/libiberty.h $(INCDIR)/hashtab.h elf-bfd.h \
  $(INCDIR)/elf/common.h $(INCDIR)/elf/internal.h $(INCDIR)/elf/external.h \
  $(INCDIR)/bfdlink.h $(INCDIR)/elf/msp430.h $(INCDIR)/elf/reloc-macros.h \
  elf32-target.h
elf32-openrisc.lo: elf32-openrisc.c $(INCDIR)/filenames.h \
  $(INCDIR)/hashtab.h elf-bfd.h $(INCDIR)/elf/common.h \
  $(INCDIR)/elf/internal.h $(INCDIR)/elf/external.h $(INCDIR)/bfdlink.h \
  $(INCDIR)/elf/openrisc.h $(INCDIR)/elf/reloc-macros.h \
  $(INCDIR)/libiberty.h elf32-target.h
elf32-or32.lo: elf32-or32.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  elf-bfd.h $(INCDIR)/elf/common.h $(INCDIR)/elf/internal.h \
  $(INCDIR)/elf/external.h $(INCDIR)/bfdlink.h $(INCDIR)/elf/or32.h \
  $(INCDIR)/elf/reloc-macros.h $(INCDIR)/libiberty.h \
  elf32-target.h
elf32-pj.lo: elf32-pj.c $(INCDIR)/filenames.h $(INCDIR)/bfdlink.h \
  $(INCDIR)/hashtab.h elf-bfd.h $(INCDIR)/elf/common.h \
  $(INCDIR)/elf/internal.h $(INCDIR)/elf/external.h $(INCDIR)/elf/pj.h \
  $(INCDIR)/elf/reloc-macros.h elf32-target.h
elf32-ppc.lo: elf32-ppc.c $(INCDIR)/filenames.h $(INCDIR)/bfdlink.h \
  $(INCDIR)/hashtab.h elf-bfd.h $(INCDIR)/elf/common.h \
  $(INCDIR)/elf/internal.h $(INCDIR)/elf/external.h $(INCDIR)/elf/ppc.h \
  $(INCDIR)/elf/reloc-macros.h elf32-ppc.h elf-vxworks.h \
  elf32-target.h
elf32-sh64.lo: elf32-sh64.c $(INCDIR)/filenames.h elf-bfd.h \
  $(INCDIR)/elf/common.h $(INCDIR)/elf/internal.h $(INCDIR)/elf/external.h \
  $(INCDIR)/bfdlink.h $(srcdir)/../opcodes/sh64-opc.h \
  elf32-sh64.h elf32-sh.c $(INCDIR)/hashtab.h $(INCDIR)/elf/sh.h \
  $(INCDIR)/elf/reloc-macros.h $(INCDIR)/libiberty.h \
  $(srcdir)/../opcodes/sh-opc.h elf32-target.h
elf32-sh64-com.lo: elf32-sh64-com.c $(INCDIR)/filenames.h \
  $(INCDIR)/hashtab.h elf-bfd.h $(INCDIR)/elf/common.h \
  $(INCDIR)/elf/internal.h $(INCDIR)/elf/external.h $(INCDIR)/bfdlink.h \
  $(INCDIR)/elf/sh.h $(INCDIR)/elf/reloc-macros.h elf32-sh64.h \
  $(srcdir)/../opcodes/sh64-opc.h
elf32-s390.lo: elf32-s390.c $(INCDIR)/filenames.h $(INCDIR)/bfdlink.h \
  $(INCDIR)/hashtab.h elf-bfd.h $(INCDIR)/elf/common.h \
  $(INCDIR)/elf/internal.h $(INCDIR)/elf/external.h $(INCDIR)/elf/s390.h \
  $(INCDIR)/elf/reloc-macros.h elf32-target.h
elf32-sh.lo: elf32-sh.c $(INCDIR)/filenames.h $(INCDIR)/bfdlink.h \
  $(INCDIR)/hashtab.h elf-bfd.h $(INCDIR)/elf/common.h \
  $(INCDIR)/elf/internal.h $(INCDIR)/elf/external.h $(INCDIR)/elf/sh.h \
  $(INCDIR)/elf/reloc-macros.h $(INCDIR)/libiberty.h \
  $(srcdir)/../opcodes/sh-opc.h elf32-target.h
elf32-sh-symbian.lo: elf32-sh-symbian.c elf32-sh.c \
  $(INCDIR)/filenames.h $(INCDIR)/bfdlink.h $(INCDIR)/hashtab.h \
  elf-bfd.h $(INCDIR)/elf/common.h $(INCDIR)/elf/internal.h \
  $(INCDIR)/elf/external.h $(INCDIR)/elf/sh.h $(INCDIR)/elf/reloc-macros.h \
  $(INCDIR)/libiberty.h $(srcdir)/../opcodes/sh-opc.h \
  elf32-target.h
elfxx-sparc.lo: elfxx-sparc.c $(INCDIR)/filenames.h \
  $(INCDIR)/bfdlink.h $(INCDIR)/hashtab.h elf-bfd.h $(INCDIR)/elf/common.h \
  $(INCDIR)/elf/internal.h $(INCDIR)/elf/external.h $(INCDIR)/elf/sparc.h \
  $(INCDIR)/elf/reloc-macros.h $(INCDIR)/opcode/sparc.h \
  elfxx-sparc.h
elf32-sparc.lo: elf32-sparc.c $(INCDIR)/filenames.h \
  $(INCDIR)/bfdlink.h $(INCDIR)/hashtab.h elf-bfd.h $(INCDIR)/elf/common.h \
  $(INCDIR)/elf/internal.h $(INCDIR)/elf/external.h $(INCDIR)/elf/sparc.h \
  $(INCDIR)/elf/reloc-macros.h $(INCDIR)/opcode/sparc.h \
  elfxx-sparc.h elf32-target.h
elf32-v850.lo: elf32-v850.c $(INCDIR)/filenames.h $(INCDIR)/bfdlink.h \
  $(INCDIR)/hashtab.h elf-bfd.h $(INCDIR)/elf/common.h \
  $(INCDIR)/elf/internal.h $(INCDIR)/elf/external.h $(INCDIR)/elf/v850.h \
  $(INCDIR)/elf/reloc-macros.h $(INCDIR)/libiberty.h \
  elf32-target.h
elf32-vax.lo: elf32-vax.c $(INCDIR)/filenames.h $(INCDIR)/bfdlink.h \
  $(INCDIR)/hashtab.h elf-bfd.h $(INCDIR)/elf/common.h \
  $(INCDIR)/elf/internal.h $(INCDIR)/elf/external.h $(INCDIR)/elf/vax.h \
  $(INCDIR)/elf/reloc-macros.h elf32-target.h
elf32-xstormy16.lo: elf32-xstormy16.c $(INCDIR)/filenames.h \
  $(INCDIR)/hashtab.h elf-bfd.h $(INCDIR)/elf/common.h \
  $(INCDIR)/elf/internal.h $(INCDIR)/elf/external.h $(INCDIR)/bfdlink.h \
  $(INCDIR)/elf/xstormy16.h $(INCDIR)/elf/reloc-macros.h \
  $(INCDIR)/libiberty.h elf32-target.h
elf32-xtensa.lo: elf32-xtensa.c $(INCDIR)/filenames.h \
  $(INCDIR)/bfdlink.h $(INCDIR)/hashtab.h elf-bfd.h $(INCDIR)/elf/common.h \
  $(INCDIR)/elf/internal.h $(INCDIR)/elf/external.h $(INCDIR)/elf/xtensa.h \
  $(INCDIR)/elf/reloc-macros.h $(INCDIR)/xtensa-isa.h \
  $(INCDIR)/xtensa-config.h elf32-target.h
elf32.lo: elf32.c elfcode.h $(INCDIR)/filenames.h $(INCDIR)/libiberty.h \
  $(INCDIR)/bfdlink.h $(INCDIR)/hashtab.h elf-bfd.h $(INCDIR)/elf/common.h \
  $(INCDIR)/elf/internal.h $(INCDIR)/elf/external.h elfcore.h
elflink.lo: elflink.c $(INCDIR)/filenames.h $(INCDIR)/bfdlink.h \
  $(INCDIR)/hashtab.h elf-bfd.h $(INCDIR)/elf/common.h \
  $(INCDIR)/elf/internal.h $(INCDIR)/elf/external.h $(INCDIR)/safe-ctype.h \
  $(INCDIR)/libiberty.h
elf-strtab.lo: elf-strtab.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  elf-bfd.h $(INCDIR)/elf/common.h $(INCDIR)/elf/internal.h \
  $(INCDIR)/elf/external.h $(INCDIR)/bfdlink.h $(INCDIR)/libiberty.h
elf-eh-frame.lo: elf-eh-frame.c $(INCDIR)/filenames.h \
  $(INCDIR)/hashtab.h elf-bfd.h $(INCDIR)/elf/common.h \
  $(INCDIR)/elf/internal.h $(INCDIR)/elf/external.h $(INCDIR)/bfdlink.h \
  $(INCDIR)/elf/dwarf2.h
elf-vxworks.lo: elf-vxworks.c $(INCDIR)/filenames.h \
  $(INCDIR)/hashtab.h elf-bfd.h $(INCDIR)/elf/common.h \
  $(INCDIR)/elf/internal.h $(INCDIR)/elf/external.h $(INCDIR)/bfdlink.h \
  elf-vxworks.h
epoc-pe-arm.lo: epoc-pe-arm.c pe-arm.c $(INCDIR)/filenames.h \
  coff-arm.c $(INCDIR)/hashtab.h $(INCDIR)/coff/arm.h \
  $(INCDIR)/coff/external.h $(INCDIR)/coff/internal.h \
  $(INCDIR)/coff/pe.h libcoff.h $(INCDIR)/bfdlink.h coffcode.h \
  peicode.h libpei.h
epoc-pei-arm.lo: epoc-pei-arm.c pei-arm.c $(INCDIR)/filenames.h \
  coff-arm.c $(INCDIR)/hashtab.h $(INCDIR)/coff/arm.h \
  $(INCDIR)/coff/external.h $(INCDIR)/coff/internal.h \
  $(INCDIR)/coff/pe.h libcoff.h $(INCDIR)/bfdlink.h coffcode.h \
  peicode.h libpei.h
hp300bsd.lo: hp300bsd.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  libaout.h $(INCDIR)/bfdlink.h aout-target.h $(INCDIR)/aout/aout64.h \
  $(INCDIR)/aout/stab_gnu.h $(INCDIR)/aout/stab.def $(INCDIR)/aout/ar.h
hp300hpux.lo: hp300hpux.c $(INCDIR)/aout/hp300hpux.h \
  aoutx.h $(INCDIR)/filenames.h $(INCDIR)/safe-ctype.h \
  $(INCDIR)/bfdlink.h libaout.h $(INCDIR)/hashtab.h $(INCDIR)/aout/aout64.h \
  $(INCDIR)/aout/stab_gnu.h $(INCDIR)/aout/stab.def $(INCDIR)/aout/ar.h \
  aout-target.h
som.lo: som.c $(INCDIR)/alloca-conf.h $(INCDIR)/filenames.h
i386aout.lo: i386aout.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  $(INCDIR)/aout/aout64.h libaout.h $(INCDIR)/bfdlink.h \
  aout-target.h $(INCDIR)/aout/stab_gnu.h $(INCDIR)/aout/stab.def \
  $(INCDIR)/aout/ar.h
i386bsd.lo: i386bsd.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  libaout.h $(INCDIR)/bfdlink.h aout-target.h $(INCDIR)/aout/aout64.h \
  $(INCDIR)/aout/stab_gnu.h $(INCDIR)/aout/stab.def $(INCDIR)/aout/ar.h
i386dynix.lo: i386dynix.c $(INCDIR)/aout/dynix3.h aoutx.h \
  $(INCDIR)/filenames.h $(INCDIR)/safe-ctype.h $(INCDIR)/bfdlink.h \
  libaout.h $(INCDIR)/hashtab.h $(INCDIR)/aout/aout64.h \
  $(INCDIR)/aout/stab_gnu.h $(INCDIR)/aout/stab.def $(INCDIR)/aout/ar.h \
  aout-target.h
i386freebsd.lo: i386freebsd.c freebsd.h $(INCDIR)/filenames.h \
  $(INCDIR)/hashtab.h libaout.h $(INCDIR)/bfdlink.h aout-target.h \
  $(INCDIR)/aout/aout64.h $(INCDIR)/aout/stab_gnu.h $(INCDIR)/aout/stab.def \
  $(INCDIR)/aout/ar.h
i386linux.lo: i386linux.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  $(INCDIR)/aout/aout64.h $(INCDIR)/aout/stab_gnu.h $(INCDIR)/aout/stab.def \
  $(INCDIR)/aout/ar.h libaout.h $(INCDIR)/bfdlink.h aout-target.h
i386lynx.lo: i386lynx.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  libaout.h $(INCDIR)/bfdlink.h $(INCDIR)/aout/aout64.h \
  aout-target.h $(INCDIR)/aout/stab_gnu.h $(INCDIR)/aout/stab.def \
  $(INCDIR)/aout/ar.h
i386msdos.lo: i386msdos.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  libaout.h $(INCDIR)/bfdlink.h
i386netbsd.lo: i386netbsd.c netbsd.h $(INCDIR)/filenames.h \
  $(INCDIR)/hashtab.h libaout.h $(INCDIR)/bfdlink.h aout-target.h \
  $(INCDIR)/aout/aout64.h $(INCDIR)/aout/stab_gnu.h $(INCDIR)/aout/stab.def \
  $(INCDIR)/aout/ar.h
i386mach3.lo: i386mach3.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  $(INCDIR)/aout/aout64.h $(INCDIR)/aout/stab_gnu.h $(INCDIR)/aout/stab.def \
  $(INCDIR)/aout/ar.h libaout.h $(INCDIR)/bfdlink.h aout-target.h
i386os9k.lo: i386os9k.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  $(INCDIR)/bfdlink.h libaout.h $(INCDIR)/os9k.h
ieee.lo: ieee.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  $(INCDIR)/ieee.h libieee.h $(INCDIR)/safe-ctype.h
m68k4knetbsd.lo: m68k4knetbsd.c netbsd.h $(INCDIR)/filenames.h \
  $(INCDIR)/hashtab.h libaout.h $(INCDIR)/bfdlink.h aout-target.h \
  $(INCDIR)/aout/aout64.h $(INCDIR)/aout/stab_gnu.h $(INCDIR)/aout/stab.def \
  $(INCDIR)/aout/ar.h
m68klinux.lo: m68klinux.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  $(INCDIR)/aout/aout64.h $(INCDIR)/aout/stab_gnu.h $(INCDIR)/aout/stab.def \
  $(INCDIR)/aout/ar.h libaout.h $(INCDIR)/bfdlink.h aout-target.h
m68knetbsd.lo: m68knetbsd.c netbsd.h $(INCDIR)/filenames.h \
  $(INCDIR)/hashtab.h libaout.h $(INCDIR)/bfdlink.h aout-target.h \
  $(INCDIR)/aout/aout64.h $(INCDIR)/aout/stab_gnu.h $(INCDIR)/aout/stab.def \
  $(INCDIR)/aout/ar.h
m88kmach3.lo: m88kmach3.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  libaout.h $(INCDIR)/bfdlink.h aout-target.h $(INCDIR)/aout/aout64.h \
  $(INCDIR)/aout/stab_gnu.h $(INCDIR)/aout/stab.def $(INCDIR)/aout/ar.h
m88kopenbsd.lo: m88kopenbsd.c netbsd.h $(INCDIR)/filenames.h \
  $(INCDIR)/hashtab.h libaout.h $(INCDIR)/bfdlink.h aout-target.h \
  $(INCDIR)/aout/aout64.h $(INCDIR)/aout/stab_gnu.h $(INCDIR)/aout/stab.def \
  $(INCDIR)/aout/ar.h
mach-o.lo: mach-o.c mach-o.h bfd_stdint.h bfd-in2.h \
  bfd-in3.h $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  $(INCDIR)/libiberty.h $(INCDIR)/aout/stab_gnu.h $(INCDIR)/aout/stab.def \
  mach-o/reloc.h mach-o/external.h mach-o-target.c
mipsbsd.lo: mipsbsd.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  libaout.h $(INCDIR)/bfdlink.h aout-target.h $(INCDIR)/aout/aout64.h \
  $(INCDIR)/aout/stab_gnu.h $(INCDIR)/aout/stab.def $(INCDIR)/aout/ar.h
newsos3.lo: newsos3.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  $(INCDIR)/aout/aout64.h $(INCDIR)/aout/stab_gnu.h $(INCDIR)/aout/stab.def \
  $(INCDIR)/aout/ar.h libaout.h $(INCDIR)/bfdlink.h aout-target.h
nlm.lo: nlm.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  libnlm.h $(INCDIR)/nlm/common.h $(INCDIR)/nlm/internal.h \
  $(INCDIR)/nlm/external.h
nlm32-i386.lo: nlm32-i386.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  $(INCDIR)/nlm/i386-ext.h libnlm.h $(INCDIR)/nlm/common.h \
  $(INCDIR)/nlm/internal.h $(INCDIR)/nlm/external.h nlmswap.h \
  nlm-target.h
nlm32-sparc.lo: nlm32-sparc.c $(INCDIR)/filenames.h \
  $(INCDIR)/hashtab.h $(INCDIR)/nlm/sparc32-ext.h libnlm.h \
  $(INCDIR)/nlm/common.h $(INCDIR)/nlm/internal.h $(INCDIR)/nlm/external.h \
  nlmswap.h nlm-target.h
nlm32-ppc.lo: nlm32-ppc.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  $(INCDIR)/nlm/ppc-ext.h libnlm.h $(INCDIR)/nlm/common.h \
  $(INCDIR)/nlm/internal.h $(INCDIR)/nlm/external.h nlmswap.h \
  nlm-target.h
nlm32.lo: nlm32.c nlmcode.h $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  libnlm.h $(INCDIR)/nlm/common.h $(INCDIR)/nlm/internal.h \
  $(INCDIR)/nlm/external.h
ns32knetbsd.lo: ns32knetbsd.c netbsd.h $(INCDIR)/filenames.h \
  $(INCDIR)/hashtab.h libaout.h $(INCDIR)/bfdlink.h aout-target.h \
  $(INCDIR)/aout/aout64.h $(INCDIR)/aout/stab_gnu.h $(INCDIR)/aout/stab.def \
  $(INCDIR)/aout/ar.h
oasys.lo: oasys.c $(INCDIR)/filenames.h $(INCDIR)/safe-ctype.h \
  $(INCDIR)/hashtab.h $(INCDIR)/oasys.h liboasys.h
pc532-mach.lo: pc532-mach.c $(INCDIR)/filenames.h libaout.h \
  $(INCDIR)/bfdlink.h $(INCDIR)/hashtab.h $(INCDIR)/aout/aout64.h \
  aout-target.h $(INCDIR)/aout/stab_gnu.h $(INCDIR)/aout/stab.def \
  $(INCDIR)/aout/ar.h
pdp11.lo: pdp11.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  libaout.h $(INCDIR)/bfdlink.h aout-target.h $(INCDIR)/aout/aout64.h \
  $(INCDIR)/aout/stab_gnu.h $(INCDIR)/aout/stab.def $(INCDIR)/aout/ar.h \
  $(INCDIR)/safe-ctype.h
pef.lo: pef.c $(INCDIR)/safe-ctype.h pef.h pef-traceback.h \
  $(INCDIR)/filenames.h $(INCDIR)/hashtab.h $(INCDIR)/libiberty.h
pe-arm.lo: pe-arm.c $(INCDIR)/filenames.h coff-arm.c \
  $(INCDIR)/hashtab.h $(INCDIR)/coff/arm.h $(INCDIR)/coff/external.h \
  $(INCDIR)/coff/internal.h $(INCDIR)/coff/pe.h libcoff.h \
  $(INCDIR)/bfdlink.h coffcode.h peicode.h libpei.h
pei-arm.lo: pei-arm.c $(INCDIR)/filenames.h coff-arm.c \
  $(INCDIR)/hashtab.h $(INCDIR)/coff/arm.h $(INCDIR)/coff/external.h \
  $(INCDIR)/coff/internal.h $(INCDIR)/coff/pe.h libcoff.h \
  $(INCDIR)/bfdlink.h coffcode.h peicode.h libpei.h
pe-i386.lo: pe-i386.c $(INCDIR)/filenames.h coff-i386.c \
  $(INCDIR)/hashtab.h $(INCDIR)/coff/i386.h $(INCDIR)/coff/external.h \
  $(INCDIR)/coff/internal.h $(INCDIR)/coff/pe.h libcoff.h \
  $(INCDIR)/bfdlink.h coffcode.h peicode.h libpei.h
pei-i386.lo: pei-i386.c $(INCDIR)/filenames.h coff-i386.c \
  $(INCDIR)/hashtab.h $(INCDIR)/coff/i386.h $(INCDIR)/coff/external.h \
  $(INCDIR)/coff/internal.h $(INCDIR)/coff/pe.h libcoff.h \
  $(INCDIR)/bfdlink.h coffcode.h peicode.h libpei.h
pe-mcore.lo: pe-mcore.c $(INCDIR)/filenames.h coff-mcore.c \
  $(INCDIR)/hashtab.h $(INCDIR)/coff/mcore.h $(INCDIR)/coff/external.h \
  $(INCDIR)/coff/internal.h $(INCDIR)/coff/pe.h libcoff.h \
  $(INCDIR)/bfdlink.h coffcode.h peicode.h libpei.h
pei-mcore.lo: pei-mcore.c $(INCDIR)/filenames.h coff-mcore.c \
  $(INCDIR)/hashtab.h $(INCDIR)/coff/mcore.h $(INCDIR)/coff/external.h \
  $(INCDIR)/coff/internal.h $(INCDIR)/coff/pe.h libcoff.h \
  $(INCDIR)/bfdlink.h coffcode.h peicode.h libpei.h
pe-ppc.lo: pe-ppc.c $(INCDIR)/filenames.h coff-ppc.c \
  $(INCDIR)/hashtab.h $(INCDIR)/coff/powerpc.h $(INCDIR)/coff/external.h \
  $(INCDIR)/coff/internal.h $(INCDIR)/coff/pe.h libcoff.h \
  $(INCDIR)/bfdlink.h coffcode.h peicode.h libpei.h
pei-ppc.lo: pei-ppc.c $(INCDIR)/filenames.h coff-ppc.c \
  $(INCDIR)/hashtab.h $(INCDIR)/coff/powerpc.h $(INCDIR)/coff/external.h \
  $(INCDIR)/coff/internal.h $(INCDIR)/coff/pe.h libcoff.h \
  $(INCDIR)/bfdlink.h coffcode.h peicode.h libpei.h
pe-sh.lo: pe-sh.c $(INCDIR)/filenames.h coff-sh.c $(INCDIR)/libiberty.h \
  $(INCDIR)/hashtab.h $(INCDIR)/bfdlink.h $(INCDIR)/coff/sh.h \
  $(INCDIR)/coff/external.h $(INCDIR)/coff/internal.h \
  $(INCDIR)/coff/pe.h libcoff.h coffcode.h peicode.h \
  libpei.h
pei-sh.lo: pei-sh.c $(INCDIR)/filenames.h coff-sh.c \
  $(INCDIR)/libiberty.h $(INCDIR)/hashtab.h $(INCDIR)/bfdlink.h \
  $(INCDIR)/coff/sh.h $(INCDIR)/coff/external.h $(INCDIR)/coff/internal.h \
  $(INCDIR)/coff/pe.h libcoff.h coffcode.h peicode.h \
  libpei.h
pe-mips.lo: pe-mips.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  $(INCDIR)/coff/mipspe.h $(INCDIR)/coff/external.h $(INCDIR)/coff/internal.h \
  $(INCDIR)/coff/pe.h libcoff.h $(INCDIR)/bfdlink.h coffcode.h \
  peicode.h libpei.h
pei-mips.lo: pei-mips.c $(INCDIR)/filenames.h pe-mips.c \
  $(INCDIR)/hashtab.h $(INCDIR)/coff/mipspe.h $(INCDIR)/coff/external.h \
  $(INCDIR)/coff/internal.h $(INCDIR)/coff/pe.h libcoff.h \
  $(INCDIR)/bfdlink.h coffcode.h peicode.h libpei.h
ppcboot.lo: ppcboot.c $(INCDIR)/safe-ctype.h $(INCDIR)/filenames.h \
  $(INCDIR)/hashtab.h
reloc16.lo: reloc16.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  $(INCDIR)/bfdlink.h genlink.h $(INCDIR)/coff/internal.h \
  libcoff.h
riscix.lo: riscix.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  libaout.h $(INCDIR)/bfdlink.h $(INCDIR)/aout/aout64.h \
  aout-target.h $(INCDIR)/aout/stab_gnu.h $(INCDIR)/aout/stab.def \
  $(INCDIR)/aout/ar.h
sparclinux.lo: sparclinux.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  $(INCDIR)/aout/aout64.h $(INCDIR)/aout/stab_gnu.h $(INCDIR)/aout/stab.def \
  $(INCDIR)/aout/ar.h libaout.h $(INCDIR)/bfdlink.h aout-target.h
sparclynx.lo: sparclynx.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  $(INCDIR)/aout/sun4.h libaout.h $(INCDIR)/bfdlink.h \
  $(INCDIR)/aout/aout64.h $(INCDIR)/aout/stab_gnu.h $(INCDIR)/aout/stab.def \
  $(INCDIR)/aout/ar.h aout-target.h
sparcnetbsd.lo: sparcnetbsd.c netbsd.h $(INCDIR)/filenames.h \
  $(INCDIR)/hashtab.h libaout.h $(INCDIR)/bfdlink.h aout-target.h \
  $(INCDIR)/aout/aout64.h $(INCDIR)/aout/stab_gnu.h $(INCDIR)/aout/stab.def \
  $(INCDIR)/aout/ar.h
sunos.lo: sunos.c $(INCDIR)/bfdlink.h libaout.h aoutf1.h \
  $(INCDIR)/filenames.h $(INCDIR)/hashtab.h $(INCDIR)/aout/sun4.h \
  $(INCDIR)/aout/aout64.h $(INCDIR)/aout/stab_gnu.h $(INCDIR)/aout/stab.def \
  $(INCDIR)/aout/ar.h aout-target.h
vaxnetbsd.lo: vaxnetbsd.c netbsd.h $(INCDIR)/filenames.h \
  $(INCDIR)/hashtab.h libaout.h $(INCDIR)/bfdlink.h aout-target.h \
  $(INCDIR)/aout/aout64.h $(INCDIR)/aout/stab_gnu.h $(INCDIR)/aout/stab.def \
  $(INCDIR)/aout/ar.h
vax1knetbsd.lo: vax1knetbsd.c netbsd.h $(INCDIR)/filenames.h \
  $(INCDIR)/hashtab.h libaout.h $(INCDIR)/bfdlink.h aout-target.h \
  $(INCDIR)/aout/aout64.h $(INCDIR)/aout/stab_gnu.h $(INCDIR)/aout/stab.def \
  $(INCDIR)/aout/ar.h
vaxbsd.lo: vaxbsd.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  libaout.h $(INCDIR)/bfdlink.h aout-target.h $(INCDIR)/aout/aout64.h \
  $(INCDIR)/aout/stab_gnu.h $(INCDIR)/aout/stab.def $(INCDIR)/aout/ar.h
versados.lo: versados.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  $(INCDIR)/libiberty.h
vms.lo: vms.c $(INCDIR)/filenames.h $(INCDIR)/bfdlink.h \
  $(INCDIR)/hashtab.h vms.h
vms-gsd.lo: vms-gsd.c $(INCDIR)/filenames.h $(INCDIR)/bfdlink.h \
  $(INCDIR)/hashtab.h vms.h
vms-hdr.lo: vms-hdr.c bfdver.h $(INCDIR)/filenames.h \
  $(INCDIR)/bfdlink.h $(INCDIR)/safe-ctype.h $(INCDIR)/hashtab.h \
  vms.h
vms-lib.lo: vms-lib.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  $(INCDIR)/safe-ctype.h bfdver.h vms.h $(INCDIR)/vms/lbr.h \
  $(INCDIR)/vms/dcx.h
vms-misc.lo: vms-misc.c $(INCDIR)/filenames.h $(INCDIR)/bfdlink.h \
  $(INCDIR)/hashtab.h vms.h
vms-tir.lo: vms-tir.c $(INCDIR)/filenames.h $(INCDIR)/bfdlink.h \
  $(INCDIR)/hashtab.h vms.h
xcofflink.lo: xcofflink.c $(INCDIR)/filenames.h $(INCDIR)/bfdlink.h \
  $(INCDIR)/hashtab.h $(INCDIR)/coff/internal.h $(INCDIR)/coff/xcoff.h \
  libcoff.h libxcoff.h
xsym.lo: xsym.c xsym.h $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
xtensa-isa.lo: xtensa-isa.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  $(INCDIR)/xtensa-isa.h $(INCDIR)/xtensa-isa-internal.h
xtensa-modules.lo: xtensa-modules.c $(INCDIR)/xtensa-isa.h \
  $(INCDIR)/xtensa-isa-internal.h
aix5ppc-core.lo: aix5ppc-core.c
aout64.lo: aout64.c aoutx.h $(INCDIR)/filenames.h $(INCDIR)/safe-ctype.h \
  $(INCDIR)/bfdlink.h libaout.h $(INCDIR)/hashtab.h $(INCDIR)/aout/aout64.h \
  $(INCDIR)/aout/stab_gnu.h $(INCDIR)/aout/stab.def $(INCDIR)/aout/ar.h
coff-alpha.lo: coff-alpha.c $(INCDIR)/filenames.h $(INCDIR)/bfdlink.h \
  $(INCDIR)/hashtab.h $(INCDIR)/coff/internal.h $(INCDIR)/coff/sym.h \
  $(INCDIR)/coff/symconst.h $(INCDIR)/coff/ecoff.h $(INCDIR)/coff/alpha.h \
  $(INCDIR)/aout/ar.h libcoff.h libecoff.h coffswap.h \
  ecoffswap.h
coff64-rs6000.lo: coff64-rs6000.c $(INCDIR)/filenames.h \
  $(INCDIR)/bfdlink.h $(INCDIR)/hashtab.h $(INCDIR)/coff/internal.h \
  $(INCDIR)/coff/xcoff.h $(INCDIR)/coff/rs6k64.h libcoff.h \
  libxcoff.h coffcode.h coffswap.h
demo64.lo: demo64.c aoutf1.h $(INCDIR)/filenames.h \
  $(INCDIR)/hashtab.h $(INCDIR)/aout/sun4.h libaout.h \
  $(INCDIR)/bfdlink.h $(INCDIR)/aout/aout64.h $(INCDIR)/aout/stab_gnu.h \
  $(INCDIR)/aout/stab.def $(INCDIR)/aout/ar.h aout-target.h
efi-app-ia64.lo: efi-app-ia64.c $(INCDIR)/filenames.h \
  coff-ia64.c $(INCDIR)/hashtab.h $(INCDIR)/coff/ia64.h \
  $(INCDIR)/coff/external.h $(INCDIR)/coff/internal.h \
  $(INCDIR)/coff/pe.h libcoff.h $(INCDIR)/bfdlink.h coffcode.h \
  peicode.h libpei.h
elf64-x86-64.lo: elf64-x86-64.c $(INCDIR)/filenames.h \
  $(INCDIR)/bfdlink.h $(INCDIR)/hashtab.h elf-bfd.h $(INCDIR)/elf/common.h \
  $(INCDIR)/elf/internal.h $(INCDIR)/elf/external.h $(INCDIR)/elf/x86-64.h \
  $(INCDIR)/elf/reloc-macros.h elf64-target.h
elf64-alpha.lo: elf64-alpha.c $(INCDIR)/filenames.h \
  $(INCDIR)/hashtab.h elf-bfd.h $(INCDIR)/elf/common.h \
  $(INCDIR)/elf/internal.h $(INCDIR)/elf/external.h $(INCDIR)/bfdlink.h \
  $(INCDIR)/elf/alpha.h $(INCDIR)/elf/reloc-macros.h \
  $(INCDIR)/coff/internal.h $(INCDIR)/coff/sym.h $(INCDIR)/coff/symconst.h \
  $(INCDIR)/coff/ecoff.h $(INCDIR)/coff/alpha.h $(INCDIR)/aout/ar.h \
  libcoff.h libecoff.h ecoffswap.h elf64-target.h
elf64-hppa.lo: elf64-hppa.c $(INCDIR)/alloca-conf.h \
  $(INCDIR)/filenames.h $(INCDIR)/hashtab.h elf-bfd.h \
  $(INCDIR)/elf/common.h $(INCDIR)/elf/internal.h $(INCDIR)/elf/external.h \
  $(INCDIR)/bfdlink.h $(INCDIR)/elf/hppa.h $(INCDIR)/elf/reloc-macros.h \
  libhppa.h elf64-hppa.h elf-hppa.h elf64-target.h
elf64-gen.lo: elf64-gen.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  elf-bfd.h $(INCDIR)/elf/common.h $(INCDIR)/elf/internal.h \
  $(INCDIR)/elf/external.h $(INCDIR)/bfdlink.h elf64-target.h
elfn32-mips.lo: elfn32-mips.c $(INCDIR)/filenames.h \
  $(INCDIR)/hashtab.h $(INCDIR)/bfdlink.h genlink.h elf-bfd.h \
  $(INCDIR)/elf/common.h $(INCDIR)/elf/internal.h $(INCDIR)/elf/external.h \
  elfxx-mips.h $(INCDIR)/elf/mips.h $(INCDIR)/elf/reloc-macros.h \
  $(INCDIR)/coff/sym.h $(INCDIR)/coff/symconst.h $(INCDIR)/coff/internal.h \
  $(INCDIR)/coff/ecoff.h $(INCDIR)/coff/mips.h $(INCDIR)/coff/external.h \
  ecoffswap.h elf32-target.h
elf64-mips.lo: elf64-mips.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  $(INCDIR)/aout/ar.h $(INCDIR)/bfdlink.h genlink.h elf-bfd.h \
  $(INCDIR)/elf/common.h $(INCDIR)/elf/internal.h $(INCDIR)/elf/external.h \
  elfxx-mips.h $(INCDIR)/elf/mips.h $(INCDIR)/elf/reloc-macros.h \
  $(INCDIR)/coff/sym.h $(INCDIR)/coff/symconst.h $(INCDIR)/coff/internal.h \
  $(INCDIR)/coff/ecoff.h $(INCDIR)/coff/alpha.h ecoffswap.h \
  elf64-target.h
elf64-mmix.lo: elf64-mmix.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  elf-bfd.h $(INCDIR)/elf/common.h $(INCDIR)/elf/internal.h \
  $(INCDIR)/elf/external.h $(INCDIR)/bfdlink.h $(INCDIR)/elf/mmix.h \
  $(INCDIR)/elf/reloc-macros.h $(INCDIR)/opcode/mmix.h \
  elf64-target.h
elf64-ppc.lo: elf64-ppc.c $(INCDIR)/filenames.h $(INCDIR)/bfdlink.h \
  $(INCDIR)/hashtab.h elf-bfd.h $(INCDIR)/elf/common.h \
  $(INCDIR)/elf/internal.h $(INCDIR)/elf/external.h $(INCDIR)/elf/ppc64.h \
  $(INCDIR)/elf/reloc-macros.h elf64-ppc.h elf64-target.h
elf64-s390.lo: elf64-s390.c $(INCDIR)/filenames.h $(INCDIR)/bfdlink.h \
  $(INCDIR)/hashtab.h elf-bfd.h $(INCDIR)/elf/common.h \
  $(INCDIR)/elf/internal.h $(INCDIR)/elf/external.h $(INCDIR)/elf/s390.h \
  $(INCDIR)/elf/reloc-macros.h elf64-target.h
elf64-sh64.lo: elf64-sh64.c $(INCDIR)/filenames.h $(INCDIR)/bfdlink.h \
  $(INCDIR)/hashtab.h elf-bfd.h $(INCDIR)/elf/common.h \
  $(INCDIR)/elf/internal.h $(INCDIR)/elf/external.h $(INCDIR)/elf/sh.h \
  $(INCDIR)/elf/reloc-macros.h elf64-target.h
elf64-sparc.lo: elf64-sparc.c $(INCDIR)/filenames.h \
  $(INCDIR)/hashtab.h elf-bfd.h $(INCDIR)/elf/common.h \
  $(INCDIR)/elf/internal.h $(INCDIR)/elf/external.h $(INCDIR)/bfdlink.h \
  $(INCDIR)/elf/sparc.h $(INCDIR)/elf/reloc-macros.h \
  $(INCDIR)/opcode/sparc.h elfxx-sparc.h elf64-target.h
elf64.lo: elf64.c elfcode.h $(INCDIR)/filenames.h $(INCDIR)/libiberty.h \
  $(INCDIR)/bfdlink.h $(INCDIR)/hashtab.h elf-bfd.h $(INCDIR)/elf/common.h \
  $(INCDIR)/elf/internal.h $(INCDIR)/elf/external.h elfcore.h
mmo.lo: mmo.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  $(INCDIR)/libiberty.h $(INCDIR)/elf/mmix.h $(INCDIR)/elf/reloc-macros.h \
  $(INCDIR)/opcode/mmix.h
nlm32-alpha.lo: nlm32-alpha.c $(INCDIR)/filenames.h \
  $(INCDIR)/hashtab.h $(INCDIR)/nlm/alpha-ext.h libnlm.h \
  $(INCDIR)/nlm/common.h $(INCDIR)/nlm/internal.h $(INCDIR)/nlm/external.h \
  nlmswap.h nlm-target.h
nlm64.lo: nlm64.c nlmcode.h $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  libnlm.h $(INCDIR)/nlm/common.h $(INCDIR)/nlm/internal.h \
  $(INCDIR)/nlm/external.h
pei-ia64.lo: pei-ia64.c $(INCDIR)/filenames.h coff-ia64.c \
  $(INCDIR)/hashtab.h $(INCDIR)/coff/ia64.h $(INCDIR)/coff/external.h \
  $(INCDIR)/coff/internal.h $(INCDIR)/coff/pe.h libcoff.h \
  $(INCDIR)/bfdlink.h coffcode.h peicode.h libpei.h
aix386-core.lo: aix386-core.c $(INCDIR)/filenames.h \
  $(INCDIR)/hashtab.h $(INCDIR)/coff/i386.h $(INCDIR)/coff/external.h \
  $(INCDIR)/coff/internal.h libcoff.h $(INCDIR)/bfdlink.h
hpux-core.lo: hpux-core.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
irix-core.lo: irix-core.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
lynx-core.lo: lynx-core.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
osf-core.lo: osf-core.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
sco5-core.lo: sco5-core.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  libaout.h $(INCDIR)/bfdlink.h
trad-core.lo: trad-core.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  libaout.h $(INCDIR)/bfdlink.h
cisco-core.lo: cisco-core.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h
elf32-ia64.lo: elf32-ia64.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  elf-bfd.h $(INCDIR)/elf/common.h $(INCDIR)/elf/internal.h \
  $(INCDIR)/elf/external.h $(INCDIR)/bfdlink.h $(INCDIR)/opcode/ia64.h \
  $(INCDIR)/elf/ia64.h $(INCDIR)/elf/reloc-macros.h $(INCDIR)/objalloc.h \
  elf32-target.h
elf64-ia64.lo: elf64-ia64.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  elf-bfd.h $(INCDIR)/elf/common.h $(INCDIR)/elf/internal.h \
  $(INCDIR)/elf/external.h $(INCDIR)/bfdlink.h $(INCDIR)/opcode/ia64.h \
  $(INCDIR)/elf/ia64.h $(INCDIR)/elf/reloc-macros.h $(INCDIR)/objalloc.h \
  elf64-target.h
peigen.lo: peigen.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  $(INCDIR)/coff/internal.h $(INCDIR)/coff/i386.h $(INCDIR)/coff/external.h \
  $(INCDIR)/coff/pe.h libcoff.h $(INCDIR)/bfdlink.h libpei.h
pepigen.lo: pepigen.c $(INCDIR)/filenames.h $(INCDIR)/hashtab.h \
  $(INCDIR)/coff/internal.h $(INCDIR)/coff/ia64.h $(INCDIR)/coff/external.h \
  $(INCDIR)/coff/pe.h libcoff.h $(INCDIR)/bfdlink.h libpei.h
# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
