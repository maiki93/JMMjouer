/* import for CMocka */
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include "cmocka.h"

/* #include "../clist_generic.c" */
#include "../cmap_game_victories.c"

/* common data, pair_game_victory_t do not use dynamic allocation
    no explicit free needed  */
struct pair_game_victory_t pair_in1 = {"first",  /* game_name */
                                       {1, 0, 2} /* victory_t:win, lost, equal*/
                                     };
struct pair_game_victory_t pair_in2 = {"first",  /* game_name */
                                       {4, 5, 6} /* victory_t:win, lost, equal*/
                                     };
struct pair_game_victory_t pair_out; /* fetch */

static void victory_to_value()
{
    value_t value_out;
    value_out = make_value_victory( pair_in1 );
    assert_string_equal("first", value_out.data);

    free(value_out.data);
    value_out.len = 0;
}

static void value_to_victory()
{
    value_t value_out;
    value_out = make_value_victory( pair_in1 );

    pair_out = extract_value_victory( &value_out );
    assert_string_equal("first", pair_out.game_name);
    assert_int_equal(1, pair_out.victories.nb_win);
    assert_int_equal(0, pair_out.victories.nb_lost);
    assert_int_equal(2, pair_out.victories.nb_equality);

    free(value_out.data);
    value_out.len = 0;
}

//static void allocation_on_stack()

static void insert_new_victory()
{
    size_t retour;
    cmap_game_victories_t map_victory;
    game_victories_init( &map_victory );
    
    retour = game_victories_insert( &map_victory, pair_in1 );
    assert_int_equal( CLIST_OK, retour );

    pair_out = game_victories_get_copy( &map_victory, "first");
    assert_string_equal("first", pair_out.game_name);
    assert_int_equal(1, pair_out.victories.nb_win);
    assert_int_equal(0, pair_out.victories.nb_lost);
    assert_int_equal(2, pair_out.victories.nb_equality);

    game_victories_clear( &map_victory );
}

static void find_not_existing_entry()
{
    cmap_game_victories_t map_victory;
    game_victories_init( &map_victory );
    
    // find with an empty list
    pair_out = game_victories_get_copy( &map_victory, "toto");
    assert_string_equal("invalid", pair_out.game_name);

    // when make an insertion
    game_victories_insert( &map_victory, pair_in1 );
    pair_out = game_victories_get_copy( &map_victory, "toto");
    assert_string_equal("invalid", pair_out.game_name);

    game_victories_clear( &map_victory );
}

int main()
{
    /* can be inside main, or as global. here no problem with identical name */
    const struct CMUnitTest tests_cmap_game_victories[] = {
        cmocka_unit_test(victory_to_value),
        cmocka_unit_test(value_to_victory),
        cmocka_unit_test(insert_new_victory),
        cmocka_unit_test(find_not_existing_entry),
    };
    /* call group_setup and teardown at the very beginning and end */
    /* return cmocka_run_group_tests(tests_historique, group_setup, group_teardown);*/
    return cmocka_run_group_tests(tests_cmap_game_victories, NULL, NULL);
}