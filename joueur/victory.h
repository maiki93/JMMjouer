#pragma once

#include "shared_EXPORTS.h"

/* @brief Strucutre describing the result(s) of one or more round of a game.
    Also used for storing the historic in records and in definition of pointer to function start_game.
*/
typedef struct {
    int nb_win;
    int nb_lost;
    int nb_equality;
} victory_t;

SHARED_EXPORT void victory_init( victory_t *victory );

