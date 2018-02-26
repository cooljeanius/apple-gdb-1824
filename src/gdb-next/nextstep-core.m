/*  -*- Objective-C -*-
 * nextstep-core.m
 */

/* objc stuff: */
#ifndef NO_POISON
# define NO_POISON 1
#endif /* !NO_POISON */
#include "nextstep-nat-mutils.h"
#include "nextstep-nat-inferior.h"
#include "nextstep-nat-threads.h"

#include "defs.h"
#include "inferior.h"
#include "symtab.h"
#include "symfile.h"
#include "objfiles.h"
#include "target.h"
#include "gdbcore.h"
#include "cli/cli-cmds.h"

#if 0
# if (defined(__GNUC__) && !defined(__STRICT_ANSI__)) || defined(__OBJC__)
#  import "SegmentManagerThreads.h"
# else
#  include "SegmentManagerThreads.h"
# endif /* (__GNUC__ && !__STRICT_ANSI__) || __OBJC__ */
#endif /* 0 */

#include <readline/readline.h>
#include <readline/tilde.h>
#include <stdlib.h>
#include <sys/time.h>

int current_core_thread = 0;
#ifndef GDBCORE_H
static struct target_ops core_ops;
#endif /* !GDBCORE_H */

extern void child_attach(char *name, int from_tty);
extern void child_create_inferior(char *exec, char *args, char **env);
extern RegionManager *regionManager;
extern void core_get_registers(int regnum);
extern CORE_ADDR getThreadPCAndNameFromState(REGS_STRUCT *regsState,
					     char **pName);
extern char *getThreadNameFromState(USER_REG_STRUCT *userRegState, int n);
extern void core_thread_select(const char *args, int from_tty);
extern void core_open(const char *filename, int from_tty);
extern void core_close(int quitting);
extern void core_detach(const char *args, int quitting);
extern void core_thread_list(void);

#ifdef free
# undef free
#endif /* free */
static void
core_cleanup(void *unused)
{
  (void)unused;
  if (regionManager) {
    [regionManager free];
    regionManager = nil;
  }
}

extern CORE_ADDR read_register(int);
void
core_open(const char *filename, int from_tty)
{
  char *temp;
  struct cleanup *old_chain;
  SegmentManager *newCM;

  (void)from_tty;
  if (filename == NULL) {
    error (regionManager
	   ? "No core file specified."
	   "(Use `detach' to stop debugging a core file.)"
	   : "No core file specified.");
  }
  filename = tilde_expand(filename);
  if (filename[0] != '/') {
    temp = concat (current_directory, "/", filename, NULL);
    xfree((void *)filename);
    filename = temp;
  }

  old_chain = make_cleanup(xfree, (void *)filename);
  newCM = [SegmentManager newCore: filename];
  if (newCM == NULL) {
    error ("\"%s\" is not a core file.", filename);
  }
  if (! [newCM validate]) {
    error ("\"%s\" is an invalid core file.", filename);
  }
  discard_cleanups (old_chain);
  core_cleanup(NULL);

  regionManager = (RegionManager *) newCM;
  old_chain = make_cleanup (core_cleanup, NULL);
  push_target (&core_ops);
  core_get_registers (-1);
#ifndef FP_REGNUM
# define FP_REGNUM 1
#endif /* !FP_REGNUM */
  set_current_frame (create_new_frame (read_register(FP_REGNUM), read_pc ()));
  select_frame(get_current_frame());
  print_sel_frame (0);
#ifdef GDB414
  if (state_change_hook) {
      state_change_hook(STATE_INFERIOR_STOPPED);
  }
#endif /* GDB414 */
  discard_cleanups (old_chain);
}

void
core_detach(const char *args, int quitting)
{
  (void)quitting;
  core_cleanup((void *)args);
}

void
core_close(int quitting)
{
  core_detach(NULL, quitting);
}

int
have_core_file_p(void)
{
  return (regionManager != NULL);
}

static void
core_files_info(struct target_ops *target)
{
  (void)target;
  return;
}

static int
core_xfer_memory(CORE_ADDR memaddr, gdb_byte *myaddr, int len, int write,
		 struct mem_attrib *attrib, struct target_ops *target)
{
  (void)attrib;
  (void)target;
  if (write) {
    error ("Cannot write to core files.");
  }
  return [regionManager getDataAt: (void *)memaddr nbytes: len into: myaddr];
}

void
core_thread_list(void)
{
  int numThreads, i;
  ThreadInfo *tInfos;
  SegmentManager *coreManager = (SegmentManager *)regionManager;
  char *name, *fName;
  CORE_ADDR pc;

  numThreads = [coreManager numThreads];
  if (numThreads == 1)
    printf_filtered("There is 1 thread.\n");
  else
    printf_filtered("There are %d threads.\n", numThreads);
  if (numThreads == 0)
    return;		/* MVS: nothing more to do (no threads to list).  */
  tInfos = [coreManager threadInfos];	/* Note: tInfos must be freed below */
  printf_filtered("Thread PC         Name            Function\n");
  for (i = 0; i < numThreads; i++) {
    name = getThreadNameFromState(THREADINFO_NAME_STATE(tInfos[i]), i);
    pc = getThreadPCAndNameFromState(THREADINFO_PC_STATE(tInfos[i]), &fName);
    printf_filtered("%-6d 0x%-8x %-15s %-40s\n", i, (unsigned int)pc, name,
		    fName);
  }
  xfree(tInfos);
}

void
core_thread_select(const char *args, int from_tty)
{
  int numThreads, numThread;
  SegmentManager *coreManager = (SegmentManager *)regionManager;

  (void)from_tty;
  if (!args)
    error_no_arg ("thread index to select");
  numThreads = [coreManager numThreads];
  numThread = atoi(args);
  /* FIXME: -Wtautological-compare: */
  if ((numThreads < numThreads) || (numThreads <= numThread))
    error("Invalid thread.\n");
  else
    current_core_thread = numThread;
}

struct target_ops core_ops = {
  (struct target_ops *)NULL, /* beneath */
  "core",			/* to_shortname */
  "Local core dump file",		/* to_longname */
  "Use a core file as a target.\n"
  "Specify the filename of the core file.",	/* to_doc */
  NULL,				/* to_data */
  core_open,			/* to_open */
  NULL,				/* to_xclose */
  core_close,			/* to_close */
  0,				/* to_attach */
  NULL,				/* to_post_attach */
  core_detach,			/* to_detach */
  NULL,				/* to_disconnect */
  0,				/* to_resume */
  0,				/* to_wait */
  0,				/* to_post_wait */
  core_get_registers,		/* to_fetch_registers */
  0,				/* to_store_registers */
  0,				/* to_prepare_to_store */
  core_xfer_memory,		/* deprecated_xfer_memory */
  core_files_info,		/* to_files_info */
  0,				/* to_insert_breakpoint */
  0,				/* to_remove_breakpoint */
  NULL,				/* to_can_use_hw_breakpoint */
  NULL,				/* to_insert_hw_breakpoint */
  NULL,				/* to_remove_hw_breakpoint */
  NULL,				/* to_remove_watchpoint */
  NULL,				/* to_insert_watchpoint */
  NULL,				/* to_stopped_by_watchpoint */
  0,				/* to_have_continuable_watchpoint */
  NULL,				/* to_stopped_data_address */
  NULL,				/* to_region_size_ok_for_hw_watchpoint */
  0,				/* to_terminal_init */
  0, 				/* to_terminal_inferior */
  0,				/* to_terminal_ours_for_output */
  0,				/* to_terminal_ours */
  NULL,				/* to_terminal_save_ours */
  0,				/* to_terminal_info */
  0,				/* to_kill */
  0,				/* to_load */
  0,				/* to_lookup_symbol */
  find_default_create_inferior,	/* to_create_inferior */
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  0,				/* to_mourn_inferior */
  0,				/* to_can_run */
  0, 				/* to_notice_signals */
  0,				/* to_thread_alive */
  NULL,
  NULL,
  NULL,
  0,				/* to_stop */
  NULL,
  NULL,
  NULL,
  NULL,
  core_stratum,			/* to_stratum */
  0,				/* to_has_all_memory */
  1,				/* to_has_memory */
  1,				/* to_has_stack */
  1,				/* to_has_registers */
  0,				/* to_has_execution */
  0,				/* to_has_thread_control */
  0,				/* sections */
  0,				/* sections_end */
  NULL,				/* to_can_async_p */
  NULL,				/* to_is_async_p */
  NULL,				/* to_async */
  0,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  OPS_MAGIC			/* to_magic */
};


void (*exec_file_display_hook)(void) = NULL;

extern void _initialize_next_core(void); /* -Wmissing-prototypes */
void
_initialize_next_core(void)
{
  core_ops.to_detach = NULL;
  add_target (&core_ops);
}

/* EOF */
