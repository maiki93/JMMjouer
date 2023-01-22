#ifndef GAME_PENDU_H_INCLUDED
#define GAME_PENDU_H_INCLUDED

#include "person.h"
#include "victory.h"

/* needed to produce the lib file with the dll */
#define SHARED_LIB __declspec(dllexport)

victory_t SHARED_LIB start_game_pendu(person_t person);

#endif
