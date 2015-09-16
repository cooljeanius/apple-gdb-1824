/* tm-aarch64.h
 * Definitions to target GDB to AArch64 targets.
 * Based off of the ARM equivalent.
 * Copyright 1986-1989, 1991, 1993-2002 Free Software Foundation, Inc.
 *
 * This file is part of GDB.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.  */

#ifndef TM_AARCH64_H
#define TM_AARCH64_H

/* Specify that for the native compiler variables for a particular
 * lexical context are listed after the beginning LBRAC instead of
 * before in the executables list of symbols.
 */
#define VARIABLES_INSIDE_BLOCK(desc, gcc_p) (!(gcc_p))

#endif /* TM_AARCH64_H */

/* EOF */
