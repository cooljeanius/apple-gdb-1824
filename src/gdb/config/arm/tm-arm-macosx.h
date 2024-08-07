/* tm-arm-macosx.h
 * Target support for Mac OS X on ARM for GDB, the GNU debugger.
 * Copyright (C) 1997-2002,
 * Free Software Foundation, Inc. */
/*
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
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef _TM_ARM_MACOSX_H_
#define _TM_ARM_MACOSX_H_

/* removing include of "tm-macosx.h" here */


struct arm_macosx_tdep_inf_status {
  CORE_ADDR macosx_half_step_pc;
};

typedef struct arm_macosx_tdep_inf_status arm_macosx_tdep_inf_status_t;

int arm_macosx_fast_show_stack(unsigned int count_limit,
                               unsigned int print_start,
                               unsigned int print_end,
                               unsigned int *count,
                               void (print_fun)(struct ui_out *uiout,
                                                int *frame_num,
                                                CORE_ADDR pc,
                                                CORE_ADDR fp));

#define FAST_COUNT_STACK_DEPTH(count_limit, print_start, print_end, count, print_fun) \
  (arm_macosx_fast_show_stack(count_limit, print_start, print_end, count, print_fun))

char *arm_throw_catch_find_typeinfo(struct frame_info *curr_frame,
                                    int exception_type);
#define THROW_CATCH_FIND_TYPEINFO(curr_frame, exception_type) \
  (arm_throw_catch_find_typeinfo(curr_frame, exception_type))

#ifndef __GDB_ARM_MACOSX_TDEP_H__
extern int arm_macosx_in_switch_glue(CORE_ADDR pc);
#endif /* !__GDB_ARM_MACOSX_TDEP_H__ */

#ifndef IN_SWITCH_GLUE
# define IN_SWITCH_GLUE(pc) \
  (arm_macosx_in_switch_glue(pc))
#endif /* !IN_SWITCH_GLUE */

#endif /* _TM_ARM_MACOSX_H_ */

/* EOF */
