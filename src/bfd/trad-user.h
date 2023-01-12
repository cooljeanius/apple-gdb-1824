/* trad-user.h -*- C -*-
 * bfd
 *
 * Created by Eric Gallager on 12/31/14.
 * Based on the <sys/user.h> from Android and Google Breakpad
 */

#ifndef __TRAD_USER_H__
#define __TRAD_USER_H__ 1

#if defined(HAVE_STDINT_H) || \
    (defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L))
# include <stdint.h>
#endif /* HAVE_STDINT_H || c99+ */

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/* to be used by our backup user struct: */
struct trad_user_i386_regs_struct
{
  long ebx, ecx, edx, esi, edi, ebp, eax;
  long xds, xes, xfs, xgs, orig_eax;
  long eip, xcs, eflags, esp, xss;
};

/* Note: on ARM, GLibc uses user_regs instead of user_regs_struct. */
struct trad_user_arm_regs
{
  /* Note: Entries 0-15 match r0..r15
   *       Entry 16 is used to store the CPSR register.
   *       Entry 17 is used to store the "orig_r0" value.
   */
  unsigned long int uregs[18];
};

struct trad_user_mips_regs_struct
{
  unsigned long long regs[32];
  unsigned long long lo;
  unsigned long long hi;
  unsigned long long epc;
  unsigned long long badvaddr;
  unsigned long long status;
  unsigned long long cause;
};

/* FIXME: This list of registers is just based on the information I could
 * find (of the PowerPC 970FX, in "Mac OS X Internals: A Systems Approach"
 * by Amit Singh, pages 188-197); I am not sure how many of them should
 * actually be used in this struct, though... */
struct trad_user_ppc64_regs_struct
{
  uint64_t GPR[32]; /* General-Purpose Registers, 64 bits wide */
  uint64_t FPR[32]; /* Floating-Point Registers, 64 bits wide */
  uint64_t VMX[32]; /* Vector Registers, 128 bits wide */
  /* (FIXME: what to do for a 128-bit type?) */
  uint32_t XER; /* Integer Exception Register */
  uint32_t FPSCR; /* Floating-Point Status and Control Register */
  uint32_t VSCR; /* Vector Status and Control Register */
  uint32_t _CR; /* Condition Register */
  uint32_t VRSAVE; /* Vector Save/Restore Register */
  uint64_t _LR; /* Link Register */
  uint64_t CTR; /* Count Register */
  uint32_t _TBL; /* Timebase Register (first) */
  uint32_t TBU; /* Timebase Register (second) */
};

/* Pages 223-224 of the aformentioned source say a lot of the registers
 * are actually still the same in 32-bit mode: */
struct trad_user_ppc32_regs_struct
{
  /* I assume the GPRs are 32-bit in this mode, as the book mentions
   * explicitly which registers remain 64-bit in 32-bit mode, and the GPRs
   * are not among those listed: */
  uint32_t GPR[32];
  uint64_t FPR[32]; /* Floating-Point Registers are still 64 bits wide */
  uint64_t VMX[32]; /* Vector Registers are still 128 bits wide */
  /* (FIXME: what to do for a 128-bit type?) */
  uint32_t XER; /* Integer Exception Register, set in 32-bit-compatible
                 * way in this mode */
  uint32_t FPSCR; /* Floating-Point Status and Control Register */
  uint32_t VSCR; /* Vector Status and Control Register */
  uint32_t _CR; /* Condition Register */
  uint32_t VRSAVE; /* Vector Save/Restore Register */
  uint32_t _LR; /* Link Register (assuming 32-bit in this mode) */
  uint32_t CTR; /* Count Register (assuming 32-bit in this mode) */
  uint32_t _TBL; /* Timebase Register (first) */
  uint32_t TBU; /* Timebase Register (second) */
};

#if defined(__i386) || defined(__i386__) || defined(__x86_64) || defined(__x86_64__)
typedef struct trad_user_i386_regs_struct *trad_user_regs_ptr_t;
#else
# if defined(__arm__)
typedef struct trad_user_arm_regs *trad_user_regs_ptr_t;
# else
#  if defined(__mips__)
typedef struct trad_user_mips_regs_struct *trad_user_regs_ptr_t;
#  else
#   if defined(__ppc) || defined(__ppc__)
typedef struct trad_user_ppc32_regs_struct *trad_user_regs_ptr_t;
#   else
#    if defined(__ppc64) || defined(__ppc64__)
typedef struct trad_user_ppc64_regs_struct *trad_user_regs_ptr_t;
#    else
#     if defined(__GNUC__) && !defined(__STRICT_ANSI__) && defined(__extension__)
 #      __extension__ warning "trad-user.h lacks support for the CPU ABI for which you are compiling."
#     endif /* __GNUC__ && !__STRICT_ANSI__ && __extension__ */
typedef void *trad_user_regs_ptr_t;
#    endif /* __ppc64 || __ppc64__ */
#   endif /* __ppc || __ppc__ */
#  endif /* __mips__ */
# endif /* __arm__ */
#endif /* __i386 || __i386__ || __x86_64 || __x86_64__ */

/* a backup in case the system one is insufficient: */
struct trad_user
{
  int u_unused1; /* should be a struct that we are currently missing */
  int u_fpvalid;
  int u_unused2; /* should be a struct that we are currently missing */
  unsigned long u_tsize;
  unsigned long u_dsize;
  unsigned long u_ssize;
  unsigned long start_code;
  unsigned long start_stack;
  long signal;
  int reserved_field;
  trad_user_regs_ptr_t u_ar0;
  int u_unused4; /* should be a struct that we are currently missing */
  unsigned long magic;
  char u_comm[32];
  int u_debugreg[8];
  /* FIXME: hppabsd-core.c needs members 'u_pcb' and 'u_code'; research */
  /* also, sco5-core.c needs members 'u_exdata', 'u_sub', and 'u_sysabort' */
};

/* these defines should be in "config.h": */
#if defined(HAVE_STRUCT_USER_U_DSIZE) && defined(HAVE_STRUCT_USER_U_SSIZE)
typedef struct user user_struct_t;
#else
typedef struct trad_user user_struct_t;
#endif /* HAVE_STRUCT_USER_U_DSIZE && HAVE_STRUCT_USER_U_SSIZE */

#ifdef __cplusplus
}  /* extern "C" */
#endif  /* __cplusplus */

#endif /* !__TRAD_USER_H__ */

/* EOF */
