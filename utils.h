#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include <stdbool.h>

void clean_stdin();

/** Demande au joueur s'il veut refaire une partie
    return 1 pour oui, 0 pour non
**/
bool rejouer_une_partie();

#endif // UTILS_H_INCLUDED
