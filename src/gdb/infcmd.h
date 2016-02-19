/* infcmd.h -*- C -*- */

#ifndef __GDB_INFCMD_H__
#define __GDB_INFCMD_H__ 1

extern void kill_if_already_running(int from_tty);

extern void re_execute_command(const char *args, int from_tty);

#endif /* __GDB_INFCMD_H__ */

/* EOF */
