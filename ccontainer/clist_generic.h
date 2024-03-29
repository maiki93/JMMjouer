#pragma once

#include "shared_EXPORTS.h"

#include "ccontainer/value.h" /* include error_code.h */
#include "ccontainer/cvector_generic.h" /* or forward declaration */

/** @file
 * 
 * @defgroup ccontainer_clist_gen_grp generic double-linked list
 * @ingroup ccontainer_grp
 * 
 * Double linked-list implementation of a generic structure.
 * 
 * ccontainer is a library of simple containers for generic type clist_generic::ccontainer_value_t and provide basic functionnalities.
 */


/** @{ \ingroup ccontainer_clist_gen_grp */

#ifdef __cplusplus
extern "C" {
#endif

/** Forward declaration of the list and node structures.
 * Keep details hidden in implementation
 * ingroup ccontainer */
typedef struct clist_type clist_gen_t;
/** Forward declaration of the node structure. */
typedef struct clist_node_type clist_node_t;

/** \name Constructor/Destructor */
/** @{ */
/** Memory Allocation. */
SHARED_EXPORT clist_gen_t* clist_gen_new();

/** Initialization. 
    \param[in] clist pointer to a clist_gen_t */
SHARED_EXPORT void clist_gen_init(clist_gen_t* clist);

/** Copy constructor. 
    \param[in] clist_src pointer to a clist_gen_t
    \param[in] ptrf_duplicater function pointer of copy
    \param[out] err_code : CCONTAINER_OK, CCONTAINER_EMPTY, CCONTAINER_ALLOCERR 
    \return pointer to a new clist_gen_t */
SHARED_EXPORT clist_gen_t* clist_gen_copy(const clist_gen_t* clist_src, duplicater_value_t ptrf_duplicater, 
                                          ccontainer_err_t *err_code );

/** Free memory of the list.
  Call clist_gen_clear() if not empty (need to store deleter ??)
  \todo check for deleter
  \param[in] clist pointer to a clist_gen_t
  \param[in] ptrf_deleter function with signature \ref deleter_value_t to properly free the value in case of dynamic allocation */
SHARED_EXPORT void clist_gen_free(clist_gen_t *clist, deleter_value_t ptrf_deleter);

/** @} */ /* end constructor/destructor */

/** Return the number of ccontainer_value_t elements in the list.
    \note Actual, store a len data member, may change later,
    Total size of the message (sum of len) may be usefull for serialization (to think)
    \param[in] clist pointer to a clist_gen_t */
SHARED_EXPORT size_t clist_gen_size(const clist_gen_t *clist);

/** Transfer (move) the input value at the first element of the list
 * to check value is null after call
 * \param[in] clist pointer to a clist_gen_t
 * \param[in] value ccontainer_value_t to be copied into the list */
SHARED_EXPORT ccontainer_err_t clist_gen_push_front(clist_gen_t *clist, ccontainer_value_t *value);

/** Make a move of the input value at the end of the list
 * \param[in] clist pointer to a clist_gen_t */
SHARED_EXPORT ccontainer_err_t clist_gen_push_back(clist_gen_t *clist, ccontainer_value_t *value_in);

/** Retrieve a reference of the first value_t matching the criteria given by a fct_equalizer implementation.
   Most important function  for reuse and specialization, very generic at this level.
   All elements of the clist will be send as parameter value1 in fct_equalizer
   \todo extend with find_all return a clist
   \param[in] clist pointer to a clist_gen_t instance
   \param[in] fct_equalizer of type \ref equalizer_value_t : bool(value_t *value1, value_t *value2)
   \param[in] value_to_match a value_t reference used as parameter value2 in fct_equalizer
   \param[out] err_code  ccontainer error_code
   \return reference to the first value_t matching the equality */
SHARED_EXPORT ccontainer_value_t* clist_gen_find( const clist_gen_t *clist, 
                        equalizer_value_t fct_equalizer, const ccontainer_value_t* value_to_match,
                        ccontainer_err_t *err_code);

/** Retrieve the first element, removing it from the list
 * \param[in] clist pointer to a clist_gen_t instance
 * \param[out] err_code  ccontainer error_code
 * \return the first value_t */
SHARED_EXPORT ccontainer_value_t clist_gen_pop_front(clist_gen_t* clist, ccontainer_err_t *err_code);

/** Find all values matching.
 * Retrun a clist of value_t */
SHARED_EXPORT clist_gen_t* clist_gen_find_all(const clist_gen_t *clist,
                        equalizer_value_t fct_equalizer, const ccontainer_value_t* value_to_match,
                        duplicater_value_t fct_duplicater, ccontainer_err_t *err_code);

/* ************ 
  Nodes involved in API
************* */

/** \name API involving internal nodes */
/** @{ */
/** Return the first node.
 * \param[in] clist pointer to a clist_gen_t instance
 * \return last node of the clist, NULL if empty */
SHARED_EXPORT clist_node_t* clist_gen_get_first_node( const clist_gen_t* clist);

/** Return the last node.
 * Complexity O(n)
 * \pre clist input must be correctly initilaized
 * \param[in] clist pointer to a clist_gen_t instance
 * \return first node of the clist, NULL if empty */
SHARED_EXPORT clist_node_t* clist_gen_get_last_node( const clist_gen_t *clist);

/** Return the next node of the provided node.
 * \param[in] current_node pointer to the current node
 * \return pointer to a node, return NULL if end of list is reached */
SHARED_EXPORT clist_node_t* clist_gen_get_next_node( const clist_node_t* current_node);

/** Return a reference to the value contained in current_node.
 * \pre current_node must be not null (assert)
 * \param[in] current_node pointer to the current node
 * \return pointer to a value_t */
SHARED_EXPORT ccontainer_value_t* clist_gen_get_node_value(const clist_node_t* current_node);

/** Return the previous node relative to the current one provided.
 * \param[in] current_node pointer to the current node
 * \return pointer to a node, return NULL if the start of list is reached */
SHARED_EXPORT clist_node_t* clist_gen_get_previous_node(const clist_node_t* current_node);

/** Remove the node_to_delete from the clist_gen_t.
 * \param[in] clist pointer to a clist_gen_t instance
 * \param[in] node_to_delete pointer to a clist_node_t to remove
 * \return ccontainer error_code */
SHARED_EXPORT ccontainer_err_t clist_gen_remove_node(clist_gen_t *clist, clist_node_t* node_to_delete, deleter_value_t ptrf_deleter );

/** Return a node with value_t matching value_to_match */
SHARED_EXPORT clist_node_t* clist_gen_find_node( const clist_gen_t *clist, const clist_node_t *first_node,
                        equalizer_value_t fct_equalizer, const ccontainer_value_t* value_to_match,
                        ccontainer_err_t *err_code );

/** @} */ /* end node methods */

#ifdef __cplusplus
}
#endif

/** @} */ /** end ingroup ccontainer */