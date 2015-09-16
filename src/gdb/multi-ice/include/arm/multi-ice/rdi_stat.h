/* RDI statistics
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
 * RCS $Revision: 1.5 $
 * Checkin $Date: 2000/08/29 21:03:39 $
 * Revising $Author: dsinclai $
 */

#ifndef rdi_stat_h
#define rdi_stat_h

#include "armtypes.h"           /* for ARMword */

typedef struct RDI_CycleDesc {
    char name[32];                /* length byte, followed by string */
} RDI_CycleDesc;

typedef struct RDI_MemDescr {
    ARMword handle;
    ARMword start, limit;  /* start & limit of this region */
    unsigned char width;   /* memory width 0,1,2 => 8,16,32 bit */
    unsigned char access;  /* Bit 0 => read access */
    /* Bit 1 => write access */
    /* Bit 2 => latched 32 bit memory */
    /* DRS 2000-08-26 This doesn't belong here - this is
     * an interface between any debugger and any target, so
     * target-specific-changes are not acceptable. */
#ifdef OldCode
#ifdef ARM925Model
    unsigned char rBurst_len; /* Read burst length in words */
    unsigned char wBurst_len; /* Write burst length in words */
    unsigned char domain;     /* Domain group number */
    unsigned int sourceChannels; /* source channel mask 0000 to FFFF */
#endif
#endif
    unsigned char d1, d2;  /* ensure padding */
    /* Access times for R/W N/S cycles */
    unsigned long Nread_ns, Nwrite_ns, Sread_ns, Swrite_ns;
} RDI_MemDescr;

typedef struct RDI_MemAccessStats {
    ARMword Nreads,     /* Counts for R/W N/S cycles */
    Nwrites,
    Sreads,
    Swrites;
    ARMword ns,         /* Time (nsec, sec) */
    s;
} RDI_MemAccessStats;

#endif /* rdi_stat_h */
