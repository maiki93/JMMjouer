#include "plugin_manager.h"

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "clogger.h"

enum { MAX_NB_SHARED_LIB = 5};

static const char* CURRENT_DIRECTORY = ".";

/* https://helpercode.com/2015/09/16/how-to-fake-a-singleton-in-c/ */
/* https://stackoverflow.com/questions/9191530/how-to-unit-test-singleton-class-c */
/* general advice (C++) split implementation behind a class wrapper, allow test/mock... */
/* or atexit
    https://stackoverflow.com/questions/39385520/do-i-have-to-free-static-dynamically-allocated-pointers */

/* declaration of the structure */
struct plugin_manager {
    HINSTANCE handle_dll[MAX_NB_SHARED_LIB]; /* INVALID_HANDLE_VALUE */
    size_t nb_handle; /*= 0 primitive initialized by default */
    char* directory_path; /* default initialized to NULL ? bad current_dir  */
};

/* We can use this unique instance for creation and deletion (and reset to null) */
static struct plugin_manager *singleton;

/*** Allocation / Constructor are private ***/
static plugin_mgr_t* plugin_manager_new();
/* Directory not used if called only from new. 
    Make an internal copy of the input */
static void plugin_manager_init( const char *directory );

plugin_mgr_t* plugin_manager_get_instance()
{
    /* only one malloc by program, should be one free (atexit for instance)
       Do not manage to reset this singleton to NULL => multipe free 
       can delete the pointed object, but not able to reset this static variable to NULL (for TU) */
    /*static plugin_mgr_t *singleton;*/

    if( singleton == NULL ) {
        singleton = plugin_manager_new();
        plugin_manager_init( CURRENT_DIRECTORY );
        CLOG_DEBUG("New instance : %p\n", (void*)singleton);
        return singleton;
    }
    CLOG_DEBUG("Existing instace : %p\n", (void*)singleton);
    return singleton;
}

plugin_mgr_t* plugin_manager_new()
{
    /* considering 0 byte initialization are safe for HANDLE, pointers etc... calloc is ok 
       In this case handle[] could be the value on Failure of LoadLibrary, specific Windows API */
    plugin_mgr_t *manager = (plugin_mgr_t*) calloc( 1, sizeof(struct plugin_manager) );
    /* else (NULL not 0 byte-array) fully portable way 
    plugin_mgr_t *manager = malloc( sizeof(struct plugin_manager) );
    memset( manager->handle_dll, 0, sizeof(manager->handle_dll));
    manager->nb_handle = 0;
    manager->directory_path = NULL;
    */
    return manager;
}

void plugin_manager_init(const char* directory)
{
    size_t i;
    
    for(i = 0; i < MAX_NB_SHARED_LIB; i++)
        singleton->handle_dll[i] = NULL;
    singleton->nb_handle = 0;

    plugin_manager_set_directory( singleton, directory );
}

void plugin_manager_clear(plugin_mgr_t *plg_manager)
{
    size_t i;
    CLOG_DEBUG("Destructor nb dll %d\n", plg_manager->nb_handle);

    for(i = 0; i < plg_manager->nb_handle; i++) {
        FreeLibrary(plg_manager->handle_dll[i]);
        plg_manager->handle_dll[i] = NULL;
        CLOG_DEBUG("Remove shared lib from handle %ld\n", (long)i);
    }
    plg_manager->nb_handle = 0;
    if( plg_manager->directory_path) {
        free(plg_manager->directory_path);
        plg_manager->directory_path = NULL;
    }
}

void plugin_manager_free() /*plugin_mgr_t *plg_manager)*/
{
    if( singleton == NULL)
        return;
    
    /* clear content / reset */
    plugin_manager_clear(singleton);
    free(singleton);
    singleton = NULL;
}

const char* plugin_manager_get_directory( const plugin_mgr_t *manager)
{
    return manager->directory_path;   
}

void plugin_manager_set_directory(plugin_mgr_t *plg_manager, const char * directory)
{
    const char *local_dir;

    if( plg_manager->directory_path != NULL)
        free(plg_manager->directory_path);

    /* assign current directory if null provided, default */
    if(directory == NULL)
        local_dir = CURRENT_DIRECTORY;
    else 
        local_dir = directory;

    plg_manager->directory_path = malloc( strlen(local_dir) + 1 );
    assert( plg_manager->directory_path );
    strcpy( plg_manager->directory_path, local_dir);
}

int plugin_manager_load_shared_library(plugin_mgr_t *plg_manager, const char *filename)
{
    HINSTANCE handle;
    char *fullname;
    bool b_delete_dir = false; /* flag if need deallocation of dir */
    char *dir = plg_manager->directory_path;
    /* if directory not set, use current directory.
       Pssible case ? */
    if( !dir ) {
        dir = malloc( 5 );
        strcpy( dir, ".//");
        b_delete_dir = true;
        CLOG_DEBUG("Search library in current directory %s\n", dir);
    } else {
        dir = plg_manager->directory_path;
    }
    CLOG_DEBUG("Load shared_library, dir= %s, name= %s\n", dir, filename);

    /* concat filename, can be a function and cross platform if dir is empty */
    fullname = malloc( strlen(dir) + strlen(filename) + 3 /* x1 */);
    strcpy(fullname, dir);
    strcat(fullname, "//" );
    strcat(fullname, filename);

    CLOG_DEBUG("Load game %s\n", fullname);

    /* FreeLibrary(handle) to call to be clean, but after use !! crash.. 
       Need encapsulation to keep handle valid */
    handle = LoadLibrary(fullname); /*"libmorpion.dll");*/
    if( !handle)
    {
        CLOG_ERR("Error loading library %s\n", fullname);
        return PLG_MANAGER_DLL_NOT_FOUND;
    }

    assert( plg_manager->nb_handle < MAX_NB_SHARED_LIB-1 );
    plg_manager->handle_dll[plg_manager->nb_handle] = handle;
    plg_manager->nb_handle++;

    /* free local memory */
    free(fullname);
    /* dir more tricky to delete */
    if( b_delete_dir )
        free(dir);
    return PLG_MANAGER_OK;
}


/*void (*)(void *) get_funct_from_last_inserted(const char *name)*/
/*void  (*get_funct_from_last_inserted(const char *name)(void*)*/
ptr_plugin_funct plugin_manager_get_game_ptrf( const plugin_mgr_t* manager, const char *name)
{
    HINSTANCE current_handle;
    FARPROC fptr; /* typedef INT_PTR (*FARPROC)() , INT_PTR long long*/

    ptr_plugin_funct funct_casted;

    current_handle = manager->handle_dll[manager->nb_handle-1];
    assert( current_handle != NULL );

    fptr = GetProcAddress(current_handle, name);
    if( fptr == (FARPROC)NULL ) 
    {
        CLOG_ERR("Error cannot find function %s in library", name);
        printf("Error cannot find function %s in library", name);
        return NULL;
    }
    /* only GCC : error: cast between incompatible function types from 'FARPROC' {aka 'long long int (*)()'} to */
    /* if add (void*) pedantic error throw */
    
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wcast-function-type"
    funct_casted = (ptr_plugin_funct) fptr;
    /* #pragma GCC diagnostic ignored "-Wpedantic"
    pf_game = (ptr_game_t) (void*) fptr; */
    #pragma GCC diagnostic pop
    
    /* printf("handle: %p\n", (long *)fptr); */
    /*printf("record in handle2 nb_element=%d",nb_element);*/

    return funct_casted;
}