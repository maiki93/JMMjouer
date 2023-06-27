#pragma once

/* needed for ptr_game_t definition */
#include "jmmjouer/ptrf_game_typedef.h"
#include "ccontainer/cvector_cstring.h"

/* always dont know where to declare new group */
/** @defgroup game_loader_grp game loader */

/** @file
 * 
 * @ingroup game_loader_grp
 * 
 * Load, store and provide pointers to execute the different games.
 *  Group all games : compiled with the source code and plugins with plugin_manager
 *
 * Use plugin_manger singleton instance to load games provided by plugins
 * Internal storage with a cmap_game<name_game, prt_f>, internal/private use only
 */

/** @{ \ingroup game_loader_grp */

/** Error code for jmmjouer core */
enum {
    GAME_LOADER_OK = 0,
    GAME_LOADER_ERROR_FILE = 1,
    GAME_LOADER_ERROR_FUNCTION = 2,
    GAME_LOADER_END = GAME_LOADER_ERROR_FUNCTION /*to check, good for extension */
};

/** Public access to the structure, implementation hidden in cpp */
typedef struct game_loader_type game_loader_t;

/** \name Constructor / Destructor */
/** \{ */
/** Allocation */
game_loader_t* game_loader_new();
/** Constructor */
void game_loader_init( game_loader_t *gameldr);
/** Destructor and deallocate memory */
void game_loader_free( game_loader_t *gameldr);
/** \} */

/** \name Public API */
/** \{ */
/** Return the number of available games
 * \param[in] gameldr pointer to an instance of game_loader_t
 * \return the number of loaded games */
size_t game_loader_size(const game_loader_t *gameldr);
/** Load all games: compiled with JMMjouer source + shared + plugins.
   Plugins are searched in the directory provided by the plugin_mamanger instance singleton
   \param[in] gameldr pointer to an instance of game_loader_t
   \return status */
int game_loader_load_all(game_loader_t *gameldr);

/** Return the names of all loaded games
 * \param[in] gameldr pointer to an instance of game_loader_t
 * \return a ccontainer vector of C-strings */
cvector_cstring_t game_loader_get_names(const game_loader_t *gameldr);

/** Get a pointer to the game function given its name
 * \param[in] gameldr pointer to an instance of game_loader_t
 * \return a pointer to function */
ptr_game_t game_loader_get_ptr_game(const game_loader_t *gameldr, const char * name_game);

/** Return an array with the game names.
   It is copy, the caller owns the array and is responsible of the deallocation 
   TO DO clist_cstring as return
   @deprecated
   \param[in] gameldr pointer to an instance of game_loader_t
   \return != 0 if error */
int game_loader_get_array_names(const game_loader_t *gameldr, char ***list_out, size_t *size_list);
/** \} */

/** @} */ /* end group */
