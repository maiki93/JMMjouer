#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "game_mastermind.h"

void make_random_secret_code( char *secret, const int size_secret, const char* colors, const int size_colors )
{
    // test choix random du computer, "RJVBO"[rand()%5];
    for(int i=0; i < size_secret; i++) {
        secret[i] = colors[ rand() % size_colors ];
    }

}

int start_game_mastermind(Joueur joueur, Historique *historique)
{
    printf("Entrée Mastermind\n");

    printf("Bienvenue ");
    printf("%s \n", joueur.nom);

    // constantes
    const int NB_PIECE = 4;
    const int MAX_TENTATIVE = 10;

    // déclare les tableaux : pieces et internal pour l'alrithme de match
    char colors[] = {'V','R','O','B','J'};        // available colors
    char tab_code_secret[NB_PIECE];
    char tab_guess[NB_PIECE];                    // guess du joueur
    int memo_deja_utilise[NB_PIECE];
    // resultats du guess
    int nb_bien_place, nb_mal_place;
    // initaliza pour random
    srand(time(NULL));

    make_random_secret_code( tab_code_secret, NB_PIECE, colors, sizeof(colors) );

    for(int i = 0; i < NB_PIECE; i++ ) {
        printf("%c", tab_code_secret[i]);
    }
    printf("\n");

    return 0;
}
