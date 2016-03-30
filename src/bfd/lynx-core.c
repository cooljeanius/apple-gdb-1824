/* lynx-core.c: BFD back end for Lynx core files
   Copyright 1993, 1994, 1995, 2001, 2002, 2004
   Free Software Foundation, Inc.
   Written by Stu Grossman of Cygnus Support.

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

#ifdef LYNX_CORE

#include <sys/conf.h>
#include <sys/kernel.h>
/* sys/kernel.h should define this, but doesn't always, sigh.  */
#ifndef __LYNXOS
# define __LYNXOS
#endif /* !__LYNXOS */
#include <sys/mem.h>
#include <sys/signal.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/itimer.h>
#include <sys/file.h>
#include <sys/proc.h>

/* Found by a Google search: */
#ifndef PNMLEN
# define PNMLEN 32
#endif /* PNMLEN */

/* Prototypes: */
extern const bfd_target *lynx_core_file_p(bfd *);
extern char *lynx_core_file_failing_command(bfd *);
extern int lynx_core_file_failing_signal(bfd *);
extern int lynx_core_file_failing_signal(bfd *);
extern bfd_boolean lynx_core_file_matches_executable_p(bfd *, bfd *);

/* These are stored in the bfd's tdata: */
struct lynx_core_struct
{
  int sig;
  char cmd[PNMLEN + 1];
};

#define core_hdr(bfd) ((bfd)->tdata.lynx_core_data)
#define core_signal(bfd) (core_hdr(bfd)->sig)
#define core_command(bfd) (core_hdr(bfd)->cmd)

/* Handle Lynx core dump file: */
static asection *
make_bfd_asection(bfd *abfd, const char *name, flagword flags,
		  bfd_size_type size, bfd_vma vma, file_ptr filepos)
{
  asection *asect;
  char *newname;

  newname = (char *)bfd_alloc(abfd, (bfd_size_type)(strlen(name) + 1UL));
  if (!newname)
    return NULL;

  strcpy (newname, name);

  asect = bfd_make_section (abfd, newname);
  if (!asect)
    return NULL;

  asect->flags = flags;
  asect->size = size;
  asect->vma = vma;
  asect->filepos = filepos;
  asect->alignment_power = 2;

  return asect;
}

const bfd_target *
lynx_core_file_p(bfd *abfd)
{
  int secnum;
  struct pssentry pss;
  bfd_size_type tcontext_size;
  core_st_t *threadp;
  int pagesize;
  asection *newsect;
  bfd_size_type amt;

  pagesize = getpagesize ();	/* Serious cross-target issue here...  This
				   really needs to come from a system-specific
				   header file.  */

  /* Get the pss entry from the core file */

  if (bfd_seek (abfd, (file_ptr) 0, SEEK_SET) != 0)
    return NULL;

  amt = sizeof pss;
  if (bfd_bread ((void *) &pss, amt, abfd) != amt)
    {
      /* Too small to be a core file */
      if (bfd_get_error () != bfd_error_system_call)
	bfd_set_error (bfd_error_wrong_format);
      return NULL;
    }

  amt = sizeof (struct lynx_core_struct);
  core_hdr (abfd) = (struct lynx_core_struct *) bfd_zalloc (abfd, amt);

  if (!core_hdr (abfd))
    return NULL;

  strncpy (core_command (abfd), pss.pname, PNMLEN + 1);

  /* Compute the size of the thread contexts */

  tcontext_size = pss.threadcnt * sizeof (core_st_t);

  /* Allocate space for the thread contexts */

  threadp = (core_st_t *) bfd_alloc (abfd, tcontext_size);
  if (!threadp)
    goto fail;

  /* Save thread contexts */

  if (bfd_seek (abfd, (file_ptr) pagesize, SEEK_SET) != 0)
    goto fail;

  if (bfd_bread ((void *) threadp, tcontext_size, abfd) != tcontext_size)
    {
      /* Probably too small to be a core file */
      if (bfd_get_error () != bfd_error_system_call)
	bfd_set_error (bfd_error_wrong_format);
      goto fail;
    }

  core_signal (abfd) = threadp->currsig;

  newsect = make_bfd_asection (abfd, ".stack",
			       SEC_ALLOC + SEC_LOAD + SEC_HAS_CONTENTS,
			       pss.ssize,
			       pss.slimit,
			       pagesize + tcontext_size);
  if (!newsect)
    goto fail;

  newsect = make_bfd_asection (abfd, ".data",
			       SEC_ALLOC + SEC_LOAD + SEC_HAS_CONTENTS,
			       pss.data_len + pss.bss_len,
			       pss.data_start,
			       pagesize + tcontext_size + pss.ssize
#if defined (SPARC) || defined (__SPARC__)
			       /* SPARC Lynx seems to start dumping
                                  the .data section at a page
                                  boundary.  It's OK to check a
                                  #define like SPARC here because this
                                  file can only be compiled on a Lynx
                                  host.  */
			       + pss.data_start % pagesize
#endif
			       );
  if (!newsect)
    goto fail;

/* And, now for the .reg/XXX pseudo sections.  Each thread has it's own
   .reg/XXX section, where XXX is the thread id (without leading zeros).  The
   currently running thread (at the time of the core dump) also has an alias
   called `.reg' (just to keep GDB happy).  Note that we use `.reg/XXX' as
   opposed to `.regXXX' because GDB expects that .reg2 will be the floating-
   point registers.  */

  newsect = make_bfd_asection (abfd, ".reg",
			       SEC_HAS_CONTENTS,
			       sizeof (core_st_t),
			       0,
			       pagesize);
  if (!newsect)
    goto fail;

  for (secnum = 0; secnum < pss.threadcnt; secnum++)
    {
      char secname[100];
      
#ifndef BUILDPID
# define BUILDPID(pid, tid) (pid)
#endif /* !BUILDPID */

      snprintf(secname, sizeof(secname), ".reg/%d",
	       BUILDPID(0, threadp[secnum].tid));
      newsect = make_bfd_asection (abfd, secname,
				   SEC_HAS_CONTENTS,
				   sizeof (core_st_t),
				   0,
				   pagesize + secnum * sizeof (core_st_t));
      if (!newsect)
	goto fail;
    }

  return abfd->xvec;

 fail:
  bfd_release (abfd, core_hdr (abfd));
  core_hdr (abfd) = NULL;
  bfd_section_list_clear (abfd);
  return NULL;
}

char *
lynx_core_file_failing_command(bfd *abfd)
{
  return core_command(abfd);
}

int
lynx_core_file_failing_signal(bfd *abfd)
{
  return core_signal(abfd);
}

bfd_boolean
lynx_core_file_matches_executable_p(bfd *core_bfd, bfd *exec_bfd)
{
  return TRUE;		/* FIXME: We have no way of telling at this point */
}
#ifdef __LYNXOS
# undef __LYNXOS
#endif /* __LYNXOS */
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__) && !defined(__STDC__)
#  warning "lynx-core.c will be empty in this configuration"
# endif /* __GNUC__ && !__STRICT_ANSI__ && !__STDC__ */
typedef int lynx_core_c_dummy_t;
extern lynx_core_c_dummy_t lynx_core_c_dummy_var;
#endif /* LYNX_CORE */

/* EOF */
