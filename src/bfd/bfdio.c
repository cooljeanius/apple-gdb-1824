/* bfdio.c: Low-level I/O routines for BFDs.
 *
 * Copyright 1990, 1991, 1992, 1993, 1994, 1995, 1996, 1997, 1998,
 * 1999, 2000, 2001, 2002, 2003, 2004, 2005
 * Free Software Foundation, Inc.
 *
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

#include "sysdep.h"

#include "bfd.h"
#include "libbfd.h"

#include <limits.h>

#ifndef S_IXUSR
# define S_IXUSR 0100    /* Execute by owner.  */
#endif /* !S_IXUSR */
#ifndef S_IXGRP
# define S_IXGRP 0010    /* Execute by group.  */
#endif /* !S_IXGRP */
#ifndef S_IXOTH
# define S_IXOTH 0001    /* Execute by others.  */
#endif /* !S_IXOTH */

/* */
file_ptr
real_ftell(FILE *file)
{
#if defined(HAVE_FTELLO64)
  return ftello64(file);
#elif defined(HAVE_FTELLO)
  return (file_ptr)ftello(file);
#else
  return ftell(file);
#endif /* HAVE_FTELLO64 || HAVE_FTELLO || HAVE_FTELL */
}

/* */
int
real_fseek(FILE *file, file_ptr offset, int whence)
{
#if defined(HAVE_FSEEKO64)
  return fseeko64(file, offset, whence);
#elif defined(HAVE_FSEEKO)
  return fseeko(file, (off_t)offset, whence);
#else
  return fseek(file, offset, whence);
#endif /* HAVE_FSEEKO64 || HAVE_FSEEKO || HAVE_FSEEK */
}

/*
INTERNAL_DEFINITION
	struct bfd_iovec

DESCRIPTION

	The <<struct bfd_iovec>> contains the internal file I/O class.
	Each <<BFD>> has an instance of this class and all file I/O is
	routed through it (it is assumed that the instance implements
	all methods listed below).

.struct bfd_iovec
.{
.  {* To avoid problems with macros, a "b" rather than "f"
.     prefix is prepended to each method name.  *}
.  {* Attempt to read/write NBYTES on ABFD's IOSTREAM storing/fetching
.     bytes starting at PTR.  Return the number of bytes actually
.     transfered (a read past end-of-file returns less than NBYTES),
.     or -1 (setting <<bfd_error>>) if an error occurs.  *}
.  file_ptr (*bread) (struct bfd *abfd, void *ptr, file_ptr nbytes);
.  file_ptr (*bwrite) (struct bfd *abfd, const void *ptr,
.                      file_ptr nbytes);
.  {* Return the current IOSTREAM file offset, or -1 (setting <<bfd_error>>
.     if an error occurs.  *}
.  file_ptr (*btell) (struct bfd *abfd);
.  {* For the following, on successful completion a value of 0 is returned.
.     Otherwise, a value of -1 is returned (and  <<bfd_error>> is set).  *}
.  int (*bseek) (struct bfd *abfd, file_ptr offset, int whence);
.  int (*bclose) (struct bfd *abfd);
.  int (*bflush) (struct bfd *abfd);
.  int (*bstat) (struct bfd *abfd, struct stat *sb);
.};

*/


/* Return value is amount read: */
bfd_size_type
bfd_bread(void *ptr, bfd_size_type size, bfd *abfd)
{
  size_t nread;

  while (abfd->my_archive != NULL)
    abfd = abfd->my_archive;

  if ((abfd->flags & BFD_IN_MEMORY) != 0)
    {
      struct bfd_in_memory *bim;
      bfd_size_type get;

      bim = (struct bfd_in_memory *)abfd->iostream;
      get = size;
      if ((abfd->where + get) > bim->size)
        {
          if (bim->size < (bfd_size_type)abfd->where)
            get = 0UL;
          else
            get = (bfd_size_type)(bim->size - abfd->where);
          bfd_set_error(bfd_error_file_truncated);
        }
      memcpy(ptr, (bim->buffer + abfd->where), (size_t)get);
      abfd->where += get;
      return get;
    }

  if (abfd->iovec)
    nread = (size_t)abfd->iovec->bread(abfd, ptr, (file_ptr)size);
  else
    nread = 0UL;
  if (nread != (size_t)-1L)
    abfd->where += nread;

  return nread;
}

/* */
bfd_size_type
bfd_bwrite(const void *ptr, bfd_size_type size, bfd *abfd)
{
  size_t nwrote;

  while (abfd->my_archive != NULL)
    abfd = abfd->my_archive;

  if (abfd->flags & BFD_IN_MEMORY)
    {
      struct bfd_in_memory *bim = (struct bfd_in_memory *)abfd->iostream;

      size = (size_t)size;
      if ((abfd->where + size) > bim->size)
	{
	  bfd_size_type newsize, oldsize;

	  oldsize = ((bim->size + 127UL) & ~(bfd_size_type)127L);
	  bim->size = (bfd_size_type)(abfd->where + size);
	  /* Round up to cut down on memory fragmentation: */
	  newsize = ((bim->size + 127UL) & ~(bfd_size_type)127L);
	  if (newsize > oldsize)
	    {
	      bim->buffer = (bfd_byte *)bfd_realloc(bim->buffer, newsize);
	      if (bim->buffer == 0)
		{
		  bim->size = 0;
		  return 0;
		}
	    }
	}
      memcpy((bim->buffer + abfd->where), ptr, (size_t)size);
      abfd->where += size;
      return size;
    }

  if (abfd->iovec)
    nwrote = (size_t)abfd->iovec->bwrite(abfd, ptr, (file_ptr)size);
  else
    nwrote = 0UL;

  if (nwrote != (size_t)-1L)
    abfd->where += nwrote;
  if (nwrote != size)
    {
#ifdef ENOSPC
      errno = ENOSPC;
#endif /* ENOSPC */
      bfd_set_error(bfd_error_system_call);
    }
  return nwrote;
}

/* */
file_ptr
bfd_tell(bfd *abfd)
{
  file_ptr ptr = 0L;
  bfd *cur = abfd;

  while (cur->my_archive) {
    ptr -= (file_ptr)cur->origin;
    cur = cur->my_archive;
  }

  if ((cur->flags & BFD_IN_MEMORY) != 0)
    {
      ptr += (file_ptr)abfd->where;
    }
  else if (cur->iovec)
    {
      cur->where = (ufile_ptr)abfd->iovec->btell(abfd);
      ptr += (file_ptr)cur->where;
    }
  else
    ptr += 0L;

  return ptr;
}

/* */
int
bfd_flush(bfd *abfd)
{
  while (abfd->my_archive)
    abfd = abfd->my_archive;

  if ((abfd->flags & BFD_IN_MEMORY) != 0)
    {
      return 0;
    }
  else if (abfd->iovec)
    return abfd->iovec->bflush (abfd);
  else
    return 0;
}

/* Returns 0 for success, negative value for failure (in which case
   bfd_get_error can retrieve the error code).  */
int
bfd_stat(bfd *abfd, struct stat *statbuf)
{
  int result;

  while (abfd->my_archive)
    abfd = abfd->my_archive;

  if ((abfd->flags & BFD_IN_MEMORY) != 0)
    {
      struct bfd_in_memory *b = (struct bfd_in_memory *)abfd->iostream;
      memset(statbuf, 0, sizeof(struct stat));
      statbuf->st_size = (off_t)b->size;
      return 0;
    }
  else if (abfd->iovec)
    result = abfd->iovec->bstat(abfd, statbuf);
  else
    result = -1;

  if (result < 0)
    bfd_set_error(bfd_error_system_call);
  return result;
}

/* Returns 0 for success, nonzero for failure (in which case bfd_get_error
   can retrieve the error code).  */
int
bfd_seek(bfd *abfd, file_ptr position, int direction)
{
  int result;
  /* For the time being, a BFD may not seek to its end.  The problem
     is that we do NOT easily have a way to recognize the end of an
     element in an archive.  */

  BFD_ASSERT((direction == SEEK_SET) || (direction == SEEK_CUR));

  if ((direction == SEEK_CUR) && (position == 0))
    return 0;

  while (abfd->my_archive != NULL)
    {
      if (direction == SEEK_SET)
	position += (file_ptr)abfd->origin;
      abfd = abfd->my_archive;
    }

  if ((abfd->flags & BFD_IN_MEMORY) != 0)
    {
      struct bfd_in_memory *bim;

      bim = (struct bfd_in_memory *)abfd->iostream;

      if (direction == SEEK_SET)
	abfd->where = (ufile_ptr)position;
      else
	abfd->where += (ufile_ptr)position;

      if (abfd->where > bim->size)
	{
	  if ((abfd->direction == write_direction)
              || (abfd->direction == both_direction))
	    {
	      bfd_size_type newsize, oldsize;

	      oldsize = ((bim->size + 127) & ~(bfd_size_type)127);
	      bim->size = (bfd_size_type)abfd->where;
	      /* Round up to cut down on memory fragmentation: */
	      newsize = ((bim->size + 127) & ~(bfd_size_type)127);
	      if (newsize > oldsize)
	        {
		  bim->buffer = (bfd_byte *)bfd_realloc(bim->buffer,
                                                        newsize);
		  if (bim->buffer == 0)
		    {
		      bim->size = 0;
		      return -1;
		    }
	        }
	    }
	  else
	    {
	      abfd->where = bim->size;
	      bfd_set_error(bfd_error_file_truncated);
	      return -1;
	    }
	}
      return 0;
    }

  if ((abfd->format != bfd_archive) && (abfd->my_archive == 0))
    {
      if ((direction == SEEK_SET) && ((bfd_vma)position == abfd->where))
	return 0;
    }
  else
    {
      /* We need something smarter to optimize access to archives.
       * Currently, anything inside an archive is read via the file handle
       * for the archive.  Which means that a bfd_seek on one component
       * affects the `current position' in the archive, as well as in any
       * other component.
       *
       * It might be sufficient to put a spike through the cache
       * abstraction, and look to the archive for the file position, but I
       * think we should try for something cleaner.
       *
       * In the meantime, no optimization for archives, so do nothing: */
      ;
    }

  if (abfd->iovec)
    result = abfd->iovec->bseek(abfd, position, direction);
  else
    result = -1;

  if (result != 0)
    {
      int hold_errno = errno;

      /* Force redetermination of `where' field: */
      bfd_tell(abfd);

      /* An EINVAL error probably means that the file offset was
         absurd.  */
      if (hold_errno == EINVAL)
	bfd_set_error(bfd_error_file_truncated);
      else
	{
	  bfd_set_error(bfd_error_system_call);
	  errno = hold_errno;
	}
    }
  else
    {
      /* Adjust `where' field: */
      if (direction == SEEK_SET)
	abfd->where = (ufile_ptr)position;
      else
	abfd->where += (ufile_ptr)position;
    }
  return result;
}

/*
FUNCTION
	bfd_get_mtime

SYNOPSIS
	long bfd_get_mtime(bfd *abfd);

DESCRIPTION
	Return the file modification time (as read from the file system, or
	from the archive header for archive members).

*/
long
bfd_get_mtime(bfd *abfd)
{
  struct stat buf;

  if (abfd->mtime_set)
    return abfd->mtime;

  if (abfd->iovec == NULL)
    return 0L;

  if (abfd->iovec->bstat(abfd, &buf) != 0)
    return 0L;

  abfd->mtime = buf.st_mtime;	 /* Save value in case anyone wants it */
  return buf.st_mtime;
}

/*
FUNCTION
	bfd_get_size

SYNOPSIS
	long bfd_get_size(bfd *abfd);

DESCRIPTION
	Return the file size (as read from file system) for the file
	associated with BFD @var{abfd}.

	The initial motivation for, and use of, this routine is not
	so we can get the exact size of the object the BFD applies to, since
	that might not be generally possible (archive members for example).
	It would be ideal if someone could eventually modify
	it so that such results were guaranteed.

	Instead, we want to ask questions like "is this NNN byte sized
	object I am about to try read from file offset YYY reasonable?"
	As as example of where we might do this, some object formats
	use string tables for which the first <<sizeof (long)>> bytes of the
	table contain the size of the table itself, including the size bytes.
	If an application tries to read what it thinks is one of these
	string tables, without some way to validate the size, and for
	some reason the size is wrong (byte swapping error, wrong location
	for the string table, etc.), the only clue is likely to be a read
	error when it tries to read the table, or a "virtual memory
	exhausted" error when it tries to allocate 15 bazillon bytes
	of space for the 15 bazillon byte table it is about to read.
	This function at least allows us to answer the question, "is the
	size reasonable?".
*/
long
bfd_get_size(bfd *abfd)
{
  struct stat buf;

  if ((abfd->flags & BFD_IN_MEMORY) != 0)
    return (long)((struct bfd_in_memory *)abfd->iostream)->size;

  if (abfd->iovec == NULL)
    return 0L;

  if (abfd->iovec->bstat(abfd, &buf) != 0)
    return 0L;

  return (long)buf.st_size;
}

/* */
bfd_boolean
_bfd_io_close(bfd *abfd)
{
  if (abfd->flags & BFD_IN_MEMORY)
    {
      int ret = 0;

      struct bfd_in_memory *b = (struct bfd_in_memory *)abfd->iostream;
      BFD_ASSERT(b != NULL);

#if 0
      ret = munmap(b->buffer, b->size);
#endif /* 0 */

      abfd->iostream = NULL;
      BFD_ASSERT(ret == 0);

      return TRUE;
    }
  else
    {
      bfd_boolean ret = TRUE;
      ret = bfd_cache_close(abfd);

      return (ret);
    }
}

/* End of bfdio.c */
