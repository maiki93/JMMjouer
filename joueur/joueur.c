#include <stdio.h>
#include <string.h>
#include <assert.h>

/* Complete structure defined in header */
#include "joueur.h"

person_status_t joueur_init(joueur_t *joueur, const char * name, 
                bool is_daltonien, bool is_admin )
{
    person_status_t status;
    /* cast to "base" ok apply to joueur.person */
    status = person_init( (person_t*)joueur, name, is_daltonien, is_admin);
    /* empty map by default, 
        even if invalid must be correctly initialized, or pointer */
    game_victories_init( & joueur->map_victories );
    /* need to force map to NULL ? in release ? */
    /*
    if( status == PERSON_INVALID) {
        return status;
    }*/
    /*assert( game_victories_size( & joueur->map_victories ) == 0 );*/
    return status;
}

/* not DRY here, static set_map_victories */
person_status_t joueur_default_init(joueur_t *joueur)
{
    person_status_t status;
    status = person_default_init( (person_t*)joueur );
    game_victories_init( & joueur->map_victories );
    return status;   
}

void joueur_delete(joueur_t *joueur)
{
    assert( joueur );
    person_delete( (person_t*) joueur);
    /* to write */
    /*
    if( game_victories_size( & joueur->map_victories) != 0) {
        printf("Joueur %s, TODO must clear the map", joueur->person.pname);
    } */
    /* to do all the time indeeded */
    game_victories_delete( &(joueur->map_victories) );       
}

void joueur_info(joueur_t *joueur)
{
    printf("\n--- Profile Joueur ----\n");
    person_info( &joueur->person );
    /* print_info victory */
    printf("-------------------\n");
}


