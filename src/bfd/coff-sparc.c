/* coff-sparc.c: BFD back-end for Sparc COFF files.
 * Copyright 1990, 1991, 1992, 1993, 1994, 1995, 1996, 1999, 2000, 2001,
 * 2002, 2003, 2005 Free Software Foundation, Inc.
 * Written by Cygnus Support. */
/*
This file is part of BFD, the Binary File Descriptor library.

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
Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "bfd.h"
#include "sysdep.h"
#include "libbfd.h"
#include "coff/sparc.h"
#include "coff/internal.h"
#include "libcoff.h"

#define COFF_DEFAULT_SECTION_ALIGNMENT_POWER (3)

#define BADMAG(x) ((x).f_magic != SPARCMAGIC && (x).f_magic != LYNXCOFFMAGIC)

/* The page size is a guess based on ELF.  */
#define COFF_PAGE_SIZE 0x10000


static reloc_howto_type *coff_sparc_reloc_type_lookup
  PARAMS ((bfd *, bfd_reloc_code_real_type));
static void rtype2howto PARAMS ((arelent *, struct internal_reloc *));

enum reloc_type
  {
    R_SPARC_NONE = 0,
    R_SPARC_8,		R_SPARC_16,		R_SPARC_32,
    R_SPARC_DISP8,	R_SPARC_DISP16,		R_SPARC_DISP32,
    R_SPARC_WDISP30,	R_SPARC_WDISP22,
    R_SPARC_HI22,	R_SPARC_22,
    R_SPARC_13,		R_SPARC_LO10,
    R_SPARC_GOT10,	R_SPARC_GOT13,		R_SPARC_GOT22,
    R_SPARC_PC10,	R_SPARC_PC22,
    R_SPARC_WPLT30,
    R_SPARC_COPY,
    R_SPARC_GLOB_DAT,	R_SPARC_JMP_SLOT,
    R_SPARC_RELATIVE,
    R_SPARC_UA32,
    R_SPARC_max
  };

/* This is stolen pretty directly from elf.c.  */
static bfd_reloc_status_type
bfd_coff_generic_reloc PARAMS((bfd *, arelent *, asymbol *, PTR,
			       asection *, bfd *, const char **));

static bfd_reloc_status_type
bfd_coff_generic_reloc(bfd *abfd ATTRIBUTE_UNUSED, arelent *reloc_entry,
                       asymbol *symbol, PTR data ATTRIBUTE_UNUSED,
                       asection *input_section, bfd *output_bfd,
                       const char **error_message ATTRIBUTE_UNUSED)
{
  if ((output_bfd != (bfd *)NULL)
      && ((symbol->flags & BSF_SECTION_SYM) == 0))
    {
      reloc_entry->address += input_section->output_offset;
      return bfd_reloc_ok;
    }

  return bfd_reloc_continue;
}

static reloc_howto_type coff_sparc_howto_table[] =
{
  HOWTO(R_SPARC_NONE,    0,0, 0,FALSE,0,complain_overflow_dont,    bfd_coff_generic_reloc,"R_SPARC_NONE",    FALSE,0,0x00000000,TRUE),
  HOWTO(R_SPARC_8,       0,0, 8,FALSE,0,complain_overflow_bitfield,bfd_coff_generic_reloc,"R_SPARC_8",       FALSE,0,0x000000ff,TRUE),
  HOWTO(R_SPARC_16,      0,1,16,FALSE,0,complain_overflow_bitfield,bfd_coff_generic_reloc,"R_SPARC_16",      FALSE,0,0x0000ffff,TRUE),
  HOWTO(R_SPARC_32,      0,2,32,FALSE,0,complain_overflow_bitfield,bfd_coff_generic_reloc,"R_SPARC_32",      FALSE,0,0xffffffff,TRUE),
  HOWTO(R_SPARC_DISP8,   0,0, 8,TRUE, 0,complain_overflow_signed,  bfd_coff_generic_reloc,"R_SPARC_DISP8",   FALSE,0,0x000000ff,TRUE),
  HOWTO(R_SPARC_DISP16,  0,1,16,TRUE, 0,complain_overflow_signed,  bfd_coff_generic_reloc,"R_SPARC_DISP16",  FALSE,0,0x0000ffff,TRUE),
  HOWTO(R_SPARC_DISP32,  0,2,32,TRUE, 0,complain_overflow_signed,  bfd_coff_generic_reloc,"R_SPARC_DISP32",  FALSE,0,0x00ffffff,TRUE),
  HOWTO(R_SPARC_WDISP30, 2,2,30,TRUE, 0,complain_overflow_signed,  bfd_coff_generic_reloc,"R_SPARC_WDISP30", FALSE,0,0x3fffffff,TRUE),
  HOWTO(R_SPARC_WDISP22, 2,2,22,TRUE, 0,complain_overflow_signed,  bfd_coff_generic_reloc,"R_SPARC_WDISP22", FALSE,0,0x003fffff,TRUE),
  HOWTO(R_SPARC_HI22,   10,2,22,FALSE,0,complain_overflow_dont,    bfd_coff_generic_reloc,"R_SPARC_HI22",    FALSE,0,0x003fffff,TRUE),
  HOWTO(R_SPARC_22,      0,2,22,FALSE,0,complain_overflow_bitfield,bfd_coff_generic_reloc,"R_SPARC_22",      FALSE,0,0x003fffff,TRUE),
  HOWTO(R_SPARC_13,      0,2,13,FALSE,0,complain_overflow_bitfield,bfd_coff_generic_reloc,"R_SPARC_13",      FALSE,0,0x00001fff,TRUE),
  HOWTO(R_SPARC_LO10,    0,2,10,FALSE,0,complain_overflow_dont,    bfd_coff_generic_reloc,"R_SPARC_LO10",    FALSE,0,0x000003ff,TRUE),
  HOWTO(R_SPARC_GOT10,   0,2,10,FALSE,0,complain_overflow_bitfield,bfd_coff_generic_reloc,"R_SPARC_GOT10",   FALSE,0,0x000003ff,TRUE),
  HOWTO(R_SPARC_GOT13,   0,2,13,FALSE,0,complain_overflow_bitfield,bfd_coff_generic_reloc,"R_SPARC_GOT13",   FALSE,0,0x00001fff,TRUE),
  HOWTO(R_SPARC_GOT22,  10,2,22,FALSE,0,complain_overflow_bitfield,bfd_coff_generic_reloc,"R_SPARC_GOT22",   FALSE,0,0x003fffff,TRUE),
  HOWTO(R_SPARC_PC10,    0,2,10,FALSE,0,complain_overflow_bitfield,bfd_coff_generic_reloc,"R_SPARC_PC10",    FALSE,0,0x000003ff,TRUE),
  HOWTO(R_SPARC_PC22,    0,2,22,FALSE,0,complain_overflow_bitfield,bfd_coff_generic_reloc,"R_SPARC_PC22",    FALSE,0,0x003fffff,TRUE),
  HOWTO(R_SPARC_WPLT30,  0,0,00,FALSE,0,complain_overflow_dont,    bfd_coff_generic_reloc,"R_SPARC_WPLT30",  FALSE,0,0x00000000,TRUE),
  HOWTO(R_SPARC_COPY,    0,0,00,FALSE,0,complain_overflow_dont,    bfd_coff_generic_reloc,"R_SPARC_COPY",    FALSE,0,0x00000000,TRUE),
  HOWTO(R_SPARC_GLOB_DAT,0,0,00,FALSE,0,complain_overflow_dont,    bfd_coff_generic_reloc,"R_SPARC_GLOB_DAT",FALSE,0,0x00000000,TRUE),
  HOWTO(R_SPARC_JMP_SLOT,0,0,00,FALSE,0,complain_overflow_dont,    bfd_coff_generic_reloc,"R_SPARC_JMP_SLOT",FALSE,0,0x00000000,TRUE),
  HOWTO(R_SPARC_RELATIVE,0,0,00,FALSE,0,complain_overflow_dont,    bfd_coff_generic_reloc,"R_SPARC_RELATIVE",FALSE,0,0x00000000,TRUE),
  HOWTO(R_SPARC_UA32,    0,0,00,FALSE,0,complain_overflow_dont,    bfd_coff_generic_reloc,"R_SPARC_UA32",    FALSE,0,0x00000000,TRUE),
};

struct coff_reloc_map {
  bfd_reloc_code_real_type bfd_reloc_val;
  unsigned char coff_reloc_val;
};

static const struct coff_reloc_map sparc_reloc_map[] =
{
  { BFD_RELOC_NONE, R_SPARC_NONE, },
  { BFD_RELOC_16, R_SPARC_16, },
  { BFD_RELOC_8, R_SPARC_8 },
  { BFD_RELOC_8_PCREL, R_SPARC_DISP8 },
  { BFD_RELOC_CTOR, R_SPARC_32 }, /* @@ Assumes 32 bits.  */
  { BFD_RELOC_32, R_SPARC_32 },
  { BFD_RELOC_32_PCREL, R_SPARC_DISP32 },
  { BFD_RELOC_HI22, R_SPARC_HI22 },
  { BFD_RELOC_LO10, R_SPARC_LO10, },
  { BFD_RELOC_32_PCREL_S2, R_SPARC_WDISP30 },
  { BFD_RELOC_SPARC22, R_SPARC_22 },
  { BFD_RELOC_SPARC13, R_SPARC_13 },
  { BFD_RELOC_SPARC_GOT10, R_SPARC_GOT10 },
  { BFD_RELOC_SPARC_GOT13, R_SPARC_GOT13 },
  { BFD_RELOC_SPARC_GOT22, R_SPARC_GOT22 },
  { BFD_RELOC_SPARC_PC10, R_SPARC_PC10 },
  { BFD_RELOC_SPARC_PC22, R_SPARC_PC22 },
  { BFD_RELOC_SPARC_WPLT30, R_SPARC_WPLT30 },
  { BFD_RELOC_SPARC_COPY, R_SPARC_COPY },
  { BFD_RELOC_SPARC_GLOB_DAT, R_SPARC_GLOB_DAT },
  { BFD_RELOC_SPARC_JMP_SLOT, R_SPARC_JMP_SLOT },
  { BFD_RELOC_SPARC_RELATIVE, R_SPARC_RELATIVE },
  { BFD_RELOC_SPARC_WDISP22, R_SPARC_WDISP22 },
#ifdef ALLOW_UNUSED
  { BFD_RELOC_SPARC_UA32, R_SPARC_UA32 }, /* not used?? */
#endif /* ALLOW_UNUSED */
};

static reloc_howto_type *
coff_sparc_reloc_type_lookup(bfd *abfd ATTRIBUTE_UNUSED,
                             bfd_reloc_code_real_type code)
{
  unsigned int i;
  for (i = 0; i < (sizeof(sparc_reloc_map) / sizeof(struct coff_reloc_map)); i++)
    {
      if (sparc_reloc_map[i].bfd_reloc_val == code)
	return &coff_sparc_howto_table[(int)sparc_reloc_map[i].coff_reloc_val];
    }
  return 0;
}
#define coff_bfd_reloc_type_lookup	coff_sparc_reloc_type_lookup

static void
rtype2howto(arelent *cache_ptr, struct internal_reloc *dst)
{
  BFD_ASSERT(dst->r_type < (unsigned int)R_SPARC_max);
  cache_ptr->howto = &coff_sparc_howto_table[dst->r_type];
}

#define RTYPE2HOWTO(internal, relocentry) rtype2howto(internal,relocentry)

#define SWAP_IN_RELOC_OFFSET	H_GET_32
#define SWAP_OUT_RELOC_OFFSET	H_PUT_32
#define CALC_ADDEND(abfd, ptr, reloc, cache_ptr) \
  cache_ptr->addend = reloc.r_offset;

/* Clear the r_spare field in relocs.  */
#define SWAP_OUT_RELOC_EXTRA(abfd,src,dst) \
  do { \
       dst->r_spare[0] = 0; \
       dst->r_spare[1] = 0; \
     } while (0)

#define __A_MAGIC_SET__

/* Enable Sparc-specific hacks in "coffcode.h": */
#ifndef COFF_SPARC
# define COFF_SPARC
#endif /* !COFF_SPARC */

#include "coffcode.h"

#ifndef TARGET_SYM
# define TARGET_SYM sparccoff_vec
#endif /* !TARGET_SYM */

#ifndef TARGET_NAME
# define TARGET_NAME "coff-sparc"
#endif /* !TARGET_NAME */

#ifndef coff_get_section_contents_in_window_with_mode
# define coff_get_section_contents_in_window_with_mode \
   _bfd_generic_get_section_contents_in_window_with_mode
#endif /* !coff_get_section_contents_in_window_with_mode */

#if defined(CREATE_BIG_COFF_TARGET_VEC) && defined(USE_DEFINITIONS)
CREATE_BIG_COFF_TARGET_VEC(TARGET_SYM, TARGET_NAME, D_PAGED, 0, '_', NULL, COFF_SWAP_TABLE)
#else
# if 0 && !defined(CREATE_BIG_COFF_TARGET_VEC)
#  define CREATE_BIG_COFF_TARGET_VEC(VAR, NAME, EXTRA_O_FLAGS, EXTRA_S_FLAGS, UNDER, ALTERNATIVE, SWAP_TABLE)
# endif /* 0 && !CREATE_BIG_COFF_TARGET_VEC */
const bfd_target TARGET_SYM =
{
  (char *)TARGET_NAME,
  bfd_target_coff_flavour,
  BFD_ENDIAN_BIG,		/* Data byte order is big.  */
  BFD_ENDIAN_BIG,		/* Header byte order is big.  */
  /* object flags */
  (HAS_RELOC | EXEC_P | HAS_LINENO | HAS_DEBUG |
   HAS_SYMS | HAS_LOCALS | WP_TEXT | D_PAGED),
  /* section flags */
  (SEC_HAS_CONTENTS | SEC_ALLOC | SEC_LOAD | SEC_RELOC | 0),
  '_',			/* Leading symbol underscore.  */
  '/',				/* AR_pad_char.  */
  15,				/* AR_max_namelen.  */

  /* Data conversion functions.  */
  bfd_getb64, bfd_getb_signed_64, bfd_putb64,
  bfd_getb32, bfd_getb_signed_32, bfd_putb32,
  bfd_getb16, bfd_getb_signed_16, bfd_putb16,

  /* Header conversion functions.  */
  bfd_getb64, bfd_getb_signed_64, bfd_putb64,
  bfd_getb32, bfd_getb_signed_32, bfd_putb32,
  bfd_getb16, bfd_getb_signed_16, bfd_putb16,

  /* bfd_check_format.  */
  { _bfd_dummy_target, coff_object_p, bfd_generic_archive_p,
    _bfd_dummy_target },
  /* bfd_set_format.  */
  { bfd_false, coff_mkobject, _bfd_generic_mkarchive, bfd_false },
  /* bfd_write_contents.  */
  { bfd_false, coff_write_object_contents, _bfd_write_archive_contents,
    bfd_false },

  BFD_JUMP_TABLE_GENERIC(coff),
  BFD_JUMP_TABLE_COPY(coff),
  BFD_JUMP_TABLE_CORE(_bfd_nocore),
  BFD_JUMP_TABLE_ARCHIVE(_bfd_archive_coff),
  BFD_JUMP_TABLE_SYMBOLS(coff),
  BFD_JUMP_TABLE_RELOCS(coff),
  BFD_JUMP_TABLE_WRITE(coff),
  BFD_JUMP_TABLE_LINK(coff),
  BFD_JUMP_TABLE_DYNAMIC(_bfd_nodynamic),
  NULL,
  COFF_SWAP_TABLE
};
#endif /* CREATE_BIG_COFF_TARGET_VEC && USE_DEFINITIONS */

#ifdef COFF_SPARC
# undef COFF_SPARC
#endif /* COFF_SPARC */

/* EOF */
