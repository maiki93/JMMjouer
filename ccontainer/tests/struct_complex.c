#include <string.h>
#include <stdbool.h>

/** \file
 * Example for creating a compatible value_t from an arbitrary structure
 * See test file \ref test_cvector_struct_complex.c
*/

/** A complex structure with dynamic allocation. */
typedef struct {
    char * pname;
    float one_float;
    /*int one_float;*/
    bool is_valid;
} struct_complex_t;

struct_complex_t* struct_complex_new()
{
    return malloc( sizeof(struct_complex_t) );
}

int struct_complex_init(struct_complex_t* this, const char *name, float value, bool valid )
{
    char *tmp_string;
    /* allocate string */
    tmp_string = malloc( strlen(name) + 1 );
    if( !tmp_string )
        return 1;
    strcpy(tmp_string, name);
    /* assign to this */
    this->pname = tmp_string;
    this->one_float = value;
    this->is_valid = valid;
    return 0;
}

void struct_complex_delete(struct_complex_t* this)
{
    if( this->pname )
        free(this->pname);
    this->pname = NULL;
    this->one_float = 0.f;
    this->is_valid = false;
}

void struct_complex_free(struct_complex_t* this)
{
    if( this )
        struct_complex_delete(this);
    free(this);
}