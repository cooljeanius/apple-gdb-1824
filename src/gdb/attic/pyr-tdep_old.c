/* pyr-tdep.c
   Low level interface to ptrace, for GDB when running under Unix.
   Copyright (C) 1988, 1989 Free Software Foundation, Inc.

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
Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */

#if !defined(__has_include)
# define __has_include(foo) 0
#endif /* !__has_include */

#if defined(HAVE_STDIO_H) || defined(__APPLE__) || __has_include(<stdio.h>)
# include <stdio.h>
#endif /* HAVE_STDIO_H || __APPLE__ */

#if defined(HAVE_STRING_H) || defined(__APPLE__) || __has_include(<string.h>)
# include <string.h>
#endif /* HAVE_STRING_H || __APPLE__ */

#if defined(HAVE_SYS_TYPES_H) || defined(__APPLE__) || __has_include(<sys/types.h>)
# include <sys/types.h>
#endif /* HAVE_SYS_TYPES_H || __APPLE__ */

#if defined(HAVE_SYS_PTRACE_H) || defined(__APPLE__) || __has_include(<sys/ptrace.h>)
# include <sys/ptrace.h>
#endif /* HAVE_SYS_PTRACE_H || __APPLE__ */

/*** Prettier register printing. ***/

/* Print registers in the same format as pyramid's dbx, adb, sdb.  */
int
pyr_print_registers(long *reg_buf[], int regnum)
{
  register int regno;
  int usp, ksp;
  struct user u;

  for (regno = 0; regno < 16; regno++) {
    printf/*_filtered*/ ("%6.6s: %8x  %6.6s: %8x  %6s: %8x  %6s: %8x\n",
		     reg_names[regno], reg_buf[regno],
		     reg_names[regno+16], reg_buf[regno+16],
		     reg_names[regno+32], reg_buf[regno+32],
		     reg_names[regno+48], reg_buf[regno+48]);
  }
  usp = ptrace (3, inferior_pid,
		      ((char *)&u.u_pcb.pcb_usp) -
		      ((char *)&u), 0);
  ksp = ptrace (3, inferior_pid,
		      ((char *)&u.u_pcb.pcb_ksp) -
		      ((char *)&u), 0);
  printf/*_filtered*/ ("\n%6.6s: %8x  %6.6s: %8x (%08x) %6.6s %8x\n",
		   reg_names[CSP_REGNUM],reg_buf[CSP_REGNUM],
		   reg_names[KSP_REGNUM], reg_buf[KSP_REGNUM], ksp,
		   "usp", usp);
}

/* Print the register regnum, or all registers if regnum is -1.
   fpregs is currently ignored.  */
int
pyr_do_registers_info(int regnum, int fpregs)
{
  /* On a pyr, we know a virtual register can always fit in an long.
     Here (and elsewhere) we take advantage of that.  Yuk.  */
  long raw_regs[MAX_REGISTER_RAW_SIZE*NUM_REGS];
  register int i;

  for (i = 0 ; i < 64 ; i++) {
    read_relative_register_raw_bytes(i, raw_regs+i);
  }
  if (regnum == -1)
    pyr_print_registers (raw_regs, regnum);
  else
    for (i = 0; i < NUM_REGS; i++)
      if (i == regnum) {
	long val = raw_regs[i];

	fputs_filtered (reg_names[i], stdout);
	printf_filtered(":");
	print_spaces_filtered (6 - strlen (reg_names[i]), stdout);
	if (val == 0)
	  printf_filtered ("0");
	else
	  printf_filtered ("0x%08x  %d", val, val);
	printf_filtered("\n");
      }
}

/*** Debugging editions of various macros from m-pyr.h ****/

CORE_ADDR frame_locals_address(FRAME frame)
{
  register int addr = find_saved_register (frame,CFP_REGNUM);
  register int result = read_memory_integer (addr, 4);
#ifdef PYRAMID_CONTROL_FRAME_DEBUGGING
  fprintf (stderr,
	   "\t[[..frame_locals:%8x, %s= %x @%x fcfp= %x foo= %x\n\t gr13=%x pr13=%x tr13=%x @%x]]\n",
	   frame->frame,
	   reg_names[CFP_REGNUM],
	   result, addr,
	   frame->frame_cfp, (CFP_REGNUM),


	   read_register(13), read_register(29), read_register(61),
	   find_saved_register(frame, 61));
#endif /* PYRAMID_CONTROL_FRAME_DEBUGGING */

  /* FIXME: I thought read_register (CFP_REGNUM) should be the right answer;
     or at least CFP_REGNUM relative to FRAME (ie, result).
     There seems to be a bug in the way the innermost frame is set up.  */

    return ((frame->next) ? result: frame->frame_cfp);
}

CORE_ADDR frame_args_addr(FRAME frame)
{
  register int addr = find_saved_register (frame,CFP_REGNUM);
  register int result = read_memory_integer (addr, 4);

#ifdef PYRAMID_CONTROL_FRAME_DEBUGGING
  fprintf (stderr,
	   "\t[[..frame_args:%8x, %s= %x @%x fcfp= %x r_r= %x\n\t gr13=%x pr13=%x tr13=%x @%x]]\n",
	   frame->frame,
	   reg_names[CFP_REGNUM],
	   result, addr,
	   frame->frame_cfp, read_register(CFP_REGNUM),

	   read_register(13), read_register(29), read_register(61),
	   find_saved_register(frame, 61));
#endif /*  PYRAMID_CONTROL_FRAME_DEBUGGING */

  /* FIXME: I thought read_register (CFP_REGNUM) should be the right answer;
     or at least CFP_REGNUM relative to FRAME (ie, result).
     There seems to be a bug in the way the innermost frame is set up.  */
    return ((frame->next) ? result: frame->frame_cfp);
}

/* EOF */
