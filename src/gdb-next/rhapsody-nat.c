/*
 * rhapsody-nat.c
 */

#include "nextstep-nat-dyld.h"
#include "nextstep-nat-inferior.h"
#include "nextstep-nat-inferior-debug.h"
#include "nextstep-nat-mutils.h"
#include "nextstep-nat-sigthread.h"
#include "nextstep-nat-threads.h"
#include "nextstep-xdep.h"
#include "nextstep-nat-inferior-util.h"

#include "defs.h"
#include "inferior.h"
#include "target.h"
#include "symfile.h"
#include "symtab.h"
#include "objfiles.h"
#include "gdbcmd.h"
#include "gdbcore.h"
#include "gdbthread.h"

#include "bfd.h"

#include <sys/ptrace.h>
#include <sys/signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <ctype.h>
#ifdef HAVE_MACH_TASK_H
# include <mach/task.h>
#endif /* HAVE_MACH_TASK_H */

#ifndef MSG_OPTION_NONE
# ifdef MACH_MSG_OPTION_NONE
#  define MSG_OPTION_NONE MACH_MSG_OPTION_NONE
# endif /* MACH_MSG_OPTION_NONE */
#endif /* !MSG_OPTION_NONE */

/* Prototypes: */
extern void next_handle_exception(struct next_inferior_status *, msg_header_t *,
				  struct target_waitstatus *);

/* Functions: */
static void
next_exception_forwarded_reply(struct next_inferior_status *inferior,
			       struct next_exception_data *erequest)
{
  kern_return_t kret;
  struct next_exception_reply ereply;

  mach_port_t orig_reply_port = erequest->header.msg_remote_port;

  erequest->header.msg_remote_port = inferior->exception_status.inferior_exception_port;
  erequest->header.msg_local_port = inferior->exception_status.inferior_exception_port;

  inferior_debug(1, "sending exception to old exception port\n");
  kret = mach_msg_send(&erequest->header);
  MACH_CHECK_ERROR(kret);

  ereply.header.msg_size = sizeof(struct next_exception_reply *);
  ereply.header.msg_local_port = inferior->exception_status.inferior_exception_port;

  inferior_debug(1, "receiving exception reply from old exception port\n");
  kret = mach_msg_receive(&ereply.header);
  MACH_CHECK_ERROR(kret);

  ereply.header.msg_local_port = PORT_NULL;
  ereply.header.msg_remote_port = orig_reply_port;

  inferior_debug(1, "sending exception reply\n");
  kret = mach_msg_send(&ereply.header);
  MACH_CHECK_ERROR(kret);
  (void)ereply;
}

#ifndef MSG_TYPE_INTEGER_32
# if defined(MACH_MSG_TYPE_INTEGER_32)
#  define MSG_TYPE_INTEGER_32 MACH_MSG_TYPE_INTEGER_32
# else
#  ifdef MACH_MSG_TYPE_INTEGER_T
#   define MSG_TYPE_INTEGER_32 MACH_MSG_TYPE_INTEGER_T
#  endif /* MACH_MSG_TYPE_INTEGER_T */
# endif /* MACH_MSG_TYPE_INTEGER_32 */
#endif /* !MSG_TYPE_INTEGER_32 */
static void
next_exception_internal_reply(struct next_inferior_status *inferior,
			      struct next_exception_data *erequest)
{
  kern_return_t ret;
  struct next_exception_reply ereply;

  CHECK_FATAL(erequest->task == inferior->task);

  ereply.header.msg_unused = 0;
  ereply.header.msg_simple = 1;
  ereply.header.msg_size = sizeof(struct next_exception_reply *);
  ereply.header.msg_type = erequest->header.msg_type;
  ereply.header.msg_local_port = PORT_NULL;
  ereply.header.msg_remote_port = erequest->header.msg_remote_port;
  ereply.header.msg_id = (erequest->header.msg_id + 100);

  ereply.retcode_type.msg_type_name = "int"; /* MSG_TYPE_INTEGER_32(?) */
  ereply.retcode_type.msg_type_size = (sizeof(int) * 8UL);
  ereply.retcode_type.msg_type_number = 1;
  ereply.retcode_type.msg_type_inline = 1;
  ereply.retcode_type.msg_type_longform = 0;
  ereply.retcode_type.msg_type_deallocate = 0;
  ereply.retcode_type.msg_type_unused = 0;

  ereply.retcode = KERN_SUCCESS;

  inferior_debug(1, "sending exception reply\n");
  ret = mach_msg_send(&ereply.header);
  MACH_WARN_ERROR(ret);
  (void)ereply;
}

void
next_handle_exception(struct next_inferior_status *inferior,
		      msg_header_t *message, struct target_waitstatus *status)
{
  struct next_exception_data *msg = (struct next_exception_data *)message;

  CHECK_FATAL(inferior != NULL);

  next_debug_exception(msg);

  if (msg->task != inferior->task) {
    inferior_debug(1, "ignoring exception forwarded from subprocess\n");
    next_exception_forwarded_reply(inferior, msg);
    return;
  }

  inferior->last_thread = msg->thread;

  if (inferior_handle_exceptions_flag) {
    next_inferior_suspend_mach(inferior);

    next_exception_internal_reply(inferior, msg);

    status->kind = TARGET_WAITKIND_STOPPED;

    switch (msg->exception) {
    case EXC_BAD_ACCESS:
      status->value.sig = TARGET_EXC_BAD_ACCESS;
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
      status->value.sig = TARGET_EXC_SOFTWARE;
      break;
    case EXC_BREAKPOINT:
      status->value.sig = TARGET_EXC_BREAKPOINT;
      status->value.sig = TARGET_SIGNAL_TRAP;
      break;
    default:
      status->value.sig = TARGET_SIGNAL_UNKNOWN;
      break;
    }
  } else {
    next_exception_forwarded_reply(inferior, (struct next_exception_data *)msg);
  }
}

#ifndef PORT_BACKLOG_MAX
# ifdef HOST_INFO_MAX
#  define PORT_BACKLOG_MAX HOST_INFO_MAX
# endif /* HOST_INFO_MAX */
#endif /* !PORT_BACKLOG_MAX */

/* */
void
next_create_inferior_for_task(struct next_inferior_status *inferior,
			      task_t task, int pid)
{
  kern_return_t ret;

  CHECK_FATAL(inferior != NULL);

  next_inferior_destroy(inferior);
  next_inferior_reset(inferior);

  inferior->task = task;
  inferior->pid = pid;

  inferior->attached_in_ptrace = 0;
  inferior->stopped_in_ptrace = 0;
  inferior->suspend_count = 0;

  /* */

  dyld_init_paths(&inferior->dyld_status.path_info);

  /* get notification messages for current task */

  ret = port_allocate(task_self(), 0,
		      &inferior->exception_status.inferior_exception_port);
  MACH_CHECK_ERROR(ret);

  ret = port_set_add(task_self(),
		     inferior->exception_status.inferior_exception_port,
		     PORT_BACKLOG_MAX);
  MACH_CHECK_ERROR(ret);

  if (inferior_bind_notify_port_flag) {
    ret =
      task_set_host_port(task_self(),
			 inferior->exception_status.inferior_exception_port);
    MACH_CHECK_ERROR(ret);
  }

  /* initialize signal port */

  ret = port_allocate(task_self(), 0,
		      (mach_port_name_t *)&inferior->signal_status);
  MACH_CHECK_ERROR(ret);

  ret = port_set_add(task_self(),
		     inferior->exception_status.inferior_exception_port,
		     PORT_BACKLOG_MAX);
  MACH_CHECK_ERROR(ret);

  /* initialize dyld port */
  /* FIXME: structure hierarchy seems to have changed; none of the dyld-related
   * sub-members seem to have ports any longer... */
  ret = port_allocate(task_self(), 0,
		      (mach_port_name_t *)&inferior->dyld_status);
  MACH_WARN_ERROR(ret);

  ret = port_set_add(task_self(),
		     inferior->exception_status.inferior_exception_port,
		     PORT_BACKLOG_MAX);
  MACH_CHECK_ERROR(ret);

  /* initialize gdb exception port */

  ret = port_allocate(task_self(), 0,
		      &inferior->exception_status.inferior_exception_port);
  MACH_CHECK_ERROR(ret);

  ret = port_set_add(task_self(),
		     inferior->exception_status.inferior_exception_port,
		     PORT_BACKLOG_MAX);
  MACH_CHECK_ERROR(ret);

  ret = port_allocate(task_self(), 0,
		      &inferior->exception_status.inferior_exception_port);
  MACH_CHECK_ERROR(ret);

  ret = port_set_add(task_self(),
		     inferior->exception_status.inferior_exception_port,
		     PORT_BACKLOG_MAX);
  MACH_CHECK_ERROR(ret);

  /* commandeer inferior exception port */

  if (inferior_bind_exception_port_flag) {
    next_save_exception_ports(inferior->task,
			      &inferior->exception_status.saved_exceptions);

    ret = task_set_exception_ports(task,
				   inferior->exception_status.inferior_exception_port,
				   0, 0, 0);
    MACH_CHECK_ERROR(ret);
  }

  inferior->last_thread = next_primary_thread_of_task(inferior->task);
}

/* EOF */
