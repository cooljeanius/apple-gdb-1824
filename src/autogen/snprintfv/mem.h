/*  -*- Mode: C -*-  */

/* mem.h --- memory handling macros
 * Copyright (C) 1999 Gary V. Vaughan
 * Originally by Gary V. Vaughan, 1999
 * This file is part of Snprintfv
 *
 * Snprintfv is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * Snprintfv program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * As a special exception to the GNU General Public License, if you
 * distribute this file as part of a program that also links with and
 * uses the libopts library from AutoGen, you may include it under
 * the same distribution terms used by the libopts library.
 */

/* Code: */

#ifndef SNPRINTFV_MEM_H
#define SNPRINTFV_MEM_H 1

#include <snprintfv/compat.h>

#ifdef WITH_DMALLOC
#  include <dmalloc.h>
#endif

#ifdef _WIN32
#  ifdef DLL_EXPORT
#    define SNV_SCOPE	extern __declspec(dllexport)
#  else
#    ifdef LIBSNPRINTFV_DLL_IMPORT
#      define SNV_SCOPE	extern __declspec(dllimport)
#    endif
#  endif
#else
#  define SNV_SCOPE	extern
#endif

/* This is the API we use throughout libsnprintfv. */
#define snv_new(type, count)		\
		((type*)snv_malloc((size_t)sizeof(type) * (count)))
#define snv_renew(type, ptr, count)	\
    		((type*)snv_xrealloc((ptr), (size_t)sizeof(type) * (count)))
#define snv_delete(old)	snv_free(old)

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef void* (malloc_proc_t )(size_t);
typedef void* (realloc_proc_t)(snv_pointer, size_t);
typedef void* (free_proc_t   )(snv_pointer);

/* These function pointers are exposed through the API incase a user
   of this library needs to map our memory management routines to
   their own (e.g. xmalloc). */

/**
 * snv_malloc:
 * @count: The number of bytes to allocate.
 *
 * Allocates a fresh block of memory whose size is @count bytes.
 *
 * Return value:
 * The pointer to the newly-allocated memory area.
 */
SNV_SCOPE malloc_proc_t *snv_malloc;

/**
 * snv_realloc:
 * @old: The pointer to the block whose size must be changed.
 * @count: The number of bytes to allocate.
 *
 * Reallocates a fresh block of memory pointed to by @old
 * so that its size becomes @count bytes.
 *
 * Return value:
 * The pointer to the newly-allocated memory area, possibly
 * the same as @old.
 */
SNV_SCOPE realloc_proc_t *snv_realloc;

/**
 * snv_free:
 * @old: The pointer to the block that must freed.
 *
 * Frees a block of memory pointed to by @old.
 */
SNV_SCOPE free_proc_t *snv_free;

/* And these are reimplemented tout court because they are
   not fully portable.  */
extern realloc_proc_t snv_xrealloc;
extern char* snv_strdup (const char *str);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* SNPRINTFV_MEM_H */

/*
 * Local Variables:
 * mode: C
 * c-file-style: "gnu"
 * indent-tabs-mode: nil
 * End:
 * end of snprintfv/mem.h */
