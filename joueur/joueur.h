#pragma once

#include "joueur/user.h"
#include "joueur/map_game_score.h"

#include "shared_EXPORTS.h"

/** @file
 * Defintion of a joueur = user + list of victories.
 * 
 * Memory Allocator/Deallocator, not needed if only use instance on stack ? 
 * Use same define from user.h for specific code error */

/** joueur_t aggregates a user_t with the results of previous games.
   Can use all public API of user_t (kind of derive class) by using explicit cast.
   Use same define from user.h for specific code error
   \ingroup entities_grp */
typedef struct {
    /** user_t as first field to allow cast to user_t */
    user_t user;
    /** associate a map<name_game,score_t> */
    map_game_score_t map_score;
} joueur_t;

/* Memory Allocator/Deallocator, not needed if only stack use ? */

/** Constructor with a default empty map of victories */
SHARED_EXPORT user_status_t joueur_init(joueur_t *joueur, size_t id, const char * name, 
        bool is_daltonien, bool is_admin );
/** Default constructor.
 * user_t is invalid and map empty */
SHARED_EXPORT user_status_t joueur_default_init(joueur_t *joueur);
/* certainly convenient to recompose a joueur from user + scores , db ? */
/*SHARED_EXPORT joueur_init_user_map_game(user_t *user, map_game_score_t* map_game_score);*/

/** Destructor, clear the map */
SHARED_EXPORT void joueur_delete(joueur_t *joueur);

/** Print info to console.
 *  much better to do later for generalization */
SHARED_EXPORT void joueur_print_info(const joueur_t *joueur);

SHARED_EXPORT user_status_t joueur_status(const joueur_t *joueur);
SHARED_EXPORT bool joueur_valid(const joueur_t *joueur);

SHARED_EXPORT size_t joueur_id(const joueur_t *joueur);

/** \name methods "inherited" from user_t */
/** \{ */
/** Return the name of a user or a joueur.
    Explicit cast to user_t needed if use with joueur_t */
SHARED_EXPORT const char* joueur_name(const joueur_t *joueur);

SHARED_EXPORT bool joueur_daltonien(const joueur_t *joueur);
SHARED_EXPORT bool joueur_admin(const joueur_t *joueur);
/* validity / right also */
/** \} */

