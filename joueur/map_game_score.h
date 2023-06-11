#pragma once

#include "ccontainer/clist_generic.h"
#include "joueur/score_game.h"

#include "shared_EXPORTS.h"

/** @file 
 * Implements a structure similar to a map<string, score_game_t> to store all results for a joueur_t.
 * 
 * score_game_t treated as Value Object, so each pair element can also be.
 * Map always replace existing entries, and always return copies to enfoce the bahavior.
 * 
 * Implementation use internally a clist_gen_t list (algorithmic performance of a list),
 * providing an interface similar to a map.
*/

/** Maximum length of the game name (terminal caracter ?).
 * Assert now, later maybe longer C-string will be tuncated.
 * Better if defined in jmmjouer / core domain */
enum { MAX_NAME_GAME_LEN = 20 };

/** Define a pair : <game name, victory_t> to be inserted in a ccontainer.
 * 
 * \ingroup entities_grp
*/
/* typedef if used from outside,
 should be hidden, really need pair details from outside ? */
struct pair_game_score_t {
    /** key, constant usually in a map */
    /*const*/ char game_name[MAX_NAME_GAME_LEN];
    /** value, results of games a Value Object constness to enforce */
    /*const*/ score_game_t score;
};

/** Definition of the "map".
 * 
 * \ingroup entities_grp
*/
typedef struct {
    /** pointer a clist_gen_t implementation */
    clist_gen_t *clist;
} map_game_score_t;


/** \name Adapter functions to ccontainer for \ref pair_game_score_t. 
 * \{ */
/** Return a ccontainer_value_t from a pair_game_score_t.
 * Create a copy of all data in pair, for insertion into the cmap
 * \param[in] pointer to a pair
 * \param[out] pointer to a ccontainer error code
 * \return ccontainer_value_t with a copy of the data in pair */
ccontainer_value_t make_value_pair_score( const struct pair_game_score_t *pair_victory_in, ccontainer_err_t *err_code);

/** Extract a pair_game_score_t from a generic value_t 
 *  Create a copy of the data in value_in into pair_victory_out
 * \pre pair_out must point to a valid adress (memory already allocated on stack or on heap before the call)
 * \note no memory allocation, err_code useless. may change api / return pair_game_score_t
 * \param[in] pointer to a value_t as source
 * \param[out] pointer to a pair_victory_t, memory must be allocated before the call
 * \return error_code from ccontainer library. */
ccontainer_err_t extract_value_pair_score( const ccontainer_value_t* value_in, struct pair_game_score_t *pair_victory_out);
/* better to use status of joueur if correct ? or level above at least */

/* All allocation on heap : do not need specific deleter or duplicater because no memory allocation
void deleter(value_t* value) */

/** \} */ /* end of adapter section */


/** \name Constructor / Destructor of the map
 * \{ */
/** Allocation */
SHARED_EXPORT map_game_score_t* map_game_score_new();
/** Constructor */
SHARED_EXPORT void map_game_score_init(map_game_score_t *map);

/** Copy constructor, deep copy.
 * May change error code, we are calling from core model here.
 * Add log
 */
SHARED_EXPORT map_game_score_t map_game_score_copy(const map_game_score_t *map_in, 
                                                        ccontainer_err_t *err_code);

/** Clear all contents */
SHARED_EXPORT void map_game_score_delete(map_game_score_t *map);
/** Free memory */
SHARED_EXPORT void map_game_score_free(map_game_score_t *map);
/** \} */

/** \name public API */
/** \{ */
/** Return the size of the map.
 *  complexity O(n)
 */
SHARED_EXPORT size_t map_game_score_size(const map_game_score_t *map);

/** Insert a pair_game_score_t into the map. 
 *  May pass pair_in as pointer
*/
SHARED_EXPORT ccontainer_err_t map_game_score_insert(map_game_score_t *map, const struct pair_game_score_t* victory_in);

/** Retrieve the pair_game_score_t value with the provided key */
SHARED_EXPORT struct pair_game_score_t map_game_score_get_from_name( const map_game_score_t *map, const char *name);
/** make a ref version ? const *pair to enforce constness */

/** Display content on console. */
SHARED_EXPORT void map_game_score_print_info(const map_game_score_t* map);
/** \} */