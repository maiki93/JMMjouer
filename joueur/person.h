#pragma once

#include <stdbool.h>

/* very few method marked as export ? it is working on windows ? */
#include "shared_EXPORTS.h"

/** @file
 * Definition of a person : name and characteristics : daltonien + admin.
 * 
 * Used as a base class for @ref joueur_t (by composition)
 * 
 * \defgroup entities_grp entities
 * Entities regroup data types of the business model.
 * Aimed for logic and storage (file, DB...)
*/

/** Maximum size for the name of a player. 
 *  It is a logic limit person_t contain a pointer to a string.
 * \ingroup entities_grp */
#define MAX_SIZE_NOM_PERSON 50
/*enum { MAX_SIZE_NOM_PERSON = 50 };*/    /* or enum considerered as constant expression */

/** \name Enum used for return values errors.
  \ingroup entities_grp */
/** @{ */
typedef enum {
    /** Invalid person. */
    PERSON_INVALID = -1,
    /** Valid person. */
    PERSON_VALID = 0,
    /** VALID and with admin right, validity test */
    PERSON_ADMIN = 1, /* with administration rights, use mask ? */
    /** for extension */
    PERSON_ENUM_END = 2
    /** case anonymous to add ? */
} person_status_t;
/** @} */

/** Description of a general person with name and specific profile.
  Full declaration of the structure in header, but api provides only accessors.
  How to enforce immutability in C, make_person() ? 
    hiding  structure declaration in impl ? but cannot use person on stack !!
  Used as a "base" class for joueur_t, so methods apply to both.
  \ingroup entities_grp */
typedef struct {
    /** name of the person.
     * const char* maybe a good idea here. do not change name of a person !
     * could use : NULL => PERSON_INVALID
     */
    char *pname; /*[ MAX_SIZE_NOM_PERSON ];*/
    /** profile, for use of colors in the game console */
    /* same for const here , possible ? or only by interface ? */
    bool is_daltonien;
    /** admin rights, put here maybe split later.
     *  default false, got info from record
    */
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
/** Destructor. 
 *  param[in] pointer to an instance of person_t */
void person_delete(person_t *person);
/** @} */

/** @name Public API usable with person_t or joueur_t */
/** @{ */

/** Return if a joueur is valid or not.
    To check if previous command succeeded or not */
SHARED_EXPORT person_status_t person_status(const person_t *person);

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
