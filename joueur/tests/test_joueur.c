/* import for CMocka */
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include "cmocka.h"

/* include user_t and map_game_score */
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

static void user_constructor()
{
    user_status_t status;

    user_t user;
    status = user_default_init(&user);
    assert_int_equal(USER_INVALID, status);
    assert_int_equal( USER_INVALID, user_status( &user) );
    assert_false( user_valid(&user));
    assert_int_equal(0, user_id(&user));
    assert_false(user_daltonien(&user));
    assert_false(user_admin(&user));
    assert_null(user_name(&user));

    user_delete(&user);

    status = user_init(&user, 1, "maiki", true, false);
    assert_int_equal(USER_VALID, status);
    assert_int_equal( USER_VALID, user_status( &user));
    assert_true( user_valid(&user));
    assert_int_equal(1, user_id(&user));
    assert_string_equal("maiki", user_name(&user));
    assert_true( user_daltonien(&user));
    // only here a setter... may avoid
    assert_false( user_admin(&user));
    //user_set_admin( &user );
    //assert_int_not_equal( USER_ADMIN, user_status( &user) );

    user_delete(&user);
}

static void joueur_constructor()
{
    user_status_t status;

    joueur_t joueur;
    status = joueur_default_init( &joueur );

    assert_int_equal(USER_INVALID, status);
    assert_int_equal( USER_INVALID, joueur_status( &joueur));
    assert_false(joueur_daltonien( &joueur));
    assert_false(joueur_admin( &joueur));

    joueur_delete(&joueur);

    status = joueur_init(&joueur, 5, "maiki", 
            true /*is_daltonien*/, true /*is_admin*/);
    /* one way to test if not an error */
    assert_true( joueur_valid( &joueur));
    assert_int_equal(5, joueur_id(&joueur));
    assert_string_equal("maiki", joueur_name( &joueur));
    assert_true( joueur_daltonien( &joueur));
    assert_true( joueur_admin( &joueur));

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
        cmocka_unit_test(user_constructor),
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