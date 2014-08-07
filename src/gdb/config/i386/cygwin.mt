# cygwin.mt
# Target: Intel 386 running win32 via cygwin

TDEPFILES= i386-tdep.o i386-cygwin-tdep.o i387-tdep.o solib-target.o \
	corelow.o win32-tdep.o

DEPRECATED_TM_FILE= tm-cygwin.h

