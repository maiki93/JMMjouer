#pragma once

#include "joueur.h"

/* to use optimized list_to_vector, must get internal for casting to gen_t
   possible to implement in list_joueur also...
typedef struct list_joueur list_joueur_t;
*/
typedef struct {
    clist_gen_t *clist;
    size_t len;
} list_joueur_t;

/* typedef struct cvector_joueur cvector_joueur_t;*/ /* better out of clist, adapter if not better */

/** @file
 *  In-Memory joueurs with their results, as read / to write from a persistent storage.
 */

list_joueur_t* list_joueur_new();
void list_joueur_init( list_joueur_t *this );
void list_joueur_free( list_joueur_t *this );

size_t list_joueur_size( const list_joueur_t *this );

int list_joueur_push_back( list_joueur_t *this, const joueur_t *joueur_in );
/* not exactly this interface, maybe a pop is good */
joueur_t list_joueur_pop_front( list_joueur_t *this);
