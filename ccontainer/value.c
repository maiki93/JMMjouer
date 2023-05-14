
#include "ccontainer/value.h"
/*#include "ccontainer/error_code.h"*/
#include "string.h"
#include "assert.h"

/* methods to create/delete ccontainer_value_t */
/*
ccontainer_value_t ccontainer_make_value(char *data, size_t len) {
    ccontainer_value_t value;
    assert( data != NULL );
    value.data = data; // (char*) if pass const char*
    value.len = len;
    return value;
}
*/
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

void ccontainer_copy_value(ccontainer_value_t *value_dest, const ccontainer_value_t *value_src)
{
    memcpy(value_dest, value_src, sizeof(ccontainer_value_t));
}

ccontainer_value_t ccontainer_move_value(ccontainer_value_t *value_src)
{
    ccontainer_value_t value_out;
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

void ccontainer_delete_value(ccontainer_value_t *value_in_out)
{
    if( value_in_out && value_in_out->data)
        free(value_in_out->data);
    value_in_out->data = NULL;
    value_in_out->len = 0;
}

/* delete_value : destructor */
void ccontainer_free_value(ccontainer_value_t *value_in)
{
    ccontainer_delete_value( value_in );
    free( value_in ); 
}


ccontainer_value_t default_duplicater_value(const ccontainer_value_t* value_in) {
    ccontainer_value_t value_out;
    char *buf = (char*)malloc( value_in->len * sizeof(char) );
    /* testing allocation error, need (some) error_code */
    memcpy( buf, value_in->data, value_in->len);
    /*return make_value(buf, value_in->len);*/
    value_out.data = buf;
    value_out.len = value_in->len;
    return value_out;
}

/*
void default_deleter_value(ccontainer_value_t* value) {
    ccontainer_delete_value( value );
}*/
