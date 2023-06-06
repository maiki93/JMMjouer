#pragma once

#include "shared_EXPORTS.h"

/** @file
 * Results of a game
*/

/** Describe the scoring of a game.
 * Typical example of Value Object, should try to enforce immutability.
 * Will change interface victory_create() / _with_param( 0, 0, 0) to work only with object (still const* possible)
 * Only accessor, equality no real meaning here
   \ingroup entities_grp
*/
typedef struct {
    /** nb of victories for a game  */
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
/* reset() */
