#ifndef PLUGIN_MANAGER_H
#define PLUGIN_MANAGER_H

/* @file
     Responsible of the loading of shared librairies, access functions  
    and storage of associated handles.

    Associate a specific directory in which the dll are searched (nice for tests)
    
    
First version load directly the dll
Improvments to see later : provide a register function for the dll to load at init
    to avoid a fixed name of the function to call (need static public entry)

Second version: 
    Plugin_manager as singleton :
        => easier to create a register function
 from dll plugin_manager_register(name_of_game, function_game, others...) OK
 but finally want to be stored in game_loader, how to do it simply
    => game_loader_load_plugin( name_plugin.dll, function_to_load_windows_dll )
           load_plugin call function_to_load_win_dll( name*dll, ptrf_game,...)
                plugin_manager_load_win_dll : store INSTANCE
                    call dll wich register itself
                    from registration : update parameter for game_loader
        does it decrease coupling ?
        certianly tests to do before

Etat actuel :
    game_loader< game_name_string, ptrf_game > 
    // miss reference to delete from plugin_manager (a later extension)
*/

enum /*PLG_MGR : short */ 
    { PLG_MANAGER_OK = 0,
      PLG_MANAGER_DLL_NOT_FOUND = -1,
      PLG_MANAGER_FCT_NOT_FOUND = -2 }; /* cannot use this one,  dans l'état (changer signature)*/

/* Forward declaration of plugin_manager and alias it for users */
/* typedef struct plugin_manager plugin_mgr_t; */
#include "plugin_manager_fwd.h"
#include "jmmjouer/ptrf_game_typedef.h"

/** local alias in plugin manager */
typedef ptr_game_t ptr_plugin_funct;

plugin_mgr_t* plugin_manager_get_instance();
/** Clear content.
   Reset directory_dll to current dir (".") */
void plugin_manager_clear(plugin_mgr_t *plg_manager);
/** Deallocate allocated memory.
   Caller should set manager to NULL after call 
   !!  possible ?? !! */
void plugin_manager_free();

/** Getter */
const char* plugin_manager_get_directory(const plugin_mgr_t *plg_manager);

/** Set directory to look for dlls.
    Make an internal copy of the input 
    \return status */
int plugin_manager_set_directory( plugin_mgr_t *plg_manager, const char *directory);

/** Load dll "filename"(.so or .dll) in currently set directory */
int plugin_manager_load_shared_library( plugin_mgr_t *plg_manager, const char *filename);

/** Restrictive must be call on the last inserted, need better container
    if array, can call by indice */
ptr_plugin_funct plugin_manager_get_game_ptrf(const plugin_mgr_t *plg_manager, const char *name_start_fct);
    /*, char** name_game_out);*/

/** Read the name of the game, and the name of the function to run the program as indicated in the plugin */
int plugin_manager_get_names(const plugin_mgr_t *plg_manager, char **name_game_out, char **name_run_fct_out);

#endif
