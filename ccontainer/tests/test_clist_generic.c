/* import for CMocka */
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include "cmocka.h"

#include "ccontainer/clist_generic.c"
#include "ccontainer/value.h"

/* MSVC void **state argument or warning c4113 */

/* declare a set of static variables for testing purpose */
#include "ccontainer/tests/value_str_for_test.h"

/* helper to fill a list with the value string is value_str_for_test.h */
static clist_gen_t* make_clist_str();

/** follow \ref equalizer_value_t pointer to function signature */
static bool string_equalizer(const ccontainer_value_t *value1, const ccontainer_value_t *value2)
{
    /*if( memcmp( curr_node->value.data, buffer, buffer_len) == 0 ) {*/
    if (memcmp( value1->data, value2->data, value2->len ) == 0)
        return true;
    return false;
}

// not possible from public API, only heap
static void initialization_on_stack()
{
    // allocation on stack, call only constructor
    clist_gen_t clist;

    clist_gen_init( &clist);
    assert_int_equal( 0, clist_gen_size(&clist));
    assert_null( clist.first_node);

    // deallocation on stack, call with value_t destructor (here default)
    clist_gen_delete( &clist, ccontainer_delete_value);
}

static void initialization_on_heap()
{
    // allocation on heap, allocate + constructor
    clist_gen_t *clist = clist_gen_new();
    assert_non_null( clist );

    clist_gen_init( clist );
    assert_int_equal( 0, clist_gen_size(clist) );
    assert_null ( clist->first_node );

    // call destructor clist_delete & deallocate
    clist_gen_free( clist, ccontainer_delete_value );
    clist = NULL;
}

static void add_one_str_as_value()
{
    ccontainer_err_t err_code;
    ccontainer_value_t tmp_value_in;

    clist_gen_t *clist = clist_gen_new();
    clist_gen_init( clist );

    // make a temporary copy
    tmp_value_in = ccontainer_make_value(value_str1.data, value_str1.len, &err_code);
    assert_int_equal( CCONTAINER_OK, err_code);
    // to be transfer / moved into the list
    err_code = clist_gen_push_front( clist, &tmp_value_in );
    assert_int_equal( CCONTAINER_OK, err_code);

    assert_non_null(clist->first_node);
    assert_null(clist->first_node->prev_node);
    assert_null(clist->first_node->next_node);
    assert_int_equal(1, clist_gen_size(clist));

    // to test clear() 

    clist_gen_free( clist, ccontainer_delete_value);
}

static void add_two_str_as_value()
{
    ccontainer_err_t err_code;
    ccontainer_value_t tmp_value_in;

    clist_gen_t* clist = clist_gen_new();
    clist_gen_init( clist );

    tmp_value_in = ccontainer_make_value(value_str1.data, value_str1.len, &err_code);
    err_code = clist_gen_push_front( clist, &tmp_value_in ); // "first" , 5
    assert_int_equal( CCONTAINER_OK, err_code);
    tmp_value_in = ccontainer_make_value(value_str2.data, value_str2.len, &err_code);
    err_code = clist_gen_push_front( clist, &tmp_value_in ); // "second", 6
    assert_int_equal( CCONTAINER_OK, err_code);

    assert_non_null(clist->first_node);
    assert_null(clist->first_node->prev_node);
    assert_non_null(clist->first_node->next_node);
    assert_int_equal(2, clist_gen_size(clist));

    /* clean deletion on stack */
    clist_gen_free( clist, ccontainer_delete_value);
}

static void add_last_two_str_as_value()
{
    ccontainer_err_t err_code;
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
    assert_int_equal(2, clist_gen_size(clist));
    // destructor + deallocation on heap
    clist_gen_free( clist, ccontainer_delete_value );
    clist = NULL;
}

static void pop_front()
{
    ccontainer_err_t err_code;
    ccontainer_value_t value_out;

    clist_gen_t *clist = clist_gen_new();
    clist_node_t *node;
    clist_gen_init( clist );

    node =  clist_gen_get_first_node( clist );
    assert_null(node);
    clist_gen_free( clist, ccontainer_delete_value );
    clist = NULL;

    clist = make_clist_str( clist );
    assert_int_equal( 6, clist_gen_size(clist));

    node =  clist_gen_get_first_node( clist );
    assert_non_null(node);

    node = clist_gen_get_next_node( node );
    assert_non_null(node);

    value_out = clist_gen_pop_front(clist, &err_code);
    assert_int_equal( CCONTAINER_OK, err_code);
    assert_int_equal( 5, value_out.len);
    assert_memory_equal("first", value_out.data, value_out.len);
    ccontainer_delete_value( &value_out );

    value_out = clist_gen_pop_front(clist, &err_code);
    assert_int_equal( CCONTAINER_OK, err_code);
    assert_int_equal( 6, value_out.len);
    assert_memory_equal("second", value_out.data, value_out.len);
    ccontainer_delete_value( &value_out );

    assert_int_equal(4, clist_gen_size(clist));

    clist_gen_free( clist, ccontainer_delete_value );
    clist = NULL;
}

static void copy_constructor()
{
    assert_int_equal(2, 1+1);

    ccontainer_err_t err_code;
    ccontainer_value_t tmp_value_in;
    clist_gen_t *clist = clist_gen_new();
    clist_gen_init( clist );

    tmp_value_in = ccontainer_make_value(value_str1.data, value_str1.len, &err_code);
    err_code = clist_gen_push_back( clist, &tmp_value_in );
    assert_int_equal( CCONTAINER_OK, err_code);
    tmp_value_in = ccontainer_make_value(value_str2.data, value_str2.len, &err_code);
    assert_int_equal( CCONTAINER_OK, err_code);
    err_code = clist_gen_push_back( clist, &tmp_value_in );

    clist_gen_t *copy;
    copy = clist_gen_copy( clist, ccontainer_copy_value, &err_code );
    assert_int_equal( CCONTAINER_OK, err_code);

    clist_gen_free( clist, ccontainer_delete_value );
    clist = NULL;

    clist_gen_free( copy, ccontainer_delete_value );
    clist = NULL;
}

static void find_element()
{
    ccontainer_value_t* pvalue_match;
    ccontainer_err_t err_code;

    clist_gen_t* clist = make_clist_str();
    assert_int_equal( 6, clist_gen_size(clist));

    // one of more generic possible, fill a ccontainer_value_t for comparison
    // a function could be nice also
    //pvalue_out = clist_gen_find( &clist, "sec", 3, &err_code );
    ccontainer_value_t value_search = {.data="sec", .len=3};
    pvalue_match = clist_gen_find( clist, string_equalizer, &value_search, &err_code);

    assert_int_equal( CCONTAINER_OK, err_code );
    assert_memory_equal("second", pvalue_match->data, pvalue_match->len);
    assert_int_equal( 6, pvalue_match->len);
    pvalue_match = NULL;
    
    clist_gen_free( clist, ccontainer_delete_value );
}

static void find_all_elements()
{
    ccontainer_err_t err_code;

    clist_gen_t* clist = make_clist_str();
    assert_int_equal( 6, clist_gen_size(clist));
    clist_gen_t* clist_out;

    // test no match, clist_out not allocated/initialized
    ccontainer_value_t value_search_nomatch = {.data="unknow", .len=6};
    clist_out = clist_gen_find_all( clist, string_equalizer, &value_search_nomatch, 
                                    ccontainer_copy_value, &err_code);
    assert_null(clist_out);
    assert_int_equal( CCONTAINER_NOT_FOUND, err_code );

    ccontainer_value_t value_search = {.data="sec", .len=3};
    clist_out = clist_gen_find_all( clist, string_equalizer, &value_search, 
                                    ccontainer_copy_value, &err_code);
    assert_int_equal( CCONTAINER_OK, err_code );
    assert_int_equal( 2, clist_gen_size(clist_out));
    
    clist_gen_free( clist_out, ccontainer_delete_value );
    clist_gen_free( clist, ccontainer_delete_value );
}

static void find_and_remove_node()
{
    clist_node_t* pnode_match;
    //ccontainer_value_t* pvalue_match;
    ccontainer_err_t err_code;

    clist_gen_t* clist = make_clist_str();
    assert_int_equal( 6, clist_gen_size(clist));

    // one of more generic possible, fill a ccontainer_value_t for comparison
    ccontainer_value_t value_search = {.data="sec", .len=3};
    pnode_match = clist_gen_find_node( clist, NULL, string_equalizer, &value_search, &err_code);

    assert_int_equal( CCONTAINER_OK, err_code );
    assert_memory_equal("second",pnode_match->value.data, pnode_match->value.len);
    //assert_int_equal( 6, pnode_match->value.len);
    //pvalue_match = NULL;
    
    clist_gen_remove_node(clist, pnode_match, ccontainer_delete_value);
    assert_int_equal( 5, clist_gen_size(clist));

    clist_gen_remove_node(clist, clist->first_node, ccontainer_delete_value);
    assert_int_equal( 4, clist_gen_size(clist));

    clist_gen_free( clist, ccontainer_delete_value );
}

/* *** Helper functions *** */
clist_gen_t* make_clist_str()
{
    ccontainer_err_t err_code;
    ccontainer_value_t tmp_value_in;

    clist_gen_t *clist;
    clist = clist_gen_new();
    clist_gen_init( clist );
    
    // push_back => same order
    tmp_value_in = ccontainer_make_value(value_str1.data, value_str1.len, &err_code);
    assert_int_equal( CCONTAINER_OK, err_code );
    clist_gen_push_back( clist, &tmp_value_in );

    tmp_value_in = ccontainer_make_value(value_str2.data, value_str2.len, &err_code);
    assert_int_equal( CCONTAINER_OK, err_code );
    clist_gen_push_back( clist, &tmp_value_in );

    tmp_value_in = ccontainer_make_value(value_str3.data, value_str3.len, &err_code);
    err_code = clist_gen_push_back( clist, &tmp_value_in );
    assert_int_equal( CCONTAINER_OK, err_code );

    tmp_value_in = ccontainer_make_value(value_str4.data, value_str4.len, &err_code);
    err_code = clist_gen_push_back( clist, &tmp_value_in );
    assert_int_equal( CCONTAINER_OK, err_code );

    // add doublon : 7 (tree) and 8 (second)
    tmp_value_in = ccontainer_make_value(value_str7.data, value_str7.len, &err_code);
    err_code = clist_gen_push_back( clist, &tmp_value_in );
    tmp_value_in = ccontainer_make_value(value_str8.data, value_str8.len, &err_code);
    err_code = clist_gen_push_back( clist, &tmp_value_in );

    return clist;
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
        cmocka_unit_test(pop_front),
        cmocka_unit_test(copy_constructor),
        cmocka_unit_test(find_element),
        cmocka_unit_test(find_all_elements),
        cmocka_unit_test(find_and_remove_node),
    };

    /* call group_setup and teardown at the very beginning and end */
    /* return cmocka_run_group_tests(tests_historique, group_setup, group_teardown);*/
    return cmocka_run_group_tests(tests_clist_generic, NULL, NULL);
}