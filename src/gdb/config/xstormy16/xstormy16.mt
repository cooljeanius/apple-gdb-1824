# Target: Sanyo Xstormy16a processor
TDEPFILES = xstormy16-tdep.o
# No simulator objects or libraries are needed -- target uses SID.
SID_SIM_OBS = remote-sim.o
SID_SIM = ../sim/xstormy16/libsim.a
