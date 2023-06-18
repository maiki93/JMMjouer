/* import for CMocka */
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include "cmocka.h"

#include "ccontainer/cvector_generic.h"
/* include directly the implementation */
#include "ccontainer/tests/struct_complex.c"

/** @file 
 * Example test for extending a complex structure.
 * 
 * @ingroup ccontainer_adapt_own_struture
*/

/** helper function. */
static void check_copy_in_value(ccontainer_value_t *value, struct_complex_t *comp_struct );

/** @{ \ingroup ccontainer_adapt_own_struture */

/** \name adapter functions to cvector_generic with ccontainer_value_t internal type */
/** \{ */
/** Return a ccontainer_value_t containing a copy of all struct_complex_t data, a "serialized" version.
    Designer of complex_struct must decide of this implemnentation :
  - Serialize the data (like for the float and bool), could be done for the string (need size + char[size])
  - Keep a pointer to a dynamic allocated memory (here done for pname), allow faster copy and move inside cvector

  Depending on this choice, different copy / destructor pointer funtion must be set to cvector.
  \param[in] complex_struct source
  \return ccontainer_value_t containing its own copy of all data of complex_struct */
ccontainer_value_t make_value_struct_complex(const struct_complex_t * complex_struct, ccontainer_err_t *err_code)
{
    ccontainer_value_t value_out;
    char *tmp_string;
    
    // allocate memory for ccontainer_value_t, 
    // here always same size len for struct_complex_t(char* + float + bool)
    value_out.len = sizeof(struct_complex_t);
    value_out.data = (char*) malloc( sizeof(struct_complex_t) );
    if( !value_out.data ) {
        *err_code = CCONTAINER_ALLOCERR;
        // not initialized to 0 by default, care if an allocation error occurs
        ccontainer_reset_value(&value_out);
        return value_out;
    }

    // copy name in temporary vector allocated on heap
    tmp_string = (char *)malloc( strlen(complex_struct->pname) + 1 );
    if( !tmp_string ) {
        *err_code = CCONTAINER_ALLOCERR;
        // not initialized to 0 by default, care if an allocation error occurs
        ccontainer_reset_value(&value_out);
        return value_out;
    }
    strcpy(tmp_string, complex_struct->pname);

    // serialize now the content into value_out.data
    memcpy(value_out.data, &tmp_string, sizeof(char *));
    memcpy(value_out.data + sizeof(char *), &(complex_struct->one_float), sizeof(float));
    memcpy(value_out.data + sizeof(char *) + sizeof(float), &(complex_struct->is_valid), sizeof(bool));

    // other possibility recopy all the structur content (copy) and overwrite first field like before
    // memcpy(value_out.data, complex_struct, value_out.len);
    // memcpy( (char**) value_out.data, &tmp_string, sizeof(char *));
    *err_code = CCONTAINER_OK;
    return value_out;
}

/** Return a copy of the ccontainer_value_t, a "deserialized" version.
  In this implementation it is a full/deep copy of the data, with dynamic allocation for the string.
  struct_out is reinitialized (pointer to memory deallocated) if not empty
  \param[in] value_in : pointer to the source ccontainer_value_t
  \param[out] struct_out : pointer to the destination struct_complex_t 
  \return ccontainer error_code */
ccontainer_err_t extract_value_struct_complex(const ccontainer_value_t* value_in, struct_complex_t *struct_out)
{
    // free memory of struct_out if already allocated
    if( struct_out && struct_out->pname ) {
        struct_complex_delete(struct_out);
    }
    
    // cast to char** very important
    char **p_str = (char **)value_in->data;
    // size_t size = strlen( *p_str);
    char *tmp_str = (char *) malloc( strlen( *p_str) + 1 );
    if( !tmp_str ) {
        return CCONTAINER_ALLOCERR;
    }
    strcpy(tmp_str,*p_str);
    // fill struct_complex content, as previous a memcpy may be possible for the two last fields
    struct_out->pname = tmp_str;
    // NOT equivalent ? cast to float * is very important here ?
    struct_out->one_float = *((float *) (value_in->data + sizeof(char*)));
    // struct_out->one_float = (float ) value_in->data[sizeof(char*)]; // => -61 ?
    // slightly different in writing, same thing
    struct_out->is_valid = (bool) value_in->data[ sizeof(char*) + sizeof(float)];
    return CCONTAINER_OK;
}

/** Adapter to delete struct_complex when serialized into a ccontainer_value_t.
 * It is called by cvector_generic for delete / free / remove ... and must be set to the cvector
 *  \param[in] value_in : pointer to a valid ccontainer_value_t element of the vector */
void struct_complex_deleter_value(ccontainer_value_t* value_in)
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

/** Deep copy implementation because contains a pointer */
ccontainer_value_t struct_complex_duplicater_value(const ccontainer_value_t* value_in, ccontainer_err_t *err_code)
{
    ccontainer_value_t value_out;
    size_t len_str;
    char *tmp_str;
    char **p_str = (char **)value_in->data;
    len_str = strlen(*p_str);
    tmp_str = (char *)malloc( len_str + 1 );
    strcpy(tmp_str,*p_str);

    value_out.len = sizeof(struct_complex_t);
    value_out.data = (char*) malloc(sizeof(struct_complex_t));
    
    // field string pname
    memcpy(value_out.data, &tmp_str, sizeof(char *));
    // next field copy byte to byte
    memcpy(value_out.data + sizeof(char *), value_in->data + sizeof(char *), sizeof(float));
    memcpy(value_out.data + sizeof(char *) + sizeof(float), value_in->data + sizeof(char *) + sizeof(float), sizeof(bool));

    *err_code = CCONTAINER_OK;
    return value_out;
}

/** Equalizer to be complete */

/** \} */

/* ********* Tests ********** */

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
    struct_complex_t comp_struct_in;
    struct_complex_t comp_struct_out = {.pname=NULL,.one_float=0.f,.is_valid=false};
    ccontainer_value_t value;
    ccontainer_err_t err_code;
    struct_complex_init( &comp_struct_in, "maiki", 3.14f, true);

    value = make_value_struct_complex( &comp_struct_in, &err_code );
    assert_int_equal(CCONTAINER_OK, err_code);
    /* keep for example testing, with debugger */
    /*
    char **p_str = (char **)value.data; // &(data[0]) == ** char, very important
    float *p_float = &(value.data[sizeof(char *)]);
    bool *p_bool = &(value.data[sizeof(char *) + sizeof(float)]);
    // equivalent
    float v_float = (float) *(value.data + sizeof(char *));
    float b_float = (bool) *(value.data + sizeof(char *) + sizeof(float));
    */
    check_copy_in_value(&value, &comp_struct_in);
    // full copy in value , can delete the first input
    struct_complex_delete( &comp_struct_in);

    // deserialization
    err_code = extract_value_struct_complex( &value, &comp_struct_out );
    assert_int_equal(CCONTAINER_OK, err_code);
    // check values
    assert_string_equal("maiki",comp_struct_out.pname);
    assert_float_equal(3.14f,comp_struct_out.one_float,0.001);
    assert_true(comp_struct_out.is_valid);
    // check they are two distinct copies
    check_copy_in_value(&value, &comp_struct_out);
    
    // call deep copy deleter, because deep copy of struct_out inside
    struct_complex_deleter_value(&value);
    // free and reset memory allocated in comp_struct when value extracted
    struct_complex_delete( &comp_struct_out);
}

static void deleter_and_duplicater_struct()
{
    ccontainer_err_t err_code;
    ccontainer_value_t value_struct, value_struct_copy;
    
    struct_complex_t comp_struct;
    struct_complex_init( &comp_struct, "maiki", 3.14f, true);
    // make a full copy
    value_struct = make_value_struct_complex(&comp_struct, &err_code);
    assert_int_equal(CCONTAINER_OK,err_code);
    // check it is a copy, delete origin just after
    check_copy_in_value(&value_struct, &comp_struct);
    struct_complex_delete(&comp_struct);

    value_struct_copy = struct_complex_duplicater_value( &value_struct, &err_code );
    
    struct_complex_deleter_value(&value_struct);
    //struct_complex_deleter_value(&value_struct_copy);

    // reproduce with pointer
    ccontainer_value_t* pvalue =  &value_struct_copy;
    ccontainer_value_t value_struct_copy2;
    value_struct_copy2 = struct_complex_duplicater_value( pvalue, &err_code );

    struct_complex_deleter_value(&value_struct_copy);
    struct_complex_deleter_value(&value_struct_copy2);

}

static void push_back_one_struct()
{
    ccontainer_err_t err_code;
    struct_complex_t struct_out = {.pname = NULL,.one_float=0.,.is_valid=false};

    struct_complex_t comp_struct;
    struct_complex_init( &comp_struct, "maiki", 3.14f, true);

    cvector_gen_t cvector;
    cvector_gen_init(&cvector);
    // struct_complex with dynamic allocation, value_t need specific deleter
    cvector_gen_set_deleter(&cvector, struct_complex_deleter_value);
    // default is fine here, not call by push_back
    //cvector_gen_set_duplicater(&cvector, struct_complex_duplicater);

    ccontainer_value_t value_struct;
    ccontainer_value_t *pvalue_ref;
    // push back must make a deep copy of the complexe_struct
    // for value, one copy is enought
    value_struct = make_value_struct_complex(&comp_struct, &err_code);
    // to check correct copy, value_struct deep copy
    struct_complex_delete(&comp_struct);

    err_code = cvector_gen_push_back(&cvector, &value_struct);
    assert_int_equal(0, err_code);
    // only temporary a temporary value, delete only value.data
    ccontainer_reset_value(&value_struct);

    // retrieve by copy (cvector_get_ref -> value_t* -> extract_struct_complex -> a deep copy)
    pvalue_ref = cvector_gen_get_at(&cvector, 0, &err_code);
    // here steal the heap allocated pname !!
    err_code = extract_value_struct_complex(pvalue_ref, &struct_out);
    // only a temporary, owns nothing
    pvalue_ref = NULL;
    assert_int_equal(0, err_code);
    assert_string_equal("maiki", struct_out.pname);
    assert_float_equal(3.14f, struct_out.one_float, 0.01);
    assert_true(struct_out.is_valid);

    // check they point to different array
    check_copy_in_value(&(cvector.array[0]), &struct_out);
    
    struct_complex_delete(&struct_out);    
    cvector_gen_delete(&cvector);
}

static void push_back_two_struct_init_capacity_2()
{
    ccontainer_value_t *pvalue_out_ref;
    struct_complex_t struct_out = {.pname = NULL,.one_float=0.,.is_valid=false};

    ccontainer_err_t err_code;
    struct_complex_t comp_struct, comp_struct2;
    struct_complex_init( &comp_struct, "maiki", 3.14f, true);
    struct_complex_init( &comp_struct2, "toto", 2.31f, false);

    cvector_gen_t cvector;
    err_code = cvector_gen_init_with_capacity(&cvector, 2);
    // struct_complex with dynamic allocation needs to set a deleter_value_t
    cvector_gen_set_deleter(&cvector, struct_complex_deleter_value);

    ccontainer_value_t value_struct,value_struct2;
    value_struct = make_value_struct_complex(&comp_struct, &err_code);
    err_code = cvector_gen_push_back(&cvector, &value_struct);
    
    value_struct2 = make_value_struct_complex(&comp_struct2, &err_code);
    err_code = cvector_gen_push_back(&cvector, &value_struct2);
    assert_int_equal(CCONTAINER_OK, err_code);
    
    struct_complex_delete(&comp_struct);
    struct_complex_delete(&comp_struct2);
    // can reset, if no test ref after
    ccontainer_reset_value( &value_struct );
    ccontainer_reset_value( &value_struct2 );

    // retrieve a reference
    pvalue_out_ref = cvector_gen_get_at(&cvector, 0, &err_code);
    err_code = extract_value_struct_complex(pvalue_out_ref, &struct_out);
    pvalue_out_ref = NULL;
    
    assert_int_equal(0, err_code);
    assert_string_equal("maiki", struct_out.pname);
    assert_float_equal(3.14f, struct_out.one_float, 0.01);
    assert_true(struct_out.is_valid);

    struct_complex_delete(&struct_out);
    cvector_gen_delete(&cvector);
}

static void push_back_two_struct_init_capacity_0()
{
    ccontainer_value_t *pvalue_out_ref;
    struct_complex_t struct_out = {.pname = NULL,.one_float=0.,.is_valid=false};

    ccontainer_err_t err_code;
    struct_complex_t comp_struct, comp_struct2;
    struct_complex_init( &comp_struct, "maiki", 3.14f, true);
    struct_complex_init( &comp_struct2, "toto", 2.31f, false);

    cvector_gen_t cvector;
    cvector_gen_init(&cvector);
    cvector_gen_set_deleter(&cvector, struct_complex_deleter_value);
    
    ccontainer_value_t value_struct;
    value_struct = make_value_struct_complex(&comp_struct, &err_code);
    err_code = cvector_gen_push_back(&cvector, &value_struct);
    assert_int_equal(CCONTAINER_OK, err_code);
    
    value_struct = make_value_struct_complex(&comp_struct2, &err_code);
    cvector_gen_push_back(&cvector, &value_struct);
    
    // copy independent of input
    struct_complex_delete(&comp_struct);
    struct_complex_delete(&comp_struct2);
    ccontainer_reset_value(&value_struct);

    // retrieve reference
    pvalue_out_ref = cvector_gen_get_at(&cvector, 0, &err_code);
    err_code = extract_value_struct_complex(pvalue_out_ref, &struct_out);
    // temporary intermediate
    pvalue_out_ref = NULL;
    assert_int_equal(0, err_code);
    assert_string_equal("maiki", struct_out.pname);
    assert_float_equal(3.14f, struct_out.one_float, 0.01);
    assert_true(struct_out.is_valid);

    pvalue_out_ref = cvector_gen_get_at(&cvector, 1, &err_code);
    assert_int_equal(0, err_code);
    // delete previous content of struct_out
    err_code = extract_value_struct_complex(pvalue_out_ref, &struct_out);
    pvalue_out_ref = NULL;

    assert_int_equal(0, err_code);
    assert_string_equal("toto", struct_out.pname);
    assert_float_equal(2.31f, struct_out.one_float, 0.01);
    assert_false(struct_out.is_valid);

    struct_complex_delete(&struct_out);
    cvector_gen_delete(&cvector);
}

static void copy_constructor()
{
    ccontainer_err_t err_code;
    struct_complex_t comp_struct, comp_struct2;
    struct_complex_init( &comp_struct, "maiki", 3.14f, true);
    struct_complex_init( &comp_struct2, "toto", 2.31f, false);

    cvector_gen_t cvector;
    cvector_gen_t cvector_copy;
    
    err_code = cvector_gen_init_with_capacity(&cvector,5);
    cvector_gen_set_deleter(&cvector, struct_complex_deleter_value);
    cvector_gen_set_duplicater(&cvector, struct_complex_duplicater_value);
    
    ccontainer_value_t value_struct;
    value_struct = make_value_struct_complex(&comp_struct, &err_code);
    err_code = cvector_gen_push_back(&cvector, &value_struct);
    assert_int_equal(CCONTAINER_OK, err_code);
    
    value_struct = make_value_struct_complex(&comp_struct2, &err_code);
    err_code = cvector_gen_push_back(&cvector, &value_struct);

    // copy independent of input, can delete
    struct_complex_delete(&comp_struct);
    struct_complex_delete(&comp_struct2);

    cvector_copy = cvector_gen_copy( &cvector, &err_code);
    assert_int_equal(CCONTAINER_OK, err_code);

    cvector_gen_delete( &cvector );

    assert_int_equal( 2, cvector_copy.len );
    assert_int_equal( 2, cvector_copy.capacity );

    cvector_gen_delete( &cvector_copy );
}

/* if we want a ref, able to modifiy inside the vector => neep wrapper */
/*
static void wrapper_ref_from_vector()
{
    ccontainer_err_t err_code;
    struct_complex_t comp_struct_in;
    struct_complex_init( &comp_struct_in, "maiki", 3.14f, true);

    cvector_gen_t cvector;
    ccontainer_value_t *pvalue_out_ref;
    ccontainer_value_t value_struct;

    cvector_gen_init(&cvector); // call default duplicater
    // struct_complex with dynamic allocation
    cvector_gen_set_deleter(&cvector, struct_complex_deleter_value);

    value_struct = make_value_struct_complex(&comp_struct_in, &err_code);
    // push back gives ownership if some memory allocated
    err_code = cvector_gen_push_back(&cvector, &value_struct);
    assert_int_equal(CCONTAINER_OK, err_code);
    
    ccontainer_delete_value(&value_struct);

    // retrieve reference as before
    pvalue_out_ref = cvector_gen_get_at(&cvector, 0, &err_code);
    //err_code = extract_value_struct_complex(pvalue_out_ref, &struct_out);

    cvector_gen_delete(&cvector);
    struct_complex_delete(&comp_struct_in);
}
*/

/* ********  Helper ********* */
/** Check it is a copy, source is not modified */
void check_copy_in_value(ccontainer_value_t *value, struct_complex_t *comp_struct )
{
    char memo = comp_struct->pname[0];
    // check they don't point to same internal copy
    comp_struct->pname[0] = 'P';
    char t = *( (char **)value->data)[0];
    assert_int_not_equal( 'P', t);
    assert_int_equal( memo, t);

    // revert
    comp_struct->pname[0] = memo;
}

/** Check it is a reference, can modify the source. */
void check_ref_in_value(ccontainer_value_t *value, struct_complex_t *comp_struct )
{
    char memo = comp_struct->pname[0];
    // check they don't point to same internal copy
    comp_struct->pname[0] = 'P';
    char t = *( (char **)value->data)[0];
    assert_int_equal( 'P', t);

    // revert for both
    comp_struct->pname[0] = memo;
}

/** @} */ /* end group ccontainer_adapt_own_struture */

/** \private */
int main()
{
    /* can be inside main, or as global. here no problem with identical name */
    const struct CMUnitTest tests_cvector_struct_complex[] = {
        cmocka_unit_test(init_complex_structure_on_heap),
        cmocka_unit_test(init_complex_structure_on_stack),
        cmocka_unit_test(serialization_struct),
        cmocka_unit_test(deleter_and_duplicater_struct),
        cmocka_unit_test(push_back_one_struct),
        cmocka_unit_test(push_back_two_struct_init_capacity_2),
        cmocka_unit_test(push_back_two_struct_init_capacity_0),
        cmocka_unit_test(copy_constructor),
        //cmocka_unit_test(wrapper_ref_from_vector),
     };

    /* call group_setup and teardown at the very beginning and end */
    /* return cmocka_run_group_tests(tests_historique, group_setup, group_teardown);*/
    return cmocka_run_group_tests(tests_cvector_struct_complex, NULL, NULL);
}

