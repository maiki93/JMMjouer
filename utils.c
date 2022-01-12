#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "utils.h"

/***** Functions ******/


bool rejouer_une_partie() {

    char rejouer_char;
    bool b_rejouer;
    bool valid = false;

    do {
        printf("Voulez-vous refaire une partie [y/Y/n/N] > " );
        scanf("%c", &rejouer_char );
        clean_stdin();

        switch( rejouer_char ) {
            case 'Y'  :
            case 'y'  : b_rejouer = true;
                        valid = true;
                        break;
            case 'N'  :
            case 'n'  : b_rejouer = false;
                        valid = true;
                        break;
            default   : valid = false;
        }

        if( valid == false ) {
            printf("Erreur en entree\n");
        }

    } while( valid == false );

    return b_rejouer;
}

void clean_stdin(void)
{
    int c;
    do {
        c = getchar();
    } while (c != '\n' && c != EOF);
}
