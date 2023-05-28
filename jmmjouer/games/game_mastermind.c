#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

/* colors in console
    #include <conio.h>  // color in console, only for windows ? and very old
    other link https://stackoverflow.com/questions/4842424/list-of-ansi-color-escape-sequences
    https://i.stack.imgur.com/9UVnC.png
*/
#include "jmmjouer/games/game_mastermind.h"
/*#include "victory.h"*/ /* included in .h */
#include "jmmjouer/utils.h"

/* if we want to mock by redefinition,
    must define weak symbol( or use objdump intermediate ) */
#ifdef UNIT_TEST_CMOCKA
#define WEAK_FOR_CMOCKA __attribute__((weak))
#else
#define WEAK_FOR_CMOCKA
#endif

/* enum { MAX_NBPIECE =  6}; */
#define MAX_NBPIECE 6

/*** functions declaration, definition below ****/

/** Toute la logique pour une partie de mastermind
    return int vainqueur : 1-le joueur, 2-l'ordinateur
**/
/* moved to h for test mock */
int mm_make_one_game( const char* colors, const int SizeColor, const int NbPIECE, const int MaxTENTATIVE, const bool mode_daltonien );
/** genere un code secret aléatoire **/
void mm_make_random_secret_code( char *secret, const int size_code, const char* colors, const int SizeColor);
/** demande une proposition du code à deviner au joueur
    return error code : si == 0 ok, si != 0 il y a une erreur
 **/
int mm_ask_guess_user( char * p_tab_guess, const int size_code, const char* colors, const int SizeColor );
/** Algorithme pour indiquer bien place et mal
    if static indicated in declaration only, gcc -Wall is happy, and test cannot access
    if non static, warning can be deleted with -Wno-implicit-function-declaration */
void mm_algo_mastermind(const char* const p_tab_guess, const char* const p_tab_code_secret, const int size, int *nb_bien_place, int *nb_mal_place );

/**  affiche les resultats du jeu de test, version colored with grpahics **/
void mm_affiche_resultat_guess( const int nb_bien_place, const int nb_mal_place, const bool mode_daltonien );
void mm_affiche_resultat_guess_blackwhite( char* p_string, const int nb_bien_place, const int nb_mal_place );
void mm_affiche_resultat_guess_colored( char* p_string, const int nb_bien_place, const int nb_mal_place );

/** affiche les pieces joues par le joueur, colored with graphic shift by SHIFT_DRAW_PIECE **/
void mm_print_code( const char* p_code, const int size_code, const bool mode_daltonien);
void mm_print_code_blackwhite( char* p_string, const char* p_code, const int size_code);
void mm_print_code_with_colors( char* p_string, const char* p_code, const int size_code );

/** helper function for colors, validation... **/
void mm_helper_formatted_string_colored( const char code, char* p_out);
int mm_valid_code_input(const char * const p_tab_guess, const int size_code, const char *colors, const int SizeColor);



/*** globale definitions ***/
/* not used, keep for memo
const int colorCode[][3] = { { (int)'R', 31, 41 },   // rouge, Foreground color, Background color
                             { (int)'V', 32, 42 },   // vert
                              {(int)'J', 33, 43 },   // jaune
                              {(int)'B', 34, 44 },   // bleu
                              {(int)'W', 97, 107}    // white for background
                           }; */
/*  for 'CUI', strange LINE_SIZE available from IDE */
/* const int LINE_SIZE_ON_SCREEN = 300;    longueur de chaine pour l'impression écran, contient aussi ansi code pour les couleurs */
/* const int SHIFT_DRAW_PIECE = 50;        shift right on screen for printing the pieice */
/* const int SHIFT_RESULTAT_PLACE = 35;    for printing results bien / mal place */
#define LINE_SIZE_ON_SCREEN 300         /* longueur de chaine pour l'impression écran, contient aussi ansi code pour les couleurs */
#define SHIFT_DRAW_PIECE 50             /* shift right on screen for printing the pieice */
#define SHIFT_RESULTAT_PLACE 35         /* for printing results bien / mal place */

/** main entry **/
victory_t start_game_mastermind(person_t person)
{
    /* constantes du jeu */
    const int NB_PIECE_MM = 4;
    const int MAX_TENTATIVE = 10;
    const int SIZE_COLOR = 5;
    const char colors[] = {'V','R','M','B','J'};    /* available colors, use sizeof(colors) to know the size.
                                                       ok, only because sizeof(char) == 1 ( or sizeof(colors)/sizeof(char) ) */

    victory_t victories;        /* results of the game(s) */
    int victoire = 0;           /* vainqueur de la partie */
    bool rejouer = false;

    /* init victories */
    victory_init(&victories);

    /* message d'intro au jeu */
    clear_screen();
    printf("Entrée Mastermind\nBienvenue %s\n", person.pname);
    /*
    ( joueur.serie_3_game == true ) ? printf("C'est du sérieux, on est dans une série de 3 jeux\n")
                                    : printf("Mode détente, qq parties pour s'entraîner\n");
    */
    do {
        victoire = mm_make_one_game( colors, SIZE_COLOR, NB_PIECE_MM, MAX_TENTATIVE, person.is_daltonien );
        /* victoire joueur */
        if (victoire == 1) {
            printf("on incremente votre historique\n");
            victories.nb_win++;
        /* victoire == 2 ordinateur */
        } else {
            printf("une defaite de plus %s et enregistrée dans votre historique \n", person.pname);
            victories.nb_lost++;
        }

        /*if( joueur.serie_3_game == false ) {*/
            rejouer = ask_yesno_question("Voulez-vous refaire une partie [y/Y/n/N] > ");
        /*}*/

    } while ( (rejouer == true) /*&& (joueur.serie_3_game == false)*/ );

    printf("Mastemind vous dit à bientôt %s, revenez vite\n\n", person.pname);
    return victories;
}

/* color[] copy de tableau par référence
// pointer char  color, pass the premier , besoin de la taille
// __attribute__((weak))
// simulate as much as possible test_mock working with func1 WEAK_FOR_CMOCKA
// int __attribute__((weak)) mm_make_one_game( const char* colors, const int SizeColor, const int NbPIECE, const int MaxTENTATIVE, const bool mode_daltonien )
*/
int WEAK_FOR_CMOCKA mm_make_one_game( const char* colors, const int SizeColor, const int NbPIECE, const int MaxTENTATIVE, const bool mode_daltonien )
{
    /* déclare les tableaux : pieces et internal pour l'alrithme de match */
    char tab_code_secret[MAX_NBPIECE];            /* code secret de l'ordinateur */
    char tab_guess[MAX_NBPIECE];                  /* code propose par le joueur */

    /* Flag to end the game : victoire or maximum of tentatives */
    int victoire = 0;
    int tentatives = 0;
    /* resultats du guess */
    int nb_bien_place, nb_mal_place;
#ifdef JMMJ_DEBUG
    int i;
#endif

    mm_make_random_secret_code( tab_code_secret, NbPIECE, colors, SizeColor );
#ifdef JMMJ_DEBUG
    /* affiche pour tests */
    for(i = 0; i < NbPIECE; i++ ) {
        printf("%c", tab_code_secret[i]);
    }
    printf("\n");
#endif
    
    /* main loop to guess the code with MAX_TENTATIVES essais */
    do {
        /* incremente tentatives */
        tentatives++;
        /* demande un code au joueur */
        while( mm_ask_guess_user( tab_guess, NbPIECE, colors, SizeColor ) != 0 ) {
            printf("Votre entrée n'est pas valide\n");
        }
#ifdef DEBUG_CODE
        for( i = 0; i < NbPIECE; i++ ) {
            printf("%c", tab_guess[i]);
        }
        printf("\n");
#endif
        /* afficher le code du joueur */
        mm_print_code( tab_guess , NbPIECE, mode_daltonien );
        /* compute le nombre de bien et mal place */
        mm_algo_mastermind( tab_guess, tab_code_secret, NbPIECE, &nb_bien_place, &nb_mal_place );
        /* affiche résultat de cet essai */
        mm_affiche_resultat_guess( nb_bien_place, nb_mal_place, mode_daltonien );
        /* check if code secret has been found */
        if( nb_bien_place == NbPIECE ) {
            victoire = 1;
        }
    /* sort si victoire ou tentative depasse MAX_TENTATIVE
    //} while( (! victoire) && (tentatives <= MAX_TENTATIVE) ); / /not so bad in fact */
    } while( !( victoire || (tentatives > MaxTENTATIVE) ) ); /* conditions de sortie plus 'lisible' avec OU ?? bof..*/

    /* message de fin de partie */
    if( victoire == 1 ) {
        printf("Gagné le code secret était bien :\n");
        mm_print_code( tab_code_secret, NbPIECE, mode_daltonien);
        printf("Trouvé en %d tentatives sur %d \n", tentatives, MaxTENTATIVE);
    } else {
        printf("Perdu.. le nombre maximum de tentatives atteint\n");
        printf("Le code secret était : \n");
        mm_print_code( tab_code_secret, NbPIECE, mode_daltonien);
        printf("\n");
    }
    /* code retour du gagnant */
    return (victoire == 1) ?  1
                           :  2;
}

void mm_make_random_secret_code( char *secret, const int size_code, const char* colors, const int SizeColor)
{
    int i;
    /* test choix random du computer, other trick "RJVBO"[rand()%5]; */
    for(i=0; i < size_code; i++) {
        secret[i] = colors[ rand() % SizeColor ];
    }
}


int mm_ask_guess_user( char* p_tab_guess, const int size_code, const char *colors, const int SizeColor )
{
    int i;
    /* demander la saisie à l'utilisateur. 
       TODO verify valid entries */
    printf("Proposez une combinaison > ");
    for( i=0; i < size_code; i++ ) { /* best way found to read a line for the moment */
        p_tab_guess[i] = (char) getchar();  /* getchar() <=> getc(stdin) */
    }
    clean_stdin();
    /* valid code are valid */
    return mm_valid_code_input( p_tab_guess, size_code, colors, SizeColor);
}

void mm_algo_mastermind(const char* const p_tab_guess, const char* const p_tab_code_secret, const int size_piece, int *nb_bien_place, int *nb_mal_place )
{
    /* internal use */
    int memo_deja_utilise[MAX_NBPIECE];
    int autre_piece_modulo = 0;
    int piece, autre_piece, i;
    /* re-initialization à chaque call */
    *nb_bien_place = 0;
    *nb_mal_place = 0;
    
    for( i = 0; i < size_piece; i++) {
        memo_deja_utilise[i] = 0;
    }

    /* 2 loops seems needed, compute nb bien placé, mark as used with == 1 */
    for( i = 0; i < size_piece; i++) {
        if( p_tab_guess[ i ] == p_tab_code_secret[ i ] ) {
            memo_deja_utilise[i] = 1;
            (*nb_bien_place)++;
        }
    }

    autre_piece_modulo = 0;
    /* first loop the over guess */
    for( piece = 0; piece < size_piece; piece++ ) {

        if( memo_deja_utilise[ piece ] == 1 ) /* deja bien place pour le joueur */
            continue;                         /* deja mal place = 2 */

        /* second loop over the secret code, start at piece+1 */
        for( autre_piece = piece + 1; autre_piece < piece + size_piece; autre_piece++ ) {
            /* assure to stay in the range [0,NB-PIECE[ */
            autre_piece_modulo = autre_piece % size_piece;

            /* one match of color and not yet used */
            if(  ( p_tab_guess[piece] == p_tab_code_secret[autre_piece_modulo] )  /* couleurs correspondent */
             &&  ( memo_deja_utilise[ autre_piece_modulo ] == 0 ) ) {

                memo_deja_utilise[ autre_piece_modulo ] = 2; /* indique utilisé pour mal place */
                /* increment seulement mal place */
                (*nb_mal_place)++;
                /* it is done for this piece */
                break;
            }
        }
    }
}

void mm_affiche_resultat_guess( const int nb_bien_place, const int nb_mal_place, const bool mode_daltonien)
{
    /* shift to right with withespace character pour l'impression console */
    char p_string[LINE_SIZE_ON_SCREEN];
    memset( p_string, (int)' ', SHIFT_RESULTAT_PLACE );
    p_string[SHIFT_RESULTAT_PLACE] = '\0';  /* must use SHIFT.. not SHIFT + 1 */

    /* cat the specific output to p_string */
    if( mode_daltonien == true ) {
        mm_affiche_resultat_guess_blackwhite( p_string, nb_bien_place, nb_mal_place );
    } else {
        mm_affiche_resultat_guess_colored( p_string, nb_bien_place, nb_mal_place );
    }
    /* print to the console */
    printf("%s\n", p_string );
}

void mm_affiche_resultat_guess_blackwhite( char* p_string, const int nb_bien_place, const int nb_mal_place )
{
    /* char buffer[ sz+1 ]; */ /* +1 for null caracheter */
    char buffer[ LINE_SIZE_ON_SCREEN ];
    /* from devdocs.io */
    const char *fmt = "B:%d / M:%d";

    /*int sz = snprintf(NULL, 0, fmt, nb_bien_place, nb_mal_place);*/
    /*printf("sz : %d", sz);*/
    /* TODO CHECK SIZE, sz +1 should be ? */
    /* snprintf C99, only sprintf available*/
    /*int sz = snprintf(buffer, sizeof(buffer), fmt, nb_bien_place, nb_mal_place);*/
    int sz = sprintf(buffer, fmt, nb_bien_place, nb_mal_place);
    assert( sz < LINE_SIZE_ON_SCREEN );
    (void)sz;
    
    printf("buffer:%s\n", buffer);
    /* cat original and buffer */
    strcat( p_string, buffer );
}

void mm_affiche_resultat_guess_colored( char* p_string, const int nb_bien_place, const int nb_mal_place ) {

    int at_least_one_output = 0;  /* flag, sinon une ligne vide est imprimée */
    char buffer[LINE_SIZE_ON_SCREEN]; /* cannot initialize = ""; */
    int i;
    buffer[0] = '\0'; /* <=> strcpy( buffer, ""); */

    /* construit la ligne de resultats dans buffer */
    for( i = 0; i < nb_bien_place; i++ ) {
        strcat( buffer, "\033[30;1m\033[100m O \033[0m"); /* Bien placé en noir */
        at_least_one_output = 1;
    }
    for( i = 0; i < nb_mal_place; i++ ) {
        strcat( buffer, "\033[97;1m\033[100m O \033[0m");  /* Mal placé en blanc */
        at_least_one_output = 1;
    }
    /* cat to p_string */
    if( at_least_one_output ) {
        strcat( p_string, buffer );
    /* mets un message explicit s'il n'y a rien */
    } else {
        strcat( p_string, "nada...");
    }
}

void mm_print_code( const char* p_code, const int size_code, const bool mode_daltonien )
{
    char p_string[LINE_SIZE_ON_SCREEN];
    memset( p_string, (int)' ', SHIFT_DRAW_PIECE );
    p_string[SHIFT_DRAW_PIECE] = '\0';  /* must use SHIFT.. not SHIFT + 1 */

    if( mode_daltonien == true ) {
        mm_print_code_blackwhite( p_string, p_code, size_code );
    } else {
        mm_print_code_with_colors( p_string, p_code, size_code );
    }
    /* affiche well formatted string */
    printf("%s\n", p_string);
}

void mm_print_code_blackwhite( char* p_string, const char* p_code, const int size_code )
{
    /* char buffer[ 2*size_code + 1]; */
    char buffer[ 2 * MAX_NBPIECE + 1]; /* minimum size to contain the full code */
    char buffer_letter[3]; /* contains one letter */
    int i;

    buffer[0] = '\0';
    /* build the code in buffer */
    for( i = 0; i < size_code; i++ ) {
        sprintf( buffer_letter, "%c ", p_code[i]); /* intermediate for formatting */
        strcat( buffer, buffer_letter );
    }
    /* cat to p_string */
    strcat( p_string, buffer );
}

void mm_print_code_with_colors( char *p_string, const char* p_code, const int size_code )
{
    char buffer[100];          /* many caracteres for encoding colors */
    int i;
    buffer[0] = '\0';
    /* other */
    for( i = 0; i < size_code; i++ ) {
        mm_helper_formatted_string_colored( p_code[i], buffer );
        strcat( p_string, buffer );
    }
}

void mm_helper_formatted_string_colored( const char code, char* p_out)
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


int mm_valid_code_input(const char * const p_tab_guess, const int size_code, const char* colors, const int SizeColor)
{
    int error;
    int indice_guess, indice_color;
    /* loop over entry and check it is a valid color */
    for( indice_guess = 0; indice_guess < size_code; indice_guess++ ) {
        error = 1; /* set to error */
        for( indice_color = 0; indice_color < SizeColor; indice_color++ ) {
            /* one match found, this indice_guess is valid */
            if( p_tab_guess[indice_guess] == colors[indice_color] ) {
                error = 0;
                break;
            }
        }
        /* still error, invalid entry */
        if( error == 1)
            return 1;
    }
    return 0;
}
