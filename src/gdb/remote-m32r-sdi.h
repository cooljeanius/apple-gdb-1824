/* remote-m32r-sdi.h */

#ifndef REMOTE_M32R_SDI_H
#define REMOTE_M32R_SDI_H 1

extern int m32r_can_use_hw_watchpoint(int, int, int);
extern int m32r_insert_watchpoint(CORE_ADDR, int, int);
extern int m32r_remove_watchpoint(CORE_ADDR, int, int);
extern int m32r_stopped_data_address(struct target_ops *, CORE_ADDR *);
extern int m32r_stopped_by_watchpoint(void);

#endif /* !REMOTE_M32R_SDI_H */

/* EOF */
