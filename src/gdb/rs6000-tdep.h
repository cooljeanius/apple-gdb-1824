/* Copyright (C) 2006, 2007, 2008, 2009 Free Software Foundation, Inc.

   Free Software Foundation, Inc.

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
   along with this program.  If not, see <http://www.gnu.org/licenses/>  */

#ifndef RS6000_TDEP_H
#define RS6000_TDEP_H 1

/* Hook in rs6000-aix-tdep.c for determining the TOC address when
   calling functions in the inferior.  */
extern CORE_ADDR (*rs6000_find_toc_address_hook)(CORE_ADDR);

/* Minimum possible text address in AIX.  */
#define AIX_TEXT_SEGMENT_BASE 0x10000000

/* Prototypes for functions in rs6000-tdep.c: */
CORE_ADDR rs6000_fetch_pointer_argument(struct frame_info *, int,
                                        struct type *);
extern void rs6000_software_single_step(enum target_signal, int);
extern CORE_ADDR rs6000_push_dummy_call(struct gdbarch *, struct value *,
                                        struct regcache *, CORE_ADDR, int,
                                        struct value **, CORE_ADDR, int,
                                        CORE_ADDR);
extern int rs6000_in_solib_return_trampoline(CORE_ADDR, const char *);
extern CORE_ADDR rs6000_skip_trampoline_code(CORE_ADDR);
extern void rs6000_register_to_value(struct frame_info *, int,
                                     struct type *, gdb_byte *);
extern void rs6000_value_to_register(struct frame_info *, int,
                                     struct type *, const gdb_byte *);
extern void rs6000_create_inferior(int);

#endif /* !RS6000_TDEP_H */

/* EOF */
