/* macosx-bundle-utils.h: Work with Mac OS X bundles, for GDB.

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

#ifndef MACOSX_BUNDLE_UTILS_H
#define MACOSX_BUNDLE_UTILS_H

extern const void *macosx_parse_plist(const char *path);
extern const char *macosx_get_plist_posix_value(const void *plist,
                                                const char* key);
extern void macosx_free_plist(const void **plist);
extern char *macosx_filename_in_bundle(const char *filename, int mainline);

#endif /* !MACOSX_BUNDLE_UTILS_H */

/* EOF */
