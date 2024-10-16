/* loadmsgcat.c
   Load needed message catalogs.
   Copyright (C) 1995, 1996, 1997, 1998 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 51 Franklin Street - Fifth Floor, Boston, MA 02110-1301, USA.  */

#ifdef HAVE_CONFIG_H
# include "config.h"
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning loadmsgcat.c expects "config.h" to be included.
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_CONFIG_H */

#if !defined(__has_include)
# define __has_include(foo) 0
#endif /* !__has_include */

#if defined(HAVE_FCNTL_H) || __has_include(<fcntl.h>)
# include <fcntl.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "loadmsgcat.c expects <fcntl.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_FCNTL_H */

#if defined(HAVE_SYS_TYPES_H) || __has_include(<sys/types.h>)
# include <sys/types.h>
#else
# if defined(HAVE_GCC_SYS_TYPES_H) || __has_include(<gcc/sys-types.h>)
#  include <gcc/sys-types.h>
# else
#  if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#   warning "loadmsgcat.c expects <sys/types.h> (or something) to be included."
#  endif /* __GNUC__ && !__STRICT_ANSI__ */
# endif /* HAVE_GCC_SYS_TYPES_H */
#endif /* HAVE_SYS_TYPES_H */

#if defined(HAVE_MALLOC_H) || __has_include(<malloc.h>)
# include <malloc.h>
#else
# if defined(HAVE_MALLOC_MALLOC_H) || __has_include(<malloc/malloc.h>)
#  include <malloc/malloc.h>
# else
#  if defined(HAVE_SYS_MALLOC_H) && defined(HAVE_U_SHORT) && defined(HAVE_U_INT64_T)
#   include <sys/malloc.h>
#  else
#   if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#    warning "loadmsgcat.c expects a malloc-related header to be included."
#   endif /* __GNUC__ && !__STRICT_ANSI__ */
#  endif /* HAVE_SYS_MALLOC_H && HAVE_U_SHORT && HAVE_U_INT64_T */
# endif /* HAVE_MALLOC_MALLOC_H */
#endif /* HAVE_MALLOC_H */

#if defined(HAVE_SYS_STAT_H) || __has_include(<sys/stat.h>)
# include <sys/stat.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "loadmsgcat.c expects <sys/stat.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_SYS_STAT_H */

#if defined(STDC_HEADERS) || defined(_LIBC) || defined(HAVE_STDLIB_H)
# include <stdlib.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "loadmsgcat.c expects <stdlib.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_STDLIB_H || _LIBC || STDC_HEADERS */

#if defined(HAVE_UNISTD_H) || defined(_LIBC) || __has_include(<unistd.h>)
# include <unistd.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "loadmsgcat.c expects <unistd.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_UNISTD_H || _LIBC */

#if (defined(HAVE_MMAP) && defined(HAVE_MUNMAP)) || defined(_LIBC) || \
    defined(HAVE_SYS_MMAN_H) || __has_include(<sys/mman.h>)
# include <sys/mman.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "loadmsgcat.c expects <sys/mman.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* (HAVE_MMAP && HAVE_MUNMAP) || _LIBC || HAVE_SYS_MMAN_H */

#include "gettext.h"
#include "gettextP.h"

/* @@ end of prolog @@ */

#ifdef _LIBC
/* Rename the non ISO C functions. This is required by the standard
   because some ISO C functions will require linking with this object
   file and the name space must not be polluted.  */
# define open   __open
# define close  __close
# define read   __read
# define mmap   __mmap
# define munmap __munmap
#endif /* _LIBC */

/* We need a sign, whether a new catalog was loaded, which can be associated
   with all translations. This is important if the translations are
   cached by one of GCC's features.  */
int _nl_msg_cat_cntr = 0;


/* Load the message catalogs specified by FILENAME. If it is no valid
   message catalog do nothing.  */
void
internal_function
_nl_load_domain(struct loaded_l10nfile *domain_file)
{
  int fd;
  size_t size;
  struct stat st;
  struct mo_file_header *data = (struct mo_file_header *) -1;
#if (defined HAVE_MMAP && defined HAVE_MUNMAP && !defined DISALLOW_MMAP) \
    || defined _LIBC
  int use_mmap = 0;
#endif /* (HAVE_MMAP && HAVE_MUNMAP && !DISALLOW_MMAP) || _LIBC */
  struct loaded_domain *domain;

  domain_file->decided = 1;
  domain_file->data = NULL;

  /* If the record does not represent a valid locale the FILENAME
   * might be NULL. This can happen when according to the given
   * specification the locale file name is different for XPG and CEN
   * syntax: */
  if (domain_file->filename == NULL) {
    return;
  }

  /* Try to open the addressed file: */
  fd = open(domain_file->filename, O_RDONLY);
  if (fd == -1) {
    return;
  }

  /* We must know about the size of the file: */
  if ((fstat(fd, &st) != 0)
      || ((size = (size_t)st.st_size) != (size_t)st.st_size)
      || (size < sizeof(struct mo_file_header))) {
      /* Something went wrong: */
      close(fd);
      return;
  }

#if (defined HAVE_MMAP && defined HAVE_MUNMAP && !defined DISALLOW_MMAP) \
    || defined _LIBC
  /* Now we are ready to load the file. If mmap() is available,
   * then we try this first. If it is not available, or if it failed,
   * then we try to load it: */
  data = (struct mo_file_header *)mmap(NULL, size, PROT_READ,
                                       MAP_PRIVATE, fd, (off_t)0L);

  if (data != (struct mo_file_header *)-1) {
      /* mmap() call was successful: */
      close(fd);
      use_mmap = 1;
  }
#endif /* (HAVE_MMAP && HAVE_MUNMAP && !DISALLOW_MMAP) || _LIBC */

  /* If the data is not yet available (i.e. mmap'ed) we try to load
     it manually.  */
  if (data == (struct mo_file_header *)-1) {
      size_t to_read;
      char *read_ptr;

      data = (struct mo_file_header *)malloc(size);
      if (data == NULL) {
	return;
      }

      to_read = size;
      read_ptr = (char *)data;
      do {
	  long int nb = (long int)read(fd, read_ptr, to_read);
	  if (nb == -1) {
	      close(fd);
	      return;
          }

	  read_ptr += nb;
	  to_read -= (size_t)nb;
      } while (to_read > 0);

      close(fd);
  }

  /* Using the magic number we can test whether it really is a message
     catalog file.  */
  if (data->magic != _MAGIC && data->magic != _MAGIC_SWAPPED)
    {
      /* The magic number is wrong: not a message catalog file: */
#if (defined HAVE_MMAP && defined HAVE_MUNMAP && !defined DISALLOW_MMAP) \
    || defined _LIBC
      if (use_mmap)
	munmap((caddr_t)data, size);
      else
#endif /* (HAVE_MMAP && HAVE_MUNMAP && !DISALLOW_MMAP) || _LIBC */
	free(data);
      return;
    }

  domain_file->data
    = (struct loaded_domain *)malloc(sizeof(struct loaded_domain));
  if (domain_file->data == NULL) {
    return;
  }

  domain = (struct loaded_domain *)domain_file->data;
  domain->data = (char *)data;
#if (defined HAVE_MMAP && defined HAVE_MUNMAP && !defined DISALLOW_MMAP) \
    || defined _LIBC
  domain->use_mmap = use_mmap;
#endif /* (HAVE_MMAP && HAVE_MUNMAP && !DISALLOW_MMAP) || _LIBC */
  domain->mmap_size = size;
  domain->must_swap = data->magic != _MAGIC;

  /* Fill in the information about the available tables.  */
  switch (W(domain->must_swap, data->revision))
    {
    case 0:
      domain->nstrings = W(domain->must_swap, data->nstrings);
      domain->orig_tab = (struct string_desc *)
	((char *)data + W(domain->must_swap, data->orig_tab_offset));
      domain->trans_tab = (struct string_desc *)
	((char *)data + W(domain->must_swap, data->trans_tab_offset));
      domain->hash_size = W(domain->must_swap, data->hash_tab_size);
      domain->hash_tab = (nls_uint32 *)
	((char *)data + W(domain->must_swap, data->hash_tab_offset));
      break;
    default:
      /* This is an illegal revision: */
#if (defined HAVE_MMAP && defined HAVE_MUNMAP && !defined DISALLOW_MMAP) \
    || defined _LIBC
      if (use_mmap)
	munmap((caddr_t)data, size);
      else
#endif /* (HAVE_MMAP && HAVE_MUNMAP && !DISALLOW_MMAP) || _LIBC */
	free(data);
      free(domain);
      domain_file->data = NULL;
      return;
    }

  /* Show that one domain is changed. This might make some cached
     translations invalid.  */
  ++_nl_msg_cat_cntr;
}


#ifdef _LIBC
void
internal_function
_nl_unload_domain(struct loaded_domain *domain)
{
  if (domain->use_mmap) {
    munmap((caddr_t)domain->data, domain->mmap_size);
  } else {
    free((void *)domain->data);
  }

  free(domain);
}
#endif /* _LIBC */

/* EOF */
