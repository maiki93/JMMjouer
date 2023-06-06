#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <assert.h>

#include <limits.h> /* ubuntu, required for INT_MAX */

/* order of include important */
#include "arcade_params.h"
#include "arcade.h"
#include "joueur/joueur.h"         /* person included */
#include "joueur/victory.h"
#include "record/irecord.h"
#include "game_loader/game_loader.h" /* already in h */
#include "game_loader/plugin_manager.h"
#include "clogger/clogger.h"
#include "utils.h"           /* clear_stdin */
#include "ccontainer/clist_cstring.h" /* to delete ?*/
#include "ccontainer/cvector_cstring.h"

/* no more use ?
enum { STREAM_INPUT_MAX_SIZE = 25 };
*/

/* global variable, but static no access from other compile unit */
static irecord_t *record;
static game_loader_t *gloader;
/*static plugin_mgr_t *manager_plg; */ /* got singleton instance on demand */

/***
 *  private functions declaration 
 ***/
/** identifaction part, try to split logic/view */
static joueur_t identification_joueur();
static void print_identification_menu(char *try_name);
/*** Menu game boucle sur le choix du jeu */ /* why cannot pass const clist ? */
static int print_game_menu(cvector_cstring_t *vect_game_name);
/*** Running a game, take out const  ***/
int run_game(joueur_t* joueur, const char *name_game);

/********* Implementation ************/
int arcade_init( game_loader_t *game_loader, irecord_t* record_impl)
{
    gloader = game_loader;
    record = record_impl;
    CLOG_DEBUG("arcade_init\nplugin directory: %s\n",
        plugin_manager_get_directory( plugin_manager_get_instance()));
    CLOG_DEBUG("game_loader nb of games(todo) %d\n",1);
    return 0;
}
/* to decide if takes ownership of injected variables. No ownership now, inject reference, better
    Not at this point, method not called (created and deleted from main) */
void arcade_clear() 
{
    CLOG_DEBUG("arcade::arcade_clear() %d\n", 0);
    record = NULL;
    gloader = NULL;
}

/* main loop */
int arcade_run(arcade_params_t *params)
{
    /* structure fully defined in header, can allocate on stack */
    joueur_t joueur;

    /* for game choice */
    cvector_cstring_t vector_game_names;
    const char *name_game_out = NULL;
    ccontainer_err_t err_code;
    int game_indice = -1;

    bool play_anonymous = params->play_anonymous;
    bool exit_loop = false;

    CLOG_DEBUG("arcde::run_game() %d\n",0);
    printf("arcade_run() ");

    /*** 1. Identification ***/
    if( play_anonymous) {
        joueur_init( &joueur, "anonymous", false, false);
    } else {
        joueur = identification_joueur();
    }
    /* check if joueur is valid , new joueur case deal before  */
    assert( person_status((person_t *)&joueur) == PERSON_VALID);
    joueur_info( &joueur );
    
    /*** 2. Game menu ***/
    /* error_code, is ok also, error code as attribute ok cvector_string ? */
    vector_game_names = game_loader_get_names( gloader );
    /* at least the two static games are present */
    assert( cvector_cstring_size( &vector_game_names) >= 2 );

    /* infinite main loop menu */
    do {
        /* usage for printing menu (and starting the game)
           get back the choice of user */
        game_indice = print_game_menu( &vector_game_names );
#ifdef JMMJ_DEBUG
        if( game_indice >= 0) {
            CLOG_DEBUG("game_indice: %d\n",game_indice);
            name_game_out = cvector_cstring_get_ref_at( &vector_game_names, game_indice, &err_code );

            printf("game name chosen : %s\n", name_game_out );
            CLOG_DEBUG("game name chosen: %s\n", name_game_out );
        }
#endif
        /* Send scoring page : joueur, record */
        /* could use enum (negative) or game number positive and switch statement ... 
            better, more explicit */
        if( game_indice == -1 ) 
        {
            printf("\n------\nVoir les scores\n-----\n");
        }
        /* exit game properly, function end_game to choose / with error also */
        else if( game_indice == -2 ) 
        {
            printf("\ngood bye %s!\n", person_name( (person_t*) &joueur));
            exit_loop = true;
        }
        /* game_indice [0, size_list_game-1] (assert inside function) */
        else 
        {
            assert( game_indice >= 0);
            assert( game_indice < (int)  cvector_cstring_size( &vector_game_names));
            /* get the game and run it */
            name_game_out = cvector_cstring_get_ref_at( &vector_game_names, game_indice, &err_code );
            run_game(&joueur, name_game_out);
            /* store result in record */
        }

    } while( exit_loop == false );

    /** 3. exit game **/
    cvector_cstring_delete( &vector_game_names );
    joueur_delete( &joueur );
    return 0;
}

/* person is enought ? take out const */
int run_game(joueur_t* joueur, const char *name_game) 
{
    victory_t victory_game;
    struct pair_game_victory_t pair_vict_rec;

    person_t person;
    ptr_game_t pf_game;
    
    /*pf_game = game_loader_get_ptr_game( game_indice );*/
    pf_game = game_loader_get_ptr_game( gloader, name_game );
    if( !pf_game ) {
        CLOG_ERR("Error in loading game name: %s\n", name_game);
    }
    
    /* above , not initialized pname, want to avoid setter */
    /* person is a joueur, copy good for game execution if new thread */
    person_init( &person, person_name( (person_t*) &joueur),
                          person_daltonien( (person_t*) &joueur),
                          person_admin( (person_t*) &joueur));

    /* game execution */
    victory_game = pf_game(person);
    printf("result win %d\n", victory_game.nb_win);
    /* Want to save the results in record ? */

    /* update results of victories, use only copies */
    /* pair maybe too much */
    pair_vict_rec = game_victories_get_copy( &joueur->map_victories, "mastermind" );
    printf("pair vict rec before update %d", pair_vict_rec.victories.nb_win);

    pair_vict_rec.victories.nb_win += victory_game.nb_win;
    game_victories_insert( &joueur->map_victories, &pair_vict_rec);
    return 0;
}

static joueur_t identification_joueur()
{
    joueur_t joueur;
    /* +1/-1 to check */
    char try_name[MAX_SIZE_NOM_PERSON];
    bool accepted = false;
    bool new_joueur_confirmation = false;

    /* do... while important, avoid potentialy initialized variable
        => may change interface, passing joueur_t as argument... or other trick */
    do {
        print_identification_menu(try_name);

        joueur = record_find_joueur_from_name((irecord_t *)record, try_name);
        printf("-- In arcade --\n");
        printf("joueur name: %s\n", person_name( (person_t*) &joueur ) /*.person.nom*/);
        printf("daltonien: %d\n", joueur.person.is_daltonien);
        printf("adresss of joueur: %p\n", (void*) &joueur);
        /* print the name because frist in structure !! same adreess !! */
        printf("name : %s\n", person_name((person_t*) &joueur));

        if( person_status((person_t*) &joueur) == PERSON_INVALID) {
            CLOG_DEBUG("INVALID PERSON %d\n",0);
            /*printf("Invalid joueur you are a new joueur, TODO to confirm !!\n");*/
            /* clean, menu for a new joueur: make its profile */
            new_joueur_confirmation = ask_yesno_question("Nouveau joueur ? confirm by y/n plz: ");
            if( new_joueur_confirmation) {
                /* delete previous invalid joueur, reset it is a person indeed 
                    or need to set argument of new joueur setName, dalto... */
                joueur_delete(&joueur);
                printf("NEW JOUEUR INIT\n");
                CLOG_DEBUG("clear previous joueur and init a new one with name %s\n", try_name);
                joueur_init(&joueur, try_name, false, false);
                accepted = true;
            }
        /* no printf in this logic function */
        /* joueur found in record */
        } else {
            printf("Welcome back %s\n", person_name( (person_t*) &joueur) );
            /* funct_welcome_back()  or deal with it on higher-level ...*/
            accepted = true;
        }

    } while(accepted == false);

    return joueur;
}

static void print_identification_menu(char *try_name) 
{
    /*char p_nom_joueur[50];*/
    printf("\n---- Identifaction ----\n");
    printf("Quel est votre nom ? ");
    /* fgets more secure with size and add automatically a '\0' caracter */
    if( fgets( try_name, MAX_SIZE_NOM_PERSON, stdin) == NULL) {
        CLOG_ERR("Error in fgets, not treated %d", 0);
    }
    /* case where \n is not present to treat (longer than MAX_SIZE)
        https://stackoverflow.com/questions/2693776/removing-trailing-newline-character-from-fgets-input */
    try_name[strcspn(try_name, "\n")] = 0; /* <=> '\n' => '\0' */
    printf("input name: %s, %ld\n", try_name, (long)strlen(try_name));
}

/********** View part of the game ****************************/

/* \brief Print the main menu.
     return int : not so nice, an enum not so simple with variable nb of games 
       or enum { QUIT=-2, SCORE=-1 } and test for positive , use of switch */
/*int print_game_menu(const char**list_game_name, size_t size_list)*/
static int print_game_menu(cvector_cstring_t *vect_game_name)
{
    int i, choice_game, nb_game;
    const char *name_out = NULL;
    ccontainer_err_t err_code;
    bool valid_entry = false;
    /* allow to deal more easily with integer inside the function */
    nb_game = (int) cvector_cstring_size( vect_game_name );
    assert( nb_game < INT_MAX );

    printf("---------------\nListe des jeux ------\n");
    for( i = 0; i < nb_game; i++) {
        name_out = cvector_cstring_get_ref_at( vect_game_name, i, &err_code);
        printf("%d. %s\n", i, name_out);
    }
    printf("%d. Un jeu au hasard\n", i++);
    printf("%d. Voir les scores\n", i++);
    printf("%d. Quittez le menu et le jeu\n", i);

    do {
        printf("Votre choix : ");

        if( scanf("%d", &choice_game) == EOF) {
            CLOG_ERR("Error in scanf, EOF not treated %d", EOF);
        };

        clean_stdin();
        /* check entry is valid */
        if( choice_game >= 0 && choice_game < nb_game ) {
            valid_entry = true;
        }
        /* before before last, a random game */
        if( choice_game == nb_game) {
            valid_entry = true;
            choice_game = rand() % nb_game; /* [0,nb_game-1] */
        }
        /* before last, see the score */ /* DO NOT FIT well here ? */
        if( choice_game == i - 1) {
            valid_entry = true;
            choice_game = -1;
        }
        /* choose to quit the game, always last value of i */
        if( choice_game == i ) {
            valid_entry = true;
            choice_game = -2;
        }
    /* could return directly from if statement rather than to set a boolean flag...*/
    } while( valid_entry == false );

    return choice_game;
}


