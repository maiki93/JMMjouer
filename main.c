#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "historique.h"
#include "game_pendu.h"
#include "game_mastermind.h"
#include "utils.h"
#include "morpion.h"


int main()
{
    printf("Hello world!\n");

    // Une initialisation temporaire en attendant mieux
    Historique* historique = NULL;
    historique = malloc( sizeof(Historique) );
    //initialize_historique( &historique );
    Joueur joueur = { "Toto", false }; // nom Toto, type de jeu: 3_jeux false
    // variable
    char p_nom_joueur[50];
    // initalize pour random on peut l'appeler d'ici pour tous les jeux
    srand(time(NULL));

    // demander le nom du joueur
    printf("Quel est votre nom ? ");
    fgets( p_nom_joueur, 50, stdin);
    //clean_stdin();
    //if( strcspn(p_nom_joueur, "\n") == NB_LETTRE ) {
    //clean_stdin(); // besoin si mot > NB_LETTER, petit bug sinon
    //}
    p_nom_joueur[strcspn(p_nom_joueur, "\n")] = 0; // <=> '\0'
    printf("Welcome !%s!\n", p_nom_joueur);
    strcpy( joueur.nom, p_nom_joueur );
    joueur.serie_3_game = false;

    //printf("joueur nom: %s \n", joueur.nom);
    //printf("joueur serie 3 games : %d \n", joueur.serie_3_game);

    // rechercher dans son historique, charge son historique
        search_joueur_in_historique( historique, joueur.nom );
        // si present on recherche son historique
        // si premiere connexion on cree un nouveau



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
    // si 1 jeu
    printf("A quel jeu voulez-vous jouer ?\n");
    printf("1. jeu du pendu\n");
    printf("2. une partie de Mastermind\n");
    printf("3. une bataille de morpion\n");
    printf("4. voir votre historique\n");
    scanf("%d", &choice_game);
    clean_stdin();
    printf("Vous avez choisi %d \n", choice_game);

    switch( choice_game ) {
        case 1 : start_game_pendu( joueur, historique );
                 break;
        case 2 : start_game_mastermind( joueur, historique );
                 break;
        case 3 : lancer_morpion( joueur, &historique );
                 break;
        case 4 : printf("Vos performances : %s", joueur.nom);
                 print_historique(  historique );
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

