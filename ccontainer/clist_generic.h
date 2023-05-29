#pragma once

#include "shared_EXPORTS.h"

#include "ccontainer/value.h" /* include error_code.h */
#include "ccontainer/cvector_generic.h" /* or forward declaration */

/** @file
 * Forward list implementation of a generic structure
 * Aimed to be used as an implementation by ccontainer's of the package.
 * 
 * ingroup ccontainer
 * page ccontainer create similar tree input as a markdown
 * 
 * ccontainer is a library of simple containers for generic type clist_generic::ccontainer_value_t and provide basic functionnalities.
 */

/**
 * @defgroup ccontainer_clist_gen_grp clist_generic generic forward list container
 * @ingroup ccontainer_grp
 */

/** @{ \ingroup ccontainer_clist_gen_grp */

#ifdef __cplusplus
extern "C" {
#endif

/** Forward declaration of the list and node structures.
 * Keep details hidden in implementation
 * \ingroup ccontainer
 */
typedef struct clist_type clist_gen_t;
typedef struct clist_node_type clist_node_t;

/** \name Constructor/Destructor */
/** @{ */
/** Memory Allocation. */
SHARED_EXPORT clist_gen_t* clist_gen_new();

/** Initialization. 
    \param[in] clist pointer to a clist_gen_t */
SHARED_EXPORT void clist_gen_init(clist_gen_t* clist);

/** Copy constructor. 
    \param[in] clist pointer to a clist_gen_t */
SHARED_EXPORT clist_gen_t* clist_gen_copy(const clist_gen_t* clist_src, duplicater_value_t ptrf_duplicater, 
                                          ccontainer_err_t *err_code );

/** Clear all contents of the list.
  \param[in] clist pointer to a clist_gen_t
  \param[in] deleter function with signature \ref deleter_t to properly free the value in case of dynamic allocation */
SHARED_EXPORT void clist_gen_delete(clist_gen_t *clist, deleter_value_t ptrf_deleter);

/** Free memory of the list.
  Call clist_gen_clear() if not empty (need to store deleter ??)
  \todo check for deleter
  \param[in] clist pointer to a clist_gen_t
  \param[in] deleter function with signature \ref deleter_t to properly free the value in case of dynamic allocation */
SHARED_EXPORT void clist_gen_free(clist_gen_t *clist, deleter_value_t ptrf_deleter);
/** @} */

/** Return the number of ccontainer_value_t elements in the list.
    \note Actual, store a len data member, may change later,
    Total size of the message (sum of len) may be usefull for serialization (to think)
    \param[in] clist pointer to a clist_gen_t */
SHARED_EXPORT size_t clist_gen_size(const clist_gen_t *clist);

/** @brief Transfer the input value at the first element of the list
 * \param[in] clist pointer to a clist_gen_t
 * \param[in] value ccontainer_value_t to be copied into the list
 */
SHARED_EXPORT ccontainer_err_t clist_gen_push_front(clist_gen_t *clist, ccontainer_value_t *value);

/** @brief Make a copy of the input value at the end of the list 
 * \param[in] clist pointer to a clist_gen_t
*/
SHARED_EXPORT ccontainer_err_t clist_gen_push_back(clist_gen_t *clist, ccontainer_value_t *value_in);

/* tricky to remove a node, need to find where !! */
SHARED_EXPORT void clist_gen_remove_node(clist_gen_t *clist, clist_node_t* node, deleter_value_t ptrf_deleter );

/** Interface and usage not very convenient ** if want to modify only, not safe as well 
   but no copy needed 
   \param[in] clist pointer to a clist_gen_t */
SHARED_EXPORT ccontainer_value_t* clist_gen_get_at( const clist_gen_t *clist, size_t index, ccontainer_err_t *err_code);
/** Get a copy of the content , better adapted for use ? 
     \param[in] clist pointer to a clist_gen_t */
/* SHARED_EXPORT int clist_gen_get_value_copy( const clist_gen_t *clist, size_t elem_nb, ccontainer_value_t *value_out); */

/** Retrieve a reference of the first value_t matching the criteria.
   Most important function  for reuse and specialization, very generic at this level 
   It checks the first n bytes given in buffer (later comp function ?)
   \todo works for string, need a comp function indeed.
   \param[in] clist pointer to a clist_gen_t
   \param[out] value_out pointer to an internally allocated ccontainer_value_t
   \return ERROR code */
SHARED_EXPORT ccontainer_value_t* clist_gen_find( const clist_gen_t* clist, 
                                    const char* buffer, size_t buffer_len, ccontainer_err_t *err_code);

/* tricky to remove generic from a node, need to retieve node before / after(ok) 
  => double link list ? or very not optimised in O(n) to find.
  and must provide a deleter if value_t has pointer to allocated memory (deleter for node indeed)
  provide before and current node to be able to remove it correctly + deleter */
/*SHARED_EXPORT ccontainer_value_t clist_gen_pop_node_value(const clist_gen_t* clist, clist_node_t *node, ccontainer_err_t *err_code);*/

/* this is optimal, pop_back less */
SHARED_EXPORT ccontainer_value_t clist_gen_pop_front_node_value(clist_gen_t* clist, ccontainer_err_t *err_code);

/** \name Internal node structure */
/** @{ */
/** Return the first node.
 *  Make API simpler to use with 2 methods : get_first(clist), get_next(ciurrent_node)
 * Maybe not necessary public : clist_pop / clist_pop_move better (to make in ccontainer_queue)
 * But convenient for extension */
clist_node_t* clist_gen_get_first_node( const clist_gen_t* clist);

/** Return the next node of the provided node.
 * \param[in] current_node pointer to the current node
 * \return pointer to a node, return NULL if end of list is reached */
clist_node_t* clist_gen_get_next_node( const clist_node_t* current_node);
/* static
clist_node_t* get_node_last( clist_node_t *fnode );
clist_node_t* get_node_number( clist_node_t *fnode, size_t elem_nb);
*/
/** @} */

/* first implementation to move in ccontainer_algorithm / helper */
/*SHARED_EXPORT cvector_gen_t* clist_move_to_cvector(clist_gen_t* clist, ccontainer_err_t *err_code);*/

#ifdef __cplusplus
}
#endif

/** @} */ /** end ingroup ccontainer */