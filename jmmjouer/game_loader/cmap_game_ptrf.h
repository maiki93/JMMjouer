#pragma once

#include <stddef.h> /* ask when use in game_loader_, load of dll ? */

#include "jmmjouer/ptrf_game_typedef.h"
#include "ccontainer/clist_cstring.h"

/** \file 
 * C-style map to store a list of 
 * the game_name / corresponding (pointer to ) function to run them. 

   Close to cmap_game_victories, but try stronger encapsulation in this implementation.
    A priori only used by the game_loader module 
*/

/** @{ \ingroup game_loader_grp */

/** Forward declaration, user can only use a pointer to it.
    Maybe shared (or some part) by plugin_manager if cannot make it fuly independent */
typedef struct cmap_game_ptrf_type cmap_game_ptrf_t;

/** \name Constructor/Destructor */
/** @{ */
cmap_game_ptrf_t* game_ptrf_new();
void game_ptrf_init(cmap_game_ptrf_t* cmap);
/* only use with dynamic allocation */
void game_ptrf_free(cmap_game_ptrf_t *cmap);
/** @} */

/** Return the number of elements */
size_t game_ptrf_size(const cmap_game_ptrf_t *cmap);

ccontainer_err_t game_ptrf_insert( cmap_game_ptrf_t *cmap, const char *name, ptr_game_t);
ptr_game_t game_ptrf_get_from_name(const cmap_game_ptrf_t *cmap, const char *name, ccontainer_err_t *err_code);

/* @brief Return the list of all game names in the map.
    It is a copy of the original data, the returned clist must be deallocated by the caller */
clist_cstring_t* game_ptrf_get_names( const cmap_game_ptrf_t *cmap );

/* boolean to delete previous entries or append data ? convenient ? */
/* @brief Return a array of string containing the game names.
    Deep copies of the internal elements.
    @deprecated 
    \param[out] heap allocated array_out with length array_len. must be deleted by caller
        (see carray_cstring_delete)
    \return CLIST_OK or error from clist_genric.h 
    */
int game_ptrf_get_array_name( const cmap_game_ptrf_t *cmap, char ***array_out, size_t *array_len );

/** @} */ /* end game_loader_grp */