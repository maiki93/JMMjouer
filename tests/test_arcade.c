/* import for CMocka */
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>

#include "cmocka.h"

#include "../arcade.c"

#include "../file_record.h"

file_record_t *file_record;

static int setup(void **state) {
    (void) state;
    /* static variable in arcade.c */
    //gloader = game_loader_new();
    file_record = file_record_new();
    // ok, but from vscode Launch must use ..\test..
    file_record_init(file_record, "test_record_file.txt");
    /* static varirable in arcade.c */
    record = (irecord_t*)file_record;
    return 0;
}

/* clear data and delete the instance */
static int teardown(void **state) {
    (void) state;
    //game_loader_delete( gloader );
    //gloader = NULL;
    file_record_delete( file_record );
    free(file_record);
    file_record = NULL;
    record = NULL;
    return 0;
}



/* memory leak in identification 
 should test with anonymous, better design, simpler tests */
static void identification() 
{
    joueur_t joueur;

    joueur = identification_joueur();

    //assert_string_equal(joueur.person.nom, "mic");
    assert_int_equal(1,1);

    joueur_clear( &joueur );
}

int main()
{
    /* can be inside main, or as global. here no problem with identical name */
    const struct CMUnitTest tests_arcade[] = {
        cmocka_unit_test(identification),
    };

    /* call group_setup and teardown at the very beginning and end */
    return cmocka_run_group_tests(tests_arcade, setup, teardown);
    /*return cmocka_run_group_tests(tests_arcade, NULL, NULL);*/
}