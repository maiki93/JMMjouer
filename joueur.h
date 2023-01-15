#ifndef JOUEUR_H_INCLUDED
#define JOUEUR_H_INCLUDED

#include "person.h"
#include "cmap_game_victories.h"

/* Use same define from person.h for specific code error */

/* @brief Joueur aggregate person with results of previous games.
    Match structure expected by records, later for joueur/person_manager */
typedef struct {
    /* first field to allow cast to person_t */
    person_t person;
    cmap_game_victories_t map_victories;
} joueur_t;

/* Memory Allocator/Deallocator, not needed if only stack use ? */

/* @brief Constructor with default map victories empty */
int joueur_init(joueur_t *joueur, const char * name, bool is_daltonien );
int joueur_default_init(joueur_t *joueur);
/* init( with Joueur already created), or with cmap already created */
/* @brief Destructor, clear the map */
void joueur_clear(joueur_t *joueur);
/* Print info to console, much better to do later for generalization */
void joueur_info(joueur_t *joueur);

#endif