/* demo64.c: BFD backend for demonstration 64-bit a.out binaries.
 * Copyright 1990, 1991, 1992, 1993, 1994, 2001
 * Free Software Foundation, Inc.
 * Written by Cygnus Support.  */
/*
This file is part of BFD, the Binary File Descriptor library.

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
Foundation, Inc., 51 Franklin St., 5th Floor, Boston, MA 02110-1301, USA */

#define ARCH_SIZE 64

/* Do not "beautify" the CONCAT* macro args.  Traditional C will not
   remove whitespace added here, and thus will fail to concatenate
   the tokens.  */
#define MY(OP) CONCAT2 (demo_64_,OP)

#define TARGETNAME  "demo64"

/* this needs to go after the usage of the CONCAT* macro mentioned above,
 * but before any other headers are included, or prototypes for functions
 * are declared: */
#if defined(__GNUC__) && (__GNUC__ >= 4) && !defined(__clang__)
 # pragma GCC diagnostic ignored "-Wtraditional"
#endif /* gcc 4+ && !__clang__ */

#include "aoutf1.h"

/* EOF */
