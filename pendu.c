#include <stdio.h>
#include <stdlib.h>

#include <stdio.h>
#include <stdlib.h>





void dessiner( int dessin_tours)
{
    char pendu[11][15]=
    {
        "  _______     ",
        " |       |    ",
        " |       _    ",
        " |      / \\  ",
        " |      \\_/  ",
        " |      _|_   ",
        " |     /| |\\ ",
        " |    / |_| \\",
        " |       ^    ",
        " |      / \\  ",
    };
    int i;
    if(dessin_tours>10)
    {
        return;
    }
    for(i=0; i<dessin_tours; i++)
    {
        printf("%s\n", pendu[i]);
    }

}


void indice( int last_turn) // pour donner un indice au dernier tour TODO: fonction qui retourne la definition du mot
{

    printf(" ");
}





void printWord(char* mot_mystere, int lengthRandWord, char* indexCorrectLetters)
{

            /** A RANGER DANS UN FONCTION printWord()
            type fonction :
            en entree : saisie
            en sortie : juste impression ds la console
            **/
            for (int toLoop=0;toLoop<lengthRandWord;toLoop++) // Imprimons les lettres trouvees au bon endroit dans notre mot
                {
                    //pour imprimer les lettres là où les indexs sauvegarde==1
                    if(indexCorrectLetters[toLoop]==1)
                        {
                            printf(" %c ", mot_mystere[toLoop]);
                        }
                    //sinon, la ou les entrees sont toujours a zero, on imprime un _
                    else
                        {
                            printf(" _ ");
                        }
                } // end for loop d'impression du mot

}


void loopSecretWord(char* saisie, int lengthRandWord, char* mot_mystere, int correct_char_counter_special, int mot_ok, int nbTourRestant)

{
    for(int i=0; i<=lengthRandWord; i++) //boucle code secret
        {
            if (saisie[i]== mot_mystere[i]) // si le char i de la saisie utilisateur est dans le mot
                {
                    correct_char_counter_special++;
                    if (correct_char_counter_special==lengthRandWord) // si le compteur de char correct est egale a la longueur du mot -> tous sont trouve
                        {
                            printf("Quelle efficience, quelle tentative courageuse... %s est le bon mot. \n", saisie);
                            mot_ok=1;
                            break;
                        }
                }
                    else // sinon, ce n'est pas le bon mot
                        {
                            mot_ok=2;
                            nbTourRestant--;
                            printf("Rate... bravo pour votre courage.\n");
                            break;
                        }
        }
}


int countRightChar(char* mot_mystere, int lengthRandWord, char* indexCorrectLetters, char* saisie, int* correct_char_counter)
{
    char lettre_saisie=saisie[0];
    //correct_char_counter=0;
    for (int toLoop=0;toLoop<lengthRandWord;toLoop++)
        {
            if(indexCorrectLetters[toLoop]==1)
                {
                    continue; //ignore les saisies correctes deja entrees
                }

            if (lettre_saisie==mot_mystere[toLoop])
                {
                    indexCorrectLetters[toLoop]=1; // on maj a 1 les entrees correctes dans la table indexCorrectLetters si la lettre est dans le mot mystère parcouru
                    correct_char_counter++;
                    printf("Caractere present %d fois. \n", correct_char_counter);
                }
        }
    return correct_char_counter; // end boucle sur le mot
}












int main()
{

srand(time(NULL));

//declaration des constantes
const int NOMBRE_MOTS_MYSTERES = 13;
const char mots_mysteres[][200]={"TOTO", "LOL", "FLEUR", "FLAGEOLET","COEUR", "CODE", "LANGAGE", "PROGRAMMATION","BATON", "BROCOLI", "LINUX", "SCANF", "SHELL"};    //liste de mot_mystere:

//declaration des variables
char saisie[16];
char lettre_saisie;
int toLoop=0; //pour varier de mon i habituel
int nbTourRestant=10, mot_ok=0;
int index=0, trying=0, correct_char_counter=0, correct_char_counter_special=0,oldCorrect=0, quit=0;
int randomIndex=rand() % NOMBRE_MOTS_MYSTERES;
int lengthRandWord=strlen(mots_mysteres[randomIndex]);
char mot_mystere[200];
strcpy(mot_mystere, mots_mysteres[randomIndex]);




// pour memoriser les index des entrees correctes et ignorer ensuite les saisies correctes deja entrees
char indexCorrectLetters[13]={'0','0','0','0','0','0','0','0','0','0','0','0','0'};
    printf("Licensed by Jasmine Banchereau - 01/2022\n\n");
    printf("###############################\n");
    printf("\n");
    printf("Bienvenue dans le jeu du pendu !\n");
    printf("\n");
    printf("###############################\n\n\n");

     // pour le random


    printf("Mot mystère : %s \n, index du mot dans la liste : %d,\n longueur du mot : %d,\n\n", mots_mysteres[randomIndex], randomIndex, lengthRandWord);
    printf("______________ \n");
    printf("Regles du jeu \n");
    printf("Votre mission est de deviner le mot mystere.\n");
    printf("Les undercores entre espaces representent chaque lettre du mot mystere.\n");
    printf("Pour jouer, entrez vos lettres en CAPITALES s'il vous plait. \n");
    printf("Pour quitter, tapez \"quit\" ou \"q\" \n");
    printf("Pour proposer un mot, tapez \"mot\" ou \"w\" \n\n");
    printf("Attention !\n");
    printf("Quelques mots de vocabulaire en programmation C parmi les mots mysteres. \n");
    printf("_____________ \n\n");
    printf("Debut de la partie\n\n");

    // Algorithme du jeu :
    while ((correct_char_counter<lengthRandWord))
        {
            if(mot_ok==1)
            {
                printf("Victoire!");
                break;
            }


        printWord(mot_mystere, lengthRandWord, indexCorrectLetters);

            printf(" \n\n ");
            printf("Nombre de lettres trouvees : %d\n Nombre de tours restant : %d\n\n", correct_char_counter, nbTourRestant);
            printf("Quelle lettre choisissez-vous ? ");
            fgets(saisie,16,stdin);
            saisie[strcspn(saisie, "\n")] = 0;

            if((strcmp(saisie,"quit",4)==0)||(strcmp(saisie,"q",4)==0)) // pour les laches qui souhaitent abandonner avant de perdre, voici le parachute !

                {
                    quit=1;
                    break;
                }


            else if ((strcmp(saisie,"mot",4)==0)||(strcmp(saisie,"w",4)==0))  //pour les courageux souhaitant entrer le mot complet directement nous allons parcourir les deux tableaux de caractères
               {
                    printf("Entrez votre mot : \n");
                    fgets(saisie,16,stdin);
                    saisie[strcspn(saisie, "\n")] = 0;
                    //pour gagner ou pas lors de la saisie d'un mot
                    loopSecretWord(&saisie, lengthRandWord, &mot_mystere, correct_char_counter_special, mot_ok, nbTourRestant);

                }

            lettre_saisie=saisie[0]; // pour catch la premiere lettre saisie
            oldCorrect=correct_char_counter;

            if (mot_ok==0)
            {
                printf("Lettre : %c\n",lettre_saisie);
                // TODO : BUGGGGGGG; le correct _char reste à 0 et ne s'incrémente pas tour après tour
                countRightChar(&mot_mystere, lengthRandWord, &indexCorrectLetters, &saisie, &correct_char_counter);
                printf("Char total trouve: %d\n",correct_char_counter);

                if(oldCorrect==correct_char_counter) // perdre des points a chaque fois que le nombre d'entree corectes n'a pas augmente une fois l'iteration terminee
                    {
                        nbTourRestant--;
                        printf("Entree incorrecte...");
                        printf("Vous pouvez encore vous trompez %d fois ;-\) \n\n", nbTourRestant);
                        dessiner(10-nbTourRestant); // dessin ASCII
                        if (nbTourRestant==0)
                            {
                                printf("Vous avez perdu. Le mot etait : %s.", mots_mysteres[randomIndex]);
                                break;
                            }
                    }
                else
                    {
                        printf("Super, la lettre %c est bien presente dans le mot. \n\n", lettre_saisie);
                    }
            }

            if(mot_ok==2)
                {
                    dessiner(10-nbTourRestant);
                    mot_ok=0;
                }
        } // end while : on sort lorque le nombre de saisies correctes "correct_char_counter" n'est plus inferieur a la longueur du mot pioche

    if(quit==1) // on a maj le quit a 1 lorsque l'input de l'utilisateur est "quit"
        {
            printf("L'utilisateur a quitte la partie avant la fin. A bientot.");
        }
    else if ( (correct_char_counter_special==lengthRandWord) || (correct_char_counter==lengthRandWord) )// pour la victoire
        {
            for (toLoop=0;toLoop<lengthRandWord;toLoop++)
                {
                    printf(" %c ", mots_mysteres[randomIndex][toLoop]);
                }
            printf("\n\n");
            printf("BRAVO ! Vous avez trouvé le mot %s \n", mots_mysteres[randomIndex]);
            printf("Fin de la partie\n");
        }
    return 0;
}
