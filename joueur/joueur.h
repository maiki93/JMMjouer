#pragma once

#include "joueur/person.h"
#include "joueur/map_game_score.h"

#include "shared_EXPORTS.h"

/** @file
 * Defintion of a joueur = person + list of victories.
 * 
 * Memory Allocator/Deallocator, not needed if only use instance on stack ? 
 * Use same define from person.h for specific code error */

/** joueur_t aggregates a person_t with the results of previous games.
   Can use all public API of person_t (kind of derive class) by using explicit cast.
   Use same define from person.h for specific code error
   \ingroup entities_grp */
typedef struct {
    /** person_t as first field to allow cast to person_t */
    person_t person;
    /** associate a map<name_game,score_t> */
    map_game_score_t map_score;
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

/** Print info to console.
 *  much better to do later for generalization */
void joueur_info(const joueur_t *joueur);

/** \name methods "inherited" from person_t */
/** \{ */
/** Return the name of a person or a joueur.
    Explicit cast to person_t needed if use with joueur_t */
const char* joueur_name(const joueur_t *joueur);

bool joueur_daltonien(const joueur_t *joueur);
bool joueur_admin(const joueur_t *joueur);
/* validity / right also */
/** \} */

