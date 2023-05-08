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
 * @defgroup ccontainer_value_grp ccontainer_value_t generic structure stored in ccontainer's
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
} ccontainer_value_t;

/** Function signature for the destruction of ccontainer_value_t whose required memory allocation. 
 *  It may be provided by the user of the library to overwritte the default deleter ccontainer_value_delete 
 * \param[in] value_in as source */
typedef void (*deleter_value_t) (ccontainer_value_t* value);

/** Function signature for a deep copy of ccontainer_value_t whose required memory allocation.
 * The \ref default_deleter_value_t makes a shallow copy, but user may provide a deep copy constructor
 * \param[in] value_in as source
 * \return a copy of the source
 */
typedef ccontainer_value_t(*duplicater_value_t) (const ccontainer_value_t * value_in);

/** Return a ccontainer_value_t from data input.
    ccontainer_value_t takes ownership of the data which should not be modified after this call.
    \param[in] data : pointer to the data to serialize
    \param[in] len : size of the data
    \return a ccontainer_value_t which keep ownership of the data    
*/
SHARED_EXPORT ccontainer_value_t ccontainer_make_value(char *data_in, size_t len);

/** Share ccontainer_value_t from value_src with value_dest.
    value_dest points to the same interal data of value_src, free must be called with care
    No check performed on initial value_dest, its data may cause memory leak.
    \param[in] value_dest : pointer to the destination ccontainer_value_t
    \param[in] value_src : pointer to the source ccontainer_value_t */
SHARED_EXPORT void ccontainer_copy_value(ccontainer_value_t *value_dest, const ccontainer_value_t *value_src);

/** Transfert data from one ccontainer_value_t to an other.
    value_dest takes ownership of the internal data stored in value_src, 
    Free should not be called on value_src after the call
    No check performed on initial value_dest, its data may cause memory leak.
    \post value_src.data = NULL && len == 0
    \param[in] value_dest : pointer to the destination ccontainer_value_t
    \return ccontainer_value_t : value with stolen data of the source */
SHARED_EXPORT ccontainer_value_t ccontainer_move_value(ccontainer_value_t *value_src);

/** Reset ccontainer_value_t.
  No check are done if previous memory is allocated.
  It is mainly a shortcut and more applicable to :
  ccontainer_value_t value = {.data = NULL,.len = 0}
  Use ccontainer_delete_value to free dynamically allocated memory
  \post value_in_out.data = NULL && len == 0
  \param[in] value_in_out : pointer to the ccontainer_value_t to reset
*/
SHARED_EXPORT void ccontainer_reset_value(ccontainer_value_t *value_in_out);

/** Reset by deleting allocated memory by value_in.data.
 * \post value_in.data = NULL && len = 0
 * \param[in] value_in_out : pointer to the ccontainer_value_t
*/
SHARED_EXPORT void ccontainer_delete_value(ccontainer_value_t *value_in_out);

/** Free memory on a ccontainer_value_t.
 * delete_value : destructor ??
 * \deprecated default_deleter_value is equivalent
 * \post value is not usuable, can only call value_in = NULL if passed from a pointer
 * \param[in] value_in : pointer to the ccontainer_value_t de deallocate */
SHARED_EXPORT void ccontainer_free_value(ccontainer_value_t *value);

/** \todo Use only one function : default_free_value / default_copy_value
 *      default_deleter_value
 */

/** Create a ccontainer_value_t copy of the input data.
 * Default implementation makes a bytes-to-bytes copy of the array data of length len.
 * If value_in contains pointers to heap allocated memory, the copy will point to the same memory.
   \param[in] value_in source
   \return a new ccontainer_value_t
*/
SHARED_EXPORT ccontainer_value_t default_duplicater_value(const ccontainer_value_t* value_in);

/* why not copy_value(value_t *) ? more explicit ! (use with cast of arguments maybe ?)*/

/** Deallocate memory of the internal data array.
 * Follow signature of deleter_t function pointer and used as default implementation.
 * \note value->data is deallocated and cannot be re-used (for optimization cvector_gen size/capacity)
 *      may specify a specific deleter to overidde this behaviour
 * \post value.len == 0 && value.data == NULL
*/
SHARED_EXPORT void default_deleter_value(ccontainer_value_t* value);

#ifdef __cplusplus
}
#endif

/** @} */ /* end group ccontainer */
