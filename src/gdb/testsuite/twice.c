/*
 * twice.c
 */

#include <stdio.h>
int nothing ()

{
    int x = 3 ;
    return x ;
}


int main ()

{
    int y ;
#ifdef usestubs
    set_debug_traps();
    breakpoint();
#endif /* usestubs */
    y = nothing () ;
    printf ("hello\n") ;
    return 0;
}

/* EOF */
