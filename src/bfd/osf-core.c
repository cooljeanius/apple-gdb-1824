/* osf-core.c: BFD back-end for OSF/1 core files.
 * Copyright 1993, 1994, 1995, 1998, 1999, 2001, 2002, 2003, 2004
 * Free Software Foundation, Inc.  */
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

/* This file can only be compiled on systems which use OSF/1 style
   core files.  */

#include "bfd.h"
#include "sysdep.h"
#include "libbfd.h"

#include <sys/user.h>
#if defined(__OSF__) || defined(HAVE_SYS_CORE_H) || __has_include(<sys/core.h>)
# include <sys/core.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__) && !defined(__clang__)
 #  warning "osf-core.c expects <sys/core.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ && !__clang__ */
#endif /* __OSF__ || HAVE_SYS_CORE_H || __has_include(<sys/core.h>) */

/* forward declarations: */
static asection *make_bfd_asection
 PARAMS((bfd *, const char *, flagword, bfd_size_type, bfd_vma, file_ptr));
static const bfd_target *osf_core_core_file_p
 PARAMS((bfd *));
static char *osf_core_core_file_failing_command
 PARAMS((bfd *));
static int osf_core_core_file_failing_signal
 PARAMS((bfd *));
static bfd_boolean osf_core_core_file_matches_executable_p
 PARAMS((bfd *, bfd *));
static void swap_abort
 PARAMS((void));

#if !defined(MAXCOMLEN) && !defined(__SYS_CORE_H__)
# define MAXCOMLEN 1
#endif /* !MAXCOMLEN && !__SYS_CORE_H__ */

/* These are stored in the bfd's tdata: */
struct osf_core_struct
{
  int sig;
  char cmd[MAXCOMLEN + 1];
};

#define core_hdr(bfd) ((bfd)->tdata.osf_core_data)
#define core_signal(bfd) (core_hdr(bfd)->sig)
#define core_command(bfd) (core_hdr(bfd)->cmd)

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
  asect->alignment_power = 8;

  return asect;
}

static const bfd_target *osf_core_core_file_p(bfd *abfd)
{
  /* FIXME: add a proper configure check for these: */
#if defined(HAVE_STRUCT_CORE_FILEHDR) && defined(HAVE_STRUCT_CORE_SCNHDR)
  int val;
  int i;
  char *secname;
  struct core_filehdr core_header;
  bfd_size_type amt;

  amt = sizeof(core_header);
  val = bfd_bread((PTR)&core_header, amt, abfd);
  if (val != sizeof(core_header))
    return NULL;

  if (strncmp(core_header.magic, "Core", 4) != 0)
    return NULL;

  core_hdr(abfd) = (struct osf_core_struct *)
    bfd_zalloc(abfd, (bfd_size_type)sizeof(struct osf_core_struct));
  if (!core_hdr(abfd))
    return NULL;

  strncpy(core_command(abfd), core_header.name, (MAXCOMLEN + 1));
  core_signal(abfd) = core_header.signo;

  for (i = 0; i < core_header.nscns; i++)
    {
      struct core_scnhdr core_scnhdr;
      flagword flags;

      amt = sizeof core_scnhdr;
      val = bfd_bread((PTR)&core_scnhdr, amt, abfd);
      if (val != sizeof core_scnhdr)
	break;

      /* Skip empty sections: */
      if ((core_scnhdr.size == 0) || (core_scnhdr.scnptr == 0))
	continue;

      switch (core_scnhdr.scntype)
	{
# ifdef SCNRGN
	case SCNRGN:
	  secname = ".data";
	  flags = (SEC_ALLOC + SEC_LOAD + SEC_HAS_CONTENTS);
	  break;
# endif /* SCNRGN */
# ifdef SCNSTACK
	case SCNSTACK:
	  secname = ".stack";
	  flags = (SEC_ALLOC + SEC_LOAD + SEC_HAS_CONTENTS);
	  break;
# endif /* SCNSTACK */
# ifdef SCNREGS
	case SCNREGS:
	  secname = ".reg";
	  flags = SEC_HAS_CONTENTS;
	  break;
# endif /* SCNREGS */
	default:
	  (*_bfd_error_handler)(_("Unhandled OSF/1 core file section type %d\n"),
                                core_scnhdr.scntype);
	  continue;
	}

      if (!make_bfd_asection(abfd, secname, flags,
			     (bfd_size_type)core_scnhdr.size,
			     (bfd_vma)core_scnhdr.vaddr,
			     (file_ptr)core_scnhdr.scnptr))
	goto fail;
    }

  /* OK, we believe you.  You are a core file (sure, sure).  */

  return abfd->xvec;

 fail:
  bfd_release(abfd, core_hdr(abfd));
  core_hdr(abfd) = NULL;
  bfd_section_list_clear(abfd);
  return NULL;
#else
  BFD_ASSERT(abfd != (bfd *)NULL);
  asection *asect = make_bfd_asection(abfd, "secname", (flagword)0L,
				      (bfd_size_type)0UL, (bfd_vma)0UL,
				      (file_ptr)0L);
  BFD_ASSERT(asect != (asection *)NULL);
  return (const bfd_target *)NULL;
#endif /* HAVE_STRUCT_CORE_FILEHDR && HAVE_STRUCT_CORE_SCNHDR */
}

static char *
osf_core_core_file_failing_command(bfd *abfd)
{
  return core_command(abfd);
}

static int
osf_core_core_file_failing_signal(bfd *abfd)
{
  return core_signal(abfd);
}

static bfd_boolean
osf_core_core_file_matches_executable_p(bfd *core_bfd ATTRIBUTE_UNUSED,
                                        bfd *exec_bfd ATTRIBUTE_UNUSED)
{
  return TRUE;	    /* FIXME: We have no way of telling at this point */
}

/* If somebody calls any byte-swapping routines, then shoot them: */
static void ATTRIBUTE_NORETURN
swap_abort(void)
{
  /* This way does NOT require any declaration for ANSI to mess up (?): */
  abort();
}

#define	NO_GET ((bfd_vma (*)(const void *))swap_abort)
#define	NO_PUT ((void (*)(bfd_vma, void *))swap_abort)
#define	NO_GETS ((bfd_signed_vma (*)(const void *))swap_abort)
#define	NO_GET64 ((bfd_uint64_t (*)(const void *))swap_abort)
#define	NO_PUT64 ((void (*)(bfd_uint64_t, void *))swap_abort)
#define	NO_GETS64 ((bfd_int64_t (*)(const void *))swap_abort)

const bfd_target osf_core_vec =
  {
    "osf-core",
    bfd_target_unknown_flavour,
    BFD_ENDIAN_LITTLE,		/* target byte order */
    BFD_ENDIAN_LITTLE,		/* target headers byte order */
    (HAS_RELOC | EXEC_P |	/* object flags */
     HAS_LINENO | HAS_DEBUG |
     HAS_SYMS | HAS_LOCALS | WP_TEXT | D_PAGED),
    (SEC_HAS_CONTENTS | SEC_ALLOC | SEC_LOAD | SEC_RELOC), /* section flags */
    0,			                                   /* symbol prefix */
    ' ',						   /* ar_pad_char */
    16,							   /* ar_max_namelen */
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
      osf_core_core_file_p		/* a core file */
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
    BFD_JUMP_TABLE_CORE(osf_core),
    BFD_JUMP_TABLE_ARCHIVE(_bfd_noarchive),
    BFD_JUMP_TABLE_SYMBOLS(_bfd_nosymbols),
    BFD_JUMP_TABLE_RELOCS(_bfd_norelocs),
    BFD_JUMP_TABLE_WRITE(_bfd_generic),
    BFD_JUMP_TABLE_LINK(_bfd_nolink),
    BFD_JUMP_TABLE_DYNAMIC(_bfd_nodynamic),

    NULL,

    (PTR) 0			/* backend_data */
  };

/* EOF */
