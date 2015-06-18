## automake Makefile for binutils source
## Process this file with automake to generate Makefile.in

AUTOMAKE_OPTIONS = 1.11.6 gnits dejagnu no-dist
## "cygnus" == "foreign no-dist" (we hardened from foreign to gnits though)
AUTOMAKE_DEPRECATED_OPTIONS = cygnus

ACLOCAL_AMFLAGS = -I m4 --install

ACLOCAL_EXTRA_AMFLAGS = -I .. -I ../config -I ../bfd

# APPLE LOCAL just skip the doc directory altogether - it is building
# info files in the srcdir and hosing B&I (Build & Integration) builds.
SUBDIRS = po
if GENINSRC_NEVER
NON_APPLE_SUBDIRS = doc
else
SUBDIRS += doc
endif
## (end GENINSRC_NEVER)

tooldir = $(exec_prefix)/$(target_alias)

## These are NOT set by automake, because they appear in
## bfd/acinclude.m4, which is included by binutils/acinclude.m4, and
## thus is not seen by automake.
CC_FOR_BUILD = @CC_FOR_BUILD@
EXEEXT_FOR_BUILD = @EXEEXT_FOR_BUILD@
CFLAGS_FOR_BUILD = @CFLAGS_FOR_BUILD@
CXXFLAGS_FOR_BUILD = @CXXFLAGS_FOR_BUILD@
LDFLAGS_FOR_BUILD = @LDFLAGS_FOR_BUILD@

## extend the automake verbosity control:
AM_V_v = $(am__v_v_@AM_V@)
am__v_v_ = $(am__v_v_@AM_DEFAULT_V@)
am__v_v_0 =
am__v_v_1 = -v

AM_V_redir = $(am__v_redir_@AM_V@)
am__v_redir_ = $(am__v_redir_@AM_DEFAULT_V@)
am__v_redir_0 = >
am__v_redir_1 = | tee

AM_V_redir_a = $(am__v_redir_a_@AM_V@)
am__v_redir_a_ = $(am__v_redir_a_@AM_DEFAULT_V@)
am__v_redir_a_0 = >>
am__v_redir_a_1 = | tee -a

YACC = `if [ -f ../bison/bison ]; then echo ../bison/bison -y -L$(srcdir)/../bison/; else echo @YACC@; fi`
AM_YFLAGS = -d -t $(AM_V_v) @MAINT@@EXTRA_YFLAGS_SEP@ -k -Wall -Wyacc
LEX = `if [ -f ../flex/flex ]; then echo ../flex/flex; else echo @LEX@; fi`
AM_LFLAGS = @MAINT@ -d

# Automake 1.10+ disables lex and yacc output file regeneration if
# maintainer mode is disabled.  Avoid this.
am__skiplex =
am__skipyacc =

## also needed:
HOST_ARCHITECTURE = @HOST_ARCHITECTURE@
## not strictly necessary, but goes with the previous:
TARGET_ARCHITECTURE = @TARGET_ARCHITECTURE@
## use the host one, because that is the one that matters:
C_ARCHFLAGS = -arch $(HOST_ARCHITECTURE)

WARN_CFLAGS = @WARN_CFLAGS@
NO_WERROR = @NO_WERROR@
AM_CFLAGS = $(WARN_CFLAGS) $(C_ARCHFLAGS)
AM_LDFLAGS = -framework CoreFoundation -L../readline -L../intl \
  -L../libiberty -L../bfd -L../opcodes

# these two are almost the same program (really?):
AR_PROG=ar
RANLIB_PROG=ranlib

# objcopy and strip should be the same program (really?):
OBJCOPY_PROG=objcopy
STRIP_PROG=strip-new

STRINGS_PROG=strings

READELF_PROG=readelf

ELFEDIT_PROG=elfedit

# These should all be the same program too. (really?)
SIZE_PROG=size
NM_PROG=nm-new
OBJDUMP_PROG=objdump

# This is the demangler, as a standalone program.
# Note: This one is used as the installed name too, unlike the above.
DEMANGLER_PROG=cxxfilt

ADDR2LINE_PROG=addr2line

NLMCONV_PROG=nlmconv
DLLTOOL_PROG=dlltool
WINDRES_PROG=windres
WINDMC_PROG=windmc
DLLWRAP_PROG=dllwrap

SRCONV_PROG=srconv$(EXEEXT) sysdump$(EXEEXT) coffdump$(EXEEXT) 

PROGS = $(SIZE_PROG) $(OBJDUMP_PROG) $(NM_PROG) $(AR_PROG) \
  $(STRINGS_PROG) $(STRIP_PROG) $(RANLIB_PROG) $(DEMANGLER_PROG) \
  $(OBJCOPY_PROG) @BUILD_NLMCONV@ @BUILD_SRCONV@ @BUILD_DLLTOOL@ \
  @BUILD_WINDRES@ $(ADDR2LINE_PROG) $(READELF_PROG) @BUILD_DLLWRAP@ \
  @BUILD_MISC@

bin_PROGRAMS = $(SIZE_PROG) $(OBJDUMP_PROG) $(AR_PROG) $(STRINGS_PROG) \
  $(RANLIB_PROG) $(OBJCOPY_PROG) @BUILD_NLMCONV@ @BUILD_SRCONV@ \
  @BUILD_DLLTOOL@ @BUILD_WINDRES@ @BUILD_WINDMC@ $(ADDR2LINE_PROG) \
  $(READELF_PROG) $(ELFEDIT_PROG) @BUILD_DLLWRAP@ @BUILD_MISC@

bin_SCRIPTS = @BUILD_INSTALL_MISC@
EXTRA_SCRIPTS = embedspu

## Test programs.
BFDTEST1_PROG = bfdtest1
BFDTEST2_PROG = bfdtest2

TEST_PROGS = $(BFDTEST1_PROG) $(BFDTEST2_PROG)

## We need a special rule to install the programs which are built with
## -new, and to rename cxxfilt to c++filt.
RENAMED_PROGS = $(NM_PROG) $(STRIP_PROG) $(DEMANGLER_PROG)
noinst_PROGRAMS = $(RENAMED_PROGS)
check_PROGRAMS = $(TEST_PROGS)
## (@BUILD_MISC@ has been moved above to bin_PROGRAMS)

## needs to be adjusted to work as proper tests:
TESTS = @MAINT@ # $(check_PROGRAMS)

EXTRA_PROGRAMS = $(NLMCONV_PROG) srconv sysdump coffdump $(DLLTOOL_PROG) \
  $(WINDRES_PROG) $(WINDMC_PROG) $(DLLWRAP_PROG)

# Stuff that goes in tooldir/ if appropriate.
TOOL_PROGS = nm-new strip-new ar ranlib dlltool objdump objcopy

BASEDIR = $(srcdir)/..
BFDDIR = $(BASEDIR)/bfd
INCDIR	= $(BASEDIR)/include

MKDEP = gcc -MM @DEFS@

PATH_TO_MOVEIFCHANGE = $(srcdir)/../move-if-change
RUN_MOVEIFCHANGE = $(SHELL) $(PATH_TO_MOVEIFCHANGE)

EMULATION = @EMULATION@
EMULATION_VECTOR = @EMULATION_VECTOR@

INCINTL = @INCINTL@

AM_CPPFLAGS = -D_GNU_SOURCE \
	 -I. -I$(srcdir) -I../bfd -I$(BFDDIR) -I$(INCDIR) \
	 @HDEFINES@ \
	 -I$(srcdir)/../intl -I../intl \
	 -DLOCALEDIR="\"$(datadir)/locale\"" \
	 -Dbin_dummy_emulation=@EMULATION_VECTOR@

HFILES = \
	arsup.h binemul.h bucomm.h budbg.h budemang.h \
	coffgrok.h debug.h dlltool.h dwarf.h elfcomm.h nlmconv.h \
	objdump.h sysdep.h unwind-ia64.h windres.h winduni.h windint.h \
	windmc.h

GENERATED_HFILES = arparse.h sysroff.h sysinfo.h defparse.h rcparse.h \
	mcparse.h nlmheader.h
BUILT_SOURCES = $(GENERATED_HFILES)

CFILES = \
	addr2line.c ar.c arsup.c bin2c.c binemul.c bucomm.c budemang.c \
	coffdump.c coffgrok.c cxxfilt.c \
	dwarf.c debug.c dlltool.c dllwrap.c \
	elfcomm.c emul_aix.c emul_vanilla.c filemode.c \
	ieee.c is-ranlib.c is-strip.c maybe-ranlib.c maybe-strip.c \
	nlmconv.c nm.c not-ranlib.c not-strip.c \
	objcopy.c objdump.c prdbg.c \
	od-xcoff.c od-macho.c \
	rclex.c rdcoff.c rddbg.c readelf.c rename.c \
	resbin.c rescoff.c resrc.c resres.c \
	size.c srconv.c stabs.c strings.c sysdump.c \
	syslex_wrap.c unwind-ia64.c elfedit.c version.c \
	windres.c winduni.c wrstabs.c windmc.c mclex.c

PREPROC_CFILES = \
	addr2line.i ar.i arsup.i bin2c.i binemul.i bucomm.i budemang.i \
	coffdump.i coffgrok.i cxxfilt.i \
	dwarf.i debug.i dlltool.i dllwrap.i \
	elfcomm.i emul_aix.i emul_vanilla.i filemode.i \
	ieee.i is-ranlib.i is-strip.i maybe-ranlib.i maybe-strip.i \
	nlmconv.i nm.i not-ranlib.i not-strip.i \
	objcopy.i objdump.i prdbg.i \
	od-xcoff.i od-macho.i \
	rclex.i rdcoff.i rddbg.i readelf.i rename.i \
	resbin.i rescoff.i resrc.i resres.i \
	size.i srconv.i stabs.i strings.i sysdump.i \
	syslex_wrap.i unwind-ia64.i elfedit.i version.i \
	windres.i winduni.i wrstabs.i windmc.i mclex.i

ASM_OF_CFILES = \
	addr2line.s ar.s arsup.s bin2c.s binemul.s bucomm.s budemang.s \
	coffdump.s coffgrok.s cxxfilt.s \
	dwarf.s debug.s dlltool.s dllwrap.s \
	elfcomm.s emul_aix.s emul_vanilla.s filemode.s \
	ieee.s is-ranlib.s is-strip.s maybe-ranlib.s maybe-strip.s \
	nlmconv.s nm.s not-ranlib.s not-strip.s \
	objcopy.s objdump.s prdbg.s \
	od-xcoff.s od-macho.s \
	rclex.s rdcoff.s rddbg.s readelf.s rename.s \
	resbin.s rescoff.s resrc.s resres.s \
	size.s srconv.s stabs.s strings.s sysdump.s \
	syslex_wrap.s unwind-ia64.s elfedit.s version.s \
	windres.s winduni.s wrstabs.s windmc.s mclex.s

GENERATED_CFILES = \
	arparse.c arlex.c sysroff.c sysinfo.c syslex.c \
	defparse.c deflex.c nlmheader.c rcparse.c rclex.c mcparse.c
BUILT_SOURCES += $(GENERATED_CFILES)

PREPROC_GENERATED_CFILES = \
	arparse.i arlex.i sysroff.i sysinfo.i syslex.i \
	defparse.i deflex.i nlmheader.i rcparse.i rclex.i mcparse.i

ASM_OF_GENERATED_CFILES = \
	arparse.s arlex.s sysroff.s sysinfo.s syslex.s \
	defparse.s deflex.s nlmheader.s rcparse.s rclex.s mcparse.s

OFILES = \
	addr2line.o ar.o arsup.o binemul.o bucomm.o budemang.o \
	coffdump.o coffgrok.o cxxfilt.o debug.o dlltool.o dllwrap.o \
	emul_aix.o emul_vanilla.o filemode.o \
	ieee.o is-ranlib.o is-strip.o maybe-ranlib.o maybe-strip.o \
	nlmconv.o nm.o not-ranlib.o not-strip.o \
	objcopy.o objdump.o prdbg.o \
	rdcoff.o rddbg.o readelf.o rename.o \
	resbin.o rescoff.o resrc.o resres.o \
	size.o srconv.o stabs.o strings.o sysdump.o version.o \
	windres.o winduni.o wrstabs.o

GENERATED_OFILES = \
	arparse.o arlex.o sysroff.o sysinfo.o syslex.o \
	defparse.o deflex.o nlmheader.o rcparse.o rclex.o

DEBUG_SRCS = rddbg.c debug.c stabs.c ieee.c rdcoff.c
DEBUG_SRC_OFILES = rddbg.o debug.o stabs.o ieee.o rdcoff.o
WRITE_DEBUG_SRCS = $(DEBUG_SRCS) wrstabs.c
WRITE_DEBUG_SRC_OFILES = $(DEBUG_SRC_OFILES) wrstabs.o

# Extra object files for objdump:
OBJDUMP_PRIVATE_OFILES = @OBJDUMP_PRIVATE_OFILES@

# Code shared by all the binutils.
BULIBS = bucomm.c version.c filemode.c
BULIB_OFILES = bucomm.o version.o filemode.o

# Code shared by the ELF related programs.
ELFLIBS = elfcomm.c
ELFLIB_OFILES = elfcomm.o

BFD = ../bfd/libbfd.la
BFD_A = ../bfd/libbfd.a
BFD_DEP = $(BFD)
BFDLIB = $(BFD)

bfdtest1_LDADD = $(BFD)
bfdtest1_DEPENDENCIES = $(BFD_DEP)

bfdtest2_LDADD = $(BFD)
bfdtest2_DEPENDENCIES = $(BFD_DEP)

OPCODES = ../opcodes/libopcodes.la
OPCODES_DEP = $(OPCODES)

LIBIBERTY = ../libiberty/libiberty.a
LIBIBERTY_DEP = $(LIBIBERTY)

INTLLIBS = ../intl/libintl.a @INTLLIBS@
lt_INTLLIBS = ../intl/libintl.la
INTLDEP = $(INTLLIBS)
INTLLIBS_DEP = $(INTLLIBS)

HISTORYLIBS = ../readline/libhistory.a
READLINELIBS = ../readline/libhistory.a ../readline/libreadline.a
READLINE_DEP = $(READLINELIBS)
READLINELIBS_DEP = $(READLINELIBS)

POTFILES = $(CFILES) $(DEBUG_SRCS) $(HFILES)
po/POTFILES.in: @MAINT@ Makefile
	for f in $(POTFILES); do echo "$${f}"; done | \
	  LC_COLLATE="" sort > tmp && \
	  mv $(AM_V_v) tmp $(srcdir)/po/POTFILES.in

my_EXPECT = expect
my_RUNTEST = runtest

CC_FOR_TARGET = ` \
  if [ -f $${r}/../gcc/xgcc ]; then \
    if [ -f $${r}/../newlib/Makefile ]; then \
      echo "$${r}/../gcc/xgcc -B$${r}/../gcc/ -idirafter $${r}/../newlib/targ-include -idirafter $${srcroot}/../newlib/libc/include -nostdinc"; \
    else \
      echo "$${r}/../gcc/xgcc -B$${r}/../gcc/"; \
    fi; \
  else \
    if [ "x@host@" = "x@target@" ]; then \
      echo "$(CC)"; \
    else \
      echo "gcc" | $(SED) '$(transform)'; \
    fi; \
  fi`

am_check-DEJAGNU: site.exp
	srcdir=`cd $(srcdir) && pwd`; export srcdir; \
	r=`pwd`; export r; \
	EXPECT=$(EXPECT); export EXPECT; \
	runtest=$(RUNTEST); \
	if $(SHELL) -c "$${runtest} --version" > /dev/null 2>&1; then \
	  CC_FOR_TARGET="$(CC_FOR_TARGET)" CFLAGS_FOR_TARGET="$(CFLAGS)" \
	    $${runtest} --tool $(DEJATOOL) --srcdir $${srcdir}/testsuite \
	    $(RUNTESTFLAGS); \
	else echo "WARNING: could not find \`runtest'" 1>&2; :;\
	fi
.PHONY: am_check-DEJAGNU

installcheck-local: $(srcdir)/sanity.sh
	/bin/sh $(srcdir)/sanity.sh $(bindir)

global_LDADD_for_everything = $(BFDLIB) $(LIBIBERTY) $(LIBINTL)

size_SOURCES = size.c $(BULIBS)
size_LDADD = $(BFD) $(LIBIBERTY) $(INTLLIBS)
size_DEPENDENCIES = $(BFD_DEP) $(LIBIBERTY_DEP) $(INTLDEP)

objcopy_and_strip_SHARED_SRC_CFILES = objcopy.c rename.c \
	$(WRITE_DEBUG_SRCS) $(BULIBS)
objcopy_SOURCES = $(objcopy_and_strip_SHARED_SRC_CFILES) not-strip.c
objcopy_LDADD = $(BFD) $(LIBIBERTY) $(INTLLIBS)
objcopy_DEPENDENCIES = $(BFD_DEP) $(LIBIBERTY_DEP) $(INTLDEP)

strip_new_SOURCES = $(objcopy_and_strip_SHARED_SRC_CFILES) is-strip.c
strip_new_LDADD = $(BFD) $(LIBIBERTY) $(INTLLIBS)
strip_new_DEPENDENCIES = $(BFD_DEP) $(LIBIBERTY_DEP) $(INTLDEP)

strings_SOURCES = strings.c $(BULIBS)
strings_LDADD = $(BFD) $(LIBIBERTY) $(INTLLIBS)
strings_DEPENDENCIES = $(BFD_DEP) $(LIBIBERTY_DEP) $(INTLDEP)

readelf_SOURCES = readelf.c version.c unwind-ia64.c
readelf_LDADD = $(LIBIBERTY) $(INTLLIBS)
readelf_DEPENDENCIES = $(LIBIBERTY_DEP) $(INTLDEP)

MY_elfedit_bad_SOURCES_COMMENTED_OUT = elfedit.c version.c $(ELFLIBS)
MY_elfedit_bad_LDADD_COMMENTED_OUT = $(LIBINTL) $(LIBIBERTY)
MY_elfedit_bad_DEPENDENCIES_COMMENTED_OUT = $(LIBINTL_DEP) $(LIBIBERTY)

elfedit_SOURCES = elfedit.c version.c $(ELFLIBS)
elfedit_LDADD = $(LIBINTL) $(LIBIBERTY)
elfedit_DEPENDENCIES = $(LIBINTL_DEP) $(LIBIBERTY)

nm_new_SOURCES = nm.c budemang.c $(BULIBS)
nm_new_LDADD = $(BFD) $(LIBIBERTY) $(INTLLIBS)
nm_new_DEPENDENCIES = $(BFD_DEP) $(LIBIBERTY_DEP) $(INTLDEP)

objdump_SOURCES = objdump.c budemang.c prdbg.c $(DEBUG_SRCS) $(BULIBS)
objdump_LDADD = $(OPCODES) $(BFD) $(LIBIBERTY) $(INTLLIBS)
objdump_DEPENDENCIES = $(OPCODES_DEP) $(BFD_DEP) $(LIBIBERTY_DEP) \
  $(INTLDEP)

objdump.@OBJEXT@:objdump.c
if am__fastdepCC
	$(AM_V_CC)$(COMPILE) -MT $@ -MD -MP -MF $(DEPDIR)/$*.Tpo -c -o $@ $(OBJDUMP_DEFS) $(srcdir)/objdump.c
	$(AM_V_at)mv -f $(AM_V_v) $(DEPDIR)/$*.Tpo $(DEPDIR)/$*.Po
else
if AMDEP
	$(AM_V_CC)source='objdump.c' object='$@' libtool=no @AMDEPBACKSLASH@
	DEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) @AMDEPBACKSLASH@
endif
	$(AM_V_CC@am__nodep@)$(COMPILE) -c $(OBJDUMP_DEFS) $(srcdir)/objdump.c
endif

cxxfilt_SOURCES = cxxfilt.c $(BULIBS)
cxxfilt_LDADD = $(BFD) $(LIBIBERTY) $(INTLLIBS)
cxxfilt_DEPENDENCIES = $(BFD_DEP) $(LIBIBERTY_DEP) $(INTLDEP)

ar_and_ranlib_SHARED_SRC_CFILES = arparse.y arlex.l ar.c arsup.c rename.c \
	binemul.c emul_@EMULATION@.c $(BULIBS)
ar_SOURCES = $(ar_and_ranlib_SHARED_SRC_CFILES) not-ranlib.c 
EXTRA_ar_SOURCES = $(CFILES)
ar_LDADD = $(BFD) $(LIBIBERTY) @LEXLIB@ $(INTLLIBS)
ar_DEPENDENCIES = $(BFD_DEP) $(LIBIBERTY_DEP) @LEXLIB@ $(INTLLIBS_DEP)

ranlib_SOURCES = $(ar_and_ranlib_SHARED_SRC_CFILES) is-ranlib.c
ranlib_LDADD = $(BFD) $(LIBIBERTY) @LEXLIB@ $(INTLLIBS)
ranlib_DEPENDENCIES = $(BFD_DEP) $(LIBIBERTY_DEP) @LEXLIB@ $(INTLDEP)

addr2line_SOURCES = addr2line.c budemang.c $(BULIBS)
addr2line_LDADD = $(BFD) $(LIBIBERTY) $(INTLLIBS)
addr2line_DEPENDENCIES = $(BFD_DEP) $(LIBIBERTY_DEP) $(INTLDEP)

ADDL_LIBS = $(BFD) $(LIBIBERTY)
ADDL_DEPS = $(BFD_DEP) $(LIBIBERTY_DEP) @LEXLIB@ $(INTLDEP)
EXTRALIBS = @LEXLIB@ $(INTLLIBS)
HLDENV = $(LIBTOOL) --tag=CC $(AM_LIBTOOLFLAGS) $(LIBTOOLFLAGS) --mode=link

ar_and_ranlib_SHARED_OFILES = arparse.o arlex.o ar.o arsup.o rename.o \
	binemul.o emul_@EMULATION@.o $(BULIB_OFILES) maybe-ranlib.o

# The following is commented out for the conversion to automake.
# This rule creates a single binary that switches between ar and ranlib
# by looking at argv[0]. Use this kludge to save some disk space.
# However, you have to install things by hand.
# (That is, after 'make install', replace the installed ranlib by a link to
# ar.) Alternatively, you can install ranlib.sh as ranlib.
ar_with_ranlib_uncommented: $(ADDL_DEPS) $(ar_and_ranlib_SHARED_OFILES)
	$(HLDENV) $(CC) $(HLDFLAGS) $(CFLAGS) $(C_ARCHFLAGS) $(LDFLAGS) \
	  $(AM_LDFLAGS) -o $(AR_PROG) $(ar_and_ranlib_SHARED_OFILES) \
	  $(ADDL_LIBS) $(EXTRALIBS)
	-rm -f $(AM_V_v) $(RANLIB_PROG)
	-ln $(AM_V_v) $(AR_PROG) $(RANLIB_PROG)
.PHONY: ar_with_ranlib_uncommented

objcopy_strip_SHARED_OFILES = objcopy.o rename.o maybe-strip.o \
	$(WRITE_DEBUG_SRC_OFILES) $(BULIB_OFILES)

# Similarly to the previous, we also have a target that builds an
# objcopy and a strip in one binary that uses argv[0] to decide its action:
objcopy_with_strip_uncommented: $(ADDL_DEPS) $(objcopy_strip_SHARED_OFILES)
	$(HLDENV) $(CC) $(HLDFLAGS) $(CFLAGS) $(C_ARCHFLAGS) $(LDFLAGS) \
	  $(AM_LDFLAGS) -o $(OBJCOPY_PROG) $(objcopy_strip_SHARED_OFILES) \
	  $(ADDL_LIBS) $(EXTRALIBS)
	-rm -f $(AM_V_v) $(STRIP_PROG)
	-ln $(AM_V_v) $(OBJCOPY_PROG) $(STRIP_PROG)
.PHONY: objcopy_with_strip_uncommented

sysroff.c: sysinfo$(EXEEXT_FOR_BUILD) sysroff.info
	echo "/* $@ is generated by sysinfo$(EXEEXT_FOR_BUILD) from sysroff.info */" > $@
	./sysinfo$(EXEEXT_FOR_BUILD) -c <$(srcdir)/sysroff.info >>$@
	echo "/* end section generated by '-c' flag to sysinfo$(EXEEXT_FOR_BUILD) */" >>$@
	./sysinfo$(EXEEXT_FOR_BUILD) -i <$(srcdir)/sysroff.info >>$@
	echo "/* end section generated by '-i' flag to sysinfo$(EXEEXT_FOR_BUILD) */" >>$@
	./sysinfo$(EXEEXT_FOR_BUILD) -g <$(srcdir)/sysroff.info >>$@
	echo "/* end section generated by '-g' flag to sysinfo$(EXEEXT_FOR_BUILD) */" >>$@

sysroff.h: sysinfo$(EXEEXT_FOR_BUILD) sysroff.info
	echo "/* $@ is generated by sysinfo$(EXEEXT_FOR_BUILD) from sysroff.info */" > $@
	./sysinfo$(EXEEXT_FOR_BUILD) -d <$(srcdir)/sysroff.info >>$@
	echo "/* end section generated by '-d' flag to sysinfo$(EXEEXT_FOR_BUILD) */" >>$@

sysinfo$(EXEEXT_FOR_BUILD): sysinfo.@OBJEXT@ syslex_wrap.@OBJEXT@ sysinfo.h
	$(AM_V_CCLD)$(CC_FOR_BUILD) $(CFLAGS_FOR_BUILD) $(C_ARCHFLAGS) $(LDFLAGS_FOR_BUILD) -o $@ sysinfo.@OBJEXT@ syslex_wrap.@OBJEXT@

syslex.@OBJEXT@: config.h syslex.c sysinfo.h
	$(AM_V_CC)if [ -r syslex.c ]; then \
	  $(CC_FOR_BUILD) -c -I. $(AM_CFLAGS) $(CFLAGS) syslex.c -Wno-error; \
	else \
	  $(CC_FOR_BUILD) -c -I. -I$(srcdir) $(AM_CFLAGS) $(CFLAGS) $(srcdir)/syslex.c -Wno-error; \
	fi

syslex_wrap.@OBJEXT@: syslex_wrap.c syslex.c sysinfo.h config.h
	$(AM_V_CC)$(CC_FOR_BUILD) -c -I. -I$(srcdir) $(AM_CFLAGS) \
	  $(CFLAGS_FOR_BUILD) $(NO_WERROR) $(srcdir)/syslex_wrap.c \
	  -Wno-error

sysinfo.@OBJEXT@: sysinfo.c @MAINT@ sysinfo.h
	$(AM_V_CC)if [ -r sysinfo.c ]; then \
	  $(CC_FOR_BUILD) -c -I. $(AM_CFLAGS) $(CFLAGS_FOR_BUILD) $(NO_WERROR) sysinfo.c; \
	else \
	  $(CC_FOR_BUILD) -c -I. $(AM_CFLAGS) $(CFLAGS_FOR_BUILD) $(NO_WERROR) $(srcdir)/sysinfo.c; \
	fi

bin2c$(EXEEXT_FOR_BUILD): bin2c.c $(INCDIR)/binary-io.h
	$(AM_V_CC)$(CC_FOR_BUILD) -o $@ $(AM_CPPFLAGS) $(AM_CFLAGS) \
	  $(CFLAGS_FOR_BUILD) $(LDFLAGS_FOR_BUILD) $(srcdir)/bin2c.c

embedspu: embedspu.sh Makefile
	$(AWK) '/^program_transform_name=/ {print "program_transform_name=\"$(program_transform_name)\""; next} {print}' < $< > $@
	chmod a+x $@

# We need these for parallel make.
##TODO: fix warnings about these overriding automake targets
my_arparse.h: arparse.c
.PHONY: my_arparse.h
my_defparse.h: defparse.c
.PHONY: my_defparse.h
my_nlmheader.h: nlmheader.c
.PHONY: my_nlmheader.h
my_rcparse.h: rcparse.c
.PHONY: my_rcparse.h
sysinfo.h: sysinfo.c sysinfo.y
sysinfo.c: sysinfo.y

# Disable -Werror, if it has been enabled, since old versions of bison/
# yacc will produce working code which contain compile time warnings.
arparse.@OBJEXT@: arparse.c ../bfd/bfd.h bucomm.h arsup.h
if am__fastdepCC
	$(AM_V_CC)$(COMPILE) -MT $@ -MD -MP -MF $(DEPDIR)/$*.Tpo -c -o $@ `test -f arparse.c || echo $(srcdir)/`arparse.c $(NO_WERROR)
	$(AM_V_at)mv -f $(AM_V_v) $(DEPDIR)/$*.Tpo $(DEPDIR)/$*.Po
else
if AMDEP
	$(AM_V_CC)source='arparse.c' object='$@' libtool=no @AMDEPBACKSLASH@
	DEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) @AMDEPBACKSLASH@
endif
	$(AM_V_CC@am__nodep@)$(COMPILE) -c `test -f arparse.c || echo $(srcdir)/`arparse.c $(NO_WERROR)
endif

arlex.@OBJEXT@: arlex.c $(INCDIR)/ansidecl.h arparse.h
if am__fastdepCC
	$(AM_V_CC)$(COMPILE) -MT $@ -MD -MP -MF $(DEPDIR)/$*.Tpo -c -o $@ `test -f arlex.c || echo $(srcdir)/`arlex.c $(NO_WERROR)
	$(AM_V_at)mv -f $(AM_V_v) $(DEPDIR)/$*.Tpo $(DEPDIR)/$*.Po
else
if AMDEP
	$(AM_V_CC)source='arlex.c' object='$@' libtool=no @AMDEPBACKSLASH@
	DEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) @AMDEPBACKSLASH@
endif
	$(AM_V_CC@am__nodep@)$(COMPILE) -c `test -f arlex.c || echo $(srcdir)/`arlex.c $(NO_WERROR)
endif

sysroff.@OBJEXT@: sysroff.c
if am__fastdepCC
	$(AM_V_CC)$(COMPILE) -MT $@ -MD -MP -MF $(DEPDIR)/$*.Tpo -c -o $@ `test -f sysroff.c || echo $(srcdir)/`sysroff.c $(NO_WERROR)
	$(AM_V_at)mv -f $(AM_V_v) $(DEPDIR)/$*.Tpo $(DEPDIR)/$*.Po
else
if AMDEP
	$(AM_V_CC)source='sysroff.c' object='$@' libtool=no @AMDEPBACKSLASH@
	DEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) @AMDEPBACKSLASH@
endif
	$(AM_V_CC@am__nodep@)$(COMPILE) -c `test -f sysroff.c || echo $(srcdir)/`sysroff.c $(NO_WERROR)
endif

defparse.@OBJEXT@: defparse.c ../bfd/bfd.h bucomm.h dlltool.h
if am__fastdepCC
	$(AM_V_CC)$(COMPILE) -MT $@ -MD -MP -MF $(DEPDIR)/$*.Tpo -c -o $@ `test -f defparse.c || echo $(srcdir)/`defparse.c $(NO_WERROR)
	$(AM_V_at)mv -f $(AM_V_v) $(DEPDIR)/$*.Tpo $(DEPDIR)/$*.Po
else
if AMDEP
	$(AM_V_CC)source='defparse.c' object='$@' libtool=no @AMDEPBACKSLASH@
	DEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) @AMDEPBACKSLASH@
endif
	$(AM_V_CC@am__nodep@)$(COMPILE) -c `test -f defparse.c || echo $(srcdir)/`defparse.c $(NO_WERROR)
endif

deflex.@OBJEXT@: deflex.c $(INCDIR)/libiberty.h defparse.h dlltool.h
if am__fastdepCC
	$(AM_V_CC)$(COMPILE) -MT $@ -MD -MP -MF $(DEPDIR)/$*.Tpo -c -o $@ `test -f deflex.c || echo $(srcdir)/`deflex.c $(NO_WERROR)
	$(AM_V_at)mv -f $(AM_V_v) $(DEPDIR)/$*.Tpo $(DEPDIR)/$*.Po
else
if AMDEP
	$(AM_V_CC)source='deflex.c' object='$@' libtool=no @AMDEPBACKSLASH@
	DEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) @AMDEPBACKSLASH@
endif
	$(AM_V_CC@am__nodep@)$(COMPILE) -c `test -f deflex.c || echo $(srcdir)/`deflex.c $(NO_WERROR)
endif

nlmheader.@OBJEXT@: nlmheader.c nlmconv.h bucomm.h $(INCDIR)/ansidecl.h
if am__fastdepCC
	$(AM_V_CC)$(COMPILE) -MT $@ -MD -MP -MF $(DEPDIR)/$*.Tpo -c -o $@ `test -f nlmheader.c || echo $(srcdir)/`nlmheader.c $(NO_WERROR)
	$(AM_V_at)mv -f $(AM_V_v) $(DEPDIR)/$*.Tpo $(DEPDIR)/$*.Po
else
if AMDEP
	$(AM_V_CC)source='nlmheader.c' object='$@' libtool=no @AMDEPBACKSLASH@
	DEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) @AMDEPBACKSLASH@
endif
	$(AM_V_CC@am__nodep@)$(COMPILE) -c `test -f nlmheader.c || echo $(srcdir)/`nlmheader.c $(NO_WERROR)
endif

rcparse.@OBJEXT@: rcparse.c bucomm.h windres.h $(INCDIR)/libiberty.h
if am__fastdepCC
	$(AM_V_CC)$(COMPILE) -MT $@ -MD -MP -MF $(DEPDIR)/$*.Tpo -c -o $@ `test -f rcparse.c || echo $(srcdir)/`rcparse.c $(NO_WERROR)
	$(AM_V_at)mv -f $(AM_V_v) $(DEPDIR)/$*.Tpo $(DEPDIR)/$*.Po
else
if AMDEP
	$(AM_V_CC)source='rcparse.c' object='$@' libtool=no @AMDEPBACKSLASH@
	DEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) @AMDEPBACKSLASH@
endif
	$(AM_V_CC@am__nodep@)$(COMPILE) -c `test -f rcparse.c || echo $(srcdir)/`rcparse.c $(NO_WERROR)
endif

mcparse.@OBJEXT@: mcparse.c sysdep.h bucomm.h windmc.h $(INCDIR)/ansidecl.h
if am__fastdepCC
	$(AM_V_CC)$(COMPILE) -MT $@ -MD -MP -MF $(DEPDIR)/$*.Tpo -c -o $@ `test -f mcparse.c || echo $(srcdir)/`mcparse.c $(NO_WERROR)
	$(AM_V_at)mv -f $(AM_V_v) $(DEPDIR)/$*.Tpo $(DEPDIR)/$*.Po
else
if AMDEP
	$(AM_V_CC)source='mcparse.c' object='$@' libtool=no @AMDEPBACKSLASH@
	DEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) @AMDEPBACKSLASH@
endif
	$(AM_V_CC@am__nodep@)$(COMPILE) -c `test -f mcparse.c || echo $(srcdir)/`mcparse.c $(NO_WERROR)
endif

rclex.@OBJEXT@: rclex.c bucomm.h windres.h rcparse.h ../bfd/bfd.h
if am__fastdepCC
	$(AM_V_CC)$(COMPILE) -MT $@ -MD -MP -MF $(DEPDIR)/$*.Tpo -c -o $@ $(srcdir)/rclex.c $(NO_WERROR)
	$(AM_V_at)mv -f $(AM_V_v) $(DEPDIR)/$*.Tpo $(DEPDIR)/$*.Po
else
if AMDEP
	$(AM_V_CC)source='rclex.c' object='$@' libtool=no @AMDEPBACKSLASH@
	DEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) @AMDEPBACKSLASH@
endif
	$(AM_V_CC@am__nodep@)$(COMPILE) -c $(srcdir)/rclex.c $(NO_WERROR)
endif

mclex.@OBJEXT@: mclex.c sysdep.h bucomm.h windmc.h mcparse.h ../bfd/bfd.h
if am__fastdepCC
	$(AM_V_CC)$(COMPILE) -MT $@ -MD -MP -MF $(DEPDIR)/$*.Tpo -c -o $@ $(srcdir)/mclex.c $(NO_WERROR)
	$(AM_V_at)mv -f $(AM_V_v) $(DEPDIR)/$*.Tpo $(DEPDIR)/$*.Po
else
if AMDEP
	$(AM_V_CC)source='mclex.c' object='$@' libtool=no @AMDEPBACKSLASH@
	DEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) @AMDEPBACKSLASH@
endif
	$(AM_V_CC@am__nodep@)$(COMPILE) -c $(srcdir)/mclex.c $(NO_WERROR)
endif

srconv_SOURCES = srconv.c coffgrok.c $(BULIBS)
srconv.@OBJEXT@: sysroff.c bucomm.h sysroff.h coffgrok.h ../bfd/bfd.h
srconv_LDADD = $(BFD) $(LIBIBERTY) $(INTLLIBS)
srconv_DEPENDENCIES = $(BFD_DEP) $(LIBIBERTY_DEP) $(INTLDEP)

## dyn-string.c has been removed from here because it actually lives in
## libiberty:
dlltool_SOURCES = dlltool.c defparse.y deflex.l $(BULIBS)
dlltool_LDADD = $(BFD) $(LIBIBERTY) @LEXLIB@ $(INTLLIBS)
dlltool_DEPENDENCIES = $(BFD_DEP) $(LIBIBERTY_DEP) @LEXLIB@ $(INTLDEP)

dlltool.@OBJEXT@: dlltool.c dlltool.h bucomm.h ../bfd/bfd.h
if am__fastdepCC
	$(AM_V_CC)$(COMPILE) -MT $@ -MD -MP -MF $(DEPDIR)/$*.Tpo -c -o $@  $(DLLTOOL_DEFS) $(srcdir)/dlltool.c
	$(AM_V_at)mv -f $(AM_V_v) $(DEPDIR)/$*.Tpo $(DEPDIR)/$*.Po
else
if AMDEP
	$(AM_V_CC)source='dlltool.c' object='$@' libtool=no @AMDEPBACKSLASH@
	DEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) @AMDEPBACKSLASH@
endif
	$(AM_V_CC@am__nodep@)$(COMPILE) -c $(DLLTOOL_DEFS) $(srcdir)/dlltool.c
endif

rescoff.@OBJEXT@: rescoff.c bucomm.h windres.h ../bfd/bfd.h
if am__fastdepCC
	$(AM_V_CC)$(COMPILE) -MT $@ -MD -MP -MF $(DEPDIR)/$*.Tpo -c -o $@  $(DLLTOOL_DEFS) $(srcdir)/rescoff.c
	$(AM_V_at)mv -f $(AM_V_v) $(DEPDIR)/$*.Tpo $(DEPDIR)/$*.Po
else
if AMDEP
	$(AM_V_CC)source='rescoff.c' object='$@' libtool=no @AMDEPBACKSLASH@
	DEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) @AMDEPBACKSLASH@
endif
	$(AM_V_CC@am__nodep@)$(COMPILE) -c $(DLLTOOL_DEFS) $(srcdir)/rescoff.c
endif

coffdump_SOURCES = coffdump.c coffgrok.c $(BULIBS)
coffdump_LDADD = $(BFD) $(LIBIBERTY) $(INTLLIBS)
coffdump_DEPENDENCIES = $(BFD_DEP) $(LIBIBERTY_DEP) $(INTLDEP)

sysdump_SOURCES = sysdump.c $(BULIBS)
sysdump.@OBJEXT@: sysroff.c bucomm.h sysroff.h ../bfd/bfd.h
sysdump_LDADD = $(BFD) $(LIBIBERTY) $(INTLLIBS)
sysdump_DEPENDENCIES = $(BFD_DEP) $(LIBIBERTY_DEP) $(INTLDEP)

# coff/sym.h and coff/ecoff.h will NOT be found by the automatic dependency
# scripts, since they are only included conditionally.
nlmconv.@OBJEXT@: nlmconv.c $(INCDIR)/coff/sym.h $(INCDIR)/coff/ecoff.h
if am__fastdepCC
	$(AM_V_CC)ldname=`echo ld | $(SED) '$(transform)'`; \
	$(COMPILE) -MT $@ -MD -MP -MF $(DEPDIR)/$*.Tpo -c -o $@ \
	  -DLD_NAME="\"$${ldname}\"" @NLMCONV_DEFS@ $(srcdir)/nlmconv.c
	$(AM_V_at)mv -f $(AM_V_v) $(DEPDIR)/$*.Tpo $(DEPDIR)/$*.Po
else
if AMDEP
	$(AM_V_CC)source='nlmconv.c' object='$@' libtool=no @AMDEPBACKSLASH@
	DEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) @AMDEPBACKSLASH@
endif
	$(AM_V_CC@am__nodep@)ldname=`echo ld | $(SED) '$(transform)'`; \
	$(COMPILE) -c -DLD_NAME="\"$${ldname}\"" @NLMCONV_DEFS@ $(srcdir)/nlmconv.c
endif

nlmconv_SOURCES = nlmconv.c nlmheader.y $(BULIBS)
nlmconv_LDADD = $(BFD) $(LIBIBERTY) $(INTLLIBS)
nlmconv_DEPENDENCIES = $(BFD_DEP) $(LIBIBERTY_DEP) $(INTLDEP)

windres_SOURCES = windres.c resrc.c rescoff.c resbin.c rcparse.y rclex.l \
	winduni.c resres.c $(BULIBS)
windres_LDADD = $(BFD) $(LIBIBERTY) @LEXLIB@ $(INTLLIBS)
windres_DEPENDENCIES = $(BFD_DEP) $(LIBIBERTY_DEP) @LEXLIB@ $(INTLDEP)

windmc_SOURCES = windmc.c mcparse.y mclex.c \
	winduni.c $(BULIBS)
windmc_LDADD = $(BFDLIB) $(LIBIBERTY) $(LEXLIB) $(LIBINTL) $(LIBICONV)
windmc_DEPENDENCIES = $(LIBINTL_DEP) $(LIBIBERTY) $(BFDLIB)

dllwrap_SOURCES = dllwrap.c version.c
dllwrap_LDADD = $(LIBIBERTY) $(INTLLIBS)
dllwrap_DEPENDENCIES = $(LIBIBERTY_DEP) $(INTLDEP)

EXTRA_DIST = arparse.c arparse.h arlex.c nlmheader.c sysinfo.c sysinfo.h \
	syslex.c deflex.c defparse.h defparse.c rclex.c \
	rcparse.h rcparse.c mcparse.h mcparse.c embedspu.sh

diststuff: $(EXTRA_DIST) info
all: info

# We extract version from bfd/configure.ac, make sure to rerun configure
# when BFD's version changes.
CONFIG_STATUS_DEPENDENCIES = $(BFDDIR)/configure.ac \
	$(BFDDIR)/development.sh

ALL_BISON_OUTPUT_FILES = \
	arparse.output \
	defparse.output \
	mcparse.output \
	nlmheader.output \
	rcparse.output \
	sysinfo.output

DISTCLEANFILES = sysinfo sysroff.c sysroff.h lex.backup \
	site.exp site.bak embedspu *~ .DS_Store *.log *.trs \
	$(ALL_BISON_OUTPUT_FILES)

distclean-local:
	rm -rf autom4te.cache || rmdir autom4te.cache
.PHONY: distclean-local

Makefile_depends: $(BFDDIR)/configure.ac config.status
.PHONY: Makefile_depends

# Targets to rebuild dependencies in this Makefile.
# Have to get rid of any pre-existing DEP1 here, so that "$?" later
# includes all of $(CFILES).
DEP: dep.sed $(CFILES) $(HFILES) $(GENERATED_CFILES) $(GENERATED_HFILES) config.h
	rm -f $(AM_V_v) DEP1
	$(MAKE) MKDEP="$(MKDEP)" DEP1
	$(SED) -f dep.sed < DEP1 | uniq $(AM_V_redir) DEPA
	echo '# IF YOU PUT ANYTHING HERE IT WILL GO AWAY' >> DEPA
	if $(GREP) ' /' DEPA > /dev/null 2> /dev/null; then \
	  echo 'make DEP failed; it produced bad directories!'>&2; \
	  exit 1; \
	else \
	  mv -f $(AM_V_v) DEPA $@; \
	fi
	touch $@
# I also want to include a check for variables not being closed properly
# (missing parentheses), but I am not sure how to do that in a way where
# the bad syntax for which we would be grepping does not also get
# evaluated and treated as an error...

# Not sure if AM_CPPFLAGS works perfectly as a 1-for-1 replacement
# for INCLUDES here in this next one:
DEP1: $(CFILES) $(GENERATED_CFILES)
	echo '# DO NOT DELETE THIS LINE -- mkdep uses it.' > DEP2
	echo '# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.' >>DEP2
	$(MKDEP) $(AM_CPPFLAGS) $(CFLAGS) $? | uniq $(AM_V_redir_a) DEP2
	mv -f $(AM_V_v) DEP2 $@
	touch $@

#FIXME: this sed script is broken and hack-ish:
dep.sed: dep-in.sed @MAINT@ config.status
	objdir=`pwd`; \
	$(SED) <$(srcdir)/dep-in.sed >dep.sed	\
	  -e 's!@INCDIR@!$(INCDIR)!'	\
	  -e 's!@BFDDIR@!$(BFDDIR)!'	\
	  -e 's!@SRCDIR@!$(srcdir)!'	\
	  -e "s!@OBJDIR@!$${objdir}!"	\
	  -e 's!@TOPDIR@!'`echo $(srcdir) | $(SED) -e s,/binutils$$,,`'!'
	touch $@

#FIXME: this target relies on dep.sed, which is broken (see above)
dep: DEP @MAINT@ $(PATH_TO_MOVEIFCHANGE)
	$(SED) -e '/^..DO NOT DELETE THIS LINE/,$$d' < Makefile > tmp-Makefile
	cat DEP >> tmp-Makefile
	$(RUN_MOVEIFCHANGE) tmp-Makefile Makefile
	touch Makefile

#FIXME: this target relies on dep.sed, which is broken (see above)
dep-in: DEP @MAINT@ $(PATH_TO_MOVEIFCHANGE)
	$(SED) -e '/^..DO NOT DELETE THIS LINE/,$$d' < $(srcdir)/Makefile.in > tmp-Makefile.in
	cat DEP >> tmp-Makefile.in
	$(RUN_MOVEIFCHANGE) tmp-Makefile.in $(srcdir)/Makefile.in
	touch $(srcdir)/Makefile.in

#FIXME: this target relies on dep.sed, which is broken (see above)
dep-am: DEP @MAINT@ $(PATH_TO_MOVEIFCHANGE)
	@echo "this target is broken/dangerous; not letting you use it" >&2
	exit 1
	$(SED) -e '/^..DO NOT DELETE THIS LINE/,$$d' < $(srcdir)/Makefile.am > tmp-Makefile.am
	cat DEP >> tmp-Makefile.am
	$(RUN_MOVEIFCHANGE) tmp-Makefile.am $(srcdir)/Makefile.am
	touch $(srcdir)/Makefile.am

.PHONY: dep dep-in dep-am

## APPLE LOCAL begin framework building:
NSLIBTOOL = cc -dynamiclib

SYSTEM_FRAMEWORK = -L../intl -lintl -framework System -framework CoreFoundation @SYSTEM_FRAMEWORK_LIBS@

FRAMEWORK = binutils
FRAMEWORK_PREFIX =
FRAMEWORK_SUFFIX =
FRAMEWORK_VERSION_SUFFIX =
FRAMEWORK_VERSION = A
FRAMEWORK_OFILES = $(OFILES) $(GENERATED_OFILES)
FRAMEWORK_LIB = -F../bfd -framework bfd -F../opcodes -framework opcodes -F../libiberty -framework liberty -F../mmalloc -framework mmalloc $(SYSTEM_FRAMEWORK) $(LDFLAGS)

stamp-framework-headers: bucomm.h stamp-framework-headers-binutils

stamp-framework: stamp-framework-binutils $(FRAMEWORK_OFILES)

BINUTILS = binutils
BINUTILS_ADDRESS = 0x0
BINUTILS_PREFIX =
BINUTILS_SUFFIX =
BINUTILS_VERSION_SUFFIX =
BINUTILS_VERSION = A
BINUTILS_OFILES = version.o
BINUTILS_LIB = $(SYSTEM_FRAMEWORK)

stamp-framework-headers-binutils:
	$(RM) -f $(AM_V_v) stamp-framework-binutils stamp-framework-headers-binutils
	$(RM) -rf $(AM_V_v) $(BINUTILS).framework
	$(MKDIR_P) $(AM_V_v) $(BINUTILS).framework/Versions/$(BINUTILS_VERSION)/Headers
	$(MKDIR_P) $(AM_V_v) $(BINUTILS).framework/Versions/$(BINUTILS_VERSION)/PrivateHeaders
	$(LN_S) $(AM_V_v) Versions/Current/Headers $(BINUTILS).framework/Headers
	$(LN_S) $(AM_V_v) Versions/Current/PrivateHeaders $(BINUTILS).framework/PrivateHeaders
	$(LN_S) $(AM_V_v) A $(BINUTILS).framework/Versions/Current
	cp -rp $(AM_V_v) $(srcdir)/../include/* $(BINUTILS).framework/Versions/Current/Headers/
	cp -rp $(AM_V_v) $(srcdir)/../intl/*.h $(BINUTILS).framework/Versions/Current/Headers/
	touch stamp-framework-headers-binutils

stamp-framework-binutils: $(BINUTILS_OFILES)
	$(RM) -f $(AM_V_v) stamp-framework-headers-binutils
	$(MAKE) stamp-framework-headers-binutils
	$(MKDIR_P) $(AM_V_v) $(BINUTILS).framework/Versions/$(BINUTILS_VERSION)/Resources
	$(LN_S) $(AM_V_v) Versions/Current/Resources $(BINUTILS).framework/Resources
	$(MKDIR_P) $(AM_V_v) $(BINUTILS).framework/Versions/Current/Resources/English.lproj

	set -ex; \
	if [ "x$(BINUTILS_SUFFIX)" != "x" ]; then \
	    $(NSLIBTOOL) -arch $(HOST_ARCHITECTURE) -seg1addr $(BINUTILS_ADDRESS) -compatibility_version 1 -current_version 1 -install_name /System/Library/PrivateFrameworks/$(BINUTILS).framework/Versions/$(BINUTILS_VERSION)/$(BINUTILS_PREFIX)$(BINUTILS)$(BINUTILS_VERSION_SUFFIX) -o $(BINUTILS).framework/Versions/Current/$(BINUTILS_PREFIX)$(BINUTILS)$(BINUTILS_VERSION_SUFFIX) $(BINUTILS_OFILES) $(BINUTILS_LIB); \
	    $(LN_S) $(AM_V_v) $(BINUTILS_PREFIX)$(BINUTILS)$(BINUTILS_VERSION_SUFFIX) $(BINUTILS).framework/Versions/Current/$(BINUTILS_PREFIX)$(BINUTILS)$(BINUTILS_SUFFIX); \
	    $(LN_S) $(AM_V_v) Versions/Current/$(BINUTILS_PREFIX)$(BINUTILS)$(BINUTILS_VERSION_SUFFIX) $(BINUTILS).framework/$(BINUTILS_PREFIX)$(BINUTILS)$(BINUTILS_VERSION_SUFFIX); \
	    $(LN_S) $(AM_V_v) Versions/Current/$(BINUTILS_PREFIX)$(BINUTILS)$(BINUTILS_SUFFIX) $(BINUTILS).framework/$(BINUTILS_PREFIX)$(BINUTILS)$(BINUTILS_SUFFIX); \
	    $(LN_S) $(AM_V_v) $(BINUTILS_PREFIX)$(BINUTILS)$(BINUTILS_SUFFIX) $(BINUTILS).framework/$(BINUTILS); \
	else \
	    $(NSLIBTOOL) -arch $(HOST_ARCHITECTURE) -seg1addr $(BINUTILS_ADDRESS) -compatibility_version 1 -current_version 1 -install_name /System/Library/PrivateFrameworks/$(BINUTILS).framework/Versions/$(BINUTILS_VERSION)/$(BINUTILS_PREFIX)$(BINUTILS) -o $(BINUTILS).framework/Versions/Current/$(BINUTILS_PREFIX)$(BINUTILS) $(BINUTILS_OFILES) $(BINUTILS_LIB); \
	    $(LN_S) $(AM_V_v) Versions/Current/$(BINUTILS_PREFIX)$(BINUTILS) $(BINUTILS).framework/$(BINUTILS_PREFIX)$(BINUTILS); \
	fi
	touch stamp-framework-binutils
## APPLE LOCAL end framework building

## APPLE LOCAL old Classic/Carbon stuff:
REZ = @REZ@

# can add "-progress" for more verbose output:
mac-binutils.rsrc: mac-binutils.r
	$(REZ) -o $@ $(C_ARCHFLAGS) -i ../gdb \
	  -d VERSION_STRING="\"$(PACKAGE) $(VERSION)\"" \
	  mac-binutils.r 2>&1

## APPLE LOCAL end old Classic/Carbon stuff

###

MOSTLYCLEANFILES = sysinfo$(EXEEXT_FOR_BUILD) bin2c$(EXEEXT_FOR_BUILD) \
  binutils.log binutils.sum abcdefgh*
mostlyclean-local:
	-rm -rf tmpdir

CLEANFILES = dep.sed DEP DEPA DEP1 DEP2 $(EXTRA_PROGRAMS) \
  $(PREPROC_GENERATED_CFILES) $(ASM_OF_GENERATED_CFILES) @MAINT@ lib*.a
clean-local:
	-rm -f $(AM_V_v) *.rsrc
	-rm -rf $(AM_V_v) *.dSYM

## DISTCLEANFILES is already set above, but add these to it here:
DISTCLEANFILES += $(PREPROC_CFILES) stamp-framework-binutils \
  stamp-framework-headers-binutils $(ASM_OF_CFILES) @MAINT@ artest.a

maintainer-clean-local:
	-$(RM) -rf $(AM_V_v) $(BINUTILS).framework || rmdir $(BINUTILS).framework

.PHONY: install-exec-local

install-exec-local: install-binPROGRAMS $(bin_PROGRAMS) $(noinst_PROGRAMS)
	@list='$(noinst_PROGRAMS)'; for p in $${list}; do \
	  if test -f $${p}; then \
	    echo " $(INSTALL_PROGRAM_ENV) $(LIBTOOL) --mode=install $(INSTALL_PROGRAM) $${p} $(bindir)/`echo $${p} | $(SED) -e 's/$(EXEEXT)$$//' -e 's/-new//' -e 's/cxxfilt/$(DEMANGLER_NAME)/' | $(SED) '$(transform)' | $(SED) 's/$$/$(EXEEXT)/'`"; \
	    $(INSTALL_PROGRAM_ENV) $(LIBTOOL) --mode=install $(INSTALL_PROGRAM) $${p} $(DESTDIR)$(bindir)/`echo $${p} | $(SED) -e 's/$(EXEEXT)$$//' -e 's/-new//' -e 's/cxxfilt/$(DEMANGLER_NAME)/' | $(SED) '$(transform)' | $(SED) 's/$$/$(EXEEXT)/'`; \
	  else echo "$${p} does not exist and cannot be installed"; fi; \
	done
	$(mkinstalldirs) $(DESTDIR)$(tooldir)/bin
	for i in $(TOOL_PROGS); do \
	  if [ -f $${i}$(EXEEXT) ]; then \
	    j=`echo $${i} | $(SED) -e 's/-new//'`; \
	    k=`echo $${j} | $(SED) '$(transform)'`; \
	    if [ "$(bindir)/$${k}" != "$(tooldir)/bin/$${j}" ]; then \
	      rm -f $(DESTDIR)$(tooldir)/bin/$$j$(EXEEXT); \
	      ln $(DESTDIR)$(bindir)/$$k$(EXEEXT) $(DESTDIR)$(tooldir)/bin/$$j$(EXEEXT) >/dev/null 2>/dev/null \
		|| $(INSTALL_PROGRAM_ENV) $(LIBTOOL) --mode=install $(INSTALL_PROGRAM) $${i}$(EXEEXT) $(DESTDIR)$(tooldir)/bin/$${j}$(EXEEXT); \
	    fi; \
	  else echo "$${i}$(EXEEXT) does not exist, ignoring it"; \
	  fi; \
	done

# What appears below is generated by a hacked mkdep using gcc -MM.
# FIXME: the sed script that generates this currently screws up some stuff
# such as incorrectly using the $(BFDDIR) variable, and removing letters
# and directory separators when they do not need to be removed
# (for example, the 't' in 'aout', the 2nd 'f' in 'coff', the 'f' in 'elf')
# can edit manually until the sed script is fixed

# DO NOT DELETE THIS LINE -- mkdep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.
addr2line.o: addr2line.c config.h ../bfd/bfd.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/symcat.h $(INCDIR)/getopt.h $(INCDIR)/libiberty.h \
  $(INCDIR)/demangle.h bucomm.h $(INCDIR)/bin-bugs.h \
  $(INCDIR)/fopen-same.h budemang.h
ar.o: ar.c sysdep.h ../bfd/bfd.h $(INCDIR)/ansidecl.h $(INCDIR)/symcat.h \
  $(INCDIR)/libiberty.h $(INCDIR)/progress.h bucomm.h config.h \
  $(INCDIR)/bin-bugs.h $(INCDIR)/fopen-same.h $(INCDIR)/getopt.h \
  $(INCDIR)/aout/ar.h $(BFDDIR)/libbfd.h $(INCDIR)/hashtab.h \
  arsup.h $(INCDIR)/filenames.h binemul.h $(BFDDIR)/plugin.h
arsup.o: arsup.c ../bfd/bfd.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/symcat.h arsup.h $(INCDIR)/libiberty.h bucomm.h config.h \
  $(INCDIR)/bin-bugs.h $(INCDIR)/fopen-same.h \
  $(INCDIR)/filenames.h
bin2c.o: bin2c.c $(INCDIR)/binary-io.h
binemul.o: binemul.c binemul.h ../bfd/bfd.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/symcat.h bucomm.h config.h $(INCDIR)/bin-bugs.h \
  $(INCDIR)/fopen-same.h
bucomm.o: bucomm.c ../bfd/bfd.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/symcat.h ../bfd/bfdver.h $(INCDIR)/libiberty.h \
  bucomm.h config.h $(INCDIR)/bin-bugs.h $(INCDIR)/fopen-same.h \
  $(INCDIR)/filenames.h $(BFDDIR)/libbfd.h $(INCDIR)/hashtab.h
budemang.o: budemang.c config.h ../bfd/bfd.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/symcat.h $(INCDIR)/libiberty.h $(INCDIR)/demangle.h \
  budemang.h
coffdump.o: coffdump.c config.h ../bfd/bfd.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/symcat.h $(INCDIR)/libiberty.h coffgrok.h \
  bucomm.h config.h $(INCDIR)/bin-bugs.h $(INCDIR)/fopen-same.h \
  $(INCDIR)/getopt.h
coffgrok.o: coffgrok.c ../bfd/bfd.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/symcat.h $(INCDIR)/libiberty.h bucomm.h config.h \
  $(INCDIR)/bin-bugs.h $(INCDIR)/fopen-same.h $(INCDIR)/coff/internal.h \
  $(BFDDIR)/libcoff.h $(INCDIR)/bfdlink.h coffgrok.h
cxxfilt.o: cxxfilt.c config.h ../bfd/bfd.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/symcat.h bucomm.h $(INCDIR)/bin-bugs.h $(INCDIR)/fopen-same.h \
  $(INCDIR)/libiberty.h $(INCDIR)/demangle.h $(INCDIR)/getopt.h \
  $(INCDIR)/safe-ctype.h
dwarf.o: dwarf.c sysdep.h $(INCDIR)/alloca-conf.h config.h \
  $(INCDIR)/ansidecl.h ../bfd/bfdver.h $(INCDIR)/fopen-same.h \
  $(INCDIR)/binary-io.h $(INCDIR)/libiberty.h ../bfd/bfd.h \
  $(INCDIR)/symcat.h ../bfd/bfd_stdint.h bucomm.h $(INCDIR)/bin-bugs.h \
  elfcomm.h $(INCDIR)/aout/ar.h $(INCDIR)/elf/common.h $(INCDIR)/dwarf2.h \
  $(INCDIR)/dwarf2.def dwarf.h $(INCDIR)/gdb/gdb-index.h
debug.o: debug.c ../bfd/bfd.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/symcat.h bucomm.h config.h $(INCDIR)/bin-bugs.h \
  $(INCDIR)/fopen-same.h $(INCDIR)/libiberty.h debug.h
dlltool.o: dlltool.c ../bfd/bfd.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/symcat.h $(INCDIR)/libiberty.h bucomm.h config.h \
  $(INCDIR)/bin-bugs.h $(INCDIR)/fopen-same.h $(INCDIR)/getopt.h \
  $(INCDIR)/demangle.h $(INCDIR)/dyn-string.h dlltool.h \
  $(INCDIR)/safe-ctype.h
dllwrap.o: dllwrap.c ../bfd/bfd.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/symcat.h $(INCDIR)/libiberty.h bucomm.h config.h \
  $(INCDIR)/bin-bugs.h $(INCDIR)/fopen-same.h $(INCDIR)/getopt.h \
  $(INCDIR)/dyn-string.h
elfcomm.o: elfcomm.c sysdep.h $(INCDIR)/alloca-conf.h \
  config.h $(INCDIR)/ansidecl.h ../bfd/bfdver.h $(INCDIR)/fopen-same.h \
  $(INCDIR)/binary-io.h $(INCDIR)/libiberty.h $(INCDIR)/filenames.h \
  ../bfd/bfd.h $(INCDIR)/symcat.h $(INCDIR)/aout/ar.h bucomm.h config.h \
  $(INCDIR)/bin-bugs.h elfcomm.h
emul_aix.o: emul_aix.c binemul.h ../bfd/bfd.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/symcat.h bucomm.h config.h $(INCDIR)/bin-bugs.h \
  $(INCDIR)/fopen-same.h $(INCDIR)/bfdlink.h $(INCDIR)/coff/internal.h \
  $(INCDIR)/coff/xcoff.h $(BFDDIR)/libcoff.h $(BFDDIR)/libxcoff.h
emul_vanilla.o: emul_vanilla.c binemul.h ../bfd/bfd.h \
  $(INCDIR)/ansidecl.h $(INCDIR)/symcat.h bucomm.h config.h \
  $(INCDIR)/bin-bugs.h $(INCDIR)/fopen-same.h
filemode.o: filemode.c ../bfd/bfd.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/symcat.h bucomm.h config.h $(INCDIR)/bin-bugs.h \
  $(INCDIR)/fopen-same.h
ieee.o: ieee.c ../bfd/bfd.h $(INCDIR)/ansidecl.h $(INCDIR)/symcat.h \
  $(INCDIR)/ieee.h bucomm.h config.h $(INCDIR)/bin-bugs.h \
  $(INCDIR)/fopen-same.h $(INCDIR)/libiberty.h \
  debug.h budbg.h $(INCDIR)/filenames.h
is-ranlib.o: is-ranlib.c
is-strip.o: is-strip.c
maybe-ranlib.o: maybe-ranlib.c
maybe-strip.o: maybe-strip.c
nlmconv.o: nlmconv.c ../bfd/bfd.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/symcat.h $(INCDIR)/libiberty.h bucomm.h config.h \
  $(INCDIR)/bin-bugs.h $(INCDIR)/fopen-same.h $(INCDIR)/safe-ctype.h \
  $(INCDIR)/getopt.h $(BFDDIR)/libnlm.h $(INCDIR)/nlm/common.h \
  $(INCDIR)/nlm/internal.h $(INCDIR)/nlm/external.h nlmconv.h
nm.o: nm.c ../bfd/bfd.h $(INCDIR)/ansidecl.h $(INCDIR)/symcat.h \
  $(INCDIR)/progress.h bucomm.h config.h $(INCDIR)/bin-bugs.h \
  $(INCDIR)/fopen-same.h budemang.h $(INCDIR)/getopt.h \
  $(INCDIR)/aout/stab_gnu.h $(INCDIR)/aout/stab.def \
  $(INCDIR)/aout/ranlib.h $(INCDIR)/demangle.h $(INCDIR)/libiberty.h \
  $(BFDDIR)/elf-bfd.h $(INCDIR)/elf/common.h \
  $(INCDIR)/elf/internal.h $(INCDIR)/elf/external.h $(INCDIR)/bfdlink.h
not-ranlib.o: not-ranlib.c
not-strip.o: not-strip.c
objcopy.o: objcopy.c ../bfd/bfd.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/symcat.h $(INCDIR)/progress.h bucomm.h config.h \
  $(INCDIR)/bin-bugs.h $(INCDIR)/fopen-same.h $(INCDIR)/getopt.h \
  $(INCDIR)/libiberty.h budbg.h $(INCDIR)/filenames.h $(INCDIR)/fnmatch.h \
  $(BFDDIR)/elf-bfd.h $(INCDIR)/elf/common.h \
  $(INCDIR)/elf/internal.h $(INCDIR)/elf/external.h $(INCDIR)/bfdlink.h
objdump.o: objdump.c ../bfd/bfd.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/symcat.h ../bfd/bfdver.h $(INCDIR)/progress.h \
  bucomm.h config.h $(INCDIR)/bin-bugs.h $(INCDIR)/fopen-same.h \
  budemang.h $(INCDIR)/getopt.h $(INCDIR)/safe-ctype.h \
  $(INCDIR)/dis-asm.h $(INCDIR)/libiberty.h $(INCDIR)/demangle.h \
  debug.h budbg.h $(INCDIR)/aout/aout64.h
prdbg.o: prdbg.c ../bfd/bfd.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/symcat.h bucomm.h config.h $(INCDIR)/bin-bugs.h \
  $(INCDIR)/fopen-same.h $(INCDIR)/libiberty.h \
  debug.h budbg.h
od-xcoff.o: od-xcoff.c sysdep.h $(INCDIR)/alloca-conf.h \
  config.h $(INCDIR)/ansidecl.h ../bfd/bfdver.h $(INCDIR)/fopen-same.h \
  $(INCDIR)/binary-io.h $(INCDIR)/safe-ctype.h \
  ../bfd/bfd.h $(INCDIR)/symcat.h objdump.h bucomm.h \
  config.h $(INCDIR)/bin-bugs.h $(INCDIR)/bfdlink.h \
  $(INCDIR)/coff/internal.h $(INCDIR)/coff/rs6000.h \
  $(INCDIR)/coff/xcoff.h $(BFDDIR)/libcoff.h $(BFDDIR)/libxcoff.h
od-macho.o: od-macho.c sysdep.h $(INCDIR)/alloca-conf.h \
  config.h $(INCDIR)/ansidecl.h ../bfd/bfdver.h $(INCDIR)/fopen-same.h \
  $(INCDIR)/binary-io.h $(INCDIR)/safe-ctype.h \
  ../bfd/bfd.h $(INCDIR)/symcat.h objdump.h bucomm.h \
  config.h $(INCDIR)/bin-bugs.h $(INCDIR)/bfdlink.h \
  $(BFDDIR)/libbfd.h $(INCDIR)/hashtab.h \
  ../bfd/mach-o.h ../bfd/mach-o/external.h \
  $(INCDIR)/mach-o-gnu/external.h $(INCDIR)/mach-o-gnu/codesign.h
rclex.o: rclex.c sysdep.h $(INCDIR)/alloca-conf.h config.h \
  $(INCDIR)/ansidecl.h ../bfd/bfdver.h $(INCDIR)/fopen-same.h \
  $(INCDIR)/binary-io.h ../bfd/bfd.h $(INCDIR)/symcat.h \
  bucomm.h config.h $(INCDIR)/bin-bugs.h $(INCDIR)/libiberty.h \
  $(INCDIR)/safe-ctype.h windres.h \
  winduni.h rcparse.h
rdcoff.o: rdcoff.c ../bfd/bfd.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/symcat.h $(INCDIR)/coff/internal.h bucomm.h config.h \
  $(INCDIR)/bin-bugs.h $(INCDIR)/fopen-same.h $(INCDIR)/libiberty.h \
  debug.h budbg.h $(BFDDIR)/libcoff.h \
  $(INCDIR)/bfdlink.h
rddbg.o: rddbg.c ../bfd/bfd.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/symcat.h bucomm.h config.h $(INCDIR)/bin-bugs.h \
  $(INCDIR)/fopen-same.h $(INCDIR)/libiberty.h \
  debug.h budbg.h
readelf.o: readelf.c ../bfd/bfd.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/symcat.h $(INCDIR)/elf/common.h $(INCDIR)/elf/external.h \
  $(INCDIR)/elf/internal.h $(INCDIR)/elf/dwarf2.h $(INCDIR)/elf/alpha.h \
  $(INCDIR)/elf/reloc-macros.h $(INCDIR)/elf/arc.h $(INCDIR)/elf/arm.h \
  $(INCDIR)/elf/avr.h $(INCDIR)/elf/cris.h $(INCDIR)/elf/d10v.h \
  $(INCDIR)/elf/d30v.h $(INCDIR)/elf/dlx.h $(INCDIR)/elf/fr30.h \
  $(INCDIR)/elf/frv.h $(INCDIR)/elf/h8.h $(INCDIR)/elf/hppa.h \
  $(INCDIR)/elf/i386.h $(INCDIR)/elf/i370.h $(INCDIR)/elf/i860.h \
  $(INCDIR)/elf/i960.h $(INCDIR)/elf/ia64.h $(INCDIR)/elf/ip2k.h \
  $(INCDIR)/elf/m32c.h $(INCDIR)/elf/m32r.h $(INCDIR)/elf/m68k.h \
  $(INCDIR)/elf/m68hc11.h $(INCDIR)/elf/mcore.h $(INCDIR)/elf/mips.h \
  $(INCDIR)/elf/mmix.h $(INCDIR)/elf/mn10200.h $(INCDIR)/elf/mn10300.h \
  $(INCDIR)/elf/ms1.h $(INCDIR)/elf/msp430.h $(INCDIR)/elf/or32.h \
  $(INCDIR)/elf/pj.h $(INCDIR)/elf/ppc.h $(INCDIR)/elf/ppc64.h \
  $(INCDIR)/elf/s390.h $(INCDIR)/elf/sh.h $(INCDIR)/elf/sparc.h \
  $(INCDIR)/elf/v850.h $(INCDIR)/elf/vax.h $(INCDIR)/elf/x86-64.h \
  $(INCDIR)/elf/xstormy16.h $(INCDIR)/elf/crx.h $(INCDIR)/elf/iq2000.h \
  $(INCDIR)/elf/xtensa.h $(INCDIR)/aout/ar.h bucomm.h config.h \
  $(INCDIR)/bin-bugs.h $(INCDIR)/fopen-same.h $(INCDIR)/getopt.h \
  $(INCDIR)/libiberty.h unwind-ia64.h
rename.o: rename.c ../bfd/bfd.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/symcat.h bucomm.h config.h $(INCDIR)/bin-bugs.h \
  $(INCDIR)/fopen-same.h
resbin.o: resbin.c ../bfd/bfd.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/symcat.h bucomm.h config.h $(INCDIR)/bin-bugs.h \
  $(INCDIR)/fopen-same.h $(INCDIR)/libiberty.h windres.h winduni.h
rescoff.o: rescoff.c ../bfd/bfd.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/symcat.h bucomm.h config.h $(INCDIR)/bin-bugs.h \
  $(INCDIR)/fopen-same.h $(INCDIR)/libiberty.h windres.h winduni.h \
  $(INCDIR)/coff/internal.h $(BFDDIR)/libcoff.h $(INCDIR)/bfdlink.h
resrc.o: resrc.c ../bfd/bfd.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/symcat.h bucomm.h config.h $(INCDIR)/bin-bugs.h \
  $(INCDIR)/fopen-same.h $(INCDIR)/libiberty.h $(INCDIR)/safe-ctype.h \
  windres.h winduni.h
resres.o: resres.c ../bfd/bfd.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/symcat.h bucomm.h config.h $(INCDIR)/bin-bugs.h \
  $(INCDIR)/fopen-same.h $(INCDIR)/libiberty.h \
  windres.h winduni.h
size.o: size.c ../bfd/bfd.h $(INCDIR)/ansidecl.h $(INCDIR)/symcat.h \
  bucomm.h config.h $(INCDIR)/bin-bugs.h $(INCDIR)/fopen-same.h \
  $(INCDIR)/libiberty.h $(INCDIR)/getopt.h
srconv.o: srconv.c ../bfd/bfd.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/symcat.h bucomm.h config.h $(INCDIR)/bin-bugs.h \
  $(INCDIR)/fopen-same.h sysroff.h coffgrok.h $(INCDIR)/libiberty.h \
  $(INCDIR)/getopt.h $(INCDIR)/coff/internal.h \
  $(BFDDIR)/libcoff.h $(INCDIR)/bfdlink.h sysroff.c
stabs.o: stabs.c ../bfd/bfd.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/symcat.h bucomm.h config.h $(INCDIR)/bin-bugs.h \
  $(INCDIR)/fopen-same.h $(INCDIR)/libiberty.h $(INCDIR)/safe-ctype.h \
  $(INCDIR)/demangle.h debug.h budbg.h $(INCDIR)/filenames.h \
  $(INCDIR)/aout/aout64.h $(INCDIR)/aout/stab_gnu.h $(INCDIR)/aout/stab.def
strings.o: strings.c ../bfd/bfd.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/symcat.h $(INCDIR)/getopt.h bucomm.h config.h \
  $(INCDIR)/bin-bugs.h $(INCDIR)/fopen-same.h $(INCDIR)/libiberty.h \
  $(INCDIR)/safe-ctype.h
sysdump.o: sysdump.c ../bfd/bfd.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/symcat.h bucomm.h config.h $(INCDIR)/bin-bugs.h \
  $(INCDIR)/fopen-same.h $(INCDIR)/safe-ctype.h $(INCDIR)/libiberty.h \
  $(INCDIR)/getopt.h sysroff.h sysroff.c
syslex_wrap.o: syslex_wrap.c syslex.c config.h sysinfo.h
unwind-ia64.o: unwind-ia64.c unwind-ia64.h $(INCDIR)/elf/ia64.h \
  $(INCDIR)/elf/reloc-macros.h ../bfd/bfd.h \
  $(INCDIR)/symcat.h
elfedit.o: elfedit.c sysdep.h $(INCDIR)/alloca-conf.h \
  config.h $(INCDIR)/ansidecl.h ../bfd/bfdver.h $(INCDIR)/fopen-same.h \
  $(INCDIR)/binary-io.h ../bfd/bfd.h $(INCDIR)/symcat.h \
  elfcomm.h $(INCDIR)/aout/ar.h bucomm.h config.h $(INCDIR)/bin-bugs.h \
  $(INCDIR)/elf/common.h $(INCDIR)/elf/external.h \
  $(INCDIR)/elf/internal.h $(INCDIR)/getopt.h $(INCDIR)/libiberty.h \
  $(INCDIR)/safe-ctype.h $(INCDIR)/filenames.h
version.o: version.c ../bfd/bfd.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/symcat.h ../bfd/bfdver.h bucomm.h config.h \
  $(INCDIR)/bin-bugs.h $(INCDIR)/fopen-same.h
windres.o: windres.c config.h ../bfd/bfd.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/symcat.h $(INCDIR)/getopt.h bucomm.h $(INCDIR)/bin-bugs.h \
  $(INCDIR)/fopen-same.h $(INCDIR)/libiberty.h $(INCDIR)/safe-ctype.h \
  $(INCDIR)/obstack.h windres.h winduni.h
winduni.o: winduni.c ../bfd/bfd.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/symcat.h bucomm.h config.h $(INCDIR)/bin-bugs.h \
  $(INCDIR)/fopen-same.h winduni.h $(INCDIR)/safe-ctype.h
wrstabs.o: wrstabs.c ../bfd/bfd.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/symcat.h bucomm.h config.h $(INCDIR)/bin-bugs.h \
  $(INCDIR)/fopen-same.h $(INCDIR)/libiberty.h $(INCDIR)/safe-ctype.h \
  debug.h budbg.h $(INCDIR)/aout/aout64.h $(INCDIR)/aout/stab_gnu.h \
  $(INCDIR)/aout/stab.def
windmc.o: windmc.c sysdep.h $(INCDIR)/alloca-conf.h \
  config.h $(INCDIR)/ansidecl.h ../bfd/bfdver.h $(INCDIR)/fopen-same.h \
  $(INCDIR)/binary-io.h ../bfd/bfd.h $(INCDIR)/symcat.h \
  $(INCDIR)/getopt.h bucomm.h config.h $(INCDIR)/bin-bugs.h \
  $(INCDIR)/libiberty.h $(INCDIR)/safe-ctype.h \
  $(INCDIR)/obstack.h windmc.h winduni.h windint.h
mclex.o: mclex.c sysdep.h $(INCDIR)/alloca-conf.h config.h \
  $(INCDIR)/ansidecl.h ../bfd/bfdver.h $(INCDIR)/fopen-same.h \
  $(INCDIR)/binary-io.h ../bfd/bfd.h $(INCDIR)/symcat.h \
  bucomm.h config.h $(INCDIR)/bin-bugs.h $(INCDIR)/libiberty.h \
  $(INCDIR)/safe-ctype.h windmc.h \
  winduni.h mcparse.h
arparse.o: arparse.c ../bfd/bfd.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/symcat.h bucomm.h config.h $(INCDIR)/bin-bugs.h \
  $(INCDIR)/fopen-same.h arsup.h
arlex.o: arlex.c config.h $(INCDIR)/ansidecl.h $(INCDIR)/libiberty.h \
  arparse.h
sysroff.o: sysroff.c
sysinfo.o: sysinfo.c
syslex.o: syslex.c config.h sysinfo.h
defparse.o: defparse.c ../bfd/bfd.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/symcat.h bucomm.h config.h $(INCDIR)/bin-bugs.h \
  $(INCDIR)/fopen-same.h dlltool.h
deflex.o: deflex.c $(INCDIR)/libiberty.h $(INCDIR)/ansidecl.h \
  defparse.h dlltool.h
nlmheader.o: nlmheader.c $(INCDIR)/ansidecl.h $(INCDIR)/safe-ctype.h \
  ../bfd/bfd.h $(INCDIR)/symcat.h bucomm.h config.h $(INCDIR)/bin-bugs.h \
  $(INCDIR)/fopen-same.h $(INCDIR)/nlm/common.h $(INCDIR)/nlm/internal.h \
  nlmconv.h
rcparse.o: rcparse.c ../bfd/bfd.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/symcat.h bucomm.h config.h $(INCDIR)/bin-bugs.h \
  $(INCDIR)/fopen-same.h $(INCDIR)/libiberty.h \
  windres.h winduni.h $(INCDIR)/safe-ctype.h
rclex.o: rclex.c ../bfd/bfd.h $(INCDIR)/ansidecl.h \
  $(INCDIR)/symcat.h bucomm.h config.h $(INCDIR)/bin-bugs.h \
  $(INCDIR)/fopen-same.h $(INCDIR)/libiberty.h $(INCDIR)/safe-ctype.h \
  windres.h winduni.h rcparse.h
mcparse.o: mcparse.c sysdep.h $(INCDIR)/alloca-conf.h \
  config.h $(INCDIR)/ansidecl.h ../bfd/bfdver.h $(INCDIR)/fopen-same.h \
  $(INCDIR)/binary-io.h ../bfd/bfd.h $(INCDIR)/symcat.h \
  bucomm.h config.h $(INCDIR)/bin-bugs.h $(INCDIR)/libiberty.h \
  windmc.h winduni.h $(INCDIR)/safe-ctype.h
# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
