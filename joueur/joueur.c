#include <stdio.h>
#include <string.h>
#include <assert.h>

/* Complete structure defined in header */
#include "joueur.h"

user_status_t joueur_init(joueur_t *joueur, size_t id, const char * name, 
                bool is_daltonien, bool is_admin )
{
    user_status_t status;
    /* cast to "base" ok apply to joueur.user */
    status = user_init( (user_t*)joueur, id, name, is_daltonien, is_admin);
    /* empty map by default, 
        even if invalid must be correctly initialized, or pointer */
    map_game_score_init( & joueur->map_score );
    assert( map_game_score_size( &(joueur->map_score) )  == 0 );
    return status;
}

/* not DRY here, static set_map_victories */
user_status_t joueur_default_init(joueur_t *joueur)
{
    user_status_t status;
    status = user_default_init( (user_t*)joueur );
    map_game_score_init( & joueur->map_score );
    return status;   
}

void joueur_delete(joueur_t *joueur)
{
    assert( joueur );
    user_delete( (user_t*) joueur);
    /* to write */
    /*
    if( map_game_score_size( & joueur->map_victories) != 0) {
        printf("Joueur %s, TODO must clear the map", joueur->user.pname);
    } */
    /* to do all the time indeeded */
    map_game_score_delete( &(joueur->map_score) );
    /*map_game_score_free( &(joueur->map_victories) );*/
}

void joueur_print_info(const joueur_t *joueur)
{
    printf("\n--- Profile Joueur ----\n");
    user_info( &joueur->user );
    /* print_info victory */
    printf("-------------------\n");
    map_game_score_print_info( &(joueur->map_score) );
}

user_status_t joueur_status(const joueur_t *joueur)
{
    return user_status((const user_t*)joueur);
}

bool joueur_valid(const joueur_t *joueur)
{
    return user_valid((const user_t*)joueur);
}

size_t joueur_id(const joueur_t *joueur)
{
    return user_id((const user_t*) joueur);
}

/*  Inherited simple pass plat */
const char* joueur_name(const joueur_t *joueur)
{
    return user_name((const user_t*) joueur );
}

bool joueur_daltonien(const joueur_t *joueur)
{
    return user_daltonien((const user_t*) joueur);
}

bool joueur_admin(const joueur_t *joueur)
{
    return user_admin((const user_t*) joueur);
}
