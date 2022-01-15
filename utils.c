#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <unistd.h> // for clear_screen

#include "utils.h"

/***** Functions ******/

// to delete
bool rejouer_une_partie() {

    bool b_rejouer;
    b_rejouer = ask_yesno_question("Voulez-vous refaire une partie [y/Y/n/N] > ");
    return b_rejouer;
}

bool ask_yesno_question(const char* message)
{
    char answer_char;
    bool valid = false, answer_yes;

    do {
        printf("%s", message );
        scanf("%c", &answer_char );
        clean_stdin();

        switch( answer_char ) {
            case 'Y'  :
            case 'y'  : answer_yes = true;
                        valid = true;
                        break;
            case 'N'  :
            case 'n'  : answer_yes = false;
                        valid = true;
                        break;
            default   : valid = false;
        }

        if( valid == false ) {
            printf("Erreur en entree\n");
        }

    } while( valid == false );

    return answer_yes;
}

void clean_stdin(void)
{
    int c;
    do {
        c = getchar();
    } while (c != '\n' && c != EOF);
}

// https://stackoverflow.com/questions/2347770/how-do-you-clear-the-console-screen-in-c
void clear_screen()
{
  const char *CLEAR_SCREEN_ANSI = "\e[1;1H\e[2J";
  //write(STDOUT_FILENO, CLEAR_SCREEN_ANSI, 12);
  printf("%s", CLEAR_SCREEN_ANSI);
}
