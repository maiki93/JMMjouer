#include <stdio.h> /* for printf, to suppress */
#include <string.h>
#include <assert.h>

#include "jmmjouer/game_loader/cmap_game_ptrf.h"

struct cmap_game_ptrf_type {
    /** first data member, cast to (clist_gen_t*) valid */
    clist_gen_t *clist; 
};

/* intermediate for storage / ccontainer_value_t specialization 
    Contains heap-allocated memory, but same deleter for ccontainer_value_t
    Only (intermediate) pair must be carefully deallocated */
/* variant to pair_game_victory_t, with dyn. allocated */
struct pair_game_ptrf {
    char *game_name; 
    ptr_game_t pfgame;
};

/*  ********* Adapter functions ************* */
/* for conversion with ccontainer_value_t
  choice of serialization different of cmap_game_victories :
  - copy all the string name + ptrf into the data, no pointer to allocated memory in value_t
    use null caracter to end the string
  - copy the pointer to function */
static ccontainer_value_t make_value_game_ptrf( struct pair_game_ptrf *pair, ccontainer_err_t *err_code)
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

static struct pair_game_ptrf extract_value_pair_game_ptrf(const ccontainer_value_t* value_in, ccontainer_err_t *err_code)
{
    struct pair_game_ptrf pair;
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
struct pair_game_ptrf make_pair_game_ptrf(const char *name, ptr_game_t pfgame_in)
{
    struct pair_game_ptrf pair;
    pair.game_name = (char*)malloc(strlen(name) + 1);
    strcpy(pair.game_name, name);
    pair.pfgame = pfgame_in;
    return pair;
}

void delete_pair_game_ptrf(struct pair_game_ptrf *pair)
{
    if( pair->game_name) {
        free(pair->game_name);
        pair->game_name = NULL;
    }
    pair->pfgame = NULL;
}

/*  ****** Constructor / Destructor ***********  */
cmap_game_ptrf_t* game_ptrf_new()
{
    return (cmap_game_ptrf_t*) malloc(sizeof (cmap_game_ptrf_t));
}

void game_ptrf_init(cmap_game_ptrf_t *cmap)
{
    cmap->clist = clist_gen_new();
    clist_gen_init( cmap->clist );
}

/* only use with dynamic allocation */
void game_ptrf_free(cmap_game_ptrf_t *cmap)
{
    if( cmap->clist ) {
        clist_gen_free( cmap->clist, ccontainer_delete_value);
        cmap->clist = NULL;
    }
    free(cmap);
}

/* behaviori, again mainly call to clist_gen */
size_t game_ptrf_size(const cmap_game_ptrf_t *cmap)
{
    return clist_gen_size( cmap->clist );
}

/* pair input certainly better */
ccontainer_err_t game_ptrf_insert( cmap_game_ptrf_t *cmap, const char *name, ptr_game_t pfgame_in)
{
    ccontainer_err_t err_code;
    ccontainer_value_t value_in;

    /* call by caller, if access pair */
    struct pair_game_ptrf pair = make_pair_game_ptrf( name, pfgame_in);
    value_in = make_value_game_ptrf( &pair, &err_code );
    err_code = clist_gen_push_back( cmap->clist, &value_in );
    /* 2 intermediate to delete !! */
    ccontainer_delete_value( &value_in );
    delete_pair_game_ptrf( &pair );
    return err_code;
}

ptr_game_t game_ptrf_get_from_name(const cmap_game_ptrf_t *cmap, const char *name, ccontainer_err_t *err_code)
{
    struct pair_game_ptrf pair_out;
    ccontainer_value_t *pvalue_out;
    /* not optimal, need intermediate because deletion of pair_out */
    ptr_game_t out;

    size_t len = strlen(name);
    pvalue_out = clist_gen_find( cmap->clist, name, len, err_code );
    if( *err_code != CCONTAINER_OK )
    {
        /*strcpy( pair_out.game_name, "invalid");*/
        return NULL;
    }
    /* create victory from value_clist */
    pair_out = extract_value_pair_game_ptrf( pvalue_out, err_code );
    /* deallocate local copy */
    /*free_value( &value_out );*/
    /*free(pair_out.game_name);*/
    out = pair_out.pfgame;
    delete_pair_game_ptrf( &pair_out );

    return out; /*pair_out.pfgame;*/
}

clist_cstring_t* game_ptrf_get_names( const cmap_game_ptrf_t *cmap )
{
    ccontainer_value_t* pvalue_out;
    struct pair_game_ptrf pair_out;
    size_t lgen_len, indice;
    
    ccontainer_err_t err_code;
    clist_cstring_t *clist_str;

    clist_str = clist_cstring_new();
    clist_cstring_init( clist_str );

    lgen_len = game_ptrf_size( cmap );
    /* iterative copy of ccontainer_value_t::data into array of cstring */
    for( indice = 0; indice < lgen_len; indice++) {
        if( (pvalue_out = clist_gen_get_at( cmap->clist, indice, &err_code) ) 
                    == NULL ) {
            return NULL;
        }
        /* transform ccontainer_value_t into pair */
        pair_out = extract_value_pair_game_ptrf( pvalue_out, &err_code );
        /* makes copy, stealing pointer possible ?? */
        err_code = clist_cstring_push_back( clist_str, pair_out.game_name);
        
        delete_pair_game_ptrf( &pair_out );
        /*free_value( pvalue_out );*/
    }
    return clist_str;
}

/* first version, set deprecated */
int game_ptrf_get_array_name( const cmap_game_ptrf_t *cmap, char ***array_out, size_t *array_len )
{
    struct pair_game_ptrf pair_out;
    ccontainer_value_t *pvalue_out;
    char **array = NULL; /* use local only */
    size_t lgen_len, indice;
    ccontainer_err_t err_code;

    /* default return in case of error, use swap idiom 
        may delete entries if already allocated / or append data ? */
    *array_out = NULL;
    *array_len = 0;

     /* get size for allocation of array of cstring */
    if( (lgen_len = clist_gen_size( cmap->clist)) == 0 ) {
        return CCONTAINER_EMPTY;
    }
    array = (char**) malloc( lgen_len * sizeof(char*) );
    if( !array )
        return CCONTAINER_ALLOCERR;
    /* iterative copy of ccontainer_value_t::data into array of cstring */
    for( indice = 0; indice < lgen_len; indice++) {
        if( (pvalue_out = clist_gen_get_at( cmap->clist, indice, &err_code)) 
                    == NULL ) {
            return err_code;
        }
        /* transform ccontainer_value_t into pair */
        pair_out = extract_value_pair_game_ptrf( pvalue_out, &err_code );
        /* makes copy, stealing pointer also possible ?? */
        array[indice] = (char*) malloc( strlen(pair_out.game_name) +1);
        strcpy( array[indice], pair_out.game_name);
        
        delete_pair_game_ptrf( &pair_out );
        /*free_value( & value_out );*/
    }
    /* finally swap output */
    *array_len = lgen_len;
    *array_out = array;

    return err_code;
}