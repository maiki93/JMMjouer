#pragma once

#include "shared_EXPORTS.h"
#include "stdlib.h"

/* do not know where best place to define defgroup ccontainer */
/** @defgroup ccontainer_grp generic C-style containers */

/** @file 
 * Basic building structure to be inserted in all ccontainer's.
 * Generic enought to store any serializable data
 *
*/

/**
 * @defgroup ccontainer_value_grp value_t generic structure stored in ccontainer's
 * @ingroup ccontainer_grp
 */

/** @{ \ingroup ccontainer_value_grp */

#ifdef __cplusplus
extern "C" {
#endif

/** Generic structure which will be inserted in the ccontainer's. 
    Can store any serializable type or data structure
    \ingroup ccontainer_value_grp */
typedef struct {
    /** pointer to a serializable data */
    char *data;
    /** Size of data pointed by data */
    size_t len; 
} value_t;

/** Used in destructor, must be provide by user of the class  */
typedef void (*deleter_t) (value_t* value);

/** Return a value_t from data input.
    value_t takes ownership of the data which should not be modified after this call.
    \param[in] data : pointer to the data to serialize
    \param[in] len : size of the data
    \return a value_t which keep ownership of the data    
*/
SHARED_EXPORT value_t make_value(char *data, size_t len);

/** Create a value_t copy of the input data.
   \param[in] data : pointer to the data to serialize
   \param[in] len : size of the data  
   \return a value_t independent of the inputs 
*/
SHARED_EXPORT value_t copy_value(const char *data, size_t len);

/** Deallocate memory of the internal data.
 * \post value.len == 0 && value.data == NULL
*/
SHARED_EXPORT void free_value(value_t* value);

#ifdef __cplusplus
}
#endif

/** @} */ /* end group ccontainer */
