/* sco5-core.c: BFD back end for SCO5 core files (U-area and raw sections)
 * Copyright 1998, 1999, 2000, 2001, 2002, 2003, 2004
 * Free Software Foundation, Inc.
 * Written by Jouke Numan <jnuman@hiscom.nl>  */
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
Foundation, Inc., 51 Franklin St., 5th Floor, Boston, MA 02110-1301, USA */

#include "bfd.h"
#include "sysdep.h"
#include "libbfd.h"
#include "libaout.h"		/* BFD a.out internal data structures */

#include "coff/internal.h"    /* BFD coff internal data structures */

#include <stdio.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/dir.h>
#include <signal.h>

#include <sys/user.h> /* After a.out.h  */
#if defined(__USLC__) || defined(HAVE_SYS_PACCESS_H) || \
    __has_include(<sys/paccess.h>)
# include <sys/paccess.h>
#endif /* __USLC__ || HAVE_SYS_PACCESS_H */
#if defined(__USLC__) || defined(HAVE_SYS_REGION_H) || \
    __has_include(<sys/region.h>)
# include <sys/region.h>
#endif /* __USLC__ || HAVE_SYS_REGION_H */

#ifdef HAVE_ERRNO_H
# include <errno.h>
#else
# ifdef HAVE_SYS_ERRNO_H
#  include <sys/errno.h>
# else
#  ifndef EPERM
#   define EPERM 1
#  endif /* !EPERM */
# endif /* HAVE_SYS_ERRNO_H */
#endif /* HAVE_ERRNO_H */

#include "trad-user.h"

struct sco5_core_struct
{
  user_struct_t u;
};

/* forward declarations: */
static asection *make_bfd_asection
  PARAMS((bfd *, const char *, flagword, bfd_size_type, bfd_vma, file_ptr));
static user_struct_t *read_uarea PARAMS((bfd *, int));
const bfd_target *sco5_core_file_p PARAMS((bfd *abfd));
char *sco5_core_file_failing_command PARAMS((bfd *abfd));
int sco5_core_file_failing_signal PARAMS((bfd *abfd));
bfd_boolean sco5_core_file_matches_executable_p
  PARAMS((bfd *core_bfd, bfd *exec_bfd));
static void swap_abort PARAMS((void));

static asection *
make_bfd_asection(bfd *abfd, const char *name, flagword flags,
                  bfd_size_type size, bfd_vma vma, file_ptr filepos)
{
  asection *asect;

  asect = bfd_make_section_anyway(abfd, name);
  if (!asect)
    return NULL;
  asect->flags = flags;
  asect->size = size;
  asect->vma = vma;
  asect->filepos = filepos;
  asect->alignment_power = 2;

  return asect;
}

static user_struct_t *
read_uarea(bfd *abfd, int filepos)
{
  struct sco5_core_struct *rawptr;
  bfd_size_type amt = sizeof(struct sco5_core_struct);

  rawptr = (struct sco5_core_struct *)bfd_zmalloc(amt);
  if (rawptr == NULL)
    return NULL;

  abfd->tdata.sco5_core_data = rawptr;

  if ((bfd_seek(abfd, (file_ptr)filepos, SEEK_SET) != 0)
      || (bfd_bread((void *)&rawptr->u, (bfd_size_type)sizeof(rawptr->u),
                    abfd) != sizeof(rawptr->u)))
    {
      bfd_set_error(bfd_error_wrong_format);
      return NULL;
    }

  /* Sanity check perhaps??? */
  if (rawptr->u.u_dsize > 0x1000000)    /* Remember, it is in pages...  */
    {
      bfd_set_error(bfd_error_wrong_format);
      return NULL;
    }
  if (rawptr->u.u_ssize > 0x1000000)
    {
      bfd_set_error(bfd_error_wrong_format);
      return NULL;
    }
  return &rawptr->u;
}


const bfd_target *
sco5_core_file_p(bfd *abfd)
{
  size_t coffset_siz;
  int nsecs, cheadoffs;
  size_t coresize;
  user_struct_t *u;
  struct coreoffsets coffsets;
  struct coresecthead chead;
  char *secname;
  flagword flags;

  /* Read coreoffsets region at end of core (see core(FP)): */
  {
    FILE *stream = bfd_cache_lookup(abfd);
    struct stat statbuf;

    if (fstat(fileno(stream), &statbuf) < 0)
      {
	bfd_set_error(bfd_error_system_call);
	return NULL;
      }
    coresize = (size_t)statbuf.st_size;
  }
  /* Last long in core is sizeof struct coreoffsets, read it: */
  if ((bfd_seek(abfd, (file_ptr)(coresize - sizeof(coffset_siz)),
                SEEK_SET) != 0)
      || bfd_bread((void *)&coffset_siz, (bfd_size_type)sizeof(coffset_siz),
                   abfd) != sizeof(coffset_siz))
    {
      bfd_set_error(bfd_error_wrong_format);
      return NULL;
    }

  /* Use it to seek start of coreoffsets region, read it, and determine
   * validity: */
  if ((bfd_seek(abfd, (file_ptr)(coresize - coffset_siz), SEEK_SET) != 0)
      || (bfd_bread((void *)&coffsets, (bfd_size_type)sizeof(coffsets), abfd)
	  != sizeof(coffsets))
      || ((coffsets.u_info != 1) && (coffsets.u_info != C_VERSION)))
    {
      bfd_set_error(bfd_error_wrong_format);
      return NULL;
    }

  if (coffsets.u_info == 1)
    {
      /* Old version, no section heads, read info from user struct: */
      u = read_uarea(abfd, coffsets.u_user);
      if (! u)
	goto fail;

      if (!make_bfd_asection(abfd, ".reg", SEC_HAS_CONTENTS,
                             (bfd_size_type)coffsets.u_usize,
                             (0 - (bfd_vma)(uintptr_t)u->u_ar0),
                             (file_ptr)coffsets.u_user))
	goto fail;

#ifdef _SCO_DS
      if (!make_bfd_asection(abfd, ".data",
                             (SEC_ALLOC + SEC_LOAD + SEC_HAS_CONTENTS),
                             ((bfd_size_type)u->u_exdata.ux_dsize
                              + u->u_exdata.ux_bsize),
                             (bfd_vma)u->u_exdata.ux_datorg,
                             (file_ptr)coffsets.u_data))
	goto fail;
#else
      if (!make_bfd_asection(abfd, ".data",
                             (SEC_ALLOC + SEC_LOAD + SEC_HAS_CONTENTS),
                             (bfd_size_type)0UL, (bfd_vma)0UL,
                             (file_ptr)coffsets.u_data))
	goto fail;
#endif /* _SCO_DS*/

/* in case this is undefined: */
#ifndef NBPC
/* choose a value by which it is safe to multiply: */
# define NBPC 1
#endif /* !NBPC */

#ifdef _SCO_DS
      if (!make_bfd_asection(abfd, ".stack",
                             (SEC_ALLOC + SEC_LOAD + SEC_HAS_CONTENTS),
                             ((bfd_size_type)u->u_ssize * NBPC),
                             (bfd_vma)u->u_sub,
                             (file_ptr)coffsets.u_stack))
	goto fail;
#else
      if (!make_bfd_asection(abfd, ".stack",
                             (SEC_ALLOC + SEC_LOAD + SEC_HAS_CONTENTS),
                             ((bfd_size_type)u->u_ssize * NBPC),
                             (bfd_vma)0UL,
                             (file_ptr)coffsets.u_stack))
	goto fail;
#endif /* _SCO_DS */

      return abfd->xvec;		/* Done for version 1 */
    }

  /* Immediately before coreoffsets region is a long with offset in core
     to first coresecthead (CORES_OFFSETS), the long before this is the
     number of section heads in the list. Read both longs and read the
     coresecthead and check its validity */

  if ((bfd_seek(abfd,
                (file_ptr)(coresize - coffset_siz - 2UL * sizeof(coffset_siz)),
                SEEK_SET) != 0)
      || (bfd_bread((void *)&nsecs, (bfd_size_type)sizeof(nsecs), abfd)
	  != sizeof(nsecs))
      || (bfd_bread((void *)&cheadoffs, (bfd_size_type)sizeof(cheadoffs),
                    abfd) != sizeof(cheadoffs))
      || (bfd_seek(abfd, (file_ptr)cheadoffs, SEEK_SET) != 0)
      || (bfd_bread((void *)&chead, (bfd_size_type)sizeof(chead), abfd)
	  != sizeof(chead))
#ifdef CORES_OFFSETS
      || (chead.cs_stype != CORES_OFFSETS)
#endif /* CORES_OFFSETS */
#ifdef COREMAGIC_NUMBER
      || (chead.cs_x.csx_magic != COREMAGIC_NUMBER)
#endif /* COREMAGIC_NUMBER */
      || (errno == EPERM))
    {
      bfd_set_error(bfd_error_wrong_format);
      goto fail;
    }

  /* OK, we believe you.  You are a core file (sure, sure).  */

  /* Now loop over all regions and map them: */
  nsecs--;			/* We have seen CORES_OFFSETS already */
  for (; nsecs; nsecs--)
    {
      if ((bfd_seek(abfd, (file_ptr)chead.cs_hseek, SEEK_SET) != 0)
	  || (bfd_bread((void *)&chead, (bfd_size_type)sizeof(chead), abfd)
	      != sizeof(chead)))
	{
	  bfd_set_error(bfd_error_wrong_format);
	  goto fail;
	}

      switch (chead.cs_stype)
	{
#ifdef CORES_MAGIC
	case CORES_MAGIC:		/* Core header, check magic */
	  if (chead.cs_x.csx_magic != COREMAGIC_NUMBER)
	    {
	      bfd_set_error(bfd_error_wrong_format);
	      goto fail;
	    }
	  secname = NULL;
	  nsecs++;			/* MAGIC not in section cnt!*/
	  break;
#endif /* CORES_MAGIC */
#ifdef CORES_UAREA
	case CORES_UAREA:		/* U-area, read in tdata */
	  u = read_uarea(abfd, chead.cs_sseek);
	  if (! u)
	    goto fail;

	  /* This is tricky.  As the "register section", we give them
	   * the entire upage and stack.  u.u_ar0 points to where
	   * "register 0" is stored.  There are two tricks with this,
	   * though.  One is that the rest of the registers might be
	   * at positive or negative (or both) displacements from
	   * *u_ar0.  The other is that u_ar0 is sometimes an absolute
	   * address in kernel memory, and on other systems it is an
	   * offset from the beginning of the `struct user'.
           *
	   * As a practical matter, we do NOT know where the registers
	   * actually are, so we have to pass the whole area to GDB.
	   * We encode the value of u_ar0 by setting the .regs section
	   * up so that its virtual memory address 0 is at the place
	   * pointed to by u_ar0 (by setting the vma of the start of
	   * the section to -u_ar0).  GDB uses this info to locate the
	   * regs, using minor trickery to get around the
	   * offset-or-absolute-addr problem: */
	  chead.cs_vaddr = (0 - (bfd_vma)u->u_ar0);

	  secname = ".reg";
	  flags = SEC_HAS_CONTENTS;

	  break;
#endif /* CORES_UAREA */
#ifdef CORES_PREGION
	case CORES_PREGION:		/* A program region, map it */
	  switch (chead.cs_x.csx_preg.csxp_rtyp)
	    {
# ifdef PT_DATA
	    case PT_DATA:
	      secname = ".data";	/* Data region.		 */
	      break;
# endif /* PT_DATA */
# ifdef PT_STACK
	    case PT_STACK:
	      secname = ".stack";	/* Stack region.	 */
	      break;
# endif /* PT_STACK*/
# ifdef PT_SHMEM
	    case PT_SHMEM:
	      secname = ".shmem";	/* Shared memory	 */
	      break;
# endif /* PT_SHMEM */
# ifdef PT_LIBDAT
	    case PT_LIBDAT:
	      secname = ".libdat";	/* Shared library data	 */
	      break;
# endif /* PT_LIBDAT */
# ifdef PT_V86
	    case PT_V86:
	      secname = ".virt86";	/* Virtual 8086 mode	 */
	      break;
# endif /* PT_V86 */
# ifdef PT_SHFIL
	    case PT_SHFIL:
	      secname = ".mmfile";	/* Memory mapped file	 */
	      break;
# endif /* PT_SHFIL */
# ifdef PT_XDATA0
	    case PT_XDATA0:
	      secname = ".Xdat0";	/* XENIX data region, virtual 0 */
	      break;
# endif /* PT_XDATA0 */
	    default:
	      secname = "";
	    }
	  flags = (SEC_ALLOC + SEC_LOAD + SEC_HAS_CONTENTS);
	  break;
#endif /* CORES_PREGION */
#ifdef CORES_PROC
	case CORES_PROC:		/* struct proc */
#endif /* CORES_PROC */
#ifdef CORES_ITIMER
	case CORES_ITIMER:		/* interval timers */
#endif /* CORES_ITIMER */
#ifdef CORES_SCOUTSNAME
	case CORES_SCOUTSNAME:		/* struct scoutsname */
	  secname = NULL;	/* Ignore these */
	  break;
#endif /* CORES_SCOUTSNAME */
	default:
	  (*_bfd_error_handler)("Unhandled SCO core file section type %d\n",
                                chead.cs_stype);
	  continue;
	}

      if (secname
	  && !make_bfd_asection(abfd, secname, flags,
                                (bfd_size_type)chead.cs_vsize,
                                (bfd_vma)chead.cs_vaddr,
                                (file_ptr)chead.cs_sseek))
	goto fail;

    }

  return abfd->xvec;

 fail:
  if (abfd->tdata.any)
    {
      bfd_release(abfd, abfd->tdata.any);
      abfd->tdata.any = NULL;
    }
  bfd_section_list_clear(abfd);
  (void)chead;
  (void)coffsets;
  return NULL;
}

char *
sco5_core_file_failing_command(bfd *abfd)
{
  char *com = abfd->tdata.sco5_core_data->u.u_comm;
  if (*com)
    return com;
  else
    return NULL;
}

int
sco5_core_file_failing_signal(bfd *ignore_abfd)
{
#ifdef _SCO_DS
  return ((ignore_abfd->tdata.sco5_core_data->u.u_sysabort != 0)
	  ? ignore_abfd->tdata.sco5_core_data->u.u_sysabort
	  : -1);
#else
  (void)ignore_abfd;
  return -1;
#endif /* _SCO_DS */
  /*NOTREACHED*/
}

bfd_boolean
sco5_core_file_matches_executable_p(bfd *core_bfd ATTRIBUTE_UNUSED,
                                    bfd *exec_bfd ATTRIBUTE_UNUSED)
{
  return TRUE;	/* FIXME, We have no way of telling at this point... */
}

/* If somebody calls any byte-swapping routines, then shoot them: */
static void
swap_abort(void)
{
  /* This way does NOT require any declaration for ANSI to mess up: */
  abort();
}

#define	NO_GET ((bfd_vma (*)(const void *))swap_abort)
#define	NO_PUT ((void (*)(bfd_vma, void *))swap_abort)
#define	NO_GETS ((bfd_signed_vma (*)(const void *))swap_abort)
#define	NO_GET64 ((bfd_uint64_t (*)(const void *))swap_abort)
#define	NO_PUT64 ((void (*)(bfd_uint64_t, void *))swap_abort)
#define	NO_GETS64 ((bfd_int64_t (*)(const void *))swap_abort)

const bfd_target sco5_core_vec =
  {
    "sco5-core",
    bfd_target_unknown_flavour,
    BFD_ENDIAN_LITTLE,	       /* target byte order */
    BFD_ENDIAN_LITTLE,	       /* target headers byte order */
    (HAS_RELOC | EXEC_P |	/* object flags */
     HAS_LINENO | HAS_DEBUG |
     HAS_SYMS | HAS_LOCALS | WP_TEXT | D_PAGED),
    (SEC_HAS_CONTENTS | SEC_ALLOC | SEC_LOAD | SEC_RELOC), /* section flags */
    0,							/* symbol prefix */
    ' ',						  /* ar_pad_char */
    16,						       /* ar_max_namelen */
    NO_GET64, NO_GETS64, NO_PUT64,	/* 64 bit data */
    NO_GET, NO_GETS, NO_PUT,		/* 32 bit data */
    NO_GET, NO_GETS, NO_PUT,		/* 16 bit data */
    NO_GET64, NO_GETS64, NO_PUT64,	/* 64 bit hdrs */
    NO_GET, NO_GETS, NO_PUT,		/* 32 bit hdrs */
    NO_GET, NO_GETS, NO_PUT,		/* 16 bit hdrs */

    {				/* bfd_check_format */
      _bfd_dummy_target,		/* unknown format */
      _bfd_dummy_target,		/* object file */
      _bfd_dummy_target,		/* archive */
      sco5_core_file_p			/* a core file */
    },
    {				/* bfd_set_format */
      bfd_false, bfd_false,
      bfd_false, bfd_false
    },
    {				/* bfd_write_contents */
      bfd_false, bfd_false,
      bfd_false, bfd_false
    },

    BFD_JUMP_TABLE_GENERIC(_bfd_generic),
    BFD_JUMP_TABLE_COPY(_bfd_generic),
    BFD_JUMP_TABLE_CORE(sco5),
    BFD_JUMP_TABLE_ARCHIVE(_bfd_noarchive),
    BFD_JUMP_TABLE_SYMBOLS(_bfd_nosymbols),
    BFD_JUMP_TABLE_RELOCS(_bfd_norelocs),
    BFD_JUMP_TABLE_WRITE(_bfd_generic),
    BFD_JUMP_TABLE_LINK(_bfd_nolink),
    BFD_JUMP_TABLE_DYNAMIC(_bfd_nodynamic),

    NULL,

    (PTR)0 /* backend_data */
  };

/* EOF */
