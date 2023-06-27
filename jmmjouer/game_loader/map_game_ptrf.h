#pragma once

#include <stddef.h> /* ask when use in game_loader_, load of dll ? */

#include "jmmjouer/ptrf_game_typedef.h"
#include "ccontainer/clist_cstring.h"
#include "ccontainer/cvector_cstring.h"

/** @file
    @ingroup game_loader_class_grp

    C-style map to store a list of "key" / "value"
    key :  the game_name 
    value : corresponding (pointer to) a function to run the game of signature ptr_game_t

   Close to map_game_victories, but try stronger encapsulation in this implementation.
   Can be only allocated on heap, only used by the game_loader "module"
*/

/** @{ \ingroup game_loader_class_grp */

/** Forward declaration, user can only use a pointer to it.
    Maybe shared (or some part) by plugin_manager if cannot make it fuly independent */
typedef struct map_game_ptrf_type map_game_ptrf_t;

/** \name Constructor/Destructor */
/** @{ */
/** Allocator for object in heap memory.
 * A contructor game_ptrf_init() must be called to get a valid map.
 * \return a map pointer to an instance of map_game_ptrf_t */
map_game_ptrf_t* game_ptrf_new();

/** Constructor.
  \post size == 0
  \param[in] map pointer to an instance of map_game_ptrf_t */
void game_ptrf_init(map_game_ptrf_t* map);

/** Destructor, only use with dynamic heap allocation.
    \param[in] map pointer to an instance of map_game_ptrf_t*/
void game_ptrf_free(map_game_ptrf_t *map);
/** @} */

/** \name Public API methods */
/** \{ */
/** Return the number of elements in the map.
 *  \param[in] map pointer to an instance of map_game_ptrf_t
 *  \return the number of pair : (game_name, ptr_game_t) */
size_t game_ptrf_size(const map_game_ptrf_t *map);

/** Insert a pair : game_name / ptrf to the function to run the game 
 * \param[in] map pointer to an instance of map_game_ptrf_t
 * \param[in] name of the game
 * \param[in] pfgame_in pointer to the function to run a game
 * \return a status code, should be more model */
ccontainer_err_t game_ptrf_insert( map_game_ptrf_t *map, const char *name, ptr_game_t pfgame_in );

/** Return the pointer to the function to run the game with name "name"
 * \param[in] map pointer to an instance of map_game_ptrf_t
 * \param[in] name of the game
 * \return a ptr_game_t, NULL if not found */
ptr_game_t game_ptrf_get_from_name(const map_game_ptrf_t *map, const char *name, ccontainer_err_t *err_code);

/** Return the list of all game names in the map.
    It is a copy of the original data, the returned clist must be deallocated by the caller
    \param[in] map pointer to an instance of map_game_ptrf_t
    \return a clist_cstring_t, empty if error ? */
clist_cstring_t* game_ptrf_get_list_names( const map_game_ptrf_t *map );

/** Return a vector of C-strings of all game names in the map.
    It is a copy of the original data, the returned cvector must be deallocated by the caller
    Choose to return on stack, certainly the most usefull usage
    \param[in] map pointer to an instance of map_game_ptrf_t 
    \return a cvector_cstring_t, empty if error ?*/
cvector_cstring_t game_ptrf_get_names( const map_game_ptrf_t *map );

/** Return a array of C-string containing the game names.
    Deep copies of the internal elements.
    \param[in] map pointer to an instance of map_game_ptrf_t
    \param[out] array_out heap allocated array_out inside the function. Must be deleted by caller
        (see \ref carray_cstring_delete ccontainer_utils::carray_cstring_delete )
    \param[out] array_len number of elements in array_out
    \return status or error from clist_genric.h */
int game_ptrf_get_array_name( const map_game_ptrf_t *map, char ***array_out, size_t *array_len );
/** \} */

/** @} */ /* end game_loader_grp */