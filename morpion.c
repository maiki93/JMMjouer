#include <stdio.h>
#include <stdlib.h>
#include "morpion.h"

//Pour voir si le header fonctionne
void test(){
    printf("Tout marche !");
}


//Lance le jeu morpion et renvoie le score du joueur en cas de victoire
int lancer_morpion(char pseudo){

    //Initialisation des variables
    char plateau [] = {'1','2','3','4','5','6','7','8','9','\0'};
    int jeu_en_cours = 1;
    int plateau_rempli = 0;
    int nb_case_joue = 0;

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
            tour_joueur(&nb_case_joue,plateau);
        }else{
            tour_ia(&nb_case_joue,plateau);
        }

        printf("C'est au tour de %s.",pseudo);








    }while( jeu_en_cours && nb_case_joue<9 );//On ne peut pas jouer plus de 9 cases !


}

//Affiche le plateau de jeu proprement et le nombre de cases remplis
void afficher_plateau(char * plateau, int *nb_jeu){
    printf("PLATEAU ACTUEL - %d CASES REMPLIS \n", *nb_jeu);
    printf("/-----\ \n");
    printf("|%c|%c|%c\n",*plateau,*(plateau+1),*(plateau+2));
    printf("|-----| \n");
    printf("|%c|%c|%c\n",*(plateau+3),*(plateau+4),*(plateau+5));
    printf("|-----| \n");
    printf("|%c|%c|%c\n",*(plateau+6),*(plateau+7),*(plateau+8));
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
int test_victoire(char * plateau,int num_jeu){
    int victoire = 0;
    char a_test;


    if(num_jeu%2==0){
        //Si c'est le joueur on test les symboles 'O'
        a_test = 'O';
    }else{
        //Sinon on test les symboles 'X' pour l'IA
        a_test = 'X';
    }


    //Test premiere ligne horizontale (haut)
    if( *(plateau) == a_test && *(plateau+1) == a_test && *(plateau+2) == a_test ){
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

        }else{

        }

    }while(!saisie_correct);


}

//Saisie de l'IA
void tour_ia(int * nb_case_joue, char * plateau){

}

//Test si la saisie en entre est correcte pour le plateau en entree
int saisie_est_correct(char * plateau, char * tentative){
    int est_correct = 0;



    return est_correct;

}
