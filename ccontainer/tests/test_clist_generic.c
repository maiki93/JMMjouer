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
void deleter_value( value_t *value)
{
    if( value ) {
        free(value->data);
        value->data = NULL;
    }
    value->len = 0;
}

/* const data used by test, use bytes no specifically string */
value_t value_str1 = {"first", 5}; // 5: do not include '\0'
value_t value_str2 = {"second", 6};

/* TODO use other type to include int representation, float 
    ... hex value, octal value */

static void initialization_on_stack()
{
    // allocation on stack, call only constructor
    clist_gen_t clist;
    clist_gen_init( &clist);

    assert_int_equal( 0, clist.len);
    assert_null( clist.first_node);
    // deallocation on stack, call destructor
    // nothing to delete here
    clist_gen_clear( &clist, deleter_value);
}

static void initialization_on_heap()
{
    // allocation on heap, allocate + constructor
    clist_gen_t *clist = clist_gen_new();
    clist_gen_init( clist );

    assert_int_equal( 0, clist->len );
    assert_null ( clist->first_node );
    /* call destructor clist_clear & deallocate */
    clist_gen_del( clist, deleter_value );
    clist = NULL; /* to protect later use */
    assert_null( clist );
}

static void add_one_str_as_value()
{
    clist_gen_t clist;
    clist_gen_init( &clist );
    clist_gen_push_front( &clist, value_str1 );

    assert_non_null(clist.first_node);
    assert_null(clist.first_node->next_node);
    assert_int_equal(1, clist.len);

    /* clean deletion on stack */
    clist_gen_clear( &clist, deleter_value);
    assert_int_equal(0, clist.len);
    assert_null(clist.first_node);
}

static void add_two_str_as_value()
{
    clist_gen_t clist;
    clist_gen_init( &clist );

    clist_gen_push_front( &clist, value_str1 ); // "first" , 5
    clist_gen_push_front( &clist, value_str2 ); // "second", 6

    assert_non_null(clist.first_node);
    assert_non_null(clist.first_node->next_node);
    assert_int_equal(2, clist.len);

    /* clean deletion on stack */
    clist_gen_clear( &clist, deleter_value);
    assert_null(clist.first_node);
    assert_int_equal(0, clist.len);
}

static void add_last_two_str_as_value()
{
    clist_gen_t *clist = clist_gen_new();
    clist_gen_init( clist );

    clist_gen_push_back( clist, &value_str1 );
    clist_gen_push_back( clist, &value_str2 );

    assert_non_null(clist->first_node);
    assert_non_null(clist->first_node->next_node);
    assert_int_equal(2, clist->len);
    // destructor + deallocation on heap
    clist_gen_del( clist, deleter_value );
    clist = NULL;
}

static void get_element_by_nb_reference() 
{
    clist_gen_t clist;
    value_t* value_out; /* value by ref */
    clist_gen_init( &clist );

    // push_front => invert order : "second" then "first"
    clist_gen_push_front( &clist, value_str1 );
    clist_gen_push_front( &clist, value_str2 );

    clist_gen_get_value_ref( &clist, 0, &value_out);
    /* by reference, no terminaison character so compare the first bytes */
    assert_memory_equal( "second", value_out->data, value_out->len );
    assert_int_equal( 6, value_out->len);
    /* not wroking, certainly because value_str2 contains de '\0' character 
        TODO to check if hypothesis correct */
    /*assert_memory_equal( &value_str2, value_out, sizeof(value_t));*/

    clist_gen_get_value_ref( &clist, 1, &value_out);
    assert_memory_equal( "first", value_out->data, value_out->len );
    assert_int_equal( 5, value_out->len);

    clist_gen_clear( &clist, deleter_value );
    assert_null(clist.first_node);
    assert_int_equal(0, clist.len);
}

static void get_element_by_nb_copy() 
{
    clist_gen_t clist;
    value_t value_str_inl, value_str_inl2; 
    int retour;
    value_t value_out; /* value by copy, must be deleted */
    value_str_inl.data="first";
    value_str_inl.len = 5; /* strlen */
    clist_gen_init( &clist );
    clist_gen_push_front( &clist, value_str_inl );
    value_str_inl2.data="second";
    value_str_inl2.len = 6; /* strlen  */
    clist_gen_push_front( &clist, value_str_inl2 );

    retour = clist_gen_get_value_copy( &clist, 0, &value_out);
    assert_int_equal(0, retour);
    /* by copy cannot compare string, '\0' is not present */
    /* assert_string_equal("second", value_out.data); */ /* may pass by chance !!! */
    assert_memory_equal("second", value_out.data, 6);
    assert_int_equal( 6, value_out.len);
    /* dr memeory leak, forgot deletion of copies */
    deleter_value( &value_out );

    retour = clist_gen_get_value_copy( &clist, 1, &value_out);
    assert_int_equal(0, retour);
    assert_int_equal( 5, value_out.len);
    assert_memory_equal("first", value_out.data, 5);
    deleter_value( &value_out );

    // => out of range : 1
    retour = clist_gen_get_value_copy( &clist, 2, &value_out);
    assert_int_equal(CLIST_OUTOFRANGE, retour); 

    clist_gen_clear( &clist, deleter_value );
}

static void find_element_by_comparison_copy() 
{
    clist_gen_t clist;
    value_t value_str_inl, value_str_inl2; 
    int retour_nb;

    value_t value_out; /*  value by copy */
    char buffer_search[4] = "sec";
    
    value_str_inl.data="first";
    value_str_inl.len = 6; /* strlen + 1 */
    clist_gen_init( &clist );
    clist_gen_push_front( &clist, value_str_inl );
    value_str_inl2.data="second";
    value_str_inl2.len = 7; /* strlen + 1 */
    clist_gen_push_front( &clist, value_str_inl2 );

    // one of more generic possible, fill a value_t for comparison
    // a function could be nice also
    retour_nb = clist_gen_find( &clist, &value_out, buffer_search, 3 );

    assert_int_equal( CLIST_OK, retour_nb );
    assert_string_equal("second", value_out.data); /* here working ? why ? */
    assert_int_equal( 7, value_out.len);
    deleter_value( &value_out );

    retour_nb = clist_gen_find( &clist, &value_out, "first", 5 );

    assert_int_equal( CLIST_OK, retour_nb );
    assert_string_equal("first", value_out.data);
    assert_int_equal( 6, value_out.len);
    deleter_value( &value_out );

    retour_nb = clist_gen_find( &clist, &value_out, "toto", 3 );
    assert_int_equal(CLIST_NOTFOUND, retour_nb);

    clist_gen_clear( &clist, deleter_value );
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
        cmocka_unit_test(get_element_by_nb_reference), /* one drmemory error but not from copy_value */
        cmocka_unit_test(get_element_by_nb_copy),
        cmocka_unit_test(find_element_by_comparison_copy),
    };

    /* call group_setup and teardown at the very beginning and end */
    /* return cmocka_run_group_tests(tests_historique, group_setup, group_teardown);*/
    return cmocka_run_group_tests(tests_clist_generic, NULL, NULL);
}