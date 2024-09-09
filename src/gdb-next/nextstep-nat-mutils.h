/*
 * nextstep-nat-mutils.h
 */

#ifndef _NEXTSTEP_NAT_MUTILS_H_
#define _NEXTSTEP_NAT_MUTILS_H_

#include "defs.h"
#include "memattr.h"
#include "inferior.h"

struct target_ops;

#if (!defined __GNUC__ || __GNUC__ < 2 || __GNUC_MINOR__ < (defined __cplusplus ? 6 : 4))
# define __MACH_CHECK_FUNCTION ((__const char *) 0)
#else
# define __MACH_CHECK_FUNCTION __PRETTY_FUNCTION__
#endif /* Something about __GNUC__? */

#define MACH_PROPAGATE_ERROR(ret) \
{ MACH_WARN_ERROR(ret); if ((ret) != KERN_SUCCESS) { return ret; } }

#define MACH_CHECK_ERROR(ret) \
mach_check_error (ret, __FILE__, __LINE__, __MACH_CHECK_FUNCTION);

#define MACH_WARN_ERROR(ret) \
mach_warn_error (ret, __FILE__, __LINE__, __MACH_CHECK_FUNCTION);

#define MACH_ERROR_STRING(ret) \
(mach_error_string (ret) ? mach_error_string (ret) : "[UNKNOWN]")

void gdb_check (const char *str, const char *file, unsigned int line, const char *func);
void gdb_check_fatal (const char *str, const char *file, unsigned int line, const char *func);

unsigned int child_get_pagesize PARAMS((void));

extern int mach_xfer_memory(CORE_ADDR, unsigned char *, int, int,
			    struct mem_attrib *, struct target_ops *);

#ifdef HAVE_MACH_MACH_H
# include <mach/mach.h>
#endif /* HAVE_MACH_MACH_H */

void mach_check_error (kern_return_t ret, const char *file, unsigned int line, const char *func);
void mach_warn_error (kern_return_t ret, const char *file, unsigned int line, const char *func);

#ifdef HAVE_MACH_PORT_H
# include <mach/port.h>
#endif /* HAVE_MACH_PORT_H */

int next_port_valid PARAMS ((port_t port));
int next_task_valid PARAMS ((task_t task));
int next_thread_valid PARAMS ((task_t task, thread_t thread));
int next_pid_valid PARAMS ((int pid));

thread_t next_primary_thread_of_task PARAMS ((task_t task));

kern_return_t next_mach_msg_receive PARAMS ((msg_header_t *msgin, size_t msgsize, unsigned long timeout, port_t port));

int call_ptrace PARAMS((int request, int pid, PTRACE_ARG3_TYPE arg3, int arg4));

extern void mutils_debug(const char *, ...) ATTRIBUTE_PRINTF_1;

#endif /* _NEXTSTEP_NAT_MUTILS_H_ */

/* EOF */
