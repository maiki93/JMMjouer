#pragma once

#include <stdbool.h>

/* very few method marked as export ? it is working on windows ? */
#include "shared_EXPORTS.h"

/** @file
 * Definition of a person : name and characteristics : daltonien + admin.
 * 
 * Used as a base class for @ref joueur_t (by composition to simulate derivation)
 * 
 * \defgroup entities_grp entities
 * Entities regroup data types of the business model.
 * Aimed for logic and storage (file, DB...)
*/

/** Maximum size for the name of a player. 
 *  It is a logic limit person_t contain a pointer to a string.
 *  Maybe not the correct level to set limitations, 
 *      better in jmmjouer for librecord
 * \ingroup entities_grp */
#define MAX_SIZE_NOM_PERSON 50

/** Anonymous persons are valid, have the same name but unqiue id identifer */
const char* ANONYMOUS_NAME = "anonymous";

/* or enum considerered as constant expression */
/*enum { MAX_SIZE_NOM_PERSON = 50 };*/

/** \name Enum used for return values code error.
 * Look like mix of error and state.
 * Only error person VALID / INVALID all other are recomputed
 * anonymous = name 'anonymous'(+XX if more than one? or keep same, only id is different)
 *              id must be different in all cases
  \ingroup entities_grp */
/** @{ */
typedef enum {
    /** Invalid person in case of error, or wrong password */
    PERSON_INVALID = -1,
    /** Valid person, anonymous is valid, iff correctly created/ recorded */
    PERSON_VALID = 0
} person_status_t;
/** @} */

/* all others only for convenience 
    is_admin()
    is_anonymous()
  */
/*
    /// VALID and with admin right, validity test
    PERSON_ADMIN = 1, // with administration rights, use mask
    /// ANONYMOUS, valid but no name or "anynonmous"
    PERSON_ANONYMOUS = 2,
    //// for extension
    PERSON_ENUM_END = 3
    ////case anonymous to add
*/

/** Description of a general person with a name and its specific profile.
  Full declaration of the structure in header, but api provides only accessors.
    hiding  structure declaration in impl ? but could not use person on stack
    a private field, but librecord shoud have access (or use DAO)
  Used as a "base" class for joueur_t, so methods apply to both with a explicit cast.
  How to enforce immutability in C, make_person() ? 
  \ingroup entities_grp */
typedef struct {
    /* unique identifier : surrogate id for db / storage */
    size_t person_id;
    /** name of the person, if NULL person_invalid or anonymous
     * const char* maybe a good idea here. do not change name of a person !
     * could use : NULL => PERSON_INVALID */
    char *pname; /*[ MAX_SIZE_NOM_PERSON ];*/
    /** profile, for use of colors in the game console */
    /* same for const here , possible ? or only by interface ? */
    bool is_daltonien;
    /** admin rights, put here maybe split later.
     *  default false, got info from record */
    bool is_admin;
} person_t;

/** @name Constructor / Destructor for only stack usage */
/** @{ */
/** Default constructor. 
    Set name:"invalid" (really needed ?)
    is_daltonien:false is_admin:false
    \param[in] pointer to an instance of person_t
    \return PERSON_INVALID */
person_status_t person_default_init(person_t *person);
/** Constructor with parameters.
 *  \param[in] pointer to an instance of person_t
 *  \param name of the person
 *  \param is_daltonien : there is worse disease
    \return PERSON_OK on success, PERSON_INVALID if name too long > MAX_SIZE_NOM_PERSON */
person_status_t person_init(person_t * person, const char * name, 
        bool is_daltonien, bool is_admin);

/** Test */
person_t make_person(size_t id, const char * name, 
        bool is_daltonien, bool is_admin, person_status_t *status);

/** Destructor. 
 *  param[in] pointer to an instance of person_t */
void person_delete(person_t *person);
/** @} */

/** @name Public API usable with person_t or joueur_t */
/** @{ */

/** Return if a joueur is valid or not.
    To check if previous command succeeded or not */
SHARED_EXPORT person_status_t person_status(const person_t *person);

/** Alternative */
bool person_valid(const person_t *person);

/** Return the name of a person or a joueur.
    Explicit cast to person_t needed if use with joueur_t */
const char* person_name(const person_t *person);

bool person_daltonien(const person_t *person);
bool person_admin(const person_t *person);
/** Print info to console.
 *  much better to do later for generalization */
void person_info(const person_t *person);

/** Give administrator right.
 * Only set method of the class
*/
/* void person_set_admin(person_t *person); */

/** @} */
