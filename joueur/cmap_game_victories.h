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
    /* clist_gen_t clist; no access generic type in h  ? musst use pointer ? to check ..*/
    /** pointer a clist_gen_t */
    clist_gen_t *clist;
} cmap_game_victories_t;

/* internal usage only, dependent value_t and victory_t only 
    shared export not needed if internal module ?? */
/** \name transfert function between \ref pair_game_victory_t and \ref value_t for ccontainer 
 * \{ */
/** Return a value_t from a pair_game_victory_t */
/*SHARED_EXPORT*/ value_t make_value_victory( struct pair_game_victory_t victory );
/** Extract a pair_game_victory_t from a generic value_t */
/*SHARED_EXPORT*/ struct pair_game_victory_t extract_value_victory( const value_t* value);
/** \} */

/* to see in this case...
void deleter(value_t* value)
*/

/** \name Constructor / Destructor 
 * \{ */
/** Allocation */
SHARED_EXPORT cmap_game_victories_t* game_victories_new();
/** Constructor */
SHARED_EXPORT void game_victories_init(cmap_game_victories_t *cmap);
/** Clear all contents */
SHARED_EXPORT void game_victories_clear(cmap_game_victories_t *cmap);
/** Free memory */
SHARED_EXPORT void game_victories_delete(cmap_game_victories_t *cmap);
/** \} */

/** \name public API */
/** \{ */
/** Return the size of the map */
SHARED_EXPORT size_t game_victories_size(cmap_game_victories_t *cmap);
/** Insert a pair_game_victory_t into the map. */
SHARED_EXPORT int game_victories_insert( cmap_game_victories_t *cmap, struct pair_game_victory_t victory);
/** Retrieve the pair_game_victory_t value with the provided key */
SHARED_EXPORT struct pair_game_victory_t game_victories_get_copy( cmap_game_victories_t *cmap, const char *name);
/** \} */