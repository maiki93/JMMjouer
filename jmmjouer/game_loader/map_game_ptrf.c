#include <stdio.h> /* for printf, to suppress */
#include <string.h>
#include <assert.h>

#include "jmmjouer/game_loader/map_game_ptrf.h"
/*#include "ccontainer/clist_cstring.h"*/
/*#include "ccontainer/cvector_cstring.h"*/
#include "ccontainer/value_cstring.h"

struct map_game_ptrf_type {
    /** first data member, cast to (clist_gen_t*) valid */
    clist_gen_t *clist;
    /** size */
    /*size_t len;*/
};

/** intermediate for storage / ccontainer_value_t specialization 
    Contains heap-allocated memory, but same deleter for ccontainer_value_t
    Only (intermediate) pair must be carefully deallocated
    Variant to pair_game_victory_t, with dynamically allocated game_name */
typedef struct /*pair_game_ptrf*/ {
    char *game_name;
    ptr_game_t pfgame;
} pair_game_ptrf_t;

/*  ********* Adapter functions ************* */
/* for conversion with ccontainer_value_t
  choice of serialization different of map_game_victories :
  - copy all the C-string name + ptrf into the data, no pointer to allocated memory in value_t
    use null caracter to mark the end of the C-string
  - copy the pointer of each function */
static ccontainer_value_t make_value_game_ptrf( pair_game_ptrf_t *pair, ccontainer_err_t *err_code)
{
    ccontainer_value_t value_out;
    char *p_buffer;
    size_t len_value;

    ccontainer_reset_value(&value_out);
    
    /* a C-string, include the terminaison caracter necessary to retrieve the second field (ptr_game)!
         or need to store the size of the string */
    len_value = strlen( pair->game_name) + 1 /* c-string */
                    + sizeof (ptr_game_t);   /* ptr_game_t */
    p_buffer = (char*)malloc( len_value );
    if( !p_buffer ) {
        *err_code = CCONTAINER_ALLOCERR;
        return value_out;
    }
    /* strcpy copy '\0' if possible 
        p_buffer not the end of the string, memcpy finally better ?*/
    strcpy( p_buffer, pair->game_name);

    /* could get until the first '\0' with cstrpn() */
    memcpy( p_buffer + strlen(pair->game_name) + 1, &pair->pfgame, sizeof(ptr_game_t) );

    *err_code = CCONTAINER_OK;
    value_out.data = p_buffer;
    value_out.len = len_value;
    return value_out;
}

static pair_game_ptrf_t extract_value_pair_game_ptrf(const ccontainer_value_t* value_in, ccontainer_err_t *err_code)
{
    pair_game_ptrf_t pair;
    size_t name_len;
    /* if well formatted, search for first '\0' */
    name_len = strlen(value_in->data);
    pair.game_name = (char*)malloc( name_len + 1);
    if( !pair.game_name ) {
        *err_code = CCONTAINER_ALLOCERR;
        pair.game_name = NULL;
        pair.pfgame = NULL;
        return pair;
    }
    strcpy(pair.game_name, value_in->data);
    /* copy ptr_game */
    memcpy(&pair.pfgame, value_in->data + name_len + 1, sizeof (ptr_game_t));

    *err_code = CCONTAINER_OK;
    return pair;
}

/*  Intermediate pair, maybe to make visible later, maybe not */
pair_game_ptrf_t make_pair_game_ptrf(const char *name, ptr_game_t pfgame_in)
{
    pair_game_ptrf_t pair;
    pair.game_name = (char*)malloc(strlen(name) + 1);
    strcpy(pair.game_name, name);
    pair.pfgame = pfgame_in;
    return pair;
}

void delete_pair_game_ptrf(pair_game_ptrf_t *pair)
{
    if( pair->game_name) {
        free(pair->game_name);
        pair->game_name = NULL;
    }
    pair->pfgame = NULL;
}

/*  ****** Constructor / Destructor ***********  */
map_game_ptrf_t* game_ptrf_new()
{
    return (map_game_ptrf_t*) malloc(sizeof (map_game_ptrf_t));
}

void game_ptrf_init(map_game_ptrf_t *map)
{
    map->clist = clist_gen_new();
    clist_gen_init( map->clist );
}

void game_ptrf_free(map_game_ptrf_t *map)
{
    if( map->clist ) {
        clist_gen_free( map->clist, ccontainer_delete_value);
        map->clist = NULL;
    }
    free(map);
}

size_t game_ptrf_size(const map_game_ptrf_t *map)
{
    return clist_gen_size( map->clist );
}

ccontainer_err_t game_ptrf_insert( map_game_ptrf_t *map, const char *name, ptr_game_t pfgame_in)
{
    ccontainer_err_t err_code;
    ccontainer_value_t value_in;

    /* call by caller, if access pair */
    pair_game_ptrf_t pair = make_pair_game_ptrf( name, pfgame_in);
    value_in = make_value_game_ptrf( &pair, &err_code );
    err_code = clist_gen_push_back( map->clist, &value_in );
    /* 2 intermediate to delete !! */
    ccontainer_delete_value( &value_in );
    delete_pair_game_ptrf( &pair );
    return err_code;
}

ptr_game_t game_ptrf_get_from_name(const map_game_ptrf_t *map, const char *name, ccontainer_err_t *err_code)
{
    pair_game_ptrf_t pair_out;
    ccontainer_value_t *pvalue_out;
    ccontainer_value_t name_value_search;
    /* not optimal, need intermediate because deletion of pair_out */
    ptr_game_t ptr_game_out;

    /*size_t len = strlen(name);*/
    name_value_search = make_value_cstring( name, err_code);
    if( *err_code != CCONTAINER_OK )
        return NULL;

    pvalue_out = clist_gen_find( map->clist, equalizer_value_cstring, &name_value_search, err_code );
    if( *err_code != CCONTAINER_OK )
        return NULL;

    /* create victory from value_clist */
    pair_out = extract_value_pair_game_ptrf( pvalue_out, err_code );
    ptr_game_out = pair_out.pfgame;

    /* deallocate local copy */
    deleter_value_cstring( &name_value_search );
    delete_pair_game_ptrf( &pair_out );

    return ptr_game_out;
}

clist_cstring_t* game_ptrf_get_list_names( const map_game_ptrf_t *map )
{
    clist_cstring_t *clist_str_out;

    clist_node_t *current_node;
    ccontainer_value_t *pcurrent_value;
    pair_game_ptrf_t pair_out;
    ccontainer_err_t err_code;

    clist_str_out = clist_cstring_new();
    clist_cstring_init( clist_str_out );

    current_node = clist_gen_get_first_node( map->clist );
    /* iterative copy of ccontainer_value_t::data into array of cstring */
    do {
        pcurrent_value = clist_gen_get_node_value( current_node );
        /* transform ccontainer_value_t into pair */
        pair_out = extract_value_pair_game_ptrf( pcurrent_value, &err_code );

        /* copy value when using push_back of clist_cstring */
        err_code = clist_cstring_push_back( clist_str_out, pair_out.game_name);
        /* deletetion necessary here */
        delete_pair_game_ptrf( &pair_out ); 

        current_node = clist_gen_get_next_node( current_node );
    } while( current_node != NULL );

    return clist_str_out;
}

cvector_cstring_t game_ptrf_get_names( const map_game_ptrf_t *map )
{
    pair_game_ptrf_t pair_out;
    cvector_cstring_t vect_str_out;
    clist_node_t *current_node;
    ccontainer_value_t *pcurrent_value;

    size_t len_names;
    ccontainer_err_t err_code;

    len_names = game_ptrf_size( map );
    if( len_names == 0 ) {
        cvector_cstring_init( &vect_str_out);
        return vect_str_out;
    }
    
    cvector_cstring_init_with_capacity( &vect_str_out, len_names );

    current_node = clist_gen_get_first_node( map->clist );
    /* iterative copy of ccontainer_value_t::data into array of cstring */
    do {
        pcurrent_value = clist_gen_get_node_value( current_node );
        /* transform ccontainer_value_t into pair */
        pair_out = extract_value_pair_game_ptrf( pcurrent_value, &err_code );

        /* copy value when using push_back of clist_cstring */
        err_code = cvector_cstring_push_back( &vect_str_out, pair_out.game_name);
        /* deletetion necessary here */
        delete_pair_game_ptrf( &pair_out ); 

        current_node = clist_gen_get_next_node( current_node );

    } while( current_node != NULL );

    return vect_str_out;

    
}

int game_ptrf_get_array_name( const map_game_ptrf_t *map, char ***array_out, size_t *array_len )
{
    pair_game_ptrf_t pair_out;
    clist_node_t *current_node;
    ccontainer_value_t *pcurrent_value;
    char **array = NULL;
    size_t lgen_len, indice;
    ccontainer_err_t err_code;

    /* default return in case of error, use swap idiom 
        may delete entries if already allocated / or append data ? */
    *array_out = NULL;
    *array_len = 0;

    /* get size for allocation of array of cstring */
    if( (lgen_len = clist_gen_size( map->clist)) == 0 ) {
        return CCONTAINER_EMPTY;
    }
    array = (char**) malloc( lgen_len * sizeof(char*) );
    if( !array )
        return CCONTAINER_ALLOCERR;


    current_node = clist_gen_get_first_node( map->clist );
    indice = 0;
    /* iterative copy of ccontainer_value_t::data into array of cstring */
    do {
        pcurrent_value = clist_gen_get_node_value( current_node );
        /* transform ccontainer_value_t into pair */
        pair_out = extract_value_pair_game_ptrf( pcurrent_value, &err_code );

        array[indice] = (char*) malloc( strlen(pair_out.game_name) +1);
        strcpy( array[indice++], pair_out.game_name);
        
        delete_pair_game_ptrf( &pair_out );
        current_node = clist_gen_get_next_node( current_node );

    } while( current_node != NULL );

    /* finally swap output */
    *array_len = lgen_len;
    *array_out = array;

    return err_code;
}
