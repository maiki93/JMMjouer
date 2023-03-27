#ifndef GAME_PENDU_H_INCLUDED
#define GAME_PENDU_H_INCLUDED

#include "joueur/person.h"
#include "joueur/victory.h"

#include "shared_EXPORTS.h"

/* really need to export ? adress of function taken in default_games.c (maybe at runtime ?) */
victory_t SHARED_EXPORT start_game_pendu(person_t person);


#endif
