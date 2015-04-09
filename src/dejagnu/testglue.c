/*
 * testglue.c
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning testglue.c would like to include "config.h"
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef NO_UNISTD_H
# include <sys/unistd.h>
#else
# ifdef HAVE_UNISTD_H
#  include <unistd.h>
# else
#  if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#   warning "testglue.c expects a unistd-related header to be included."
#  endif /* __GNUC__ && !__STRICT_ANSI__ */
# endif /* HAVE_UNISTD_H */
#endif /* !NO_UNISTD_H */
#ifndef _UNISTD_H_
# ifdef HAVE_UNISTD_H
#  include <unistd.h>
# else
#  if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#   warning "testglue.c expects <unistd.h> to be included."
#  endif /* __GNUC__ && !__STRICT_ANSI__ */
# endif /* HAVE_UNISTD_H */
#endif /* !_UNISTD_H_ */

/* A simple glue file for embedded targets so we can get the real exit
 * status from the program. This assumes we are using GNU ld and can use
 * the -wrap option, and that write(1, ...) does something useful.
 */

/* There is a bunch of weird cruft with #ifdef UNDERSCORES. This is needed
 * because currently GNU ld does NOT deal well with a.out targets and
 * the -wrap option. When GNU ld is fixed, this should definitely be
 * removed. Note that we actually wrap __exit, not _exit on a target
 * that has UNDERSCORES defined. On non-UNDERSCORE targets, we
 * wrap _exit separately; it is actually a different function.
 */

#ifdef WRAP_M68K_AOUT
# define REAL_EXIT(code) asm ( "trap %0" : : "i" (0) );
# define REAL_ABORT() REAL_EXIT(6)
# define ORIG_EXIT _exit
# define ORIG_ABORT abort
#else
# ifdef UNDERSCORES
#  define REAL_EXIT _real___exit
#  define REAL_MAIN _real__main
#  define REAL_ABORT _real__abort
#  define ORIG_EXIT _wrap___exit
#  define ORIG_ABORT _wrap__abort
#  define ORIG_MAIN _wrap__main
# else
#  define REAL_EXIT __real_exit
#  ifndef VXWORKS
#   define REAL__EXIT __real__exit
#  endif /* !VXWORKS */
#  define REAL_MAIN __real_main
#  define REAL_ABORT __real_abort
#  define ORIG_EXIT __wrap_exit
#  define ORIG__EXIT __wrap__exit
#  define ORIG_ABORT __wrap_abort
#  define ORIG_MAIN __wrap_main
# endif /* UNDERSCORES */
#endif /* WRAP_M68K_AOUT */

#ifdef REAL_MAIN
extern void REAL_EXIT ();
extern void REAL_ABORT ();
extern int REAL_MAIN (int argc, char **argv, char **envp);
#endif /* REAL_MAIN */
#ifdef REAL__EXIT
extern void REAL__EXIT ();
#endif /* REAL__EXIT */

static int done_exit_message = 0;

int ___constval = 1;

#ifdef VXWORKS
static void __runexit();
#endif /* VXWORKS */

static char *
write_int(val, ptr)
     int val;
     char *ptr;
{
  char c;
  if (val<0) {
    *(ptr++) = '-';
    val = -val;
  }
  if (val>9) {
    ptr = write_int (val/10, ptr);
  }
  c = (val%10)+'0';
  *(ptr++) = c;
  return ptr;
}

void
ORIG_EXIT (code)
     int code;
{
  char buf[30];
  char *ptr;

#ifdef VXWORKS
  __runexit ();
#endif /* VXWORKS */
  strcpy (buf, "\n*** EXIT code ");
  ptr = write_int (code, buf + strlen(buf));
  *(ptr++) = '\n';
  write (1, buf, ptr-buf);
  done_exit_message = 1;
  REAL_EXIT (code);
  while (___constval);
}

#ifdef ORIG__EXIT
void
ORIG__EXIT (code)
     int code;
{
  char buf[30];
  char *ptr;

  /* Since exit may call _exit, we need to avoid a second message.  */
  if (! done_exit_message)
    {
      strcpy (buf, "\n*** EXIT code ");
      ptr = write_int (code, buf + strlen(buf));
      *(ptr++) = '\n';
      write (1, buf, ptr-buf);
    }

  REAL__EXIT (code);
  while (___constval);
}
#endif /* ORIG__EXIT */

void
ORIG_ABORT ()
{
  write (1, "\n*** EXIT code 4242\n", 20);
  REAL_ABORT ();
  while (___constval);
  abort ();
}

#ifdef REAL_MAIN
int
ORIG_MAIN (argc, argv, envp)
     int argc;
     char **argv;
     char **envp;
{
# ifdef WRAP_FILE_ARGS
  extern int __argc;
  extern char *__args[];

  exit (REAL_MAIN (__argc,__args,envp));
# else
  exit (REAL_MAIN (argc, argv, envp));
# endif /* WRAP_FILE_ARGS */
  while (___constval);
}
#endif /* REAL_MAIN */

#ifdef VXWORKS
void
_exit (status)
     int status;
{
  REAL_EXIT (status);
}

typedef (*PFV)(void);

static PFV __list[32];
static int __listcnt = 0;
static int __running = 0;

int
atexit (PFV func)
{
  __list[__listcnt++] = func;
}

static void
__runexit ()
{
  int i;
  if (__running++)
    return;

  for (i = 0; i < __listcnt; i++)
    __list[i]();
  __running = 0;
}
#endif /* VXWORKS */

/* EOF */
