# Target: c30
TDEPFILES = c30-tdep.o

#
# There is no simulator provided with gdb (yet).
#
FUTURE_SIM_OBS = remote-sim.o
FUTURE_SIM = ../sim/c30/libsim.a
