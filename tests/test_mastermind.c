#include <stdio.h>
#include <stdlib.h>

// import for CMocka
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>

#include <cmocka.h>

// declare enum ELT_JEU and fonction to test
#include "../game_mastermind.h"

/***************
    Found 2 librairies for specific C testing and active maintenance/dev :
    CMocka available with CMake
        https://blog.microjoe.org/2017/unit-tests-c-cmocka-coverage-cmake.html
        https://stackoverflow.com/questions/48412862/unit-test-run-configuration // makefile ideas
        https://www.linuxtut.com/en/c287eb805c7c56c88dbb/
    Check

    Main  function to test, business model
    void mm_algo_mastermind(const char* const p_tab_guess, const char* const p_tab_code_secret, const int size_piece, int *nb_bien_place, int *nb_mal_place )
****/

////// functions
/* A test case that does nothing and succeeds. from Tuto */
static void null_test_success(void **state) {
    (void) state; /* unused  here*/
}

static void test_RRVB_RRBM(void **state) {
    (void) state; /* for setup, fixture */
    // given
    char secret[4] = {'R','R','V','B'};
    char guess[4] =  {'R','R','B','M'};
    int nb_bien=0, nb_mal=0;
    // when
    mm_algo_mastermind(guess, secret, 4, &nb_bien, &nb_mal );
    // then
    assert_int_equal( 2, nb_bien );
    assert_int_equal( 1, nb_mal);
}

static void test_RRRJ_JRRR(void **state) {
    (void) state; /*  for setup, fixture  */
    // given
    char secret[4] = {'R','R','R','J'};
    char guess[4] =  {'J','R','R','R'};
    int nb_bien=0, nb_mal=0;
    // when
    mm_algo_mastermind(guess, secret, 4, &nb_bien, &nb_mal );
    // then
    assert_int_equal( 2, nb_bien );
    assert_int_equal( 2, nb_mal);
}
//void ciseau_gagne_contre_feuille() {
//    enum ELT_JEU user_choice = CISEAU;
//    enum ELT_JEU computer_choice = FEUILLE;
//
//    int gagnant = designe_gagnant( user_choice, computer_choice );
//    assert_int_equal( 1, gagnant);
//}

/****************
    Main fonction
****/
int main()
{
    // this can be moved, maybe shared amongst *c file ?
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(null_test_success),
        cmocka_unit_test(test_RRVB_RRBM),
        cmocka_unit_test(test_RRRJ_JRRR),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
