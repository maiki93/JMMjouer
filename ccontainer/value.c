
#include "ccontainer/value.h"
#include "string.h"

/** methods to create/delete value_t ***/
/* declare const char* for usage, but cast used to allow the assignment.
    make value_t.data const char* ? (usage make sense...TODO check) */
value_t make_value(char *data, size_t len) {
    value_t value;
    value.data = data; /* (char*) if pass const char* */
    value.len = len;
    return value;
}

void copy_value(value_t *value_dest, const value_t *value_src)
{
    memcpy(value_dest, value_src, sizeof(value_t));
}

value_t move_value(value_t *value_src)
{
    value_t value_out;
    memcpy(&value_out, value_src, sizeof(value_t));
    /* invalidate the source */
    value_src->data = NULL;
    value_src->len = 0;
    return value_out;
}

void free_value(value_t *value_in)
{
    if( value_in->data)
        free(value_in->data);
    value_in->data = NULL;
    value_in->len = 0;
}


value_t default_duplicater_value(const value_t* value_in) {
    value_t value_out;
    char *buf = (char*)malloc( value_in->len * sizeof(char) );
    /* testing allocation error, need (some) error_code */
    memcpy( buf, value_in->data, value_in->len);
    /*return make_value(buf, value_in->len);*/
    value_out.data = buf;
    value_out.len = value_in->len;
    return value_out;
}

void default_deleter_value(value_t* value) {
    if( value && value->data ) {
        free(value->data);
        value->data = NULL;
        value->len = 0;
    }
}
