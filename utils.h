#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include <stdbool.h>

void clean_stdin();
/* https://stackoverflow.com/questions/2347770/how-do-you-clear-the-console-screen-in-c */
void clear_screen();


/** Demande au joueur s'il veut refaire une partie
    return 1 pour oui, 0 pour non
    @deprecated ask yesno question more usefull
**/
bool rejouer_une_partie();

/* same as previous, more generic with texte */
/** Print a message to the user, loop until a vid (y/Y,n,N) answer is provided.

    return true for Y/y, false for n/N
**/
bool ask_yesno_question(const char* message);

#endif
