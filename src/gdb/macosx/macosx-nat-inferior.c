/* macosx/macosx-nat-inferior.c: Mac OS X support for GDB, the GNU debugger.
   Copyright 1997, 1998, 1999, 2000, 2001, 2002, 2004
   Free Software Foundation, Inc.

   Contributed by Apple Computer, Inc.

   This file is part of GDB.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* define this because of a libxml header out of our control: */
#if !defined(NO_POISON) && defined(HAVE_LIBXML2)
# define NO_POISON 1
#endif /* !NO_POISON && HAVE_LIBXML2 */
#include "defs.h"
#include "top.h"
#include "inferior.h"
#include "target.h"
#include "symfile.h"
#include "symtab.h"
#include "objfiles.h"
#include "gdb.h"
#include "gdbcmd.h"
#include "gdbcore.h"
#include "gdbthread.h"
#include "regcache.h"
#include "environ.h"
#include "event-top.h"
#include "event-loop.h"
#include "inf-loop.h"
#include "gdb_stat.h"
#include "exceptions.h"
#include "checkpoint.h"
#include "value.h"
#include "gdb_regex.h"
#include "osabi.h"
#include "gdb_assert.h"
#include "objc-lang.h"

#include "bfd.h"

#include <sys/ptrace.h>
#include <sys/signal.h>
#ifndef __has_include
# define __has_include(x) 0
#endif /* !__has_include */
#if defined(HAVE_MACHINE_SETJMP_H) || __has_include(<machine/setjmp.h>)
# include <machine/setjmp.h>
#else
# if defined(HAVE_SETJMP_H) || __has_include(<setjmp.h>)
#  include <setjmp.h>
# else
#  if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#   warning "macosx-nat-inferior.c wants to include a header for setjmp."
#  endif /* __GNUC__ && !__STRICT_ANSI__ */
# endif /* HAVE_SETJMP_H */
#endif /* HAVE_MACHINE_SETJMP_H */
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <ctype.h>
#include <sys/param.h>
#include <sys/sysctl.h>
#include <sys/proc.h>
#include <mach/mach_error.h>
#ifdef HAVE_SPAWN_H
# include <spawn.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "macosx-nat-inferior.c expects <spawn.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_SPAWN_H */

#include <semaphore.h>

#include <dlfcn.h>
#include <libproc.h>
#include <sys/proc_info.h>
#include <libintl.h>

#include "macosx-nat-dyld.h"
#include "macosx-nat-inferior.h"
#include "macosx-nat-infthread.h"
#include "macosx-nat-inferior-debug.h"
#include "macosx-nat-mutils.h"
#include "macosx-nat-excthread.h"
#include "macosx-nat-sigthread.h"
#include "macosx-nat-threads.h"
#include "macosx-xdep.h"
/* classic-inferior-support */
#include "macosx-nat.h"
#include "macosx-nat-inferior-util.h"
#include "macosx-nat-dyld-process.h"

#if WITH_CFM
# include "macosx-nat-cfm.h"
#endif /* WITH_CFM */

#if defined(TARGET_ARM) && !defined(__GDB_ARM_MACOSX_TDEP_H__)
# include "arm-macosx-tdep.h"
#endif /* TARGET_ARM && !__GDB_ARM_MACOSX_TDEP_H__ */

#if !defined(_PTRACE_) && !defined(PTRACE_CONT) && !defined(PTRACE_DETACH) && \
    !defined(PTRACE_KILL) && !defined(PTRACE_TRACEME)
# if !defined(_NM_NEXTSTEP_H_) && !defined(NM_NEXTSTEP)
#  include "vx-share/ptrace.h"
# endif /* !_NM_NEXTSTEP_H_ && !NM_NEXTSTEP */
#endif /* !_PTRACE_ && !PTRACE_CONT && !PTRACE_DETACH && !PTRACE_KILL && !PTRACE_TRACEME */

#include <mach/mach_vm.h>

/* FIXME: redefines ATTRIBUTE_PRINTF on us: */
#if defined(LIBXML2_IS_USABLE) && LIBXML2_IS_USABLE
/* try to prevent C++ headers from being dragged in via icu headers; our
 * our definition of min() and max() macros breaks them: */
# ifdef U_HAVE_STD_STRING
#  undef U_HAVE_STD_STRING
# endif /* U_HAVE_STD_STRING */
# define U_HAVE_STD_STRING 0 /* '0' means "missing" */
# if defined(__cplusplus) && !defined(__STD_STRING_H__)
#  define __STD_STRING_H__ 1
# endif /* __cplusplus && !__STD_STRING_H__ */
# include <libxml/parser.h>
# include <libxml/tree.h>
#endif /* LIBXML2_IS_USABLE */

#ifndef EXC_SOFT_SIGNAL
# define EXC_SOFT_SIGNAL 0
#endif /* !EXC_SOFT_SIGNAL */

/* The code values for single step vrs. breakpoint
   trap are not defined in ppc header files. There is a
   definition in the i386 exception.h, but it is a i386 specific
   define.  */

#if defined(TARGET_I386)
# define SINGLE_STEP EXC_I386_SGL
#elif defined(TARGET_POWERPC)
# define SINGLE_STEP 5
#elif defined(TARGET_ARM)
# define SINGLE_STEP 1
# define SINGLE_STEP_2 0
# if !defined(__GDB_ARM_MACOSX_TDEP_H__)
#  include "arm-macosx-tdep.h"
# endif /* !__GDB_ARM_MACOSX_TDEP_H__ */
#else
# ifdef S_SPLINT_S
#  ifndef SINGLE_STEP
#   define SINGLE_STEP 0
#  endif /* !SINGLE_STEP */
# else
#  error "unknown architecture"
# endif /* S_SPLINT_S */
#endif /* TARGET_(arch) */

#define _dyld_debug_make_runnable(a, b) DYLD_FAILURE
#define _dyld_debug_restore_runnable(a, b) DYLD_FAILURE
#define _dyld_debug_module_name(a, b, c, d, e) DYLD_FAILURE
#define _dyld_debug_set_error_func(a) DYLD_FAILURE
#define _dyld_debug_add_event_subscriber(a, b, c, d, e) DYLD_FAILURE

extern int standard_is_async_p(void);
extern int standard_can_async_p(void);

extern bfd *exec_bfd;

extern struct target_ops deprecated_child_ops;
extern struct target_ops macosx_child_ops;

extern struct target_ops exec_ops;
extern struct target_ops macosx_exec_ops;

macosx_inferior_status *macosx_status = NULL;
extern macosx_dyld_thread_status macosx_dyld_status;

int inferior_ptrace_flag = 1;
int inferior_ptrace_on_attach_flag = 1;
int inferior_bind_exception_port_flag = 1;

struct target_ops macosx_child_ops;
struct target_ops macosx_exec_ops;

/* From inftarg.c, needed by classic-inferior-support */
extern void init_child_ops(void);
extern void init_exec_ops(void);

extern int inferior_auto_start_dyld_flag;

int macosx_fake_resume = 0;

static int announce_attach = 1;

extern int disable_aslr_flag;

enum macosx_source_type
{
  NEXT_SOURCE_NONE = 0x0,
  NEXT_SOURCE_EXCEPTION = 0x1,
  NEXT_SOURCE_SIGNAL = 0x2,
  NEXT_SOURCE_CFM = 0x4,
  NEXT_SOURCE_ERROR = 0x8,
  NEXT_SOURCE_ALL = 0xf
};

struct macosx_pending_event
{
  enum macosx_source_type type;
  unsigned char *buf;
  struct macosx_pending_event *next;
  struct macosx_pending_event *prev;
};

/* 'struct pid_list' has been moved to the header, for use in prototypes */

struct macosx_pending_event *pending_event_chain, *pending_event_tail;

static void (*async_client_callback)(enum inferior_event_type event_type,
                                     void *context);
static void *async_client_context;

static enum macosx_source_type macosx_fetch_event(struct
                                                  macosx_inferior_status
                                                  *inferior,
                                                  unsigned char *buf,
                                                  size_t len,
                                                  unsigned int flags,
                                                  int timeout);

static int macosx_service_event(enum macosx_source_type source,
                                unsigned char *buf,
                                struct target_waitstatus *status);

static void macosx_handle_signal(macosx_signal_thread_message *msg,
                                 struct target_waitstatus *status);

static void macosx_handle_exception(macosx_exception_thread_message *msg,
                                    struct target_waitstatus *status);

static int macosx_process_events(struct macosx_inferior_status *ns,
                                 struct target_waitstatus *status,
                                 int timeout, int service_first_event);

static struct macosx_pending_event *
macosx_add_to_pending_events(enum macosx_source_type, unsigned char *buf);

static int macosx_post_pending_event(void);
static void macosx_pending_event_handler(void *data);
static ptid_t macosx_process_pending_event(struct macosx_inferior_status *ns,
                                           struct target_waitstatus *status,
                                           gdb_client_data client_data);

static void macosx_clear_pending_events(void);

static void macosx_child_stop(void);

static void macosx_child_resume(ptid_t ptid, int step,
                                enum target_signal signal);

static ptid_t macosx_child_wait(ptid_t ptid,
                                struct target_waitstatus *status,
                                gdb_client_data client_data);

static void macosx_mourn_inferior(void);

static int macosx_lookup_task(const char *args, task_t * ptask, int *ppid);

static void macosx_child_attach(const char *args, int from_tty);

static void macosx_child_detach(const char *args, int from_tty);

static int macosx_kill_inferior(void *);
static void macosx_kill_inferior_safe(void);

static void macosx_ptrace_me(void);

static void macosx_ptrace_him(int pid);
/* alias, for gender equality: */
#define macosx_ptrace_her macosx_ptrace_him

static void macosx_child_create_inferior(char *exec_file, char *allargs,
                                         char **env, int from_tty);

static void macosx_child_files_info(struct target_ops *ops);

static char *macosx_get_thread_name(ptid_t ptid);

static char *macosx_get_thread_id_str(ptid_t ptid);

static int macosx_child_thread_alive(ptid_t tpid);

static struct pid_list *find_existing_processes_by_name(const char *procname);
static int pid_present_on_pidlist(pid_t pid, struct pid_list *proclist);

/*@unused@*/static const char *get_bundle_executable_from_plist(const char *pnm)
  ATTRIBUTE_USED;

#if defined(LIBXML2_IS_USABLE) && LIBXML2_IS_USABLE
static const char *find_executable_name_in_xml_tree(xmlNode * a_node);
#endif /* LIBXML2_IS_USABLE */

/* static */ void cpfork_info(const char *args, int from_tty);

extern void _initialize_macosx_inferior(void);

static void
macosx_handle_signal(macosx_signal_thread_message *msg,
                     struct target_waitstatus *status)
{
  kern_return_t kret;

  CHECK_FATAL(macosx_status != NULL);

  CHECK_FATAL(macosx_status->attached_in_ptrace);
  CHECK_FATAL(!macosx_status->stopped_in_ptrace);
#if 0
  CHECK_FATAL(!macosx_status->stopped_in_softexc);
#endif /* 0 */

  if (inferior_debug_flag)
    {
      macosx_signal_thread_debug_status(stderr, msg->status);
    }

  if (msg->pid != macosx_status->pid)
    {
      warning("macosx_handle_signal: signal message was for pid %d, "
              "not for inferior process (pid %d)\n",
              msg->pid, macosx_status->pid);
      return;
    }

  if (WIFEXITED(msg->status))
    {
      status->kind = TARGET_WAITKIND_EXITED;
      status->value.integer = WEXITSTATUS(msg->status);
      return;
    }

  if (!WIFSTOPPED(msg->status))
    {
      status->kind = TARGET_WAITKIND_SIGNALLED;
      status->value.sig = target_signal_from_host(WTERMSIG(msg->status));
      return;
    }

  macosx_status->stopped_in_ptrace = 1;

  kret = macosx_inferior_suspend_mach(macosx_status);
  MACH_CHECK_ERROR(kret);

  prepare_threads_after_stop(macosx_status);

  status->kind = TARGET_WAITKIND_STOPPED;
  status->value.sig = target_signal_from_host(WSTOPSIG(msg->status));
}

static void
macosx_handle_exception(macosx_exception_thread_message *msg,
                        struct target_waitstatus *status)
{
  kern_return_t kret;

  CHECK_FATAL(status != NULL);
  CHECK_FATAL(macosx_status != NULL);

  if (inferior_ptrace_flag)
    {
      CHECK_FATAL(macosx_status->attached_in_ptrace);
      CHECK_FATAL(!macosx_status->stopped_in_ptrace);
    }

  inferior_debug(2, "macosx_handle_exception: received exception message\n");

  if (msg->task_port != macosx_status->task)
    {
      /* If the exception was for a child other than the process being
         debugged, reset the exception ports for the child back to
         default, and resume.  Ideally the exception ports would never
         have been set to the one as modified by GDB in the first
         place, but this should work in most cases. */

      inferior_debug(2,
                     "macosx_handle_exception: exception was for child of process being debugged\n");

      kret =
        macosx_restore_exception_ports(msg->task_port,
                                       &macosx_status->exception_status.
                                       saved_exceptions);
      MACH_WARN_ERROR(kret);

      macosx_inferior_resume_mach(macosx_status, 0);

      status->kind = TARGET_WAITKIND_SPURIOUS;
      return;
    }

  macosx_status->last_thread = msg->thread_port;

  kret = macosx_inferior_suspend_mach(macosx_status);
  MACH_CHECK_ERROR(kret);

  prepare_threads_after_stop(macosx_status);

  status->kind = TARGET_WAITKIND_STOPPED;

  switch (msg->exception_type)
    {
    case EXC_BAD_ACCESS:
      status->value.sig = TARGET_EXC_BAD_ACCESS;
      /* Preserve the exception data so we can print it later.  */
      status->code = (int)msg->exception_data[0];
      /* When gcc casts a signed int to an unsigned long long, it first
         casts it to a long long (sign extending it in the process) then
         it uselessly casts it to unsigned. So we first have to cast it
         to an unsigned of its size. I cannot figure out a way to do that
         automatically, but I know the exception_data is either going to
         be an int, or a long long, so I will just handle those cases.  */

      if (sizeof(msg->exception_data[1]) < sizeof(CORE_ADDR))
        status->address = (CORE_ADDR)((unsigned int)msg->exception_data[1]);
      else
        status->address = (CORE_ADDR)msg->exception_data[1];
      break;
    case EXC_BAD_INSTRUCTION:
      status->value.sig = TARGET_EXC_BAD_INSTRUCTION;
      break;
    case EXC_ARITHMETIC:
      status->value.sig = TARGET_EXC_ARITHMETIC;
      break;
    case EXC_EMULATION:
      status->value.sig = TARGET_EXC_EMULATION;
      break;
    case EXC_SOFTWARE:
      {
        switch (msg->exception_data[0])
          {
          case EXC_SOFT_SIGNAL:
            status->value.sig =
	      target_signal_from_host((unsigned int)msg->exception_data[1]);
            macosx_status->stopped_in_softexc = 1;
            break;
          default:
            status->value.sig = TARGET_EXC_SOFTWARE;
            break;
          }
      }
      break;
    case EXC_BREAKPOINT:
      /* Many internal GDB routines expect breakpoints to be reported
         as TARGET_SIGNAL_TRAP, and will report TARGET_EXC_BREAKPOINT
         as a spurious signal. */
      status->value.sig = TARGET_SIGNAL_TRAP;
      break;
    default:
      status->value.sig = TARGET_SIGNAL_UNKNOWN;
      break;
    }
}

static void
macosx_add_to_port_set(struct macosx_inferior_status *inferior,
                       fd_set * fds, int flags)
{
  FD_ZERO(fds);

  if ((flags & NEXT_SOURCE_EXCEPTION)
      && (inferior->exception_status.receive_from_fd > 0))
    {
      FD_SET(inferior->exception_status.receive_from_fd, fds);
    }

  if ((flags & NEXT_SOURCE_ERROR)
      && (inferior->exception_status.error_receive_fd > 0))
    {
      FD_SET(inferior->exception_status.error_receive_fd, fds);
    }

  if ((flags & NEXT_SOURCE_SIGNAL)
      && (inferior->signal_status.receive_fd > 0))
    {
      FD_SET(inferior->signal_status.receive_fd, fds);
    }
}

/* TIMEOUT is either -1, 0, or greater than 0.
   For 0, check if there is anything to read, but do NOT block.
   For -1, block until there is something to read.
   For >0, block at least the specified number of microseconds, or until there
   is something to read.
   The kernel does NOT give better than ~1HZ (0.01 sec) resolution, so
   do NOT use this as a high accuracy timer. */

static enum macosx_source_type
macosx_fetch_event(struct macosx_inferior_status *inferior,
                   unsigned char *buf, size_t len,
                   unsigned int flags, int timeout)
{
  fd_set fds;
  int fd, ret;
  struct timeval tv;

  CHECK_FATAL(len >= sizeof(macosx_exception_thread_message));
  CHECK_FATAL(len >= sizeof(macosx_signal_thread_message));

  tv.tv_sec = 0;
  tv.tv_usec = timeout;

  macosx_add_to_port_set(inferior, &fds, flags);

  for (;;)
    {
      if (timeout == -1)
        {
          ret = select(FD_SETSIZE, &fds, NULL, NULL, NULL);
        }
      else
        {
          ret = select(FD_SETSIZE, &fds, NULL, NULL, &tv);
        }
      if ((ret < 0) && (errno == EINTR))
        {
          continue;
        }
      if (ret < 0)
        {
          internal_error(__FILE__, __LINE__, "unable to select: %s",
                         safe_strerror(errno));
        }
      if (ret == 0)
        {
          return NEXT_SOURCE_NONE;
        }
      break;
    }

  fd = inferior->exception_status.error_receive_fd;
  if ((fd > 0) && FD_ISSET(fd, &fds))
    {
      read(fd, buf, 1);
      return NEXT_SOURCE_ERROR;
    }

  fd = inferior->exception_status.receive_from_fd;
  if ((fd > 0) && FD_ISSET(fd, &fds))
    {
      read(fd, buf, sizeof(macosx_exception_thread_message));
      return NEXT_SOURCE_EXCEPTION;
    }

  fd = inferior->signal_status.receive_fd;
  if ((fd > 0) && FD_ISSET(fd, &fds))
    {
      read(fd, buf, sizeof(macosx_signal_thread_message));
      return NEXT_SOURCE_SIGNAL;
    }

  return NEXT_SOURCE_NONE;
}

/* This takes the data from an event and puts it on the tail of the
   "pending event" chain. */

static struct macosx_pending_event *
macosx_add_to_pending_events(enum macosx_source_type type,
                             unsigned char *buf)
{
  struct macosx_pending_event *new_event;

  new_event = ((struct macosx_pending_event *)
               xmalloc(sizeof(struct macosx_pending_event)));

  new_event->type = type;

  if (type == NEXT_SOURCE_SIGNAL)
    {
      macosx_signal_thread_message *mssg;
      mssg = ((macosx_signal_thread_message *)
              xmalloc(sizeof(macosx_signal_thread_message)));
      memcpy(mssg, buf, sizeof(macosx_signal_thread_message));
      inferior_debug(1,
                     "macosx_add_to_pending_events: adding a signal event "
		     "to the pending events.\n");
      new_event->buf = (unsigned char *)mssg;
    }
  else if (type == NEXT_SOURCE_EXCEPTION)
    {
      macosx_exception_thread_message *mssg;
      mssg = ((macosx_exception_thread_message *)
              xmalloc(sizeof(macosx_exception_thread_message)));
      memcpy(mssg, buf, sizeof(macosx_exception_thread_message));
      inferior_debug(1,
                     "macosx_add_to_pending_events: adding an exception event "
		     "to the pending events.\n");
      new_event->buf = (unsigned char *)mssg;
    }

  new_event->next = NULL;

  if (pending_event_chain == NULL)
    {
      pending_event_chain = new_event;
      pending_event_tail = new_event;
      new_event->prev = NULL;
    }
  else
    {
      new_event->prev = pending_event_tail;
      pending_event_tail->next = new_event;
      pending_event_tail = new_event;
    }
  return new_event;
}

static void
macosx_free_pending_event (struct macosx_pending_event *event_ptr)
{
  xfree (event_ptr->buf);
  xfree (event_ptr);
}

static int
macosx_count_pending_events(void)
{
  int counter = 0;
  struct macosx_pending_event *event_ptr;

  for (event_ptr = pending_event_chain; event_ptr != NULL; event_ptr = event_ptr->next)
    counter++;

  return counter;
}

static void
macosx_remove_pending_event(struct macosx_pending_event *event_ptr,
                            int delete_it)
{
  if (event_ptr == pending_event_chain)
    pending_event_chain = event_ptr->next;
  if (event_ptr == pending_event_tail)
    pending_event_tail = event_ptr->prev;

  if (event_ptr->prev != NULL)
    event_ptr->prev->next = event_ptr->next;
  if (event_ptr->next != NULL)
    event_ptr->next->prev = event_ptr->prev;

  if (delete_it)
    macosx_free_pending_event(event_ptr);
}

static void
macosx_clear_pending_events(void)
{
  struct macosx_pending_event *event_ptr = pending_event_chain;

  while (event_ptr != NULL)
    {
      pending_event_chain = event_ptr->next;
      macosx_free_pending_event(event_ptr);
      event_ptr = pending_event_chain;
    }
}

/* This extracts the top of the pending event chain and posts a gdb event
   with its content to the gdb event queue.  Returns 0 if there were no
   pending events to be posted, 1 otherwise. */

static int
macosx_post_pending_event (void)
{
  struct macosx_pending_event *event;

  if (pending_event_chain == NULL)
    {
      inferior_debug (1, "macosx_post_pending_event: no events to post\n");
      return 0;
    }
  else
    {
      event = pending_event_chain;
      macosx_remove_pending_event (event, 0);
      inferior_debug (1,
                      "macosx_post_pending_event: consuming event off queue\n");
      gdb_queue_event (macosx_pending_event_handler, (void *) event, HEAD);

      return 1;
    }
}

static void
macosx_pending_event_handler (void *data)
{
  inferior_debug (4, "Called in macosx_pending_event_handler\n");
  async_client_callback (INF_REG_EVENT, data);
}

static int
macosx_service_event (enum macosx_source_type source,
                      unsigned char *buf, struct target_waitstatus *status)
{
  /* We are using code not equal to -1 to signal we have extra goodies that
     the upper level code might want to print.  */
  status->code = -1;

  if (source == NEXT_SOURCE_EXCEPTION)
    {
      macosx_exception_thread_message *msg =
        (macosx_exception_thread_message *)buf;
      inferior_debug(1,
                     "macosx_service_events: got exception message 0x%lx\n",
                     (unsigned long)msg->exception_type);
      CHECK_FATAL(inferior_bind_exception_port_flag);
      macosx_handle_exception((macosx_exception_thread_message *) buf,
                              status);
      if (status->kind != TARGET_WAITKIND_SPURIOUS)
        return 1;
    }
  else if (source == NEXT_SOURCE_SIGNAL)
    {
      inferior_debug (1, "macosx_service_events: got signal message\n");
      macosx_handle_signal ((macosx_signal_thread_message *) buf, status);
      CHECK_FATAL (status->kind != TARGET_WAITKIND_SPURIOUS);
      return 1;
    }
  else if (source == NEXT_SOURCE_ERROR)
    {
      inferior_debug (1, "macosx_service_events: got an error\n");
      target_mourn_inferior ();
      return 0;
    }
  else
    {
      error ("got message from unknown source: 0x%08x\n", source);
      return 0;
    }
  return 1;
}

/* We treat single step events, and breakpoint events
   specially - though only if we get more than one event
   at a time.  This enum and the get_exception_type function
   are helpers for the code that does this.  */

enum bp_ss_or_other {
  bp_event = 0,
  ss_event,
  sig_event,
  other_event
};

static enum bp_ss_or_other
get_exception_type (struct macosx_exception_thread_message *msg)
{
  if (msg->exception_type == EXC_BREAKPOINT)
    {
      if (msg->data_count == 2
#ifdef SINGLE_STEP_2
	  && msg->exception_data[0] == SINGLE_STEP
	  && msg->exception_data[1] == SINGLE_STEP_2)
#else
	  && msg->exception_data[0] == SINGLE_STEP)
#endif
	return ss_event;
      else
	return bp_event;
    }
  else if (msg->exception_type == EXC_SOFTWARE
      && (msg->data_count == 2)
      && (msg->exception_data[0] == EXC_SOFT_SIGNAL))
    return sig_event;
  else
    return other_event;
}

static enum bp_ss_or_other
get_event_type(struct macosx_pending_event *event)
{
  if (event->type == NEXT_SOURCE_EXCEPTION)
    {
      macosx_exception_thread_message *msg =
	(macosx_exception_thread_message *)event->buf;
      return get_exception_type(msg);
    }
  else
    return other_event;
}

/* This function services the first of the non-breakpoint type events.
   It pushes all the other "other" type events back on the pending events chain.
   It deletes all the others.
   FIXME: This is a bit of a hack, but I do NOT know how to REALLY push the
   signal events back onto the target.  So I have to fake it by leaving them
   around on the pending event queue, and that will mean the next time you
   try to run, you will hit the next event without actually running...
   Fortunately, it looks like this is academic, because the system
   seems to serialize all the other events for the debugger.  */

int
macosx_service_one_other_event(struct target_waitstatus *status)
{
  struct macosx_pending_event *event;
  int count = 0;

  event = pending_event_chain;

  while (event != NULL)
    {
      struct macosx_pending_event *next_event = event->next;
      macosx_exception_thread_message *msg =
	(macosx_exception_thread_message *)event->buf;
      if ((event->type != NEXT_SOURCE_EXCEPTION)
	  || (get_exception_type(msg) == other_event)
	  || (get_exception_type(msg) == sig_event))
	{
	  count++;
	  if (count == 1)
	    {
	      macosx_service_event(event->type, event->buf, status);
	      macosx_remove_pending_event(event, 1);
	    }
	}
      else
	{
	  macosx_remove_pending_event(event, 1);
	}
      event = next_event;
    }
  return count;
}

/* If possible, "back up" EVENT.  Right now, we only
   know how to back up breakpoint events.  */

static void
macosx_backup_this_event(struct macosx_pending_event *event)
{
    if (event->type == NEXT_SOURCE_EXCEPTION)
      {
	macosx_exception_thread_message *msg =
	  (macosx_exception_thread_message *)event->buf;
	ptid_t ptid = ptid_build(macosx_status->pid, 0, msg->thread_port);
	CORE_ADDR new_pc = (read_pc_pid(ptid) - DECR_PC_AFTER_BREAK);

	/* APPLE LOCAL - If we are processing a breakpoint trap,
	   the only time we might not want to back up the PC is if
	   the trap is part of the user's program (rather than
	   inserted by gdb).  If gdb set a breakpoint there, or
	   there is not a breakpoint trap there (because, say, the
	   trap was already removed on a different thread), then
	   back up the PC.  */

	if (get_exception_type(msg) == bp_event)
	  if (breakpoint_here_p(new_pc)
	      || address_contained_breakpoint_trap(new_pc))
	  {
	    /* Back up the PC if necessary: */
	    if (DECR_PC_AFTER_BREAK)
	      {
		write_pc_pid(new_pc, ptid);
		inferior_debug(6, "backup_before_break: setting PID for thread: 0x%lx to %s\n",
                               (unsigned long)msg->thread_port, paddr_nz(new_pc));
	      }
	  }
      }
}

/* Backs up the pc to before the breakpoint (if necessary) for all
   the pending breakpoint events - except for breakpoint event
   IGNORE, if IGNORE is not < 0.  Returns the event ignored, or
   NULL if there is no such event.  NB. IGNORE is the ordinal
   of the breakpoint event among the breakpoint events pending,
   not among all events pending.  */

struct macosx_pending_event *
macosx_backup_before_break(int ignore)
{
  int count = 0;
  struct macosx_pending_event *ret_event = NULL, *event;

  for (event = pending_event_chain; event != NULL; event = event->next)
    {
      if (get_event_type(event) == bp_event)
	{
	  if (count == ignore)
	    {
	      ret_event = event;
	    }
	  else
	    macosx_backup_this_event(event);

	  count++;
	}
    }

  if (DECR_PC_AFTER_BREAK)
    {
      if (ret_event)
	inferior_debug(6, "Backing up all breakpoint hits except thread 0x%lx\n",
                       (unsigned long)((macosx_exception_thread_message *)ret_event->buf)->thread_port);
      else
	inferior_debug(6, "Backing up all breakpoint hits\n");
    }
  return ret_event;
}

/* Un-nested from the following function for both
 * '-Wdeclaration-after-statement' and '-Wnested-externs': */
extern ptid_t get_hand_call_ptid(void);

/* This drains the event sources.  The first event found is directly
   handled.  The rest are "pushed back" to the target as best we can.

   The priority is:

   1) If there is a single step event pending, we report that.
   2) Otherwise, if there are any non-breakpoint events, we report
   them.
   3) Otherwise we pick one breakpoint, and report that.
       a) If the scheduler is locked, and one of the breakpoints is
          on the locked thread, then we report that.
       b) Otherwise we pick a breakpoint at random from the list.

   All the other events are pushed back if we know how to do this.

   Caveats:
   1) At present, I do NOT know how to "push back" a signal. So if there
   is more than one SOFTEXC event we just send them all. Not sure what
   gdb will do with this. I have NOT been able get the system to send
   more than one at a time.
   2) Ditto for other traps. Dunno what I would do with two EXC_BAD_ACCESS
   messages, for instance.

   Returns: The number of events found. */

static int
macosx_process_events(struct macosx_inferior_status *inferior,
                      struct target_waitstatus *status, int timeout,
                      int service_first_event ATTRIBUTE_UNUSED)
{
  enum macosx_source_type source;
  unsigned char buf[1024];
  int event_count = 0;
  int breakpoint_count = 0;
  int other_count = 0;
  int scheduler_bp = -1;
  enum bp_ss_or_other event_type;
  int hand_call_function_bp = -1;
  struct macosx_pending_event *event = NULL, *single_step = NULL,
    *hand_call_function_thread_event = NULL,
    *signal_event = NULL;

  int first_time_through = 1;

  CHECK_FATAL(status->kind == TARGET_WAITKIND_SPURIOUS);

  event_count = macosx_count_pending_events();
  if (event_count != 0)
    {
      inferior_debug(2, "Had a pending event of the old kind.\n");
      return event_count;
    }

  /* Fetch events from the exc & signal threads.  First time through,
     we use TIMEOUT and wait, then we poll to drain the rest of the
     events the exc & signal threads got.  */
  for (;;)
    {
      source = macosx_fetch_event(inferior, buf, sizeof(buf),
                                  NEXT_SOURCE_ALL, timeout);
      /* After we wake up from select, get the write lock.  This
         insures that we do NOT start reading the exception data when
         the exception thread is only part way through writing it.  Be
         sure to release this before exiting the function.  Since each
         event is written in a single shot, we do NOT have to worry
         about getting a partial first event, it is only the subsequent
         events we need to sync with.  */
      if (first_time_through)
        {
          first_time_through = 0;
          macosx_exception_get_write_lock(&inferior->exception_status);
        }

      if (source == NEXT_SOURCE_NONE)
        {
          break;
        }

      event_count++;
      if (source == NEXT_SOURCE_ERROR)
	{
	  /* We could NOT read from the inferior exception port. Dunno why,
	     but we are NOT going to get much further. So tell ourselves that
	     the target exited, cons up some bogus status, and get out
	     of here.  */
	  inferior_debug(2, "Got NEXT_SOURCE_ERROR from macosx_fetch_event\n");
	  status->kind = TARGET_WAITKIND_EXITED;
	  status->value.integer = 0;
	  return 0;
	}
      else if (source == NEXT_SOURCE_SIGNAL)
	{
	  event = macosx_add_to_pending_events(source, buf);
	  signal_event = event;
	  other_count++;
	}
      else
        {
	  struct ptid this_ptid;
	  this_ptid = ptid_build(macosx_status->pid, 0,
				 ((macosx_exception_thread_message *)buf)->thread_port);

	  event = macosx_add_to_pending_events(source, buf);
	  event_type = get_exception_type((macosx_exception_thread_message *)buf);


	  if (event_type == bp_event)
	    {
	      struct thread_info *tp = NULL;
	      CORE_ADDR new_pc = (read_pc_pid(this_ptid) - DECR_PC_AFTER_BREAK);

              /* Sometimes the kernel is NOT ready to tell us about a
               * pending exception when we stop. But when we resume
               * the task, it will tell us about it. For the most
               * part this is okay, but if we have suspended a thread,
               * and run the task, and THEN the kernel comes back &
               * tells us that the thread we have suspended just got a
               * chance to run & hit a breakpoint, that confuses us.
               * This is particularly bad when we are single-stepping
               * over the real instruction under one of our
               * breakpoints, since then it will say we have just hit a
               * trap that we have already removed.
               * So just pretend this event did NOT happen, and arrange
               * for it to be rewound.  */

	      tp = find_thread_pid(this_ptid);
	      if ((tp != NULL) && (tp->privatedata->gdb_suspend_count != 0))
		{
		  inferior_debug(2, "Backing up and ignoring event for thread 0x%x since the thread was suspended.\n",
				 ((macosx_exception_thread_message *)buf)->thread_port);
		  macosx_backup_this_event(event);
		  macosx_remove_pending_event(event, 1);
		  event_count--;
		  goto loop_cleanup;
		}
              /* APPLE LOCAL - Sometimes a breakpoint trap gets hit by
               * two or more threads simultaneously, but the kernel
               * hands us the exceptions one at a time, rather than
               * all together. By the time we are handling it on the
               * second or third thread, the trap has already been
               * removed and we have no record of the breakpoint. In
               * which case we need to check the PC against addresses
               * containing recently removed breakpoint traps; if it is
               * in the list, backup the PC on the thread in question
               * and remove the event.  */
	      else if (!breakpoint_here_p(new_pc)
		       && address_contained_breakpoint_trap(new_pc))
		{
		  inferior_debug(2, "Backing up and ignoring event for thread 0x%x since there is no trap for the breakpont.\n",
				 ((macosx_exception_thread_message *)buf)->thread_port);
		  macosx_backup_this_event(event);
		  macosx_remove_pending_event(event, 1);
		  event_count--;
		  goto loop_cleanup;
		}
	    }

	  /* If this event is for the thread we were calling a function
	     on, prefer that event.  Also record the bp number if it is a
	     breakpoint event, so we can rewind the right breakpoints.  */

	  if (ptid_equal(this_ptid, get_hand_call_ptid()))
	    {
	      hand_call_function_thread_event = event;
	      if (event_type == bp_event)
		hand_call_function_bp = breakpoint_count;
	    }

	  if (event_type == ss_event)
	    single_step = event;
	  else if (event_type == bp_event)
	    {
	      if (scheduler_lock_on_p())
		{
		  if (ptid_equal(get_scheduler_lock_ptid(), this_ptid))
		    scheduler_bp = breakpoint_count;
		}
	      breakpoint_count += 1;
	    }
	  else if (event_type == sig_event)
	    {
	      signal_event = event;
	      other_count += 1;
	    }
	  else
	    other_count += 1;
        }
    loop_cleanup:
      timeout = 0;
    }

  if (other_count == 0) {
    ; /* ??? */
  }

  macosx_exception_release_write_lock(&inferior->exception_status);

  if (event_count == 0)
    {
      macosx_clear_pending_events();
      inferior_debug(2, "No events I actually want to process.\n");
      return 0;
    }

  inferior_debug(2,
          "macosx_process_events: returning with (status->kind == %d)\n",
                 status->kind);

  /* Okay, now that we have gotten the events what should we do?
     If we only got one, let us service it and exit. */
  if (event_count == 1)
    {
      int retval;

      if (macosx_service_event(event->type,
                               event->buf, status) == 0)
	retval = 0;
      else
	retval = 1;

      macosx_clear_pending_events();
      return retval;
    }
  else
    {
      /* If we have more than one, then look through them to figure out
       * what to do.  */

      /* We have more than one exception event, but the upper layers of gdb
       * can only deal with one at a time. We currently use the following
       * heuristics pick which one we are going to report:
       *
       * * First priority is single-step events. This is so we can finish
       *   stepping over a breakpoint without interruption.
       * * Next, if there is a signal event, we handle it. That is because
       *   I do NOT know how to push back a signal event.
       * * Next is if we are calling a function by hand and that stops, we
       *   should handle that immediately.
       * * If there is any other non-breakpoint exception in the set, we
       *   handle that next.
       * * Next if we hit a breakpoint on the thread we were trying to run
       *   the scheduler on, we prefer that. This is an issue because we
       *   might have pushed back many breakpoint hits, then tried to
       *   run something on one of the threads. We do NOT want to service
       *   the left-over events from the other threads till we are done with
       *   what we are doing on this thread.
       * * Finally, we randomly pick one of the breakpoint events, and
       *   handle that.
       */

      if (single_step != NULL)
	{
	  inferior_debug(2, "macosx_process_events: found a single step "
			 "event, and %d breakpoint events\n", breakpoint_count);
	  macosx_backup_before_break(-1);
	  event_count = macosx_service_event(single_step->type,
                                             single_step->buf, status);
	}
      else if (signal_event != NULL)
	{
	  inferior_debug(2, "macosx_process_events: forwarding signal event");
	  macosx_backup_before_break(-1);
	  event_count = macosx_service_event(signal_event->type,
					     signal_event->buf,
					     status);
	}
      else if (hand_call_function_thread_event != NULL)
	{
	  inferior_debug(2, "macosx_process_events: found an event for "
			 "the hand-call function thread "
			 ", and %d breakpoint events\n", breakpoint_count);
	  macosx_backup_before_break(hand_call_function_bp);
	  event_count = macosx_service_event(hand_call_function_thread_event->type,
                                             hand_call_function_thread_event->buf,
                                             status);
	}
      else if (breakpoint_count != event_count)
	{
	  /* This is the case where we have maybe some breakpoints, and one or more
	     other exceptions.  */
	  inferior_debug(2, "macosx_process_events: found %d breakpoint events out of "
			 "%d total events\n", breakpoint_count, event_count);
	  macosx_backup_before_break(-1);
	  event_count = macosx_service_one_other_event(status);
	}
      else
	{
	  /* This is the case where we just have a bunch of breakpoint hits: */
	  struct macosx_pending_event *chosen_break;
	  int random_selector;
	  if (scheduler_bp != -1)
	    {
              /* If we are trying to run a single thread, then let us favor
               * that thread over another that might have just gotten
               * created and hit a breakpoint: */
	      inferior_debug(6, "macosx_process_events: Choosing scheduler "
			     "breakpoint thread index: %d\n", scheduler_bp);
	      random_selector = scheduler_bp;
	    }
	  else
	    {
	      /* Otherwise pick one of the threads randomly, and report
	       * the breakpoint hit for that one.  */
	      random_selector = ((int)
                                 ((breakpoint_count * (double)rand())
                                  / (double)((float)RAND_MAX + 1.0f)));
	      inferior_debug(6, "macosx_process_events: Choosing random "
			     "selector index %d\n", random_selector);
	    }
	  chosen_break = macosx_backup_before_break(random_selector);
	  event_count = macosx_service_event(chosen_break->type,
                                             chosen_break->buf, status);
	}
      macosx_clear_pending_events();
    }
  return event_count;
}

/* */
void
macosx_check_new_threads(thread_array_t thread_list, unsigned int nthreads)
{
  kern_return_t kret;
  unsigned int i;
  int dealloc_thread_list = (thread_list == NULL);

  if (thread_list == NULL)
    {
      kret = task_threads(macosx_status->task, &thread_list, &nthreads);
      MACH_CHECK_ERROR(kret);
    }

  for (i = 0; i < nthreads; i++)
    {
      ptid_t ptid = ptid_build(macosx_status->pid, 0, thread_list[i]);
      struct thread_info *tp0;

      tp0 = find_thread_pid(ptid);

      if (tp0 == NULL)
        {
          struct thread_info *tp1;
          tp1 = add_thread(ptid);
          if (create_private_thread_info(tp1))
            tp1->privatedata->app_thread_port =
              get_application_thread_port(thread_list[i]);
        }
      else if (tp0->privatedata && (tp0->privatedata->app_thread_port == 0))
	{
	  /* This seems a little odd, but it turns out when we stop
	     early on in the program startup, the mach_thread_names
	     call does NOT return any threads, even though task_threads
	     does. So we keep trying and eventually it will work.  */
	  tp0->privatedata->app_thread_port =
	    get_application_thread_port(thread_list[i]);
	}
    }

  if (dealloc_thread_list)
    {
      kret =
	vm_deallocate(mach_task_self(), (vm_address_t)thread_list,
		      (nthreads * sizeof(int)));
      MACH_CHECK_ERROR(kret);
    }
}

/* This differs from child_stop in that we do NOT send
   "-inferior_process_group" to kill when we are attached to the
   process, we just send inferior_process_group.  Even this is kind
   of a lie, since inferior_process_group really is not, it is just
   the pid of the child process, look at "terminal_init_inferior"
   in inflow.c, which sets inferior_process_group.  This just passes
   in the pid of the child process!  I think all the job control
   stuff in inflow.c looks bogus to me, we ought to use MacOS X
   specific versions everywhere we can, and avoid that mess...
*/
static void
macosx_child_stop(void)
{
  pid_t pid = PIDGET(inferior_ptid);
  int ret;

  ret = kill(pid, SIGINT);

  if (ret == -1) {
    warning("Unable to kill process-id %d: %s (%d).\n",
            pid, safe_strerror(errno), errno);
    /* If we got here, errno should be one of EINVAL, EPERM, or ESRCH */
  }
}

/* */
static void
macosx_child_resume(ptid_t ptid, int step, enum target_signal signal)
{
  int nsignal = target_signal_to_host(signal);
  struct target_waitstatus status;
  int stop_others = 1;
  pid_t pid;
  thread_t thread;

  status.code = -1;

  if (ptid_equal(ptid, minus_one_ptid))
    {
      ptid = inferior_ptid;
      stop_others = 0;
    }

  pid = ptid_get_pid(ptid);
  thread = (thread_t)ptid_get_tid(ptid);

  /* pid 0 is reserved for kernel_task on OS X: */
  if (pid == 0) {
    ; /* should probably do something; right now just silence a warning */
  }

  CHECK_FATAL(macosx_status != NULL);

  macosx_inferior_check_stopped(macosx_status);
  if (!macosx_inferior_valid(macosx_status))
    return;

  /* Check for pending events.  If we find any, then we will NOT really resume,
     but rather we will extract the first event from the pending events
     queue, and post it to the gdb event queue, and then "pretend" that we
     have in fact resumed. */

  inferior_debug(2, "macosx_child_resume: checking for pending events\n");
  status.kind = TARGET_WAITKIND_SPURIOUS;
  macosx_process_events(macosx_status, &status, 0, 0);

  inferior_debug(1, "macosx_child_resume: %s process with signal %d\n",
                 (step ? "stepping" : "continuing"), nsignal);

  if (macosx_post_pending_event())
    {
      /* QUESTION: Do I need to lie about target_executing here? */
      macosx_fake_resume = 1;
      if (target_is_async_p())
        target_executing = 1;
      return;
    }

  if (macosx_status->stopped_in_ptrace || macosx_status->stopped_in_softexc)
    {
      macosx_inferior_resume_ptrace(macosx_status, thread, nsignal,
                                    PTRACE_CONT);
    }

  if (!macosx_inferior_valid(macosx_status))
    return;

  if (step)
    prepare_threads_before_run(macosx_status, step, thread, 1);
  else
    prepare_threads_before_run(macosx_status, 0, thread, stop_others);

  macosx_inferior_resume_mach(macosx_status, -1);

  if (target_can_async_p())
    target_async(inferior_event_handler, 0);

  if (target_is_async_p())
    target_executing = 1;
}

static ptid_t
macosx_process_pending_event(struct macosx_inferior_status *ns ATTRIBUTE_UNUSED,
                             struct target_waitstatus *status,
                             gdb_client_data client_data)
{
  struct macosx_pending_event *event =
    (struct macosx_pending_event *)client_data;

  inferior_debug(1, "Processing pending event type: %d\n", event->type);
  macosx_service_event(event->type, (unsigned char *)event->buf, status);

  return ptid_build(macosx_status->pid, 0, macosx_status->last_thread);
}

/*
 * This fetches & processes events from the exception & signal file
 * descriptors.  If client_data is not NULL, then the client data must
 * hold a macosx_pending_event structure, and in that case, this just
 * processes that event.
 */
ptid_t
macosx_wait(struct macosx_inferior_status *ns,
            struct target_waitstatus *status, gdb_client_data client_data)
{
  int first_pass = 1;
  CHECK_FATAL(ns != NULL);

  if (client_data != NULL)
    return macosx_process_pending_event(ns, status, client_data);

  set_sigint_trap();
  set_sigio_trap();

  status->kind = TARGET_WAITKIND_SPURIOUS;
  while (status->kind == TARGET_WAITKIND_SPURIOUS)
    {
      if (first_pass)
	first_pass = 0;
      else
	  macosx_inferior_resume_mach(ns, -1);

      macosx_process_events(ns, status, -1, 1);
    }

  clear_sigio_trap();
  clear_sigint_trap();

  if ((status->kind == TARGET_WAITKIND_EXITED)
      || (status->kind == TARGET_WAITKIND_SIGNALLED))
    return null_ptid;

  macosx_check_new_threads(NULL, 0);

  if (!macosx_thread_valid(macosx_status->task, macosx_status->last_thread))
    {
      if (macosx_task_valid(macosx_status->task))
        {
          warning("Currently selected thread no longer alive; "
                  "selecting initial thread");
          macosx_status->last_thread =
            macosx_primary_thread_of_task(macosx_status->task);
        }
    }

  return ptid_build(macosx_status->pid, 0, macosx_status->last_thread);
}

static ptid_t
macosx_child_wait(ptid_t pid ATTRIBUTE_UNUSED,
                  struct target_waitstatus *status,
                  gdb_client_data client_data)
{
  CHECK_FATAL(macosx_status != NULL);
  return macosx_wait(macosx_status, status, client_data);
}

static void
macosx_mourn_inferior(void)
{
  unpush_target(&macosx_child_ops);
  deprecated_child_ops.to_mourn_inferior();
  macosx_inferior_destroy(macosx_status);

  inferior_ptid = null_ptid;
  attach_flag = 0;

  macosx_dyld_mourn_inferior();

  macosx_clear_pending_events();
  remove_thread_event_breakpoints();
#if HAVE_64_BIT_STACK_LOGGING
  macosx_clear_logging_path();
#endif /* HAVE_64_BIT_STACK_LOGGING */
}

void
macosx_fetch_task_info(struct kinfo_proc **info, size_t * count)
{
  struct kinfo_proc *proc;
  int control[4] = { CTL_KERN, KERN_PROC, KERN_PROC_ALL, 0 };
  size_t length;

  CHECK_FATAL(info != NULL);
  CHECK_FATAL(count != NULL);

  sysctl(control, 3, NULL, &length, NULL, 0);
  proc = (struct kinfo_proc *)xmalloc(length);
  sysctl(control, 3, proc, &length, NULL, 0);

  *count = (length / sizeof(struct kinfo_proc));
  *info = proc;
}

/* */
char **
macosx_process_completer_quoted(const char *text, char *word, int quote,
                                struct pid_list *ignorepids)
{
  struct kinfo_proc *proc = (struct kinfo_proc *)NULL;
  size_t count, i, found = 0UL;
  pid_t gdb_pid = getpid();

  char **procnames = NULL;
  char **ret = NULL;
  int quoted = 0;

  if (text[0] == '"')
    {
      quoted = 1;
    }

  macosx_fetch_task_info(&proc, &count);

  procnames = (char **)xmalloc((count + 1) * sizeof(char *));

  for (i = 0; i < count; i++)
    {
      char *temp;
      size_t templen, found_procnames_len;
      /* gdb cannot attach to the same instance of itself: */
      if (proc[i].kp_proc.p_pid == gdb_pid)
        continue;
      /* classic-inferior-support: */
      if (!can_attach(proc[i].kp_proc.p_pid))
        continue;
      if (pid_present_on_pidlist(proc[i].kp_proc.p_pid, ignorepids))
        continue;
      /* Skip zombie processes: */
      if ((proc[i].kp_proc.p_stat == SZOMB) || proc[i].kp_proc.p_stat == 0)
        continue;
      /* FIXME: check math: */
      templen = (strlen(proc[i].kp_proc.p_comm)
		 + strlen(target_pid_to_str(pid_to_ptid(proc[i].kp_proc.p_pid)))
		 + 1UL + 16UL + 12UL);
      temp = (char *)xmalloc(templen);
      snprintf(temp, templen, "%s.%d", proc[i].kp_proc.p_comm,
	       (int)proc[i].kp_proc.p_pid);
      found_procnames_len = ((strlen(temp) * 2UL) + 2UL + 2UL);
      procnames[found] = (char *)xmalloc(found_procnames_len);
      if (quote)
        {
          if (quoted)
            {
              snprintf(procnames[found], found_procnames_len, "\"%s\"", temp);
            }
          else
            {
              char *s = temp;
              char *t = procnames[found];
              while (*s != '\0')
                {
                  if (strchr("\" ", *s) != NULL)
                    {
                      *t++ = '\\';
                      *t++ = *s++;
                    }
                  else
                    {
                      *t++ = *s++;
                    }
                }
              *t++ = '\0';
            }
        }
      else
        {
          snprintf(procnames[found], found_procnames_len, "%s", temp);
        }
      found++;
    }
  procnames[found] = NULL;

  ret = complete_on_enum((const char **)procnames, text, word);

  xfree(proc);
  return ret;
}

/* */
char **
macosx_process_completer(const char *text, char *word)
{
  return macosx_process_completer_quoted(text, word, 1, NULL);
}

/* */
static void
macosx_lookup_task_local(char *pid_str, pid_t pid, task_t *ptask, int *ppid,
                         struct pid_list *ignorepids)
{
  CHECK_FATAL(ptask != NULL);
  CHECK_FATAL(ppid != NULL);

  if (pid_str == NULL)
    {

      task_t itask;
      kern_return_t kret;

      kret = task_for_pid(mach_task_self(), pid, &itask);
      if (kret != KERN_SUCCESS)
	{
	  if (macosx_get_task_for_pid_rights() == 1)
	    kret = task_for_pid(mach_task_self(), pid, &itask);
	}

      if (kret != KERN_SUCCESS)
        {
          error(_("Unable to access task for process-id %d: %s."), pid,
		MACH_ERROR_STRING(kret));
        }

      *ptask = itask;
      *ppid = pid;

    }
  else
    {
      struct cleanup *cleanups = NULL;
      char **ret = macosx_process_completer_quoted(pid_str, pid_str, 0,
                                                   ignorepids);
      char *tmp = NULL;
      char *tmp2 = NULL;
      unsigned long lpid = 0UL;

      task_t itask;
      kern_return_t kret;

      cleanups = make_cleanup(xfree, ret);

      if ((ret == NULL) || (ret[0] == NULL))
        {
          error(_("Unable to locate process named \"%s\"."), pid_str);
        }
      if (ret[1] != NULL)
        {
          error(_("Multiple processes exist with the name \"%s\"."), pid_str);
        }

      tmp = strrchr(ret[0], '.');
      if (tmp == NULL)
        {
          error(_("Unable to parse process-specifier \"%s\" "
		  "(does not contain process-id)"), ret[0]);
        }
      tmp++;
      lpid = strtoul(tmp, &tmp2, 10);
      if (!isdigit(*tmp) || (*tmp2 != '\0'))
        {
          error(_("Unable to parse process-specifier \"%s\" "
		  "(does not contain process-id)"), ret[0]);
        }
      if ((lpid > INT_MAX) || ((lpid == ULONG_MAX) && (errno == ERANGE)))
        {
          error(_("Unable to parse process-id \"%s\" (integer overflow)."),
                ret[0]);
        }
      pid = (pid_t)lpid;

      kret = task_for_pid(mach_task_self(), pid, &itask);
      if (kret != KERN_SUCCESS)
	{
	  if (macosx_get_task_for_pid_rights() == 1)
	    kret = task_for_pid(mach_task_self(), pid, &itask);
	}

      if (kret != KERN_SUCCESS)
        {
          error(_("Unable to locate task for process-id %d: %s."), pid,
                MACH_ERROR_STRING(kret));
        }

      *ptask = itask;
      *ppid = pid;

      do_cleanups(cleanups);
    }
}

/* Execute a busywait loop and poll to see if a process of a given
   name has been started.  procname is case sensitive and does not
   include the pathname of the process.  */

static void
wait_for_process_by_name(const char *procname, struct pid_list *ignorepids)
{
  struct kinfo_proc *proc = NULL;
  size_t count, i;

  if (procname == NULL || procname[0] == '\0')
    return;

  printf_filtered(_("Waiting for process '%s' to launch.\n"), procname);

  while (1)
    {
      QUIT;
      macosx_fetch_task_info(&proc, &count);
      for (i = 0; i < count; i++)
        {
          if (pid_present_on_pidlist(proc[i].kp_proc.p_pid, ignorepids))
            continue;
          if (strncmp(proc[i].kp_proc.p_comm, procname, MAXCOMLEN) == 0)
            {
              xfree(proc);
              return;
            }
        }
      usleep(400);
      xfree(proc);
    }
}

/* -waitfor should ignore any processes by name that are already
   up & running -- we want to attach to the first newly-launched
   process.  So we begin by creating a list of all processes
   with that name that are executing/zombied/etc.
   This function returns an xmalloc'ed array - the caller is
   responsible for freeing it.
   NULL is returned if there are no matching processes. */

static struct pid_list *
find_existing_processes_by_name (const char *procname)
{
  struct kinfo_proc *proc = NULL;
  struct pid_list *pidlist;
  size_t count, i;
  int matching_processes, j;

  macosx_fetch_task_info (&proc, &count);
  for (i = 0, matching_processes = 0; i < count; i++)
    if (strncmp (proc[i].kp_proc.p_comm, procname, MAXCOMLEN) == 0)
      matching_processes++;
  if (matching_processes == 0)
    {
      xfree (proc);
      return NULL;
    }

  pidlist = (struct pid_list *) xmalloc (sizeof (struct pid_list));
  pidlist->count = matching_processes;
  pidlist->pids = (pid_t *) xmalloc (sizeof (pid_t) * matching_processes);

  for (i = 0, j = 0; i < count; i++)
    if (strncmp (proc[i].kp_proc.p_comm, procname, MAXCOMLEN) == 0)
      pidlist->pids[j++] = proc[i].kp_proc.p_pid;

  xfree (proc);
  return pidlist;
}

/* Returns 1 if PID is present on PROCLIST.
   0 if PID is not present or PROCLIST is empty.  */

static int
pid_present_on_pidlist (pid_t pid, struct pid_list *proclist)
{
  int i;
  if (proclist == NULL)
    return 0;
  for (i = 0; i < proclist->count ; i++)
    if (proclist->pids[i] == pid)
      return 1;
  return 0;
}

/* FIXME: needs comment: */
static int
macosx_lookup_task(const char *args, task_t *ptask, int *ppid)
{
  char *pid_str = NULL;
  char *tmp = NULL;
  struct pid_list *ignorepids = NULL; /* processes to ignore */
  struct cleanup *cleanups = NULL;
  char **argv = NULL;
  unsigned int argc;

  unsigned long lpid = 0;
  pid_t pid = 0;

  CHECK_FATAL (ptask != NULL);
  CHECK_FATAL (ppid != NULL);

  *ptask = TASK_NULL;
  *ppid = 0;

  if (args == NULL)
    {
      return 0;
    }

  argv = buildargv(args);
  if (argv == NULL)
    {
      nomem(0);
    }

  cleanups = make_cleanup_freeargv(argv);

  for (argc = 0; argv[argc] != NULL; argc++)
  {
    ; /* (empty) */
  }

  switch (argc)
    {
    case 1:
      pid_str = argv[0];
      break;
    case 2:
      if ((strcmp("-waitfor", argv[0]) == 0)
          || (strcmp("--waitfor", argv[0]) == 0))
        {
           /* If the user-provided process name string is long than the
              proc name string provided by the kernel we will need to truncate
              the user's string if we are going to find a match.  */
           pid_str = argv[1];
           if (strlen(pid_str) > MAXCOMLEN)
             pid_str[MAXCOMLEN] = '\0';
           ignorepids = find_existing_processes_by_name(pid_str);
           if (ignorepids)
             {
               make_cleanup(xfree, ignorepids->pids);
               make_cleanup(xfree, ignorepids);
             }
           wait_for_process_by_name(pid_str, ignorepids);
	   break;
        }
      break; /* -Wimplicit-fallthrough */
    default:
      error(_("Usage: attach <-waitfor procname>|<pid>|<procname>."));
      break;
    }

  CHECK_FATAL(pid_str != NULL);
  lpid = strtoul(pid_str, &tmp, 10);
  if ((pid_str != NULL) && isdigit(*pid_str) && (*tmp == '\0'))
    {
      if ((lpid > INT_MAX) || ((lpid == ULONG_MAX) && (errno == ERANGE)))
        {
          error("Unable to locate pid \"%s\" (integer overflow).", pid_str);
        }
      pid_str = NULL;
      pid = (pid_t)lpid;
    }

  macosx_lookup_task_local(pid_str, pid, ptask, ppid, ignorepids);

  do_cleanups(cleanups);
  return 0;
}


/* declaration for dyld_symbols_prefix is now in macosx-nat-dyld.h */

static void
macosx_child_attach(const char *args, int from_tty ATTRIBUTE_UNUSED)
{
  task_t itask;
  int pid;
  /* Keep the preprocessor condition the same as where the variable is used: */
#ifdef PTRACE_ATTACHEXC
  int ret;
#endif /* PTRACE_ATTACHEXC */
  kern_return_t kret;
  char *exec_file = NULL;

  if (args == NULL)
    {
      error_no_arg(_("process-id to attach"));
    }

  macosx_lookup_task(args, &itask, &pid);
  if (itask == TASK_NULL)
    {
      error("unable to locate task");
    }

  if (itask == mach_task_self())
    {
      error("unable to debug self");
    }

  CHECK_FATAL(macosx_status != NULL);
  macosx_inferior_destroy(macosx_status);

  exec_file = get_exec_file(0);
  if (announce_attach)
    {
      if (exec_file)
        printf_filtered("Attaching to program: `%s', %s.\n",
                        exec_file, target_pid_to_str(pid_to_ptid(pid)));
      else
        printf_filtered("Attaching to %s.\n",
                        target_pid_to_str(pid_to_ptid(pid)));
    }

  /* classic-inferior-support
     A bit of a hack: Despite being in the middle of macosx_child_attach(),
     if we are about to attach to a classic process we are going to use an
     entirely different attach procedure and skip out of here.  */

  if (attaching_to_classic_process_p(pid))
    {
      attach_to_classic_process(pid);
      return;
    }

  /* A native (i386) gdb trying to attach to a translated (ppc) app will
     result in a gdb crash.  Let us flag it as an error instead.  */
  if ((is_pid_classic(getpid()) == 0) && (is_pid_classic(pid) == 1))
    warning("Attempting to attach to a PPC process with an i386 "
            "native gdb - attach will not succeed.");

  macosx_create_inferior_for_task(macosx_status, itask, pid);

  macosx_exception_thread_create(&macosx_status->exception_status,
                                 macosx_status->task);

  if (inferior_ptrace_on_attach_flag)
    {
#ifdef PTRACE_ATTACHEXC
      ret = call_ptrace(PTRACE_ATTACHEXC, pid, 0, 0);
      if (ret != 0)
        {
          macosx_inferior_destroy(macosx_status);
          if (errno == EPERM)
            {
              error("Unable to attach to process-id %d: %s (%d).\n"
                    "This request requires that the target process be neither setuid nor "
                    "setgid and have the same real userid as the debugger, or that the "
                    "debugger be running with administrator privileges.",
                    pid, safe_strerror(errno), errno);
            }
          else
            {
              error("Unable to attach to process-id %d: %s (%d)",
                    pid, safe_strerror(errno), errno);
            }
        }
#else
      error(_("PTRACE_ATTACHEXC not defined for this configuration."));
#endif /* PTRACE_ATTACHEXC */

      macosx_status->attached_in_ptrace = 1;
      macosx_status->stopped_in_ptrace = 0;
      macosx_status->stopped_in_softexc = 0;

      macosx_status->suspend_count = 0;
    }
  else if (inferior_bind_exception_port_flag)
    {
      kret = macosx_inferior_suspend_mach(macosx_status);
      if (kret != KERN_SUCCESS)
        {
          macosx_inferior_destroy(macosx_status);
          MACH_CHECK_ERROR(kret);
        }
    }

  macosx_check_new_threads(NULL, 0);

  inferior_ptid = ptid_build(pid, 0, macosx_status->last_thread);
  attach_flag = 1;

  push_target(&macosx_child_ops);

  if (macosx_status->attached_in_ptrace)
    {
      enum macosx_source_type source;
      unsigned char buf[1024];

      /* read attach notification. This is a little gross, as it
       * turns out. We call ptrace with PT_ATTACHEXC and we get the
       * EXC_SOFT_SIGNAL for the SIGSTOP, but for some reason I do NOT
       * quite understand, we ALSO get a SIGSTOP from the waitpid in
       * the signal thread. This happens even though we have NOT
       * replied to the exception yet, or restarted the target.
       *
       * HOWEVER, when we attach to a program that has been stopped
       * under CrashReporter, we do NOT get the waitpid SIGSTOP. So we
       * cannot do a blocking wait for it or we will never return in
       * the CrashReporter case. Instead, we just poll the event
       * threads and see if anything has come in. If it is/was the
       * SIGSTOP, we silently discard it.
       *
       * Finally... We have to make sure the signal thread has a
       * chance to run through the first waitpid call or we will pick
       * up the SIGSTOP next time we look for events (when we go to
       * continue.) To do that we call macosx_fetch_event for the
       * signal thread with a fairly long (.1 sec) timeout.  */

      /* So this wait - which has only the exception thread running,
       * will get us the EXC_SOFT_SIGNAL,SIGSTOP.  */
      stop_soon = STOP_QUIETLY;
      wait_for_inferior ();

      /* Now we need to start up the signal thread, wait to give it a
       * chance to start up, then we will snag the signal & discard it: */

      macosx_signal_thread_create(&macosx_status->signal_status,
                                  macosx_status->pid);

      source = macosx_fetch_event(macosx_status, buf, sizeof(buf),
				  NEXT_SOURCE_SIGNAL, 100000);

      if (source == NEXT_SOURCE_SIGNAL)
	{
	  unsigned int signo;

	  macosx_signal_thread_message *msg
	    = (macosx_signal_thread_message *)buf;
	  signo = target_signal_from_host(WSTOPSIG(msg->status));
	  if (signo != SIGSTOP)
	    {
              /* If we are attaching to a crashed program - this
               * happens when Xcode catches the crash, for instance -
               * we will pick up the crash signal rather than a SIGSTOP: */
	      inferior_debug(2, "Attach returned signal: %d\n", signo);
	    }
	  else
	    inferior_debug(2, "Got SIGSTOP from signal thread.");
	}
      else if (source == NEXT_SOURCE_NONE)
	{
	  inferior_debug(2, "No signal event on stop after attach.");
	}
    }

  if (inferior_auto_start_dyld_flag)
    {
      macosx_solib_add(NULL, 0, NULL, 0);
    }

  /* I do NOT have any good way to know whether the malloc library
     has been initialized yet. But I am going to guess that we are
     unlikely to be able to attach BEFORE then...  */
  /* BUT sometimes we get a process that has been stopped at the
     first instruction when launched so we can attach to it.  In
     that case, we know that malloc has NOT been inited. We had
     better not set malloc inited in that case, or somebody will
     try to call a function that does malloc, and we will corrupt
     the target.

     Note, we do NOT know what the "first instruction is", we are just
     relying on the fact that it is currently _dyld_start. Yecch...
     But I cannot think of anything better to do.  */
  {
    int result;
    const char *name;
    CORE_ADDR addr;

    result = find_pc_partial_function_no_inlined(stop_pc, &name, &addr, NULL);
    if (result != 0)
      {
	char *decorated_dyld_start;
	size_t dds_len = (strlen("_dyld_start") + strlen(dyld_symbols_prefix)
			  + 2UL);
	decorated_dyld_start = (char *)xmalloc(dds_len);
	snprintf(decorated_dyld_start, dds_len, "%s_dyld_start",
		 dyld_symbols_prefix);
	/* I also check to make sure we are not too far away from
	   _dyld_start, in case dyld gets stripped and there are a
	   bunch of functions after dyld_start that do NOT have
	   symbols.  */
	if ((strcmp(name, decorated_dyld_start) != 0)
	    || ((stop_pc - addr) > 30))
	  {
	    macosx_set_malloc_inited(1);
	  }
	xfree(decorated_dyld_start);
      }
    else
      macosx_set_malloc_inited(1);
  }
}

/* FIXME: needs comment: */
static void
macosx_child_detach(const char *args ATTRIBUTE_UNUSED, int from_tty)
{
  kern_return_t kret;

  CHECK_FATAL(macosx_status != NULL);

  if (ptid_equal(inferior_ptid, null_ptid))
    {
      return;
    }

  if (from_tty)
    {
      char *exec_file = get_exec_file (0);
      if (exec_file)
        printf_filtered ("Detaching from program: `%s', %s.\n",
                         exec_file, target_pid_to_str (inferior_ptid));
      else
        printf_filtered ("Detaching from %s.\n",
                         target_pid_to_str (inferior_ptid));
    }

  if (!macosx_inferior_valid (macosx_status))
    {
      target_mourn_inferior ();
      return;
    }

  macosx_inferior_check_stopped (macosx_status);
  CHECK (macosx_inferior_valid (macosx_status));

  if (macosx_status->attached_in_ptrace
      && (!macosx_status->stopped_in_ptrace)
      && (!macosx_status->stopped_in_softexc))
    {
      macosx_inferior_suspend_ptrace (macosx_status);
      CHECK_FATAL (macosx_status->stopped_in_ptrace
                   || macosx_status->stopped_in_softexc);
    }

  if (inferior_bind_exception_port_flag)
    {
      kret =
        macosx_restore_exception_ports (macosx_status->task,
                                        &macosx_status->exception_status.
                                        saved_exceptions);
      MACH_CHECK_ERROR (kret);
    }

  if (macosx_status->attached_in_ptrace)
    {
      macosx_inferior_resume_ptrace (macosx_status, 0, 0, PTRACE_DETACH);
    }

  if (!macosx_inferior_valid (macosx_status))
    {
      target_mourn_inferior ();
      return;
    }

  macosx_inferior_suspend_mach (macosx_status);

  if (!macosx_inferior_valid (macosx_status))
    {
      target_mourn_inferior ();
      return;
    }

  prepare_threads_before_run (macosx_status, 0, THREAD_NULL, 0);
  macosx_inferior_resume_mach (macosx_status, -1);

  target_mourn_inferior ();
  return;
}

static int
macosx_kill_inferior(void *arg)
{
  kern_return_t *errval = (kern_return_t *)arg;
  struct target_waitstatus wait_status;

  CHECK_FATAL(macosx_status != NULL);
  *errval = KERN_SUCCESS;

  if (ptid_equal(inferior_ptid, null_ptid))
    {
      return 1;
    }

  if (!macosx_inferior_valid(macosx_status))
    {
      target_mourn_inferior();
      return 1;
    }

  macosx_inferior_check_stopped(macosx_status);
  CHECK(macosx_inferior_valid(macosx_status));

  if (macosx_status->attached_in_ptrace
      && (!macosx_status->stopped_in_ptrace)
      && (!macosx_status->stopped_in_softexc))
    {
      macosx_inferior_suspend_ptrace(macosx_status);
      CHECK_FATAL((macosx_status->stopped_in_ptrace
                   || macosx_status->stopped_in_softexc));
    }

  macosx_inferior_suspend_mach(macosx_status);
  prepare_threads_before_run(macosx_status, 0, THREAD_NULL, 0);

  if (macosx_status->attached_in_ptrace)
    {
      CHECK_FATAL(macosx_status->stopped_in_ptrace
                  || macosx_status->stopped_in_softexc);
      if (call_ptrace(PTRACE_KILL, macosx_status->pid, 0, 0) != 0)
        {
          error("macosx_child_detach: ptrace (%d, %d, %d, %d): %s",
                PTRACE_KILL, macosx_status->pid, 0, 0, safe_strerror(errno));
        }
      macosx_status->stopped_in_ptrace = 0;
      macosx_status->stopped_in_softexc = 0;
    }

  if (!macosx_inferior_valid(macosx_status))
    {
      target_mourn_inferior();
      return 1;
    }

  remove_breakpoints();
  macosx_inferior_resume_mach(macosx_status, -1);

  inferior_debug(2, "Sent PT_KILL, resumed target, start to wait for it to die.\n");

  while (1)
    {
      volatile int signo;
      struct gdb_exception e;
      ptid_t stopped_ptid;

      sched_yield();

      wait_status.kind = TARGET_WAITKIND_SPURIOUS;

      stopped_ptid = macosx_wait(macosx_status, &wait_status, NULL);

      if (wait_status.kind == TARGET_WAITKIND_EXITED)
	{
	  inferior_debug(2, "Process exited.\n");
	  break;
	}
      else if (wait_status.kind == TARGET_WAITKIND_SIGNALLED)
	{
	  inferior_debug(2, "Process signalled - signo: %d\n", wait_status.value.sig);
	  break;
	}
      else if (wait_status.kind == TARGET_WAITKIND_STOPPED)
	{
	  inferior_debug(2, "Process stopped - signo: %d\n", wait_status.value.sig);
	  signo = wait_status.value.sig;
	}
      else
	{
	  inferior_debug(2, "Process stopped - reason: %d", wait_status.kind);
	  signo = 0;
	}

      TRY_CATCH(e, RETURN_MASK_ERROR)
	{
	  macosx_child_resume(stopped_ptid, 0, (enum target_signal)signo);
	}
      if (e.reason == RETURN_ERROR)
	{
	  warning("Got an error resuming target to kill it: %s.", e.message);
	  break;
	}
    }

  target_executing = 0;
  target_mourn_inferior();
  return 1;
}

static void
macosx_kill_inferior_safe(void)
{
  kern_return_t kret;
  int ret;

  /* FIXME: I have actually caught errors with this when debugging: */
  ret = catch_errors(macosx_kill_inferior, &kret,
                     "error while killing target (killing anyway): ",
                     RETURN_MASK_ALL);

  if (ret == 0)
    {
      kret = task_terminate(macosx_status->task);
      MACH_WARN_ERROR(kret);
      sched_yield();
      target_mourn_inferior();
    }
}

/* */
static ATTRIBUTE_USED void
macosx_ptrace_me(void)
{
  sem_t *sem;
  char sem_name[64];

  restore_file_rlimit();

  /* This is the child side of the semaphore that I use to make sure
     the exception thread has started up before I exec.  The child
     side gets to create it, and also destroys it.  */

  snprintf(sem_name, 63, "gdb-%d", (int)getpid());
  sem = sem_open(sem_name, (O_CREAT | O_EXCL), 0644, 0);
  if (sem == (sem_t *)SEM_FAILED)
    {
      perror("Failed to create semaphore in fork.\n");
      exit(0);
    }
  else
    {
      int retval;
      errno = 0;

    try_again:
      retval = sem_wait(sem);
      if (retval == -1)
	{
	  if (errno == EINTR)
	    {
	      inferior_debug(2, "Interrupted waiting on semaphore in child.\n");
	      goto try_again;
	    }
	  else
	    {
	      perror("Failed to wait on semaphore in child.");
	      sem_close(sem);
	      sem_unlink(sem_name);
	      exit(0);
	    }
	}
      inferior_debug(1, "Got out of sem_wait on child side.\n");
      sem_close(sem);
      sem_unlink(sem_name);
    }

  call_ptrace(PTRACE_TRACEME, 0, 0, 0);
#ifdef PTRACE_SIGEXC
  call_ptrace(PTRACE_SIGEXC, 0, 0, 0);
#else
  warning(_("PTRACE_SIGEXC not defined for this configuration."));
#endif /* PTRACE_SIGEXC */
}

static void
post_to_semaphore(void *input)
{
  pid_t pid = (pid_t)(intptr_t)input;
  sem_t *sem;
  char sem_name[64];

  snprintf(sem_name, 63, "gdb-%d", (int)pid);
  while (1)
    {
      sem = sem_open(sem_name, 0);
      /* If the semaphore has NOT been created on the child side of the
       * fork yet, sleep a teeny bit and then try again.  */

      if (sem == (sem_t *)SEM_FAILED)
	{
	  inferior_debug(2, "Waiting for the child to create the semaphore.\n");
	  usleep(10);
	}
      else
	break;
    }
  inferior_debug(2, "Posting to semaphore on gdb side.\n");
  sem_post(sem);
  sem_close(sem);
}

/* */
static ATTRIBUTE_USED void
macosx_ptrace_him(int pid)
{
  task_t itask;
  kern_return_t kret;
  int traps_expected;
  struct cleanup *sem_cleanup;

  CHECK_FATAL(!macosx_status->attached_in_ptrace);
  CHECK_FATAL(!macosx_status->stopped_in_ptrace);
  CHECK_FATAL(!macosx_status->stopped_in_softexc);
  CHECK_FATAL(macosx_status->suspend_count == 0);

  /* I have to make sure that the exception thread is up & waiting
     before I let the child exec. I do this with this little
     semaphore. I do it in a cleanup so I will NOT leave the fork
     side hanging if I run into an error here.  */

  sem_cleanup = make_cleanup(post_to_semaphore,
                             (void *)(intptr_t)pid);

  kret = task_for_pid(mach_task_self(), pid, &itask);
  if (kret != KERN_SUCCESS)
    {
      if (macosx_get_task_for_pid_rights() == 1)
	kret = task_for_pid(mach_task_self(), pid, &itask);
    }

  {
    char buf[64];
    snprintf(buf, sizeof(buf), "%s=%d", "TASK", (int)itask);
    putenv(buf);
  }
  if (kret != KERN_SUCCESS)
    {
      error("Unable to find Mach task port for process-id %d: %s (0x%lx).",
            pid, MACH_ERROR_STRING(kret), (unsigned long)kret);
    }

  inferior_debug (2, "inferior task: 0x%08x, pid: %d\n", itask, pid);

  push_target (&macosx_child_ops);
  macosx_create_inferior_for_task (macosx_status, itask, pid);

  macosx_signal_thread_create (&macosx_status->signal_status,
                               macosx_status->pid);
  macosx_exception_thread_create (&macosx_status->exception_status,
                                  macosx_status->task);

  macosx_status->attached_in_ptrace = 1;
  macosx_status->stopped_in_ptrace = 0;
  macosx_status->stopped_in_softexc = 0;

  macosx_status->suspend_count = 0;

  /* This set how many traps we expect before we get to the
     PT_TRACEME trap we really care about. When using just
     "exec" in the shell, we will get one for the shell exec'ing,
     and one for the exec command.  For "arch" there is one more
     for the "arch" command running.  */

#ifdef USE_ARCH_FOR_EXEC
  traps_expected = (start_with_shell_flag ? 3 : 1);
#else
  traps_expected = (start_with_shell_flag ? 2 : 1);
#endif /* USE_ARCH_FOR_EXEC */

  /* Okay, the exception & signal listeners are set up,
     now signal the child side that it can proceed.  */
  do_cleanups (sem_cleanup);

  startup_inferior (traps_expected);

  if (ptid_equal (inferior_ptid, null_ptid))
    {
      return;
    }

  if (!macosx_task_valid (macosx_status->task))
    {
      target_mourn_inferior ();
      return;
    }

  macosx_inferior_check_stopped (macosx_status);
  CHECK (macosx_inferior_valid (macosx_status));

  if (inferior_ptrace_flag)
    {
      CHECK_FATAL (macosx_status->attached_in_ptrace);
      CHECK_FATAL (macosx_status->stopped_in_ptrace
                   || macosx_status->stopped_in_softexc);
    }
  else
    {
      macosx_inferior_resume_ptrace (macosx_status, 0, 0, PTRACE_DETACH);
      CHECK_FATAL (!macosx_status->attached_in_ptrace);
      CHECK_FATAL (!macosx_status->stopped_in_ptrace);
      CHECK_FATAL (!macosx_status->stopped_in_softexc);
    }
  target_post_startup_inferior (pid_to_ptid (pid));
}

#if defined(TARGET_ARM)

/* On the phone task_for_pid() rights are marshalled done by the kernel: */
int ATTRIBUTE_CONST
macosx_get_task_for_pid_rights(void)
{
  return 1;
}

/* These comes from fork_child.c; un-nested for '-Wnested-externs': */
extern char *exec_pathname;
extern char *exec_argv0;

/* This version of macosx_child_create_inferior is needed to work
   around the fact that the task port does NOT persist across a
   fork/exec.  It would be nice if we could just switch to the new
   task after the exec, but that is currently buggy, and the
   PT_THUPDATE stops working.  So instead, this code just lets
   posix_spawn launch the app (with the POSIX_SPAWN_START_SUSPENDED
   argument telling the target to stop on the first instruction.)
   Then we attach, and continue.
   FIXME: This version silently ignores the "start-with-shell" setting.  */

static void
macosx_child_create_inferior(char *exec_file, char *allargs, char **env,
			     int from_tty)
{
  pid_t new_pid;
  char pid_str[32];
  static int debug_setpgrp = 657473;
  unsigned int i;
  char **argt = NULL;
  static char **argv;
  char *fileptr;
  posix_spawnattr_t attr;
  int retval;
  cpu_type_t cpu = 0;
  const char *osabi_name = gdbarch_osabi_name(gdbarch_osabi(current_gdbarch));
  struct gdb_exception e;
  int ps_flags;

  if (osabi_name == NULL) {
    warning(_("Null osabi name for current architecture."));
  }

  /* If no exec file handed to us, get it from the exec-file command
     -- with a good, common error message if none is specified.  */
  if (exec_file == 0)
    exec_file = get_exec_file(1);

  if (exec_pathname[0] != '\0')
    fileptr = exec_pathname;
  else
    fileptr = exec_file;

  argt = buildargv(allargs);
  if (argt == NULL)
    {
      error("unable to build argument vector for inferior process (out of memory)");
    }

  if ((allargs == NULL) || (allargs[0] == '\0'))
    argt[0] = NULL;
  for (i = 0; argt[i] != NULL; i++);
  argv = (char **)xmalloc((i + 1UL + 1UL) * (sizeof(*argt)));
  argv[0] = exec_file;
  if (exec_argv0[0] != '\0')
    argv[0] = exec_argv0;
  memcpy(&argv[1], argt, (i + 1UL) * sizeof(*argt));
# if 0
  freeargv(argt);
# endif /* 0 */

# if defined(TARGET_POWERPC)
  if (strcmp(osabi_name, "Darwin") == 0)
    {
      cpu = CPU_TYPE_POWERPC;
    }
  else if (strcmp(osabi_name, "Darwin64") == 0)
    {
      cpu = CPU_TYPE_POWERPC64;
    }
# elif defined(TARGET_I386)
  if (strcmp(osabi_name, "Darwin") == 0)
    {
      cpu = CPU_TYPE_I386;
    }
  else if (strcmp(osabi_name, "Darwin64") == 0)
    {
      cpu = CPU_TYPE_X86_64;
    }
# elif defined(TARGET_ARM)

#  if 0
  /* Do NOT set the CPU type for ARM as we currently cannot reliably set
     the CPU type with posix_spawnattr_setbinpref_np as it will pick the
     first CPU type that matches CPU_TYPE_ARM which is not necessarily the
     one that we want to run. If we ever want to use
     posix_spawnattr_setbinpref_np for ARM, we will also need a way to set
     the CPU subtype.  */
  if (strcmp(osabi_name, "Darwin") == 0)
    {
      cpu = CPU_TYPE_ARM;
    }
  else if (strcmp(osabi_name, "DarwinV6") == 0)
    {
      cpu = CPU_TYPE_ARM;
    }
  else if (strcmp(osabi_name, "DarwinV7") == 0)
    {
      cpu = CPU_TYPE_ARM;
    }
#  endif /* 0 */
# endif /* TARGET_(architecture) */

  retval = posix_spawnattr_init(&attr);
  if (retval != 0)
    error("Failed to initialize attributes for posix_spawn, error: %d", retval);

  ps_flags = 0;

# ifndef _POSIX_SPAWN_DISABLE_ASLR
#  define _POSIX_SPAWN_DISABLE_ASLR 0x0100
# endif /* !_POSIX_SPAWN_DISABLE_ASLR */

  ps_flags = POSIX_SPAWN_START_SUSPENDED;
  if (disable_aslr_flag) {
    ps_flags |= _POSIX_SPAWN_DISABLE_ASLR;
  }

  retval = posix_spawnattr_setflags(&attr, ps_flags);
  if (retval != 0) {
    error("Could not add _POSIX_SPAWN_DISABLE_ASLR and POSIX_SPAWN_START_SUSPENDED to posix_spawn flags, got return value %d", retval);
  }

  if (cpu != 0)
    {
      size_t copied = 0UL;
      retval = posix_spawnattr_setbinpref_np(&attr, 1, &cpu, &copied);
      if ((retval != 0) || (copied != 1))
        error("Could NOT set the binary preferences, error: %d", retval);
    }
  retval = posix_spawnattr_setpgroup(&attr, debug_setpgrp);
  if (retval != 0)
    error("Could NOT set the process group, error: %d", retval);

  retval = posix_spawnp(&new_pid, fileptr, NULL,  &attr, argv, env);

  snprintf(pid_str, 31, "%d", new_pid);

  announce_attach = 0;
  TRY_CATCH(e, RETURN_MASK_ERROR)
    {
      macosx_child_attach(pid_str, from_tty);
    }
  announce_attach = 1;
  /* Clear the attach flag that was set by calling macosx_child_attach() so
     if/when gdb is quit it will NOT keep the child process around and
     running.  */
  attach_flag = 0;

  if (e.reason != (int)NO_ERROR)
    throw_exception(e);

  if (target_can_async_p())
    target_async(inferior_event_handler, 0);

  clear_proceed_status();
  proceed((CORE_ADDR)(-1L), TARGET_SIGNAL_0, 0);
}
#else /* #if defined(TARGET_ARM)  */

# ifndef S_SPLINT_S
/* FIXME: <Security/Authorization.h> should be updated to say:
enum { kAuthorizationExternalFormLength = 32 };
 ...instead of the current:
static const size_t kAuthorizationExternalFormLength = 32;
See:
 - https://github.com/cooljeanius/apple-gdb-1824/issues/8
 - https://gcc.gnu.org/bugzilla/show_bug.cgi?id=93082
 - rdar://FB8919799
 */
#  include <Security/Security.h>
# else
#  if !defined(__MACTYPES__) && !defined(__TYPES__)
#   if defined(__LP64__) && __LP64__
typedef unsigned int UInt32;
typedef signed int SInt32;
#   else
typedef unsigned long UInt32;
typedef signed long SInt32;
#   endif /* __LP64__ */
typedef SInt32 OSStatus;
#  endif /* !__MACTYPES__ && !__TYPES__ */
typedef const char *AuthorizationString;
typedef struct {
  AuthorizationString name;
  size_t valueLength;
  void *value;
  UInt32 flags;
} AuthorizationItem;
typedef struct {
  UInt32 count;
  AuthorizationItem *items;
} AuthorizationItemSet;
typedef AuthorizationItemSet AuthorizationRights;
typedef AuthorizationItemSet AuthorizationEnvironment;
typedef const void *AuthorizationRef;
typedef UInt32 AuthorizationFlags;
# endif /* !S_SPLINT_S */

int
macosx_get_task_for_pid_rights(void)
{
  OSStatus stat;
  AuthorizationItem taskport_item[] = {
    { "system.privilege.taskport.debug", 0UL, NULL, 0U }
  };
  AuthorizationRights rights = { 1, taskport_item }, *out_rights = NULL;
  AuthorizationRef author;
  int retval = 0;

  AuthorizationFlags auth_flags;
  auth_flags = (kAuthorizationFlagExtendRights
                | kAuthorizationFlagPreAuthorize
                | kAuthorizationFlagInteractionAllowed
                | (1 << 5) /* kAuthorizationFlagLeastPrivileged */);

  stat = AuthorizationCreate(NULL, kAuthorizationEmptyEnvironment,
			     auth_flags,
			     &author);
  if (stat != errAuthorizationSuccess)
    return 0;

  /* If you have a window server connection, then this call will put
     up a dialog box if it can.  However, if the current user does NOT
     have a connection to the window server (for instance if they are
     in an ssh session) then this call will return
     errAuthorizationInteractionNotAllowed.
     I want to do this way first, however, since I would prefer the dialog
     box - for instance if I am running under Xcode - to trying to prompt.  */

  stat = AuthorizationCopyRights(author, &rights,
				 kAuthorizationEmptyEnvironment,
				 auth_flags, &out_rights);
  if (stat == errAuthorizationSuccess)
    {
      retval = 1;
      goto cleanup;
    }
  else if (stat == errAuthorizationInteractionNotAllowed)
    {
      /* Okay, so the straight call could NOT query, so we are going to
         have to get the username & password and send them by hand to
         AuthorizationCopyRights.  */
      char *pass;
      char *login_name;
      char entered_login[256];
      AuthorizationItem auth_items[] = {
	{ kAuthorizationEnvironmentUsername, 0UL, NULL, 0U },
	{ kAuthorizationEnvironmentPassword, 0UL, NULL, 0U },
	{ kAuthorizationEnvironmentShared, 0UL, NULL, 0U }
      };
      AuthorizationEnvironment env = { 3, auth_items };
      /* However, if we are running under the mi, I cannot do hidden
       * password input, so I return failure instead: */
      if (ui_out_is_mi_like_p(uiout))
	{
	  struct cleanup *notify_cleanup;
	  notify_cleanup
	    = make_cleanup_ui_out_notify_begin_end(uiout,
						   "task_for_pid-failure");
	  do_cleanups(notify_cleanup);
	  return 0;
	}

      login_name = getlogin();
      if (! login_name )
	return 0;

      fprintf_unfiltered(gdb_stdout, "We need authorization from an admin user to run the debugger.\n");
      fprintf_unfiltered(gdb_stdout, "This will only happen once per login session.\n");
      fprintf_unfiltered(gdb_stdout, "Admin username (%s): ", login_name);
      fgets(entered_login, 255, stdin);
      if (entered_login[0] != '\n')
	{
	  entered_login[strlen(entered_login) - 1] = '\0';
	  login_name = entered_login;
	}
      pass = getpass("Password:");
      if (!pass)
	return 0;

      auth_items[0].valueLength = strlen(login_name);
      auth_items[0].value = login_name;
      auth_items[1].valueLength = strlen(pass);
      auth_items[1].value = pass;

      /* If we got rights in the AuthorizationCopyRights call above, then
       * free it before we reuse the pointer: */
      if (out_rights != NULL)
	AuthorizationFreeItemSet(out_rights);

      stat = AuthorizationCopyRights(author, &rights, &env, auth_flags,
				     &out_rights);

      memset(pass, 0, strlen(pass));
      if (stat == errAuthorizationSuccess)
	retval = 1;
      else
	retval = 0;
    }

 cleanup:
  if (out_rights != NULL)
    AuthorizationFreeItemSet(out_rights);
  AuthorizationFree(author, kAuthorizationFlagDefaults);

  return retval;
}


static void
macosx_child_create_inferior(char *exec_file, char *allargs, char **env,
			     int from_tty ATTRIBUTE_UNUSED)
{
  if ((exec_bfd != NULL) &&
      ((exec_bfd->xvec->flavour == bfd_target_pef_flavour)
       || (exec_bfd->xvec->flavour == bfd_target_pef_xlib_flavour)))
    {
      error("Cannot run PEF binary; use LaunchCFMApp as the executable file.");
    }

  /* It is not safe to call functions in the target until we have initialized
     the libsystem malloc package. So for now, mark it unsafe.  */
  macosx_set_malloc_inited(0);

  fork_inferior(exec_file, allargs, env, macosx_ptrace_me, macosx_ptrace_him,
                NULL, NULL);
  if (ptid_equal(inferior_ptid, null_ptid))
    return;

  macosx_dyld_create_inferior_hook((CORE_ADDR)-1);

  attach_flag = 0;

  if (target_can_async_p())
    target_async(inferior_event_handler, 0);

  clear_proceed_status();
  proceed((CORE_ADDR)-1, TARGET_SIGNAL_0, 0);
}
#endif /* TARGET_ARM (way up, before the previous one) */

static void
macosx_child_files_info(struct target_ops *ops ATTRIBUTE_UNUSED)
{
  CHECK_FATAL(macosx_status != NULL);
  macosx_debug_inferior_status(macosx_status);
}

/* Return an ascii string showing the name of the thread, if any is set.
   An empty string is returned if there is no name.
   The returned char* points into to a static buffer that will be
   reused on subsequent calls.  */

static char *
macosx_get_thread_name(ptid_t ptid)
{
#if defined(THREAD_IDENTIFIER_INFO) && defined(THREAD_IDENTIFIER_INFO_COUNT)
  static char buf[128];
  pid_t pid = ptid_get_pid(ptid);
  thread_t tid = (thread_t)ptid_get_tid(ptid);
  struct thread_info *tp;

  thread_identifier_info_data_t tident;
  unsigned int info_count;
  kern_return_t kret;
  struct proc_threadinfo pth;
  int retval;

  buf[0] = '\0';
  tp = find_thread_pid(ptid);
  if ((tp->privatedata == NULL) || (tp->privatedata->app_thread_port == 0))
    return NULL;

  info_count = THREAD_IDENTIFIER_INFO_COUNT;
  kret = thread_info(tid, THREAD_IDENTIFIER_INFO, (thread_info_t)&tident,
                     &info_count);
  MACH_CHECK_ERROR(kret);
  retval = proc_pidinfo(pid, PROC_PIDTHREADINFO, tident.thread_handle,
                        &pth, sizeof(pth));
  if ((retval != 0) && (pth.pth_name[0] != '\0'))
    {
      strlcpy(buf, pth.pth_name, sizeof(buf));
    }
  else
    {
      if (tident.thread_handle)
        {
          char *queue_name =
	    get_dispatch_queue_name((CORE_ADDR)tident.dispatch_qaddr);
          if (queue_name && (queue_name[0] != '\0'))
            {
              strlcpy(buf, queue_name, sizeof(buf));
            }
        }
    }
  return buf;
#else
  return (char *)"";
#endif /* THREAD_IDENTIFIER_INFO && THREAD_IDENTIFIER_INFO_COUNT */
}


/* Return an ascii string showing the Mach port # of a given thread
   The returned char* points into to a static buffer that will be
   reused on subsequent calls.  */

static char *
macosx_get_thread_id_str(ptid_t ptid)
{
  static char buf[128];
  struct thread_info *tp;

  tp = find_thread_pid(ptid);
  if ((tp->privatedata == NULL) || (tp->privatedata->app_thread_port == 0))
    {
      thread_t thread = (thread_t)ptid_get_tid(ptid);
      snprintf(buf, sizeof(buf), "local thread 0x%lx", (unsigned long)thread);
      return buf;
    }

  snprintf(buf, sizeof(buf), "port# 0x%s",
	   paddr_nz(tp->privatedata->app_thread_port));

  return buf;
}

/* FIXME: needs comment */
static int
macosx_child_thread_alive(ptid_t ptid)
{
  return macosx_thread_valid(macosx_status->task, (thread_t)ptid_get_tid(ptid));
}

/* FIXME: needs comment */
void
macosx_create_inferior_for_task(struct macosx_inferior_status *inferior,
                                task_t task, int pid)
{
  CHECK_FATAL(inferior != NULL);

  macosx_inferior_destroy(inferior);
  macosx_inferior_reset(inferior);

  inferior->task = task;
  inferior->pid = pid;

  inferior->attached_in_ptrace = 0;
  inferior->stopped_in_ptrace = 0;
  inferior->stopped_in_softexc = 0;

  inferior->suspend_count = 0;

  inferior->last_thread = macosx_primary_thread_of_task(inferior->task);
}

static int remote_async_terminal_ours_p = 1;
static void (*ofunc)(int);
static PTR sigint_remote_twice_token;
static PTR sigint_remote_token;

static void remote_interrupt_twice(int signo);
static void remote_interrupt(int signo);
static void handle_remote_sigint_twice(int sig);
static void handle_remote_sigint(int sig);
static void async_remote_interrupt_twice(gdb_client_data arg);
static void async_remote_interrupt(gdb_client_data arg);

static void
interrupt_query(void)
{
  target_terminal_ours();

  if (query("Interrupted while waiting for the program.\n\
Give up (and stop debugging it)? "))
    {
      target_mourn_inferior();
      deprecated_throw_reason(RETURN_QUIT);
    }

  target_terminal_inferior();
}

static void
remote_interrupt_twice(int signo)
{
  signal(signo, ofunc);
  interrupt_query();
  signal(signo, remote_interrupt);
}

static void
remote_interrupt(int signo)
{
  signal(signo, remote_interrupt_twice);
  target_stop();
}

static void
handle_remote_sigint_twice(int sig)
{
  signal(sig, handle_sigint);
  sigint_remote_twice_token =
    create_async_signal_handler(inferior_event_handler_wrapper, NULL);
  mark_async_signal_handler_wrapper(sigint_remote_twice_token);
}

static void
handle_remote_sigint(int sig)
{
  signal(sig, handle_remote_sigint_twice);
  sigint_remote_twice_token =
    create_async_signal_handler(async_remote_interrupt_twice, NULL);
  mark_async_signal_handler_wrapper(sigint_remote_token);
}

static void
async_remote_interrupt_twice(gdb_client_data arg ATTRIBUTE_UNUSED)
{
  if (target_executing)
    {
      interrupt_query();
      signal(SIGINT, handle_remote_sigint);
    }
}

static void
async_remote_interrupt(gdb_client_data arg ATTRIBUTE_UNUSED)
{
  target_stop();
}

static void
cleanup_sigint_signal_handler(void *dummy ATTRIBUTE_UNUSED)
{
  signal(SIGINT, handle_sigint);
  if (sigint_remote_twice_token)
    delete_async_signal_handler((struct async_signal_handler **)
                                &sigint_remote_twice_token);
  if (sigint_remote_token)
    delete_async_signal_handler((struct async_signal_handler **)
                                &sigint_remote_token);
}

static void
initialize_sigint_signal_handler(void)
{
  sigint_remote_token =
    create_async_signal_handler(async_remote_interrupt, NULL);
  signal(SIGINT, handle_remote_sigint);
}

static void
macosx_terminal_inferior(void)
{
  terminal_inferior();

  if (!sync_execution)
    return;
  if (!remote_async_terminal_ours_p)
    return;
  CHECK_FATAL(sync_execution);
  CHECK_FATAL(remote_async_terminal_ours_p);
  delete_file_handler(input_fd);
  remote_async_terminal_ours_p = 0;
  initialize_sigint_signal_handler();
}

static void
macosx_terminal_ours(void)
{
  terminal_ours();

  if (!sync_execution)
    return;
  if (remote_async_terminal_ours_p)
    return;
  CHECK_FATAL(sync_execution);
  CHECK_FATAL(!remote_async_terminal_ours_p);
  cleanup_sigint_signal_handler(NULL);

  add_file_handler(input_fd, stdin_event_handler, 0);

  remote_async_terminal_ours_p = 1;
}

static void
macosx_file_handler(int error ATTRIBUTE_UNUSED,
                    gdb_client_data client_data ATTRIBUTE_UNUSED)
{
  async_client_callback(INF_REG_EVENT, async_client_context);
}

static void
macosx_async(void (*callback)(enum inferior_event_type event_type,
                              void *context), void *context)
{
  if (current_target.to_async_mask_value == 0)
    internal_error(__FILE__, __LINE__,
                   "Calling remote_async when async is masked");

  if (callback != NULL)
    {
      async_client_callback = callback;
      async_client_context = context;
      if (macosx_status->exception_status.error_receive_fd > 0)
        add_file_handler(macosx_status->exception_status.error_receive_fd,
                         macosx_file_handler, NULL);
      if (macosx_status->exception_status.receive_from_fd > 0)
        add_file_handler(macosx_status->exception_status.receive_from_fd,
                         macosx_file_handler, NULL);
      if (macosx_status->signal_status.receive_fd > 0)
        add_file_handler(macosx_status->signal_status.receive_fd,
                         macosx_file_handler, NULL);
    }
  else
    {
      if (macosx_status->exception_status.error_receive_fd > 0)
        delete_file_handler(macosx_status->exception_status.error_receive_fd);
      if (macosx_status->exception_status.receive_from_fd > 0)
        delete_file_handler(macosx_status->exception_status.receive_from_fd);
      if (macosx_status->signal_status.receive_fd > 0)
        delete_file_handler(macosx_status->signal_status.receive_fd);
    }
}

#if defined(GDB_RC_VERSION) && (GDB_RC_VERSION < 1461)
/* This flag tells us whether we have determined that malloc
   is unsafe since the last time we stopped (excepting hand_call_functions.)
   -1 means we haven't checked yet.
   0 means it is safe
   1 means it is unsafe.
   If you set this, be sure to add a hand_call_cleanup to restore it.  */

static int malloc_unsafe_flag = -1;

static void
do_reset_malloc_unsafe_flag(void *unused ATTRIBUTE_UNUSED)
{
  malloc_unsafe_flag = -1;
}

/* macosx_check_malloc_is_unsafe calls into LibC to see if the malloc lock is
 * taken by any thread.  It returns 1 if malloc is locked, 0 if malloc is
 * unlocked, and -1 if LibC doesn't support the malloc lock check function. */
static int
macosx_check_malloc_is_unsafe(void)
{
  static struct cached_value *malloc_check_fn = NULL;
  struct cleanup *scheduler_cleanup;
  struct value *volatile tmp_value;
  struct gdb_exception e;
  int success;

  if (malloc_unsafe_flag != -1)
    return malloc_unsafe_flag;

  if (malloc_check_fn == NULL)
    {
      if (lookup_minimal_symbol("malloc_gdb_po_unsafe", 0, 0))
        {
          struct type *func_type;
          func_type = builtin_type_int;
          func_type = lookup_function_type(func_type);
          func_type = lookup_pointer_type(func_type);
          malloc_check_fn = create_cached_function("malloc_gdb_po_unsafe",
						   func_type);
        }
      else
	return -1;
    }

  scheduler_cleanup =
    make_cleanup_set_restore_scheduler_locking_mode(scheduler_locking_on);
  /* Suppress the objc runtime mode checking here: */
  make_cleanup_set_restore_debugger_mode(NULL, -1);

  make_cleanup_set_restore_unwind_on_signal(1);

  TRY_CATCH (e, RETURN_MASK_ALL)
    {
      tmp_value = call_function_by_hand(lookup_cached_function(malloc_check_fn),
                                        0, NULL);
    }

  do_cleanups(scheduler_cleanup);

  /* If we got an error calling the malloc_check_fn, assume it is not
     safe to call... */
  /* FIXME: -Wenum-compare: */
  if (e.reason != NO_ERROR)
    return 1;

  success = value_as_long(tmp_value);
  if (success == 0 || success == 1)
    {
      malloc_unsafe_flag = success;
      make_hand_call_cleanup(do_reset_malloc_unsafe_flag, 0);
      return success;
    }
  else
    {
      warning("Got unexpected value from malloc_gdb_po_unsafe: %d.", success);
      return 1;
    }

  return -1;
}

/* This code implements the Mac OS X side of the safety checks given
   in target_check_safe_call.  The list of modules is defined in
   defs.h.  */

enum {
  MALLOC_SUBSYSTEM_INDEX = 0,
  LOADER_SUBSYSTEM_INDEX = 1,
  OBJC_SUBSYSTEM_INDEX = 2,
  SPINLOCK_SUBSYSTEM_INDEX = 3,
  LAST_SUBSYSTEM_INDEX = 4,
};

static const char *macosx_unsafe_regexes[] = {
"(^(m|c|re|v)?alloca*)|(::[^ ]*allocator)|(^szone_)",
"(^dlopen)|(^__dyld)|(^dyld)|(NSBundle load)|"
"(NSBundle unload)|(CFBundleLoad)|(CFBundleUnload)",
"(_class_lookup)|(^objc_lookUpClass)|(^look_up_class)",
"(^__spin_lock)|(^pthread_mutex_lock)|(^pthread_mutex_unlock)|(^__spin_unlock)"
};

/* This is the Mac OS X implementation of target_check_safe_call: */
int
macosx_check_safe_call(int which, enum check_which_threads thread_mode)
{
  int retval = 1;
  regex_t unsafe_patterns[LAST_SUBSYSTEM_INDEX];
  int num_unsafe_patterns = 0;
  int depth = 0;

  static regex_t macosx_unsafe_patterns[LAST_SUBSYSTEM_INDEX];
  static int patterns_initialized = 0;

  if (!patterns_initialized)
    {
      int i;
      patterns_initialized = 1;

      for (i = 0; i < LAST_SUBSYSTEM_INDEX; i++)
	{
	  int err_code;
	  err_code = regcomp(&(macosx_unsafe_patterns[i]),
			     macosx_unsafe_regexes[i],
			     (REG_EXTENDED | REG_NOSUB));
	  if (err_code != 0)
	    {
	      char err_str[512];
	      regerror(err_code, &(macosx_unsafe_patterns[i]),
                       err_str, 512);
	      internal_error(__FILE__, __LINE__,
			     "Failed to compile unsafe call pattern %s, error %s",
			     macosx_unsafe_regexes[i], err_str);
	    }
	}

    }

  /* Because check_safe_call will potentially scan all threads, which can be
     time consuming, we accumulate all the regexp patterns we are going to
     apply into UNSAFE_PATTERNS and pass them at one go to check_safe_call.  */

  if (which & MALLOC_SUBSYSTEM)
    {
      int malloc_unsafe;
      if (macosx_get_malloc_inited() == 0)
	{
	  ui_out_text(uiout, "Unsafe to run code: ");
	  ui_out_field_string(uiout, "problem", "malloc library is not initialized yet");
	  ui_out_text(uiout, ".\n");
	  return 0;
	}

      /* macosx_check_malloc_is_unsafe doesn't tell us about the current thread.
	 So if the caller has asked explicitly about the current thread only, try
	 the patterns.  */
      if (thread_mode == CHECK_CURRENT_THREAD)
	malloc_unsafe = -1;
      else
	malloc_unsafe = macosx_check_malloc_is_unsafe();

      if (malloc_unsafe == 1)
	{
	  ui_out_text(uiout, "Unsafe to run code: ");
	  ui_out_field_string(uiout, "problem", "malloc zone lock is held for some zone.");
	  ui_out_text(uiout, ".\n");
	  return 0;
	}
      else if (malloc_unsafe == -1)
	{
	  unsafe_patterns[num_unsafe_patterns] =
            macosx_unsafe_patterns[MALLOC_SUBSYSTEM_INDEX];
	  num_unsafe_patterns++;
	  if (depth < 5)
	    depth = 5;
	}
    }

  if (which & OBJC_SUBSYSTEM)
    {
      struct cleanup *runtime_cleanup;
      enum objc_debugger_mode_result objc_retval;

      /* Again, the debugger mode requires you only run the current thread.
       * If the caller requested information about the current thread, that
       * means she will be running the all threads - just with code on the
       * current thread.  So we should avoid the debugger mode: */
      if (thread_mode != CHECK_CURRENT_THREAD)
	{
	  objc_retval = make_cleanup_set_restore_debugger_mode(&runtime_cleanup, 0);
	  do_cleanups(runtime_cleanup);
	  if (objc_retval == objc_debugger_mode_success)
	    {
	      return 1;
	    }
	}

      if (thread_mode == CHECK_CURRENT_THREAD
	  || objc_retval == objc_debugger_mode_fail_objc_api_unavailable)
        {
          unsafe_patterns[num_unsafe_patterns] =
            macosx_unsafe_patterns[OBJC_SUBSYSTEM_INDEX];
          num_unsafe_patterns++;
          if (depth < 5)
            depth = 5;
        }
      else
        {
          ui_out_text(uiout, "Unsafe to run code: ");
          ui_out_field_string(uiout, "problem", "objc runtime lock is held");
          ui_out_text(uiout, ".\n");
          return 0;
        }
    }

  if (which & LOADER_SUBSYSTEM)
    {
      /* FIXME: There might be a better way to do this in SL: */
      struct minimal_symbol *dyld_lock_p;
      int got_it_easy = 0;
      dyld_lock_p = lookup_minimal_symbol("_dyld_global_lock_held", 0, 0);
      if (dyld_lock_p != NULL)
	{
	  ULONGEST locked;

	  if (safe_read_memory_unsigned_integer(SYMBOL_VALUE_ADDRESS(dyld_lock_p),
                                                4, &locked))
	    {
	      got_it_easy = 1;
	      if (locked == 1)
		return 0;
	    }
	}

      if (!got_it_easy)
	{
	  unsafe_patterns[num_unsafe_patterns] =
            macosx_unsafe_patterns[LOADER_SUBSYSTEM_INDEX];
	  num_unsafe_patterns++;
	  if (depth < 5)
	    depth = 5;
	}
    }

  if (which & SPINLOCK_SUBSYSTEM)
    {
      unsafe_patterns[num_unsafe_patterns] =
        macosx_unsafe_patterns[SPINLOCK_SUBSYSTEM_INDEX];
      num_unsafe_patterns++;
      if (depth < 1)
	depth = 1;
    }

  if (num_unsafe_patterns > 0)
    {
      retval = check_safe_call(unsafe_patterns, num_unsafe_patterns, depth,
                               thread_mode);
    }

  return retval;
}

/* */
void
macosx_print_extra_stop_info(int code, CORE_ADDR address)
{
  ui_out_text(uiout, "Reason: ");
  switch (code)
    {
    case KERN_PROTECTION_FAILURE:
      ui_out_field_string(uiout, "access-reason", "KERN_PROTECTION_FAILURE");
      break;
    case KERN_INVALID_ADDRESS:
      ui_out_field_string(uiout, "access-reason", "KERN_INVALID_ADDRESS");
      break;
    default:
      ui_out_field_int(uiout, "access-reason", code);
    }
  ui_out_text(uiout, " at address: ");
  ui_out_field_core_addr(uiout, "address", address);
  ui_out_text(uiout, "\n");
}
#endif /* (GDB_RC_VERSION < 1461) */

/* Info for a random fork (actually a random process). If/when fork-based
 * checkpoints are ever fully functional, this can go away.  */

/* static */ void
cpfork_info(const char *args, int from_tty ATTRIBUTE_UNUSED)
{
  task_t itask;
  int pid;
  int total = 0;

  if (args == NULL)
    {
      pid = ptid_get_pid(inferior_ptid);
    }

  macosx_lookup_task(args, &itask, &pid);
  if (itask == TASK_NULL)
    {
      error("unable to locate task with pid %d", pid);
      return;
    }

  {
    vm_address_t address = 0;
    vm_size_t size = 0;
    kern_return_t err = 0;

    while (1) {
      mach_msg_type_number_t count;
      struct vm_region_submap_info_64 info;
      natural_t nestingDepth;

      count = VM_REGION_SUBMAP_INFO_COUNT_64;
      err = vm_region_recurse_64(itask, &address, &size, &nestingDepth,
				 (vm_region_info_64_t)&info, &count);
      if (err == KERN_INVALID_ADDRESS) {
	break;
      } else if (err) {
	mach_error((char *)"vm_region", err);
	break; /* reached last region */
      }

      ++total;

      if (info.protection & VM_PROT_WRITE)
        {
          printf("addr 0x%s size 0x%s (",
                 paddr_nz(address), paddr_nz(size));
          {
            int rslt;
	    vm_offset_t mempointer;   /* local copy of inferior's memory */
	    mach_msg_type_number_t memcopied;     /* for vm_read to use */

	    rslt = mach_vm_read(itask, address, 16, &mempointer,
                                &memcopied);

	    if (rslt == KERN_SUCCESS)
	      {
		int i;

		for (i = 0; i < 16; ++i)
		  printf("%x ", ((unsigned char *)mempointer)[i]);
	      }
	  }
	  printf(")\n");
	}

      if (info.is_submap) { /* is it a submap? */
	nestingDepth++;
      } else {
	address = address+size;
      }
    }
  }
  printf("%d regions total.\n", total);
}

/* Checkpoint support.  */

/* Given a checkpoint, collect blocks of memory from the inferior and
   save them in GDB. This is very inefficient, and should only be used
   as a fallback.  */

void
direct_memcache_get(struct checkpoint *cp)
{
  task_t itask;
  int kret;
  int pid = cp->pid;
  vm_address_t address = 0;
  vm_size_t size = 0;
  kern_return_t err = 0;

  kret = task_for_pid(mach_task_self(), pid, &itask);
  if (kret != KERN_SUCCESS)
    {
      error("Unable to locate task for process-id %d: %s.", pid,
	    MACH_ERROR_STRING(kret));
    }
  if (itask == TASK_NULL)
    {
      error("unable to locate task");
      return;
    }

  while (1)
    {
      mach_msg_type_number_t count;
      struct vm_region_submap_info_64 info;
      natural_t nesting_depth;

      count = VM_REGION_SUBMAP_INFO_COUNT_64;
      err = vm_region_recurse_64(itask, &address, &size, &nesting_depth,
				 (vm_region_info_64_t)&info, &count);
      if (err == KERN_INVALID_ADDRESS)
	{
	  break;
	}
      else if (err)
	{
	  mach_error((char *)"vm_region", err);
	  break; /* reached last region */
	}

      if (info.protection & VM_PROT_WRITE)
	{
	  memcache_get(cp, address, (int)size);
	}

      if (info.is_submap)
	nesting_depth++;
      else
	address += size;
    }
}

/* Given a checkpoint, collect blocks of memory from the fork that is serving
   as its "backing store", and install them into the current inferior.  */

void
fork_memcache_put(struct checkpoint *cp)
{
  task_t itask;
  int kret;
  int pid = cp->pid;
  vm_address_t address = 0;
  vm_size_t size = 0;
  kern_return_t err = 0;

  kret = task_for_pid(mach_task_self(), pid, &itask);
  if (kret != KERN_SUCCESS)
    {
      error("Unable to locate task for process-id %d: %s.", pid,
	    MACH_ERROR_STRING(kret));
    }
  if (itask == TASK_NULL)
    {
      error("unable to locate task");
      return;
    }

  while (1)
    {
      mach_msg_type_number_t count;
      struct vm_region_submap_info_64 info;
      natural_t nesting_depth;

      count = VM_REGION_SUBMAP_INFO_COUNT_64;
      err = vm_region_recurse_64(itask, &address, &size, &nesting_depth,
				 (vm_region_info_64_t)&info, &count);
      if (err == KERN_INVALID_ADDRESS)
	{
	  break;
	}
      else if (err)
	{
	  mach_error((char *)"vm_region", err);
	  break; /* reached last region */
	}

      if (info.protection & VM_PROT_WRITE)
	{
	  int rslt;
	  vm_offset_t mempointer;     /* local copy of inferior's memory */
	  mach_msg_type_number_t memcopied;     /* for vm_read to use */

	  if ((0))
	    printf("count now %u addr 0x%s size 0x%s\n", count,
                   paddr_nz(address), paddr_nz(size));

	  rslt = mach_vm_read(itask, address, size, &mempointer, &memcopied);

	  if ((0))
	    printf("rslt is %d, copied %u\n", rslt, memcopied);

	  if (rslt == KERN_SUCCESS)
	    {
	      target_write(&current_target, TARGET_OBJECT_MEMORY, NULL,
			   (bfd_byte *)mempointer, address, memcopied);
	    }
	}

      if (info.is_submap)
	nesting_depth++;
      else
	address += size;
    }
}

/* Find an app bundle Info.plist XML file and use libxml2 to parse it. */
/* The string returned (NULL if unsuccessful) has been malloc()'ed by
 * libxml2, so free() it; do NOT xfree() or xmfree() it.  */
static ATTRIBUTE_USED const char *
get_bundle_executable_from_plist(const char *pathname)
{
#if !defined(LIBXML2_IS_USABLE) || !LIBXML2_IS_USABLE
  return NULL;
#else
  xmlDoc *doc = NULL;
  xmlNode *root_element = NULL;
  char *info_plist_name;
  const char *exe_name = NULL;
  struct stat s;

  LIBXML_TEST_VERSION
  info_plist_name =
    (char *)xmalloc(strlen(pathname) + strlen("/Info.plist") + 1UL);
  strcpy(info_plist_name, pathname);
  strcat(info_plist_name, "/Info.plist");

  if (stat(info_plist_name, &s) != 0) {
    return NULL;
  }

  doc = xmlParseFile(info_plist_name);

  xfree(info_plist_name);
  if (doc == NULL) {
    return NULL;
  }

  root_element = xmlDocGetRootElement(doc);
  if (root_element != NULL) {
    exe_name = find_executable_name_in_xml_tree(root_element);
  }

  xmlFreeDoc(doc);
  xmlCleanupParser();

  return exe_name;
#endif /* !LIBXML2_IS_USABLE */
}

/* Step through a libxml2 XML tree to find a CFBundleExecutable
 * key/value pair indicating the name of the executable in an
 * application bundle.  It would be nice if there were a higher
 * level way of doing this, but I do NOT want to add any
 * dependencies on Foundation-like things... */
#if defined(LIBXML2_IS_USABLE) && LIBXML2_IS_USABLE
static const char *
find_executable_name_in_xml_tree(xmlNode * a_node)
{
  xmlNode *cur_node = NULL;
  int just_saw_CFBundleExecutable = 0;
  const char *ret;

  for (cur_node = a_node; cur_node; cur_node = cur_node->next)
    {
      if ((cur_node->type == XML_ELEMENT_NODE) && cur_node->name)
	{
	  xmlChar *contents = xmlNodeGetContent(cur_node);
	  if ((strcmp((const char *)cur_node->name, "key") == 0)
	      && (strcmp((const char *)contents, "CFBundleExecutable") == 0))
	    {
	      just_saw_CFBundleExecutable = 1;
	      continue;
	    }
	  if ((strcmp((const char *)cur_node->name, "string") == 0)
	      && (just_saw_CFBundleExecutable == 1))
	    {
	      return (const char *)contents;
	    }
	  just_saw_CFBundleExecutable = 0;
	}
      ret = find_executable_name_in_xml_tree(cur_node->children);
      if (ret != NULL) {
	return ret;
      }
    }
  return (NULL);
}
#endif /* LIBXML2_IS_USABLE */

#if defined(GDB_RC_VERSION) && (GDB_RC_VERSION < 1461)
static struct cached_value *dlerror_function;

/* */
struct value *
macosx_load_dylib(char *name, char *flags)
{
  /* We are basically just going to call dlopen, and return the
     cookie that it returns.  BUT, we also have to make sure that
     we can get the unlimited mode of the ObjC debugger mode, since
     if the runtime is present, it is very likely that the new library
     will change the runtime...  */

  struct cleanup *debugger_mode_cleanup;
  struct cleanup *sched_cleanup;
  static struct cached_value *dlopen_function = NULL;
  struct value *arg_val[2];
  struct value *ret_val;
  int int_flags;
  enum objc_debugger_mode_result objc_retval;

  if (!macosx_check_safe_call(LOADER_SUBSYSTEM, CHECK_ALL_THREADS))
    error("Cannot call into the loader at present, it is locked.");

  if (dlopen_function == NULL)
    {
      if (lookup_minimal_symbol("dlopen", 0, 0))
	{
	  dlopen_function = create_cached_function("dlopen",
						   builtin_type_voidptrfuncptr);
	}
    }

  if (dlopen_function == NULL)
    error("Cannot find dlopen function, so it is not possible to load shared libraries.");

  if (dlerror_function == NULL)
    {
      if (lookup_minimal_symbol("dlerror", 0, 0))
	{
	  dlerror_function = create_cached_function("dlerror",
						    builtin_type_voidptrfuncptr);
	}
    }

  /* Decode the flags:  */
  int_flags = 0;
  if (flags != NULL)
    {
      /* The list of flags should be in the form A|B|C, but I am actually going
       * to do an even cheesier job of parsing, and just look for the elements
       * I want instead: */
      if (strstr(flags, "RTLD_LAZY") != NULL)
	int_flags |= RTLD_LAZY;
      if (strstr(flags, "RTLD_NOW") != NULL)
	int_flags |= RTLD_NOW;
      if (strstr(flags, "RTLD_LOCAL") != NULL)
	int_flags |= RTLD_LOCAL;
      if (strstr(flags, "RTLD_GLOBAL") != NULL)
	int_flags |= RTLD_GLOBAL;
      if (strstr(flags, "RTLD_NOLOAD") != NULL)
	int_flags |= RTLD_NOLOAD;
      if (strstr(flags, "RTLD_NODELETE") != NULL)
	int_flags |= RTLD_NODELETE;
      if (strstr(flags, "RTLD_FIRST") != NULL)
	int_flags |= RTLD_FIRST;
    }

  /* If the user did NOT pass in anything, set some sensible defaults: */
  if (int_flags == 0)
    int_flags = RTLD_GLOBAL|RTLD_NOW;

  arg_val[1] = value_from_longest(builtin_type_int, int_flags);

  /* Have to do the hand_call function cleanups here, since if the debugger
   * mode is already turned on, it may be turned on more permissively than we
   * want: */
  do_hand_call_cleanups(ALL_CLEANUPS);

  sched_cleanup = make_cleanup_set_restore_scheduler_locking_mode(scheduler_locking_on);

  arg_val[0] = value_coerce_array(value_string(name, strlen(name) + 1));

  objc_retval = make_cleanup_set_restore_debugger_mode(&debugger_mode_cleanup, 1);

  if (objc_retval == objc_debugger_mode_fail_objc_api_unavailable)
    if (target_check_safe_call(OBJC_SUBSYSTEM, CHECK_SCHEDULER_VALUE))
      objc_retval = objc_debugger_mode_success;

  if (objc_retval != objc_debugger_mode_success)
    error("Not safe to call dlopen at this time.");

  ret_val = call_function_by_hand(lookup_cached_function(dlopen_function),
				  2, arg_val);
  do_cleanups(debugger_mode_cleanup);
  do_cleanups(sched_cleanup);

  /* Again we have to clear this out, since we do NOT want to preserve
   * this version of the debugger mode: */
  do_hand_call_cleanups(ALL_CLEANUPS);
  if (ret_val != NULL)
    {
      CORE_ADDR dlopen_token;
      dlopen_token = value_as_address(ret_val);
      if (dlopen_token == 0)
	{
	  /* This indicates an error in the attempt to call dlopen.
           * Call dlerror to get a pointer to the error message.  */

	  char *error_str;
	  int error_str_len;
	  int read_error;
	  CORE_ADDR error_addr;

	  struct cleanup *scheduler_cleanup;

	  if (dlerror_function == NULL)
	    error("dlopen got an error, but dlerror is NOT available to report the error.");

	  scheduler_cleanup =
	    make_cleanup_set_restore_scheduler_locking_mode(scheduler_locking_on);

	  gdb_assert(scheduler_cleanup != NULL);

	  ret_val = call_function_by_hand(lookup_cached_function(dlerror_function),
                                                                 0, NULL);
	  /* Now read the string out of the target: */
	  error_addr = value_as_address(ret_val);
	  error_str_len = target_read_string(error_addr, &error_str, INT_MAX,
					     &read_error);
	  if (read_error == 0)
	    {
	      make_cleanup(xfree, error_str);
              if (error_str_len > 0)
	        error("Error calling dlopen for: \"%s\": \"%s\"", name, error_str);
              else
                error("Error calling dlopen for: \"%s\", but error string has invalid length %d.",
                      name, error_str_len);
	    }
	  else
	    error("Error calling dlopen for \"%s\", could not fetch error string.",
		  name);
	}
      else
	{
	  ui_out_field_core_addr(uiout, "handle", value_as_address(ret_val));
	  inferior_debug(1, "Return token was: %s.\n", paddr_nz(value_as_address(ret_val)));
	}
    }
  else
    inferior_debug(1, "Return value was NULL.\n");

  return ret_val;
}
#endif /* (GDB_RC_VERSION < 1461) */

/* */
void
_initialize_macosx_inferior(void)
{
  CHECK_FATAL(macosx_status == NULL);
  macosx_status = ((struct macosx_inferior_status *)
                   xmalloc(sizeof(struct macosx_inferior_status)));

  macosx_inferior_reset(macosx_status);

  dyld_init_paths(&macosx_dyld_status.path_info);
  dyld_objfile_info_init(&macosx_dyld_status.current_info);

  init_child_ops();
  macosx_child_ops = deprecated_child_ops;
  deprecated_child_ops.to_can_run = NULL;

  init_exec_ops();
  macosx_exec_ops = exec_ops;
  exec_ops.to_can_run = NULL;

  macosx_exec_ops.to_shortname = "macos-exec";
  macosx_exec_ops.to_longname = "Mac OS X executable";
  macosx_exec_ops.to_doc = "Mac OS X executable";
  macosx_exec_ops.to_can_async_p = standard_can_async_p;
  macosx_exec_ops.to_is_async_p = standard_is_async_p;

  macosx_exec_ops.to_has_thread_control = (tc_schedlock | tc_switch);

  macosx_exec_ops.to_find_exception_catchpoints
    = macosx_find_exception_catchpoints;
  macosx_exec_ops.to_enable_exception_callback
    = macosx_enable_exception_callback;
  macosx_exec_ops.to_get_current_exception_event
    = macosx_get_current_exception_event;

  macosx_complete_child_target(&macosx_exec_ops);

  /* We do NOT currently ever use the "macos-exec" target ops.
     Instead, just make them be the default exec_ops.  */

  /* FIXME: The original intent was that we have a macosx_exec_ops
     struct, that inherits from exec_ops, and provides some extra
     functions.  But the problem is that the exec target gets pushed
     in generic code in exec.c, and that code has exec_ops hard-coded
     into it.  We should most likely make the exec-target be
     determined along with the architecture by the ABI recognizer.  */

  exec_ops = macosx_exec_ops;
  macosx_exec_ops.to_can_run = NULL;

  macosx_child_ops.to_shortname = "macos-child";
  macosx_child_ops.to_longname = "Mac OS X child process";
  macosx_child_ops.to_doc =
    "Mac OS X child process (started by the \"run\" command).";
  macosx_child_ops.to_attach = macosx_child_attach;
  macosx_child_ops.to_detach = macosx_child_detach;
  macosx_child_ops.to_create_inferior = macosx_child_create_inferior;
  macosx_child_ops.to_files_info = macosx_child_files_info;
  macosx_child_ops.to_wait = macosx_child_wait;
  macosx_child_ops.to_mourn_inferior = macosx_mourn_inferior;
  macosx_child_ops.to_kill = macosx_kill_inferior_safe;
  macosx_child_ops.to_stop = macosx_child_stop;
  macosx_child_ops.to_resume = macosx_child_resume;
  macosx_child_ops.to_thread_alive = macosx_child_thread_alive;
  macosx_child_ops.to_get_thread_id_str = macosx_get_thread_id_str;
  macosx_child_ops.to_get_thread_name = macosx_get_thread_name;
  macosx_child_ops.to_load = NULL;
  macosx_child_ops.deprecated_xfer_memory = mach_xfer_memory;
  macosx_child_ops.to_xfer_partial = mach_xfer_partial;
  macosx_child_ops.to_can_async_p = standard_can_async_p;
  macosx_child_ops.to_is_async_p = standard_is_async_p;
  macosx_child_ops.to_terminal_inferior = macosx_terminal_inferior;
  macosx_child_ops.to_terminal_ours = macosx_terminal_ours;
  macosx_child_ops.to_async = macosx_async;
  macosx_child_ops.to_async_mask_value = 1;
  macosx_child_ops.to_bind_function = dyld_lookup_and_bind_function;
  macosx_child_ops.to_check_safe_call = macosx_check_safe_call;
  macosx_child_ops.to_setup_safe_print = objc_setup_safe_print;
  macosx_child_ops.to_allocate_memory = macosx_allocate_space_in_inferior;
  macosx_child_ops.to_check_is_objfile_loaded = dyld_is_objfile_loaded;
#if defined(TARGET_ARM)
  macosx_child_ops.to_keep_going = arm_macosx_keep_going;
  macosx_child_ops.to_save_thread_inferior_status = arm_macosx_save_thread_inferior_status;
  macosx_child_ops.to_restore_thread_inferior_status = arm_macosx_restore_thread_inferior_status;
  macosx_child_ops.to_free_thread_inferior_status = arm_macosx_free_thread_inferior_status;
#endif /* TARGET_ARM */
  macosx_child_ops.to_has_thread_control = (tc_schedlock | tc_switch);

  macosx_child_ops.to_find_exception_catchpoints
    = macosx_find_exception_catchpoints;
  macosx_child_ops.to_enable_exception_callback
    = macosx_enable_exception_callback;
  macosx_child_ops.to_get_current_exception_event
    = macosx_get_current_exception_event;

  macosx_child_ops.to_find_exception_catchpoints
    = macosx_find_exception_catchpoints;
  macosx_child_ops.to_enable_exception_callback
    = macosx_enable_exception_callback;
  macosx_child_ops.to_get_current_exception_event
    = macosx_get_current_exception_event;

  macosx_child_ops.to_load_solib = macosx_load_dylib;

  macosx_complete_child_target(&macosx_child_ops);

  add_target(&macosx_exec_ops);
  add_target(&macosx_child_ops);

  inferior_stderr = fdopen(fileno(stderr), "w");
  inferior_debug(2, "GDB task: 0x%lx, pid: %d\n",
                 (unsigned long)mach_task_self(), getpid());

  add_setshow_boolean_cmd("inferior-bind-exception-port", class_obscure,
			  &inferior_bind_exception_port_flag, _("\
Set if GDB should bind the task exception port."), _("\
Show if GDB should bind the task exception port."), NULL,
			  NULL, NULL,
			  &setlist, &showlist);

  add_setshow_boolean_cmd("inferior-ptrace", class_obscure,
			  &inferior_ptrace_flag, _("\
Set if GDB should attach to the subprocess using ptrace()."), _("\
Show if GDB should attach to the subprocess using ptrace()."), NULL,
			  NULL, NULL,
			  &setlist, &showlist);

  add_setshow_boolean_cmd("inferior-ptrace-on-attach", class_obscure,
			  &inferior_ptrace_on_attach_flag, _("\
Set if GDB should attach to the subprocess using ptrace()."), _("\
Show if GDB should attach to the subprocess using ptrace()."), NULL,
			  NULL, NULL,
			  &setlist, &showlist);

  add_info("fork", cpfork_info, "help");
}

#ifdef U_HAVE_STD_STRING
# undef U_HAVE_STD_STRING
#endif /* U_HAVE_STD_STRING */

#ifdef _dyld_debug_make_runnable
# undef _dyld_debug_make_runnable
#endif /* _dyld_debug_make_runnable */
#ifdef _dyld_debug_restore_runnable
# undef _dyld_debug_restore_runnable
#endif /* _dyld_debug_restore_runnable */
#ifdef _dyld_debug_module_name
# undef _dyld_debug_module_name
#endif /* _dyld_debug_module_name */
#ifdef _dyld_debug_set_error_func
# undef _dyld_debug_set_error_func
#endif /* _dyld_debug_set_error_func */
#ifdef _dyld_debug_add_event_subscriber
# undef _dyld_debug_add_event_subscriber
#endif /* _dyld_debug_add_event_subscriber */

#ifdef macosx_ptrace_her
# undef macosx_ptrace_her
#endif /* macosx_ptrace_her */

/* EOF */
