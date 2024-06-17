/* gdbserver/macosx-threads.c
   Mac OS X support for GDB, the GNU debugger.
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

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#ifndef __has_include
# define __has_include(foo) 0
#endif /* !__has_include */

#include <stdio.h>
#include <sys/param.h>
#include <sys/dir.h>
#include <sys/types.h>
#include <sys/errno.h>

#include <unistd.h>

#include <mach/machine/thread_status.h>
/* This is for thread_suspend.  */
#if defined(HAVE_MACH_THREAD_ACT_H) || __has_include(<mach/thread_act.h>)
# include <mach/thread_act.h>
#else
# if defined(TARGET_ARM) || defined(HOST_ARM) || defined(__arm__)
#  include <mach/arm/thread_act.h>
# elif defined(TARGET_I386) || defined(HOST_I386) || defined(__i386__)
#  include <mach/i386/thread_act.h>
# elif defined(TARGET_X86_64) || defined(HOST_X86_64) || defined(__x86_64__)
#  include <mach/x86_64/thread_act.h>
# elif defined(TARGET_POWERPC) || defined(HOST_POWERPC) || defined(__ppc__)
#  include <mach/ppc/thread_act.h>
# elif defined(TARGET_POWERPC64) || defined(HOST_POWERPC64) || defined(__ppc64__)
#  include <mach/ppc64/thread_act.h>
# else
#  if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#   warning "unknown architecture, check the ifdefs..."
#  endif /* __GNUC__ && !__STRICT_ANSI__ */
# endif /* architecture checks */
#endif /* HAVE_MACH_THREAD_ACT_H */

#include "macosx-threads.h"
#include "macosx-low.h"

/* */
void
gdb_pthread_kill(pthread_t pthread)
{
  mach_port_t mthread;
  kern_return_t kret;
  int ret;

  mthread = pthread_mach_thread_np(pthread);

  kret = thread_suspend(mthread);
  MACH_CHECK_ERROR(kret);

  ret = pthread_cancel(pthread);
  if (ret != 0)
    {
/* in case a macro has re-defined this function: */
#undef strerror
      warning("Unable to cancel thread: %s (%d)", strerror(errno), errno);
      thread_terminate(mthread);
    }

  kret = thread_abort(mthread);
  MACH_CHECK_ERROR(kret);

  kret = thread_resume(mthread);
  MACH_CHECK_ERROR(kret);

  ret = pthread_join(pthread, NULL);
  if (ret != 0)
    {
      warning("Unable to join to canceled thread: %s (%d)", strerror(errno),
              errno);
    }
}

/* */
pthread_t
gdb_pthread_fork(pthread_fn_t function, void *arg)
{
  int result;
  pthread_t pthread = NULL;
  pthread_attr_t attr;

  result = pthread_attr_init(&attr);
  if (result != 0)
    {
      error("Unable to initialize thread attributes: %s (%d)",
            strerror(errno), errno);
    }

  result = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
  if (result != 0)
    {
      error("Unable to initialize thread attributes: %s (%d)",
            strerror(errno), errno);
    }

  result = pthread_create(&pthread, &attr, function, arg);
  if (result != 0)
    {
      error("Unable to create thread: %s (%d)", strerror(errno), errno);
    }

  result = pthread_attr_destroy(&attr);
  if (result != 0)
    {
      warning("Unable to deallocate thread attributes: %s (%d)",
              strerror(errno), errno);
    }

  return pthread;
}

/* EOF */
