/*
 * gdb-next/ppc-tdep.c
 */

#include "ppc-reg.h"
#include "ppc-tdep.h"
#include "ppc-frameops.h"

#include "defs.h"
#include "ax.h"
#include "exceptions.h"
#include "frame.h"
#include "inferior.h"
#include "symtab.h"
#include "target.h"
#include "gdbcore.h"
#include "symfile.h"
#include "objfiles.h"
#include "gdbcmd.h"
#include "arch-utils.h"
#include "floatformat.h"
#include "gdbtypes.h"
#include "regcache.h"

#include "coff/internal.h"	/* for libcoff.h */

#include "libbfd.h"		/* for bfd_default_set_arch_mach */
#include "libcoff.h"		/* for xcoff_data */

#include "elf-bfd.h"

#include "ppc-frameinfo.h"
#include "ppc-frameops.h"

#undef XMALLOC
#define XMALLOC(TYPE) ((TYPE *)xmalloc(sizeof(TYPE)))

#ifndef CPU_TYPE_POWERPC
# define CPU_TYPE_POWERPC (18)
#endif /* !CPU_TYPE_POWERPC */

#ifndef store_floating
# define store_floating(a, l, v) deprecated_store_floating(a, l, v)
#endif /* !store_floating */

#ifndef extract_floating
# define extract_floating(a, l) deprecated_extract_floating(a, l)
#endif /* !extract_floating */

#ifndef REGISTER_BYTE
# ifdef DEPRECATED_REGISTER_BYTE
#  define REGISTER_BYTE(foo) DEPRECATED_REGISTER_BYTE(foo)
# endif /* DEPRECATED_REGISTER_BYTE */
#endif /* REGISTER_BYTE */

struct gdbarch_tdep
{
  void *unused;
};

/* external functions and globals */

extern int print_insn_big_powerpc PARAMS((bfd_vma, struct disassemble_info *));
extern struct obstack frame_cache_obstack;
extern int stop_stack_dummy;

/* local definitions */

static int ppc_debugflag = 0;

void ppc_debug(const char *fmt, ...)
{
  va_list ap;
  if (ppc_debugflag) {
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
  }
}

/* Default offset from SP where the LR is stored */

#define	DEFAULT_LR_SAVE 8

/* function implementations */

void
ppc_init_extra_frame_info(int fromleaf, struct frame_info *frame)
{
  CHECK_FATAL (frame != NULL);

#if 0
  frame->extra_info = ((struct frame_extra_info *)
		       frame_obstack_alloc(sizeof(struct frame_extra_info)));

  frame->extra_info->initial_sp = 0;
  frame->extra_info->bounds = NULL;
  frame->extra_info->props = 0;
  frame->signal_handler_caller = 0;
#endif /* 0 */
}

void
ppc_print_extra_frame_info(struct frame_info *frame)
{
#if 0
  if (frame->signal_handler_caller) {
    printf_filtered (" This function was called from a signal handler.\n");
  } else {
    printf_filtered (" This function was not called from a signal handler.\n");
  }
#endif /* 0 */

  ppc_frame_cache_initial_stack_address (frame);
#if 0
  if (frame->extra_info->initial_sp) {
    printf_filtered (" The initial stack pointer for this frame was 0x%lx.\n",
		     (unsigned long) frame->extra_info->initial_sp);
  } else {
    printf_filtered (" Unable to determine initial stack pointer for this frame.\n");
  }
#endif /* 0 */

  ppc_frame_cache_boundaries (frame, NULL);
#if 0
  if (frame->extra_info->bounds != NULL) {
    ppc_print_boundaries (frame->extra_info->bounds);
  } else {
    printf_filtered (" Unable to determine function boundary information.\n");
  }
#endif /* 0 */

  ppc_frame_cache_properties (frame, NULL);
#if 0
  if (frame->extra_info->props != NULL) {
    ppc_print_properties (frame->extra_info->props);
  } else {
    printf_filtered (" Unable to determine function property information.\n");
  }
#endif /* 0 */
}
/* We need to make sure that ppc_init_frame_pc_first will NOT longjmp,
 * since if it does it can leave a frame structure with no extra_info
 * allocated in get_prev_frame, and there are way too many places that
 * assume that a valid frame has a valid extra_info to catch them all. */

struct ppc_init_frame_pc_args
{
  int fromleaf;
  struct frame_info *frame;
};

int
ppc_init_frame_pc_first_unsafe(struct ppc_init_frame_pc_args *args)
{
  struct frame_info *frame = args->frame;
  struct frame_info *next;

  CHECK_FATAL (frame != NULL);
  next = get_next_frame (frame);
  CHECK_FATAL (next != NULL);
#if 0
  frame->pc = ppc_frame_saved_pc (next);
#endif /* 0 */

  return 1;
}

CORE_ADDR
ppc_init_frame_pc_first(int fromleaf, struct frame_info *frame)
{
  struct ppc_init_frame_pc_args args;

  args.fromleaf = fromleaf;
  args.frame = frame;

  if (!catch_errors ((catch_errors_ftype *) ppc_init_frame_pc_first_unsafe,
		     &args, "", RETURN_MASK_ERROR))
    {
      ppc_debug ("ppc_init_frame_pc_first: got an error calling %s.\n",
		 "ppc_frame_saved_pc.\n");
    }
  return 0UL;
}

CORE_ADDR
ppc_init_frame_pc(int fromleaf, struct frame_info *frame)
{
  CHECK_FATAL (frame != NULL);
  return 0UL;
}

/* ppc_get_unsaved_pc
 *
 * This function hides a little complication when you are looking for
 * a the saved pc in a frame which either does not save the pc, or saves
 * it but has NOT done so yet. In that case, the pc may still be in the
 * link register, but it ALSO might have gotten displaced by the PIC base
 * setting call - which does "blc pc+4; mflr r31" so the lr is wrong for
 * a little while.
 */
CORE_ADDR
ppc_get_unsaved_pc(struct frame_info *frame, ppc_function_properties *props)
{
  struct frame_info *cur_frame;
  CORE_ADDR retval;

  if ((props->lr_reg == -1)
      || ((props->lr_invalid == 0)
#if 0
	  || (frame->pc <= props->lr_invalid)
	  || (frame->pc > props->lr_valid_again)
#endif
	  || 0))
    {
      ppc_debug ("ppc_get_unsaved_pc: link register was not saved.\n");
      cur_frame = get_current_frame ();
#if 0
      set_current_frame (frame);
#endif /* 0 */
      retval = read_register (LR_REGNUM);
#if 0
      set_current_frame (cur_frame);
#else
      if (cur_frame == NULL) {
	(void)cur_frame;
      }
#endif /* 0 */
    }
  else
    {
      ppc_debug ("ppc_get_unsaved_pc: link register stashed in register %d.\n",
		 props->lr_reg);
      cur_frame = get_current_frame ();
#if 0
      set_current_frame (frame);
#endif /* 0 */
      retval = read_register (props->lr_reg);
#if 0
      set_current_frame (cur_frame);
#else
      if (cur_frame == NULL) {
	(void)cur_frame;
      }
#endif /* 0 */
    }
  return retval;
}

CORE_ADDR
ppc_frame_find_pc(struct frame_info *frame)
{
  CORE_ADDR prev;

  CHECK_FATAL (frame != NULL);

#if 0
  if (frame->signal_handler_caller) {
    CORE_ADDR psp = read_memory_unsigned_integer (frame->frame, 4);
    CORE_ADDR retval;
    /* psp is the top of the signal handler data pushed by the kernel */
    /* 0x220 is offset to SIGCONTEXT; 0x10 is offset to $pc */
    ppc_debug ("ppc_frame_saved_pc: determing previous pc from signal context\n");

    retval = read_memory_unsigned_integer (psp + 0x70, 4);
    ppc_debug ("Signal frame at: 0x%lx, saved pc at: 0x%lx.\n", psp, retval);
    return retval;
  }
#else
  if (frame != NULL) {
    CORE_ADDR psp = 0UL;
    CORE_ADDR retval;
    ppc_debug("ppc_frame_saved_pc: determing previous pc from signal context\n");

    retval = read_memory_unsigned_integer((psp + 0x70), 4);
    ppc_debug("Signal frame at: 0x%lx, saved pc at: 0x%lx.\n", psp, retval);
    return retval;
  }
#endif /* 0 */

  prev = ppc_frame_chain (frame);
  if ((prev == 0) || (! ppc_frame_chain_valid (prev, frame))) {
    ppc_debug ("ppc_frame_find_pc: previous stack frame not valid: returning 0\n");
    return 0;
  }
  ppc_debug ("ppc_frame_find_pc: value of prev is 0x%lx\n", (unsigned long) prev);

  if (ppc_frame_cache_properties (frame, NULL) != 0)
    {
#if 0
      ppc_debug("ppc_frame_find_pc: unable to find properties of function containing 0x%lx\n",
		(unsigned long)frame->pc);
#else
      ppc_debug("ppc_frame_find_pc: unable to find properties of function\n");
#endif /* 0 */
      ppc_debug("ppc_frame_find_pc: assuming link register saved in normal location\n");

      if (ppc_frameless_function_invocation (frame))
	{
	  /* FIXME: Formally, we ought to check for frameless function
	   * invocation, but in fact, ppc_frameless_function_invocation
	   * first calls ppc_frame_cache_properties and returns 0 if it
	   * fails. Since this just failed above, we will NOT ever get
	   * into this branch. But I do NOT know another way to figure
	   * this out. */
	  return read_register (LR_REGNUM);
	}
      else
	{
	  ppc_function_properties lprops;
	  CORE_ADDR body_start;
#if 0
	  body_start = ppc_parse_instructions(frame->pc, INVALID_ADDRESS,
					      &lprops);
#else
	  body_start = ppc_parse_instructions(INVALID_ADDRESS, INVALID_ADDRESS,
					      &lprops);
#endif /* 0 */

	  if (body_start == INVALID_ADDRESS) {
	    ; /* ??? */
	  }

	  /* We get here if we are a bit lost: after all we were NOT
	   * able to successfully run ppc_frame_cache_properties.  So
	   * we should treat the lprops with some caution. One common
	   * mistake is to falsely assume the lr has NOT been saved
	   * (since un-interpretible prologue and frameless prologue
	   * look kind of the same). So add this obvious check
	   * here for being a leaf fn. in the first branch... */
#if 0
	  if (((frame->next == NULL) && (lprops.lr_saved == 0))
	      || (lprops.lr_saved >= frame->pc))
	    {
      	      /* Either we are NOT saving the link register, or our scan
      	       * shows that the link register WILL be saved, but has not
      	       * been yet. So figure out where it actually is. To do this,
      	       * we need to see if the lr is ever stomped, and if so, if we
      	       * are within that part of the prologue.
      	       */
	      return ppc_get_unsaved_pc (frame, &lprops);
	    }
	  else
	    {
	      /* The link register is safely on the stack... */
	      return read_memory_unsigned_integer (prev + DEFAULT_LR_SAVE, 4);
	    }
#else
	  return read_memory_unsigned_integer((prev + DEFAULT_LR_SAVE), 4);
#endif /* 0 */
	}
    }
  else
    {
      ppc_function_properties *props;

#if 0
      props = frame->extra_info->props;
#else
      props = (ppc_function_properties *)frame;
#endif /* 0 */
      CHECK_FATAL (props != NULL);

      if (props->lr_saved)
	{
#if 0
	  if (props->lr_saved < frame->pc)
	    {
	      /* The link register is safely on the stack... */
	      return read_memory_unsigned_integer (prev
						   + props->lr_offset, 4);
	    }
	  else
	    {
	      ppc_debug ("ppc_frame_find_pc: function did not save link register\n");
	      return ppc_get_unsaved_pc (frame, props);
	    }
#else
	  return INVALID_ADDRESS;
#endif /* 0 */
        }
      else if (frame->next && frame->next->signal_handler_caller)
	{
	  ppc_debug ("ppc_frame_find_pc: %s\n",
		     "using link area from signal handler.");
	  return read_memory_unsigned_integer (frame->frame - 0x320
					       + DEFAULT_LR_SAVE, 4);
	}
      else if (frame->next && ppc_is_dummy_frame (frame->next))
	{
	  ppc_debug ("ppc_frame_find_pc: using link area from call dummy\n");
	  return read_memory_unsigned_integer (frame->frame - 0x1c, 4);
	}
      else if (!props->lr_saved)
	{
	  return ppc_get_unsaved_pc (frame, props);
	}
      else
	{
	  ppc_debug ("ppc_frame_find_pc: function is not a leaf\n");
	  ppc_debug ("ppc_frame_find_pc: assuming link register saved in normal location\n");
	  return read_memory_unsigned_integer (prev + DEFAULT_LR_SAVE, 4);
	}
    }
  /* Should never get here, just quiets the compiler. */
  return 0;
}

/* */
CORE_ADDR
ppc_frame_saved_pc(struct frame_info *frame)
{
  return (ppc_frame_find_pc (frame));
}

/* */
CORE_ADDR
ppc_frame_saved_pc_after_call(struct frame_info *frame)
{
  CHECK_FATAL (frame != NULL);
  return read_register (LR_REGNUM);
}

/* */
CORE_ADDR
ppc_frame_chain(struct frame_info *frame)
{
  CORE_ADDR psp = read_memory_unsigned_integer (frame->frame, 4);

  if (frame->signal_handler_caller) {
    /* psp is the top of the signal handler data pushed by the kernel */
    /* 0x70 is offset to PPC_SAVED_STATE; 0xc is offset to $r1 */
    return read_memory_unsigned_integer (psp + 0x70 + 0xc, 4);
  }

  /* If a frameless function is interrupted by a signal, no change to
     the stack pointer */
  if (frame->next != NULL
      && frame->next->signal_handler_caller
      && ppc_frameless_function_invocation (frame)) {
    return frame->frame;
  }

  return psp;
}

int
ppc_frame_chain_valid(CORE_ADDR chain, struct frame_info *frame)
{
  unsigned long retval;

  if (chain == 0) { return 0; }

  /* reached end of stack? */
  if (!safe_read_memory_unsigned_integer (chain, 4, &retval))
    {
        ppc_debug ("ppc_frame_chain_valid: Got an error reading at 0x%lx",
                   chain);
        return 0;
    }
  else
    if (retval == 0) { return 0; }

#if 0
  if (inside_entry_func (frame->pc)) { return 0; }
  if (inside_main_func (frame->pc)) { return 0; }
#endif /* 0 */

  /* check for bogus stack frames */
  if (! (chain >= frame->frame)) {
    warning ("ppc_frame_chain_valid: stack pointer from 0x%lx to 0x%lx "
	     "grows upward; assuming invalid\n",
	     (unsigned long) frame->frame, (unsigned long) chain);
    return 0;
  }
  if ((chain - frame->frame) > 65536) {
    warning ("ppc_frame_chain_valid: stack frame from 0x%lx to 0x%lx "
	     "larger than 65536 bytes; assuming invalid",
	     (unsigned long) frame->frame, (unsigned long) chain);
    return 0;
  }

  return 1;
}

/* */
int
ppc_is_dummy_frame(struct frame_info *frame)
{
  /* using get_prev_frame or ppc_frame_chain
     would cause infinite recursion in some cases */

  CORE_ADDR chain = read_memory_unsigned_integer (frame->frame, 4);

  if (frame->signal_handler_caller) {
    /* psp is the top of the signal handler data pushed by the kernel */
    /* 0x70 is offset to PPC_SAVED_STATE; 0xc is offset to $r1 */
    chain = read_memory_unsigned_integer (chain + 0x70 + 0xc, 4);
  }

  if (chain == 0) { return 0; }
#ifdef PC_IN_CALL_DUMMY
  return (PC_IN_CALL_DUMMY (frame->pc, frame->frame, chain));
#else
  return -1;
#endif /* PC_IN_CALL_DUMMY */
}

/* Return the address of a frame. This is the inital %sp value when the frame
   was first allocated. For functions calling alloca(), it might be saved in
   the frame pointer register. */

CORE_ADDR
ppc_frame_cache_initial_stack_address(struct frame_info *frame)
{
  CHECK_FATAL (frame != NULL);
#if 0
  CHECK_FATAL (frame->extra_info != NULL);

  if (frame->extra_info->initial_sp == 0) {
    frame->extra_info->initial_sp = ppc_frame_initial_stack_address (frame);
  }
  return frame->extra_info->initial_sp;
#else
  return INVALID_ADDRESS;
#endif /* 0 */
}

/* */
CORE_ADDR
ppc_frame_initial_stack_address(struct frame_info *frame)
{
  CORE_ADDR tmpaddr;
  struct frame_info *callee;

  /* Find out if this function is using an alloca register. */

  if (ppc_frame_cache_properties (frame, NULL) != 0) {
    ppc_debug ("ppc_frame_initial_stack_address: unable to find properties of "
		 "function containing 0x%lx\n", frame->pc);
    return 0;
  }

  /* Read and cache saved registers if necessary. */

  ppc_frame_cache_saved_regs (frame);

  /* If no alloca register is used, then frame->frame is the value of
     %sp for this frame, and it is valid. */

  if (frame->extra_info->props->frameptr_reg < 0) {
    frame->extra_info->initial_sp = frame->frame;
    return frame->extra_info->initial_sp;
  }

  /* This function has an alloca register. If this is the top-most frame
     (with the lowest address), the value in alloca register is valid. */

  if (! get_next_frame (frame))
    {
      frame->extra_info->initial_sp
	= read_register (frame->extra_info->props->frameptr_reg);
      return frame->extra_info->initial_sp;
    }

  /* Otherwise, this is a caller frame. Callee has usually already saved
   * registers, but there are exceptions (such as when the callee
   * has no parameters). Find the address in which caller's alloca
   * register is saved. */

  for (callee = get_next_frame (frame); callee != NULL;
       callee = get_next_frame (callee))
    {

      ppc_frame_cache_saved_regs (callee);

      /* this is the address in which alloca register is saved. */

      tmpaddr = callee->saved_regs[frame->extra_info->props->frameptr_reg];
      if (tmpaddr) {
	frame->extra_info->initial_sp
	  = read_memory_unsigned_integer (tmpaddr, 4);
	return frame->extra_info->initial_sp;
      }

      /* Go look into deeper levels of the frame chain to see if any one of
       * the callees has saved the frame pointer register. */
    }

#if 0
  /* If frame pointer register was not saved, by the callee (or any of
   * its callees) then the value in the register is still good. */

  frame->extra_info->initial_sp
    = read_register (frame->extra_info->props->frameptr_reg);
  return frame->extra_info->initial_sp;
#else
  return -1;
#endif /* 0 */
}

int
ppc_is_magic_function_pointer(CORE_ADDR addr)
{
  return 0;
}

/* Usually a function pointer's representation is simply the address of
 * the function. On the RS/6000 however, a function pointer is represented
 * by a pointer to a TOC entry. This TOC entry contains three words,
 * the first word is the address of the function, the second word is the
 * TOC pointer (r2), and the third word is the static chain value.
 * Throughout GDB it is currently assumed that a function pointer contains
 * the address of the function, which is not easy to fix.
 * In addition, the conversion of a function address to a function
 * pointer would require allocation of a TOC entry in the inferior's
 * memory space, with all its drawbacks.
 * To be able to call C++ virtual methods in the inferior (which are called
 * via function pointers), find_function_addr uses this macro to
 * get the function address from a function pointer.  */

CORE_ADDR
ppc_convert_from_func_ptr_addr(CORE_ADDR addr)
{
  return (ppc_is_magic_function_pointer (addr) ? read_memory_unsigned_integer (addr, 4) : (addr));
}

CORE_ADDR
ppc_find_toc_address(CORE_ADDR pc)
{
  return 0;
}

int ppc_use_struct_convention(int gccp, struct type *valtype)
{
  return 1;
}

CORE_ADDR
ppc_extract_struct_value_address(char regbuf[REGISTER_BYTES])
{
  return extract_unsigned_integer((const gdb_byte *)&regbuf[REGISTER_BYTE(GP0_REGNUM + 3)], 4);
}

#ifndef MAX_REGISTER_RAW_SIZE
/* arbitrarily made-up default: */
# define MAX_REGISTER_RAW_SIZE 4
#endif /* !MAX_REGISTER_RAW_SIZE */
#ifndef REGISTER_RAW_SIZE
# ifdef DEPRECATED_REGISTER_RAW_SIZE
#  define REGISTER_RAW_SIZE(foo) DEPRECATED_REGISTER_RAW_SIZE(foo)
# else
#  define REGISTER_RAW_SIZE(foo) MAX_REGISTER_RAW_SIZE
# endif /* DEPRECATED_REGISTER_RAW_SIZE */
#endif /* !REGISTER_RAW_SIZE */

/* */
void
ppc_extract_return_value(struct type *valtype,
			 struct regcache *regbuf,
			 gdb_byte *valbuf)
{
  int offset = 0;

  if (TYPE_CODE (valtype) == TYPE_CODE_FLT) {

    /* floats and doubles are returned in fpr1. fpr's have a size of 8 bytes.
     * We need to truncate the return value into float size (4 byte) if
     * necessary. */

    double dd;
    float ff;

    switch (TYPE_LENGTH(valtype)) {
    case 8: /* double */
      memcpy(valbuf, &regbuf[REGISTER_BYTE(FP0_REGNUM + 1)], 8);
      break;
    case 4:
      memcpy(&dd, &regbuf[REGISTER_BYTE(FP0_REGNUM + 1)], 8);
      ff = (float)dd;
      memcpy(valbuf, &ff, sizeof(float));
      break;
    default:
      error("unknown TYPE_LENGTH for return type %d", TYPE_LENGTH(valtype));
    }

  } else {

    unsigned int gpretreg = GP0_REGNUM + 3;
    /* return value is copied starting from r3. */
    if ((TARGET_BYTE_ORDER == BFD_ENDIAN_BIG)
	&& (TYPE_LENGTH(valtype) < REGISTER_RAW_SIZE(gpretreg)))
      offset = (REGISTER_RAW_SIZE(gpretreg) - TYPE_LENGTH(valtype));

#if defined(HAVE_MEMCPY) && defined(REGISTER_BYTE) && defined(TYPE_LENGTH)
    memcpy(valbuf, (regbuf + REGISTER_BYTE(gpretreg) + offset),
	   TYPE_LENGTH(valtype));
#endif /* HAVE_MEMCPY && REGISTER_BYTE && TYPE_LENGTH */
  }
}

CORE_ADDR
ppc_skip_prologue(CORE_ADDR pc)
{
  ppc_function_boundaries_request request;
  ppc_function_boundaries bounds;
  int ret;

  ppc_clear_function_boundaries_request (&request);
  ppc_clear_function_boundaries (&bounds);

  request.prologue_start = pc;
  ret = ppc_find_function_boundaries (&request, &bounds);
  if (ret != 0) { return 0; }

  return bounds.body_start;
}

/* Determines whether the current frame has allocated a frame on the
   stack or not.  */

int
ppc_frameless_function_invocation(struct frame_info *frame)
{
  /* if not a leaf, it is not frameless (unless it was interrupted by a
   * signal or a call_dummy) */

  if (frame->next != NULL
      && !frame->next->signal_handler_caller
      && !ppc_is_dummy_frame (frame->next))
    {
      return 0;
    }

  if (ppc_frame_cache_properties (frame, NULL) != 0) {
    ppc_debug ("frameless_function_invocation: unable to find properties of "
		 "function containing 0x%lx; assuming not frameless\n",
	       frame->pc);
    return 0;
  }

#if 0
  return frame->extra_info->props->frameless;
#else
  return -1;
#endif /* 0 */
}

const char *gdb_register_names[] =
{
  "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7",
  "r8", "r9", "r10","r11","r12","r13","r14","r15",
  "r16","r17","r18","r19","r20","r21","r22","r23",
  "r24","r25","r26","r27","r28","r29","r30","r31",
  "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7",
  "f8", "f9", "f10","f11","f12","f13","f14","f15",
  "f16","f17","f18","f19","f20","f21","f22","f23",
  "f24","f25","f26","f27","f28","f29","f30","f31",
  "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7",
  "v8", "v9", "v10","v11","v12","v13","v14","v15",
  "v16","v17","v18","v19","v20","v21","v22","v23",
  "v24","v25","v26","v27","v28","v29","v30","v31",
  "vscr",
  "pc", "ps", "cr", "lr", "ctr", "xer", "mq",
  "fpscr",
  "vrsave"
};

const char *
ppc_register_name(int reg_nr)
{
  if (reg_nr < 0)
    return NULL;
  if ((size_t)reg_nr >= (sizeof(gdb_register_names)
			 / sizeof(*gdb_register_names)))
    return NULL;
  return gdb_register_names[reg_nr];
}

/* */
void ppc_store_struct_return (CORE_ADDR addr, CORE_ADDR sp)
{
  write_register (SRA_REGNUM, addr);
}

/* */
void ppc_store_return_value(struct type *type, struct regcache *unused,
			    const gdb_byte *valbuf)
{
#if 0
  /* Floating point values are returned starting from FPR1 and up.
     Say a double_double_double type could be returned in
     FPR1/FPR2/FPR3 triple. */

  if (TYPE_CODE (type) == TYPE_CODE_FLT)
    write_register_bytes(REGISTER_BYTE(FPRV_REGNUM), (valbuf), TYPE_LENGTH(type));
  else
    write_register_bytes(REGISTER_BYTE(RV_REGNUM), (valbuf), TYPE_LENGTH(type));
#else
  (void)type;
  (void)valbuf;
#endif /* 0 */
  (void)unused;
}

CORE_ADDR ppc_push_return_address (CORE_ADDR pc, CORE_ADDR sp)
{
  return sp;
}

/* Nonzero if register N requires conversion
   from raw format to virtual format.
   The register format for rs6000 floating point registers is always
   double, we need a conversion if the memory format is float.  */

int ppc_register_convertible (int regno)
{
  return ((regno >= FIRST_FP_REGNUM) && (regno <= LAST_FP_REGNUM));
}

/* Convert data from raw format for register REGNUM in buffer FROM
   to virtual format with type TYPE in buffer TO.  */

void
ppc_register_convert_to_virtual(int regno, struct type *type, char *from,
				char *to)
{
  if (TYPE_LENGTH(type) != REGISTER_RAW_SIZE(regno))
    {
      double val = (double)extract_floating(from, REGISTER_RAW_SIZE(regno));
      store_floating(to, TYPE_LENGTH(type), val);
    }
  else
    memcpy(to, from, REGISTER_RAW_SIZE(regno));
}

/* Convert data from virtual format with type TYPE in buffer FROM
   to raw format for register REGNUM in buffer TO.  */

void
ppc_register_convert_to_raw(struct type *type, int regno, char *from, char *to)
{
  if (TYPE_LENGTH(type) != REGISTER_RAW_SIZE(regno))
    {
      double val = (double)extract_floating(from, TYPE_LENGTH(type));
      store_floating(to, REGISTER_RAW_SIZE(regno), val);
    }
  else
    memcpy(to, from, REGISTER_RAW_SIZE(regno));
}

/* Sequence of bytes for breakpoint instruction.  */

#define BIG_BREAKPOINT { 0x7f, 0xe0, 0x00, 0x08 }
#define LITTLE_BREAKPOINT { 0x08, 0x00, 0xe0, 0x7f }

static const gdb_byte *
ppc_breakpoint_from_pc (CORE_ADDR *addr, int *size)
{
  static unsigned char big_breakpoint[] = BIG_BREAKPOINT;
  static unsigned char little_breakpoint[] = LITTLE_BREAKPOINT;
  *size = 4;
  if (TARGET_BYTE_ORDER == BFD_ENDIAN_BIG)
    return big_breakpoint;
  else
    return little_breakpoint;
}

static int
ppc_register_byte (int N)
{
return (((N) >= FIRST_SP_REGNUM) ? ((((N) - FIRST_SP_REGNUM) * 4) + SIZE_GP_REGS + SIZE_FP_REGS + SIZE_VP_REGS) \
	: (((N) >= FIRST_VP_REGNUM) ? ((((N) - FIRST_VP_REGNUM) * 16) + SIZE_GP_REGS + SIZE_FP_REGS) \
	   : (((N) >= FIRST_FP_REGNUM) ? ((((N) - FIRST_FP_REGNUM) * 8) + SIZE_GP_REGS) \
	      : ((N) * 4))));
}

static int
ppc_register_raw_size (int N)
{
  return (((N) >= FIRST_SP_REGNUM) ? 4
	  : (((N) >= FIRST_VP_REGNUM) ? 16
	     : (((N) >= FIRST_FP_REGNUM) ? 8
		: 4)));
}

static int
ppc_register_virtual_size (int N)
{
  return (((N) >= FIRST_SP_REGNUM) ? 4
	  : (((N) >= FIRST_VP_REGNUM) ? 16
	     : (((N) >= FIRST_FP_REGNUM) ? 8
		: 4)));
}

static struct type *
ppc_register_virtual_type (int N)
{
  return (((N) >= FIRST_SP_REGNUM) ? builtin_type_unsigned_int
	  : (((N) >= FIRST_VP_REGNUM) ? builtin_type_v4sf
	     : (((N) >= FIRST_FP_REGNUM) ? builtin_type_double
		: builtin_type_unsigned_int)));
}

static unsigned LONGEST ppc_call_dummy_words[]=
{
  0xfeedfeed, 0xfeedfeed, 0xfeedfeed, 0xfeedfeed,
  0xfeedfeed, 0xfeedfeed, 0xfeedfeed, 0xfeedfeed,
  0xfeedfeed, 0xfeedfeed, 0xfeedfeed, 0xfeedfeed,
  0xfeedfeed, 0xfeedfeed, 0xfeedfeed, 0xfeedfeed,
  0xfeedfeed, 0xfeedfeed, 0xfeedfeed, 0xfeedfeed,
  0xfeedfeed, 0xfeedfeed, 0xfeedfeed, 0xfeedfeed,
  0xfeedfeed, 0xfeedfeed, 0xfeedfeed, 0xfeedfeed,
  0xfeedfeed, 0xfeedfeed, 0xfeedfeed, 0xfeedfeed,

  0x7c0802a6, /* mflr   r0             */
  0xd8010000, /* stfd   r?, num(r1)    */
  0xbc010000, /* stm    r0, num(r1)    */
  0x94210000, /* stwu   r1, num(r1)    */
  0xfeedfeed,
  0xfeedfeed,
  /* save toc pointer */
  0x3c400000, /* addis  r2, 0, 0x0     */
  0x60420000, /* ori    r2, r2, 0x0    */
  /* save function pointer */
  0x3d800000, /* lis    r12, 0x0       */
  0x618c0000, /* ori    r12, r12, 0x0  */
  /* call function */
  0x7d8903a6, /* mtctr r12              */
  0x4e800421, /* bctrl                 */
  /* breakpoint for function return */
  0x7fe00008, /* trap                  */
  0x60000000, /* nop                   */
  0xfeedfeed,
  0xfeedfeed,

  0xfeedfeed, 0xfeedfeed, 0xfeedfeed, 0xfeedfeed,
  0xfeedfeed, 0xfeedfeed, 0xfeedfeed, 0xfeedfeed,
  0xfeedfeed, 0xfeedfeed, 0xfeedfeed, 0xfeedfeed,
  0xfeedfeed, 0xfeedfeed, 0xfeedfeed, 0xfeedfeed,
  0xfeedfeed, 0xfeedfeed, 0xfeedfeed, 0xfeedfeed,
  0xfeedfeed, 0xfeedfeed, 0xfeedfeed, 0xfeedfeed,
  0xfeedfeed, 0xfeedfeed, 0xfeedfeed, 0xfeedfeed,
  0xfeedfeed, 0xfeedfeed, 0xfeedfeed, 0xfeedfeed
};

/* keep this as multiple of 16 ($sp requires 16 byte alignment) */

#define INSTRUCTION_SIZE 4

static struct gdbarch *
ppc_gdbarch_init (struct gdbarch_info info, struct gdbarch_list *arches)
{
  struct gdbarch *gdbarch;
  struct gdbarch_tdep *tdep;

  arches = gdbarch_list_lookup_by_info (arches, &info);
  if (arches != NULL)
    return arches->gdbarch;

  tdep = XMALLOC (struct gdbarch_tdep);
  gdbarch = gdbarch_alloc (&info, tdep);

#if 0
  set_gdbarch_read_pc (gdbarch, generic_target_read_pc);
#endif /* 0 */
  set_gdbarch_write_pc (gdbarch, generic_target_write_pc);
#if 0
  set_gdbarch_read_fp (gdbarch, generic_target_read_fp);
  set_gdbarch_write_fp (gdbarch, generic_target_write_fp);
  set_gdbarch_read_sp (gdbarch, generic_target_read_sp);
  set_gdbarch_write_sp (gdbarch, generic_target_write_sp);
#endif /* 0 */

  set_gdbarch_num_regs (gdbarch, NUM_REGS);
  set_gdbarch_sp_regnum (gdbarch, SP_REGNUM);
#if defined(FP_REGNUM) && defined(SP_REGNUM) && (FP_REGNUM != SP_REGNUM)
  set_gdbarch_fp_regnum (gdbarch, FP_REGNUM);
#endif /* FP_REGNUM != SP_REGNUM */
  set_gdbarch_pc_regnum (gdbarch, PC_REGNUM);

  set_gdbarch_register_name (gdbarch, ppc_register_name);
#if 0
  set_gdbarch_register_size (gdbarch, 4);
  set_gdbarch_register_bytes (gdbarch, REGISTER_BYTES);
  set_gdbarch_register_byte (gdbarch, ppc_register_byte);
  set_gdbarch_register_raw_size (gdbarch, ppc_register_raw_size);
  set_gdbarch_max_register_raw_size (gdbarch, 16);
  set_gdbarch_register_virtual_size (gdbarch, ppc_register_virtual_size);
  set_gdbarch_max_register_virtual_size (gdbarch, 16);
  set_gdbarch_register_virtual_type (gdbarch, ppc_register_virtual_type);
#else
  (void)ppc_register_byte;
  (void)ppc_register_raw_size;
  (void)ppc_register_virtual_size;
  (void)ppc_register_virtual_type;
#endif /* 0 */

  set_gdbarch_ptr_bit (gdbarch, 4 * TARGET_CHAR_BIT);
  set_gdbarch_short_bit (gdbarch, 2 * TARGET_CHAR_BIT);
  set_gdbarch_int_bit (gdbarch, 4 * TARGET_CHAR_BIT);
  set_gdbarch_long_bit (gdbarch, 4 * TARGET_CHAR_BIT);
  set_gdbarch_long_long_bit (gdbarch, 8 * TARGET_CHAR_BIT);
  set_gdbarch_float_bit (gdbarch, 4 * TARGET_CHAR_BIT);
  set_gdbarch_double_bit (gdbarch, 8 * TARGET_CHAR_BIT);
  set_gdbarch_long_double_bit (gdbarch, 8 * TARGET_CHAR_BIT);
  set_gdbarch_addr_bit (gdbarch, 4 * TARGET_CHAR_BIT);

  switch (info.byte_order)
    {
    case BFD_ENDIAN_BIG:
      set_gdbarch_float_format (gdbarch, &floatformat_ieee_single_big);
      set_gdbarch_double_format (gdbarch, &floatformat_ieee_double_big);
      set_gdbarch_long_double_format (gdbarch, &floatformat_ieee_double_big);
      break;
    case BFD_ENDIAN_LITTLE:
      set_gdbarch_float_format (gdbarch, &floatformat_ieee_single_little);
      set_gdbarch_double_format (gdbarch, &floatformat_ieee_double_little);
      set_gdbarch_long_double_format (gdbarch, &floatformat_ieee_double_little);
      break;
    default:
      internal_error (__FILE__, __LINE__, "ppc_gdbarch_init: bad byte order for float format");
    }

#if 0
  set_gdbarch_call_dummy_words (gdbarch, ppc_call_dummy_words);
  set_gdbarch_sizeof_call_dummy_words (gdbarch, sizeof (ppc_call_dummy_words));
  set_gdbarch_use_generic_dummy_frames (gdbarch, 0);
  set_gdbarch_call_dummy_length (gdbarch, (32 + 16 + 32) * INSTRUCTION_SIZE);
#else
  (void)ppc_call_dummy_words;
#endif /* 0 */
  set_gdbarch_call_dummy_location (gdbarch, ON_STACK);
#if 0
  set_gdbarch_call_dummy_address (gdbarch, 0);
  set_gdbarch_call_dummy_start_offset (gdbarch, (32 + 6) * INSTRUCTION_SIZE);
  set_gdbarch_call_dummy_breakpoint_offset_p (gdbarch, 1);
  set_gdbarch_call_dummy_breakpoint_offset (gdbarch, (32 + 6 + 6) * INSTRUCTION_SIZE);
  set_gdbarch_pc_in_call_dummy (gdbarch, pc_in_call_dummy_on_stack);
  set_gdbarch_call_dummy_p (gdbarch, 1);
  set_gdbarch_call_dummy_stack_adjust_p (gdbarch, 0);
  set_gdbarch_get_saved_register (gdbarch, generic_get_saved_register);
  set_gdbarch_fix_call_dummy (gdbarch, ppc_fix_call_dummy);
  set_gdbarch_push_dummy_frame (gdbarch, ppc_push_dummy_frame);

  set_gdbarch_push_return_address (gdbarch, ppc_push_return_address);
#endif /* 0 */
  set_gdbarch_believe_pcc_promotion (gdbarch, 1);
#if 0
  set_gdbarch_coerce_float_to_double (gdbarch, standard_coerce_float_to_double);

  set_gdbarch_register_convertible (gdbarch, ppc_register_convertible);
  set_gdbarch_register_convert_to_virtual (gdbarch, ppc_register_convert_to_virtual);
  set_gdbarch_register_convert_to_raw (gdbarch, ppc_register_convert_to_raw);
#endif /* 0 */

  set_gdbarch_extract_return_value (gdbarch, ppc_extract_return_value);

#if 0
  set_gdbarch_push_arguments (gdbarch, ppc_push_arguments);

  set_gdbarch_store_struct_return (gdbarch, ppc_store_struct_return);
#endif /* 0 */
  set_gdbarch_store_return_value (gdbarch, ppc_store_return_value);
#if 0
  set_gdbarch_extract_struct_value_address (gdbarch, ppc_extract_struct_value_address);
  set_gdbarch_use_struct_convention (gdbarch, ppc_use_struct_convention);
  set_gdbarch_pop_frame (gdbarch, ppc_pop_frame);
#endif /* 0 */

  set_gdbarch_skip_prologue (gdbarch, ppc_skip_prologue);
  set_gdbarch_inner_than (gdbarch, core_addr_lessthan);
  set_gdbarch_decr_pc_after_break (gdbarch, 0);
#if 0
  set_gdbarch_function_start_offset (gdbarch, 0);
#endif /* 0 */
  set_gdbarch_breakpoint_from_pc (gdbarch, ppc_breakpoint_from_pc);

  set_gdbarch_frame_args_skip (gdbarch, 0);

#if 0
  set_gdbarch_frame_chain_valid (gdbarch, ppc_frame_chain_valid);
  set_gdbarch_frameless_function_invocation (gdbarch, ppc_frameless_function_invocation);
  set_gdbarch_frame_chain (gdbarch, ppc_frame_chain);
  set_gdbarch_frame_saved_pc (gdbarch, ppc_frame_saved_pc);
  set_gdbarch_frame_init_saved_regs (gdbarch, ppc_frame_cache_saved_regs);
  set_gdbarch_init_extra_frame_info (gdbarch, ppc_init_extra_frame_info);
  set_gdbarch_frame_args_address (gdbarch, ppc_frame_cache_initial_stack_address);
  set_gdbarch_frame_locals_address (gdbarch, ppc_frame_cache_initial_stack_address);
  set_gdbarch_saved_pc_after_call (gdbarch, ppc_frame_saved_pc_after_call);

  set_gdbarch_frame_num_args (gdbarch, frame_num_args_unknown);
#endif /* 0 */

  return gdbarch;
}

void
ppc_print_count_info(int frame, CORE_ADDR fp, CORE_ADDR pc, int get_names)
{
  char num_buf[8];
  const char *name;

  snprintf(num_buf, sizeof(num_buf), "%d", frame);
  ui_out_text (uiout, "Frame ");
  ui_out_text(uiout, num_buf);
  ui_out_text(uiout, ": ");
#if 0
  ui_out_list_begin (uiout, num_buf);
#endif /* 0 */
  ui_out_field_core_addr (uiout, "pc", pc);

  if (get_names)
    {
      ui_out_text (uiout, " func: ");

      find_pc_partial_function (pc, &name, NULL, NULL);
      if (name != NULL)
	ui_out_field_string (uiout, "func", name);
      else
	ui_out_field_skip (uiout, "func");
    }

  ui_out_text (uiout, " fp: ");
  ui_out_field_core_addr (uiout, "fp", fp);
  ui_out_text (uiout, "\n");
#if 0
  ui_out_list_end (uiout);
#endif /* 0 */
}

/*
 * implementation for ppc-fast-show-stack command
 * prints the stack depth, a valid bit, and list
 * of the frames showing the pc & fp for each frame
 */
void
ppc_fast_show_stack(const char *args, int from_tty)
{
  int get_names = 0;
  int show_frames = 1;
  int valid, count;

  if (args != NULL)
    {
      if (strstr (args, "noframe") != 0)
	show_frames = 0;
      else if (strstr (args, "frame") != 0)
	show_frames = 1;
      else
	show_frames = 1;

      if (show_frames)
	{
	  if (strstr (args, "noname") != 0)
	    get_names = 0;
	  else if (strstr (args, "name") != 0)
	    get_names = 1;
	  else
	    get_names = 0;

	}
    }

  valid = ppc_fast_show_stack_helper (show_frames, get_names, &count);

  ui_out_text (uiout, "Valid: ");
  ui_out_field_int (uiout, "valid", valid);
  ui_out_text (uiout, "\nCount: ");
  ui_out_field_int (uiout, "count", count);
  ui_out_text (uiout, "\n");
}

/*
 * This is the helper function for ppc_fast_show_stack, but it is
 * also set to the FAST_COUNT_STACK macro for ppc.  The return value
 * is 1 if no errors were encountered traversing the stack, and 0 otherwise.
 * it sets count to the stack depth.  If SHOW_FRAMES is 1, then it also
 * emits a list of frame info bits, with the pc & fp for each frame to
 * the current UI_OUT.  If GET_NAMES is 1, it also emits the names for
 * each frame (though this slows the function a good bit.)
 */

int
ppc_fast_show_stack_helper (int show_frames, int get_names, int *count)
{
  CORE_ADDR fp;
  static CORE_ADDR sigtramp_start = 0;
  static CORE_ADDR sigtramp_end = 0;
  struct frame_info *fi;
  int i = 0, valid = 0;
  unsigned long next_fp, pc;

  /* Get the first two frames. If anything funky is going on, it will
   * be here. The second frame helps us get above frameless functions
   * called from signal handlers...
   * Above these frames we have to deal with sigtramps
   * and alloca frames, that is about all. */

  if (sigtramp_start == 0)
    {
      const char *name;
      struct minimal_symbol *msymbol;

      msymbol = lookup_minimal_symbol ("_sigtramp", NULL, NULL);
      if (msymbol == NULL)
	warning ("Could not find minimal symbol for \"_sigtramp\" - backtraces may be unreliable");
      else
	{
	  pc = SYMBOL_VALUE_ADDRESS (msymbol);
	  if (find_pc_partial_function (pc, &name,
					&sigtramp_start, &sigtramp_end) == 0)
	    {
	      error ("Could not find _sigtramp symbol - backtraces will be unreliable");
	    }
	}
    }

  if (show_frames) {
#if 0
    ui_out_list_begin (uiout, "frames");
#else
    ; /* ??? */
#endif /* 0 */
  }

  i = 0;

  fi = get_current_frame();

  if (fi == NULL)
    {
      goto ppc_count_finish;
    }

  if (show_frames)
    ppc_print_count_info (i, fi->frame, fi->pc, get_names);
  i = 1;
  valid = 1;

  fi = get_prev_frame(fi);
  if (fi == NULL)
    {
      goto ppc_count_finish;
    }

  pc = fi->pc;
  fp = fi->frame;

  if (show_frames)
    ppc_print_count_info (i, fp, pc, get_names);
  i = 2;

  if (safe_read_memory_unsigned_integer (fp, 4, &next_fp))
    {
      while (1) {
        if ((sigtramp_start<= pc) && (pc <= sigtramp_end))
	  {
	    fp = next_fp + 0x70 + 0xc;
	    if (!safe_read_memory_unsigned_integer (fp,
						    4, &next_fp))
	      goto ppc_count_finish;

	    if (!safe_read_memory_unsigned_integer (fp - 0xc, 4, &pc))
	      goto ppc_count_finish;

	    fp = next_fp;
	    if (!safe_read_memory_unsigned_integer (fp, 4, &next_fp))
	      goto ppc_count_finish;
	  }
	else
	  {
	    fp = next_fp;

	    if (!safe_read_memory_unsigned_integer (fp, 4, &next_fp))
	      goto ppc_count_finish;

	    if (next_fp == 0)
	      {
		valid = 1;
		goto ppc_count_finish;
	      }

	    if (i > 10000)
	      goto ppc_count_finish;

	    if (!safe_read_memory_unsigned_integer (fp + DEFAULT_LR_SAVE, 4, &pc))
	      goto ppc_count_finish;
	  }

	if (show_frames)
	  ppc_print_count_info (i, fp, pc, get_names);
	i++;

      }
    }

 ppc_count_finish:
  if (show_frames) {
#if 0
    ui_out_list_end (uiout);
#else
    (void)uiout;
#endif /* 0 */
  }

  *count = i;
  return valid;
}

extern void _initialize_ppc_tdep(void); /* -Wmissing-prototypes */

/* Usual gdb initialization hook: */
void
_initialize_ppc_tdep(void)
{
  struct gdbarch *gdbarch = NULL;
  struct cmd_list_element *cmd = NULL;

  register_gdbarch_init(bfd_arch_powerpc, ppc_gdbarch_init);

  set_gdbarch_print_insn(gdbarch, print_insn_big_powerpc);

  cmd = add_set_cmd("debug-ppc", class_obscure, var_boolean,
		    (char *)&ppc_debugflag,
		    "Set if printing PPC stack analysis debugging statements.",
		    &setlist),
  add_show_from_set(cmd, &showlist);

  add_com("ppc-fast-show-stack", class_obscure, ppc_fast_show_stack,
	  "List stack pc & frame pointers without building the stack info.\n\
If you pass the \"-name\" argument, it will also return function names.");
}

/* EOF */
