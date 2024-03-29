/* coffswap.h: Generic COFF swapping routines, for BFD.
   Copyright 1990, 1991, 1992, 1993, 1994, 1995, 1996, 1997, 1999, 2000,
   2001, 2002, 2005
   Free Software Foundation, Inc.
   Written by Cygnus Support.

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
   Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA 02110-1301, USA.  */

/* This file contains routines used to swap COFF data.  It is a header
   file because the details of swapping depend on the details of the
   structures used by each COFF implementation.  This is included by
   coffcode.h, as well as by the ECOFF backend.

   Any file which uses this must first include "coff/internal.h" and
   "coff/CPU.h".  The functions will then be correct for that CPU.  */

#ifndef GET_FCN_LNNOPTR
# define GET_FCN_LNNOPTR(abfd, ext) \
   H_GET_32 (abfd, ext->x_sym.x_fcnary.x_fcn.x_lnnoptr)
#endif

#ifndef GET_FCN_ENDNDX
# define GET_FCN_ENDNDX(abfd, ext) \
   H_GET_32 (abfd, ext->x_sym.x_fcnary.x_fcn.x_endndx)
#endif

#ifndef PUT_FCN_LNNOPTR
#define PUT_FCN_LNNOPTR(abfd, in, ext) \
  H_PUT_32 (abfd,  in, ext->x_sym.x_fcnary.x_fcn.x_lnnoptr)
#endif
#ifndef PUT_FCN_ENDNDX
#define PUT_FCN_ENDNDX(abfd, in, ext) \
  H_PUT_32 (abfd, in, ext->x_sym.x_fcnary.x_fcn.x_endndx)
#endif
#ifndef GET_LNSZ_LNNO
#define GET_LNSZ_LNNO(abfd, ext) \
  H_GET_16 (abfd, ext->x_sym.x_misc.x_lnsz.x_lnno)
#endif
#ifndef GET_LNSZ_SIZE
# define GET_LNSZ_SIZE(abfd, ext) \
   H_GET_16(abfd, ext->x_sym.x_misc.x_lnsz.x_size)
#endif /* !GET_LNSZ_SIZE */
#ifndef PUT_LNSZ_LNNO
#define PUT_LNSZ_LNNO(abfd, in, ext) \
  H_PUT_16 (abfd, in, ext->x_sym.x_misc.x_lnsz.x_lnno)
#endif
#ifndef PUT_LNSZ_SIZE
#define PUT_LNSZ_SIZE(abfd, in, ext) \
  H_PUT_16 (abfd, in, ext->x_sym.x_misc.x_lnsz.x_size)
#endif
#ifndef GET_SCN_SCNLEN
#define GET_SCN_SCNLEN(abfd, ext) \
  H_GET_32 (abfd, ext->x_scn.x_scnlen)
#endif
#ifndef GET_SCN_NRELOC
#define GET_SCN_NRELOC(abfd, ext) \
  H_GET_16 (abfd, ext->x_scn.x_nreloc)
#endif
#ifndef GET_SCN_NLINNO
#define GET_SCN_NLINNO(abfd, ext) \
  H_GET_16 (abfd, ext->x_scn.x_nlinno)
#endif
#ifndef PUT_SCN_SCNLEN
#define PUT_SCN_SCNLEN(abfd, in, ext) \
  H_PUT_32 (abfd, in, ext->x_scn.x_scnlen)
#endif
#ifndef PUT_SCN_NRELOC
#define PUT_SCN_NRELOC(abfd, in, ext) \
  H_PUT_16 (abfd, in, ext->x_scn.x_nreloc)
#endif
#ifndef PUT_SCN_NLINNO
#define PUT_SCN_NLINNO(abfd, in, ext) \
  H_PUT_16 (abfd, in, ext->x_scn.x_nlinno)
#endif
#ifndef GET_LINENO_LNNO
#define GET_LINENO_LNNO(abfd, ext) \
  H_GET_16 (abfd, ext->l_lnno);
#endif
#ifndef PUT_LINENO_LNNO
#define PUT_LINENO_LNNO(abfd, val, ext) \
  H_PUT_16 (abfd, val, ext->l_lnno);
#endif

/* The f_symptr field in the filehdr is sometimes 64 bits: */
#ifndef GET_FILEHDR_SYMPTR
# define GET_FILEHDR_SYMPTR H_GET_32
#endif /* !GET_FILEHDR_SYMPTR */
#ifndef PUT_FILEHDR_SYMPTR
# define PUT_FILEHDR_SYMPTR H_PUT_32
#endif /* !PUT_FILEHDR_SYMPTR */

/* Some fields in the aouthdr are sometimes 64 bits: */
#ifndef GET_AOUTHDR_TSIZE
#define GET_AOUTHDR_TSIZE H_GET_32
#endif
#ifndef PUT_AOUTHDR_TSIZE
#define PUT_AOUTHDR_TSIZE H_PUT_32
#endif
#ifndef GET_AOUTHDR_DSIZE
#define GET_AOUTHDR_DSIZE H_GET_32
#endif
#ifndef PUT_AOUTHDR_DSIZE
#define PUT_AOUTHDR_DSIZE H_PUT_32
#endif
#ifndef GET_AOUTHDR_BSIZE
#define GET_AOUTHDR_BSIZE H_GET_32
#endif
#ifndef PUT_AOUTHDR_BSIZE
#define PUT_AOUTHDR_BSIZE H_PUT_32
#endif
#ifndef GET_AOUTHDR_ENTRY
#define GET_AOUTHDR_ENTRY H_GET_32
#endif
#ifndef PUT_AOUTHDR_ENTRY
#define PUT_AOUTHDR_ENTRY H_PUT_32
#endif
#ifndef GET_AOUTHDR_TEXT_START
#define GET_AOUTHDR_TEXT_START H_GET_32
#endif
#ifndef PUT_AOUTHDR_TEXT_START
#define PUT_AOUTHDR_TEXT_START H_PUT_32
#endif
#ifndef GET_AOUTHDR_DATA_START
#define GET_AOUTHDR_DATA_START H_GET_32
#endif
#ifndef PUT_AOUTHDR_DATA_START
#define PUT_AOUTHDR_DATA_START H_PUT_32
#endif

/* Some fields in the scnhdr are sometimes 64 bits: */
#ifndef GET_SCNHDR_PADDR
#define GET_SCNHDR_PADDR H_GET_32
#endif
#ifndef PUT_SCNHDR_PADDR
#define PUT_SCNHDR_PADDR H_PUT_32
#endif
#ifndef GET_SCNHDR_VADDR
#define GET_SCNHDR_VADDR H_GET_32
#endif
#ifndef PUT_SCNHDR_VADDR
#define PUT_SCNHDR_VADDR H_PUT_32
#endif
#ifndef GET_SCNHDR_SIZE
#define GET_SCNHDR_SIZE H_GET_32
#endif
#ifndef PUT_SCNHDR_SIZE
#define PUT_SCNHDR_SIZE H_PUT_32
#endif
#ifndef GET_SCNHDR_SCNPTR
#define GET_SCNHDR_SCNPTR H_GET_32
#endif
#ifndef PUT_SCNHDR_SCNPTR
#define PUT_SCNHDR_SCNPTR H_PUT_32
#endif
#ifndef GET_SCNHDR_RELPTR
#define GET_SCNHDR_RELPTR H_GET_32
#endif
#ifndef PUT_SCNHDR_RELPTR
#define PUT_SCNHDR_RELPTR H_PUT_32
#endif
#ifndef GET_SCNHDR_LNNOPTR
#define GET_SCNHDR_LNNOPTR H_GET_32
#endif
#ifndef PUT_SCNHDR_LNNOPTR
#define PUT_SCNHDR_LNNOPTR H_PUT_32
#endif
#ifndef GET_SCNHDR_NRELOC
#define GET_SCNHDR_NRELOC H_GET_16
#endif
#ifndef MAX_SCNHDR_NRELOC
#define MAX_SCNHDR_NRELOC 0xffff
#endif
#ifndef PUT_SCNHDR_NRELOC
#define PUT_SCNHDR_NRELOC H_PUT_16
#endif
#ifndef GET_SCNHDR_NLNNO
#define GET_SCNHDR_NLNNO H_GET_16
#endif
#ifndef MAX_SCNHDR_NLNNO
#define MAX_SCNHDR_NLNNO 0xffff
#endif
#ifndef PUT_SCNHDR_NLNNO
#define PUT_SCNHDR_NLNNO H_PUT_16
#endif
#ifndef GET_SCNHDR_FLAGS
#define GET_SCNHDR_FLAGS H_GET_32
#endif
#ifndef PUT_SCNHDR_FLAGS
#define PUT_SCNHDR_FLAGS H_PUT_32
#endif

#ifndef GET_RELOC_VADDR
# define GET_RELOC_VADDR H_GET_32
#endif /* !GET_RELOC_VADDR */
#ifndef PUT_RELOC_VADDR
# define PUT_RELOC_VADDR H_PUT_32
#endif /* !PUT_RELOC_VADDR */

#ifndef NO_COFF_RELOCS
static void
coff_swap_reloc_in(bfd *abfd, void *src, void *dst)
{
  RELOC *reloc_src = (RELOC *)src;
  struct internal_reloc *reloc_dst = (struct internal_reloc *)dst;

  reloc_dst->r_vaddr = GET_RELOC_VADDR(abfd, reloc_src->r_vaddr);
  reloc_dst->r_symndx = (long)H_GET_S32(abfd, reloc_src->r_symndx);
  reloc_dst->r_type = (unsigned short)H_GET_16(abfd, reloc_src->r_type);

# ifdef SWAP_IN_RELOC_OFFSET
  reloc_dst->r_offset =
    (unsigned long)SWAP_IN_RELOC_OFFSET(abfd, reloc_src->r_offset);
# endif /* SWAP_IN_RELOC_OFFSET */
}

static unsigned int
coff_swap_reloc_out(bfd * abfd, void *src, void *dst)
{
  struct internal_reloc *reloc_src = (struct internal_reloc *)src;
  struct external_reloc *reloc_dst = (struct external_reloc *)dst;

  PUT_RELOC_VADDR(abfd, reloc_src->r_vaddr, reloc_dst->r_vaddr);
  H_PUT_32(abfd, (bfd_vma)reloc_src->r_symndx, reloc_dst->r_symndx);
  H_PUT_16(abfd, (bfd_vma)reloc_src->r_type, reloc_dst->r_type);

# ifdef SWAP_OUT_RELOC_OFFSET
  SWAP_OUT_RELOC_OFFSET(abfd, (bfd_vma)reloc_src->r_offset, reloc_dst->r_offset);
# endif /* SWAP_OUT_RELOC_OFFSET */
# ifdef SWAP_OUT_RELOC_EXTRA
  SWAP_OUT_RELOC_EXTRA(abfd, reloc_src, reloc_dst);
# endif /* SWAP_OUT_RELOC_EXTRA */

  return bfd_coff_relsz(abfd);
}
#endif /* NO_COFF_RELOCS */

static void
coff_swap_filehdr_in(bfd *abfd, void *src, void *dst)
{
  FILHDR *filehdr_src = (FILHDR *)src;
  struct internal_filehdr *filehdr_dst = (struct internal_filehdr *)dst;

#ifdef COFF_ADJUST_FILEHDR_IN_PRE
  COFF_ADJUST_FILEHDR_IN_PRE(abfd, src, dst);
#endif /* COFF_ADJUST_FILEHDR_IN_PRE */
  filehdr_dst->f_magic = (unsigned short)H_GET_16(abfd,
                                                  filehdr_src->f_magic);
  filehdr_dst->f_nscns = (unsigned short)H_GET_16(abfd,
                                                  filehdr_src->f_nscns);
  filehdr_dst->f_timdat = (long)H_GET_32(abfd, filehdr_src->f_timdat);
  filehdr_dst->f_symptr = GET_FILEHDR_SYMPTR(abfd, filehdr_src->f_symptr);
  filehdr_dst->f_nsyms = (long)H_GET_32(abfd, filehdr_src->f_nsyms);
  filehdr_dst->f_opthdr = (unsigned short)H_GET_16(abfd, filehdr_src->f_opthdr);
  filehdr_dst->f_flags = (unsigned short)H_GET_16(abfd, filehdr_src->f_flags);
#ifdef TIC80_TARGET_ID
  filehdr_dst->f_target_id =
    (unsigned short)H_GET_16(abfd, filehdr_src->f_target_id);
#endif /* TIC80_TARGET_ID */

#ifdef COFF_ADJUST_FILEHDR_IN_POST
  COFF_ADJUST_FILEHDR_IN_POST(abfd, src, dst);
#endif /* COFF_ADJUST_FILEHDR_IN_POST */
}

static  unsigned int
coff_swap_filehdr_out(bfd *abfd, void *in, void * out)
{
  struct internal_filehdr *filehdr_in = (struct internal_filehdr *)in;
  FILHDR *filehdr_out = (FILHDR *)out;

#ifdef COFF_ADJUST_FILEHDR_OUT_PRE
  COFF_ADJUST_FILEHDR_OUT_PRE(abfd, in, out);
#endif /* COFF_ADJUST_FILEHDR_IN_PRE */
  H_PUT_16(abfd, (bfd_vma)filehdr_in->f_magic, filehdr_out->f_magic);
  H_PUT_16(abfd, (bfd_vma)filehdr_in->f_nscns, filehdr_out->f_nscns);
  H_PUT_32(abfd, (bfd_vma)filehdr_in->f_timdat, filehdr_out->f_timdat);
  PUT_FILEHDR_SYMPTR(abfd, filehdr_in->f_symptr, filehdr_out->f_symptr);
  H_PUT_32(abfd, (bfd_vma)filehdr_in->f_nsyms, filehdr_out->f_nsyms);
  H_PUT_16(abfd, (bfd_vma)filehdr_in->f_opthdr, filehdr_out->f_opthdr);
  H_PUT_16(abfd, (bfd_vma)filehdr_in->f_flags, filehdr_out->f_flags);
#ifdef TIC80_TARGET_ID
  H_PUT_16(abfd, (bfd_vma)filehdr_in->f_target_id,
           filehdr_out->f_target_id);
#endif /* TIC80_TARGET_ID */

#ifdef COFF_ADJUST_FILEHDR_OUT_POST
  COFF_ADJUST_FILEHDR_OUT_POST(abfd, in, out);
#endif /* COFF_ADJUST_FILEHDR_IN_POST */
  return bfd_coff_filhsz(abfd);
}

#ifndef NO_COFF_SYMBOLS

static void
coff_swap_sym_in(bfd *abfd, void *ext1, void *in1)
{
  SYMENT *ext = (SYMENT *)ext1;
  struct internal_syment *in0 = (struct internal_syment *)in1;

  if (ext->e.e_name[0] == 0) {
      in0->_n._n_n._n_zeroes = 0;
      in0->_n._n_n._n_offset = (long)H_GET_32(abfd, ext->e.e.e_offset);
  } else {
#if SYMNMLEN != E_SYMNMLEN
# error "we need to cope with truncating or extending SYMNMLEN"
#else
      memcpy(in0->_n._n_name, ext->e.e_name, (size_t)SYMNMLEN);
#endif /* SYMNMLEN != E_SYMNMLEN */
  }

  in0->n_value = H_GET_32(abfd, ext->e_value);
  in0->n_scnum = (short)H_GET_16(abfd, ext->e_scnum);
  if (sizeof(ext->e_type) == 2) {
      /*NOTREACHED*/
      in0->n_type = (unsigned short)H_GET_16(abfd, ext->e_type);
  } else {
      /*NOTREACHED*/
      in0->n_type = (unsigned short)H_GET_32(abfd, ext->e_type);
  }
  in0->n_sclass = H_GET_8(abfd, ext->e_sclass);
  in0->n_numaux = H_GET_8(abfd, ext->e_numaux);
#ifdef COFF_ADJUST_SYM_IN_POST
  COFF_ADJUST_SYM_IN_POST(abfd, ext1, in1);
#endif /* COFF_ADJUST_SYM_IN_POST */
}

static unsigned int
coff_swap_sym_out(bfd *abfd, void *inp, void *extp)
{
  struct internal_syment *in = (struct internal_syment *)inp;
  SYMENT *ext = (SYMENT *)extp;

#ifdef COFF_ADJUST_SYM_OUT_PRE
  COFF_ADJUST_SYM_OUT_PRE(abfd, inp, extp);
#endif /* COFF_ADJUST_SYM_OUT_PRE */

  if (in->_n._n_name[0] == 0) {
      H_PUT_32(abfd, (bfd_vma)0UL, ext->e.e.e_zeroes);
      H_PUT_32(abfd, (bfd_vma)in->_n._n_n._n_offset, ext->e.e.e_offset);
  } else {
#if SYMNMLEN != E_SYMNMLEN
# error "we need to cope with truncating or extending SYMNMLEN"
#else
      memcpy(ext->e.e_name, in->_n._n_name, (size_t)SYMNMLEN);
#endif /* SYMNMLEN != E_SYMNMLEN */
    }

  H_PUT_32(abfd, in->n_value, ext->e_value);
  H_PUT_16(abfd, (bfd_vma)in->n_scnum, ext->e_scnum);

  if (sizeof(ext->e_type) == 2) {
      /*NOTREACHED*/
      H_PUT_16(abfd, (bfd_vma)in->n_type, ext->e_type);
  } else {
      /*NOTREACHED*/
      H_PUT_32(abfd, (bfd_vma)in->n_type, ext->e_type);
  }

  H_PUT_8(abfd, in->n_sclass, ext->e_sclass);
  H_PUT_8(abfd, in->n_numaux, ext->e_numaux);

#ifdef COFF_ADJUST_SYM_OUT_POST
  /* FIXME: -Wconversion (2x) somewhere in here: */
  COFF_ADJUST_SYM_OUT_POST(abfd, inp, extp);
#endif /* COFF_ADJUST_SYM_OUT_POST */

  return SYMESZ;
}

static void
coff_swap_aux_in(bfd *abfd, void *ext1, int type, int classnum, int indx,
		 int numaux, void *in1)
{
  AUXENT *ext = (AUXENT *)ext1;
  union internal_auxent *innit = (union internal_auxent *)in1;

#ifdef COFF_ADJUST_AUX_IN_PRE
  COFF_ADJUST_AUX_IN_PRE(abfd, ext1, type, classnum, indx, numaux, in1);
#endif /* COFF_ADJUST_AUX_IN_PRE */

  switch (classnum) {
    case C_FILE:
      if (ext->x_file.x_fname[0] == 0) {
	  innit->x_file.x_n.x_zeroes = 0;
	  innit->x_file.x_n.x_offset = (long)H_GET_32(abfd, ext->x_file.x_n.x_offset);
      } else {
#if FILNMLEN != E_FILNMLEN
# error "we need to cope with truncating or extending FILNMLEN"
#else
	  if (numaux > 1) {
	      if (indx == 0) {
		  memcpy(innit->x_file.x_fname, ext->x_file.x_fname,
			 ((size_t)numaux * sizeof(AUXENT)));
	      }
	  } else {
	    memcpy(innit->x_file.x_fname, ext->x_file.x_fname,
                   (size_t)E_FILNMLEN);
	  }
#endif /* FILNMLEN != E_FILNMLEN */
      }
      goto end;

    case C_STAT:
#ifdef C_LEAFSTAT
    case C_LEAFSTAT:
#endif /* C_LEAFSTAT */
    case C_HIDDEN:
      if (type == T_NULL) {
	  innit->x_scn.x_scnlen = (long)GET_SCN_SCNLEN(abfd, ext);
	  innit->x_scn.x_nreloc = (unsigned short)GET_SCN_NRELOC(abfd, ext);
	  innit->x_scn.x_nlinno = (unsigned short)GET_SCN_NLINNO(abfd, ext);

	  /* PE defines some extra fields; we zero them out for safety: */
	  innit->x_scn.x_checksum = 0;
	  innit->x_scn.x_associated = 0;
	  innit->x_scn.x_comdat = 0;

	  goto end;
      }
      break;
    default:
      break; /* not sure if correct? */
  } /* end "switch (classnum)" */

  innit->x_sym.x_tagndx.l = (long)H_GET_32(abfd, ext->x_sym.x_tagndx);
#ifndef NO_TVNDX
  innit->x_sym.x_tvndx = (unsigned short)H_GET_16(abfd, ext->x_sym.x_tvndx);
#endif /* !NO_TVNDX */

  if ((classnum == C_BLOCK) || (classnum == C_FCN) || ISFCN((int)type) || ISTAG(classnum)) {
      innit->x_sym.x_fcnary.x_fcn.x_lnnoptr =
	(bfd_signed_vma)GET_FCN_LNNOPTR(abfd, ext);
      innit->x_sym.x_fcnary.x_fcn.x_endndx.l = (long)GET_FCN_ENDNDX(abfd,
                                                                    ext);
  } else {
#if DIMNUM != E_DIMNUM
# error "we need to cope with truncating or extending DIMNUM"
#endif /* DIMNUM != E_DIMNUM */
      innit->x_sym.x_fcnary.x_ary.x_dimen[0] =
	(unsigned short)H_GET_16(abfd, ext->x_sym.x_fcnary.x_ary.x_dimen[0]);
      innit->x_sym.x_fcnary.x_ary.x_dimen[1] =
	(unsigned short)H_GET_16(abfd, ext->x_sym.x_fcnary.x_ary.x_dimen[1]);
      innit->x_sym.x_fcnary.x_ary.x_dimen[2] =
	(unsigned short)H_GET_16(abfd, ext->x_sym.x_fcnary.x_ary.x_dimen[2]);
      innit->x_sym.x_fcnary.x_ary.x_dimen[3] =
	(unsigned short)H_GET_16(abfd, ext->x_sym.x_fcnary.x_ary.x_dimen[3]);
    }

  if (ISFCN((int)type)) {
      innit->x_sym.x_misc.x_fsize = (long)H_GET_32(abfd,
                                                   ext->x_sym.x_misc.x_fsize);
  } else {
      innit->x_sym.x_misc.x_lnsz.x_lnno = (unsigned short)GET_LNSZ_LNNO(abfd,
									ext);
      innit->x_sym.x_misc.x_lnsz.x_size = (unsigned short)GET_LNSZ_SIZE(abfd,
									ext);
  }

 end: ;

#ifdef COFF_ADJUST_AUX_IN_POST
  COFF_ADJUST_AUX_IN_POST(abfd, ext1, type, classnum, indx, numaux, in1);
#endif /* COFF_ADJUST_AUX_IN_POST */
}

static unsigned int
coff_swap_aux_out(bfd *abfd, void *inp, int type, int classnum, int indx,
		  int numaux, void *extp)
{
  union internal_auxent *innit = (union internal_auxent *)inp;
  AUXENT *ext = (AUXENT *)extp;

#ifdef COFF_ADJUST_AUX_OUT_PRE
  COFF_ADJUST_AUX_OUT_PRE(abfd, inp, type, classnum, indx, numaux, extp);
#endif /* COFF_ADJUST_AUX_OUT_PRE */

  memset(ext, 0, (size_t)AUXESZ);

  switch (classnum) {
    case C_FILE:
      if (innit->x_file.x_fname[0] == 0) {
	  H_PUT_32(abfd, (bfd_vma)0UL, ext->x_file.x_n.x_zeroes);
	  H_PUT_32(abfd, (bfd_vma)innit->x_file.x_n.x_offset, ext->x_file.x_n.x_offset);
      } else {
#if FILNMLEN != E_FILNMLEN
# error "we need to cope with truncating or extending FILNMLEN"
#else
	  memcpy(ext->x_file.x_fname, innit->x_file.x_fname,
                 (size_t)E_FILNMLEN);
#endif /* FILNMLEN != E_FILNMLEN */
	}
      goto end;

    case C_STAT:
#ifdef C_LEAFSTAT
    case C_LEAFSTAT:
#endif /* C_LEAFSTAT */
    case C_HIDDEN:
      if (type == T_NULL) {
	  PUT_SCN_SCNLEN(abfd, (bfd_vma)innit->x_scn.x_scnlen, ext);
	  PUT_SCN_NRELOC(abfd, (bfd_vma)innit->x_scn.x_nreloc, ext);
	  PUT_SCN_NLINNO(abfd, (bfd_vma)innit->x_scn.x_nlinno, ext);
	  goto end;
      }
      break;
    default:
      break; /* not sure if correct? */
  } /* end "switch (classnum)" */

  H_PUT_32(abfd, (bfd_vma)innit->x_sym.x_tagndx.l, ext->x_sym.x_tagndx);
#ifndef NO_TVNDX
  H_PUT_16(abfd, (bfd_vma)innit->x_sym.x_tvndx, ext->x_sym.x_tvndx);
#endif /* !NO_TVNDX */

  if ((classnum == C_BLOCK) || (classnum == C_FCN) || ISFCN((int)type) || ISTAG(classnum)) {
      PUT_FCN_LNNOPTR(abfd, (bfd_vma)innit->x_sym.x_fcnary.x_fcn.x_lnnoptr, ext);
      PUT_FCN_ENDNDX(abfd, (bfd_vma)innit->x_sym.x_fcnary.x_fcn.x_endndx.l, ext);
  } else {
#if DIMNUM != E_DIMNUM
# error "we need to cope with truncating or extending DIMNUM"
#endif /* DIMNUM != E_DIMNUM */
      H_PUT_16(abfd, (bfd_vma)innit->x_sym.x_fcnary.x_ary.x_dimen[0],
	       ext->x_sym.x_fcnary.x_ary.x_dimen[0]);
      H_PUT_16(abfd, (bfd_vma)innit->x_sym.x_fcnary.x_ary.x_dimen[1],
	       ext->x_sym.x_fcnary.x_ary.x_dimen[1]);
      H_PUT_16(abfd, (bfd_vma)innit->x_sym.x_fcnary.x_ary.x_dimen[2],
	       ext->x_sym.x_fcnary.x_ary.x_dimen[2]);
      H_PUT_16(abfd, (bfd_vma)innit->x_sym.x_fcnary.x_ary.x_dimen[3],
	       ext->x_sym.x_fcnary.x_ary.x_dimen[3]);
  }

  if (ISFCN((int)type)) {
      H_PUT_32(abfd, (bfd_vma)innit->x_sym.x_misc.x_fsize,
               ext->x_sym.x_misc.x_fsize);
  } else {
      PUT_LNSZ_LNNO(abfd, (bfd_vma)innit->x_sym.x_misc.x_lnsz.x_lnno, ext);
      PUT_LNSZ_SIZE(abfd, (bfd_vma)innit->x_sym.x_misc.x_lnsz.x_size, ext);
  }

 end:
#ifdef COFF_ADJUST_AUX_OUT_POST
  COFF_ADJUST_AUX_OUT_POST(abfd, inp, type, classnum, indx, numaux, extp);
#endif /* COFF_ADJUST_AUX_OUT_POST */
  (void)indx;
  (void)numaux;
  return AUXESZ;
}
#endif /* NO_COFF_SYMBOLS */

#ifndef NO_COFF_LINENOS
static void
coff_swap_lineno_in(bfd * abfd, void * ext1, void * in1)
{
  LINENO *ext = (LINENO *)ext1;
  struct internal_lineno *innit = (struct internal_lineno *)in1;

  innit->l_addr.l_symndx = (bfd_signed_vma)H_GET_32(abfd, ext->l_addr.l_symndx);
  innit->l_lnno = (unsigned long)GET_LINENO_LNNO(abfd, ext);
}

static unsigned int
coff_swap_lineno_out(bfd *abfd, void *inp, void *outp)
{
  struct internal_lineno *in = (struct internal_lineno *)inp;
  struct external_lineno *ext = (struct external_lineno *)outp;
  H_PUT_32(abfd, (bfd_vma)in->l_addr.l_symndx, ext->l_addr.l_symndx);

  PUT_LINENO_LNNO(abfd, (bfd_vma)in->l_lnno, ext);
  return LINESZ;
}

#endif /* NO_COFF_LINENOS */

static void
coff_swap_aouthdr_in(bfd * abfd, void * aouthdr_ext1, void * aouthdr_int1)
{
  AOUTHDR *aouthdr_ext;
  struct internal_aouthdr *aouthdr_int;

  aouthdr_ext = (AOUTHDR *)aouthdr_ext1;
  aouthdr_int = (struct internal_aouthdr *)aouthdr_int1;
  aouthdr_int->magic = (short)H_GET_16(abfd, aouthdr_ext->magic);
  aouthdr_int->vstamp = (short)H_GET_16(abfd, aouthdr_ext->vstamp);
  aouthdr_int->tsize = GET_AOUTHDR_TSIZE(abfd, aouthdr_ext->tsize);
  aouthdr_int->dsize = GET_AOUTHDR_DSIZE(abfd, aouthdr_ext->dsize);
  aouthdr_int->bsize = GET_AOUTHDR_BSIZE(abfd, aouthdr_ext->bsize);
  aouthdr_int->entry = GET_AOUTHDR_ENTRY(abfd, aouthdr_ext->entry);
  aouthdr_int->text_start =
    GET_AOUTHDR_TEXT_START(abfd, aouthdr_ext->text_start);
/* when being included from coff-ia64.c, AOUTHDR is defined in coff/ia64.h, and
 * there it only has member 'data_start' when BFD64 is undefined: */
#if !defined(IA64) || (defined(IA64) && !defined(BFD64))
  aouthdr_int->data_start =
    GET_AOUTHDR_DATA_START(abfd, aouthdr_ext->data_start);
#endif /* !IA64 || (IA64 && !BFD64) */

#ifdef I960
  aouthdr_int->tagentries = (unsigned long)H_GET_32(abfd,
						    aouthdr_ext->tagentries);
#endif /* I960 */

#ifdef APOLLO_M68
  H_PUT_32(abfd, (bfd_vma)aouthdr_int->o_inlib, aouthdr_ext->o_inlib);
  H_PUT_32(abfd, (bfd_vma)aouthdr_int->o_sri, aouthdr_ext->o_sri);
  H_PUT_32(abfd, (bfd_vma)aouthdr_int->vid[0], aouthdr_ext->vid);
  H_PUT_32(abfd, (bfd_vma)aouthdr_int->vid[1], (aouthdr_ext->vid + 4));
#endif /* APOLLO_M68 */

#ifdef RS6000COFF_C
# ifdef XCOFF64
  aouthdr_int->o_toc = H_GET_64(abfd, aouthdr_ext->o_toc);
# else
  aouthdr_int->o_toc = H_GET_32(abfd, aouthdr_ext->o_toc);
# endif /* XCOFF64 */
  aouthdr_int->o_snentry = (short)H_GET_16(abfd, aouthdr_ext->o_snentry);
  aouthdr_int->o_sntext = (short)H_GET_16(abfd, aouthdr_ext->o_sntext);
  aouthdr_int->o_sndata = (short)H_GET_16(abfd, aouthdr_ext->o_sndata);
  aouthdr_int->o_sntoc = (short)H_GET_16(abfd, aouthdr_ext->o_sntoc);
  aouthdr_int->o_snloader = (short)H_GET_16(abfd, aouthdr_ext->o_snloader);
  aouthdr_int->o_snbss = (short)H_GET_16(abfd, aouthdr_ext->o_snbss);
  aouthdr_int->o_algntext = (short)H_GET_16(abfd, aouthdr_ext->o_algntext);
  aouthdr_int->o_algndata = (short)H_GET_16(abfd, aouthdr_ext->o_algndata);
  aouthdr_int->o_modtype = (short)H_GET_16(abfd, aouthdr_ext->o_modtype);
  aouthdr_int->o_cputype = (short)H_GET_16(abfd, aouthdr_ext->o_cputype);
# ifdef XCOFF64
  aouthdr_int->o_maxstack = H_GET_64(abfd, aouthdr_ext->o_maxstack);
  aouthdr_int->o_maxdata = H_GET_64(abfd, aouthdr_ext->o_maxdata);
# else
  aouthdr_int->o_maxstack = H_GET_32(abfd, aouthdr_ext->o_maxstack);
  aouthdr_int->o_maxdata = H_GET_32(abfd, aouthdr_ext->o_maxdata);
# endif /* XCOFF64 */
#endif /* RS6000COFF_C */

#ifdef MIPSECOFF
  aouthdr_int->bss_start = H_GET_32(abfd, aouthdr_ext->bss_start);
  aouthdr_int->gp_value = H_GET_32(abfd, aouthdr_ext->gp_value);
  aouthdr_int->gprmask = (unsigned long)H_GET_32(abfd, aouthdr_ext->gprmask);
  aouthdr_int->cprmask[0] = (unsigned long)H_GET_32(abfd,
						    aouthdr_ext->cprmask[0]);
  aouthdr_int->cprmask[1] = (unsigned long)H_GET_32(abfd,
						    aouthdr_ext->cprmask[1]);
  aouthdr_int->cprmask[2] = (unsigned long)H_GET_32(abfd,
						    aouthdr_ext->cprmask[2]);
  aouthdr_int->cprmask[3] = (unsigned long)H_GET_32(abfd,
						    aouthdr_ext->cprmask[3]);
#endif /* MIPSECOFF */

#ifdef ALPHAECOFF
  aouthdr_int->bss_start = H_GET_64(abfd, aouthdr_ext->bss_start);
  aouthdr_int->gp_value = H_GET_64(abfd, aouthdr_ext->gp_value);
  aouthdr_int->gprmask = (unsigned long)H_GET_32(abfd, aouthdr_ext->gprmask);
  aouthdr_int->fprmask = (unsigned long)H_GET_32(abfd, aouthdr_ext->fprmask);
#endif /* ALPHAECOFF */
}

static unsigned int
coff_swap_aouthdr_out(bfd *abfd, void *input_data, void *output_data)
{
  struct internal_aouthdr *aouthdr_in = (struct internal_aouthdr *)input_data;
  AOUTHDR *aouthdr_out = (AOUTHDR *)output_data;

  H_PUT_16(abfd, (bfd_vma)aouthdr_in->magic, aouthdr_out->magic);
  H_PUT_16(abfd, (bfd_vma)aouthdr_in->vstamp, aouthdr_out->vstamp);
  PUT_AOUTHDR_TSIZE(abfd, aouthdr_in->tsize, aouthdr_out->tsize);
  PUT_AOUTHDR_DSIZE(abfd, aouthdr_in->dsize, aouthdr_out->dsize);
  PUT_AOUTHDR_BSIZE(abfd, aouthdr_in->bsize, aouthdr_out->bsize);
  PUT_AOUTHDR_ENTRY(abfd, aouthdr_in->entry, aouthdr_out->entry);
  PUT_AOUTHDR_TEXT_START(abfd, aouthdr_in->text_start,
			 aouthdr_out->text_start);
  /* as previously, when being included from coff-ia64.c, AOUTHDR is defined
   * in coff/ia64.h, and there it only has member 'data_start'
   * when BFD64 is undefined: */
#if !defined(IA64) || (defined(IA64) && !defined(BFD64))
  PUT_AOUTHDR_DATA_START (abfd, aouthdr_in->data_start,
			  aouthdr_out->data_start);
#endif /* !IA64 || (IA64 && !BFD64) */

#ifdef I960
  H_PUT_32(abfd, (bfd_vma)aouthdr_in->tagentries, aouthdr_out->tagentries);
#endif /* I960 */

#ifdef RS6000COFF_C
#ifdef XCOFF64
  H_PUT_64(abfd, aouthdr_in->o_toc, aouthdr_out->o_toc);
#else
  H_PUT_32(abfd, aouthdr_in->o_toc, aouthdr_out->o_toc);
#endif
  H_PUT_16(abfd, (bfd_vma)aouthdr_in->o_snentry, aouthdr_out->o_snentry);
  H_PUT_16(abfd, (bfd_vma)aouthdr_in->o_sntext, aouthdr_out->o_sntext);
  H_PUT_16(abfd, (bfd_vma)aouthdr_in->o_sndata, aouthdr_out->o_sndata);
  H_PUT_16(abfd, (bfd_vma)aouthdr_in->o_sntoc, aouthdr_out->o_sntoc);
  H_PUT_16(abfd, (bfd_vma)aouthdr_in->o_snloader, aouthdr_out->o_snloader);
  H_PUT_16(abfd, (bfd_vma)aouthdr_in->o_snbss, aouthdr_out->o_snbss);
  H_PUT_16(abfd, (bfd_vma)aouthdr_in->o_algntext, aouthdr_out->o_algntext);
  H_PUT_16(abfd, (bfd_vma)aouthdr_in->o_algndata, aouthdr_out->o_algndata);
  H_PUT_16(abfd, (bfd_vma)aouthdr_in->o_modtype, aouthdr_out->o_modtype);
  H_PUT_16(abfd, (bfd_vma)aouthdr_in->o_cputype, aouthdr_out->o_cputype);
#ifdef XCOFF64
  H_PUT_64(abfd, aouthdr_in->o_maxstack, aouthdr_out->o_maxstack);
  H_PUT_64(abfd, aouthdr_in->o_maxdata, aouthdr_out->o_maxdata);
#else
  H_PUT_32(abfd, aouthdr_in->o_maxstack, aouthdr_out->o_maxstack);
  H_PUT_32(abfd, aouthdr_in->o_maxdata, aouthdr_out->o_maxdata);
#endif
  memset(aouthdr_out->o_resv2, 0, sizeof(aouthdr_out->o_resv2));
#ifdef XCOFF64
  memset(aouthdr_out->o_debugger, 0, sizeof(aouthdr_out->o_debugger));
  memset(aouthdr_out->o_resv3, 0, sizeof(aouthdr_out->o_resv3));
#endif
#endif

#ifdef MIPSECOFF
  H_PUT_32(abfd, aouthdr_in->bss_start, aouthdr_out->bss_start);
  H_PUT_32(abfd, aouthdr_in->gp_value, aouthdr_out->gp_value);
  H_PUT_32(abfd, (bfd_vma)aouthdr_in->gprmask, aouthdr_out->gprmask);
  H_PUT_32(abfd, (bfd_vma)aouthdr_in->cprmask[0], aouthdr_out->cprmask[0]);
  H_PUT_32(abfd, (bfd_vma)aouthdr_in->cprmask[1], aouthdr_out->cprmask[1]);
  H_PUT_32(abfd, (bfd_vma)aouthdr_in->cprmask[2], aouthdr_out->cprmask[2]);
  H_PUT_32(abfd, (bfd_vma)aouthdr_in->cprmask[3], aouthdr_out->cprmask[3]);
#endif /* MIPSECOFF */

#ifdef ALPHAECOFF
  /* FIXME: What does bldrev mean? Build revision, perhaps? */
  H_PUT_16(abfd, (bfd_vma)2UL, aouthdr_out->bldrev);
  H_PUT_16(abfd, (bfd_vma)0UL, aouthdr_out->padding);
  H_PUT_64(abfd, aouthdr_in->bss_start, aouthdr_out->bss_start);
  H_PUT_64(abfd, aouthdr_in->gp_value, aouthdr_out->gp_value);
  H_PUT_32(abfd, (bfd_vma)aouthdr_in->gprmask, aouthdr_out->gprmask);
  H_PUT_32(abfd, (bfd_vma)aouthdr_in->fprmask, aouthdr_out->fprmask);
#endif /* ALPHAECOFF */

  return AOUTSZ;
}

static void
coff_swap_scnhdr_in(bfd *abfd, void *ext, void *in_ptr)
{
  SCNHDR *scnhdr_ext = (SCNHDR *)ext;
  struct internal_scnhdr *scnhdr_int = (struct internal_scnhdr *)in_ptr;

#ifdef COFF_ADJUST_SCNHDR_IN_PRE
  COFF_ADJUST_SCNHDR_IN_PRE(abfd, ext, in_ptr);
#endif /* COFF_ADJUST_SCNHDR_IN_PRE */
  memcpy(scnhdr_int->s_name, scnhdr_ext->s_name,
         sizeof(scnhdr_int->s_name));

  scnhdr_int->s_vaddr = GET_SCNHDR_VADDR(abfd, scnhdr_ext->s_vaddr);
  scnhdr_int->s_paddr = GET_SCNHDR_PADDR(abfd, scnhdr_ext->s_paddr);
  scnhdr_int->s_size = GET_SCNHDR_SIZE(abfd, scnhdr_ext->s_size);

  scnhdr_int->s_scnptr = GET_SCNHDR_SCNPTR(abfd, scnhdr_ext->s_scnptr);
  scnhdr_int->s_relptr = GET_SCNHDR_RELPTR(abfd, scnhdr_ext->s_relptr);
  scnhdr_int->s_lnnoptr = GET_SCNHDR_LNNOPTR(abfd, scnhdr_ext->s_lnnoptr);
  scnhdr_int->s_flags = (long)GET_SCNHDR_FLAGS(abfd, scnhdr_ext->s_flags);
  scnhdr_int->s_nreloc = (unsigned long)GET_SCNHDR_NRELOC(abfd,
                                                          scnhdr_ext->s_nreloc);
  scnhdr_int->s_nlnno = (unsigned long)GET_SCNHDR_NLNNO(abfd,
                                                        scnhdr_ext->s_nlnno);
#ifdef I960
  scnhdr_int->s_align = (long)GET_SCNHDR_ALIGN(abfd, scnhdr_ext->s_align);
#endif /* I960 */
#ifdef COFF_ADJUST_SCNHDR_IN_POST
  /* FIXME: -Wconversion (4x) somewhere in here: */
  COFF_ADJUST_SCNHDR_IN_POST(abfd, ext, in_ptr);
#endif /* COFF_ADJUST_SCNHDR_IN_POST */
}

static unsigned int
coff_swap_scnhdr_out(bfd * abfd, void *inp, void *outp)
{
  struct internal_scnhdr *scnhdr_int = (struct internal_scnhdr *)inp;
  SCNHDR *scnhdr_ext = (SCNHDR *)outp;
  unsigned int ret = bfd_coff_scnhsz(abfd);

#ifdef COFF_ADJUST_SCNHDR_OUT_PRE
  COFF_ADJUST_SCNHDR_OUT_PRE(abfd, inp, outp);
#endif /* COFF_ADJUST_SCNHDR_OUT_PRE */
  memcpy(scnhdr_ext->s_name, scnhdr_int->s_name, sizeof(scnhdr_int->s_name));

  PUT_SCNHDR_VADDR(abfd, scnhdr_int->s_vaddr, scnhdr_ext->s_vaddr);
  PUT_SCNHDR_PADDR(abfd, scnhdr_int->s_paddr, scnhdr_ext->s_paddr);
  PUT_SCNHDR_SIZE(abfd, scnhdr_int->s_size, scnhdr_ext->s_size);
  PUT_SCNHDR_SCNPTR(abfd, scnhdr_int->s_scnptr, scnhdr_ext->s_scnptr);
  PUT_SCNHDR_RELPTR(abfd, scnhdr_int->s_relptr, scnhdr_ext->s_relptr);
  PUT_SCNHDR_LNNOPTR(abfd, scnhdr_int->s_lnnoptr, scnhdr_ext->s_lnnoptr);
  PUT_SCNHDR_FLAGS(abfd, (bfd_vma)scnhdr_int->s_flags, scnhdr_ext->s_flags);
#if defined(M88)
  H_PUT_32(abfd, (bfd_vma)scnhdr_int->s_nlnno, scnhdr_ext->s_nlnno);
  H_PUT_32(abfd, (bfd_vma)scnhdr_int->s_nreloc, scnhdr_ext->s_nreloc);
#else
  if (scnhdr_int->s_nlnno <= MAX_SCNHDR_NLNNO)
    PUT_SCNHDR_NLNNO(abfd, (bfd_vma)scnhdr_int->s_nlnno,
                     scnhdr_ext->s_nlnno);
  else
    {
      char buf[sizeof(scnhdr_int->s_name) + 1UL];

      memcpy(buf, scnhdr_int->s_name, sizeof(scnhdr_int->s_name));
      buf[sizeof(scnhdr_int->s_name)] = '\0';
      (*_bfd_error_handler)
	(_("%s: warning: %s: line number overflow: 0x%lx > 0xffff"),
	 bfd_get_filename(abfd),
	 buf, scnhdr_int->s_nlnno);
      PUT_SCNHDR_NLNNO(abfd, (bfd_vma)0xffff, scnhdr_ext->s_nlnno);
    }

  if (scnhdr_int->s_nreloc <= MAX_SCNHDR_NRELOC)
    PUT_SCNHDR_NRELOC(abfd, (bfd_vma)scnhdr_int->s_nreloc,
                      scnhdr_ext->s_nreloc);
  else
    {
      char buf[sizeof(scnhdr_int->s_name) + 1UL];

      memcpy(buf, scnhdr_int->s_name, sizeof(scnhdr_int->s_name));
      buf[sizeof(scnhdr_int->s_name)] = '\0';
      (*_bfd_error_handler)(_("%s: %s: reloc overflow: 0x%lx > 0xffff"),
                            bfd_get_filename(abfd),
                            buf, scnhdr_int->s_nreloc);
      bfd_set_error(bfd_error_file_truncated);
      PUT_SCNHDR_NRELOC(abfd, (bfd_vma)0xffff, scnhdr_ext->s_nreloc);
      ret = 0;
    }
#endif /* M88 */

#ifdef I960
  PUT_SCNHDR_ALIGN(abfd, (bfd_vma)scnhdr_int->s_align,
                   scnhdr_ext->s_align);
#endif /* I960 */
#ifdef COFF_ADJUST_SCNHDR_OUT_POST
  COFF_ADJUST_SCNHDR_OUT_POST(abfd, inp, outp);
#endif /* COFF_ADJUST_SCNHDR_OUT_POST */
  return ret;
}

/* EOF */
