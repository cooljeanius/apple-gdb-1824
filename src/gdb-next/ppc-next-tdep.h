/*
 * ppc-next-tdep.h
 */

#ifndef __PPC_NEXT_TDEP_H__
#define __PPC_NEXT_TDEP_H__ 1

#include "tm-ppc.h"

#define IS_GP_REGNUM(regno) ((regno >= FIRST_GP_REGNUM) && (regno <= LAST_GP_REGNUM))
#define IS_FP_REGNUM(regno) ((regno >= FIRST_FP_REGNUM) && (regno <= LAST_FP_REGNUM))
#define IS_VP_REGNUM(regno) ((regno >= FIRST_VP_REGNUM) && (regno <= LAST_VP_REGNUM))

#define IS_GSP_REGNUM(regno) ((regno >= FIRST_GSP_REGNUM) && (regno <= LAST_GSP_REGNUM))
#define IS_FSP_REGNUM(regno) ((regno >= FIRST_FSP_REGNUM) && (regno <= LAST_FSP_REGNUM))
#define IS_VSP_REGNUM(regno) ((regno >= FIRST_VSP_REGNUM) && (regno <= LAST_VSP_REGNUM))

#include "ppc-thread-status.h"

extern void ppc_next_fetch_sp_registers PARAMS((unsigned char *, gdb_ppc_thread_state_t *));
extern void ppc_next_store_sp_registers PARAMS((unsigned char *, gdb_ppc_thread_state_t *));
extern void ppc_next_fetch_gp_registers PARAMS((unsigned char *, gdb_ppc_thread_state_t *));
extern void ppc_next_store_gp_registers PARAMS((unsigned char *, gdb_ppc_thread_state_t *));
extern void ppc_next_fetch_fp_registers PARAMS((unsigned char *, gdb_ppc_thread_fpstate_t *));
extern void ppc_next_store_fp_registers PARAMS((unsigned char *, gdb_ppc_thread_fpstate_t *));
extern void ppc_next_fetch_vp_registers PARAMS((unsigned char *, gdb_ppc_thread_vpstate_t *));
extern void ppc_next_store_vp_registers PARAMS((unsigned char *, gdb_ppc_thread_vpstate_t *));
extern CORE_ADDR ppc_next_skip_trampoline_code PARAMS((CORE_ADDR));
extern int ppc_next_in_solib_return_trampoline PARAMS((CORE_ADDR, char *));
extern int ppc_next_in_solib_call_trampoline PARAMS((CORE_ADDR, char *));

extern CORE_ADDR ppc_next_dynamic_trampoline_nextpc PARAMS((CORE_ADDR));
extern int ppc_next_in_solib_dynsym_resolve_code PARAMS((CORE_ADDR));

#endif /* !__PPC_NEXT_TDEP_H__ */

/* EOF */
