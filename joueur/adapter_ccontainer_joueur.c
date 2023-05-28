#include <string.h>
#include <assert.h>

#include "joueur/adapter_ccontainer_joueur.h"
/* had to include in header, cannot forward (redefinition)
#include "joueur/list_joueur.h"
#include "joueur/cvector_joueur.h"
*/
/* an optimized version in ccontainer */
#include "ccontainer/ccontainer_utils.h"

/* joueur = person_t + map_victories
    => person_t = (char *) + 2 bool = 8+2x4 = 16 => name copy need memory allocation
    => map_victories = (clist_gen_t*) = 8 / All the map(clist* ) stays heap allocated in this version
    Total : 24

    Here map_victories kept in heap.

    Possible (better ?) linerize everything, all pair are name[20]+3*int
    Already written for a pair => just linearize all (limit < data.len is reached)

    No need of specifc duplicater for joueur, a copy of map_game_victories is done on the heap
*/
ccontainer_value_t make_value_joueur(const joueur_t *joueur_in, ccontainer_err_t *err_code)
{
    ccontainer_value_t value_out;
    
    char *p_name_tmp;
    const char *p_person_name; /* ref to the person name */
    map_game_victories_t map_copy;
    size_t size_joueur_t;

    /* only for debug 
    char **dbg_str;
    bool *dbg_bool1, *dbg_bool2;
    map_game_victories_t *dbg_cmap;
    size_t size_person_t
    size_person_t = sizeof(person_t);
    size_map_t = sizeof(map_game_victories_t);
    dbg_cmap = &(joueur_in->map_victories); 
    */

    ccontainer_reset_value(&value_out);

    size_joueur_t = sizeof(joueur_t);
    
    /* person_t.p_name */
    p_person_name = person_name( (const person_t*) joueur_in);
    p_name_tmp = (char *) malloc( strlen( p_person_name ) + 1 );
    if( !p_name_tmp ) {
        *err_code = CCONTAINER_ALLOCERR;
        return value_out;
    }
    strcpy( p_name_tmp, p_person_name );

    /* joueur_t.map_victories */
    map_copy = game_victories_copy( &(joueur_in->map_victories), err_code );
    if( *err_code != CCONTAINER_OK) {
        return value_out;
    }

    /* all allocation done, will throw after */
    value_out.len = size_joueur_t;
    value_out.data = (char *) malloc( size_joueur_t );

    /* Make a full copy of the input */
    memcpy(value_out.data, &p_name_tmp, sizeof(char *) );
    memcpy(value_out.data + sizeof(char *), &(joueur_in->person.is_daltonien), sizeof(bool));
    memcpy(value_out.data + sizeof(char *) + sizeof(bool), &(joueur_in->person.is_admin), sizeof(bool));
    /* copy of the cmap => need deep copy like person_name */
    /* first version, like the same reference to cmap.clist* */
    /*memcpy(value_out.data + sizeof(person_t), &(joueur_in->map_victories), sizeof(map_game_victories_t));*/
    /* second version, use new allocated copy */
    memcpy(value_out.data + sizeof(person_t), &map_copy, sizeof(map_game_victories_t));
    /* other possible, linearize all pairs of <game,victory> code for pair to reuse
        => increase a lot the size of each value_t, use len to know how much pair are present 'or header data' */

    /* dbg
    dbg_str = (char **) value_out.data;
    dbg_bool1 = (bool*) (value_out.data + sizeof(char*));
    dbg_bool2 = (bool*) (value_out.data + sizeof(char*)+sizeof(bool));
    dbg_cmap = (map_game_victories_t*) (value_out.data + sizeof(person_t));
    */

    *err_code = CCONTAINER_OK;
    return value_out;
}

/* Must return a copy of the value inserted in the ccontainer.
 Queue pop / push not be the case in fact ... */
ccontainer_err_t extract_value_joueur(const ccontainer_value_t *value_in, joueur_t *joueur_out)
{
    ccontainer_err_t err_code;
    char **p_name;
    map_game_victories_t *map_victories_value;
    
    /* extract person : to do joueur_init(...) or person_init + joueur with map_game_victories ? */
    p_name = (char **) value_in->data;
    joueur_out->person.pname = malloc( strlen( *p_name ) + 1);
    strcpy( joueur_out->person.pname, *p_name );
    /* 2 boolean, may memcopy both  */
    joueur_out->person.is_daltonien = *( (bool *) (value_in->data + sizeof(char *)) );
    joueur_out->person.is_admin = *( (bool *) (value_in->data + sizeof(char *) + sizeof(bool)) );

    /* extract map_victories in making a copy and return the last error code */
    map_victories_value = (map_game_victories_t*) (value_in->data + sizeof(person_t));
    joueur_out->map_victories = game_victories_copy( map_victories_value, &err_code );
    return err_code;
}

void joueur_deleter_value(ccontainer_value_t* value_in)
{
    map_game_victories_t *map;
    /*
    if( !value_in || !value_in->data)
        return;
    */
    /* first field for person.p_name */
    char **p_str = (char **)value_in->data;
    if( *p_str )
        free(*p_str);
    /*p_str = NULL;*/
    /* second field in joueur */
    map = (map_game_victories_t*) (value_in->data + sizeof(person_t));
    game_victories_delete( map );
    free(value_in->data);
    value_in->data = NULL;
    value_in->len = 0;
}

/* no needed yet 
ccontainer_value_t joueur_duplicater_value(const ccontainer_value_t *value_src, ccontainer_err_t *err_code)
{
    ccontainer_value_t value_out;
    // will call copy_constructor of map
    *err_code = CCONTAINER_OK;
    return value_out;
}
*/

/* implementation using generic utilities, not possible ?? */
/* 76 alloc free vs 104 free in test_ccontainer_joueur */
vector_joueur_t* list_joueur_to_vector_generic_impl( list_joueur_t *list)
{

    vector_joueur_t* vect_joueur_out;
    ccontainer_err_t err_code;

    vect_joueur_out = vector_joueur_new();
    vect_joueur_out->cvect = NULL;

    vect_joueur_out->cvect = transfert_clist_to_cvector_gen( (clist_gen_t*) list->clist , &err_code );
    list->len = 0;

    /* VERY IMPORTANT for correct destruction */
    cvector_gen_set_deleter( vect_joueur_out->cvect, joueur_deleter_value );
    return vect_joueur_out;
}


/* implementation using only high level method of ccontainer joueur */
vector_joueur_t* list_joueur_to_vector( list_joueur_t *list)
{
    vector_joueur_t *pvector_joueur;
    joueur_t joueur;
    size_t size_list;
    int status;

    size_list = list_joueur_size( list );
    if( size_list == 0 )
        return NULL;
     /* at least one, allocate vector out with correct capacity */   
    pvector_joueur = vector_joueur_new();
    vector_joueur_init_with_capacity( pvector_joueur, size_list );

    /* size > 0, first always valid ? not sure an invalid person is possible */
    joueur = list_joueur_pop_front( list );
    assert( person_status( (const person_t *) &joueur ) >= PERSON_VALID );

    while ( person_status( (const person_t *) &joueur ) >= PERSON_VALID ) 
    {
        /* joueur _status */
        status = vector_joueur_push_back( pvector_joueur, &joueur);
        /* status of the push, should not be a problem */
        assert( status == 0);
        /* deallocate memory of intermediate joueur_t */
        joueur_delete( &joueur );
        /* try to pop the next one*/
        joueur = list_joueur_pop_front( list );
    };
    /* same for memory even if invalid */
    joueur_delete( &joueur );
    return pvector_joueur;
}
