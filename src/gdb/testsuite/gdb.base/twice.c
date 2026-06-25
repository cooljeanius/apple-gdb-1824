#include <stdio.h>
int
nothing(void)

{
    int x = 3 ;
    return x ;
}

int
main(void)

{
    int y ;
#ifdef usestubs
    set_debug_traps();
    breakpoint();
#endif    
    y = nothing () ;
    printf ("hello\n") ;
    return 0;
}
