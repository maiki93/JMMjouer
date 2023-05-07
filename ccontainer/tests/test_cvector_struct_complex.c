/* import for CMocka */
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include "cmocka.h"

#include "ccontainer/cvector_generic.h"
/* include directly the implementation, it is for tests */
#include "ccontainer/tests/struct_complex.c"

/** \name adapter functions to cvector_generic internal type */
/** \{ */
/** Return a value_t containing data from struct_complex_t.
 Designer of complex_struct must decide of this implemnentation :
  - Serialize the data (like for the float and bool)
  - Keep a pointer indirection to a dynamic allocated memory (here done for pname) */
value_t make_value_struct_complex(const struct_complex_t * complex_struct)
{
    value_t value_out;
    char *tmp_string;
    // keep indirection for pname( size of a pointer ) + sizeof(float) + sizeof(bool) => sizeof(struct_complex_t)
    value_out.len = sizeof(struct_complex_t);
    value_out.data = (char*) malloc( value_out.len );
    // copy name in temporary vector
    tmp_string = (char *)malloc( strlen(complex_struct->pname) + 1 );
    // misss check of malloc, passing by pointer or error_code
    strcpy(tmp_string, complex_struct->pname);
    // serialize now the content into value_out.data
    //memcpy( (char**) value_out.data, &tmp_string, sizeof(char *));
    memcpy(value_out.data, &tmp_string, sizeof(char *));
    memcpy(value_out.data + sizeof(char *), &(complex_struct->one_float), sizeof(float));
    memcpy(value_out.data + sizeof(char *) + sizeof(float), &(complex_struct->is_valid), sizeof(bool));

    // other possibility recopy all the structur content (copy) and overwrite first field like before
    // memcpy(value_out.data, complex_struct, value_out.len);
    // memcpy( (char**) value_out.data, &tmp_string, sizeof(char *));
    return value_out;
}

/** Return a struct_complex_t from the value_t in source.
 * Steal the allocated memory for pname of value_in !
 * Possible also to return struct_complex_t by copy if size is know to be small
 * Deserialze the data
 * \param[in] value_in source, not functional after call !
 * \param[out] struct_out struct_complex_t previously allocated by caller (allow to pass variable allocated on stack)
*/
int extract_value_struct_complex(const value_t* value_in, struct_complex_t *struct_out)
{
    // here also, cast to char** very important
    char **p_str = (char **)value_in->data;
    struct_out->pname = *p_str;
    // NOT equivalent
    // cast to float * is very important here !!
    struct_out->one_float = *((float *) (value_in->data + sizeof(char*)));
    // struct_out->one_float = (float ) value_in->data[sizeof(char*)]; // => -61 ?

    // slightly different in writing
    struct_out->is_valid = (bool) value_in->data[ sizeof(char*) + sizeof(float)];
    return 0;
}

/** Adapter to delete struct_complex when serialized into a value_t.
 * It is called by cvector_delete / free
 */
void struct_complex_deleter(value_t* value_in)
{
    //if( !value_in || !value_in->data)
    // return
    char **p_str = (char **)value_in->data;
    if(*p_str)
        free(*p_str);
    p_str = NULL;
    free(value_in->data);
    value_in->data = NULL;
    value_in->len = 0;
}

/* Default duplicater is correct, copy byte-to-byte pname allocated on heap does not change.
 This implementation certainly incorrect */
value_t struct_complex_duplicater(const value_t* value_in)
{
    value_t value_out;
    size_t len_str;
    char *tmp_str;
    char **p_str = (char **)value_in->data;
    len_str = strlen(*p_str);
    tmp_str = (char *)malloc( len_str );
    strcpy(tmp_str,*p_str);

    value_out.data = (char*) malloc(value_in->len);

    // field string pname
    memcpy(value_out.data, &tmp_str, sizeof(char *));
    *p_str = value_out.data; /* for debug */
    // next field copy byte to byte
    memcpy(value_out.data + sizeof(char *), value_in + sizeof(char *), sizeof(float));
    memcpy(value_out.data + sizeof(char *), value_in + sizeof(char *) + sizeof(float), sizeof(bool));

    return value_out;
}
/** \} */

static void init_complex_structure_on_heap()
{
    struct_complex_t *p_struct;
    int err_code;
    // construction
    p_struct = struct_complex_new();
    err_code = struct_complex_init(p_struct, "maiki", 3.14f, true);
    assert_int_equal(0, err_code);
    assert_string_equal("maiki", p_struct->pname);
    assert_float_equal(3.14f, p_struct->one_float, 0.01);
    assert_true(p_struct->is_valid);

    // destruction
    struct_complex_free(p_struct);
    p_struct = NULL;
}

static void init_complex_structure_on_stack()
{
    struct_complex_t comp_struct;
    int err_code;
    // construction
    err_code = struct_complex_init( &comp_struct, "maiki", 3.14f, true);
    assert_int_equal(0, err_code);
    assert_string_equal("maiki", comp_struct.pname);
    assert_float_equal(3.14f, comp_struct.one_float, 0.01);
    assert_true(comp_struct.is_valid);

    // destruction on stack
    struct_complex_delete(&comp_struct);
}

static void serialization_struct()
{
    struct_complex_t comp_struct_in, comp_struct_in2;
    struct_complex_t comp_struct_out = {.pname=NULL,.one_float=0.f,.is_valid=false};
    value_t value;
    struct_complex_init( &comp_struct_in, "maiki", 3.14f, true);

    value = make_value_struct_complex( &comp_struct_in );
    /* keep for example testing, debug */
    /*
    char **p_str = (char **)value.data; // &(data[0]) == ** char, very important
    float *p_float = &(value.data[sizeof(char *)]);
    bool *p_bool = &(value.data[sizeof(char *) + sizeof(float)]);
    // equivalent
    float v_float = (float) *(value.data + sizeof(char *));
    float b_float = (bool) *(value.data + sizeof(char *) + sizeof(float));
    */

    extract_value_struct_complex( &value, &comp_struct_out );
    assert_string_equal("maiki",comp_struct_out.pname);
    assert_float_equal(3.14f,comp_struct_out.one_float,0.001);
    assert_true(comp_struct_out.is_valid);

    // memory allocated in comp_struct
    struct_complex_delete( &comp_struct_out);
    // value.data has been stolen 
    default_deleter_value( &value);

    struct_complex_init( &comp_struct_in2, "toto", 2.11f, false);
    value = make_value_struct_complex( &comp_struct_in2 );
    extract_value_struct_complex( &value, &comp_struct_out );
    assert_string_equal("toto",comp_struct_out.pname);
    assert_float_equal(2.11f,comp_struct_out.one_float,0.001);
    assert_false(comp_struct_out.is_valid);

    // destructor must be called with objects on stack
    struct_complex_delete( &comp_struct_in);
    struct_complex_delete( &comp_struct_in2);
    // memory allocated in value
    struct_complex_delete( &comp_struct_out);
    default_deleter_value( &value);
}

static void push_back_one_struct()
{
    ccontainer_err err_code;
    struct_complex_t struct_out = {.pname = NULL,.one_float=0.,.is_valid=false};

    struct_complex_t comp_struct;
    struct_complex_init( &comp_struct, "maiki", 3.14f, true);

    cvector_gen_t cvector;
    cvector_gen_init(&cvector);
    // struct_complex with dynamic allocation, value_t need specific deleter
    cvector_gen_set_deleter(&cvector, struct_complex_deleter);
    // default is fine here, duplicater cause errors
    // cvector_gen_set_duplicater(&cvector, struct_complex_duplicater);

    value_t value_struct;
    value_t *pvalue_ref;
    // push back must make a deep copy of the complexe_struct
    // for value, one copy is enought
    value_struct = make_value_struct_complex(&comp_struct);
    cvector_gen_push_back(&cvector, &value_struct);
    // only temporary, heap pname has been transfered ? so no memory leak with default deleter ?
    default_deleter_value(&value_struct);

    // retrieve reference
    pvalue_ref = cvector_gen_get_ref(&cvector, 0, &err_code);
    // here steal the heap allocated pname !!
    extract_value_struct_complex(pvalue_ref, &struct_out);
    // only a temporary, owns nothing
    pvalue_ref = NULL;
    assert_int_equal(0, err_code);
    assert_string_equal("maiki", struct_out.pname);
    assert_float_equal(3.14f, struct_out.one_float, 0.01);
    assert_true(struct_out.is_valid);

    struct_complex_delete(&comp_struct);
    default_deleter_value(&value_struct);
    
    cvector_gen_delete(&cvector);
}

static void push_back_two_struct()
{
    value_t *pvalue_out_ref;
    struct_complex_t struct_out = {.pname = NULL,.one_float=0.,.is_valid=false};

    ccontainer_err err_code;
    struct_complex_t comp_struct, comp_struct2;
    struct_complex_init( &comp_struct, "maiki", 3.14f, true);
    struct_complex_init( &comp_struct2, "toto", 2.31f, false);

    cvector_gen_t cvector;
    cvector_gen_init(&cvector);
    // struct_complex with dynamic allocation
    cvector_gen_set_deleter(&cvector, struct_complex_deleter);
    // default is fine here, error in duplicator
    // cvector_gen_set_duplicater(&cvector, struct_complex_duplicater);

    value_t value_struct,value_struct2;
    value_struct = make_value_struct_complex(&comp_struct);
    // value_struct2 = make_value_struct_complex(&comp_struct2);
    // push back makes a deep copy
    cvector_gen_push_back(&cvector, &value_struct);
    // cvector_gen_push_back(&cvector, &value_struct2); // pass test but memory leak

    // retrieve reference
    pvalue_out_ref = cvector_gen_get_ref(&cvector, 0, &err_code);
    extract_value_struct_complex(pvalue_out_ref, &struct_out);
    assert_int_equal(0, err_code);
    assert_string_equal("maiki", struct_out.pname);
    assert_float_equal(3.14f, struct_out.one_float, 0.01);
    assert_true(struct_out.is_valid);

    struct_complex_delete(&comp_struct);
    struct_complex_delete(&comp_struct2);

    default_deleter_value(&value_struct);
    default_deleter_value(&value_struct2);

    cvector_gen_delete(&cvector);
}

int main()
{
    /* can be inside main, or as global. here no problem with identical name */
    const struct CMUnitTest tests_cvector_struct_complex[] = {
        cmocka_unit_test(init_complex_structure_on_heap),
        cmocka_unit_test(init_complex_structure_on_stack),
        cmocka_unit_test(serialization_struct),
        cmocka_unit_test(push_back_one_struct),
        //cmocka_unit_test(push_back_two_struct),
     };

    /* call group_setup and teardown at the very beginning and end */
    /* return cmocka_run_group_tests(tests_historique, group_setup, group_teardown);*/
    return cmocka_run_group_tests(tests_cvector_struct_complex, NULL, NULL);
}   