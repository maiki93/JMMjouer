#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

/***** Functions ******/
void clean_stdin(void)
{
    int c;
    do {
        c = getchar();
    } while (c != '\n' && c != EOF);
}
