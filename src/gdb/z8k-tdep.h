/* z8k-tdep.h */

#ifndef Z8K_TDEP_H
#define Z8K_TDEP_H 1

extern CORE_ADDR z8k_frame_saved_pc(struct frame_info *);

extern CORE_ADDR z8k_skip_prologue(CORE_ADDR);
extern CORE_ADDR z8k_addr_bits_remove(CORE_ADDR);
extern CORE_ADDR z8k_frame_chain(struct frame_info *);
extern void init_frame_pc(void);
extern void z8k_frame_init_saved_regs(struct frame_info *);
extern void z8k_push_dummy_frame(void);
extern int gdb_print_insn_z8k(bfd_vma, disassemble_info *);
extern CORE_ADDR NEXT_PROLOGUE_INSN(CORE_ADDR, CORE_ADDR, short *);
extern int z8k_saved_pc_after_call(struct frame_info *);
extern void extract_return_value(struct type *, char *, char *);
extern void write_return_value(struct type *, char *);
extern void store_struct_return(CORE_ADDR, CORE_ADDR);
extern void z8k_print_register_hook(int);
extern void z8k_pop_frame(void);
extern void z8k_set_pointer_size(int);

#endif /* !Z8K_TDEP_H */

/* EOF */
