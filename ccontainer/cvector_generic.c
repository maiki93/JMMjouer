#include "ccontainer/cvector_generic.h"
#include <string.h>
#include <assert.h>

/* Call deleter_value on elements in range[begin,last[ */
static void delete_elements( cvector_gen_t *cvect, size_t index_begin, size_t index_last );

/* Constructor/destructor */
cvector_gen_t* cvector_gen_new()
{
    return (cvector_gen_t*) malloc( sizeof(cvector_gen_t) );
}

void cvector_gen_init( cvector_gen_t* cvect )
{
    cvect->array = NULL;
    cvect->capacity = 0;
    cvect->len = 0;
    /* default copy and destructor */
    cvect->ptrf_deleter = default_deleter_value;
    cvect->ptrf_duplicater = default_duplicater_value;
}

ccontainer_err cvector_gen_init_with_capacity( cvector_gen_t* cvect, size_t init_capacity)
{
    /* swap idiom for memroy allocation */
    ccontainer_value_t *tab = calloc( init_capacity, sizeof(ccontainer_value_t) );
    if( tab == NULL )
        return CCONTAINER_ALLOCERR;
    
    cvect->array = tab;
    /* capacity 2, size still 0 */
    cvect->capacity = init_capacity;
    cvect->len = 0;
    /* default copy and destructor */
    cvect->ptrf_deleter = default_deleter_value;
    cvect->ptrf_duplicater = default_duplicater_value;
    return CCONTAINER_OK;
}

void cvector_gen_delete( cvector_gen_t* cvect )
{
    if( !cvect )
        return;

    /* free memory allocated by all valid ccontainer_value_t (size) */
    if( cvect->len > 0 )
        cvector_gen_clear(cvect);
    /* free memory of the array of ccontainer_value_t (capacity) */
    if( cvect->array )
        free(cvect->array);

    /* not necessary, will be invalidated by free but safer if retest */
    cvect->array = NULL;
    cvect->capacity = 0;
    cvect->len = 0;
}

void cvector_gen_free(cvector_gen_t *cvect)
{
    cvector_gen_delete( cvect );
    free( cvect );
}

/* setters of specific copy and destruction */
void cvector_gen_set_deleter( cvector_gen_t *cvect, deleter_value_t fct_deleter )
{
    if( fct_deleter )
        cvect->ptrf_deleter = fct_deleter;
}

void cvector_gen_set_duplicater( cvector_gen_t *cvect, duplicater_value_t fct_duplicater )
{
    if( fct_duplicater)
        cvect->ptrf_duplicater = fct_duplicater;
}

/* ********* Behavior *********** */

/* delete all valid (len) ccontainer_value_t, others(capacity) should not be allocated */
void cvector_gen_clear( cvector_gen_t *cvect )
{
    if( cvect && cvect->array )
        delete_elements( cvect, 0,  cvect->len);
    cvect->len = 0;
}

size_t cvector_gen_size(const cvector_gen_t *cvect)
{
    return cvect->len;
}

size_t cvector_gen_capacity(const cvector_gen_t *cvect)
{
    return cvect->capacity;
}

ccontainer_err cvector_gen_set_capacity(cvector_gen_t *cvect, size_t new_capacity)
{
    ccontainer_value_t *tmp;
    size_t i, memo_len = cvect->len;

    /* nothing to do, it is large enought
        Improvment allow to decrease the size ? */
    if( new_capacity <= cvect->capacity)
        return CCONTAINER_OK;

    /* temporary variable in case of allocation failure, initialized to 0 */
    tmp = (ccontainer_value_t*) calloc( new_capacity, sizeof(ccontainer_value_t) );
    if( tmp == NULL )
        return CCONTAINER_ALLOCERR;

    /* copy previous content, copy byte-to-byte respect values and/or pointer */
    if( cvect->array ) {
        for(i = 0; i< cvect->len; i++)
            /*tmp[i] = (*cvect->ptrf_duplicater)( cvect->array + i );*/
            tmp[i] = ccontainer_move_value( cvect->array + i );
    }

    /* free previous value_t in array properly */
    /* cvector_gen_delete( cvect );  // will delete the string associated */
    /* with move no need of deleting */
    /*
    for(i = 0; i< cvect->len; i++ ) {
        free_value( cvect->array + i );
    }*/
    if( cvect->array )
        free( cvect->array );

    /* reset new attributes of cvect, len does not change */
    cvect->array = tmp;
    cvect->len = memo_len;
    cvect->capacity = new_capacity;
    return CCONTAINER_OK;
}

ccontainer_err cvector_gen_push_back(cvector_gen_t *cvect, const ccontainer_value_t *value_in)
{
    ccontainer_err code_err;

    if( (code_err = cvector_gen_set_capacity( cvect, cvect->len+1 )) != CCONTAINER_OK ) {
        return code_err;
    }

    assert( cvect->capacity >= cvect->len);
    /* copy byte to byte value_in at the last position of the array */
    /*cvect->array[cvect->len] = (*cvect->ptrf_duplicater)(value_in);*/
    ccontainer_copy_value( cvect->array + cvect->len, value_in);
    cvect->len++;

    /* all attributes of cvect have been reset */
    return CCONTAINER_OK;
}

ccontainer_value_t* cvector_gen_get_at(const cvector_gen_t *cvect, size_t index, ccontainer_err* err_code)
{
    /* size_t always positive */
    if( index >= cvect->len) {
        *err_code = CCONTAINER_OUTOFRANGE;
        return NULL;
    }
    *err_code = CCONTAINER_OK;
    /*return &(cvect->array[index]); */ /* <=> array + index */
    return cvect->array + index;
}
/* to delete
ccontainer_err cvector_gen_get_copy(cvector_gen_t *cvect, size_t index, value_t* value_out)
{
    value_t value_tmp;
    // check range
    if( index >= cvect->len) {
        return CCONTAINER_OUTOFRANGE;
    }
    // delete previous value_out if already allocated
    if( value_out && value_out->data) {
        assert(value_out->len != 0); // if data len should != 0 
        // cvect->ptrf_deleter( value_out );
        free_value(value_out);
    }

    // make full duplication
    //value_tmp = (*cvect->ptrf_duplicater)( cvect->array + index );
    //copy_value(value_out,&value_tmp);
    
    // copy_value(value_out, cvect->array + index); 
    // *value_out = (*cvect->ptrf_duplicater)( cvect->array + index );

    return CCONTAINER_OK;
}
*/
/* Methods for algorithms */
void cvector_gen_swap(cvector_gen_t *cvect, size_t index1, size_t index2)
{
    ccontainer_value_t value_tmp;
    /* array[ind1]->tmp */
    value_tmp = ccontainer_move_value( cvect->array + index1 );
    /* array[ind2]->array[ind1] */
    cvect->array[index1] = ccontainer_move_value( cvect->array + index2 );
    /* tmp->array[ind2] */
    cvect->array[index2] = ccontainer_move_value( &value_tmp );
}

/******* Private methods *******/
void delete_elements( cvector_gen_t *cvect, size_t index_begin, size_t index_last )
{
    size_t i;
    for( i = index_begin; i < index_last; i++ ) {
        (*cvect->ptrf_deleter)( cvect->array + i);
    }
}

/* static copy_elements( cvector_gen_t *cvect, index_begin, index_end, shift ) */
