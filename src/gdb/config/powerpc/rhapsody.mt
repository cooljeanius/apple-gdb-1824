# rhapsody.mt
# Target: PowerPC running NeXTStep/Rhapsody

MT_CFLAGS = \
	-DTARGET_POWERPC \
	-I../gdb-next \
	-Imacosx

VPATH += ../gdb-next

TDEPFILES = \
	core-macho.o \
	xcoffread.o \
	ppc-next-tdep.o \
	ppc-frameinfo.o \
	ppc-frameops.o \
	ppc-tdep.o \
	remote-kdp.o \
	kdp-udp.o \
	kdp-transactions.o \
	kdp-protocol.o \
	nextstep-tdep.o

TM_FILE = tm-ppc-next.h
DEPRECATED_TM_FILE = tm-ppc-next.h
