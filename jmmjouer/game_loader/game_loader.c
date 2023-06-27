
/*#include <dlfcn.h>*/
/* https://stackoverflow.com/questions/53530566/loading-dll-in-windows-c-for-cross-platform-design nice idea */
/* #include <windows.h> */ /* for_each_file move into utils_files, isolated */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "jmmjouer/game_loader/game_loader.h"
#include "jmmjouer/game_loader/map_game_ptrf.h"

#include "jmmjouer/game_loader/plugin_manager.h" /* to get singleton instance */
#include "jmmjouer/utils_file.h"                 /* contains algo generic */
#include "clogger/clogger.h"
/* Define games known at compilation in a separate file, always included at compilation */
#include "jmmjouer/games/default_games.h"

#if defined(_WIN32)
    #define EXT_DLL "dll"
#else
    #define EXT_DLL "so"
#endif

/** Complete definition of game_loader_t in implementation */
struct game_loader_type {
    /** internal map, no provided access */
    map_game_ptrf_t *map_game;
};

/*********** private functions *********/
/*********** called by public API game_loader_load_all */
static int load_static_game(game_loader_t *gameldr);
static int load_shared_game(game_loader_t *gameldr);
int add_game(game_loader_t *gload, const char *name, ptr_game_t pf_game);
bool  is_empty(const game_loader_t *gload);
/* Used as callback for use with generic utils methods */
static int load_game_dll_callback( clist_gen_t *clist, const char *name);
/* load shared library, and insert the pair name_game and ptrf into the internal map */
int load_game_dll( map_game_ptrf_t *map, const char *filename );
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
    
void game_loader_free( game_loader_t *gameldr)
{
    CLOG_DEBUG("Clear game_loader %d\n", 1);
    game_ptrf_free( gameldr->map_game );
    gameldr->map_game = NULL;
    free( gameldr );
}

size_t game_loader_size(const game_loader_t *gameldr)
{
    return game_ptrf_size( gameldr->map_game);
}

int game_loader_load_all(game_loader_t *gameldr)
{
    /* should execute in order, important for container i think */
    if( load_static_game(gameldr) || load_shared_game(gameldr) )
        return 1;
    return 0;
}

cvector_cstring_t game_loader_get_names(const game_loader_t *gameldr)
{
    return game_ptrf_get_names( gameldr->map_game);
}

/* if allocation inside, the caller will need to delete the array 
   1. structure specific with a free fonction. 
   2. send only a const refernece to the internal array !! impossible ?
   3. can send back a full copy of the struct (on stack not possible ?) */
int game_loader_get_array_names(const game_loader_t *gload, char ***list_name_out, size_t *size_list)
{
    return game_ptrf_get_array_name( gload->map_game, list_name_out, size_list );
}

ptr_game_t game_loader_get_ptr_game( const game_loader_t *gameldr, const char * name_game)
{
    ccontainer_err_t err_code;
    ptr_game_t pfgame_local;
    pfgame_local = game_ptrf_get_from_name( gameldr->map_game, name_game, &err_code);
    return (ptr_game_t)pfgame_local;
}

/*** Private functions ***/
static int load_static_game(game_loader_t *gload) 
{
    int status=0,i;
    /* load games known at compile-time*/
    for( i = 0; i < nb_default_game; i++ ) {
        status = game_ptrf_insert( gload->map_game,
                                   default_game_name[i],
                                   default_game_prtf[i]);

        if( status /*!= CLIST_OK */ ) {
            CLOG_ERR("Cannot load static game : %s\n", "Mastermind or Pendu");
            return status;
        }
    }
    return status;
}

static int load_shared_game(game_loader_t *gload)
{
    int status;
    const char *search_directory = plugin_manager_get_directory( plugin_manager_get_instance() );
    assert( search_directory );

    /* 2 functional style */
    CLOG_DEBUG("load all shared librairies in directory: %s\n", search_directory);
    status = for_files_with_extension( search_directory, EXT_DLL,
                                       &load_game_dll_callback, /* interface to use utils */
                                       (clist_gen_t *)gload->map_game );
    /* to check status or return directly */
    return status;
}

int load_game_dll_callback( clist_gen_t *clist_base, const char *filename )
{
   int status;

    /* cast , it allows to retrive the map, not provided in initial call to utils
        a bit tricky ... but keep for_extension_general */
    map_game_ptrf_t *map = (map_game_ptrf_t *)clist_base;
    assert( map );

    status = load_game_dll( map, filename);
    return status;
}

int load_game_dll( map_game_ptrf_t *map, const char *filename )
{
    ptr_game_t game_ptrf = NULL;
    int status;
    char *name_game;
    char *name_start_fct;

    plugin_mgr_t *plg_manager = plugin_manager_get_instance();
    CLOG_DEBUG("Load game dll: %s\n", filename);

    /* could return the name of the start_game_function / with register easier */
    status = plugin_manager_load_shared_library( plg_manager, filename );
    if(status) {
        CLOG_ERR("Error in loading library %s ", filename);
        return 1;   
    }

    /* 2 stages : 1. get the name of the game and the name of the function to call for starting a game */
    status = plugin_manager_get_names( plg_manager, &name_game, &name_start_fct);
    if(status != 0) {
        return status;
    }
    /* get the required function */
    game_ptrf = (ptr_game_t) plugin_manager_get_game_ptrf( plg_manager, name_start_fct);
    
    /* insert the pair <name_game, game_ptrf */
    status = game_ptrf_insert( map, name_game, game_ptrf);

    /* free tmp */
    free(name_game);
    free(name_start_fct);
    return status;
}

int add_game(game_loader_t *gload, const char *name, ptr_game_t pf_game) 
{
    CLOG_DEBUG("name: %s, ptr_game %p\n", name, pf_game);
    return game_ptrf_insert( gload->map_game, name, pf_game);
}

bool is_empty(const game_loader_t *gload) {

   return (game_loader_size( gload) == 0) ? true
                                          : false;
}
