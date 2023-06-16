/* import for CMocka */
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include "cmocka.h"

/* include implementation for access to pair_game_score_t */
#include "joueur/map_game_score.c"
/* or split test_impl_map_game_score if too long */

/* value object, only on stack.
   pair will stay in implementation, not visible in public API */
struct pair_game_score_t pair_in1 = {"first",  /* game_name */
                                       {1, 0, 2} /* score_t:win, lost, equal*/
                                     };
struct pair_game_score_t pair_in2 = {"second",
                                       {4, 5, 6}
                                     };
struct  pair_game_score_t pair_in3 = {"three",
                                        {0, 1, 2}
                                     };

/* equivalent with API without pair, test really the public API */
const char* name_game1 = "first";
score_game_t score1 = {1, 0, 2};

const char* name_game2 = "second";
score_game_t score2 = {4, 5, 6};

const char* name_game3 = "three";
score_game_t score3 = {0, 1, 2};
// fourt equal three + 1 win
const char* name_game4 = "fourth";
score_game_t score4 = {1, 1, 2};
// default score
score_game_t score_default = {0, 0, 0};

/* extracted value */
struct pair_game_score_t pair_out;
score_game_t score_out;

/* for tests, may include in the structure if needed 
    comparison will be at a point */
static bool score_equal(score_game_t score1, score_game_t score2);

//  helper functions //
static void pair_score_to_value()
{
    ccontainer_err_t err_code;
    ccontainer_value_t value_out;

    value_out = make_value_pair_score( &pair_in1, &err_code );
    assert_int_equal(CCONTAINER_OK, err_code);
    assert_string_equal("first", value_out.data);
    /* test memory bytes */
    assert_memory_equal( &score1, value_out.data + MAX_NAME_GAME_LEN, sizeof(score_game_t) );

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
    /* equality for value object make sense */
    assert_int_equal(1, pair_out.score.nb_win);
    assert_int_equal(0, pair_out.score.nb_lost);
    assert_int_equal(2, pair_out.score.nb_draw);
    /* same check with new function */
    assert_true( score_equal( pair_out.score, score1) );

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

static void insert_and_replace_new_score()
{
    map_game_score_t map_victory;
    int status;
    
    map_game_score_init( &map_victory );
    
    // insert one
    status = map_game_score_insert( &map_victory, name_game1, &score1 );
    assert_int_equal( 0, status ); /* check 0 for insertion new one */
    assert_int_equal(1, map_game_score_size( &map_victory));

    status = map_game_score_get_from_name( &map_victory, "first", &score_out );
    assert_true( score_equal( score1, score_out) );
    
    // insert the same name => replace values
    status = map_game_score_insert( &map_victory, name_game1, &score1 );
    assert_int_equal( 1, status ); /* check 1 for replacement */
    assert_int_equal(1, map_game_score_size( &map_victory));

    map_game_score_delete( &map_victory );
}

static void get_from_name()
{
    map_game_score_t map_victory;
    int status;
    map_game_score_init( &map_victory );

    // get with an empty list
    status = map_game_score_get_from_name( &map_victory, "toto", &score_out);
    assert_int_equal( -1, status );
    assert_true( score_equal(score_default, score_out) );
    // insert 2 elements
    map_game_score_insert( &map_victory, name_game1, &score1 );
    map_game_score_insert( &map_victory, name_game2, &score2 );

    // find the second one
    status = map_game_score_get_from_name( &map_victory, "second", &score_out);
    assert_int_equal( 0, status );
    assert_true( score_equal(score2, score_out) );
    
    // find the first one
    status = map_game_score_get_from_name( &map_victory, "first", &score_out);
    assert_int_equal( 0, status );
    assert_true( score_equal(score1, score_out) );
    
    // not found, when map not empty
    status = map_game_score_get_from_name( &map_victory, "tata", &score_out);
    assert_int_equal( -1, status );
    assert_true( score_equal(score_default, score_out) );

    map_game_score_delete( &map_victory );
}

static void copy_map()
{
    int  status;
    map_game_score_t map_victory, map_victory_copy;
    map_game_score_init( &map_victory );

    // copy empty string
    map_victory_copy = map_game_score_copy( &map_victory, &status);
    assert_int_equal(0, status);
    assert_int_equal(0, map_game_score_size(&map_victory_copy));
    // delete the copy
    map_game_score_delete( &map_victory_copy);
    
    status = map_game_score_insert( &map_victory, name_game1, &score1 );
    status = map_game_score_insert( &map_victory, name_game2, &score2 );
    
    map_victory_copy = map_game_score_copy( &map_victory, &status);
    assert_int_equal( 0, status );
    // delete original
    map_game_score_delete( &map_victory );
    // copy correct size
    assert_int_equal( 2, map_game_score_size(&map_victory_copy));
    
    // delete copy
    map_game_score_delete( &map_victory_copy );
}

/*********** Helper *********/
bool score_equal(score_game_t score1, score_game_t score2)
{
    return ( score1.nb_win == score2.nb_win )
            && ( score1.nb_lost == score2.nb_lost )
            && ( score1.nb_draw == score2.nb_draw );
}

int main()
{
    /* can be inside main, or as global. here no problem with identical name */
    const struct CMUnitTest tests_map_game_score[] = {
        cmocka_unit_test(pair_score_to_value),
        cmocka_unit_test(value_to_pair_score),
        cmocka_unit_test(construction_map),
        cmocka_unit_test(insert_and_replace_new_score),
        cmocka_unit_test(get_from_name),
        cmocka_unit_test(copy_map),
    };
    /* call group_setup and teardown at the very beginning and end */
    /* return cmocka_run_group_tests(tests_historique, group_setup, group_teardown);*/
    return cmocka_run_group_tests(tests_map_game_score, NULL, NULL);
}