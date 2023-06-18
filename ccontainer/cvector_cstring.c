#include <string.h>
#include <assert.h>

#include "ccontainer/cvector_cstring.h"
#include "ccontainer/value_cstring.h"


/* ****** Constructor / Destructor / Copy ********** */
cvector_cstring_t* cvector_cstring_new()
{
    return (cvector_cstring_t*)calloc( 1, sizeof(cvector_cstring_t) );
}

void cvector_cstring_init(cvector_cstring_t* cvector_str)
{
    cvector_str->cvector = cvector_gen_new();
    cvector_gen_init( cvector_str->cvector );
    cvector_gen_set_deleter( cvector_str->cvector, deleter_value_cstring );
    cvector_gen_set_duplicater( cvector_str->cvector, duplicater_value_cstring );
}

void cvector_cstring_init_with_capacity(cvector_cstring_t* cvector_str, size_t init_capacity)
{
    cvector_str->cvector = cvector_gen_new();
    cvector_gen_init_with_capacity( cvector_str->cvector, init_capacity );
    cvector_gen_set_deleter( cvector_str->cvector, deleter_value_cstring );
    cvector_gen_set_duplicater( cvector_str->cvector, duplicater_value_cstring );
}

void cvector_cstring_clear(cvector_cstring_t *cvector_str)
{
    cvector_gen_clear( cvector_str->cvector );
}

void cvector_cstring_delete(cvector_cstring_t* cvector_str)
{
    /* delete content */
    cvector_gen_free( cvector_str->cvector );
    cvector_str->cvector = NULL;
}

/* deleter specific provided for value_cstring */
void cvector_cstring_free(cvector_cstring_t* cvector_str)
{
    cvector_cstring_delete( cvector_str );
    free(cvector_str);
}

size_t cvector_cstring_size(const cvector_cstring_t *cvector_str)
{
    return cvector_gen_size(cvector_str->cvector);
}

size_t cvector_cstring_capacity(const cvector_cstring_t *cvector_str)
{
    return cvector_gen_capacity(cvector_str->cvector);
}

ccontainer_err_t cvector_cstring_set_capacity(cvector_cstring_t *cvector_str, size_t new_capacity)
{
    return cvector_gen_set_capacity( cvector_str->cvector, new_capacity );
}

char* cvector_cstring_get_copy_at( const cvector_cstring_t *cvector_str, size_t index, ccontainer_err_t *err_code)
{
    ccontainer_value_t *pvalue_out;
    /*ccontainer_err_t err_code;*/

    pvalue_out = cvector_gen_get_at( cvector_str->cvector, index, err_code);
    return extract_value_cstring( pvalue_out, err_code);
}

const char* cvector_cstring_get_ref_at( const cvector_cstring_t *cvector_str, size_t index, ccontainer_err_t *err_code)
{
    ccontainer_value_t *pvalue_out;
    
    pvalue_out = cvector_gen_get_at( cvector_str->cvector, index, err_code);
    return pvalue_out->data;
}


ccontainer_err_t cvector_cstring_push_back(cvector_cstring_t *cvector_str, const char *to_insert)
{
    ccontainer_value_t tmp_value_in;
    ccontainer_err_t err_code;
    assert( cvector_str );
    assert( to_insert );

    tmp_value_in = make_value_cstring( to_insert, &err_code );
    if( err_code != CCONTAINER_OK ) {
        return err_code;
    }
    err_code = cvector_gen_push_back( cvector_str->cvector, &tmp_value_in );
    return err_code;
}

ccontainer_err_t cvector_cstring_get_array( cvector_cstring_t *cvector_str, char ***array_out, size_t *array_len )
{
    ccontainer_err_t err_code;
    char **array = NULL; /* use local only */
    size_t vgen_len, indice;

    const char *str_out;
    
    assert( cvector_str );
    /* default return in case of error, use swap idiom */
    *array_out = NULL;
    *array_len = 0;
    /* get size for allocation of array of cstring */
    if( (vgen_len = cvector_gen_size( cvector_str->cvector)) == 0 ) {
        return CCONTAINER_EMPTY;
    }
    array = (char**) malloc( vgen_len * sizeof(char*) );
    if( !array )
        return CCONTAINER_OK;
   
    for( indice = 0; indice < vgen_len; indice++ )
    {
        str_out = cvector_cstring_get_ref_at( cvector_str, indice, &err_code );
        array[indice] = (char*) malloc( strlen(str_out) + 1 );
        strcpy( array[indice], str_out);
    }
    /* finally swap output */
    *array_len = vgen_len;
    *array_out = array;
    return CCONTAINER_OK;
}
