/*
 * calc.c
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#else
# include "calc.h"
#endif /* HAVE_CONFIG_H */
#ifdef HAVE_STDLIB_H
# include <stdlib.h>
#else
# warning calc.c expects <stdlib.h> to be included.
#endif /* HAVE_STDLIB_H */
#include <ctype.h>
#include <stdio.h>
#ifdef HAVE_STRING_H
# include <string.h>
#else
# ifdef HAVE_STRINGS_H
#  include <strings.h>
# else
#  warning calc.c expects a string-related header to be included.
# endif /* HAVE_STRINGS_H */
#endif /* HAVE_STRING_H */
#ifndef VERSION
# ifdef PACKAGE_VERSION
#  define VERSION PACKAGE_VERSION
# else
#  define VERSION 1.1
# endif /* PACKAGE_VERSION */
#endif /* !VERSION */
#define NWORD 10
#define SIZE 100

static int words();

int main()
{
   char line[SIZE];
   int nword;
   char *words[NWORD];

   while(printf("calc: "), fflush(stdout), fgets(line,SIZE,stdin) != NULL) {
      if((nword = split(line,words,NWORD)) == 0) continue;
      if(strcmp(words[0],"add") == 0) {
	 if(nword != 3) {
	    printf("Usage: add #1 #2\n");
	 } else {
	    printf("%d",atoi(words[1]) + atoi(words[2]));
	 }
      } else if(strcmp(words[0],"multiply") == 0) {
	 if(nword != 3) {
	    printf("Usage: multiply #1 #2\n");
	 } else {
	    int i1 = atoi(words[1]);
	    if(i1 == 2) i1 = 3;		/* this is a bug */
	    printf("%d",i1*atoi(words[2]));
	 }
      } else if(strcmp(words[0],"quit") == 0) {
	 break;
      } else if(strcmp(words[0],"version") == 0) {
	 printf("Version: %s",VERSION);
      } else {
	 printf("Unknown command: %s",words[0]);
      }
      printf("\n");
   }

   return(0);
}

int
split(line,words,nword)
char *line;
char **words;
int nword;				/* number of elements in words */
{
   int i;

   while(isspace(*line)) line++;
   if(*line == '\0') return(0);

   for(i = 0;i < nword;i++) {
      words[i] = line;
      while(*line != '\0' && !isspace(*line)) line++;
      if(*line == '\0') break;
      *line++ = '\0';
      while(isspace(*line)) line++;
   }

   return(i);
}
