
#include "ccontainer/value.h"

/** methods to create/delete value_t ***/
/* declare const char* for usage, but cast used to allow the assignment.
    make value_t.data const char* ? (usage make sense...TODO check) */
value_t make_value(char *data, size_t len) {
    value_t value;
    value.data = data; /* (char*) if pass const char* */
    value.len = len;
    return value;
}

value_t copy_value(const char *data, size_t len) {
    char *buf = (char*)malloc( len * sizeof(char) );
    memcpy( buf, data, len);
    return make_value(buf, len);
}

void free_value(value_t* value) {
    if( value && value->data ) {
        free(value->data);
        value->data = NULL;
        value->len = 0;
    }
}
