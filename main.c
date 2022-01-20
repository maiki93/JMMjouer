#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

#include "historique.h"
#include "game_pendu.h"
#include "game_mastermind.h"
#include "morpion.h"
#include "utils.h"

/** define global et types **/
// pointer to function type to run the game
typedef int(*PtrGame)(Joueur, Historique*);

/** Demande le nom du joueur, recherche dans le fichier historique
  si present on charge son historique dans historique
  si absent on en cree un nouveau qu sera sauvegardé en fin de partie
  Historique alloué dynamiquement dans la fonction **/
void set_joueur_and_historique(Joueur *joueur, Historique **historique, bool *new_joueur);
/** Lance un jeu de chaque à la suite **/
int serie_3_game();
/** Choisi un jeu au hasard
  PtrGame is a pointer **/
PtrGame jeu_au_hasard();

int main()
{
    clear_screen();
    printf("\n---------------------------------\n");
    printf("JMMjouer \U00002764 vous accueille à son salon d'arcade\n");
    printf("Un choix de jeux un peu ..\U00001F47E.. de réflexion \U0001F9D0 \n\n");
#ifdef DEBUG_CODE
	printf("compilé avec DEBUG_CODE \n");
#endif

    // declaration of variables,
    Historique * historique = NULL;
    Joueur joueur;                    // not initialized at this point
    bool b_new_joueur;                // joueur deja inscrit dans fichier.txt ou non
    // initalize pour random on peut l'appeler d'ici pour tous les jeux
    srand(time(NULL));
    // pointer to function
    PtrGame pf_game = NULL;

    // load joueur et son historique si dispo in  file.txt
#ifdef DEBUG_CODE
    printf("adress of historique in main %p \n", (void *)&historique);
#endif
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
#ifdef DEBUG_CODE
    printf("In main after set_joueur_historique historique = %p \n", (void *)historique); // Fine, point to the dynamic allocated memory
#endif
    // intermediate test
    if( historique == NULL ) {
        printf("ERROR !!");
        return 1;
    }

    // menu principal, choix du jeu ou print info
    int choice_game;
    bool rejouer = true;

    do {
        // si 1 jeu
        printf("A quel jeu voulez-vous jouer ?\n");
        printf("1. un jeu du pendu\n");
        printf("2. une partie de mastermind\n");
        printf("3. une bataille de morpion\n");
        printf("4. un jeu au hasard\n");
        printf("5. une série sur tous les jeux\n");
        printf("6. voir votre historique\n");
        printf("7. meilleurs scores enregistrés\n");
        printf("8. quittez\n\tVotre choix : ");
        scanf("%d", &choice_game);
        clean_stdin();

        // logique propre au jeu
        // incrementer historique directement dans le jeu, plus flexible pour les points
        switch( choice_game ) {
            case 1 : pf_game = &start_game_pendu;
                     //start_game_pendu( joueur, historique );
                     break;
            case 2 : pf_game = &start_game_mastermind;
                     //start_game_mastermind( joueur, historique );
                     break;
            case 3 : pf_game = &lancer_morpion;
                     //lancer_morpion( joueur.nom );
                     break;
            case 4 : pf_game = jeu_au_hasard();
                     break;
            case 5 : pf_game = NULL;
                     joueur.serie_3_game = true;
                     pf_game = &serie_3_game;
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

            default : printf("Error !");
                     return 1;
        }
        // test if one game to execute
        if( pf_game != NULL ) {
            pf_game( joueur, historique ); // valeur retour pas utilisée
        }
#ifdef DEBUG_CODE
        else { printf("pf_game== NULL"); }
#endif
    // voulez-vous rejouer/ ou quitter (retour à 1/)
    } while ( rejouer == true );

    // sauvegarder historique : // si fichier global, relire et reecrire tous les historiques
                                // si fichier individuel, juste le fichier historique_nom.txt a écrire
    // deallocation of memory
    free( historique );
    historique = NULL;
    return 0;
}

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

int serie_3_game(Joueur joueur, Historique* historique)
{
    start_game_pendu(joueur, historique);
    start_game_mastermind(joueur, historique);
    lancer_morpion(joueur, historique);
    return 0;
}

PtrGame jeu_au_hasard()
{
    int rdm = rand() % 3;
    rdm++;
    PtrGame local_p_game;

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
}
