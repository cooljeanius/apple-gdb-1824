/* peXXigen.c: Support for the generic parts of PE/PEI;
 * the common executable parts.
 *
 * Copyright 1995, 1996, 1997, 1998, 1999, 2000, 2001, 2002, 2003, 2004,
 * 2005 Free Software Foundation, Inc.
 * Written by Cygnus Solutions.
 *
 * This file is part of BFD, the Binary File Descriptor library.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St. - 5th Floor, Boston, MA 02110-1301, USA.
 */

/* Most of this hacked by Steve Chamberlain <sac@cygnus.com>.
 *
 * PE/PEI rearrangement (and code added): Donn Terry
 *					  Softway Systems, Inc.
 */

/* Hey look, some documentation [and in a place you expect to find it]!
 *
 * The main reference for the pei format is "Microsoft Portable Executable
 * and Common Object File Format Specification 4.1". Get it if you need to
 * do some serious hacking on this code.
 *
 * Another reference:
 * "Peering Inside the PE: A Tour of the Win32 Portable Executable
 * File Format", MSJ 1994, Volume 9.
 *
 * The *sole* difference between the pe format and the pei format is that the
 * latter has an MSDOS 2.0 .exe header on the front that prints the message
 * "This app must be run under Windows." (or some such).
 * (FIXME: Whether that statement is *really* true or not is unknown.
 * Are there more subtle differences between pe and pei formats?
 * For now assume there are NOT any such differences. If you find one,
 * then for God sakes document it here!)
 *
 * The Microsoft docs use the word "image" instead of "executable" because
 * the former can also refer to a DLL (shared library). Confusion can arise
 * because the `i' in `pei' also refers to "image". The `pe' format can
 * also create images (i.e. executables), it is just that to run on a win32
 * system you need to use the pei format.
 *
 * FIXME: Please add more docs here so the next poor fool that has to hack
 * on this code has a chance of getting something accomplished without
 * wasting too much time. */

/* This expands into COFF_WITH_pe or COFF_WITH_pep or COFF_WITH_pex64
 * depending on whether we are compiling for straight PE or PE+ or 64-bit
 * PE (see the Makefile for how this happens). */
#ifndef COFF_WITH_XX
# define COFF_WITH_XX
#endif /* !COFF_WITH_XX */

#include "sysdep.h"
#include "bfd.h"
#include "libbfd.h"
#include "coff/internal.h"

/* NOTE: it is strange to be including an architecture specific header
 * in what is supposed to be general (to PE/PEI) code. However, that is
 * where the definitions are, and they do NOT vary per architecture
 * within PE/PEI, so it is okay for us to get them from there.
 * FIXME: The lack of variance is an assumption which may prove to be
 * incorrect if new PE/PEI targets are created.  */
#ifdef COFF_WITH_pep
# include "coff/ia64.h"
#else
# include "coff/i386.h"
#endif /* COFF_WITH_pep */

#include "coff/pe.h"
#include "libcoff.h"
#include "libpei.h"

#ifdef COFF_WITH_pep
# undef AOUTSZ
# define AOUTSZ		PEPAOUTSZ
# define PEAOUTHDR	PEPAOUTHDR
#endif /* COFF_WITH_pep */

/* FIXME: This file has various tests of POWERPC_LE_PE.  Those tests
   worked when the code was in peicode.h, but no longer work now that
   the code is in peigen.c.  PowerPC NT is said to be dead.  If
   anybody wants to revive the code, you will have to figure out how
   to handle those issues.  */

void
_bfd_XXi_swap_sym_in(bfd *abfd, void *ext1, void *in1)
{
  SYMENT *ext = (SYMENT *)ext1;
  struct internal_syment *in = (struct internal_syment *)in1;

  if (ext->e.e_name[0] == 0)
    {
      in->_n._n_n._n_zeroes = 0;
      in->_n._n_n._n_offset = (long)H_GET_32(abfd, ext->e.e.e_offset);
    }
  else
    memcpy(in->_n._n_name, ext->e.e_name, (size_t)SYMNMLEN);

  in->n_value = H_GET_32(abfd, ext->e_value);
  in->n_scnum = (short)H_GET_16(abfd, ext->e_scnum);

  if (sizeof(ext->e_type) == 2UL)
    in->n_type = (unsigned short)H_GET_16(abfd, ext->e_type);
  else
    in->n_type = (unsigned short)H_GET_32(abfd, ext->e_type);

  in->n_sclass = H_GET_8(abfd, ext->e_sclass);
  in->n_numaux = H_GET_8(abfd, ext->e_numaux);

#ifndef STRICT_PE_FORMAT
  /* This is for Gnu-created DLLs.  */

  /* The section symbols for the .idata$ sections have class 0x68
     (C_SECTION), which MS documentation indicates is a section
     symbol.  Unfortunately, the value field in the symbol is simply a
     copy of the .idata section's flags rather than something useful.
     When these symbols are encountered, change the value to 0 so that
     they will be handled somewhat correctly in the bfd code.  */
  if (in->n_sclass == C_SECTION)
    {
      in->n_value = 0x0;

      /* Create synthetic empty sections as needed.  DJ */
      if (in->n_scnum == 0)
	{
	  asection *sec;

	  for (sec = abfd->sections; sec; sec = sec->next)
	    {
	      if (strcmp(sec->name, in->n_name) == 0)
		{
		  in->n_scnum = (short)sec->target_index;
		  break;
		}
	    }
	}

      if (in->n_scnum == 0)
	{
	  int unused_section_number = 0;
	  asection *sec;
	  char *name;
          bfd_size_type namelen;

	  for (sec = abfd->sections; sec; sec = sec->next)
	    if (unused_section_number <= sec->target_index)
	      unused_section_number = (sec->target_index + 1);

	  namelen = ((bfd_size_type)strlen(in->n_name) + 10UL);
	  name = (char *)bfd_alloc(abfd, namelen);
	  if (name == NULL)
	    return;
	  strncpy(name, in->n_name, namelen);
	  sec = bfd_make_section_anyway(abfd, name);

	  sec->vma = 0;
	  sec->lma = 0;
	  sec->size = 0;
	  sec->filepos = 0;
	  sec->rel_filepos = 0;
	  sec->reloc_count = 0;
	  sec->line_filepos = 0;
	  sec->lineno_count = 0;
	  sec->userdata = NULL;
	  sec->next = NULL;
	  sec->alignment_power = 2;
	  sec->flags = SEC_HAS_CONTENTS | SEC_ALLOC | SEC_DATA | SEC_LOAD;

	  sec->target_index = unused_section_number;

	  in->n_scnum = (short)unused_section_number;
	}
      in->n_sclass = C_STAT;
    }
#endif /* !STRICT_PE_FORMAT */

#ifdef coff_swap_sym_in_hook
  /* This will NOT work in peigen.c, but since it is for PPC PE, it is not
     worth fixing.  */
  coff_swap_sym_in_hook(abfd, ext1, in1);
#endif /* coff_swap_sym_in_hook */
}

/* */
unsigned int
_bfd_XXi_swap_sym_out(bfd *abfd, void *inp, void *extp)
{
  struct internal_syment *in = (struct internal_syment *)inp;
  SYMENT *ext = (SYMENT *)extp;

  if (in->_n._n_name[0] == 0) {
      H_PUT_32(abfd, (bfd_vma)0UL, ext->e.e.e_zeroes);
      H_PUT_32(abfd, (bfd_vma)in->_n._n_n._n_offset, ext->e.e.e_offset);
  } else {
      memcpy(ext->e.e_name, in->_n._n_name, (size_t)SYMNMLEN);
  }

  H_PUT_32(abfd, in->n_value, ext->e_value);
  H_PUT_16(abfd, (bfd_vma)in->n_scnum, ext->e_scnum);

  /*NOTREACHED*/

  if (sizeof(ext->e_type) == 2UL) {
      H_PUT_16(abfd, (bfd_vma)in->n_type, ext->e_type);
  } else {
      H_PUT_32(abfd, (bfd_vma)in->n_type, ext->e_type);
  }

  H_PUT_8(abfd, in->n_sclass, ext->e_sclass);
  H_PUT_8(abfd, in->n_numaux, ext->e_numaux);

  return SYMESZ;
}

/* */
void
_bfd_XXi_swap_aux_in(bfd *abfd, void *ext1, int type, int theclass,
		     int indx ATTRIBUTE_UNUSED,
		     int numaux ATTRIBUTE_UNUSED,
		     void *in1)
{
  AUXENT *ext = (AUXENT *)ext1;
  union internal_auxent *in = (union internal_auxent *)in1;

  switch (theclass) {
    case C_FILE:
      if (ext->x_file.x_fname[0] == 0) {
	  in->x_file.x_n.x_zeroes = 0;
	  in->x_file.x_n.x_offset = (long)H_GET_32(abfd, ext->x_file.x_n.x_offset);
      } else {
#if defined(FILNMLEN) && defined(E_FILNMLEN) && (FILNMLEN != E_FILNMLEN)
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "peXXigen.c expects FILNMLEN and E_FILNMLEN to be equivalent."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* (FILNMLEN != E_FILNMLEN) */
	  memcpy(in->x_file.x_fname, ext->x_file.x_fname,
                 (size_t)E_FILNMLEN);
      }
      return;

    case C_STAT:
    case C_LEAFSTAT:
    case C_HIDDEN:
      if (type == T_NULL) {
	  in->x_scn.x_scnlen = (long)GET_SCN_SCNLEN(abfd, ext);
	  in->x_scn.x_nreloc = (unsigned short)GET_SCN_NRELOC(abfd, ext);
	  in->x_scn.x_nlinno = (unsigned short)GET_SCN_NLINNO(abfd, ext);
	  in->x_scn.x_checksum = H_GET_32(abfd, ext->x_scn.x_checksum);
	  in->x_scn.x_associated = (unsigned short)H_GET_16(abfd, ext->x_scn.x_associated);
	  in->x_scn.x_comdat = H_GET_8(abfd, ext->x_scn.x_comdat);
	  return;
      }
      break;
    default:
      break; /* not sure if correct... */
  }

  in->x_sym.x_tagndx.l = (long)H_GET_32(abfd, ext->x_sym.x_tagndx);
  in->x_sym.x_tvndx = (unsigned short)H_GET_16(abfd, ext->x_sym.x_tvndx);

  if ((theclass == C_BLOCK) || (theclass == C_FCN) || ISFCN(type)
      || ISTAG(theclass)) {
      in->x_sym.x_fcnary.x_fcn.x_lnnoptr = (bfd_signed_vma)GET_FCN_LNNOPTR(abfd, ext);
      in->x_sym.x_fcnary.x_fcn.x_endndx.l = (long)GET_FCN_ENDNDX(abfd, ext);
  } else {
      in->x_sym.x_fcnary.x_ary.x_dimen[0] =
	(unsigned short)H_GET_16(abfd, ext->x_sym.x_fcnary.x_ary.x_dimen[0]);
      in->x_sym.x_fcnary.x_ary.x_dimen[1] =
	(unsigned short)H_GET_16(abfd, ext->x_sym.x_fcnary.x_ary.x_dimen[1]);
      in->x_sym.x_fcnary.x_ary.x_dimen[2] =
	(unsigned short)H_GET_16(abfd, ext->x_sym.x_fcnary.x_ary.x_dimen[2]);
      in->x_sym.x_fcnary.x_ary.x_dimen[3] =
	(unsigned short)H_GET_16(abfd, ext->x_sym.x_fcnary.x_ary.x_dimen[3]);
  }

  if (ISFCN (type))
    {
      in->x_sym.x_misc.x_fsize = (long)H_GET_32(abfd, ext->x_sym.x_misc.x_fsize);
    }
  else
    {
      in->x_sym.x_misc.x_lnsz.x_lnno = (unsigned short)GET_LNSZ_LNNO(abfd, ext);
      in->x_sym.x_misc.x_lnsz.x_size = (unsigned short)GET_LNSZ_SIZE(abfd, ext);
    }
}

/* */
unsigned int
_bfd_XXi_swap_aux_out(bfd *abfd, void *inp, int type, int theclass,
		      int indx ATTRIBUTE_UNUSED,
		      int numaux ATTRIBUTE_UNUSED,
		      void *extp)
{
  union internal_auxent *in = (union internal_auxent *)inp;
  AUXENT *ext = (AUXENT *)extp;

  memset(ext, 0, (size_t)AUXESZ);

  switch (theclass) {
    case C_FILE:
      if (in->x_file.x_fname[0] == 0) {
	  H_PUT_32(abfd, (bfd_vma)0UL, ext->x_file.x_n.x_zeroes);
	  H_PUT_32(abfd, (bfd_vma)in->x_file.x_n.x_offset,
                   ext->x_file.x_n.x_offset);
      } else {
#if defined(FILNMLEN) && defined(E_FILNMLEN) && (FILNMLEN != E_FILNMLEN)
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "peXXigen.c expects FILNMLEN and E_FILNMLEN to be equivalent."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* (FILNMLEN != E_FILNMLEN) */
	  memcpy(ext->x_file.x_fname, in->x_file.x_fname,
                 (size_t)E_FILNMLEN);
      }

      return AUXESZ;

    case C_STAT:
    case C_LEAFSTAT:
    case C_HIDDEN:
      if (type == T_NULL) {
	  PUT_SCN_SCNLEN(abfd, (bfd_vma)in->x_scn.x_scnlen, ext);
	  PUT_SCN_NRELOC(abfd, (bfd_vma)in->x_scn.x_nreloc, ext);
	  PUT_SCN_NLINNO(abfd, (bfd_vma)in->x_scn.x_nlinno, ext);
	  H_PUT_32(abfd, (bfd_vma)in->x_scn.x_checksum, ext->x_scn.x_checksum);
	  H_PUT_16(abfd, (bfd_vma)in->x_scn.x_associated,
                   ext->x_scn.x_associated);
	  H_PUT_8(abfd, in->x_scn.x_comdat, ext->x_scn.x_comdat);
	  return AUXESZ;
      }
      break;
    default:
      break; /* not sure if correct? */
  }

  H_PUT_32(abfd, (bfd_vma)in->x_sym.x_tagndx.l, ext->x_sym.x_tagndx);
  H_PUT_16(abfd, (bfd_vma)in->x_sym.x_tvndx, ext->x_sym.x_tvndx);

  if ((theclass == C_BLOCK) || (theclass == C_FCN) || ISFCN(type)
      || ISTAG(theclass))
    {
      PUT_FCN_LNNOPTR(abfd, (bfd_vma)in->x_sym.x_fcnary.x_fcn.x_lnnoptr,
                      ext);
      PUT_FCN_ENDNDX(abfd, (bfd_vma)in->x_sym.x_fcnary.x_fcn.x_endndx.l,
                     ext);
    }
  else
    {
      H_PUT_16(abfd, (bfd_vma)in->x_sym.x_fcnary.x_ary.x_dimen[0],
               ext->x_sym.x_fcnary.x_ary.x_dimen[0]);
      H_PUT_16(abfd, (bfd_vma)in->x_sym.x_fcnary.x_ary.x_dimen[1],
               ext->x_sym.x_fcnary.x_ary.x_dimen[1]);
      H_PUT_16(abfd, (bfd_vma)in->x_sym.x_fcnary.x_ary.x_dimen[2],
               ext->x_sym.x_fcnary.x_ary.x_dimen[2]);
      H_PUT_16(abfd, (bfd_vma)in->x_sym.x_fcnary.x_ary.x_dimen[3],
               ext->x_sym.x_fcnary.x_ary.x_dimen[3]);
    }

  if (ISFCN(type))
    H_PUT_32(abfd, (bfd_vma)in->x_sym.x_misc.x_fsize,
             ext->x_sym.x_misc.x_fsize);
  else
    {
      PUT_LNSZ_LNNO(abfd, (bfd_vma)in->x_sym.x_misc.x_lnsz.x_lnno, ext);
      PUT_LNSZ_SIZE(abfd, (bfd_vma)in->x_sym.x_misc.x_lnsz.x_size, ext);
    }

  return AUXESZ;
}

/* */
void
_bfd_XXi_swap_lineno_in(bfd *abfd, void *ext1, void *in1)
{
  LINENO *ext = (LINENO *)ext1;
  struct internal_lineno *in = (struct internal_lineno *)in1;

  in->l_addr.l_symndx = (bfd_signed_vma)H_GET_32(abfd, ext->l_addr.l_symndx);
  in->l_lnno = GET_LINENO_LNNO(abfd, ext);
}

/* */
unsigned int
_bfd_XXi_swap_lineno_out(bfd * abfd, void * inp, void * outp)
{
  struct internal_lineno *in = (struct internal_lineno *)inp;
  struct external_lineno *ext = (struct external_lineno *)outp;
  H_PUT_32(abfd, (bfd_vma)in->l_addr.l_symndx, ext->l_addr.l_symndx);

  PUT_LINENO_LNNO(abfd, (bfd_vma)in->l_lnno, ext);
  return LINESZ;
}

/* */
void
_bfd_XXi_swap_aouthdr_in(bfd *abfd, void *aouthdr_ext1, void *aouthdr_int1)
{
  struct internal_extra_pe_aouthdr *a;
  PEAOUTHDR *src = (PEAOUTHDR *)(aouthdr_ext1);
  AOUTHDR *aouthdr_ext = (AOUTHDR *)aouthdr_ext1;
  struct internal_aouthdr *aouthdr_int = (struct internal_aouthdr *)aouthdr_int1;

  aouthdr_int->magic = (short)H_GET_16(abfd, aouthdr_ext->magic);
  aouthdr_int->vstamp = (short)H_GET_16(abfd, aouthdr_ext->vstamp);
  aouthdr_int->tsize = GET_AOUTHDR_TSIZE(abfd, aouthdr_ext->tsize);
  aouthdr_int->dsize = GET_AOUTHDR_DSIZE(abfd, aouthdr_ext->dsize);
  aouthdr_int->bsize = GET_AOUTHDR_BSIZE(abfd, aouthdr_ext->bsize);
  aouthdr_int->entry = GET_AOUTHDR_ENTRY(abfd, aouthdr_ext->entry);
  aouthdr_int->text_start =
    GET_AOUTHDR_TEXT_START(abfd, aouthdr_ext->text_start);
#ifndef COFF_WITH_pep
  /* PE32+ does not have data_start member!  */
  aouthdr_int->data_start =
    GET_AOUTHDR_DATA_START(abfd, aouthdr_ext->data_start);
#endif /* !COFF_WITH_pep */

  a = &aouthdr_int->pe;
  a->ImageBase = GET_OPTHDR_IMAGE_BASE(abfd, src->ImageBase);
  a->SectionAlignment = H_GET_32(abfd, src->SectionAlignment);
  a->FileAlignment = H_GET_32(abfd, src->FileAlignment);
  a->MajorOperatingSystemVersion =
    (short)H_GET_16(abfd, src->MajorOperatingSystemVersion);
  a->MinorOperatingSystemVersion =
    (short)H_GET_16(abfd, src->MinorOperatingSystemVersion);
  a->MajorImageVersion = (short)H_GET_16(abfd, src->MajorImageVersion);
  a->MinorImageVersion = (short)H_GET_16(abfd, src->MinorImageVersion);
  a->MajorSubsystemVersion = (short)H_GET_16(abfd, src->MajorSubsystemVersion);
  a->MinorSubsystemVersion = (short)H_GET_16(abfd, src->MinorSubsystemVersion);
  a->Reserved1 = (long)H_GET_32(abfd, src->Reserved1);
  a->SizeOfImage = (long)H_GET_32(abfd, src->SizeOfImage);
  a->SizeOfHeaders = (long)H_GET_32(abfd, src->SizeOfHeaders);
  a->CheckSum = (long)H_GET_32(abfd, src->CheckSum);
  a->Subsystem = (short)H_GET_16(abfd, src->Subsystem);
  a->DllCharacteristics = (short)H_GET_16(abfd, src->DllCharacteristics);
  a->SizeOfStackReserve =
    GET_OPTHDR_SIZE_OF_STACK_RESERVE(abfd, src->SizeOfStackReserve);
  a->SizeOfStackCommit =
    GET_OPTHDR_SIZE_OF_STACK_COMMIT(abfd, src->SizeOfStackCommit);
  a->SizeOfHeapReserve =
    GET_OPTHDR_SIZE_OF_HEAP_RESERVE(abfd, src->SizeOfHeapReserve);
  a->SizeOfHeapCommit =
    GET_OPTHDR_SIZE_OF_HEAP_COMMIT(abfd, src->SizeOfHeapCommit);
  a->LoaderFlags = (long)H_GET_32(abfd, src->LoaderFlags);
  a->NumberOfRvaAndSizes = (long)H_GET_32(abfd, src->NumberOfRvaAndSizes);

  {
    int idx;

    for (idx = 0; idx < 16; idx++)
      {
        /* If data directory is empty, rva also should be 0.  */
	int size =
	  (int)H_GET_32(abfd, src->DataDirectory[idx][1]);
	a->DataDirectory[idx].Size = size;

	if (size)
	  a->DataDirectory[idx].VirtualAddress =
	    H_GET_32 (abfd, src->DataDirectory[idx][0]);
	else
	  a->DataDirectory[idx].VirtualAddress = 0;
      }
  }

  if (aouthdr_int->entry)
    {
      aouthdr_int->entry += a->ImageBase;
#ifndef COFF_WITH_pep
      aouthdr_int->entry &= 0xffffffff;
#endif
    }

  if (aouthdr_int->tsize)
    {
      aouthdr_int->text_start += a->ImageBase;
#ifndef COFF_WITH_pep
      aouthdr_int->text_start &= 0xffffffff;
#endif
    }

#ifndef COFF_WITH_pep
  /* PE32+ does not have data_start member!  */
  if (aouthdr_int->dsize)
    {
      aouthdr_int->data_start += a->ImageBase;
      aouthdr_int->data_start &= 0xffffffff;
    }
#endif

#ifdef POWERPC_LE_PE
  /* These three fields are normally set up by ppc_relocate_section.
     In the case of reading a file in, we can pick them up from the
     DataDirectory.  */
  first_thunk_address = a->DataDirectory[12].VirtualAddress;
  thunk_size = a->DataDirectory[12].Size;
  import_table_size = a->DataDirectory[1].Size;
#endif
}

/* A support function for below.  */

static void
add_data_entry(bfd *abfd, struct internal_extra_pe_aouthdr *aout,
	       int idx, const char *name, bfd_vma base)
{
  asection *sec = bfd_get_section_by_name (abfd, name);

  /* Add import directory information if it exists.  */
  if ((sec != NULL)
      && (coff_section_data (abfd, sec) != NULL)
      && (pei_section_data (abfd, sec) != NULL))
    {
      /* If data directory is empty, rva also should be 0.  */
      size_t size = (size_t)pei_section_data(abfd, sec)->virt_size;
      aout->DataDirectory[idx].Size = (long)size;

      if (size)
	{
	  aout->DataDirectory[idx].VirtualAddress =
	    (sec->vma - base) & 0xffffffff;
	  sec->flags |= SEC_DATA;
	}
    }
}

/* */
unsigned int
_bfd_XXi_swap_aouthdr_out(bfd *abfd, void *in, void *out)
{
  struct internal_aouthdr *aouthdr_in = (struct internal_aouthdr *)in;
  pe_data_type *pe = pe_data (abfd);
  struct internal_extra_pe_aouthdr *extra = &pe->pe_opthdr;
  PEAOUTHDR *aouthdr_out = (PEAOUTHDR *)out;
  bfd_vma sa, fa, ib;
  IMAGE_DATA_DIRECTORY idata2, idata5, tls;

  if (pe->force_minimum_alignment)
    {
      if (!extra->FileAlignment)
	extra->FileAlignment = PE_DEF_FILE_ALIGNMENT;
      if (!extra->SectionAlignment)
	extra->SectionAlignment = PE_DEF_SECTION_ALIGNMENT;
    }

  if (extra->Subsystem == IMAGE_SUBSYSTEM_UNKNOWN)
    extra->Subsystem = (short)pe->target_subsystem;

  sa = extra->SectionAlignment;
  fa = extra->FileAlignment;
  ib = extra->ImageBase;

  idata2 = pe->pe_opthdr.DataDirectory[1];
  idata5 = pe->pe_opthdr.DataDirectory[12];
  tls = pe->pe_opthdr.DataDirectory[9];

  if (aouthdr_in->tsize)
    {
      aouthdr_in->text_start -= ib;
#ifndef COFF_WITH_pep
      aouthdr_in->text_start &= 0xffffffff;
#endif
    }

  if (aouthdr_in->dsize)
    {
      aouthdr_in->data_start -= ib;
#ifndef COFF_WITH_pep
      aouthdr_in->data_start &= 0xffffffff;
#endif
    }

  if (aouthdr_in->entry)
    {
      aouthdr_in->entry -= ib;
#ifndef COFF_WITH_pep
      aouthdr_in->entry &= 0xffffffff;
#endif
    }

#define FA(x) (((x) + fa -1 ) & (- fa))
#define SA(x) (((x) + sa -1 ) & (- sa))

  /* We like to have the sizes aligned.  */
  aouthdr_in->bsize = FA (aouthdr_in->bsize);

  extra->NumberOfRvaAndSizes = IMAGE_NUMBEROF_DIRECTORY_ENTRIES;

  /* First null out all data directory entries.  */
  memset (extra->DataDirectory, 0, sizeof (extra->DataDirectory));

  add_data_entry (abfd, extra, 0, ".edata", ib);
  add_data_entry (abfd, extra, 2, ".rsrc", ib);
  add_data_entry (abfd, extra, 3, ".pdata", ib);

  /* In theory we do not need to call add_data_entry for .idata$2 or
     .idata$5.  It will be done in bfd_coff_final_link where all the
     required information is available.  If however, we are not going
     to perform a final link, eg because we have been invoked by objcopy
     or strip, then we need to make sure that these Data Directory
     entries are initialised properly.

     So - we copy the input values into the output values, and then, if
     a final link is going to be performed, it can overwrite them.  */
  extra->DataDirectory[1]  = idata2;
  extra->DataDirectory[12] = idata5;
  extra->DataDirectory[9] = tls;

  if (extra->DataDirectory[1].VirtualAddress == 0)
    /* Until other .idata fixes are made (pending patch), the entry for
       .idata is needed for backwards compatibility.  FIXME.  */
    add_data_entry (abfd, extra, 1, ".idata", ib);

  /* For some reason, the virtual size (which is what's set by
     add_data_entry) for .reloc is not the same as the size recorded
     in this slot by MSVC; it doesn't seem to cause problems (so far),
     but since it's the best we've got, use it.  It does do the right
     thing for .pdata.  */
  if (pe->has_reloc_section)
    add_data_entry (abfd, extra, 5, ".reloc", ib);

  {
    asection *sec;
    bfd_vma hsize = 0;
    bfd_vma dsize = 0;
    bfd_vma isize = 0;
    bfd_vma tsize = 0;

    for (sec = abfd->sections; sec; sec = sec->next)
      {
	int rounded = (int)FA(sec->size);

	/* The first non-zero section filepos is the header size.
	   Sections without contents will have a filepos of 0.  */
	if (hsize == 0)
	  hsize = (bfd_vma)sec->filepos;
	if (sec->flags & SEC_DATA)
	  dsize += (bfd_vma)rounded;
	if (sec->flags & SEC_CODE)
	  tsize += (bfd_vma)rounded;
	/* The image size is the total VIRTUAL size (which is what is
	   in the virt_size field).  Files have been seen (from MSVC
	   5.0 link.exe) where the file size of the .data segment is
	   quite small compared to the virtual size.  Without this
	   fix, strip munges the file.  */
	if ((coff_section_data(abfd, sec) != NULL)
	    && (pei_section_data(abfd, sec) != NULL))
	  isize += SA(FA(pei_section_data(abfd, sec)->virt_size));
      }

    aouthdr_in->dsize = dsize;
    aouthdr_in->tsize = tsize;
    extra->SizeOfHeaders = (long)hsize;
    extra->SizeOfImage = (long)(SA(hsize) + isize);
  }

  H_PUT_16(abfd, (bfd_vma)aouthdr_in->magic, aouthdr_out->standard.magic);

#define LINKER_VERSION 256 /* That is, 2.56 */

  /* This piece of magic sets the "linker version" field to
     LINKER_VERSION.  */
  H_PUT_16(abfd, (bfd_vma)((LINKER_VERSION / 100UL)
                           + ((LINKER_VERSION % 100UL) * 256UL)),
	   aouthdr_out->standard.vstamp);

  PUT_AOUTHDR_TSIZE(abfd, aouthdr_in->tsize, aouthdr_out->standard.tsize);
  PUT_AOUTHDR_DSIZE(abfd, aouthdr_in->dsize, aouthdr_out->standard.dsize);
  PUT_AOUTHDR_BSIZE(abfd, aouthdr_in->bsize, aouthdr_out->standard.bsize);
  PUT_AOUTHDR_ENTRY(abfd, aouthdr_in->entry, aouthdr_out->standard.entry);
  PUT_AOUTHDR_TEXT_START(abfd, aouthdr_in->text_start,
			 aouthdr_out->standard.text_start);

#ifndef COFF_WITH_pep
  /* PE32+ does not have data_start member!  */
  PUT_AOUTHDR_DATA_START(abfd, aouthdr_in->data_start,
			 aouthdr_out->standard.data_start);
#endif /* !COFF_WITH_pep */

  PUT_OPTHDR_IMAGE_BASE(abfd, extra->ImageBase, aouthdr_out->ImageBase);
  H_PUT_32(abfd, extra->SectionAlignment, aouthdr_out->SectionAlignment);
  H_PUT_32(abfd, extra->FileAlignment, aouthdr_out->FileAlignment);
  H_PUT_16(abfd, (bfd_vma)extra->MajorOperatingSystemVersion,
	   aouthdr_out->MajorOperatingSystemVersion);
  H_PUT_16(abfd, (bfd_vma)extra->MinorOperatingSystemVersion,
	   aouthdr_out->MinorOperatingSystemVersion);
  H_PUT_16(abfd, (bfd_vma)extra->MajorImageVersion,
           aouthdr_out->MajorImageVersion);
  H_PUT_16(abfd, (bfd_vma)extra->MinorImageVersion,
           aouthdr_out->MinorImageVersion);
  H_PUT_16(abfd, (bfd_vma)extra->MajorSubsystemVersion,
	   aouthdr_out->MajorSubsystemVersion);
  H_PUT_16(abfd, (bfd_vma)extra->MinorSubsystemVersion,
	   aouthdr_out->MinorSubsystemVersion);
  H_PUT_32(abfd, (bfd_vma)extra->Reserved1, aouthdr_out->Reserved1);
  H_PUT_32(abfd, (bfd_vma)extra->SizeOfImage, aouthdr_out->SizeOfImage);
  H_PUT_32(abfd, (bfd_vma)extra->SizeOfHeaders,
           aouthdr_out->SizeOfHeaders);
  H_PUT_32(abfd, (bfd_vma)extra->CheckSum, aouthdr_out->CheckSum);
  H_PUT_16(abfd, (bfd_vma)extra->Subsystem, aouthdr_out->Subsystem);
  H_PUT_16(abfd, (bfd_vma)extra->DllCharacteristics,
           aouthdr_out->DllCharacteristics);
  PUT_OPTHDR_SIZE_OF_STACK_RESERVE(abfd, extra->SizeOfStackReserve,
				   aouthdr_out->SizeOfStackReserve);
  PUT_OPTHDR_SIZE_OF_STACK_COMMIT(abfd, extra->SizeOfStackCommit,
				  aouthdr_out->SizeOfStackCommit);
  PUT_OPTHDR_SIZE_OF_HEAP_RESERVE(abfd, extra->SizeOfHeapReserve,
				  aouthdr_out->SizeOfHeapReserve);
  PUT_OPTHDR_SIZE_OF_HEAP_COMMIT(abfd, extra->SizeOfHeapCommit,
				 aouthdr_out->SizeOfHeapCommit);
  H_PUT_32(abfd, (bfd_vma)extra->LoaderFlags, aouthdr_out->LoaderFlags);
  H_PUT_32(abfd, (bfd_vma)extra->NumberOfRvaAndSizes,
	   aouthdr_out->NumberOfRvaAndSizes);
  {
    int idx;

    for (idx = 0; idx < 16; idx++)
      {
	H_PUT_32(abfd, (bfd_vma)extra->DataDirectory[idx].VirtualAddress,
		 aouthdr_out->DataDirectory[idx][0]);
	H_PUT_32(abfd, (bfd_vma)extra->DataDirectory[idx].Size,
		 aouthdr_out->DataDirectory[idx][1]);
      }
  }

  return AOUTSZ;
}

unsigned int
_bfd_XXi_only_swap_filehdr_out(bfd *abfd, void *in, void *out)
{
  int idx;
  struct internal_filehdr *filehdr_in = (struct internal_filehdr *)in;
  struct external_PEI_filehdr *filehdr_out = (struct external_PEI_filehdr *)out;

  if (pe_data(abfd)->has_reloc_section)
    filehdr_in->f_flags &= (unsigned short)~F_RELFLG;

  if (pe_data(abfd)->dll)
    filehdr_in->f_flags |= F_DLL;

  filehdr_in->pe.e_magic = DOSMAGIC;
  filehdr_in->pe.e_cblp = 0x90;
  filehdr_in->pe.e_cp = 0x3;
  filehdr_in->pe.e_crlc = 0x0;
  filehdr_in->pe.e_cparhdr = 0x4;
  filehdr_in->pe.e_minalloc = 0x0;
  filehdr_in->pe.e_maxalloc = 0xffff;
  filehdr_in->pe.e_ss = 0x0;
  filehdr_in->pe.e_sp = 0xb8;
  filehdr_in->pe.e_csum = 0x0;
  filehdr_in->pe.e_ip = 0x0;
  filehdr_in->pe.e_cs = 0x0;
  filehdr_in->pe.e_lfarlc = 0x40;
  filehdr_in->pe.e_ovno = 0x0;

  for (idx = 0; idx < 4; idx++)
    filehdr_in->pe.e_res[idx] = 0x0;

  filehdr_in->pe.e_oemid = 0x0;
  filehdr_in->pe.e_oeminfo = 0x0;

  for (idx = 0; idx < 10; idx++)
    filehdr_in->pe.e_res2[idx] = 0x0;

  filehdr_in->pe.e_lfanew = 0x80;

  /* This next collection of data are mostly just characters.  It
     appears to be constant within the headers put on NT exes.  */
  filehdr_in->pe.dos_message[0] = 0x0eba1f0e;
  filehdr_in->pe.dos_message[1] = 0xcd09b400;
  filehdr_in->pe.dos_message[2] = 0x4c01b821;
  filehdr_in->pe.dos_message[3] = 0x685421cd;
  filehdr_in->pe.dos_message[4] = 0x70207369;
  filehdr_in->pe.dos_message[5] = 0x72676f72;
  filehdr_in->pe.dos_message[6] = 0x63206d61;
  filehdr_in->pe.dos_message[7] = 0x6f6e6e61;
  filehdr_in->pe.dos_message[8] = 0x65622074;
  filehdr_in->pe.dos_message[9] = 0x6e757220;
  filehdr_in->pe.dos_message[10] = 0x206e6920;
  filehdr_in->pe.dos_message[11] = 0x20534f44;
  filehdr_in->pe.dos_message[12] = 0x65646f6d;
  filehdr_in->pe.dos_message[13] = 0x0a0d0d2e;
  filehdr_in->pe.dos_message[14] = 0x24;
  filehdr_in->pe.dos_message[15] = 0x0;
  filehdr_in->pe.nt_signature = NT_SIGNATURE;

  H_PUT_16(abfd, (bfd_vma)filehdr_in->f_magic, filehdr_out->f_magic);
  H_PUT_16(abfd, (bfd_vma)filehdr_in->f_nscns, filehdr_out->f_nscns);

  H_PUT_32(abfd, (bfd_vma)time(0), filehdr_out->f_timdat);
  PUT_FILEHDR_SYMPTR(abfd, filehdr_in->f_symptr,
		     filehdr_out->f_symptr);
  H_PUT_32(abfd, (bfd_vma)filehdr_in->f_nsyms, filehdr_out->f_nsyms);
  H_PUT_16(abfd, (bfd_vma)filehdr_in->f_opthdr, filehdr_out->f_opthdr);
  H_PUT_16(abfd, (bfd_vma)filehdr_in->f_flags, filehdr_out->f_flags);

  /* Put in extra dos header stuff.  This data remains essentially
     constant, it just has to be tacked on to the beginning of all exes
     for NT.  */
  H_PUT_16(abfd, (bfd_vma)filehdr_in->pe.e_magic, filehdr_out->e_magic);
  H_PUT_16(abfd, (bfd_vma)filehdr_in->pe.e_cblp, filehdr_out->e_cblp);
  H_PUT_16(abfd, (bfd_vma)filehdr_in->pe.e_cp, filehdr_out->e_cp);
  H_PUT_16(abfd, (bfd_vma)filehdr_in->pe.e_crlc, filehdr_out->e_crlc);
  H_PUT_16(abfd, (bfd_vma)filehdr_in->pe.e_cparhdr,
           filehdr_out->e_cparhdr);
  H_PUT_16(abfd, (bfd_vma)filehdr_in->pe.e_minalloc,
           filehdr_out->e_minalloc);
  H_PUT_16(abfd, (bfd_vma)filehdr_in->pe.e_maxalloc,
           filehdr_out->e_maxalloc);
  H_PUT_16(abfd, (bfd_vma)filehdr_in->pe.e_ss, filehdr_out->e_ss);
  H_PUT_16(abfd, (bfd_vma)filehdr_in->pe.e_sp, filehdr_out->e_sp);
  H_PUT_16(abfd, (bfd_vma)filehdr_in->pe.e_csum, filehdr_out->e_csum);
  H_PUT_16(abfd, (bfd_vma)filehdr_in->pe.e_ip, filehdr_out->e_ip);
  H_PUT_16(abfd, (bfd_vma)filehdr_in->pe.e_cs, filehdr_out->e_cs);
  H_PUT_16(abfd, (bfd_vma)filehdr_in->pe.e_lfarlc, filehdr_out->e_lfarlc);
  H_PUT_16(abfd, (bfd_vma)filehdr_in->pe.e_ovno, filehdr_out->e_ovno);

  for (idx = 0; idx < 4; idx++)
    H_PUT_16(abfd, (bfd_vma)filehdr_in->pe.e_res[idx],
             filehdr_out->e_res[idx]);

  H_PUT_16(abfd, (bfd_vma)filehdr_in->pe.e_oemid, filehdr_out->e_oemid);
  H_PUT_16(abfd, (bfd_vma)filehdr_in->pe.e_oeminfo,
           filehdr_out->e_oeminfo);

  for (idx = 0; idx < 10; idx++)
    H_PUT_16(abfd, (bfd_vma)filehdr_in->pe.e_res2[idx],
             filehdr_out->e_res2[idx]);

  H_PUT_32(abfd, filehdr_in->pe.e_lfanew, filehdr_out->e_lfanew);

  for (idx = 0; idx < 16; idx++)
    H_PUT_32(abfd, (bfd_vma)filehdr_in->pe.dos_message[idx],
	     filehdr_out->dos_message[idx]);

  /* Also put in the NT signature: */
  H_PUT_32(abfd, filehdr_in->pe.nt_signature, filehdr_out->nt_signature);

  return FILHSZ;
}

unsigned int
_bfd_XX_only_swap_filehdr_out(bfd *abfd, void *in, void *out)
{
  struct internal_filehdr *filehdr_in = (struct internal_filehdr *)in;
  FILHDR *filehdr_out = (FILHDR *)out;

  H_PUT_16(abfd, (bfd_vma)filehdr_in->f_magic, filehdr_out->f_magic);
  H_PUT_16(abfd, (bfd_vma)filehdr_in->f_nscns, filehdr_out->f_nscns);
  H_PUT_32(abfd, (bfd_vma)filehdr_in->f_timdat, filehdr_out->f_timdat);
  PUT_FILEHDR_SYMPTR(abfd, filehdr_in->f_symptr, filehdr_out->f_symptr);
  H_PUT_32(abfd, (bfd_vma)filehdr_in->f_nsyms, filehdr_out->f_nsyms);
  H_PUT_16(abfd, (bfd_vma)filehdr_in->f_opthdr, filehdr_out->f_opthdr);
  H_PUT_16(abfd, (bfd_vma)filehdr_in->f_flags, filehdr_out->f_flags);

  return FILHSZ;
}

unsigned int
_bfd_XXi_swap_scnhdr_out(bfd *abfd, void *in, void *out)
{
  struct internal_scnhdr *scnhdr_int = (struct internal_scnhdr *)in;
  SCNHDR *scnhdr_ext = (SCNHDR *)out;
  unsigned int ret = SCNHSZ;
  bfd_vma ps;
  bfd_vma ss;

  memcpy(scnhdr_ext->s_name, scnhdr_int->s_name, sizeof(scnhdr_int->s_name));

  PUT_SCNHDR_VADDR (abfd,
		    ((scnhdr_int->s_vaddr
		      - pe_data (abfd)->pe_opthdr.ImageBase)
		     & 0xffffffff),
		    scnhdr_ext->s_vaddr);

  /* NT wants the size data to be rounded up to the next
     NT_FILE_ALIGNMENT, but zero if it has no content (as in .bss,
     sometimes).  */
  if ((scnhdr_int->s_flags & IMAGE_SCN_CNT_UNINITIALIZED_DATA) != 0)
    {
      if (bfd_pe_executable_p (abfd))
	{
	  ps = scnhdr_int->s_size;
	  ss = 0;
	}
      else
       {
         ps = 0;
         ss = scnhdr_int->s_size;
       }
    }
  else
    {
      if (bfd_pe_executable_p (abfd))
	ps = scnhdr_int->s_paddr;
      else
	ps = 0;

      ss = scnhdr_int->s_size;
    }

  PUT_SCNHDR_SIZE (abfd, ss,
		   scnhdr_ext->s_size);

  /* s_paddr in PE is really the virtual size.  */
  PUT_SCNHDR_PADDR (abfd, ps, scnhdr_ext->s_paddr);

  PUT_SCNHDR_SCNPTR (abfd, scnhdr_int->s_scnptr,
		     scnhdr_ext->s_scnptr);
  PUT_SCNHDR_RELPTR (abfd, scnhdr_int->s_relptr,
		     scnhdr_ext->s_relptr);
  PUT_SCNHDR_LNNOPTR (abfd, scnhdr_int->s_lnnoptr,
		      scnhdr_ext->s_lnnoptr);

  {
    /* Extra flags must be set when dealing with PE.  All sections should also
       have the IMAGE_SCN_MEM_READ (0x40000000) flag set.  In addition, the
       .text section must have IMAGE_SCN_MEM_EXECUTE (0x20000000) and the data
       sections (.idata, .data, .bss, .CRT) must have IMAGE_SCN_MEM_WRITE set
       (this is especially important when dealing with the .idata section since
       the addresses for routines from .dlls must be overwritten).  If .reloc
       section data is ever generated, we must add IMAGE_SCN_MEM_DISCARDABLE
       (0x02000000).  Also, the resource data should also be read and
       writable.  */

    /* FIXME: Alignment is also encoded in this field, at least on PPC and
       ARM-WINCE.  Although - how do we get the original alignment field
       back ?  */

    typedef struct
    {
      const char * 	section_name;
      unsigned long	must_have;
    }
    pe_required_section_flags;

    pe_required_section_flags known_sections [] =
      {
	{ ".arch",  IMAGE_SCN_MEM_READ | IMAGE_SCN_CNT_INITIALIZED_DATA | IMAGE_SCN_MEM_DISCARDABLE | IMAGE_SCN_ALIGN_8BYTES },
	{ ".bss",   IMAGE_SCN_MEM_READ | IMAGE_SCN_CNT_UNINITIALIZED_DATA | IMAGE_SCN_MEM_WRITE },
	{ ".data",  IMAGE_SCN_MEM_READ | IMAGE_SCN_CNT_INITIALIZED_DATA | IMAGE_SCN_MEM_WRITE },
	{ ".edata", IMAGE_SCN_MEM_READ | IMAGE_SCN_CNT_INITIALIZED_DATA },
	{ ".idata", IMAGE_SCN_MEM_READ | IMAGE_SCN_CNT_INITIALIZED_DATA | IMAGE_SCN_MEM_WRITE },
	{ ".pdata", IMAGE_SCN_MEM_READ | IMAGE_SCN_CNT_INITIALIZED_DATA },
	{ ".rdata", IMAGE_SCN_MEM_READ | IMAGE_SCN_CNT_INITIALIZED_DATA },
	{ ".reloc", IMAGE_SCN_MEM_READ | IMAGE_SCN_CNT_INITIALIZED_DATA | IMAGE_SCN_MEM_DISCARDABLE },
	{ ".rsrc",  IMAGE_SCN_MEM_READ | IMAGE_SCN_CNT_INITIALIZED_DATA | IMAGE_SCN_MEM_WRITE },
	{ ".text" , IMAGE_SCN_MEM_READ | IMAGE_SCN_CNT_CODE | IMAGE_SCN_MEM_EXECUTE },
	{ ".tls",   IMAGE_SCN_MEM_READ | IMAGE_SCN_CNT_INITIALIZED_DATA | IMAGE_SCN_MEM_WRITE },
	{ ".xdata", IMAGE_SCN_MEM_READ | IMAGE_SCN_CNT_INITIALIZED_DATA },
	{ NULL, 0}
      };

    pe_required_section_flags * p;

    /* We have defaulted to adding the IMAGE_SCN_MEM_WRITE flag, but now
       we know exactly what this specific section wants so we remove it
       and then allow the must_have field to add it back in if necessary.
       However, we don't remove IMAGE_SCN_MEM_WRITE flag from .text if the
       default WP_TEXT file flag has been cleared.  WP_TEXT may be cleared
       by ld --enable-auto-import (if auto-import is actually needed),
       by ld --omagic, or by obcopy --writable-text.  */

    for (p = known_sections; p->section_name; p++)
      if (strcmp(scnhdr_int->s_name, p->section_name) == 0)
	{
	  if (strcmp(scnhdr_int->s_name, ".text")
	      || (bfd_get_file_flags(abfd) & WP_TEXT))
	    scnhdr_int->s_flags &= ~IMAGE_SCN_MEM_WRITE;
	  scnhdr_int->s_flags |= (long)p->must_have;
	  break;
	}

    H_PUT_32(abfd, (bfd_vma)scnhdr_int->s_flags, scnhdr_ext->s_flags);
  }

  if (coff_data(abfd)->link_info
      && ! coff_data(abfd)->link_info->relocatable
      && ! coff_data(abfd)->link_info->shared
      && (strcmp(scnhdr_int->s_name, ".text") == 0))
    {
      /* By inference from looking at MS output, the 32 bit field
	 which is the combination of the number_of_relocs and
	 number_of_linenos is used for the line number count in
	 executables.  A 16-bit field won't do for cc1.  The MS
	 document says that the number of relocs is zero for
	 executables, but the 17-th bit has been observed to be there.
	 Overflow is not an issue: a 4G-line program will overflow a
	 bunch of other fields long before this!  */
      H_PUT_16(abfd, (bfd_vma)(scnhdr_int->s_nlnno & 0xffff), scnhdr_ext->s_nlnno);
      H_PUT_16(abfd, (bfd_vma)(scnhdr_int->s_nlnno >> 16), scnhdr_ext->s_nreloc);
    }
  else
    {
      if (scnhdr_int->s_nlnno <= 0xffff)
	H_PUT_16(abfd, (bfd_vma)scnhdr_int->s_nlnno, scnhdr_ext->s_nlnno);
      else
	{
	  (*_bfd_error_handler)(_("%s: line number overflow: 0x%lx > 0xffff"),
                                bfd_get_filename(abfd),
                                scnhdr_int->s_nlnno);
	  bfd_set_error(bfd_error_file_truncated);
	  H_PUT_16(abfd, (bfd_vma)0xffff, scnhdr_ext->s_nlnno);
	  ret = 0;
	}

      /* Although we could encode 0xffff relocs here, we do not, to be
         consistent with other parts of bfd. Also it lets us warn, as
         we should never see 0xffff here w/o having the overflow flag
         set.  */
      if (scnhdr_int->s_nreloc < 0xffff)
	H_PUT_16(abfd, (bfd_vma)scnhdr_int->s_nreloc, scnhdr_ext->s_nreloc);
      else
	{
	  /* PE can deal with large #s of relocs, but not here: */
	  H_PUT_16(abfd, (bfd_vma)0xffff, scnhdr_ext->s_nreloc);
	  scnhdr_int->s_flags |= IMAGE_SCN_LNK_NRELOC_OVFL;
	  H_PUT_32(abfd, (bfd_vma)scnhdr_int->s_flags, scnhdr_ext->s_flags);
	}
    }
  return ret;
}

static const char *dir_names[IMAGE_NUMBEROF_DIRECTORY_ENTRIES] =
{
  N_("Export Directory [.edata (or where ever we found it)]"),
  N_("Import Directory [parts of .idata]"),
  N_("Resource Directory [.rsrc]"),
  N_("Exception Directory [.pdata]"),
  N_("Security Directory"),
  N_("Base Relocation Directory [.reloc]"),
  N_("Debug Directory"),
  N_("Description Directory"),
  N_("Special Directory"),
  N_("Thread Storage Directory [.tls]"),
  N_("Load Configuration Directory"),
  N_("Bound Import Directory"),
  N_("Import Address Table Directory"),
  N_("Delay Import Directory"),
  N_("Reserved"),
  N_("Reserved")
};

#ifdef POWERPC_LE_PE
/* The code for the PPC really falls in the "architecture dependent"
   category.  However, it's not clear that anyone will ever care, so
   we're ignoring the issue for now; if/when PPC matters, some of this
   may need to go into peicode.h, or arguments passed to enable the
   PPC- specific code.  */
#endif

static bfd_boolean
pe_print_idata (bfd * abfd, void * vfile)
{
  FILE *file = (FILE *) vfile;
  bfd_byte *data;
  asection *section;
  bfd_signed_vma adj;

#ifdef POWERPC_LE_PE
  asection *rel_section = bfd_get_section_by_name (abfd, ".reldata");
#endif

  bfd_size_type datasize = 0;
  bfd_size_type dataoff;
  bfd_size_type i;
  int onaline = 20;

  pe_data_type *pe = pe_data (abfd);
  struct internal_extra_pe_aouthdr *extra = &pe->pe_opthdr;

  bfd_vma addr;

  addr = extra->DataDirectory[1].VirtualAddress;

  if (addr == 0 && extra->DataDirectory[1].Size == 0)
    {
      /* Maybe the extra header isn't there.  Look for the section.  */
      section = bfd_get_section_by_name (abfd, ".idata");
      if (section == NULL)
	return TRUE;

      addr = section->vma;
      datasize = section->size;
      if (datasize == 0)
	return TRUE;
    }
  else
    {
      addr += extra->ImageBase;
      for (section = abfd->sections; section != NULL; section = section->next)
	{
	  datasize = section->size;
	  if (addr >= section->vma && addr < section->vma + datasize)
	    break;
	}

      if (section == NULL)
	{
	  fprintf (file,
		   _("\nThere is an import table, but the section containing it could not be found\n"));
	  return TRUE;
	}
    }

  fprintf (file, _("\nThere is an import table in %s at 0x%lx\n"),
	   section->name, (unsigned long) addr);

  dataoff = addr - section->vma;
  datasize -= dataoff;

#ifdef POWERPC_LE_PE
  if (rel_section != 0 && rel_section->size != 0)
    {
      /* The toc address can be found by taking the starting address,
	 which on the PPC locates a function descriptor. The
	 descriptor consists of the function code starting address
	 followed by the address of the toc. The starting address we
	 get from the bfd, and the descriptor is supposed to be in the
	 .reldata section.  */

      bfd_vma loadable_toc_address;
      bfd_vma toc_address;
      bfd_vma start_address;
      bfd_byte *data;
      bfd_vma offset;

      if (!bfd_malloc_and_get_section (abfd, rel_section, &data))
	{
	  if (data != NULL)
	    free (data);
	  return FALSE;
	}

      offset = abfd->start_address - rel_section->vma;

      if (offset >= rel_section->size || offset + 8 > rel_section->size)
        {
          if (data != NULL)
            free (data);
          return FALSE;
        }

      start_address = bfd_get_32 (abfd, data + offset);
      loadable_toc_address = bfd_get_32 (abfd, data + offset + 4);
      toc_address = loadable_toc_address - 32768;

      fprintf (file,
	       _("\nFunction descriptor located at the start address: %04lx\n"),
	       (unsigned long int) (abfd->start_address));
      fprintf (file,
	       _("\tcode-base %08lx toc (loadable/actual) %08lx/%08lx\n"),
	       start_address, loadable_toc_address, toc_address);
      if (data != NULL)
	free (data);
    }
  else
    {
      fprintf (file,
	       _("\nNo reldata section! Function descriptor not decoded.\n"));
    }
#endif /* POWERPC_LE_PE */

  fprintf (file,
	   _("\nThe Import Tables (interpreted %s section contents)\n"),
	   section->name);
  fprintf (file,
	   _("\
 vma:            Hint    Time      Forward  DLL       First\n\
                 Table   Stamp     Chain    Name      Thunk\n"));

  /* Read the whole section.  Some of the fields might be before dataoff: */
  if (!bfd_malloc_and_get_section(abfd, section, &data))
    {
      if (data != NULL)
	free(data);
      return FALSE;
    }

  adj = (bfd_signed_vma)(section->vma - extra->ImageBase);

  /* Print all image import descriptors.  */
  for (i = 0; i < datasize; i += (bfd_size_type)onaline)
    {
      bfd_vma hint_addr;
      bfd_vma time_stamp;
      bfd_vma forward_chain;
      bfd_vma dll_name;
      bfd_vma first_thunk;
      int idx = 0;
      bfd_size_type j;
      char *dll;

      /* Print (i + extra->DataDirectory[1].VirtualAddress).  */
      fprintf(file, " %08lx\t", (i + (unsigned long)adj + dataoff));
      hint_addr = bfd_get_32 (abfd, data + i + dataoff);
      time_stamp = bfd_get_32 (abfd, data + i + 4 + dataoff);
      forward_chain = bfd_get_32 (abfd, data + i + 8 + dataoff);
      dll_name = bfd_get_32 (abfd, data + i + 12 + dataoff);
      first_thunk = bfd_get_32 (abfd, data + i + 16 + dataoff);

      fprintf (file, "%08lx %08lx %08lx %08lx %08lx\n",
	       (unsigned long) hint_addr,
	       (unsigned long) time_stamp,
	       (unsigned long) forward_chain,
	       (unsigned long) dll_name,
	       (unsigned long) first_thunk);

      if (hint_addr == 0 && first_thunk == 0)
	break;

      if ((dll_name - (bfd_vma)adj) >= section->size)
        break;

      dll = (char *) data + dll_name - adj;
      fprintf (file, _("\n\tDLL Name: %s\n"), dll);

      if (hint_addr != 0)
	{
	  bfd_byte *ft_data;
	  asection *ft_section;
	  bfd_vma ft_addr;
	  bfd_size_type ft_datasize;
	  int ft_idx;
	  int ft_allocated = 0;

	  fprintf (file, _("\tvma:  Hint/Ord Member-Name Bound-To\n"));

	  idx = (int)(hint_addr - (bfd_vma)adj);

	  ft_addr = first_thunk + extra->ImageBase;
	  ft_data = data;
	  ft_idx = (int)(first_thunk - (bfd_vma)adj);
	  ft_allocated = 0;

	  if (first_thunk != hint_addr)
	    {
	      /* Find the section which contains the first thunk.  */
	      for (ft_section = abfd->sections;
		   ft_section != NULL;
		   ft_section = ft_section->next)
		{
		  ft_datasize = ft_section->size;
		  if (ft_addr >= ft_section->vma
		      && ft_addr < ft_section->vma + ft_datasize)
		    break;
		}

	      if (ft_section == NULL)
		{
		  fprintf (file,
		       _("\nThere is a first thunk, but the section containing it could not be found\n"));
		  continue;
		}

	      /* Now check to see if this section is the same as our current
		 section.  If it is not then we will have to load its data in.  */
	      if (ft_section == section)
		{
		  ft_data = data;
		  ft_idx = (int)(first_thunk - (bfd_vma)adj);
		}
	      else
		{
		  ft_idx =
		    (int)(first_thunk - (ft_section->vma - extra->ImageBase));
		  ft_data = (bfd_byte *)bfd_malloc(datasize);
		  if (ft_data == NULL)
		    continue;

		  /* Read datasize bfd_bytes starting at offset ft_idx: */
		  if (!bfd_get_section_contents(abfd, ft_section, ft_data,
                                                (file_ptr)ft_idx,
                                                datasize))
		    {
		      free(ft_data);
		      continue;
		    }

		  ft_idx = 0;
		  ft_allocated = 1;
		}
	    }

	  /* Print HintName vector entries.  */
	  for (j = 0; j < datasize; j += 4)
	    {
	      unsigned long member =
		(unsigned long)bfd_get_32(abfd, data + idx + j);

	      /* Print single IMAGE_IMPORT_BY_NAME vector.  */
	      if (member == 0)
		break;

	      if (member & 0x80000000)
		fprintf (file, "\t%04lx\t %4lu  <none>",
			 member, member & 0x7fffffff);
	      else
		{
		  int ordinal;
		  char *member_name;

		  ordinal = (int)bfd_get_16(abfd, data + member - adj);
		  member_name = (char *) data + member - adj + 2;
		  fprintf (file, "\t%04lx\t %4d  %s",
			   member, ordinal, member_name);
		}

	      /* If the time stamp is not zero, the import address
		 table holds actual addresses.  */
	      if (time_stamp != 0
		  && first_thunk != 0
		  && first_thunk != hint_addr)
		fprintf (file, "\t%04lx",
			 (long) bfd_get_32 (abfd, ft_data + ft_idx + j));

	      fprintf (file, "\n");
	    }

	  if (ft_allocated)
	    free (ft_data);
	}

      fprintf (file, "\n");
    }

  free (data);

  return TRUE;
}

/* */
static bfd_boolean
pe_print_edata(bfd *abfd, void *vfile)
{
  FILE *file = (FILE *)vfile;
  bfd_byte *data;
  asection *section;
  bfd_size_type datasize = 0;
  bfd_size_type dataoff;
  bfd_size_type i;
  bfd_signed_vma adj;
  struct EDT_type
  {
    long export_flags;          /* Reserved - should be zero.  */
    long time_stamp;
    short major_ver;
    short minor_ver;
    bfd_vma name;               /* RVA - relative to image base.  */
    long base;                  /* Ordinal base.  */
    unsigned long num_functions;/* Number in the export address table.  */
    unsigned long num_names;    /* Number in the name pointer table.  */
    bfd_vma eat_addr;		/* RVA to the export address table.  */
    bfd_vma npt_addr;		/* RVA to the Export Name Pointer Table.  */
    bfd_vma ot_addr;		/* RVA to the Ordinal Table.  */
  } edt;

  pe_data_type *pe = pe_data (abfd);
  struct internal_extra_pe_aouthdr *extra = &pe->pe_opthdr;

  bfd_vma addr;

  addr = extra->DataDirectory[0].VirtualAddress;

  if (addr == 0 && extra->DataDirectory[0].Size == 0)
    {
      /* Maybe the extra header isn't there.  Look for the section.  */
      section = bfd_get_section_by_name (abfd, ".edata");
      if (section == NULL)
	return TRUE;

      addr = section->vma;
      dataoff = 0;
      datasize = section->size;
      if (datasize == 0)
	return TRUE;
    }
  else
    {
      addr += extra->ImageBase;

      for (section = abfd->sections; section != NULL; section = section->next)
	if (addr >= section->vma && addr < section->vma + section->size)
	  break;

      if (section == NULL)
	{
	  fprintf(file,
                  _("\nThere is an export table, but the section containing it could not be found\n"));
	  return TRUE;
	}

      dataoff = (addr - section->vma);
      datasize = (bfd_size_type)extra->DataDirectory[0].Size;
      if (datasize > (section->size - dataoff))
	{
	  fprintf(file,
                  _("\nThere is an export table in %s, but it does not fit into that section\n"),
                  section->name);
	  return TRUE;
	}
    }

  fprintf(file, _("\nThere is an export table in %s at 0x%lx\n"),
          section->name, (unsigned long)addr);

  data = (bfd_byte *)bfd_malloc(datasize);
  if (data == NULL)
    return FALSE;

  if (! bfd_get_section_contents(abfd, section, data,
                                 (file_ptr)dataoff, datasize))
    return FALSE;

  /* Go get Export Directory Table: */
  edt.export_flags = (long)bfd_get_32(abfd, (data + 0));
  edt.time_stamp = (long)bfd_get_32(abfd, (data + 4));
  edt.major_ver = (short)bfd_get_16(abfd, (data + 8));
  edt.minor_ver = (short)bfd_get_16(abfd, (data + 10));
  edt.name = bfd_get_32(abfd, (data + 12));
  edt.base = (long)bfd_get_32(abfd, (data + 16));
  edt.num_functions = bfd_get_32(abfd, (data + 20));
  edt.num_names = bfd_get_32(abfd, (data + 24));
  edt.eat_addr = bfd_get_32(abfd, (data + 28));
  edt.npt_addr = bfd_get_32(abfd, (data + 32));
  edt.ot_addr = bfd_get_32(abfd, (data + 36));

  adj = (bfd_signed_vma)(section->vma - extra->ImageBase + dataoff);

  /* Dump the EDT first.  */
  fprintf (file,
	   _("\nThe Export Tables (interpreted %s section contents)\n\n"),
	   section->name);

  fprintf (file,
	   _("Export Flags \t\t\t%lx\n"), (unsigned long) edt.export_flags);

  fprintf (file,
	   _("Time/Date stamp \t\t%lx\n"), (unsigned long) edt.time_stamp);

  fprintf (file,
	   _("Major/Minor \t\t\t%d/%d\n"), edt.major_ver, edt.minor_ver);

  fprintf (file,
	   _("Name \t\t\t\t"));
  fprintf_vma (file, edt.name);
  fprintf (file,
	   " %s\n", data + edt.name - adj);

  fprintf (file,
	   _("Ordinal Base \t\t\t%ld\n"), edt.base);

  fprintf (file,
	   _("Number in:\n"));

  fprintf (file,
	   _("\tExport Address Table \t\t%08lx\n"),
	   edt.num_functions);

  fprintf (file,
	   _("\t[Name Pointer/Ordinal] Table\t%08lx\n"), edt.num_names);

  fprintf (file,
	   _("Table Addresses\n"));

  fprintf (file,
	   _("\tExport Address Table \t\t"));
  fprintf_vma (file, edt.eat_addr);
  fprintf (file, "\n");

  fprintf (file,
	   _("\tName Pointer Table \t\t"));
  fprintf_vma (file, edt.npt_addr);
  fprintf (file, "\n");

  fprintf (file,
	   _("\tOrdinal Table \t\t\t"));
  fprintf_vma (file, edt.ot_addr);
  fprintf (file, "\n");

  /* The next table to find is the Export Address Table. It's basically
     a list of pointers that either locate a function in this dll, or
     forward the call to another dll. Something like:
      typedef union
      {
        long export_rva;
        long forwarder_rva;
      } export_address_table_entry;  */

  fprintf (file,
	  _("\nExport Address Table -- Ordinal Base %ld\n"),
	  edt.base);

  for (i = 0; i < edt.num_functions; ++i)
    {
      bfd_vma eat_member = bfd_get_32 (abfd,
				       data + edt.eat_addr + (i * 4) - adj);
      if (eat_member == 0)
	continue;

      if ((eat_member - (bfd_vma)adj) <= datasize)
	{
	  /* This rva is to a name (forwarding function) in our section: */
	  /* Should locate a function descriptor.  */
	  fprintf(file, "\t[%4ld] +base[%4ld] %04lx %s -- %s\n", (long)i,
		  ((long)i + edt.base), (unsigned long)eat_member,
		  _("Forwarder RVA"), (data + eat_member - adj));
	}
      else
	{
	  /* Should locate a function descriptor in the reldata section: */
	  fprintf(file, "\t[%4ld] +base[%4ld] %04lx %s\n", (long)i,
		  ((long)i + edt.base), (unsigned long)eat_member,
		  _("Export RVA"));
	}
    }

  /* The Export Name Pointer Table is paired with the Export Ordinal Table.  */
  /* Dump them in parallel for clarity.  */
  fprintf (file,
	   _("\n[Ordinal/Name Pointer] Table\n"));

  for (i = 0; i < edt.num_names; ++i)
    {
      bfd_vma name_ptr = bfd_get_32 (abfd,
				    data +
				    edt.npt_addr
				    + (i*4) - adj);

      char *name = (char *) data + name_ptr - adj;

      bfd_vma ord = bfd_get_16 (abfd,
				    data +
				    edt.ot_addr
				    + (i*2) - adj);
      fprintf (file,
	      "\t[%4ld] %s\n", (long) ord, name);
    }

  free (data);

  return TRUE;
}

/* This really is architecture dependent.  On IA-64, a .pdata entry
   consists of three dwords containing relative virtual addresses that
   specify the start and end address of the code range the entry
   covers and the address of the corresponding unwind info data.  */

static bfd_boolean
pe_print_pdata (bfd * abfd, void * vfile)
{
#ifdef COFF_WITH_pep
# define PDATA_ROW_SIZE	(3*8)
#else
# define PDATA_ROW_SIZE	(5*4)
#endif
  FILE *file = (FILE *) vfile;
  bfd_byte *data = 0;
  asection *section = bfd_get_section_by_name (abfd, ".pdata");
  bfd_size_type datasize = 0;
  bfd_size_type i;
  bfd_size_type start, stop;
  int onaline = PDATA_ROW_SIZE;

  if (section == NULL
      || coff_section_data (abfd, section) == NULL
      || pei_section_data (abfd, section) == NULL)
    return TRUE;

  stop = pei_section_data (abfd, section)->virt_size;
  if ((stop % (bfd_size_type)onaline) != 0)
    fprintf (file,
	     _("Warning, .pdata section size (%ld) is not a multiple of %d\n"),
	     (long) stop, onaline);

  fprintf (file,
	   _("\nThe Function Table (interpreted .pdata section contents)\n"));
#ifdef COFF_WITH_pep
  fprintf (file,
	   _(" vma:\t\t\tBegin Address    End Address      Unwind Info\n"));
#else
  fprintf (file, _("\
 vma:\t\tBegin    End      EH       EH       PrologEnd  Exception\n\
     \t\tAddress  Address  Handler  Data     Address    Mask\n"));
#endif

  datasize = section->size;
  if (datasize == 0)
    return TRUE;

  if (! bfd_malloc_and_get_section (abfd, section, &data))
    {
      if (data != NULL)
	free (data);
      return FALSE;
    }

  start = 0;

  for (i = start; i < stop; i += (bfd_size_type)onaline)
    {
      bfd_vma begin_addr;
      bfd_vma end_addr;
      bfd_vma eh_handler;
      bfd_vma eh_data;
      bfd_vma prolog_end_addr;
      int em_data;
      int tmp_i = (int)i;  /* (loop counter may overflow otherwise) */

      if (((bfd_size_type)tmp_i + PDATA_ROW_SIZE) > stop) {
	break;
      } else {
        ; /* (check if using 'tmp_i' over 'i' affected anything here) */
      }


      begin_addr = GET_PDATA_ENTRY(abfd, data + i);
      end_addr = GET_PDATA_ENTRY(abfd, data + i +  4);
      eh_handler = GET_PDATA_ENTRY(abfd, data + i +  8);
      eh_data = GET_PDATA_ENTRY(abfd, data + i + 12);
      prolog_end_addr = GET_PDATA_ENTRY(abfd, data + i + 16);

      if ((begin_addr == 0) && (end_addr == 0) && (eh_handler == 0)
	  && (eh_data == 0) && (prolog_end_addr == 0))
	/* We are probably into the padding of the section now: */
	break;

      em_data = (int)(((eh_handler & 0x1) << 2) | (prolog_end_addr & 0x3));
      eh_handler &= ~(bfd_vma)0x3;
      prolog_end_addr &= ~(bfd_vma)0x3;

      fputc(' ', file);
      fprintf_vma(file, i + section->vma); fputc('\t', file);
      fprintf_vma(file, begin_addr); fputc(' ', file);
      fprintf_vma(file, end_addr); fputc(' ', file);
      fprintf_vma(file, eh_handler);
#ifndef COFF_WITH_pep
      fputc(' ', file);
      fprintf_vma(file, eh_data); fputc(' ', file);
      fprintf_vma(file, prolog_end_addr);
      fprintf(file, "   %x", em_data);
#else
      (void)em_data;
      (void)prolog_end_addr;
#endif /* COFF_WITH_pep */

#ifdef POWERPC_LE_PE
      if ((eh_handler == 0) && (eh_data != 0))
	{
	  /* Special bits here, although the meaning may be a little
	     mysterious. The only one I know for sure is 0x03
	     Code Significance
	     0x00 None
	     0x01 Register Save Millicode
	     0x02 Register Restore Millicode
	     0x03 Glue Code Sequence.  */
	  switch (eh_data)
	    {
	    case 0x01:
	      fprintf(file, _(" Register save millicode"));
	      break;
	    case 0x02:
	      fprintf(file, _(" Register restore millicode"));
	      break;
	    case 0x03:
	      fprintf(file, _(" Glue code sequence"));
	      break;
	    default:
	      break;
	    }
	}
#else
      (void)eh_handler;
#endif /* POWERPC_LE_PE */
      fprintf(file, "\n");
    }

  free(data);

  return TRUE;
}

#define IMAGE_REL_BASED_HIGHADJ 4
static const char * const tbl[] =
{
  "ABSOLUTE",
  "HIGH",
  "LOW",
  "HIGHLOW",
  "HIGHADJ",
  "MIPS_JMPADDR",
  "SECTION",
  "REL32",
  "RESERVED1",
  "MIPS_JMPADDR16",
  "DIR64",
  "HIGH3ADJ",
  "UNKNOWN",   /* MUST be last.  */
};

static bfd_boolean
pe_print_reloc(bfd *abfd, void *vfile)
{
  FILE *file = (FILE *) vfile;
  bfd_byte *data = 0;
  asection *section = bfd_get_section_by_name (abfd, ".reloc");
  bfd_size_type datasize;
  bfd_size_type i;
  bfd_size_type start, stop;

  if (section == NULL)
    return TRUE;

  if (section->size == 0)
    return TRUE;

  fprintf (file,
	   _("\n\nPE File Base Relocations (interpreted .reloc section contents)\n"));

  datasize = section->size;
  if (! bfd_malloc_and_get_section (abfd, section, &data))
    {
      if (data != NULL)
	free (data);
      return FALSE;
    }

  start = 0;

  stop = section->size;

  for (i = start; i < stop;)
    {
      int j;
      bfd_vma virtual_address;
      long number, size;

      /* The .reloc section is a sequence of blocks, with a header consisting
	 of two 32 bit quantities, followed by a number of 16 bit entries.  */
      virtual_address = bfd_get_32(abfd, (data + i));
      size = (long)bfd_get_32(abfd, (data + i + 4));
      number = (size - 8) / 2;

      if (size == 0)
	break;

      fprintf (file,
	       _("\nVirtual Address: %08lx Chunk size %ld (0x%lx) Number of fixups %ld\n"),
	       (unsigned long) virtual_address, size, size, number);

      for (j = 0; j < number; ++j)
	{
	  unsigned short e =
            (unsigned short)bfd_get_16(abfd, (data + i + 8 + (j * 2)));
	  unsigned int t = ((e & 0xF000) >> 12);
	  int off = (e & 0x0FFF);

	  if (t >= (sizeof(tbl) / sizeof(tbl[0])))
	    t = ((sizeof(tbl) / sizeof(tbl[0])) - 1U);

	  fprintf(file, _("\treloc %4d offset %4x [%4lx] %s"), j, off,
   		  (long)((bfd_vma)off + virtual_address), tbl[t]);

	  /* HIGHADJ takes an argument, - the next record *is* the
	     low 16 bits of addend.  */
	  if (t == IMAGE_REL_BASED_HIGHADJ)
	    {
	      fprintf(file, " (%4x)",
                      ((unsigned int)
                       bfd_get_16(abfd, (data + i + 8 + (j * 2) + 2))));
	      j++;
	    }

	  fprintf(file, "\n");
	}

      i += (bfd_size_type)size;
    }
  
  if (i > datasize) {
    ; /* ??? */
  }

  free (data);

  return TRUE;
}

/* Print out the program headers.  */

bfd_boolean
_bfd_XX_print_private_bfd_data_common (bfd * abfd, void * vfile)
{
  FILE *file = (FILE *) vfile;
  int j;
  pe_data_type *pe = pe_data (abfd);
  struct internal_extra_pe_aouthdr *i = &pe->pe_opthdr;
  const char *subsystem_name = NULL;

  /* The MS dumpbin program reportedly ands with 0xff0f before
     printing the characteristics field.  Not sure why.  No reason to
     emulate it here.  */
  fprintf (file, _("\nCharacteristics 0x%x\n"), pe->real_flags);
#undef PF
#define PF(x, y) if (pe->real_flags & x) { fprintf (file, "\t%s\n", y); }
  PF (IMAGE_FILE_RELOCS_STRIPPED, "relocations stripped");
  PF (IMAGE_FILE_EXECUTABLE_IMAGE, "executable");
  PF (IMAGE_FILE_LINE_NUMS_STRIPPED, "line numbers stripped");
  PF (IMAGE_FILE_LOCAL_SYMS_STRIPPED, "symbols stripped");
  PF (IMAGE_FILE_LARGE_ADDRESS_AWARE, "large address aware");
  PF (IMAGE_FILE_BYTES_REVERSED_LO, "little endian");
  PF (IMAGE_FILE_32BIT_MACHINE, "32 bit words");
  PF (IMAGE_FILE_DEBUG_STRIPPED, "debugging information removed");
  PF (IMAGE_FILE_SYSTEM, "system file");
  PF (IMAGE_FILE_DLL, "DLL");
  PF (IMAGE_FILE_BYTES_REVERSED_HI, "big endian");
#undef PF

  /* ctime implies '\n'.  */
  {
    time_t t = pe->coff.timestamp;
    fprintf (file, "\nTime/Date\t\t%s", ctime (&t));
  }
  fprintf (file, "\nImageBase\t\t");
  fprintf_vma (file, i->ImageBase);
  fprintf (file, "\nSectionAlignment\t");
  fprintf_vma (file, i->SectionAlignment);
  fprintf (file, "\nFileAlignment\t\t");
  fprintf_vma (file, i->FileAlignment);
  fprintf (file, "\nMajorOSystemVersion\t%d\n", i->MajorOperatingSystemVersion);
  fprintf (file, "MinorOSystemVersion\t%d\n", i->MinorOperatingSystemVersion);
  fprintf (file, "MajorImageVersion\t%d\n", i->MajorImageVersion);
  fprintf (file, "MinorImageVersion\t%d\n", i->MinorImageVersion);
  fprintf (file, "MajorSubsystemVersion\t%d\n", i->MajorSubsystemVersion);
  fprintf (file, "MinorSubsystemVersion\t%d\n", i->MinorSubsystemVersion);
  fprintf (file, "Win32Version\t\t%08lx\n", i->Reserved1);
  fprintf (file, "SizeOfImage\t\t%08lx\n", i->SizeOfImage);
  fprintf (file, "SizeOfHeaders\t\t%08lx\n", i->SizeOfHeaders);
  fprintf (file, "CheckSum\t\t%08lx\n", i->CheckSum);

  switch (i->Subsystem) {
    case IMAGE_SUBSYSTEM_UNKNOWN:
      subsystem_name = "unspecified";
      break;
    case IMAGE_SUBSYSTEM_NATIVE:
      subsystem_name = "NT native";
      break;
    case IMAGE_SUBSYSTEM_WINDOWS_GUI:
      subsystem_name = "Windows GUI";
      break;
    case IMAGE_SUBSYSTEM_WINDOWS_CUI:
      subsystem_name = "Windows CUI";
      break;
    case IMAGE_SUBSYSTEM_POSIX_CUI:
      subsystem_name = "POSIX CUI";
      break;
    case IMAGE_SUBSYSTEM_WINDOWS_CE_GUI:
      subsystem_name = "Wince CUI";
      break;
    case IMAGE_SUBSYSTEM_EFI_APPLICATION:
      subsystem_name = "EFI application";
      break;
    case IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER:
      subsystem_name = "EFI boot service driver";
      break;
    case IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER:
      subsystem_name = "EFI runtime driver";
      break;
    default:
      subsystem_name = "unknown subsystem";
      break;
  }

  fprintf (file, "Subsystem\t\t%08x", i->Subsystem);
  if (subsystem_name)
    fprintf (file, "\t(%s)", subsystem_name);
  fprintf (file, "\nDllCharacteristics\t%08x\n", i->DllCharacteristics);
  fprintf (file, "SizeOfStackReserve\t");
  fprintf_vma (file, i->SizeOfStackReserve);
  fprintf (file, "\nSizeOfStackCommit\t");
  fprintf_vma (file, i->SizeOfStackCommit);
  fprintf (file, "\nSizeOfHeapReserve\t");
  fprintf_vma (file, i->SizeOfHeapReserve);
  fprintf (file, "\nSizeOfHeapCommit\t");
  fprintf_vma (file, i->SizeOfHeapCommit);
  fprintf (file, "\nLoaderFlags\t\t%08lx\n", i->LoaderFlags);
  fprintf (file, "NumberOfRvaAndSizes\t%08lx\n", i->NumberOfRvaAndSizes);

  fprintf (file, "\nThe Data Directory\n");
  for (j = 0; j < IMAGE_NUMBEROF_DIRECTORY_ENTRIES; j++)
    {
      fprintf (file, "Entry %1x ", j);
      fprintf_vma (file, i->DataDirectory[j].VirtualAddress);
      fprintf (file, " %08lx ", i->DataDirectory[j].Size);
      fprintf (file, "%s\n", dir_names[j]);
    }

  pe_print_idata(abfd, vfile);
  pe_print_edata(abfd, vfile);
  pe_print_pdata(abfd, vfile);
  pe_print_reloc(abfd, vfile);

  return TRUE;
}

/* Copy any private info we understand from the input bfd
   to the output bfd.  */
bfd_boolean
_bfd_XX_bfd_copy_private_bfd_data_common(bfd *ibfd, bfd *obfd)
{
  /* One day we may try to grok other private data.  */
  if ((ibfd->xvec->flavour != bfd_target_coff_flavour)
      || (obfd->xvec->flavour != bfd_target_coff_flavour))
    return TRUE;

  pe_data(obfd)->pe_opthdr = pe_data(ibfd)->pe_opthdr;
  pe_data(obfd)->dll = pe_data(ibfd)->dll;

  /* For strip: if we removed .reloc, we'll make a real mess of things
     if we don't remove this entry as well.  */
  if (!pe_data(obfd)->has_reloc_section)
    {
      pe_data(obfd)->pe_opthdr.DataDirectory[5].VirtualAddress = 0;
      pe_data(obfd)->pe_opthdr.DataDirectory[5].Size = 0;
    }
  return TRUE;
}

/* Copy private section data.  */
bfd_boolean
_bfd_XX_bfd_copy_private_section_data(bfd *ibfd, asection *isec,
				      bfd *obfd, asection *osec)
{
  if ((bfd_get_flavour(ibfd) != bfd_target_coff_flavour)
      || (bfd_get_flavour(obfd) != bfd_target_coff_flavour))
    return TRUE;

  if ((coff_section_data(ibfd, isec) != NULL)
      && (pei_section_data(ibfd, isec) != NULL))
    {
      if (coff_section_data(obfd, osec) == NULL)
	{
	  bfd_size_type amt = sizeof(struct coff_section_tdata);
	  osec->used_by_bfd = bfd_zalloc(obfd, amt);
	  if (osec->used_by_bfd == NULL)
	    return FALSE;
	}

      if (pei_section_data(obfd, osec) == NULL)
	{
	  bfd_size_type amt = sizeof(struct pei_section_tdata);
	  coff_section_data(obfd, osec)->tdata = bfd_zalloc(obfd, amt);
	  if (coff_section_data(obfd, osec)->tdata == NULL)
	    return FALSE;
	}

      pei_section_data(obfd, osec)->virt_size =
	pei_section_data(ibfd, isec)->virt_size;
      pei_section_data(obfd, osec)->pe_flags =
	pei_section_data(ibfd, isec)->pe_flags;
    }

  return TRUE;
}

/* */
void
_bfd_XX_get_symbol_info(bfd *abfd, asymbol *symbol, symbol_info *ret)
{
  coff_get_symbol_info(abfd, symbol, ret);
}

/* Handle the .idata section and other things that need symbol table
   access.  */
bfd_boolean
_bfd_XXi_final_link_postscript(bfd *abfd, struct coff_final_link_info *pfinfo)
{
  struct coff_link_hash_entry *h1;
  struct bfd_link_info *info = pfinfo->info;

  /* There are a few fields that need to be filled in now while we
     have symbol table access.

     The .idata subsections aren't directly available as sections, but
     they are in the symbol table, so get them from there.  */

  /* The import directory.  This is the address of .idata$2, with size
     of .idata$2 + .idata$3.  */
  h1 = coff_link_hash_lookup(coff_hash_table(info),
			     ".idata$2", FALSE, FALSE, TRUE);
  if (h1 != NULL)
    {
      pe_data(abfd)->pe_opthdr.DataDirectory[1].VirtualAddress =
	(h1->root.u.def.value
	 + h1->root.u.def.section->output_section->vma
	 + h1->root.u.def.section->output_offset);
      h1 = coff_link_hash_lookup(coff_hash_table(info),
				 ".idata$4", FALSE, FALSE, TRUE);
      pe_data(abfd)->pe_opthdr.DataDirectory[1].Size =
	(long)((h1->root.u.def.value
                + h1->root.u.def.section->output_section->vma
	  	+ h1->root.u.def.section->output_offset)
               - pe_data(abfd)->pe_opthdr.DataDirectory[1].VirtualAddress);

      /* The import address table.  This is the size/address of
         .idata$5.  */
      h1 = coff_link_hash_lookup(coff_hash_table (info),
				 ".idata$5", FALSE, FALSE, TRUE);
      pe_data(abfd)->pe_opthdr.DataDirectory[12].VirtualAddress =
	(h1->root.u.def.value
	 + h1->root.u.def.section->output_section->vma
	 + h1->root.u.def.section->output_offset);
      h1 = coff_link_hash_lookup(coff_hash_table(info),
				 ".idata$6", FALSE, FALSE, TRUE);
      pe_data(abfd)->pe_opthdr.DataDirectory[12].Size =
	(long)((h1->root.u.def.value
	  	+ h1->root.u.def.section->output_section->vma
	  	+ h1->root.u.def.section->output_offset)
               - pe_data(abfd)->pe_opthdr.DataDirectory[12].VirtualAddress);
    }

  h1 = coff_link_hash_lookup(coff_hash_table(info),
			     "__tls_used", FALSE, FALSE, TRUE);
  if (h1 != NULL)
    {
      pe_data(abfd)->pe_opthdr.DataDirectory[9].VirtualAddress =
	(h1->root.u.def.value
	 + h1->root.u.def.section->output_section->vma
	 + h1->root.u.def.section->output_offset
	 - pe_data(abfd)->pe_opthdr.ImageBase);
      pe_data(abfd)->pe_opthdr.DataDirectory[9].Size = 0x18;
    }

  /* If we could NOT find idata$2, we either have an excessively trivial
   * program, or we are in DEEP trouble; we have to assume trivial
   * program...  */
  return TRUE;
}

#ifdef COFF_WITH_XX
# undef COFF_WITH_XX
#endif /* COFF_WITH_XX */

/* EOF */
