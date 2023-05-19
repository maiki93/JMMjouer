#pragma once

#include "ccontainer/clist_generic.h"
#include "victory.h"

#include "shared_EXPORTS.h"

/** @file 
 * Implements a structure similar to a map<string, victory_t> to store all results for a joueur_t.
 * 
 * Implementation use internally a clist_gen_t list (algorithmic performance of a list).
 * Provide an interface similar to a map.
*/

/** Define a pair : <game name, victory_t> to be inserted in a ccontainer.
 * 
 * \ingroup entities_grp
*/
struct pair_game_victory_t {
    /** key */
    char game_name[20];
    /** value */
    victory_t victories;
};

/** Definition of the "map".
 * 
 * \ingroup entities_grp
*/
typedef struct {
    /** pointer a clist_gen_t */
    clist_gen_t *clist;
} cmap_game_victories_t;


/** \name Adapter functions to ccontainer for \ref pair_game_victory_t. 
 * \{ */
/** Return a ccontainer_value_t from a pair_game_victory_t.
 * Create a copy of all data in pair, for insertion into the cmap
 * \param[in] pointer to a pair
 * \param[out] pointer to a ccontainer error code
 * \return ccontainer_value_t with a copy of the data in pair
 */
ccontainer_value_t make_value_pair_victory( const struct pair_game_victory_t *pair_victory_in, ccontainer_err_t *err_code);

/** Extract a pair_game_victory_t from a generic value_t 
 *  Create a copy of the data in value_in into pair_victory_out
 * \pre pair_out must point to a valid adress (memory already allocated on stack or on heap before the call)
 * \note no memory allocation, err_code useless. may change api.
 * \param[in] pointer to a value_t as source
 * \param[out] pointer to a pair_victory_t, memory must be allocated before the call
 * \return error_code from ccontainer library. */
ccontainer_err_t extract_value_pair_victory( const ccontainer_value_t* value_in, struct pair_game_victory_t *pair_victory_out);

/* All allocation on heap : do not need specific deleter or duplicater
void deleter(value_t* value)
*/

/** \} */ /* end of adapter section */


/** \name Constructor / Destructor 
 * \{ */
/** Allocation */
SHARED_EXPORT cmap_game_victories_t* game_victories_new();
/** Constructor */
SHARED_EXPORT void game_victories_init(cmap_game_victories_t *cmap);
/** Clear all contents */
SHARED_EXPORT void game_victories_delete(cmap_game_victories_t *cmap);
/** Free memory */
SHARED_EXPORT void game_victories_free(cmap_game_victories_t *cmap);
/** \} */

/** \name public API */
/** \{ */
/** Return the size of the map */
SHARED_EXPORT size_t game_victories_size(cmap_game_victories_t *cmap);
/** Insert a pair_game_victory_t into the map. */
SHARED_EXPORT ccontainer_err_t game_victories_insert( cmap_game_victories_t *cmap, struct pair_game_victory_t victory);
/** Retrieve the pair_game_victory_t value with the provided key */
SHARED_EXPORT struct pair_game_victory_t game_victories_get_copy( cmap_game_victories_t *cmap, const char *name);
/** \} */