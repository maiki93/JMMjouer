#ifndef MORPION_H_INCLUDED
#define MORPION_H_INCLUDED

#include "person.h"
#include "victory.h"

/*
#ifdef BUILD_DLL
    #define SHARED_LIB __declspec(dllexport)
#else
    #define SHARED_LIB __declspec(dllimport)
#endif
*/
/* to check also exported __stdcall (more windows eg. win32) or __cdecl(more standard) */
#define SHARED_LIB __declspec(dllexport)

void test();

/*  objdump.exe -tT game_morpion.dll | grep 'start' do not see difference start_game_morpion appears in both */
/* gcc default is visibilty on ? */
/*victory_t SHARED_LIB start_game_morpion(person_t joueur);*/
victory_t start_game_morpion(person_t joueur);


void afficher_plateau(char *, int *);

int test_plateau_rempli(char *);

int test_victoire(char *, char );

int test_case_libre(char *, int *);

void tour_joueur(int *, char *);

/* void tour_ia(int *, char *); */
void tour_ia(char *);

int saisie_est_correct(char *, char );

void maj_plateau(char *, char, char );

#endif
