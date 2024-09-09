## automake Makefile for zlib
## Process this file with automake to create Makefile.in.

AUTOMAKE_OPTIONS = 1.11.6 foreign dejagnu no-dist no-installinfo -Wno-obsolete
## "cygnus" == "foreign no-dist"
AUTOMAKE_DEPRECATED_OPTIONS = cygnus
TEXINFO_TEX = ../texinfo/texinfo.tex

ACLOCAL_AMFLAGS = -I m4 -I.. -I../config --install

## configure should subst HOST_ARCHITECTURE for us:
C_ARCHFLAGS = @C_ARCHFLAGS@
CXX_ARCHFLAGS = $(C_ARCHFLAGS)
AM_CFLAGS = $(C_ARCHFLAGS) $(WARN_CFLAGS) $(WARN_PEDANTIC)
AM_CXXFLAGS = $(CXX_ARCHFLAGS)

## likewise with the other archflags:
C_BUILDARCHFLAGS = @C_BUILDARCHFLAGS@
C_TARGARCHFLAGS = @C_TARGARCHFLAGS@

ZLIB_SOURCES = adler32.c compress.c crc32.c crc32.h deflate.c \
deflate.h gzguts.h gzread.c gzclose.c gzwrite.c gzlib.c \
infback.c inffast.c inffast.h inffixed.h inflate.c \
inflate.h inftrees.c inftrees.h trees.c trees.h uncompr.c zconf.h \
zconf.h.in zlib.h zutil.c zutil.h

if TARGET_LIBRARY
noinst_LTLIBRARIES = libzgcj_convenience.la
libzgcj_convenience_la_SOURCES = $(ZLIB_SOURCES)
else
toolexeclib_LIBRARIES = libz.a
libz_a_SOURCES = $(ZLIB_SOURCES)
libz_a_CFLAGS = $(AM_CFLAGS) $(PICFLAG)
endif

# Work around what appears to be a GNU make bug handling MAKEFLAGS
# values defined in terms of make variables, as is the case for CC and
# friends when we are called from the top level Makefile.
AM_MAKEFLAGS = \
	"AR_FLAGS=$(AR_FLAGS)" \
	"CC_FOR_BUILD=$(CC_FOR_BUILD)" \
	"CFLAGS=$(CFLAGS) $(C_ARCHFLAGS)" \
	"CXXFLAGS=$(CXXFLAGS) $(CXX_ARCHFLAGS)" \
	"CFLAGS_FOR_BUILD=$(CFLAGS_FOR_BUILD) $(C_BUILDARCHFLAGS)" \
	"CFLAGS_FOR_TARGET=$(CFLAGS_FOR_TARGET) $(C_TARGARCHFLAGS)" \
	"INSTALL=$(INSTALL)" \
	"INSTALL_DATA=$(INSTALL_DATA)" \
	"INSTALL_PROGRAM=$(INSTALL_PROGRAM)" \
	"INSTALL_SCRIPT=$(INSTALL_SCRIPT)" \
	"LDFLAGS=$(LDFLAGS)" \
	"LIBCFLAGS=$(LIBCFLAGS)" \
	"LIBCFLAGS_FOR_TARGET=$(LIBCFLAGS_FOR_TARGET)" \
	"MAKE=$(MAKE)" \
	"MAKEINFO=$(MAKEINFO) $(MAKEINFOFLAGS)" \
	"PICFLAG=$(PICFLAG)" \
	"PICFLAG_FOR_TARGET=$(PICFLAG_FOR_TARGET)" \
	"SHELL=$(SHELL)" \
	"EXPECT=$(EXPECT)" \
	"RUNTEST=$(RUNTEST)" \
	"RUNTESTFLAGS=$(RUNTESTFLAGS)" \
	"exec_prefix=$(exec_prefix)" \
	"infodir=$(infodir)" \
	"libdir=$(libdir)" \
	"prefix=$(prefix)" \
	"tooldir=$(tooldir)" \
	"AR=$(AR)" \
	"AS=$(AS)" \
	"CC=$(CC)" \
	"CXX=$(CXX)" \
	"LD=$(LD)" \
	"LIBCFLAGS=$(LIBCFLAGS)" \
	"NM=$(NM)" \
	"PICFLAG=$(PICFLAG)" \
	"RANLIB=$(RANLIB)" \
	"DESTDIR=$(DESTDIR)"

## other things from the original Makefile:
CFLAGS_WITH_DEFINES="-O1 -DMAX_WBITS=14 -DMAX_MEM_LEVEL=7"
DEBUG_C_FLAGS_AND_DEFS="-ggdb -DDEBUG"
OPT_CWARNFLAGS="-O3 -Wall -Wwrite-strings -Wpointer-arith -Wconversion -Wstrict-prototypes -Wmissing-prototypes"
SFLAGS=-O1
LDSHARED=$(CC)

STATICLIBZ=libz.a
SHAREDLIBZ=libz.so
DARWIN_SHAREDLIBZ=libz.dylib
SHAREDLIBV=libz.so.1.2.8
SHAREDLIBM=libz.so.1
LDSHAREDLIBC=-lc

SUBDIRS = . contrib/minizip

OBJZ = adler32.o crc32.o deflate.o infback.o inffast.o inflate.o inftrees.o trees.o zutil.o
OBJG = compress.o uncompr.o gzclose.o gzlib.o gzread.o gzwrite.o
OBJ_ALL = $(OBJZ) $(OBJG)

PIC_OBJZ = adler32.lo crc32.lo deflate.lo infback.lo inffast.lo inflate.lo inftrees.lo trees.lo zutil.lo
PIC_OBJG = compress.lo uncompr.lo gzclose.lo gzlib.lo gzread.lo gzwrite.lo
PIC_OBJ_ALL = $(PIC_OBJZ) $(PIC_OBJG)

# to use the asm code: make OBJA=match.o, PIC_OBJA=match.lo
OBJA =
PIC_OBJA =

THE_OBJS = $(OBJ_ALL) $(OBJA)

PIC_OBJS = $(PIC_OBJ_ALL) $(PIC_OBJA)

CLEANFILES = *.exe

DISTCLEANFILES = *~ *.cache.bak

mostlyclean-local: clean-local
	-if test -r config.cache && test -w .; then rm -f config.cache; fi
clean-local:
	rm -f *.o *.lo *~ \
	   example minigzip examplesh minigzipsh \
	   example64 minigzip64 \
	   infcover \
	   libz.* foo.gz so_locations \
	   _match.s maketree contrib/infback9/*.o
	rm -rf objs
	rm -rf *.dSYM
	rm -f *.gcda *.gcno *.gcov
	rm -f contrib/infback9/*.gcda contrib/infback9/*.gcno contrib/infback9/*.gcov
	-rm -f CMakeCache.txt
	-if test -d test; then (pushd test && rm -f *.o minigzip && popd); fi
	-if test -d test/Apple_test; then (cd test/Apple_test && rm -f *.o tmpfile* *.gz compress decompress); fi
	-if test -e contrib/perl/Compress-Raw-Zlib-2.063/Makefile; then (cd contrib/perl/Compress-Raw-Zlib-2.063 && make clean); fi
	-if test -e config.cache; then rm -f config.cache; \
	elif test -e config.cache.bak; then mv -f config.cache.bak config.cache; \
	fi

maintainer-clean-local: distclean-local
distclean-local: clean-local
	rm -f Makefile Makefile_orig zlib.pc configure.log autoscan.log
	-rm -rfv .deps/*.*o || rmdir .deps
	-rm -rfv *.cache || (test -d autom4te.cache && rmdir autom4te.cache) || (test -L config.cache && unlink config.cache)
	rm -rf ztest*.dSYM || rmdir ztest*.dSYM
	-if test -e zlib.xcodeproj; then (rm -rf build || rmdir build); fi
	-if test -e contrib/perl/Compress-Raw-Zlib-2.063/Makefile; then (cd contrib/perl/Compress-Raw-Zlib-2.063 && make distclean); fi
	-@rm -f .DS_Store contrib/.DS_Store
