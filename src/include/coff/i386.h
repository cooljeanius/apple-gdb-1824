/* coff/i386.h: coff information for Intel 386/486.
 *
 * Copyright 2001 Free Software Foundation, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St., 5th Floor, Boston, MA 02110-1301, USA
 */

#ifndef COFF_I386_H
#define COFF_I386_H 1

#define L_LNNO_SIZE 2
#define INCLUDE_COMDAT_FIELDS_IN_AUXENT
#include "coff/external.h"

/* Bits for f_flags:
 *	F_RELFLG	relocation info stripped from file
 *	F_EXEC		file is executable (no unresolved external references)
 *	F_LNNO		line numbers stripped from file
 *	F_LSYMS		local symbols stripped from file
 *	F_AR32WR	file has byte ordering of an AR32WR machine (e.g. vax).
 */

#define F_RELFLG	(0x0001)
#define F_EXEC		(0x0002)
#define F_LNNO		(0x0004)
#define F_LSYMS		(0x0008)

#define	I386MAGIC	0x14c
#define I386PTXMAGIC	0x154
#define I386AIXMAGIC	0x175

/* This is Lynx's all-platform magic number for executables: */
#define LYNXCOFFMAGIC	0415

#ifndef I386BADMAG
# define I386BADMAG(x) (((x).f_magic != I386MAGIC) \
		        && (x).f_magic != I386AIXMAGIC \
		        && (x).f_magic != I386PTXMAGIC \
		        && (x).f_magic != LYNXCOFFMAGIC)
#endif /* !I386BADMAG */

#define OMAGIC 0404    /* object files, eg as output */
#define ZMAGIC 0413    /* demand load format, e.g. normal ld output */
#define STMAGIC 0401	/* target shlib */
#define SHMAGIC 0443	/* host   shlib */

/* define some NT default values */
#ifndef NT_IMAGE_BASE
/* moved to internal.h, but the ifdefs will protect us: */
# define NT_IMAGE_BASE 0x400000
#endif /* !NT_IMAGE_BASE */
#define NT_SECTION_ALIGNMENT 0x1000
#define NT_FILE_ALIGNMENT    0x200
#define NT_DEF_RESERVE       0x100000
#define NT_DEF_COMMIT        0x1000

/********************** RELOCATION DIRECTIVES **********************/

struct external_reloc
{
  char r_vaddr[4];
  char r_symndx[4];
  char r_type[2];
};

#define RELOC struct external_reloc
#define RELSZ 10

#if defined(_AIX)
# if !defined(I386MAGIC)
#  define I386MAGIC 0565 /* Danbury AIX C compiler */
# endif /* !I386MAGIC */
# ifndef I386SVMAGIC
#  define I386SVMAGIC 0514 /* System V C Compiler */
# endif /* !I386SVMAGIC */
# ifndef I386BADMAG
#  define I386BADMAG(x)  (((x).f_magic!=I386MAGIC) && \
			    + ((x).f_magic!=I386SVMAGIC))
# endif /* !I386BADMAG */
#else /* not _AIX: */
# ifndef I386MAGIC
#  define I386MAGIC 0x14c
# endif /* !I386MAGIC */
# ifndef I386BADMAG
#  define I386BADMAG(x) (((x).f_magic!=I386MAGIC))
# endif /* !I386BADMAG */
#endif /* not _AIX */

#ifndef FILHDR
# define FILHDR struct external_filehdr
#endif /* !FILHDR */
#ifndef FILHSZ
# define FILHSZ sizeof(FILHDR)
#endif /* !FILHSZ */

/*
 * names of "special" sections:
 */
#define _TEXT    ".text"    /* executable code section     */
#define _DATA    ".data"    /* initialized data            */
#define _BSS     ".bss"     /* un-initialized data         */
#define _DEBUG   ".debug"   /* special section used by dbx */
#define _COMMENT ".comment" /* version info                */
#define _LIB     ".lib"     /* shared lib info section     */
#define _TV      ".tv"

 /* For new sections we have NOT heard of before: */
#define DEFAULT_SECTION_ALIGNMENT 4

/* --------------------------------------------------------------------- */

#if (defined(_AIX) && defined(_I386)) || defined(__i386) || \
    defined(__i386__) || defined(__x86_64) || defined(__x86_64__)
/* -- from uinfo.h -- */
# define UINFOSIZ 64 /* size of user info buffer */
typedef char uinfo_t[UINFOSIZ];

typedef struct env387
{
    unsigned short control;
    unsigned short r0;
    unsigned short status;
    unsigned short r1;
    unsigned short tag;
    unsigned short r2;
    unsigned long eip;
    unsigned short code_seg;
    unsigned short opcode;
    unsigned long operand;
    unsigned short operand_seg;
    unsigned short r3;
    unsigned char regs[8][10];
} env387_t;

# define CD_NAMELEN 16  /* length of most names in this header */
# define CORHDRSIZ 2048 /* size to which header is padded out */
# define MAX_CORE_SEGS 32 /* maximum segments in a core dump */
# define NUM_FREGS 1 /* # of saved FP regs */

/*
 * These are defined such that 286 and 386 kernels can produce
 * compatible dumps.
 */
# define CD_AX    0
# define CD_BX    1
# define CD_CX    2
# define CD_DX    3
# define CD_SI    4
# define CD_DI    5
# define CD_BP    6
# define CD_SP    7
# define CD_FL    8
# define CD_IP    9
# define CD_CS    10
# define CD_DS    11
# define CD_ES    12
# define CD_FS    13
# define CD_GS    14
# define CD_SS    15
# define NUM_REGS 16

# ifndef SPATHLEN
#  define SPATHLEN 16 /* sys/param.h */
# endif /* !SPATHLEN */

# if defined(HAVE_SYS_SIGNAL_H)
#  include <sys/signal.h>
# else
#  if defined(HAVE_SIGNAL_H)
#   include <signal.h>
#  else
#   ifndef NSIG
#    ifdef __DARWIN_NSIG
#     define NSIG __DARWIN_NSIG
#    else
#     define NSIG 32
#    endif /* __DARWIN_NSIG */
#   endif /* !NSIG */
#  endif /* HAVE_SIGNAL_H */
# endif /* HAVE_SYS_SIGNAL_H */

/* do this separately from the checks for including the signal header(s); they
 * may or may not define this: */
# if (!defined(HAVE_KSIGMASK_T) || !defined(ksigmask_t)) && !defined(_H_SIGNAL)
#  ifndef _POSIX_SOURCE
#   if !defined(SIGSETSZ)
#    if defined(_SIGSETSZ)
#     define SIGSETSZ _SIGSETSZ
#    else
#     if defined(NSIG)
#      define SIGSETSZ ((NSIG+31)/32)
#     else
#      define SIGSETSZ 1 /* result of doing the math from the previous case */
#     endif /* NSIG */
#    endif /* _SIGSETSZ */
#   endif /* !SIGSETSZ */
typedef	struct ksigmask {	/* used in u struct */
  unsigned long sigs[SIGSETSZ];
} ksigmask_t;
/* we just typedef-ed it, so we should now "HAVE" it: */
#   if !defined(HAVE_KSIGMASK_T)
#    define HAVE_KSIGMASK_T 1
#   endif /* !HAVE_KSIGMASK_T */
#  else
#   if defined(__GNUC__) && !defined(__STRICT_ANSI__)
/* FIXME: why exactly is this the case? */
#    warning "ksigmask_t cannot be used if _POSIX_SOURCE is defined."
#   endif /* __GNUC__ && !__STRICT_ANSI__ */
#  endif /* !_POSIX_SOURCE */
# else
#  if defined(_H_SIGNAL) && !defined(HAVE_KSIGMASK_T) && !defined(ksigmask_t)
#   if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#    warning "your signal header failed to define ksigmask_t properly."
#   endif /* __GNUC__ && !__STRICT_ANSI__ */
#  endif /* _H_SIGNAL && !HAVE_KSIGMASK_T && !ksigmask_t */
# endif /* (!HAVE_KSIGMASK_T || !ksigmask_t) && !_H_SIGNAL */

struct corehdr
{
  char cd_magic[4]; /* COR_MAGIC = "core" */

  /* general information about the dump itself: */
  struct dumpseg {   /* table of contents for dump   */
    long cs_type;    /* seg. type; see below         */
    long cs_len;     /* length (in bytes) of segment */
    long cs_offset;  /* offset (in dump) of segment  */
    long cs_address; /* address segment had in mem   */
  } cd_segs[MAX_CORE_SEGS];

  /* general information about the process: */
  char cd_comm[CD_NAMELEN]; /* command being run           */
  char cd_mach[CD_NAMELEN]; /* type of machine it ran on   */
  char cd_site[CD_NAMELEN]; /* name of site it ran on      */
  long cd_ldtype;           /* type of load module running */
  char cd_intsize;          /* sizeof(int)                 */
  char cd_dptrsize;         /* sizeof(char *)              */
  char cd_tptrsize;         /* sizeof(int(*)())           */
  char cd_unused;

  /* user-mode program state: */
  long        cd_regs[NUM_REGS]; /* user-mode general registers    */
  struct env387 cd_fpregs;       /* user-mode floating-point state */

  /* kernel-mode program state: */
#if defined(__PROTOTYPES) || defined(PROTOTYPES) || \
    (defined(__STDC__) && __STDC__)
  int         (*cd_sig[NSIG])(void); /* disposition of signals */
#else
  int         (*cd_sig[NSIG])();    /* disposition of signals      */
#endif /* __PROTOTYPES || PROTOTYPES || __STDC__ */

  ksigmask_t  cd_sigmask;           /* signals to be blocked       */
  ksigmask_t  cd_sigpend;           /* signals currently pending   */
  long        cd_cursig;            /* signal that caused the dump */

  pid_t       cd_pid;               /* process ID of the corpse    */
  pid_t       cd_ppid;              /* parent process ID of corpse */
  short       cd_uid;               /* process effective user ID   */
  short       cd_ruid;              /* process real user ID        */
  short       cd_gid;               /* process effective group ID  */
  short       cd_rgid;              /* process real group ID       */

  uinfo_t     cd_uinfo;             /* buffer of user information  */
  char        cd_locname[32];       /* name of /local              */
  char        cd_uvers[CD_NAMELEN]; /* user version string         */
#if defined(HAVE_SITEPATH_T) || defined(sitepath_t)
  sitepath_t  cd_spath[SPATHLEN];   /* sitepath */
#else
  /* not sure if this works... anyways, try it: */
  char        cd_spath[SPATHLEN];   /* sitepath */
#endif /* HAVE_SITEPATH_T || sitepath_t */
};

# ifndef NOCHECKS
#  define NOCHECKS 1
# endif /* !NOCHECKS */
# ifndef NOCHECKS
/* this will generate an error if sizeof(struct corehdr) > CORHDRSIZ */
struct { char xxcdxx[(CORHDRSIZ + 1 - sizeof(struct corehdr))]; };
# endif /* ! NOCHECKS */

/*
 * segment types (in cs_type)
 * each segment in the address space appears here, whether or not it
 * is actually dumped. Read/only segments will not actually be dumped.
 * A segment that is not in the dump will have a cs_offset of zero.
 */
# define COR_TYPE_CODE    'x' /* process code - NOT IN DUMP    */
# define COR_TYPE_DATA    'd' /* process data segment          */
# define COR_TYPE_STACK   's' /* process stack segment         */
# define COR_TYPE_LIBCODE 'X' /* shared lib code - NOT IN DUMP */
# define COR_TYPE_LIBDATA 'D' /* shared lib data               */
# define COR_TYPE_READ    'r' /* other read/only - NOT IN DUMP */
# define COR_TYPE_WRITE   'w' /* other writeable               */
# define COR_TYPE_MSC     '?' /* other, mapped in segment      */

#endif /* _AIX && _I386 */

#endif /* !COFF_I386_H */

/* EOF */
