#include "plugin_manager.h"

#if defined(_WIN32)
    #include <windows.h>
#else
    #include <dlfcn.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "clogger/clogger.h"

/** Maximum number of shared libraries */
enum { MAX_NB_SHARED_LIB = 5};

/** Default value */
static const char* CURRENT_DIRECTORY = ".";

/* https://helpercode.com/2015/09/16/how-to-fake-a-singleton-in-c/ */
/* https://stackoverflow.com/questions/9191530/how-to-unit-test-singleton-class-c */
/* general advice (C++) split implementation behind a class wrapper, allow test/mock... */
/* or atexit
    https://stackoverflow.com/questions/39385520/do-i-have-to-free-static-dynamically-allocated-pointers */

/* declaration of the structure */
struct plugin_manager {
    #if defined(_WIN32)
        HINSTANCE handle_dll[MAX_NB_SHARED_LIB]; /* INVALID_HANDLE_VALUE */
    #else
        void* handle_dll[MAX_NB_SHARED_LIB];
    #endif
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
        #if defined(_WIN32)
            FreeLibrary(plg_manager->handle_dll[i]);
        #else
            dlclose(plg_manager->handle_dll[i]);
        #endif
        plg_manager->handle_dll[i] = NULL;
        CLOG_DEBUG("Remove shared lib from handle %ld\n", (long)i);
    }
    plg_manager->nb_handle = 0;
    if( plg_manager->directory_path) {
        free(plg_manager->directory_path);
        plg_manager->directory_path = NULL;
    }
}

void plugin_manager_free()
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

/* strcmp if already correct */
int plugin_manager_set_directory(plugin_mgr_t *plg_manager, const char * directory)
{
    const char *local_dir;

    /* really need to delete all the time ? , compare ? better */
    if( plg_manager->directory_path != NULL)
        free(plg_manager->directory_path);

    /* assign current directory if null provided, default */
    if(directory == NULL)
        local_dir = CURRENT_DIRECTORY;
    else 
        local_dir = directory;

    /* need tmp_buffer */
    plg_manager->directory_path = malloc( strlen(local_dir) + 1 );
    if( !plg_manager->directory_path )
        return -1;
    /*assert( plg_manager->directory_path );*/
    strcpy( plg_manager->directory_path, local_dir);
    return 0;
}

/* very windows dependent, or maybe not so much in fact */
int plugin_manager_load_shared_library(plugin_mgr_t *plg_manager, const char *filename)
{
#if defined(_WIN32)
    HINSTANCE handle;
#else
    void* handle;
#endif

    char *fullname;
    bool b_delete_dir = false; /* flag if need deallocation of dir */
    char *dir = plg_manager->directory_path;
    /* if directory not set, use current directory. Possible ?, not "." by default ? */
    if( !dir ) {
        assert(true); /*never happens with default */
        dir = malloc( 5 );
        strcpy( dir, ".//");
        b_delete_dir = true;
        CLOG_DEBUG("Search library in current directory %s\n", dir);
    } else {
        dir = plg_manager->directory_path;
    }
    CLOG_DEBUG("Load shared_library, dir= %s, name= %s\n", dir, filename);

    /* concat filename, can be a function and cross platform if dir is empty 
        works for all tested platform (not a real linux, only wsl2) */
    fullname = malloc( strlen(dir) + strlen(filename) + 3);
    strcpy(fullname, dir);
    strcat(fullname, "//" );
    strcat(fullname, filename);
    CLOG_DEBUG("Load game %s\n", fullname);

    /* FreeLibrary(handle) to call to be clean, but after use !! crash.. 
       Need encapsulation to keep handle valid */
#if defined(_WIN32)
    handle = LoadLibrary(fullname);
#else
    handle = dlopen(fullname, RTLD_LAZY);
#endif
    if( !handle)
    {
        CLOG_ERR("Error loading library %s\n", fullname);
        /* no memory leak, but bad duplication, better to use local varirable */
        free(fullname);
        if( b_delete_dir )
            free(dir);
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

int plugin_manager_get_names(const plugin_mgr_t *plg_manager, char **name_game_out, 
                             char **name_start_fct_out)
{
#if defined(_WIN32)
    HINSTANCE current_handle;
    FARPROC fptr_name_game; /* typedef INT_PTR (*FARPROC)() , INT_PTR long long*/
    FARPROC fptr_name_start_fct; /* for the name */
#else
    void* current_handle;
    void* fptr_name_game;
    void *fptr_name_start_fct;
#endif

    fptr_name_game =NULL;
    fptr_name_start_fct = NULL;
    *name_game_out = NULL;
    *name_start_fct_out = NULL;

    current_handle = plg_manager->handle_dll[plg_manager->nb_handle-1];
    assert( current_handle != NULL );

#if defined(_WIN32)
    fptr_name_game = GetProcAddress(current_handle, "");
    if( fptr == (FARPROC)NULL ) 
    {
        CLOG_ERR("Error cannot find function %s in library", name);
        return -1;
    }
#else
    fptr_name_game = dlsym( current_handle, "name_game_plugin");
    if( fptr_name_game == NULL) {
        CLOG_ERR("Error cannot find variable \"name_game_plugin\" in the plugin %d", 0);
        return -1;
    }
#endif

    *name_game_out = (char*) malloc( strlen((char*)fptr_name_game) + 1);
    strcpy( *name_game_out, (char*)fptr_name_game);

    /* name function to call */
#if defined(_WIN32)
    fptr_name_run_fct = GetProcAddress(current_handle, "name_start_fct");
    if( fptr == (FARPROC)NULL ) 
    {
        CLOG_ERR("Error cannot find function %s in library", name);
        return -1;
    }
#else
    fptr_name_start_fct = dlsym( current_handle, "name_start_fct");
    if( fptr_name_start_fct == NULL) {
        CLOG_ERR("Error cannot find variable \"name_start_fct\" in the plugin %d", 0);
        return -1;
    }
#endif

    *name_start_fct_out = (char*) malloc( strlen((char*)fptr_name_start_fct) + 1);
    strcpy( *name_start_fct_out, (char*)fptr_name_start_fct);
    return 0;
}


ptr_plugin_funct plugin_manager_get_game_ptrf( const plugin_mgr_t* manager, const char *name_start_fct)
{
#if defined(_WIN32)
    HINSTANCE current_handle;
    FARPROC fptr; /* typedef INT_PTR (*FARPROC)() , INT_PTR long long*/
#else
    void* current_handle;
    void* fptr;
#endif

    ptr_plugin_funct funct_casted;
    /* current is always the last one */
    current_handle = manager->handle_dll[manager->nb_handle-1];
    assert( current_handle != NULL );

#if defined(_WIN32)
    fptr = GetProcAddress(current_handle, name_start_fct);
    if( fptr == (FARPROC)NULL ) 
    {
        CLOG_ERR("Error cannot find function %s in library", name_start_fct);
        return NULL;
    }
#else
    fptr = dlsym( current_handle, name_start_fct);
    if( fptr == NULL) {
        CLOG_ERR("Error cannot find function %s in linux library", name_start_fct);
        return NULL;
    }
#endif

    /* only GCC : error: cast between incompatible function types from 'FARPROC' {aka 'long long int (*)()'} to */
    /* if add (void*) pedantic error throw */ /* and on linux ?? */
#if defined(__GNUC__) || defined(__GNUG__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wcast-function-type" /* mingw64 (x86_64-mingw32 (not 64 bits!) ) warning -Wcast-function-type */
    #pragma GCC diagnostic ignored "-Wpedantic" /* on linux, previous one is not enought */
#endif

    funct_casted = (ptr_plugin_funct) fptr;

#if defined(__GNUC__) || defined(__GNUG__)
    #pragma GCC diagnostic pop
#endif

    /*CLOG_DEBUG("handle: %p\n", (long *)funct_casted);*/
    CLOG_DEBUG("record in handle nb_element=%d",manager->nb_handle);

    return funct_casted;
}

