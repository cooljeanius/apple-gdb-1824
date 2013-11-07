#ifndef __TM_PPC_H__
#define __TM_PPC_H__

#ifndef GDB_MULTI_ARCH
#define GDB_MULTI_ARCH 1
#endif

struct frame_info;

void ppc_init_frame_pc_first PARAMS ((int fromleaf, struct frame_info *prev));
#define INIT_FRAME_PC_FIRST(FROMLEAF, PREV) ppc_init_frame_pc_first ((FROMLEAF), (PREV))

void ppc_init_frame_pc PARAMS ((int fromleaf, struct frame_info *prev));
#define INIT_FRAME_PC(FROMLEAF, PREV) ppc_init_frame_pc (FROMLEAF, PREV)

void ppc_stack_alloc PARAMS ((CORE_ADDR *sp, CORE_ADDR *start, size_t argsize, size_t len));
#define STACK_ALLOC(SP, NSP, LEN) ppc_stack_alloc (&(SP), &(NSP), 0, (LEN))

CORE_ADDR ppc_convert_from_func_ptr_addr PARAMS ((CORE_ADDR addr));
#define CONVERT_FROM_FUNC_PTR_ADDR(ADDR) ppc_convert_from_func_ptr_addr (ADDR)

int ppc_fast_show_stack_helper (int show_frames, int show_names, int *count);
#define FAST_COUNT_STACK_DEPTH(count) \
    (ppc_fast_show_stack_helper(0, 0, count))

#endif /* __TM_PPC_H__ */

