/* not-strip.c */
/* Linked with objcopy.o to flag that this program is 'objcopy'
 * (i.e. not 'strip').  */

extern int is_strip;
int is_strip = 0;
