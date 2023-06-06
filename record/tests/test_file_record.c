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
    person_status_t status_p;
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
    status_p = person_status( (const person_t*) &joueur);
    assert_int_equal( PERSON_INVALID, status_p );

    joueur_delete( &joueur );
    file_record_delete( file );
    free( file );
}

static void joueur_valid_daltonien_no_game()
{
    joueur_t joueur;
    person_status_t status_person;
    const char *name_person;
    /*joueur_default_init( &joueur );*/

    file_record_t *file = file_record_new();
    file_record_init( file, "test_record_file.txt");
    // kevin no game=> error
    joueur = record_find_joueur_from_name( (irecord_t*)file, "kevin");

    //joueur = record_find_joueur_from_name( (irecord_t*)file, "jasmine");
    status_person = person_status( (const person_t*) &joueur);
    assert_int_equal( PERSON_VALID, status_person );
    name_person = person_name( (const person_t*) &joueur );
    assert_string_equal( name_person, "kevin");
    assert_int_equal( joueur.person.is_daltonien, 1);
    // kevin has no games recorded
    assert_int_equal( 0, game_victories_size( & joueur.map_victories) );

    joueur_delete( &joueur );
    file_record_delete( file );
    free( file );
}

static void joueur_with_historic()
{
    joueur_t joueur;
    struct pair_game_victory_t pair_out;
    /*joueur_default_init( &joueur );*/

    file_record_t *file = file_record_new();
    file_record_init( file, "test_record_file.txt");

    joueur = record_find_joueur_from_name( (irecord_t*)file, "jasmine");
    assert_string_equal( joueur.person.pname, "jasmine");
    assert_int_equal( joueur.person.is_daltonien, 1);

    size_t nb_games = game_victories_size( & joueur.map_victories );
    assert_int_equal(3, nb_games);
    //assert_int_equal(3, game_victories_size( & joueur.map_victories ));

    pair_out = game_victories_get_copy( &(joueur.map_victories), "mastermind" );
    assert_string_equal( "mastermind", pair_out.game_name);
    assert_int_equal( 4, pair_out.victories.nb_win);
    assert_int_equal( 5, pair_out.victories.nb_lost);

    pair_out = game_victories_get_copy( &(joueur.map_victories), "pendu" );
    assert_string_equal( "pendu", pair_out.game_name);
    assert_int_equal( 2, pair_out.victories.nb_win);
    assert_int_equal( 1, pair_out.victories.nb_lost);

    pair_out = game_victories_get_copy( &(joueur.map_victories), "morpion" );
    assert_string_equal( "morpion", pair_out.game_name);
    assert_int_equal( 1, pair_out.victories.nb_win);
    assert_int_equal( 3, pair_out.victories.nb_lost);

    pair_out = game_victories_get_copy( &(joueur.map_victories), "toto" );
    assert_string_equal( "invalid", pair_out.game_name);

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

    assert_string_equal( joueur.person.pname, "jasmine");
    assert_int_equal( joueur.person.is_daltonien, 1);

    size_t nb_games = game_victories_size( & joueur.map_victories );
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

