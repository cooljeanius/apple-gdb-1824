/* Tags for RDI configuration databases
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
 * RCS $Revision: 1.22 $
 * Checkin $Date: 2000/10/05 12:03:42 $
 * Revising $Author: dsinclai $
 */

#ifndef rdi_conf_h
#define rdi_conf_h

#ifndef RDI_VERSION
#define RDI_VERSION 150
#endif

/*
 * Define the "Toolconf" tags regardless.
 * These tags start Dbg_Cnf_, for historical reasons
 */

/* new debugger toolbox uses a toolconf */
#include "toolconf.h"

/*
 * The debugger uses the following fields:
 *  LLSymsNeedPrefix  (Boolean)
 *  DeviceName        (String, name of device - eg. ARM, ETM)
 *  ByteSex           (something starting L or B, may be updated by toolkit)
 *  MemConfigToLoad   (filename)
 *  CPUSpeed          (int)
 *  ConfigToLoad      (filename)
 *
 * Specific targets use other fields, with the suggested names:
 *  FPE               \
 *  MemorySize         }  ISS
 *  Processor         /
 *  SerialPort        \
 *  SerialLineSpeed    |
 *  ParallelPort       |
 *  ParallelLineSpeed   } Dev Card
 *  EthernetTarget     |
 *  RDIType            |
 *  DriverType        /
 *  HeartbeatOn           Debug Monitor
 */

#define Dbg_Cnf_TargetDir (tag_t)"TARGET_DIR"
#define Dbg_Cnf_FeatureFile (tag_t)"TARGET_FEATURE_FILE"
#define Dbg_Cnf_ControllerIsAgent (tag_t)"TARGET_CONTROLLER_IS_AGENT"
#define Dbg_Cnf_TargetHWEndian (tag_t)"TARGET_HW_ENDIAN"

/*
 * IMPORTANT NOTE
 * ==============
 *
 * The tags that follow are suggested names for the toolconf entries
 * for debug targets.  They are not 'required names' under RDI 1.5
 *
 * For every debug target which is released, a list of toolconf entries
 * which the target is sensitive to MUST be provided in the target
 * documentation
 */

#define Dbg_Cnf_DeviceName (tag_t)"DEVICENAME"
#define Dbg_Cnf_DriverFile (tag_t)"DRIVERFILE"
#define Dbg_Cnf_ByteSex (tag_t)"BYTESEX"
#define Dbg_Cnf_MemConfigToLoad (tag_t)"MEMCONFIGTOLOAD"
#define Dbg_Cnf_CPUSpeed (tag_t)"CPUSPEED"
#define Dbg_Cnf_ConfigToLoad (tag_t)"CONFIGTOLOAD"
#define Dbg_Cnf_FPE (tag_t)"FPE"
#define Dbg_Cnf_MemorySize (tag_t)"MEMORYSIZE"
#define Dbg_Cnf_Processor (tag_t)"PROCESSOR"
#define Dbg_Cnf_Board (tag_t)"BOARD"
#define Dbg_Cnf_Default (tag_t)"Default"

/* New ARMulator Configuration dialog entries */
#define Dbg_Cnf_Endianness (tag_t)"ENDIANNESS"


#define Dbg_Cnf_Port (tag_t)"PORT"
#define Dbg_Cnf_SerialPort (tag_t)"SERIALPORT"
#define Dbg_Cnf_SerialLineSpeed (tag_t)"SERIALLINESPEED"
#define Dbg_Cnf_ParallelPort (tag_t)"PARALLELPORT"
#define Dbg_Cnf_ParallelLineSpeed (tag_t)"PARALLELLINESPEED"
#define Dbg_Cnf_EthernetTarget (tag_t)"ETHERNETTARGET"
#define Dbg_Cnf_RDIType (tag_t)"RDITYPE"
#define Dbg_Cnf_Driver (tag_t)"DRIVER"

#define Dbg_Cnf_Heartbeat (tag_t)"HEARTBEAT"
#define Dbg_Cnf_LLSymsNeedPrefix (tag_t)"LLSYMSNEEDPREFIX"
#define Dbg_Cnf_Reset (tag_t)"RESET"
#define Dbg_Cnf_TargetName (tag_t)"TARGETNAME"

#define Dbg_Cnf_MixedEndLongLong (tag_t)"MIXEDENDLONGLONG"
#define Dbg_Cnf_MixedEndDouble (tag_t)"MIXEDENDDOUBLE"
#define Dbg_Cnf_StrictDwarf2 (tag_t)"STRICTDWARF2"

/* MICE config */
#define Dbg_Cnf_MICEServer    (tag_t)"MICESERVER"
#define Dbg_Cnf_MICEDbgConn   (tag_t)"MICEDBGCONN"
#define Dbg_Cnf_MICEDriver0   (tag_t)"MICEDRIVER0"
#define Dbg_Cnf_MICETapPos0   (tag_t)"MICETAPPOS0"
#define Dbg_Cnf_MICEDriver1   (tag_t)"MICEDRIVER1"
#define Dbg_Cnf_MICETapPos1   (tag_t)"MICETAPPOS1"

#define Dbg_ConfigFlag_Reset 1
#define Dbg_ConfigFlag_LLSymsNeedPrefix 2

/* ARMISS config */
#define Dbg_Cnf_MapFilePath (tag_t)"MAPFILEPATH"
#define Dbg_Cnf_UseMapFile  (tag_t)"USEMAPFILE"



#if 1                           /* defined in rdi_hif.h */
typedef struct RDI_HostosInterface RDI_HostosInterface;
/* This structure allows access by the (host-independent) C-library support
 module of armulator or pisd (armos.c) to host-dependent functions for
 which there is no host-independent interface.  Its contents are unknown
 to the debugger toolbox.
 The assumption is that, in a windowed system, fputc(stderr) for example
 may not achieve the desired effect of the character appearing in some
 window.
 */
#endif

#endif /* rdi_conf_h */

#if RDI_VERSION == 100 || defined(rdi_DefineConfigBlock)
/* Use a different guard for the configblock definition, so this header
 * can be included with rdi_DefineConfigBlock defined, even if something
 * else has already included it.
 * Note: rdi_DefineConfigBlock is for "internal RDI" use
 */
#ifndef rdi_conf_configblock
#define rdi_conf_configblock

typedef struct RDI_ConfigBlock {
    int bytesex;
    int fpe;               /* Target should initialise FPE */
    long memorysize;
    unsigned long cpu_speed;/* Cpu speed (HZ) */
    int serialport;        /*) remote connection parameters */
    int seriallinespeed;   /*) (serial connection) */
    int parallelport;      /*) ditto */
    int parallellinespeed; /*) (parallel connection) */
    char *ethernettarget;  /* name of remote ethernet target */
    int processor;         /* processor the armulator is to emulate (eg ARM60) */
    int rditype;           /* armulator / remote processor */
    int heartbeat_on;      /* angel heartbeat */
    int drivertype;        /* parallel / serial / etc */
    char const *configtoload;
    char const *memconfigtoload;
    int flags;
    char const *target_name;
} RDI_ConfigBlock;


#endif /* rdi_conf_configblock */
#endif /* RDI_VERSION == 100 || ... */

/* EOF rdi_conf.h */
