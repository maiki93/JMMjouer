#include <stdio.h>
#include "joueur/score_game.h"

score_game_t score_game_create()
{
    return score_game_create_with_param( 0, 0, 0);
}

score_game_t score_game_create_with_param(  unsigned int nb_win, unsigned int nb_lost, 
                                            unsigned int nb_draw)
{
    score_game_t new_score;
    new_score.nb_win = nb_win;
    new_score.nb_lost = nb_lost;
    new_score.nb_draw = nb_draw;
    return new_score;
}

void score_game_print_info(const score_game_t* score)
{
    printf("victory : %d\n", score->nb_win);
    printf("lost    : %d\n", score->nb_lost);
    printf("equality: %d\n", score->nb_draw);
}