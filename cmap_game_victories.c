#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "cmap_game_victories.h"

/*** Serialization / Deserialization / Copy ? with value_t **********/
value_t make_value_victory( struct pair_game_victory_t pair )
{
    value_t value;
    char buffer[sizeof(struct pair_game_victory_t)];
    /* pointer to follow the serialization */
    char *p_buffer = buffer;
    /* maybe return value to use */    
    memcpy( p_buffer, pair.game_name, 20);
    p_buffer += 20;
    memcpy( p_buffer, &pair.victories, sizeof(victory_t) );
    
    value.data = (char*)malloc( sizeof(struct pair_game_victory_t) );
    memcpy( value.data, buffer, sizeof(struct pair_game_victory_t));

    /* again a copy, dr_memeroy */
    /* not possible buffer ? */
    
    value.len = sizeof(struct pair_game_victory_t);
    return value;
}

struct pair_game_victory_t extract_value_victory(const value_t *value)
{
    struct pair_game_victory_t pair;
    char *p = value->data;
    
    memcpy( pair.game_name, value->data, 20);
    p += 20;
    memcpy( &pair.victories, p, sizeof(victory_t) );
    return pair;
}
/* again the same function, should use a default */
static void deleter_pair_game_victory( value_t *value)
{
    if( value ) {
        free(value->data);
        value->data = NULL;
    }
    value->len = 0;
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

void game_victories_clear(cmap_game_victories_t *cmap)
{
    /* to test if not null ? */
    clist_gen_clear( cmap->clist, deleter_pair_game_victory);
    /* post condition before continuing */
    assert( cmap->clist != NULL);
    assert( clist_gen_size( cmap->clist) == 0);
    /* in clist_cstring: free clist
        here try to keep valid but empty cmap 
        not possible because clist is a pointer !! 
        on stack no other possibility to delelte (need object ! )*/
    free(cmap->clist);
    cmap->clist = NULL;
}

void game_victories_delete(cmap_game_victories_t *cmap)
{
    if( ! cmap->clist ) {
        /*clist_gen_del( cmap->clist, deleter_pair);*/
        game_victories_clear( cmap );
    }
    /* to de done only is declred on heap */
    free(cmap);
}

/*** Public API ****/
size_t game_victories_size(cmap_game_victories_t *cmap)
{
    return clist_gen_size( cmap->clist );
}

int game_victories_insert( cmap_game_victories_t *cmap, struct pair_game_victory_t pair)
{
    int retour;

    value_t value_clist = make_value_victory( pair );
    retour = clist_gen_push_back( cmap->clist, &value_clist );
    /* free_value if access */
    /*
    free(value_clist.data);
    value_clist.data = NULL;
    value_clist.len = 0;
    */
   free_value( &value_clist );
   return retour;
}

struct pair_game_victory_t game_victories_get_copy( cmap_game_victories_t *cmap, const char *name)
{
    int retour;
    struct pair_game_victory_t pair;
    value_t value_clist;
    
    retour = clist_gen_find( cmap->clist, &value_clist, name, strlen(name) );
    if( retour != CLIST_OK )
    {
        strcpy( pair.game_name, "invalid");
        return pair;
    }
    /* create victory from value_clist */
    pair = extract_value_victory( &value_clist );
    /* deallocate local copy */
    free(value_clist.data);

    return pair;
}

