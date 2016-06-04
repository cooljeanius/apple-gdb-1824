/* cris-tdep.h: Target dependent code for CRIS, for GDB, the GNU debugger.

   Copyright (C) 2001-2013 Free Software Foundation, Inc.

   Contributed by Axis Communications AB.
   Written by Hendrik Ruijter, Stefan Andersson, and Orjan Friberg.

   This file is part of GDB.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#ifndef CRIS_TDEP_H
#define CRIS_TDEP_H

#include "ansidecl.h"

#ifndef GDBARCH_H
/* CRIS architecture specific information: */
struct gdbarch_tdep
{
  unsigned int cris_version;
  const char *cris_mode;
  int cris_dwarf2_cfi;
};
#endif /* !GDBARCH_H */

/* Prototypes */
extern int crisv32_single_step_through_delay(struct gdbarch *,
					     struct frame_info *);
extern int cris_can_use_hardware_watchpoint(int, int, int);
extern int cris_region_ok_for_watchpoint(CORE_ADDR, int) ATTRIBUTE_CONST;
extern CORE_ADDR cris_stopped_data_address(void);
extern struct cris_unwind_cache *cris_frame_unwind_cache(struct frame_info *,
							 void **);
extern const struct frame_unwind *cris_frame_sniffer(struct frame_info *)
  ATTRIBUTE_CONST;

#endif /* CRIS_TDEP_H */

/* EOF */
