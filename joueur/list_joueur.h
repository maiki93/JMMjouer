#pragma once

#include "joueur.h"
#include "shared_EXPORTS.h"

typedef struct {
    clist_gen_t *clist;
    size_t len;
} list_joueur_t;

/** @file
 *  In-Memory joueurs with their results, as read / to write from a persistent storage.
 */

SHARED_EXPORT list_joueur_t* list_joueur_new();
SHARED_EXPORT void list_joueur_init( list_joueur_t *this );
SHARED_EXPORT void list_joueur_free( list_joueur_t *this );

SHARED_EXPORT size_t list_joueur_size( const list_joueur_t *this );

SHARED_EXPORT int list_joueur_push_back( list_joueur_t *this, const joueur_t *joueur_in );
/* not exactly this interface, maybe a pop is good */
SHARED_EXPORT joueur_t list_joueur_pop_front( list_joueur_t *this);
