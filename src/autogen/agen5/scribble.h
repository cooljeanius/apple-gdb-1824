
#ifndef SCRIBBLE_H
#define SCRIBBLE_H 1

#include <stdlib.h>

void   scribble_init(  void);
void   scribble_deinit(void);
void   scribble_free(  void);
void * scribble_get(   ssize_t);
void * xscribble_get(  ssize_t);

#endif /* SCRIBBLE_H */
