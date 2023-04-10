#pragma once

#include "clist_generic.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
  @file
  Specialization of  a clist_generic with C-string values.
  
  Implemented with an internal forward list : clist_generic
  Make interface convenient for C-strings
  Common error messages are the same than defined in clist_generic.h
*/

/**
 * @defgroup ccontainer_clist_cstring_grp clist specialization for C-strings
 * @ingroup ccontainer_grp
 */

/** @{ \ingroup ccontainer_clist_cstring_grp */

/** Definition of the type. 
 * Use a clist_gen_t for container implementation with private value_cstring */
typedef struct {
    /* like cmap, clist_gen_t clist; no access generic type in .h 
     may contain the size at this level, rather than clist_generic (cmap also..) */
     /** clist_generic container implementation */
    clist_gen_t *clist;
} clist_cstring_t;

/** A unique simple interface to help deletion of 
        array of structure return by get_array methods.
    Otherwise typedef / struct overall complex and increase dependencies */
SHARED_EXPORT void carray_cstring_delete( char **carray, size_t len);

/** @name Constructor/ Destructor */
/**@{*/
/** Allocation of empty list */
SHARED_EXPORT clist_cstring_t* clist_cstring_new();
/** Constructor equivalent */
SHARED_EXPORT void clist_cstring_init(clist_cstring_t* list);
/* a default deleter would be enough in this case */
/** @brief Delete all the content of the list.
 * @post size is null, list is still functional
*/
SHARED_EXPORT void clist_cstring_clear(clist_cstring_t* list);
/** @brief Deallocation.
 * This function calls clist_cstring_clear if not empty
 */
SHARED_EXPORT void clist_cstring_delete(clist_cstring_t* list);
/**@}*/ 

/**************************************************************/
/** @name Public methods */
/** @{ */
/** Return the number of element nf the list. */
SHARED_EXPORT size_t clist_cstring_size(clist_cstring_t *list);
/** Insert a (copy of the) cstring at the end of the list.
   The null terminaison caracter is present in value_t::data  to allow the retrieval by reference */
SHARED_EXPORT int clist_cstring_push_back(clist_cstring_t *list, const char *to_insert);

/** Get a copy of elem_nb-th element.
  Copy allocated on heap, must be freed by caller.
  \param[in] list pointer to a clist_string_t instance
  \param[in] elem_nb of the collection to retrieve
  \param[inout] string_out pointer to the internally allocated destination string, will contain the terminaison caracter on exit
  \return CLIST_OK or error from clist_genric.h */
SHARED_EXPORT int clist_cstring_get_copy( clist_cstring_t *list, size_t elem_nb, char **string_out);

/* boring to use, for printf for instance, it is a ref so return by pointer is enought */
/** @brief Get a reference to one element of the list.
    May be very tricky to modify the original throught the reference (size problems) !!
    \param[in] elem_nb of the collection to retrive
    \param[inout] string_out of the destination string, will contain the terminaison caracter on exit
    \return CLIST_OK or error from clist_genric.h */
SHARED_EXPORT int clist_cstring_get_ref( clist_cstring_t *list, size_t elem_nb, char **string_out);

/** @brief Return a array of string, deep copies of the internal elements.
    \param[in] list pointer to a clist_string_t instance
    \param[out] array_out allocated array_out with length array_len. must be deleted by caller
        (see carray_cstring_delete)
    \param[out] array_len size of array_out
    \return CLIST_OK or error from clist_genric.h */
SHARED_EXPORT int clist_cstring_get_array( clist_cstring_t *list, char ***array_out, size_t *array_len );
/* An array of reference may be faster ? */

/** @} */ /** public API */

/** @} */ /** end ccontainer_clist_cstring_grp */

#ifdef __cplusplus
}
#endif
