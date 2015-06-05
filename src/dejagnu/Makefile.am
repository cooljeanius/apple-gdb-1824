## automake Makefile for dejagnu
## Process this file with automake to generate Makefile.in

ACLOCAL_AMFLAGS = -I./m4 --install

AUTOMAKE_OPTIONS = 1.11.6 gnits dejagnu -Wno-portability subdir-objects

# Install all the associated data files
EXTRA_DIST = MAINTAINERS runtest site.tmpl

SUBDIRS = doc testsuite example . contrib/bluegnu2.0.3

# Install the bourne shell script driver in the bindir:
bin_SCRIPTS = runtest

# Install the header:
include_HEADERS = dejagnu.h

# Install the main DejaGnu expect file in the datadir:
pkgdata_SCRIPTS = runtest.exp \
  lib/debugger.exp lib/dejagnu.exp lib/dg.exp lib/framework.exp \
  lib/ftp.exp lib/kermit.exp lib/libgloss.exp lib/remote.exp \
  lib/rlogin.exp lib/rsh.exp lib/standard.exp lib/target.exp

configdir = $(pkgdatadir)/config
config_DATA = config/README
config_SCRIPTS = \
	config/arc.exp \
	config/arm-ice.exp \
	config/base-config.exp \
	config/base68k.exp \
	config/bug.exp \
	config/cfdbug.exp \
	config/cygmon.exp \
	config/d10v.exp \
	config/ddb-ether.exp \
	config/ddb.exp \
	config/default.exp \
	config/dos.exp \
	config/dve.exp \
	config/gdb-comm.exp \
	config/gdb_stub.exp \
	config/h8300.exp \
	config/i386-bozo.exp \
	config/i960.exp \
	config/m32r-stub.exp \
	config/m32r.exp \
	config/m68hc11.exp \
	config/m68k-emc.exp \
	config/mips-idt.exp \
	config/mn10200-eval.exp \
	config/mn10300-eval.exp \
	config/netware.exp \
	config/powerpc-bug.exp \
	config/proelf.exp \
	config/rom68k.exp \
	config/sh.exp \
	config/sid.exp \
	config/sim.exp \
	config/slite.exp \
	config/sparclet.exp \
	config/tic80.exp \
	config/unix.exp \
	config/vr4100.exp \
	config/vr4300.exp \
	config/vr5000.exp \
	config/vxworks.exp

baseboarddir = $(pkgdatadir)/baseboards
baseboard_DATA = baseboards/README
baseboard_SCRIPTS = \
	baseboards/am33_2.0-libremote.exp \
	baseboards/arc-sim.exp \
	baseboards/arm-ice.exp \
	baseboards/arm-sid.exp \
	baseboards/arm-sim.exp \
	baseboards/basic-sid.exp \
	baseboards/basic-sim.exp \
	baseboards/cf.exp \
	baseboards/cris-sim.exp \
	baseboards/cygmon.exp \
	baseboards/cygwin.exp \
	baseboards/d10v-sim.exp \
	baseboards/d10v.exp \
	baseboards/d30v-sim.exp \
	baseboards/danlite-elf.exp \
	baseboards/dos.exp \
	baseboards/fr30-cygmon.exp \
	baseboards/fr30-elf.exp \
	baseboards/fr30-sim.exp \
	baseboards/frv-elf.exp \
	baseboards/frv-sim.exp \
	baseboards/frv-tiger.exp \
	baseboards/frv-tomcat.exp \
	baseboards/gdbserver-sample.exp \
	baseboards/h8300-sim.exp \
	baseboards/h8300.exp \
	baseboards/i386-bozo.exp \
	baseboards/i386-sid.exp \
	baseboards/i960-cyclone.exp \
	baseboards/i960-sim.exp \
	baseboards/iq2000-sim.exp \
	baseboards/jmr3904-sim.exp \
	baseboards/linux-gdbserver.exp \
	baseboards/linux-libremote.exp \
	baseboards/m32r-elf.exp \
	baseboards/m32r-linux-sim.exp \
	baseboards/m32r-sid.exp \
	baseboards/m32r-sim.exp \
	baseboards/m68hc11-sim.exp \
	baseboards/m68k-emc.exp \
	baseboards/m68k-sid.exp \
	baseboards/mcore-elf.exp \
	baseboards/mcore-moto-sim.exp \
	baseboards/mcore-pe.exp \
	baseboards/mcore-sim.exp \
	baseboards/mips-idt.exp \
	baseboards/mips-lnews-sim.exp \
	baseboards/mips-lsi-sim.exp \
	baseboards/mips-sim-idt32.exp \
	baseboards/mips-sim-idt64.exp \
	baseboards/mips-sim.exp \
	baseboards/mips64-sim.exp \
	baseboards/mips64vr4100-sim.exp \
	baseboards/mmixware-sim.exp \
	baseboards/mn10200-cygmon.exp \
	baseboards/mn10200-sim.exp \
	baseboards/mn10300-cygmon.exp \
	baseboards/mn10300-sim.exp \
	baseboards/msparc-cygmon.exp \
	baseboards/mt-sid.exp \
	baseboards/op50n.exp \
	baseboards/powerpc-bug.exp \
	baseboards/powerpc-bug1.exp \
	baseboards/powerpc-sim.exp \
	baseboards/powerpcle-sim.exp \
	baseboards/rom68k-idp.exp \
	baseboards/sh-hms.exp \
	baseboards/sh-sid.exp \
	baseboards/sh-sim.exp \
	baseboards/sparc-sim.exp \
	baseboards/sparc64-sim.exp \
	baseboards/sparclet-aout.exp \
	baseboards/sparclite-coff.exp \
	baseboards/sparclite-cygmon.exp \
	baseboards/sparclite-sim-le.exp \
	baseboards/sparclite-sim.exp \
	baseboards/strongarm-cygmon.exp \
	baseboards/tic80-sim.exp \
	baseboards/tx39-dve.exp \
	baseboards/tx39-sim.exp \
	baseboards/unix.exp \
	baseboards/usparc-cygmon.exp \
	baseboards/v850-sim.exp \
	baseboards/vr4100-ddb.exp \
	baseboards/vr4100-sim.exp \
	baseboards/vr4111-sim.exp \
	baseboards/vr4300-ddb.exp \
	baseboards/vr4300-sim.exp \
	baseboards/vr4300.exp \
	baseboards/vr5000-ddb.exp \
	baseboards/vx4300.exp \
	baseboards/vx68k.exp \
	baseboards/vx960.exp \
	baseboards/vxsparc.exp \
	baseboards/x86-cygmon.exp \
	baseboards/xscale-cygmon.exp \
	baseboards/xtensa-sim.exp

# Install config.guess too, so we can use it to figure out
# what type of machine we are running on.
libexecdir = $(pkgdatadir)/libexec
djlibexecdir = $(pkgdatadir)/libexec
libexec_SCRIPTS = config.guess
djlibexec_SCRIPTS = config.guess

# Contributed scripts, etc.

CONTRIB = contrib/compare_tests

TLCSH = @TCLSH@

# renamed because the old variable overrode the default Automake one:
RUNTESTDEFAULTFLAGSLOCAL = --srcdir $(srcdir)/testsuite RUNTEST=$(RUNTESTLOCAL)
RUNTESTLOCAL = ` \
  if [ -f ${srcdir}/runtest$(EXEEXT) ] ; then \
    echo ${srcdir}/runtest$(EXEEXT); \
  else echo "ERROR: runtest not found" ; exit 1; fi`

AM_CXXFLAGS = -I$(top_srcdir) -ggdb -Wall
AM_CFLAGS = -Wall -Wc++-compat -DHAVE_CONFIG_H

OFILES = testglue.o stub-loader.o

ALL_OFILES = $(OFILES) i960glue.o

CLEANFILES = x.log x.sum site.bak setval.tmp $(ALL_OFILES) *~
DISTCLEANFILES = xXx.log xXx.sum testrun.log testrun.sum .DS_Store \
  contrib/.DS_Store autoscan.log
MAINTAINERCLEANFILES = configure.scan

CFILES = testglue.c stub-loader.c i960glue.c

pkgdata_DATA = stub-loader.c testglue.c

# Documentation.

dist_man_MANS = doc/runtest.1
info_TEXINFOS = doc/dejagnu.texi
XML = doc/dejagnu.xml doc/legal.xml doc/ref.xml doc/user.xml

# Now some targets and their dependencies

check-DEJAGNU-local:
	cd testsuite ; $(MAKE) check
	cd example ; $(MAKE) check
	@echo "Do nothing here, because we do NOT want to run the tests twice"
.PHONY: check-DEJAGNU-local

testglue.o: testglue.c
	$(CC) $(CFLAGS) $(AM_CFLAGS) -o testglue.o -c testglue.c

stub-loader.o: stub-loader.c
	$(CC) $(CFLAGS) $(AM_CFLAGS) -o stub-loader.o -c stub-loader.c

i960glue.o: i960glue.c
	$(CC) $(CFLAGS) $(AM_CFLAGS) -o i960glue.o -c i960glue.c

all: $(OFILES)
.PHONY: all

configure.scan: mostlyclean
	autoscan || touch $@

# Since we do (did) NOT use SUBDIRS, we have to manually clean the sub
# directories.
# We do (did) NOT use SUBDIRS, so the example or test cases do (did) NOT
# get built by default for a "make all".
clean-local: Makefile
	for my_subdir in `echo doc testsuite example contrib/bluegnu2.0.3`; do \
	  if test -e $${my_subdir}/Makefile; then \
	    cd $${my_subdir} && $(MAKE) clean-local; \
	  fi; \
	done;
.PHONY: clean-local

# Hack:
.PHONY: -c

distclean-local: config.status Makefile
	-rm -fr autom4te.cache || rmdir autom4te.cache
	-if test -e doc/Makefile; then \
	  cd doc && $(MAKE) distclean-local; \
	fi
.PHONY: distclean-local

realclean: config.status Makefile
	-if test -e doc/Makefile; then \
	  cd doc && $(MAKE) realclean; \
	  if test -d doc/autom4te.cache; then \
	    rm -fr doc/autom4te.cache || rmdir doc/autom4te.cache; \
	  fi; \
	fi
.PHONY: realclean

maintainer-clean-local: realclean
	-if test -e doc/Makefile; then \
	  cd doc && $(MAKE) maintainer-clean-local; \
	elif test -d doc/autom4te.cache; then \
	  rm -fr doc/autom4te.cache || rmdir doc/autom4te.cache; \
	fi
.PHONY: maintainer-clean-local

# Set ourselves up to build a RedHat package
tarball: overview.html overview.ps overview.pdf
	-rm -fr dejagnu-${VERSION}
	cp -fpr $(srcdir) dejagnu-${VERSION}
	@echo "clean up stuff we do NOT want in the source file"
	src=$(srcdir); \
	dir=`(cd $${src}; pwd)`; \
	(cd dejagnu-${VERSION} ; $${dir}/Clean.tcl); \
	-mkdir -p doc/overview; \
	-cp -fr doc/overview dejagnu-${VERSION}/doc/; \
	-cp -fr doc/overview.ps dejagnu-${VERSION}/doc/; \
	-cp -fr doc/overview.pdf dejagnu-${VERSION}/doc/; \
	tar cvf ../dejagnu-${VERSION}.tar dejagnu-${VERSION}; \
	gzip -9fv ../dejagnu-${VERSION}.tar; \
	cp -f ../dejagnu-${VERSION}.tar.gz /usr/src/redhat/SOURCES/; \
	cp -f $${dir}/redhat/dejagnu.spec /usr/src/redhat/SPECS/

DATE = `date "+%Y%m%d`
snapshot:
	-rm -fr dejagnu-${DATE}
	cp -fpr $(srcdir) dejagnu-${DATE}
	@echo "clean up stuff we do NOT want in the source file"
	src=$(srcdir); \
	dir=`(cd $${src}; pwd)`; \
	(cd dejagnu-${DATE} ; $${dir}/Clean.tcl);
	-mkdir -p doc/overview
	-cp -fr doc/overview dejagnu-${DATE}/doc/
	-cp -fr doc/overview.ps dejagnu-${DATE}/doc/
	-cp -fr doc/overview.pdf dejagnu-${DATE}/doc/
	tar cf ../dejagnu-${DATE}.tar dejagnu-${DATE}
	gzip -9fv ../dejagnu-${DATE}.tar

rpmspec: $(srcdir)/redhat/dejagnu.spec
	cp -f $(srcdir)/redhat/dejagnu.spec /usr/src/redhat/SPECS/

rpm: tarball rpmspec
	cp -f ../$(distdir).tar.gz /usr/src/redhat/SOURCES/
	rpm -ba /usr/src/redhat/SPECS/dejagnu.spec

deb: overview.html overview.ps overview.pdf
	-rm -fr dejagnu-${VERSION}
	cp -fpr $(srcdir) dejagnu-${VERSION}
	@echo "clean up stuff we do NOT want in the source file"
	src=$(srcdir); \
	dir=`(cd $${src}; pwd)`; \
	(cd dejagnu-${VERSION} ; $${dir}/Clean.tcl);
	-mkdir -p doc/overview;
	-cp -fr $(srcdir)/debian dejagnu-${VERSION}/;
	-cp -fr doc/overview dejagnu-${VERSION}/doc/;
	-cp -fr doc/overview.ps dejagnu-${VERSION}/doc/;
	-cp -fr doc/overview.pdf dejagnu-${VERSION}/doc/;
	(cd dejagnu-${VERSION}; dpkg-buildpackage)

overview.html overview.ps overview.pdf install-doc:
	cd doc && $(MAKE) $(FLAGS_TO_PASS) $@

# Automake sucks when it comes to building a distribution, so
# we just copy the source tree onto of what Automake did, and
# then we Clean it. Automake only includes the source files.
# We want all the testsuites, data files, and html versions of the doc.
dist-hook: rpmspec overview.html overview.ps overview.pdf
	-rm -fr $(distdir)
	cp -fpr $(srcdir) $(distdir)
	(cd $(distdir) ; $(TCLSH) $(srcdir)/Clean.tcl)
	-cp -fr doc/overview $(distdir)/doc/
	-cp -fr doc/overview.ps $(distdir)/doc/
	-cp -fr doc/overview.pdf $(distdir)/doc/


# Below, host-independent data files that need to get installed.
# We do it this way so we can use globbing.

lib_dest = $(DESTDIR)$(pkgdatadir)
lib_files = $(srcdir)/*.c $(srcdir)/lib/*.exp

baseboards_dest = $(DESTDIR)$(pkgdatadir)/baseboards
baseboards_files = $(srcdir)/baseboards/README $(srcdir)/baseboards/*.exp

config_dest = $(DESTDIR)$(pkgdatadir)/config
config_files = $(srcdir)/config/README $(srcdir)/config/*.exp

install-data-local:
	$(mkinstalldirs) $(lib_dest) $(includedir)
	for f in $(lib_files); do \
	  test ! -f "$$f" || $(INSTALL_DATA) $$f $(lib_dest); \
	done
	$(mkinstalldirs) $(baseboards_dest)
	for f in $(baseboards_files); do \
	  test ! -f "$$f" || $(INSTALL_DATA) $$f $(baseboards_dest); \
	done
	$(INSTALL_DATA) $(srcdir)/dejagnu.h $(includedir); \
	$(mkinstalldirs) $(config_dest)
	for f in $(config_files); do \
	  test ! -f "$$f" || $(INSTALL_DATA) $$f $(config_dest); \
	done

uninstall-local:
	for f in $(lib_files); do \
	  test ! -f "$$f" || rm -f $(lib_dest)/`basename "$$f"`; \
	done
	for f in $(config_files); do \
	  test ! -f "$$f" || rm -f $(config_dest)/`basename "$$f"`; \
	done
	for f in $(baseboards_files); do \
	  test ! -f "$$f" || rm -f $(baseboards_dest)/`basename "$$f"`; \
	done

	for f in $(baseboards_files); do \
	  test ! -f "$$f" || rm -f $(baseboards_dest)/`basename "$$f"`; \
	done
	rm -f $(includedir)/dejagnu.h;

my_site.exp: Makefile
	@echo 'Making a new site.exp file...'
	@test ! -f site.bak || rm -f site.bak
	@echo '## these variables are automatically generated by make ##' > $@-t
	@echo '# Do not edit here. If you wish to override these values' >> $-t
	@echo '# edit the last section' >> $@-t
	@echo 'set srcdir $(srcdir)' >> $@-t
	@echo 'set objdir' `pwd` >> $@-t
	@echo '## All variables above are generated by configure. Do Not Edit ##' >> $@-t
	@test ! -f site.exp || sed '1,/^## All variables above are.*##/ d' site.exp >> $@-t
	@test ! -f site.exp || mv site.exp site.bak
	@mv $@-t site.exp
.PHONY: my_site.exp
