#include <stdio.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include "ALU.h"  /* Include the header here, to obtain the function declaration */

int main(void)
{
    int y = add(3);  /* Use the function here */
    printf("%d\n", y);
    return 0;
}