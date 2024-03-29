/* BFD support for the Freescale 9S12-XGATE co-processor
   Copyright 2008, 2009, 2010, 2011, 2012 Free Software Foundation, Inc.

   This file is part of BFD, the Binary File Descriptor library.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston,
   MA 02110-1301, USA.  */

#ifndef USE_NEW_ELF_BFD_STRUCT_MEMBERS
# define USE_NEW_ELF_BFD_STRUCT_MEMBERS 1
#endif /* !USE_NEW_ELF_BFD_STRUCT_MEMBERS */
#include "sysdep.h"
#include "bfd.h"
#include "libbfd.h"

const bfd_arch_info_type bfd_m9s12xg_arch =
{
    16,	/* 16 bits in a word.  */
    32,	/* 16 bits in an address.  */
    8,	/* 8 bits in a byte.  */
    bfd_arch_m9s12xg,
    0,
    "m9s12xg",
    "m9s12xg",
    4, /* Section alignment power.  */
    TRUE,
    bfd_default_compatible,
    bfd_default_scan,
    bfd_arch_default_fill,
    0,
};

