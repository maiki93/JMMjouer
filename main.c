#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "historique.h"
#include "game_pendu.h"
#include "game_mastermind.h"
#include "utils.h"
#include "morpion.h"


int main()
{
    printf("Hello world!\n");

    // initalize pour random on peut l'appeler d'ici pour tous les jeux
    srand(time(NULL));

    // demander le nom du joueur
    // rechercher dans son historique
        // si present on recherche son historique
        // si premiere connexion on cree un nouveau

// Une initialisation temporaire en attendant mieux
    Historique historique;
    initialize_historique( &historique );
    Joueur joueur = { "Toto", false }; // nom Toto, type de jeu: 3_jeux false

    int choice_game;            // choix du jeu
    // int choice_style_game    // suite de 3 jeu, aleatoire...
 //1   // Proposer le style de jeu :

        // 3 a la suite
        // 1 au choix
            // Proposer la liste de jeu
        // 1 au hasard

// 1 ou 3 jeux
    // si 1 jeu
    printf("A quel jeu voulez-vous jouer ?\n");
    printf("1. jeu du pendu\n");
    printf("2. une partie de Mastermind\n");
    printf("3. une bataille de morpion\n");
    scanf("%d", &choice_game);
    clean_stdin();
    printf("Vous avez choisi %d \n", choice_game);

    switch( choice_game ) {
        case 1 : start_game_pendu( joueur, &historique );
                 break;
        case 2 : start_game_mastermind( joueur, &historique );
                 break;
        case 3 : lancer_morpion( joueur, &historique );
                 break;
        default : printf("Error !");
                 return 1;
    }

    // valider entree et lancer le(s) jeu

            // logique propiore au jeu du pendu
            // incrementer historique directement , sauvegarder historiques
            // dans pendu combien de points donner pour une victoire

    // voulez-vous rejouer/ ou quitter (retour à 1/)


    // sauvegarder historique : // si fichier global, relire et reecrire tous les historiques
                                // si fichier individuel, juste le fichier historique_nom.txt a écrire

    // fin
    return 0;
}

