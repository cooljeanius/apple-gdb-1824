## automake Makefile for cgen
## Process this file with automake to generate Makefile.in

AUTOMAKE_OPTIONS = 1.9 gnits
AUTOMAKE_DEPRECATED_OPTIONS = cygnus

SUBDIRS = doc

GUILE = "`if test -f ../guile/libguile/guile ; then echo ../guile/libguile/guile; else echo guile ; fi`" -l $(srcdir)/guile.scm -s
CGENFLAGS = -v
CPU_ARCH = @cpu_arch@
ARCHFILE = $(srcroot)/cpu/$(CPU_ARCH).cpu

dist_noinst_SCRIPTS = gen-all-doc gen-all-opcodes gen-all-sid gen-all-sim

# for various utility rules:
MACHS = all
ISAS = all

# for the html rule:
INSN_FILE_NAME = $(CPU_ARCH)-insn.html

srcroot = $(srcdir)/..
PATH_TO_MOVEIFCHANGE = $(srcroot)/move-if-change
RUN_MOVEIFCHANGE = $(SHELL) $(PATH_TO_MOVEIFCHANGE)

# Applications depend on stamp-cgen to tell them when .scm files have
# been changed (so files need to be regenerated).
# ??? Application specific files are kept with cgen for now, but may
# eventually go with the app. stamp-cgen might still be useful to track
# app-independent files.

all-local: stamp-cgen

stamp-cgen: $(CGENFILES)
	rm -f stamp-cgen
	echo "timestamp" > stamp-cgen

all: @MAINT@ all-local

# Phony targets to run each of the applications,
# though most of these are for development purposes only.
# When actually building the toolchain, the Makefile in the appropriate
# directory will run cgen.
#
# NOTE: If running for a cpu other than the configured one you may wish to
# override ISAS and MACHS.

# Build the basic description support.
# We just stuff them in tmp-* files.
# Usage: make desc [ARCHFILE=<arch-file>] OPTIONS="<option list>"

.PHONY: desc
# FIXME: needs more dependencies:
desc: desc.scm $(srcdir)/cgen-opc.scm @MAINT@ $(ARCHFILE)
	rm -f tmp-desc.h tmp-desc.c tmp-opinst.c
	$(GUILE) $(srcdir)/cgen-opc.scm \
		-s $(srcdir) \
		$(CGENFLAGS) \
		-f "$(OPTIONS)" \
		-a $(ARCHFILE) \
		-i "$(ISAS)" \
		-m "$(MACHS)" \
		-H tmp-desc.h -C tmp-desc.c

# Build the machine generated cpu documentation.
# Usage: make html-local [CPU_ARCH=<cpu_arch>] [ARCHFILE=<arch-file>]
.PHONY: html-local
html-local: desc.scm html.scm cgen-doc.scm @MAINT@ $(PATH_TO_MOVEIFCHANGE)
	rm -f tmp-doc.html
	$(GUILE) $(srcdir)/cgen-doc.scm \
		-s $(srcdir) \
		$(CGENFLAGS) \
		-f "$(OPTIONS)" \
		-a $(ARCHFILE) \
		-i "$(ISAS)" \
		-m "$(MACHS)" \
		-N $(INSN_FILE_NAME) \
		-H tmp.html \
		-I tmp-insn.html
	$(RUN_MOVEIFCHANGE) tmp.html $(CPU_ARCH).html
	$(RUN_MOVEIFCHANGE) tmp-insn.html $(CPU_ARCH)-insn.html

# Build the opcodes files.
# We just stuff them in tmp-* files.
# Usage: make opcodes [ARCHFILE=<arch-file>] OPTIONS="<option list>"

.PHONY: opcodes
# FIXME: needs more dependencies:
opcodes: opcodes.scm $(srcdir)/cgen-opc.scm @MAINT@ $(ARCHFILE)
	rm -f tmp-opc.h tmp-itab.c
	rm -f tmp-asm.in tmp-dis.in tmp-ibld.h tmp-ibld.in
	$(GUILE) $(srcdir)/cgen-opc.scm \
		-s $(srcdir) \
		$(CGENFLAGS) \
		-f "$(OPTIONS) opinst" \
		-a $(ARCHFILE) \
		-i "$(ISAS)" \
		-m "$(MACHS)" \
		-O tmp-opc.h -P tmp-opc.c -Q tmp-opinst.c \
		-B tmp-ibld.h -L tmp-ibld.in \
		-A tmp-asm.in -D tmp-dis.in

# Build the simulator files.
# We just stuff them in tmp-* files.
# Usage: make sim-arch [ARCHFILE=<arch-file>] OPTIONS="<option list>"
#    make sim-cpu [ARCHFILE=<arch-file>] ISAS="<isa>" MACHS="<mach list>" \
#      OPTIONS="<option list>"

.PHONY: sim-arch sim-cpu
# FIXME: needs more dependencies:
sim-arch: sim.scm $(srcdir)/cgen-sim.scm @MAINT@ $(ARCHFILE)
	rm -f tmp-arch.h tmp-arch.c tmp-cpuall.h
	$(GUILE) $(srcdir)/cgen-sim.scm \
		-s $(srcdir) \
		$(CGENFLAGS) \
		-f "$(OPTIONS)" \
		-a $(ARCHFILE) \
		-i "$(ISAS)" \
		-m "$(MACHS)" \
		-A tmp-arch.h -B tmp-arch.c -N tmp-cpuall.h
sim-cpu: sim.scm $(srcdir)/cgen-sim.scm @MAINT@ $(ARCHFILE)
	rm -f tmp-cpu.h tmp-cpu.c tmp-decode.h tmp-decode.c
	rm -f tmp-model.c tmp-sem.c tmp-sem-switch.c
	$(GUILE) $(srcdir)/cgen-sim.scm \
		-s $(srcdir) \
		$(CGENFLAGS) \
		-f "$(OPTIONS)" \
		-a $(ARCHFILE) \
		-i "$(ISAS)" \
		-m "$(MACHS)" \
		-C tmp-cpu.h -U tmp-cpu.c \
		-T tmp-decode.h -D tmp-decode.c \
		-M tmp-model.c \
		-S tmp-semantics.c -X tmp-sem-switch.c

# Build GAS testcase generator.
# Usage: make gas-test [ARCHFILE=<arch-file>]

.PHONY: gas-test
gas-test: gas-test.scm cgen-gas.scm @MAINT@ $(ARCHFILE)
	@if test -z "$(ISAS)"; then \
	  echo "ISAS not specified!" >&2;\
	  exit 1;\
	fi
	$(GUILE) $(srcdir)/cgen-gas.scm \
		-s $(srcdir) \
		$(CGENFLAGS) \
		-a $(ARCHFILE) \
		-i "$(ISAS)" \
		-m "$(MACHS)" \
		-B gas-build.sh \
		-E gas-allinsn.exp

# Build simulator testcase generator.
# Usage: make sim-test [ARCHFILE=<arch-file>]

.PHONY: sim-test
sim-test: sim-test.scm cgen-stest.scm @MAINT@ $(ARCHFILE)
	@if test -z "$(ISAS)"; then \
	  echo "ISAS not specified!" >&2;\
	  exit 1;\
	fi
	$(GUILE) $(srcdir)/cgen-stest.scm \
		-s $(srcdir) \
		$(CGENFLAGS) \
		-a $(ARCHFILE) \
		-i "$(ISAS)" \
		-m "$(MACHS)" \
		-B sim-build.sh \
		-E sim-allinsn.exp

CLEANFILES = tmp-*
DISTCLEANFILES = stamp-cgen

## is that all?
