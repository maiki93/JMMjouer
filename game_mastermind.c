#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// colors in console
// #include <conio.h>  // color in console, only for windows ?
// other link https://stackoverflow.com/questions/4842424/list-of-ansi-color-escape-sequences
// https://i.stack.imgur.com/9UVnC.png

#include "game_mastermind.h"
#include "utils.h"

/*** functions declaration, definition below ****/
// genere un code secret
void mm_make_random_secret_code( char *secret, const int size_secret, const char* colors, const int size_colors );

void mm_ask_guess_user( char * p_tab_guess, const int size );

void mm_algo_mastermind(char* p_tab_guess, char* p_tab_code_secret, const int size, int *nb_bien_place, int *nb_mal_place );

//  affiche les resultats du jeu de test, colored with grpahics
void mm_affiche_resultat_guess( const int nb_bien_place, const int nb_mal_place );
void mm_affiche_resultat_guess_colored( const int nb_bien_place, const int nb_mal_place );

// affiche les pieces joues par le joueur, colored with graphic shift by SHIFT_DRAW_PIECE
void mm_print_code( char* p_code, const int size );
// version with colring
void mm_print_code_with_colors( char* p_code, const int size );

/*** globale definitions of colors ***/
const int colorCode[][3] = { { (int)'R', 31, 41 },   // rouge, Foreground color, Background color
                             { (int)'V', 32, 42 },   // vert
                              {(int)'J', 33, 43 },   // jaune
                              {(int)'B', 34, 44 },   // bleu
                              {(int)'W', 97, 107}    // white for background
                           };
                            //{(int)'J', 35, 45},   // Magenta

const int SHIFT_DRAW_PIECE = 50; // shift on screen

int start_game_mastermind(Joueur joueur, Historique *historique)
{
    printf("Entrée Mastermind\nBienvenue ");
    printf("%s \n", joueur.nom);

    // constantes
    const int NB_PIECE_MM = 4;
    const int MAX_TENTATIVE = 10;

    // déclare les tableaux : pieces et internal pour l'alrithme de match
    char colors[] = {'V','R','M','B','J'};        // available colors
    char tab_code_secret[NB_PIECE_MM];            // code secret de l'ordinateur
    char tab_guess[NB_PIECE_MM];                  // guess du joueur

    // initaliza pour random
    srand(time(NULL));

    mm_make_random_secret_code( tab_code_secret, NB_PIECE_MM, colors, sizeof(colors) );
    // affiche pour tests
    for(int i = 0; i < NB_PIECE_MM; i++ ) {
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
        // incremente tentatives
        tentatives++;

        mm_ask_guess_user( tab_guess, NB_PIECE_MM );
        // afficher tableau joueur pour test
        // mm_print_code( tab_guess , NB_PIECE_MM );
        mm_print_code_with_colors( tab_guess , NB_PIECE_MM );
//        for( int i = 0; i < NB_PIECE_MM; i++ ) {
//            printf("%c", tab_guess[i]);
//        }
        printf("\n");
        // compute bnombre de ien et mal place
        mm_algo_mastermind( tab_guess, tab_code_secret, NB_PIECE_MM, &nb_bien_place, &nb_mal_place );
        // affiche résultat de cet essai
        mm_affiche_resultat_guess_colored( nb_bien_place, nb_mal_place );

        // check if secret has been found
        if( nb_bien_place == NB_PIECE_MM ) {
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

void mm_make_random_secret_code( char *secret, const int size_secret, const char* colors, const int size_colors )
{
    // test choix random du computer, other trick "RJVBO"[rand()%5];
    for(int i=0; i < size_secret; i++) {
        secret[i] = colors[ rand() % size_colors ];
    }
}

void mm_ask_guess_user( char* p_tab_guess, const int size ) {
    // demander la saisie à l'utilisateur. TODO verify valid entries
    printf("Proposez une combinaison > ");
    for( int i=0; i < size; i++ ) { // best way found to read a line for the moment
        p_tab_guess[i] = getchar(); // getchar() <=> getc(stdin)
    }
    clean_stdin();
}

void mm_algo_mastermind(char* p_tab_guess, char* p_tab_code_secret, const int size_piece, int *nb_bien_place, int *nb_mal_place )
{
    // re-initialization à chaque call
    *nb_bien_place = 0;
    *nb_mal_place = 0;
    // internal use
    int memo_deja_utilise[size_piece];
    for(int i=0; i < size_piece; i++) {
            memo_deja_utilise[i] = 0;
    }

    int autre_piece_modulo = 0;
    // first loop the over guess
    for(int piece = 0; piece < size_piece; piece++ ) {
        // second loop over the secret code, start at piece => check bien place first autre_pice need modulo
        // maintain the priority of bien place
        for( int autre_piece = piece; autre_piece < piece+size_piece; autre_piece++ ) {
            // assure to stay in the range [0,NB-PIECE[
            autre_piece_modulo = autre_piece % size_piece;

            // one match of color and not yet used
            if(  ( p_tab_guess[piece] == p_tab_code_secret[autre_piece_modulo] )  // couleurs correspondent
             &&  ( memo_deja_utilise[ autre_piece_modulo ] == 0 ) ) { // l'autre piece n'a pas déjà été utilisé
                // on marque les pièces du code secret deja utilisees
                // bien place a toujours priorite : piece == autre_piece at first iteration
                memo_deja_utilise[ autre_piece_modulo ] = 1;
                // increment bien ou mal place
                ( piece == autre_piece_modulo ) ? (*nb_bien_place)++
                                                : (*nb_mal_place)++ ;
                // it is done for this piece
                break;
            }
        }
    }
}

void mm_affiche_resultat_guess( const int nb_bien_place, const int nb_mal_place ) {
    printf("Nbre de pieces bien placées : %d\n", nb_bien_place);
    printf("Nbre de pieces mal placées  : %d\n", nb_mal_place);
}

void mm_affiche_resultat_guess_colored( const int nb_bien_place, const int nb_mal_place ) {
//    printf("Nbre de pieces bien placées : %d\n", nb_bien_place);
//    printf("Nbre de pieces mal placées  : %d\n", nb_mal_place);

    char p_string[400];
    //char p_tmp[100];
    for(int i = 0; i < 25; i++ ) {
        p_string[i] = ' ';
    }
    p_string[25] = '\0';
    for(int i = 0; i < nb_bien_place; i++ ) {
        strcat(p_string, "\033[97;1m\033[100m B \033[0m");
    }
    for(int i = 0; i < nb_mal_place; i++ ) {
        strcat(p_string,"\033[30;1m\033[100m M \033[0m");
    }
    printf("résultat : %s\n", p_string);
}

void mm_print_code( char* p_code, const int size )
{
    for( int i = 0; i < size; i++ ) {
        printf("%c", p_code[i]);
    }
    printf("\n");
    printf("\033[31;1;4mHello\033[0m");
}

void mm_get_formatted_string_colored( char code, char* p_out)
{

    switch( code ) {
        case 'R' : strcpy( p_out, "\033[31;1m\033[107m O \033[0m");
                   break;
        case 'V' : strcpy( p_out, "\033[32;1m\033[107m O \033[0m");
                   break;
        case 'J' : strcpy( p_out, "\033[33;1m\033[107m O \033[0m");
                   break;
        case 'B' : strcpy( p_out, "\033[34;1m\033[107m O \033[0m");
                   break;
        case 'M' : strcpy( p_out, "\033[35;1m\033[107m O \033[0m");
                   break;
        default  : strcpy( p_out, "?");
    }

}

void mm_print_code_with_colors( char* p_code, const int size )
{
    char p_string[400];
    char p_tmp[100];
    for(int i = 0; i < SHIFT_DRAW_PIECE; i++ ) {
        p_string[i] = ' ';
    }
    p_string[SHIFT_DRAW_PIECE] = '\0';

    // for first indice
    mm_get_formatted_string_colored( p_code[0], p_tmp );
    strcat( p_string, p_tmp );
    // other
    for( int i = 1; i < size; i++ ) {
        mm_get_formatted_string_colored( p_code[i], p_tmp );
        strcat( p_string, p_tmp );
    }
    printf("%s\n", p_string);
}
