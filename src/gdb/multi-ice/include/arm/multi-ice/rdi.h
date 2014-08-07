/* rdi.h
 * based on ardi.h in ../rdi-share
 * RDI basic types and defintions
 * Copyright (C) 2001 Free Software Foundation, Inc. */
/*
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
 * RCS $Revision: 1.20 $
 * Checkin $Date: 2000/09/14 15:14:19 $
 * Revising $Author: hbullman $
 */

#ifndef rdi_h
#define rdi_h

#ifndef __ARM_MULTI_ICE_RDI_H
#define __ARM_MULTI_ICE_RDI_H 1

#include "host.h"

typedef unsigned32 ARMword;

#endif /* !__ARM_MULTI_ICE_RDI_H */

/*
 * Currently supports RDI 1.0 and RDI 1.5 and RDI 1.5.1
 */

#include "armtypes.h"
#include "rdi_stat.h"

#ifndef RDI_VERSION
# define RDI_VERSION 150
#endif

#include "rdi_err.h"            /* RDIError codes */
#include "rdi_info.h"           /* RDIInfo codes */

/*
 * Other RDI values
 */

/*
 * Bits of OpenAgentProc/OpenProc type parameter
 */
#define RDIOpen_BootLevel         0
#define RDIOpen_BootLevelMask    (1 << RDIOpen_BootLevel)
#define RDIOpen_ColdBoot         (0 << RDIOpen_BootLevel)
#define RDIOpen_WarmBoot         (1 << RDIOpen_BootLevel)

#define RDIOpen_CommsReset        1
#define RDIOpen_CommsResetMask   (1 << RDIOpen_CommsReset)

#define RDIOpen_ByteSexShift      2
#define RDIOpen_ByteSexMask      (3 << RDIOpen_ByteSexShift)
#define RDIOpen_LittleEndian     (RDISex_Little << RDIOpen_ByteSexShift)
#define RDIOpen_BigEndian        (RDISex_Big << RDIOpen_ByteSexShift)
#define RDIOpen_DontCareEndian   (RDISex_DontCare << RDIOpen_ByteSexShift)

#define RDIOpen_NonStopDebug      4
#define RDIOpen_NonStopDebugMask (1 << RDIOpen_NonStopDebug)

/* The following are defined also in rdi_async.h */
#define RDIOpen_Asynchronous      5
#define RDIOpen_AsynchronousMask (1 << RDIOpen_Asynchronous)

#define RDIOpen_OutOfThreadCallbacks 6
#define RDIOpen_OutOfThreadCallbacksMask (1 << RDIOpen_OutOfThreadCallback)



#define RDIOpen_UseThreads        7
#define RDIOpen_UseThreadsMask   (1 << RDIOpen_UseThreads)
#define RDIOpen_ThreadShift       8
#define RDIOpen_ThreadMask       (0xff << RDIOpen_ThreadShift)

/* Monitor mode: TRUE for monitor; FALSE for halt  - for RDI_OpenProc */
#define RDIOpen_MonitorMode      16
#define RDIOpen_MonitorModeMask  (1 << RDIOpen_MonitorMode)

/* Byte sex */
#define RDISex_Little           0
#define RDISex_Big              1
#define RDISex_DontCare         2

/* The different types of break- and watchpoints */
#define RDIPoint_EQ             0
#define RDIPoint_GT             1
#define RDIPoint_GE             2
#define RDIPoint_LT             3
#define RDIPoint_LE             4
#define RDIPoint_IN             5
#define RDIPoint_OUT            6
#define RDIPoint_MASK           7

#define RDIPoint_16Bit          16  /* 16-bit breakpoint                */
#define RDIPoint_Conditional    32

/* ORRed with point type in extended RDP break and watch messages       */
#define RDIPoint_Inquiry        64
#define RDIPoint_Handle         128 /* messages                         */

#define RDIPoint_8Bit           256  /* 8-bit breakpoint (Zircon)       */

#define RDIWatch_ByteRead       1 /* types of data accesses to watch for*/
#define RDIWatch_HalfRead       2
#define RDIWatch_WordRead       4
#define RDIWatch_ByteWrite      8
#define RDIWatch_HalfWrite      16
#define RDIWatch_WordWrite      32


/* mask values for registers (CPURead and CPUWrite) */
/* ARM */
#define RDIReg_R15Shift         15/* obsolete */
#define RDIReg_PCShift          16
#define RDIReg_CPSRShift        17
#define RDIReg_SPSRShift       18

#define RDIReg_R15              (1L << RDIReg_R15Shift)
#define RDIReg_PC               (1L << RDIReg_PCShift)
#define RDIReg_CPSR             (1L << RDIReg_CPSRShift)
#define RDIReg_SPSR             (1L << RDIReg_SPSRShift)

#define RDINumCPURegs           19
/* Magic value for the "current mode". Other mode numbers are in armtypes.h */
#define RDIMode_Curr            255

/* Piccolo modes *** Use of Piccolo is obsolete *** */
#define RDIMode_Bank0           0
#define RDIMode_Bank1           1
#define RDIMode_Bank2           2
#define RDIMode_Bank3           3
#define RDIMode_Bank4           4
#define RDIMode_Bank5           5
#define RDIMode_Bank6           6

/* Coprocessors */
#define RDINumCPRegs            10 /* current maximum                   */


/* profile map type */
typedef struct {
    ARMword len;
    ARMword map[1];
} RDI_ProfileMap;

/* types for opaque handles - of a defined size */
/* break and watchpoint handles */
typedef unsigned32 RDI_PointHandle;
#define RDI_NoPointHandle        ((RDI_PointHandle)-1L)
/* thread handles */
typedef unsigned32 RDI_ThreadHandle;
#define RDI_NoHandle             ((RDI_ThreadHandle)-1L)

/* A combined pointer type, not yet used in vanilla RDI */
typedef union {
    ARMword *d;
    void (*f)(void);
} RDI_Pointer;

/* other RDI types. HostosInterface is defined in rdi_hif.h */
/* rdi_stat.h typedef's this to just RDI_HostosInterface */
struct RDI_HostosInterface;

/* RDI_DbgState is to be defined by the Debug Controller */
typedef struct RDI_OpaqueDbgStateStr RDI_DbgState;

/* Types used by RDI_AddConfig */
typedef enum {
    RDI_ConfigCPU,
    RDI_ConfigSystem
} RDI_ConfigAspect;

typedef enum {
    RDI_MatchAny,
    RDI_MatchExactly,
    RDI_MatchNoEarlier
} RDI_ConfigMatchType;

/* A type of a function to fill a buffer, used in RDI_LoadAgent */
typedef struct RDI_GetBufferArgStr RDI_GetBufferArg;
typedef char *RDI_GetBufferProc(RDI_GetBufferArg *getbarg, unsigned32 *sizep);

/* A function that can be registered with the Debug Target to allow it
 * to report errors with the underlying transport service back to the
 * Debug Controller. This gives the Debug Controller a chance to
 * intervene before an RDI error is reported. (For example, to
 * completely reset and start again, under user control.)  See
 * rdi_info.h - RDIInfo_AddTimeoutFunc/RDIInfo_RemTimeoutFunc.  The
 * values for errtyp and the return value are one of:
 */
#define RDIErrorProc_TargetTimeout         274
#define RDIErrorProc_UnexpectedTargetBoot  275

typedef int RDI_OnErrorProc(const void *device, int errtyp, void *data);

/* Type for list of names returned from RDI_CPUNames and RDI_DriverNames */
typedef struct {
    int itemmax;
    char const * const *names;
} RDI_NameList;

/* The actual RDI implementations are in seperate headers */


#if RDI_VERSION == 100
#  include "rdi100.h"
#elif RDI_VERSION == 151
#  include "rdi151.h"
#else
#  include "rdi150.h"
#endif

#endif /* rdi_h */

/* EOF rdi.h */
