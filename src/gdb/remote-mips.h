/* remote-mips.h */

#ifndef REMOTE_MIPS_H
#define REMOTE_MIPS_H 1

extern int mips_can_use_watchpoint(int, int, int);
extern int mips_insert_watchpoint(CORE_ADDR, int, int);
extern int mips_remove_watchpoint(CORE_ADDR, int, int);
extern int mips_stopped_by_watchpoint(void);

#endif /* !REMOTE_MIPS_H */

/* EOF */
