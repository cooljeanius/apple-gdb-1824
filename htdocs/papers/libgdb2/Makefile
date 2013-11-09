all: libgdb_toc.html libgdb.info libgdb.html

libgdb_toc.html: libgdb.texi
	texi2html -split_chapter libgdb.texi
libgdb.html: libgdb.texi
	texi2html -monolithic libgdb.texi

libgdb.info: libgdb.texi
	makeinfo libgdb.texi
