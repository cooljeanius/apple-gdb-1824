# Target: AArch64 based embedded system
# Based off of the ARM equivalent

TDEPFILES= aarch64-tdep.o remote-rdp.o remote-rdi.o
TDEPLIBS= rdi-share/libangsd.a
DEPRECATED_TM_FILE= tm-embed.h

SIM_OBS = remote-sim.o
SIM = ../sim/arm/libsim.a
