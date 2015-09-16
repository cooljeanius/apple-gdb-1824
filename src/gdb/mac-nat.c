/* Target-vector operations for controlling Mac applications, for GDB.
   Copyright 1995, 1996, 1998, 1999, 2000, 2001
   Free Software Foundation, Inc.
   Written by Stan Shebs.  Contributed by Cygnus Support.

   This file is part of GDB.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without eve nthe implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* Note that because all the available Mac compilers are ANSI or very
 * close, and this is a native-only file, the code may be purely ANSI;
 * thus we can also use ANSI-specific compiler flags with it, too. */

#ifdef __APPLE_API_EVOLVING
# undef __APPLE_API_EVOLVING
#endif /* __APPLE_API_EVOLVING */

/* hack to deal with troublesome headers: */
#ifndef _SYS_ACL_H
# define _SYS_ACL_H 1
struct _acl;
typedef struct _acl *acl_t;
#endif /* !_SYS_ACL_H */

#if !defined(__LP64__) || (defined(__LP64__) && !__LP64__)
# if !defined(_MSC_VER)
#  pragma options align=mac68k
# endif /* !_MSC_VER */
#endif /* !__LP64__ */

#include "defs.h"
#include "frame.h"		/* required by inferior.h */
#include "inferior.h"
#include "target.h"
#include "gdb_wait.h"
#include "gdbcore.h"
#include "command.h"
#include <signal.h>
#include <sys/types.h>
#include <fcntl.h>
#include "buildsym.h"
#include "gdb_string.h"
#include "gdbthread.h"
#include "gdbcmd.h"
#include "regcache.h"

#include <Processes.h>

/* We call the functions "child_..." rather than "mac_..." so no one
 * is tempted to try to link this with other native-only code.  */

/* Forward declaration(s): */
extern struct target_ops child_ops;

#if !defined(REGCACHE_H) || \
    defined(_PREPEND_DEPRECATED_TO_REGCACHE_FUNCTIONS)
extern void supply_register(int, const void *);
#endif /* !REGCACHE_H || _PREPEND_DEPRECATED_TO_REGCACHE_FUNCTIONS */

static void child_stop(void);

extern void child_kill_inferior(void);

extern void _initialize_mac_nat(void);

static void
child_fetch_inferior_registers(int r)
{
  if (r < 0)
    {
      for (r = 0; r < NUM_REGS; r++) {
	child_fetch_inferior_registers(r);
      }
    }
  else
    {
      supply_register(r, 0);
    }
}

static void
child_store_inferior_registers(int r)
{
  if (r < 0)
    {
      for (r = 0; r < NUM_REGS; r++) {
	child_store_inferior_registers(r);
      }
    }
  else
    {
#ifdef REGCACHE_H
      deprecated_read_register_gen(r, 0);
#else
      read_register_gen(r, 0);
#endif /* REGCACHE_H */
    }
}

#ifdef _NM_NEXTSTEP_H_
int
child_wait(int ptid, struct target_waitstatus *ourstatus, void *data)
{
# if (defined(__APPLE__) && defined(__APPLE_CC__)) || defined(__MWERKS__)
#  pragma unused (ptid, ourstatus, data)
# endif /* (__APPLE__ && __APPLE_CC__) || __MWERKS__ */
  return 0;
}
#else
static ptid_t
child_wait(ptid_t ptid, struct target_waitstatus *ourstatus)
{
# if (defined(__APPLE__) && defined(__APPLE_CC__)) || defined(__MWERKS__)
#  pragma unused (ptid, target_waitstatus)
# endif /* (__APPLE__ && __APPLE_CC__) || __MWERKS__ */
  ;
}
#endif /* _NM_NEXTSTEP_H_ */

/* Attach to process PID, then initialize for debugging it: */
static void
child_attach(char *args, int from_tty)
{
  ProcessSerialNumber psn;
  ProcessInfoRec inforec;
  Str31 name;
  FSSpecPtr fsspec;
  OSType code;
  int pid;
  char *exec_file;

  if (!args) {
    error_no_arg("process-id to attach");
  }

  pid = atoi(args);

  psn.highLongOfPSN = 0;
  psn.lowLongOfPSN = pid;

  inforec.processInfoLength = sizeof(ProcessInfoRec);
  inforec.processName = name;
  inforec.processAppSpec = fsspec;

  if (GetProcessInformation(&psn, &inforec) == noErr)
    {
      if (from_tty)
	{
	  exec_file = (char *)get_exec_file(0);

	  if (exec_file) {
	    printf_unfiltered("Attaching to program `%s', %s\n", exec_file,
                              target_pid_to_str(pid_to_ptid(pid)));
	  } else {
	    printf_unfiltered("Attaching to %s\n",
                              target_pid_to_str(pid_to_ptid(pid)));
          }

	  gdb_flush(gdb_stdout);
	}
      /* Do we need to do anything special? */
      attach_flag = 1;
      inferior_ptid = pid_to_ptid(pid);
      push_target(&child_ops);
    }
}

static void
child_detach(char *args, int from_tty)
{
#if (defined(__APPLE__) && defined(__APPLE_CC__)) || defined(__MWERKS__)
# pragma unused (args)
#endif /* (__APPLE__ && __APPLE_CC__) || __MWERKS__ */
  char *exec_file;

  if (from_tty)
    {
      exec_file = get_exec_file(0);
      if (exec_file == 0) {
	exec_file = "";
      }
      printf_unfiltered("Detaching from program: %s %s\n", exec_file,
                        target_pid_to_str(inferior_ptid));
      gdb_flush(gdb_stdout);
    }
  inferior_ptid = null_ptid;
  unpush_target(&child_ops);
}

/* Print status information about what we are accessing: */
static void
child_files_info(struct target_ops *ignore)
{
#if (defined(__APPLE__) && defined(__APPLE_CC__)) || defined(__MWERKS__)
# pragma unused (ignore)
#endif /* (__APPLE__ && __APPLE_CC__) || __MWERKS__ */
  printf_unfiltered("\tUsing the running image of %s %s.\n",
                    (attach_flag ? "attached" : "child"),
                    target_pid_to_str(inferior_ptid));
}

/* ARGSUSED */
static void
child_open(char *arg, int from_tty)
{
#if (defined(__APPLE__) && defined(__APPLE_CC__)) || defined(__MWERKS__)
# pragma unused (arg, from_tty)
#endif /* (__APPLE__ && __APPLE_CC__) || __MWERKS__ */
  error("Use the \"run\" command to start a Mac application.");
}

/* Start an inferior Mac program and sets inferior_ptid to its pid.
 * EXEC_FILE is the file to run.
 * ALLARGS is a string containing the arguments to the program.
 * ENV is the environment vector to pass.  Errors reported with error(): */
static void
child_create_inferior(char *exec_file, char *allargs, char **env, int duh)
{
#if (defined(__APPLE__) && defined(__APPLE_CC__)) || defined(__MWERKS__)
# pragma unused (allargs, env, duh)
#endif /* (__APPLE__ && __APPLE_CC__) || __MWERKS__ */
  LaunchParamBlockRec launchparms;
  FSSpec fsspec;
  OSErr launch_err;

  if (!exec_file)
    {
      error("No executable specified, use `target exec'.\n");
    }

  launchparms.launchBlockID = extendedBlock;
  launchparms.launchEPBLength = extendedBlockLen;
  launchparms.launchFileFlags = 0;
  launchparms.launchControlFlags = (launchContinue | launchNoFileFlags);
  fsspec.vRefNum = 0;
  fsspec.parID = 0;
  strcpy((char *)(fsspec.name + 1), exec_file);
  fsspec.name[0] = strlen(exec_file);
  launchparms.launchAppSpec = &fsspec;
  launchparms.launchAppParameters = nil;

  launch_err = LaunchApplication(&launchparms);

  if (launch_err == 999 /*memFullErr*/)
    {
      error("Not enough memory to launch %s\n", exec_file);
    }
  else if (launch_err != noErr)
    {
      error("Error launching %s, code %d\n", exec_file, launch_err);
    }

  inferior_ptid = pid_to_ptid((int)launchparms.launchProcessSN.lowLongOfPSN);
  /* FIXME: be sure that high long of PSN is 0 */

  push_target (&child_ops);
  init_wait_for_inferior();
  clear_proceed_status();

#if 0
  proceed((CORE_ADDR)-1, TARGET_SIGNAL_0, 0);
#endif /* 0 */
}

static void
child_mourn_inferior(void)
{
  unpush_target(&child_ops);
  generic_mourn_inferior();
}

static void
child_stop(void)
{
  return;
}

#ifdef TARGET_H
int
child_xfer_memory(CORE_ADDR memaddr, gdb_byte *myaddr, int len,
                  int writeit, struct mem_attrib *attrib,
                  struct target_ops *target)
{
# if (defined(__APPLE__) && defined(__APPLE_CC__)) || defined(__MWERKS__)
#  pragma unused (attrib, target)
# endif /* (__APPLE__ && __APPLE_CC__) || __MWERKS__ */
  int i;

  for (i = 0; i < len; ++i) {
    if (writeit) {
      ((gdb_byte *)memaddr)[i] = myaddr[i];
    } else {
      myaddr[i] = ((gdb_byte *)memaddr)[i];
    }
  }
  return len;
}
#else
int
child_xfer_memory(CORE_ADDR memaddr, char *myaddr, int len, int write,
                  struct mem_attrib *attrib, struct target_ops *target)
{
  int i;

  for (i = 0; i < len; ++i)
    {
      if (write)
	{
	  ((char *)memaddr)[i] = myaddr[i];
	}
      else
	{
	  myaddr[i] = ((char *)memaddr)[i];
	}
    }
  return len;
}
#endif /* TARGET_H */

void
child_kill_inferior(void)
{
  return;
}

void
child_resume(ptid_t ptid, int step, enum target_signal signal_param)
{
#if (defined(__APPLE__) && defined(__APPLE_CC__)) || defined(__MWERKS__)
# pragma unused (ptid, step, signal_param)
#endif /* (__APPLE__ && __APPLE_CC__) || __MWERKS__ */
  return;
}

static void
child_prepare_to_store(void)
{
  ; /* Do nothing, since we can store individual regs */
}

static int
child_can_run(void)
{
  return 1;
}

/* ARGSUSED */
static void
child_close(int an_unused_arg)
{
#if (defined(__APPLE__) && defined(__APPLE_CC__)) || defined(__MWERKS__)
# pragma unused (an_unused_arg)
#endif /* (__APPLE__ && __APPLE_CC__) || __MWERKS__ */
  return;
}

static void
info_proc(char *args, int from_tty)
{
#if (defined(__APPLE__) && defined(__APPLE_CC__)) || defined(__MWERKS__)
# pragma unused (args, from_tty)
#endif /* (__APPLE__ && __APPLE_CC__) || __MWERKS__ */
  ProcessSerialNumber psn;
  ProcessInfoRec inforec;
  Str31 name;
  FSSpecPtr fsspec;
  OSType code;

  /* Eventually use args, but not right now: */
  psn.highLongOfPSN = 0;
  psn.lowLongOfPSN = kNoProcess;

  inforec.processInfoLength = sizeof(ProcessInfoRec);
  inforec.processName = name;
  inforec.processAppSpec = fsspec;

  printf_filtered("Process Name                     Sgnt Type    PSN   Loc Size FreeMem Time\n");

  while (GetNextProcess(&psn) == noErr)
    {
      if (GetProcessInformation(&psn, &inforec) == noErr)
	{
	  name[name[0] + 1] = '\0';
	  printf_filtered("%-32.32s", name + 1);
	  code = inforec.processSignature;
	  printf_filtered(" %luc%luc%luc%luc", ((code >> 24) & 0xff),
                          ((code >> 16) & 0xff), ((code >> 8) & 0xff),
                          ((code >> 0) & 0xff));
	  code = inforec.processType;
	  printf_filtered(" %luc%luc%luc%luc", ((code >> 24) & 0xff),
                          ((code >> 16) & 0xff), ((code >> 8) & 0xff),
                          ((code >> 0) & 0xff));
	  if (psn.highLongOfPSN == 0) {
	    printf_filtered(" %9lud", psn.lowLongOfPSN);
	  } else {
	    printf_filtered(" %9lud,%9lud\n",
                            psn.highLongOfPSN, psn.lowLongOfPSN);
          }
	  printf_filtered(" 0x%x", (unsigned int)inforec.processLocation);
	  printf_filtered(" %9lud", inforec.processSize);
	  printf_filtered(" %9lud", inforec.processFreeMem);
	  printf_filtered(" %9lud", inforec.processActiveTime);
	  printf_filtered("\n");
	}
    }
}

struct target_ops child_ops;

static void
init_child_ops(void)
{
  child_ops.to_shortname = "mac";
  child_ops.to_longname = "MacOS application";
  child_ops.to_doc = "MacOS application (started by the \"run\" command).";
  child_ops.to_open = child_open;
  child_ops.to_close = child_close;
  child_ops.to_attach = child_attach;
  child_ops.to_post_attach = NULL;
#ifndef TARGET_H
  child_ops.to_require_attach = NULL;	/* to_require_attach */
#endif /* !TARGET_H */
  child_ops.to_detach = child_detach;
#ifndef TARGET_H
  child_ops.to_require_detach = NULL;	/* to_require_detach */
#endif /* !TARGET_H */
  child_ops.to_resume = child_resume;
  child_ops.to_wait = (ptid_t (*)(ptid_t, struct target_waitstatus *, gdb_client_data))child_wait;
  child_ops.to_post_wait = NULL;	/* to_post_wait */
  child_ops.to_fetch_registers = child_fetch_inferior_registers;
  child_ops.to_store_registers = child_store_inferior_registers;
  child_ops.to_prepare_to_store = child_prepare_to_store;
#ifndef TARGET_H
  child_ops.to_xfer_memory = child_xfer_memory;
#endif /* !TARGET_H */
  child_ops.to_files_info = child_files_info;
  child_ops.to_insert_breakpoint = memory_insert_breakpoint;
  child_ops.to_remove_breakpoint = memory_remove_breakpoint;
  child_ops.to_terminal_init = 0;
  child_ops.to_terminal_inferior = 0;
  child_ops.to_terminal_ours_for_output = 0;
  child_ops.to_terminal_ours = 0;
  child_ops.to_terminal_info = 0;
  child_ops.to_kill = child_kill_inferior;
  child_ops.to_load = 0;
  child_ops.to_lookup_symbol = 0;
  child_ops.to_create_inferior = child_create_inferior;
  child_ops.to_post_startup_inferior = NULL; /* to_post_startup_inferior */
  child_ops.to_acknowledge_created_inferior = NULL; /* to_acknowledge_created_inferior */
#ifndef TARGET_H
  child_ops.to_clone_and_follow_inferior = NULL; /* to_clone_and_follow_inferior */
  child_ops.to_post_follow_inferior_by_clone = NULL; /* to_post_follow_inferior_by_clone */
#endif /* !TARGET_H */
  child_ops.to_insert_fork_catchpoint = NULL;
  child_ops.to_remove_fork_catchpoint = NULL;
  child_ops.to_insert_vfork_catchpoint = NULL;
  child_ops.to_remove_vfork_catchpoint = NULL;
#ifndef TARGET_H
  child_ops.to_has_forked = NULL;	/* to_has_forked */
  child_ops.to_has_vforked = NULL;	/* to_has_vforked */
  child_ops.to_can_follow_vfork_prior_to_exec = NULL;
  child_ops.to_post_follow_vfork = NULL;	/* to_post_follow_vfork */
#endif /* !TARGET_H */
  child_ops.to_insert_exec_catchpoint = NULL;
  child_ops.to_remove_exec_catchpoint = NULL;
#ifndef TARGET_H
  child_ops.to_has_execd = NULL;
#endif /* !TARGET_H */
  child_ops.to_reported_exec_events_per_exec_call = NULL;
  child_ops.to_has_exited = NULL;
  child_ops.to_mourn_inferior = child_mourn_inferior;
  child_ops.to_can_run = child_can_run;
  child_ops.to_notice_signals = 0;
  child_ops.to_thread_alive = 0;
  child_ops.to_stop = child_stop;
  child_ops.to_pid_to_exec_file = NULL;		/* to_pid_to_exec_file */
  child_ops.to_stratum = process_stratum;
#ifndef TARGET_H
  child_ops.DONT_USE = 0;
#endif /* !TARGET_H */
  child_ops.to_has_all_memory = 1;
  child_ops.to_has_memory = 1;
  child_ops.to_has_stack = 1;
  child_ops.to_has_registers = 1;
  child_ops.to_has_execution = 1;
  child_ops.to_sections = 0;
  child_ops.to_sections_end = 0;
  child_ops.to_magic = OPS_MAGIC;
}/*;*/

void
_initialize_mac_nat(void)
{
  init_child_ops();

  add_info("proc", info_proc,
           "Show information about processes.");
}

/* EOF */
