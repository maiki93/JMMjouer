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
    cvect->ptrf_deleter = ccontainer_delete_value;
    cvect->ptrf_duplicater = ccontainer_copy_value;
}

ccontainer_err_t cvector_gen_init_with_capacity( cvector_gen_t* cvect, size_t init_capacity)
{
    /* memory allocation of array of ccontainer_value_t */
    ccontainer_value_t *tab = calloc( init_capacity, sizeof(ccontainer_value_t) );
    if( tab == NULL )
        return CCONTAINER_ALLOCERR;
    
    cvect->array = tab;
    /* capacity 2, size still 0 */
    cvect->capacity = init_capacity;
    cvect->len = 0;
    /* default copy and destructor */
    cvect->ptrf_deleter = ccontainer_delete_value;
    cvect->ptrf_duplicater = ccontainer_copy_value;
    return CCONTAINER_OK;
}

cvector_gen_t cvector_gen_copy(const cvector_gen_t* cvect_src, ccontainer_err_t *err_code)
{
    cvector_gen_t cvect_out;
    size_t i;
    assert( cvect_src );

    cvector_gen_init_with_capacity( &cvect_out, cvect_src->len );
    cvect_out.ptrf_deleter = cvect_src->ptrf_deleter;
    cvect_out.ptrf_duplicater = cvect_src->ptrf_duplicater;

    /* size to set manually or should call push back */
    cvect_out.len = cvect_src->len;
    /* deep_copy of all elements (up to len) */
    for( i = 0; i < cvect_src->len; i++) {
        cvect_out.array[i] = (*cvect_out.ptrf_duplicater)( cvect_src->array + i, err_code);
        if( *err_code != CCONTAINER_OK ) {
            return cvect_out;
        }
    }
    
    assert( cvect_out.len == cvect_src->len );
    assert( cvect_out.len <= cvect_src->capacity );

    *err_code = CCONTAINER_OK;
    return cvect_out;
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

ccontainer_err_t cvector_gen_set_capacity(cvector_gen_t *cvect, size_t new_capacity)
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
            tmp[i] = ccontainer_move_value( cvect->array + i );
        /* deallocate previous array */
        free( cvect->array );
    }

    /* reset new attributes of cvect with the new capacity, len does not change */
    cvect->array = tmp;
    cvect->len = memo_len;
    cvect->capacity = new_capacity;
    return CCONTAINER_OK;
}

ccontainer_err_t cvector_gen_push_back(cvector_gen_t *cvect, ccontainer_value_t *value_in)
{
    ccontainer_err_t code_err;

    if( (code_err = cvector_gen_set_capacity( cvect, cvect->len+1 )) != CCONTAINER_OK ) {
        return code_err;
    }

    assert( cvect->capacity >= cvect->len);

    /* ownership is transfered to the vector */
    cvect->array[ cvect->len ] = ccontainer_move_value(value_in);
    cvect->len++;

    /* all attributes of cvect have been reset */
    return CCONTAINER_OK;
}

ccontainer_value_t* cvector_gen_get_at(const cvector_gen_t *cvect, size_t index, ccontainer_err_t* err_code)
{
    /* size_t always positive */
    if( index >= cvect->len) {
        *err_code = CCONTAINER_OUTOFRANGE;
        return NULL;
    }
    *err_code = CCONTAINER_OK;
    return cvect->array + index; /* <=> &(cvect->array[index]) */
}

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

/* nice to find the 10 best, may be even better than quicksort algo 
   just need additional parameter 0 : all, 10 : while ... && ((init_size-size_array ) < 10 */
void cvector_bubble_sort(cvector_gen_t *cvect, comparater_value_t fct_comparater)
{
    size_t size_array, i;
    bool b_swap;
 
    size_array = cvect->len;
    
    do {
        b_swap = false;
        for( i = 1; i < size_array; i++)
        {
            /* if ( (arr[i-1] > arr[i] ) swap( arr[i-1], arr[i] ) */
            if( (*fct_comparater)( cvect->array + (i-1), 
                                   cvect->array + i) ) 
            {
                cvector_gen_swap( cvect, i-1, i );
                b_swap = true;
            }
        }
        /* bigger always placed first, reduce inner loop */
        size_array--;
    } while( b_swap == true);

    return;
}

/******* Private methods *******/
void delete_elements( cvector_gen_t *cvect, size_t index_begin, size_t index_last )
{
    size_t i;
    for( i = index_begin; i < index_last; i++ ) {
        (*cvect->ptrf_deleter)( cvect->array + i);
    }
}

