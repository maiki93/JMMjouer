#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "historique.h"

/** function déclaration **/
int read_historique(Historique *histo, FILE* pt_fichier);
// text is constant, but FILENAME is modifiable from test_historique
const char * FILENAME="historique.txt";
const int LINE_SIZE = 100;

void initialize_historique( Historique* histo)
{
    // global pour tous les jeux
    histo->nbre_victoire_total = 0;
    histo->nbre_defaite_total = 0;
    histo->nbre_egalite_total = 0;

    // Mastermind
    histo->nbre_victoire_mm = 0;
    histo->nbre_defaite_mm = 0;
    histo->nbre_victoire_pendu = 0;
    histo->nbre_defaite_pendu = 0;
    histo->nbre_victoire_morpion = 0;
    histo->nbre_defaite_morpion = 0;
}


bool search_joueur_in_historique( Historique* historique, Joueur* joueur)
{
#ifdef DEBUG_CODE
    printf("Ouverture du fichier: %s\n", FILENAME);
#endif
    // open file
    FILE* pt_fichier = fopen( FILENAME, "r"); //+");
    char line[LINE_SIZE];
    bool new_joueur = true;       // default nouveau, switch if found in file
    // tricks for reading bool value
    int is_daltonien = false;

    if( pt_fichier == NULL ) {
        printf("Impossible d'ouvrir le fichier %s", FILENAME);
        return -1; //RETURN_KO;
    }

    // loop into the file until find '+++++' new_record
    while( fgets( line, LINE_SIZE, pt_fichier ) != NULL )
    {
        // reach a new record
        if( line[0] == '+' ) {
            // read the next line
            fscanf( pt_fichier, "\n%s %d\n", line, &is_daltonien );  // ACHTUNG besoin du slash \n !!
#ifdef DEBUG_CODE
            printf("scanf = %s \n", line);
            printf("strlen(joueur->nom) %ld  \n", strlen(joueur->nom));
            printf("is daltonien = %d \n", is_daltonien);
#endif
            // match  the player name : retrieve info player and load the historic record

// BUG !! mic and michael in history, stop to the first
            //if( strcmp( line, joueur->nom ) == 0 ) {
            if ( strncmp( joueur->nom, line, strlen(joueur->nom) ) == 0 ) { // same problem
#ifdef DEBUG_CODE
                printf("match joueur.name \n");
#endif
                new_joueur = false;
                // do not compile ?? ok need parenthesis around  (joueur..= false)...??
                is_daltonien == 1 ? (joueur->is_daltonien = true)
                                  : (joueur->is_daltonien = false);
                read_historique( historique, pt_fichier );
                break;
            }
        }
    }
    fclose( pt_fichier );
    return new_joueur;
}

int read_historique(Historique *histo, FILE* pt_fichier)
{
    //printf("\nRead_historique pt_fichier %p \n", pt_fichier);
    char line2[LINE_SIZE];
    char *p_data;
    // first argument, file or buffer
    //fscanf( pt_fichier, "mastermind: %d %d", &(histo->nbre_victoire_mm), &(histo->nbre_defaite_mm));
    //fscanf( pt_fichier, "pendu: %d %d", &(histo->nbre_victoire_mm), &(histo->nbre_defaite_mm));
    fgets( line2, LINE_SIZE, pt_fichier );
    //printf("line2 :%s:\n", line2);
    p_data = strchr( line2, ':' );
    //printf("toto\n");
    sscanf( ++p_data, "%d %d\n", &(histo->nbre_victoire_mm), &(histo->nbre_defaite_mm));

    // pendu
    fgets( line2, LINE_SIZE, pt_fichier );
    p_data = strchr( line2, ':' );
    sscanf( ++p_data, "%d %d\n", &(histo->nbre_victoire_pendu), &(histo->nbre_defaite_pendu));

    // morpion
    fgets( line2, LINE_SIZE, pt_fichier );
    p_data = strchr( line2, ':' );
    sscanf( ++p_data, "%d %d\n", &(histo->nbre_victoire_morpion), &(histo->nbre_defaite_morpion));

    //printf("victoire mastermind %d \n", histo->nbre_victoire_mm);
    return 0;
}

void print_historique( Historique* histo )
 {
    printf("\nHistorique de vos performances\n\n");
    printf("-----------------------------------\n");
    printf("Jeu             | Gagné  |  Perdu |\n");
    printf("-----------------------------------\n");
    printf("%-15s | %6d | %6d |\n", "MasterMind", histo->nbre_victoire_mm,      histo->nbre_defaite_mm);
    printf("%-15s | %6d | %6d |\n", "Pendu",      histo->nbre_victoire_pendu,   histo->nbre_defaite_pendu);
    printf("%-15s | %6d | %6d |\n", "Morpion",    histo->nbre_victoire_morpion, histo->nbre_defaite_morpion);
    printf("-----------------------------------\n");
}

////////// Joueur //////////
void print_info_joueur(Joueur *joueur)
{
    printf("\n------- Joueur informations -------\n");
    printf("\tPseudonyme : %s\n", joueur->nom);
    printf("\tDaltonien : %s\n", (joueur->is_daltonien) ? "yes" : "no" );
}
