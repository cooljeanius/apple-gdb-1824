/* Interface to bare machine for GDB running as kernel debugger.
   Copyright (C) 1986, 1989, 1991 Free Software Foundation, Inc.

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
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  */

#include <stdio.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <sys/types.h>
#include "gdb_stat.h"

#if defined(SIGTSTP) && defined(SIGIO)
# include <sys/time.h>
# include <sys/resource.h>
#endif /* SIGTSTP and SIGIO defined (must be 4.2) */

#include "defs.h"
#include "signals.h"
#include "symtab.h"
#include "frame.h"
#include "inferior.h"
#include "wait.h"
#include "gdb_wait.h"
#include "tm.h"
#include "xm.h"
#include "gdbarch.h"

#ifndef REGISTER_TYPE
# ifdef DEPRECATED_REGISTER_VIRTUAL_TYPE
#  define REGISTER_TYPE DEPRECATED_REGISTER_VIRTUAL_TYPE
# else
#  define REGISTER_TYPE unsigned int
# endif /* DEPRECATED_REGISTER_VIRTUAL_TYPE */
#endif /* !REGISTER_TYPE */

#ifndef FAULT_CODE_UNITS
# define FAULT_CODE_UNITS 4 /* value used by all xm-*.h currently present */
#endif /* !FAULT_CODE_UNITS */

extern void fault(void);
extern void save_frame_pointer(CORE_ADDR);
extern void save_registers(int);
extern void restore_gdb(void);
extern void display_string(char *);
extern int tty_input(void);

extern void _initialize_standalone(void);

/* Random system calls, mostly no-ops to prevent link problems  */
int
ioctl(int desc, unsigned long code, ...)
{
  return desc;
}

RETSIGTYPE (* signal ()) ()
{}

int
kill(pid_t unused1, int unused2)
{
  return 0;
}

int
getpid(void)
{
  return 0;
}

int
sigsetmask(int mask)
{
  return mask;
}

int
chdir(const char *path)
{
  return 0;
}

char *
getcwd(char *buf, size_t len)
{
  buf[0] = '/';
  buf[1] = 0;
  return buf;
}

/* Used to check for existence of .gdbinit.  Say no: */
int access()
{
  return -1;
}

void exit(int ignored)
{
  error("Fatal error; restarting.");
}

/* Reading "files".  The contents of some files are written into kdb's
   data area before it is run.  These files are used to contain the
   symbol table for kdb to load, and the source files (in case the
   kdb user wants to print them).  The symbols are stored in a file
   named "kdb-symbols" in a.out format (except that all the text and
   data have been stripped to save room).

   The files are stored in the following format:
   int     number of bytes of data for this file, including these four.
   char[]  name of the file, ending with a null.
   padding to multiple of 4 boundary.
   char[]  file contents.  The length can be deduced from what was
           specified before.  There is no terminating null here.

   If the int at the front is zero, it means there are no more files.

   Opening a file in kdb returns a nonzero value to indicate success,
   but the value does not matter.  Only one file can be open, and only
   for reading.  All the primitives for input from the file know
   which file is open and ignore what is specified for the descriptor
   or for the stdio stream.

   Input with fgetc can be done either on the file that is open
   or on stdin (which reads from the terminal through tty_input ()  */

/* Address of data for the files stored in format described above.  */
char *files_start;

/* The file stream currently open:  */

char *sourcebeg;		/* beginning of contents */
int sourcesize;			/* size of contents */
char *sourceptr;		/* current read pointer */
int sourceleft;			/* number of bytes to eof */

/* "descriptor" for the file now open.
   Incremented at each close.
   If specified descriptor does not match this,
   it means the program is trying to use a closed descriptor.
   We report an error for that.  */

int sourcedesc;

#ifndef STRCMP
# define STRCMP(foo, bar) strcmp(foo, bar)
#endif /* !STRCMP */

int
open(const char *filename, int modes, ...)
{
  register char *next;

  if (modes)
    {
      errno = EROFS;
      return -1;
    }

  if (sourceptr)
    {
      errno = EMFILE;
      return -1;
    }

  for (next = files_start; *(int *)next; next += *(int *)next)
    {
      if (!STRCMP((next + 4), filename))
	{
	  sourcebeg = (next + 4 + strlen(next + 4) + 1);
	  sourcebeg = (char *)(((int)sourcebeg + 3) & (-4));
	  sourceptr = sourcebeg;
	  sourcesize = (next + *(int *)next - sourceptr);
	  sourceleft = sourcesize;
	  return sourcedesc;
	}
    }
  return 0;
}

int
close(int desc)
{
  sourceptr = 0;
  sourcedesc++;
  /* Do NOT let sourcedesc get big enough to be confused with stdin: */
  if (sourcedesc == 100) {
    sourcedesc = 5;
  }
  return 0;
}

FILE *
fopen(const char *filename, const char *modes)
{
  return (FILE *)open(filename, (*modes == 'w'));
}

FILE *
fdopen(int desc, const char *unused)
{
  return (FILE *)desc;
}

int
fclose(FILE *desc)
{
  return close(fileno(desc));
}

int
fstat(desc, statbuf)
     struct stat *statbuf;
{
  if (desc != sourcedesc)
    {
      errno = EBADF;
      return -1;
    }
  statbuf->st_size = sourcesize;
  return 0;
}

int
myread(int desc, char *destptr, int size)
{
  int len = min(sourceleft, size);

  if (desc != sourcedesc)
    {
      errno = EBADF;
      return -1;
    }

  memcpy(destptr, sourceptr, len);
  sourceleft -= len;
  return len;
}

size_t
fread(void *restrict bufp, size_t numelts, size_t eltsize,
      FILE *restrict stream)
{
  register int elts = min(numelts, (sourceleft / eltsize));
  register int len = (elts * eltsize);

  if (fileno(stream) != sourcedesc)
    {
      errno = EBADF;
      return -1;
    }

  memcpy(bufp, sourceptr, len);
  sourceleft -= len;
  return elts;
}

int
fgetc(FILE *desc)
{
  if (desc == stdin)
    return tty_input();

  if (fileno(desc) != sourcedesc)
    {
      errno = EBADF;
      return -1;
    }

  if (sourceleft-- <= 0)
    return EOF;
  return *sourceptr++;
}

off_t
lseek(int desc, off_t pos, int whence)
{

  if (desc != sourcedesc)
    {
      errno = EBADF;
      return -1;
    }

  if ((pos < 0) || (pos > sourcesize))
    {
      errno = EINVAL;
      return -1;
    }

  sourceptr = (sourcebeg + pos);
  sourceleft = (sourcesize - pos);
  return sourceleft;
}

/* Output in kdb can go only to the terminal, so the stream
   specified may be ignored.  */
int
printf(a1, a2, a3, a4, a5, a6, a7, a8, a9)
{
  char buffer[1024];
  snprintf(buffer, sizeof(buffer), a1, a2, a3, a4, a5, a6, a7, a8, a9);
  display_string(buffer);
}

int
fprintf(ign, a1, a2, a3, a4, a5, a6, a7, a8, a9)
{
  char buffer[1024];
  snprintf(buffer, sizeof(buffer), a1, a2, a3, a4, a5, a6, a7, a8, a9);
  display_string(buffer);
}

size_t
fwrite(const void *restrict buf, size_t numelts, size_t size, FILE *stream)
{
  register int i = (numelts * size);
  while (i-- > 0)
    fputc(*buf++, stream);
}

int
fputc(int c, FILE *ign)
{
  char buf[2];
  buf[0] = (char)c;
  buf[1] = 0;
  display_string(buf);
}

/* sprintf refers to this, but loading this from the
   library would cause fflush to be loaded from it too.
   In fact there should be no need to call this (I hope).  */
static void
_flsbuf()
{
  error("_flsbuf was actually called.");
}

int
fflush(FILE *ign)
{
}

/* Entries into core and inflow, needed only to make things link ok.  */
static void
exec_file_command(void)
{}

static void
core_file_command(void)
{}

static const char *
get_exec_file(int err)
{
  /* Makes one printout look reasonable; value does not matter otherwise: */
  return "run";
}

/* Nonzero if there is a core file: */
static int
have_core_file_p(void)
{
  return 0;
}

static void
kill_command(void)
{
  inferior_pid = 0;
}

void
terminal_inferior(void)
{}

void
terminal_ours(void)
{}

void
terminal_init_inferior(void)
{}

static void
write_inferior_register(void)
{}

static void
read_inferior_register(void)
{}

static void
read_memory(CORE_ADDR memaddr, char *myaddr, int len)
{
  memcpy(myaddr, (void *)(uintptr_t)memaddr, len);
}

/* Always return 0 indicating success.  */
static int
write_memory(CORE_ADDR memaddr, char *myaddr, int len)
{
  memcpy((void *)(uintptr_t)memaddr, myaddr, len);
  return 0;
}

#ifndef NUM_REGS
# define NUM_REGS 10 /* smallest of any defs currently in tm-*.h files */
#endif /* !NUM_REGS */
static REGISTER_TYPE saved_regs[NUM_REGS];

static REGISTER_TYPE
read_register(int regno)
{
  if ((regno < 0) || (regno >= NUM_REGS))
    error("Register number %d out of range.", regno);
  return saved_regs[regno];
}

static void
write_register(int regno, REGISTER_TYPE value)
{
  if ((regno < 0) || (regno >= NUM_REGS))
    error("Register number %d out of range.", regno);
  saved_regs[regno] = value;
}

/* System calls needed in relation to running the "inferior".  */

int vfork()
{
  /* Just appear to "succeed".  Say the inferior's pid is 1.  */
  return 1;
}

/* These are called by code that normally runs in the inferior
   that has just been forked.  That code never runs, when standalone,
   and these definitions are so it will link without errors.  */
static void
ptrace(void)
{}

pid_t
setpgrp(void)
{}

int
execle(const char *path, const char *arg0, ...)
{}

void
_exit(int unused)
{}

/* Malloc calls these.  */
static void
malloc_warning(char *str)
{
  printf("\n%s.\n\n", str);
}

char *next_free;
char *memory_limit;

void *
sbrk(int amount)
{
  if ((next_free + amount) > memory_limit)
    return (void *)-1;
  next_free += amount;
  return (next_free - amount);
}

/* Various ways malloc might ask where end of memory is.  */

static char *
ulimit(void)
{
  return memory_limit;
}

static int
vlimit(void)
{
  return (memory_limit - next_free);
}

int
getrlimit(int unused, struct rlimit *addr)
{
  addr->rlim_cur = (memory_limit - next_free);
}

/* Context switching to and from program being debugged.  */

/* GDB calls here to run the user program.
   The frame pointer for this function is saved in
   gdb_stack by save_frame_pointer; then we restore
   all of the user program's registers, including PC and PS.  */

static int fault_code;
static REGISTER_TYPE gdb_stack;

void
resume(int unused1, enum target_signal unused2)
{
  REGISTER_TYPE restore[NUM_REGS];

#ifdef PUSH_FRAME_PTR
  PUSH_FRAME_PTR;
#endif /* PUSH_FRAME_PTR */
  save_frame_pointer(INVALID_ADDRESS);

  memcpy(restore, saved_regs, sizeof(restore));
#ifdef POP_REGISTERS
  POP_REGISTERS;
#else
  return;
#endif /* POP_REGISTERS */
  /* Control does not drop through here!  */
}

void
save_frame_pointer(CORE_ADDR val)
{
  gdb_stack = val;
}

/* Fault handlers call here, running in the user program stack.
   They must first push a fault code,
   old PC, old PS, and any other info about the fault.
   The exact format is machine-dependent and is known only
   in the definition of PUSH_REGISTERS.  */
void
fault(void)
{
  /* Transfer all registers and fault code to the stack
     in canonical order: registers in order of GDB register number,
     followed by fault code.  */
#ifdef PUSH_REGISTERS
  PUSH_REGISTERS;
#endif /* PUSH_REGISTERS */

  /* Transfer them to saved_regs and fault_code: */
  save_registers(0);

  restore_gdb();
  /* Control does not reach here */
}

void
restore_gdb(void)
{
  CORE_ADDR new_fp = gdb_stack;
  /* Switch to GDB's stack  */
#ifdef POP_FRAME_PTR
  POP_FRAME_PTR;
#else
  (void)new_fp;
  return;
#endif /* POP_FRAME_PTR */
  /* Return from the function `resume'.  */
}

/* Assuming register contents and fault code have been pushed on the stack as
   arguments to this function, copy them into the standard place
   for the program's registers while GDB is running.  */
void
save_registers(int firstreg)
{
  memcpy(saved_regs, &firstreg, sizeof(saved_regs));
  fault_code = (&firstreg)[NUM_REGS];
}

/* Store into the structure such as `wait' would return
   the information on why the program faulted,
   converted into a machine-independent signal number.  */
#ifdef FAULT_TABLE
static int fault_table[] = FAULT_TABLE;
#else
static int fault_table[] = {0};
#endif /* FAULT_TABLE */

int
wait(WAITTYPE *w)
{
  WSETSTOP(*w, fault_table[fault_code / FAULT_CODE_UNITS]);
  return inferior_pid;
}

/* Allocate a big space in which files for kdb to read will be stored.
   Whatever is left is where malloc can allocate storage.

   Initialize it, so that there will be space in the executable file
   for it.  Then the files can be put into kdb by writing them into
   kdb's executable file.  */

/* The default size is as much space as we expect to be available
   for kdb to use!  */

#ifndef HEAP_SIZE
# define HEAP_SIZE 400000
#endif /* !HEAP_SIZE */

char heap[HEAP_SIZE] = {0};

#ifndef STACK_SIZE
# define STACK_SIZE 100000
#endif /* !STACK_SIZE */

int kdb_stack_beg[STACK_SIZE / sizeof(int)];
int kdb_stack_end;

void
_initialize_standalone(void)
{
  register char *next;

  /* Find start of data on files.  */

  files_start = heap;

  /* Find the end of the data on files: */
  for (next = files_start; *(int *)next; next += *(int *)next) {}

  /* That is where free storage starts for sbrk to give out.  */
  next_free = next;

  memory_limit = (heap + sizeof(heap));
}

