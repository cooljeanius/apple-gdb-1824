#include "list0.h"

int
main(void)
{
    int x;
#ifdef usestubs
    set_debug_traps();
    breakpoint();
#endif
    x = 0;
    foo (x++);
    foo (x++);
    foo (x++);
    foo (x++);
    foo (x++);
    foo (x++);
    foo (x++);
    foo (x++);
    foo (x++);
    foo (x++);
    foo (x++);
    foo (x++);
    foo (x++);
    foo (x++);
    foo (x++);
    foo (x++);
    foo (x++);
    foo (x++);
    foo (x++);
    foo (x++);
    foo (x++);
    foo (x++);
    foo (x++);
    foo (x++);
    foo (x++);
    return 0;
}

static void
unused(void)
{
    /* Not used for anything */
}
