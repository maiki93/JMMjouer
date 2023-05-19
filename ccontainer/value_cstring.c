#include "ccontainer/value_cstring.h"
#include "string.h"
#include "assert.h"

ccontainer_value_t make_value_cstring( const char* text_to_insert, ccontainer_err_t *err_code)
{
    ccontainer_value_t value_out;
    char *tmp_buffer;
    size_t text_len;

    assert(text_to_insert);
    ccontainer_reset_value(&value_out);

    /* include the terminaison caracter */
    text_len = strlen(text_to_insert) + 1;
    tmp_buffer = (char*)malloc(text_len);
    if( !tmp_buffer ) {
        *err_code = CCONTAINER_ALLOCERR;
        return value_out;
    }
    /* strcpy will copy the '\0' if possible: ok len+1 than source */
    strcpy( tmp_buffer, text_to_insert);

    *err_code = CCONTAINER_OK;
    value_out.data = tmp_buffer;
    value_out.len = text_len;
    return value_out;
}

char* extract_value_cstring( const ccontainer_value_t *value_in, ccontainer_err_t *err_code)
{
    char *string_out;
    /* ccontainer_value_t::data and len include the terminaison caracter */
    string_out = (char*)malloc( value_in->len );
    if( !string_out ) {
        *err_code = CCONTAINER_ALLOCERR;
        return NULL;
    }
    strcpy( string_out, value_in->data );
    return string_out;
}

void deleter_value_cstring( ccontainer_value_t *value)
{
    if( value ) {
        free(value->data);
        value->data = NULL;
    }
    value->len = 0;
}

ccontainer_value_t duplicater_value_cstring( const ccontainer_value_t *value_src, ccontainer_err_t *err_code)
{
    ccontainer_value_t value_out;
    char* tmp_buffer;
    /* default initialization to NULL */
    ccontainer_reset_value(&value_out);

    if( (tmp_buffer = extract_value_cstring( value_src, err_code)) == NULL) {
        return value_out;
    }
    /* fine so swap buffer */
    value_out.data = tmp_buffer;
    value_out.len = value_src->len;
    return value_out;
}