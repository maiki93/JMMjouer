/* import for CMocka */
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include "cmocka.h"

#include "ccontainer/cvector_generic.h"

/* test values with simple strings, do not include final \0 */
static ccontainer_value_t value_str1 = {"first", 5};
static ccontainer_value_t value_str2 = {"second", 6};
static ccontainer_value_t value_str3 = {.data="tree",.len=4}; /* available C90 */

static void initialization_on_stack()
{
    ccontainer_err_t err_code;
    cvector_gen_t cvect;
    // no memory allocation, no error_code, default destructor and copy
    cvector_gen_init( &cvect );

    assert_int_equal( 0, cvector_gen_size( &cvect ));
    assert_int_equal( 0, cvector_gen_capacity( &cvect ));
    // empty but need to delete anyway
    cvector_gen_delete( &cvect );
    
    cvector_gen_t cvect2;
    err_code = cvector_gen_init_with_capacity( &cvect2, 2 );
    assert_int_equal( CCONTAINER_OK, err_code);
    assert_int_equal( 0, cvector_gen_size( &cvect2 ));
    assert_int_equal( 2, cvector_gen_capacity( &cvect2 ));

    // clear do not change the capacity
    cvector_gen_clear( &cvect2 );
    assert_int_equal( 0, cvector_gen_size( &cvect2) );
    assert_int_equal( 2, cvector_gen_capacity( &cvect2) );
    // dealloate internal array properly
    cvector_gen_delete( &cvect2 );
}

static void initialization_on_heap()
{
    ccontainer_err_t err_code;
    cvector_gen_t *cvect = cvector_gen_new();
    cvector_gen_init( cvect );

    assert_int_equal( 0, cvector_gen_size( cvect ));
    assert_int_equal( 0, cvector_gen_capacity( cvect ));

    cvector_gen_free( cvect );
    cvect = NULL;
    ///////////////////
    cvector_gen_t *cvect2 = cvector_gen_new();
    err_code = cvector_gen_init_with_capacity( cvect2, 2 );
    assert_int_equal( CCONTAINER_OK, err_code);
    assert_int_equal( 0, cvector_gen_size( cvect2 ));
    assert_int_equal( 2, cvector_gen_capacity( cvect2 ));
    
    cvector_gen_free( cvect2 );
    cvect2 = NULL;
}

static void push_two_str_in_capacity_five()
{
    ccontainer_err_t err_code;
    cvector_gen_t *cvect;

    ccontainer_value_t tmp_value_in;

    cvect = cvector_gen_new();
    err_code = cvector_gen_init_with_capacity( cvect, 5 );
    
    // create a copy of the input in a ccontainer_value_t format
    tmp_value_in = ccontainer_make_value(value_str1.data, value_str1.len, &err_code);
    assert_int_equal( CCONTAINER_OK, err_code);
    // transfer value_t into the cvector_gen_t
    err_code = cvector_gen_push_back( cvect, &tmp_value_in );
    assert_int_equal( CCONTAINER_OK, err_code);
    // content of tmp_value has been moved
    assert_null( tmp_value_in.data );
    assert_int_equal(0, tmp_value_in.len);
    
    tmp_value_in = ccontainer_make_value(value_str2.data, value_str2.len, &err_code);
    err_code = cvector_gen_push_back( cvect, &tmp_value_in );
    assert_int_equal( CCONTAINER_OK, err_code);

    assert_int_equal( 2, cvector_gen_size( cvect ));
    assert_int_equal( 5, cvector_gen_capacity( cvect ));

    cvector_gen_free( cvect );
    cvect = NULL;
}

static void push_two_str_in_empty_cvector()
{
    ccontainer_err_t err_code;
    ccontainer_value_t tmp_value_in;
    cvector_gen_t cvect;

    cvector_gen_init( &cvect );

    tmp_value_in = ccontainer_make_value( value_str1.data, value_str1.len, &err_code);
    err_code = cvector_gen_push_back( &cvect, &tmp_value_in );
    assert_int_equal( CCONTAINER_OK, err_code);
    assert_int_equal( 1, cvector_gen_size( &cvect ));
    assert_int_equal( 1, cvector_gen_capacity( &cvect ));

    tmp_value_in = ccontainer_make_value( value_str2.data, value_str2.len, &err_code);
    err_code = cvector_gen_push_back( &cvect, &tmp_value_in );
    assert_int_equal( CCONTAINER_OK, err_code);
    assert_int_equal( 2, cvector_gen_size( &cvect ));
    assert_int_equal( 2, cvector_gen_capacity( &cvect ));

    cvector_gen_delete( &cvect );
}

static void get_reference()
{
    ccontainer_err_t err_code;
    cvector_gen_t cvect;
    ccontainer_value_t tmp_value_in;
    ccontainer_value_t* pvalue_out;
    
    cvector_gen_init( &cvect );

    tmp_value_in = ccontainer_make_value( value_str1.data, value_str1.len, &err_code);
    cvector_gen_push_back( &cvect, &tmp_value_in );
    tmp_value_in = ccontainer_make_value( value_str2.data, value_str2.len, &err_code);
    cvector_gen_push_back( &cvect, &tmp_value_in );

    // out of range
    pvalue_out = cvector_gen_get_at( &cvect, 5, &err_code);
    assert_null( pvalue_out);
    assert_int_equal( CCONTAINER_OUTOFRANGE, err_code);
    // access second elememt
    pvalue_out = cvector_gen_get_at( &cvect, 1, &err_code);
    assert_non_null( pvalue_out);
    assert_int_equal( CCONTAINER_OK, err_code);
    // assert the content is equal
    assert_int_equal( 6, pvalue_out->len );
    assert_memory_equal( "second", pvalue_out->data, 6 );
    // assert it points to the same memory
    assert_ptr_equal( &(cvect.array[1]), pvalue_out);

    // cannot do this, must use memory in ccontainer_value_t to modify it
    //pvalue_out->data = "THIRD";
    //pvalue_out->len = 5;
    // delete pvalue_out
    // cvector_gen_replace( ) / cvector_gen_insert( index, ccontainer_value_t*)

    //pvalue_out2 = cvector_gen_get_const_ref( &cvect, 1, &err_code);
    // assert the content has been modified
    //assert_int_equal( 5, pvalue_out2->len );
    //assert_memory_equal( "THIRD", pvalue_out2->data, 5 );
    // error compilation, because of const pvalue_out mainly , 
    // not sure function_const_ref very effective in C
    //pvalue_out2->data = "THIRD";
    //pvalue_out2->len = 5;
    //pvalue_out2 = cvector_gen_get_const_ref( &cvect, 0, &err_code);

    // a copy can be done with duplicater_
    //value_out3 = cvect.ptrf_duplicater( cvector_gen_get_ref( &cvect, 1, &err_code) );

    cvector_gen_delete( &cvect );
}

static void copy_constructor()
{
    ccontainer_err_t err_code;
    cvector_gen_t cvect, cvect_copy;
    ccontainer_value_t tmp_value_in;
    
    cvector_gen_init( &cvect );

    tmp_value_in = ccontainer_make_value( value_str1.data, value_str1.len, &err_code);
    cvector_gen_push_back( &cvect, &tmp_value_in );
    tmp_value_in = ccontainer_make_value( value_str2.data, value_str2.len, &err_code);
    cvector_gen_push_back( &cvect, &tmp_value_in );

    cvect_copy = cvector_gen_copy( &cvect, &err_code);
    assert_int_equal(CCONTAINER_OK, err_code);

    cvector_gen_delete( &cvect );

    assert_int_equal( 2, cvect_copy.len );
    assert_int_equal( 2, cvect_copy.capacity );

    cvector_gen_delete( &cvect_copy );
}

static void swap_2index()
{
    ccontainer_err_t err_code;
    cvector_gen_t cvect;
    ccontainer_value_t tmp_value_in;
    ccontainer_value_t* pvalue_out;
    
    // no memory allocation, no error_code
    cvector_gen_init( &cvect );

    tmp_value_in = ccontainer_make_value(value_str1.data,value_str1.len,&err_code);
    cvector_gen_push_back( &cvect, &tmp_value_in );
    tmp_value_in = ccontainer_make_value(value_str2.data,value_str2.len,&err_code);
    cvector_gen_push_back( &cvect, &tmp_value_in );
    tmp_value_in = ccontainer_make_value(value_str3.data,value_str3.len,&err_code);
    cvector_gen_push_back( &cvect, &tmp_value_in );

    cvector_gen_swap( &cvect, 0, 2);

    pvalue_out = cvector_gen_get_at( &cvect, 0, &err_code);
    assert_int_equal( 4, pvalue_out->len );
    assert_memory_equal( "tree", pvalue_out->data, pvalue_out->len );

    pvalue_out = cvector_gen_get_at( &cvect, 1, &err_code);
    assert_int_equal( 6, pvalue_out->len );
    assert_memory_equal( "second", pvalue_out->data, pvalue_out->len );

    pvalue_out = cvector_gen_get_at( &cvect, 2, &err_code);
    assert_int_equal( 5, pvalue_out->len );
    assert_memory_equal( "first", pvalue_out->data, pvalue_out->len );

    cvector_gen_delete( &cvect);
}

int main()
{
    /* can be inside main, or as global. here no problem with identical name */
    const struct CMUnitTest tests_cvector_generic[] = {
        cmocka_unit_test(initialization_on_stack),
        cmocka_unit_test(initialization_on_heap),
        cmocka_unit_test(push_two_str_in_capacity_five),
        cmocka_unit_test(push_two_str_in_empty_cvector),
        cmocka_unit_test(get_reference),
        cmocka_unit_test(copy_constructor),
        cmocka_unit_test(swap_2index),
    };

    /* call group_setup and teardown at the very beginning and end */
    /* return cmocka_run_group_tests(tests_historique, group_setup, group_teardown);*/
    return cmocka_run_group_tests(tests_cvector_generic, NULL, NULL);
}