
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <assert.h>

/*#include <dlfcn.h>*/
/* https://stackoverflow.com/questions/53530566/loading-dll-in-windows-c-for-cross-platform-design nice idea */
/*#include <windows.h> moved to game_loader */ 

#include "historique.h"
#include "joueur.h"

/* storage of available game + in charge of loading dll (composition) */
#include "game_loader.h"
#include "plugin_manager.h"

/* hard-coded in base code */
/* #include "game_mastermind.h" */
/* link shared library at compile-time */
/* #include "game_pendu.h" */
/* link shared library at run-time, plugin
#include "morpion.h"
*/
#include "clogger.h"
#include "utils.h"

/********** define global et types **********/
/* pointer to function type to run the game */
/* typedef int(*ptr_game_t)(Joueur, struct historique_t*); */

/** Demande le nom du joueur, recherche dans le fichier historique
  si present on charge son historique dans historique
  si absent on en cree un nouveau qu sera sauvegardé en fin de partie
  Historique alloué dynamiquement dans la fonction **/
/* void set_joueur_and_historique(Joueur *joueur, Historique **historique, bool *new_joueur); */
/** Lance un jeu de chaque à la suite **/
/* int serie_3_game(); */

ptr_game_t loadGame();
/** Choisi un jeu au hasard
  ptr_game_t is a pointer **/
ptr_game_t jeu_au_hasard();
/* boucle sur le choix du jeu */
int print_game_menu(const char**list_game_name, size_t nb_game);
int run_game(const Joueur* joueur, int game_indice);

void clear_ressources_and_exit( struct historique_t *histo );

void choix_jeu(const Joueur joueur, struct historique_t *histo);

/** print preprocessor directives */
void log_directives();

int main()
{
    /**********
     * Declaration of local variables 
     * *********/
    /* Encapsulation for historique, even attributes are not exposed in its header */
    struct historique_t *historique = NULL;
    /* structure fully defined in header, can allocate on stack */
    Joueur joueur;

    plugin_mgr_t *manager;

    /*ptr_game_t pf_game = NULL;*/
    /* for game choice */
    char ** list_game_name = NULL;
    size_t size_list_game = 0;
    int game_indice = -1;
    int retour;
    /* bool b_new_joueur; */

    /* initialize the logger */
    init_clogger("jmmjouer.log");

    /* log preprocessor directives */
    /* clear_screen(); */
    printf("\n---------------------------------\n");
    log_directives();
    printf("\n---------------------------------\n");
    
    printf("\n\nJMMjouer vous accueille a son salon d'arcade\n");
    /* warning: universal character names are only valid in C++ and C99 */
    /* printf("Un choix de jeux un peu ..\U00001F47E.. de réflexion \U0001F9D0 \n\n"); */
    /* initalize pour random on peut l'appeler d'ici pour tous les jeux */
    srand(time(NULL));
    
    /* initialisation of variables */
    historique = historique_new();
    if( historique == NULL ) {
        CLOG_ERR("Error allocation\n", 0);
        return 1;
    }
    historique_init( historique );
    
    retour = joueur_init( &joueur, "toto", true);
    if( retour ) {
        CLOG_ERR("Error initialisation joueur\n",0);
        /*return 1;*/
        abort(); /* to check in C terminate, clean ending */
    }

    /* load joueur et son historique si dispo in  file.txt */
#ifdef JMMJ_DEBUG
    printf("adress of historique in main %p \n", (void *)historique);
    printf("adress of &historique in main %p \n", (void *)&historique);
#endif
    /*
    set_joueur_and_historique( &joueur, &historique, &b_new_joueur); // pass pointer to pointer of historique
                                                                     //want to assign local historique to the allocated memory inside the function
    if( b_new_joueur ) {
        printf("Ah ! un nouveau venu !! \n");
        printf("Quelques questions pour mieux voir connaitre  \n");
        joueur.is_daltonien = ask_yesno_question("Etes-vous daltonien ? [Y/y/n/N] > ");
    } else {
        printf("Cela fait plaisir de vous revoir %s \n", joueur.nom);
        print_info_joueur( &joueur );
        print_historique( historique );
        printf("\nCommençons dès maintenant \n\n");
    }
    choix_jeu(joueur, historique);
    */

    /* game loader as singleton, does not need pointer */
    /* load all static games + games in shared libraies in plugins directory */
    /*retour = game_loader_load_all("plugins");*/

    manager = plugin_manager_new();
    plugin_manager_init( manager, "plugins");
    game_loader_init( manager );
    retour = game_loader_load_all();
    if( retour) {
        printf("Error in loading games\n");
        exit(1);
    }
    
    printf("After load_all_games : ret = %d\n", retour);

    retour = game_loader_get_names(&list_game_name, &size_list_game);
    /* potentially not portable , erroneous with large number, z modifier , %zu not avaialble in C89
       https://stackoverflow.com/questions/2524611/how-can-one-print-a-size-t-variable-portably-using-the-printf-family*/ 
    printf("ret %d, size list_game_name %lu\n", retour, (unsigned long)size_list_game);

    /* infinite main loop menu */
    do {
        /* usage for printing menu (and starting the game)
           get back the choice of user */
        game_indice = print_game_menu((const char**)list_game_name, size_list_game);
        CLOG_DEBUG("game_indice: %d\n",game_indice);
        /* Send scoring page : joueur, record */
        /* could use enum (negative) or game number positive and switch statement ... 
            better, more explicit */
        if( game_indice == -1 ) {
            printf("\n------\nVoir les scores\n-----\n");
        }
        /* exit game properly, function end_game to choose / with error also */
        else if( game_indice == -2 ) {
            printf("\ngood bye %s!\n",joueur.nom);
            clear_ressources_and_exit(historique); /* not clean at all !*/
        }
        /* game_indice [0, size_list_game-1] (assert inside function) */
        else {
            assert( game_indice >= 0);
            assert( game_indice < (int)size_list_game);
            /* get the game and run it */
            run_game(&joueur, game_indice);
            /* store result in record */
        }

    } while( true );
    
    /* nice, not possible (or easy) to compile ! */
    /* is_empty(); */

    /*
    // sauvegarder historique : // si fichier global, relire et reecrire tous les historiques
                                // si fichier individuel, juste le fichier historique_nom.txt a écrire
    */

    /* unload games and deallocation of memory on heap */
    clear_ressources_and_exit( historique );
    return 0;
}

/* joueur on stack, game_loader global
   see set_terminate() */
void clear_ressources_and_exit( struct historique_t *histo )
{
    /* historique_t allocated on heap */
    if( histo ) {
        historique_del( histo );
        histo = NULL;
    }
    /* global variable, reset it */
    game_loader_clear();
    close_clogger();
    exit(0);
}

int run_game(const Joueur* joueur, int game_indice) 
{
    ptr_game_t pf_game;
    assert( game_indice >= 0);
    pf_game = game_loader_get_ptr_game( game_indice );
    if( !pf_game ) {
        CLOG_ERR("Error in loading game indice %d \n", game_indice);
    }
    /* execute game, sending a copy of the joueur
       and get results */
    pf_game(*joueur);
    /* return structure with results */
    return 0;
}


/*
// if we want to allocate memory in historique
// 1. pass a pointer to a pointer to Historique, implement this
// 2. return a valid adress to assign
//void set_joueur_and_historique(Joueur *joueur, Historique *historique, bool *new_joueur)
void set_joueur_and_historique(Joueur *joueur, Historique **historique, bool *new_joueur)
{
    //printf("historique ** passe en copie %p", historique);
    // we can now assign to the correct address
    *historique = malloc( sizeof(Historique) );
    initialize_historique( *historique );

#ifdef DEBUG_CODE
    printf("print **historique passe en copie %p \n", (void *)historique); //with Historique * histo print 'nil' null pointer. lost access to original pointer in main
                                                           //with Historique ** histo print the same adress than historique in main
    printf("*historique = %p pointe to to new allocated memory \n", (void *)*historique);  // cast to avoid warning ??
#endif

    char p_nom_joueur[50];
    printf("Quel est votre nom ? ");
    fgets( p_nom_joueur, 50, stdin);
    // really needed ?
    p_nom_joueur[strcspn(p_nom_joueur, "\n")] = 0; // <=> '\0'
    strcpy( joueur->nom, p_nom_joueur );
    joueur->serie_3_game = false;
    joueur->is_daltonien = false;

   // load historique avec le nom du joueur
    *new_joueur = search_joueur_in_historique( *historique, joueur );
}
*/
/*
int serie_3_game(Joueur joueur, Historique* historique)
{
    start_game_pendu(joueur, historique);
    start_game_mastermind(joueur, historique);
    lancer_morpion(joueur, historique);
    return 0;
}
*/

/********** View part of the game ****************************/

/* \brief Print the main menu.
     return int : not so nice, an enum not so simple with variable nb of games 
       or enum { QUIT=-2, SCORE=-1 } and test for positive , use of switch */
int print_game_menu(const char**list_game_name, size_t size_list)
{
    int i, choice_game, nb_game;
    bool valid_entry = false;
    /* allow to deal more easily with integer inside the function */
    assert( nb_game < INT_MAX );
    nb_game = (int) size_list;

    printf("---------------\nListe des jeux ------\n");
    for( i = 0; i < nb_game; i++) {
        printf("%d. %s\n", i, list_game_name[i]);
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

void choix_jeu(Joueur joueur, struct historique_t *historique) 
{
    int choice_game;
    bool rejouer = true;

    /* pointer to the selected start_game function */
    ptr_game_t pf_game = NULL;

    do {
        /* si 1 jeu */
        printf("A quel jeu voulez-vous jouer ?\n");
        printf("1. un jeu du pendu\n");
        printf("2. une partie de mastermind\n");
        printf("3. une bataille de morpion\n");
        printf("4. un jeu au hasard\n");
        printf("5. une serie sur tous les jeux\n");
        printf("6. voir votre historique\n");
        printf("7. meilleurs scores enregistrés\n");
        printf("8. quittez\n\tVotre choix : ");
        scanf("%d", &choice_game);
        clean_stdin();

        /* logique propre au jeu
           incrementer historique directement dans le jeu, plus flexible pour les points */
        switch( choice_game ) {
            case 1 : /* pf_game = &start_game_pendu;
                        start_game_pendu( joueur, historique );
                     */
                     printf("Jeu du pendu\n");
                     break;
            case 2 : /* pf_game = &start_game_mastermind; */
                     /* start_game_mastermind( joueur ); */
                     printf("Jeu de mastermind\n");
                     break;
            case 3 : /* pf_game = &lancer_morpion;
                     //lancer_morpion( joueur.nom ); */
                     printf("Jeu du morpion\n");
                     break;
            case 4 : pf_game = jeu_au_hasard();
                     printf("Jeu de hasard\n");
                     break;
            case 5 : pf_game = NULL;
                     /* joueur.serie_3_game = true;
                     pf_game = &serie_3_game; */
                     printf("série de 3 jeux\n");
                     break;
            case 6 : printf("\n%s,", joueur.nom);
                     print_historique(  historique );
                     break;
            case 7 : printf("meilleurs scores to implement\n");
                     pf_game = NULL;
                     break;
            case 8 : pf_game = NULL;
                     rejouer = false;
                     break;
            
            default : printf("Error no corresponding entry !");
                     return;/* NULL; */
        }
        /* test if one game to execute */
        if( pf_game != NULL ) {
            pf_game( joueur ); /* valeur retour sera les resultats */
        }
#ifdef JMMJ_DEBUG
        else { printf("pf_game== NULL"); }
#endif
    /* voulez-vous rejouer/ ou quitter (retour à 1/) */
    } while ( rejouer == true );

    return;
}

ptr_game_t jeu_au_hasard()
{
    /*
    int rdm = rand() % 3;
    rdm++;
    ptr_game_t local_p_game;

    switch( rdm ) {
        case 1 : local_p_game = &start_game_pendu;
                 break;
        case 2 : local_p_game = &start_game_mastermind;
                 break;
        case 3 : local_p_game = &lancer_morpion;
                 break;
        default : printf("Error in jeu_au_hasard()");
                  local_p_game = NULL;
    }
    return local_p_game;
    */
   return NULL;
}

void log_directives()
{
    printf("LOG directives\n");

/* only if windows.h is incuded */
#ifdef _WINDOWS_
	printf("compile avec _WINDOWS_ \n");
#endif
/* defined by compiler, safer. WIN32 and UNIX to test also (see stackoverflow, not sure)
   It is about the platform, not the compiler, gcc on mingw64 is 1 */
#ifdef _WIN32
	printf("compile pour OS Windows\n");
    #ifdef _WIN64
        printf("Architecture 64 Bits\n");
    #else
        printf("Architecture 32 bits\n");
    #endif
#endif
/* VER of MSVC see _MSS_VER_LONG..
   MSVC compiler
   _DEBUG Defined as 1 when the /LDd, /MDd, or /MTd compiler option is set. Otherwise, undefined.*/
#ifdef _MSC_VER
	printf("compile avec MSVC compiler\n");
    #if defined(_DEBUG)
        printf("MSVC in debug mode\n");
    #else 
        printf("MSVC in release mode\n");
    #endif
#elif defined(__GNUC__) || defined(__GNUG__)
/* #elif __GNUC__ */
    printf("compile with GCC compiler\n");
    #ifdef NDEBUG
        printf("GCC in release mode\n");
    #else
        printf("GCC in debug mode\n");
    #endif
#endif

#ifdef JMMJ_DEBUG 
    printf("DIRECTIVE JMMJ_DEBUG defined\n");
#endif

}