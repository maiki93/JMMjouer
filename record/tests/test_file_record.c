/* import for CMocka */
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>

#include "cmocka.h"

/*
Create error with use of sscanf
=> suppress_sscanf.txt
run with -suppress suppress_sscanf.txt

more general problem with mingw64:
https://metricpanda.com/rival-fortress-update-45-dealing-with-__chkstk-__chkstk_ms-when-cross-compiling-for-windows/
*/

/*#include "../irecord.h"*/ /* ambigous redefinition of irecord_t */
#include "../irecord_private.h"
#include "../file_record.c"

//#include "../joueur.h" // already included by ??

/* cannot include public API of irecord, need to define them here,
    acceptable for tests */
char* record_get_storage_info(irecord_t *this);
joueur_t record_find_joueur_from_name( irecord_t *record, const char* name);

static void initialization()
{
    file_record_t *file = file_record_new();
    file_record_init( file, "test_record_file.txt");

    char *description = record_get_storage_info( (irecord_t*) file);
    assert_int_equal(57, strlen(description));
    assert_string_equal("Record: a simple text file\nfilename test_record_file.txt\n", description);

    /* except declaring "const" char*, no way to forbid call to free */
    /*free(description);*/ /* would liberate internal memory from irecord */

    description = record_get_storage_info( (irecord_t*) file);
    assert_int_equal(57, strlen(description));

    file_record_delete( file );
    free( file );
}

static void joueur_not_found()
{
    joueur_t joueur;
    user_status_t status_p;
    /* dr memory suppress a leak clist_new */
    /* joueur returned by copy, previously allocated is lost 
        return by value, no way to check/clean properly 
        it is a (strong) limitation ?
    */
    /*joueur_default_init( &joueur );*/

    file_record_t *file = file_record_new();
    file_record_init( file, "test_record_file.txt");

    joueur = record_find_joueur_from_name( (irecord_t*)file, "titi");
    //assert_string_equal( joueur.person.pname, "invalid");
    status_p = joueur_status( &joueur);
    assert_int_equal( USER_INVALID, status_p );

    joueur_delete( &joueur );
    file_record_delete( file );
    free( file );
}

static void joueur_valid_daltonien_no_game()
{
    joueur_t joueur;
    user_status_t status_user;
    const char *name_user;
    
    file_record_t *file = file_record_new();
    file_record_init( file, "test_record_file.txt");
    // kevin no game=> error
    joueur = record_find_joueur_from_name( (irecord_t*)file, "kevin");

    //joueur = record_find_joueur_from_name( (irecord_t*)file, "jasmine");
    status_user = joueur_status( &joueur);
    assert_int_equal( USER_VALID, status_user );
    name_user = joueur_name( &joueur );
    assert_string_equal( name_user, "kevin");
    assert_int_equal( joueur.user.is_daltonien, 1);
    // kevin has no games recorded
    assert_int_equal( 0, map_game_score_size( & joueur.map_score) );

    joueur_delete( &joueur );
    file_record_delete( file );
    free( file );
}

static void joueur_with_historic()
{
    joueur_t joueur;
    score_game_t score;
    int status;
    /*struct pair_game_victory_t pair_out;*/
    /*joueur_default_init( &joueur );*/

    file_record_t *file = file_record_new();
    file_record_init( file, "test_record_file.txt");

    joueur = record_find_joueur_from_name( (irecord_t*)file, "jasmine");
    assert_string_equal( joueur.user.pname, "jasmine");
    assert_int_equal( joueur.user.is_daltonien, 1);

    size_t nb_games = map_game_score_size( & joueur.map_score );
    assert_int_equal(3, nb_games);


    /*pair_out = game_victories_get_copy( &(joueur.map_victories), "mastermind" );*/
    status = map_game_score_get_from_name(&(joueur.map_score), "mastermind", &score);
    /*assert_string_equal( "mastermind", pair_out.game_name);*/
    assert_int_equal(status, 0); /* key found */
    assert_int_equal( 4, score.nb_win);
    assert_int_equal( 5, score.nb_lost);
    assert_int_equal( 0, score.nb_draw);

    status = map_game_score_get_from_name(&(joueur.map_score), "pendu", &score);
    assert_int_equal(status, 0); /* key found */
    /*pair_out = game_victories_get_copy( &(joueur.map_victories), "pendu" );*/
    /*assert_string_equal( "pendu", pair_out.game_name);*/
    assert_int_equal( 2, score.nb_win);
    assert_int_equal( 1, score.nb_lost);
    assert_int_equal( 0, score.nb_draw);

    //pair_out = game_victories_get_copy( &(joueur.map_victories), "morpion" );
    status = map_game_score_get_from_name(&(joueur.map_score), "morpion", &score);
    assert_int_equal(status, 0); /* key found */
    assert_int_equal( 1, score.nb_win);
    assert_int_equal( 3, score.nb_lost);
    assert_int_equal( 0, score.nb_draw);

    // game not existing not found
    //pair_out = game_victories_get_copy( &(joueur.map_victories), "toto" );
    status = map_game_score_get_from_name(&(joueur.map_score), "toto", &score);
    assert_int_equal(-1, status); /* key not found => default score */
    assert_int_equal( 0, score.nb_win);
    assert_int_equal( 0, score.nb_lost);
    assert_int_equal( 0, score.nb_draw);
    
    joueur_delete( &joueur );
    file_record_delete( file );
    free( file );
}

/* no point to delelte */
static void default_init_joueur_with_historic()
{
    joueur_t joueur;
    
    file_record_t *file = file_record_new();
    file_record_init( file, "test_record_file.txt");

    /* or joueur already existing */
    /*joueur_default_init( &joueur );*/
    joueur = record_find_joueur_from_name( (irecord_t*)file, "jasmine");

    assert_string_equal( joueur.user.pname, "jasmine");
    assert_int_equal( joueur.user.is_daltonien, 1);

    size_t nb_games = map_game_score_size( & joueur.map_score );
    assert_int_equal(3, nb_games);

    joueur_delete( &joueur );
    file_record_delete( file );
    free( file );
}

static void count_functional_style_test()
{
    size_t count=0;
    file_record_t *file = file_record_new();
    file_record_init( file, "test_record_file.txt");
    /* to make a public overidden API of it */
    count = count_records( file );
    assert_int_equal(6, count);

    file_record_delete( file );
    free( file );
}

int main()
{
    /* can be inside main, or as global. here no problem with identical name */
    const struct CMUnitTest tests_file_record[] = {
        cmocka_unit_test(initialization),
        cmocka_unit_test(joueur_not_found),
        cmocka_unit_test(joueur_valid_daltonien_no_game),
        cmocka_unit_test(joueur_with_historic),
        cmocka_unit_test(default_init_joueur_with_historic),
        cmocka_unit_test(count_functional_style_test),
    };

    /* call group_setup and teardown at the very beginning and end */
    /* return cmocka_run_group_tests(tests_historique, group_setup, group_teardown);*/
    return cmocka_run_group_tests(tests_file_record, NULL, NULL);
}

