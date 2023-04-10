#pragma once

#include "shared_EXPORTS.h"

/** @file
 * Results of games.
*/

/** Describe the scoring of a game.
   \ingroup entities_grp
*/
typedef struct {
    /** nb of victory */
    int nb_win;
    /** nb of lost */
    int nb_lost;
    /** nb of equality */
    int nb_equality;
} victory_t;

/** Default contructor. 
 * \param[in] victory pointer to a allocated victory_t instance
*/
SHARED_EXPORT void victory_init( victory_t *victory );

