/* xmalloca.c: Safe automatic memory allocation w/OOM checking.
   Copyright (C) 2003, 2006-2007, 2009-2012 Free Software Foundation, Inc.
   Written by Bruno Haible <bruno@clisp.org>, 2003.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>  */

#include <config.h>

#if 0
/* Specification: */
# include "xmalloca.h"
#else
# if defined(PROTOTYPES) || defined(__PROTOTYPES) || defined(__STDC__)
#  if defined(HAVE_ALLOCA) && HAVE_ALLOCA
extern void *xmmalloca(size_t);
#  endif /* HAVE_ALLOCA */
# endif /* PROTOTYPES || __PROTOTYPES || __STDC__ */
#endif /* 0 || 1 */

#if 0
# include "xalloc.h"
#else
# include "malloca.h"
# ifndef XALLOC_H_
#  if defined(PROTOTYPES) || defined(__PROTOTYPES) || defined(__STDC__)
#   if defined(HAVE_ALLOCA) && HAVE_ALLOCA
extern void xalloc_die(void);
#   endif /* HAVE_ALLOCA */
#  endif /* PROTOTYPES || __PROTOTYPES || __STDC__ */
# endif /* !XALLOC_H_ */
#endif /* 0 */

#if defined(HAVE_ALLOCA) && HAVE_ALLOCA
void *
xmmalloca(size_t n)
{
  void *p;

  p = mmalloca(n);
  if (p == NULL) {
    xalloc_die();
  }
  return p;
}
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__) && defined(XALLOC_H_)
#  warning "Useless..."
# endif /* __GNUC__ && !__STRICT_ANSI__ && XALLOC_H_ */
#endif /* HAVE_ALLOCA */

/* EOF */
