# JMM♡Jouer : le jeu d'arcade à ne pas manquer

**Langage** : C
**Compileur** : GNU GCC Compiler
**Environnment de développement** : CodeBlocks

Ce programme est un jeu d'arcade.
Les jeux proposés sont :
- le jeu du morpion ;
- le jeu du pendu ;
- le célèbre jeu de logique MasterMind ©.


L'utilisateur, le joueur, arrive sur un menu.
Son nom lui est demandé. Son compteur de points est initialisé à 0.
S’il est déjà parmi la liste des noms contenus dans le fichier `data.txt`, il pourra continuer avec ce compte-ci, son nombre de points et son nombre de partie jouer (total et décomposé).

Le menu propose 3 façon de jouer :
- choisir un jeu ;
- désigner aléatoirement un jeu ;
- jouer aux trois jeux d'affilés.

Les jeux peuvent être quitté depuis ce menu.

À chaque tour de jeux, une proposition pour rejouer est faite au joueur, il sera alors dirigé vers le menu, ou bien il pourra fermer l'arcade en choisissant de quitter.



À la fin du jeu, son inscription est sauvegardée : son nom et son compteur de point sont exportés vers le fichier `data.txt`.


![UserCase UML Diagram](to/.png)

# Compilation

### Avec GNU Makefile, testé sur Linux seulement. 

il y a quelques options (compilateur, debug mode..) in the Makefile
- make clean
- make or make JMMjouer : principal executable
- make unit_test : génère une série de tests executables test_X (requiert la librairie CMocka sur le système) 

# Documentation

```

```



# Suivi de projet

### SPRINT 2

SCRUMBOARD
**Mi** == Michael
**Ma** == Mathieu
**J** == Jasmine
**A**== All
**PP**==Peer Programming à déterminer


| to do | doing | to validate | done |
|---|---|---|---|
|| | |Faire le point rapide sur l’avancée de chacun **A**|
|| | |Écriture fonction ajouter points **Mi** |
|| | |Écriture fonction sauvegarder**Mi** |
||| | Écriture fonction export de l’historique (sauvegarde) du joueur dans un fichier externe .txt et choix de la démarche (un ou plusieurs fichiers) **pp** **A** |
|| | |Revue du Jeu du Morpion **Ma** |
|| | |Premier Merge branche Mathieu **Ma**|
|| | |Première intégration Jeu du Morpion **Ma** |
|| | |Revue du Jeu du Pendu **JMi** |
|| | | Premier Merge branche Jasmine **J**|
|| | |Première intégration Jeu du Pendu **J** |
||| | Ajout fonctionnalité rejouer **J**|
||| |Ajout fonctionnalité ajouter les victoires **J**  |
||| | Second Merge branche Jasmine **J** |
|| | | Seconde intégration Jeu du Pendu **J** |
|| | | Revue sur l’utilisation des structures Joueur et Historique **A**|
|| | | Implémentation de la fonction ajouter points jeu du Morpion **Ma**|
|| | | Implémentation de la fonction sauvegarder jeu du Morpion **Ma**|
||| | Implémentation de la fonction ajouter points jeu du pendu (update victoire) **J** |
|| | |Implémentation de la fonction ajouter points MasterMind **Mi** |
||| | Implémentation de la fonction sauvegarder Jeu du Pendu **J**|
|| | |Implémentation de la fonction sauvegarder MasterMind **Mi** |
|| | |Integration finale Jeu du Pendu **J** |
|| | |Merge Final Jeu du Morpion **Ma** |
|| | |Merge Final Jeu du MasteMind **Mi**|


### SPRINT 1
de lundi 10/01 vers 15h40 jusqu'au mercredi 12/01 11h20

| to do | doing | to validate | done |
|---|---|---|---|
| | | |Choix des utilisations du jeu d’arcade **A** |
| | | | Écriture des structures Joueur et Hitorique en prévision du stock des datas dans un fichier data.txt ou joueur.txt **JMi** |
| | | | Cours sur les structures **Mi**|
| | | | Rangement des structures : création historique.h et historique.c **JMi**|
| | | | Rangement des structures : création utils.h et utils.c **Mi**|
| | | | Revue du Master Mind : passage en fonctions| **Mi** |
| | | | Écriture du nouveau Jeu du Morpion **Ma** __ |
| | | | Revue du jeu du pendu : passage en fonctions **J**|
| | | | Écrire fonction pour affichage en couleurs des pions ♟ (MasterMind) **Mi**|
| | | |Écrire fonction pour proposer des indices (pendu) **J** |
| | ||  Revue/utilisation code fourni pour écriture `datas.txt` **A**|

### L'ensemble des tâches/product backlog


| taches | points | développeur | État de la tache |
|---|---|---|---|
|Choix des utilisations du jeu d’arcade | _3_ | **A**| done |
| Écriture des structures Joueur et Hitorique en prévision du stock des datas dans un fichier data.txt ou joueur.txt | _11_ |en peerprogramming par Michael, suivi par Jasmine | done |
|Cours sur les structures | |Michael | done|
| Rangement des structures : création historique.h et historique.c | _11_|Peer-progrmming Michael-Jas | done |
| Rangement des structures : création utils.h et utils.c | _11_ | Michael | done |
| Revue du Master Mind : passage en fonctions| | Michael | done |
| Écriture du nouveau Jeu du Morpion par Mathieu| _55_ | Mathieu | done |
| Revue du jeu du pendu : passage en fonctions | _34_ | Jasmine | done |
| Écrire fonction pour affichage en couleurs des pions ♟ (MasterMind) | | Michael | done |
|Écrire fonction pour proposer des indices (pendu) |_3_ | Jasmine | done |
|Faire le point rapide sur l’avancée de chacun|_1_ | **A** |done/to do (iteration) |
|Écriture fonction ajouter points| _55_ | | done|
|Écriture fonction sauvegarder| _55_ | | donce|
|Écriture fonction export de l’historique du joueur dans un fichier externe .txt et choix de la démarche (un ou plusieurs fichiers)| _89_ | **A** | done |
|Ranger le Jeu du Morpion en fonctions|_5_ | Mathieu | done |
|1er Merge branche Mathieu|_2_ | Mathieu | done|
|Revue du Jeu du Pendu| _21_|Jasmine et Michael |done |
|1er Merge branche Jasmine| _1_| Jasmine | done |
|Revue sur l’utilisation des structures Joueur et Historique|_5_ |**A** |done |
|Implémentation de la fonction ajouter points jeu du Morpion| | | done |
|Implémentation de la fonction ajouter points jeu du pendu|_13_ | | done |
|Implémentation de la fonction ajouter points MasterMind| | |done|
|Implémentation de la fonction sauvegarder jeu du Morpion| | |done|
|Implémentation de la fonction sauvegarder Jeu du Pendu|_13_ | |done|
|Implémentation de la fonction sauvegarder MasterMind| | | done|
| 1ere Integration du jeu du pendu|_1_|Jasmine |done |
| 1ere Integration du mastermind | _1_ | Michael| done|
| 1ere Integration du jeu du morpion | _1_ |Mathieu | done |
| Integration FINALE du jeu du pendu| _5_ |Jasmine | done |
| Integration FINALE du mastermind | _5_ | Michael| done|
| Integration FINALE du jeu du morpion | _5_ | Mathieu| done|
| Revue/utilisation code fourni pour écriture datas.txt| _13_ |**A** |doing |
| Réécrire un ficher d'historique avec l'Historique courantjuste en changeant les lignes du joueur concerné|_13_|Michael |done |
| charger historique joueur : afficher historique des lignes concernées |_2_| JMi|done|
| | | | |
| | | | |

> Ajouter des tâches si besoin, actualiser si possible ou prevenir ScrumMaster pour reporter les avancées pour suivi de projet optimisé

_Estimation des charges avec un poker planning basé sur la suite de Fibonacci : 0 1 1 2 3 5 8 13 21 34 55 89._
