/* macosx-nat-inferior-stubs.c: Mac OS X support for GDB, the GNU debugger.
   Copyright 1997, 1998, 1999, 2000, 2001, 2002
   Free Software Foundation, Inc.

   Contributed by Apple Computer, Inc.

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
#include "target.h"
#include "event-loop.h"

/* The following functions are defined for the benefit of inftarg.c;
   they should never be called */

int ATTR_NORETURN
attach(int pid)
{
#ifdef ATTR_NORETURN
  internal_error(__FILE__, __LINE__,
                 "macosx_nat_inferior: unexpected call to attach()");
#else
  return 0;
#endif /* ATTR_NORETURN */
}

void ATTR_NORETURN
detach(int sig)
{
  internal_error(__FILE__, __LINE__,
                 "macosx_nat_inferior: unexpected call to detach()");
}

void ATTR_NORETURN
kill_inferior(void)
{
  internal_error(__FILE__, __LINE__,
                 "macosx_nat_inferior: unexpected call to kill_inferior()");
}

void ATTR_NORETURN
child_resume(int pid, int step, enum target_signal sig)
{
  internal_error(__FILE__, __LINE__,
                 "macosx_nat_inferior: unexpected call to child_resume()");
}

int ATTR_NORETURN
child_wait(int pid, struct target_waitstatus *status,
           gdb_client_data client_data)
{
#ifdef ATTR_NORETURN
  internal_error(__FILE__, __LINE__,
                 "macosx_nat_inferior: unexpected call to child_wait()");
#else
  return 0;
#endif /* ATTR_NORETURN */
}

int child_xfer_memory
  (CORE_ADDR memaddr, gdb_byte *myaddr, int len, int write,
   struct mem_attrib *attrib, struct target_ops *target)
{
  internal_error(__FILE__, __LINE__,
                 "macosx_nat_inferior: unexpected call to child_xfer_memory()");
}

/* EOF */
