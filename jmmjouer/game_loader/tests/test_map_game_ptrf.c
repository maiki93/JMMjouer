/* import for CMocka */
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include "cmocka.h"

/* No ! main test is make_value / extract_value, others are only recopy */
#include "jmmjouer/game_loader/map_game_ptrf.c"
/*#include "jmmjouer/game_loader/map_game_ptrf.h"*/
#include "ccontainer/ccontainer_utils.h"

/* function to pointer test, explicit correct argument
    really need to include person from other module for a test ? */
static victory_t test1(person_t person) {
    victory_t vict_out;
    strcpy( person.pname, "nom_test");
    person.is_daltonien = true;
    vict_out.nb_win = 1;
    vict_out.nb_lost = 1;
    vict_out.nb_draw = 1;
    return vict_out;
}

pair_game_ptrf_t pair_in1 = {"first",
                                 NULL};

pair_game_ptrf_t pair_in2 = {"second",
                                 &test1};

static void pair_game_to_value()
{
    ccontainer_err_t err_code;
    ccontainer_value_t value_out;
    // copy all data into value_t (serailization string + ptr_game)
    value_out = make_value_game_ptrf( &pair_in2, &err_code );
    assert_int_equal(CCONTAINER_OK, err_code);
    assert_string_equal("second", value_out.data);
    // default deleter, no pointer inside value_t
    ccontainer_delete_value( &value_out );

    value_out = make_value_game_ptrf( &pair_in1, &err_code );
    assert_string_equal("first", value_out.data);
    ccontainer_delete_value( &value_out );
}

static void value_to_pair_game()
{
    ccontainer_err_t err_code;
    ccontainer_value_t value_out;
    // allocated inside call
    value_out = make_value_game_ptrf( &pair_in2, &err_code );

    // allocated inside function, caller repsonsible deallocation
    pair_game_ptrf_t pair_out = extract_value_pair_game_ptrf( &value_out, &err_code );
    assert_int_equal(CCONTAINER_OK, err_code);
    assert_string_equal("second", pair_out.game_name);
    assert_ptr_equal( &test1, pair_out.pfgame);

    // delete pair
    delete_pair_game_ptrf( &pair_out );
    // delete copy in value_t
    ccontainer_delete_value( &value_out );
}

static void initialization_on_heap()
{
    map_game_ptrf_t *map;
    map = game_ptrf_new();

    game_ptrf_init( map );
    assert( map->clist != NULL);
    assert( game_ptrf_size( map ) == 0);

    game_ptrf_free( map );
    map = NULL;
}

static void insert_two_pair()
{
    ccontainer_err_t err_code;
    map_game_ptrf_t *map;

    map = game_ptrf_new();
    game_ptrf_init( map );

    err_code = game_ptrf_insert( map, pair_in1.game_name, pair_in1.pfgame);
    assert_int_equal(err_code, CCONTAINER_OK);
    assert_int_equal(1, game_ptrf_size(map));

    err_code = game_ptrf_insert( map, pair_in2.game_name, pair_in2.pfgame);
    assert_int_equal(err_code, CCONTAINER_OK);
    assert_int_equal(2, game_ptrf_size(map));

    game_ptrf_free( map );
}

static void get_ptrf_from_name()
{
    ccontainer_err_t err_code;
    map_game_ptrf_t *map;

    map = game_ptrf_new();
    game_ptrf_init( map );

    err_code = game_ptrf_insert( map, pair_in1.game_name, pair_in1.pfgame);
    err_code = game_ptrf_insert( map, pair_in2.game_name, pair_in2.pfgame);
    assert_int_equal(CCONTAINER_OK, err_code);

    ptr_game_t ptrf1;
    ptrf1 = game_ptrf_get_from_name(map, "second", &err_code);
    assert_int_equal(CCONTAINER_OK, err_code);
    assert_ptr_equal( &test1, ptrf1 );

    ptr_game_t ptrf2;
    ptrf2 = game_ptrf_get_from_name(map, "first", &err_code);
    assert_ptr_equal( NULL, ptrf2 );

    game_ptrf_free( map );
}

static void get_clist_cstring_game_name()
{
    ccontainer_err_t err_code;
    map_game_ptrf_t *map;
    clist_cstring_t *list_name;
    char *name_game_out; 

    map = game_ptrf_new();
    game_ptrf_init( map );
    game_ptrf_insert( map, pair_in1.game_name, pair_in1.pfgame);
    game_ptrf_insert( map, pair_in2.game_name, pair_in2.pfgame);

    // clist of names, not convenient to check for indexes (in tests)
    list_name = game_ptrf_get_list_names( map );
    assert_int_equal( 2, clist_cstring_size( list_name ));

    name_game_out = clist_cstring_pop_front( list_name, &err_code );
    assert_string_equal("first", name_game_out);
    assert_int_equal( 1, clist_cstring_size( list_name ));
    // must be deallocated
    free( name_game_out );
    name_game_out = NULL;
    /* pass ok, make list_name empty
    name_game_out = clist_cstring_pop_front( list_name, &err_code );
    assert_string_equal("second", name_game_out);
    assert_int_equal( 0, clist_cstring_size( list_name ));
    // must be deallocated
    free( name_game_out );
    name_game_out = NULL;
    */
    clist_cstring_free( list_name );
    game_ptrf_free( map );
}

static void get_cvector_cstring_game_name()
{
    ccontainer_err_t err_code;
    map_game_ptrf_t *map;
    cvector_cstring_t vect_names;
    const char *name_game_ref_out;
    char *name_game_copy_out;

    map = game_ptrf_new();
    game_ptrf_init( map );
    game_ptrf_insert( map, pair_in1.game_name, pair_in1.pfgame);
    game_ptrf_insert( map, pair_in2.game_name, pair_in2.pfgame);

    // clist of names, not convenient to check for indexes (in tests)
    vect_names = game_ptrf_get_names( map );
    assert_int_equal( 2, cvector_cstring_size( &vect_names ));

    name_game_ref_out = cvector_cstring_get_ref_at( &vect_names, 0, &err_code );
    assert_string_equal("first", name_game_ref_out);
    name_game_ref_out = NULL;
    
    name_game_copy_out = cvector_cstring_get_copy_at( &vect_names, 1, &err_code );
    assert_string_equal("second", name_game_copy_out);
    free( name_game_copy_out);

    // original and cvector of names are not modified 
    assert_int_equal( 2, game_ptrf_size( map ));
    assert_int_equal( 2, cvector_cstring_size( &vect_names ));

    cvector_cstring_delete( &vect_names );
    game_ptrf_free( map );
    map = NULL;
}

/* return array, set deprecated until cvector */
static void get_array_game_name()
{
    map_game_ptrf_t *map;
    char **names;
    size_t names_len /*,i*/;
    int retour;

    map = game_ptrf_new();
    game_ptrf_init( map );

    game_ptrf_insert( map, pair_in1.game_name, pair_in1.pfgame);
    game_ptrf_insert( map, pair_in2.game_name, pair_in2.pfgame);

    retour = game_ptrf_get_array_name( map, &names, &names_len);
    assert_int_equal( CCONTAINER_OK, retour );
    assert_int_equal( 2, names_len );
    assert_string_equal( "first", names[0]);
    assert_string_equal( "second", names[1]);

    game_ptrf_free( map );
    /* free array memory
    for(i = 0; i< names_len; i++) {
        free(names[i]);
        names[i] = NULL;
    }
    free(names);
    */
    // equivalent, free is called inside the function (good/ bad ?)
    carray_cstring_delete( names, names_len);
    names = NULL;
}

int main()
{
    /* can be inside main, or as global. here no problem with identical name */
    const struct CMUnitTest tests_map_game_ptrf[] = {
        cmocka_unit_test(pair_game_to_value),
        cmocka_unit_test(value_to_pair_game),
        cmocka_unit_test(initialization_on_heap),
        cmocka_unit_test(insert_two_pair),
        cmocka_unit_test(get_ptrf_from_name),
        cmocka_unit_test(get_clist_cstring_game_name),
        cmocka_unit_test(get_cvector_cstring_game_name),
        cmocka_unit_test(get_array_game_name),
        /*
        cmocka_unit_test(find_not_existing_entry),
        */
    };
    /* call group_setup and teardown at the very beginning and end */
    /* return cmocka_run_group_tests(tests_historique, group_setup, group_teardown);*/
    return cmocka_run_group_tests(tests_map_game_ptrf, NULL, NULL);
}



