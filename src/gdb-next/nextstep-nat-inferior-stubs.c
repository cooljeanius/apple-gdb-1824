#include "defs.h"
#include "target.h"
#include "event-loop.h"

/* The following functions are defined for the benefit of inftarg.c;
   they should never be called */

int attach (int pid)
{ 
  internal_error (__FILE__, __LINE__, "nextstep_nat_inferior: unexpected call to attach ()");
  return 0;
}

void detach (int sig) 
{ 
  internal_error (__FILE__, __LINE__, "nextstep_nat_inferior: unexpected call to detach ()"); 
}

void kill_inferior ()
{ 
  internal_error (__FILE__, __LINE__, "nextstep_nat_inferior: unexpected call to kill_inferior ()"); 
}

void child_resume (int pid, int step, enum target_signal sig)
{
  internal_error (__FILE__, __LINE__, "nextstep_nat_inferior: unexpected call to child_resume ()");
}

int child_wait (int pid, struct target_waitstatus *status, gdb_client_data client_data)
{
  internal_error (__FILE__, __LINE__, "nextstep_nat_inferior: unexpected call to child_wait ()");
}

int child_xfer_memory
 (CORE_ADDR memaddr, char *myaddr, int len, int write, struct mem_attrib *attrib, struct target_ops *target)
{
  internal_error (__FILE__, __LINE__, "nextstep_nat_inferior: unexpected call to child_xfer_memory ()");
}
