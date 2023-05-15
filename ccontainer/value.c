
#include "ccontainer/value.h"
/*#include "ccontainer/error_code.h"*/
#include "string.h"
#include "assert.h"

/* methods to create/delete ccontainer_value_t */
ccontainer_value_t ccontainer_make_value(char *data, size_t len, ccontainer_err *err_code) 
{
    ccontainer_value_t value_out;
    char *tmp_string;

    assert( data != NULL );
    
    tmp_string = (char *)malloc( len );
    if( !tmp_string ) {
        *err_code = CCONTAINER_ALLOCERR;
        ccontainer_reset_value(&value_out); /* to assur data pointer is NULL */
        return value_out;
    }
    memcpy(tmp_string, data, len);

    value_out.data = tmp_string; /* (char*) if pass const char* */
    value_out.len = len;
    *err_code = CCONTAINER_OK;
    return value_out;
}

ccontainer_value_t ccontainer_copy_value(const ccontainer_value_t *value_src, ccontainer_err *err_code)
{
    ccontainer_value_t value_out;
    char * tmp_buf;

    assert( value_src && value_src->data );

    tmp_buf = (char*)malloc( value_src->len );
    if( !tmp_buf ) {
        *err_code = CCONTAINER_ALLOCERR;
        ccontainer_reset_value(&value_out);
        return value_out;
    }
    /* testing allocation error, need (some) error_code */
    memcpy( tmp_buf, value_src->data, value_src->len);
    /* fill return value */
    value_out.data = tmp_buf;
    value_out.len = value_src->len;
    *err_code = CCONTAINER_OK;
    return value_out;
}

ccontainer_value_t ccontainer_move_value(ccontainer_value_t *value_src)
{
    ccontainer_value_t value_out;

    assert( value_src && value_src->data );

    memcpy(&value_out, value_src, sizeof(ccontainer_value_t));
    /* invalidate the source */
    value_src->data = NULL;
    value_src->len = 0;
    return value_out;
}

void ccontainer_reset_value(ccontainer_value_t *value_in_out)
{
    value_in_out->data = NULL;
    value_in_out->len = 0;
}

/* delete : destructor */
void ccontainer_delete_value(ccontainer_value_t *value_in_out)
{
    if( value_in_out && value_in_out->data)
        free(value_in_out->data);
    value_in_out->data = NULL;
    value_in_out->len = 0;
}

/* delete and free memory of value_in */
void ccontainer_free_value(ccontainer_value_t *value_in)
{
    ccontainer_delete_value( value_in );
    free( value_in ); 
}
