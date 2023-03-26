#include <stdio.h>
#include <stdlib.h>

// import for CMocka
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>

#include <cmocka.h>

/* in  fact not used, mm_algo not declared in header
    2nde version, include directly *.c file */
#include "jmmjouer/games/game_mastermind.c"

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

/***** test mm_algo function *****/
/* A test case that does nothing and succeeds. from Tuto */
static void null_test_success(void **state) {
    (void) state; /* unused  here*/
}

static void test_RRVB_RRBM(void **state) {
    (void) state; /* for setup, fixture */
    /* given */
    char secret[4] = {'R','R','V','B'};
    char guess[4] =  {'R','R','B','M'};
    int nb_bien=0, nb_mal=0;
    /* when */
    mm_algo_mastermind(guess, secret, 4, &nb_bien, &nb_mal );
    /* then */
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

static void test_VRVJ_JVJR(void **state) {
    (void) state; /*  for setup, fixture  */
    // given
    char secret[4] = {'V','R','V','J'};
    char guess[4] =  {'J','V','J','R'};
    int nb_bien=0, nb_mal=0;
    // when
    mm_algo_mastermind(guess, secret, 4, &nb_bien, &nb_mal );
    // then
    assert_int_equal( 0, nb_bien );
    assert_int_equal( 3, nb_mal);
}

static void test_BRBB_BBBB(void **state) {
    (void) state; /*  for setup, fixture  */
    // given
    char secret[4] = {'B','R','B','B'};
    char guess[4] =  {'B','B','B','B'};
    int nb_bien=0, nb_mal=0;
    // when
    mm_algo_mastermind(guess, secret, 4, &nb_bien, &nb_mal );
    // then
    assert_int_equal( 3, nb_bien );
    assert_int_equal( 0, nb_mal);
}

static void test_BBBB_BRBB(void **state) {
    (void) state; /*  for setup, fixture  */
    // given
    char secret[4] = {'B','B','B','B'};
    char guess[4] =  {'B','R','B','B'};
    int nb_bien=0, nb_mal=0;
    // when
    mm_algo_mastermind(guess, secret, 4, &nb_bien, &nb_mal );
    // then
    assert_int_equal( 3, nb_bien );
    assert_int_equal( 0, nb_mal);
}

static void test_BBRR_JRBB(void **state) {
    (void) state; /*  for setup, fixture  */
    // given
    char secret[4] = {'B','B','R','R'};
    char guess[4] =  {'J','R','B','B'};
    int nb_bien=0, nb_mal=0;
    // when
    mm_algo_mastermind(guess, secret, 4, &nb_bien, &nb_mal );
    // then
    assert_int_equal( 0, nb_bien );
    assert_int_equal( 3, nb_mal);
}

/***** test start_game_mastermind : correct update of nb_victoire / defaite *****/
// mock une partie


/****************
    Main fonction
****/
int main()
{
    // this can be moved, maybe shared amongst *c file ? seems not so easy. not expected it seems
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(null_test_success),
        cmocka_unit_test(test_RRVB_RRBM),
        cmocka_unit_test(test_RRRJ_JRRR),
        cmocka_unit_test(test_VRVJ_JVJR),
        cmocka_unit_test(test_BRBB_BBBB),
        cmocka_unit_test(test_BBBB_BRBB),
        cmocka_unit_test(test_BBRR_JRBB),
    };
    
    int return_test_algo = cmocka_run_group_tests(tests, NULL, NULL);
    return return_test_algo;
}
