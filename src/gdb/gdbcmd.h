/* ***DEPRECATED***  The gdblib files must not be calling/using things in any
   of the possible command languages.  If necessary, a hook (that may be
   present or not) must be used and set to the appropriate routine by any
   command language that cares about it.  If you are having to include this
   file you are possibly doing things the old way.  This file will disapear.
   <fnasser@redhat.com>    */

/* Header file for GDB-specific command-line stuff.
   Copyright 1986, 1989, 1990, 1991, 1992, 1993, 1994, 1998, 1999,
   2000, 2002 Free Software Foundation, Inc.

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

#if !defined(GDBCMD_H)
#define GDBCMD_H 1

#include "ansidecl.h"

#include "command.h"
#include "ui-out.h"

/* Since this entire file is deprecated, mark everything in it as
 * deprecated, as well.  */

/* Chain containing all defined commands: */
extern struct cmd_list_element *cmdlist ATTRIBUTE_DEPRECATED;

/* Chain containing all defined info subcommands: */
extern struct cmd_list_element *infolist ATTRIBUTE_DEPRECATED;

/* Chain containing all defined enable subcommands: */
extern struct cmd_list_element *enablelist ATTRIBUTE_DEPRECATED;

/* Chain containing all defined disable subcommands: */
extern struct cmd_list_element *disablelist ATTRIBUTE_DEPRECATED;

/* Chain containing all defined delete subcommands: */
extern struct cmd_list_element *deletelist ATTRIBUTE_DEPRECATED;

/* Chain containing all defined toggle subcommands: */
extern struct cmd_list_element *togglelist ATTRIBUTE_DEPRECATED;

/* Chain containing all defined stop subcommands: */
extern struct cmd_list_element *stoplist ATTRIBUTE_DEPRECATED;

/* Chain containing all defined "enable breakpoint" subcommands: */
extern struct cmd_list_element *enablebreaklist ATTRIBUTE_DEPRECATED;

/* Chain containing all defined set subcommands: */
extern struct cmd_list_element *setlist ATTRIBUTE_DEPRECATED;

/* Chain containing all defined unset subcommands: */
extern struct cmd_list_element *unsetlist ATTRIBUTE_DEPRECATED;

/* Chain containing all defined show subcommands: */
extern struct cmd_list_element *showlist ATTRIBUTE_DEPRECATED;

/* Chain containing all defined \"set history\": */
extern struct cmd_list_element *sethistlist ATTRIBUTE_DEPRECATED;

/* Chain containing all defined \"show history\": */
extern struct cmd_list_element *showhistlist ATTRIBUTE_DEPRECATED;

/* Chain containing all defined \"unset history\": */
extern struct cmd_list_element *unsethistlist ATTRIBUTE_DEPRECATED;

/* Chain containing all defined maintenance subcommands: */
extern struct cmd_list_element *maintenancelist ATTRIBUTE_DEPRECATED;

/* Chain containing all defined "maintenance info" subcommands: */
extern struct cmd_list_element *maintenanceinfolist ATTRIBUTE_DEPRECATED;

/* Chain containing all defined "maintenance print" subcommands: */
extern struct cmd_list_element *maintenanceprintlist ATTRIBUTE_DEPRECATED;

/* Chain containing all defined "maintenance set" subcommands: */
extern struct cmd_list_element *maintenance_set_cmdlist
  ATTRIBUTE_DEPRECATED;

/* Chain containing all defined "maintenance show" subcommands: */
extern struct cmd_list_element *maintenance_show_cmdlist
  ATTRIBUTE_DEPRECATED;

extern struct cmd_list_element *setprintlist ATTRIBUTE_DEPRECATED;

extern struct cmd_list_element *showprintlist ATTRIBUTE_DEPRECATED;

extern struct cmd_list_element *setdebuglist ATTRIBUTE_DEPRECATED;

extern struct cmd_list_element *showdebuglist ATTRIBUTE_DEPRECATED;

extern struct cmd_list_element *setchecklist ATTRIBUTE_DEPRECATED;

extern struct cmd_list_element *showchecklist ATTRIBUTE_DEPRECATED;

extern void execute_command(const char *, int)
  ATTRIBUTE_DEPRECATED;

enum command_control_type execute_control_command(struct command_line *)
  ATTRIBUTE_DEPRECATED;

extern void print_command_line(struct command_line *, unsigned int,
                               struct ui_file *)
  ATTRIBUTE_DEPRECATED;
extern void print_command_lines(struct ui_out *,
                                struct command_line *, unsigned int)
  ATTRIBUTE_DEPRECATED;

#endif /* !defined(GDBCMD_H) */

/* EOF */
