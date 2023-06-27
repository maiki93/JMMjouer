#pragma once

#include "jmmjouer/game_loader/game_loader.h"
#include "record/irecord_fwd.h" /* typedef irecord_t */

/** @defgroup jmmjouer_core_grp core module */

/** @file 
 * @ingroup jmmjouer_core_grp
 *  Core of the game.
*/

/** @{ \ingroup jmmjouer_core_grp */

/** Forward declaration. */
struct arcade_params_t;

/** Constructor. */
int arcade_init( game_loader_t *game_loader, irecord_t *record_impl);
/** Destructor */
void arcade_clear();
/** Enter main loop. */
int arcade_run( arcade_params_t *params );

/** @} */