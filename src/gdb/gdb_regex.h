/* Portable <regex.h>.
   Copyright 2000, 2001, 2003 Free Software Foundation, Inc.

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

#ifndef GDB_REGEX_H
#define GDB_REGEX_H 1

#ifdef USE_INCLUDED_REGEX
# undef USE_INCLUDED_REGEX
#endif /* USE_INCLUDED_REGEX */
#if !defined(__APPLE__) || (defined(GDB_RC_VERSION) && (GDB_RC_VERSION <= 1705))
# define USE_INCLUDED_REGEX 1
#endif /* !__APPLE__ || (GDB_RC_VERSION <= 1705) */

#if defined(USE_INCLUDED_REGEX) && USE_INCLUDED_REGEX
# include "xregex.h"
#else
/* Request 4.2 BSD regex functions: */
# ifndef _REGEX_RE_COMP
#  define _REGEX_RE_COMP
# endif /* !_REGEX_RE_COMP */
# include <regex.h>
#endif

#if !defined(_REGEX_H) && !defined(_REGEX_H_) && !defined(__REGEX_H_)
# include <regex.h>
#endif /* !_REGEX_H && !_REGEX_H_ && !__REGEX_H_ */

#endif /* not GDB_REGEX_H */
