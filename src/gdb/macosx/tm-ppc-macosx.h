/*
 * macosx/tm-ppc-macosx.h
 */

#ifndef __GDB_TM_PPC_MACOSX_H__
#define __GDB_TM_PPC_MACOSX_H__ 1

#ifndef GDB_MULTI_ARCH
# define GDB_MULTI_ARCH 1
#endif /* !GDB_MULTI_ARCH */

int
ppc_fast_show_stack(unsigned int count_limit, unsigned int print_start,
                    unsigned int print_end, unsigned int *count,
                    void (print_fun)(struct ui_out *uiout, int *frame_num,
                                     CORE_ADDR pc, CORE_ADDR fp));
#ifndef FAST_COUNT_STACK_DEPTH
# define FAST_COUNT_STACK_DEPTH(count_limit, print_start, print_end, count, print_fun) \
  (ppc_fast_show_stack(count_limit, print_start, print_end, count, print_fun))
#endif /* !FAST_COUNT_STACK_DEPTH */

char *ppc_throw_catch_find_typeinfo(struct frame_info *curr_frame,
                                    int exception_type);
#ifndef THROW_CATCH_FIND_TYPEINFO
# define THROW_CATCH_FIND_TYPEINFO(curr_frame, exception_type) \
  (ppc_throw_catch_find_typeinfo(curr_frame, exception_type))
#endif /* !THROW_CATCH_FIND_TYPEINFO */

#endif /* __GDB_TM_PPC_MACOSX_H__ */

/* EOF */
