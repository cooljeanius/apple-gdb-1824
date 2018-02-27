/*
 * ppc-tdep.h
 */

#ifndef __PPC_TDEP_H__
#define __PPC_TDEP_H__

#include "defs.h"
#include "ppc-frameinfo.h"

#ifdef INVALID_ADDRESS
# undef INVALID_ADDRESS
#endif /* INVALID_ADDRESS */
/* Used in frameinfo, and passed to ppc_parse_instructions it means
 * keep looking as long as necessary... */
#define INVALID_ADDRESS 0xffffffff

struct type;
struct frame_info;
struct ppc_init_frame_pc_args;

struct frame_extra_info
{
  CORE_ADDR initial_sp;			/* initial stack pointer. */
  struct ppc_function_boundaries *bounds;
  struct ppc_function_properties *props;
};

extern void ppc_debug(const char *, ...) ATTR_FORMAT(gnu_printf, 1, 2);

const char *ppc_register_name(int regno);

/* core stack frame decoding functions */

void ppc_init_extra_frame_info PARAMS ((int fromleaf, struct frame_info *prev));

void ppc_print_extra_frame_info PARAMS ((struct frame_info *frame));

CORE_ADDR ppc_init_frame_pc_first PARAMS((int fromleaf, struct frame_info *prev));

CORE_ADDR ppc_init_frame_pc PARAMS((int fromleaf, struct frame_info *prev));

CORE_ADDR ppc_frame_saved_pc PARAMS ((struct frame_info *fi));

CORE_ADDR ppc_frame_saved_pc_after_call PARAMS ((struct frame_info *frame));

CORE_ADDR ppc_frame_prev_pc PARAMS ((struct frame_info *frame));

CORE_ADDR ppc_frame_chain PARAMS ((struct frame_info *frame));

int ppc_frame_chain_valid PARAMS ((CORE_ADDR chain, struct frame_info *frame));

/* more esoteric functions */

int ppc_is_dummy_frame PARAMS ((struct frame_info *frame));

CORE_ADDR ppc_frame_cache_initial_stack_address PARAMS ((struct frame_info *fi));
CORE_ADDR ppc_frame_initial_stack_address PARAMS ((struct frame_info *fi));

int ppc_is_magic_function_pointer PARAMS ((CORE_ADDR addr));

CORE_ADDR ppc_skip_trampoline_code PARAMS ((CORE_ADDR pc));

CORE_ADDR ppc_convert_from_func_ptr_addr PARAMS ((CORE_ADDR addr));

CORE_ADDR ppc_find_toc_address PARAMS ((CORE_ADDR pc));

int ppc_use_struct_convention PARAMS ((int gccp, struct type *valtype));

CORE_ADDR ppc_extract_struct_value_address PARAMS
  ((char regbuf[]));

void ppc_extract_return_value PARAMS
  ((struct type *valtype, struct regcache *regbuf, gdb_byte *valbuf));

CORE_ADDR ppc_skip_prologue PARAMS ((CORE_ADDR pc));

int ppc_frameless_function_invocation PARAMS ((struct frame_info *frame));

int ppc_invalid_float PARAMS ((char *f, size_t len));

extern int ppc_init_frame_pc_first_unsafe(struct ppc_init_frame_pc_args *);
extern CORE_ADDR ppc_get_unsaved_pc(struct frame_info *,
				    ppc_function_properties *);
extern CORE_ADDR ppc_frame_find_pc(struct frame_info *);
extern void ppc_store_struct_return(CORE_ADDR, CORE_ADDR);
extern void ppc_store_return_value(struct type *, struct regcache *,
				   const gdb_byte *);
extern CORE_ADDR ppc_push_return_address(CORE_ADDR, CORE_ADDR);
extern int ppc_register_convertible(int);
extern void ppc_register_convert_to_virtual(int, struct type *, char *, char *);
extern void ppc_register_convert_to_raw(struct type *, int, char *, char *);
extern void ppc_print_count_info(int, CORE_ADDR, CORE_ADDR, int);
extern void ppc_fast_show_stack(const char *, int);

#endif /* __PPC_TDEP_H__ */

/* EOF */
