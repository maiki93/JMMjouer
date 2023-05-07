#pragma once

#include "shared_EXPORTS.h"
#include "stdlib.h"

/* Do not know where best place to define defgroup ccontainer, 
    a page of doxygen documentation maybe */
/** @defgroup ccontainer_grp generic C-style containers */

/** @file 
 * Basic building structure to be inserted in all ccontainer's.
 * Storing an array of char with its length is generic enought to store any structure.
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
    /** pointer to a serializable structure */
    char *data;
    /** size of data pointed by data */
    size_t len; 
} value_t;

/** Used in the destruction of value_t who required memory_allocation, 
 *  must be provide by user of the class  */
typedef void (*deleter_value_t) (value_t* value);

/** Used as a copy constructor of value_t.
 * value_t defines \ref default_deleter_value_t for shallow copy,
 * but user may provide a deep_copy constructor
 * \param[in] value as source
 * \return a copy of the source
 */
typedef value_t(*duplicater_value_t) (const value_t * value);

/** Return a value_t from data input.
    value_t takes ownership of the data which should not be modified after this call.
    \param[in] data : pointer to the data to serialize
    \param[in] len : size of the data
    \return a value_t which keep ownership of the data    
*/
SHARED_EXPORT value_t make_value(char *data, size_t len);

/** Share value_t from value_src with value_dest.
    value_dest points to the same interal data of value_src, free be called with care
    No check performed on initial value_dest, its data may cause memory leak.
    \param[in] value_dest : pointer to the destination value_t
    \param[in] value_src : pointer to the source value_t */
SHARED_EXPORT void copy_value(value_t *value_dest, const value_t *value_src);

/** Transfert data from one value_t to an other.
    value_dest takes ownership of the internal data stored in value_src, 
    Free should not be called on value_src after the call !
    No check performed on initial value_dest, its data may cause memory leak.
    \post value_src.data = NULL && len == 0
    \param[in] value_dest : pointer to the destination value_t
    return value_t : value with stolen data of the source */
SHARED_EXPORT value_t move_value(value_t *value_src);

/** Free memory on a value_t.
 * \deprecated default_deleter_value is equivalent
 * \post value_in.data = NULL && len = 0
 * \param[in] value_in : pointer to the destination value_t */
SHARED_EXPORT void free_value(value_t *value_in);

/** \todo Use only one function : default_free_value / default_copy_value
 *      default_deleter_value
 */

/** Create a value_t copy of the input data.
 * Default implementation makes a bytes-to-bytes copy of the array data of length len.
 * If value_in contains pointers to heap allocated memory, the copy will point to the same memory.
   \param[in] value_in source
   \return a new value_t
*/
SHARED_EXPORT value_t default_duplicater_value(const value_t* value_in);

/* why not copy_value(value_t *) ? more explicit ! (use with cast of arguments maybe ?)*/

/** Deallocate memory of the internal data array.
 * Follow signature of deleter_t function pointer and used as default implementation.
 * \note value->data is deallocated and cannot be re-used (for optimization cvector_gen size/capacity)
 *      may specify a specific deleter to overidde this behaviour
 * \post value.len == 0 && value.data == NULL
*/
SHARED_EXPORT void default_deleter_value(value_t* value);

#ifdef __cplusplus
}
#endif

/** @} */ /* end group ccontainer */
