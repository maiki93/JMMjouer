#pragma once

#include "ccontainer/error_code.h"
#include "stdlib.h" /* size_t */

#include "shared_EXPORTS.h"

/* Do not know where best place to define defgroup ccontainer, 
    a page of doxygen documentation maybe */
/** @defgroup ccontainer_grp generic C-style containers */

/** @file
 * Basic building structure to be inserted in all ccontainer's.
 * Storing an array of char with its length is generic enought to store any structure.
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
    Can store any serializable type or complex data structure with pointers on heap memory
    \ingroup ccontainer_value_grp */
typedef struct {
    /** pointer to serialized data */
    char *data;
    /** size of the data array */
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
typedef ccontainer_value_t(*duplicater_value_t) (const ccontainer_value_t * value_in, ccontainer_err *err_code);


/** Return a ccontainer_value_t from data input.
    ccontainer_value_t takes ownership of the data which should not be modified after this call.
    \param[in] data : pointer to the data to serialize
    \param[in] len : size of the data
    \return a ccontainer_value_t which keep ownership of the data    
*/
SHARED_EXPORT ccontainer_value_t ccontainer_make_value(char *data_in, size_t len, ccontainer_err *err_code);

/** Return a deep copy of value_src.
    Make a new heap allocation for data and copy the content of value_src.
    \pre value_src must be valid ( not null and value.data not null )
    \param[in] value_src : pointer to the source ccontainer_value_t */
SHARED_EXPORT ccontainer_value_t ccontainer_copy_value(const ccontainer_value_t *value_src, ccontainer_err *err_code);

/** Transfert data from one ccontainer_value_t to an other.
    Returned value_t takes ownership of the internal data stored in value_src,
    free should not be called on value_src after the call.
    \pre value_src must be valid ( not null and value.data not null )
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

#ifdef __cplusplus
}
#endif

/** @} */ /* end group ccontainer */
