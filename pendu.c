#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

void indice( char* mot_mystere) // pour donner un indice au dernier tour TODO: fonction qui retourne la definition du mot if nbTourRestant==1
{
    if (strcmp(mot_mystere, "TOTO") == 0)
        {
            printf("Indice : egal a zero \n");
        }
    else if (strcmp(mot_mystere, "LOL") == 0)
        {
            printf("Indice : hahahahaha");
        }
    else if (strcmp(mot_mystere, "LOL") == 0)
        {
            printf("Indice : hahahahaha");
        }
    else if (strcmp(mot_mystere, "FLEUR") == 0)
        {
            printf("Indice : ");
        }
    else if (strcmp(mot_mystere, "FLAGEOLET") == 0)
        {
            printf("Indice : ");
        }
    else if (strcmp(mot_mystere, "COEUR") == 0)
        {
            printf("Indice : ");
        }
    else if (strcmp(mot_mystere, "CODE") == 0)
        {
            printf("Indice : ");
        }
    else if (strcmp(mot_mystere, "LANGAGE") == 0)
        {
            printf("Indice : ");
        }
    else if (strcmp(mot_mystere, "PROGRAMMATION") == 0)
        {
            printf("Indice : ");
        }
    else if (strcmp(mot_mystere, "BATON") == 0)
        {
            printf("Indice : ");
        }
    else if (strcmp(mot_mystere, "BROCOLI") == 0)
        {
            printf("Indice : ");
        }
    else if (strcmp(mot_mystere, "LINUX") == 0)
        {
            printf("Indice : ");
        }
    else if (strcmp(mot_mystere, "SCANF") == 0)
        {
            printf("Indice : ");
        }
    else if (strcmp(mot_mystere, "SHELL") == 0)
        {
            printf("Indice : ");
        }
    //else // DEFAULT
     //   {
      //      printf("Pas d'indice pour ce mot.");
       // } , "FLEUR", "FLAGEOLET","COEUR", "CODE", "LANGAGE", "PROGRAMMATION","BATON", "BROCOLI", "LINUX", "SCANF", "SHELL" 
       /**    else if (strcmp(mot_mystere, "FLAGEOLET") == 0)
        {
            printf("Indice : ");
        }**/
}

void printWord(char* mot_mystere, int lengthRandWord, int* indexCorrectLetters)
{
            for (int toLoop=0;toLoop<lengthRandWord;toLoop++) // Imprimons les lettres trouvees au bon endroit dans notre mot
                {
                    if(indexCorrectLetters[toLoop]==1)//pour imprimer les lettres là où les indexs sauvegarde==1
                        {
                            printf(" %c ", mot_mystere[toLoop]);
                        }

                    else //sinon, la ou les entrees sont toujours a zero, on imprime un _
                        {
                            printf(" _ ");
                        }
                } // end for loop d'impression du mot
}

int loopSecretWord(char* saisie, int lengthRandWord, char* mot_mystere, int* correct_char_counter_special, int* nbTourRestant)
{
    *correct_char_counter_special=0;
    for(int i=0; i<=lengthRandWord; i++) //boucle code secret
        {
            if (saisie[i]== mot_mystere[i]) // si le char i de la saisie utilisateur est dans le mot
                {
                    (*correct_char_counter_special)= *correct_char_counter_special+1;
                    if (*correct_char_counter_special==lengthRandWord) // si le compteur de char correct est egale a la longueur du mot -> tous sont trouve
                        {
                            printf("Quelle efficience, quelle tentative courageuse... %s est le bon mot. \n\n", saisie);
                            //*mot_ok=1; //utiliser victory a la place.....
                            //break;
                            return 1;
                        }
                }
                    else // sinon, ce n'est pas le bon mot
                        {
                            (*nbTourRestant)--;
                            printf("Rate... bravo pour votre courage.\n");
                            return 0;
                        }
        }
}

void countRightChar(char* mot_mystere, int lengthRandWord, int* indexCorrectLetters, char* saisie, int* correct_char_counter)
{
    char lettre_saisie=saisie[0];
    //*correct_char_counter=0;
    for (int toLoop=0;toLoop<lengthRandWord;toLoop++)
        {
            if(indexCorrectLetters[toLoop]==1)
                {
                    continue; //ignore les saisies correctes deja entrees
                }

            if (lettre_saisie==mot_mystere[toLoop])
                {
                    indexCorrectLetters[toLoop]=1; // on maj a 1 les entrees correctes dans la table indexCorrectLetters si la lettre est dans le mot mystère parcouru
                    (*correct_char_counter)++;
                }
        }// end boucle sur le mot     //printf("nombre de char correct ok dans la boucle : %d", correct_char_counter); //printf("Caractere present %d fois. \n", *correct_char_counter);
}

int victoire(int lengthRandWord, int* indexCorrectLetters)
{
    int victory_counter=0;
    for(int i=0;i<lengthRandWord;i++)
    {
        if(indexCorrectLetters[i]==1)
            {
                victory_counter++;
            }
    }
    if (victory_counter==lengthRandWord)
        {
            printf("\n\n");
            printf("BRAVO ! Vous avez trouvé le mot.\n");
            printf("Fin de la partie\n");
            return 1;
        }
    return 0;
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
    int nbTourRestant=10;
    int correct_char_counter=0, correct_char_counter_special=0,oldCorrect=0, quit=0;
    int randomIndex=rand() % NOMBRE_MOTS_MYSTERES;
    int lengthRandWord=strlen(mots_mysteres[randomIndex]);
    int indexCorrectLetters[13]={0,0,0,0,0,0,0,0,0,0,0,0,0};// pour memoriser les index des entrees correctes et ignorer ensuite les saisies correctes deja entrees
    int win;
    char mot_mystere[200];
    strcpy(mot_mystere, mots_mysteres[randomIndex]);

    // Accueil de l'utilisateur
    printf("Licensed by Jasmine Banchereau - 01/2022\n\n");
    printf("###############################\n");
    printf("\n");
    printf("Bienvenue dans le jeu du pendu !\n");
    printf("\n");
    printf("###############################\n\n\n");
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
    while (victoire(lengthRandWord, indexCorrectLetters)==0)
        {
            printWord(mot_mystere, lengthRandWord, indexCorrectLetters);
            printf(" \n\n "); //printf("Nombre de lettres trouvees : %d\n", correct_char_counter); // on ne met plus a jour la variable correct_char_counter
            printf("Nombre de tours restant : %d\n\n", nbTourRestant);
            printf("Quelle lettre choisissez-vous ? ");
            if (fgets(saisie, 16, stdin) == NULL) //fgets(saisie,16,stdin) rend un warning;
                {
                    printf("error fgets");
                }
            saisie[strcspn(saisie, "\n")] = 0;
            if((strncmp(saisie,"quit",4)==0)||(strncmp(saisie,"q",1)==0)) // pour les laches qui souhaitent abandonner avant de perdre, voici le parachute !
                {
                    quit=1;
                    break;
                }
            else if ((strncmp(saisie,"mot",3)==0)||(strncmp(saisie,"w",1)==0))  //pour les courageux souhaitant entrer le mot complet directement nous allons parcourir les deux tableaux de caractères
               {
                    printf("Entrez votre mot : \n");
                    if (fgets(saisie, 16, stdin) == NULL)
                        {
                            printf("error fgets");
                        }
                    saisie[strcspn(saisie, "\n")] = 0;

                    // TODO: il faudrait peut-etre avoir une autre approche afin de reutiliser la fonction victoire()
                    win=loopSecretWord(saisie, lengthRandWord, mot_mystere, &correct_char_counter_special, &nbTourRestant);//pour gagner ou pas lors de la saisie d'un mot
                    if (win==1)
                        {
                            printf("Bravo ! La sortie de loopSecretWord est 1.\n");
                            break;
                        }
                }
            else // si la saisie n'est ni "w" ni "mot" ni "q" ni "quit"
                {
                    lettre_saisie=saisie[0]; // pour catch la premiere lettre saisie
                    oldCorrect=correct_char_counter;
                    //printf("avant countRightCounter : correct char counter =%d, oldCorrect=%d\n", correct_char_counter, oldCorrect);
                    printf("Lettre : %c\n",lettre_saisie);
                    countRightChar(mot_mystere, lengthRandWord, indexCorrectLetters, saisie, &correct_char_counter);
                    //printf("apres countRightCounter : correct char counter =%d, oldCorrect=%d\n", correct_char_counter, oldCorrect);
                    win=victoire(lengthRandWord, indexCorrectLetters);
                    if(win==1)
                        {
                            for(int i=0;i<lengthRandWord;i++)
                            {
                                printf(" %c ", mot_mystere[i]);
                            }
                            break;
                        }

                     /**
                     A RANGER DANS UNE FONCTION A VOTRE AVIS????? Je vois pas... c bien comme ça je trouve
                     **/
                    if(oldCorrect==correct_char_counter) // perdre des points a chaque fois que le nombre d'entree corectes n'a pas augmente une fois l'iteration terminee
                        {
                            printf("IF CORR_COUNER ==OLD : correct char counter =%d, oldCorrect=%d\n\n", correct_char_counter, oldCorrect);
                            nbTourRestant--;
                            printf("Entree incorrecte...");
                            printf("Vous pouvez encore vous trompez %d fois ;-) \n\n", nbTourRestant);
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
                    /**
                    FIN DE LA DERNIERE FONCTION A RANGER EVENTUELLEMENT si possible
                    **/
                }
        if (nbTourRestant==1)
                {
                    indice(&mot_mystere);
                }
        } // END WHILE victory==0
        
    if(quit==1) // on a maj le quit a 1 lorsque l'input de l'utilisateur est "quit"
        {
            printf("L'utilisateur a quitte la partie avant la fin. A bientot.");
        }

    return 0;
}

