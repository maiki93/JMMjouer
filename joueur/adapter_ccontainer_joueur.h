#pragma once

#include "joueur/joueur.h"
#include "ccontainer/value.h"

/* added for list_to_vector, maybe to split in an other file */
#include "joueur/list_joueur.h"
#include "joueur/vector_joueur.h"

/** @file
 * Adapter functions to use objects values from joueur library with ccontainer types.
 * 
 * provided by libjoueur (entities)? 
 *   or better inside jmmjouer main domain core , adapter_ccontainer subdirectory ?
 * => used by librecord also
 * 
 * No need of specifc duplicater for joueur, a copy of map_game_victories is done on the heap
*/

/**
 * \ingroup entities_grp
*/

/** @name Joueur adapters functions */
/** @{ */

SHARED_EXPORT ccontainer_value_t make_value_joueur(const joueur_t *joueur_in, ccontainer_err_t *err_code);

SHARED_EXPORT ccontainer_err_t extract_value_joueur(const ccontainer_value_t *value_in, joueur_t *joueur_out);

SHARED_EXPORT void joueur_deleter_value(ccontainer_value_t* value_in);

/* no need
ccontainer_value_t joueur_duplicater_value(const ccontainer_value_t *value_src, ccontainer_err_t *err_code);
*/

/** @} */

SHARED_EXPORT vector_joueur_t* list_joueur_to_vector( list_joueur_t *list);
/* using generic implementation, we could move data more efficiently
    No need to pass by intermediate joueur of push and pop, serailization/deserialization */
SHARED_EXPORT vector_joueur_t* list_joueur_to_vector_generic_impl( list_joueur_t *list);