#ifndef HISTORIQUE_H_INCLUDED
#define HISTORIQUE_H_INCLUDED

// boolean variable is not in the C standard, must be imported
// use bool variable = true / false; or 0 / 1 (less clear I find ...)
#include <stdbool.h>

/** Definit une structure pour garder l'historique d'un joueur
    Après création d'un Historique vide, la fonction initialize_historique doit être OBLIGATOIREMENT appelée
    A chaque ajout de variable dans la structure, mettre a jour la function d'initialisation.
**/
typedef struct struct_historique {
    int nbre_victoire_total, nbre_defaite_total, nbre_egalite_total;   // peut-être pas necessaire, la somme peut etre recalculée
    int nbre_victoire_mm, nbre_defaite_mm;                         // pas d"égalite
    int nbre_victoire_morpion, nb_defaite_morpion;
    int nbre_points_pendu, nbre_parties_pendu ;
} Historique;


typedef struct struct_joueur {
    char nom[50];
    bool serie_3_game;                      // true : indicate if the player participe to a serie of all games, maximum one of each game)
                                            // false : ask to make again a game
    //Historique p_historique; possible...
} Joueur;

void initialize_historique( Historique* );
void read_historique( Historique* );
void write_historique( Historique* );



#endif // HISTORIQUE_H_INCLUDED
