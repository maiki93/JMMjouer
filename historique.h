#ifndef HISTORIQUE_H_INCLUDED
#define HISTORIQUE_H_INCLUDED

typedef struct struct_historique {
    int nbre_victoire_total, nbre_defaite_total;
    //int nbre_victoire_morpion, nb_defaite_morpion;
    //int nbre_points_pendu, nbre_parties_pendu ;
    //, points_MM, nb parties_MM;
} Historique;


typedef struct struct_joueur {
    char nom[50];
    //Historique p_historique;
} Joueur;

void read_historique( Historique* );
void write_historique( Historique* );


#endif // HISTORIQUE_H_INCLUDED
