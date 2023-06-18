# JMM♡Jouer : le jeu d'arcade à ne pas manquer

**Langage** : C
**Compileur** : GNU GCC Compiler, MSVC on Windows
**OS** : Linux, Windows, Mingw (GCC on Windows)

Projet débuté lors d'une formation en C/C++.
Poursuivi à titre personnel pour:
- découvrir le développement Windows avec ses spécificités : compilation, librairies, plugins...
- expérimenter des techniques orientés objets en C (classes dérivées, polymorphisme)
- tenter une structure clean du code

## Description du projet
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

# Compilation

### Avec GNU Makefile, sur toutes les plateformes

Un Makefile spécifique est fourni pour chaque plateforme

il y a quelques options (compilateur, debug mode..) in the Makefile
- make -f Makefile.XXX clean
- make or make -f Makefile.XXX JMMjouer : principal executable
- make -f Makefile.XXX unit_test : génère une série de tests executables test_X (requiert la librairie CMocka sur le système)

Pour générer le plugin de jeu du morpion;
- make -f Makefile.XXX libgame_morpion sur Linux et windows Mingw
- make -f Makefile.XXX game_morpion.dll sur Windows avec MSVC

# Documentation

Documentation générée par Doxygen (en cours)

# Composants
Projet structuré en librairies et modules :

## Un système de plugins pour étendre les jeux (game_loader)
Les jeux peuvent être :
- inclus directement dans le code (mastermind)
- inclus dans une libraries dynamique et ajouté à la compilation (pendu)
- inclus dans un plugin et chargé dynamiquement à l'éxécution (morpion)

## Une librairie de container "générique" en C (ccontainer)
- Une liste générique 
- Un vecteur générique
- Des implémentations pour intégrer des C-strings
- Quelques algorithmes de tri
- De nombreuses spécifiques implméentations sont utilisées par le reste du système

## Une interface pour gérer les sauvegardes (record)
- Modéle d'interface / injection de dépendences
- Implémentation utilisant des fichiers textes

## Validation
- test unitaires avec CMocka
- Valgrind, Dr.Memory
