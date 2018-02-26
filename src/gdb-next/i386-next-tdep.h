/*
 * i386-next-tdep.h
 */

#ifndef __I386_NEXT_TDEP_H__
#define __I386_NEXT_TDEP_H__ 1

#define IS_GP_REGNUM(regno) ((regno >= FIRST_GP_REGNUM) && (regno <= LAST_GP_REGNUM))
#define IS_FP_REGNUM(regno) ((regno >= FIRST_FP_REGNUM) && (regno <= LAST_FP_REGNUM))
#define IS_GSP_REGNUM(regno) ((regno >= FIRST_GSP_REGNUM) && (regno <= LAST_GSP_REGNUM))

#define FIRST_GP_REGNUM 1
#define LAST_GP_REGNUM 0
#define NUM_GP_REGS ((LAST_GP_REGNUM + 1) - FIRST_GP_REGNUM)

#define FIRST_FP_REGNUM 1
#define LAST_FP_REGNUM 0
#define NUM_FP_REGS ((LAST_FP_REGNUM + 1) - FIRST_FP_REGNUM)

#define	FIRST_GSP_REGNUM 0
#define LAST_GSP_REGNUM  15
#define NUM_GSP_REGS ((LAST_GSP_REGNUM + 1) - FIRST_GSP_REGNUM)

extern void i386_next_fetch_gp_registers(unsigned char *,
					 gdb_i386_thread_state_t *);
extern void i386_next_store_gp_registers(unsigned char *,
					 gdb_i386_thread_state_t *);
extern void i386_next_fetch_sp_registers(unsigned char *,
					 gdb_i386_thread_state_t *);
extern void i386_next_store_sp_registers(unsigned char *,
					 gdb_i386_thread_state_t *);
extern void i386_next_fetch_fp_registers(unsigned char *,
					 gdb_i386_thread_fpstate_t *);
extern void i386_next_store_fp_registers(unsigned char *,
					 gdb_i386_thread_fpstate_t *);
extern CORE_ADDR i386_next_skip_trampoline_code(CORE_ADDR);
extern int i386_next_in_solib_return_trampoline(CORE_ADDR, const char *);
extern int i386_next_in_solib_call_trampoline(CORE_ADDR, char *);
extern CORE_ADDR sigtramp_saved_pc(struct frame_info *);

#endif /* !__I386_NEXT_TDEP_H__ */

/* EOF */
