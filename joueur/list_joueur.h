#pragma once

#include "joueur.h"

typedef struct {
    clist_gen_t *clist;
    size_t len;
} list_joueur_t;

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
