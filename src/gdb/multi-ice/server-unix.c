/* multi-ice/server-unix.c: Main code for multi-ice server for GDB on Unix-like
 * systems, such as GNU/Linux, Darwin, and the various BSD systems.
 * Based on server-win.c which was originally for Windows.
 * Copyright (C) 1999 Free Software Foundation, Inc. */
/*
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
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#if defined(HAVE_CONFIG_H) && 0
# include "config.h"
#endif /* HAVE_CONFIG_H && 0 */
#include "defs.h"
#include "server.h"
#include "low.h"
#include "unix.h"

void *main_window;
void *app_instance;

/*
 * Prototypes for functions used only in this file:
 */
static long window_proc(void *hWnd, unsigned int messg,
                        uintptr_t wParam, intptr_t lParam);

static bool console_control_handler(unsigned long ctrl_type);

void *get_main_window(void);

/*
 * For Windows, we need to do junk that is probably irrelevant here on
 * Unix-like systems, so most of this here is pared down from the Windows
 * version:
 */
char program_class[] = "MICE gdbserver";

int platform_init(void)
{
  if (main_window != NULL) {
    return 0;
  }

  return 1;
}

static long window_proc(void *hWnd, unsigned int messg,
                        uintptr_t wParam, intptr_t lParam)
{
  return (0L);
}

/*
 * This handler gets registered with the console subsystem,
 * so we can nicely shutdown the connection to the Multi-ICE
 * server when the user closes the console.
 */
int got_signal = 0;
static bool console_control_handler(unsigned long ctrl_type)
{
  return FALSE;
}


void *get_main_window(void)
{
  if (main_window == NULL) {
      platform_init();
  }
  return main_window;
}

int handle_system_events(void)
{
  return 0;
}


/* Print an error message and return to command level.
 * STRING is the error message, used as a fprintf string,
 * and ARG is passed as an argument to it.  */
void output_error(char *format, ...)
{
  va_list args;

  va_start(args, format);
  vfprintf(stderr, format, args);
  va_end(args);
}

/* Print an error message and exit reporting failure.
 * This is for a error from which we cannot continue.
 * STRING and ARG are passed to fprintf.  */
void output(char *format, ...)
{
  va_list args;
  va_start(args, format);
  vfprintf(stdout, format, args);
  va_end(args);
}

/* EOF */
