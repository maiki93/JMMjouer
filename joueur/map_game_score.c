#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "joueur/map_game_score.h"
#include "ccontainer/clist_generic.h"
#include "ccontainer/value_cstring.h"
#include "clogger/clogger.h"


/** Define a pair : <game name, victory_t> to be inserted in a ccontainer.
 * Implementation detail, stay in implementation.
 * Using const for game and score is very restrictive, enfocre Value Object by usage only
 * \ingroup entities_grp */
struct pair_game_score_t {
    /** key, constant usually in a map */
    char game_name[MAX_NAME_GAME_LEN];
    /** value, results of games a Value Object constness to enforce */
    score_game_t score;
};

/** \name Adapter functions to ccontainer for \ref pair_game_score_t. 
 * \{ */
/** Return a ccontainer_value_t from a pair_game_score_t.
 * Create a copy of all data in the provided pair, for insertion into the clist_gen_t
 * \param[in] pointer to a pair
 * \param[out] pointer to a ccontainer error code
 * \return ccontainer_value_t with a copy of the data in pair */
static ccontainer_value_t make_value_pair_score( const struct pair_game_score_t *pair_victory_in, ccontainer_err_t *err_code);

/** Extract a pair_game_score_t from a generic value_t 
 *  Create a copy of the data in value_in into pair_victory_out
 * \pre pair_out must point to a valid adress (memory already allocated on stack or on heap before the call)
 * \note no memory allocation, err_code useless. may change api / return pair_game_score_t
 * \param[in] pointer to a value_t as source
 * \param[out] pointer to a pair_victory_t, memory must be allocated before the call
 * \return error_code from ccontainer library. */
static ccontainer_err_t extract_value_pair_score( const ccontainer_value_t* value_in, struct pair_game_score_t *pair_victory_out);

/* Ddo not need specific deleter or duplicater because no memory allocation on heap in the pair
void deleter_pair_score(value_t* value) */

/** \} */

/** Implementation of  \ref equalizer_value_t for the key.
 *  It is a simple string comparison on maximum 20 caracters. */
static bool map_name_match(const ccontainer_value_t *value1, const ccontainer_value_t *value2 );

/* ************************ 
    * Serialization / Deserialization / Default copy and deleter from value_t *
*************************** */
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
    memcpy( &(pair_victory_out->victories), p, sizeof(game_score_t) );
    */
    memcpy( pair_victory_out, value->data, sizeof(struct pair_game_score_t));
    
    return CCONTAINER_OK;
}

/* to test, creation value without an intermediate pair for optimization ??
static ccontainer_value_t make_value_pair_score2( const char* name, score_game_t score, ccontainer_err_t *err_code)
*/

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

map_game_score_t map_game_score_copy(const map_game_score_t *map_src, int *status)
{
    map_game_score_t map_out;
    clist_gen_t* tmp_clist;
    ccontainer_err_t err_code;

    assert(map_src);

    /* map game victories is a clist of pair_game_score,
        no memmory allocation in pair => default copy of "pair_value" is enought */
    tmp_clist = clist_gen_copy( map_src->clist, ccontainer_copy_value, &err_code );
    /* case empty is also included , not really an error ... */
    if( err_code == CCONTAINER_ALLOCERR) {
        /* error, or empty copy */
        CLOG_ERR("allocation error in clist_gen_copy %d", 0);
        *status = 1;
        /* undefined behavior or an empty list ? */
        map_game_score_init( &map_out );
        return map_out;
    }
    /* else success : empty is ok, swap clist */
    map_out.clist = tmp_clist;
    *status = 0;
    return map_out;
}

void map_game_score_delete(map_game_score_t *map)
{
    assert( map && map->clist);
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

int map_game_score_insert(map_game_score_t *map, const char *name_game, const score_game_t *score)
{
    ccontainer_err_t err_code;
    struct pair_game_score_t pair_in; /* no call to delete, all on stack */
    ccontainer_value_t new_value, value_name_match;
    ccontainer_value_t* p_old_value;  /* a temporary value_t to be moved into clist_gen_t */
    clist_node_t *node;

    /* new pair to insert in value_t format */
    strncpy( pair_in.game_name, name_game, MAX_NAME_GAME_LEN);
    pair_in.score = *score;
    new_value = make_value_pair_score( &pair_in, &err_code );

    assert( map && map->clist );

    /* search if exists, need name_game in value_t format as well */
    value_name_match = make_value_cstring( pair_in.game_name, &err_code);
    if( err_code == CCONTAINER_ALLOCERR) {
        return -1;
    }
    
    node = clist_gen_find_node( map->clist,
                                clist_gen_get_first_node( map->clist ),
                                map_name_match, &value_name_match, &err_code);
    /* all case, delete temporary value_name_match */
    deleter_value_cstring(&value_name_match);

    /* case not present, move new_value */
    if( err_code == CCONTAINER_EMPTY || err_code == CCONTAINER_NOT_FOUND ) {
        assert( node == NULL );
        err_code = clist_gen_push_back( map->clist, &new_value );
        if( err_code != CCONTAINER_OK) {
            CLOG_ERR("Cannot insert new value %s", pair_in.game_name);
        }
        /* return 0 for new insertion */
        return 0;
    }

    /* already present, move the value directly (always the same size) inside the clist_gen_t */   
    assert( node != NULL );
    /* get pointer to previous value_t */
    p_old_value = clist_gen_get_node_value( node );
    assert( p_old_value != NULL );
    /* must delete old content before transfert value_clist */
    ccontainer_delete_value( p_old_value );
    /* move content direclty inside the container */
    *p_old_value = ccontainer_move_value( &new_value );
    /* new_value invalidated after move call */
    /* return 1 for replacement */
    return 1;
}

int map_game_score_get_from_name( const map_game_score_t *map, const char *name, score_game_t *score_out)
{
    ccontainer_value_t *pvalue_clist;
    ccontainer_value_t value_name_match;
    ccontainer_err_t err_code;
    
    /* need to transform name to search into a value_t */
    value_name_match = make_value_cstring( name, &err_code);
    if( err_code == CCONTAINER_ALLOCERR) {
        deleter_value_cstring(&value_name_match);
        *score_out = score_game_create();
        return -1;
    }
    
    pvalue_clist = clist_gen_find( map->clist, map_name_match, &value_name_match, &err_code );
    if( err_code != CCONTAINER_OK )  {
        deleter_value_cstring(&value_name_match);
        *score_out = score_game_create();
        return -1;
    }
    /* temp to free */
    deleter_value_cstring(&value_name_match);

    /* could create a pair_victory from pvalue_clist and return pair.score
    extract_value_pair_score( pvalue_clist, &pair ); / *score_out = pair.score; */
    /* <=> to use static size of name */
    memcpy( score_out, pvalue_clist->data + MAX_NAME_GAME_LEN, sizeof(score_game_t) );
    return 0;
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
        printf("%25s : ", pair_out.game_name);
        /* print on the same line */
        score_game_print_info( &(pair_out.score) );

        current_node = clist_gen_get_next_node( current_node);
    } while( current_node != NULL);
}




