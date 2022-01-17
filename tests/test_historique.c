#include <stdio.h>
#include <stdlib.h>
// import for CMocka
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

// project dependence
// adviced by cmocka, can test static function without problems.
// no need of Wno-implicit-function-declaration, include  *.h by
#include "../historique.c"

// global values, can be managed by **state ? good, bad ??
Historique* histo;
//Joueur joueur; why not compiling ?
//extern const char * FILENAME;  // without historique.c include can change the filename of the backup file, nice !
                                 // with include : in setup (or to test in FixtureSetup )

// run only once, before all tests
static int group_setup(void **state)
{
    (void) state;
    // will pick the file for testing only
    FILENAME = "historique_test.txt";
    return 0;
}

static int group_teardown(void **state)
{
    (void) state;
    FILENAME = "historique.txt";
    return 0;
}

// setup and teardown, allocate memory for Historique
// some examples use state to pass the value between setup and teardown. Allow to avoid global data in the file ?
// https://www.linuxtut.com/en/c287eb805c7c56c88dbb/
static int test_setup(void **state) {
    (void) state; // avoid warning not used variable
    histo = malloc( sizeof(Historique) );
    initialize_historique( histo );
    return 0;
}

static int test_teardown(void **state) {
    (void) state; // avoid warning not used variable
    free(histo);
    histo = NULL;
    return 0;
}

static void search_existing_joueur_load_historique(void **state) {
    (void) state; // avoid warning not used variable
    Joueur joueur = {"toto",true,false};
    bool new_joueur = search_joueur_in_historique(histo, &joueur);
    //assert value of joueur toto, and its performance
    assert_false( new_joueur );
    assert_false( joueur.is_daltonien);
    assert_int_equal( histo->nbre_victoire_mm, 2);
    assert_int_equal( histo->nbre_defaite_morpion, 3);
}

static void search_a_new_joueur_provide_default_historique(void **state) {
    (void) state; // avoid warning not used variable
    Joueur joueur = {"titi",true,false};
    bool new_joueur = search_joueur_in_historique(histo, &joueur);
    //assert value of new joueur titi, and its performance
    assert_true( new_joueur );
    assert_true( joueur.is_daltonien);              // keep original value
    assert_int_equal( histo->nbre_victoire_mm, 0);  // default value new joueur
    assert_int_equal( histo->nbre_defaite_morpion, 0);
}

// bug mic / michael :> michael matches first
static void search_joueur_mic_and_not_michael(void **state) {
    (void) state; // avoid warning not used variable
    Joueur joueur = {"mic",true,false};
    bool new_joueur = search_joueur_in_historique(histo, &joueur);
    //then
    assert_false( new_joueur );
    assert_true( joueur.is_daltonien);
    assert_int_equal( histo->nbre_victoire_mm, 4);
    assert_int_equal( histo->nbre_defaite_morpion, 2);
    assert_int_equal( histo->nbre_defaite_pendu, 3);
}

/****************
    Main fonction
****/
int main()
{
    // can be inside main, or as global. here no problem with identical name
    const struct CMUnitTest tests_historique[] = {
        cmocka_unit_test_setup_teardown(search_existing_joueur_load_historique, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(search_a_new_joueur_provide_default_historique, test_setup, test_teardown),
        cmocka_unit_test_setup_teardown(search_joueur_mic_and_not_michael, test_setup, test_teardown),
    };
    // apply only setup and teardown at the beginning and end : e.g. change the FILENAME
    return cmocka_run_group_tests(tests_historique, group_setup, group_teardown);
}
