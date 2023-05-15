#pragma once

#include "ccontainer/value.h"
/* may be more general to group error_code in a "clean" way 
    header + implementation, extensible error_code
*/
#include "ccontainer/error_code.h"

/** \file
 * Generic implementation of a C-vector container with dynamic size (re-)allocation.
 * 
 * Try to Follow functionalities and interfaces similar to a std::vector with \ref ccontainer_value_t elements.
 * 
 * ccontainer_value_t may contain pointer to dynamically allocated memory, 
 * 
 * Declare complete structure in header first, 
 *  it would allow better performance ? construction on stack(more carray in fact) ? for tests.
 *  => really true ?
*/

/**
 * \defgroup ccontainer_cvector_gen_grp dynamic array with generic elements
 * \ingroup ccontainer_grp
*/

/** @{ \ingroup ccontainer_cvector_gen_grp */

/** Internal structure : an array of generic ccontainer_value_t. */
typedef struct {
    ccontainer_value_t *array;
    size_t capacity;
    size_t len;
    /* to suppress in generic ? or not too bad here */
    deleter_value_t ptrf_deleter;  /* only for delete / free / remove */
    duplicater_value_t ptrf_duplicater; /* only for copy_constructor */
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
 * \return \ref ccontainer_err if allocation error
*/
SHARED_EXPORT ccontainer_err cvector_gen_init_with_capacity( cvector_gen_t* cvect, size_t init_capacity);

/** Copy constructor, deep copy of the ccontainer_value_t elements.
 * On specialization, may need to set duplicater for a correct copy of the elements
 * \param[in] cvect_dest pointer to a cvector_gen_t instance
 * \param[in] cvect_src pointer to a cvector_gen_t instance */
/*SHARED_EXPORT ccontainer_err cvector_gen_copy(cvector_gen_t* cvect_dest, cvector_gen_t* cvect_src);*/
SHARED_EXPORT cvector_gen_t cvector_gen_copy(const cvector_gen_t* cvect_src, ccontainer_err *err_code);

/** Clear all contents of the vector.
 * Call deleter_value_t on all elements of the vector, but the capacity stays untouched.
  \param[in] cvect pointer to a cvector_gen_t instance */
SHARED_EXPORT void cvector_gen_clear(cvector_gen_t *cvect);

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

/** \@{ 
 * \name Specific destructor and copy constructor for a specific implementation / specialization
*/
/** Apply an user defined destructor to ccontainer_value_t.
 * Will replace the default implementation \ref ccontainer_delete_value()
 * Mandatory if the user defined structure contains pointer on heap allocated memory
 * \param[in] cvect pointer to a cvector_gen_t instance
 * \param[in] function with signature void(value_t *value)
 */
SHARED_EXPORT void cvector_gen_set_deleter( cvector_gen_t *cvect, deleter_value_t fct_delete );
/**  Apply an user defined copy constructor to ccontainer_value_t.
 * Will replace the default implementation \ref ccontainer_copy_value()
 * \param[in] cvect pointer to a cvector_gen_t instance
 * \param[in] function with signature void(value_t *value)
 */
SHARED_EXPORT void cvector_gen_set_duplicater( cvector_gen_t *cvect, duplicater_value_t fct_duplicater );
/** \@} */

/** Clear all valid contents of the vector.
 * Call duplicater_value_t on elements [0,size()] of the vector, but the capacity stays untouched for later re-use.
  \param[in] cvect pointer to a cvector_gen_t instance */
SHARED_EXPORT void cvector_gen_clear(cvector_gen_t *cvect);


SHARED_EXPORT size_t cvector_gen_size(const cvector_gen_t *cvect);
SHARED_EXPORT size_t cvector_gen_capacity(const cvector_gen_t *cvect);
SHARED_EXPORT ccontainer_err cvector_gen_set_capacity(cvector_gen_t *cvect, size_t new_size);

/** Transfer the input ccontainer_value_t at the end of the vector.
 * Vector grows dynamically if necessary by increasing its capacity.
 * value_in is moved into the cvector, invalid after the call.
 * \param[in] cvect pointer to a cvector_gen_t instance
 * \param[in] value_in input ccontainer_value_t to insert, reset after the call
*/
SHARED_EXPORT ccontainer_err cvector_gen_push_back(cvector_gen_t *cvect, ccontainer_value_t *value_in);

/** Retrive ccontainer_value_t at position given by indice.
 * Should return if error or keep return value and erro_code as a output parameter
 * More flexible if pass value_out as pointer, alllow allocation on heap or stack ? only intermediate...(except string)
 * \param[in] cvect pointer to a cvector_gen_t instance
 * \param[in] index position of the elemnt to retrieve [0,len-1]
 * \param[in] err_code error_code status returned by function
 * \return pointer associated to the ccontainer_value_t element in the vector
*/
SHARED_EXPORT ccontainer_value_t* cvector_gen_get_at(const cvector_gen_t *cvect, size_t index, ccontainer_err* err_code);

/** Swap elements between 2 indices.
 * Efficient because only internal pointers must be exchanged.
 * \param[in] cvect pointer to a cvector_gen_t instance
 * \param[in] index1, index2 elments dex to swap */
SHARED_EXPORT void cvector_gen_swap(cvector_gen_t *cvect, size_t index1, size_t index2);


#ifdef __cplusplus
}
#endif
/** @} */ /* end grp */

