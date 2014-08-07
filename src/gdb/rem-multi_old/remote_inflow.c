/* remote_inflow.c
 * Low level interface to ptrace, for GDB when running under Unix.
 * Copyright (C) 1986, 1987 Free Software Foundation, Inc.
 */

#if defined(HAVE_CONFIG_H) && !defined(__CONFIG_H__)
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "defs.h"
#include "param_old.h" /* renamed from "param.h" */
#if !defined(_SYS_WAIT_H_)
# include "wait_old.h" /* renamed from "wait.h" */
#else
# include "gdb_wait.h"
#endif /* !_SYS_WAIT_H_ */
#include "frame.h"
#if 0
# include "inferior.h" /* some things in it conflict with things here */
# include "initialize.h"
#endif /* 0 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/param.h>
#include <sys/dir.h>
#include <sys/user.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <sgtty.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

/* Definitions: */
#if !defined(REGISTER_BYTES) && !defined(HAVE_DECL_REGISTER_BYTES)
/* arbitrarily made-up default: */
# define REGISTER_BYTES 0
#endif /* !REGISTER_BYTES && !HAVE_DECL_REGISTER_BYTES */

#ifndef MAX_REGISTER_RAW_SIZE
/* arbitrarily made-up default: */
# define MAX_REGISTER_RAW_SIZE 12
#endif /* !MAX_REGISTER_RAW_SIZE */

#ifndef PTRACE_TYPE_ARG3
# define PTRACE_TYPE_ARG3 caddr_t
#endif /* !PTRACE_TYPE_ARG3 */

#ifndef WRETCODE
# define WRETCODE(w) ((w) >> 8) /* same as WEXITSTATUS */
#endif /* !WRETCODE */

#ifndef gdb_assert
/* pared-down version from "../gdb-assert.h" */
# define gdb_assert(expr) ((void)((expr) ? 0 : -1))
#endif /* !gdb_assert */

/* parts of gdbarch.[c|h] we need: */
int gdbarch_debug = 0;

typedef int (gdbarch_deprecated_register_byte_ftype)(int reg_nr);

struct gdbarch
{
  int dummy;
  /* this is incomplete, we only use the members we need here: */
  gdbarch_deprecated_register_byte_ftype *deprecated_register_byte;
};

int generic_register_byte(int regnum)
{
  int byte;
  int i;
  gdb_assert((regnum >= 0) && (regnum < (16 + 71))); /* (16 + 71) = 87 */
  byte = 0;
  for ((i = 0); (i < regnum); i++) {
    byte++;
  }
  return byte;
}

struct gdbarch startup_gdbarch =
{
  0,
  /* deprecated_register_byte (currently the only actual member of this version of the struct) */
  generic_register_byte
};

#if !defined(current_gdbarch) || 1
struct gdbarch *current_gdbarch = &startup_gdbarch;
#endif /* !current_gdbarch || 1 */

int gdbarch_deprecated_register_byte(struct gdbarch *gdbarch, int reg_nr)
{
  gdb_assert(gdbarch != NULL);
  gdb_assert(gdbarch->deprecated_register_byte != NULL);
  /* Do not check predicate: gdbarch->deprecated_register_byte != generic_register_byte, allow call. */
  if (gdbarch_debug >= 2) {
    fprintf(stderr, "gdbarch_deprecated_register_byte called\n");
  }
  return gdbarch->deprecated_register_byte(reg_nr);
}

#if !defined(REGISTER_BYTE)
# if defined(DEPRECATED_REGISTER_BYTE)
#  define REGISTER_BYTE(reg_nr) DEPRECATED_REGISTER_BYTE(reg_nr)
# else
#  define REGISTER_BYTE(reg_nr) (gdbarch_deprecated_register_byte(current_gdbarch, reg_nr))
# endif /* DEPRECATED_REGISTER_BYTE */
#endif /* !REGISTER_BYTE */

/***************Begin MY defs*********************/
int quit_flag = 0;
char registers[REGISTER_BYTES];

/* Index within `registers' of the 1st byte of the space for register N. */

char buf2[MAX_REGISTER_RAW_SIZE];
/***************End MY defs*********************/

#ifdef NEW_SUN_PTRACE
# include <sys/ptrace.h>
# include <machine/reg.h>
#else
# if defined(HAVE_SYS_PTRACE_H)
#  include <sys/ptrace.h>
# endif /* HAVE_SYS_PTRACE_H */
# if !defined(PTRACE_GETREGS) && defined(PT_GETREGS)
#  define PTRACE_GETREGS PT_GETREGS
# endif /* !PTRACE_GETREGS && PT_GETREGS */
# if !defined(PTRACE_SETREGS) && defined(PT_SETREGS)
#  define PTRACE_SETREGS PT_SETREGS
# endif /* !PTRACE_SETREGS && PT_SETREGS */
# if !defined(PTRACE_GETFPREGS) && defined(PT_GETFPREGS)
#  define PTRACE_GETFPREGS PT_GETFPREGS
# endif /* !PTRACE_GETFPREGS && PT_GETFPREGS */
# if !defined(PTRACE_SETFPREGS) && defined(PT_SETFPREGS)
#  define PTRACE_SETFPREGS PT_SETFPREGS
# endif /* !PTRACE_SETFPREGS && PT_SETFPREGS */
#endif /* NEW_SUN_PTRACE */

/* global variables: */
extern char **environ;
extern int errno;
extern int inferior_pid;

/* prototypes: */
extern NORETURN void error(const char *fmt, ...) ATTR_NORETURN ATTR_FORMAT(printf, 1, 2);
void quit(void); /* unneeded? */
void perror_with_name(const char *errname);
extern int query(const char *fmt, ...) ATTR_FORMAT(printf, 1, 2);
void supply_register(void); /* unneeded? */
void write_register(int regno, int val);
CORE_ADDR read_register(int regno);
int store_inferior_registers(int regno);
int have_inferior_p(void);
void fetch_inferior_registers(void);
int write_inferior_memory(CORE_ADDR memaddr, char *myaddr, int len);

/* Nonzero if we are debugging an attached outside process
 * rather than an inferior. */


/* Start an inferior process and returns its pid.
 * ALLARGS is a vector of program-name and args.
 * ENV is the environment vector to pass.  */
int create_inferior (char ** allargs, char ** env)
{
  int pid;
  extern const int sys_nerr;
  extern const char *const sys_errlist[];
  extern int errno;

  /* exec is said to fail if the executable is open. */
  /****************close_exec_file ();*****************/

  pid = vfork();
  if (pid < 0) {
      perror_with_name("vfork");
  }

  if (pid == 0) {
      /* Run inferior in a separate process group. */
#ifdef SETPGRP_VOID
      setpgrp();
#else
      setpgrp(getpid(), getpid());
#endif /* SETPGRP_VOID */

/* Not needed on Sun, at least, and loses there because it clobbers
 * the superior: */
#if (!defined(__sun__) && !defined(sun)) && defined(SIGQUIT) && defined(SIGINT) && defined(SIG_DFL)
      signal(SIGQUIT, SIG_DFL);
      signal(SIGINT, SIG_DFL);
#endif /* (!__sun__ && !sun) && SIGQUIT && SIGINT && SIG_DFL */

      errno = 0;
#if 0
      /*FIXME ptrace() never takes only one argument: */
      ptrace(0);
#endif /* 0 */

      execle("/bin/sh", "sh", "-c", allargs, (char *)0, env);

      fprintf(stderr, "Cannot exec /bin/sh: %s.\n",
              ((errno < sys_nerr) ? sys_errlist[errno] : "unknown error"));
      fflush(stderr);
      _exit(0177);
  }
  return pid;
}

/* Kill the inferior process. Make us have no inferior: */
extern void kill_inferior(void)
{
  if (inferior_pid == 0) {
      return;
  }
  ptrace(8, inferior_pid, 0, 0);
  wait(0);
  /*************inferior_died();****VK**************/
}

/* Resume execution of the inferior process.
 * If STEP is nonzero, single-step it.
 * If SIGNAL is nonzero, give it that signal. */
unsigned char resume(int step, int signal, char *status)
{
  int pid ;
  WAITTYPE w;

  errno = 0;
  ptrace((step ? 9 : 7), inferior_pid, (PTRACE_TYPE_ARG3)1, signal);
  if (errno) {
    perror_with_name("ptrace");
  }
  pid = wait(&w);
  if (pid != inferior_pid) {
    perror_with_name("wait");
  }

  if (WIFEXITED(w)) {
    printf("\nchild exited with retcode = %x \n", WRETCODE(w));
    *status = 'E';
    return ((unsigned char)WRETCODE(w));
  } else if(!WIFSTOPPED(w)) {
    printf("\nchild did terminated with signal = %x \n", WTERMSIG(w));
    *status = 'T';
    return((unsigned char)WTERMSIG(w));
  } else {
    printf("\nchild stopped with signal = %x \n", WSTOPSIG(w));
    *status = 'S';
    return ((unsigned char)WSTOPSIG(w));
  }
}


#if defined(NEW_SUN_PTRACE) || \
    (defined(PTRACE_GETREGS) && defined(PTRACE_GETFPREGS) && defined(FPC_REGNUM) && defined(PTRACE_SETREGS) && defined(PTRACE_SETFPREGS))
void fetch_inferior_registers(void)
{
  struct regs inferior_registers;
  struct fp_status inferior_fp_registers;
  extern char registers[];

  ptrace(PTRACE_GETREGS, inferior_pid, &inferior_registers);
  if (errno) {
    perror_with_name("ptrace");
  }
  /**********debugging begin **********/
  print_some_registers(&inferior_registers);
  /**********debugging end **********/
  ptrace(PTRACE_GETFPREGS, inferior_pid, &inferior_fp_registers);
  if (errno) {
    perror_with_name("ptrace");
  }

  bcopy(&inferior_registers, registers, (16 * 4)); /* (16 * 4) = 64 */
  bcopy(&inferior_fp_registers, &registers[REGISTER_BYTE(FP0_REGNUM)],
        sizeof(inferior_fp_registers.fps_regs));
  *(int *)&registers[REGISTER_BYTE(PS_REGNUM)] = inferior_registers.r_ps;
  *(int *)&registers[REGISTER_BYTE(PC_REGNUM)] = inferior_registers.r_pc;
  bcopy(&inferior_fp_registers.fps_control,
        &registers[REGISTER_BYTE(FPC_REGNUM)],
        (sizeof(inferior_fp_registers) - sizeof(inferior_fp_registers.fps_regs)));
}

/* Store our register values back into the inferior.
 * If REGNO is -1, do this for all registers.
 * Otherwise, REGNO specifies which register (so we can save time). */
int store_inferior_registers(int regno)
{
  struct regs inferior_registers;
  struct fp_status inferior_fp_registers;
  extern char registers[];

  bcopy(registers, &inferior_registers, (16 * 4)); /* (16 * 4) = 64 */
  bcopy(&registers[REGISTER_BYTE(FP0_REGNUM)], &inferior_fp_registers,
        sizeof(inferior_fp_registers.fps_regs));
  inferior_registers.r_ps = *(int *)&registers[REGISTER_BYTE(PS_REGNUM)];
  inferior_registers.r_pc = *(int *)&registers[REGISTER_BYTE(PC_REGNUM)];
  bcopy(&registers[REGISTER_BYTE(FPC_REGNUM)],
        &inferior_fp_registers.fps_control,
        (sizeof(inferior_fp_registers) - sizeof(inferior_fp_registers.fps_regs)));

  ptrace(PTRACE_SETREGS, inferior_pid, &inferior_registers);
  if (errno) {
    perror_with_name("ptrace");
  }
  ptrace(PTRACE_SETFPREGS, inferior_pid, &inferior_fp_registers);
  if (errno) {
    perror_with_name("ptrace");
  }
  return 0;
}
#else
/* still need stub functions for these: */
void fetch_inferior_registers(void)
{
  if (errno) {
    perror_with_name("ptrace");
  }
}

int store_inferior_registers(int regno)
{
  if (errno == regno) {
    perror_with_name("ptrace: regno equals errno");
  } else if (errno) {
    perror_with_name("ptrace");
  }
  return 0;
}
#endif /* not NEW_SUN_PTRACE */


/* NOTE! I tried using PTRACE_READDATA, etc., to read and write memory
 * in the NEW_SUN_PTRACE case.
 * It ought to be straightforward. But it appears that writing did
 * not write the data that I specified. I cannot understand where
 * it got the data that it actually did write. */

/* Copy LEN bytes from inferior's memory starting at MEMADDR
 * to debugger memory starting at MYADDR: */
int read_inferior_memory(CORE_ADDR memaddr, char *myaddr, int len)
{
  register int i;
  register CORE_ADDR addr;
  register int count;
  register int *buffer;

  /* Round starting address down to longword boundary: */
  addr = (memaddr & - sizeof(int));
  /* Round ending address up; get number of longwords that makes: */
  count = ((((memaddr + len) - addr) + sizeof(int) - 1) / sizeof(int));
  /* Allocate buffer of that many longwords: */
  buffer = (int *)alloca(count * sizeof(int));

  /* Read all the longwords */
  for ((i = 0); (i < count); i++, (addr += sizeof(int))) {
    buffer[i] = ptrace(1, inferior_pid, (PTRACE_TYPE_ARG3)addr, 0);
  }

  /* Copy appropriate bytes out of the buffer: */
  bcopy((char *)buffer + (memaddr & (sizeof(int) - 1)), myaddr, len);

  return 0;
}

/* Copy LEN bytes of data from debugger memory at MYADDR to inferior's
 * memory at MEMADDR. On failure (cannot write the inferior) returns
 * the value of errno. */
int write_inferior_memory(CORE_ADDR memaddr, char *myaddr, int len)
{
  register int i;
  register CORE_ADDR addr;
  register int count;
  register int *buffer;
  extern int errno;

  /* Round starting address down to longword boundary: */
  addr = (memaddr & - sizeof(int));
  /* Round ending address up; get number of longwords that makes: */
  count = ((((memaddr + len) - addr) + sizeof(int) - 1) / sizeof(int));
  /* Allocate buffer of that many longwords: */
  buffer = (int *)alloca(count * sizeof(int));

  /* Fill start and end extra bytes of buffer with existing memory data: */
  buffer[0] = ptrace(1, inferior_pid, (PTRACE_TYPE_ARG3)addr, 0);

  if (count > 1) {
    buffer[(count - 1)] = ptrace(1, inferior_pid,
                                 (PTRACE_TYPE_ARG3)(addr + (count - 1) * sizeof(int)), 0);
  }

  /* Copy data to be written over corresponding part of buffer: */
  bcopy(myaddr, (char *)buffer + (memaddr & (sizeof(int) - 1)), len);

  /* Write the entire buffer: */
  for ((i = 0); (i < count); i++, (addr += sizeof(int))) {
      errno = 0;
      ptrace(4, inferior_pid, (PTRACE_TYPE_ARG3)addr, buffer[i]);
      if (errno) {
          return errno;
      }
  }

  return 0;
}


void try_writing_regs_command(void)
{
  register int i;
  register int value;
  extern int errno;

  if (inferior_pid == 0) {
    error("There is no inferior process now.");
  }

  fetch_inferior_registers();
  for ((i = 0); (i < 18); i ++) {
      QUIT;
      errno = 0;
      value = read_register(i);
      write_register(i, value);
      if (errno == 0) {
        printf(" Succeeded with register %d; value 0x%x (%d).\n",
               i, value, value);
      } else {
        printf(" Failed with register %d.\n", i);
      }
  }
}

void initialize(void)
{
  inferior_pid = 0;
}


/* Return the contents of register REGNO, regarding it as an integer: */
CORE_ADDR read_register(int regno)
{
  /* This loses when (REGISTER_RAW_SIZE(regno) != sizeof(int)): */
  return *(int *)&registers[REGISTER_BYTE(regno)];
}

/* Store VALUE in the register number REGNO, regarded as an integer: */
void write_register(int regno, int val)
{
  /* This loses when (REGISTER_RAW_SIZE(regno) != sizeof(int)): */
  *(int *)&registers[REGISTER_BYTE(regno)] = val;

  if (have_inferior_p()) {
    store_inferior_registers(regno);
  }
}


int have_inferior_p(void)
{
  return (inferior_pid != 0);
}

int print_some_registers(int regs[])
{
  register int i;
  for ((i = 0); (i < 18); i++) {
    printf("reg[%d] = %x\n", i, regs[i]);
  }
  return 0;
}

/* EOF */
