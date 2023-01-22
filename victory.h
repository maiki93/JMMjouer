#pragma once

/* @brief Strucutre describing the result(s) of one or more round of a game.
    Also used for storic the historic in records. */
typedef struct {
    int nb_win;
    int nb_lost;
    int nb_equality;
} victory_t;

void victory_init( victory_t *victory );
