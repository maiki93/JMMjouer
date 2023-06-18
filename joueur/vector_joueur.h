#pragma once

#include "joueur.h"
#include "shared_EXPORTS.h"

/** @file
 * 
 * @ingroup ccontainer_joueur_grp 
 * 
 * Vector of joueur container.
*/

/** @{ @ingroup ccontainer_joueur_grp */

/** Definition of the vector */
typedef struct {
   /** Internal cvector generic */
   cvector_gen_t* cvect; 
} vector_joueur_t;


/** \name Constructor / Destructor */
/** \{ */
/** Memory allocation */
SHARED_EXPORT vector_joueur_t* vector_joueur_new();
/** Constructor 
 * \return status 0 is SUCCESS */
SHARED_EXPORT int vector_joueur_init( vector_joueur_t *this );
/** Constructor with an initial capacity */
SHARED_EXPORT int vector_joueur_init_with_capacity( vector_joueur_t *this, size_t init_capacity );
/** Destructor */
SHARED_EXPORT void vector_joueur_delete( vector_joueur_t *this );
/** Free memory */
SHARED_EXPORT void vector_joueur_free( vector_joueur_t *this );
/** \} */

/** \name Public methods */
/** @{ */ 
/** Return nb of elements in the cvector */
SHARED_EXPORT size_t vector_joueur_size( const vector_joueur_t *this );

/** Return a copy of the element at index 
 *  Always same : better return or pass by pointer..easy to use return. */
SHARED_EXPORT joueur_t vector_joueur_get_at( const vector_joueur_t *this, size_t index);

/** Insert a copy of a joueur_t */
SHARED_EXPORT int vector_joueur_push_back( vector_joueur_t *this, const joueur_t *joueur_in );
/** @} */ 

/** @} */ /* end ccontainer_joueur_grp */
