#pragma once

#include "plugin_manager_fwd.h"
#include "jmmjouer/ptrf_game_typedef.h"

/** @file
 * @defgroup plugin_manager_grp plugins manager
    @ingroup game_loader_grp

    Responsible of the loading of plugins (shared librairies)
    Store the handles, must stay opened all the time.

    Associate a specific directory in which the dll are searched (nice for tests)
*/

/** @{ \ingroup plugin_manager_grp */

enum /*PLG_MGR : short */ 
{ PLG_MANAGER_OK = 0,
  PLG_MANAGER_DLL_NOT_FOUND = -1,
  PLG_MANAGER_FCT_NOT_FOUND = -2,
  PLG_MANAGER_VAR_NOT_FOUND = -3
}; /* cannot use all,  dans l'état (changer signature)*/

/** Local alias in plugin manager. */
typedef ptr_game_t ptr_plugin_funct;

/** Get a pointer to the singleton instance. 
 * Created if not yet initialised
*/
plugin_mgr_t* plugin_manager_get_instance();

/** Clear content.
   Reset directory_dll to current dir (".") */
void plugin_manager_clear(plugin_mgr_t *plg_manager);
/** Deallocate allocated memory. 
 * singleton point to NULL after the call, new instance recreated with call to get_instance */
void plugin_manager_free();

/** Getter of the current directory. */
const char* plugin_manager_get_directory(const plugin_mgr_t *plg_manager);

/** Set directory to look for dlls.
    Make an internal copy of the input 
    \return status */
int plugin_manager_set_directory( plugin_mgr_t *plg_manager, const char *directory);

/** Load dll "filename"(.so or .dll).
 *  search in currently set directory */
int plugin_manager_load_shared_library( plugin_mgr_t *plg_manager, const char *filename);

/** Get the pointer to start game function. 
 * Restrictive must be call on the last inserted, need better container
    if array, can call by indice */
ptr_plugin_funct plugin_manager_get_game_ptrf(const plugin_mgr_t *plg_manager, const char *name_start_fct);


/** Read the name of the game, and the name of the function to run the program as indicated in the plugin. */
int plugin_manager_get_names(const plugin_mgr_t *plg_manager, char **name_game_out, char **name_run_fct_out);

/** @} */ /* end group */