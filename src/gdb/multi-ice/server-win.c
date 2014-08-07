/* server-win.c: Main code for multi-ice server for GDB on Windows.
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

#include "defs.h"
#include "server.h"
#include "low.h"
#include "windows.h"

/* from the "windres.h" header for binutils: */
#ifndef WS_OVERLAPPEDWINDOW
# define WS_OVERLAPPEDWINDOW (0xcf0000L)
#endif /* WS_OVERLAPPEDWINDOW */
#ifndef CW_USEDEFAULT
# define CW_USEDEFAULT (0x80000000)
#endif /* !CW_USEDEFAULT */

HWND main_window;
HINSTANCE app_instance;

/*
 * Prototypes for functions used only in this file:
 */
static LRESULT CALLBACK window_proc(HWND hWnd, UINT messg,
                                    WPARAM wParam, LPARAM lParam);

static BOOL console_control_handler(DWORD ctrl_type);

HWND get_main_window(void);

/*
 * For Windows, we need to create a window (which never gets posted)
 * to use to post the Config dialog, and because WinRDI_Initialize needs
 * it for no apparent reason.
 */
char program_class[] = "MICE gdbserver";

int platform_init(void)
{
  WNDCLASS wc;

  if (main_window != NULL) {
    return 0;
  }

  app_instance = GetModuleHandle((LPCTSTR)NULL);

  wc.lpszClassName  = program_class;
  wc.hInstance 	    = app_instance;
  wc.lpfnWndProc    = window_proc;
#ifdef IDC_ARROW
  wc.hCursor	    = LoadCursor((HINSTANCE)NULL, IDC_ARROW);
#else
  wc.hCursor	    = LoadCursor((HINSTANCE)NULL, NULL);
#endif /* IDC_ARROW */
#ifdef IDI_APPLICATION
  wc.hIcon	    = LoadIcon((HINSTANCE)NULL, IDI_APPLICATION);
#else
  wc.hIcon	    = LoadIcon((HINSTANCE)NULL, NULL);
#endif /* IDI_APPLICATION */
  wc.lpszMenuName   = (char *)NULL;
#ifdef WHITE_BRUSH
  wc.hbrBackground  = (HBRUSH)GetStockObject(WHITE_BRUSH);
#else
  wc.hbrBackground  = (HBRUSH)GetStockObject(NULL);
#endif /* WHITE_BRUSH */
  wc.style	    = 0;
  wc.cbClsExtra	    = 0;
  wc.cbWndExtra	    = 0;

  if (!RegisterClass(&wc)) {
      return FALSE;
  }


  main_window = CreateWindow(program_class,
                             "Multi-Ice Gdbserver",
                             WS_OVERLAPPEDWINDOW,
                             CW_USEDEFAULT,
                             CW_USEDEFAULT,
                             CW_USEDEFAULT,
                             CW_USEDEFAULT,
                             (HWND)NULL,
                             (HMENU)NULL,
                             (HINSTANCE)app_instance,
                             (void *)NULL);

  SetConsoleCtrlHandler((PHANDLER_ROUTINE)console_control_handler, TRUE);

  return 1;
}

static LRESULT CALLBACK
window_proc(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam)
{
  HDC hdc; /* handle to the device context */
  PAINTSTRUCT pstruct; /*struct for the call to BeginPaint */

  switch (messg) {
#ifdef WM_PAINT
    case WM_PAINT:
      hdc = BeginPaint(hWnd, &pstruct);
      EndPaint(hWnd, &pstruct);
      break;
#endif /* WM_PAINT */

    case WM_DESTROY:
      PostQuitMessage(0);
      break;

    default:
      return (DefWindowProc(hWnd, messg, wParam, lParam));
  }

  return (0L);
}

/*
 * This handler gets registered with the console subsystem,
 * so we can nicely shutdown the connection to the Multi-ICE
 * server when the user closes the console.
 */
int got_signal = 0;
static BOOL console_control_handler(DWORD ctrl_type)
{
    switch (ctrl_type) {
      case CTRL_BREAK_EVENT:
            close_connection();
            return TRUE;
      case CTRL_C_EVENT:
      case CTRL_CLOSE_EVENT:
      case CTRL_LOGOFF_EVENT:
      case CTRL_SHUTDOWN_EVENT:
            low_close_target();
            output("Exiting...\n");
            ExitProcess(0);
            return FALSE;
      default:
            return FALSE;
    }
    return FALSE;
}


HWND get_main_window(void)
{
  if (main_window == NULL) {
      platform_init();
  }
  return main_window;
}

int handle_system_events(void)
{
  MSG lpMsg;

  if (PeekMessage(&lpMsg, (HWND)NULL, 0, 0, PM_REMOVE) == 0) {
      return 1;
  } else if (lpMsg.message == WM_QUIT) {
      return 0;
  } else {
      TranslateMessage(&lpMsg);
      DispatchMessage(&lpMsg);
      return 1;
  }
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
