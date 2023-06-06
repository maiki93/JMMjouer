#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "joueur/map_game_victories.h"
#include "ccontainer/clist_generic.h"
#include "ccontainer/value_cstring.h"

/** Implementation of a equalizer_value_t for the key.
 *  It is a simple string comparison on maximum 20 caracters. 
 *  can hide in implementation, or reusue with pair from outside ? */
static bool map_name_match(const ccontainer_value_t *value1, const ccontainer_value_t *value2 );

/* ** Serialization / Deserialization / Default copy and deleter from value_t ***/
ccontainer_value_t make_value_pair_victory( const struct pair_game_victory_t *pair_victory_in, ccontainer_err_t *err_code)
{
    ccontainer_value_t value_out;
    char *p_buffer;
    assert( pair_victory_in );
    assert( strlen(pair_victory_in->game_name) < 20 );

    ccontainer_reset_value(&value_out);
    /* pointer to follow the serialization */
    p_buffer = (char *) malloc( sizeof(struct pair_game_victory_t));
    if( !p_buffer ) {
        *err_code = CCONTAINER_ALLOCERR;
        return value_out;
    }
    
    /* serialize name (fixed=20) + victory (fixed=3*8) */ 
    /*memcpy( p_buffer, &(pair_victory_in->game_name), 20);*/
    /* p_buffer += 20; */ /* it not equivalent !! */
    /*memcpy( p_buffer + 20, &(pair_victory_in->victories), sizeof(victory_t) );*/

    /* no pointer in pair structure, can copy byte-to-byte */
    memcpy( p_buffer, pair_victory_in, sizeof(struct pair_game_victory_t));
    
    /* swap and return */
    *err_code = CCONTAINER_OK;
    value_out.data = p_buffer;
    value_out.len = sizeof(struct pair_game_victory_t);
    return value_out;
}

ccontainer_err_t extract_value_pair_victory(const ccontainer_value_t *value, struct pair_game_victory_t *pair_victory_out)
{
    assert( pair_victory_out );
    /* equivalent, here p += 20 working also */
    /*
    memcpy( pair_victory_out->game_name, value->data, 20);
    p += 20;
    memcpy( &(pair_victory_out->victories), p, sizeof(victory_t) );
    */
    memcpy( pair_victory_out, value->data, sizeof(struct pair_game_victory_t));
    
    return CCONTAINER_OK;
}

bool map_name_match(const ccontainer_value_t *value1, const ccontainer_value_t *value2 )
{
    assert( value1 && value2);
    /* case of same value (same pointer ??) */
    /* compare the first 20 bytes, corresponding to the name */
    return strncmp(value1->data, value2->data, 20) == 0 ? true
                                            : false;
}

/*** Implementation Constructor/Destructor *****/
map_game_victories_t* game_victories_new()
{
    return (map_game_victories_t*) malloc(sizeof(map_game_victories_t));
}

void game_victories_init(map_game_victories_t *map)
{
    map->clist = clist_gen_new();
    clist_gen_init( map->clist );
}

map_game_victories_t game_victories_copy(const map_game_victories_t *map_in, ccontainer_err_t *err_code)
{
    map_game_victories_t map_out;
    clist_gen_t* tmp_clist;

    assert(map_in);
    /* only pointer avaliable in public API fir clist_gen_t */
    /* map game victories is a clist of pair_game_victory,
        no memmory allocation in pair => default copy of "pair_value" is enought */
    tmp_clist = clist_gen_copy( map_in->clist, ccontainer_copy_value, err_code );
    map_out.clist = tmp_clist;
    return map_out;
}

void game_victories_delete(map_game_victories_t *map)
{
    assert( map && map->clist); /* != NULL); */
    /* it is a pointer to clist_gen_t */
    clist_gen_free( map->clist, ccontainer_delete_value );
    map->clist = NULL;
}

void game_victories_free(map_game_victories_t *map)
{
    game_victories_delete( map );
    free(map);
}

/*** Public API ****/
size_t game_victories_size(const map_game_victories_t *map)
{
    return clist_gen_size( map->clist );
}

ccontainer_err_t game_victories_insert(map_game_victories_t *cmap, const struct pair_game_victory_t* pair_in)
{
    ccontainer_err_t err_code;
    /* a temporary value_t to be moved into clist_gen_t */
    ccontainer_value_t value_clist = make_value_pair_victory( pair_in, &err_code );
    err_code = clist_gen_push_back( cmap->clist, &value_clist );
   return err_code;
}

struct pair_game_victory_t game_victories_get_copy( map_game_victories_t *map, const char *name)
{
    struct pair_game_victory_t pair;
    ccontainer_value_t *pvalue_clist;
    ccontainer_value_t value_name_match;
    ccontainer_err_t err_code;
    
    /* need to transform name to search into a value_t */
    /*
    value_name_match.data = (char*)malloc( strlen(name) + 1 );
    strncpy( value_name_match.data, name, 20);
    value_name_match.len = strlen(name) + 1;
    */
    /* or use already implemented function for cstring */
    value_name_match = make_value_cstring( name, &err_code);
    if( err_code == CCONTAINER_ALLOCERR) {
        strcpy( pair.game_name, "invalid");
        return pair;
    }
    
    pvalue_clist = clist_gen_find( map->clist, map_name_match, &value_name_match, &err_code );
    if( err_code != CCONTAINER_OK )
    {
        deleter_value_cstring(&value_name_match);
        strcpy( pair.game_name, "invalid");
        return pair;
    }
    /* create pair_victory from value_clist */
    err_code = extract_value_pair_victory( pvalue_clist, &pair );

    deleter_value_cstring(&value_name_match);
    return pair;
}


