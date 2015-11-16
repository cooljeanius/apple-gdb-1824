/* arm/multi-ice/winrdi.h
 * WinRDI - Interface to a Win32 DLL containing an RDI target
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
 * RCS $Revision: 1.28.16.1 $
 * Checkin $Date: 2001/01/17 15:46:36 $
 * Revising $Author: hbullman $
 */

#ifndef winrdi_h
#define winrdi_h

/* The IGNORE(x) macro as defined by host.h conflicts with windows */
#ifdef IGNORE
#   undef IGNORE
#endif /* IGNORE */

#include "windows.h"
#if (RDI_VERSION >= 150) && (RDI_VERSION < 200)
# include "toolconf.h"
#endif /* (RDI_VERSION >= 150) && (RDI_VERSION < 200) */

enum {
    WinRDI100_Version = 1,
    WinRDI150_Version = 150,
    WinRDI151_Version = 151
};

#if (RDI_VERSION == 150)
# define WinRDI_Version WinRDI150_Version
#elif (RDI_VERSION == 151)
# define WinRDI_Version WinRDI151_Version
#else
# define WinRDI_Version WinRDI100_Version
#endif /* RDI_VERSION == 15[0|1] */

/* Get the address of a WinRDI_ function, using "GetProcAddress". This
 * header defines the required strings for this to work.
 * Paramters:  handle - Windows handle onto the DLL
 *             name - name of the WinRDI function (e.g. "Valid_RDI_DLL")
 * Returns:    address of the function, cast to the appropriate type
 */
/* macros to return the addresses of standard functions using GetProcAddress */
#define WinRDI_GetProcAddress(handle,name) \
    ( (WinRDI_func##name)GetProcAddress(handle, WinRDI_str##name) )


/*
 * The following functions are those defined by an RDI DLL. Each function
 * is either REQUIRED---it must be defined by a conforming DLL---or
 * OPTIONAL---a conforming Debug Controller must be able to cope with a
 * DLL not defining that function.
 */

/*
 * MJW: @@@ would like to get rid of these names, and make style-guide
 * compliant:
 *  typedef int WinRDI_InitialiseProc(...);
 * etc.
 */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* INITIALIZATION AND VALIDATION */

/* Identify that the DLL is an RDI Debug Target, with WinRDI extensions.
 * Returns:    TRUE if this DLL exports some version of RDI and WinRDI
 *             FALSE otherwise
 * Status:     REQUIRED
 */
typedef BOOL (WINAPI *WinRDI_funcValid_RDI_DLL)(void);

extern BOOL WINAPI WinRDI_Valid_RDI_DLL(void);

#define WinRDI_strValid_RDI_DLL "WinRDI_Valid_RDI_DLL"

/* Specify the RDI and WinRDI version which is prefered by the controller.
 * Returns:    nothing
 * Status:     Optional. Only multi-lingual debug targets need provide this
 */
typedef void (WINAPI *WinRDI_funcSetVersion)(int version);

extern void WINAPI WinRDI_SetVersion(int version);

#define WinRDI_strSetVersion "WinRDI_SetVersion"

/* Specify the RDI and WinRDI version which is supported by the DLL. Should
 * return WinRDI_Version, as defined by this header.
 * Returns:    WinRDI_Version
 * Status:     REQUIRED
 */
typedef int (WINAPI *WinRDI_funcGetVersion)(void);

extern int WINAPI WinRDI_GetVersion(void);

#define WinRDI_strGetVersion "WinRDI_GetVersion"

/*
 *        Function: WinRDI_Info
 *         Purpose: A general purpose WinRDI call, used to communicate issues
 *                  concerning configuration, initialisation and capabilities
 *
 *  Pre-conditions: Should be called immediately following WinRDI_Initialise,
 *                  and before any call to WinRDI_Config, or any RDI_* call.
 *
 *          Params:
 *                   In: config - toolconf
 *                       reason - Reason for calling
 *                       arg1 - (void *) argument: meaning defined by reason
 *                       arg2 - (void *) argument: meaning defined by reason
 *
 *    Return value:  As defined by reason
 */
typedef unsigned (WINAPI *WinRDI_funcInfo)(toolconf config, unsigned reason,
                                           void *arg1, void *arg2);

extern unsigned WINAPI WinRDI_Info(toolconf config, unsigned reason,
                                   void *arg1, void *arg2);

#define WinRDI_strInfo "WinRDI_Info"

/*
 *        Function: WinRDI_Info#WinRDIInfo_ControllerCapabilities
 *         Purpose: Allows the controller to indicate that it can understand
 *                  certain RDI features.
 *
 *          Params:
 *                   In: reason = WinRDIInfo_ControllerCapabilities
 *                   In: *((uint32 *)arg1) = Bitfield describing
 *                                           controller capabilities
 *
 *    Return value: RDIError_NoError - OK
 *                  RDIError_IncompatibleRDILevels - The target can not be
 *                                                   debugged by a controller
 *                                                   with these capabilities
 *
 */
#define WinRDIInfo_ControllerCapabilities 0x1000

#define RDIControllerCapability_ETMModules 1 /* Controller understands ETM */
#define RDIControllerCapability_SelfDescribingModules 2
    /* Controller understands sdm */
#define RDIControllerCapability_Trace 4 /* Controller can use trace (ETM) */


/*
 *        Function: WinRDI_Info#WinRDIInfo_TargetCapabilities
 *         Purpose: Allows the target to advertise that, if configured with
 *                  the supplied toolconf, it will have a particular set of
 *                  RDI features.
 *
 *  Pre-conditions: Should be called immediately following a call to
 *                  WinRDI_TargetCapabilities
 *
 *          Params:
 *                   In: toolconf
 *                  Out: *((uint32 *)arg1 Target capability bitfield
 *                  Out: *((uint32 *)arg2 Level of understanding bitfield
 *
 *                       arg1[n] arg2[n]
 *                         0       0     Feature [n] has no meaning to target
 *                         0       1     Target incapable of feature [n]
 *                         1       0     Target understands [n], but is unable
 *                                       to provide an answer
 *                         1       1     Target capable of feature [n]
 *
 *    Return value: RDIError_NoError
 *
 */
#define WinRDIInfo_TargetCapabilities 0x1001

/*
 * HGB 28/03/2000 - Removed spurious #undefs, checked in by plg.  If there is
 * a genuine reason for these #undefs, will plg please get in touch.
 */
#define RDITargetCapability_CallDuringExecute 0x01
    /* Can accept calls during execute */
#define RDITargetCapability_RT       0x02 /* Target has RDI 1.5.1rt */
#define RDITargetCapability_TX       0x04 /* Target has RDI 1.5.1tx */
#define RDITargetCapability_TXRun    0x08 /* Target has RDI 1.5.1tx run */
#define RDITargetCapability_TXBuffer 0x10 /* Target has RDI 1.5.1tx buffer */
#define RDITargetCapability_Asynch   0x20 /* Target can use asynch RDI */
#define RDITargetCapability_Synch    0x40 /* Target can use synch RDI */


/*
 *        Function: WinRDI_Info#WinRDIInfo_ConfigInPlace
 *         Purpose: Open a configuration dialog inside the dialog
 *                  box of the caller.  If this function is supplied, the
 *                  dialog should provide identical functionality to
 *                  WinRDI_Config.
 *
 *          Params:
 *               arg1(in):  (WinRDIConfigInPlace_ParameterBlock *)arg1 =
 *                           Parent window and rectangle information
 *                          If (arg1 == NULL), this is simply an inquiry call
 *
 *               arg2(out):  *(HWND *)arg2 = Output handle of child window
 *
 *         Returns: arg1 == NULL: RDIError_NoError  => This call is supported
 *                  arg1 != NULL: RDIError_NoError  => In place dialog now up
 *                  arg1 == NULL: RDIError_UnimplementedMessage => Unsupported
 *                  arg1 != NULL: RDIError_UnimplementedMessage => Unsupported
 *                  arg1 != NULL: RDIError_UnimplementedSize => Inplace dialog
 *                                will not fit into specified rectangle
 *                  arg1 != NULL: RDIError_SoftInitialiseError => Inplace
 *                                dialog failed for an unknown reason
 *
 *         Note: dimensions specified in parameter block are in
 *               Dialog Units, not pixels.
 */
typedef struct WinRDIConfigInPlace_ParameterBlock {
    HWND parent;
    RECT area;
} WinRDIConfigInPlace_ParameterBlock;

#define WinRDIInfo_ConfigInPlace 0x1002


/* Return a nul-terminated ASCII string, descibing the DLL; for display to
 * the user.
 * Returns:    A static message describing the DLL. The string should be
 *             limited to 200 characters in length. This allows the Debug
 *             Controller to estimate the amount of window space required to
 *             display the string.
 * Status:     REQUIRED
 */
typedef char *(WINAPI *WinRDI_funcGet_DLL_Description)(void);

extern char *WINAPI WinRDI_Get_DLL_Description(void);

#define WinRDI_strGet_DLL_Description "WinRDI_Get_DLL_Description"


/* Return a pointer to the RDI_ProcVec exported by this DLL. Note that the
 * Debug Controller should call WinRDI_GetVersion to assuming the form of
 * this structure.
 * Returns:    Pointer to the exported RDI interface.
 * Status:     REQUIRED
 */
typedef RDI_ProcVec *(WINAPI *WinRDI_funcGetRDIProcVec)(void);

extern RDI_ProcVec *WINAPI WinRDI_GetRDIProcVec(void);

#define WinRDI_strGetRDIProcVec "WinRDI_GetRDIProcVec"


/* Called by the Debug Controller before any RDI_ functions are called. The
 * DLL performs any initialization required. After this call, the DLL is
 * ready to accept RDI_OpenAgent. If WinRDI_Initialise is exported, the DLL
 * must be ready to accept RDI_OpenAgent at any time.
 * Parameters: hParent - Handle to the parent window. Used in the case of the
 *                 DLL creating user-interface components, such as dialog
 *                 boxes.
 *             config - Device specific configuration information
 * Returns:    TRUE if initialization was successful
 *             FALSE if initialization failed. There is no method for
 *                 returning extended error information. If initialization
 *                 fails, the Debug Target displays a dialog box or other
 *                 user-interface component, informing the user of the
 *                 reason for the failure.
 * Status:     OPTIONAL
 */
typedef int (WINAPI *WinRDI_funcInitialise)(HWND hParent,
                                            RDI_ConfigPointer config);

extern int WINAPI WinRDI_Initialise(HWND hParent, RDI_ConfigPointer conf);

#define WinRDI_strInitialise "WinRDI_Initialise"


/* Called when initialized from a console application, rather than a
 * windows environment.
 * Parameters: drivername - Specifies connection. (ethernet, serial or
 *                 parallel for Angel.)
 *             args - String of command-line arguments
 *             heartbeat - (Angel only) TRUE - Angel heartbeat on
 *                                      FALSE - Angel heartbeat off
 *                 may be ignored by other Targets
 * Returns:    TRUE if initialization was successful
 *             FALSE if initialization failed
 * Status:     OPTIONAL
 */
typedef int (WINAPI *WinRDI_funcConsoleInitialise)(char *drivername,
                                                   char *args,
                                                   int heartbeat);

extern int WINAPI WinRDI_ConsoleInitialise(char *drivername, char *args,
                                           int heartbeat);

#define WinRDI_strConsoleInitialise "WinRDI_ConsoleInitialise"

/* CONFIGURATION */

/* Called by the Debug Controller to provide the user with a dialog box to
 * configure the Target. The Target brings up a modal dialog box attached
 * to the provided window handle, and returns control to the Debug Controller
 * when the dialog is closed.
 * Parameters: hParent - Handle to the parent window. The dialog box is
 *                 created as a child of this window
 *             config - Device specific configuration information
 * Returns:    TRUE if the user clicked "Okay" in the dialog box
 *             FALSE if the user clicked "Cancel" in the dialog box
 * Status:     OPTIONAL
 */
typedef BOOL (WINAPI *WinRDI_funcConfig)(RDI_ConfigPointer config,
                                         HWND hParent);

extern BOOL WINAPI WinRDI_Config(RDI_ConfigPointer config, HWND hParent);

#define WinRDI_strConfig "WinRDI_Config"


#if (RDI_VERSION >= 151)
/* Called by the Debug Controller to provide the user with a dialog box to
 * configure the Target. The Target brings up a modal dialog box attached
 * to the provided window handle, and returns control to the Debug Controller
 * when the dialog is closed.
 * Parameters: hParent - Handle to the parent window. The dialog box is
 *                 created as a child of this window
 *             nExecUnits - The number of execution units. If nExecUnits > 1
 *                 the RDIOpen_UseThreads bit must be set on RDI_OpenAgentProc.
 *                 Otherwise this bit must be clear.
 *             config - Device specific configuration information
 * Returns:    TRUE if the user clicked "Okay" in the dialog box
 *             FALSE if the user clicked "Cancel" in the dialog box
 * Status:     OPTIONAL
 */
typedef BOOL (WINAPI *WinRDI_funcMultiConfig)(RDI_ConfigPointer config,
                                              unsigned int *nExecUnits,
                                              HWND hParent);

extern BOOL WINAPI WinRDI_MultiConfig(RDI_ConfigPointer config,
                                      unsigned int *nExecUnits,
                                      HWND hParent);
# define WinRDI_strMultiConfig "WinRDI_MultiConfig"
#endif /* RDI_VERSION >= 151 */

/* YIELDING CONTROL */

/* Supply a callback to yield control to other processes during Target
 * execution. The callback is passed as a function/argument closure.
 */
typedef struct WinRDI_OpaqueYieldArgStr WinRDI_YieldArg;
typedef void WinRDI_YieldProc(WinRDI_YieldArg *);

/* Parameters: yieldcallback - Function to be called every-so-often during
 *                 Target execution (i.e. RDI_ExecuteProc). The calling
 *             hyieldcallback - Argument to be passed to yieldcallback
 * Status:     OPTIONAL
 */
typedef void (WINAPI *WinRDI_funcRegister_Yield_Callback)(WinRDI_YieldProc *yieldcallback,
                                                          WinRDI_YieldArg *hyieldcallback);

extern void WINAPI WinRDI_Register_Yield_Callback(WinRDI_YieldProc *yieldcallback,
                                                  WinRDI_YieldArg *hyieldcallback);

#define WinRDI_strRegister_Yield_Callback "WinRDI_Register_Yield_Callback"

/* PROGRESS INDICATORS */

/* Register a callback for the Debug Target to call periodically during
 * download. The callback function receives data (counters) indicating the
 * progress made so far, and typically displays this in a user-interface
 * component.
 */
/* The progress indicator callback is passed an opaque handle */
typedef struct WinRDI_OpaqueProgressArgStr WinRDI_ProgressArg;
typedef struct {
    WinRDI_ProgressArg *handle; /* handle passed to SetProgressFunc */
    int nRead;     /* number of bytes read from channel */
    int nWritten;  /* number of bytes written           */
} WinRDI_ProgressInfo;

typedef void (WINAPI *WinRDI_funcProgressFunc)(WinRDI_ProgressInfo *info);

/* Parameters: func - Function for the Debug Target to call periodically
 *                 during download
 *             handle - Argument to fill the 'handle' field of the
 *                 WinRDI_ProgressInfo structure passed to the callback
 * Status:     OPTIONAL
 */
typedef void (WINAPI *WinRDI_funcSetProgressFunc)(WinRDI_funcProgressFunc func,
                                                  WinRDI_ProgressArg *handle);

extern void WINAPI WinRDI_SetProgressFunc(WinRDI_funcProgressFunc func,
                                          WinRDI_ProgressArg *handle);

#define WinRDI_strSetProgressFunc "WinRDI_SetProgressFunc"

/* Zero the counters
 * Status:     OPTIONAL
 */
typedef void (WINAPI *WinRDI_funcZeroProgressValues)(void);

extern void WINAPI WinRDI_ZeroProgressValues(void);

#define WinRDI_strZeroProgressValues "WinRDI_ZeroProgressValues"

/* EXTERNAL DOWNLOAD */

/* The DLL may need to download a debug monitor, this is called an
 * external startup download. It may also need/want to download the
 * image itself, this is called an external image download.
 * In either case, the Controller tells the Target to do what it likes.
 * It then returns saying if the Controller needs to download the image.
 */
/* Download flags */
enum {
    WinRDI_DownloadStartup  = 1,
    WinRDI_DownloadImage    = 2
};

/* Parameters: flags - a bitflag of the above two parameters, to specify
 *                 whether 'startup' or 'image' download is requested.
 *             filename - name of an image to download, if the 'image' bit
 *                 of 'flags' was set; NULL otherwise.
 * Returns:    TRUE if the download was successful
 *             FALSE otherwise
 *             On exit the 'image' bit of 'flags' is set if an image was
 *             downloaded.
 */
typedef bool (WINAPI *WinRDI_funcDownload)(int *options,
                                           char const *filename);

extern int WINAPI WinRDI_Download(int *options, char const *filename);

#define WinRDI_strDownload "WinRDI_Download"

/* UNUSED */

/* WinRDI_Download_... download bitflags  */
/* typedef int (WINAPI *WinRDI_funcDownload)(int *options, char const *filename); */
typedef BOOL (WINAPI *WinRDI_funcIsIdleProcessing)(void);
typedef int (WINAPI *WinRDI_funcIdle)(void);
typedef BOOL (WINAPI *WinRDI_funcIsProcessingSWI)(void);
typedef void (WINAPI *WinRDI_funcSetStopping)(BOOL stopping);
typedef char *(WINAPI *WinRDI_funcFlashDLAvailable)(void);


extern void WINAPI WinRDI_ProgressFunc(WinRDI_ProgressInfo *info);
extern BOOL WINAPI WinRDI_IsIdleProcessing(void);
extern int WINAPI WinRDI_Idle(void);
extern BOOL WINAPI WinRDI_IsProcessingSWI(void);
extern void WINAPI WinRDI_SetStopping(BOOL stopping);

/* find out whether we can use Flash Download... */
extern char *WINAPI WinRDI_FlashDLAvailable(void);


/* exports from windows-based RDI DLLs */
#define WinRDI_strIsIdleProcessing    "WinRDI_IsIdleProcessing"
#define WinRDI_strIdle                "WinRDI_Idle"
#define WinRDI_strIsProcessingSWI     "WinRDI_IsProcessingSWI"
#define WinRDI_strSetStopping         "WinRDI_SetStopping"
#define WinRDI_strFlashDLAvailable    "WinRDI_FlashDLAvailable"

#ifdef __cplusplus
}; /* extern "C" */
#endif /* __cplusplus */

#endif /* winrdi_h */

/* EOF winrdi.h */
