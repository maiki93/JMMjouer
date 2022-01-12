#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "historique.h"

/** function déclaration **/
int read_historique(Historique *histo, FILE* pt_fichier);

const char *FILENAME="historique.txt";
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
}


int search_joueur_in_historique( Historique* historique, const char *nom_joueur ) {

    printf("Entree read_historique avec nom %s \n", nom_joueur );
    // open file
    // ouverture du fichier
    FILE* pt_fichier = fopen( FILENAME, "r"); //+");
    char line[LINE_SIZE];

    char line2[LINE_SIZE];

    if( pt_fichier == NULL ) {
        printf("Impossible d'ouvrir le fichier %s", FILENAME);
        return -1; //RETURN_KO;
    }

    //char *p_retour_fgets;
    // loop into the file until find '+++++' new_record
    while( fgets( line, LINE_SIZE, pt_fichier ) != NULL )
    {
        //line[strcspn(line, "\n")] = 0;
        // new record
        if( line[0] == '+' ) {
            // read the next line
            fscanf( pt_fichier, "\n%s\n", line );  // besoin du slash \n !!
            printf("scanf = %s \n", line);

            if( strcmp( line, nom_joueur ) == 0 ) {
                printf("trouvé joueur %s !! pt_fichier %p \n", nom_joueur, pt_fichier);

                read_historique( historique, pt_fichier );
                fclose( pt_fichier );
                //break;
                return 0;
            }
            //else {
            //    printf("Nouveau joueur \n");
            //}
        }
    };

    if( historique == NULL )  {
        printf("found new record, read nom joueur");
        //fscanf( pt_fichier, "%s", line );
        //printf("nom joueur = %s", line);

    }


    fclose( pt_fichier );
    return 0;
}

int read_historique(Historique *histo, FILE* pt_fichier)
{
    printf("\nRead_historique pt_fichier %p \n", pt_fichier);
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

    printf("victoire mastermind %d \n", histo->nbre_victoire_mm);
    printf("defaite mastermind %d \n", histo->nbre_defaite_mm);
    printf("victoire pendu %d \n", histo->nbre_victoire_pendu);
    printf("defaite pendu %d \n", histo->nbre_defaite_pendu);
    printf("victoire morpion %d \n", histo->nbre_victoire_morpion);
    printf("defaite morpion %d \n", histo->nbre_defaite_morpion);

    return 0;
}

void print_historique( Historique* ) {
}
