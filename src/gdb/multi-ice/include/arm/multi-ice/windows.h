/* arm/multi-ice/windows.h */
/* not sure what this header is supposed to contain... */

#ifndef _ARM_MULTI_ICE_WINDOWS_H
#define _ARM_MULTI_ICE_WINDOWS_H 1

#include "host.h"

#if defined(_WIN32) || defined(__CYGWIN32__) || defined(HAVE_BASETSD_H)
# include <basetsd.h>
#else
# ifndef LONG_PTR
#  if defined(_WIN64)
typedef __int64 LONG_PTR;
#  else
typedef long LONG_PTR;
#  endif /* _WIN64 */
# endif /* !LONG_PTR */
# ifndef UINT_PTR
#  if defined(_WIN64)
typedef unsigned __int64 UINT_PTR;
#  else
typedef unsigned int UINT_PTR;
#  endif /* _WIN64 */
# endif /* !UINT_PTR */
#endif /* _WIN32 || __CYGWIN32__ || HAVE_BASETSD_H */

#if defined(_WIN32) || defined(__CYGWIN32__) || defined(HAVE_INTSAFE_H)
# include <intsafe.h>
#else
# ifndef DWORD
typedef unsigned long DWORD;
# endif /* !DWORD */
#endif /* _WIN32 || __CYGWIN32__ || HAVE_INTSAFE_H */

#if defined(_WIN32) || defined(__CYGWIN32__) || defined(HAVE_WINNT_H)
# include <winnt.h>
#else
# ifndef PVOID
typedef void *PVOID;
# endif /* !PVOID */
# ifndef HANDLE
typedef PVOID HANDLE;
# endif /* !HANDLE */
# ifndef LONG
typedef long LONG;
# endif /* !LONG */
# ifndef CONST
#  define CONST const /* actually from <windef.h>, but needed here */
# endif /* !CONST */
# ifndef CHAR
typedef char CHAR;
# endif /* !CHAR */
# ifndef __nullterminated
#  define __nullterminated /* (nothing) */
# endif /* !__nullterminated */
# ifndef LPCSTR
typedef __nullterminated CONST CHAR *LPCSTR;
# endif /* !LPCSTR */
# ifndef WCHAR
typedef wchar_t WCHAR;
# endif /* !WCHAR */
# ifndef LPCWSTR
typedef CONST WCHAR *LPCWSTR;
# endif /* !LPCWSTR */
# ifndef LPCTSTR
#  ifdef UNICODE
typedef LPCWSTR LPCTSTR;
#  else
typedef LPCSTR LPCTSTR;
#  endif /* UNICODE */
# endif /* !LPCTSTR */
#endif /* _WIN32 || __CYGWIN32__ || HAVE_WINNT_H */

#if defined(_WIN32) || defined(__CYGWIN32__) || defined(HAVE_WINDEF_H)
# include <windef.h>
#else
# if !defined(BOOL) && !defined(__OBJC__)
typedef int BOOL;
# endif /* !BOOL && !__OBJC__ */
# ifndef BYTE
typedef unsigned char BYTE;
# endif /* !BYTE */
# ifndef HBRUSH
typedef HANDLE HBRUSH;
# endif /* !HBRUSH */
# ifndef HICON
typedef HANDLE HICON;
# endif /* !HICON */
# ifndef HCURSOR
typedef HICON HCURSOR;
# endif /* !HCURSOR */
# ifndef HDC
typedef HANDLE HDC;
# endif /* !HDC */
# ifndef HINSTANCE
typedef HANDLE HINSTANCE;
# endif /* !HINSTANCE */
# ifndef HMENU
typedef HANDLE HMENU;
# endif /* !HMENU */
# ifndef HWND
typedef HANDLE HWND;
# endif /* HWND */
# ifndef CALLBACK
#  if defined(__stdcall) || defined(_WIN32)
#   define CALLBACK __stdcall
#  else
#   define CALLBACK /* (nothing) */
#  endif /* __stdcall || _WIN32 */
# endif /* CALLBACK */
# ifndef WINAPI
#  if defined(__stdcall) || defined(_WIN32)
#   define WINAPI __stdcall
#  else
#   define WINAPI /* (nothing) */
#  endif /* __stdcall || _WIN32 */
# endif /* WINAPI */
# if !defined(_RECT) && !defined(RECT) && !defined(PRECT)
typedef struct _RECT {
    LONG left;
    LONG top;
    LONG right;
    LONG bottom;
} RECT, *PRECT;
# endif /* !_RECT && !RECT && !PRECT */
# ifndef LPARAM
typedef LONG_PTR LPARAM;
# endif /* !LPARAM */
# ifndef LRESULT
typedef LONG_PTR LRESULT;
# endif /* !LRESULT */
# ifndef UINT
typedef unsigned int UINT;
# endif /* !UINT */
# ifndef WPARAM
typedef UINT_PTR WPARAM;
# endif /* !WPARAM */
# if !defined(tagPOINT) && !defined(POINT) && !defined(PPOINT) && !defined(LPPOINT)
/* The POINT structure: */
typedef struct tagPOINT
{
    LONG  x;
    LONG  y;
} POINT, *PPOINT, *LPPOINT;
# endif /* !tagPOINT && !POINT && !PPOINT && !LPPOINT */
#endif /* _WIN32 || __CYGWIN32__ || HAVE_WINDEF_H */

#if defined(_WIN32) || defined(__CYGWIN32__) || defined(HAVE_WINCON_H)
# include <wincon.h>
#else
/* handler routine control signal types: */
# ifndef CTRL_C_EVENT
#  define CTRL_C_EVENT 0
# endif /* !CTRL_C_EVENT */
# ifndef CTRL_BREAK_EVENT
#  define CTRL_BREAK_EVENT 1
# endif /* !CTRL_BREAK_EVENT */
# ifndef CTRL_CLOSE_EVENT
#  define CTRL_CLOSE_EVENT 2
# endif /* !CTRL_CLOSE_EVENT */
# ifndef CTRL_LOGOFF_EVENT
#  define CTRL_LOGOFF_EVENT 5
# endif /* !CTRL_LOGOFF_EVENT */
# ifndef CTRL_SHUTDOWN_EVENT
#  define CTRL_SHUTDOWN_EVENT 6
# endif /* !CTRL_SHUTDOWN_EVENT */
# ifndef PHANDLER_ROUTINE
/* needs to go after <windef.h> stuff for WINAPI definition: */
typedef BOOL (WINAPI *PHANDLER_ROUTINE)(DWORD dwCtrlType);
# endif /* !PHANDLER_ROUTINE */
#endif /* _WIN32 || __CYGWIN32__ || HAVE_WINCON_H */

#if defined(_WIN32) || defined(__CYGWIN32__) || defined(HAVE_WINUSER_H)
# include <winuser.h>
#else
# ifndef WNDPROC
typedef LRESULT (CALLBACK *WNDPROC)(HWND,UINT,WPARAM,LPARAM);
# endif /* !WNDPROC */
# if !defined(tagWNDCLASS) && !defined(WNDCLASS) && !defined(PWNDCLASS)
typedef struct tagWNDCLASS {
    UINT      style;
    WNDPROC   lpfnWndProc;
    int       cbClsExtra;
    int       cbWndExtra;
    HINSTANCE hInstance;
    HICON     hIcon;
    HCURSOR   hCursor;
    HBRUSH    hbrBackground;
    LPCTSTR   lpszMenuName;
    LPCTSTR   lpszClassName;
} WNDCLASS, *PWNDCLASS;
# endif /* !tagWNDCLASS && !WNDCLASS && PWNDCLASS */
# ifndef PM_REMOVE
#  define PM_REMOVE 0x0001
# endif /* !PM_REMOVE */
# ifndef WM_DESTROY
#  define WM_DESTROY 0x0002
# endif /* !WM_DESTROY */
# ifndef WM_QUIT
#  define WM_QUIT 0x0012
# endif /* !WM_QUIT */
# if !defined(tagMSG) && !defined(MSG) && !defined(PMSG) && !defined(LPMSG)
typedef struct tagMSG
{
    HWND    hwnd;
    UINT    message;
    WPARAM  wParam;
    LPARAM    lParam;
    DWORD     time;
    POINT   pt;
} MSG, *PMSG, *LPMSG;
# endif /* !tagMSG && !MSG && !PMSG && !LPMSG */
# if !defined(tagPAINTSTRUCT) && !defined(PAINTSTRUCT) && !defined(PPAINTSTRUCT)
typedef struct tagPAINTSTRUCT {
  HDC  hdc;
  BOOL fErase;
  RECT rcPaint;
  BOOL fRestore;
  BOOL fIncUpdate;
  BYTE rgbReserved[32];
} PAINTSTRUCT, *PPAINTSTRUCT;
# endif /* !tagPAINTSTRUCT && !PAINTSTRUCT && !PPAINTSTRUCT */
#endif /* _WIN32 || __CYGWIN32__ || HAVE_WINUSER_H */

#endif /* !_ARM_MULTI_ICE_WINDOWS_H */

/* EOF */
