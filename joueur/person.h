#ifndef PERSON_H_INCLUDED
#define PERSON_H_INCLUDED

#include <stdbool.h>

#include "shared_EXPORTS.h"

/* by default all *h files included in doxygen, or just use at least one command block ? */
/** @file
 * Definition of a person : name and characteristics (daltonien).
 * 
 * Used as a base class for @ref joueur_t (by composition)
 * 
 * \defgroup entities_grp entities
 * Entities regroup data types of the business model.
 * Aimed for logic and storage (file, DB...)
*/

/* const int MAX_SIZE_NOM = 50; // not a constant expression */
/* #define MAX_SIZE_NOM 50     // macro possible */
/** Maximum size for the name of a player
 * \ingroup entities_grp */
enum { MAX_SIZE_NOM_PERSON = 20 };    /* or enum considerered as constant expression */

/** \name Enum used for return values errors.
  \ingroup entities_grp */
/** @{ */
/** Valid person. */
#define PERSON_OK 0
/** Invalid person. */
#define PERSON_INVALID -1
/** @} */
/* #define PERSON_ADMIN 1 */ /* with administration rights */

/** Description of a general person with name and specific profile.
  Full declaration of the structure in header, gives public access in C.
  Used as a "base" class for joueur_t, so methods apply to both.
  \ingroup entities_grp */
typedef struct {
    /** name of the person */
    char nom[ MAX_SIZE_NOM_PERSON ];
    /** profile, for use of colors in the game console */
    bool is_daltonien;
    /*bool is_admin;*/ /* default false, got info from record */
    /* to be in param game
    bool serie_3_game;  true : indicate if the player participe to a serie of all games, maximum one of each game)
                        false : ask to make again a game */
    /* simpler to include the list of victories inside,
     maybe too linked... copy etc... make only one joueur_with_victory ? */
    /*cmap_game_victories_t map_victories;*/
} person_t;

/** @name Constructor */
/** @{ */
/** Default constructor. 
    Set name:"invalid" and is_daltonien:false */
int person_default_init(person_t *person);
/** Constructor with parameters.
    \return PERSON_OK on success, PERSON_INVALID if name too long */
int person_init(person_t * person, const char * name, bool is_daltonien);
/** @} */

/** @name Public API usable with person_t or joueur_t */
/** @{ */
/** Return if a joueur is valid or not.
    To check if previous command succeeded or not */
SHARED_EXPORT bool person_is_valid(person_t *person);
/** Return the name of a person or a joueur.
    Explicit cast to person_t needed if use with joueur_t */
const char* person_name(person_t *person);
/** Print info to console.
 *  much better to do later for generalization */
void person_info(person_t *person);
/** @} */

#endif