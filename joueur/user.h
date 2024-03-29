#pragma once

#include <stddef.h> 
#include <stdbool.h>

#include "shared_EXPORTS.h"

/* best place to declare a group ? */
/** @defgroup lib_joueur_grp entities library */
/** @ingroup lib_joueur_grp */
/** @defgroup entities_grp entities : user, joueur and scores */

/** @file
 * 
 * @ingroup entities_grp
 * 
 * Definition of a user : name and characteristics : daltonien + admin.
 * 
 * defgroup entities_grp entities : user, joueur and scores
 * ingroup lib_joueur_grp
 * 
 * Used as a base class for joueur_t (by composition, simulate derivation)
 * 
 * Entities regroup data types of the business model.
 * Aimed for logic and storage (file, DB...) 
 */

/** @{ \ingroup entities_grp entities */

/** Maximum size for the name of a player. 
 *  It is a logic limit user_t contain a pointer to a string.
 *  Maybe not the correct level to set limitations, 
 *      better in jmmjouer for librecord or at compilation -D */
#define MAX_SIZE_NAME_USER 50

/** Anonymous users, "anonymous" name, are valid , same name but unqiue id identifer
 * really need to be in API ? */
extern const char* ANONYMOUS_NAME; /* extern avoid multiple definition at link stage */

/** Enum used for return values code error.
 * Look like mix of error and state.
 * Only error user VALID / INVALID all other are recomputed
 * anonymous = name 'anonymous'(+XX if more than one? or keep same, only id is different)
 *              id must be different in all cases */
typedef enum {
    /** Invalid user in case of error, or wrong password */
    USER_INVALID = -1,
    /** Valid user, anonymous is valid, iff correctly created/ recorded */
    USER_VALID = 0
} user_status_t;

/* all others only for convenience 
    is_admin()
    is_anonymous()
  */
/*
    /// VALID and with admin right, validity test
    USER_ADMIN = 1, // with administration rights, use mask
    /// ANONYMOUS, valid but no name or "anynonmous"
    USER_ANONYMOUS = 2,
    //// for extension
    USER_ENUM_END = 3
    ////case anonymous to add
*/

/** Description of a general user with a name and its specific profile.
    Full declaration of the structure in header, but api provides only accessors.
    hiding  structure declaration in impl ? but could not use user on stack
    a private field, but librecord shoud not have access (or use DAO)...
    Used as a "base" class for joueur_t, so methods apply to both with a explicit cast. */
typedef struct {
    /** unique identifier 0 for invalid */
    size_t user_id;
    /** name of the user, [ MAX_SIZE_NAME_USER ].
     * NULL if user_invalid, anonymous ?
     * const char* maybe a good idea here. do not change name of a user !
     * could use : NULL => USER_INVALID */
    char *pname;
    /** profile, for use of colors in the game console */
    /* same for const here , possible ? or only by interface ? */
    bool is_daltonien;
    /** admin rights, put here maybe split later.
     *  default false, got info from record */
    bool is_admin;
} user_t;

/** \name Constructor / Destructor for stack usage only */
/** \{ */
/** Default constructor. 
    Set name:"invalid" (really needed ?)
    is_daltonien:false is_admin:false
    \param[in] user pointer to an instance of user_t
    \return USER_INVALID */
SHARED_EXPORT user_status_t user_default_init(user_t *user);

/** Constructor with parameters.
 *  \param[in] user pointer to an instance of user_t
 *  \param name of the user
 *  \param is_daltonien : there is worse disease
    \return USER_OK on success, USER_INVALID if name too long > MAX_SIZE_NAME_USER */
SHARED_EXPORT user_status_t user_init(user_t *user, size_t id, const char * name,
                                      bool is_daltonien, bool is_admin );

/** Test */
SHARED_EXPORT user_t make_user(size_t id, const char *name, 
                        bool is_daltonien, bool is_admin, user_status_t *status);

/** Destructor. 
 *  param[in] pointer to an instance of user_t */
SHARED_EXPORT void user_delete(user_t *user);
/** \} */

/** @name Public API usable with user_t or joueur_t */
/** \{ */
/** Return if a joueur is valid or not.
    To check if previous command succeeded or not */
SHARED_EXPORT user_status_t user_status(const user_t *user);
/** Alternative */
SHARED_EXPORT bool user_valid(const user_t *user);
/** Return uuid of the joueur
 * \param[in] user pointer to an instance of user_t
 * \return id valid users > 0, invalid == 0 */
SHARED_EXPORT size_t user_id(const user_t *user);
/** Return the name of a user or a joueur.
    Explicit cast to user_t needed if use with joueur_t */
SHARED_EXPORT const char* user_name(const user_t *user);
/** Return profile data */
SHARED_EXPORT bool user_daltonien(const user_t *user);
/** Return profile data */
SHARED_EXPORT bool user_admin(const user_t *user);
/** Print info to console.
 *  much better to do later for generalization */
SHARED_EXPORT void user_info(const user_t *user);
/** \} */

/** @} */ /* end entities_grp */
