#pragma once
/* include here the depenencies to joueur's struct, 
    (NOTE: may ask caller to define them, fast-forward possible ?)
    include full declaration of them, no fast forwarded */
#include "joueur/score_game.h"
#include "joueur/user.h"

/* define pointer on start_game function in one unique place to avoid
 warning: redefinition of typedef ‘ptr_game_t’ [-Wpedantic] */
typedef score_game_t(*ptr_game_t)(user_t);