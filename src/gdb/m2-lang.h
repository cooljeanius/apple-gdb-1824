/* m2-lang.h: Modula 2 language support definitions for GDB, the GNU debugger.

   Copyright 1992, 1998, 2000, 2005 Free Software Foundation, Inc.

   This file is part of GDB.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

#ifndef M2_LANG_H
#define M2_LANG_H 1

#include "language.h"

extern int m2_parse(void);	/* Defined in m2-exp.y */

extern void m2_error(const char *);	/* Defined in m2-exp.y */

extern int m2_char; /* Defined in m2-exp.y */

extern int m2_nerrs; /* Defined in m2-exp.y */

/* Defined in m2-typeprint.c */
extern void m2_print_type(struct type *, const char *, struct ui_file *, int,
			  int);

extern int m2_val_print(struct type *, const gdb_byte *, int, CORE_ADDR,
			struct ui_file *, int, int, int,
			enum val_prettyprint);

/* Defined in m2-lang.c */
extern struct type **const (m2_builtin_types[]);
extern const struct language_defn m2_language_defn;

#endif /* !M2_LANG_H */

/* EOF */
