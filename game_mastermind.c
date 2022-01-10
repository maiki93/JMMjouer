#include <stdio.h>
#include <stdlib.h>

#include "game_mastermind.h"

int start_game_mastermind(Joueur joueur, Historique *historique)
{
    printf("Entrée Mastermind\n");

    printf("Bienvenue ");
    printf("%s \n", joueur.nom);

    // constantes
    const int NB_PIECE = 4;
    const int MAX_TENTATIVE = 10;

    // déclare les tableaux : pieces et internal pour l'alrithme de match
    char colors[] = {'V','R','V','B','J'};        // available colors
    char tab_code_secret[NB_PIECE];
    char tab_guess[NB_PIECE];                    // guess du joueur
    int memo_deja_utilise[NB_PIECE];



    return 0;
}
