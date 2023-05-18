#include <string.h>
#include <assert.h>

#include "ccontainer/clist_cstring.h"
#include "ccontainer/value_cstring.h"

/* **** Public **** */
void carray_cstring_delete( char **carray, size_t len)
{
    size_t i;
    for( i = 0; i < len; ++i) {
        free(carray[i]);
        carray[i] = NULL;
    }
    free(carray);
}

/* ****** Constructor / Destructor / Copy ********** */
clist_cstring_t* clist_cstring_new()
{
    return (clist_cstring_t*)calloc( 1, sizeof(clist_cstring_t) );
}

void clist_cstring_init(clist_cstring_t* clist_str)
{
    clist_str->clist = clist_gen_new();
    clist_gen_init( clist_str->clist );
}

void clist_cstring_delete(clist_cstring_t* clist_str)
{
    /* delete content */
    clist_gen_delete( clist_str->clist, deleter_value_cstring);
    assert( clist_str->clist != NULL);
    assert( clist_gen_size( clist_str->clist) == 0);
    /* free mem clist, leak memory otherwise */
    free(clist_str->clist);
    clist_str->clist = NULL;
}

/* deleter specific provided for value_cstring */
void clist_cstring_free(clist_cstring_t* clist_str)
{
    clist_cstring_delete( clist_str );
    free(clist_str);
}

size_t clist_cstring_size(clist_cstring_t *clist_str)
{
    return clist_gen_size( clist_str->clist );
}

ccontainer_err clist_cstring_push_back(clist_cstring_t *clist_str, const char *to_insert)
{
    ccontainer_value_t tmp_value_in;
    ccontainer_err err_code;
    assert( clist_str );
    assert( to_insert );

    tmp_value_in = make_value_cstring( to_insert, &err_code );
    if( err_code != CCONTAINER_OK ) {
        return err_code;
    }
    return clist_gen_push_back( clist_str->clist, &tmp_value_in );
}

char* clist_cstring_get_copy_at( clist_cstring_t *list, size_t index, ccontainer_err *err_code )
{
    char* string_out;
    ccontainer_value_t* pvalue_out;

    assert( list );
    /* error treated in clist_gen_t */
    if( (pvalue_out = clist_gen_get_at( list->clist, index, err_code)) == NULL ) {
        return NULL;
    }
    string_out = extract_value_cstring( pvalue_out, err_code );
    return string_out;
}

char* clist_cstring_get_ref_at( clist_cstring_t *clist_str, size_t index, ccontainer_err *err_code)
{
    ccontainer_value_t *pvalue;
    char *string_out;

    assert(clist_str);
    string_out = NULL;
    
    if( (pvalue = clist_gen_get_at( clist_str->clist, index, err_code)) == NULL ) {
        return NULL;
    }
    /* valid because data contains the null caracter at the end of the string */
    string_out = pvalue->data;
    return string_out;
}

int clist_cstring_get_array( clist_cstring_t *list, char ***array_out, size_t *array_len )
{
    /*ccontainer_value_t value_out;*/
    ccontainer_value_t* pvalue_out; /* temporary value */
    ccontainer_err err_code;
    char **array = NULL; /* use local only */
    size_t lgen_len, indice;
    
    assert( list );
    /* default return in case of error, use swap idiom */
    *array_out = NULL;
    *array_len = 0;
    /* get size for allocation of array of cstring */
    if( (lgen_len = clist_gen_size( list->clist)) == 0 ) {
        return CCONTAINER_EMPTY;
    }
    array = (char**) malloc( lgen_len * sizeof(char*) );
    if( !array )
        return CCONTAINER_OK;
    /* iterative copy of ccontainer_value_t::data into array of cstring */
    for( indice = 0; indice < lgen_len; indice++) {
        /*
        if( (retour = clist_gen_get_value_copy( list->clist, indice, &value_out)) 
                    != CLIST_OK ) {
            return retour;
        }*/
        if( (pvalue_out = clist_gen_get_at( list->clist, indice, &err_code)) == NULL ) {
            return 1;
        }

        array[indice] = extract_value_cstring( pvalue_out, &err_code );
        /* same as other dr memory leak */
        /*free( value_out.data );*/
        /*value_out.len = 0;*/
        /* or free_value( & value_out ); */
    }
    /* finally swap output */
    *array_len = lgen_len;
    *array_out = array;
    return CCONTAINER_OK;
}
