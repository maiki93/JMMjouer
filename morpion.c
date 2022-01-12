#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "morpion.h"

//Pour voir si le header fonctionne
void test(){
    printf("Tout marche !");
}


//Lance le jeu morpion et renvoie le score du joueur en cas de victoire
int lancer_morpion(char * pseudo){

    //Initialisation des variables
    char plateau [] = {'1','2','3','4','5','6','7','8','9','\0'};
    int jeu_en_cours = 1;
    int plateau_rempli = 0;
    int nb_case_joue = 0;
    //score = 1 si joueur gagne
    //score = 0 si match nul ou victoire de l'IA
    int score = 0;

    //Bienvenue au joueur dans le jeu
    printf("Bienvenue %s dans le jeu du morpion !\n",pseudo);
    printf("C'est a vous de commencer le premier tour.\n");
    printf("Vous avez le symbole 'O' et l'IA surentraine aura le symbole 'X'.\n");
    printf("Bonne chance !");

    //Boucle de jeu
    do{

        //Le nombre de case joue paire -> tour du joueur
        //Sinon tour de l'ia
        if(nb_case_joue%2 == 0){
            printf("C'est au tour de %s.\n",pseudo);
            tour_joueur(&nb_case_joue,plateau);
            if( test_victoire(plateau,'O') ){
                    printf("Bravo !\nVous avez gagne !!!\n");
                    jeu_en_cours = 0;
                    score = 1;
            }
        }else{
            printf("C'est au tour de l'IA.\n");
            tour_ia(&nb_case_joue,plateau);
            if( test_victoire(plateau,'X') ){
                    printf("Oh non !\nL'IA vous a battu !!!");
                    jeu_en_cours = 0;
                    score = 0;
            }
        }
        //S'il n'y a pas de victoire a ce tour (le jeu est toujours en cours), on regarde si le tableau n'est pas rempli
        //Si le tableau est rempli alors c'est un match nul
        if(jeu_en_cours && test_plateau_rempli(plateau)){
            printf("La teableau est rempli !\nC'est un match nul !\n");
            score = 0;
            jeu_en_cours = 0;
        }

        nb_case_joue++;


    }while( jeu_en_cours && nb_case_joue<9 );//On ne peut pas jouer plus de 9 cases !

    return score;
}

//Affiche le plateau de jeu proprement et le nombre de cases remplis
void afficher_plateau(char * plateau, int *nb_jeu){
    printf("PLATEAU ACTUEL - %d CASES REMPLIS \n", *nb_jeu);
    printf("/-----\ \n");
    printf("|%c|%c|%c|\n",*plateau,*(plateau+1),*(plateau+2));
    printf("|-----| \n");
    printf("|%c|%c|%c|\n",*(plateau+3),*(plateau+4),*(plateau+5));
    printf("|-----| \n");
    printf("|%c|%c|%c|\n",*(plateau+6),*(plateau+7),*(plateau+8));
    printf("\-----/ \n");

}

//Test si le plateau est rempli, c'est a dire qu'il est rempli que de 'X' ou de 'O'
int test_plateau_rempli(char * plateau){
    int est_rempli = 1;

    for(int i=0;i<9;i++){
        if( *(plateau+i)!='X' && *(plateau+i)!='O' ){
            est_rempli = 0;
        }
    }

    return est_rempli;
}

//Test si le joueur/IA apres avoir joue son tour a gagne ou non
int test_victoire(char * plateau, char * a_test){
    printf("Entre dans test victoire\n");
    int victoire = 0;

    //Test premiere ligne horizontale (haut)
    if( *plateau == a_test && *(plateau+1) == a_test && *(plateau+2) == a_test ){
        victoire = 1;
    }
    //Test deuxieme ligne horizontale (milieu)
    if( *(plateau+3) == a_test && *(plateau+4) == a_test && *(plateau+5) == a_test ){
        victoire = 1;
    }
    //Test troisieme ligne horizontale (bas)
    if( *(plateau+6) == a_test && *(plateau+7) == a_test && *(plateau+8) == a_test ){
        victoire = 1;
    }
    //Test premiere ligne verticale (gauche)
    if( *(plateau) == a_test && *(plateau+3) == a_test && *(plateau+6) == a_test ){
        victoire = 1;
    }
    //Test deuxieme ligne verticale (centre)
    if( *(plateau+1) == a_test && *(plateau+4) == a_test && *(plateau+7) == a_test ){
        victoire = 1;
    }
    //Test troisieme ligne verticale (droite)
    if( *(plateau+2) == a_test && *(plateau+5) == a_test && *(plateau+8) == a_test ){
        victoire = 1;
    }
    //Test premiere diagonale (de haut gauche a bas droite)
    if( *(plateau) == a_test && *(plateau+4) == a_test && *(plateau+8) == a_test ){
        victoire = 1;
    }
    //Test deuxieme diagonale (de haut droite a bas gauche)
    if( *(plateau+2) == a_test && *(plateau+4) == a_test && *(plateau+6) == a_test ){
        victoire = 1;
    }

    printf(" victoire = %d\n",victoire);
    printf("Sort de test victoire\n");

    return victoire;
}

//On test si la case jouee est libre pour un symbole
int test_case_libre(char * plateau, int * tentative){
    int est_libre = 0;

    //La fonction isdigit vient de la librairie ctype.h
    //Il sert a tester si le caractere est un chiffre decimal ou non
    if( isdigit((char)*(plateau+*(tentative)-1) )){
        est_libre = 1;
    }

    return est_libre;
}

//Saisie du joueur
void tour_joueur(int * nb_case_joue, char * plateau){
    int saisie_correct = 0;
    char tentative;

    do{
        afficher_plateau(plateau,nb_case_joue);
        printf("Veuillez entrer un chiffre correspondant aux cases libre ci-dessus\n> ");
        fflush(stdin);
        scanf("%c",&tentative);

        if( saisie_est_correct(plateau,tentative) ){
            maj_plateau(plateau,tentative,'O');
            saisie_correct=1;
            printf("Vous avez joue votre tour !\n");
        }else{
            printf("Il y a une erreur dans votre saisie, veuillez recommencer.\n");
        }

    }while(!saisie_correct);


}

//Saisie de l'IA
void tour_ia(int * nb_case_joue, char * plateau){

    int jeu_ordinateur=0;
    char tentative = '0';

    //Boucle de generation de nombre aleatoire jusqu'a que ce soit valide
    do{
        jeu_ordinateur = (rand() % 9) + 1; // On g�n�re un nombre entier entre 1 et 9
    }while(!saisie_est_correct(plateau,tentative+jeu_ordinateur));
    maj_plateau(plateau,tentative+jeu_ordinateur,'X');


}

//Test si la saisie en entre est correcte pour le plateau en entree
int saisie_est_correct(char * plateau, char * tentative){
    printf("Entrer dans saisie_est_correct\n");
    int est_correct = 0;

    for(int i=0;i<9;i++){
        printf("tentative = %c\n",tentative);
        printf("plateau[%d] = %c\n",i, *(plateau+i) );

        if( *(plateau+i) == tentative ){
            est_correct = 1;
        }
    }

    printf("Fonction saisie_est_correct : est_correct = %d\n",est_correct);

    return est_correct;

}

//Mettre a jour le plateau avec une saisie correct
void maj_plateau(char * plateau, char * tentative, char * symbole){

    // Instruction qui permet de convertir un caractere de 0 a 9 en entier
    //On le decremente de 1 aussi car notre tableau est de 0 a 8 et non de 1 a 9
    int i = tentative - '0' -1;

    printf("Entre dans maj plateau.\n");
    printf("tentative = %c\n",tentative);
    printf("i = %d\n",i);
    printf("Ce que l'on souhaite modifier : %c\n", *(plateau+i) );
    printf("Le symbole %c\n",symbole);

    *(plateau + i) = symbole;


    printf("Sort de maj plateau\n");
}
