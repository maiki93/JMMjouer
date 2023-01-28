#include <string.h>
#include <assert.h>

#include "clist_cstring.h"

/*** private ***/
static value_t make_value_cstring( const char* text_to_insert);
/* @brief Return heap allocated cstring for value_t.
    Null terminaison character will be added (not present in value_t::data)
    Must be free by caller */
static char * extract_value_cstring_copy( value_t *value_in );

/* maybe a default deleter to give to user ? 
   works until value does not contain allocated memory himself */
   /* here a copy of deleter in test_clist_generic, default enought */
static void deleter_cstring( value_t *value);

/***** Public *****/
void carray_cstring_delete( char **carray, size_t len)
{
    size_t i;
    for( i = 0; i < len; ++i) {
        free(carray[i]);
        carray[i] = NULL;
    }
    free(carray);
}

/** Constructor / Destructor */
clist_cstring_t* clist_cstring_new()
{
    return (clist_cstring_t*)calloc( 1, sizeof(clist_cstring_t) );
}

void clist_cstring_init(clist_cstring_t* list)
{
    list->clist = clist_gen_new();
    clist_gen_init( list->clist );
}

void clist_cstring_clear(clist_cstring_t* list)
{
    /* delete content */
    clist_gen_clear( list->clist, deleter_cstring);
    assert( list->clist != NULL);
    assert( clist_gen_size( list->clist) == 0);
    /* free mem clist, leak memory otherwise */
    free(list->clist);
    list->clist = NULL;
}
/* deleter fixed for cstring */
void clist_cstring_delete(clist_cstring_t* list)
{
    /* clear the list, and free(list->clist)  */
    clist_gen_del( list->clist, deleter_cstring);
    /* better if ! list->clist and call cstring_clear; more uniform */
    list->clist = NULL;
    free(list);
}

size_t clist_cstring_size(clist_cstring_t *list)
{
    return clist_gen_size( list->clist );
}

int clist_cstring_push_back(clist_cstring_t *list, const char *to_insert)
{
    value_t value_in;
    int retour;
    assert( list );
    assert( to_insert );

    value_in = make_value_cstring( to_insert );
    retour = clist_gen_push_back( list->clist, &value_in );
    /* TODO dr_memory reports missing, would need access to deleter value_t 
        or avoid this allocation (optimization later)*/
    free( value_in.data );
    value_in.len = 0;
    return retour;
}

int clist_cstring_get_copy( clist_cstring_t *list, size_t elem_nb, char **string_out )
{
    value_t value_out;
    int retour;
    assert( list );
    /* default in case of error */
    *string_out = NULL;

    if( (retour = clist_gen_get_value_copy( list->clist, elem_nb, &value_out)) 
                != CLIST_OK ) {
        return retour;
    }
    *string_out = extract_value_cstring_copy( &value_out );

    /* TODO dr_memory reports LEAK, would need access to deleter value_t 
        or avoid this allocation (optimization later)*/
    free( value_out.data );
    value_out.len = 0;
    return retour;
}

int clist_cstring_get_ref( clist_cstring_t *list, size_t elem_nb, char **string_out)
{
    value_t *value_ref_out;
    int retour;

    *string_out = NULL;

    if( (retour = clist_gen_get_value_ref( list->clist, elem_nb, &value_ref_out))
                != CLIST_OK) 
    {
        return retour;
    }
    /* valid because data contains the null caracter at the end of the string */
    *string_out = value_ref_out->data;
    return retour;
}

int clist_cstring_get_array( clist_cstring_t *list, char ***array_out, size_t *array_len )
{
    value_t value_out;
    char **array = NULL; /* use local only */
    size_t lgen_len, indice;
    int retour = 0;
    assert( list );
    /* default return in case of error, use swap idiom */
    *array_out = NULL;
    *array_len = 0;
    /* get size for allocation of array of cstring */
    if( (lgen_len = clist_gen_size( list->clist)) == 0 ) {
        return CLIST_EMPTY;
    }
    array = (char**) malloc( lgen_len * sizeof(char*) );
    if( !array )
        return CLIST_ALLOC_ERR;
    /* iterative copy of value_t::data into array of cstring */
    for( indice = 0; indice < lgen_len; indice++) {
        if( (retour = clist_gen_get_value_copy( list->clist, indice, &value_out)) 
                    != CLIST_OK ) {
            return retour;
        }
        array[indice] = extract_value_cstring_copy( &value_out );
        /* same as other dr memory leak */
        free( value_out.data );
        value_out.len = 0;
        /* or free_value( & value_out ); */
    }
    /* finally swap output */
    *array_len = lgen_len;
    *array_out = array;
    return retour;
}

value_t make_value_cstring( const char* text_to_insert)
{
    value_t value;
    size_t text_len;
    /* include the terminaison caracter */
    text_len = strlen(text_to_insert) + 1;
    value.data = (char*)malloc(text_len);
    /* strcpy will copy the '\0' if possible: ok len+1 than source */
    strcpy( value.data, text_to_insert);
    value.len = text_len;
    return value;
}

char * extract_value_cstring_copy( value_t *value_in )
{
    char *string_out;
    /* value_t::data and len include the terminaison caracter */
    /* memcpy may be faster in this case , TODO to check */
    string_out = (char*)malloc( value_in->len );
    /* chaine source taille inférieur, 
        destination will contain the terminaison caracter !! */
    /*strncpy( string_out, value_in->data, text_len );*/
    /* strcpy copy terminaison carater if possible */
    strcpy( string_out, value_in->data );
    /* string_out[ text_len ] = 0; */ /* <=> '\0' */

    return string_out;
}

void deleter_cstring( value_t *value)
{
    if( value ) {
        free(value->data);
        value->data = NULL;
    }
    value->len = 0;
}