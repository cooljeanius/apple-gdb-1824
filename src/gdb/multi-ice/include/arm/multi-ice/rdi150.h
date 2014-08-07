/* rdi150.h: RDI 1.5.0 definitions
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
 * RCS $Revision: 1.25 $
 * Checkin $Date: 2000/10/05 13:47:15 $
 * Revising $Author: hbullman $
 */

#ifndef rdi150_h
#define rdi150_h

#include "toolconf.h"

#if (RDI_VERSION == 150)
#  define RDI150_OpenAgentProc RDI_OpenAgentProc
#  define RDI150_CloseAgentProc RDI_CloseAgentProc
#  define RDI150_OpenProc RDI_OpenProc
#  define RDI150_CloseProc RDI_CloseProc
#  define RDI150_ReadProc RDI_ReadProc
#  define RDI150_WriteProc RDI_WriteProc
#  define RDI150_CPUReadProc RDI_CPUReadProc
#  define RDI150_CPUWriteProc RDI_CPUWriteProc
#  define RDI150_CPReadProc RDI_CPReadProc
#  define RDI150_CPWriteProc RDI_CPWriteProc
#  define RDI150_SetBreakProc RDI_SetBreakProc
#  define RDI150_ClearBreakProc RDI_ClearBreakProc
#  define RDI150_SetWatchProc RDI_SetWatchProc
#  define RDI150_ClearWatchProc RDI_ClearWatchProc
#  define RDI150_ExecuteProc RDI_ExecuteProc
#  define RDI150_StepProc RDI_StepProc
#  define RDI150_InfoProc RDI_InfoProc
#  define RDI150_PointInquiryProc RDI_PointInquiryProc
#  define RDI150_AddConfigProc RDI_AddConfigProc
#  define RDI150_LoadConfigDataProc RDI_LoadConfigDataProc
#  define RDI150_SelectConfigProc RDI_SelectConfigProc
#  define RDI150_NameListProc RDI_NameListProc
#  define RDI150_ErrMessProc RDI_ErrMessProc
#  define RDI150_LoadAgentProc RDI_LoadAgentProc
#  define RDI150_TargetIsDeadProc RDI_TargetIsDeadProc
#  define RDI150_ProcVec RDI_ProcVec
#  define RDI150_ConfigPointer RDI_ConfigPointer
#endif /* RDI_VERSION == 150 */


/*
 * RDI 1.50:  An introduction
 * --------------------------
 *
 * This file, which originally contained the definition for RDI 1, has been
 * extended (through conditional compilation via the RDI_VERSION macro) to
 * additionally include the definition of RDI 1.50.
 *
 * To use RDI 1, then RDI_VERSION should either be undefined, or be defined thus
 *     #define RDI_VERSION 100
 *
 * To use RDI 1.50, then RDI_VERSION should be defined this
 *     #define RDI_VERSION 150
 *
 * CHANGES
 * -------
 * Most RDI functions have had one additional parameter added
 *  RDI_ModuleHandle mh
 * This qualifies the functions for use in a debuggee with:
 *  multi ARM processors
 *  coprocssors that can run their own instructions
 *
 * BACKWARD COMPATIBILITY
 * ----------------------
 * The new framework can use existing debuggees.
 * To this effect error codes, structures, typedefs and flag bit positions
 * must remain as they are. The new RDI provides additional functionality
 * with minimal change.
 *
 * THREADS
 * -------
 * Threads will be dealt with in a later OS dependent interface.
 * ThreadHandles are included, for future use, in break and watch points.
 * Note: There is no method for obtaining ThreadHandles in the RDI.
 *
 */

/* opaque handle types */
typedef struct RDI_OpaqueAgentStateStr RDI_AgentState;
typedef RDI_AgentState *RDI_AgentHandle;

typedef struct RDI_OpaqueModuleStateStr RDI_ModuleState;
typedef RDI_ModuleState *RDI_ModuleHandle;

/* type used for RDIInfo_Modules */
typedef struct RDI_ModuleDesc {
    char type[8];           /* fill in with name - e.g. "ARM", "ETM" */
    char name[8];           /* unique name of this instance e.g. "ARM_1" */
    struct RDI_ProcVec const *rdi;      /* NULL for "same as agent" */
    RDI_ModuleHandle handle;
} RDI_ModuleDesc;

/*
 * 27 July 2000
 *
 * Additional definition for RDI_ModuleDesc explicitly showing the small,
 * medium and large formats.
 *
 * RDI_ModuleDesc and RDI_ModuleDesc_Extra MUST remain binary compatible.
 */
typedef struct RDI_ModuleDesc_Extra
{
    char type[8];
    union {
        char shortname[8];
        struct {
            char quote;
            char string[7];
        } mediumname; /* Use of mediumname to be deprecated where possible */
        struct {
            char s[4];
            char const *string;
        } longname;
    } name;
    struct RDI_ProcVec const *rdi;
    RDI_ModuleHandle handle;

} RDI_ModuleDesc_Extra;

/**********************************************************************/

/*
 * RDI Implementation Prototypes
 */


/*
 *  Function:   RDI_OpenAgent
 *   Purpose:   Initialisation routine for debug agent, which supports both
 *              hot and cold calls; the cold call is guaranteed to be the
 *              very first call made from the debugger into the debuggee.
 *
 *      Input:  type
 *                  b0          0   cold start
 *                              1   warm start
 *                  b1          1   reset the comms link
 *                  b2          0/1 little/big endian
 *                  b3          1   return endian
 *
 *              config          structure containing configuration
 *                              information
 *
 *              hostif          used for reading/writing to a console
 *
 *              dbg_state       handle to identify the current Debugger
 *                              state, or an umbrella-agent if toolconf
 *                              tag "CONTROLLER_ISAGENT" is set.
 *
 *     In/Out:  agent           cold start: passed in as NULL, returns
 *                                          pointer to debug agent instance
 *
 *                              warm start: pointer to debug agent instance
 *
 *  Returns:                                 b3     b2
 *      Error:  RDIError_SoftInitialiseError X      X   If things go wrong!
 *      Error:  RDIError_WrongByteSex        0      E   bad match
 *      OK:     RDIError_NoError             0      E   match
 *      OK:     RDIError_LittleEndian        1      X
 *      OK:     RDIError_BigEndian           1      X
 *
 * Post-conditions: If no error occured, then *agent will contain a pointer
 *                  to the debug agent instance (newly created if a cold
 *                  start), and all parts of the system other than the
 *                  Processor modules will have seen the open too.
 */

struct RDI_LibraryData;  /* Opaque type - see rditools.h */

/* This is passed to sub-agents by an umbrella-agent during openagent. */
typedef struct RDI_DbgStateForSubAgent
{
    size_t size; /* Must be sizeof(RDI_DbgStateForSubAgent) */
    RDI_ModuleDesc *agent_desc; /* Really an Agent */
    RDI_ModuleDesc *parent_desc; /* Really a Module */
    /* This allows a sub-agent to use its parent's set of DLLs */
    struct RDI_LibraryData *lib_handle;
} RDI_DbgStateForSubAgent;

typedef toolconf RDI150_ConfigPointer;

typedef int RDI150_OpenAgentProc(RDI_AgentHandle *agent, unsigned type,
                                 RDI150_ConfigPointer config,
                                 struct RDI_HostosInterface const *i,
                                 RDI_DbgState *dbg_state);

/*
 * Function:    RDI_CloseAgent
 *  Purpose:    Equivalent to the debuggee destructor.
 *              Ensure that all processors are also closed down
 *              Release any memory and close down the emulator.
 *              Only an RDI_open call can follow.
 *
 *  Params:     agent - handle of agent to close down
 *
 *  Returns:    RDIError_NoError
 */
typedef int RDI150_CloseAgentProc(RDI_AgentHandle agent);

/*
 * Function:    RDI_OpenProc
 *  Purpose:    This is the Initialization routine and as such is the first
 *              call made to the RDI. It allows the program to set up the
 *              initial debuggee working environment. It will be called
 *              later if the emulator is to be reset. There are hot and
 *              cold calls. On the very first cold call is equivalent to a
 *              constructor when memory should be claimed.
 *              The initialization also provides a console window for
 *              reading and writing text. This can be used now and in the
 *              future for messages. The call provides information on the
 *              settings on the debugger.
 *
 *  Pre-conditions:
 *
 *  Params:
 *      Input:  handle          Processor module handle
 *
 *              type
 *                  b0          0   cold start
 *                              1   warm start
 *                  b1          1   reset the comms link
 *                  b2          0/1 little/big endian
 *                  b3          1   return endian
 *
 *              config          structure containing configuration
 *                              information
 *
 *              hostif          used for reading/writing to a console
 *
 *              dbg_state       handle to identify the current Debugger
 *                              state
 *
 *  Returns:                                 b3     b2
 *      Error:  RDIError_SoftInitialiseError X      X   If things go wrong!
 *      Error:  RDIError_WrongByteSex        0      E   bad match
 *      OK:     RDIError_NoError             0      E   match
 *      OK:     RDIError_LittleEndian        1      X
 *      OK:     RDIError_BigEndian           1      X
 */
typedef int RDI150_OpenProc(RDI_ModuleHandle rdi_handle, unsigned type,
                            RDI150_ConfigPointer config,
                            struct RDI_HostosInterface const *i,
                            RDI_DbgState *dbg_state);

/*
 * Function:    RDI_CloseProc
 *  Purpose:    Close down an individual processor.
 *              > ISSUE <
 *              Behaviour is currently undefined if only a subset of
 *              processors are 'open' and execution is requested.
 *
 *  Params:     mh - Handle of processor to close down
 *
 *  Returns:    RDIError_NoError
 */
typedef int RDI150_CloseProc(RDI_ModuleHandle mh);

/*
 * Access types for RDI(Read,Write}Proc
 */
typedef enum {
    RDIAccess_Data = 0,
    RDIAccess_Data8,
    RDIAccess_Data16,
    RDIAccess_Data32,
    RDIAccess_Data64,
    RDIAccess_Code = 8,
    RDIAccess_Code8, /* Not used at present */
    RDIAccess_Code16,
    RDIAccess_Code32,
    /* Not used yet - we don't model prefetch units that well. */
    RDIAccess_Code64
} RDI_AccessType;


#ifdef OldCode /* Nothing uses these. */
/*
 * 2000-03-16 dsinclai
 * ARMulator 3 access-function options.
 */
# define RDIAccess_Read 0x10
# define RDIAccess_Write 0x20
# define RDIAccess_ByteLanes 0x40
#endif


/*
 * 1999-12-07
 * UNDER CONSTRUCTION - RDI for NDI
 * (these may become 16 and 32 when we re-align ARMISS and ARMulator).
 */
/* This is >not< a debugger-access, so >do< cycle-count it and
 * do not use the debugger's endianness. */
#define RDI_AccessType_NotForDebugger 0x100
/* This is a debugger-access to physical memory, so do NOT address-translate */
#define RDI_AccessType_Physical 0x200
/* This is an access between one emulated module and another. */
#define RDI_AccessType_HostEndian 0x400

/*
 * Function:    RDI_ReadProc
 *  Purpose:    Read memory contents in byte format from the debuggee.
 *              read *nbytes of data from source address and write to dest
 *              in the caller's address space using the processor specified.
 *
 *              Assuming nothing went wrong with the transfer, the debuggee
 *              will normally return RDIError_NoError.  However, if the
 *              debuggee has changed its byte sex, it should return a
 *              notification of this change, i.e. either RDIError_BigEndian
 *              or RDI_ErrorLittleEndian; the caller must then acknowledge
 *              this notification by calling RDI_Info RDIInfo_AckByteSex.
 *              The debuggee will keep returning this notification until an
 *              acknowledgement has been received.
 *
 *  Params:
 *      Input:  mh          handle identifies processor
 *
 *              source      address
 *
 *              access      type and width of access
 *
 *      In/Out: *nbytes     In:  number of bytes to read
 *                          Out: number of bytes actually read
 *
 *      Output: *dest       destination buffer
 *
 *  Returns:
 *      OK:     RDIError_NoError
 *              RDIError_BigEndian
 *              RDIError_LittleEndian
 *      Error:  RDIError_DataAbort  (if a memory exception occurs)
 */
typedef int RDI150_ReadProc(RDI_ModuleHandle mh, ARMword source,
                            void *dest, unsigned *nbytes,
                            RDI_AccessType type);


/*
 * Function:    RDI_WriteProc
 *  Purpose:    Write memory contents in byte format to the debuggee.
 *              read *nbytes of data from source address in the caller's
 *              address space, and write to dest using the processor specified.
 *
 *              Assuming nothing went wrong with the transfer, the debuggee
 *              will normally return RDIError_NoError.  However, if the
 *              debuggee has changed its byte sex, it should return a
 *              notification of this change, i.e. either RDIError_BigEndian
 *              or RDI_ErrorLittleEndian; the caller must then acknowledge
 *              this notification by calling RDI_Info RDIInfo_AckByteSex.
 *              The debuggee will keep returning this notification until an
 *              acknowledgement has been received.
 *
 *  Params:
 *      Input:  mh          handle identifies processor
 *
 *              source      source buffer
 *
 *              dest        destination address
 *
 *              access      type and width of access
 *
 *      In/Out: *nbytes     In:  number of bytes to write
 *                          Out: number of bytes actually written
 *
 *  Returns:
 *      OK:     RDIError_NoError
 *              RDIError_BigEndian
 *              RDIError_LittleEndian
 *      Error:  RDIError_DataAbort  (if a memory exception occurs)
 */
typedef int RDI150_WriteProc(RDI_ModuleHandle mh, void const *source,
                             ARMword dest, unsigned *nbytes,
                             RDI_AccessType type);

/*
 * Function:    RDI_CPUreadProc
 *  Purpose:    Read the register content from the specified processor.
 *              The mask specifies which register(s) are to be transferred
 *              as 32-bit ARMwords.
 *              The mask has been extended to support more than 32 registers
 *  Params:
 *      Input:  mh          handle identifies processor
 *
 *      Input:  mode        mode defines register context
 *                          ARM
 *                              RDIMode_Curr    use the current mode
 *                              Other values match the mode bits of the PSR.
 *                          PICCOLO  *** Use of Piccolo is obsolete ***
 *                              RDIModeBankX    Transfer registers
 *                                              from bankX
 *
 *      Input:  mask[]      bits defining which registers are to be
 *                          transferred. These bits can be ORed to read any
 *                          combination of registers.
 *                          ARM
 *                              b0 - b14    r0 - r14
 *                              b15         Reserved.  Bit 15 must be clear;
 *                                          targets must ignore bit 15.
 *                              RDIReg_PC   PC
 *                              RDIReg_CPSR CPSR register
 *                              RDIReg_SPSR SPSR register
 *                          PICCOLO  *** Use of Piccolo is obsolete ***
 *                              b0-b<x>     mask of which words in the
 *                                          bank to transfer
 *
 *      Output: state[]     destination array of ARMwords
 *
 *  Returns:
 *      OK:     RDIError_NoError
 *      Error:  error code
 */
typedef int RDI150_CPUReadProc(RDI_ModuleHandle mh, unsigned mode,
                               unsigned32 mask, ARMword *state);

/*
 * Function:    RDI_CPUwriteProc
 *  Purpose:    Write the register content to the specified processor.
 *              The mask specifies which register(s) are to be transferred
 *              as 32-bit ARMwords. The mask has been extended to support
 *              more than 32 registers in RDI2.
 *
 *  Params:
 *      Input:  mh          handle identifies processor
 *      Input:  mode        mode defines register context
 *                          ARM
 *                              RDIMode_Curr    use the current mode
 *                              Other values match the mode bits of the PSR.
 *                          PICCOLO  *** Use of Piccolo is obsolete ***
 *                              As for RDI_CPUreadProc
 *      Input:  mask[]      bits defining which registers are to be
 *                          transferred. These bits can be ORed to read any
 *                          combination of registers.
 *                          ARM
 *                              b0 - b14    r0 - r14
 *                              b15         Reserved.  Bit 15 must be clear;
 *                                          targets must ignore bit 15.
 *                              RDIReg_PC   PC
 *                              RDIReg_CPSR CPSR register
 *                              RDIReg_SPSR SPSR register
 *                          PICCOLO  *** Use of Piccolo is obsolete ***
 *                              As for RDI_CPUreadProc
 *      Input:  state[]     source array of ARMwords
 *      InOut:  none
 *      Output: none
 *
 *  Returns:
 *         OK:  RDIError_NoError
 *     Not OK:  RDIError_ReadOnly - if writing is not allowed at this time
 *      Error:  error code
 */
typedef int RDI150_CPUWriteProc(RDI_ModuleHandle mh, unsigned mode,
                                unsigned32 mask, ARMword const *state);

/*
 * Function:    RDI_CPreadProc
 *  Purpose:    Read the register content from the specified coprocessor.
 *              The mask specifies which register(s) are to be transferred
 *              as 32-bit ARMwords. The mask has been extended to support
 *              more than 32 registers in RDI2.
 *
 *              Each coprocessor will have its own register set defined in
 *              another header file. The actual registers transferred, and
 *              their size is dependent on the coprocessor specified.
 *
 *  Params:
 *      Input:  mh          handle identifies processor
 *      Input:  CPnum       coprocessor number for this processor
 *      Input   mask[]      bits define which registers are to be transferred
 *      InOut:  none
 *      Output: state[]     destination array for register values
 *
 *  Returns:
 *      OK:     RDIError_NoError
 *      Error:  error code
 */
typedef int RDI150_CPReadProc(RDI_ModuleHandle mh, unsigned CPnum,
                              unsigned32 mask, ARMword *state);

/*
 * Function:    RDI_CPwriteProc
 *  Purpose:    Write the register content to the specified coprocessor.
 *              The mask specifies which register(s) are to be transferred
 *              as 32-bit ARMwords. The mask has been extended to support
 *              more than 32 registers in RDI2.
 *
 *              Each coprocessor will have its own register set defined in
 *              another header file. The actual registers transferred, and
 *              their size is dependent on the coprocessor specified.
 *
 *  Params:
 *      Input:  mh          handle identifies processor
 *      Input:  CPnum       coprocessor number for this processor
 *      Input   mask[]      bits define which registers are to be transferred
 *      Input:  state[]     source array of register values
 *      InOut:  none
 *      Output: none
 *
 *  Returns:
 *      OK:     RDIError_NoError
 *      Error:  error code
 */
typedef int RDI150_CPWriteProc(RDI_ModuleHandle mh, unsigned CPnum,
                               unsigned32 mask, ARMword const *state);


/*
 * Function:    RDI_SetBreakProc
 *  Purpose:    Set a breakpoint in the debuggee.
 *
 *  Params:
 *      Input:  mh          handle identifies processor
 *      Input:  address     address of breakpoint
 *      Input:  type        one of RDIPoint_ types
 *
 *   RDIPoint_EQ          equal to address
 *   RDIPoint_GT          greater than address
 *   RDIPoint_GE          greater than or equal to address
 *   RDIPoint_LT          less than address
 *   RDIPoint_LE          less than or equal to address
 *   RDIPoint_IN          in the range from address to (address+bound), incl
 *   RDIPoint_OUT         not in the range from address to (address+bound),incl
 *   RDIPoint_MASK        halt execution if (pc & bound) = address
 *   RDIPoint_16Bit       if set breakpoint is on 16-bit (Thumb) instruction
 *   RDIPoint_Conditional if set breakpoint only occurs when the
 *                        instructions condition is true.
 *
 *      Input:  bound     upper address range
 *      Input:  thread    thread used by OS (set to RDI_NoHandle for global)
 *      InOut:  none
 *      Output: *handle   A handle to identify the breakpoint
 *
 *  Returns:
 *      OK:     RDIError_NoError
 *      OK:     RDIError_NoMorePoints - successful,
 *                                      but no more points available
 *      Error:  RDIError_CantSetPoint
 */
typedef int RDI150_SetBreakProc(RDI_ModuleHandle mh, ARMword address,
                                unsigned type, ARMword bound,
                                RDI_ThreadHandle thread,
                                RDI_PointHandle *handle);

/*
 * Function:    RDI_ClearBreakProc
 *  Purpose:    Clear a breakpoint in the debuggee.
 *
 *  Params:
 *      Input:  mh          identifies processor
 *      Input:  handle      identifies breakpoint
 *      InOut:  none
 *      Output: none
 *
 *  Returns:
 *      OK:     RDIError_NoError
 *      Error:  RDIError_NoSuchPoint
 */
typedef int RDI150_ClearBreakProc(RDI_ModuleHandle mh,
                                  RDI_PointHandle handle);

/*
 * Function:    RDI_SetWatchProc
 *  Purpose:    Set a watchpoint in the debuggee.
 *
 *  Params:
 *      Input:  mh          handle identifies processor
 *      Input:  address     address of watchpoint
 *      Input:  type        one of RDIPoint_ types
 *
 *   RDIPoint_EQ          equal to address
 *   RDIPoint_GT          greater than address
 *   RDIPoint_GE          greater than or equal to address
 *   RDIPoint_LT          less than address
 *   RDIPoint_LE          less than or equal to address
 *   RDIPoint_IN          in the range from address to (address+bound), inclus
 *   RDIPoint_OUT         not in the range from address to (address+bound),incl
 *   RDIPoint_MASK        halt execution if (pc & bound) = address
 *
 *      Input:  datatype    one of RDIWatch_ data types
 *      Input:  bound       upper address range
 *      Input:  thread      thread used by OS (set to RDI_NoHandle for global)
 *      InOut:  none
 *      Output: *handle     A handle to identify the watchpoint
 *
 *  Returns:
 *      OK:     RDIError_NoError
 *      OK:     RDIError_NoMorePoints - successful,
 *                                      but no more points available
 *      Error:  RDIError_CantSetPoint
 */
typedef int RDI150_SetWatchProc(RDI_ModuleHandle mh, ARMword address,
                                unsigned type, unsigned datatype,
                                ARMword bound, RDI_ThreadHandle thread,
                                RDI_PointHandle *handle);

/*
 * Function:    RDI_ClearWatchProc
 *  Purpose:    Clear a watchpoint in the debuggee.
 *
 *  Params:
 *      Input:  mh          identifies processor
 *      Input:  handle      identifies watchpoint
 *      InOut:  none
 *      Output: none
 *
 *  Returns:
 *      OK:     RDIError_NoError
 *      Error:  RDIError_NoSuchPoint
 */
typedef int RDI150_ClearWatchProc(RDI_ModuleHandle mh,
                                  RDI_PointHandle handle);

/*
 * Function:    RDI_ExecuteProc
 *  Purpose:    Starts execution of the debuggee from the current state.
 *              The debuggee continues until a natural exit occurs.
 *              The reason for exit is returned.
 *
 *  Params:
 *      Input:  agent   Identifies the debug agent.
 *      Input:  stop_others
 *                      TRUE => Only run the specified processor (*mh).  All
 *                              other processors remain halted
 *                      FALSE =>Start the whole target running
 *
 *      InOut:  *mh
 *          In:         (stop_others == TRUE):
 *                           identifies a specific processor to execute
 *                      (stop_others == FALSE):
 *                           ignored
 *
 *          Out:        Handle of the processor that caused the exit.
 *                      If no specific processor caused the exit, this is the
 *                      handle of any valid processor.
 *
 *      Output: *point  if (return == RDIError_BreakpointReached)
 *                          handle of breakpoint
 *                      if (return == RDIError_WatchpointAccessed)
 *                          handle of watchpoint
 *                      else RDINoHandle
 *                      In the current Demon debuggee the handles are not set.
 *                      Here the debugger is required to fetch the pc to
 *                      determine the point.
 *                      (Demon condition is (pointReached && RDINoHandle))
 *
 *  Returns:
 *      OK:         RDIError_NoError
 *      OK:         RDIError_BreakpointReached    Breakpoint reached
 *      OK:         RDIError_WatchpointAccessed   Watch point accessed
 *      OK:         RDIError_UserInterrupt        User stop via RDISignal_Stop
 *
 *      Vector Catches (see RDIVector_Catch Info call)
 *      OK:         RDIError_BranchThrough0       Branch through zero
 *      OK:         RDIError_UndefinedInstruction Undefined instruction
 *      OK:         RDIError_SoftwareInterrupt    Software interrupt
 *      OK:         RDIError_PrefetchAbort        Prefetch abort
 *      OK:         RDIError_DataAbort            Data abort
 *      OK:         RDIError_AddressException     Address exception
 *      OK:         RDIError_IRQ                  Interrupt (IRQ)
 *      OK:         RDIError_FIQ                  Fast Interrupt (FRQ)
 *      OK:         RDIError_Error                Error (see RDIErrorP)
 *
 *      Error:      RDIError_UnimplementedMessage under the following conditions
 *                      if stop_others = TRUE and *mh = RDINoHandle
 *                  or
 *                      if stop_others = TRUE and stopping of individual
 *                      processors is not supported on the target
 *                  or
 *                      if stop_others = TRUE and the particular configuration
 *                      of running the specified processor and stopping others
 *                      is not permitted, even though other 'stop_others'
 *                      configurations are supported.
 */
typedef int RDI150_ExecuteProc(RDI_AgentHandle agent, RDI_ModuleHandle *mh,
                               bool stop_others, RDI_PointHandle *handle);

/*
 * Function:    RDI_StepProc
 *  Purpose:    Steps the debuggee from the current state.
 *              The debuggee continues until a natural exit occurs
 *              or the specified processor completes its next instruction.
 *              If a processor if not specified (RDINoHandle) then the exit
 *              shall be when any processor reaches its next instruction
 *              completion. The reason for exit is returned.
 *
 *  Params:
 *      Input:  agent   Identifies the debug agent.
 *      Input:  stop_others
 *                      TRUE => Only run the specified processor (*mh).  All
 *                              other processors remain halted
 *                      FALSE =>Start the whole target running
 *
 *      InOut:  *mh
 *          In:         identifies a specific processor to step.  In the absence
 *                      of other exit conditions, the execution will halt when
 *                      the specified processor completes a step.
 *
 *                      if (*mh == RDINoHandle), then in the absence of other
 *                      exit conditions, the execution will halt when any
 *                      processor completes a step.
 *
 *          Out:        Handle of the processor that caused the exit.
 *                      If no specific processor caused the exit, this is the
 *                      handle of any valid processor.
 *
 *      Output: *point  if (return == RDIError_BreakpointReached)
 *                          handle of breakpoint
 *                      if (return == RDIError_WatchpointAccessed)
 *                          handle of watchpoint
 *                      else RDINoHandle
 *                      In the current Demon debuggee the handles are not set.
 *                      Here the debugger is required to fetch the pc to
 *                      determine the point.
 *                      (Demon condition is (pointReached && RDINoHandle))
 *
 *  Returns:
 *      OK:         RDIError_NoError
 *      OK:         RDIError_BreakpointReached    Breakpoint reached
 *      OK:         RDIError_WatchpointAccessed   Watch point accessed
 *      OK:         RDIError_UserInterrupt        User stop via RDISignal_Stop
 *
 *      Vector Catches (see RDIVector_Catch Info call)
 *      OK:         RDIError_BranchThrough0       Branch through zero
 *      OK:         RDIError_UndefinedInstruction Undefined instruction
 *      OK:         RDIError_SoftwareInterrupt    Software interrupt
 *      OK:         RDIError_PrefetchAbort        Prefetch abort
 *      OK:         RDIError_DataAbort            Data abort
 *      OK:         RDIError_AddressException     Address exception
 *      OK:         RDIError_IRQ                  Interrupt (IRQ)
 *      OK:         RDIError_FIQ                  Fast Interrupt (FRQ)
 *      OK:         RDIError_Error                Error (see RDIErrorP)
 *
 *      Error:      RDIError_UnimplementedMessage under the following conditions
 *                      if stop_others = TRUE and *mh = RDINoHandle
 *                  or
 *                      if stop_others = TRUE and stopping of individual
 *                      processors is not supported on the target
 *                  or
 *                      if stop_others = TRUE and the particular configuration
 *                      of running the specified processor and stopping others
 *                      is not permitted, even though other 'stop_others'
 *                      configurations are supported.
 */
typedef int RDI150_StepProc(RDI_AgentHandle agent, RDI_ModuleHandle *mh,
                            bool stop_others, unsigned ninstr,
                            RDI_PointHandle *handle);


/* The argument to rdi_info_proc is either an RDI_ModuleHandle (for the
 * majority of calls) or an RDI_AgentHandle (for a small number of
 * "Agent" calls). Which it is depends on the info code. Unless otherwise
 * stated (above) all calls take a ModuleHandle.
 */

/*
 * Function:    RDI_InfoProc
 *  Purpose:    Used to transfer miscellaneous information between the
 *              Debuger and the Debuggee. The parameters meanings change for
 *              each information type.
 *              Each type is defined in RDI_Info subcodes.
 *
 *  Params:
 *      Input:  mh          handle identifies processor
 *                          if RDINoHandle this is general target information
 *                          (e.g. hadware configuration) ?? This looks wrong ??
 *      Input:  type        Information type code
 *
 *      Use of arg parameters varies dependent on type code.
 *      See each individual definition below.
 *      InOut:  arg1        parameter 1
 *      InOut:  arg2        parameter 2
 *
 *  Returns:
 *      OK:     RDIError_NoError
 *      OK:     codes dependent on type
 *      Error:  codes dependent on type
 *      Error:  RDIError_UnimplementedMessage (unimplemented functions)
 */
typedef int RDI150_InfoProc(void *, unsigned type,
                            ARMword *arg1, ARMword *arg2);

/*
 * Function:    RDI_PointinqProc
 *  Purpose:    Breakpoint or watchpoint inquiry.
 *              Used to check before setbreak or setwatch is used.
 *              Returns what will happen if a corresponding call is made
 *              to setbreak or setwatch.
 *
 *  Params:
 *      Input:  mh          handle identifies processor
 *      Input:  type        see setbreak or setwatch for definition
 *      Input:  datatype    0       breakpoint
 *                          != 0    see setwatch for definition
 *      InOut:  *address
 *          In:             address of point
 *          Out:            set to actual value that will be used
 *      InOut:  *bound
 *          In:             upper address range
 *          Out:            set to actual value that will be used
 *      @@@ MJW notes: it's not passed a ThreadHandle
 *      Output: none
 *
 *  Returns:
 *      OK:     RDIError_NoError
 *      Error:  RDIError_NoMorePoints (if cannot set)
 */
typedef int RDI150_PointInquiryProc(RDI_ModuleHandle, ARMword *address,
                                    unsigned type, unsigned datatype,
                                    ARMword *bound);
/*
 * Function:    RDI_AddConfigProc
 *  Purpose:    Declares the size of a config block about to be loaded.
 *              Only use if RDIInfo_DownLoad returns no error.
 *
 *  Params:
 *      Input:  agent   handle identifies degug agent
 *
 *              nbytes  size of configuration block to follow
 *
 *  Returns:
 *      OK:     RDIError_NoError
 *      Error:  RDIError_UnimplementedMessage (unimplemented functions)
 */
typedef int RDI150_AddConfigProc(RDI_AgentHandle agent, unsigned32 nbytes);

/*
 * Function:    RDI_LoadConfigDataProc
 *  Purpose:    Loads the config block.
 *              The block specifies target-dependent information to the
 *              debuggee. (e.g. ICEman)
 *              Only use if RDIInfo_DownLoad returns no error.
 *
 *  Params:
 *      Input:  agent   handle identifies debug agent
 *
 *              nBytes  size of block (as declared in addconfig)
 *
 *              data    source of block
 *
 *  Returns:
 *      OK:     RDIError_NoError
 *      Error:  RDIError_UnimplementedMessage (unimplemented functions)
 */
typedef int RDI150_LoadConfigDataProc(RDI_AgentHandle agent,
                                      unsigned32 nbytes, char const *data);

/*
 * Function:    RDI_SelectConfigProc
 *  Purpose:    Selects which of the loaded config blocks should be used
 *              and then reinitialises the Debuggee to use the selected
 *              config data.
 *              Only use if RDIInfo_DownLoad returns no error.
 *
 *  Params:
 *      Input:  mh          handle identifies processor
 *                          > ISSUE < Specification says 'processor', but
 *                          implmentation says 'agent'.  Surely processor
 *                          is required, in case the aspect = ConfigCPU.
 *
 *              aspect      RDI_ConfigCPU or RDI_ConfigSystem
 *
 *              name        name of the configuration to be selected
 *
 *              matchtype   specifies exactly how the version number
 *                          must be matched
 *                              RDI_MatchAny
 *                              RDI_MatchExactly
 *                              RDI_MatchNoEarlier
 *
 *              versionreq  the version number requested
 *
 *      Output: *versionp   the version actually selected
 *
 *  Returns:
 *      OK:     RDIError_NoError
 *      Error:  RDIError_UnimplementedMessage (unimplemented functions)
 */
typedef int RDI150_SelectConfigProc(RDI_AgentHandle,
                                    RDI_ConfigAspect aspect,
                                    char const *name,
                                    RDI_ConfigMatchType matchtype,
                                    unsigned versionreq,
                                    unsigned *versionp);

/*
 * Function:    RDI_LoadAgentProc
 *  Purpose:    Loads a replacement EmbeddedICE ROM image, and starts it
 *              (in RAM).
 *              Only if RDIInfo_Target returns a value with
 *              RDITarget_CanReloadAgent set.
 *
 *  Params:
 *      Input:  agent       handle identifies agent
 *      Input:  dest        address in the Debuggee's memory where the new
 *                          version will be put
 *      Input:  size        size of the new version
 *      Input:  getb        a function that can be called (with getbarg as
 *                          the first argument) and the number of bytes to
 *                          down load as the second argument.
 *      InOut:  none
 *      Output: none
 *
 *  Returns:
 *      OK:     RDIError_NoError
 *      Error:  RDIError_UnimplementedMessage (unimplemented functions)
 */
typedef int RDI150_LoadAgentProc(RDI_AgentHandle, ARMword dest,
                                 unsigned32 size, RDI_GetBufferProc *getb,
                                 RDI_GetBufferArg *getbarg);

/*
 * Function:    RDI_TargetIsDeadProc
 *  Purpose:    Used to inform local device drivers that a remote
 *              (typically hardware) debuggee is unresponsive.
 *
 *  Params:
 *      Input:  agent
 *
 *  Returns:
 *      OK:     RDIError_NoError
 *      Error:  RDIError_UnimplementedMessage
 */
typedef int RDI150_TargetIsDeadProc(RDI_AgentHandle agent);

/*
 * Function:    RDI_ErrMessProc
 *  Purpose:    Gets an error message corresponding to the error number.
 *
 *  Params:
 *      Input:  agent
 *      Input:  buflen      length of the buffer
 *      Input:  errnum      the error number
 *      InOut:  none
 *      Output: buf         destination for the message
 *
 *  Returns:    errmessproc returns the number of bytes written,
 *                rather than an RDIError
 */
typedef int RDI150_ErrMessProc(RDI_AgentHandle agent, char *buf,
                               int buflen, int errnum);

/*
 * Function:    RDI_CPUNamesProc and RDI_DriverNamesProc
 *  Purpose:    Get a list of names.
 *              The returned structure contains the number of names
 *              and a list of pointers to the sz names.
 *              The index to the chosen name is used in RDI 1.0 only:
 *                  RDI_ConfigBlock.procesor    (RDI 1.0)
 *                  RDI_ConfigBlock.drivertype
 *              This is used to select a harware driver for the target
 *              or a processor from a processor family.
 *              E.g. If the processor family was PROCESSOR_ARM
 *              the list would show the members of that family
 *              (ARM6, ARM7 etc.)
 *  Params:
 *      Input:  handle      handle identifies processor in cpunames
 *                          RDI_NoHandle for drivernames
 *                          (i.e. target hadware configuration)
 *      InOut:  none
 *      Output: none
 *
 *  Returns:
 *      OK:     RDI_NameList
 *      Error:  none
 */
typedef RDI_NameList const *RDI150_NameListProc(RDI_AgentHandle handle);

typedef struct RDI150_ProcVec
{
    char rditypename[12];

    RDI150_OpenAgentProc       *openagent;
    RDI150_CloseAgentProc      *closeagent;
    RDI150_OpenProc            *open;
    RDI150_CloseProc           *close;
    RDI150_ReadProc            *read;
    RDI150_WriteProc           *write;
    RDI150_CPUReadProc         *CPUread;
    RDI150_CPUWriteProc        *CPUwrite;
    RDI150_CPReadProc          *CPread;
    RDI150_CPWriteProc         *CPwrite;
    RDI150_SetBreakProc        *setbreak;
    RDI150_ClearBreakProc      *clearbreak;
    RDI150_SetWatchProc        *setwatch;
    RDI150_ClearWatchProc      *clearwatch;
    RDI150_ExecuteProc         *execute;
    RDI150_StepProc            *step;
    RDI150_InfoProc            *info;
    RDI150_PointInquiryProc    *pointinquiry;

    /* These three useable only if RDIInfo_DownLoad returns no error */
    RDI150_AddConfigProc       *addconfig;
    RDI150_LoadConfigDataProc  *loadconfigdata;
    RDI150_SelectConfigProc    *selectconfig;

    RDI150_NameListProc        *drivernames;
    RDI150_NameListProc        *cpunames;

    RDI150_ErrMessProc         *errmess;

    /* Only if RDIInfo_Target returns a value with RDITarget_LoadAgent set */
    RDI150_LoadAgentProc       *loadagent;
    RDI150_TargetIsDeadProc    *targetisdead;
} RDI150_ProcVec;

#endif /* !rdi150_h */

/* EOF rdi150.h */
