#pragma once

/* @brief List the games known at compilation.

    Mastermind: included in source code
    Pendu: use shared library, linked at linkage. Header available at compilation.

    Moved all dependencies (include) into implementation file.
    Here only define what will be exported by the implementation.
    Less to recompile if a game is added.
*/

/* Not a "good practice" to define variable in header
    Here every include will get its own copy (static) of the variables, fine ? only one include
   Advice use extern X in .h in define them *.c 
   To see with other compilers 
static const int nb_default_game = 2;
static const ptr_game_t default_game_prtf[] = { &start_game_mastermind, &start_game_pendu };
*/

extern const int nb_default_game;
extern const char* default_game_name[];

/*  warning: array ‘default_game_prtf’ assumed to have one element
    because it must be declared with a size (or assume {0})
    extern indicates it is allocated in an other unit (*c implementation) */
extern const ptr_game_t default_game_prtf[];

