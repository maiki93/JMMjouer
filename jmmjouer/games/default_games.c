

/* object file included at compilation */
#include "jmmjouer/games/game_mastermind.h"
/* implementation through shared library at compilation, must still know the API */
#include "jmmjouer/games/game_pendu.h"

#include "jmmjouer/ptrf_game_typedef.h"


const int nb_default_game = 2;
const char* default_game_name[] = {"Mastermind", "Pendu"};

const ptr_game_t default_game_prtf[] = { &start_game_mastermind, &start_game_pendu };