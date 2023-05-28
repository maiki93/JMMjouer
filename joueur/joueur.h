#pragma once

#include "joueur/person.h"
/* not needed here, only implementation */
#include "joueur/map_game_victories.h"

#include "shared_EXPORTS.h"

/* Use same define from person.h for specific code error */
/** @file
 * Defintion of a joueur = person + list of victories.
 * 
 * Memory Allocator/Deallocator, not needed if only use instance on stack ? */

/** joueur_t aggregates a person_t with the results of previous games.
   Can use all public API of person_t (kind of derive class) by using explicit cast
   \ingroup entities_grp */
typedef struct {
    /** person_t in first field to allow cast to person_t */
    person_t person;
    /** associate a map<name_game,victory_t> */
    map_game_victories_t map_victories;
} joueur_t;

/* Memory Allocator/Deallocator, not needed if only stack use ? */

/** Constructor with a default empty map of victories */
SHARED_EXPORT person_status_t joueur_init(joueur_t *joueur, const char * name, 
        bool is_daltonien, bool is_admin );
/** Default constructor.
 * person_t is invalid and map empty */
SHARED_EXPORT person_status_t joueur_default_init(joueur_t *joueur);
/** Destructor, clear the map */
SHARED_EXPORT void joueur_delete(joueur_t *joueur);

/** Print info to console, much better to do later for generalization */
SHARED_EXPORT void joueur_info(joueur_t *joueur);
