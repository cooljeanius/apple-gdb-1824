/* common/gdb_string.h: Portable <string.h>

   Copyright (C) 1995-2013 Free Software Foundation, Inc.

   This file is part of GDB.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#if !defined(GDB_STRING_H)
#define GDB_STRING_H

#ifdef STDC_HEADERS
# include <string.h>
# ifdef HAVE_STRINGS_H
#  include <strings.h> /* strcasecmp etc..  */
# endif /* HAVE_STRINGS_H */
#else
# ifdef HAVE_STRING_H
#  include <string.h>
# else
#  include <strings.h>
# endif /* HAVE_STRING_H */

# ifndef strchr
extern char *strchr(const char *, int);   /* X3.159-1989  4.11.5.2 */
# endif /* !strchr */

# ifndef strrchr
extern char *strrchr(const char *, int);	/* X3.159-1989  4.11.5.5 */
# endif /* !strrchr */

# ifndef strtok
extern char *strtok(char *, const char *);	/* X3.159-1989  4.11.5.8 */
# endif /* !strtok */

# ifdef HAVE_MEMORY_H
#  include <memory.h>
# else
extern void *memset();
extern void *memcpy();
extern void *memmove();
extern int memcmp();
# endif /* HAVE_MEMORY_H */
#endif /* STDC_HEADERS */

#if !HAVE_DECL_STRERROR
# if !defined(DEFS_H_DOING_POISONING) && \
     !defined(DEFS_H_POISONING_STRERROR_TOO) && !defined(strerror)
extern char *strerror(int);	/* X3.159-1989  4.11.6.2 */
# endif /* !DEFS_H_DOING_POISONING && !DEFS_H_POISONING_STRERROR_TOO && !strerror */
#endif /* !HAVE_DECL_STRERROR */

#if !HAVE_DECL_STRSTR
# ifndef strstr
extern char *strstr(const char *, const char *); /* X3.159-1989  4.11.5.7 */
# endif /* !strstr */
#endif /* !HAVE_DECL_STRERROR */

#endif /* !defined(GDB_STRING_H) */
