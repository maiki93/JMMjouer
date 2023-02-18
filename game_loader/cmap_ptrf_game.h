#pragma once

#include <stddef.h> /* ask when use in game_loader_, load of dll ? */

#include "joueur/victory.h"
#include "joueur/person.h"
#include "ccontainer/clist_cstring.h"

typedef victory_t(*ptr_game_local_t)(person_t);

/* @brief C-style map to store a list of the game_name and their corresponding
    (pointer to ) function to run them. */

/* Close to cmap_game_victories, but try stronger encapsulation in this implementation.
    A priori only used by the game_loader module */

/* forward declaration, user can only use a pointer to it 
    Maybe not needed if encapsulated in a module... 
    Maybe shared (or some part) by plugin_manager if cannot make it fuly independent */
typedef struct cmap_ptrf_game_type cmap_ptrf_game_t;

/** Public API **/
cmap_ptrf_game_t* game_ptrf_new();
void game_ptrf_init(cmap_ptrf_game_t* cmap);
/* only use with dynamic allocation */
void game_ptrf_delete(cmap_ptrf_game_t *cmap);

size_t game_ptrf_size(const cmap_ptrf_game_t *cmap);

int game_ptrf_insert( cmap_ptrf_game_t *cmap, const char *name, ptr_game_local_t);
ptr_game_local_t game_ptrf_get_from_name(const cmap_ptrf_game_t *cmap, const char *name);

/* @brief Return the list of all game names in the map.
    It is a copy of the original data, the returned clist must be deallocated by the caller */
clist_cstring_t* game_ptrf_get_names( const cmap_ptrf_game_t *cmap );

/* boolean to delete previous entries or append data ? convenient ? */
/* @brief Return a array of string containing the game names.
    Deep copies of the internal elements.
    @deprecated 
    \param[out] heap allocated array_out with length array_len. must be deleted by caller
        (see carray_cstring_delete)
    \return CLIST_OK or error from clist_genric.h 
    */
int game_ptrf_get_array_name( const cmap_ptrf_game_t *cmap, char ***array_out, size_t *array_len );

