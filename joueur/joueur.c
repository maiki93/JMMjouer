#include <stdio.h>
#include <string.h>
#include <assert.h>

/* Complete structure defined in header */
#include "joueur.h"

int joueur_init(joueur_t *joueur, const char * name, bool is_daltonien )
{
    int retour;
    /* cast to "base" ok apply to joueur.person */
    retour = person_init( (person_t*)joueur, name, is_daltonien);
    if( retour != PERSON_OK)
        return retour;
    /* empty map by default */
    game_victories_init( & joueur->map_victories );
    assert( game_victories_size( & joueur->map_victories ) == 0 );
    return PERSON_OK;
}

int joueur_default_init(joueur_t *joueur)
{
    int retour;
    retour = person_default_init( (person_t*)joueur );
    if( retour != PERSON_OK)
        return retour;
    game_victories_init( & joueur->map_victories );
    assert( game_victories_size( & joueur->map_victories ) == 0 );
    return PERSON_OK;   
}

void joueur_delete(joueur_t *joueur)
{
    assert( joueur );
    /* to write */
    if( game_victories_size( & joueur->map_victories) != 0) {
        /* game_victories_clear; */ /* TODO */
        printf("Joueur %s, TODO must clear the map", joueur->person.nom);
    } 
    /* to do all the time indeeded */
    game_victories_delete( &(joueur->map_victories) );       
}

void joueur_info(joueur_t *joueur)
{
    printf("\n--- Profile ----\n");
    person_info( &joueur->person );
    printf("-------------------\n");
    
}


