#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <assert.h>

/* order of include important */
#include "arcade_params.h"
#include "arcade.h"
#include "joueur.h"         /* person included */
#include "victory.h"
#include "irecord.h"
#include "game_loader.h"
#include "plugin_manager.h"
#include "clogger.h"
#include "utils.h"           /* clear_stdin */
#include "clist_cstring.h"

/* no more use ?
enum { STREAM_INPUT_MAX_SIZE = 25 };
*/

/* global variable, but static no access from other compile unit */
static irecord_t *record;
static game_loader_t *gloader;
/*static plugin_mgr_t *manager_plg; */ /* got singleton instanc eon demand */

/***
 *  private functions declaration 
 ***/
/** identifaction part, try to split logic/view */
static joueur_t identification_joueur();
static void print_identification_menu(char *try_name);
/*** Menu game boucle sur le choix du jeu */ /* why cannot pass const clist ? */
static int print_game_menu(clist_cstring_t *clist_game_name);
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
/* to decide if takes ownership of injected variables.
    Not at this point */
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
    clist_cstring_t *clist_game_name = NULL;
    char *name_game_out = NULL;
    int game_indice = -1;

    bool play_anonymous = params->play_anonymous;

    CLOG_DEBUG("arcde::run_game() %d\n",0);
    printf("arcade_run() ");

    /*** 1. Identification ***/
    if( play_anonymous) {
        joueur_init( &joueur, "anonymous", false);
    } else {
        printf("Must implement identification page, try with toto\n");
        joueur = identification_joueur();
    }
    /* joueur new or not ? Not important at this point, 
        just need to assert it is not invalid for continuing !! */
    /* check if joueur is valid , even if done before ... ? */
    assert( person_is_valid((person_t *)&joueur) == true);
    joueur_info( &joueur );
    /*
    if( strcmp( person_name((person_t*)&joueur),  "invalid" ) == 0) { 
        printf("Error initialisation joueur name_to_write %d\n",0);
        CLOG_ERR("Error initialisation joueur name_to_write %d\n",0);
        return 1;
        abort(); to check in C terminate, clean ending
        return EXIT_FAILURE;
    }
    */

    /*** 2. Game menu ***/
    /* error_code, is ok also */
    clist_game_name = game_loader_get_names( gloader );
    assert(clist_game_name);

    /* infinite main loop menu */
    do {
        /* usage for printing menu (and starting the game)
           get back the choice of user */
        game_indice = print_game_menu(clist_game_name);

        /* if choose quit directly, big error */
        if( game_indice >= 0) {
            CLOG_DEBUG("game_indice: %d\n",game_indice);
            /* ok not convenient if not in return */
            clist_cstring_get_ref( clist_game_name, game_indice, &name_game_out );
            printf("game name: %s\n", name_game_out );
            CLOG_DEBUG("game name: %s\n", name_game_out );
        }
        /* Send scoring page : joueur, record */
        /* could use enum (negative) or game number positive and switch statement ... 
            better, more explicit */
        if( game_indice == -1 ) {
            printf("\n------\nVoir les scores\n-----\n");
        }
        /* exit game properly, function end_game to choose / with error also */
        else if( game_indice == -2 ) {
            printf("\ngood bye %s!\n",joueur.person.nom);
            return 0;
            /*
            clear_ressources_and_exit(historique); */ /* not clean at all !*/
        }
        /* game_indice [0, size_list_game-1] (assert inside function) */
        else {
            assert( game_indice >= 0);
            assert( game_indice < (int)  clist_cstring_size(clist_game_name));
            /* get the game and run it */
            clist_cstring_get_ref( clist_game_name, game_indice, &name_game_out );
            run_game(&joueur, name_game_out);

            /* store result in record */
        }

    } while( true );

    /** 3. exit game **/
    clist_cstring_delete( clist_game_name );
    clist_game_name = NULL;
    joueur_clear( &joueur );
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
    /* execute game, sending a copy of the joueur
       and get results */
    /*victories = pf_game((person_t)*joueur); cast not allowed*/
    /* return structure with results */
    strcpy(person.nom, joueur->person.nom);
    person.is_daltonien = joueur->person.is_daltonien;

    /* game execution */
    victory_game = pf_game(person);
    printf("result win mastermind %d\n", victory_game.nb_win);
    /* Want to save the results in record ? */

    /* update results of victories, use only copies */
    /* pair maybe too much */
    pair_vict_rec = game_victories_get_copy( &joueur->map_victories, "mastermind" );
    printf("pair vict rec before update %d", pair_vict_rec.victories.nb_win);

    pair_vict_rec.victories.nb_win += victory_game.nb_win;
    game_victories_insert( &joueur->map_victories, pair_vict_rec);

    return 0;
}

static joueur_t identification_joueur()
{
    joueur_t joueur;
    /* +1/-1 to check */
    char try_name[MAX_SIZE_NOM_PERSON];
    bool accepted = false;
    bool new_joueur_confirmation = false;

    joueur_default_init( &joueur );

    while( accepted == false) {
        print_identification_menu(try_name);

        joueur = record_find_joueur_from_name((irecord_t *)record, try_name);
        printf("-- In arcade --\n");
        printf("joueur name: %s\n", joueur.person.nom);
        printf("daltonien: %d\n", joueur.person.is_daltonien);
        printf("adresss of joueur: %p\n", (void*) &joueur);
        /* print the name because frist in structure !! same adreess !! */
        printf("name : %s\n", (char*) &joueur);

        /*if( person_name((person_t*)joueur) == "invalid") {*/
        if( person_is_valid((person_t*)&joueur) == false) {
            CLOG_DEBUG("INVALID PERSON %d\n",0);
            printf("Invalid joueur it is a new joueur, TODO to confirm !!\n");
            /* clean, menu for a new joueur: make its profile */
            new_joueur_confirmation = ask_yesno_question("Nouveau joueur ? confirm by y/n plz: ");
            if( new_joueur_confirmation) {
                joueur_init(&joueur, try_name, false);
                accepted = true;
            }
        /* no printf in this logic function */
        /* joueur found in record */
        } else {
            printf("Welcome back %s\n", joueur.person.nom);
            /* funct_welcome_back()  or deal with it on higher-level ...*/
            accepted = true;
        }
    }
    return joueur;
}

static void print_identification_menu(char *try_name) 
{
    /*char p_nom_joueur[50];*/
    printf("\n---- Identifaction ----\n");
    printf("Quel est votre nom ? ");
    /* fgets more secure with size and add automatically a '\0' caracter */
    fgets( try_name, MAX_SIZE_NOM_PERSON, stdin);
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
static int print_game_menu(clist_cstring_t *clist_game_name)
{
    int i, choice_game, nb_game;
    char *name_out = NULL;
    bool valid_entry = false;
    /* allow to deal more easily with integer inside the function */
    nb_game = (int) clist_cstring_size( clist_game_name);
    assert( nb_game < INT_MAX );

    printf("---------------\nListe des jeux ------\n");
    for( i = 0; i < nb_game; i++) {
        clist_cstring_get_ref( clist_game_name, i, &name_out);
        printf("%d. %s\n", i, name_out);
    }
    printf("%d. Un jeu au hasard\n", i++);
    printf("%d. Voir les scores\n", i++);
    printf("%d. Quittez le menu et le jeu\n", i);

    do {
        printf("Votre choix : ");
        scanf("%d", &choice_game);
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


