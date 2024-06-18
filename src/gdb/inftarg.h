/* -*- C -*-
 *  inftarg.h
 *  gdb
 *
 *  Created by Eric Gallager on 5/10/19.
 *  Copyright 2019 George Washington University. All rights reserved.
 *
 */

#ifndef INFTARG_H
#define INFTARG_H 1

extern int child_suppress_run;

#if !defined(PROTOTYPES) || !PROTOTYPES
extern void inftarg_set_find_memory_regions();
extern void inftarg_set_make_corefile_notes();
#endif /* !PROTOTYPES */

#endif /* !INFTARG_H */
