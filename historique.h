#ifndef HISTORIQUE_H_INCLUDED
#define HISTORIQUE_H_INCLUDED

// boolean variable is not in the C standard, must be imported
// use bool variable = true / false; or 0 / 1 (less clear I find ...)
#include <stdbool.h>

/** DEFINE, code retour **/
// #define RETURN_OK 0
// #define RETURN_KO -1

/** Definit une structure pour garder l'historique d'un joueur
    Après création d'un Historique vide, la fonction initialize_historique doit être OBLIGATOIREMENT appelée
    A chaque ajout de variable dans la structure, mettre a jour la function d'initialisation.
**/
typedef struct struct_historique {
    int nbre_victoire_total, nbre_defaite_total, nbre_egalite_total;   // peut-être pas necessaire, la somme peut etre recalculée
    int nbre_victoire_mm, nbre_defaite_mm;                             // pas d"égalite
    int nbre_victoire_morpion, nbre_defaite_morpion;                   //  égalité ?
    int nbre_victoire_pendu, nbre_defaite_pendu;
} Historique;

typedef struct struct_joueur {
    char nom[50];
    bool serie_3_game;                      // true : indicate if the player participe to a serie of all games, maximum one of each game)
                                            // false : ask to make again a game
    //Historique p_historique; possible...
} Joueur;

void initialize_historique( Historique* );
/** lit le fichier et recherche le nom du joueur
    param historique == NULL nouveau joueur, sinon pointe sur une structure historique valide
**/
int search_joueur_in_historique( Historique*, const char *nom_joueur );
//int read_historique( historique* );
//

//TODO récrire le fichier d'historique  avec l'Historique courrant
// réécrire touts le fichier, juste en changeaunt la prtie du joueur concerné
// void write_historique( Joueur,   Historique* );

// TODO a faire, rajouter dans le menu entre:: afficher votre historique
void print_historique( Historique* );



#endif // HISTORIQUE_H_INCLUDED
