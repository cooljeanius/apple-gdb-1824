/* stuff.c
   Program to stuff files into a specially prepared space in kdb.
   Copyright (C) 1986, 1989, 1991 Free Software Foundation, Inc.

This file is part of GDB.

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
Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */

/* Written 13-Mar-86 by David Bridgham. */

#include <stdio.h>
#if defined(__unix__) || defined(__linux__) || defined(HAVE_A_OUT_H)
# include <a.out.h>
#else
# if !defined(__A_OUT_GNU_H__) && !defined(NINDY_SHARE_B_OUT_H) && \
     !defined(__GNU_EXEC_MACROS__) && !defined(__STRUCT_EXEC_OVERRIDE__)
struct exec {
  int a_syms;
  int a_text;
};
struct nlist {
  struct {
    int n_strx;
  } n_un;
  int n_value;
};
# else
struct exec;
struct nlist;
# endif /* !__A_OUT_GNU_H__ && !NINDY_SHARE_B_OUT_H && !__GNU_EXEC_MACROS__ && !__STRUCT_EXEC_OVERRIDE__ */
#endif /* __unix__ || __linux__ || HAVE_A_OUT_H */
#if defined(__STDC__) || defined(STDC_HEADERS) || defined(HAVE_STDLIB_H)
# include <stdlib.h>
#endif /* __STDC__ || STDC_HEADERS || HAVE_STDLIB_H */
#if defined(__STDC__) || defined(STDC_HEADERS) || defined(HAVE_STRING_H)
# include <string.h>
#endif /* __STDC__ || STDC_HEADERS || HAVE_STRING_H */
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#if defined(__STDC__) || defined(STDC_HEADERS) || defined(HAVE_STDARG_H)
# include <stdarg.h>
#else
# include <varargs.h>
#endif /* __STDC__ || STDC_HEADERS || HAVE_STDARG_H */
#if defined(__APPLE__) || defined(__unix__) || defined(HAVE_UNISTD_H)
# include <unistd.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "stuff_old.c may wish to include <unistd.h>"
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* __APPLE__ || __unix__ || HAVE_UNISTD_H */

#if defined(__APPLE__) || defined(HAVE_ERRNO_H)
# include <errno.h>
#else
extern char *sys_errlist[];
#endif /* __APPLE__ || HAVE_ERRNO_H */

#if defined(__STDC__) || defined(PROTOTYPES) || defined(__PROTOTYPES)
extern int get_offset(char *, const char *);
extern int find_symbol(const char *, struct nlist *, int, char *);
# if !defined(_VARARGS_H) && (defined(_STDARG_H) || defined(_ANSI_STDARG_H_))
#  if defined(__GNUC__) && (__GNUC__ >= 3)
extern void err(const char *str, ...) __attribute__((__format__(__printf__, 1, 2)))
  __attribute__((__noreturn__));
#  else
extern void err(const char *str, ...);
#  endif /* __GNUC__ >= 3 && 0 */
# else
#  if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#   warning "Cannot declare prototype here for err() with your current ifdefs."
#  endif /* __GNUC__ && !__STRICT_ANSI__ */
# endif /* !_VARARGS_H && (_STDARG_H || _ANSI_STDARG_H_) */
#endif /* __STDC__ || PROTOTYPES || __PROTOTYPES */

#if defined(__STDC__) || defined(PROTOTYPES) || defined(__PROTOTYPES)
int main(int argc, char *argv[])
#else
main (argc, argv)
     int argc;
     char *argv[];
#endif /* __STDC__ || PROTOTYPES || __PROTOTYPES */
{
  char *outfile;
  register int i;
  int offset;
  int out_fd, in_fd;
  struct stat stat_buf;
  int size, pad;
  char buf[1024];
  static char zeros[4] = {0};

  if (argc < 4)
    err("Not enough arguments\nUsage: %s -o kdb file1 file2 ...\n",
	argv[0]);

  outfile = 0;
  for (i = 1; i < argc; i++)
    {
      if (strcmp(argv[i], "-o") == 0)
	outfile = argv[++i];
    }
  if (outfile == 0)
    err("Output file not specified\n");

  offset = get_offset(outfile, "_heap");

  out_fd = open(outfile, O_WRONLY);
  if (out_fd < 0)
    err("Error opening %s for write: %s\n", outfile, sys_errlist[errno]);
  if (lseek(out_fd, offset, 0) < 0)
    err("Error seeking to heap in %s: %s\n", outfile, sys_errlist[errno]);

  /* For each file listed on the command line, write it into the
   * 'heap' of the output file.  Make sure to skip the arguments
   * that name the output file. */
  for (i = 1; i < argc; i++)
    {
      if (strcmp(argv[i], "-o") == 0)
	continue;
      if ((in_fd = open(argv[i], O_RDONLY)) < 0)
	err("Error opening %s for read: %s\n", argv[i], sys_errlist[errno]);
      size = strlen(argv[i]);
      if (size > 1024)
        err("Error: argument too big: %d (%s)\n", size, sys_errlist[errno]);
      if (fstat(in_fd, &stat_buf) < 0)
	err("Error stat'ing %s: %s\n", argv[i], sys_errlist[errno]);

      pad = (4 - (size & 3));
      size += (pad + stat_buf.st_size + sizeof(int));
      write(out_fd, &size, sizeof(int));
      write(out_fd, argv[i], strlen(argv[i]));
      write(out_fd, zeros, pad);
      while ((size = read(in_fd, buf, sizeof(buf))) > 0)
	write(out_fd, buf, size);
      close(in_fd);
    }
  size = 0;
  write(out_fd, &size, sizeof(int));
  close(out_fd);
  return (0);
}

/* Read symbol table from file and returns the offset into the file
 * where symbol sym_name is located.  If error, print message and
 * exit. */
#if defined(__STDC__) || defined(PROTOTYPES) || defined(__PROTOTYPES)
int get_offset(char *file, const char *sym_name)
#else
get_offset (file, sym_name)
     char *file;
     char *sym_name;
#endif /* __STDC__ || PROTOTYPES || __PROTOTYPES */
{
  int f;
  struct exec file_hdr;
  struct nlist *symbol_table;
  int size;
  char *strings;
#if defined(__APPLE__) && (!defined(HAVE_DECL_ORIGIN) || !HAVE_DECL_ORIGIN)
  int origin;
#endif /* __APPLE__ && !HAVE_DECL_ORIGIN */
#if defined(__APPLE__) && (!defined(HAVE_DECL_COREADDR) || !HAVE_DECL_COREADDR)
  int coreaddr;
#endif /* __APPLE__ && !HAVE_DECL_COREADDR */

  f = open(file, O_RDONLY);
  if (f < 0)
    err("Error opening %s: %s\n", file, sys_errlist[errno]);
  if (read(f, &file_hdr, sizeof(file_hdr)) < 0)
    err("Error reading exec structure: %s\n", sys_errlist[errno]);
#if defined(N_BADMAG)
  if (N_BADMAG(file_hdr))
    err("File %s not an a.out file\n", file);
#endif /* N_BADMAG */

  /* read in symbol table: */
  if ((symbol_table = (struct nlist *)malloc(file_hdr.a_syms)) == 0)
    err("Could NOT allocate space for symbol table\n");
#if defined(N_SYMOFF)
  if (lseek(f, N_SYMOFF(file_hdr), 0) == -1)
    err("lseek error: %s\n", sys_errlist[errno]);
#else
  if (lseek(f, (off_t)0L, 0) == -1)
    err("lseek error: %s\n", sys_errlist[errno]);
#endif /* N_SYMOFF */
  if (read(f, symbol_table, file_hdr.a_syms) == -1)
    err("Error reading symbol table from %s: %s\n", file, sys_errlist[errno]);

  /* read in string table: */
  if (read(f, &size, 4) == -1)
    err("reading string table size: %s\n", sys_errlist[errno]);
  if ((strings = (char *)malloc(size)) == 0)
    err("Could NOT allocate memory for string table\n");
  if (read(f, strings, size - 4) == -1)
    err("reading string table: %s\n", sys_errlist[errno]);

  /* Find the core address at which the first byte of kdb text segment
     should be loaded into core when kdb is run.  */
  origin = (find_symbol("_etext", symbol_table, file_hdr.a_syms, strings)
            - file_hdr.a_text);
  /* Find the core address at which the heap will appear: */
  coreaddr = find_symbol(sym_name, symbol_table, file_hdr.a_syms, strings);
#ifdef N_TXTOFF
  /* Return address in file of the heap data space: */
  return (N_TXTOFF(file_hdr) + coreaddr - origin);
#else
  (void)file_hdr;
  (void)coreaddr;
  (void)origin;
  return -1;
#endif /* N_TXTOFF */
}

#if defined(__STDC__) || defined(PROTOTYPES) || defined(__PROTOTYPES)
int find_symbol(const char *sym_name, struct nlist *symbol_table, int length,
		char *strings)
#else
find_symbol (sym_name, symbol_table, length, strings)
     char *sym_name;
     struct nlist *symbol_table;
     int length;
     char *strings;
#endif /* __STDC__ || PROTOTYPES || __PROTOTYPES */
{
  register struct nlist *sym;
#if defined(__APPLE__) && (!defined(HAVE_DECL_FILE) || !HAVE_DECL_FILE)
  const char *file = ".";
#endif /* __APPLE__ && !HAVE_DECL_FILE */

  /* Find symbol in question */
  for (sym = symbol_table;
       sym != (struct nlist *)((char *)symbol_table + length);
       sym++)
      {
#if defined(N_TYPE) && defined(N_DATA)
	if ((sym->n_type & N_TYPE) != N_DATA) continue;
#endif /* N_TYPE && N_DATA */
	if (sym->n_un.n_strx == 0) continue;
	if (strcmp(sym_name, (strings + sym->n_un.n_strx - 4)) == 0)
	  return sym->n_value;
      }
    err("Data symbol %s not found in %s\n", sym_name, file);
    return -1;
}

#if defined(__STDC__) || defined(PROTOTYPES) || defined(__PROTOTYPES)
void err(const char *format, ...)
{
  va_list args;
  char *string;

  va_start(args, format);
  string = va_arg(args, char *);
  vfprintf(stderr, string, args);
  va_end(args);
  exit(-1);
}
#else
/* VARARGS */
void
err (va_alist)
     va_dcl
{
  va_list args;
  char *string;

  va_start(args);
  string = va_arg(args, char *);
  vfprintf(stderr, string, args);
  va_end(args);
  exit(-1);
}
#endif /* __STDC__ || PROTOTYPES || __PROTOTYPES */

/* EOF */
