#include <assert.h>

#include "ccontainer/ccontainer_utils.h"

void carray_cstring_delete( char **carray, size_t len)
{
    size_t i;
    for( i = 0; i < len; ++i) {
        free(carray[i]);
        carray[i] = NULL;
    }
    free(carray);
}

/* move value_t from a clist into a cvector : make "a move" could apply to al lsorts of value_t */
cvector_gen_t* transfert_clist_to_cvector_gen(clist_gen_t* clist, ccontainer_err_t *err_code)
{
    cvector_gen_t *cvect_out;
    ccontainer_value_t tmp_value;
    size_t size_list;
    ccontainer_err_t err_code_pop, err_code_push;

    size_list = clist_gen_size( clist );

    /* if list empty => return NULL and CCONTAINER_EMPTY */
    if( size_list == 0 ) {
        *err_code = CCONTAINER_EMPTY;
        return NULL;
    }

    cvect_out = cvector_gen_new();
    cvector_gen_init_with_capacity( cvect_out, size_list );

    /* sure it is not empty : first pop correct for sure
        and pop until loop is empty */
    tmp_value = clist_gen_pop_front( clist, &err_code_pop );
    assert(err_code_pop == CCONTAINER_OK);

    while ( err_code_pop != CCONTAINER_EMPTY ) {
        /* error empty possible */
        /* no reason to return error, already allocated with correct capacity.
           tmp_value is moved, invalid after call */
        err_code_push = cvector_gen_push_back( cvect_out, &tmp_value );
        assert(err_code_push == CCONTAINER_OK);
        assert( !tmp_value.data ); /* assert NULL */
        assert(tmp_value.len == 0);

        tmp_value = clist_gen_pop_front( clist, &err_code_pop );
    };
    /* it is NULL, the last one on exit */
    /*ccontainer_delete_value( &tmp_value );*/
    return cvect_out;
}

