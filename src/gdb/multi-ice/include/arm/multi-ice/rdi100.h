/* rdi100.h: RDI 1.0 defintions
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
 * RCS $Revision: 1.10 $
 * Checkin $Date: 2000/09/12 10:09:53 $
 * Revising $Author: hbullman $
 */

#ifndef rdi100_h
#define rdi100_h

#if (RDI_VERSION == 100)
#  define RDI100_OpenProc RDI_OpenProc
#  define RDI100_CloseProc RDI_CloseProc
#  define RDI100_ReadProc RDI_ReadProc
#  define RDI100_WriteProc RDI_WriteProc
#  define RDI100_CPUReadProc RDI_CPUReadProc
#  define RDI100_CPUWriteProc RDI_CPUWriteProc
#  define RDI100_CPReadProc RDI_CPReadProc
#  define RDI100_CPWriteProc RDI_CPWriteProc
#  define RDI100_SetBreakProc RDI_SetBreakProc
#  define RDI100_ClearBreakProc RDI_ClearBreakProc
#  define RDI100_SetWatchProc RDI_SetWatchProc
#  define RDI100_ClearWatchProc RDI_ClearWatchProc
#  define RDI100_ExecuteProc RDI_ExecuteProc
#  define RDI100_StepProc RDI_StepProc
#  define RDI100_InfoProc RDI_InfoProc
#  define RDI100_PointInquiryProc RDI_PointInquiryProc
#  define RDI100_AddConfigProc RDI_AddConfigProc
#  define RDI100_LoadConfigDataProc RDI_LoadConfigDataProc
#  define RDI100_SelectConfigProc RDI_SelectConfigProc
#  define RDI100_NameListProc RDI_NameListProc
#  define RDI100_ErrMessProc RDI_ErrMessProc
#  define RDI100_LoadAgentProc RDI_LoadAgentProc
#  define RDI100_TargetIsDeadProc RDI_TargetIsDeadProc
#  define RDI100_ProcVec RDI_ProcVec
#  define RDI100_ConfigPointer RDI_ConfigPointer
#endif /* RDI_VERSION 100 */

/*
 * RDI Implementation Prototypes
 */


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
 *                              state
 *
 *  Returns:                                 b3     b2
 *      Error:  RDIError_SoftInitialiseError X      X   If things go wrong!
 *      Error:  RDIError_WrongByteSex        0      E   bad match
 *      OK:     RDIError_NoError             0      E   match
 *      OK:     RDIError_LittleEndian        1      X
 *      OK:     RDIError_BigEndian           1      X
 */

typedef struct RDI_ConfigBlock *RDI100_ConfigPointer;

typedef int RDI100_OpenProc(
                            unsigned type, struct RDI_ConfigBlock const *config,
                            struct RDI_HostosInterface const *i, RDI_DbgState *dbg_state);


/*
 * Function:    RDI_CloseProc
 *  Purpose:    Close down
 *
 *  Params:
 *
 *  Returns:    RDIError_NoError
 */
typedef int RDI100_CloseProc(void);


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
 *      Input:  source      address
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
typedef int RDI100_ReadProc(ARMword source, void *dest, unsigned *nbytes);


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
 *      Input:  source      source buffer
 *
 *              dest        destination address
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
typedef int RDI100_WriteProc(
                             const void *source, ARMword dest, unsigned *nbytes);

/*
 * Function:    RDI_CPUreadProc
 *  Purpose:    Read the register content from the specified processor.
 *              The mask specifies which register(s) are to be transferred
 *              as 32-bit ARMwords.
 *              The mask has been extended to support more than 32 registers
 *  Params:
 *      Input:  mode        mode defines register context
 *                          ARM
 *                              RDIMode_Curr    use the current mode
 *                              Other values match the mode bits of the PSR.
 *                          PICCOLO *** Use of Piccolo is obsolete ***
 *                              RDIModeBankX    Transfer registers
 *                                              from bankX
 *
 *      Input:  mask[]      bits defining which registers are to be
 *                          transferred. These bits can be ORed to read any
 *                          combination of registers.
 *                          ARM
 *                              b0 - b14    r0 - r14
 *                              b15         Reserved.  Targets must ignore b15.
 *                                          Controllers must not set b15
 *                              RDIReg_PC   Just the program counter
 *                              RDIReg_CPSR CPSR register
 *                              RDIReg_SPSR SPSR register
 *
 *      Output: state[]     destination array of ARMwords
 *
 *  Returns:
 *      OK:     RDIError_NoError
 *      Error:  error code
 */
typedef int RDI100_CPUReadProc(unsigned mode, unsigned32 mask, ARMword *state);


/*
 * Function:    RDI_CPUwriteProc
 *  Purpose:    Write the register content to the specified processor.
 *              The mask specifies which register(s) are to be transferred
 *              as 32-bit ARMwords. The mask has been extended to support
 *              more than 32 registers in RDI2.
 *
 *  Params:
 *      Input:  mode        mode defines register context
 *                          ARM
 *                              RDIMode_Curr    use the current mode
 *                              Other values match the mode bits of the PSR.
 *      Input:  mask[]      bits defining which registers are to be
 *                          transferred. These bits can be ORed to read any
 *                          combination of registers.
 *                          ARM
 *                              b0 - b14    r0 - r14
 *                              b15         Reserved.  Targets must ignore b15.
 *                                          Controllers must not set b15
 *                              RDIReg_PC   Just the program counter
 *                              RDIReg_CPSR CPSR register
 *                              RDIReg_SPSR SPSR register
 *      Input:  state[]     source array of ARMwords
 *      InOut:  none
 *      Output: none
 *
 *  Returns:
 *         OK:  RDIError_NoError
 *     Not OK:  RDIError_ReadOnly - if writing is not allowed at this time
 *      Error:  error code
 */
typedef int RDI100_CPUWriteProc(
                                unsigned mode, unsigned32 mask, ARMword const *state);


/*
 * Function:    RDI_CPreadProc
 *  Purpose:    Read the register content from the specified coprocessor.
 *              The mask specifies which register(s) are to be transferred
 *              as 32-bit ARMwords.
 *
 *              Each coprocessor will have its own register set defined in
 *              another header file. The actual registers transferred, and
 *              their size is dependent on the coprocessor specified.
 *
 *  Params:
 *      Input:  CPnum       coprocessor number for this processor
 *      Input   mask[]      bits define which registers are to be transferred
 *      InOut:  none
 *      Output: state[]     destination array for register values
 *
 *  Returns:
 *      OK:     RDIError_NoError
 *      Error:  error code
 */
typedef int RDI100_CPReadProc(unsigned CPnum, unsigned32 mask, ARMword *state);

/*
 * Function:    RDI_CPwriteProc
 *  Purpose:    Write the register content to the specified coprocessor.
 *              The mask specifies which register(s) are to be transferred
 *              as 32-bit ARMwords.
 *
 *              Each coprocessor will have its own register set defined in
 *              another header file. The actual registers transferred, and
 *              their size is dependent on the coprocessor specified.
 *
 *  Params:
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
typedef int RDI100_CPWriteProc(
                               unsigned CPnum, unsigned32 mask, ARMword const *state);


/*
 * Function:    RDI_SetBreakProc
 *  Purpose:    Set a breakpoint in the debuggee.
 *
 *  Params:
 *      Input:  address     address of breakpoint
 *      Input:  type        one of RDIPoint_ types
 *
 *   RDIPoint_EQ          equal to address
 *   RDIPoint_GT          greater than address
 *   RDIPoint_GE          greater than or equal to address
 *   RDIPoint_LT          less than address
 *   RDIPoint_LE          less than or equal to address
 *   RDIPoint_IN          in the range from address to bound, inclusive
 *   RDIPoint_OUT         not in the range from address to bound, inclusive
 *   RDIPoint_MASK        halt execution if (pc & bound) = address
 *   RDIPoint_16Bit       if set breakpoint is on 16-bit (Thumb) instruction
 *   RDIPoint_Conditional if set breakpoint only occurs when the
 *                        instructions condition is true.
 *
 *      Input:  bound       upper address range
 *      InOut:  none
 *      Output: *handle     A handle to identify the breakpoint
 *
 *  Returns:
 *      OK:     RDIError_NoError
 *      OK:     RDIError_NoMorePoints - successful,
 *                                      but no more points available
 *      Error:  RDIError_CantSetPoint
 */
typedef int RDI100_SetBreakProc(
                                ARMword address, unsigned type, ARMword bound, RDI_PointHandle *handle);


/*
 * Function:    RDI_ClearBreakProc
 *  Purpose:    Clear a breakpoint in the debuggee.
 *
 *  Params:
 *      Input:  handle      identifies breakpoint
 *      InOut:  none
 *      Output: none
 *
 *  Returns:
 *      OK:     RDIError_NoError
 *      Error:  RDIError_NoSuchPoint
 */
typedef int RDI100_ClearBreakProc(RDI_PointHandle handle);


/*
 * Function:    RDI_SetWatchProc
 *  Purpose:    Set a watchpoint in the debuggee.
 *
 *  Params:
 *      Input:  address     address of watchpoint
 *      Input:  type        one of RDIPoint_ types
 *
 *   RDIPoint_EQ          equal to address
 *   RDIPoint_GT          greater than address
 *   RDIPoint_GE          greater than or equal to address
 *   RDIPoint_LT          less than address
 *   RDIPoint_LE          less than or equal to address
 *   RDIPoint_IN          in the range from address to bound, inclusive
 *   RDIPoint_OUT         not in the range from address to bound, inclusive
 *   RDIPoint_MASK        halt execution if (pc & bound) = address
 *
 *      Input:  datatype    one of RDIWatch_ data types
 *      Input:  bound       upper address range
 *      InOut:  none
 *      Output: *handle     A handle to identify the watchpoint
 *
 *  Returns:
 *      OK:     RDIError_NoError
 *      OK:     RDIError_NoMorePoints - successful,
 *                                      but no more points available
 *      Error:  RDIError_CantSetPoint
 */
typedef int RDI100_SetWatchProc(
                                ARMword address, unsigned type, unsigned datatype, ARMword bound,
                                RDI_PointHandle *handle);


/*
 * Function:    RDI_ClearWatchProc
 *  Purpose:    Clear a watchpoint in the debuggee.
 *
 *  Params:
 *      Input:  handle      identifies watchpoint
 *      InOut:  none
 *      Output: none
 *
 *  Returns:
 *      OK:     RDIError_NoError
 *      Error:  RDIError_NoSuchPoint
 */
typedef int RDI100_ClearWatchProc(RDI_PointHandle handle);


/*
 * Function:    RDI_ExecuteProc
 *  Purpose:    Starts execution of the debuggee from the current state.
 *              The debuggee continues until a natural exit occurs.
 *              The reason for exit is returned.
 *
 *  Params:
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
 */
typedef int RDI100_ExecuteProc(RDI_PointHandle *handle);


/*
 * Function:    RDI_StepProc
 *  Purpose:    Steps the debuggee from the current state.
 *              The debuggee continues until a natural exit occurs
 *              or the processor completes its next instruction.
 *
 *  Params:
 *      Input:  ninstr  number of instructions to step
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
 */
typedef int RDI100_StepProc(unsigned ninstr, RDI_PointHandle *handle);


/*
 * Function:    RDI_InfoProc
 *  Purpose:    Used to transfer miscellaneous information between the
 *              Debuger and the Debuggee. The parameters meanings change for
 *              each information type.
 *              Each type is defined in RDI_Info subcodes.
 *
 *  Params:
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
typedef int RDI100_InfoProc(unsigned type, ARMword *arg1, ARMword *arg2);


/*
 * Function:    RDI_PointinqProc
 *  Purpose:    Breakpoint or watchpoint inquiry.
 *              Used to check before setbreak or setwatch is used.
 *              Returns what will happen if a corresponding call is made
 *              to setbreak or setwatch.
 *
 *  Params:
 *      Input:  type        see setbreak or setwatch for definition
 *      Input:  datatype    0       breakpoint
 *                          != 0    see setwatch for definition
 *      InOut:  *address
 *          In:             address of point
 *          Out:            set to actual value that will be used
 *      InOut:  *bound
 *          In:             upper address range
 *          Out:            set to actual value that will be used
 *      Output: none
 *
 *  Returns:
 *      OK:     RDIError_NoError
 *      Error:  RDIError_NoMorePoints (if cannot set)
 */
typedef int RDI100_PointInquiryProc(
                                    ARMword *address, unsigned type, unsigned datatype, ARMword *bound);


/*
 * Function:    RDI_AddConfigProc
 *  Purpose:    Declares the size of a config block about to be loaded.
 *              Only use if RDIInfo_DownLoad returns no error.
 *
 *  Params:
 *      Input:  nbytes  size of configuration block to follow
 *
 *  Returns:
 *      OK:     RDIError_NoError
 *      Error:  RDIError_UnimplementedMessage (unimplemented functions)
 */
typedef int RDI100_AddConfigProc(unsigned32 nbytes);


/*
 * Function:    RDI_LoadConfigDataProc
 *  Purpose:    Loads the config block.
 *              The block specifies target-dependent information to the
 *              debuggee. (e.g. ICEman)
 *              Only use if RDIInfo_DownLoad returns no error.
 *
 *  Params:
 *      Input:  nBytes  size of block (as declared in addconfig)
 *
 *              data    source of block
 *
 *  Returns:
 *      OK:     RDIError_NoError
 *      Error:  RDIError_UnimplementedMessage (unimplemented functions)
 */
typedef int RDI100_LoadConfigDataProc(unsigned32 nbytes, char const *data);


/*
 * Function:    RDI_SelectConfigProc
 *  Purpose:    Selects which of the loaded config blocks should be used
 *              and then reinitialises the Debuggee to use the selected
 *              config data.
 *              Only use if RDIInfo_DownLoad returns no error.
 *
 *  Params:
 *      Input:  aspect      RDI_ConfigCPU or RDI_ConfigSystem
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
typedef int RDI100_SelectConfigProc(
                                    RDI_ConfigAspect aspect, char const *name,
                                    RDI_ConfigMatchType matchtype, unsigned versionreq, unsigned *versionp);

/*
 * Function:    RDI_LoadAgentProc
 *  Purpose:    Loads a replacement EmbeddedICE ROM image, and starts it
 *              (in RAM).
 *              Only if RDIInfo_Target returns a value with
 *              RDITarget_CanReloadAgent set.
 *
 *  Params:
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

/* should be defined for RDI150 also */

typedef int RDI100_LoadAgentProc(
                                 ARMword dest, unsigned32 size, RDI_GetBufferProc *getb,
                                 RDI_GetBufferArg *getbarg);


/*
 * Function:    RDI_TargetIsDeadProc
 *  Purpose:    Used to inform local device drivers that a remote
 *              (typically hardware) debuggee is unresponsive.
 *
 *  Params:
 *
 *  Returns:
 *      OK:     RDIError_NoError
 *      Error:  RDIError_UnimplementedMessage
 */
typedef int RDI100_TargetIsDeadProc(void);


/*
 * Function:    RDI_ErrMessProc
 *  Purpose:    Gets an error message corresponding to the error number.
 *
 *  Params:
 *      Input:  buflen      length of the buffer
 *      Input:  errnum      the error number
 *      InOut:  none
 *      Output: buf         destination for the message
 *
 *  Returns:    errmessproc returns the number of bytes written,
 rather than an RDIError
 */
typedef int RDI100_ErrMessProc(char *buf, int buflen, int errnum);

/*
 * Function:    RDI_CpuNamesProc and RDI_DriverNamesProc
 *  Purpose:    Get a list of names.
 *              The returned structure contains the number of names
 *              and a list of pointers to the sz names.
 *              The index to the chosen name is used in:
 *                  Dbg_ConfigBlock.procesor    (RDI 1)
 *                  Dbg_ConfigBlock.drivertype
 *                  RDI_ModuleDesc.procesor     (RDI 2)
 *              This is used to select a harware driver for the target
 *              or a processor from a processor family.
 *              E.g. If the processor family was PROCESSOR_ARM
 *              the list would show the members of that family
 *              (ARM6, ARM7 etc.)
 *  Params:
 *
 *  Returns:
 *      OK:     RDI_NameList
 *      Error:  none
 */
typedef RDI_NameList const *RDI100_NameListProc(void);

typedef struct RDI100_ProcVec
{
    char rditypename[12];

    RDI100_OpenProc           *open;
    RDI100_CloseProc          *close;
    RDI100_ReadProc           *read;
    RDI100_WriteProc          *write;
    RDI100_CPUReadProc        *CPUread;
    RDI100_CPUWriteProc       *CPUwrite;
    RDI100_CPReadProc         *CPread;
    RDI100_CPWriteProc        *CPwrite;
    RDI100_SetBreakProc       *setbreak;
    RDI100_ClearBreakProc     *clearbreak;
    RDI100_SetWatchProc       *setwatch;
    RDI100_ClearWatchProc     *clearwatch;
    RDI100_ExecuteProc        *execute;
    RDI100_StepProc           *step;
    RDI100_InfoProc           *info;

    /* RDI 1.02 */
    RDI100_PointInquiryProc   *pointinquiry;

    /* These three useable only if RDIInfo_DownLoad returns no error */
    RDI100_AddConfigProc      *addconfig;
    RDI100_LoadConfigDataProc *loadconfigdata;
    RDI100_SelectConfigProc   *selectconfig;

    RDI100_NameListProc       *drivernames;
    RDI100_NameListProc       *cpunames;

    RDI100_ErrMessProc        *errmess;

    /* Only if RDIInfo_Target returns a value with RDITarget_LoadAgent set */
    RDI100_LoadAgentProc      *loadagent;
    RDI100_TargetIsDeadProc   *targetisdead;
} RDI100_ProcVec;


#endif /* rdi100_h */
