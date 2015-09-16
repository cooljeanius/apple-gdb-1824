# Target: Intel 386 with a.out in osf 1/mk
TDEPFILES= i386-tdep.o
TM_FILE= tm-i386osf1mk.h

# quoting here is just to fix syntax highlighting, feel free to remove
# if it messes stuff up:
TM_CFLAGS= "-I/usr/mach3/include"
TM_CLIBS= /usr/mach3/ccs/lib/libmachid.a /usr/mach3/ccs/lib/libnetname.a \
	/usr/mach3/ccs/lib/libmach.a
