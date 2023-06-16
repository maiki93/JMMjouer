#ifndef GAME_PENDU_H_INCLUDED
#define GAME_PENDU_H_INCLUDED

/*
#include "joueur/person.h"
#include "joueur/victory.h" */

#include "jmmjouer/ptrf_game_typedef.h"

#include "shared_EXPORTS.h"

/* really need to export ? adress of function taken in default_games.c (maybe at runtime ?) */
score_game_t SHARED_EXPORT start_game_pendu(user_t user);


#endif
