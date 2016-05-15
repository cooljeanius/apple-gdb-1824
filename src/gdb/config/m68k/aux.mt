# aux.mt
# Target: really old Mac (Apple A/UX running on an old m68k Mac)

TDEPFILES= m68k-tdep.o xcoffread.o m68klinux-tdep.o solib.o solib-svr4.o
# mac-nat.o and mac-xdep.o go in the ".mh" version
TM_FILE= tm-mac.h

MT_CFLAGS = \
	-DTARGET_M68K \
	-I$(srcdir)/macosx \
	-DELIEVE_PCC_PROMOTION=1
##FIXME: this file is mostly just copied from macos.mt; need to tweak this copy
## a bit more