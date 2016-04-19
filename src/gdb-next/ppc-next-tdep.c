/*
 * ppc-next-tdep.c
 */

#include "ppc-reg.h"

#include "defs.h"
#include "frame.h"
#include "inferior.h"
#include "symtab.h"
#include "target.h"
#include "gdbcore.h"
#include "symfile.h"
#include "objfiles.h"

#include "ppc-next-tdep.h"

#ifdef MACOSX_DYLD
# include "macosx-tdep.h"
#else
# ifdef __NeXT__
#  include "nextstep-tdep.h"
# endif /* __NeXT__ */
#endif /* MACOSX_DYLD */

#ifndef REGISTER_BYTE
# ifdef DEPRECATED_REGISTER_BYTE
#  define REGISTER_BYTE(i) DEPRECATED_REGISTER_BYTE(i)
# endif /* DEPRECATED_REGISTER_BYTE */
#endif /* !REGISTER_BYTE */

#ifndef store_floating
# define store_floating(a, l, v) deprecated_store_floating(a, l, v)
#endif /* !store_floating */

#ifndef extract_floating
# define extract_floating(a, l) deprecated_extract_floating(a, l)
#endif /* !extract_floating */

/* */
void
ppc_next_fetch_sp_registers(unsigned char *rdata,
			    gdb_ppc_thread_state_t *gp_regs)
{
  store_unsigned_integer((rdata + (REGISTER_BYTE(PC_REGNUM))),
			 sizeof(REGISTER_TYPE), gp_regs->srr0);
  store_unsigned_integer((rdata + (REGISTER_BYTE(PS_REGNUM))),
			 sizeof(REGISTER_TYPE), gp_regs->srr1);
  store_unsigned_integer((rdata + (REGISTER_BYTE(CR_REGNUM))),
			 sizeof(REGISTER_TYPE), gp_regs->cr);
  store_unsigned_integer((rdata + (REGISTER_BYTE(LR_REGNUM))),
			 sizeof(REGISTER_TYPE), gp_regs->lr);
  store_unsigned_integer((rdata + (REGISTER_BYTE(CTR_REGNUM))),
			 sizeof(REGISTER_TYPE), gp_regs->ctr);
  store_unsigned_integer((rdata + (REGISTER_BYTE(XER_REGNUM))),
			 sizeof(REGISTER_TYPE), gp_regs->xer);
  store_unsigned_integer((rdata + (REGISTER_BYTE(MQ_REGNUM))),
			 sizeof(REGISTER_TYPE), gp_regs->mq);
}

/* */
void
ppc_next_store_sp_registers(unsigned char *rdata,
			    gdb_ppc_thread_state_t *gp_regs)
{
  gp_regs->srr0 = extract_unsigned_integer((rdata + (REGISTER_BYTE(PC_REGNUM))),
					   sizeof(REGISTER_TYPE));
  gp_regs->srr1 = extract_unsigned_integer((rdata + (REGISTER_BYTE(PS_REGNUM))),
					   sizeof(REGISTER_TYPE));
  gp_regs->cr = extract_unsigned_integer((rdata + (REGISTER_BYTE(CR_REGNUM))),
					 sizeof(REGISTER_TYPE));
  gp_regs->lr = extract_unsigned_integer((rdata + (REGISTER_BYTE(LR_REGNUM))),
					 sizeof(REGISTER_TYPE));
  gp_regs->ctr = extract_unsigned_integer((rdata + (REGISTER_BYTE(CTR_REGNUM))),
					  sizeof(REGISTER_TYPE));
  gp_regs->xer = extract_unsigned_integer((rdata + (REGISTER_BYTE(XER_REGNUM))),
					  sizeof(REGISTER_TYPE));
  gp_regs->mq = extract_unsigned_integer((rdata + (REGISTER_BYTE(MQ_REGNUM))),
					 sizeof(REGISTER_TYPE));
  gp_regs->pad = 0;
}

/* */
void
ppc_next_fetch_gp_registers(unsigned char *rdata,
			    gdb_ppc_thread_state_t *gp_regs)
{
  int i;
  for (i = 0; i < NUM_GP_REGS; i++) {
    store_unsigned_integer((rdata + (REGISTER_BYTE(FIRST_GP_REGNUM + i))),
			   sizeof(REGISTER_TYPE), gp_regs->gpregs[i]);
  }
}

/* */
void
ppc_next_store_gp_registers(unsigned char *rdata,
			    gdb_ppc_thread_state_t *gp_regs)
{
  int i;
  for (i = 0; i < NUM_GP_REGS; i++) {
    gp_regs->gpregs[i] =
      extract_unsigned_integer((rdata + (REGISTER_BYTE(FIRST_GP_REGNUM + i))),
			       sizeof(REGISTER_TYPE));
  }
}

/* */
void
ppc_next_fetch_fp_registers(unsigned char *rdata,
			    gdb_ppc_thread_fpstate_t *fp_regs)
{
  int i;
  FP_REGISTER_TYPE *fpr = fp_regs->fpregs;
  for (i = 0; i < NUM_FP_REGS; i++) {
    store_floating((rdata + (REGISTER_BYTE(FIRST_FP_REGNUM + i))),
		   sizeof(FP_REGISTER_TYPE), fpr[i]);
  }
  store_unsigned_integer(rdata + (REGISTER_BYTE(FPSCR_REGNUM)),
			 sizeof(REGISTER_TYPE), fp_regs->fpscr);
}

/* */
void
ppc_next_store_fp_registers(unsigned char *rdata,
			    gdb_ppc_thread_fpstate_t *fp_regs)
{
  int i;
  FP_REGISTER_TYPE *fpr = fp_regs->fpregs;
  for (i = 0; i < NUM_FP_REGS; i++) {
    fpr[i] =
      ((FP_REGISTER_TYPE)
       extract_floating((rdata + (REGISTER_BYTE(FIRST_FP_REGNUM + i))),
			sizeof(FP_REGISTER_TYPE)));
  }
  fp_regs->fpscr_pad = 0;
  fp_regs->fpscr =
    extract_unsigned_integer((rdata + (REGISTER_BYTE(FPSCR_REGNUM))),
			     sizeof(REGISTER_TYPE));
}

/* */
void
ppc_next_fetch_vp_registers(unsigned char *rdata,
			    gdb_ppc_thread_vpstate_t *vp_regs)
{
  int i, j;
  for (i = 0; i < NUM_VP_REGS; i++) {
    for (j = 0; j < 4; j++) {
      store_floating((rdata + (REGISTER_BYTE(FIRST_VP_REGNUM + i)) + (j * 4)),
		     4, *((float *)&vp_regs->save_vr[i][j]));
    }
  }
  store_unsigned_integer((rdata + (REGISTER_BYTE(VRSAVE_REGNUM))),
			 sizeof(REGISTER_TYPE), vp_regs->save_vrvalid);
}

/* */
void
ppc_next_store_vp_registers(unsigned char *rdata,
			    gdb_ppc_thread_vpstate_t *vp_regs)
{
  int i, j;
  for (i = 0; i < NUM_VP_REGS; i++) {
    for (j = 0; j < 4; j++) {
      vp_regs->save_vr[i][j] =
	((unsigned long)
	 extract_floating((rdata + (REGISTER_BYTE(FIRST_VP_REGNUM + i))
			   + (j * 4)), 4));
    }
  }
  memset(&vp_regs->save_pad5, 0, sizeof(vp_regs->save_pad5));
  vp_regs->save_vrvalid =
    extract_unsigned_integer((rdata + (REGISTER_BYTE(VRSAVE_REGNUM))),
			     sizeof(REGISTER_TYPE));
  memset(&vp_regs->save_pad5, 0, sizeof(vp_regs->save_pad6));
}

/* mread -- read memory (unsigned) and apply a bitmask: */
static unsigned long
mread(CORE_ADDR addr, unsigned long len, unsigned long mask)
{
  long ret = read_memory_unsigned_integer(addr, len);
  if (mask) { ret &= mask; }
  return ret;
}

/* */
static short
ext16(unsigned long n)
{
  if (n > 32767UL) {
    return (short)(n - 65536UL);
  } else {
    return (short)n;
  }
}

/* */
static long
ext32(unsigned long n)
{
  return (long)n;
}

/* */
CORE_ADDR
ppc_next_skip_trampoline_code(CORE_ADDR pc)
{
  return dyld_symbol_stub_function_address(pc, NULL);
}

/* */
CORE_ADDR
ppc_next_dynamic_trampoline_nextpc(CORE_ADDR pc)
{
  return dyld_symbol_stub_function_address(pc, NULL);
}

/* */
int ATTRIBUTE_CONST
ppc_next_in_solib_dynsym_resolve_code(CORE_ADDR pc ATTRIBUTE_UNUSED)
{
  return 0;
}

/* */
int ATTRIBUTE_CONST
ppc_next_in_solib_return_trampoline(CORE_ADDR pc ATTRIBUTE_UNUSED,
				    char *name ATTRIBUTE_UNUSED)
{
  return 0;
}

/* */
int
ppc_next_in_solib_call_trampoline(CORE_ADDR pc, char *name)
{
  if (ppc_next_skip_trampoline_code(pc) != 0) { return 1; }
  return 0;
}

/* EOF */
