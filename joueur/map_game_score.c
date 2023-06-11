#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "joueur/map_game_score.h"
#include "ccontainer/clist_generic.h"
#include "ccontainer/value_cstring.h"
#include "clogger/clogger.h"

/** Implementation of  \ref equalizer_value_t for the key.
 *  It is a simple string comparison on maximum 20 caracters. 
 *  can hide in implementation, or reusue with pair from outside ? */
static bool map_name_match(const ccontainer_value_t *value1, const ccontainer_value_t *value2 );

/* ** Serialization / Deserialization / Default copy and deleter from value_t ***/
ccontainer_value_t make_value_pair_score( const struct pair_game_score_t *pair_victory_in, ccontainer_err_t *err_code)
{
    ccontainer_value_t value_out;
    char *p_buffer;
    assert( pair_victory_in );
    assert( strlen(pair_victory_in->game_name) < MAX_NAME_GAME_LEN );

    ccontainer_reset_value(&value_out);
    /* pointer to follow the serialization */
    p_buffer = (char *) malloc( sizeof(struct pair_game_score_t));
    if( !p_buffer ) {
        *err_code = CCONTAINER_ALLOCERR;
        return value_out;
    }
    
    /* serialize name (fixed=20) + victory (fixed=3*8) */ 
    /*memcpy( p_buffer, &(pair_victory_in->game_name), 20);*/
    /* p_buffer += 20; */ /* it not equivalent !! */
    /*memcpy( p_buffer + 20, &(pair_victory_in->victories), sizeof(victory_t) );*/

    /* no pointer in pair structure, can copy byte-to-byte */
    memcpy( p_buffer, pair_victory_in, sizeof(struct pair_game_score_t));
    
    /* swap and return */
    *err_code = CCONTAINER_OK;
    value_out.data = p_buffer;
    value_out.len = sizeof(struct pair_game_score_t);
    return value_out;
}

ccontainer_err_t extract_value_pair_score(const ccontainer_value_t *value, struct pair_game_score_t *pair_victory_out)
{
    assert( pair_victory_out );
    /* equivalent, here p += 20 working also */
    /*
    memcpy( pair_victory_out->game_name, value->data, 20);
    p += 20;
    memcpy( &(pair_victory_out->victories), p, sizeof(victory_t) );
    */
    memcpy( pair_victory_out, value->data, sizeof(struct pair_game_score_t));
    
    return CCONTAINER_OK;
}

bool map_name_match(const ccontainer_value_t *value1, const ccontainer_value_t *value2 )
{
    assert( value1 && value2);
    /* case of same value (same pointer ??) */
    /* compare the first 20 bytes, corresponding to the name */
    return ( strncmp(value1->data, value2->data, MAX_NAME_GAME_LEN) == 0 )
                    ? true
                    : false;
}

/*** Implementation Constructor/Destructor *****/
map_game_score_t* map_game_score_new()
{
    return (map_game_score_t*) malloc(sizeof(map_game_score_t));
}

void map_game_score_init(map_game_score_t *map)
{
    map->clist = clist_gen_new();
    clist_gen_init( map->clist );
}

map_game_score_t map_game_score_copy(const map_game_score_t *map_in, ccontainer_err_t *err_code)
{
    map_game_score_t map_out;
    clist_gen_t* tmp_clist;

    assert(map_in);
    /* only pointer avaliable in public API fir clist_gen_t */
    /* map game victories is a clist of pair_game_score,
        no memmory allocation in pair => default copy of "pair_value" is enought */
    tmp_clist = clist_gen_copy( map_in->clist, ccontainer_copy_value, err_code );
    map_out.clist = tmp_clist;
    return map_out;
}

void map_game_score_delete(map_game_score_t *map)
{
    assert( map && map->clist); /* != NULL); */
    /* it is a pointer to clist_gen_t */
    clist_gen_free( map->clist, ccontainer_delete_value );
    map->clist = NULL;
}

void map_game_score_free(map_game_score_t *map)
{
    map_game_score_delete( map );
    free(map);
}

/*** Public API ****/
size_t map_game_score_size(const map_game_score_t *map)
{
    return clist_gen_size( map->clist );
}

ccontainer_err_t map_game_score_insert(map_game_score_t *map, const struct pair_game_score_t* pair_in)
{
    ccontainer_err_t err_code;
    /* a temporary value_t to be moved into clist_gen_t */
    ccontainer_value_t* pvalue;
    ccontainer_value_t value_name_match;
    clist_node_t *node;

    ccontainer_value_t value_clist = make_value_pair_score( pair_in, &err_code );

    /* search if exists */
    value_name_match = make_value_cstring( pair_in->game_name, &err_code);
    if( err_code == CCONTAINER_ALLOCERR) {
        //strcpy( pair.game_name, "invalid");
        return -1;
    }
    /*
    pvalue = clist_gen_find( map->clist, equalizer_value_cstring,
                             pair_in->game_name, &err_code); */
    node = clist_gen_find_node( map->clist, 
                                clist_gen_get_first_node( map->clist ), 
                                map_name_match, &value_name_match, &err_code);
    /* not present */
    if( err_code == CCONTAINER_EMPTY || err_code == CCONTAINER_NOT_FOUND ) {
        assert( node == NULL );
        err_code = clist_gen_push_back( map->clist, &value_clist );
        deleter_value_cstring(&value_name_match);
        return err_code;
    }

    assert( node != NULL );
    /* already present, replace the value
        1. remove old, insert new_one with clist_gen_push_back
        2. replace function in clist_generic replace, not really needed for one case
            => cmap_generic
        3. rewrite value directly, same size
    */
    /* method 3. get pointer to value_t */
    pvalue = clist_gen_get_node_value( node );
    assert( pvalue != NULL );
    /* must delete old content before trnasfert value_clist 
        pvalue is not modified, only content (!= free_value)*/
    ccontainer_delete_value( pvalue );
    /* move content direclty inside container */
    *pvalue = ccontainer_move_value( &value_clist );
    /* value_clist invalidated after move call */

    /* must write two times the delete */
    deleter_value_cstring(&value_name_match);
    return CCONTAINER_OK;
}

struct pair_game_score_t map_game_score_get_from_name( const map_game_score_t *map, 
                                                       const char *name)
{
    struct pair_game_score_t pair;
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
    err_code = extract_value_pair_score( pvalue_clist, &pair );

    deleter_value_cstring(&value_name_match);
    return pair;
}

void map_game_score_print_info(const map_game_score_t* map)
{
    clist_node_t*current_node;
    struct pair_game_score_t pair_out;
    ccontainer_value_t* pvalue;
    ccontainer_err_t err_code;

    current_node = clist_gen_get_first_node( map->clist);
    if (current_node == NULL) {
        CLOG_DEBUG("Empty map_game_score %d\n", 0);
        return;
    }
    do {
        pvalue = clist_gen_get_node_value( current_node );
        err_code = extract_value_pair_score( pvalue, &pair_out );
        assert( err_code == CCONTAINER_OK );
        printf("%s : ", pair_out.game_name);
        score_game_print_info( &(pair_out.score) );

        current_node = clist_gen_get_next_node( current_node);
    } while( current_node != NULL);

    /*return;*/
}


