/*
 * macosx/arm-macosx-tdep.h
 */

#ifndef __GDB_ARM_MACOSX_TDEP_H__
#define __GDB_ARM_MACOSX_TDEP_H__

#if !defined(NO_POISON) && defined(POISON_FREE_TOO)
/* included files can drag in "free": */
# define NO_POISON 1
#endif /* !NO_POISON && POISON_FREE_TOO */

#include "defs.h"
#include "arm-macosx-thread-status.h"
#include "arm-macosx-regnums.h"
#ifndef __GDB_MACOSX_TDEP_H__
# include "macosx-tdep.h"
#endif /* !__GDB_MACOSX_TDEP_H__ */

#ifndef INVALID_ADDRESS
# define INVALID_ADDRESS ((CORE_ADDR)(-1L))
#endif /* !INVALID_ADDRESS */

void arm_macosx_fetch_gp_registers(struct gdb_arm_thread_state *gp_regs);
void arm_macosx_fetch_gp_registers_raw(struct gdb_arm_thread_state *gp_regs);
void arm_macosx_store_gp_registers(struct gdb_arm_thread_state *gp_regs);
void arm_macosx_store_gp_registers_raw(struct gdb_arm_thread_state *gp_regs);
void arm_macosx_fetch_vfpv1_regs(gdb_arm_thread_vfpv1_state_t *);
void arm_macosx_fetch_vfpv3_regs(gdb_arm_thread_vfpv3_state_t *);
void arm_macosx_fetch_vfpv1_regs_raw(gdb_arm_thread_vfpv1_state_t *);
void arm_macosx_fetch_vfpv3_regs_raw(gdb_arm_thread_vfpv3_state_t *);
void arm_macosx_store_vfpv1_regs(gdb_arm_thread_vfpv1_state_t *);
void arm_macosx_store_vfpv3_regs(gdb_arm_thread_vfpv3_state_t *);
void arm_macosx_store_vfpv1_regs_raw(gdb_arm_thread_vfpv1_state_t *);
void arm_macosx_store_vfpv3_regs_raw(gdb_arm_thread_vfpv3_state_t *);
int arm_macosx_keep_going(CORE_ADDR stop_pc);
void *arm_macosx_save_thread_inferior_status(void);
void arm_macosx_restore_thread_inferior_status(void *);
void arm_macosx_free_thread_inferior_status(void *);
enum gdb_osabi arm_host_osabi(void);
enum gdb_osabi arm_set_osabi_from_host_info(void);
int arm_macosx_in_switch_glue(CORE_ADDR pc);
CORE_ADDR arm_fetch_pointer_argument(struct frame_info *, int,
                                     struct type *);
int arm_macosx_stab_reg_to_regnum(int);

#endif /* __GDB_ARM_MACOSX_TDEP_H__ */

/* EOF */
