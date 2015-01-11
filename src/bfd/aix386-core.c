/* aix386-core.c: BFD back-end for AIX on PS/2 core files.
 * This was based on trad-core.c, which was written by John Gilmore of
 *      Cygnus Support.
 * Copyright 1988, 1989, 1991, 1992, 1993, 1994, 1996, 1998, 1999, 2000,
 * 2001, 2002, 2004
 * Free Software Foundation, Inc.
 * Written by Minh Tran-Le <TRANLE@INTELLICORP.COM>.
 * Converted to back end form by Ian Lance Taylor <ian@cygnus.com>. */
/*
This file is part of BFD, the Binary File Descriptor library.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "bfd.h"
#include "sysdep.h"
#include "libbfd.h"
#include "coff/i386.h"
#include "coff/internal.h"
#include "libcoff.h"

#include <signal.h>

#if defined (_AIX) && defined (_I386)
# define NOCHECKS /* This is for coredump.h.  */
# define _h_USER /* Avoid including user.h from coredump.h.  */
# include <uinfo.h>
# include <sys/i386/coredump.h>
#else
# ifndef NOCHECKS
#  define NOCHECKS 1
# endif /* !NOCHECKS */
# ifndef _h_USER
#  define _h_USER 1
# endif /* !_h_USER */
# if defined(HAVE_UINFO_H)
#  include <uinfo.h>
# endif /* HAVE_UINFO_H */
# if defined(HAVE_SYS_I386_COREDUMP_H)
#  include <sys/i386/coredump.h>
# endif /* HAVE_SYS_I386_COREDUMP_H */
#endif /* _AIX && _I386 */

#ifdef HAVE_SYS_SOCKET_H
# include <sys/socket.h>
#endif /* HAVE_SYS_SOCKET_H */

/* AIX 4.2.x does NOT have nanosleep, but it does have nsleep,
 * which is equivalent: */
#if (!defined(HAVE_NANOSLEEP) && !defined(nanosleep)) && \
    (defined(HAVE_NSLEEP) || defined(nsleep))
# define nanosleep(a,b) nsleep(a,b)
#endif /* (!HAVE_NANOSLEEP && !nanosleep) && (HAVE_NSLEEP || nsleep) */

/* For struct timespec on AIX 4.2.x */
#ifdef HAVE_SYS_TIMERS_H
# include <sys/timers.h>
#endif /* HAVE_SYS_TIMERS_H */

/* for setpcred and friends: */
#ifdef HAVE_USERSEC_H
# include <usersec.h>
#endif /* HAVE_USERSEC_H */

/*
 * According to the setauthdb man page, AIX password registries must be 15
 * chars or less plus terminating NUL.
 */
#if defined(HAVE_SETAUTHDB) && !defined(REGISTRY_SIZE)
# define REGISTRY_SIZE 16
#endif /* HAVE_SETAUTHDB && !REGISTRY_SIZE */

#ifdef WITH_AIXAUTHENTICATE
# ifndef CUSTOM_SYS_AUTH_PASSWD
#  define CUSTOM_SYS_AUTH_PASSWD 1
# endif /* !CUSTOM_SYS_AUTH_PASSWD */
# ifndef CUSTOM_SYS_AUTH_ALLOWED_USER
#  define CUSTOM_SYS_AUTH_ALLOWED_USER 1
# endif /* !CUSTOM_SYS_AUTH_ALLOWED_USER */
# ifndef CUSTOM_SYS_AUTH_RECORD_LOGIN
#  define CUSTOM_SYS_AUTH_RECORD_LOGIN 1
# endif /* !CUSTOM_SYS_AUTH_RECORD_LOGIN */
# ifndef CUSTOM_SYS_AUTH_GET_LASTLOGIN_MSG
#  define CUSTOM_SYS_AUTH_GET_LASTLOGIN_MSG /* nothing */
# endif /* !CUSTOM_SYS_AUTH_GET_LASTLOGIN_MSG */
# ifndef CUSTOM_FAILED_LOGIN
#  define CUSTOM_FAILED_LOGIN 1
# endif /* !CUSTOM_FAILED_LOGIN */
# if defined(S_AUTHDOMAIN) && defined(S_AUTHNAME) && !defined(USE_AIX_KRB_NAME)
#  define USE_AIX_KRB_NAME /* nothing */
# endif /* S_AUTHDOMAIN && S_AUTHNAME && !USE_AIX_KRB_NAME */
#endif /* WITH_AIXAUTHENTICATE */

#if defined(AIX_GETNAMEINFO_HACK) && !defined(BROKEN_GETADDRINFO) && \
    !defined(getnameinfo) && defined(sshaix_getnameinfo)
# define getnameinfo(a,b,c,d,e,f,g) (sshaix_getnameinfo(a,b,c,d,e,f,g))
#endif /* AIX_GETNAMEINFO_HACK && !BROKEN_GETADDRINFO && !getnameinfo && sshaix_getnameinfo */

/*
 * We use getgrset in preference to multiple getgrent calls for efficiency
 * plus it supports NIS and LDAP groups.
 */
#if !defined(HAVE_GETGROUPLIST) && defined(HAVE_GETGRSET) && !defined(USE_GETGRSET)
# define HAVE_GETGROUPLIST /* nothing */
# define USE_GETGRSET /* nothing */
#endif /* !HAVE_GETGROUPLIST && HAVE_GETGRSET && !USE_GETGRSET */

/* Maybe this could work on some other i386 but I have not tried it
 * mtranle@paris - Tue Sep 24 12:49:35 1991
 */

#ifndef COR_MAGIC
# define COR_MAGIC "core"
#endif /* !COR_MAGIC */

/* Need this cast because ptr is really void *.  */
#define core_hdr(bfd) \
    (((bfd->tdata.trad_core_data))->hdr)
#define core_section(bfd,n) \
    (((bfd)->tdata.trad_core_data)->sections[n])
#define core_regsec(bfd) \
    (((bfd)->tdata.trad_core_data)->reg_section)
#define core_reg2sec(bfd) \
    (((bfd)->tdata.trad_core_data)->reg2_section)

#ifndef MAX_CORE_SEGS
# define MAX_CORE_SEGS 32 /* arbitrary (something in a Google search preview) */
#endif /* !MAX_CORE_SEGS */

/* in case "coff/i386.h" failed to define these for us; possibly because
 * of ifdefs: */
#ifndef COR_TYPE_DATA
# define COR_TYPE_DATA    'd' /* process data segment          */
#endif /* !COR_TYPE_DATA */
#ifndef COR_TYPE_STACK
# define COR_TYPE_STACK   's' /* process stack segment         */
#endif /* !COR_TYPE_STACK */
#ifndef COR_TYPE_LIBDATA
# define COR_TYPE_LIBDATA 'D' /* shared lib data               */
#endif /* !COR_TYPE_LIBDATA */
#ifndef COR_TYPE_WRITE
# define COR_TYPE_WRITE   'w' /* other writeable               */
#endif /* !COR_TYPE_WRITE */
#ifndef COR_TYPE_MSC
# define COR_TYPE_MSC     '?' /* other, mapped in segment      */
#endif /* !COR_TYPE_MSC */

/* These are stored in the bfd's tdata: */
struct trad_core_struct {
  struct corehdr *hdr; /* core file header */
  asection *reg_section;
  asection *reg2_section;
  asection *sections[MAX_CORE_SEGS];
};

static void swap_abort PARAMS((void));

static const bfd_target *
aix386_core_file_p(bfd *abfd)
{
  int i, n;
  unsigned char longbuf[4]; /* Raw bytes of various header fields */
  /* try to work around -Wstack-protector warnings for building Release: */
  char longerbuf[8L] ATTRIBUTE_UNUSED;
  long *buf_of_longs[8L] ATTRIBUTE_UNUSED;
#if defined(_AIX) || defined(HAVE_SYS_I386_COREDUMP_H)
  bfd_size_type core_size = sizeof(struct corehdr);
#else
  bfd_size_type core_size = sizeof(struct trad_core_struct);
#endif /* _AIX || HAVE_SYS_I386_COREDUMP_H */
  bfd_size_type amt;
  struct corehdr *core;
  struct mergem {
    struct trad_core_struct coredata;
    struct corehdr internal_core;
  } *mergem;

  amt = sizeof(longbuf);
  if (bfd_bread((PTR)longbuf, amt, abfd) != amt) {
      if (bfd_get_error() != bfd_error_system_call) {
	  bfd_set_error(bfd_error_wrong_format);
      }
      return 0;
  }

  if (strncmp((const char *)longbuf, COR_MAGIC, 4)) {
      return 0;
  }

  if (bfd_seek(abfd, (file_ptr)0, 0) != 0) {
      return 0;
  }

  amt = sizeof(struct mergem);
  mergem = (struct mergem *)bfd_zalloc(abfd, amt);
  if (mergem == NULL) {
      return 0;
  }

  core = &mergem->internal_core;

  if ((bfd_bread((PTR)core, core_size, abfd)) != core_size) {
      if (bfd_get_error() != bfd_error_system_call) {
	  bfd_set_error(bfd_error_wrong_format);
      }
    loser:
      bfd_release(abfd, (char *)mergem);
      abfd->tdata.any = NULL;
      bfd_section_list_clear(abfd);
      return 0;
  }

  set_tdata(abfd, &mergem->coredata);
  core_hdr(abfd) = core;

  /* Create the sections: */
  core_regsec(abfd) = bfd_make_section_anyway(abfd, ".reg");
  if (core_regsec(abfd) == NULL) {
      goto loser;
  }

  core_regsec(abfd)->flags = SEC_HAS_CONTENTS;
  core_regsec(abfd)->size = sizeof(core->cd_regs);
  core_regsec(abfd)->vma = (bfd_vma)-1;

  /* We shall access the regs afresh in the core file, like any section: */
  core_regsec (abfd)->filepos =
    (file_ptr)offsetof(struct corehdr, cd_regs[0]);

  core_reg2sec(abfd) = bfd_make_section_anyway(abfd, ".reg2");
  if (core_reg2sec(abfd) == NULL) {
      /* bfd_release frees everything allocated after its arg: */
      goto loser;
  }

  core_reg2sec(abfd)->flags = SEC_HAS_CONTENTS;
  core_reg2sec(abfd)->size = sizeof(core->cd_fpregs);
  core_reg2sec(abfd)->vma = (bfd_vma)-1;
  core_reg2sec(abfd)->filepos =
    (file_ptr)offsetof(struct corehdr, cd_fpregs);

  for ((i = 0), (n = 0); (i < MAX_CORE_SEGS) && (core->cd_segs[i].cs_type); i++) {
      const char *sname;
      flagword flags;

      if (core->cd_segs[i].cs_offset == 0) {
	  continue;
      }

      switch (core->cd_segs[i].cs_type) {
	case COR_TYPE_DATA:
	  sname = ".data";
	  flags = (SEC_ALLOC + SEC_LOAD + SEC_HAS_CONTENTS);
	  break;
	case COR_TYPE_STACK:
	  sname = ".stack";
	  flags = (SEC_ALLOC + SEC_LOAD + SEC_HAS_CONTENTS);
	  break;
	case COR_TYPE_LIBDATA:
	  sname = ".libdata";
	  flags = (SEC_ALLOC + SEC_HAS_CONTENTS);
	  break;
	case COR_TYPE_WRITE:
	  sname = ".writeable";
	  flags = (SEC_ALLOC + SEC_HAS_CONTENTS);
	  break;
	case COR_TYPE_MSC:
	  sname = ".misc";
	  flags = (SEC_ALLOC + SEC_HAS_CONTENTS);
	  break;
	default:
	  sname = ".unknown";
	  flags = (SEC_ALLOC + SEC_HAS_CONTENTS);
	  break;
      }
      core_section(abfd, n) = bfd_make_section_anyway(abfd, sname);
      if (core_section(abfd, n) == NULL) {
	  goto loser;
      }

      core_section(abfd, n)->flags = flags;
      core_section(abfd, n)->size = core->cd_segs[i].cs_len;
      core_section(abfd, n)->vma = core->cd_segs[i].cs_address;
      core_section(abfd, n)->filepos = core->cd_segs[i].cs_offset;
      core_section(abfd, n)->alignment_power = 2;
      n++;
  }

  return abfd->xvec;
}

static char *
aix386_core_file_failing_command(bfd *abfd)
{
  return core_hdr(abfd)->cd_comm;
}

static int
aix386_core_file_failing_signal(bfd *abfd)
{
  return core_hdr(abfd)->cd_cursig;
}

static bfd_boolean
aix386_core_file_matches_executable_p(bfd *core_bfd ATTRIBUTE_UNUSED,
                                      bfd *exec_bfd ATTRIBUTE_UNUSED)
{
  /* FIXME: We have no way of telling at this point. */
  return TRUE;
}

/* If somebody calls any byte-swapping routines, shoot them: */
static void ATTRIBUTE_NORETURN
swap_abort(void)
{
  /* This way does NOT require any declaration for ANSI to mess up (?). */
  abort();
}

#define	NO_GET ((bfd_vma (*) (const void *)) swap_abort)
#define	NO_PUT ((void (*) (bfd_vma, void *)) swap_abort)
#define	NO_GETS ((bfd_signed_vma (*) (const void *)) swap_abort)
#define	NO_GET64 ((bfd_uint64_t (*) (const void *)) swap_abort)
#define	NO_PUT64 ((void (*) (bfd_uint64_t, void *)) swap_abort)
#define	NO_GETS64 ((bfd_int64_t (*) (const void *)) swap_abort)

const bfd_target aix386_core_vec = {
  (char *)"aix386-core",
  bfd_target_unknown_flavour,
  BFD_ENDIAN_BIG,		/* target byte order */
  BFD_ENDIAN_BIG,		/* target headers byte order */
  (HAS_RELOC | EXEC_P |		/* object flags */
   HAS_LINENO | HAS_DEBUG |
   HAS_SYMS | HAS_LOCALS | WP_TEXT),

  (SEC_HAS_CONTENTS | SEC_ALLOC | SEC_LOAD | SEC_RELOC), /* section flags */
  0,				/* leading underscore */
  ' ',				/* ar_pad_char */
  16,				/* ar_max_namelen */
  NO_GET64, NO_GETS64, NO_PUT64,
  NO_GET, NO_GETS, NO_PUT,
  NO_GET, NO_GETS, NO_PUT,	/* data */
  NO_GET64, NO_GETS64, NO_PUT64,
  NO_GET, NO_GETS, NO_PUT,
  NO_GET, NO_GETS, NO_PUT,	/* hdrs */

  {_bfd_dummy_target, _bfd_dummy_target,
   _bfd_dummy_target, aix386_core_file_p},
  {bfd_false, bfd_false,	/* bfd_create_object */
   bfd_false, bfd_false},
  {bfd_false, bfd_false,	/* bfd_write_contents */
   bfd_false, bfd_false},

  BFD_JUMP_TABLE_GENERIC(_bfd_generic),
  BFD_JUMP_TABLE_COPY(_bfd_generic),
  BFD_JUMP_TABLE_CORE(aix386),
  BFD_JUMP_TABLE_ARCHIVE(_bfd_noarchive),
  BFD_JUMP_TABLE_SYMBOLS(_bfd_nosymbols),
  BFD_JUMP_TABLE_RELOCS(_bfd_norelocs),
  BFD_JUMP_TABLE_WRITE(_bfd_generic),
  BFD_JUMP_TABLE_LINK(_bfd_nolink),
  BFD_JUMP_TABLE_DYNAMIC(_bfd_nodynamic),

  NULL,

  (PTR)0
};

/* silence '-Wunused-macros' warnings: */
#ifdef _h_USER
# undef _h_USER
#endif /* _h_USER */
#ifdef NOCHECKS
# undef NOCHECKS
#endif /* NOCHECKS */

/* EOF */
