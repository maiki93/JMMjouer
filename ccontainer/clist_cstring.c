#include <string.h>
#include <assert.h>

#include "ccontainer/clist_cstring.h"
#include "ccontainer/value_cstring.h"

/* ****** Constructor / Destructor / Copy ********** */
clist_cstring_t* clist_cstring_new()
{
    return (clist_cstring_t*)calloc( 1, sizeof(clist_cstring_t) );
}

void clist_cstring_init(clist_cstring_t* clist_str)
{
    clist_str->clist = clist_gen_new();
    clist_gen_init( clist_str->clist );
    clist_str->len = 0;
}

void clist_cstring_delete(clist_cstring_t* clist_str)
{
    /* delete content */
    clist_gen_free( clist_str->clist, deleter_value_cstring);
    /*free(clist_str->clist);*/

    /*assert( clist_str->clist != NULL);*/
    /*assert( clist_gen_size( clist_str->clist) == 0);*/
    clist_str->clist = NULL;
    clist_str->len = 0;
}

/* deleter specific provided for value_cstring */
void clist_cstring_free(clist_cstring_t* clist_str)
{
    clist_cstring_delete( clist_str );
    free(clist_str);
}

size_t clist_cstring_size(clist_cstring_t *clist_str)
{
    return clist_str->len;
}

ccontainer_err_t clist_cstring_push_back(clist_cstring_t *clist_str, const char *to_insert)
{
    ccontainer_value_t tmp_value_in;
    ccontainer_err_t err_code;
    assert( clist_str );
    assert( to_insert );

    tmp_value_in = make_value_cstring( to_insert, &err_code );
    if( err_code != CCONTAINER_OK ) {
        return err_code;
    }
    err_code = clist_gen_push_back( clist_str->clist, &tmp_value_in );
    if( err_code == CCONTAINER_OK )
        clist_str->len++;
    return err_code;
}

char* clist_cstring_pop_front( clist_cstring_t *clist_str, ccontainer_err_t *err_code)
{
    ccontainer_value_t tmp_value;
    char *string_out;

    assert(clist_str);
    string_out = NULL;
    
    tmp_value = clist_gen_pop_front( clist_str->clist, err_code);
    if( *err_code != CCONTAINER_OK)
        return NULL;
    clist_str->len--;
    string_out = extract_value_cstring( &tmp_value, err_code);
    /* tmp copy to delete */
    ccontainer_delete_value( &tmp_value);
    return string_out;
}

/*
int clist_cstring_get_array( clist_cstring_t *list, char ***array_out, size_t *array_len )
{
    ccontainer_value_t* pvalue_out; // temporary value
    ccontainer_err_t err_code;
    char **array = NULL; // use local only
    size_t lgen_len, indice;
    
    assert( list );
    // default return in case of error, use swap idiom
    *array_out = NULL;
    *array_len = 0;
    // get size for allocation of array of cstring
    if( (lgen_len = clist_gen_size( list->clist)) == 0 ) {
        return CCONTAINER_EMPTY;
    }
    array = (char**) malloc( lgen_len * sizeof(char*) );
    if( !array )
        return CCONTAINER_OK;
    // iterative copy of ccontainer_value_t::data into array of cstring
    for( indice = 0; indice < lgen_len; indice++) {
        
        if( (pvalue_out = clist_gen_get_at( list->clist, indice, &err_code)) == NULL ) {
            return 1;
        }

        array[indice] = extract_value_cstring( pvalue_out, &err_code );
    }
    // finally swap output
    *array_len = lgen_len;
    *array_out = array;
    return CCONTAINER_OK;
}
*/

ccontainer_err_t clist_cstring_get_array( clist_cstring_t *list_str, char ***array_out, size_t *array_len )
{
    ccontainer_value_t* pvalue_out; /* temporary value */
    clist_node_t* curr_node;
    ccontainer_err_t err_code;
    char **array = NULL; /* use local only */
    size_t lgen_len, indice;
    
    assert( list_str );
    /* default return in case of error, use swap idiom */
    *array_out = NULL;
    *array_len = 0;
    /* get size for allocation of array of cstring */
    if( (lgen_len = clist_gen_size( list_str->clist)) == 0 ) {
        return CCONTAINER_EMPTY;
    }
    array = (char**) malloc( lgen_len * sizeof(char*) );
    if( !array )
        return CCONTAINER_OK;
        
   indice = 0;
   curr_node = clist_gen_get_first_node( list_str->clist );
   while( curr_node != NULL )
   {
        pvalue_out = clist_gen_get_node_value( curr_node) ;
        array[indice++] = extract_value_cstring( pvalue_out, &err_code );
        curr_node = clist_gen_get_next_node(curr_node);
   }

    /* finally swap output */
    *array_len = lgen_len;
    *array_out = array;
    return CCONTAINER_OK;
}
