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

/** Demande le nom du joueur, recherche dans le fichier historique
  si present on charge son historique dans historique
  si absent on en cree un nouveau qu sera sauvegardé en fin de partie **/
void set_joueur_and_historique(Joueur *joueur, Historique **historique, bool *new_joueur);

int main()
{
    printf("\nJMMjouer vous accueille à son salon d'arcade \n");
#ifdef DEBUG_CODE
	printf("compilé avec DEBUG_CODE \n");
#endif

    // declaration of variables,
    Historique * historique = NULL;
    Joueur joueur;                    // not initialized at this point
    bool b_new_joueur;                // joueur deja inscrit dans fichier.txt ou non
    // initalize pour random on peut l'appeler d'ici pour tous les jeux
    srand(time(NULL));
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

    int choice_game;            // choix du jeu
    // int choice_style_game    // suite de 3 jeu, aleatoire...
 //1   // Proposer le style de jeu :

        // 3 a la suite
        // 1 au choix
            // Proposer la liste de jeu
        // 1 au hasard

#ifdef DEBUG_CODE
    printf("In main after set_joueur_historique historique = %p \n", (void *)historique); // Fine, point to the dynamic allocated memory
#endif

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
    // deallocation of memory
    free( historique );
    historique = NULL;
    // fin
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

