/* mvalloc.c: Allocate memory on a page boundary.
 * Copyright (C) 1991 Free Software Foundation, Inc.  */
/*
The GNU C Library is free software; you can redistribute it and/or
modify it under the terms of the GNU Library General Public License as
published by the Free Software Foundation; either version 2 of the
License, or (at your option) any later version.

The GNU C Library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Library General Public License for more details.

You should have received a copy of the GNU Library General Public
License along with the GNU C Library; see the file COPYING.LIB.  If
not, write to the Free Software Foundation, Inc., 59 Temple Pl., Suite 330,
Boston, MA 02111-1307, USA.  */

#include "mmprivate.h"

#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif /* HAVE_UNISTD_H */

/* Cache the pagesize for the current host machine.  Note that if the host
 * does not readily provide a getpagesize() function, we need to emulate it
 * elsewhere, not clutter up this file with lots of kluges to try to figure
 * it out. */
static size_t cache_pagesize;
#if defined(NEED_DECLARATION_GETPAGESIZE) && NEED_DECLARATION_GETPAGESIZE
extern int getpagesize PARAMS((void));
#endif /* NEED_DECLARATION_GETPAGESIZE */

PTR
mvalloc(PTR md, size_t size)
{
  if (cache_pagesize == (size_t)0UL)
    {
      cache_pagesize = (size_t)getpagesize();
    }

  return (mmemalign(md, cache_pagesize, size));
}


#if defined(REPLACE_SYSMALLOC) && REPLACE_SYSMALLOC
/* When using this package, provide a version of malloc/realloc/free built
 * on top of it, so that if we use the default sbrk() region we will not
 * collide with another malloc package trying to do the same thing, if
 * the application contains any "hidden" calls to malloc/realloc/free (such
 * as inside a system library). */
PTR
valloc(size_t size)
{
  return mvalloc((PTR)NULL, size);
}
#endif /* REPLACE_SYSMALLOC */

/* EOF */
