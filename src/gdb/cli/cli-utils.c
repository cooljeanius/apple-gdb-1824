/* cli/cli-utils.c: GDB CLI utility library.
   Copyright 2001 Free Software Foundation, Inc.

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
#include "cli/cli-utils.h"

#if defined(__sun) || defined(__GNUC__)
/* This declaration ensures that this object will export at least 1 symbol.  */
int cli_utils_c_dummy_symbol;
#else
/* This declaration is solely to ensure that after preprocessing
 * this file is never empty.  */
typedef int cli_utils_c_dummy_t;
#endif /* __sun || __GNUC__ */

/* EOF */
