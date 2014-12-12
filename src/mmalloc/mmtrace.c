/* mmtrace.c: More debugging hooks for `mmalloc'.
 * Copyright 1991, 1992, 1994 Free Software Foundation
 *
 * Written April 2, 1991 by John Gilmore of Cygnus Support
 * Based on mcheck.c by Mike Haertel.
 * Modified Mar 1992 by Fred Fish.  <fnf@cygnus.com>  */
/*
This file is part of the GNU C Library.

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

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#if defined(HAVE_STDLIB_H) || defined(STDC_HEADERS) || defined(__STDC__)
# include <stdlib.h>
#endif /* HAVE_STDLIB_H || STDC_HEADERS || __STDC__ */
#include "mmprivate.h"

static void tr_break PARAMS((void));
static void tr_freehook PARAMS((PTR, PTR));
static PTR tr_mallochook PARAMS((PTR, size_t));
static PTR tr_reallochook PARAMS((PTR, PTR, size_t));

#if !defined(__GNU_LIBRARY__) && !defined(_STDLIB_H_) && \
    (!defined(HAVE_DECL_GETENV) || !HAVE_DECL_GETENV)
extern char *getenv();
#endif /* !__GNU_LIBRARY__ && !_STDLIB_H_ && !HAVE_DECL_GETENV */

static FILE *mallstream;

#if 0	/* FIXME:  Disabled for now. */
static char mallenv[] = "MALLOC_TRACE";
static char mallbuf[BUFSIZ];	/* Buffer for the output.  */
#endif /* 0 */

/* Address to breakpoint on accesses to... */
static PTR mallwatch;

/* Old hook values: */
static void (*old_mfree_hook)PARAMS((PTR, PTR));
static PTR (*old_mmalloc_hook)PARAMS((PTR, size_t));
static PTR (*old_mrealloc_hook)PARAMS((PTR, PTR, size_t));

/* This function is called when the block being alloc'd, realloc'd, or
 * freed has an address matching the variable "mallwatch".  In a debugger,
 * set "mallwatch" to the address of interest, then put a breakpoint on
 * tr_break.  */
static void
tr_break(void)
{
  return;
}

static void
tr_freehook(PTR md, PTR ptr)
{
  struct mdesc *mdp;

  mdp = MD_TO_MDP(md);
  /* Be sure to print it first: */
  fprintf (mallstream, "- %08lx\n", (unsigned long) ptr);
  if (ptr == mallwatch)
    tr_break ();
  mdp->mfree_hook = old_mfree_hook;
  mfree (md, ptr);
  mdp->mfree_hook = tr_freehook;
}

static PTR
tr_mallochook(PTR md, size_t size)
{
  PTR hdr;
  struct mdesc *mdp;

  mdp = MD_TO_MDP(md);
  mdp->mmalloc_hook = old_mmalloc_hook;
  hdr = (PTR)mmalloc(md, size);
  mdp->mmalloc_hook = tr_mallochook;

  /* We could be printing a NULL here; that is OK: */
  fprintf (mallstream, "+ 0x%08lx 0x%lx\n",
	   (unsigned long) hdr, (unsigned long) size);

  if (hdr == mallwatch) {
    tr_break();
  }

  return (hdr);
}

static PTR
tr_reallochook(PTR md, PTR ptr, size_t size)
{
  PTR hdr;
  struct mdesc *mdp;

  mdp = MD_TO_MDP(md);

  if (ptr == mallwatch) {
    tr_break();
  }

  mdp->mfree_hook = old_mfree_hook;
  mdp->mmalloc_hook = old_mmalloc_hook;
  mdp->mrealloc_hook = old_mrealloc_hook;
  hdr = (PTR)mrealloc(md, ptr, size);
  mdp->mfree_hook = tr_freehook;
  mdp->mmalloc_hook = tr_mallochook;
  mdp->mrealloc_hook = tr_reallochook;
  if (hdr == NULL) {
    /* Failed realloc: */
    fprintf(mallstream, "! 0x%08lx 0x%lx\n",
            (unsigned long)ptr, (unsigned long)size);
  } else {
    fprintf(mallstream, "< 0x%08lx\n> 0x%08lx 0x%lx\n",
            (unsigned long)ptr, (unsigned long)hdr, (unsigned long)size);
  }

  if (hdr == mallwatch) {
    tr_break();
  }

  return hdr;
}

/* We enable tracing if either the environment variable MALLOC_TRACE
 * is set, or if the variable mallwatch has been patched to an address
 * that the debugging user wants us to stop on.  When patching mallwatch,
 * do NOT forget to set a breakpoint on tr_break!  */
int
mmtrace(void)
{
  /* FIXME!  This is disabled for now until we figure out how to
   * maintain a stack of hooks per heap, since we might have other
   * hooks (such as set by mmcheck/mmcheckf) active also. */
#if defined(WE_FIGURED_IT_OUT) || (defined(__clang_analyzer__) && 0)
  char *mallfile;

# ifdef mallenv
  mallfile = getenv(mallenv);
# else
#  if 1
  mallfile = "/dev/null";
#  else
  mallfile = NULL;
#  endif /* 1 */
# endif /* mallenv */
  if ((mallfile != NULL) || (mallwatch != NULL))
    {
      mallstream = fopen((mallfile != NULL) ? mallfile : "/dev/null", "w");
      if (mallstream != NULL)
	{
# if !defined(mdp)
          struct mdesc *mdp = (struct mdesc *)NULL;
# endif /* !mdp */

# if !defined(__clang_analyzer__) || defined(mallbuf)
	  /* Be sure it does NOT mmalloc its buffer!  */
	  setbuf(mallstream, mallbuf);
# endif /* !__clang_analyzer__ || mallbuf */
	  fprintf(mallstream, "= Start\n");
	  old_mfree_hook = mdp->mfree_hook;
	  mdp->mfree_hook = tr_freehook;
	  old_mmalloc_hook = mdp->mmalloc_hook;
	  mdp->mmalloc_hook = tr_mallochook;
	  old_mrealloc_hook = mdp->mrealloc_hook;
	  mdp->mrealloc_hook = tr_reallochook;
	}
    }
#else
# if defined(__STRICT_ANSI__)
  if ((mallstream != NULL) && 0 && 1)
    {
      struct mdesc *mdp = (struct mdesc *)NULL;

      mdp->mfree_hook = tr_freehook;
      mdp->mmalloc_hook = tr_mallochook;
      mdp->mrealloc_hook = tr_reallochook;
    }
# endif /* !__STRICT_ANSI__ */
#endif /* WE_FIGURED_IT_OUT || (__clang_analyzer__ && 0) */

  return (1);
}

/* EOF */
