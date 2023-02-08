
// import for CMocka
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>

#include <cmocka.h>

/* test internal in including the implementation */
#include "../plugin_manager.c"

/* constante for tests */
const char* DIR_PLUGINS = "test_plugins";
/* coud use only specific extension + macro to create the name?*/
#if defined(_WIN32)
    const char* NAME_DLL_MORPION = "libgame_morpion.dll";
#else
    const char* NAME_DLL_MORPION = "libgame_morpion.so";
#endif

const char* START_GAME_FUNCTION = "start_game_morpion";

/* because inclusion of *c file, can check private data */
static void get_instance_manager()
{
    /* get_instance() only way to access the one instance,
        will allocate and default init the instance.
        Must not be freed !!  */
    plugin_mgr_t *manager = plugin_manager_get_instance();
    assert_int_equal(0, manager->nb_handle);
    assert_null(manager->handle_dll[0]);
    assert_null(manager->handle_dll[MAX_NB_SHARED_LIB-1]);
    assert_string_equal(CURRENT_DIRECTORY, manager->directory_path);

    // clean properly the singleton, internal singleton set to NULL
    plugin_manager_free();
    // because access to *.c implementation
    assert_null( singleton );
}

static void set_directory_dll()
{
    plugin_mgr_t *manager = plugin_manager_get_instance();
    assert_string_equal(".", plugin_manager_get_directory( manager ));

    plugin_manager_set_directory(manager,"tata");
    assert_string_equal("tata", plugin_manager_get_directory( manager ));

    plugin_manager_free();
}

static void load_one_existing_dll()
{
    int retour;
    plugin_mgr_t *manager = plugin_manager_get_instance();
    plugin_manager_set_directory( manager, DIR_PLUGINS );

    retour = plugin_manager_load_shared_library(manager, NAME_DLL_MORPION);
    assert_int_equal(PLG_MANAGER_OK, retour);
    // bacause internal access
    assert_int_equal( 1, singleton->nb_handle);

    plugin_manager_free();
}

static void retrieve_function_from_dll()
{
    ptr_plugin_funct pf_game = NULL;
    plugin_mgr_t *manager = plugin_manager_get_instance();
    plugin_manager_set_directory( manager, DIR_PLUGINS );
    plugin_manager_load_shared_library( manager, NAME_DLL_MORPION);
    
    pf_game = plugin_manager_get_game_ptrf( manager, START_GAME_FUNCTION);
    assert_non_null( pf_game );

    plugin_manager_free();
}

static void shared_library_does_not_exist()
{
    int retour;
    plugin_mgr_t *manager = plugin_manager_get_instance();
    plugin_manager_set_directory( manager, "test_plugins");

    retour = plugin_manager_load_shared_library( manager, "not_existing.dll" );
    assert_int_equal(PLG_MANAGER_DLL_NOT_FOUND, retour);

    plugin_manager_free();
}

static void function_does_not_exist()
{
    ptr_plugin_funct pf_game = NULL;
    plugin_mgr_t *manager = plugin_manager_get_instance();
    plugin_manager_set_directory( manager, "test_plugins");
    plugin_manager_load_shared_library( manager, NAME_DLL_MORPION );

    /* api cannot use PLG_MANAGER_FCT_NOT_FOUUND */
    pf_game = plugin_manager_get_game_ptrf( manager, "start_game_titi");
    assert_null( pf_game );

    plugin_manager_free();
}


/****************
    Main fonction
****/
int main()
{
    // this can be moved, maybe shared amongst *c file ? seems not so easy. not expected it seems
    const struct CMUnitTest tests[] = {
        
        cmocka_unit_test(get_instance_manager),
        cmocka_unit_test(set_directory_dll),
        cmocka_unit_test(load_one_existing_dll),
        cmocka_unit_test(retrieve_function_from_dll),
        cmocka_unit_test(shared_library_does_not_exist),
        cmocka_unit_test(function_does_not_exist),
    };

    // if singleton may need a reset 
    return cmocka_run_group_tests(tests, NULL, NULL);
}