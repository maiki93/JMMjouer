#include <stdio.h>
#include <stdlib.h>

#include "game_pendu.h"

int start_game_pendu( Joueur joueur, Historique *histo ) {

    printf("start jeu du pendu");
    printf("nom %s", joueur.nom);

    histo.nbre_defaite_total ++;
    //write_historique( histo );
    return 0;
}

