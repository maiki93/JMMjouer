#include <stdio.h>
#include <stdlib.h>

#include "historique.h"
#include "game_pendu.h"

int main()
{
    printf("Hello world!\n");

    // demander le nom du joueur
    // rechercher dans son historique
    Historique historique;
    Joueur joueur = { "Toto" };
    //joueur.nom = "Toto";
    //char nom[50] = "Toto";
    historique.nbre_victoire_total = 10;


        // si present on recherche son historique
        // si premiere connexion on cree un nouveau

 //1   // Proposer le style de jeu :
        // 3 a la suite
        // 1 au choix
            // Proposer la liste de jeu
        // 1 au hasard

// 1 ou 3 jeux
    // valider entree et lancer le(s) jeu
    start_game_pendu( joueur, historique );
            // logique propiore au jeu du pendu
            // incrementer historique directement , sauvegarder historiques
            // dans pendu combien de points donner pour une victoire

    // voulez-vous rejouer/ ou quitter (retour à 1/)


    // sauvegarder historique : // si fichier global, relire et reecrire tous les historiques
                                // si fichier individuel, juste le fichier historique_nom.txt a écrire

    // fin

    return 0;
}
