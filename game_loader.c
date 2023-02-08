
/*#include <dlfcn.h>*/
/* https://stackoverflow.com/questions/53530566/loading-dll-in-windows-c-for-cross-platform-design nice idea */
/* #include <windows.h> */ /* for_each_file move into utils_files, isolated */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/* \file Implementation of the loader

    Storage : similar to a map<nameGame, ptr_game_t>
*/

#include "game_loader.h"
#include "cmap_ptrf_game.h"
#include "plugin_manager.h" /* to get singleton instance */
#include "utils_file.h"
#include "clogger.h"
/* if forward declare in header */

/* Define games known at compilation in a separate file, always included */
#include "default_games.h"

#if defined(_WIN32)
    #define EXT_DLL "dll"
#else
    #define EXT_DLL "so"
#endif

/* full definition, a bit overkill this class with one data member but clear design and extendable */
struct game_loader_type {
    cmap_ptrf_game_t *map_game;
};

/*********** private functions *********/

/* split functions for game_loader_load_all */

static int load_static_game(game_loader_t *gameldr);
static int load_shared_game(game_loader_t *gameldr);
int add_game(game_loader_t *gload, const char *name, ptr_game_t pf_game);
bool  is_empty(const game_loader_t *gload);
/* static unsigned int size() {return nb_element;} */

/*static int load_game_dll( cmap_ptrf_game_t *map, const char *name); not used */
/* used as callback function by load_shared_game */
static int load_game_dll_callback( clist_gen_t *clist, const char *name);
/*********** end private functions *********/

/**********  intialization and destruction **************/
game_loader_t* game_loader_new()
{
    return (game_loader_t*)malloc( sizeof (game_loader_t));
}

void game_loader_init( game_loader_t *gameldr)
{
    /* initialize data member */
    gameldr->map_game = game_ptrf_new();
    game_ptrf_init( gameldr->map_game );
}
    
void game_loader_delete( game_loader_t *gameldr)
{
    CLOG_DEBUG("Clear game_loader %d\n", 1);
    game_ptrf_delete( gameldr->map_game );
    gameldr->map_game = NULL;
    free( gameldr );
}

int game_loader_load_all(game_loader_t *gameldr)
{
    /* should execute in order, important for container i think */
    if( load_static_game(gameldr) || load_shared_game(gameldr) )
        return 1;
    return 0;
}

clist_cstring_t * game_loader_get_names(const game_loader_t *gameldr)
{
    return game_ptrf_get_names( gameldr->map_game);
}

/* if allocation inside, the caller will need to delete the array 
   1. structure specific with a free fonction. 
   2. send only a const refernece to the internal array !! impossible ?
   3. can send back a full copy of the struct (on stack not possible ?) */
int game_loader_get_array_names(const game_loader_t *gload, char ***list_name_out, size_t *size_list)
{
    /* shoud be clist_cstring !!!  */
    return game_ptrf_get_array_name( gload->map_game, list_name_out, size_list );
}
/*  shoud be clist_cstring !!! 
int game_loader_get_names2(const game_loader_t *gload, clist_cstring *list_names)
{
    return game_ptrf_get_array_name( gload->map_game, list_name_out, size_list );
}*/

ptr_game_t game_loader_get_ptr_game( const game_loader_t *gameldr, const char * name_game)
{
    ptr_game_local_t pfgame_local;
    pfgame_local = game_ptrf_get_from_name( gameldr->map_game, name_game);
    return (ptr_game_t)pfgame_local;
}

/*** Private functions ***/
static int load_static_game(game_loader_t *gload) 
{
    int retour=0,i;
    /* load games known at compile-time*/
    for( i = 0; i < nb_default_game; i++ ) {
        retour = game_ptrf_insert( gload->map_game,
                                   default_game_name[i],
                                   default_game_prtf[i]);

        if( retour /*!= CLIST_OK */ ) {
            CLOG_ERR("Cannot load game : %s\n", "Mastermind or Morpion");
            return retour;
        }
    }
    return retour;
}

static int load_shared_game(game_loader_t *gload)
{
    int retour;
    const char *search_directory = plugin_manager_get_directory( plugin_manager_get_instance() );
    assert( search_directory );

    /* 2 functional style */
    CLOG_DEBUG("load all shared librairies in directory: %s\n", search_directory);
    retour = for_files_with_extension( search_directory, EXT_DLL,
                                       &load_game_dll_callback, 
                                       (clist_gen_t *)gload->map_game );
    /* to check retour or return directly */
    return retour;
}


int load_game_dll_callback( clist_gen_t *clist_base, const char *filename )
{
    ptr_game_t pf_game = NULL;
    int retour;
    plugin_mgr_t *plg_manager;
    /* down cast , must be sure */
    cmap_ptrf_game_t *map = (cmap_ptrf_game_t *)clist_base;
    assert( map );

    plg_manager = plugin_manager_get_instance();
    CLOG_DEBUG("Load game dll: %s\n", filename);

    /* could return the name of the start_game_function / with register easier */
    retour = plugin_manager_load_shared_library( plg_manager, filename );
    if(retour) {
        CLOG_ERR("Error in loading library %s ", filename);
        return 1;   
    }
    pf_game = (ptr_game_t) plugin_manager_get_game_ptrf( plg_manager, "start_game_morpion");
    
    /* need only map_game to be updated */
    retour = game_ptrf_insert( map, "Morpion", pf_game);

    return retour;
}

/* could pass plugin_manager as parameter / or own it / or call getInstance() */
int load_game_dll( cmap_ptrf_game_t *map, const char *filename )
{
    ptr_game_t pf_game = NULL;
    int retour;

    plugin_mgr_t *plg_manager = plugin_manager_get_instance();
    CLOG_DEBUG("Load game dll: %s\n", filename);

    /* could return the name of the start_game_function / with register easier */
    retour = plugin_manager_load_shared_library( plg_manager, filename );
    if(retour) {
        CLOG_ERR("Error in loading library %s ", filename);
        return 1;   
    }
    pf_game = (ptr_game_t) plugin_manager_get_game_ptrf( plg_manager, "start_game_morpion");
    
    /* need only map_game to be updated */
    retour = game_ptrf_insert( map, "Morpion", pf_game);

    return retour;
}

int add_game(game_loader_t *gload, const char *name, ptr_game_t pf_game) 
{
    
    CLOG_DEBUG("name: %s, ptr_game %p\n", name, pf_game);
    return game_ptrf_insert( gload->map_game, name, pf_game);
}

/* private function, can be tested with cmocka */
bool is_empty(const game_loader_t *gload) {

   return (game_ptrf_size( gload->map_game) == 0) ? true
                                                  : false;
}
