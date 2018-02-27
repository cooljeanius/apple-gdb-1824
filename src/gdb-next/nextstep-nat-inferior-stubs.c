/*
 * nextstep-nat-inferior-stubs.c
 */

#include "defs.h"
#include "target.h"
#include "event-loop.h"
#include "nextstep-nat-inferior-stubs.h"

/* The following functions are defined for the benefit of inftarg.c;
 * they should never be called */

int ATTRIBUTE_NORETURN
attach(int pid)
{
  internal_error(__FILE__, __LINE__,
		 "nextstep_nat_inferior: unexpected call to attach()");
}

void ATTRIBUTE_NORETURN
detach(int sig)
{
  internal_error(__FILE__, __LINE__,
		 "nextstep_nat_inferior: unexpected call to detach()");
}

void ATTRIBUTE_NORETURN
kill_inferior(void)
{
  internal_error(__FILE__, __LINE__,
		 "nextstep_nat_inferior: unexpected call to kill_inferior()");
}

void ATTRIBUTE_NORETURN
child_resume(int pid, int step, enum target_signal sig)
{
  internal_error(__FILE__, __LINE__,
		 "nextstep_nat_inferior: unexpected call to child_resume()");
}

int ATTRIBUTE_NORETURN
child_wait(int pid, struct target_waitstatus *status,
	   gdb_client_data client_data)
{
  internal_error(__FILE__, __LINE__,
		 "nextstep_nat_inferior: unexpected call to child_wait()");
}

int ATTRIBUTE_NORETURN
child_xfer_memory(CORE_ADDR memaddr, gdb_byte *myaddr, int len, int write,
		  struct mem_attrib *attrib, struct target_ops *target)
{
  internal_error(__FILE__, __LINE__,
		 "nextstep_nat_inferior: unexpected call to child_xfer_memory()");
}

/* EOF */
