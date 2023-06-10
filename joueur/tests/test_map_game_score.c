/* import for CMocka */
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include "cmocka.h"

#include "joueur/map_game_score.c"

/* common data, pair_game_score_t do not use dynamic allocation
    no explicit free needed  */
struct pair_game_score_t pair_in1 = {"first",  /* game_name */
                                       {1, 0, 2} /* score_t:win, lost, equal*/
                                     };
struct pair_game_score_t pair_in2 = {"second",  /* game_name */
                                       {4, 5, 6} /* score_t:win, lost, equal*/
                                     };
struct  pair_game_score_t pair_in3 = {"three",
                                        {0, 1, 2}
                                       };

struct pair_game_score_t pair_out; /* fetch */

//  helper functions //
static void pair_score_to_value()
{
    ccontainer_err_t err_code;
    ccontainer_value_t value_out;

    value_out = make_value_pair_score( &pair_in1, &err_code );
    assert_int_equal(CCONTAINER_OK, err_code);
    assert_string_equal("first", value_out.data);

    // default deleter when serializing pair_victory_t
    ccontainer_delete_value(&value_out);
}

static void value_to_pair_score()
{
    ccontainer_err_t err_code;
    ccontainer_value_t value_out;

    value_out = make_value_pair_score( &pair_in1, &err_code );
    assert_int_equal(CCONTAINER_OK, err_code);
    err_code = extract_value_pair_score( &value_out, &pair_out );
    assert_int_equal(CCONTAINER_OK, err_code);

    assert_string_equal("first", pair_out.game_name);
    assert_int_equal(1, pair_out.score.nb_win);
    assert_int_equal(0, pair_out.score.nb_lost);
    assert_int_equal(2, pair_out.score.nb_equality);

    ccontainer_delete_value(&value_out);
}

static void construction_map()
{
    map_game_score_t map_on_stack;
    map_game_score_t *map_on_heap;

    map_game_score_init(&map_on_stack);
    
    map_on_heap = map_game_score_new();
    map_game_score_init( map_on_heap );

    map_game_score_delete( &map_on_stack );
    map_game_score_free( map_on_heap );
    map_on_heap = NULL;
}

static void copy_map()
{
    ccontainer_err_t err_code;
    map_game_score_t map_victory, map_victory_copy;
    map_game_score_init( &map_victory );
    
    err_code = map_game_score_insert( &map_victory, &pair_in1 );
    assert_int_equal( CCONTAINER_OK, err_code );
    err_code = map_game_score_insert( &map_victory, &pair_in2 );
    assert_int_equal( CCONTAINER_OK, err_code );

    map_victory_copy = map_game_score_copy( &map_victory, &err_code);
    assert_int_equal( CCONTAINER_OK, err_code );
    // suppress original
    map_game_score_delete( &map_victory );
    // need to test / get_victories 

    // suppress copy
    map_game_score_delete( &map_victory_copy );
}

static void insert_new_victory()
{
    ccontainer_err_t err_code;
    map_game_score_t map_victory;
    map_game_score_init( &map_victory );
    
    err_code = map_game_score_insert( &map_victory, &pair_in1 );
    assert_int_equal( CCONTAINER_OK, err_code );

    pair_out = map_game_score_from_name( &map_victory, "first");
    assert_string_equal("first", pair_out.game_name);
    assert_int_equal(1, pair_out.score.nb_win);
    assert_int_equal(0, pair_out.score.nb_lost);
    assert_int_equal(2, pair_out.score.nb_equality);

    map_game_score_delete( &map_victory );
}

static void find_not_existing_entry()
{
    map_game_score_t map_victory;
    map_game_score_init( &map_victory );
    
    // find with an empty list
    pair_out = map_game_score_from_name( &map_victory, "toto");
    assert_string_equal("invalid", pair_out.game_name);

    // when make an insertion
    map_game_score_insert( &map_victory, &pair_in1 );
    pair_out = map_game_score_from_name( &map_victory, "toto");
    assert_string_equal("invalid", pair_out.game_name);

    map_game_score_delete( &map_victory );
}

static void find_existing_entry()
{
    map_game_score_t map_victory;
    map_game_score_init( &map_victory );
    
    map_game_score_insert( &map_victory, &pair_in1 );
    map_game_score_insert( &map_victory, &pair_in2 );

    // find the second one
    pair_out = map_game_score_from_name( &map_victory, "second");
    assert_string_equal("second", pair_out.game_name);
    // find the first one
    pair_out = map_game_score_from_name( &map_victory, "first");
    assert_string_equal("first", pair_out.game_name);

    map_game_score_delete( &map_victory );
}

int main()
{
    /* can be inside main, or as global. here no problem with identical name */
    const struct CMUnitTest tests_map_game_score[] = {
        cmocka_unit_test(pair_score_to_value),
        cmocka_unit_test(value_to_pair_score),
        cmocka_unit_test(construction_map),
        cmocka_unit_test(copy_map),
        cmocka_unit_test(insert_new_victory),
        cmocka_unit_test(find_not_existing_entry),
        cmocka_unit_test(find_existing_entry),
    };
    /* call group_setup and teardown at the very beginning and end */
    /* return cmocka_run_group_tests(tests_historique, group_setup, group_teardown);*/
    return cmocka_run_group_tests(tests_map_game_score, NULL, NULL);
}