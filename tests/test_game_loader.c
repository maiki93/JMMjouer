
/* import for CMocka */
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>

#include "cmocka.h"

#include "../game_loader.c"

game_loader_t * gload;

static int setup(void **state) {
    (void) state;
    gload = game_loader_new();
    return 0;
}

/* clear data and delete the instance */
static int teardown(void **state) {
    (void) state;
    game_loader_delete( gload );
    gload = NULL;
    return 0;
}

static void initialization_on_heap() 
{    
    // constructor, pass a plugin_mamnger_t (or mock ? would be nice)
    game_loader_init( gload );
    // check, the size 
    assert_int_equal( 0, game_ptrf_size(gload->map_game) );
}

/* static void init_with_manager() {
    plugin_mgr_t *manager = plugin_manager_new();
    game_loader_init( manager );
    game_loader_set_dll_directory("tests_plugins");

    assert_non_null( manager );
    assert_string_equal("tests_plugins", 
                        game_loader_get_dll_directory());
    game_loader_clear();
    //assert_null(manager);
}
*/

/* static void init_with_manager_default_dir() 
{
    plugin_mgr_t *manager = plugin_manager_new();
    game_loader_init( manager );
    
    assert_string_equal(".", game_loader_get_dll_directory());
}
*/

/* test private functions */
static void empty_and_add_one_element(void **state) {
    (void) state;
    int ret_value;
    game_loader_init( gload );
    // private function 
    ret_value = add_game( gload, "toto", (ptr_game_t) 0x10 );

    assert_int_equal( ret_value, GAME_LOADER_OK);
    assert_int_equal( 1, game_ptrf_size(gload->map_game) );
    assert_int_equal( false, is_empty(gload) );
}

/* test public API */
static void get_array_2_names(void **state) {
    (void) state;
    // to send to function
    size_t size_list,i;
    char **name = NULL;
    int ret_value;
    game_loader_init( gload );
    ret_value = add_game( gload, "toto", (ptr_game_t) 0x10 );
    ret_value = add_game( gload, "titi2", (ptr_game_t) 0x20 );

    game_loader_get_array_names( gload, &name, &size_list);
    assert_int_equal( GAME_LOADER_OK, ret_value );
    assert_int_equal( 2, game_ptrf_size(gload->map_game));  // compare to size of game_loader array directly
    //assert_int_equal( size_list, size());

    assert_int_equal( 4, strlen(name[0]));
    assert_string_equal( "toto", name[0] );
    assert_string_equal( "titi2", name[1] );
    // name to delete !!
    for( i = 0; i < size_list; i++ )
        free(name[i]);
    free(name);
}

static void get_clist_cstring_2_names() {

    clist_cstring_t *clist;
    char *name_game_out;
    
    game_loader_init( gload );
    add_game( gload, "toto", (ptr_game_t) 0x10 );
    add_game( gload, "titi2", (ptr_game_t) 0x20 );

    // return pointer directly, could send error_code in parameter
    clist = game_loader_get_names( gload );
    assert_non_null(clist);
    assert_int_equal( 2, game_ptrf_size(gload->map_game));  // compare to size of game_loader array directly
    assert_int_equal( 2, clist_cstring_size( clist) );

    clist_cstring_get_ref( clist, 0, &name_game_out );
    assert_int_equal( 4, strlen(name_game_out));
    assert_string_equal( "toto", name_game_out );
    clist_cstring_get_ref( clist, 1, &name_game_out );
    assert_string_equal( "titi2", name_game_out );

    clist_cstring_delete( clist );
}



static void get_ptr_games_from_name() 
{
    //int retour;
    game_loader_init( gload );
    add_game( gload, "toto", (ptr_game_t) 0x10 );
    add_game( gload, "titi2", (ptr_game_t) 0x20 );

    ptr_game_t pfgame1 = game_loader_get_ptr_game( gload, "toto" );
    ptr_game_t pfgame2 = game_loader_get_ptr_game( gload, "titi2" );

    assert_non_null( pfgame1 );
    assert_non_null( pfgame2 );
    /* not working on linux, same version ?? */
    /*
    assert_uint_equal( 0x10, pfgame1 );
    assert_uint_equal( 0x20, pfgame2 );
    */
    assert_int_equal( 0x10, pfgame1 );
    assert_int_equal( 0x20, pfgame2 );
}

/* static void get_ptr_games_from_indices() {
    add_game("toto", (ptr_game_t) 0x10 );
    add_game("titi2", (ptr_game_t) 0x20 );

    ptr_game_t pfgame1 = game_loader_get_ptr_game( 0 );
    ptr_game_t pfgame2 = game_loader_get_ptr_game( 1 );

    assert_non_null( pfgame1 );
    assert_non_null( pfgame2 );
    assert_uint_equal( 0x10, pfgame1 );
    assert_uint_equal( 0x20, pfgame2 );
}
*/

static void load_static_and_shared_libraries() {
    int retour;
    plugin_manager_set_directory( plugin_manager_get_instance(), "test_plugins");
    game_loader_init( gload );
    
    retour = game_loader_load_all( gload );
    assert_int_equal(0, retour);
    assert_int_equal(3,  game_ptrf_size(gload->map_game) );
    plugin_manager_free();
}

static void list_plugins_functional()
{
    int retour;
    plugin_manager_set_directory( plugin_manager_get_instance(), "test_plugins");
    game_loader_init( gload );
    retour = for_files_with_extension("test_plugins", "so", //"dll", //"lib", 
                                      &load_game_dll_callback, 
                                      (clist_gen_t *)gload->map_game);

    assert_int_equal( 0, retour);
    assert_int_equal( 1, game_ptrf_size(gload->map_game) );
    plugin_manager_free();
}

/****************
    Main fonction
****/
int main()
{
    /* can be inside main, or as global. here no problem with identical name */
    const struct CMUnitTest tests_game_loader[] = {
        cmocka_unit_test_setup_teardown(initialization_on_heap, setup, teardown),
        cmocka_unit_test_setup_teardown(empty_and_add_one_element, setup, teardown),
        cmocka_unit_test_setup_teardown(get_array_2_names, setup, teardown),
        cmocka_unit_test_setup_teardown(get_clist_cstring_2_names, setup, teardown),
        cmocka_unit_test_setup_teardown(get_ptr_games_from_name,setup, teardown),
        //cmocka_unit_test_setup(get_ptr_games_from_indices,setup),
        cmocka_unit_test_setup_teardown(load_static_and_shared_libraries, setup, teardown),
        cmocka_unit_test_setup_teardown(list_plugins_functional,setup, teardown),
    };

    /* call group_setup and teardown at the very beginning and end */
    /* return cmocka_run_group_tests(tests_historique, group_setup, group_teardown);*/
    return cmocka_run_group_tests(tests_game_loader, NULL, NULL);
}
