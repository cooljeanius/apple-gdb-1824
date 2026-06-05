#include <stdio.h>

int
callee(int x)
{
    int y = x * x;
    return (y - 2);
}

int
main(void)
{
    int i;
    for (i = 1; i < 10; i++)
        {
            printf( "%d ", callee( i ));
            
        }
    printf( " Goodbye!\n" );
    
}
