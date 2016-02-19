/* Target-vector operations for controlling Unix child processes, for GDB.

   Copyright 1990, 1991, 1992, 1993, 1994, 1995, 1996, 1998, 1999,
   2000, 2002, 2003, 2004 Free Software Foundation, Inc.

   Contributed by Cygnus Support.

   ## Contains "temporary" hacks...

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

#include "defs.h"
#include "frame.h"		/* required by inferior.h */
#include "inferior.h"
#include "target.h"
#include "gdbcore.h"
#include "command.h"
#include "gdb_stat.h"
#include <signal.h>
#include <sys/types.h>
#include <fcntl.h>
/* APPLE LOCAL event-loop.h */
#include "event-loop.h"

#include "observer.h"
#include "gdb_wait.h"
#include "inflow.h"

extern int child_enable_exception_callback(enum exception_event_kind, int);
/* APPLE LOCAL begin exception catchpoints */
extern struct symtabs_and_lines *
child_find_exception_catchpoints(enum exception_event_kind,
                                 struct objfile *);
/* APPLE LOCAL end exception catchpoints */

extern struct exception_event_record
  *child_get_current_exception_event(void);

extern void _initialize_inftarg(void);

static void child_prepare_to_store(void);

#ifndef CHILD_WAIT
/* APPLE LOCAL gdb client data */
static ptid_t child_wait(ptid_t, struct target_waitstatus *,
                         gdb_client_data client_data);
#endif /* CHILD_WAIT */

static void child_open(const char *, int);

static void child_files_info(struct target_ops *);

static void child_detach(const char *, int);

static void child_attach(const char *, int);

#if !defined(CHILD_POST_ATTACH)
extern void child_post_attach(int);
#endif /* !CHILD_POST_ATTACH */

static void ptrace_me(void);

static void ptrace_him(int);

static void child_create_inferior(char *, char *, char **, int);

static void child_mourn_inferior(void);

static int child_can_run(void);

static void child_stop(void);

#ifndef CHILD_THREAD_ALIVE
int child_thread_alive(ptid_t);
#endif /* !CHILD_THREAD_ALIVE */

/* APPLE LOCAL make globally visible: */
extern void init_child_ops(void);

#if !defined(CHILD_PID_TO_STR)
extern char *child_pid_to_str(ptid_t);
#endif /* !CHILD_PID_TO_STR */

extern char **environ;

int child_suppress_run = 0;  /* Non-zero if inftarg should pretend not to
                              * be a runnable target.  Used by targets
                              * that can sit atop inftarg, such as HPUX
                              * thread support.  */

#ifndef CHILD_WAIT
/* Wait for child to do something.  Return pid of child, or -1 in case
 * of error; store status through argument pointer OURSTATUS: */
static ptid_t
/* APPLE LOCAL gdb client data */
child_wait(ptid_t ptid, struct target_waitstatus *ourstatus,
           gdb_client_data client_data)
{
  int save_errno;
  int status;
  char *execd_pathname = NULL;
  int exit_status;
  int related_pid;
  int syscall_id;
  enum target_waitkind kind;
  int pid;

  do {
      set_sigint_trap();  /* Causes SIGINT to be passed on to the
                           * attached process. */
      set_sigio_trap();

      pid = wait(&status);

      save_errno = errno;

      clear_sigio_trap();

      clear_sigint_trap();

      if (pid == -1)
	{
	  if (save_errno == EINTR)
	    continue;

	  fprintf_unfiltered(gdb_stderr,
                             "Child process unexpectedly missing: %s.\n",
                             safe_strerror(save_errno));

	  /* Claim it exited with unknown signal: */
	  ourstatus->kind = TARGET_WAITKIND_SIGNALLED;
	  ourstatus->value.sig = TARGET_SIGNAL_UNKNOWN;
	  return pid_to_ptid(-1);
	}

      /* Did it exit? */
      if (target_has_exited(pid, status, &exit_status))
	{
	  /* ??rehrauer: For now, ignore this. */
	  continue;
	}

      if (!target_thread_alive(pid_to_ptid(pid)))
	{
	  ourstatus->kind = TARGET_WAITKIND_SPURIOUS;
	  return pid_to_ptid(pid);
	}
  } while (pid != PIDGET(inferior_ptid)); /* Some other child died or stopped */

  store_waitstatus(ourstatus, status);
  return pid_to_ptid(pid);
}
#endif /* CHILD_WAIT */

#ifndef CHILD_THREAD_ALIVE
/* Check to see if the given thread is alive.

   FIXME: Is kill() ever the right way to do this?  I doubt it, but
   for now we're going to try and be compatable with the old thread
   code.  */
int
child_thread_alive(ptid_t ptid)
{
  pid_t pid = PIDGET(ptid);

  return (kill(pid, 0) != -1);
}
#endif /* !CHILD_THREAD_ALIVE */

/* Attach to process PID, then initialize for debugging it: */
static void
child_attach(const char *args, int from_tty)
{
  char *exec_file;
  int pid;
  const char *dummy;

  if (!args)
    error_no_arg(_("process-id to attach"));

  dummy = args;
  pid = strtol(args, (char **)&dummy, 0);
  /* Some targets do NOT set errno on errors, grrr! */
  if ((pid == 0) && (args == dummy))
    error(_("Illegal process-id: %s."), args);

  if (pid == getpid())	/* Trying to masturbate? (?) */
    error(_("I refuse to debug myself!"));

  if (from_tty)
    {
      exec_file = (char *)get_exec_file(0);

      if (exec_file)
	printf_unfiltered(_("Attaching to program: %s, %s\n"), exec_file,
			  target_pid_to_str(pid_to_ptid(pid)));
      else
	printf_unfiltered(_("Attaching to %s\n"),
			  target_pid_to_str(pid_to_ptid(pid)));

      gdb_flush(gdb_stdout);
    }

  attach(pid);

  inferior_ptid = pid_to_ptid(pid);
  push_target(&deprecated_child_ops);

  /* Do this first, before anything has had a chance to query the
   * inferior's symbol table or similar.  */
  observer_notify_inferior_created(&current_target, from_tty);
}

#if !defined(CHILD_POST_ATTACH)
void ATTRIBUTE_CONST
child_post_attach(int pid ATTRIBUTE_UNUSED)
{
  /* This version of Unix does NOT require a meaningful "post attach"
   * operation by a debugger.  */
  return;
}
#endif /* !CHILD_POST_ATTACH */

/* Take a program previously attached to and detaches it.
 * The program resumes execution and will no longer stop on signals, etc.
 * We had better not have left any breakpoints in the program, or it will
 * die when it hits one.  For this to work, it may be necessary for the
 * process to have been previously attached.  It *might* work if the
 * program was started via the normal ptrace (PTRACE_TRACEME): */
static void
child_detach(const char *args, int from_tty)
{
  int siggnal = 0;
  int pid = PIDGET(inferior_ptid);

  if (from_tty)
    {
      const char *exec_file = get_exec_file(0);
      if (exec_file == 0)
	exec_file = "";
      printf_unfiltered(_("Detaching from program: %s, %s\n"), exec_file,
                        target_pid_to_str(pid_to_ptid(pid)));
      gdb_flush (gdb_stdout);
    }
  if (args)
    siggnal = atoi(args);

  detach(siggnal);

  inferior_ptid = null_ptid;
  unpush_target(&deprecated_child_ops);
}

/* Get ready to modify the registers array.  On machines which store
   individual registers, this doesn't need to do anything.  On machines
   which store all the registers in one fell swoop, this makes sure
   that registers contains all the registers from the program being
   debugged.  */

static void
child_prepare_to_store(void)
{
#ifdef CHILD_PREPARE_TO_STORE
  CHILD_PREPARE_TO_STORE();
#endif /* CHILD_PREPATE_TO_STORE */
}

/* Print status information about what we are accessing: */
static void
child_files_info(struct target_ops *ignore)
{
  printf_unfiltered(_("\tUsing the running image of %s %s.\n"),
                    (attach_flag ? "attached" : "child"),
                    target_pid_to_str(inferior_ptid));
}

static void ATTR_NORETURN
child_open(const char *arg ATTRIBUTE_UNUSED, int from_tty ATTRIBUTE_UNUSED)
{
  error(_("Use the \"run\" command to start a Unix child process."));
}

/* Stub function which causes the inferior that runs it, to be ptrace-able
 * by its parent process: */
static void
ptrace_me(void)
{
  /* "Trace me, Dr. Memory!" */
  call_ptrace(0, 0, (PTRACE_TYPE_ARG3)0, 0);
}

/* Stub function which causes the GDB that runs it, to start ptrace-ing
 * the child process: */
static void
ptrace_him(int pid)
{
  push_target(&deprecated_child_ops);

  /* On some targets, there must be some explicit synchronization
     between the parent and child processes after the debugger
     forks, and before the child execs the debuggee program.  This
     call basically gives permission for the child to exec.  */
  target_acknowledge_created_inferior(pid);

  /* APPLE LOCAL startup with shell: */
  startup_inferior(0); /* FIXME: '0' is a dummy value */

  /* On some targets, there must be some explicit actions taken after
   * the inferior has been started up: */
  target_post_startup_inferior(pid_to_ptid(pid));
}

/* Start an inferior Unix child process and sets inferior_ptid to its pid.
   EXEC_FILE is the file to run.
   ALLARGS is a string containing the arguments to the program.
   ENV is the environment vector to pass.  Errors reported with error().  */

static void
child_create_inferior(char *exec_file, char *allargs, char **env,
                      int from_tty)
{
  fork_inferior(exec_file, allargs, env, ptrace_me, ptrace_him, NULL, NULL);

  /* We are at the first instruction we care about: */
  observer_notify_inferior_created(&current_target, from_tty);
  /* Pedal to the metal... */
  proceed((CORE_ADDR)-1, TARGET_SIGNAL_0, 0);
}

#if !defined(CHILD_POST_STARTUP_INFERIOR)
void
child_post_startup_inferior(ptid_t ptid ATTRIBUTE_UNUSED)
{
  /* This version of Unix does NOT require a meaningful
   * "post startup inferior" operation by a debugger: */
  return;
}
#endif /* !CHILD_POST_STARTUP_INFERIOR */

#if !defined(CHILD_ACKNOWLEDGE_CREATED_INFERIOR)
void ATTRIBUTE_CONST
child_acknowledge_created_inferior(int pid ATTRIBUTE_UNUSED)
{
  /* This version of Unix does NOT require a meaningful
   * "acknowledge created inferior" operation by a debugger: */
  return;
}
#endif /* !CHILD_ACKNOWLEDGE_CREATED_INFERIOR */


#if !defined(CHILD_INSERT_FORK_CATCHPOINT)
void ATTRIBUTE_CONST
child_insert_fork_catchpoint(int pid ATTRIBUTE_UNUSED)
{
  /* This version of Unix does NOT support notification of fork events: */
  return;
}
#endif /* !CHILD_INSERT_FORK_CATCHPOINT */

#if !defined(CHILD_REMOVE_FORK_CATCHPOINT)
int ATTRIBUTE_CONST
child_remove_fork_catchpoint(int pid ATTRIBUTE_UNUSED)
{
  /* This version of Unix does NOT support notification of fork events: */
  return 0;
}
#endif /* !CHILD_REMOVE_FORK_CATCHPOINT */

#if !defined(CHILD_INSERT_VFORK_CATCHPOINT)
void ATTRIBUTE_CONST
child_insert_vfork_catchpoint(int pid ATTRIBUTE_UNUSED)
{
  /* This version of Unix does NOT support notification of vfork events: */
  return;
}
#endif /* !CHILD_INSERT_VFORK_CATCHPOINT */

#if !defined(CHILD_REMOVE_VFORK_CATCHPOINT)
int ATTRIBUTE_CONST
child_remove_vfork_catchpoint(int pid ATTRIBUTE_UNUSED)
{
  /* This version of Unix does NOT support notification of vfork events: */
  return 0;
}
#endif /* !CHILD_REMOVE_VFORK_CATCHPOINT */

#if !defined(CHILD_FOLLOW_FORK)
int ATTRIBUTE_CONST
child_follow_fork(int follow_child ATTRIBUTE_UNUSED)
{
  /* This version of Unix does NOT support following fork or vfork
   * events: */
  return 0;
}
#endif /* !CHILD_FOLLOW_FORK */

#if !defined(CHILD_INSERT_EXEC_CATCHPOINT)
void ATTRIBUTE_CONST
child_insert_exec_catchpoint(int pid ATTRIBUTE_UNUSED)
{
  /* This version of Unix does NOT support notification of exec events: */
  return;
}
#endif /* !CHILD_INSERT_EXEC_CATCHPOINT */

#if !defined(CHILD_REMOVE_EXEC_CATCHPOINT)
int ATTRIBUTE_CONST
child_remove_exec_catchpoint(int pid ATTRIBUTE_UNUSED)
{
  /* This version of Unix does NOT support notification of exec events: */
  return 0;
}
#endif /* !CHILD_REMOVE_EXEC_CATCHPOINT */

#if !defined(CHILD_REPORTED_EXEC_EVENTS_PER_EXEC_CALL)
int ATTRIBUTE_CONST
child_reported_exec_events_per_exec_call(void)
{
  /* This version of Unix does NOT support notification of exec events: */
  return 1;
}
#endif /* !CHILD_REPORTED_EXEC_EVENTS_PER_EXEC_CALL */

#if !defined(CHILD_HAS_EXITED)
int
child_has_exited(int pid, int wait_status, int *exit_status)
{
  if (WIFEXITED(wait_status))
    {
      *exit_status = WEXITSTATUS(wait_status);
      return 1;
    }

  if (WIFSIGNALED(wait_status))
    {
      *exit_status = 0;	    /* ?? Do NOT know what else to say here. */
      return 1;
    }

  /* ?? Do we really need to consult the event state, too?  Assume the
   * wait_state alone suffices: */
  return 0;
}
#endif /* !CHILD_HAS_EXITED */


static void
child_mourn_inferior(void)
{
  unpush_target(&deprecated_child_ops);
  generic_mourn_inferior();
}

static int
child_can_run(void)
{
  /* This variable is controlled by modules that sit atop inftarg that may
   * layer their own process structure atop that provided here.  For
   * example, hpux-thread.c does this because of the Hpux user-mode level
   * thread model: */
  return !child_suppress_run;
}

/* Send a SIGINT to the process group.  This acts just like the user typed
 * a ^C on the controlling terminal.
 *
 * XXX - This may not be correct for all systems.  Some may want to use
 * killpg() instead of kill(-pgrp): */
static void
child_stop(void)
{
  kill(-inferior_process_group, SIGINT);
}

#if !defined(CHILD_ENABLE_EXCEPTION_CALLBACK)
/* APPLE LOCAL begin exception catchpoints */
struct symtabs_and_lines * ATTRIBUTE_CONST
child_find_exception_catchpoints(enum exception_event_kind kind ATTRIBUTE_UNUSED,
                                 struct objfile *objfile ATTRIBUTE_UNUSED)
{
  return (struct symtabs_and_lines *)NULL;
}
/* APPLE LOCAL end exception catchpoints */

int ATTRIBUTE_CONST
child_enable_exception_callback(enum exception_event_kind kind ATTRIBUTE_UNUSED,
                                int enable ATTRIBUTE_UNUSED)
{
  return 0;
}
#endif /* !CHILD_ENABLE_EXCEPTION_CALLBACK */

#if !defined(CHILD_GET_CURRENT_EXCEPTION_EVENT)
struct exception_event_record * ATTRIBUTE_CONST
child_get_current_exception_event(void)
{
  return (struct exception_event_record *)NULL;
}
#endif /* !CHILD_GET_CURRENT_EXCEPTION_EVENT */


#if !defined(CHILD_PID_TO_EXEC_FILE)
char * ATTRIBUTE_CONST
child_pid_to_exec_file(int pid ATTRIBUTE_UNUSED)
{
  /* This version of Unix does NOT support translation of a process ID
   * to the filename of the executable file. */
  return NULL;
}
#endif /* !CHILD_PID_TO_EXEC_FILE */

char * ATTRIBUTE_CONST
child_core_file_to_sym_file(char *core)
{
  /* The target stratum for a running executable need not support
   * this operation: */
  return NULL;
}

/* Perform a partial transfer to/from the specified object.  For memory
 * transfers, fall back to the old memory xfer functions: */
static LONGEST
child_xfer_partial(struct target_ops *ops, enum target_object object,
		   const char *annex, gdb_byte *readbuf,
		   const gdb_byte *writebuf, ULONGEST offset, LONGEST len)
{
  switch (object)
    {
    case TARGET_OBJECT_MEMORY:
      if (readbuf)
	return child_xfer_memory(offset, readbuf, (int)len, 0/*write*/,
				 NULL, ops);
      if (writebuf)
	return child_xfer_memory(offset, (gdb_byte *)writebuf, (int)len,
                                 1/*write*/, NULL, ops);
      return -1;

    case TARGET_OBJECT_UNWIND_TABLE:
#ifndef NATIVE_XFER_UNWIND_TABLE
# define NATIVE_XFER_UNWIND_TABLE(OPS,OBJECT,ANNEX,WRITEBUF,READBUF,OFFSET,LEN) (-1)
#endif /* !NATIVE_XFER_UNWIND_TABLE */
      return NATIVE_XFER_UNWIND_TABLE(ops, object, annex, readbuf, writebuf,
                                      offset, len);

    case TARGET_OBJECT_AUXV:
#ifndef NATIVE_XFER_AUXV
# define NATIVE_XFER_AUXV(OPS,OBJECT,ANNEX,WRITEBUF,READBUF,OFFSET,LEN) (-1)
#endif /* !NATIVE_XFER_AUXV */
      return NATIVE_XFER_AUXV(ops, object, annex, readbuf, writebuf,
                              offset, len);

    default:
      return -1;
    }
}

#if !defined(CHILD_PID_TO_STR)
char *
child_pid_to_str(ptid_t ptid)
{
  return normal_pid_to_str(ptid);
}
#endif /* !CHILD_PID_TO_STR */

/* APPLE LOCAL make globally visible: */
void
init_child_ops(void)
{
  deprecated_child_ops.to_shortname = "child";
  deprecated_child_ops.to_longname = "Unix child process";
  deprecated_child_ops.to_doc = "Unix child process (started by the \"run\" command).";
  deprecated_child_ops.to_open = child_open;
  deprecated_child_ops.to_attach = child_attach;
  deprecated_child_ops.to_post_attach = child_post_attach;
  deprecated_child_ops.to_detach = child_detach;
  deprecated_child_ops.to_resume = child_resume;
  deprecated_child_ops.to_wait =
    (__typeof__(deprecated_child_ops.to_wait))child_wait;
  deprecated_child_ops.to_fetch_registers = fetch_inferior_registers;
  deprecated_child_ops.to_store_registers = store_inferior_registers;
  deprecated_child_ops.to_prepare_to_store = child_prepare_to_store;
  deprecated_child_ops.deprecated_xfer_memory = child_xfer_memory;
  deprecated_child_ops.to_xfer_partial = child_xfer_partial;
  deprecated_child_ops.to_files_info = child_files_info;
  deprecated_child_ops.to_insert_breakpoint = memory_insert_breakpoint;
  deprecated_child_ops.to_remove_breakpoint = memory_remove_breakpoint;
  deprecated_child_ops.to_terminal_init = terminal_init_inferior;
  deprecated_child_ops.to_terminal_inferior = terminal_inferior;
  deprecated_child_ops.to_terminal_ours_for_output = terminal_ours_for_output;
  deprecated_child_ops.to_terminal_save_ours = terminal_save_ours;
  deprecated_child_ops.to_terminal_ours = terminal_ours;
  deprecated_child_ops.to_terminal_info = child_terminal_info;
  deprecated_child_ops.to_kill = kill_inferior;
  deprecated_child_ops.to_create_inferior = child_create_inferior;
  deprecated_child_ops.to_post_startup_inferior = child_post_startup_inferior;
  deprecated_child_ops.to_acknowledge_created_inferior = child_acknowledge_created_inferior;
  deprecated_child_ops.to_insert_fork_catchpoint = child_insert_fork_catchpoint;
  deprecated_child_ops.to_remove_fork_catchpoint = child_remove_fork_catchpoint;
  deprecated_child_ops.to_insert_vfork_catchpoint = child_insert_vfork_catchpoint;
  deprecated_child_ops.to_remove_vfork_catchpoint = child_remove_vfork_catchpoint;
  deprecated_child_ops.to_follow_fork = child_follow_fork;
  deprecated_child_ops.to_insert_exec_catchpoint = child_insert_exec_catchpoint;
  deprecated_child_ops.to_remove_exec_catchpoint = child_remove_exec_catchpoint;
  deprecated_child_ops.to_reported_exec_events_per_exec_call = child_reported_exec_events_per_exec_call;
  deprecated_child_ops.to_has_exited = child_has_exited;
  deprecated_child_ops.to_mourn_inferior = child_mourn_inferior;
  deprecated_child_ops.to_can_run = child_can_run;
  deprecated_child_ops.to_thread_alive = child_thread_alive;
  deprecated_child_ops.to_pid_to_str = child_pid_to_str;
  deprecated_child_ops.to_stop = child_stop;
  deprecated_child_ops.to_enable_exception_callback = child_enable_exception_callback;
  /* APPLE LOCAL exception catchpoints */
  deprecated_child_ops.to_find_exception_catchpoints = child_find_exception_catchpoints;
  deprecated_child_ops.to_get_current_exception_event = child_get_current_exception_event;
  deprecated_child_ops.to_pid_to_exec_file = child_pid_to_exec_file;
  deprecated_child_ops.to_stratum = process_stratum;
  deprecated_child_ops.to_has_all_memory = 1;
  deprecated_child_ops.to_has_memory = 1;
  deprecated_child_ops.to_has_stack = 1;
  deprecated_child_ops.to_has_registers = 1;
  deprecated_child_ops.to_has_execution = 1;
  deprecated_child_ops.to_magic = OPS_MAGIC;
}

void
_initialize_inftarg(void)
{
#if defined(HAVE_OPTIONAL_PROC_FS)
  char procname[32];
  int fd;

  /* If we have an optional /proc filesystem (e.g. under OSF/1), then
   * avoid adding ptrace support if we can access the running GDB
   * via /proc: */
# ifndef PROC_NAME_FMT
#  define PROC_NAME_FMT "/proc/%05d"
# endif /* !PROC_NAME_FMT */
  snprintf(procname, sizeof(procname), PROC_NAME_FMT, getpid());
  fd = open(procname, O_RDONLY);
  if (fd >= 0)
    {
      close(fd);
      return;
    }
#endif /* HAVE_OPTIONAL_PROC_FS */

  init_child_ops();
  add_target(&deprecated_child_ops);
}

/* EOF */
