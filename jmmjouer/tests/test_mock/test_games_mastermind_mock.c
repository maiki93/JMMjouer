#include <stdio.h>
#include <stdlib.h>
// import for CMocka
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

/*************************************
    unit test of start_game_mastermin : win must increase the historique nb_victoire_mm

    compilation :
    gcc -Wall -g  -c tests/test_games_mastemrind_mock.c -o tests/test_games_mastemrind_mock.o
    gcc  -o test_games_mastermind_mock game_mastermind.o historique.o test_games_mastemrind_mock.o utils.o  -Wl,--wrap=ask_yesno_question  -lcmocka
**************************************/

// Need to mock mm_make_one_game, same compilation unit than start_game_mastermind
//    => must redefine inital function marked as __attribute__((weak))
//#include "../game_mastermind.c"  // cannot incude the implementation file like typical cmocka tests
#include "../../game_mastermind.h"  // dependent where is placed the test executable, here tests/test_mock


/******************
    Mock functions
******************/

// working with function mocked in the same file implementation file that the function to test.
// need __attribute__((weak)) on the mocked function, no need of -Wl,--wrap=mm_make_one_game in linker
// BUT cannot really mock, change the return value for instance. not very flexible !!
//       => other behavior other file and implemntation, maybe a DEFINE could help
int mm_make_one_game( const char* colors, const int SizeColor, const int NbPIECE, const int MaxTENTATIVE, const bool mode_daltonien )
{
    //printf("redefine make_one_game as a mock, always return a victory\n");
    // we could DEFINE a code compile time value  MM_GAMES_VICTOIRE_JOUEUR  1 / other file MM_.. 2
    return 1;
}

// it is in utls*h/c, __wrap_  work, can test the results of 1..n games
// if wanted to cal lthe real function, use __real_ask_yesno_qestion
// if we comment this part of the code (and no link option) it should work normmally TO TEST
bool __wrap_ask_yesno_question(const char* message)
{
    //printf("enter mocked yesno question\n");
    check_expected( message );
    return mock();
}

/******************
    tests functions
******************/
static void win_one_partie_increment1_nb_victoire_mm( void **state )
{
    (void) state;
    // given
    Historique* historique = malloc( sizeof(Historique) );
    initialize_historique( historique );
    Joueur joueur = {"toto", false, false };
    // mock setup with wrap

    will_return(__wrap_ask_yesno_question, false); // do not want to play again
    expect_string(__wrap_ask_yesno_question, message, "Voulez-vous refaire une partie [y/Y/n/N] > " );
    //when
    int retour_game = start_game_mastermind( joueur, historique );
    // then
    assert_int_equal( 1, retour_game ); // always return 1
    assert_int_equal( 1, historique->nbre_victoire_mm );
    assert_int_equal( 0, historique->nbre_defaite_mm );
}

/*** limit of overiding weak methods ***/
// not possible, because mm_make_one_game is simply overriden locally
// void lose_one_partie_increment1_nb_defaite_mm(void **state)
// Possible , but compile time macro DEFINE to use. CMake could help to make this easy ?

// if play 2 games, increments 2 times. Use mock to make an other game (rejouer une fois)
static void win_2games_increment2( void **state )
{
    (void) state;
    // given
    Historique* historique = malloc( sizeof(Historique) );
    initialize_historique( historique );
    Joueur joueur = {"toto", false, false };
    // mock with wrap much more flexible
    //expect_string(__wrap_ask_yesno_question, message, "Voulez-vous refaire une partie [y/Y/n/N] > " ); // mandatory to call 2 times with expect_string
    //expect_string(__wrap_ask_yesno_question, message, "Voulez-vous refaire une partie [y/Y/n/N] > " );
    expect_string_count(__wrap_ask_yesno_question, message, "Voulez-vous refaire une partie [y/Y/n/N] > ", 2);
    will_return(__wrap_ask_yesno_question, true);  // premiere fois rejouer
    will_return(__wrap_ask_yesno_question, false); // deuxieme fois quitter

    //when
    int retour_game = start_game_mastermind( joueur, historique );
    // then
    assert_int_equal( 1, retour_game );
    assert_int_equal( 2, historique->nbre_victoire_mm );
    assert_int_equal( 0, historique->nbre_defaite_mm );
}

/****************
    Main fonction
****/
int main()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test( win_one_partie_increment1_nb_victoire_mm ),
        cmocka_unit_test( win_2games_increment2 ),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
