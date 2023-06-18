#pragma once

#include "ccontainer/cvector_generic.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @file
 *
 * @defgroup ccontainer_cvector_cstring_grp cvector specialization for C-strings
 * @ingroup ccontainer_grp
 * 
 * Specialization of  a cvector for C-string values.
*
*  Implemented with an internal generic cvector_gen_t.
*  Make interface convenient for C-strings */

/** @{ \ingroup ccontainer_cvector_cstring_grp */

/** Typedef of the type. */
typedef struct {
     /** cvector_generic container implementation */
    cvector_gen_t *cvector;
} cvector_cstring_t;

/** @name Constructor/ Destructor */
/** @{ */
/** Allocation memory for a cvector cstring allocated on heap.
 * A constructor (\ref cvector_cstring_init) must be called to have a functional container
 * \return a pointer to an instance of cvector_cstring_t
 */
SHARED_EXPORT cvector_cstring_t* cvector_cstring_new();

/** Constructor.
 * \post size == 0
 * \param[inout] cvector_str pointer to an instance of cvector_cstring_t */
SHARED_EXPORT void cvector_cstring_init(cvector_cstring_t* cvector_str);

/** Constructor with initial capacity.
 * \post size == 0 && capacity == init_capacity
 * \param[inout] cvector_str pointer to an instance of cvector_cstring_t */
SHARED_EXPORT void cvector_cstring_init_with_capacity(cvector_cstring_t* cvector_str, size_t init_capacity);

/** Clear all contents of the vector.
 * Call deleter_value_t on all elements of the vector, but the capacity stays untouched.
 * \param[in] cvector_str pointer to a cvector_gen_t instance */
SHARED_EXPORT void cvector_cstring_clear(cvector_cstring_t *cvector_str);

/** Destructor, delete before all the content of the list.
 * To call to free memory when list cvector_cstring_t is allocated on stack
 * \param[inout] cvector_str pointer to an instance of cvector_cstring_t */
SHARED_EXPORT void cvector_cstring_delete(cvector_cstring_t* cvector_str);

/** Deallocation of cvector allocated on heap.
 * This function calls cvector_cstring_delete if not empty 
 * \param[inout] cvector_str pointer to an instance of cvector_cstring_t */
SHARED_EXPORT void cvector_cstring_free(cvector_cstring_t* cvector_str);
/**@}*/ 

/**************************************************************/
/** @name Public methods */
/** @{ */
/** Return the number of element in the vector. 
 * \param[in] cvector_str pointer to an instance of cvector_cstring_t 
 * \return number of elements in the vector */
SHARED_EXPORT size_t cvector_cstring_size(const cvector_cstring_t *cvector_str);

/** Return the capacity the vector. 
 * \param[in] cvector_str pointer to an instance of cvector_cstring_t 
 * \return capacity of the vector */
SHARED_EXPORT size_t cvector_cstring_capacity(const cvector_cstring_t *cvector_str);

/** Set a new capacity for the vector.
 * Works only for extended
 * \pre new_capacity >= actual_capacity
 * \param[in] cvector_str pointer to an instance of cvector_cstring_t 
 * \param[in] new_capacity required */
SHARED_EXPORT ccontainer_err_t cvector_cstring_set_capacity(cvector_cstring_t *cvector_str, size_t new_capacity);

/** Insert a value_cstring_t at the end of the list.
   The null terminaison caracter is present in value_t::data  to allow the retrieval by reference 
   \param[inout] cvector_str pointer to an instance of cvector_cstring_t 
   \param[in] to_insert a string in C, must be null terminated */
SHARED_EXPORT ccontainer_err_t cvector_cstring_push_back(cvector_cstring_t *cvector_str, const char *to_insert);

/** Get a copy of nb-th element.
  Copy allocated on heap, must be freed by caller.
  \param[in] cvector_str pointer to a cvector_string_t instance
  \param[in] index of the collection to retrieve
  \param[out] err_code ccontainer error code
  \return  pointer to a copy string with terminason caracter, NULL on error */
SHARED_EXPORT char* cvector_cstring_get_copy_at( const cvector_cstring_t *cvector_str, size_t index, ccontainer_err_t *err_code);

/** Get a reference the C-string value */
SHARED_EXPORT const char* cvector_cstring_get_ref_at( const cvector_cstring_t *cvector_str, size_t index, ccontainer_err_t *err_code);

/* Pop cvector, not implemented
  \param[in] cvector_str pointer to a cvector_string_t instance
  \param[out] err_code ccontainer error code
  \return  pointer to a string in the cvector, NULL on error
SHARED_EXPORT char* cvector_cstring_pop_front( cvector_cstring_t *cvector_str, ccontainer_err_t *err_code); */

/** Return a array of string, deep copies of the internal elements.
  \param[in] cvector_str pointer to a cvector_string_t instance
  \param[out] array_out allocated array_out with length array_len. must be deleted by caller
        (see \ref carray_cstring_delete)
  \param[out] array_len size of array_out
  \return ccontainer error code */
SHARED_EXPORT ccontainer_err_t cvector_cstring_get_array( cvector_cstring_t *cvector_str, char ***array_out, size_t *array_len );

/** @} */ /** public API */

/** @} */ /** end ccontainer_cvector_cstring_grp */

#ifdef __cplusplus
}
#endif
