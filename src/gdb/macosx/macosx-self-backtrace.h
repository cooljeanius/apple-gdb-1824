/*
 * macosx-self-backtrace.h
 */

#ifndef __GDB_MACOSX_SELF_BACKTRACE_H__
#define __GDB_MACOSX_SELF_BACKTRACE_H__ 1

int gdb_self_backtrace(void **buffer, int bufsize);

char **gdb_self_backtrace_symbols(void **addrbuf, int num_of_addrs);

void gdb_self_backtrace_symbols_fd(void **addrbuf, int num_of_addrs,
                                   int fd, int skip, int maxdepth);

#endif /* !__GDB_MACOSX_SELF_BACKTRACE_H__ */

/* EOF */
