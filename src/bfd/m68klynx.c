/* BFD back-end for m68k binaries under LynxOS.
   Copyright (C) 1990, 1991, 1992 Free Software Foundation, Inc.

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
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  */

#if defined(__GNUC__) && defined(__GNUC_MINOR__)
# if (__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 6))
 #  pragma GCC diagnostic push
 #  pragma GCC diagnostic warning "-Wtraditional"
# endif /* gcc 4.6+ */
#endif /* GCC */

#define BYTES_IN_WORD 4
#define N_SHARED_LIB(x) 0

#define TEXT_START_ADDR 0
#define TARGET_PAGE_SIZE 4096
#define SEGMENT_SIZE TARGET_PAGE_SIZE
#define DEFAULT_ARCH bfd_arch_m68k

/* Do not "beautify" the CONCAT* macro args.  Traditional C will not
 * remove whitespace added here, and thus will fail to concatenate
 * the tokens: */
#define MY(OP) CONCAT2(m68klynx_aout_,OP)
#define TARGETNAME "a.out-m68k-lynx"

/* keep condition the same as where we push: */
#if defined(__GNUC__) && defined(__GNUC_MINOR__)
# if (__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 6))
 #  pragma GCC diagnostic pop
# endif /* gcc 4.6+ */
#endif /* GCC */

#include "bfd.h"
#include "sysdep.h"
#include "libbfd.h"

#include "libaout.h"
#include "aout/aout64.h"

#define TARGET_IS_BIG_ENDIAN_P

#ifdef LYNX_CORE

char *lynx_core_file_failing_command();
int lynx_core_file_failing_signal();
boolean lynx_core_file_matches_executable_p();
const bfd_target *lynx_core_file_p();

#define	MY_core_file_failing_command lynx_core_file_failing_command
#define	MY_core_file_failing_signal lynx_core_file_failing_signal
#define	MY_core_file_matches_executable_p lynx_core_file_matches_executable_p
#define	MY_core_file_p lynx_core_file_p

#endif /* LYNX_CORE */

#include "aout-target.h"
