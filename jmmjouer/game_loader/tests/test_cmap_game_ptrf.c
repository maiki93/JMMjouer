/* import for CMocka */
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include "cmocka.h"

/* variant from cmap, include header */
/* #include "../cmap_ptrf_game.h" */
/* No ! main test is make_value / extract_value, others are only recopy */
#include "jmmjouer/game_loader/cmap_ptrf_game.c"

/* function to pointer test, explicit correct argument */
static victory_t test1(person_t person) {
    victory_t vict_out;
    strcpy( person.nom, "nom_test");
    person.is_daltonien = true;
    vict_out.nb_win = 1;
    vict_out.nb_lost = 1;
    vict_out.nb_equality = 1;
    return vict_out;
}

struct pair_ptr_game pair_in1 = {"first",
                                 NULL};

struct pair_ptr_game pair_in2 = {"second",
                                 &test1};

static void pair_game_to_value()
{
    value_t value_out;

    value_out = make_value_ptr_game( &pair_in2 );
    assert_string_equal("second", value_out.data);
    // free memory before calling again
    // or delete inside ?? unitialized not clear...
    free_value( &value_out );

    value_out = make_value_ptr_game( &pair_in1 );
    assert_string_equal("first", value_out.data);
    free_value( &value_out );
}

static void value_to_pair_game()
{
    value_t value_out;
    // allocated inside call
    value_out = make_value_ptr_game( &pair_in2 );

    // allocated inside function, caller repsonsible deallocation
    struct pair_ptr_game pair_out = extract_value_ptr_game( &value_out );

    assert_string_equal("second", pair_out.game_name);
    assert_ptr_equal( &test1, pair_out.pfgame);

    assert_int_equal(1,1);
    // delete
    delete_pair_ptr_game( &pair_out );
    free_value( &value_out );
    
}

static void initialization_on_heap()
{
    cmap_ptrf_game_t *cmap;
    cmap = game_ptrf_new();

    game_ptrf_init( cmap );
    assert( cmap->clist != NULL);

    game_ptrf_delete( cmap );
    cmap = NULL;
}

static void insert_new_pair_game()
{
    int retour;
    cmap_ptrf_game_t *cmap;
    cmap = game_ptrf_new();
    game_ptrf_init( cmap );

    retour = game_ptrf_insert( cmap, pair_in1.game_name, pair_in1.pfgame);
    assert_int_equal(retour, CLIST_OK);
    assert_int_equal(1, game_ptrf_size(cmap));

    game_ptrf_delete( cmap );
}

static void get_ptrf_from_name()
{
    cmap_ptrf_game_t *cmap;
    cmap = game_ptrf_new();
    game_ptrf_init( cmap );

    game_ptrf_insert( cmap, pair_in1.game_name, pair_in1.pfgame);
    game_ptrf_insert( cmap, pair_in2.game_name, pair_in2.pfgame);

    ptr_game_t ptrf1;
    ptrf1 = game_ptrf_get_from_name(cmap, "second");
    assert_ptr_equal( &test1, ptrf1 );

    ptr_game_t ptrf2;
    ptrf2 = game_ptrf_get_from_name(cmap, "first");
    assert_ptr_equal( NULL, ptrf2 );

    game_ptrf_delete( cmap );
}

static void get_clist_cstring_game_name()
{
    cmap_ptrf_game_t *cmap;
    clist_cstring_t *names;
    char *name_game_out; 

    cmap = game_ptrf_new();
    game_ptrf_init( cmap );
    game_ptrf_insert( cmap, pair_in1.game_name, pair_in1.pfgame);
    game_ptrf_insert( cmap, pair_in2.game_name, pair_in2.pfgame);

    names = game_ptrf_get_names( cmap );
    
    assert_int_equal( 2, clist_cstring_size( names ));
    clist_cstring_get_ref( names, 0, &name_game_out );
    assert_string_equal("first", name_game_out);
    clist_cstring_get_ref( names, 1, &name_game_out );
    assert_string_equal("second", name_game_out);
    clist_cstring_delete( names );
    game_ptrf_delete( cmap );
}

/* return array, set deprecated */
static void get_array_game_name()
{
    cmap_ptrf_game_t *cmap;
    char **names;
    size_t names_len,i;
    int retour;

    cmap = game_ptrf_new();
    game_ptrf_init( cmap );

    game_ptrf_insert( cmap, pair_in1.game_name, pair_in1.pfgame);
    game_ptrf_insert( cmap, pair_in2.game_name, pair_in2.pfgame);

    retour = game_ptrf_get_array_name( cmap, &names, &names_len);
    assert_int_equal( CLIST_OK, retour );
    assert_int_equal( 2, names_len );
    assert_string_equal( "first", names[0]);
    assert_string_equal( "second", names[1]);

    game_ptrf_delete( cmap );
    for(i = 0; i< names_len; i++) {
        free(names[i]);
        names[i] = NULL;
    }
    free(names);
}

int main()
{
    /* can be inside main, or as global. here no problem with identical name */
    const struct CMUnitTest tests_cmap_game_ptrf[] = {
        cmocka_unit_test(pair_game_to_value),
        cmocka_unit_test(value_to_pair_game),
        cmocka_unit_test(initialization_on_heap),
        cmocka_unit_test(insert_new_pair_game),
        cmocka_unit_test(get_ptrf_from_name),
        cmocka_unit_test(get_clist_cstring_game_name),
        cmocka_unit_test(get_array_game_name),
        /*
        cmocka_unit_test(find_not_existing_entry),
        */
    };
    /* call group_setup and teardown at the very beginning and end */
    /* return cmocka_run_group_tests(tests_historique, group_setup, group_teardown);*/
    return cmocka_run_group_tests(tests_cmap_game_ptrf, NULL, NULL);
}



