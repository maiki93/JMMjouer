/* import for CMocka */
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include "cmocka.h"

#include <string.h>
#include "joueur/list_joueur.h"
#include "joueur/vector_joueur.h"
#include "joueur/adapter_ccontainer_joueur.h"

/* same as test joueur , to combine */
/* not const if want to add/change map_score associated */
static joueur_t joueur1, joueur2, joueur3;
/* only available C89 + */
static score_game_t victory1 = {.nb_win=1,.nb_lost=2,.nb_draw=3};
static score_game_t victory2 = {.nb_win=1,.nb_lost=2,.nb_draw=3};
static score_game_t victory3 = {.nb_win=1,.nb_lost=2,.nb_draw=3};
/* helper */
static joueur_t make_joueur1();
static void make_three_joueur();
static void delete_three_joueur();

static size_t get_next_joueur_id() {
    static size_t id = 1;
    return id++;
}

static void adapter_ccontainer_joueur()
{
    ccontainer_err_t err_code;
    /* initialize joueur1 setup/teradown */
    joueur1 = make_joueur1();

    joueur_t joueur_extracted; /* */
    ccontainer_value_t value_joueur;
    // want a deep copy to be moved into a ccontainer
    value_joueur = make_value_joueur( &joueur1, &err_code );

    // suprress original, sure there is no copy of a reference
    joueur_delete( &joueur1 );

    err_code = extract_value_joueur( &value_joueur, &joueur_extracted );
    assert_int_equal( CCONTAINER_OK, err_code );
    assert_string_equal("maiki", joueur_name(&joueur_extracted) );
    assert_false( joueur_daltonien( (joueur_t*) &joueur_extracted) );
    assert_true( joueur_admin( (joueur_t*) &joueur_extracted) );
    assert_int_equal( USER_VALID, joueur_status( (joueur_t*) &joueur_extracted) );
    // check map_score
    assert_int_equal( 1, map_game_score_size( &(joueur_extracted.map_score)) );

    joueur_delete( &joueur_extracted );
    // must suprress allocated in values if not inserted into ccontainer
    joueur_deleter_value( &value_joueur );
}

static void list_joueur_constructor()
{
    list_joueur_t *list_joueur = list_joueur_new();
    assert_non_null( list_joueur );

    list_joueur_init( list_joueur );
    assert_int_equal( 0, list_joueur_size( list_joueur ));
    list_joueur_free( list_joueur );
    list_joueur = NULL;
}

static void list_push_back_and_pop_joueur()
{
    /* initialize joueur1 setup/teradown */
    joueur1 = make_joueur1();

    joueur_t joueur_extracted;
    int ret;

    list_joueur_t *list_joueur = list_joueur_new();
    list_joueur_init( list_joueur );

    ret = list_joueur_push_back( list_joueur, &joueur1 );
    assert_int_equal(0, ret);
    assert_int_equal( 1, list_joueur_size( list_joueur ) );
    
    // suprress original, sure there is no copy of a reference
    joueur_delete( &joueur1 );
    // pop first
    joueur_extracted = list_joueur_pop_front( list_joueur );

    assert_string_equal("maiki", joueur_name(&joueur_extracted) );
    assert_false( joueur_daltonien(&joueur_extracted) );
    assert_true( joueur_admin(&joueur_extracted) );
    assert_int_equal( USER_VALID, joueur_status(&joueur_extracted) );

    // state of list is empty now
    assert_int_equal( 0, list_joueur_size( list_joueur ));
    // must delete tmp copy
    joueur_delete( &joueur_extracted );
    // pop but empty
    joueur_extracted = list_joueur_pop_front( list_joueur );
    assert_int_equal( USER_INVALID, joueur_status(&joueur_extracted) );
    
    joueur_delete( &joueur_extracted );

    list_joueur_free( list_joueur );
    list_joueur = NULL;
}

// vector_joueur on stack or on heap
static void vector_joueur_constructor()
{
    vector_joueur_t *vect_joueur;
    vector_joueur_t vect_joueur2;
    int status;

    vect_joueur = vector_joueur_new();
    assert_non_null( vect_joueur );
    status = vector_joueur_init( vect_joueur );
    assert_int_equal( 0, status);
    assert_int_equal( 0, vector_joueur_size( vect_joueur ));
    vector_joueur_free( vect_joueur );
    vect_joueur = NULL;

    // init on stack
    status = vector_joueur_init_with_capacity( &vect_joueur2, 5 );
    assert_int_equal( 0, status);
    assert_int_equal( 0, vector_joueur_size( &vect_joueur2 ));
    // capacity 5
    //assert_int_equal( 0, vector_joueur_capacity( &vect_joueur2 ));
    vector_joueur_delete( &vect_joueur2 );
}

static void vector_push_back_and_get_joueur()
{
    vector_joueur_t vector_joueur;
    int status;

    joueur_t joueur1 = make_joueur1();
    joueur_t joueur_extracted;
    vector_joueur_init(&vector_joueur);

    // push back
    status = vector_joueur_push_back( &vector_joueur, &joueur1);
    assert_int_equal(0, status);
    assert_int_equal(1, vector_joueur_size(&vector_joueur) );
    // delete original
    joueur_delete( &joueur1 );

    // get copy valid
    joueur_extracted = vector_joueur_get_at( &vector_joueur, 0);
    assert_string_equal("maiki", joueur_name(&joueur_extracted) );
    assert_false( joueur_daltonien(&joueur_extracted) );
    assert_true( joueur_admin(&joueur_extracted) );
    assert_int_equal( USER_VALID, joueur_status(&joueur_extracted) );
    joueur_delete( &joueur_extracted );

    // copy invalid
    joueur_extracted = vector_joueur_get_at( &vector_joueur, 1);
    assert_int_equal( USER_INVALID, joueur_status(&joueur_extracted) );
    joueur_delete( &joueur_extracted );

    vector_joueur_delete( &vector_joueur );
}

static void list_to_vector_joueur()
{
    // list_joueur_t list_joueur;
    list_joueur_t *list_joueur = list_joueur_new();
    vector_joueur_t* pvector_joueur;

    make_three_joueur();
    list_joueur_init( list_joueur );
    list_joueur_push_back( list_joueur, &joueur1);
    list_joueur_push_back( list_joueur, &joueur2);
    list_joueur_push_back( list_joueur, &joueur3);

    assert_int_equal(3, list_joueur_size(list_joueur));

    // both working, generic better optimized
    pvector_joueur = list_joueur_to_vector( list_joueur );
    //pvector_joueur = list_joueur_to_vector_generic_impl( list_joueur );

    assert_non_null( pvector_joueur );
    assert_int_equal( 3, vector_joueur_size( pvector_joueur) );
    assert_int_equal( 0, list_joueur_size( list_joueur) );

    list_joueur_free( list_joueur );
    vector_joueur_free(pvector_joueur);

    delete_three_joueur();
}

static void list_to_vector_joueur_gen_optimized()
{
    // list_joueur_t list_joueur;
    list_joueur_t *list_joueur = list_joueur_new();
    vector_joueur_t* pvector_joueur;

    make_three_joueur();
    list_joueur_init( list_joueur );
    list_joueur_push_back( list_joueur, &joueur1);
    list_joueur_push_back( list_joueur, &joueur2);
    list_joueur_push_back( list_joueur, &joueur3);

    assert_int_equal(3, list_joueur_size(list_joueur));

    // both working, generic better optimized
    //pvector_joueur = list_joueur_to_vector( list_joueur );
    pvector_joueur = list_joueur_to_vector_generic_impl( list_joueur );

    assert_non_null( pvector_joueur );
    assert_int_equal( 3, vector_joueur_size( pvector_joueur) );
    assert_int_equal( 0, list_joueur_size( list_joueur) );

    list_joueur_free( list_joueur );
    vector_joueur_free(pvector_joueur);

    delete_three_joueur();
}

/* utilities/helper function in joueur */
joueur_t make_joueur1()
{
    joueur_t joueur;
    joueur_init(&joueur, 
                1,
                "maiki", false, true);
    map_game_score_insert( &(joueur.map_score), "jeu1", &victory1 );
    return joueur;
}

static void make_three_joueur()
{
    joueur_init(&joueur1,
                get_next_joueur_id(),
                "maiki", false, false);

    
    map_game_score_insert( &(joueur1.map_score), "jeu1", &victory1 );

    joueur_init(&joueur2,
                get_next_joueur_id(),
                "kevin", true, false);
    map_game_score_insert( &(joueur2.map_score), "jeu2", &victory2 );

    joueur_init(&joueur3,
                get_next_joueur_id(),
                "admin", false, true);
    map_game_score_insert( &joueur3.map_score, "jeu3", &victory3 );

    return;
}

void delete_three_joueur()
{
    joueur_delete(&joueur1);
    joueur_delete(&joueur2);
    joueur_delete(&joueur3);
}

int main()
{
    /* can be inside main, or as global. here no problem with identical name */
    const struct CMUnitTest tests_ccontainer_joueur[] = {
        cmocka_unit_test(adapter_ccontainer_joueur),
        cmocka_unit_test(list_joueur_constructor),
        cmocka_unit_test(list_push_back_and_pop_joueur),
        cmocka_unit_test(vector_joueur_constructor),
        cmocka_unit_test(vector_push_back_and_get_joueur),
        cmocka_unit_test(list_to_vector_joueur),
        cmocka_unit_test(list_to_vector_joueur_gen_optimized),
        /*
        cmocka_unit_test(pair_victory_to_value),
        cmocka_unit_test(value_to_pair_victory),
        cmocka_unit_test(construction_cmap),
        cmocka_unit_test(insert_new_victory),
        cmocka_unit_test(find_not_existing_entry),
        */
    };
    /* call group_setup and teardown at the very beginning and end */
    /* return cmocka_run_group_tests(tests_historique, group_setup, group_teardown);*/
    return cmocka_run_group_tests(tests_ccontainer_joueur, NULL, NULL);
}


