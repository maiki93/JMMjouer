#pragma once

#include "stdlib.h"
#include "shared_EXPORTS.h"

#include "ccontainer/value.h"

/** @file
 * Forward list implementation of a generic structure
 * Aimed to be used as an implementation by ccontainer's of the package.
 * 
 * ingroup ccontainer
 * page ccontainer create similar tree input as a markdown
 * 
 * ccontainer is a library of simple containers for generic type clist_generic::value_t and provide basic functionnalities.
 */

/**
 * @defgroup ccontainer_clist_gen_grp clist_generic generic forward list container
 * @ingroup ccontainer_grp
 */

/** @{ \ingroup ccontainer_clist_gen_grp */

#ifdef __cplusplus
extern "C" {
#endif

/** \name Enumeration for dealing with errors returned by clist_gen_t list. */
/**@{*/
/** no error */
#define CLIST_OK 0
/** data not found */
#define CLIST_NOTFOUND -1
/** out of range */
#define CLIST_OUTOFRANGE -2
/** the container is empty */
#define CLIST_EMPTY -3
/** allocation error */
#define CLIST_ALLOC_ERR -4
/**@}*/

/** Forward declaration of the list structure.
 * Keep details hidden in implementation
 * \ingroup ccontainer
 */
typedef struct clist_type clist_gen_t;

/** \name Constructor/Destructor */
/** @{ */
/** Memory Allocation. */
SHARED_EXPORT clist_gen_t* clist_gen_new();
/** Initialization. 
    \param[in] clist pointer to a clist_gen_t */
SHARED_EXPORT void clist_gen_init(clist_gen_t* clist);
/** Free memory of the list.
  Call clist_gen_clear() if not empty (need to store deleter ??)
  \todo check for deleter
  \param[in] clist pointer to a clist_gen_t
  \param[in] deleter function with signature \ref deleter_t to properly free the value in case of dynamic allocation */
SHARED_EXPORT void clist_gen_del(clist_gen_t *clist, deleter_t deleter);
/** Clear all contents of the list.
  The goal is to call deleter(&value)
  \param[in] clist pointer to a clist_gen_t
  \param[in] deleter function with signature \ref deleter_t to properly free the value in case of dynamic allocation */
SHARED_EXPORT void clist_gen_clear(clist_gen_t *clist, deleter_t deleter);
/** @} */

/** Return the number of value_t elements in the list.
    \note Actual, store a len data member, may change later,
    Total size of the message (sum of len) may be usefull for serialization (to think)
    \param[in] clist pointer to a clist_gen_t */
SHARED_EXPORT size_t clist_gen_size(const clist_gen_t *clist);

/** @brief Make a copy the input value at the first element of the list
 * \param[in] clist pointer to a clist_gen_t
 * \param[in] value value_t to be copied into the list
 */
SHARED_EXPORT int clist_gen_push_front(clist_gen_t *clist, value_t value);

/** @brief Make a copy of the input value at the end of the list 
 * \param[in] clist pointer to a clist_gen_t
*/
SHARED_EXPORT int clist_gen_push_back(clist_gen_t *clist, const value_t *value_in);

/** Interface and usage not very convenient ** if want to modify only, not safe as well 
   but no copy needed 
   \param[in] clist pointer to a clist_gen_t */
SHARED_EXPORT int clist_gen_get_value_ref( const clist_gen_t *clist, size_t elem_nb, value_t **value_out);
/** Get a copy of the content , better adapted for use ? 
     \param[in] clist pointer to a clist_gen_t */
SHARED_EXPORT int clist_gen_get_value_copy( const clist_gen_t *clist, size_t elem_nb, value_t *value_out);

/** @brief Retrieve a COPY of the first element where the buffer match the internal value_t::data.

   \todo equivalent with reference. improve interface with usage.
   Most important function  for reuse and specialization, very generic at this level 
   It checks the first n bytes given in buffer (later comp function ?)
   \param[in] clist pointer to a clist_gen_t
   \param[out] value_out pointer to an internally allocated value_t
   \return ERROR code */
SHARED_EXPORT int clist_gen_find( const clist_gen_t* clist, value_t* value_out, 
                               const char* buffer, size_t buffer_len);

#ifdef __cplusplus
}
#endif

/** @} */ /** end ingroup ccontainer */