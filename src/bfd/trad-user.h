/* trad-user.h -*- C -*-
 * bfd
 *
 * Created by Eric Gallager on 12/31/14.
 * Based on the <sys/user.h> from Android and Google Breakpad
 */

#ifndef __TRAD_USER_H__
#define __TRAD_USER_H__ 1

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

#if defined(__i386) || defined(__i386__) || defined(__x86_64) || defined(__x86_64__)
typedef struct trad_user_i386_regs_struct *trad_user_regs_ptr_t;
#else
# if defined(__arm__)
typedef struct trad_user_arm_regs *trad_user_regs_ptr_t;
# else
#  if defined(__mips__)
typedef struct trad_user_mips_regs_struct *trad_user_regs_ptr_t;
#  else
#   if defined(__GNUC__) && !defined(__STRICT_ANSI__)
 #    warning "trad-user.h lacks support for the CPU ABI for which you are compiling."
#   endif /* __GNUC__ && !__STRICT_ANSI__ */
typedef void *trad_user_regs_ptr_t;
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
