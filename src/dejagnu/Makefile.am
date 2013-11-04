## Process this file with automake to generate Makefile.in

ACLOCAL_AMFLAGS = -I./m4 --install

AUTOMAKE_OPTIONS = 1.4 dejagnu -Wno-portability

# Install all the associated data files
SUBDIRS = doc testsuite example

# Install the bourne shell script driver in the bindir
bin_SCRIPTS = runtest

# Install the main DejaGnu expect file in the datadir
pkgdata_SCRIPTS = runtest.exp

# Install config.guess too, so we can use it to figure out
# what type of machine we are running on.
libexecdir = $(pkgdatadir)/libexec
libexec_SCRIPTS = config.guess

TLCSH = @TCLSH@

# renamed because the old variable overrode the default Automake one:
RUNTESTDEFAULTFLAGSLOCAL = --srcdir $(srcdir)/testsuite RUNTEST=$(RUNTESTLOCAL)
RUNTESTLOCAL = ` \
  if [ -f ${srcdir}/runtest$(EXEEXT) ] ; then \
    echo ${srcdir}/runtest$(EXEEXT); \
  else echo "ERROR: runtest not found" ; exit 1; fi`

CLEANFILES = x.log x.sum site.bak setval.tmp
check-DEJAGNU-local:
	cd testsuite ; $(MAKE) check
	cd example ; $(MAKE) check
	@echo "Do nothing here, because we do NOT want to run the tests twice"
.PHONY: check-DEJAGNU-local

# Since we do (did) NOT use SUBDIRS, we have to manually clean the sub
# directories.
# We do (did) NOT use SUBDIRS, so the example or test cases do (did) NOT
# get built by default for a "make all".
clean-local:
	for my_subdir in `echo doc testsuite example`; do \
	  if test -e $${my_subdir}/Makefile; then \
	    cd $${my_subdir} && $(MAKE) clean; \
	  fi; \
	done;

# Hack:
.PHONY: -c

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
