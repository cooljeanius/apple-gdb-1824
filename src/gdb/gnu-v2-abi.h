/* gnu-v2-abi.h: Abstraction of GNU v2 abi.

   Copyright 2005 Free Software Foundation, Inc.

   This file is part of GDB.

   This program is free software; you can redistribute it and/or
   modify
   it under the terms of the GNU General Public License as published
   by
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

#ifndef GNU_V2_ABI_H
#define GNU_V2_ABI_H

#include "gdbtypes.h"
#include "cp-abi.h"

extern int gnuv2_baseclass_offset(struct type *type, int index,
				  const bfd_byte *valaddr,
				  CORE_ADDR address);

extern struct cp_abi_ops gnu_v2_abi_ops;

#endif /* !GNU_V2_ABI_H */

/* EOF */
