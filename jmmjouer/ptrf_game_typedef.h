#pragma once
/* include here the depenencies to joueur's struct, 
    (NOTE: may ask caller to define them, fast-forward possible ?)
    include full declaration of them, no fast forwarded */
#include "joueur/victory.h"
#include "joueur/person.h"

/* define pointer on start_game function in one unique place to avoid
 warning: redefinition of typedef ‘ptr_game_t’ [-Wpedantic] */
typedef victory_t(*ptr_game_t)(person_t);