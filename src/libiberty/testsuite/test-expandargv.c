/* test-expandargv.c: expandargv test program,
   Copyright (C) 2006 Free Software Foundation, Inc.
   Written by Carlos O'Donell <carlos@codesourcery.com>

   This file is part of the libiberty library, which is part of GCC.

   This file is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   In addition to the permissions in the GNU General Public License, the
   Free Software Foundation gives you unlimited permission to link the
   compiled version of this file into combinations with other programs,
   and to distribute those combinations without any restriction coming
   from the use of this file.  (The General Public License restrictions
   do apply in other respects; for example, they cover modification of
   the file, and distribution when not linked into a combined
   executable.)

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St., 5th Floor, Boston, MA 02110-1301, USA
*/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */
#include "libiberty.h"
#include <stdio.h>
#include <errno.h>
#ifdef HAVE_STDLIB_H
# include <stdlib.h>
#endif /* HAVE_STDLIB_H */
#ifdef HAVE_STRING_H
# include <string.h>
#endif /* HAVE_STRING_H */

#ifdef HAVE_UNISTD_H
# include <unistd.h> /* for unlink() */
#endif /* HAVE_UNISTD_H */

#ifndef EXIT_SUCCESS
# define EXIT_SUCCESS 0
#endif /* !EXIT_SUCCESS */

#ifndef EXIT_FAILURE
# define EXIT_FAILURE 1
#endif /* !EXIT_FAILURE */

static void fatal_error(int, const char *, int) ATTRIBUTE_NORETURN;
void writeout_test(int, const char *);
void run_replaces(char *);
void hook_char_replace(char *, size_t, char, char);
int run_tests(const char **);
void erase_test(int);

/* Test input data, argv before, and argv after:

   The \n is an important part of test_data since expandargv
   may have to work in environments where \n is translated
   as \r\n. Thus \n is included in the test data for the file.

   We use \b to indicate that the test data is the null character.
   This is because we use \0 normally to represent the end of the
   file data, so we need something else for this. */

#define FILENAME_PATTERN "test-expandargv-%d.lst"
#define ARGV0 "test-expandargv"

const char *test_data[] = {
  /* Test 0 - Check for expansion with \r\n */
  "a\r\nb",	/* Test 0 data */
  ARGV0,
  "@test-expandargv-0.lst",
  0, /* End of argv[] before expansion */
  ARGV0,
  "a",
  "b",
  0, /* End of argv[] after expansion */

  /* Test 1 - Check for expansion with \n */
  "a\nb",	/* Test 1 data */
  ARGV0,
  "@test-expandargv-1.lst",
  0,
  ARGV0,
  "a",
  "b",
  0,

  /* Test 2 - Check for expansion with \0 */
  "a\bb",	/* Test 2 data */
  ARGV0,
  "@test-expandargv-2.lst",
  0,
  ARGV0,
  "a",
  0,

  /* Test 3 - Check for expansion with only \0 */
  "\b",		/* Test 3 data */
  ARGV0,
  "@test-expandargv-3.lst",
  0,
  ARGV0,
  0,

  0 /* Test done marker, don't remove. */
};

/* Print a fatal error and exit.  LINE is the line number where we
   detected the error, ERRMSG is the error message to print, and ERR
   is 0 or an errno value to print.  */

static void
fatal_error(int line, const char *errmsg, int err)
{
  fprintf(stderr, "test-expandargv:%d: %s", line, errmsg);
  if (errno != 0)
    fprintf(stderr, ": %s", xstrerror(err));
  fprintf(stderr, "\n");
  exit(EXIT_FAILURE);
}

/* hook_char_replace:
     Replace 'replacethis' with 'withthis' */

void
hook_char_replace(char *string, size_t len, char replacethis, char withthis)
{
  size_t i = 0UL;
  for (i = 0; i < len; i++)
    if (string[i] == replacethis)
      string[i] = withthis;
}

/* run_replaces:
     Hook here all the character for character replaces.
     Be warned that expanding the string or contracting the string
     should be handled with care. */

void
run_replaces(char *string)
{
  /* Store original string size */
  size_t len = strlen(string);
  hook_char_replace(string, len, (char)'\b', (char)'\0');
}

/* write_test:
   Write test datafile */

void
writeout_test(int test, const char *test_data_buf)
{
  char filename[256];
  FILE *fd;
  size_t len;
  char * parse;

  /* Unique filename per test */
  snprintf(filename, sizeof(filename), FILENAME_PATTERN, test);
  fd = fopen(filename, "w");
  if (fd == NULL)
    fatal_error(__LINE__, "Failed to create test file.", errno);

  /* Generate RW copy of data for replaces */
  len = strlen(test_data_buf);
  parse = (char *)malloc(sizeof(char) * (len + 1));
  if (parse == NULL)
    fatal_error(__LINE__, "Failed to malloc parse.", errno);

  memcpy(parse, test_data_buf, sizeof(char) * len);
  /* Run all possible replaces */
  run_replaces(parse);

  fwrite(parse, len, sizeof(char), fd);
  free(parse);
  fclose(fd);
}

/* erase_test:
     Erase the test file */

void
erase_test(int test)
{
  char filename[256];
  snprintf(filename, sizeof(filename), FILENAME_PATTERN, test);
  if (unlink(filename) != 0)
    fatal_error(__LINE__, "Failed to erase test file.", errno);
}


/* run_tests:
    Run expandargv
    Compare argv before and after.
    Return number of fails */

int
run_tests(const char **test_data_buf)
{
  int argc_after, argc_before;
  char **argv_before, **argv_after;
  int i, j, k, fails, failed;

  i = j = fails = 0;
  /* Loop over all the tests: */
  while (test_data_buf[j])
    {
      /* Write test data: */
      writeout_test(i, test_data_buf[j++]);
      /* Copy argv before: */
      argv_before = dupargv((char **)&test_data_buf[j]);

      /* Count argc before/after: */
      argc_before = 0;
      argc_after = 0;
      while (test_data_buf[j + argc_before])
        argc_before++;
      /* FIXME: we cannot take the advice of -Wpointer-compare here, because
       * then we would be dereferencing a null pointer: */
      if (test_data_buf[j + argc_before] == '\0')
        j += (argc_before + 1); /* Skip null terminator */
      else
        j += argc_before;
      while (test_data_buf[j + argc_after])
        argc_after++;

      /* Copy argv after: */
      argv_after = dupargv((char **)&test_data_buf[j]);

      /* Run all possible replaces: */
      for (k = 0; k < argc_before; k++)
        run_replaces(argv_before[k]);
      for (k = 0; k < argc_after; k++)
        run_replaces(argv_after[k]);

      /* Run test: Expand arguments: */
      expandargv(&argc_before, &argv_before);

      failed = 0;
      /* Compare size first: */
      if (argc_before != argc_after)
        {
          printf("FAIL: test-expandargv-%d. Number of arguments don't match (%d vs. %d).\n",
                 i, argc_before, argc_after);
	  failed++;
        }
      /* Compare each of the argv's ... */
      else
        for (k = 0; k < argc_after; k++)
          if (strncmp(argv_before[k], argv_after[k], strlen(argv_after[k])) != 0)
            {
              printf("FAIL: test-expandargv-%d. Arguments don't match.\n", i);
              failed++;
            }

      if (!failed)
        printf("PASS: test-expandargv-%d.\n", i);
      else
        fails++;

      freeargv(argv_before);
      freeargv(argv_after);
      /* Advance to next test: */
      j += (argc_after + 1);
      /* Erase test file */
      erase_test(i);
      i++;
    }
  return fails;
}

/* main:
    Run tests.
    Check result and exit with appropriate code. */

int
main(int argc, char **argv)
{
  int fails;

#ifdef DEBUG
  printf("argc for main() is '%i', and its argv[0] is '%s'.\n",
         argc, argv[0]);
#else
# if (defined(__APPLE__) && defined(__APPLE_CC__)) || defined(__MWERKS__)
#  pragma unused (argc, argv)
# endif /* (__APPLE__ && __APPLE_CC__) || __MWERKS__ */
#endif /* DEBUG */
  /* Repeat for all the tests:
     - Parse data array and write into file.
       - Run replace hooks before writing to file.
     - Parse data array and build argv before/after.
       - Run replace hooks on argv before/after
     - Run expandargv.
     - Compare output of expandargv argv to after argv.
       - If they compare the same then test passes
         else the test fails.
     - Erase test file. */

  fails = run_tests(test_data);
  if (!fails)
    exit(EXIT_SUCCESS);
  else
    exit(EXIT_FAILURE);
}

/* EOF */
