// import for CMocka
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>

#include <cmocka.h>

#include "clogger/clogger.h"

static void create_new_file() 
{
    int retour;
    retour = init_clogger("unit_test_logger.log");
    assert_int_equal(0, retour);
    close_clogger();
    assert_int_equal(1,1);
}

int main()
{
    /* can be inside main, or as global. here no problem with identical name */
    const struct CMUnitTest tests_clogger[] = {
        cmocka_unit_test(create_new_file),
    };

    /* call group_setup and teardown at the very beginning and end */
    /*return cmocka_run_group_tests(tests_clogger, setup, teardown);*/
    return cmocka_run_group_tests(tests_clogger, NULL, NULL);
}
