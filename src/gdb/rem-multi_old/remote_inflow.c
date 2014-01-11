/* remote_inflow.c
 * Low level interface to ptrace, for GDB when running under Unix.
 * Copyright (C) 1986, 1987 Free Software Foundation, Inc.
 */

#include "defs.h"
#include "param_old.h" /* renamed from "param.h" */
#include "wait.h"
#include "frame.h"
#include "inferior.h"
#if 0
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

/***************Begin MY defs*********************/ 
int quit_flag = 0; 
char registers[REGISTER_BYTES]; 

/* Index within `registers' of the first byte of the space for
 * register N.
 */


char buf2[MAX_REGISTER_RAW_SIZE];
/***************End MY defs*********************/ 

#ifdef NEW_SUN_PTRACE
# include <sys/ptrace.h>
# include <machine/reg.h>
#endif /* NEW_SUN_PTRACE */

extern char **environ; 
extern int errno;
extern int inferior_pid; 
void error(), quit(), perror_with_name();
int query(); 
void supply_register(), write_register(); 
CORE_ADDR read_register(); 

/* Nonzero if we are debugging an attached outside process
   rather than an inferior.  */


/* Start an inferior process and returns its pid.
   ALLARGS is a vector of program-name and args.
   ENV is the environment vector to pass.  */

int
create_inferior (allargs, env)
     char **allargs;
     char **env;
{
  int pid;
  extern int sys_nerr;
  extern char *sys_errlist[];
  extern int errno;

  /* exec is said to fail if the executable is open.  */
  /****************close_exec_file ();*****************/ 

  pid = vfork ();
	if (pid < 0) {
		perror_with_name ("vfork");
	}

  if (pid == 0)
    {
      /* Run inferior in a separate process group.  */
      setpgrp (getpid (), getpid ());

/* Not needed on Sun, at least, and loses there
   because it clobbers the superior.  */
/*???      signal (SIGQUIT, SIG_DFL);
      signal (SIGINT, SIG_DFL);  */

	  errno = 0; 
      ptrace (0);

      execle ("/bin/sh", "sh", "-c", allargs, 0, env);

      fprintf (stderr, "Cannot exec /bin/sh: %s.\n",
	       errno < sys_nerr ? sys_errlist[errno] : "unknown error");
      fflush (stderr);
      _exit (0177);
    }
  return pid;
}

/* Kill the inferior process.  Make us have no inferior.  */

kill_inferior ()
{
  if (inferior_pid == 0)
    return;
  ptrace (8, inferior_pid, 0, 0);
  wait (0);
  /*************inferior_died ();****VK**************/ 
}

/* Resume execution of the inferior process.
   If STEP is nonzero, single-step it.
   If SIGNAL is nonzero, give it that signal.  */

unsigned char
resume (step, signal,status)
     int step;
     int signal;
	 char *status; 
{
	int pid ; 
	WAITTYPE w; 

  	errno = 0;
    ptrace (step ? 9 : 7, inferior_pid, 1, signal);
    if (errno)
		perror_with_name ("ptrace");
	pid = wait(&w); 
	if(pid != inferior_pid) 
		perror_with_name ("wait"); 

	if(WIFEXITED(w))
	{
		printf("\nchild exited with retcode = %x \n",WRETCODE(w)); 
		*status = 'E'; 
		return((unsigned char) WRETCODE(w));
	} 
	else if(!WIFSTOPPED(w))
	{
		printf("\nchild did terminated with signal = %x \n",WTERMSIG(w)); 
		*status = 'T'; 
		return((unsigned char) WTERMSIG(w)); 
	} 
	else 
	{
		printf("\nchild stopped with signal = %x \n",WSTOPSIG(w)); 
		*status = 'S'; 
		return((unsigned char) WSTOPSIG(w)); 
	} 
		 
}


#ifdef NEW_SUN_PTRACE

void
fetch_inferior_registers ()
{
  struct regs inferior_registers;
  struct fp_status inferior_fp_registers;
  extern char registers[];

      ptrace (PTRACE_GETREGS, inferior_pid, &inferior_registers);
      if (errno)
		perror_with_name ("ptrace");
	  /**********debugging begin **********/ 
	  print_some_registers(&inferior_registers); 
	  /**********debugging end **********/ 
      ptrace (PTRACE_GETFPREGS, inferior_pid, &inferior_fp_registers);
      if (errno)
		perror_with_name ("ptrace");

      bcopy (&inferior_registers, registers, 16 * 4);
      bcopy (&inferior_fp_registers, &registers[REGISTER_BYTE (FP0_REGNUM)],
	     sizeof inferior_fp_registers.fps_regs);
      *(int *)&registers[REGISTER_BYTE (PS_REGNUM)] = inferior_registers.r_ps;
      *(int *)&registers[REGISTER_BYTE (PC_REGNUM)] = inferior_registers.r_pc;
      bcopy (&inferior_fp_registers.fps_control,
	     &registers[REGISTER_BYTE (FPC_REGNUM)],
	     sizeof inferior_fp_registers - sizeof inferior_fp_registers.fps_regs);
}

/* Store our register values back into the inferior.
   If REGNO is -1, do this for all registers.
   Otherwise, REGNO specifies which register (so we can save time).  */

store_inferior_registers (regno)
     int regno;
{
  struct regs inferior_registers;
  struct fp_status inferior_fp_registers;
  extern char registers[];

      bcopy (registers, &inferior_registers, 16 * 4);
      bcopy (&registers[REGISTER_BYTE (FP0_REGNUM)], &inferior_fp_registers,
	     sizeof inferior_fp_registers.fps_regs);
      inferior_registers.r_ps = *(int *)&registers[REGISTER_BYTE (PS_REGNUM)];
      inferior_registers.r_pc = *(int *)&registers[REGISTER_BYTE (PC_REGNUM)];
      bcopy (&registers[REGISTER_BYTE (FPC_REGNUM)],
	     &inferior_fp_registers.fps_control,
	     sizeof inferior_fp_registers - sizeof inferior_fp_registers.fps_regs);

      ptrace (PTRACE_SETREGS, inferior_pid, &inferior_registers);
      if (errno)
		perror_with_name ("ptrace");
      ptrace (PTRACE_SETFPREGS, inferior_pid, &inferior_fp_registers);
      if (errno)
		perror_with_name ("ptrace");
}

#endif /* not NEW_SUN_PTRACE */


/* NOTE! I tried using PTRACE_READDATA, etc., to read and write memory
   in the NEW_SUN_PTRACE case.
   It ought to be straightforward.  But it appears that writing did
   not write the data that I specified.  I cannot understand where
   it got the data that it actually did write.  */

/* Copy LEN bytes from inferior's memory starting at MEMADDR
   to debugger memory starting at MYADDR.  */

read_inferior_memory (memaddr, myaddr, len)
     CORE_ADDR memaddr;
     char *myaddr;
     int len;
{
  register int i;
  /* Round starting address down to longword boundary.  */
  register CORE_ADDR addr = memaddr & - sizeof (int);
  /* Round ending address up; get number of longwords that makes.  */
  register int count
    = (((memaddr + len) - addr) + sizeof (int) - 1) / sizeof (int);
  /* Allocate buffer of that many longwords.  */
  register int *buffer = (int *) alloca (count * sizeof (int));

  /* Read all the longwords */
  for (i = 0; i < count; i++, addr += sizeof (int))
    {
	buffer[i] = ptrace (1, inferior_pid, addr, 0);
    }

  /* Copy appropriate bytes out of the buffer.  */
  bcopy ((char *) buffer + (memaddr & (sizeof (int) - 1)), myaddr, len);
}

/* Copy LEN bytes of data from debugger memory at MYADDR
   to inferior's memory at MEMADDR.
   On failure (cannot write the inferior)
   returns the value of errno.  */

int
write_inferior_memory (memaddr, myaddr, len)
     CORE_ADDR memaddr;
     char *myaddr;
     int len;
{
  register int i;
  /* Round starting address down to longword boundary.  */
  register CORE_ADDR addr = memaddr & - sizeof (int);
  /* Round ending address up; get number of longwords that makes.  */
  register int count
    = (((memaddr + len) - addr) + sizeof (int) - 1) / sizeof (int);
  /* Allocate buffer of that many longwords.  */
  register int *buffer = (int *) alloca (count * sizeof (int));
  extern int errno;

  /* Fill start and end extra bytes of buffer with existing memory data.  */

    buffer[0] = ptrace (1, inferior_pid, addr, 0);

  if (count > 1)
    {
	buffer[count - 1]
	  = ptrace (1, inferior_pid,
		    addr + (count - 1) * sizeof (int), 0);
    }

  /* Copy data to be written over corresponding part of buffer */

  bcopy (myaddr, (char *) buffer + (memaddr & (sizeof (int) - 1)), len);

  /* Write the entire buffer.  */

  for (i = 0; i < count; i++, addr += sizeof (int))
    {
      errno = 0;
	ptrace (4, inferior_pid, addr, buffer[i]);
		if (errno) {
			return errno;
		}
    }

  return 0;
}

void
try_writing_regs_command ()
{
  register int i;
  register int value;
  extern int errno;

  if (inferior_pid == 0)
    error ("There is no inferior process now.");

  fetch_inferior_registers(); 
  for (i = 0;i<18 ; i ++)
    {
      QUIT;
      errno = 0;
      value = read_register(i); 
      write_register ( i, value);
      if (errno == 0)
	{
	  	printf (" Succeeded with register %d; value 0x%x (%d).\n",
		  i, value, value);
	}
      else 
		printf (" Failed with register %d.\n", i);
    }
}

void
initialize ()
{

  inferior_pid = 0;

}


/* Return the contents of register REGNO,
   regarding it as an integer.  */

CORE_ADDR
read_register (regno)
     int regno;
{
  /* This loses when REGISTER_RAW_SIZE (regno) != sizeof (int) */
  return *(int *) &registers[REGISTER_BYTE (regno)];
}

/* Store VALUE in the register number REGNO, regarded as an integer.  */

void
write_register (regno, val)
     int regno, val;
{
  /* This loses when REGISTER_RAW_SIZE (regno) != sizeof (int) */
  *(int *) &registers[REGISTER_BYTE (regno)] = val;

	if (have_inferior_p ()) {
		store_inferior_registers (regno);
	}
}


int
have_inferior_p ()
{
  return inferior_pid != 0;
}

print_some_registers(regs)
int regs[];
{
   register int i;
   for (i = 0; i < 18; i++) {
	 printf("reg[%d] = %x\n", i, regs[i]);
	 }
}

/* EOF */
