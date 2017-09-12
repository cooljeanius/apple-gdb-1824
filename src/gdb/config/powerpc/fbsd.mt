# Target: PowerPC, running FreeBSD

TDEPFILES= rs6000-tdep.o ppc-sysv-tdep.o ppcfbsd-tdep.o fbsd-tdep.o \
	corelow.o solib.o solib-svr4.o
DEPRECATED_TM_FILE= tm-ppc-eabi.h
