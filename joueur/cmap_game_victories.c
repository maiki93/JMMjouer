#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "cmap_game_victories.h"

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
    /* equivalent, here p += 20 working !! */
    /*
    memcpy( pair_victory_out->game_name, value->data, 20);
    p += 20;
    memcpy( &(pair_victory_out->victories), p, sizeof(victory_t) );
    */
    memcpy( pair_victory_out, value->data, sizeof(struct pair_game_victory_t));
    
    return CCONTAINER_OK;
}

/*** Implementation Constructor/Destructor *****/
cmap_game_victories_t* game_victories_new()
{
    return (cmap_game_victories_t*) malloc(sizeof(cmap_game_victories_t));
}

void game_victories_init(cmap_game_victories_t *cmap)
{
    cmap->clist = clist_gen_new();
    clist_gen_init( cmap->clist );
}

void game_victories_delete(cmap_game_victories_t *cmap)
{
    assert( cmap->clist != NULL);
    /* it is a pointer to clist_gen_t */
    clist_gen_free( cmap->clist, ccontainer_delete_value );
    cmap->clist = NULL;
}

void game_victories_free(cmap_game_victories_t *cmap)
{
    game_victories_delete( cmap );
    free(cmap);
}

/*** Public API ****/
size_t game_victories_size(cmap_game_victories_t *cmap)
{
    return clist_gen_size( cmap->clist );
}

ccontainer_err_t game_victories_insert( cmap_game_victories_t *cmap, struct pair_game_victory_t pair)
{
    ccontainer_err_t err_code;
    /* a temporary value_t to be moved into clist_gen_t */
    ccontainer_value_t value_clist = make_value_pair_victory( &pair, &err_code );
    err_code = clist_gen_push_back( cmap->clist, &value_clist );
   return err_code;
}

struct pair_game_victory_t game_victories_get_copy( cmap_game_victories_t *cmap, const char *name)
{
    struct pair_game_victory_t pair;
    ccontainer_value_t *pvalue_clist;
    ccontainer_err_t err_code;
    
    pvalue_clist = clist_gen_find( cmap->clist, name, strlen(name), &err_code );
    if( err_code != CCONTAINER_OK )
    {
        strcpy( pair.game_name, "invalid");
        return pair;
    }
    /* create pair_victory from value_clist */
    err_code = extract_value_pair_victory( pvalue_clist, &pair );
    return pair;
}

