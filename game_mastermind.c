#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "game_mastermind.h"
#include "utils.h"

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

    // initaliza pour random
    srand(time(NULL));

    make_random_secret_code( tab_code_secret, NB_PIECE, colors, sizeof(colors) );
    //
    for(int i = 0; i < NB_PIECE; i++ ) {
        printf("%c", tab_code_secret[i]);
    }
    printf("\n");

    // flag to end the game : victoire or maximum of tentatives
    int victoire = 0;
    int tentatives = 0;
    // resultats du guess
    int nb_bien_place, nb_mal_place;

    // main loop to guess the code with MAX_TENTATIVES essais
    do {
        // re-initialization à chaque tour compteurs et memo array
        nb_bien_place = 0;
        nb_mal_place = 0;
        for(int i=0; i < NB_PIECE; i++) {
            memo_deja_utilise[i] = 0;
        }
        // incremente tentatives
        tentatives++;

        // demander la saisie à l'utilisateur. TODO verify valid entries
        printf("Proposez une combinaison : \n");
        for( int i=0; i < NB_PIECE; i++ ) { // best way found to read a line for the moment
            tab_guess[i] = getchar(); // getchar() <=> getc(stdin)
        }
        clean_stdin(); // works fine

        // afficher tableau joueur pour test
        for( int i = 0; i < NB_PIECE; i++ ) {
            printf("%c", tab_guess[i]);
        }
        printf("\n");

        int autre_piece_modulo = 0;
        // first loop the over guess
        for(int piece = 0; piece < NB_PIECE; piece++ ) {
            // second loop over the secret code, start at piece => check bien place first autre_pice need modulo
            // maintain the priority of bien place
            for( int autre_piece = piece; autre_piece < piece+NB_PIECE; autre_piece++ ) {
                // assure to stay in the range [0,NB-PIECE[
                autre_piece_modulo = autre_piece % NB_PIECE;

                // one match of color and not yet used
                if(  ( tab_guess[piece] == tab_code_secret[autre_piece_modulo] )  // couleurs correspondent
                 &&  ( memo_deja_utilise[ autre_piece_modulo ] == 0 ) ) { // l'autre piece n'a pas déjà été utilisé
                    // on marque les pièces du code secret deja utilisees
                    // bien place a toujours priorite : piece == autre_piece at first iteration
                    memo_deja_utilise[ autre_piece_modulo ] = 1;
                    // increment bien ou mal place
                    ( piece == autre_piece_modulo ) ? nb_bien_place++
                                                    : nb_mal_place++ ;
                    // it is done for this piece
                    break;
                }
            }
        }
        // résultat de ce guess
        printf("Nbre de pieces bien placées : %d\n", nb_bien_place);
        printf("Nbre de pieces mal placées : %d\n", nb_mal_place);

        // check if secret has been found
        if( nb_bien_place == 4 ) {
            victoire = 1;
            break;
        }
    // sort si victoire ou tentative depasse MAX_TENTATIVE
    //} while( (! victoire) && (tentatives <= MAX_TENTATIVE) ); / /not so bad in fact
    } while( !( victoire || (tentatives > MAX_TENTATIVE) ) ); // conditions de sortie plus 'lisible' avec OU ?? bof..

    // message de fin de partie gagné/perdu
    if( victoire ) {
        printf("gagne en %d tentatives sur %d", tentatives, MAX_TENTATIVE);
    } else {
        printf("perdu : nbre de tentatives max atteinte\n");
        printf("Le code était ");
    }

    return 0;
}
