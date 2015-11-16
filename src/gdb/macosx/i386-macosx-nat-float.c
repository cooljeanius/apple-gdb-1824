/* macosx/i386-macosx-nat-float.c
   Mac OS X support for GDB, the GNU debugger.
   Copyright 1997, 1998, 1999, 2000, 2001, 2002
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

/* This is derived from i386-xdep.c */

#include "defs.h"
#include "inferior.h"
#include "target.h"
#include "symfile.h"
#include "symtab.h"
#include "objfiles.h"
#include "gdbcmd.h"

extern void i386_float_info(void);

/* see <mach/i386/_structs.h> for reference: */
#if (defined(__DARWIN_UNIX03) && __DARWIN_UNIX03) && !defined(fp_control_t)
typedef __darwin_fp_control_t fp_control_t;
/* all fields need underscores in this case: */
# ifndef pc
#  define pc __pc
# endif /* !pc */
# ifndef rc
#  define rc __rc
# endif /* !rc */
# ifndef invalid
#  define invalid __invalid
# endif /* !invalid */
# ifndef denorm
#  define denorm __denorm
# endif /* !denorm */
# ifndef zdiv
#  define zdiv __zdiv
# endif /* !zdiv */
# ifndef ovrfl
#  define ovrfl __ovrfl
# endif /* !ovrfl */
# ifndef undfl
#  define undfl __undfl
# endif /* !undfl */
# ifndef precis
#  define precis __precis
# endif /* !precis */
#endif /* __DARWIN_UNIX03 && !fp_control_t */

static void
print_387_control_word(fp_control_t *control)
{
  printf("control 0x%04x: ", *((unsigned short *)control));
  printf("compute to ");
  switch (control->pc) {
    case FP_PREC_24B: printf("24 bits; "); break;
    case FP_PREC_53B: printf("53 bits; "); break;
    case FP_PREC_64B: printf("64 bits; "); break;
    default: printf("(bad); "); break;
  }

  printf("round ");
  switch (control->rc) {
    case FP_RND_NEAR: printf("NEAREST; "); break;
    case FP_RND_DOWN: printf("DOWN; "); break;
    case FP_RND_UP: printf("UP; "); break;
    case FP_CHOP: printf("CHOP; "); break;
  }
  printf("mask:");
  if (control->invalid)	printf(" INVALID");
  if (control->denorm) printf(" DENORM");
  if (control->zdiv) printf(" DIVZ");
  if (control->ovrfl) printf(" OVERF");
  if (control->undfl) printf(" UNDERF");
  if (control->precis) printf(" LOS");
  printf(";");

  printf("\n");
}

#if (defined(__DARWIN_UNIX03) && __DARWIN_UNIX03) && !defined(fp_status_t)
typedef __darwin_fp_status_t fp_status_t;
/* likewise with the underscores here: */
# ifndef stkflt
#  define stkflt __stkflt
# endif /* !stkflt */
# ifndef errsumm
#  define errsumm __errsumm
# endif /* !errsumm */
# ifndef c0
#  define c0 __c0
# endif /* !c0 */
# ifndef c1
#  define c1 __c1
# endif /* !c1 */
# ifndef c2
#  define c2 __c2
# endif /* !c2 */
# ifndef c3
#  define c3 __c3
# endif /* !c3 */
# ifndef tos
#  define tos __tos
# endif /* !tos */
#endif /* __DARWIN_UNIX03 && !fp_status_t */

static void
print_387_status_word(fp_status_t *status)
{
  printf("status 0x%04x: ", *((unsigned short *)status));

  printf("exceptions:");
  if (status->invalid) printf(" INVALID");
  if (status->denorm) printf(" DENORM");
  if (status->zdiv) printf(" DIVZ");
  if (status->ovrfl) printf(" OVERF");
  if (status->undfl) printf(" UNDERF");
  if (status->precis) printf(" LOS");
  if (status->stkflt) printf(" FPSTACK");
  if (status->errsumm) printf(" ERRSUMM");
  printf("; ");

  printf("flags: %d%d%d%d; ",
	 status->c0, status->c1, status->c2, status->c3);

  printf("top %d\n", status->tos);
}

static void
print_387_status(gdb_i386_thread_fpstate_t *fp)
{
  int i;
  int top;
  int fpreg;
  unsigned short tag;
  unsigned char *p;

  print_387_status_word(&fp->environ.status);
  print_387_control_word(&fp->environ.control);
  printf("last exception: ");
  printf("opcode 0x%x; ", fp->environ.opcode);
  printf("pc 0x%x:0x%x; ",
	 *((unsigned short *)&fp->environ.cs), fp->environ.ip);
  printf("operand 0x%x:0x%x\n",
	 *((unsigned short *)&fp->environ.ds), fp->environ.dp);

  top = fp->environ.status.tos;
  tag = *((unsigned short *)&fp->environ.tag);

  printf("regno  tag  msb              lsb  value\n");
  for (fpreg = 7; fpreg >= 0; fpreg--) {
    double val;

    printf("%s %d: ", (fpreg == top) ? "=>" : "  ", fpreg);

    switch ((tag >> (fpreg * 2)) & 3) {
      case 0: printf("valid "); break;
      case 1: printf("zero  "); break;
      case 2: printf("trap  "); break;
      case 3: printf("empty "); break;
    }
    p = (char *)&(fp->stack.ST[fpreg]);
    for (i = 9; i >= 0; i--)
      printf("%02x", p[i]);

    i387_to_double((char *)&fp->stack.ST[fpreg], (char *)&val);
    printf("  %g\n", val);
  }
}

void
i386_float_info(void)
{
  gdb_i386_thread_fpstate_t fpstate;
#ifdef i386_THREAD_FPSTATE_COUNT
  unsigned int fpstate_size = i386_THREAD_FPSTATE_COUNT;
#else
# if defined GDB_i386_THREAD_FPSTATE_COUNT
  unsigned int fpstate_size = GDB_i386_THREAD_FPSTATE_COUNT;
# else
  unsigned int fpstate_size = 0U;
# endif /* GDB_i386_THREAD_FPSTATE_COUNT */
#endif /* i386_THREAD_FPSTATE_COUNT */
  thread_t current_thread = macosx_current_thread();

  if (current_thread) {
#ifdef i386_THREAD_FPSTATE
    mach_check_ret(thread_get_state(current_thread, i386_THREAD_FPSTATE, \
			       (thread_state_t)&fpstate, &fpstate_size),
	      "thread_get_state", "i386_float_info");
#else
# ifdef GDB_i386_THREAD_FPSTATE
    mach_check_ret(thread_get_state(current_thread, GDB_i386_THREAD_FPSTATE, \
                                    (thread_state_t)&fpstate, &fpstate_size),
                   "thread_get_state", "i386_float_info");
# endif /* GDB_i386_THREAD_FPSTATE */
#endif /* i386_THREAD_FPSTATE */
    print_387_status(&fpstate);
  }
  return;
}

/* EOF */
