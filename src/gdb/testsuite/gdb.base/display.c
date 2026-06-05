/* Loop and vars for tests of display commands
*/
#include <stdio.h>
#define LOOP 10

int sum = 0;

int
do_loops(void)
{
    int i=0;
    int k=0;
    int j=0;
    float f=3.1415;
    for( i = 0; i < LOOP; i++ ) {
        for( j = 0; j < LOOP; j++ ) {
            for( k = 0; k < LOOP; k++ ) {
                sum++; f++;
            }
        }
    } 
    return i;
}

int
do_vars(void)
{
    int       j;
    int       i = 9;
    float     f = 1.234;
    char      c = 'Q';
    int    *p_i = &i;
    float  *p_f = &f;
    char   *p_c = "rubarb and fries";

    /* Need some code here to set breaks on.
     */
    for( j = 0; j < LOOP; j++ ) {
        if( p_c[j] == c ) {
            j++;
        } 
        else {
            i++;
        }
    }

    return *p_i;
}

int
main(void)
{
    do_loops();
    do_vars();    
}
