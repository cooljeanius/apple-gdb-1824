/*
 * macosx/macosx-nat-mutils.h
 */

#ifndef __GDB_MACOSX_NAT_MUTILS_H__
#define __GDB_MACOSX_NAT_MUTILS_H__

#include "defs.h"
#include "memattr.h"
#include "target.h"
#include "inferior.h"
#include "macosx-nat-utils.h"

#include <mach/mach.h>

struct target_ops;

void mutils_debug(const char *fmt, ...)
  ATTR_FORMAT(gnu_printf, 1, 2);

void ATTR_NORETURN gdb_check(const char *str, const char *file,
			     unsigned int line, const char *func);
void ATTR_NORETURN gdb_check_fatal(const char *str, const char *file,
				   unsigned int line, const char *func);

int
mach_xfer_memory(CORE_ADDR memaddr, gdb_byte *myaddr,
                 int len, int write,
                 struct mem_attrib *attrib, struct target_ops *target);

LONGEST
mach_xfer_partial(struct target_ops *ops,
		  enum target_object object, const char *annex,
		  gdb_byte *readbuf, const gdb_byte *writebuf,
		  ULONGEST offset, LONGEST len);

int macosx_port_valid(mach_port_t port);
int macosx_task_valid(task_t task);
int macosx_thread_valid(task_t task, thread_t thread);
int macosx_pid_valid(int pid);

thread_t macosx_primary_thread_of_task(task_t task);

kern_return_t macosx_msg_receive(mach_msg_header_t * msgin, size_t msgsize,
                                 unsigned long timeout, mach_port_t port);

extern int call_ptrace(int, int, PTRACE_ARG3_TYPE, int);


CORE_ADDR allocate_space_in_inferior_mach(int len);

CORE_ADDR macosx_allocate_space_in_inferior(int len);

# if defined(HAVE_64_BIT_STACK_LOGGING) && HAVE_64_BIT_STACK_LOGGING
void macosx_clear_logging_path(void);
# endif /* HAVE_64_BIT_STACK_LOGGING */

void malloc_history_info_command(const char *arg, int from_tty);

int build_path_to_element(struct type *type, CORE_ADDR offset,
                          char **symbol_name);

char *
get_symbol_at_address_on_stack(CORE_ADDR stack_address, int *frame_level);

void gc_reference_tracing_command(const char *arg, int from_tty);
#endif /* __GDB_MACOSX_NAT_MUTILS_H__ */

/* EOF */
