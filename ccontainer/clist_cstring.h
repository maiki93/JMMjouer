#pragma once

#include "ccontainer/clist_generic.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @file
 * 
 * @ingroup ccontainer_spec_cstring_grp
 * 
 * Specialization of  a clist generic with C-string values.
 * 
 * Implemented with an internal generic clist_gen_t and use of value_cstring_t
 * Make interface convenient for C-strings */

/** @{ \ingroup ccontainer_spec_cstring_grp */

/** Use a clist_gen_t for container implementation with private value_cstring */
typedef struct {
     /** clist_generic container implementation */
    clist_gen_t *clist;
    /** the actual size is stored internally */
    size_t len;
} clist_cstring_t;

/** @name Constructor/ Destructor */
/** @{ */
/** Allocation memory for a clist cstring allocated on heap.
 * A constructor (clist_cstring_init() ) must be called to have a functional container
 * \return a pointer to an instance of clist_cstring_t
 */
SHARED_EXPORT clist_cstring_t* clist_cstring_new();

/** Constructor equivalent.
 * \post size == 0
 * \param[inout] clist_str pointer to an instance of clist_cstring_t */
SHARED_EXPORT void clist_cstring_init(clist_cstring_t* clist_str);

/** Destructor, delete before all the content of the list.
 * To call to free memory when list clist_cstring_t is allocated on stack
 * \param[inout] clist_str pointer to an instance of clist_cstring_t */
SHARED_EXPORT void clist_cstring_delete(clist_cstring_t* clist_str);

/** Deallocation of clist allocated on heap.
 * This function calls clist_cstring_delete if not empty 
 * \param[inout] clist_str pointer to an instance of clist_cstring_t */
SHARED_EXPORT void clist_cstring_free(clist_cstring_t* clist_str);
/**@}*/ 

/**************************************************************/
/** @name Public methods */
/** @{ */
/** Return the number of element in the list. 
 * \param[inout] clist_str pointer to an instance of clist_cstring_t */
SHARED_EXPORT size_t clist_cstring_size(clist_cstring_t *clist_str);

/** Insert a value_cstring_t at the end of the list.
   The null terminaison caracter is present in value_t::data  to allow the retrieval by reference 
   \param[inout] clist_str pointer to an instance of clist_cstring_t
   \param[in] to_insert a string in C, must be null terminated */
SHARED_EXPORT ccontainer_err_t clist_cstring_push_back(clist_cstring_t *clist_str, const char *to_insert);

/** Get a reference to one element of the list.
    May be very tricky to modify the original throught the reference (size problems) !!
    \param[in] clist_str pointer to an instance of clist_cstring_t
    \param[out] err_code ccontainer error code
    \return  pointer to a string in the clist, NULL on error */
SHARED_EXPORT char* clist_cstring_pop_front( clist_cstring_t *clist_str, ccontainer_err_t *err_code);

/** Return a array of string, deep copies of the internal elements.
  \param[in] clist_str pointer to a clist_string_t instance
  \param[out] array_out allocated array_out with length array_len. must be deleted by caller
        (see carray_cstring_delete)
  \param[out] array_len size of array_out
  \return ccontainer error code */
SHARED_EXPORT ccontainer_err_t clist_cstring_get_array( clist_cstring_t *clist_str, char ***array_out, size_t *array_len );

/** @} */ /** public API */

/** @} */ /** end ccontainer_clist_cstring_grp */

#ifdef __cplusplus
}
#endif
