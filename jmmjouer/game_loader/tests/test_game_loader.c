/* import for CMocka */
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>

#include "cmocka.h"

#include "../game_loader.c"

/* constante for tests */
const char* DIR_PLUGINS = "test_plugins";
/* constant defined inside the plugin */
const char* NAME_GAME = "Morpion";
const char* START_GAME_FUNCTION = "start_game_morpion";

game_loader_t * gload;

static int group_setup(void **state)
{
    (void) state;
    init_clogger("test_game_loader.log");
    return 0;
}

static int group_teardown(void **state)
{
    (void) state;
    close_clogger();
    return 0;
}

/* creation and initilaization of an empty game_loader */
static int setup(void **state) {
    (void) state;
    gload = game_loader_new();
    game_loader_init( gload );
    assert_int_equal( 0, game_loader_size( gload ));
    return 0;
}

/* clear data and delete the instance */
static int teardown(void **state) {
    (void) state;
    game_loader_free( gload );
    gload = NULL;
    return 0;
}

/* test private functions */
static void add_one_element(void **state) {
    (void) state;
    int ret_value;
    // private function 
    ret_value = add_game( gload, "toto", (ptr_game_t) 0x10 );

    assert_int_equal( ret_value, GAME_LOADER_OK);
    assert_int_equal( 1, game_loader_size(gload) );
    assert_int_equal( false, is_empty(gload) ); // private function
}

/* test public API */
static void get_array_2_names(void **state) {
    (void) state;
    // to send to function
    size_t size_list,i;
    char **name = NULL;
    int ret_value;
    
    ret_value = add_game( gload, "toto", (ptr_game_t) 0x10 );
    ret_value = add_game( gload, "titi2", (ptr_game_t) 0x20 );

    game_loader_get_array_names( gload, &name, &size_list);
    assert_int_equal( GAME_LOADER_OK, ret_value );
    assert_int_equal( 2, game_ptrf_size(gload->map_game));  // compare to size of game_loader array directly
    
    assert_int_equal( 4, strlen(name[0]));
    assert_string_equal( "toto", name[0] );
    assert_string_equal( "titi2", name[1] );
    // name to delete, ccontainer_utlis available
    for( i = 0; i < size_list; i++ )
        free(name[i]);
    free(name);
}

static void get_clist_cstring_2_names() 
{
    ccontainer_err_t err_code;
    cvector_cstring_t vect_cstring;
    // ref to names
    const char *name_game_out;
    
    add_game( gload, "toto", (ptr_game_t) 0x10 );
    add_game( gload, "titi2", (ptr_game_t) 0x20 );

    // return pointer directly, could send error_code in parameter
    vect_cstring = game_loader_get_names( gload );
    assert_int_equal( 2, cvector_cstring_size( &vect_cstring) );
    assert_int_equal( 2, game_ptrf_size(gload->map_game));  // compare to size of game_loader array directly

    name_game_out = cvector_cstring_get_ref_at( &vect_cstring, 0, &err_code );
    assert_int_equal( err_code, CCONTAINER_OK);
    assert_string_equal( "toto", name_game_out );
    name_game_out = cvector_cstring_get_ref_at( &vect_cstring, 1, &err_code );
    assert_int_equal( err_code, CCONTAINER_OK);
    assert_string_equal( "titi2", name_game_out );

    cvector_cstring_delete( &vect_cstring );
}

static void get_ptr_games_from_name() 
{
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

static void load_static_and_shared_libraries() {
    int retour;
    plugin_manager_set_directory( plugin_manager_get_instance(), DIR_PLUGINS);
    retour = game_loader_load_all( gload );
    assert_int_equal(0, retour);
    assert_int_equal(3,  game_ptrf_size(gload->map_game) );
    plugin_manager_free();
}

static void list_plugins_functional()
{
    int retour;
    plugin_manager_set_directory( plugin_manager_get_instance(), DIR_PLUGINS);
    retour = for_files_with_extension(DIR_PLUGINS, EXT_DLL,
                                      &load_game_dll_callback, 
                                      (clist_gen_t *)gload->map_game);

    assert_int_equal( 0, retour);
    assert_int_equal( 1, game_loader_size(gload) );
    plugin_manager_free();
}

/****************
    Main fonction
****/
int main()
{
    /* can be inside main, or as global. here no problem with identical name */
    const struct CMUnitTest tests_game_loader[] = {
        cmocka_unit_test_setup_teardown(add_one_element, setup, teardown),
        cmocka_unit_test_setup_teardown(get_array_2_names, setup, teardown),
        cmocka_unit_test_setup_teardown(get_clist_cstring_2_names, setup, teardown),
        cmocka_unit_test_setup_teardown(get_ptr_games_from_name,setup, teardown),
        cmocka_unit_test_setup_teardown(load_static_and_shared_libraries, setup, teardown),
        cmocka_unit_test_setup_teardown(list_plugins_functional,setup, teardown),
    };

    /* call group_setup and teardown at the very beginning and end */
    return cmocka_run_group_tests(tests_game_loader, group_setup, group_teardown);
}
