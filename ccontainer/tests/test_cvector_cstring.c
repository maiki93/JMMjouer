/* import for CMocka */
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include "cmocka.h"

#include <string.h>
#include "ccontainer/cvector_cstring.h"
#include "ccontainer/ccontainer_utils.h" /* for helper carray_cstring_delete or utils ?*/

/* string insertion */
const char string1[] = "first";
const char* string2 = "second";
const char* string3 = "tree";

cvector_cstring_t* vect_cstring_heap = NULL;

static int setup_heap(void **state)
{
    (void) state;
    // allocation on heap, allocate + default constructor
    vect_cstring_heap = cvector_cstring_new();
    cvector_cstring_init( vect_cstring_heap );  
    return 0;
}

static int teardown_heap(void **state)
{
    (void) state;
    /* call destructor clist_clear & deallocate */
    cvector_cstring_free( vect_cstring_heap );
    /* to protect later use */
    vect_cstring_heap = NULL;
    return 0;
}

static void initialization_on_stack()
{
    // allocation on stack, call constructor
    cvector_cstring_t vect_cstr;
    // init default
    cvector_cstring_init( &vect_cstr);
    assert_int_equal( 0, cvector_cstring_size( &vect_cstr) );
    assert_int_equal( 0, cvector_cstring_capacity( &vect_cstr) );
    // deallocation on stack, call destructor
    cvector_cstring_delete( &vect_cstr );
    // init capacity 3
    cvector_cstring_init_with_capacity( &vect_cstr, 3 );
    assert_int_equal( 0, cvector_cstring_size( &vect_cstr) );
    assert_int_equal( 3, cvector_cstring_capacity( &vect_cstr) );
    cvector_cstring_delete( &vect_cstr );
}

static void initialization_on_heap()
{
    // allocation on heap, allocate + default constructor
    cvector_cstring_t *vect_cstr = cvector_cstring_new();
    cvector_cstring_init( vect_cstr );
    assert_int_equal( 0, cvector_cstring_size(vect_cstr) );
    assert_int_equal( 0, cvector_cstring_capacity(vect_cstr) );
    // call destructor cvector_clear & deallocate
    cvector_cstring_free( vect_cstr );

    // init with capacity 3
    vect_cstr = cvector_cstring_new();
    cvector_cstring_init_with_capacity( vect_cstr, 5 );
    assert_int_equal( 0, cvector_cstring_size(vect_cstr) );
    assert_int_equal( 5, cvector_cstring_capacity(vect_cstr) );

    cvector_cstring_free( vect_cstr );
}

static void push_back_and_clear_string()
{
    ccontainer_err_t err_code;
    // allocation done in setup
    // push one
    err_code = cvector_cstring_push_back(vect_cstring_heap, string1);
    assert_int_equal( CCONTAINER_OK, err_code );
    assert_int_equal( 1, cvector_cstring_size( vect_cstring_heap) );
    assert_int_equal( 1, cvector_cstring_capacity( vect_cstring_heap) );
    // push a second
    err_code = cvector_cstring_push_back(vect_cstring_heap, string2);
    assert_int_equal( CCONTAINER_OK, err_code);
    assert_int_equal( 2, cvector_cstring_size( vect_cstring_heap));
    err_code = cvector_cstring_push_back(vect_cstring_heap, string3);
    assert_int_equal( CCONTAINER_OK, err_code);
    assert_int_equal( 3, cvector_cstring_size( vect_cstring_heap ));
    assert_int_equal( 3, cvector_cstring_capacity( vect_cstring_heap) );

    cvector_cstring_clear( vect_cstring_heap );
    // still one in cvector
    assert_int_equal( 0, cvector_cstring_size( vect_cstring_heap));
    assert_int_equal( 3, cvector_cstring_capacity( vect_cstring_heap));
    //deallocation vect_cstring_heap done in teardown
}

static void get_and_get_ref_at()
{
    ccontainer_err_t err_code;
    const char *ref_string;
    char *copy_string;
    // created on setup
    assert_non_null( vect_cstring_heap );
    // push : first, econd and third
    cvector_cstring_push_back(vect_cstring_heap, string1);
    cvector_cstring_push_back(vect_cstring_heap, string2);
    cvector_cstring_push_back(vect_cstring_heap, "third");

    copy_string = cvector_cstring_get_copy_at( vect_cstring_heap, 1, &err_code);
    assert_int_equal( CCONTAINER_OK, err_code);
    assert_string_equal( "second", copy_string );
    // must deallocate the copy
    free(copy_string);

    ref_string = cvector_cstring_get_ref_at( vect_cstring_heap, 2, &err_code);
    assert_int_equal( CCONTAINER_OK, err_code);
    assert_string_equal( "third", ref_string );
    // ref on constant string, no memory allocation
    ref_string = NULL;
    
}

static void get_array()
{
    char **p_array = NULL;
    size_t array_len = 0;
    int retour;
    // push : first, econd and third
    cvector_cstring_push_back(vect_cstring_heap, string1);
    cvector_cstring_push_back(vect_cstring_heap, string2);
    cvector_cstring_push_back(vect_cstring_heap, "third");

    retour = cvector_cstring_get_array( vect_cstring_heap, &p_array, &array_len );
    assert_int_equal( CCONTAINER_OK, retour);
    assert_int_equal( 3, array_len);

    assert_string_equal("first", p_array[0]);
    assert_string_equal("second", p_array[1]);
    assert_string_equal("third", p_array[2]);

    // explicit delete of the copies, defined in cvector_cstring
    carray_cstring_delete( p_array, array_len );
    p_array = NULL;
    //deallocation vect_cstring_heap done in teardown
}

/*
static void get_array_error_empty()
{
    char **p_array = NULL;
    size_t array_len = 0;
    int retour;

    retour = cvect_cstring_get_array( vect_cstring_heap, &p_array, &array_len );
    assert_int_equal( CCONTAINER_EMPTY, retour);
    assert_int_equal( 0, array_len);
}
*/

int main()
{
    /* can be inside main, or as global. here no problem with identical name */
    const struct CMUnitTest tests_cvector_cstring[] = {
        cmocka_unit_test(initialization_on_stack),
        cmocka_unit_test(initialization_on_heap),
        cmocka_unit_test_setup_teardown(push_back_and_clear_string, setup_heap, teardown_heap),
        cmocka_unit_test_setup_teardown(get_and_get_ref_at, setup_heap, teardown_heap),
        cmocka_unit_test_setup_teardown(get_array, setup_heap, teardown_heap),
        //cmocka_unit_test_setup_teardown(get_array_error_empty,setup_heap, teardown_heap),
    };

    /* call group_setup and teardown at the very beginning and end */
    /* return cmocka_run_group_tests(tests_historique, group_setup, group_teardown);*/
    return cmocka_run_group_tests(tests_cvector_cstring, NULL, NULL);
}