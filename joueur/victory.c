#include "victory.h"

void victory_init( victory_t *victory )
{
    victory->nb_win = 0;
    victory->nb_lost = 0;
    victory->nb_equality = 0;
}