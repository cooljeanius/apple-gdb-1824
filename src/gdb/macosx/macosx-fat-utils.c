/* Work with fat "Universal Binaries", for GDB.

 Copyright 1988, 1989, 1990, 1991, 1992, 1993, 1994, 1995, 1996,
 1997, 1998, 1999, 2000, 2001, 2002, 2003 Free Software Foundation,
 Inc.

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

#include "defs.h"
#include "osabi.h"
#include "gdbarch.h"
#include "arch-utils.h"

#include "macosx-fat-utils.h"

/* APPLE LOCAL: This routine opens the slice of a fat file (faking as a
   bfd_archive) that matches the current architecture.  */

bfd *
open_bfd_matching_arch (bfd *archive_bfd, bfd_format expected_format)
{
  enum gdb_osabi osabi;
  bfd *abfd;

  osabi = GDB_OSABI_UNINITIALIZED;
  abfd = NULL;

  osabi = gdbarch_osabi (get_current_arch ());
  if ((osabi <= GDB_OSABI_UNKNOWN) || (osabi >= GDB_OSABI_INVALID))
    osabi = gdbarch_lookup_osabi (archive_bfd);

  /* FIXME: gdbarch_lookup_osabi() does NOT work yet on archive_bfd,
   * so hardcode instead: */
  if ((osabi <= GDB_OSABI_UNKNOWN) || (osabi >= GDB_OSABI_INVALID))
    osabi = GDB_OSABI_DARWIN;

  for (;;)
    {
      abfd = bfd_openr_next_archived_file (archive_bfd, abfd);
      if (abfd == NULL)
        break;
      if (! bfd_check_format (abfd, expected_format))
        continue;
      if (osabi == gdbarch_lookup_osabi_from_bfd (abfd))
        break;
    }

  // Copy the filename of the archive to the binary bfd.
  if (abfd)
  {
    xfree(abfd->filename);
    abfd->filename = archive_bfd->filename;
  }

  return abfd;
}
