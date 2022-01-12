#include <stdio.h>
#include <stdlib.h>
// #include <time.h>
#include <string.h>
#include <stdbool.h>

// colors in console
// #include <conio.h>  // color in console, only for windows ?
// other link https://stackoverflow.com/questions/4842424/list-of-ansi-color-escape-sequences
// https://i.stack.imgur.com/9UVnC.png

#include "game_mastermind.h"
#include "utils.h"

/*** functions declaration, definition below ****/

/** Toute la logique pour une partie de mastermind

    return int vainqueur : 1-le joueur, 2-l'ordinateur
**/
int mm_make_one_game( const int NbPIECE, const int MaxTENTATIVE );
/** genere un code secret aléatoire **/
void mm_make_random_secret_code( char *secret, const int size_secret, const char* colors, const int size_colors );
/** demande une proposition du code à deviner au joueur **/
void mm_ask_guess_user( char * p_tab_guess, const int size );
/** algorithme pour indiquer bien place et mal **/
void mm_algo_mastermind(char* p_tab_guess, char* p_tab_code_secret, const int size, int *nb_bien_place, int *nb_mal_place );

/**  affiche les resultats du jeu de test, version colored with grpahics **/
void mm_affiche_resultat_guess( const int nb_bien_place, const int nb_mal_place );
void mm_affiche_resultat_guess_colored( const int nb_bien_place, const int nb_mal_place );

/** affiche les pieces joues par le joueur, colored with graphic shift by SHIFT_DRAW_PIECE **/
void mm_print_code( char* p_code, const int size );
void mm_print_code_with_colors( char* p_code, const int size );

/** helper function for colors **/
void mm_get_formatted_string_colored( char code, char* p_out);

/*** globale definitions of colors ***/
// not used, keep  for help
/*
const int colorCode[][3] = { { (int)'R', 31, 41 },   // rouge, Foreground color, Background color
                             { (int)'V', 32, 42 },   // vert
                              {(int)'J', 33, 43 },   // jaune
                              {(int)'B', 34, 44 },   // bleu
                              {(int)'W', 97, 107}    // white for background
                           };
*/

const int SHIFT_DRAW_PIECE = 50; // shift right on screen for printing the pieice
const int SHIFT_RESULTAT_PLACE = 25;

/** main entry **/
int start_game_mastermind(Joueur joueur, Historique *historique)
{
    // message d'intro au jeu
    printf("Entrée Mastermind\nBienvenue %s\n", joueur.nom);
    ( joueur.serie_3_game == true ) ? printf("C'est du sérieux, on est dans une série de 3 jeux\n")
                                    : printf("Mode détente, qq parties pour s'entraîner\n");
    // constantes du jeu
    const int NB_PIECE_MM = 4;
    const int MAX_TENTATIVE = 10;

    // initalization pour random
    //srand(time(NULL));
    // vainqueur de la partie
    int victoire = 0;
    bool rejouer = false;

    do {
        victoire = mm_make_one_game( NB_PIECE_MM, MAX_TENTATIVE );

        if (victoire == 1) {
            printf("on incremente votre historique\n");
            historique->nbre_victoire_mm++;
        } else {
            printf("une defaite de plus %s et enregistrée dans votre historique \n", joueur.nom);
            historique->nbre_defaite_mm++;
        }

        if( joueur.serie_3_game == false ) {
            rejouer = rejouer_une_partie();
        }

    } while ( (rejouer == true) && (joueur.serie_3_game == false) );

    printf("Mastemind vous dit à bientôt %s, revenez vite\n\n", joueur.nom);
    return 0;
}

int mm_make_one_game( const int NbPIECE, const int MaxTENTATIVE ) {
    // déclare les tableaux : pieces et internal pour l'alrithme de match
    char colors[] = {'V','R','M','B','J'};        // available colors
    char tab_code_secret[NbPIECE];            // code secret de l'ordinateur
    char tab_guess[NbPIECE];

    mm_make_random_secret_code( tab_code_secret, NbPIECE, colors, sizeof(colors) );
    // affiche pour tests
    for(int i = 0; i < NbPIECE; i++ ) {
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

        mm_ask_guess_user( tab_guess, NbPIECE );
        // afficher tableau joueur pour test
        // mm_print_code( tab_guess , NbPIECE );
        mm_print_code_with_colors( tab_guess , NbPIECE );
        for( int i = 0; i < NbPIECE; i++ ) {
            printf("%c", tab_guess[i]);
        }
        printf("\n");
        // compute bnombre de ien et mal place
        mm_algo_mastermind( tab_guess, tab_code_secret, NbPIECE, &nb_bien_place, &nb_mal_place );
        // affiche résultat de cet essai
        mm_affiche_resultat_guess_colored( nb_bien_place, nb_mal_place );

        // check if secret has been found
        if( nb_bien_place == NbPIECE ) {
            victoire = 1;
            break;
        }
    // sort si victoire ou tentative depasse MAX_TENTATIVE
    //} while( (! victoire) && (tentatives <= MAX_TENTATIVE) ); / /not so bad in fact
    } while( !( victoire || (tentatives > MaxTENTATIVE) ) ); // conditions de sortie plus 'lisible' avec OU ?? bof..

    // message de fin de partie
    if( victoire == 1 ) {
        printf("Gagné !! en %d tentatives sur %d \n", tentatives, MaxTENTATIVE);
    } else {
        printf("Perdu.. le nombre maximum de tentatives atteint\n");
        printf("Le code secret était : \n");
        mm_print_code_with_colors( tab_code_secret, NbPIECE);
        printf("\n");
    }
    // code retour pour le gagnant
    return (victoire == 1) ?  1
                           :  2;
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
    int at_least_one_output = 0;  // flag, sinon une ligne vide est imprimée
    //char p_tmp[100];
    for(int i = 0; i < SHIFT_RESULTAT_PLACE; i++ ) {
        p_string[i] = ' ';
    }
    p_string[SHIFT_RESULTAT_PLACE ] = '\0';

    // affiche bien place et mal place
    for(int i = 0; i < nb_bien_place; i++ ) {
        strcat(p_string, "\033[97;1m\033[100m B \033[0m");
        at_least_one_output = 1;
    }
    for(int i = 0; i < nb_mal_place; i++ ) {
        strcat(p_string,"\033[30;1m\033[100m M \033[0m");
        at_least_one_output = 1;
    }

    // impression console
    if( at_least_one_output ) {
        printf("%s\n", p_string);
    // mettre un message explicit s'il n'y a rien
    } else {
        printf("%*s nada... \n", SHIFT_RESULTAT_PLACE-1, " ");
    }

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
