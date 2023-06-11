/*#include <string.h>*/
#include <stdio.h>
/*#include <assert.h>*/

#include "joueur/score_game.h"

SHARED_EXPORT void victory_init( score_game_t *victory )
{
    victory->nb_win = 0;
    victory->nb_lost = 0;
    victory->nb_equality = 0;
}

score_game_t score_game_create()
{
    return score_game_create_with_param( 0, 0, 0);
}

score_game_t score_game_create_with_param(  unsigned int nb_win, 
                            unsigned int nb_lost, unsigned int nb_equality)
{
    score_game_t new_score;
    new_score.nb_win = nb_win;
    new_score.nb_lost = nb_lost;
    new_score.nb_equality = nb_equality;
    return new_score;
}
/*
score_game_t score_game_copy( const score_game_t *src_score)
{
    score_game_t new_score;
    new_score.nb_win = src_score->nb_win;
    new_score.nb_lost = src_score->nb_lost;
    new_score.nb_equality = src_score->nb_equality;
    return new_score;
}
*/

void score_game_print_info(const score_game_t* score)
{
    printf("victory : %d\n", score->nb_win);
    printf("lost    : %d\n", score->nb_lost);
    printf("equality: %d\n", score->nb_equality);
}