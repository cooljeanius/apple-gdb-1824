/* not-ranlib.c */
/* Linked with ar.o to flag that this program is 'ar' (not 'ranlib').  */

extern int is_ranlib;
int is_ranlib = 0;

/* EOF */
