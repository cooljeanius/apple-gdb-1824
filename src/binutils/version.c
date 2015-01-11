/* version.c -- binutils version information
   Copyright 1991, 1996-2005 Free Software Foundation, Inc.

   This file is part of GNU Binutils.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, 51 Franklin Street - Fifth Floor, Boston, MA 02110-1301, USA.  */

#ifdef HAVE_CONFIG_H
# include "config.h"
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning version.c expects "config.h" to be included.
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_CONFIG_H */

#ifdef HAVE_STDIO_H
# include <stdio.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "version.c expects <stdio.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_STDIO_H */
#include "bfd.h"
#include "bfdver.h"
#include "bucomm.h"

/* Print the version number and copyright information, and exit.  This
 * implements the --version option for the various programs.  */
void ATTRIBUTE_NORETURN
print_version(const char *name)
{
  /* This output is intended to follow the GNU standards document.  */
  /* xgettext:c-format */
  printf("GNU %s %s\n", name, BFD_VERSION_STRING);
  printf(_("Copyright 2005 Free Software Foundation, Inc.\n"));
  printf(_("\
This program is free software; you may redistribute it under the terms of\n\
the GNU General Public License.  This program has absolutely no warranty.\n"));
  exit(0);
}

/* EOF */
