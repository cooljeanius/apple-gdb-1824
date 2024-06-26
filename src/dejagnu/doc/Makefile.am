## Process this file with automake to generate Makefile.in

AUTOMAKE_OPTIONS = 1.11.6 dejagnu foreign -Wno-obsolete

man_MANS = runtest.1
# FIXME: We need to generate an info doc from SGML somehow, but for
# now, do NOT install the old texinfo document from version 1.3
# info_TEXINFOS = dejagnu.texi
DOCBOOK = @DOCBOOK@
TARGETS = overview.html overview.ps overview.pdf overview.rtf

DISTCLEANFILES = *~

docs: $(TARGETS)
	$(AM_V_at)for mytarg in $(TARGETS); do
	  if test -e $${mytarg}; then \
	    stat $${mytarg} || echo "issue with $${mytarg}"; \
	  else \
	    echo "$${mytarg} is missing"; \
	  fi; \
	done
.PHONY: docs

%.ps: %.dvi
	src=$(srcdir); \
	dir=`(cd $${src}; pwd)`; \
	dvips -o $@ $*.dvi

%.pdf: %.sgml
	src=$(srcdir); \
	dir=`(cd $${src}; pwd)`; \
	$(DOCBOOK)/db2pdf $${dir}/$*.sgml

%.dvi: %.sgml
	src=$(srcdir); \
	dir=`(cd $${src}; pwd)`; \
	$(DOCBOOK)/db2dvi $${dir}/$*.sgml

%.rtf: %.sgml
	src=$(srcdir); \
	dir=`(cd $${src}; pwd)`; \
	touch $@; \
	$(DOCBOOK)/db2rtf -o $@ $${dir}/$*.sgml

%.gif: %.fig
	convert -transparency white $< $@ # .fig -> .gif

%.epsi: %.eps
	src=$(srcdir); \
	dir=`(cd $${src}; pwd)`; \
	ps2epsi $${dir}/$*.sgml                        # .eps -> .epsi

%.eps: %.fig
	fig2dev -L ps -m 0.7 -p dummy $< > $@          # .fig -> .eps/portrait

%.html: %.sgml
	src=$(srcdir); \
	dir=`(cd $${src}; pwd)`; \
	$(DOCBOOK)/db2html $${dir}/$*.sgml

# now for some extra dependencies that the automatic rules will not
# catch:

html rtf overview.pdf overview.ps overview.dvi overview.rtf overview.html: overview.sgml ref.sgml user.sgml
.PHONY: rtf

clean-local realclean distclean-local maintainer-clean-local: Makefile
	rm -fv *~
	rm -fr $(TARGETS) DBHTOHTML* overview.junk overview overview.{aux,dvi,log,ps,pdf,tex}
	rm -fr autom4te.cache || rmdir autom4te.cache
.PHONY: clean-local realclean distclean-local maintainer-clean-local

install-doc: $(TARGETS)
	$(mkinstalldirs) $(DESTDIR)$(pkgdatadir)/html $(DESTDIR)$(pkgdatadir)/html/stylesheet-images
	$(INSTALL_DATA) overview/*.html $(DESTDIR)$(pkgdatadir)/html
	$(INSTALL_DATA) overview/stylesheet-images/*.png $(DESTDIR)$(pkgdatadir)/html/stylesheet-images
	$(INSTALL_DATA) overview.rtf $(DESTDIR)$(pkgdatadir)/dejagnu.rtf
	$(INSTALL_DATA) overview.ps $(DESTDIR)$(pkgdatadir)/dejagnu.ps
	$(INSTALL_DATA) overview.pdf $(DESTDIR)$(pkgdatadir)/dejagnu.pdf
	$(INSTALL_DATA) $(srcdir)/runtest.1 $(mandir)/man1/
.PHONY: install-doc
