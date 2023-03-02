#pragma once

#include "ccontainer/clist_generic.h"
#include "victory.h"

#include "shared_EXPORTS.h"

struct pair_game_victory_t {
    char game_name[20]; /* key */
    victory_t victories; /* value, easier if one structure */
};

typedef struct {
    /* clist_gen_t clist; no access generic type in h */
    clist_gen_t *clist;
} cmap_game_victories_t;

/* internal usage only, dependent value_t and victory_t only 
    shared export not needed if internal module ?? */
/*SHARED_EXPORT*/ value_t make_value_victory( struct pair_game_victory_t victory );
/*SHARED_EXPORT*/ struct pair_game_victory_t extract_value_victory( const value_t* value);

/*
void deleter(value_t* value)
*/

SHARED_EXPORT cmap_game_victories_t* game_victories_new();
SHARED_EXPORT void game_victories_init(cmap_game_victories_t *cmap);
SHARED_EXPORT void game_victories_clear(cmap_game_victories_t *cmap);
SHARED_EXPORT void game_victories_delete(cmap_game_victories_t *cmap);


SHARED_EXPORT size_t game_victories_size(cmap_game_victories_t *cmap);

/*game_victories_insert(const char* name, victories);*/
SHARED_EXPORT int game_victories_insert( cmap_game_victories_t *cmap, struct pair_game_victory_t victory);
SHARED_EXPORT struct pair_game_victory_t game_victories_get_copy( cmap_game_victories_t *cmap, const char *name);
