/* Program to stuff files into a specially prepared space in kdb.
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
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  */

/* Written 13-Mar-86 by David Bridgham. */

#include "config.h"
#include "defs.h"

#include <stdio.h>
#if defined(HAVE_A_OUT_H) || __has_include(<a.out.h>)
# include <a.out.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "stuff.c expects <a.out.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
struct nlist;
struct exec;
#endif /* HAVE_A_OUT_H */
#include <sys/types.h>
#include "gdb_stat.h"
#include <sys/file.h>
#if defined(HAVE_STDARG_H) || defined(__STDC__) || __has_include(<stdarg.h>)
# include <stdarg.h>
#else
# if defined(HAVE_VARARGS_H) || __has_include(<varags.h>)
#  include <varargs.h>
# endif /* HAVE_VARARGS_H */
#endif /* HAVE_STDARG_H */
#if defined(HAVE_UNISTD_H) || __has_include(<unistd.h>)
# include <unistd.h>
#endif /* HAVE_UNISTD_H */
#include <stdlib.h>
#if defined(HAVE_ERRNO_H) || __has_include(<errno.h>)
# include <errno.h>
#else
# if defined(HAVE_SYS_ERRNO_H) || __has_include(<sys/errno.h>)
#  include <sys/errno.h>
# endif /* HAVE_SYS_ERRNO_H */
#endif /* HAVE_ERRNO_H */
#include <string.h>

#ifndef STREQ
# ifdef DEPRECATED_STREQ
#  define STREQ(foo, bar) DEPRECATED_STREQ(foo, bar)
# endif
#endif /* !STREQ */

extern void err(const char *, ...);
extern int get_offset(char *, const char *);
extern int find_symbol(char *, struct nlist *, int, char *);

int
main(int argc, char *argv[])
{
  register char *cp = NULL;
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
      if (STREQ(argv[i], "-o"))
	outfile = argv[++i];
    }
  if (outfile == 0)
    err("Output file not specified\n");

  offset = get_offset(outfile, "_heap");

  out_fd = open(outfile, O_WRONLY);
  if (out_fd < 0)
    err("Error opening %s for write: %s\n", outfile, xstrerror(errno));
  if (lseek (out_fd, offset, 0) < 0)
    err("Error seeking to heap in %s: %s\n", outfile, xstrerror(errno));

  /* For each file listed on the command line, write it into the
   * 'heap' of the output file.  Make sure to skip the arguments
   * that name the output file. */
  for (i = 1; i < argc; i++)
    {
      if (STREQ(argv[i], "-o"))
	continue;
      if ((in_fd = open(argv[i], O_RDONLY)) < 0)
	err("Error opening %s for read: %s\n", argv[i],
            xstrerror(errno));
      if (fstat(in_fd, &stat_buf) < 0)
	err("Error stat'ing %s: %s\n", argv[i], xstrerror(errno));
      size = strlen(argv[i]);
      pad = (4 - (size & 3));
      size += (pad + stat_buf.st_size + sizeof(int));
      write(out_fd, &size, sizeof(int));
      write(out_fd, argv[i], strlen(argv[i]));
      write(out_fd, zeros, pad);
      while ((size = read (in_fd, buf, sizeof(buf))) > 0)
	write(out_fd, buf, size);
      close(in_fd);
    }
  size = 0;
  write(out_fd, &size, sizeof(int));
  close(out_fd);
  (void)cp;
  return (0);
}

/* Read symbol table from file and returns the offset into the file
 * where symbol sym_name is located.  If error, print message and
 * exit. */
int
get_offset(char *file, const char *sym_name)
{
  int f;
  struct exec file_hdr;
  struct nlist *symbol_table;
  int size;
  char *strings;

  f = open(file, O_RDONLY);
  if (f < 0)
    err("Error opening %s: %s\n", file, xstrerror(errno));
  if (read(f, &file_hdr, sizeof(file_hdr)) < 0)
    err("Error reading exec structure: %s\n", xstrerror(errno));
  if (N_BADMAG(file_hdr))
    err("File %s not an a.out file\n", file);

  /* read in symbol table */
  if ((symbol_table = (struct nlist *)xmalloc(file_hdr.a_syms)) == 0)
    err("Failed to allocate space for symbol table\n");
  if (lseek(f, N_SYMOFF(file_hdr), 0) == -1)
    err("lseek error: %s\n", xstrerror(errno));
  if (read(f, symbol_table, file_hdr.a_syms) == -1)
    err("Error reading symbol table from %s: %s\n", file,
        xstrerror(errno));

  /* read in string table */
  if (read(f, &size, 4) == -1)
    err("reading string table size: %s\n", xstrerror(errno));
  if ((strings = (char *)xmalloc(size)) == 0)
    err("Failed to allocate memory for string table\n");
  if (read(f, strings, (size - 4)) == -1)
    err("reading string table: %s\n", xstrerror(errno));

  /* Find the core address at which the first byte of kdb text segment
     should be loaded into core when kdb is run.  */
  origin = (find_symbol("_etext", symbol_table, file_hdr.a_syms, strings)
            - file_hdr.a_text);
  /* Find the core address at which the heap will appear: */
  coreaddr = find_symbol (sym_name, symbol_table, file_hdr.a_syms, strings);
  /* Return address in file of the heap data space: */
  return (N_TXTOFF(file_hdr) + core_addr - origin);
}

/* */
int
find_symbol(char *sym_name, struct nlist *symbol_table, int length,
            char *strings)
{
  register struct nlist *sym;

  /* Find symbol in question */
  for (sym = symbol_table;
       sym != (struct nlist *)((char *)symbol_table + length);
       sym++)
      {
	if ((sym->n_type & N_TYPE) != N_DATA) continue;
	if (sym->n_un.n_strx == 0) continue;
	if (STREQ(sym_name, (strings + sym->n_un.n_strx - 4)))
	  return sym->n_value;
      }
    err("Data symbol %s not found in %s\n", sym_name, file);
}

/* VARARGS */
void
err(va_alist)
     va_dcl
{
  va_list args;
  char *string;

  va_start(args);
  string = va_arg(args, char *);
  vfprintf(gdb_stderr, string, args);
  va_end(args);
  exit(-1);
}
