/*
 * macosx/macosx-nat-inferior.h
 */

#ifndef __GDB_MACOSX_NAT_INFERIOR_H__
#define __GDB_MACOSX_NAT_INFERIOR_H__

#if !defined(NO_POISON) && defined(POISON_FREE_TOO)
/* include chain from <mach/mach.h> can drag in "free": */
# define NO_POISON 1
#endif /* !NO_POISON && POISON_FREE_TOO */

#include "macosx-nat-threads.h"
#include "macosx-nat-sigthread.h"
#include "macosx-nat-excthread.h"
#include "macosx-nat-dyld.h"
#include "macosx-nat-dyld-info.h"

#if defined(WITH_CFM) && WITH_CFM
# include "macosx-nat-cfmthread.h"
#endif /* WITH_CFM */

#include "defs.h"
#include "target.h"
#include "checkpoint.h"

#include <mach/mach.h>

#define GDB_MACH_EXC_MASK_ALL    (EXC_MASK_BAD_ACCESS |         \
				  EXC_MASK_BAD_INSTRUCTION |	\
				  EXC_MASK_ARITHMETIC |		\
				  EXC_MASK_EMULATION |		\
				  EXC_MASK_SOFTWARE |		\
				  EXC_MASK_BREAKPOINT |		\
				  EXC_MASK_SYSCALL |		\
				  EXC_MASK_MACH_SYSCALL |	\
				  EXC_MASK_RPC_ALERT)


struct kinfo_proc;

struct macosx_inferior_status
{
  int pid;
  task_t task;

  int attached_in_ptrace;
  int stopped_in_ptrace;
  int stopped_in_softexc;

  unsigned int suspend_count;

  thread_t last_thread;

  macosx_signal_thread_status signal_status;
  macosx_exception_thread_status exception_status;
#if defined(WITH_CFM) && WITH_CFM
  macosx_cfm_thread_status cfm_status;
#endif /* WITH_CFM */
};
typedef struct macosx_inferior_status macosx_inferior_status;

/* A list of processes already running at gdb-startup with the same
 * name.  Used for the "-waitfor" command line option so we can ignore
 * existing zombies/running copies of the process/etc and detect a newly
 * launched version: */
struct pid_list
{
  int count;
  pid_t *pids;
};

struct private_thread_info
{
  thread_t app_thread_port;
  void *core_thread_state;
  int gdb_suspend_count;
  int gdb_dont_suspend_stepping;
};

void macosx_check_new_threads(thread_array_t thread_list, unsigned int nthreads);
ptid_t macosx_wait(struct macosx_inferior_status *inferior,
                   struct target_waitstatus *status,
                   gdb_client_data client_data);

extern int inferior_bind_exception_port_flag;
extern int inferior_bind_notify_port_flag;

/* from rhapsody-nat.c and macosx-nat.c */

void macosx_create_inferior_for_task(struct macosx_inferior_status *inferior,
                                     task_t task, int pid);

void macosx_fetch_task_info(struct kinfo_proc **info, size_t *count);

char **macosx_process_completer(const char *text, char *word);

int create_private_thread_info(struct thread_info *thrd_info);
void delete_private_thread_info(struct thread_info *thrd_info);
int create_core_thread_state_cache(struct thread_info *thrd_info);
void delete_core_thread_state_cache(struct thread_info *thrd_info);

/* This should probably go in a separate machoread.h, but since it is
   only one function, I will wait on that:  */
void
macho_calculate_offsets_for_dsym(struct objfile *main_objfile,
                                 bfd *sym_bfd,
				 struct section_addr_info *addrs,
				 struct section_offsets *in_offsets,
				 int in_num_offsets,
				 struct section_offsets **sym_offsets,
				 int *sym_num_offsets);

/* This one is called in macosx-nat-inferior.c, but needs to be provided by
 * the platform-specific nat code.  It allows each platform to add platform
 * specific stuff to the macosx_child_target: */
void macosx_complete_child_target(struct target_ops *target);
int macosx_get_task_for_pid_rights(void);
/* Miscellaneous other silencing of '-Wmissing-declarations' in
 * macosx-nat-inferior.c: */
int macosx_service_one_other_event(struct target_waitstatus *status);
struct macosx_pending_event *macosx_backup_before_break(int ignore);
char **macosx_process_completer_quoted(const char *text, char *word, int quote,
                                       struct pid_list *ignorepids);
void direct_memcache_get(struct checkpoint *cp);
void fork_memcache_put(struct checkpoint *cp);

extern macosx_inferior_status *macosx_status;

extern int inferior_ptrace_flag;
extern int inferior_ptrace_on_attach_flag;
extern int inferior_bind_exception_port_flag;

extern int macosx_fake_resume;
struct macosx_pending_event;
extern struct macosx_pending_event *pending_event_chain, *pending_event_tail;
#endif /* __GDB_MACOSX_NAT_INFERIOR_H__ */

/* EOF */
