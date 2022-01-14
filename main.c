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

int main()
{
    printf("\nJMMjouer vous accueille à son salon d'arcade \n");
#ifdef DEBUG_CODE
	printf("compilé avec DEBUG_CODE \n");
#endif

    // allocation of variables, default to an empty historique
    Historique * historique = malloc( sizeof(Historique) );
    initialize_historique( historique );

    Joueur joueur; // = { "Toto", false }; // nom Toto, type de jeu: 3_jeux false

    // initalize pour random on peut l'appeler d'ici pour tous les jeux
    srand(time(NULL));

    // set joueur
    char p_nom_joueur[50];
    printf("Quel est votre nom ? ");
    fgets( p_nom_joueur, 50, stdin);
    // really needed ?
    p_nom_joueur[strcspn(p_nom_joueur, "\n")] = 0; // <=> '\0'
    strcpy( joueur.nom, p_nom_joueur );
    joueur.serie_3_game = false;

    //printf("joueur nom: %s \n", joueur.nom);
    //printf("joueur serie 3 games : %d \n", joueur.serie_3_game);

    // rechercher dans son historique, charge son historique
        // si present on recherche son historique
        // si premiere connexion on cree un nouveau
    bool b_new_joueur = search_joueur_in_historique( historique, joueur.nom );
    if( b_new_joueur ) {
        printf("Ah ! un nouveau venu !! \n");
        printf("Quelques questions pour mieux voir connaitre  \n");
        printf("Etes-vous daltonien ? [Y/n] > " );
    } else {
        printf("Cela fait plaisir de vous revoir %s \ncommenèons dès maintenant \n\n", joueur.nom);
    }

    int choice_game;            // choix du jeu
    // int choice_style_game    // suite de 3 jeu, aleatoire...
 //1   // Proposer le style de jeu :

        // 3 a la suite
        // 1 au choix
            // Proposer la liste de jeu
        // 1 au hasard
    if( historique == NULL ) {
        printf("ERROR !!");
        return 1;
    }

// 1 ou 3 jeux
    bool rejouer = true;
    do {
        // si 1 jeu
        printf("A quel jeu voulez-vous jouer ?\n");
        printf("1. jeu du pendu\n");
        printf("2. une partie de Mastermind\n");
        printf("3. une bataille de morpion\n");
        printf("4. voir votre historique\n");
        printf("5. quittez\n\tVotre choix : ");
        scanf("%d", &choice_game);
        clean_stdin();
        //printf("Vous avez choisi %d \n", choice_game);

        // logique propre au jeu
        // incrementer historique directement
        switch( choice_game ) {
            case 1 : start_game_pendu( joueur, historique );
                     break;
            case 2 : start_game_mastermind( joueur, historique );
                     break;
            case 3 : lancer_morpion( joueur.nom );
                     break;
            case 4 : printf("\n%s,", joueur.nom);
                     print_historique(  historique );
                     break;
            case 5 : rejouer = false;
                     break;

            default : printf("Error !");
                     return 1;
        }
    // voulez-vous rejouer/ ou quitter (retour à 1/)
    } while ( rejouer == true );

    // sauvegarder historique : // si fichier global, relire et reecrire tous les historiques
                                // si fichier individuel, juste le fichier historique_nom.txt a écrire

    free( historique );
    historique = NULL;
    // fin
    return 0;
}

