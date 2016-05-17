# Target: VxWorks running on an Intel 960
TDEPFILES= i960-tdep_old.o remote-vx.o remote-vx960.o xdr_ld.o \
	xdr_ptrace.o xdr_rdb.o
TM_FILE= tm-vx960.h

# Define this for the vx-share routines, which do NOT see param_old.h.
MT_CFLAGS= -DI80960

VPATH += ./vx-share
