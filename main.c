#include <stdio.h>
#include <stdlib.h>

#include "historique.h"
#include "game_pendu.h"
#include "game_mastermind.h"

int main()
{
    printf("Hello world!\n");

    // demander le nom du joueur
    // rechercher dans son historique
    Historique historique;
    initialize_historique( &historique );
    Joueur joueur = { "Toto" };
    //joueur.nom = "Toto";
    //char nom[50] = "Toto";
    historique.nbre_victoire_total = 10;

    int choice_game;            // choix du jeu

        // si present on recherche son historique
        // si premiere connexion on cree un nouveau

 //1   // Proposer le style de jeu :

        // 3 a la suite
        // 1 au choix
            // Proposer la liste de jeu
        // 1 au hasard

// 1 ou 3 jeux
    // si 1 jeu
    printf("A quel jeu voulez-lous jouer ?\n");
    printf("1. jeu du pendu\n");
    printf("2. une partie de Mastermind\n");
    printf("3. une bataille de morpion\n");
    scanf("%d", &choice_game);

    printf("Vous avez choisi %d \n", choice_game);

    switch( choice_game ) {
        case 1 : start_game_pendu( joueur, historique );
                 break;
        case 2 : start_game_mastermind( joueur, historique );
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
