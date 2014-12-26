/* is-ranlib.c */
/* Linked with ar.o to flag that this program is 'ranlib' (not 'ar').  */

extern int is_ranlib;
int is_ranlib = 1;
