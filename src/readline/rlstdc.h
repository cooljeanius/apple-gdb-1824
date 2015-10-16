/* [rl]stdc.h -- macros to make source compile on both ANSI C and K&R C
 * compilers. */

/* Copyright (C) 1993 Free Software Foundation, Inc.

   This file is part of GNU Bash, the Bourne Again SHell.

   Bash is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   Bash is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.

   You should have received a copy of the GNU General Public License
   along with Bash; see the file COPYING.  If not, write to the Free
   Software Foundation, 59 Temple Place, Suite 330, Boston, MA 02111 USA. */

#if !defined(_RL_STDC_H_)
#define _RL_STDC_H_

/* Adapted from BSD /usr/include/sys/cdefs.h. */

#if defined(__GNUC__) && defined(__GNUC_MINOR__) && !defined(__cplusplus)
# if (__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 6))
 #  pragma GCC diagnostic push
 #  pragma GCC diagnostic warning "-Wtraditional"
# endif /* gcc 4.6+ */
#endif /* GCC (non-c++) */

/* A function can be defined using prototypes and compile on both ANSI C
 * and traditional C compilers with something like this:
 *	extern char *func PARAMS((char *, char *, int));
 */

#if !defined(PARAMS)
#  if defined(__STDC__) || defined(__GNUC__) || defined(__cplusplus)
#    define PARAMS(protos) protos
#  else
#    define PARAMS(protos) ()
#  endif /* __STDC__ || __GNUC__ || __cplusplus */
#endif /* !PARAMS */

#ifndef __attribute__
#  if (defined(__GNUC__) && (__GNUC__ < 2)) || \
      ((defined(__GNUC__) && (__GNUC__ == 2)) && (defined(__GNUC_MINOR__) && (__GNUC_MINOR__ < 8))) || \
      (defined(__STRICT_ANSI__) && __STRICT_ANSI__)
#    define __attribute__(x) /* (nothing) */
#  endif /* old __GNUC__ || __STRICT_ANSI__ */
#endif /* !__attribute__ */

/* keep condition the same as where we push: */
#if defined(__GNUC__) && defined(__GNUC_MINOR__) && !defined(__cplusplus)
# if (__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 6))
 #  pragma GCC diagnostic pop
# endif /* gcc 4.6+ */
#endif /* GCC (non-c++) */

/* in case the popping failed: */
#if defined(__GNUC__) && (__GNUC__ >= 4) && !defined(__clang__) && \
    !defined(__cplusplus)
 # pragma GCC diagnostic ignored "-Wtraditional"
#endif /* gcc 4+ && !__clang__ && !__cplusplus */

#endif /* !_RL_STDC_H_ */

/* EOF */
