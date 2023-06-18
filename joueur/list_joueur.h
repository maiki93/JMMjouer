#pragma once

#include "joueur.h"
#include "shared_EXPORTS.h"

/** @ingroup lib_joueur_grp */
/** @defgroup ccontainer_joueur_grp ccontainers of joueurs */

/** @file
 * 
 * @ingroup ccontainer_joueur_grp
 * 
 * CContainers of joueurs with their results.
 *  as read / to write from a persistent storage.
 */

/** @{ @ingroup ccontainer_joueur_grp */

/** list joueur definition */
typedef struct {
    /** internal clist_gen_t */
    clist_gen_t *clist;
    /** store size of the list */
    size_t len;
} list_joueur_t;

/** \name Constructor / Destructor */
/** \{ */
/** Memory allocation */
SHARED_EXPORT list_joueur_t* list_joueur_new();
/** Constructor */
SHARED_EXPORT void list_joueur_init( list_joueur_t *this );
/** Destructor and free memory */
SHARED_EXPORT void list_joueur_free( list_joueur_t *this );
/** \} */

/** \name Public methods */
/** \{ */
/** Return size of the list */
SHARED_EXPORT size_t list_joueur_size( const list_joueur_t *this );
/** Insert a joueur at the end of the list */
SHARED_EXPORT int list_joueur_push_back( list_joueur_t *this, const joueur_t *joueur_in );
/** Get, and remove of the list, the first element */
SHARED_EXPORT joueur_t list_joueur_pop_front( list_joueur_t *this);
/** \} */

/** @} */ /* end ccontainer_joueur_grp */