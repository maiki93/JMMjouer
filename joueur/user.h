#pragma once

/*#include <stdlib.h>*/ /*size_t*/
#include <stddef.h> /*size_t windows also ? */
#include <stdbool.h>

/* very few method marked as export ? it is working on windows ? */
#include "shared_EXPORTS.h"

/** @file
 * Definition of a user : name and characteristics : daltonien + admin.
 * 
 * Used as a base class for @ref joueur_t (by composition to simulate derivation)
 * 
 * \defgroup entities_grp entities
 * Entities regroup data types of the business model.
 * Aimed for logic and storage (file, DB...)
*/

/** Maximum size for the name of a player. 
 *  It is a logic limit user_t contain a pointer to a string.
 *  Maybe not the correct level to set limitations, 
 *      better in jmmjouer for librecord or at compilation -D
 * \ingroup entities_grp */
#define MAX_SIZE_NAME_USER 50

/** Anonymous users, "anonymous" name, are valid , same name but unqiue id identifer */
extern const char* ANONYMOUS_NAME; /* extern avoid multiple definition at link stage */

/** \name Enum used for return values code error.
 * Look like mix of error and state.
 * Only error user VALID / INVALID all other are recomputed
 * anonymous = name 'anonymous'(+XX if more than one? or keep same, only id is different)
 *              id must be different in all cases
  \ingroup entities_grp */
/** @{ */
typedef enum {
    /** Invalid user in case of error, or wrong password */
    USER_INVALID = -1,
    /** Valid user, anonymous is valid, iff correctly created/ recorded */
    USER_VALID = 0
} user_status_t;
/** @} */

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
  Used as a "base" class for joueur_t, so methods apply to both with a explicit cast.
  \ingroup entities_grp */
typedef struct {
    /* unique identifier : surrogate id for db / storage, 0 for invalid */
    size_t user_id;
    /** name of the user, if NULL user_invalid or anonymous
     * const char* maybe a good idea here. do not change name of a user !
     * could use : NULL => USER_INVALID */
    char *pname; /*[ MAX_SIZE_NAME_USER ];*/
    /** profile, for use of colors in the game console */
    /* same for const here , possible ? or only by interface ? */
    bool is_daltonien;
    /** admin rights, put here maybe split later.
     *  default false, got info from record */
    bool is_admin;
} user_t;

/** @name Constructor / Destructor for only stack usage */
/** @{ */
/** Default constructor. 
    Set name:"invalid" (really needed ?)
    is_daltonien:false is_admin:false
    \param[in] pointer to an instance of user_t
    \return USER_INVALID */
user_status_t user_default_init(user_t *user);
/** Constructor with parameters.
 *  \param[in] pointer to an instance of user_t
 *  \param name of the user
 *  \param is_daltonien : there is worse disease
    \return USER_OK on success, USER_INVALID if name too long > MAX_SIZE_NAME_USER */
user_status_t user_init(user_t *user, size_t id, const char * name,
                        bool is_daltonien, bool is_admin );

/** Test */
user_t make_user(size_t id, const char *name, 
        bool is_daltonien, bool is_admin, user_status_t *status);

/** Destructor. 
 *  param[in] pointer to an instance of user_t */
void user_delete(user_t *user);
/** @} */

/** @name Public API usable with user_t or joueur_t */
/** @{ */

/** Return if a joueur is valid or not.
    To check if previous command succeeded or not */
SHARED_EXPORT user_status_t user_status(const user_t *user);

/** Alternative */
bool user_valid(const user_t *user);

/** Return uuid of the joueur
 * \param[in]
 * \param[out] valid users > 0, invalid == 0 */
size_t user_id(const user_t *user);


/** Return the name of a user or a joueur.
    Explicit cast to user_t needed if use with joueur_t */
const char* user_name(const user_t *user);

bool user_daltonien(const user_t *user);
bool user_admin(const user_t *user);

/** Print info to console.
 *  much better to do later for generalization */
void user_info(const user_t *user);

/** @} */
