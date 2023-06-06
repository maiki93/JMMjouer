/* import for CMocka */
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include "cmocka.h"

/*#include <stdio.h>*/ /*only for printf, may be commented */
#include <string.h>

#include "ccontainer/clist_cstring.h"
#include "ccontainer/ccontainer_utils.h"

/* string insertion */
const char string1[] = "first";
const char* string2 = "second";
const char* string3 = "tree";
/* string retrieval */
char* get_string1;
//char *get_string2;

clist_cstring_t* lcstring_heap = NULL;

static int setup_heap(void **state)
{
    (void) state;
    // allocation on heap, allocate + constructor
    lcstring_heap = clist_cstring_new();
    assert_null( lcstring_heap->clist);

    clist_cstring_init( lcstring_heap );
    assert_non_null( lcstring_heap->clist );
    assert_int_equal( 0, clist_cstring_size(lcstring_heap) );
    return 0;
}

static int teardown_heap(void **state)
{
    (void) state;
    /* call destructor clist_clear & deallocate */
    clist_cstring_free( lcstring_heap );
    /* cannot access clist anymore */
    /*assert_null( lcstring_heap->clist);*/
    lcstring_heap = NULL; /* to protect later use */
    assert_null( lcstring_heap );
    return 0;
}

static void initialization_on_stack()
{
    // allocation on stack, call constructor
    clist_cstring_t list_cstr;
    clist_cstring_init( &list_cstr);

    assert_non_null( list_cstr.clist );
    assert_int_equal( 0, clist_cstring_size( &list_cstr) );
    
    // deallocation on stack, call destructor
    clist_cstring_delete( &list_cstr );
    assert_null( list_cstr.clist );
}

static void initialization_on_heap()
{
    // allocation on heap, allocate + constructor
    clist_cstring_t *list_cstr = clist_cstring_new();
    assert_null( list_cstr->clist);

    clist_cstring_init( list_cstr );
    assert_non_null( list_cstr->clist );
    assert_int_equal( 0, clist_cstring_size(list_cstr) );
    
    // call destructor clist_clear & deallocate
    clist_cstring_free( list_cstr ); 
    list_cstr = NULL; // to protect later use
}

static void push_back_strings_and_pop_front()
{
    ccontainer_err_t err_code;
    char * string_out = NULL;
    // allocation done in setup
    // push one
    err_code = clist_cstring_push_back(lcstring_heap, string1);
    assert_int_equal( CCONTAINER_OK, err_code );
    assert_int_equal( 1, clist_cstring_size( lcstring_heap) );
    // push a second
    err_code = clist_cstring_push_back(lcstring_heap, string2);
    assert_int_equal( CCONTAINER_OK, err_code);
    assert_int_equal( 2, clist_cstring_size( lcstring_heap ));
    err_code = clist_cstring_push_back(lcstring_heap, string3);
    assert_int_equal( CCONTAINER_OK, err_code);
    assert_int_equal( 3, clist_cstring_size( lcstring_heap ));

    string_out = clist_cstring_pop_front(lcstring_heap, &err_code);
    assert_memory_equal( "first", string_out, 5);
    assert_string_equal( "first", string_out);
    assert_int_equal(5, strlen(string_out));
    // it is a copy, must be free
    free( string_out );
    string_out = NULL;

    string_out = clist_cstring_pop_front(lcstring_heap, &err_code);
    assert_memory_equal( "second", string_out, 6);
    assert_string_equal( "second", string_out);
    assert_int_equal(6, strlen(string_out));
    free( string_out );
    string_out = NULL;
    
    // still one in clist
    assert_int_equal( 1, clist_cstring_size( lcstring_heap ));
    //deallocation lcstring_heap done in teardown
}

static void get_array()
{
    char **p_array = NULL;
    size_t array_len = 0;
    int retour;
    // push : first, econd and third
    clist_cstring_push_back(lcstring_heap, string1);
    clist_cstring_push_back(lcstring_heap, string2);
    clist_cstring_push_back(lcstring_heap, "third");

    retour = clist_cstring_get_array( lcstring_heap, &p_array, &array_len );
    assert_int_equal( CCONTAINER_OK, retour);
    assert_int_equal( 3, array_len);

    assert_string_equal("first", p_array[0]);
    assert_string_equal("second", p_array[1]);
    assert_string_equal("third", p_array[2]);

    // explicit delete of the copies, defined in clist_cstring
    carray_cstring_delete( p_array, 3 );
    p_array = NULL;
    //deallocation lcstring_heap done in teardown
}

/*
static void get_array_error_empty()
{
    char **p_array = NULL;
    size_t array_len = 0;
    int retour;

    retour = clist_cstring_get_array( lcstring_heap, &p_array, &array_len );
    assert_int_equal( CCONTAINER_EMPTY, retour);
    assert_int_equal( 0, array_len);
}
*/

int main()
{
    /* can be inside main, or as global. here no problem with identical name */
    const struct CMUnitTest tests_clist_cstring[] = {
        cmocka_unit_test(initialization_on_stack),
        cmocka_unit_test(initialization_on_heap),
        cmocka_unit_test_setup_teardown(push_back_strings_and_pop_front, setup_heap, teardown_heap),
        cmocka_unit_test_setup_teardown(get_array, setup_heap, teardown_heap),
        //cmocka_unit_test_setup_teardown(get_array_error_empty,setup_heap, teardown_heap),
    };

    /* call group_setup and teardown at the very beginning and end */
    /* return cmocka_run_group_tests(tests_historique, group_setup, group_teardown);*/
    return cmocka_run_group_tests(tests_clist_cstring, NULL, NULL);
}