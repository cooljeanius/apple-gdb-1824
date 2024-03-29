/* dyn-string.c: An abstract string datatype.
   Copyright (C) 1998 Free Software Foundation, Inc.
   Contributed by Mark Mitchell (mark@markmitchell.com).

   This file is part of GNU CC.
   
   GNU CC is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   GNU CC is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with GNU CC; see the file COPYING.  If not, write to the Free
   Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.  */

/* This file lives in at least two places: binutils and gcc.
   Don't change one without the other.  */

#include "config.h"
#ifdef IN_GCC
# include "system.h"
# include "gansidecl.h"
#else
# include "ansidecl.h"
#endif /* IN_GCC */
#include "dyn-string_old.h"

#if defined(HAVE_STDLIB_H) || defined(STDC_HEADERS) || defined(__STDC__)
# include <stdlib.h>
#endif /* HAVE_STDLIB_H || STDC_HEADERS || __STDC__ */
#if defined(HAVE_STRING_H) || defined(STDC_HEADERS) || defined(__STDC__)
# include <string.h>
#endif /* HAVE_STRING_H || STDC_HEADERS || __STDC__ */

extern char *xmalloc(size_t);
extern char *xrealloc(void *, size_t);

/* Create a new dynamic string capable of holding at least SPACE
   characters, including the terminating NUL.  If SPACE is 0, it
   will be silently increased to 1.  */
dyn_string_t 
dyn_string_new(int space)
{
  dyn_string_t result = (dyn_string_t)xmalloc(sizeof(struct dyn_string));
 
  if (space == 0)
    /* We need at least one byte in which to store the terminating
       NUL.  */
    space = 1;

  result->allocated = space;
  result->s = (char *)xmalloc(space);
  result->length = 0;
  result->s[0] = '\0';

  return result;
}

/* Free the memory used by DS.  */
void 
dyn_string_delete(dyn_string_t ds)
{
  free(ds->s);
  free(ds);
}

/* Append the NUL-terminated string S to DS, resizing DS if
   necessary.  */
dyn_string_t 
dyn_string_append(dyn_string_t ds, char *s)
{
  size_t len = strlen(s);
  dyn_string_resize(ds, ds->length + len + 1 /* '\0' */);
  strcpy((ds->s + ds->length), s);
  ds->length += len;

  return ds;
}

/* Increase the capacity of DS so that it can hold at least SPACE
   characters, including the terminating NUL.  This function will not
   (at present) reduce the capacity of DS.  */
dyn_string_t 
dyn_string_resize(dyn_string_t ds, int space)
{
  int new_allocated = ds->allocated;

  while (space > new_allocated)
    new_allocated *= 2;
    
  if (new_allocated != ds->allocated)
    {
      /* We actually need more space: */
      ds->allocated = new_allocated;
      ds->s = (char *)xrealloc(ds->s, ds->allocated);
    }

  return ds;
}

/* EOF */
