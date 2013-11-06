/* Portable <sys/stat.h>
   Copyright 1995 Free Software Foundation, Inc.

   This file is part of GDB.

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
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

#if !defined(GDB_STAT_H)
#define GDB_STAT_H

#include <sys/types.h>
#include <sys/stat.h>

#ifdef STAT_MACROS_BROKEN
# undef S_ISBLK
# undef S_ISCHR
# undef S_ISDIR
# undef S_ISREG
# undef S_ISFIFO
# undef S_ISLNK
# undef S_ISSOCK
# undef S_ISMPB
# undef S_ISMPC
# undef S_ISNWK
#endif /* STAT_MACROS_BROKEN */

#if !defined(S_ISBLK) && defined(S_IFBLK)
# define S_ISBLK(m) (((m) & S_IFMT) == S_IFBLK)
#endif /* !S_ISBLK && S_IFBLK */
#if !defined(S_ISCHR) && defined(S_IFCHR)
# define S_ISCHR(m) (((m) & S_IFMT) == S_IFCHR)
#endif /* !S_ISCHR && S_IFCHR */
#if !defined(S_ISDIR) && defined(S_IFDIR)
# define S_ISDIR(m) (((m) & S_IFMT) == S_IFDIR)
#endif /* !S_ISDIR && S_IFDIR */
#if !defined(S_ISREG) && defined(S_IFREG)
# define S_ISREG(m) (((m) & S_IFMT) == S_IFREG)
#endif /* !S_ISREG && S_IFREG */
#if !defined(S_ISFIFO) && defined(S_IFIFO)
# define S_ISFIFO(m) (((m) & S_IFMT) == S_IFIFO)
#endif /* !S_ISFIFO && S_IFIFO */
#if !defined(S_ISLNK) && defined(S_IFLNK)
# define S_ISLNK(m) (((m) & S_IFMT) == S_IFLNK)
#endif /* !S_ISLNK && S_IFLNK */
#if !defined(S_ISSOCK) && defined(S_IFSOCK)
# define S_ISSOCK(m) (((m) & S_IFMT) == S_IFSOCK)
#endif /* !S_ISSOCK && S_IFSOCK */
#if !defined(S_ISMPB) && defined(S_IFMPB)	/* V7 */
# define S_ISMPB(m) (((m) & S_IFMT) == S_IFMPB)
# define S_ISMPC(m) (((m) & S_IFMT) == S_IFMPC)
#endif /* !S_ISMPB && S_IFMPB */
#if !defined(S_ISNWK) && defined(S_IFNWK)	/* HP/UX */
# define S_ISNWK(m) (((m) & S_IFMT) == S_IFNWK)
#endif /* !S_ISNWK && S_IFWNK */

/* Microsoft C's stat.h does NOT define all the POSIX file modes.  */
#ifndef S_IROTH
# define S_IROTH	S_IREAD
#endif /* !S_IROTH */

#endif /* !defined(GDB_STAT_H) */
