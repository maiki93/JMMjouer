#pragma once

#include "game_loader/game_loader.h"
#include "record/irecord_fwd.h" /* typedef irecord_t */

/* forward declaration */
struct arcade_params_t;

/*arcade_t arcade_new();*/
int arcade_init( game_loader_t *game_loader, irecord_t *record_impl);
void arcade_clear();
/* main loop */
int arcade_run( arcade_params_t *params );