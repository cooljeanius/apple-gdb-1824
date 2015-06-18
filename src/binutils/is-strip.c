/* is-strip.c */
/* Linked with objcopy.o to flag that this program is 'strip'
 * (i.e. not 'objcopy').  */

extern int is_strip;
int is_strip = 1;

/* EOF */
