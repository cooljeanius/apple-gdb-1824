/* RDI host operating system interface
 Copyright (C) 2001 Free Software Foundation, Inc.


 This file is part of GDB.


 GDB is free software; you can redistribute it and/or modify it under
 the terms of the GNU General Public License as published by the Free
 Software Foundation; either version 2, or (at your option) any later
 version.


 GDB is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or
 FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 for more details.


 You should have received a copy of the GNU General Public License
 along with GDB; see the file COPYING.  If not, write to the Free
 Software Foundation, 59 Temple Place - Suite 330, Boston, MA
 02111-1307, USA.  */

/*
 * RCS $Revision: 1.10 $
 * Checkin $Date: 2000/08/09 09:52:58 $
 * Revising $Author: hbullman $
 */

#ifndef rdi_hif_h
#define rdi_hif_h

#include "host.h"

#if defined(__STDC__) || defined(ANSI) || defined (__cplusplus) \
|| defined(_WIN32)
#  include <stdarg.h>
#else
#  include <varargs.h>
#endif

/*
 * Definition: RDI_HIF_BLOCKSIZE
 *    Purpose: The maximum number of bytes that should be passed in a call
 *             to RDI_Hif_Write.
 */
#define RDI_HIF_BLOCKSIZE 4096


typedef struct RDI_Hif_DbgArgStr RDI_Hif_DbgArg;
typedef void RDI_Hif_DbgPrint(
                              RDI_Hif_DbgArg *arg, const char *format, va_list ap);
typedef void RDI_Hif_DbgPause(RDI_Hif_DbgArg *arg);

typedef struct RDI_Hif_HostosArgStr RDI_Hif_HostosArg;
typedef void RDI_Hif_WriteC(RDI_Hif_HostosArg *arg, int c);
typedef int RDI_Hif_ReadC(RDI_Hif_HostosArg *arg);
typedef int RDI_Hif_Write(RDI_Hif_HostosArg *arg, char const *buffer, int len);
typedef char *RDI_Hif_GetS(RDI_Hif_HostosArg *arg, char *buffer, int len);

typedef struct RDI_Hif_ResetArgStr RDI_Hif_ResetArg;
typedef void RDI_Hif_ResetProc(RDI_Hif_ResetArg *arg);

/*
 * Added for RDI 1.51
 */
#if RDI_VERSION == 151

#define RDI_MB_OK               0
#define RDI_MB_OKCANCEL         1
#define RDI_MB_YESNO            2
#define RDI_MB_YESNOCANCEL      3
#define RDI_MB_RETRYCANCEL      4
#define RDI_MB_ABORTRETRYIGNORE 5
#define RDI_MB_STATUS           6 /* No response required; agent scope only.
* Maximum string length 32 chars - see spec
*/

#define RDI_MBICON_EXCLAMATION  0x100
#define RDI_MBICON_INFORMATION  0x200
#define RDI_MBICON_QUESTION     0x300
#define RDI_MBICON_STOP         0x400

typedef enum
{
    RDI_MBANSWER_OK,
    RDI_MBANSWER_CANCEL,
    RDI_MBANSWER_YES,
    RDI_MBANSWER_NO,
    RDI_MBANSWER_ABORT,
    RDI_MBANSWER_RETRY,
    RDI_MBANSWER_IGNORE
} RDI_Hif_UserMessageReturn;

typedef struct RDI_Hif_UserMessageArg RDI_Hif_UserMessageArg;
typedef RDI_Hif_UserMessageReturn RDI_Hif_UserMessageProc(
                                                          RDI_Hif_UserMessageArg *arg, char const *msg, uint32 type);
#endif

struct RDI_HostosInterface {
    RDI_Hif_DbgPrint *dbgprint;
    RDI_Hif_DbgPause *dbgpause;
    RDI_Hif_DbgArg *dbgarg;

    RDI_Hif_WriteC *writec;
    RDI_Hif_ReadC *readc;
    RDI_Hif_Write *write;
    RDI_Hif_GetS *gets_f;
    RDI_Hif_HostosArg *hostosarg;

    RDI_Hif_ResetProc *reset;
    RDI_Hif_ResetArg *resetarg;

#if RDI_VERSION == 151
    RDI_Hif_UserMessageProc *message;
    RDI_Hif_UserMessageArg  *messagearg;
#endif

};

#endif
