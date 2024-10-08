/*
 * macosx/macosx-nat-sigthread.h
 */

#ifndef __GDB_MACOSX_NAT_SIGTHREAD_H__
#define __GDB_MACOSX_NAT_SIGTHREAD_H__

#if !defined(NO_POISON) && defined(POISON_FREE_TOO)
/* included headers can drag in "free": */
# define NO_POISON 1
#endif /* !NO_POISON && POISON_FREE_TOO */

#include "defs.h"
#include "macosx-nat-mutils.h"
#include "macosx-nat-threads.h"

#include <sys/wait.h>

typedef int WAITSTATUS;

struct macosx_signal_thread_message
{
  int pid;
  WAITSTATUS status;
};

struct macosx_signal_thread_status
{
  gdb_thread_t signal_thread;

  int transmit_fd;
  int receive_fd;

  int inferior_pid;
};

typedef struct macosx_signal_thread_message macosx_signal_thread_message;
typedef struct macosx_signal_thread_status macosx_signal_thread_status;

extern void sigthread_debug_re(const char *fmt, ...)
  ATTR_FORMAT(gnu_printf, 1, 2);

void macosx_signal_thread_debug(FILE *f,
                                struct macosx_signal_thread_status *s);
void macosx_signal_thread_debug_status(FILE *f, WAITSTATUS status);

void macosx_signal_thread_init(macosx_signal_thread_status *s);

void macosx_signal_thread_create(macosx_signal_thread_status *s, int pid);
void macosx_signal_thread_destroy(macosx_signal_thread_status *s);

#endif /* __GDB_MACOSX_NAT_SIGTHREAD_H__ */

/* EOF */
