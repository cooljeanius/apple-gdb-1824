/* bdfwin.c: Support for memory-mapped windows into a BFD.
 * Copyright 1995, 1996, 2001, 2002, 2003 Free Software Foundation, Inc.
 * Written by Cygnus Support.  */
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
Foundation, Inc., 51 Franklin St., 5th Floor, Boston, MA 02110-1301, USA
*/

#include "sysdep.h"

#include "bfd.h"
#include "libbfd.h"

#undef HAVE_MPROTECT

#if (defined(HAVE_MMAP) && HAVE_MMAP) || \
    (defined(HAVE_MPROTECT) && HAVE_MPROTECT) || \
    (defined(HAVE_MADVISE) && HAVE_MADVISE)
# include <sys/types.h>
# include <sys/mman.h>
#endif /* HAVE_MMAP || HAVE_MPROTECT || HAVE_MADVISE */

#undef MAP_SHARED
#define MAP_SHARED MAP_PRIVATE

#ifndef MAP_FILE
# define MAP_FILE 0
#endif /* !MAP_FILE */

#if !defined(HAVE_GETPAGESIZE) || !defined(HAVE_DECL_GETPAGESIZE) || !HAVE_DECL_GETPAGESIZE
# define getpagesize() 2048
#endif /* !HAVE_GETPAGESIZE || !HAVE_DECL_GETPAGESIZE */

static bfd_boolean _bfd_get_file_window_mmap
  PARAMS((bfd *abfd, ufile_ptr offset, bfd_size_type size,
          bfd_window *windowp, bfd_window_internal *i, bfd_boolean writable));

#if !defined(HAVE_MMAP) || !HAVE_MMAP
static bfd_boolean _bfd_get_file_window_malloc
  PARAMS((bfd *abfd, ufile_ptr offset, bfd_size_type size,
          bfd_window *windowp, bfd_window_internal *i, bfd_boolean writable));
#endif /* !HAVE_MMAP */

/* The idea behind the next and refcount fields is that one mapped
   region can suffice for multiple read-only windows or multiple
   non-overlapping read-write windows.  It's not implemented yet
   though.  */

/*
CODE_FRAGMENT
. {* XXX: implement this *}
*/

/* APPLE LOCAL: Add new value for 'mapped', for the case where we have
   a window into a bfd that has already been fully mapped into
   memory. */

/*
INTERNAL_DEFINITION

.struct _bfd_window_internal {
.  struct _bfd_window_internal *next;
.  void *data;
.  bfd_size_type size;
.  int refcount : 30;		{* should be enough...  *}
.  unsigned mapped : 2; {* 2 = window into mapped bfd, 1 = mmap, 0 = malloc *}
.};
*/

/*
FUNCTION
	bfd_init_window

SYNOPSIS
	void bfd_init_window(bfd_window *windowp);

DESCRIPTION
	Initialization function for a bfd window.

*/
void
bfd_init_window(bfd_window *windowp)
{
  windowp->data = 0;
  windowp->i = 0;
  windowp->size = 0;
}

/* Currently, if USE_MMAP is undefined, none if the window stuff is
   used.  Okay, so it is mis-named.  At least the command-line option
   "--without-mmap" is more obvious than "--without-windows" or some
   such.  */

static int debug_windows;

/*
FUNCTION
	bfd_free_window

SYNOPSIS
	void bfd_free_window(bfd_window *windowp);

DESCRIPTION
	Cleanup function for bfd windows.

*/
void
bfd_free_window(bfd_window *windowp)
{
  bfd_window_internal *i = windowp->i;

  windowp->i = 0;
  windowp->data = 0;

  if (i == 0)
    return;

  i->refcount--;
  if (debug_windows)
    fprintf(stderr, "freeing window @%p<%p,%lx,%p>\n",
	    (void *)windowp, windowp->data, (unsigned long)windowp->size,
            (void *)windowp->i);
  if (i->refcount > 0)
    return;

  switch (i->mapped) {

  case 2:
    i->data = NULL;
    break;

  case 1:
#if defined(HAVE_MMAP) && HAVE_MMAP
    munmap(i->data, (size_t)i->size);
    i->data = NULL;
    break; /* -Wimplicit-fallthrough */
#else
    abort();
#endif /* HAVE_MMAP */

  case 0:
#if defined(HAVE_MPROTECT) && HAVE_MPROTECT
    mprotect(i->data, i->size, (PROT_READ | PROT_WRITE));
#endif /* HAVE_MPROTECT */
    free(i->data);
    i->data = NULL;
    break;

  default:
    BFD_ASSERT((i->mapped == 0) || (i->mapped == 1) || (i->mapped == 2));
  }

  /* There should be no more references to i at this point: */
  free(i);
}

static int ok_to_map = 1;

#if defined(HAVE_MMAP) && HAVE_MMAP
static bfd_boolean
_bfd_get_file_window_mmap(bfd *abfd, ufile_ptr offset, bfd_size_type size,
                          bfd_window *windowp, bfd_window_internal *i,
                          bfd_boolean writable)
{
  static size_t pagesize;
  bfd_size_type size_to_alloc = size;

  if (debug_windows)
    fprintf(stderr,
            "bfd_get_file_window (%p, %6ld, %6ld, %p<%p,%lx,%p>, %d)",
            (void *)abfd, (long)offset, (long)size, (void *)windowp,
            windowp->data, (unsigned long)windowp->size,
            (void *)windowp->i, writable);

  /* Make sure we know the page size, so we can be friendly to mmap: */
  if (pagesize == 0) {
#if defined(HAVE_GETPAGESIZE) || defined(getpagesize)
    pagesize = (size_t)getpagesize();
#else
    pagesize = 2048;
#endif /* HAVE_GETPAGESIZE || getpagesize */
  }
  if (pagesize == 0)
    abort();

# ifdef HAVE_MMAP
  if (ok_to_map
      && ((i->data == 0) || (i->mapped == 1))
      && ((abfd->flags & BFD_IN_MEMORY) == 0))
    {
      file_ptr file_offset, offset2;
      size_t real_size;
      int fd;
      FILE *f;

      /* Find the real file and the real offset into it: */
      while (abfd->my_archive != NULL)
	{
	  offset += abfd->origin;
	  abfd = abfd->my_archive;
	}
      f = bfd_cache_lookup(abfd);
      fd = fileno(f);

      /* Compute offsets and size for mmap and for the user's data: */
      offset2 = (file_ptr)(offset % pagesize);
      if (offset2 < 0)
	abort();
      file_offset = ((file_ptr)offset - offset2);
      real_size = (size_t)(offset + size - (unsigned long)file_offset);
      real_size = (real_size + pagesize - 1UL);
      real_size -= (real_size % pagesize);

      /* If we are re-using a memory region, make sure it is big enough: */
      if (i->data && (i->size < size))
	{
	  munmap(i->data, (size_t)i->size);
	  i->data = 0;
	}
      /* FIXME: what if MAP_PRIVATE and MAP_SHARED have the same value? */
      i->data = mmap(i->data, real_size,
                     (writable ? (PROT_WRITE | PROT_READ) : PROT_READ),
                     (writable
                      ? (MAP_FILE | MAP_PRIVATE)
                      : (MAP_FILE | MAP_SHARED)),
                     fd, (off_t)file_offset);
      if (i->data == (void *)-1)
	{
	  /* An error happened.  Report it, or try using malloc, or
	   * something: */
	  bfd_set_error(bfd_error_system_call);
	  i->data = 0;
	  windowp->data = 0;
	  if (debug_windows)
	    fprintf(stderr, "\t\tmmap failed!\n");
	  return FALSE;
	}
      if (debug_windows)
	fprintf(stderr, "\n\tmapped %ld at %p, offset is %ld\n",
                (long)real_size, i->data, (long)offset2);
      i->size = real_size;
      windowp->data = ((bfd_byte *)i->data + offset2);
      windowp->size = size;
      i->mapped = 1;
      return TRUE;
    }
  else if (debug_windows)
    {
      if (ok_to_map)
	fprintf(stderr, _("not mapping: data=%lx mapped=%d\n"),
                (unsigned long)i->data, (int)i->mapped);
      else
	fprintf(stderr, _("not mapping: env var not set\n"));
    }
# else
  ok_to_map = 0;
# endif /* HAVE_MMAP */

# ifdef HAVE_MPROTECT
  if (!writable)
    {
      size_to_alloc += (pagesize - 1);
      size_to_alloc -= (size_to_alloc % pagesize);
    }
# endif /* HAVE_MPROTECT */
  if (debug_windows)
    fprintf(stderr, "\n\t%s(%6ld)",
            (i->data ? "realloc" : " malloc"), (long)size_to_alloc);
  i->data = bfd_realloc(i->data, size_to_alloc);
  if (debug_windows)
    fprintf(stderr, "\t-> %p\n", i->data);
  i->refcount = 1;
  if (i->data == NULL)
    {
      if (size_to_alloc == 0)
	return TRUE;
      return FALSE;
    }
  if (bfd_seek(abfd, (file_ptr)offset, SEEK_SET) != 0)
    return FALSE;
  i->size = bfd_bread(i->data, size, abfd);
  if (i->size != size)
    return FALSE;
  i->mapped = 0;
# ifdef HAVE_MPROTECT
  if (!writable)
    {
      if (debug_windows)
	fprintf(stderr, "\tmprotect (%p, %ld, PROT_READ)\n", i->data,
                (long)i->size);
      mprotect(i->data, i->size, PROT_READ);
    }
# endif /* HAVE_MPROTECT */
  windowp->data = i->data;
  windowp->size = i->size;
  return TRUE;
}
#endif /* HAVE_MMAP */

#if !defined(HAVE_MMAP) || !HAVE_MMAP
static bfd_boolean
_bfd_get_file_window_malloc(bfd *abfd, ufile_ptr offset,
                            bfd_size_type size, bfd_window *windowp,
                            bfd_window_internal *i,
                            bfd_boolean writable ATTRIBUTE_UNUSED)
{
  static size_t pagesize;
  size_t size_to_alloc = size;

  /* Make sure we know the page size, so we can be friendly to mmap: */
  if (pagesize == 0) {
#if defined(HAVE_GETPAGESIZE) || defined(getpagesize)
    pagesize = (size_t)getpagesize();
#else
    pagesize = 2048;
#endif /* HAVE_GETPAGESIZE || getpagesize */
  }
  if (pagesize == 0)
    abort();

# if defined(HAVE_MPROTECT) && HAVE_MPROTECT
  if (!writable)
    {
      size_to_alloc += (pagesize - 1);
      size_to_alloc -= (size_to_alloc % pagesize);
    }
# endif /* HAVE_MPROTECT */

  if (debug_windows)
    fprintf(stderr, "\n\t%s(%6ld)",
            (i->data ? "realloc" : " malloc"), (long)size_to_alloc);
  i->data = (PTR)bfd_realloc(i->data, size_to_alloc);
  if (debug_windows)
    fprintf(stderr, "\t-> %p\n", i->data);
  i->refcount = 1;
  if (i->data == NULL)
    {
      if (size_to_alloc == 0)
	return TRUE;
      bfd_set_error(bfd_error_no_memory);
      return FALSE;
    }
  if (bfd_seek(abfd, (file_ptr)offset, SEEK_SET) != 0)
    return FALSE;
  i->size = bfd_bread(i->data, size, abfd);
  if (i->size != size)
    return FALSE;
  i->mapped = 0;

# if defined(HAVE_MPROTECT) && HAVE_MPROTECT
  if (!writable)
    {
      if (debug_windows)
	fprintf(stderr, "\tmprotect (%p, %ld, PROT_READ)\n", i->data,
                (long)i->size);
      mprotect(i->data, i->size, PROT_READ);
    }
# endif /* HAVE_MPROTECT */

  windowp->data = i->data;
  windowp->size = i->size;
  return TRUE;
}
#endif /* ! HAVE_MMAP */

/*
FUNCTION
	bfd_get_file_window

SYNOPSIS
	bfd_boolean bfd_get_free_window(bfd *abfd, ufile_ptr offset,
					bfd_size_type size, bfd_window *windowp,
	  				bfd_boolean writable);

DESCRIPTION
	TODO: write description

*/
bfd_boolean
bfd_get_file_window(bfd *abfd, ufile_ptr offset, bfd_size_type size,
                    bfd_window *windowp, bfd_boolean writable)
{
  bfd_window_internal *i = windowp->i;

  if (debug_windows)
    fprintf(stderr,
            "bfd_get_file_window (%p, %6ld, %6ld, %p<%p,%lx,%p>, %d)",
            (void *)abfd, (long)offset, (long)size, (void *)windowp,
            windowp->data, (unsigned long)windowp->size,
            (void *)windowp->i, writable);

  BFD_ASSERT(i == NULL);

  if (i == NULL)
    {
      windowp->i = i = ((bfd_window_internal *)
                        bfd_zmalloc((bfd_size_type)sizeof(bfd_window_internal)));
      if (i == 0)
	return FALSE;
      i->data = 0;
    }

  if ((abfd->flags & BFD_IN_MEMORY) != 0)
    {
      struct bfd_in_memory *bim = (struct bfd_in_memory *)abfd->iostream;
      BFD_ASSERT(bim != NULL);

      if ((offset > bim->size) || ((bim->size - offset) < size))
	{
	  bfd_set_error(bfd_error_file_truncated);
	  return FALSE;
	}

      i->next = NULL;
      i->data = (bim->buffer + offset);
      i->size = size;
      i->refcount = 1;
      i->mapped = 2;

      windowp->data = i->data;
      windowp->size = i->size;

      return TRUE;
  } else {
#if defined(HAVE_MMAP) && HAVE_MMAP
    if (!_bfd_get_file_window_mmap(abfd, offset, size, windowp, i, writable)) {
      return FALSE;
    }
#else
    if (!_bfd_get_file_window_malloc(abfd, offset, size, windowp, i, writable)) {
      return FALSE;
    }
#endif /* HAVE_MMAP */
  }

  return TRUE;
}

/* End of bfdwin.c */
