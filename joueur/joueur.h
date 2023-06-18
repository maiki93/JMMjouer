#pragma once

#include "joueur/user.h"
#include "joueur/map_game_score.h"

#include "shared_EXPORTS.h"

/** @file
 * 
 *  @ingroup entities_grp
 * 
 * Defintion of a joueur = user + map of scores.
 * 
 * Use same define from user.h for specific code error.
 * 
 * Memory Allocator/Deallocator, not needed if only use instance on stack ? 
 */

/** @{ \ingroup entities_grp */

/** joueur_t aggregates a user_t with the results of previous games.
   Can use all public API of user_t (kind of derive class) by using explicit cast.
   Use same define from user.h for specific code error */
typedef struct {
    /** user_t as first field to allow cast to user_t */
    user_t user;
    /** associate a map<name_game,score_t> */
    map_game_score_t map_score;
} joueur_t;

/** Constructor with a default empty map of scores */
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

/** \name methods "inherited" from user_t */
/** \{ */

/** Return the status */
SHARED_EXPORT user_status_t joueur_status(const joueur_t *joueur);
/** Return validity */
SHARED_EXPORT bool joueur_valid(const joueur_t *joueur);
/** Return unique id */
SHARED_EXPORT size_t joueur_id(const joueur_t *joueur);
/** Return the name of a user or a joueur.
    Explicit cast to user_t needed if use with joueur_t */
SHARED_EXPORT const char* joueur_name(const joueur_t *joueur);
/** Return profile data */
SHARED_EXPORT bool joueur_daltonien(const joueur_t *joueur);
/** Return profile data */
SHARED_EXPORT bool joueur_admin(const joueur_t *joueur);
/** \} */

/** @} */ /* end entities_grp */

