#include <stdio.h>
#include <stdlib.h>

#include "historique.h"

void initialize_historique( Historique* histo)
{
    histo->nbre_victoire_total = 0;
    histo->nbre_defaite_total = 0;
    histo->nbre_egalite_total = 0;
}
