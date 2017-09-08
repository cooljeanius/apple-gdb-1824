/* mac-xdep.c: Top level support for Mac interface to GDB, the GNU debugger.
 * Copyright 1994, 1995, 1998, 2000, 2001 Free Software Foundation, Inc.
 * Contributed by Cygnus Support.  Written by Stan Shebs.
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

#ifdef __APPLE_API_EVOLVING
# undef __APPLE_API_EVOLVING
#endif /* __APPLE_API_EVOLVING */

/* hack to deal with troublesome headers: */
#ifndef _SYS_ACL_H
# define _SYS_ACL_H 1
struct _acl;
typedef struct _acl *acl_t;
#endif /* !_SYS_ACL_H */

/* try to deal with struct opacity: */
#ifdef OPAQUE_TOOLBOX_STRUCTS
# undef OPAQUE_TOOLBOX_STRUCTS
#endif  /* OPAQUE_TOOLBOX_STRUCTS */
#define OPAQUE_TOOLBOX_STRUCTS 0
#ifndef MenuRef
# define MenuRef MenuHandle
#endif /* !MenuRef */

#ifdef OPAQUE_UPP_TYPES
# undef OPAQUE_UPP_TYPES
#endif  /* OPAQUE_UPP_TYPES */
#define OPAQUE_UPP_TYPES 0

#ifdef CALL_NOT_IN_CARBON
# undef CALL_NOT_IN_CARBON
#endif /* CALL_NOT_IN_CARBON */
#define CALL_NOT_IN_CARBON 1

#if !defined(__LP64__) || (defined(__LP64__) && !__LP64__)
# if !defined(_MSC_VER)
#  pragma options align=mac68k
# endif /* !_MSC_VER */
#endif /* !__LP64__ */

#include "defs.h"
#include "config/xm-mpw.h"

#include <readline/readline.h>
#include <readline/history.h>

#ifndef ALLOW_OBSOLETE_CARBON
/* Not quite sure if 0 or 1 is what I want; comments in headers kind of seem to
 * disagree with what the headers actually do... */
# define ALLOW_OBSOLETE_CARBON 1
#endif /* !ALLOW_OBSOLETE_CARBON */

#include <ApplicationServices/ApplicationServices.h>
#include <QuickDraw.h>
#include <Carbon/Carbon.h>
#include <Desk.h>
#include <SegLoad.h>
#include <OSEvents.h>
#include <DiskInit.h>
#include <Packages.h>
#include <Traps.h>
#include <PPCToolbox.h>
#include <StandardFile.h>
#ifdef USE_CARBON_FRAMEWORK
# ifndef __MACERRORS__
#  include <CarbonCore/MacErrors.h>
# endif /* !__MACERRORS__ */
# ifndef __OSUTILS__
#  include <CarbonCore/OSUtils.h>
# endif /* !__OSUTILS__ */
# ifndef __DIALOGS__
#  include <HIToolbox/Dialogs.h>
# endif /* !__DIALOGS__ */
# ifndef __MACTEXTEDITOR__
#  include <HIToolbox/MacTextEditor.h>
# endif /* __MACTEXTEDITOR__ */
# ifndef __MENUS__
#  include <HIToolbox/Menus.h>
# endif /* !__MENUS__ */
# ifndef __TEXTEDIT__
#  include <HIToolbox/TextEdit.h>
# endif /* !__TEXTEDIT__ */
# ifndef __QUICKDRAWTYPES__
#  include <QD/QuickdrawTypes.h>
# endif /* !__QUICKDRAWTYPES__ */
#endif /* USE_CARBON_FRAMEWORK */

#ifdef MPW
# define QD(whatever) (qd.##whatever)
# define QDPat(whatever) (&(qd.##whatever))
#endif /* MPW */

#ifdef THINK_C
# define QD(whatever) (whatever)
#endif /* THINK_C */

#define p2c(pstr,cbuf)  \
  strncpy(cbuf, ((char *) (pstr) + 1), pstr[0]);  \
  cbuf[pstr[0]] = '\0';

#define pascalify(STR) \
  snprintf(tmpbuf, sizeof(tmpbuf), " %s", STR);  \
  tmpbuf[0] = strlen(STR);

#include "gdbcmd.h"
#ifdef ALLOW_OLD_INCLUDES
# include "call-cmds.h"
#endif /* ALLOW_OLD_INCLUDES */
#include "symtab.h"
#include "inferior.h"
#include <signal.h>
#include "target.h"
#include "breakpoint.h"
#include "gdbtypes.h"
#include "expression.h"
#include "language.h"

#include "mac-defs.h"

#if !defined(NSInteger) && !defined(NSUInteger) && !defined(__OBJC__)
# if (defined(__LP64__) && __LP64__) || \
     (defined(TARGET_OS_EMBEDDED) && TARGET_OS_EMBEDDED) || \
     (defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE) || \
     (defined(TARGET_OS_WIN32) && TARGET_OS_WIN32) || \
     (defined(NS_BUILD_32_LIKE_64) && NS_BUILD_32_LIKE_64)
typedef long NSInteger;
typedef unsigned long NSUInteger;
# else
typedef int NSInteger;
typedef unsigned int NSUInteger;
# endif /* __LP64__ || TARGET_OS_EMBEDDED || TARGET_OS_IPHONE || TARGET_OS_WIN32 || NS_BUILD_32_LIKE_64 */
#endif /* !NSInteger && !NSUInteger && !__OBJC__ */

/* global variables: */
NSInteger debug_openp = 0;

/* This is true if we are running as a standalone application: */
int mac_app;

/* This is true if we are using WaitNextEvent: */
NSInteger use_wne;

/* This is true if we have Color Quickdraw: */
NSInteger has_color_qd;

/* This is true if we are using Color Quickdraw: */
NSInteger use_color_qd;

NSInteger inbackground;

Rect dragrect =
{ -32000, -32000, 32000, 32000 };
Rect sizerect;

NSInteger sbarwid = 15;

#ifndef OPAQUE_TOOLBOX_STRUCTS
struct GrafPort {
  short      device;     /* not available in Carbon*/
  BitMap    portBits; /* in Carbon use GetPortBitMapForCopyBits or IsPortColor*/
  Rect       portRect;   /* in Carbon use Get/SetPortBounds*/
  RgnHandle  visRgn;     /* in Carbon use Get/SetPortVisibleRegion*/
  RgnHandle  clipRgn;    /* in Carbon use Get/SetPortClipRegion*/
  Pattern    bkPat;    /* not available in Carbon all GrafPorts are CGrafPorts*/
  Pattern    fillPat;  /* not available in Carbon all GrafPorts are CGrafPorts*/
  Point      pnLoc;      /* in Carbon use GetPortPenLocation or MoveTo*/
  Point      pnSize;     /* in Carbon use Get/SetPortPenSize*/
  short      pnMode;     /* in Carbon use Get/SetPortPenMode*/
  Pattern    pnPat;    /* not available in Carbon all GrafPorts are CGrafPorts*/
  short      pnVis;      /* in Carbon use GetPortPenVisibility or Show/HidePen*/
  short      txFont;     /* in Carbon use GetPortTextFont or TextFont*/
  StyleField txFace;     /* in Carbon use GetPortTextFace or TextFace*/
  short      txMode;     /* in Carbon use GetPortTextMode or TextMode*/
  short      txSize;     /* in Carbon use GetPortTextSize or TextSize*/
  Fixed      spExtra;    /* in Carbon use GetPortSpExtra or SpaceExtra*/
  long       fgColor;    /* not available in Carbon */
  long       bkColor;    /* not available in Carbon*/
  short      colrBit;    /* not available in Carbon*/
  short      patStretch; /* not available in Carbon*/
  Handle     picSave;    /* in Carbon use IsPortPictureBeingDefined*/
  Handle     rgnSave;    /* not available in Carbon*/
  Handle     polySave;   /* not available in Carbon*/
  QDProcsPtr grafProcs;/* not available in Carbon all GrafPorts are CGrafPorts*/
};
typedef struct GrafPort                 GrafPort;
typedef GrafPort *                      TransparentGrafPtr;
typedef TransparentGrafPtr              TransparentWindowPtr;
typedef TransparentWindowPtr            TransparentDialogPtr;
#endif /* !defined(OPAQUE_TOOLBOX_STRUCTS) */

/* Globals for the console window: */
#ifndef OPAQUE_TOOLBOX_STRUCTS
TransparentWindowPtr console_window;
#else
# if !OPAQUE_TOOLBOX_STRUCTS
WindowPtr console_window;
# else
#  error "Window pointers need to be transparent."
# endif /* !OPAQUE_TOOLBOX_STRUCTS */
#endif /* !defined(OPAQUE_TOOLBOX_STRUCTS) */

/* FIXME: the opacity of this struct is problematic: */
#ifdef __HIOBJECT__
ControlRef console_v_scrollbar;
#else
ControlHandle console_v_scrollbar;
#endif /* __HIOBJECT__ */

Rect console_v_scroll_rect;

TEHandle console_text;

Rect console_text_rect;

/* prototypes for functions used only in this file: */
extern int mac_init(void);
extern int new_console_window(void);
extern int mac_command_loop(void);
extern int get_global_mouse(Point *);
extern int adjust_cursor(Point, RgnHandle);
extern int do_event(EventRecord *);
extern int do_idle(void);
extern int grow_window(WindowPtr, Point);
extern int zoom_window(WindowPtr, Point, short);
extern int resize_console_window(void);
extern int close_window(WindowPtr);
extern pascal void v_scroll_proc(ControlHandle, short);
extern int do_mouse_down(WindowPtr, EventRecord *);
extern int scroll_text(NSInteger, NSInteger);
extern int activate_window(WindowPtr, NSInteger);
extern int update_window(WindowPtr);
extern int adjust_menus(void);
extern int do_menu_command(long);
extern int do_keyboard_command(NSInteger);
extern int draw_console(void);
extern int force_update(WindowPtr);
extern int adjust_console_sizes(void);
extern int adjust_console_scrollbars(void);
extern int adjust_console_text(void);
extern const char *filename_completion_function(char *, char *);

/* This will go away eventually: */
int gdb_has_a_terminal(void)
{
  return 1;
}

#if !defined(__THPRINT_H__) && !defined(SysEnvRec)
struct SysEnvRec {
  short environsVersion;
  short machineType;
  short systemVersion;
  short processor;
  Boolean hasFPU;
  Boolean hasColorQD;
  short keyBoardType;
  short atDrvrVersNum;
  short sysVRefNum;
};
typedef struct SysEnvRec SysEnvRec;
#endif /* !__THPRINT_H__ && !SysEnvRec */

#if !defined(__QUICKDRAW__) && !defined(qd)
extern struct {
  char privates[76];
  long randSeed;
  BitMap screenBits;
  Cursor arrow;
  Pattern dkGray;
  Pattern ltGray;
  Pattern gray;
  Pattern black;
  Pattern white;
  GrafPtr thePort;
} qd;
#endif /* !__QUICKDRAW__ && !qd */

/* */
int mac_init(void)
{
  SysEnvRec se;
  char *str;
  Handle menubar;
  MenuHandle menu;
  Handle siow_resource;

  mac_app = 0;

  str = getenv("DEBUG_GDB");
  if ((str != NULL) && (str[0] != '\0')) {
      if (strcmp(str, "openp") == 0) {
          debug_openp = 1;
      }
  }

  /* Do NOT do anything if we are running under MPW: */
  if (!StandAlone) {
    return 0;
  }

  /* Do NOT do anything if we are using SIOW. */
  /* This test requires that the siow 0 resource, as defined in
   * {RIncludes}siow.r, not be messed with. If it is, then the
   * standard Mac setup below will step on SIOW's Mac setup and
   * most likely crash the machine. */
  siow_resource = GetResource('siow', (ResID)0);
  if (siow_resource != nil) {
    return 0;
  }

  mac_app = 1;

  /* Do the standard Mac environment setup: */
#if defined(qd) || (defined(__QUICKDRAW__) && defined(QD) && defined(InitGraf))
  InitGraf(&QD(thePort));
#endif /* qd || (__QUICKDRAW__ && QD && InitGraf) */
#if (!defined(__LP64__) || !__LP64__) && (defined(WITH_CFM) && WITH_CFM)
  InitFonts();
#endif /* !__LP64__ && WITH_CFM */
  FlushEvents((EventMask)everyEvent, (EventMask)0U);
  InitWindows();
  InitMenus();
  TEInit();
  InitDialogs(NULL);
  InitCursor();

  /* Color Quickdraw is different from Classic QD: */
  SysEnvirons(2, &se);
  has_color_qd = se.hasColorQD;
  /* Use it if we got it: */
  use_color_qd = has_color_qd;

  sizerect.top = 50;
  sizerect.left = 50;
  sizerect.bottom = 1000;
  sizerect.right = 1000;
#if 0
  sizerect.bottom = (screenBits.bounds.bottom - screenBits.bounds.top);
  sizerect.right = (screenBits.bounds.right - screenBits.bounds.left);
#endif /* 0 */

  /* Set up the menus: */
  menubar = GetNewMBar((short)mbMain);
  SetMenuBar(menubar);
  /* Add the DAs etc as usual: */
  menu = GetMenuHandle((MenuID)mApple);
  if (menu != nil) {
      AddResMenu(menu, 'DRVR');
  }
  DrawMenuBar();

  new_console_window();
  
  return 0;
}

/* */
int new_console_window(void)
{
  /* Create the main window we are going to play in: */
  if (has_color_qd) {
    console_window = GetNewCWindow((short)wConsole, NULL,
                                   ((WindowPtr)-1L));
  } else {
    console_window = GetNewWindow((short)wConsole, NULL,
                                  ((WindowPtr)-1L));
  }

  SetPort((GrafPtr)console_window);
  console_text_rect = console_window->portRect;
  /* Leave 8 pixels of blank space, for aesthetic reasons and to make it easier
   * to select from the beginning of a line: */
  console_text_rect.left += 8;
  console_text_rect.bottom -= (sbarwid - 1);
  console_text_rect.right -= (sbarwid - 1);
  console_text = TENew(&console_text_rect, &console_text_rect);
  TESetSelect(0, 40000, console_text);
  TEDelete(console_text);
  TEAutoView((Boolean)1U, console_text);

  console_v_scroll_rect = console_window->portRect;
  console_v_scroll_rect.bottom -= (sbarwid - 1);
  console_v_scroll_rect.left = (console_v_scroll_rect.right - sbarwid);
  console_v_scrollbar =
    NewControl(console_window, &console_v_scroll_rect,
               "\p", (Boolean)1U, (SInt16)0, (SInt16)0, (SInt16)0,
               (SInt16)scrollBarProc, 0L);

  ShowWindow(console_window);
  SelectWindow(console_window);
  
  return 0;
}

#if !defined(OSTrap) && !defined(ToolTrap) && !defined(__OSUTILS__)
enum { OSTrap, ToolTrap };
#endif /* !OSTrap && !ToolTrap */

/* */
int mac_command_loop(void)
{
  NSInteger eventloopdone = 0;
  Boolean gotevent;
  Point mouse;
  EventRecord event;
  RgnHandle cursorRgn;
  NSInteger tm;

#ifdef ToolTrap
  /* Figure out if the WaitNextEvent Trap is available: */
  use_wne =
    (NGetTrapAddress(0x60, ToolTrap) != NGetTrapAddress(0x9f, ToolTrap));
#else
  use_wne = 0;
#endif /* ToolTrap */
  /* Pass WaitNextEvent an empty region the first time through.  */
  cursorRgn = NewRgn();
  /* Go into the main event-handling loop: */
  while (!eventloopdone) {
      /* Use WaitNextEvent if it is available, otherwise GetNextEvent: */
      if (use_wne) {
          get_global_mouse(&mouse);
          adjust_cursor(mouse, cursorRgn);
          tm = GetCaretTime();
          gotevent = WaitNextEvent((EventMask)everyEvent, &event,
                                   (UInt32)tm, cursorRgn);
      } else {
          SystemTask();
          gotevent = GetNextEvent((EventMask)everyEvent, &event);
      }
      /* First decide if the event is for a dialog or is just any old event: */
      if ((FrontWindow() != nil) && IsDialogEvent(&event)) {
          short itemhit;
          DialogPtr dialog;

          /* Handle all the modeless dialogs here: */
          if (DialogSelect(&event, &dialog, &itemhit)) {
              /* (do nothing) */ ;
          }
      } else if (gotevent) {
          /* Make sure we have the right cursor before handling the event: */
          adjust_cursor(event.where, cursorRgn);
          do_event(&event);
      } else {
          do_idle();
      }
  }
  
  return 0;
}

/* Collect the global coordinates of the mouse pointer: */
int get_global_mouse(Point *mouse)
{
  EventRecord evt;

  OSEventAvail((short)0, &evt);
  *mouse = evt.where;
  
  return 0;
}

/* Change cursor appearance to be appropriate for the given mouse location: */
int adjust_cursor(Point mouse, RgnHandle region)
{
#if (defined(__APPLE__) && defined(__APPLE_CC__)) || defined(__MWERKS__)
# pragma unused (mouse, region)
#endif /* (__APPLE__ && __APPLE_CC__) || __MWERKS__ */
  /* do nothing, besides return: */
  return 0;
}

/* Decipher an event, maybe do something with it: */
int do_event(EventRecord *evt)
{
  short part, err, rslt = 0;
  WindowPtr win;
  char key;
  Point pnt;

  switch (evt->what) {
    case mouseDown:
      /* See if the click happened in a special part of the screen. */
      part = FindWindow(evt->where, &win);
      switch (part) {
          case inMenuBar:
              adjust_menus();
              do_menu_command(MenuSelect(evt->where));
              break;
          case inSysWindow:
              SystemClick(evt, win);
              break;
          case inContent:
              if (win != FrontWindow()) {
                  /* Bring the clicked-on window to the front. */
                  SelectWindow(win);
                  /* Fix the menu to match the new front window. */
                  adjust_menus();
                  /* We always want to discard the event now, since clicks
                   * in a window are often irreversible actions. */
              } else {
                  /* Mouse clicks on front window do something useful. */
                  do_mouse_down(win, evt);
              }
              break;
          case inDrag:
              /* Standard drag behavior, no tricks necessary. */
              DragWindow(win, evt->where, &dragrect);
              break;
          case inGrow:
              grow_window(win, evt->where);
              break;
          case inZoomIn:
          case inZoomOut:
              zoom_window(win, evt->where, (short)part);
              break;
          case inGoAway:
              close_window(win);
              break;
      } /* end "switch (part)" */
      break;
    case keyDown:
    case autoKey:
      key = (evt->message & charCodeMask);
      /* Check for menukey equivalents: */
      if (evt->modifiers & cmdKey) {
          if (evt->what == keyDown) {
              adjust_menus();
              do_menu_command(MenuKey((CharParameter)key));
          }
      } else {
          if (evt->what == keyDown) {
              /* Random keypress, interpret it: */
              do_keyboard_command(key);
          }
      }
      break;
    case activateEvt:
      activate_window((WindowPtr)evt->message,
                      (evt->modifiers & activeFlag));
      break;
    case updateEvt:
      update_window((WindowPtr)evt->message);
      break;
    case diskEvt:
      /* Call DIBadMount in response to a diskEvt, so that the user can
       * format a floppy. (from DTS Sample) */
      if (HiWord(evt->message) != noErr) {
          SetPt(&pnt, (short)50, (short)50);
          err = DIBadMount(pnt, evt->message);
      }
      break;
    case app4Evt:
      /* Grab only a single byte: */
      switch ((evt->message >> 24) & 0xFF) {
          case 0xfa:
              break;
          case 1:
              inbackground = !(evt->message & 1);
              activate_window(FrontWindow(), !inbackground);
              break;
      } /* end "switch ((evt->message >> 24) & 0xFF)" */
      break;
    case kHighLevelEvent:
      AEProcessAppleEvent(evt);
      break;
    case nullEvent:
      do_idle();
      rslt = 1;
      break;
    default:
      break;
  } /* end "switch (evt->what)" */
  return rslt;
}

/* Do any idle-time activities: */
int do_idle(void)
{
  TEIdle(console_text);
  
  return 0;
}

/* */
int grow_window(WindowPtr win, Point where)
{
#if defined(__LP64__) && __LP64__ && defined(__GNUC__) && !defined(__STRICT_ANSI__)
# warning "64BIT: Inspect use of long"
#endif /* __LP64__ && __GNUC__ && !__STRICT_ANSI__ */
  long winsize;
  NSInteger h, v;
  GrafPtr oldport;

  winsize = GrowWindow(win, where, &sizerect);
  /* Only do anything if it actually changed size: */
  if (winsize != 0) {
      GetPort(&oldport);
      SetPort((GrafPtr)win);
      if (win == console_window) {
          EraseRect(&win->portRect);
          h = LoWord(winsize);
          v = HiWord(winsize);
          SizeWindow(win, (short)h, (short)v, (Boolean)1U);
          resize_console_window();
      }
      SetPort((GrafPtr)oldport);
  }
  
  return 0;
}

/* */
int zoom_window(WindowPtr win, Point where, short part)
{
# if (defined(__APPLE__) && defined(__APPLE_CC__)) || defined(__MWERKS__)
#  pragma unused (where)
# endif /* (__APPLE__ && __APPLE_CC__) || __MWERKS__ */
  ZoomWindow(win, (WindowPartCode)part, (Boolean)(win == FrontWindow()));
  if (win == console_window) {
      resize_console_window();
  }
  
  return 0;
}

/* */
int resize_console_window(void)
{
  adjust_console_sizes();
  adjust_console_scrollbars();
  adjust_console_text();
#if 0
  InvalRect(&console_window->portRect);
#endif /* 0 */
  return 0;
}

/* 0 */
int close_window(WindowPtr win)
{
#if (defined(__APPLE__) && defined(__APPLE_CC__)) || defined(__MWERKS__)
# pragma unused (win)
#endif /* (__APPLE__ && __APPLE_CC__) || __MWERKS__ */
  /* do nothing, besides return: */
  return 0;
}

pascal void v_scroll_proc(ControlHandle control, short part)
{
  NSInteger oldval, amount = 0, newval;
  NSInteger pagesize = (((*console_text)->viewRect.bottom
                         - (*console_text)->viewRect.top)
                        / (*console_text)->lineHeight);
  if (part) {
      oldval = GetCtlValue(control);
      switch (part) {
          case inUpButton:
              amount = 1;
              break;
          case inDownButton:
              amount = -1;
              break;
          case inPageUp:
              amount = pagesize;
              break;
          case inPageDown:
              amount = -pagesize;
              break;
          default:
              /* (should freak out) */
              break;
      } /* end "switch (part)" */
      SetControlValue(control, (SInt16)(oldval - amount));
      newval = GetCtlValue(control);
      amount = (oldval - newval);
      if (amount) {
        TEScroll((short)0, (short)(amount * (*console_text)->lineHeight),
                 console_text);
      }
  } /* end "if (part)" */
}

int do_mouse_down(WindowPtr win, EventRecord * event)
{
  short part, value;
  Point mouse;
  ControlHandle control;

  if (1/*is_app_window(win)*/) {
      SetPort((GrafPtr)win);
      mouse = event->where;
      GlobalToLocal(&mouse);
      part = FindControl(mouse, win, &control);
      if (control == console_v_scrollbar) {
          switch (part) {
            case inThumb:
                value = GetCtlValue(control);
                part = TrackControl(control, mouse, nil);
		if (part) {
                    value -= GetCtlValue(control);
                    if (value) {
                        TEScroll((short)0,
                                 (short)(value * (*console_text)->lineHeight),
                                 console_text);
                    }
		} /* end "if (part)" */
		break;
            default:
#if 0		/* do NOT deal with right now */
# if 1		/* universal headers */
		value = TrackControl(control, mouse,
                                     (ControlActionUPP)v_scroll_proc);
# else
		value = TrackControl(control, mouse,
                                     (ProcPtr)v_scroll_proc);
# endif /* 1 */
#endif /* 0 */
		 break;
          } /* end "switch (part)" */
      } else {
          TEClick(mouse, (Boolean)0U, console_text);
      }
  }
  
  return 0;
}

/* */
int scroll_text(NSInteger hlines, NSInteger vlines)
{
#if (defined(__APPLE__) && defined(__APPLE_CC__)) || defined(__MWERKS__)
# pragma unused (hlines, vlines)
#endif /* (__APPLE__ && __APPLE_CC__) || __MWERKS__ */
  /* do nothing, besides return: */
  return 0;
}

/* */
int activate_window(WindowPtr win, NSInteger activate)
{
  Rect grow_rect;

  if (win == nil) {
    return 0;
  }
  /* It is convenient to make the active window also be the current GrafPort: */
  if (activate) {
    SetPort((GrafPtr)win);
  }
  /* Activate the console window's scrollbar. */
  if (win == console_window) {
      if (activate) {
	TEActivate(console_text);
	/* Cause the grow icon to be redrawn at the next update: */
	grow_rect = console_window->portRect;
	grow_rect.top = (grow_rect.bottom - sbarwid);
	grow_rect.left = (grow_rect.right - sbarwid);
#if defined(USE_CARBON_FRAMEWORK) && (!defined(__LP64__) || !__LP64__)
	TXNForceUpdate((TXNObject)&grow_rect);
#else
	InvalRect(&grow_rect);
#endif /* USE_CARBON_FRAMEWORK && !__LP64__ */
      } else {
	TEDeactivate(console_text);
	DrawGrowIcon(console_window);
      }
      HiliteControl(console_v_scrollbar,
                    (ControlPartCode)(activate ? 0 : 255));
  }
  
  return 0;
}

/* */
int update_window(WindowPtr win)
{
  NSInteger controls = 1, growbox = 0;
  GrafPtr oldport;

  /* Set the updating window to be the current grafport. */
  GetPort(&oldport);
  SetPort((GrafPtr)win);
#if 0
  recalc_depths();
#endif /* 0 */
  BeginUpdate(win);
  if (win == console_window) {
      draw_console();
      controls = 1;
      growbox = 1;
  }
  if (controls) {
      UpdateControls(win, win->visRgn);
  }
  if (growbox) {
      DrawGrowIcon(win);
  }
  EndUpdate(win);
  SetPort((GrafPtr)oldport);
  
  return 0;
}

/* */
int adjust_menus(void)
{
  /* do nothing, besides return: */
  return 0;
}

#if defined(__LP64__) && __LP64__ && defined(__GNUC__) && !defined(__STRICT_ANSI__)
# warning "64BIT: Inspect use of long"
#endif /* __LP64__ && __GNUC__ && !__STRICT_ANSI__ */
int do_menu_command(long which)
{
  short menuid, menuitem;
  Str255 daname;
  short daRefNum;
  char cmdbuf[300];

  cmdbuf[0] = '\0';
  menuid = HiWord(which);
  menuitem = LoWord(which);
  switch (menuid) {
    case mApple:
      switch (menuitem) {
        case miAbout:
          Alert((SInt16)128, nil);
          break;
#if 0
        case miHelp:
          /* (should pop up help info) */
          break;
#endif /* 0 */
        default:
          GetMenuItemText(GetMenuHandle((MenuID)mApple),
                          (MenuItemIndex)menuitem, daname);
          daRefNum = OpenDeskAcc(daname);
      } /* end "switch (menuitem)" */
      break;
    case mFile:
      switch (menuitem) {
        case miFileNew:
          if (console_window == FrontWindow()) {
            close_window(console_window);
          }
          new_console_window();
          break;
        case miFileOpen:
          SysBeep((short)20);
          break;
        case miFileQuit:
          ExitToShell();
          break;
      } /* end "switch (menuitem)" */
      break;
    case mEdit:
      switch (menuitem) {
        case miEditCut:
          TECut(console_text);
          break;
        case miEditCopy:
          TECopy(console_text);
          break;
        case miEditPaste:
          TEPaste(console_text);
          break;
        case miEditClear:
          TEDelete(console_text);
          break;
      } /* end "switch (menuitem)" */
      /* All of these operations need the same postprocessing: */
      adjust_console_sizes();
      adjust_console_scrollbars();
      adjust_console_text();
      break;
    case mDebug:
      switch (menuitem) {
        case miDebugTarget:
#if defined(__LP64__) && __LP64__ && defined(__GNUC__) && !defined(__STRICT_ANSI__)
# warning "64BIT: Check formatting arguments"
#endif /* __LP64__ && __GNUC__ && !__STRICT_ANSI__ */
          snprintf(cmdbuf, sizeof(cmdbuf), "target %s", "remote");
          break;
        case miDebugRun:
#if defined(__LP64__) && __LP64__ && defined(__GNUC__) && !defined(__STRICT_ANSI__)
# warning "64BIT: Check formatting arguments"
#endif /* __LP64__ && __GNUC__ && !__STRICT_ANSI__ */
          snprintf(cmdbuf, sizeof(cmdbuf), "run");
          break;
        case miDebugContinue:
#if defined(__LP64__) && __LP64__ && defined(__GNUC__) && !defined(__STRICT_ANSI__)
# warning "64BIT: Check formatting arguments"
#endif /* __LP64__ && __GNUC__ && !__STRICT_ANSI__ */
          snprintf(cmdbuf, sizeof(cmdbuf), "continue");
          break;
        case miDebugStep:
#if defined(__LP64__) && __LP64__ && defined(__GNUC__) && !defined(__STRICT_ANSI__)
# warning "64BIT: Check formatting arguments"
#endif /* __LP64__ && __GNUC__ && !__STRICT_ANSI__ */
          snprintf(cmdbuf, sizeof(cmdbuf), "step");
          break;
        case miDebugNext:
#if defined(__LP64__) && __LP64__ && defined(__GNUC__) && !defined(__STRICT_ANSI__)
# warning "64BIT: Check formatting arguments"
#endif /* __LP64__ && __GNUC__ && !__STRICT_ANSI__ */
          snprintf(cmdbuf, sizeof(cmdbuf), "next");
          break;
      } /* end "switch (menuitem)" */
      break;
  } /* end "switch (menuid)" */
  HiliteMenu((MenuID)0);
  /* Execute a command if one had been given. Do here because a command
   * may longjmp before we get a chance to unhilite the menu. */
  if (strlen(cmdbuf) > 0) {
    execute_command(cmdbuf, 0);
  }
  
  return 0;
}

char commandbuf[1000];

/* */
int do_keyboard_command(NSInteger key)
{
  NSInteger startpos, endpos, i, len;
  char *last_newline;
  char buf[10], *text_str, *command, *cmd_start;
  CharsHandle text;

  if ((key == '\015') || (key == '\003')) {
      text = TEGetText(console_text);
      HLock((Handle)text);
      text_str = *text;
      startpos = (*console_text)->selStart;
      endpos = (*console_text)->selEnd;
      if (startpos != endpos) {
	len = (endpos - startpos);
	cmd_start = (text_str + startpos);
      } else {
	for ((i = startpos - 1); (i >= 0); --i) {
          if (text_str[i] == '\015') {
            break;
          }
	}
	last_newline = (text_str + i);
	len = ((text_str + startpos) - 1 - last_newline);
	cmd_start = (last_newline + 1);
      }
      if (len > 1000) {
	len = 999;
      }
      if (len < 0) {
	len = 0;
      }
      strncpy((commandbuf + 1), cmd_start, (size_t)len);
      commandbuf[(1 + len)] = 0;
      command = (commandbuf + 1);
      HUnlock((Handle)text);
      commandbuf[0] = strlen(command);

      /* Insert a newline and recalculate before doing any command: */
      key = '\015';
      TEKey((CharParameter)key, console_text);
      TEInsert(buf, 1, console_text);
      adjust_console_sizes();
      adjust_console_scrollbars();
      adjust_console_text();

      if (strlen(command) > 0) {
	execute_command(command, 0);
	bpstat_do_actions(&stop_bpstat);
      }
  } else {
      /* A self-inserting character. This includes delete: */
      TEKey((CharParameter)key, console_text);
  }
  
  return 0;
}

/* Draw all graphical stuff in the console window: */
int draw_console(void)
{
  SetPort((GrafPtr)console_window);
  TEUpdate(&(console_window->portRect), console_text);
  
  return 0;
}

/* Cause an update of a given window's entire contents: */
int force_update(WindowPtr win)
{
  GrafPtr oldport;

  if (win == nil) {
    return 0;
  }
  GetPort(&oldport);
  SetPort((GrafPtr)win);
  EraseRect(&win->portRect);
#if defined(USE_CARBON_FRAMEWORK) && (!defined(__LP64__) || !__LP64__)
  TXNForceUpdate((TXNObject)&win->portRect);
#else
  InvalRect(&win->portRect);
#endif /* USE_CARBON_FRAMEWORK && !__LP64__ */
  SetPort((GrafPtr)oldport);
  
  return 0;
}

/* */
int adjust_console_sizes(void)
{
  Rect tmprect;

  tmprect = console_window->portRect;
  /* Move and size the scrollbar: */
  MoveControl(console_v_scrollbar, (SInt16)(tmprect.right - sbarwid),
              (SInt16)0);
  SizeControl(console_v_scrollbar, (SInt16)(sbarwid + 1),
              (SInt16)(tmprect.bottom - sbarwid + 1));
  /* Move and size the text: */
  tmprect.left += 7;
  tmprect.right -= sbarwid;
  tmprect.bottom -= sbarwid;
  InsetRect(&tmprect, (short)1, (short)1);
  (*console_text)->destRect = tmprect;
  /* Fiddle bottom of viewrect to be even multiple of text lines: */
  tmprect.bottom = (tmprect.top
                    + ((tmprect.bottom - tmprect.top)
                       / (*console_text)->lineHeight)
                    * (*console_text)->lineHeight);
  (*console_text)->viewRect = tmprect;
  
  return 0;
}

/* */
int adjust_console_scrollbars(void)
{
  NSInteger lines, newmax, value;

#if 0
  (*console_v_scrollbar)->contrlVis = 0;
#endif /* 0 */
  lines = (*console_text)->nLines;
  newmax = (lines - (((*console_text)->viewRect.bottom
                      - (*console_text)->viewRect.top)
                     / (*console_text)->lineHeight));
  if (newmax < 0) {
    newmax = 0;
  }
  SetControlMaximum(console_v_scrollbar, (SInt16)newmax);
  value = (((*console_text)->viewRect.top - (*console_text)->destRect.top)
           / (*console_text)->lineHeight);
  SetControlValue(console_v_scrollbar, (SInt16)value);
#if 0
  (*console_v_scrollbar)->contrlVis = 0xff;
#endif /* 0 */
  ShowControl(console_v_scrollbar);
  
  return 0;
}

/* Scroll the TE record so that it is consistent with the scrollbar(s): */
int adjust_console_text(void)
{
  TEScroll((short)(((*console_text)->viewRect.left
                    - (*console_text)->destRect.left)
                   - 0 /* get h scroll value */),
           (short)(((((*console_text)->viewRect.top - (*console_text)->destRect.top)
                     / (*console_text)->lineHeight)
                    - GetCtlValue(console_v_scrollbar))
                   * (*console_text)->lineHeight),
	    console_text);
  
  return 0;
}

#ifndef _READLINE_H_
/* Readline substitute: */
char *readline(char *prrompt)
{
  return gdb_readline(prrompt);
}

char *rl_completer_word_break_characters;

char *rl_completer_quote_characters;

NSInteger (*rl_completion_entry_function)(const char *, int);

NSInteger rl_point;

char *rl_line_buffer;

char *rl_readline_name;
#endif /* !_READLINE_H_ */

#ifndef _HISTORY_H_
/* History substitute: */
void add_history(char *buf)
{
  /* do nothing, besides return: */
  return;
}
#endif /* !_HISTORY_H_ */

void stifle_history(NSInteger n)
{
#if (defined(__APPLE__) && defined(__APPLE_CC__)) || defined(__MWERKS__)
# pragma unused (n)
#endif /* (__APPLE__ && __APPLE_CC__) || __MWERKS__ */
  /* do nothing, besides return: */
  return;
}

NSInteger unstifle_history(void)
{
  /* do nothing, besides return: */
  return 0;
}

#ifndef _HISTORY_H_
NSInteger read_history(char *name)
{
  /* do nothing, besides return: */
  return 0;
}

NSInteger write_history(char *name)
{
  /* do nothing, besides return: */
  return 0;
}
#endif /* !_HISTORY_H_ */


NSInteger history_expand(char *x, char **y)
{
#if (defined(__APPLE__) && defined(__APPLE_CC__)) || defined(__MWERKS__)
# pragma unused (x, y)
#endif /* (__APPLE__ && __APPLE_CC__) || __MWERKS__ */
  /* do nothing, besides return: */
  return 0;
}

extern HIST_ENTRY *history_get(NSInteger xxx)
{
#if (defined(__APPLE__) && defined(__APPLE_CC__)) || defined(__MWERKS__)
# pragma unused (xxx)
#endif /* (__APPLE__ && __APPLE_CC__) || __MWERKS__ */
  return NULL;
}

NSInteger history_base;

const char *filename_completion_function(char *text, char *name)
{
#if (defined(__APPLE__) && defined(__APPLE_CC__)) || defined(__MWERKS__)
# pragma unused (text, name)
#endif /* (__APPLE__ && __APPLE_CC__) || __MWERKS__ */
  return "?";
}

#ifndef _TILDE_H_
char *tilde_expand(char *str)
{
  return xstrdup(str);
}
#endif /* !_TILDE_H_ */

/* Modified versions of standard I/O: */
#if defined(__LP64__) && __LP64__ && defined(__GNUC__) && !defined(__STRICT_ANSI__)
# warning "64BIT: Check formatting arguments"
#endif /* __LP64__ && __GNUC__ && !__STRICT_ANSI__ */
#undef fprintf
extern NSInteger hacked_fprintf(FILE *, const char *, ...);
NSInteger hacked_fprintf(FILE *fp, const char *fmt, ...)
{
  NSInteger ret;
  va_list ap;

  va_start (ap, fmt);
  if (mac_app && ((fp == stdout) || (fp == stderr))) {
      char buf[1000];

      ret = vsnprintf(buf, sizeof(buf), fmt, ap);
      TEInsert(buf, (long)strlen(buf), console_text);
  } else {
#if defined(__LP64__) && __LP64__ && defined(__GNUC__) && !defined(__STRICT_ANSI__)
# warning "64BIT: Check formatting arguments"
#endif /* __LP64__ && __GNUC__ && !__STRICT_ANSI__ */
    ret = vfprintf(fp, fmt, ap);
  }
  va_end(ap);
  return ret;
}

#if defined(__LP64__) && __LP64__ && defined(__GNUC__) && !defined(__STRICT_ANSI__)
# warning "64BIT: Check formatting arguments"
#endif /* __LP64__ && __GNUC__ && !__STRICT_ANSI__ */
#undef vfprintf
extern NSInteger hacked_vfprintf(FILE *, const char *, va_list);
NSInteger hacked_vfprintf(FILE *fp, const char *format, va_list args)
{
  if (mac_app && ((fp == stdout) || (fp == stderr))) {
      char buf[1000];
      NSInteger ret;

      ret = vsnprintf(buf, sizeof(buf), format, args);
      TEInsert(buf, (long)strlen(buf), console_text);
      if (strchr(buf, '\n')) {
	adjust_console_sizes();
        adjust_console_scrollbars();
        adjust_console_text();
      }
      return ret;
  } else {
#if defined(__LP64__) && __LP64__ && defined(__GNUC__) && !defined(__STRICT_ANSI__)
# warning "64BIT: Check formatting arguments"
#endif /* __LP64__ && __GNUC__ && !__STRICT_ANSI__ */
    return vfprintf(fp, format, args);
  }
}

#if defined(__LP64__) && __LP64__ && defined(__GNUC__) && !defined(__STRICT_ANSI__)
# warning "64BIT: Check formatting arguments"
#endif /* __LP64__ && __GNUC__ && !__STRICT_ANSI__ */
#undef printf
extern NSInteger hacked_printf(const char *, ...);
NSInteger hacked_printf(const char *fmt, ...)
{
  NSInteger ret;
  va_list ap;

  va_start(ap, fmt);
  ret = hacked_vfprintf(stdout, fmt, ap);
  va_end(ap);
  return ret;
}

#undef fputs
extern int hacked_fputs(const char *, FILE *);
int hacked_fputs(const char *s, FILE *fp)
{
  if (mac_app && ((fp == stdout) || (fp == stderr))) {
      TEInsert(s, (long)strlen(s), console_text);
      if (strchr(s, '\n')) {
	adjust_console_sizes();
	adjust_console_scrollbars();
	adjust_console_text();
      }
      return 0;
  } else {
    return fputs(s, fp);
  }
}

#undef fputc
extern int hacked_fputc(const char, FILE *);
int hacked_fputc(const char c, FILE *fp)
{
  if (mac_app && ((fp == stdout) || (fp == stderr))) {
      char buf[1];

      buf[0] = c;
      TEInsert(buf, 1, console_text);
      if (c == '\n') {
        adjust_console_sizes();
        adjust_console_scrollbars();
        adjust_console_text();
      }
      return c;
  } else {
    return fputc(c, fp);
  }
}

#undef putc
extern int hacked_putc(const char, FILE *);
int hacked_putc(const char c, FILE *fp)
{
  if (mac_app && ((fp == stdout) || (fp == stderr))) {
      char buf[1];

      buf[0] = c;
      TEInsert(buf, 1, console_text);
      if (c == '\n') {
        adjust_console_sizes();
        adjust_console_scrollbars();
        adjust_console_text();
      }
      return c;
  } else {
    return fputc(c, fp);
  }
}

#undef fflush
extern int hacked_fflush(FILE *);
int hacked_fflush(FILE *fp)
{
  if (mac_app && ((fp == stdout) || (fp == stderr))) {
      adjust_console_sizes();
      adjust_console_scrollbars();
      adjust_console_text();
      return 0;
  }
  return fflush(fp);
}

#undef fgetc
extern int hacked_fgetc(FILE *);
int hacked_fgetc(FILE *fp)
{
  if (mac_app && (fp == stdin)) {
      /* Catch any attempts to use this: */
      DebugStr("\pShould not be reading from stdin!");
      return '\n';
  }
  return fgetc(fp);
}

#if !defined(__LP64__) || (defined(__LP64__) && !__LP64__)
# if !defined(_MSC_VER)
#  pragma options align=reset
# endif /* !_MSC_VER */
#endif /* !__LP64__ */

/* EOF */
