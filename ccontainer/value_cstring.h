#pragma once

#include "ccontainer/value.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @file
 * Adapter function for storing a string into any ccontainer.
 * 
 * Choice has been done to include the '\0' terminated character into the ccontainer_value_t representation
 * to provide an access by reference (see clist_cstring::clist_gen_get_ref_at)
 * which is directly usable as a valid string by standard C methods (strlen,strcmp..). 
 */

/**
 * @defgroup ccontainer_value_cstring_grp adapter example for strings
 * @ingroup ccontainer_grp
*/

/** @{ \ingroup ccontainer_value_cstring_grp */

/** @name Constructor/ Destructor / Copy */
/** @{ */

/** Return a ccontainer_value_t from a string.
 * The returned structure can be inserted in any ccontainer of the library.
 * \param[in] text_to_insert well-formed string with a null terminal character
 * \return ccontainer_value_t containing a copy of the input */
ccontainer_value_t make_value_cstring( const char* text_to_insert, ccontainer_err_t *err_code);

/** Return an heap allocated cstring from a ccontainer_value_t.
   It creates a copy which must be freed by the caller.
   \param[inout]  value_in 
   \return a well formed string with the terminason character. */
char* extract_value_cstring( const ccontainer_value_t *value_in, ccontainer_err_t *err_code);

/** Specific destructor for ccontainer_value_t representation value_cstring data.
   Method to provide to ccontainer for safe deallocation of memory : delete/free/remove...
   \param[inout] pointer to ccontainer_value_t to deallocate */
void deleter_value_cstring( ccontainer_value_t *value );

ccontainer_value_t duplicater_value_cstring( const ccontainer_value_t *value_src, ccontainer_err_t *err_code);

/** @} */ /** public API */

/** @} */ /** end ccontainer_value_cstring_grp */

#ifdef __cplusplus
}
#endif