# Target: tic6x
TDEPFILES= tic6x-tdep.o
SIM_OBS = remote-sim.o
SIM = ../sim/tic6x/libsim.a
GDBSERVER_DEPFILES= low-sim.o
GDBSERVER_LIBS = ../../sim/tic6x/libsim.a ../../bfd/libbfd.a ../../libiberty/libiberty.a -lm
