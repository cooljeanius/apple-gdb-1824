/*
 * nextstep-nat-inferior.h
 */

#ifndef _NEXTSTEP_NAT_INFERIOR_H_
#define _NEXTSTEP_NAT_INFERIOR_H_

#include "defs.h"
#include "target.h"

#include "nextstep-nat-sigthread.h"
#include "nextstep-nat-excthread.h"
#if defined(WITH_CFM) && WITH_CFM
# include "nextstep-nat-cfmthread.h"
#endif /* WITH_CFM */
#include "nextstep-nat-dyld.h"
#include "nextstep-nat-threads.h"

#include <mach/mach.h>

struct kinfo_proc;

struct next_inferior_status
{
  int pid;
  task_t task;

  int attached_in_ptrace;
  int stopped_in_ptrace;

  unsigned int suspend_count;

  thread_t last_thread;

  next_signal_thread_status signal_status;
  next_exception_thread_status exception_status;
#if defined(WITH_CFM) && WITH_CFM
  next_cfm_thread_status cfm_status;
#endif /* WITH_CFM */
  next_dyld_thread_status dyld_status;
};
typedef struct next_inferior_status next_inferior_status;

void next_mach_check_new_threads(void);
ptid_t next_wait (struct next_inferior_status *inferior,
                  struct target_waitstatus *status,
		  gdb_client_data client_data);

extern int inferior_bind_exception_port_flag;
extern int inferior_bind_notify_port_flag;
extern int inferior_handle_exceptions_flag;

/* from rhapsody-nat.c and macosx-nat.c */

void next_create_inferior_for_task
  PARAMS ((struct next_inferior_status *inferior, task_t task, int pid));

void next_fetch_task_info PARAMS ((struct kinfo_proc **info, size_t *count));

char **next_process_completer PARAMS((const char *text, char *word));

#endif /* _NEXTSTEP_NAT_INFERIOR_H_ */
