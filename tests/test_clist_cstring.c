/* import for CMocka */
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include "cmocka.h"

#include <stdio.h> /*only for printf, may be commented */
#include <string.h>

#include "../clist_cstring.h"

/* string insertion */
const char string1[] = "first";
const char* string2 = "second";
/* string retrieval */
char* get_string1;
char *get_string2;

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
    clist_cstring_delete( lcstring_heap );
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
    clist_cstring_clear( &list_cstr );
    /*assert_int_equal( 0, clist_cstring_size( &list_cstr) );*/
}

static void initialization_on_heap()
{
    // allocation on heap, allocate + constructor
    clist_cstring_t *list_cstr = clist_cstring_new();
    assert_null( list_cstr->clist);

    clist_cstring_init( list_cstr );
    assert_non_null( list_cstr->clist );
    assert_int_equal( 0, clist_cstring_size(list_cstr) );
    /* no acces, private */
    /* assert_null ( list_cstr->clist->first_node ); */

    /* call destructor clist_clear & deallocate */
    clist_cstring_delete( list_cstr ); 
    /* cannot access member of list_cstr now */
    /* assert_null( list_cstr->clist); */
    list_cstr = NULL; /* to protect later use */
    assert_null( list_cstr );
}

/* insert one string by copy and retrieve one string */
static void push_back_two_strings_get_copy()
{
    // allocation on stack, call constructor
    clist_cstring_t list_cstr;
    char *string_out = NULL;
    int retour;

    clist_cstring_init( &list_cstr);
    retour = clist_cstring_push_back(&list_cstr, string1);
    assert_int_equal( CLIST_OK, retour );
    assert_int_equal( 1, clist_cstring_size(&list_cstr) );

    retour = clist_cstring_get_copy( &list_cstr, 0, &string_out );
    assert_int_equal( CLIST_OK, retour);
    assert_string_equal( "first", string_out);
    
    // deallocation on stack, call destructor
    clist_cstring_clear( &list_cstr );
    /* cannot access anymore */
    /*assert_int_equal( 0, clist_cstring_size( &list_cstr) );*/

    // local allocated memory 
    free(string_out);
    string_out = NULL;
}

static void push_back_two_strings_get_ref()
{
    int retour;
    char * string_out = NULL;
    // push one
    clist_cstring_push_back(lcstring_heap, string1);

    retour = clist_cstring_get_ref( lcstring_heap, 0, &string_out);
    assert_int_equal( CLIST_OK, retour);
    assert_memory_equal( "first", string_out, 5);
    assert_int_equal(5, strlen(string_out));

    // only one, return not found
    retour = clist_cstring_get_ref( lcstring_heap, 1, &string_out);
    assert_int_equal( CLIST_OUTOFRANGE, retour);

    // push a second
    clist_cstring_push_back(lcstring_heap, string2);
    retour = clist_cstring_get_ref( lcstring_heap, 1, &string_out);
    assert_int_equal( CLIST_OK, retour);
    assert_memory_equal( "second", string_out, 6);
    assert_int_equal(6, strlen(string_out));
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
    assert_int_equal( CLIST_OK, retour);
    assert_int_equal( 3, array_len);

    assert_string_equal("first", p_array[0]);
    assert_string_equal("third", p_array[2]);
    assert_string_equal("second", p_array[1]);
    // explicit delete of the copies, need function api ? yes certainly
    carray_cstring_delete( p_array, 3 );
    p_array = NULL;
}

static void get_array_error_empty()
{
    char **p_array = NULL;
    size_t array_len = 0;
    int retour;

    retour = clist_cstring_get_array( lcstring_heap, &p_array, &array_len );
    assert_int_equal( CLIST_EMPTY, retour);
    assert_int_equal( 0, array_len);
}


int main()
{
    /* can be inside main, or as global. here no problem with identical name */
    const struct CMUnitTest tests_clist_cstring[] = {
        cmocka_unit_test(initialization_on_stack),
        cmocka_unit_test(initialization_on_heap),
        cmocka_unit_test(push_back_two_strings_get_copy),
        cmocka_unit_test_setup_teardown(push_back_two_strings_get_ref,setup_heap, teardown_heap),
        cmocka_unit_test_setup_teardown(get_array, setup_heap, teardown_heap),
        cmocka_unit_test_setup_teardown(get_array_error_empty,setup_heap, teardown_heap),
    };

    /* call group_setup and teardown at the very beginning and end */
    /* return cmocka_run_group_tests(tests_historique, group_setup, group_teardown);*/
    return cmocka_run_group_tests(tests_clist_cstring, NULL, NULL);
}