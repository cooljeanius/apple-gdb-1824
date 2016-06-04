/* h8500-tdep.h */

#ifndef H8500_TDEP_H
#define H8500_TDEP_H 1

struct frame_saved_regs;

extern int minimum_mode;

extern CORE_ADDR h8500_skip_prologue(CORE_ADDR);
extern CORE_ADDR h8500_addr_bits_remove(CORE_ADDR);
extern CORE_ADDR h8500_frame_chain(struct frame_info *);
extern CORE_ADDR NEXT_PROLOGUE_INSN(CORE_ADDR, CORE_ADDR, char *);
extern CORE_ADDR frame_saved_pc(struct frame_info *);
extern void h8500_pop_frame(void);
extern void print_register_hook(int);
extern int h8500_register_size(int);
extern struct type *h8500_register_virtual_type(int);

extern void frame_find_saved_regs(struct frame_info *,
				  struct frame_saved_regs *);
extern CORE_ADDR saved_pc_after_call(void);
extern void h8500_set_pointer_size(int);
extern int h8500_is_trapped_internalvar(char *);
extern struct value *h8500_value_of_trapped_internalvar(struct internalvar *);
extern void h8500_set_trapped_internalvar(struct internalvar *, struct value *,
					  int, int, int);
extern CORE_ADDR h8500_read_sp(void);
extern void h8500_write_sp(CORE_ADDR);
extern CORE_ADDR h8500_read_pc(ptid_t);
extern void h8500_write_pc(CORE_ADDR, ptid_t);
extern CORE_ADDR h8500_read_fp(void);
extern void h8500_write_fp(CORE_ADDR);

#endif /* !H8500_TDEP_H */

/* EOF */
