## Process this file with automake to generate Makefile.in

ACLOCAL_AMFLAGS = -I../m4 --install
AUTOMAKE_OPTIONS = gnits dejagnu
AUTOMAKE_DEPRECATED_OPTIONS = cygnus

noinst_LIBRARIES = libangsd.a

libangsd_a_SOURCES = ardi.c angel_bytesex.c crc.c devsw.c drivers.c \
                     etherdrv.c hostchan.c hsys.c logging.c msgbuild.c \
                     params.c rx.c serdrv.c serpardr.c tx.c unixcomm.c

noinst_HEADERS = adp.h adperr.h angel.h ardi.h armdbg.h buffers.h \
                 chandefs.h channels.h chanpriv.h crc.h dbg_conf.h \
                 dbg_cp.h dbg_hif.h dbg_rdi.h devclnt.h devices.h \
                 devsw.h drivers.h angel_endian.h ethernet.h host.h \
                 hostchan.h hsys.h logging.h msgbuild.h params.h rxtx.h \
                 sys.h unixcomm.h angel_bytesex.h

EXTRA_DIST = README.CYGNUS

AM_CPPFLAGS = -DRETRANS -DARM_RELEASE='"Berkeley Licence for Cygnus"'
AM_CFLAGS = -Wall -Wextra -Wimplicit -Wreturn-type -Wcomment -Wtrigraphs -Wformat=2 -Wparentheses -Wpointer-arith -Wunused-label -Wunused-function -Wno-unused-parameter -Wundef -Wc++-compat -Wmissing-prototypes -Wstrict-prototypes -Wmissing-declarations -Winline -Wvolatile-register-var -Woverlength-strings -Wno-pragmas -ggdb

DISTCLEANFILES = *~ autoscan.log

MAINTAINERCLEANFILES = configure.scan

configure.scan:
	autoscan || touch $@

distclean-local:
	rm -rfv autom4te.cache || rmdir autom4te.cache
