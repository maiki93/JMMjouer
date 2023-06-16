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
 * Better if defined in jmmjouer / core domain
 * Or better, a compile time value provided with make -DMAX_NAME_GAME_LEN, coherent across all modules (default 20) 
 * => necessary #define ? */
enum { MAX_NAME_GAME_LEN = 20 };

/** Definition of the "map"
 * \ingroup entities_grp
*/
typedef struct {
    /** pointer a clist_gen_t implementation */
    clist_gen_t *clist;
} map_game_score_t;

/** \name Constructor / Destructor of the map
 * \{ */
/** Allocation for a map on heap.
 * A init function must be called to finalize the instance
 * \return pointer on an instance of map_game_score_t */
SHARED_EXPORT map_game_score_t* map_game_score_new();

/** Constructor.
 * \param[in] pointer on an instance of map_game_score_t */
SHARED_EXPORT void map_game_score_init(map_game_score_t *map);

/** Copy constructor, deep copy.
 * If map_src is empty, ?
 * Add log
 * \param[in] pointer on an instance of map_game_score_t
 * \param[out] status code, 0 : no error, to make more core model
 * \return a new instance on stack */
SHARED_EXPORT map_game_score_t map_game_score_copy( const map_game_score_t *map_src, 
                                                    int *status);

/** Clear all contents and free internal clist.
 * Instance is not usable after the call, need a new call to map_game_score_init
 * \param[in] pointer on an instance of map_game_score_t */
SHARED_EXPORT void map_game_score_delete(map_game_score_t *map);

/** Free memory on map allocated on heap
 * \param[in] pointer on an instance of map_game_score_t */
SHARED_EXPORT void map_game_score_free(map_game_score_t *map);
/** \} */

/** \name public methods API */
/** \{ */
/** Return the number of elements of the map.
 *  Complexity O(n)
 *  \param[in] pointer on an instance of map_game_score_t */
SHARED_EXPORT size_t map_game_score_size(const map_game_score_t *map);

/** Insert (with replcement) a pair_game_score_t into the map. 
 *  May pass pair_in as pointer
 * If an entry with the same game name is already present, it is replaced by the new score_t 
 * \param[in] pointer on an instance of map_game_score_t
 * \param[in] game_name name of the game, key value in map
 * \param[in] score of the game
 * \return status : -1=error, 0=insert new pair, 1=replace new pair */
SHARED_EXPORT int map_game_score_insert(map_game_score_t *map, const char *name_game, const score_game_t *score);

/** Retrieve the pair_game_score_t value with the provided key.
 *  Create a new entry if not present ? like a map ? need metod is_present()
    to think: make a ref version ? const *pair to enforce constness
 * \param[in] pointer on an instance of map_game_score_t
 * \param[in] name of the game
 * \param[out] score associated, 0,0,0 in case of errors 
 * \return  status 0: OK, -1: ERROR (may split) or indicate the key was not found */
SHARED_EXPORT int map_game_score_get_from_name( const map_game_score_t *map, const char *name, score_game_t *score_out);
   
/** Display content on console.
 *  print title and call print_info for each pair of the map. 
 *  \param[in] pointer on an instance of map_game_score_t */
SHARED_EXPORT void map_game_score_print_info(const map_game_score_t* map);

/** \} */