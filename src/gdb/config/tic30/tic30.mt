# Target: tic30
TDEPFILES= tic30-tdep.o
SIM_OBS = remote-sim.o
SIM = ../sim/tic30/libsim.a
GDBSERVER_DEPFILES= low-sim.o
GDBSERVER_LIBS = ../../sim/tic30/libsim.a ../../bfd/libbfd.a ../../libiberty/libiberty.a -lm

