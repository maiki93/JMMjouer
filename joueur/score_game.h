#pragma once

#include "shared_EXPORTS.h"

/** @file
 * 
 * @ingroup entities_grp
 * 
 * Results of a specific game.
 * Used as result after playing a game (or serie of games) 
 * and for storing the scores for each joueur_t in a map_score_t (in-memory and db storage)
*/

/** @{ @ingroup entities_grp */

/** Describe the total scoring of a game.
 * Typical example of Value Object, should try to enforce immutability.
 * Only accessor, equality no real meaning here ? (used in test)
 * Miss privacy to completely forbid mutability (or something else ?) */
typedef struct {
    /** nb of victories for a game  */
    unsigned int nb_win;
    /** nb of lost */
    unsigned int nb_lost;
    /** nb of draw */
    unsigned int nb_draw;
} score_game_t;

/** Create a score_game default initialized
 * \post nb_win = nb_lost = nb_draw = 0
 * \return fresh new score_game_t */
SHARED_EXPORT score_game_t score_game_create();

/** Create a score_game with given initilization
 * \return fresh new score_game_t with provided values (const by behavior) */
SHARED_EXPORT score_game_t score_game_create_with_param(  unsigned int nb_win, 
                                    unsigned int nb_lost, unsigned int nb_draw);
/** Display info on console */
SHARED_EXPORT void score_game_print_info(const score_game_t* score);

/** @} */