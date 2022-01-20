#include <stdio.h>
#include <stdlib.h>
// import for CMocka
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

//#include "../game_mastermind.c"
#include "../game_mastermind.h"

/*** test start_game_mastermind : check update ***/
static void null_test_success(void **state) {
    (void) state; /* unused  here*/
}

/** mock fuynctions **/
/*
int __wrap_mm_make_one_game( const char* colors, const int SizeColor, const int NbPIECE, const int MaxTENTATIVE, const bool mode_daltonien )
{
    // check_expected   //to check paramters
    return mock();
}*/

// working with function mocked in the same file implementation file that the function to test.
// need __attribute__((weak)) on the mocked function, no need of -Wl,--wrap=mm_make_one_game in linker
// BUT cannot really mock, change the return value for instance. not very flexible !!
int mm_make_one_game( const char* colors, const int SizeColor, const int NbPIECE, const int MaxTENTATIVE, const bool mode_daltonien )
{
    printf("redefine make_one_game as a mock\n");
    return 1;
}

// it is in utls*h/c, __wrap should work
// if we comment this part of the code (and no link option, it shoudl work normmally TO TEST
bool __wrap_ask_yesno_question(const char* message)
{
    printf("enter mocked yesno question\n");
    return mock();
}

/** tests functions **/
static void win_one_partie_increment1_nb_victoire_mm( void **state )
{
    // given
    Historique* historique = malloc( sizeof(Historique) );
    initialize_historique( historique );
    Joueur joueur = {"toto", false, false };
    // mock setup with wrap
    /*
    will_return(__wrap_mm_make_one_game, 1); // victoire du joueur
    */
    will_return(__wrap_ask_yesno_question, false); // do not want to play again
    //when
    int retour_game = start_game_mastermind( joueur, historique );
    // then
    assert_int_equal( 3, retour_game ); // 3 just for test
    assert_int_equal( 1, historique->nbre_victoire_mm );
    assert_int_equal( 0, historique->nbre_defaite_mm );
}

/*** limit of overiding weak methods ***/
// not possible, because mm_make_one_game is simply overriden locally
// void lose_one_partie_increment1_nb_defaite_mm(void **state)

// if play 2 games, increments 2 times. Use mock to make an other game (rejouer une fois)
static void win_2games_increment2( void **state )
{
    // given
    Historique* historique = malloc( sizeof(Historique) );
    initialize_historique( historique );
    Joueur joueur = {"toto", false, false };
    // mock with wrap much more flexible
    will_return(__wrap_ask_yesno_question, true);  // premiere fois rejouer
    will_return(__wrap_ask_yesno_question, false); // deuxieme fois quitter
    //when
    int retour_game = start_game_mastermind( joueur, historique );
    // then
    assert_int_equal( 3, retour_game ); // 3 just for test
    assert_int_equal( 2, historique->nbre_victoire_mm );
    assert_int_equal( 0, historique->nbre_defaite_mm );
}

/****************
    Main fonction
****/
int main()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(null_test_success),
        cmocka_unit_test( win_one_partie_increment1_nb_victoire_mm ),
        cmocka_unit_test( win_2games_increment2 ),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
