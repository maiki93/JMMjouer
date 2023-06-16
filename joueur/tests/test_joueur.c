/* import for CMocka */
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include "cmocka.h"

/* include person_t and map_game_score */
#include "joueur/joueur.h"
/* ccontainer adapter */
#include "joueur/adapter_ccontainer_joueur.h"

/*#include <string.h>*/ /* only for make_joueur1, to move */

/* not const if want to add/change map_victories associated */
/* static joueur_t joueur1;*/
/* only available C89 + */
/*static const victory_t victory1 = {.nb_win=1,.nb_lost=2,.nb_draw=3};*/
/* helper */
/*static joueur_t make_joueur1();*/

static void person_constructor()
{
    person_status_t status;

    person_t person;
    status = person_default_init(&person);

    assert_int_equal(PERSON_INVALID, status);
    assert_int_equal( PERSON_INVALID, person_status( &person) );
    assert_false(person_daltonien(&person));
    assert_false(person_admin(&person));

    person_delete(&person);

    status = person_init(&person, "maiki", true, false);
    assert_int_equal(PERSON_VALID, status);
    assert_int_equal( PERSON_VALID, person_status( &person));
    assert_string_equal("maiki", person_name(&person));
    assert_true( person_daltonien(&person));
    // only here a setter... may avoid
    assert_false( person_admin(&person));
    //person_set_admin( &person );
    assert_int_not_equal( PERSON_ADMIN, person_status( &person) );

    person_delete(&person);
}

static void joueur_constructor()
{
    person_status_t status;

    joueur_t joueur;
    status = joueur_default_init( &joueur );

    assert_int_equal(PERSON_INVALID, status);
    assert_int_equal( PERSON_INVALID, person_status( (person_t*) &joueur) );
    assert_false(person_daltonien( (person_t*) &joueur));
    assert_false(person_admin( (person_t*) &joueur));

    joueur_delete(&joueur);

    status = joueur_init(&joueur, "maiki", 
            true /*is_daltonien*/, true /*is_admin*/);
    assert_int_equal(PERSON_ADMIN, status);
    /* one way to test if not an error */
    assert_true( PERSON_VALID <= person_status( (person_t*) &joueur));
    assert_int_equal( PERSON_ADMIN, person_status( (person_t*) &joueur));
    assert_string_equal("maiki", person_name( (person_t*) &joueur));
    assert_true( person_daltonien((person_t*) &joueur));
    assert_true( person_admin((person_t*) &joueur));

    joueur_delete(&joueur);
}

/* utilities/helper function in joueur */
/*
static joueur_t make_joueur1()
{
    joueur_t joueur;
    joueur_init(&joueur, "maiki", false, true);
    // to hide in joueur implementation the usage of pair
    struct pair_game_victory_t pair1;
    strcpy( pair1.game_name, "jeu1");
    pair1.victories = victory1;

    game_victories_insert( & joueur.map_victories, &pair1 );
    return joueur;
} 
*/

int main()
{
    /* can be inside main, or as global. here no problem with identical name */
    const struct CMUnitTest tests_joueur[] = {
        cmocka_unit_test(person_constructor),
        cmocka_unit_test(joueur_constructor),
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
    return cmocka_run_group_tests(tests_joueur, NULL, NULL);
}