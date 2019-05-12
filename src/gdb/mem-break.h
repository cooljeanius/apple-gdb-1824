/* -*- C -*-
 *  mem-break.h
 *  gdb
 *
 *  Created by Eric Gallager on 5/12/19.
 *  Copyright 2019 George Washington University. All rights reserved.
 *
 */

#ifndef MEM_BREAK_H
#define MEM_BREAK_H 1

#define BKPT_ARRAY_SIZE 100
extern CORE_ADDR recent_breakpoints[BKPT_ARRAY_SIZE];
extern int last_bkpt_index;

#endif /* !MEM_BREAK_H */
