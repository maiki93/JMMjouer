
#include "game_mastermind.h"
#include "game_pendu.h"

/* @brief List the games known at compilation.

    Mastermind: included in source code
    Pendu: use shared library, but header available at compilation
*/

/* not a "good practice" to define variable in header
    Here every include will get its own copy (static) of the variables, fine ? only one include
   Advice use exter X in .h in define them *.c 
   To see with other compilers */
static const int nb_default_game = 2;
static const char * default_game_name[] = {"Mastermind", "Pendu"};
static const ptr_game_t default_game_prtf[] = { &start_game_mastermind, &start_game_pendu };
