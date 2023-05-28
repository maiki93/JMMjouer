#pragma once

/* needed for ptr_game_t definition */
#include "jmmjouer/ptrf_game_typedef.h"
#include "ccontainer/clist_cstring.h"

/* not exported as shared library */
/*#include "shared_EXPORTS.h"*/
/*#undef SHARED_EXPORT*/

/* plg_manager singleton => used only internally but too restrictive */
/*#include "plugin_manager_fwd.h"*/
/*#include "plugin_manager.h" *//* in charge of shared libraries */

/* \brief game_loader 
   Load, store and provide pointers to execute the different games (compiled and plugins)
   To call during during init phase (to extend lazy loading ? make sense ?)

   At least 2 responsabilities: storage and retrieve data, action of loading dll
    plugin_manager took resp. of loading dll 
    
  v2 no singleton implementation of game_loader (often better for tests, but not necessary)
    plugin_manager, makes sense ? 
      for register function for sure need a static function
          singleton really necessary ? how to do without a static instance ?
      for testing : mock easy to create ? without derive class ?
            1. singleton plugin_manager
            2. singleton mock_plugin_manager
          maybe 2 functions to load singleton:
          game::loader::get_plug_sing( funct_get_mock/real ) // return type !! arg!
                                                            // must be castable !

  For sure better internal storage 
    - with a cmap_game<name_game, prt_f>, internal/private use only

  Try to avoid to recopy the interface of plugin_manager
  */


/* to get a better API, to try "namespace" ERR_GAME_LOADER::GAME_LOADER_OK */
enum {
    GAME_LOADER_OK = 0,
    GAME_LOADER_ERROR_FILE = 1,
    GAME_LOADER_ERROR_FUNCTION = 2,
    GAME_LOADER_END = GAME_LOADER_ERROR_FUNCTION /*to check, good for extension */
};

typedef struct game_loader_type game_loader_t;

/***
 * Constructor / Destructor 
***/
/*SHARED_EXPORT*/ game_loader_t* game_loader_new();
/*SHARED_EXPORT*/ void game_loader_init( game_loader_t *gameldr);
/* Clear content and delete memory */
/*SHARED_EXPORT*/ void game_loader_free( game_loader_t *gameldr);

/* @brief Load all games: static + shared
    Directory providied by plugin_mamanger instance() singleton 
    TODO better */
/*SHARED_EXPORT*/ int game_loader_load_all(game_loader_t *gameldr);

/*SHARED_EXPORT*/ clist_cstring_t * game_loader_get_names(const game_loader_t *gameldr);
/* @brief Return an array with the game names.
   It is copy, the caller owns the array and is responsible of the deallocation 
   TO DO clist_cstring as return
   \return != 0 if error */
/*SHARED_EXPORT*/ int game_loader_get_array_names(const game_loader_t *gameldr, char ***list_out, size_t *size_list);

/* @brief Get pointer to game function given its name */
/*SHARED_EXPORT*/ ptr_game_t game_loader_get_ptr_game(const game_loader_t *gameldr, const char * name_game);
