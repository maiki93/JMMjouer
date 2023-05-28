/* import for CMocka */
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include "cmocka.h"

#include "joueur/map_game_victories.c"

/* common data, pair_game_victory_t do not use dynamic allocation
    no explicit free needed  */
struct pair_game_victory_t pair_in1 = {"first",  /* game_name */
                                       {1, 0, 2} /* victory_t:win, lost, equal*/
                                     };
struct pair_game_victory_t pair_in2 = {"second",  /* game_name */
                                       {4, 5, 6} /* victory_t:win, lost, equal*/
                                     };
struct pair_game_victory_t pair_out; /* fetch */

static void pair_victory_to_value()
{
    ccontainer_err_t err_code;
    ccontainer_value_t value_out;

    value_out = make_value_pair_victory( &pair_in1, &err_code );
    assert_int_equal(CCONTAINER_OK, err_code);
    assert_string_equal("first", value_out.data);

    // default deleter when serializing pair_victory_t
    ccontainer_delete_value(&value_out);
}

static void value_to_pair_victory()
{
    ccontainer_err_t err_code;
    ccontainer_value_t value_out;

    value_out = make_value_pair_victory( &pair_in1, &err_code );
    assert_int_equal(CCONTAINER_OK, err_code);
    err_code = extract_value_pair_victory( &value_out, &pair_out );
    assert_int_equal(CCONTAINER_OK, err_code);

    assert_string_equal("first", pair_out.game_name);
    assert_int_equal(1, pair_out.victories.nb_win);
    assert_int_equal(0, pair_out.victories.nb_lost);
    assert_int_equal(2, pair_out.victories.nb_equality);

    ccontainer_delete_value(&value_out);
}

static void construction_cmap()
{
    map_game_victories_t map_on_stack;
    map_game_victories_t *map_on_heap;

    game_victories_init(&map_on_stack);
    
    map_on_heap = game_victories_new();
    game_victories_init( map_on_heap );

    game_victories_delete( &map_on_stack );
    game_victories_free( map_on_heap );
    map_on_heap = NULL;
}



static void insert_new_victory()
{
    ccontainer_err_t err_code;
    map_game_victories_t map_victory;
    game_victories_init( &map_victory );
    
    err_code = game_victories_insert( &map_victory, pair_in1 );
    assert_int_equal( CCONTAINER_OK, err_code );

    pair_out = game_victories_get_copy( &map_victory, "first");
    assert_string_equal("first", pair_out.game_name);
    assert_int_equal(1, pair_out.victories.nb_win);
    assert_int_equal(0, pair_out.victories.nb_lost);
    assert_int_equal(2, pair_out.victories.nb_equality);

    game_victories_delete( &map_victory );
}

static void copy_cmap()
{
    ccontainer_err_t err_code;
    map_game_victories_t map_victory, map_victory_copy;
    game_victories_init( &map_victory );
    
    err_code = game_victories_insert( &map_victory, pair_in1 );
    assert_int_equal( CCONTAINER_OK, err_code );

    map_victory_copy = game_victories_copy( &map_victory, &err_code);
    assert_int_equal( CCONTAINER_OK, err_code );
    // suppress original
    game_victories_delete( &map_victory );
    // need to test / get_victories 

    // suppress copy
    game_victories_delete( &map_victory );
    
}

static void find_not_existing_entry()
{
    map_game_victories_t map_victory;
    game_victories_init( &map_victory );
    
    // find with an empty list
    pair_out = game_victories_get_copy( &map_victory, "toto");
    assert_string_equal("invalid", pair_out.game_name);

    // when make an insertion
    game_victories_insert( &map_victory, pair_in1 );
    pair_out = game_victories_get_copy( &map_victory, "toto");
    assert_string_equal("invalid", pair_out.game_name);

    game_victories_delete( &map_victory );
}

int main()
{
    /* can be inside main, or as global. here no problem with identical name */
    const struct CMUnitTest tests_map_game_victories[] = {
        cmocka_unit_test(pair_victory_to_value),
        cmocka_unit_test(value_to_pair_victory),
        cmocka_unit_test(construction_cmap),
        cmocka_unit_test(insert_new_victory),
        cmocka_unit_test(find_not_existing_entry),
    };
    /* call group_setup and teardown at the very beginning and end */
    /* return cmocka_run_group_tests(tests_historique, group_setup, group_teardown);*/
    return cmocka_run_group_tests(tests_map_game_victories, NULL, NULL);
}