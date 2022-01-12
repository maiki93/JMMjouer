#include <stdio.h>
#include <stdlib.h>

#include "historique.h"

void initialize_historique( Historique* histo)
{
    // global pour tous les jeux
    histo->nbre_victoire_total = 0;
    histo->nbre_defaite_total = 0;
    histo->nbre_egalite_total = 0;

    // Mastermind
    histo->nbre_victoire_mm = 0;
    histo->nbre_defaite_mm = 0;

    // Morpion
    histo->nbre_victoire_morpion = 0;
    histo->nbre_defaite_morpion = 0;


}
