#pragma once

#include "shared_EXPORTS.h"

/** @file
 * Results of a specific game.
 * Used as result after playing a game (or serie of games) 
 * and for storing the scores for each joueur_t in a map_score_t (in-memory and db storage)
*/

/** Describe the total scoring of a game.
 * Typical example of Value Object, should try to enforce immutability.
 * Will change interface victory_create() / _with_param( 0, 0, 0) to work only with object (still const* possible)
 * Only accessor, equality no real meaning here
   \ingroup entities_grp
*/
typedef struct {
    /** nb of victories for a game  */
    unsigned int nb_win;
    /** nb of lost */
    unsigned int nb_lost;
    /** nb of equality */
    unsigned int nb_equality;
} score_game_t;

/** Default contructor. 
 * @deprecated
 * \param[in] score pointer to a allocated victory_t instance
*/
SHARED_EXPORT void score_game_init( score_game_t *score );

/** Create a score_game default initialized
 * \post nb_win = nb_lost = nb_equality = 0
 * \return fresh new score_game_t */
SHARED_EXPORT score_game_t score_game_create();

/** Create a score_game with given initilization
 * \post nb_win = nb_lost = nb_equality = 0
 * \return fresh new score_game_t */
SHARED_EXPORT score_game_t score_game_create_with_param(  unsigned int nb_win, 
                            unsigned int nb_lost, unsigned int nb_equality);

SHARED_EXPORT score_game_t score_game_copy( const score_game_t *src_score);

/* reset() */
