#include <string.h>
#include <stdbool.h>

/** @file
 * 
 * @defgroup ccontainer_adapt_own_struture adapting its own structure
 * @ingroup ccontainer_grp
 * 
 * Example for creating a compatible value_t from an arbitrary structure
 * 
 * Declaration of a structure with:
 * - a pointer on a C-string allocated on heap
 * - one float
 * - one bool
 * 
 * Adapter functions to a ccontainer and tests are in a test file : \ref test_cvector_struct_complex.c
*/

/** A complex structure with dynamic allocation. 
 * @ingroup ccontainer_adapt_own_struture */
typedef struct {
    /** a pointer to heap memory */
    char * pname;
    /** one simple float */
    float one_float;
    /** one simple boolean */
    bool is_valid;
} struct_complex_t;

/** @{ \ingroup ccontainer_adapt_own_struture */

/** Memory allocation.
 * To be used for an allocation on heap */
struct_complex_t* struct_complex_new()
{
    return malloc( sizeof(struct_complex_t) );
}

/** Constructor. */
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

/** Delete attributes with memory allocated on heap */
void struct_complex_delete(struct_complex_t* this)
{
    if( this->pname )
        free(this->pname);
    this->pname = NULL;
    this->one_float = 0.f;
    this->is_valid = false;
}

/** Delete instance.
 * To be called when instance has been allocated on heap (after a new) */
void struct_complex_free(struct_complex_t* this)
{
    if( this )
        struct_complex_delete(this);
    free(this);
}

/** @} */ /* end group ccontainer_adapt_own_struture */