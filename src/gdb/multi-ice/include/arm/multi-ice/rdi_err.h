/* RDI Error codes
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
 * RCS $Revision: 1.11 $
 * Checkin $Date: 2000/04/10 14:20:06 $
 * Revising $Author: hbullman $
 */

#ifndef rdi_err_h
#define rdi_err_h

/*
 * Error codes
 */

/* THESE ARE DUPLICATED IN adp.h */ /* HGB: NOT ANY MORE */

/*
 * Errors pertinent to all RDI versions
 */

typedef int RDI_Error;

#define RDIError_NoError                ((RDI_Error)0)

#define RDIError_Reset                  ((RDI_Error)1)
#define RDIError_UndefinedInstruction   ((RDI_Error)2)
#define RDIError_SoftwareInterrupt      ((RDI_Error)3)
#define RDIError_PrefetchAbort          ((RDI_Error)4)
#define RDIError_DataAbort              ((RDI_Error)5)
#define RDIError_AddressException       ((RDI_Error)6)
#define RDIError_IRQ                    ((RDI_Error)7)
#define RDIError_FIQ                    ((RDI_Error)8)
#define RDIError_Error                  ((RDI_Error)9)
#define RDIError_BranchThrough0         ((RDI_Error)10)

#define RDIError_NotInitialised         ((RDI_Error)128)
#define RDIError_UnableToInitialise     ((RDI_Error)129)
#define RDIError_WrongByteSex           ((RDI_Error)130)
#define RDIError_UnableToTerminate      ((RDI_Error)131)
#define RDIError_BadInstruction         ((RDI_Error)132)
#define RDIError_IllegalInstruction     ((RDI_Error)133)
#define RDIError_BadCPUStateSetting     ((RDI_Error)134)
#define RDIError_UnknownCoPro           ((RDI_Error)135)
#define RDIError_UnknownCoProState      ((RDI_Error)136)
#define RDIError_BadCoProState          ((RDI_Error)137)
#define RDIError_BadPointType           ((RDI_Error)138)
#define RDIError_UnimplementedType      ((RDI_Error)139)
#define RDIError_BadPointSize           ((RDI_Error)140)
#define RDIError_UnimplementedSize      ((RDI_Error)141)
#define RDIError_NoMorePoints           ((RDI_Error)142)
#define RDIError_BreakpointReached      ((RDI_Error)143)
#define RDIError_WatchpointAccessed     ((RDI_Error)144)
#define RDIError_NoSuchPoint            ((RDI_Error)145)
#define RDIError_ProgramFinishedInStep  ((RDI_Error)146)
#define RDIError_UserInterrupt          ((RDI_Error)147)
#define RDIError_CantSetPoint           ((RDI_Error)148)
#define RDIError_IncompatibleRDILevels  ((RDI_Error)149)

#define RDIError_CantLoadConfig         ((RDI_Error)150)
#define RDIError_BadConfigData          ((RDI_Error)151)
#define RDIError_NoSuchConfig           ((RDI_Error)152)
#define RDIError_BufferFull             ((RDI_Error)153)
#define RDIError_OutOfStore             ((RDI_Error)154)
#define RDIError_NotInDownload          ((RDI_Error)155)
#define RDIError_PointInUse             ((RDI_Error)156)
#define RDIError_BadImageFormat         ((RDI_Error)157)
#define RDIError_TargetRunning          ((RDI_Error)158)
#define RDIError_DeviceWouldNotOpen     ((RDI_Error)159)
#define RDIError_NoSuchHandle           ((RDI_Error)160)
#define RDIError_ConflictingPoint       ((RDI_Error)161)

#define RDIError_TargetBroken           ((RDI_Error)162)
#define RDIError_TargetStopped          ((RDI_Error)163)

/*
 * RDI 1.51 errors
 */

#define RDIError_BadValue               ((RDI_Error)164)
#define RDIError_Unset                  ((RDI_Error)165)

/*
 * RDI 1.51 rt/tx errors
 */

#define RDIError_NotAllowedWhilstExecuting ((RDI_Error)166)
#define RDIError_BadFormat                 ((RDI_Error)167)
#define RDIError_Executing                 ((RDI_Error)168)
#define RDIError_ExecutingLittleEndian     ((RDI_Error)169)
#define RDIError_ExecutingBigEndian        ((RDI_Error)170)
#define RDIError_ReentrantDuringProxy      ((RDI_Error)171)
#define RDIError_Busy                      ((RDI_Error)172)

/*
 * RDI 1.51 asynch errors
 */
#define RDIError_NotExecuting              ((RDI_Error)173)
#define RDIError_ProgramFinished           ((RDI_Error)174)
#define RDIError_AlreadyExecuting          ((RDI_Error)175)
#define RDIError_NoSuchCallback            ((RDI_Error)176)

/*
 * Errors pertinent to all versions
 */

#define RDIError_LinkTimeout            ((RDI_Error)200)  /* data link timeout error */
#define RDIError_OpenTimeout            ((RDI_Error)201)  /* open timeout (c.f link timeout, on an open */
#define RDIError_LinkDataError          ((RDI_Error)202)  /* data error (read/write) on link */
#define RDIError_Interrupted            ((RDI_Error)203)  /* (e.g. boot) interrupted */
#define RDIError_NoModules              ((RDI_Error)204)  /* e.g. empty target */

#define RDIError_LittleEndian           ((RDI_Error)240)
#define RDIError_BigEndian              ((RDI_Error)241)
#define RDIError_SoftInitialiseError    ((RDI_Error)242)

/* New error: ReadOnly, when state can't be written (RDI 1.50 and later) */
#define RDIError_ReadOnly               ((RDI_Error)252)
#define RDIError_InsufficientPrivilege  ((RDI_Error)253)
#define RDIError_UnimplementedMessage   ((RDI_Error)254)
#define RDIError_UndefinedMessage       ((RDI_Error)255)

/* Range of numbers that are reserved for RDI implementations */
#define RDIError_TargetErrorBase        ((RDI_Error)256)
#define RDIError_TargetErrorTop         ((RDI_Error)(0x1000 - 1))

#endif /* rdi_err_h */
