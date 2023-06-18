#pragma once

#include "ccontainer/value.h"
#include "ccontainer/error_code.h"

/** @file
 * 
 * @defgroup ccontainer_cvector_gen_grp dynamic array with generic elements
 * @ingroup ccontainer_grp
 * 
 * Generic implementation of a C-vector container with dynamic size (re-)allocation.
 * 
 * Try to Follow functionalities and interfaces similar to a std::vector with \ref ccontainer_value_t elements */

/** @{ \ingroup ccontainer_cvector_gen_grp */

/** Internal structure : an array of generic ccontainer_value_t. */
typedef struct {
    /** array of value_t's */
    ccontainer_value_t *array;
    /** size total allocated */
    size_t capacity;
    /** nb of valid elements in the vector */
    size_t len;
    /** pointer to function for properly deleting the value_t*/
    deleter_value_t ptrf_deleter;
    /** pointer to function for properly copying the value_t */
    duplicater_value_t ptrf_duplicater;
} cvector_gen_t;


#ifdef __cplusplus
extern "C" {
#endif

/** \name Constructor/Destructor */
/** @{ */
/** Memory Allocation of a cvector on heap. 
 * Caller must check that the return pointer is not NULL and call a constructor
 * \return pointer to a heap allocated cvecotr_gen_t */
SHARED_EXPORT cvector_gen_t* cvector_gen_new();

/** Initilization of an empty container.
 * A default deleter_value and duplicater_value is set :
 * - \ref ccontainer_delete_value
 * - ccontainer_copy_value
 * \post cvector fully functional, size() == capacity() == 0
 * \param[in] cvect pointer to a cvector_gen_t instance */
SHARED_EXPORT void cvector_gen_init( cvector_gen_t* cvect );

/** Constructor with an inttial capacity argument. 
 *  All elements are default initialized (0 or null)
 * \post capacity() return init_size, size() is still null
 * \param[in] cvect pointer to a cvector_gen_t instance
 * \param[in] init_capacity initial size of the container
 * \return \ref ccontainer_err_t if allocation error */
SHARED_EXPORT ccontainer_err_t cvector_gen_init_with_capacity( cvector_gen_t* cvect, size_t init_capacity);

/** Copy constructor, deep copy of the ccontainer_value_t elements.
 * Assign same duplicater and deleter than the original
 * \param[in] cvect_src pointer to a cvector_gen_t instance 
 * \param[out] err_code error code 
 * \return a new vector*/
SHARED_EXPORT cvector_gen_t cvector_gen_copy(const cvector_gen_t* cvect_src, ccontainer_err_t *err_code);

/** Delete all contents and free internal allocated memory of cvect.
 * Must be called explicitly when cvect is allocated on stack,
 * implicitly called by  \ref cvector_gen_free when allocated on heap.
 * \param[in] cvect pointer to a cvector_gen_t instance */
SHARED_EXPORT void cvector_gen_delete(cvector_gen_t *cvect);

/** Free memory of the vector.
 * To be called only if cvect is allocated  on heap ( created by cvector_gen_new() )
   It will call \ref cvector_gen_delete()
  \param[in] cvect pointer to a cvector_gen_t instance */
SHARED_EXPORT void cvector_gen_free(cvector_gen_t *cvect);
/** @} */ /* end constructor/destructor */

/** \name Specific destructor and copy constructor needed for specialization */
/** @{ */
/** Apply an user defined destructor to ccontainer_value_t.
 * Will replace the default implementation \ref ccontainer_delete_value()
 * Mandatory if the user defined structure contains pointer on heap allocated memory
 * \param[in] cvect pointer to a cvector_gen_t instance
 * \param[in] fct_deleter pointer function with signature void(value_t *value) */
SHARED_EXPORT void cvector_gen_set_deleter( cvector_gen_t *cvect, deleter_value_t fct_deleter );

/**  Apply an user defined copy constructor to ccontainer_value_t.
 * Will replace the default implementation \ref ccontainer_copy_value()
 * \param[in] cvect pointer to a cvector_gen_t instance
 * \param[in] fct_duplicater pointer function with signature of duplicater_value_t */
SHARED_EXPORT void cvector_gen_set_duplicater( cvector_gen_t *cvect, duplicater_value_t fct_duplicater );
/** @} */

/** \name Standard behavior expected by a vector */
/** @{ */
/** Clear all valid contents of the vector.
 * Call duplicater_value_t on elements [0,size()] of the vector, the capacity stays unchanged.
  \param[in] cvect pointer to a cvector_gen_t instance */
SHARED_EXPORT void cvector_gen_clear(cvector_gen_t *cvect);

/** Return the size of the vector */
SHARED_EXPORT size_t cvector_gen_size(const cvector_gen_t *cvect);

/** Return the capacity of the vector */
SHARED_EXPORT size_t cvector_gen_capacity(const cvector_gen_t *cvect);

/** Set a new capacity */
SHARED_EXPORT ccontainer_err_t cvector_gen_set_capacity(cvector_gen_t *cvect, size_t new_capacity);

/** Transfer the input ccontainer_value_t at the end of the vector.
 * Vector grows dynamically if necessary by increasing its capacity.
 * value_in is moved into the cvector, invalid after the call.
 * \param[in] cvect pointer to a cvector_gen_t instance
 * \param[in] value_in input ccontainer_value_t to insert, reset after the call */
SHARED_EXPORT ccontainer_err_t cvector_gen_push_back(cvector_gen_t *cvect, ccontainer_value_t *value_in);

/** Retrive ccontainer_value_t at position given by indice.
 * Should return if error or keep return value and err_code as a output parameter
 * More flexible if pass value_out as pointer, alllow allocation on heap or stack ? only intermediate...(except string)
 * \param[in] cvect pointer to a cvector_gen_t instance
 * \param[in] index position of the elemnt to retrieve [0,len-1]
 * \param[in] err_code error_code status returned by function
 * \return pointer associated to the ccontainer_value_t element in the vector */
SHARED_EXPORT ccontainer_value_t* cvector_gen_get_at(const cvector_gen_t *cvect, size_t index, ccontainer_err_t* err_code);
/** @} */

/** \name Algorithms */
/** @{ */
/** Swap elements between 2 indices.
 * Efficient because only internal pointers must be exchanged.
 * \param[in] cvect pointer to a cvector_gen_t instance
 * \param[in] index1, index2 elments dex to swap */
SHARED_EXPORT void cvector_gen_swap(cvector_gen_t *cvect, size_t index1, size_t index2);

/** Sorting algorithm using buuble sort algorithm.
 * Comparater will be used to compare the value_t, 
 * if comparater(value1, value2) -> true if value1 > value2
 *  - the vector will be sorted in ascending order. (see test_cvector_generic.c)
 * Efficiency generally bad O(n^2), but maybe not bad to sort X biggest/smallest value
 * \param[inout] cvect pointer to a cvector_gen_t instance, sorted after the call
 * \param[in] fct_comparater a comparison function bool(const value_t*, const value_t*) */
SHARED_EXPORT void cvector_bubble_sort(cvector_gen_t *cvect, comparater_value_t fct_comparater);
/** @} */

#ifdef __cplusplus
}
#endif
/** @} */ /* end grp */

