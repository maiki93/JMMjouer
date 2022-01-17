#ifndef MORPION_H_INCLUDED
#define MORPION_H_INCLUDED

#include "historique.h"

void test();

//int lancer_morpion(char*);
int lancer_morpion(Joueur joueur,  Historique *histo);

void afficher_plateau(char *, int *);

int test_plateau_rempli(char *);

int test_victoire(char *, char *);

int test_case_libre(char *, int *);

void tour_joueur(int *, char *);

void tour_ia(int *, char *);

int saisie_est_correct(char *, char *);

void maj_plateau(char *, char *, char *);

#endif // MORPION_H_INCLUDED
