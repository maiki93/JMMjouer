#ifndef PERSON_H_INCLUDED
#define PERSON_H_INCLUDED

#include <stdbool.h>

/* const int MAX_SIZE_NOM = 50; // not a constant expression */
/* #define MAX_SIZE_NOM 50     // macro possible */
enum { MAX_SIZE_NOM_PERSON = 20 };    /* or enum considerered as constant expression */

#define PERSON_OK 0
/*#define PERSON_ADMIN 1 */ /* with administration rights */
#define PERSON_INVALID -1

/* @brief Description of a general person with name and specific profile.
    Full declaration of the structure in header, public access in C  */
typedef struct {
    char nom[ MAX_SIZE_NOM_PERSON ] ; /* memory allocated better */
    bool is_daltonien;  /* for use of colors in the game console */
    /*bool is_admin;*/ /* default false, got info from record */
    /* to be in param game
    bool serie_3_game;  true : indicate if the player participe to a serie of all games, maximum one of each game)
                        false : ask to make again a game */
    /* simpler to include the list of victories inside,
     maybe too linked... copy etc... make only one joueur_with_victory ? */
    /*cmap_game_victories_t map_victories;*/
} person_t;

/* @brief Constructor
    return PERSON_OK on success, PERSON_INVALID if name too long */
int person_init(person_t * person, const char * name, bool is_daltonien);
/* \brief Default constructor. 
    Set name:"invalid" and is_daltonien:false */
int person_default_init(person_t *person);

/* @brief Return if a joueur is valid or not.
    To check if previous command succeeded or not */
bool person_is_valid(person_t *person);
/* @brief Return the name of a person or a joueur
    Explicit cast to person_t needed if use with joueur */
const char* person_name(person_t *person);
/* Print info to console, much better to do later for generalization */
void person_info(person_t *person);

#endif