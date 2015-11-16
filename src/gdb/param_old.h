/* rem-multi_old/param_old.h */
/* Copyright (C) 1990 Free Software Foundation, Inc.

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
Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */

/* The standard thing is to include param.h. However, files that are
 * specific to a particular target can include that tm-xxx.h file and
 * param-no-tm.h. Any future inclusions of param.h will be protected
 * against by the #if !defined stuff below.  */

/* This file requires defs.h (so why not include it?). */
#ifdef HAVE_CONFIG_H
# if !defined(__CONFIG_H__)
#  include "config.h"
# endif /* !__CONFIG_H__ */
# include "defs.h"
#endif /* HAVE_CONFIG_H */

#if !defined(PARAM_H)
# include "tm.h"
#endif /* !PARAM_H */

#include "param-no-tm_old.h" /* renamed from "param-no-tm.h" */

#define PARAM_H 1

/* EOF */
