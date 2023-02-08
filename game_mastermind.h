#ifndef GAME_MASTERMIND_H
#define GAME_MASTERMIND_H

#include "person.h"
#include "victory.h"

#if defined(_WIN32)
    #define SHARED_LIB __declspec(dllexport)
#else
    #undef SHARED_LIB
#endif

victory_t SHARED_LIB start_game_mastermind(person_t person);

#endif
