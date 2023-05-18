/* import for CMocka */
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include "cmocka.h"

#include "ccontainer/clist_generic.c"

/* TODO Check comparison by string, when possible/not */
/* MSVC void **state argument or warning c4113 */

/* maybe a default deleter to give to user ? 
   works until value does not contain allocated memory himself */
/*
void deleter_value( ccontainer_value_t *value)
{
    if( value ) {
        free(value->data);
        value->data = NULL;
    }
    value->len = 0;
}
*/

/* const data used by test, use bytes no specifically string */
ccontainer_value_t value_str1 = {"first", 5}; // 5: do not include '\0'
ccontainer_value_t value_str2 = {"second", 6};

static void initialization_on_stack()
{
    // allocation on stack, call only constructor
    clist_gen_t clist;
    clist_gen_init( &clist);

    assert_int_equal( 0, clist.len);
    assert_null( clist.first_node);
    // deallocation on stack, call destructor
    // nothing to delete here
    clist_gen_delete( &clist, ccontainer_delete_value);
    assert_int_equal(0, clist.len);
}

static void initialization_on_heap()
{
    // allocation on heap, allocate + constructor
    clist_gen_t *clist = clist_gen_new();
    clist_gen_init( clist );

    assert_int_equal( 0, clist->len );
    assert_null ( clist->first_node );
    /* call destructor clist_clear & deallocate */
    clist_gen_free( clist, ccontainer_delete_value );
    clist = NULL; /* to protect later use */
}

static void add_one_str_as_value()
{
    ccontainer_err err_code;
    ccontainer_value_t tmp_value_in;
    clist_gen_t clist;
    clist_gen_init( &clist );

    // make a copy
    tmp_value_in = ccontainer_make_value(value_str1.data, value_str1.len, &err_code);
    assert_int_equal( CCONTAINER_OK, err_code);
    // transfer into the list
    err_code = clist_gen_push_front( &clist, &tmp_value_in );
    assert_int_equal( CCONTAINER_OK, err_code);

    assert_non_null(clist.first_node);
    assert_null(clist.first_node->next_node);
    assert_int_equal(1, clist.len);

    /* clean deletion on stack */
    clist_gen_delete(&clist, ccontainer_delete_value);
    assert_int_equal(0, clist.len);
    assert_null(clist.first_node);
}

static void add_two_str_as_value()
{
    ccontainer_err err_code;
    ccontainer_value_t tmp_value_in;
    clist_gen_t clist;
    clist_gen_init( &clist );

    tmp_value_in = ccontainer_make_value(value_str1.data, value_str1.len, &err_code);
    err_code = clist_gen_push_front( &clist, &tmp_value_in ); // "first" , 5
    assert_int_equal( CCONTAINER_OK, err_code);
    tmp_value_in = ccontainer_make_value(value_str2.data, value_str2.len, &err_code);
    err_code = clist_gen_push_front( &clist, &tmp_value_in ); // "second", 6
    assert_int_equal( CCONTAINER_OK, err_code);

    assert_non_null(clist.first_node);
    assert_non_null(clist.first_node->next_node);
    assert_int_equal(2, clist.len);

    /* clean deletion on stack */
    clist_gen_delete( &clist, ccontainer_delete_value);
    assert_null(clist.first_node);
    assert_int_equal(0, clist.len);
}

static void add_last_two_str_as_value()
{
    ccontainer_err err_code;
    ccontainer_value_t tmp_value_in;
    clist_gen_t *clist = clist_gen_new();
    clist_gen_init( clist );

    tmp_value_in = ccontainer_make_value(value_str1.data, value_str1.len, &err_code);
    err_code = clist_gen_push_back( clist, &tmp_value_in );
    assert_int_equal( CCONTAINER_OK, err_code);
    tmp_value_in = ccontainer_make_value(value_str2.data, value_str2.len, &err_code);
    assert_int_equal( CCONTAINER_OK, err_code);
    err_code = clist_gen_push_back( clist, &tmp_value_in );

    assert_non_null(clist->first_node);
    assert_non_null(clist->first_node->next_node);
    assert_int_equal(2, clist->len);
    // destructor + deallocation on heap
    clist_gen_free( clist, ccontainer_delete_value );
    clist = NULL;
}

static void get_element_by_index() 
{
    ccontainer_err err_code;
    ccontainer_value_t tmp_value_in;
    clist_gen_t clist;
    ccontainer_value_t* pvalue_out; /* value by ref */
    clist_gen_init( &clist );

    // push_front => invert order : "second" then "first"
    tmp_value_in = ccontainer_make_value(value_str1.data, value_str1.len, &err_code);
    clist_gen_push_front( &clist, &tmp_value_in );
    tmp_value_in = ccontainer_make_value(value_str2.data, value_str2.len, &err_code);
    clist_gen_push_front( &clist, &tmp_value_in );

    pvalue_out = clist_gen_get_at( &clist, 0, &err_code);
    /* by reference, no terminaison character so compare the first bytes */
    assert_memory_equal( "second", pvalue_out->data, pvalue_out->len );
    assert_int_equal( 6, pvalue_out->len);
    
    pvalue_out = clist_gen_get_at( &clist, 1, &err_code);
    assert_int_equal( CCONTAINER_OK, err_code );
    assert_memory_equal( "first", pvalue_out->data, pvalue_out->len );
    assert_int_equal( 5, pvalue_out->len);
    // temporary pointer
    pvalue_out = NULL;

    clist_gen_delete( &clist, ccontainer_delete_value );
    assert_null(clist.first_node);
    assert_int_equal(0, clist.len);
}


static void find_element_by_comparison() 
{
    ccontainer_err err_code;
    ccontainer_value_t tmp_value_in;
    clist_gen_t clist;
    ccontainer_value_t value_str_local, value_str_local2; 
    ccontainer_value_t* pvalue_out; /* tmp ref to value */
    char buffer_search[4] = "sec";
    
    clist_gen_init( &clist );

    value_str_local.data="first";
    value_str_local.len = 5; /* strlen */
    tmp_value_in = ccontainer_make_value(value_str_local.data, value_str_local.len, &err_code);
    err_code = clist_gen_push_front( &clist, &tmp_value_in );
    assert_int_equal( CCONTAINER_OK, err_code );

    value_str_local2.data="second";
    value_str_local2.len = 6; /* strlen + 1 */
    tmp_value_in = ccontainer_make_value(value_str_local2.data, value_str_local2.len, &err_code);
    err_code = clist_gen_push_front( &clist, &tmp_value_in );

    // one of more generic possible, fill a ccontainer_value_t for comparison
    // a function could be nice also
    pvalue_out = clist_gen_find( &clist, buffer_search, 3, &err_code );

    assert_int_equal( CCONTAINER_OK, err_code );
    assert_memory_equal("second", pvalue_out->data, pvalue_out->len);
    assert_int_equal( 6, pvalue_out->len);
    pvalue_out = NULL;
    
    clist_gen_delete( &clist, ccontainer_delete_value );
}

int main()
{
    /* can be inside main, or as global. here no problem with identical name */
    const struct CMUnitTest tests_clist_generic[] = {
        cmocka_unit_test(initialization_on_stack),
        cmocka_unit_test(initialization_on_heap),
        cmocka_unit_test(add_one_str_as_value),
        cmocka_unit_test(add_two_str_as_value),
        cmocka_unit_test(add_last_two_str_as_value),
        cmocka_unit_test(get_element_by_index),
        cmocka_unit_test(find_element_by_comparison),
    };

    /* call group_setup and teardown at the very beginning and end */
    /* return cmocka_run_group_tests(tests_historique, group_setup, group_teardown);*/
    return cmocka_run_group_tests(tests_clist_generic, NULL, NULL);
}