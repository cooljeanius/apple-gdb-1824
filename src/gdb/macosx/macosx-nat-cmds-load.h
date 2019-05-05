/* macosx/macosx-nat-cmds-load.h -*- C -*- */

#ifndef __GDB_MACOSX_NAT_CMDS_LOAD_H__
#define __GDB_MACOSX_NAT_CMDS_LOAD_H__ 1

extern void load_plugin(const char *arg, int from_tty);

extern void info_plugins_command(const char *arg, int from_tty);

extern void *_plugin_global_data;

#endif /* !__GDB_MACOSX_NAT_CMDS_LOAD_H__ */

/* EOF */
