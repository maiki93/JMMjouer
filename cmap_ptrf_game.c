
#include <stdio.h> /* for printf, to suppress */
#include <string.h>
#include <assert.h>

#include "cmap_ptrf_game.h"
#include "clist_generic.h"

/*
#include "victory.h"
#include "person.h" */
/* typedef for exact function signature => lots of dependencies
   may cast at a higher-level ?? */
/* typedef victory_t(*ptr_game_local_t)(person_t); */
/*typedef void(*ptr_game_local_t)(void*);*/

struct cmap_ptrf_game_type {
    clist_gen_t *clist; /* first data member, cast to (clist_gen_t*) valid */
};


/* intermediate for storage / value_t specialization 
    Contains heap-allocated memory, but same deleter for value_t
    Only (intermediate) pair must be carefully deallocated */
struct pair_ptr_game {
    char *game_name; /* variant to pair_game_victory_t, with dyn. allocated */
    ptr_game_local_t pfgame;
};

void delete_pair_ptr_game(struct pair_ptr_game *pair)
{
    if( pair->game_name) {
        free(pair->game_name);
        pair->game_name = NULL;
    }
    pair->pfgame = NULL;
}

/* for conversion with value_t 
    optimize more tahn cmap_game_victories */
static value_t make_value_ptr_game( struct pair_ptr_game *pair )
{
    value_t value_out;
    /* to follow the serailization */
   /* char *p_buffer; */

    /* => return 8 (x64) */
    /*printf("sizeof ptr %ld\n", (long)sizeof(ptr_game_local_t) ); */ /* => 8 */

    /* a c-string, include the terminaison caracter
       necessary to retrieve the second field (ptr_game)!
         or need to store the size of the string, more memory (blank forbidden) */
    value_out.len =  strlen(pair->game_name) + 1 /* c-string */
                     + sizeof (ptr_game_local_t) ; /* ptr_f */
    value_out.data = (char*)malloc( value_out.len );
    /* strcpy copy '\0' if possible 
        p_buffer not the end of the string, memcpy finally better ?*/
    /*p_buffer = */
    strcpy( value_out.data, pair->game_name);
    /*printf("p_buffer - data: %ld\n", (long)(p_buffer - value_out.data));*/ /* => 0, p_buffer useless */
    /* append ptr_f */
    /*memcpy( p_buffer, pair->pfgame, sizeof (ptr_game_local_t));*/
    /*memcpy( p_buffer, &pair->pfgame, 8);*/
    /* memcpy( p_buffer + strlen(pair->game_name) + 1, &pair->pfgame, 8); */
    /* could get until the first '\0' cstrpn() */
    memcpy( value_out.data + strlen(pair->game_name) + 1, &pair->pfgame, 8);

    return value_out;
}


static struct pair_ptr_game extract_value_ptr_game(const value_t* value_in)
{
    struct pair_ptr_game pair;
    size_t name_len;
    /* if well formatted, search for first '\0' */
    name_len = strlen(value_in->data);
    pair.game_name = (char*)malloc( name_len + 1);
    strcpy(pair.game_name, value_in->data);
    /* copy ptr_game */
    /*memcpy(pair.pfgame, value_in->data + name_len + 1, sizeof (ptr_game_local_t));*/
    memcpy(&pair.pfgame, value_in->data + name_len + 1, sizeof (ptr_game_local_t));

    return pair;
}

/* Could simulate a template with all those redondancies ?
    Or extract functions to reuse ? make_value / extract_value only change
            other recopy (make_value, 
            extract could be used as pointer to function, 
                problem with type : pair derivatives ....)
   a new with sizeof(cmap ) */

cmap_ptrf_game_t* game_ptrf_new()
{
    return (cmap_ptrf_game_t*) malloc(sizeof (cmap_ptrf_game_t));
}

void game_ptrf_init(cmap_ptrf_game_t *cmap)
{
    cmap->clist = clist_gen_new();
    clist_gen_init( cmap->clist );
}

/* again same deleter, really need to do something !! 
  deleter value in fact */
static void  deleter_pair_ptr_game( value_t *value)
{
    if( value ) {
        free(value->data);
        value->data = NULL;
    }
    value->len = 0;
}
/* only use with dynamic allocation */
void game_ptrf_delete(cmap_ptrf_game_t *cmap)
{
    if( cmap->clist ) {
        /*clist_gen_clear( cmap->clist, deleter_pair_ptr_game);
        assert( cmap->clist != NULL);
        assert( clist_gen_size( cmap->clist) == 0);
        free(cmap->clist); */

        clist_gen_del( cmap->clist, free_value);
        cmap->clist = NULL;
    }
    free(cmap);
}

struct pair_ptr_game make_pair(const char *name, ptr_game_local_t pfgame_in)
{
    struct pair_ptr_game pair;
    pair.game_name = (char*)malloc(strlen(name) + 1);
    strcpy(pair.game_name, name);
    pair.pfgame = pfgame_in;
    return pair;
}

/* behaviori, again mainly call to clist_gen */
size_t game_ptrf_size(const cmap_ptrf_game_t *cmap)
{
    return clist_gen_size( cmap->clist );
}

/* pair input certainly better */
int game_ptrf_insert( cmap_ptrf_game_t *cmap, const char *name, ptr_game_local_t pfgame_in)
{
    int retour;
    value_t value_in;
    /* call by caller, if access pair */
    struct pair_ptr_game pair = make_pair( name, pfgame_in);
    value_in = make_value_ptr_game( &pair );

    retour = clist_gen_push_back( cmap->clist, &value_in );
    /* 2 intermediate to delete !! */
    free_value( &value_in );
    delete_pair_ptr_game( &pair );
    return retour;
}

ptr_game_local_t game_ptrf_get_from_name(const cmap_ptrf_game_t *cmap, const char *name)
{
    int retour;
    struct pair_ptr_game pair_out;
    value_t value_out;

    retour = clist_gen_find( cmap->clist, &value_out, name, strlen(name) );
    if( retour != CLIST_OK )
    {
        /*strcpy( pair_out.game_name, "invalid");*/
        return NULL;
    }
    /* create victory from value_clist */
    pair_out = extract_value_ptr_game( &value_out );
    /* deallocate local copy */
    free_value( &value_out );
    free(pair_out.game_name);

    return pair_out.pfgame;
}



clist_cstring_t* game_ptrf_get_names( const cmap_ptrf_game_t *cmap )
{
    value_t value_out;
    struct pair_ptr_game pair_out;
    size_t lgen_len, indice;
    int retour;

    clist_cstring_t *clist_str;

    clist_str = clist_cstring_new();
    clist_cstring_init( clist_str );

    lgen_len = game_ptrf_size( cmap );
    /* iterative copy of value_t::data into array of cstring */
    for( indice = 0; indice < lgen_len; indice++) {
        if( (retour = clist_gen_get_value_copy( cmap->clist, indice, &value_out)) 
                    != CLIST_OK ) {
            return NULL;
        }
        /* transform value_t into pair */
        pair_out = extract_value_ptr_game( &value_out );
        /* makes copy, stealing pointer possible ?? */
        clist_cstring_push_back( clist_str, pair_out.game_name);
        
        delete_pair_ptr_game( &pair_out );
        free_value( & value_out );
    }
    return clist_str;
}

/* first version, set deprecated */
int game_ptrf_get_array_name( const cmap_ptrf_game_t *cmap, char ***array_out, size_t *array_len )
{
    value_t value_out;
    struct pair_ptr_game pair_out;
    char **array = NULL; /* use local only */
    size_t lgen_len, indice;
    int retour = 0;

    /* default return in case of error, use swap idiom 
        may delete entries if already allocated / or append data ? */
    *array_out = NULL;
    *array_len = 0;

     /* get size for allocation of array of cstring */
    if( (lgen_len = clist_gen_size( cmap->clist)) == 0 ) {
        return CLIST_EMPTY;
    }
    array = (char**) malloc( lgen_len * sizeof(char*) );
    if( !array )
        return CLIST_ALLOC_ERR;
    /* iterative copy of value_t::data into array of cstring */
    for( indice = 0; indice < lgen_len; indice++) {
        if( (retour = clist_gen_get_value_copy( cmap->clist, indice, &value_out)) 
                    != CLIST_OK ) {
            return retour;
        }
        /* transform value_t into pair */
        pair_out = extract_value_ptr_game( &value_out );
        /* makes copy, stealing pointer also possible ?? */
        array[indice] = (char*) malloc( strlen(pair_out.game_name) +1);
        strcpy( array[indice], pair_out.game_name);
        
        delete_pair_ptr_game( &pair_out );
        free_value( & value_out );
    }
    /* finally swap output */
    *array_len = lgen_len;
    *array_out = array;

    return retour;
}