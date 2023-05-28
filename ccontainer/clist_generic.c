#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "ccontainer/clist_generic.h"

/** @file
    Implementation file
*/

/** Nodes of the list. 
 * Structure kept in internal implementation
 * \private
 * \ingroup ccontainer_clist_gen_grp
*/
struct clist_node_type {
    /** ccontainer_value_t contained in the node */
    ccontainer_value_t value;
    /** pointer to the next clist_node */
    struct clist_node_type *next_node;
};

/** @brief Declaration of the list type.
 * Contains the first node and the actual size of the list.
 * \ingroup ccontainer_clist_gen_grp
*/
struct clist_type {
    /** First node of the list */
    clist_node_t* first_node;
    /** Number of actual elements stored in the list, it is not necessary for a list (vs vector)*/
    size_t len;
};

/* **** static functions *****/
/** \cond  Doxygen_Suprress */
/* a priori all could be const node*, need cast or other technique to check */
static clist_node_t* get_node_last( clist_node_t *node );
static clist_node_t* get_node_number( clist_node_t *node, size_t elem_nb);
/** \endcond */

/** Constructor/ destructor clist_gen */
clist_gen_t* clist_gen_new() 
{
    return (clist_gen_t*) malloc( sizeof(clist_gen_t) );
}

void clist_gen_init(clist_gen_t* clist)
{
    clist->first_node = NULL;
    clist->len = 0;
}

clist_gen_t* clist_gen_copy(const clist_gen_t* clist_src, duplicater_value_t ptrf_duplicater,
                            ccontainer_err_t *err_code )
{

    clist_node_t* next_node;
    clist_node_t* curr_node = clist_src->first_node;

    clist_gen_t *clist_dest;
    ccontainer_value_t tmp_value;
    
    clist_dest = clist_gen_new();
    if( !clist_dest ) {
        *err_code = CCONTAINER_ALLOCERR;
        return NULL;
    }
    clist_gen_init( clist_dest );
    /* clist is valid, empty */
    if( !curr_node ) {
        assert( clist_src->len == 0);
        return clist_dest;
    }

    do {
        /* store next node before deleting the current one */
        next_node = clist_gen_get_next_node(curr_node);

/* only changing part of the loop  : for_each( clist_dest, function (with duplicater...)) to think ! */
        /* copy the current one and insert intro a the new clist, same order */
        tmp_value = (*ptrf_duplicater)(&curr_node->value, err_code);
        if( *err_code != CCONTAINER_OK ) {
            /* or null, miss type of error ? */
            return clist_dest;
        }
        clist_gen_push_back( clist_dest, &tmp_value );
        curr_node = next_node; 
   } while (curr_node != NULL);

    assert( clist_src->len == clist_dest->len );
    return clist_dest;
}

/* need intermediate with clist_node , propagate deleter (of value) */
/* it is a method of nodes only in fact */
void clist_gen_delete(clist_gen_t *clist, deleter_value_t ptrf_deleter) 
{
    clist_node_t* next_node;
    clist_node_t* curr_node = clist->first_node;

   if( !curr_node ) {
        assert( clist->len == 0);
        return;
   }

   do {
        /* store next node before deleting the current one */
        next_node = clist_gen_get_next_node(curr_node);
        /* delete the current one */
        (*ptrf_deleter)(&curr_node->value);
        curr_node->next_node = NULL;
        free( curr_node );
        curr_node = NULL;
        
        clist->len--;
        curr_node = next_node; 
   } while (curr_node != NULL);
   /* all has been deleted, clist.first_node can be safetly nullified */
   clist->first_node = NULL;
   assert( clist->len == 0);
}

void clist_gen_free(clist_gen_t* clist, deleter_value_t ptrf_deleter) 
{
    clist_gen_delete(clist, ptrf_deleter);
    free( clist );
}

/*** Public API implementation ****/
size_t clist_gen_size(const clist_gen_t *clist)
{
    return clist->len;
}

/* push_front */
ccontainer_err_t clist_gen_push_front(clist_gen_t *clist, ccontainer_value_t *value_in)
{
    /* store value of the first node */
    clist_node_t *first_node_copy = clist->first_node;

    /* creation and copy of value_in into a new node */
    clist_node_t *p_new_node = (clist_node_t*) malloc( sizeof(clist_node_t));
    if( !p_new_node )
        return CCONTAINER_ALLOCERR;
    p_new_node->value = ccontainer_move_value( value_in );
    
    /* reassign first node to the new one, ok if NULL */
    p_new_node->next_node = first_node_copy;
    /* clist point to the new one */
    clist->first_node = p_new_node;
    clist->len++;
    return CCONTAINER_OK;
}

ccontainer_err_t clist_gen_push_back(clist_gen_t *clist, ccontainer_value_t *value_in)
{
    clist_node_t *new_node;
    /* pointer to last node, a reference */
    clist_node_t *last_node = get_node_last(clist->first_node);
    /* create a  new node */
    new_node = (clist_node_t*) malloc( sizeof(clist_node_t));
    if( !new_node )
        return CCONTAINER_ALLOCERR;

    new_node->value = ccontainer_move_value( value_in );
    new_node->next_node = NULL;
    /* specific for first one, 
        would need the adress of the pointer (and *node = new_node) to avoid the if ? */
   if( last_node == NULL)
        clist->first_node = new_node;
    else
        last_node->next_node = new_node;

    clist->len++;
    return CCONTAINER_OK;
}

/*
void clist_gen_remove_node(clist_gen_t *clist, clist_node_t* node, deleter_value_t ptrf_deleter )
{
    clist_node_t *prev_node = clist->
}*/



/* get by number, not very "list", otherwise need to give access to clist_node interface in API
    not the point on this interface to get by number ? really needed ? */
ccontainer_value_t* clist_gen_get_at( const clist_gen_t *clist, size_t index, ccontainer_err_t *err_code)
{
    clist_node_t *node;
    if( index >= clist->len ) {
        *err_code = CCONTAINER_OUTOFRANGE;
        return NULL;
    }
    node = get_node_number( clist->first_node, index );
    *err_code = CCONTAINER_OK;
    return &(node->value);
}

ccontainer_value_t* clist_gen_find( const clist_gen_t *clist, 
                    const char* buffer, size_t buffer_len, ccontainer_err_t *err_code)
{
    ccontainer_value_t value_out;

    clist_node_t *curr_node = clist->first_node;

    ccontainer_reset_value(&value_out);
    
    if( clist->first_node == NULL) {
        *err_code = CCONTAINER_EMPTY;
        return NULL;
    }
    /* loop over nodes */
    do {
        /* comparison criteria */
        if( memcmp( curr_node->value.data, buffer, buffer_len) == 0 ) {
            /*value_out = ccontainer_copy_value( &(curr_node->value), err_code );*/
            *err_code = CCONTAINER_OK;
            return &(curr_node->value);
        }
        /* update node */
        curr_node = clist_gen_get_next_node( curr_node );
    } while ( curr_node != NULL );

    *err_code = CCONTAINER_NOT_FOUND;
    return NULL;
}

/*
cvector_gen_t* clist_move_to_cvector(clist_gen_t* clist, ccontainer_err_t *err_code)
{
    clist_node_t* curr_node = clist->first_node;

    cvector_gen_t* cvect_out;
    ccontainer_value_t value;

    if( !curr_node ) {
        assert( clist->len == 0);
        return NULL;
    }

    cvect_out = cvector_gen_new();
    cvector_gen_init_with_capacity( cvect_out,
                                    clist_gen_size(clist));

    do {
        value = ccontainer_move_value( &(curr_node->value) );
        cvector_gen_push_back( cvect_out, &value );
        // update node
        curr_node = clist_gen_get_next_node( curr_node );
    } while ( curr_node != NULL );

    return cvect_out;
}
*/

ccontainer_value_t clist_gen_pop_front_node_value(clist_gen_t* clist, ccontainer_err_t *err_code )
{
    ccontainer_value_t value_out;
    clist_node_t *second_node; /* second will become first */
    assert(clist);
    ccontainer_reset_value(&value_out);

    /* list empty */
    if( clist->first_node == NULL ) {
        *err_code = CCONTAINER_EMPTY;
        return value_out;
    }

    /* transfert value_t  */
    value_out = ccontainer_move_value( &(clist->first_node->value) );
    
    second_node = clist->first_node->next_node;
    /* default deleter_value because content has been moved : delete_first_node(clist, NULL) */
    ccontainer_delete_value( &(clist->first_node->value));
    free( clist->first_node );
    /* reasign the new first node */
    clist->first_node = second_node;
    clist->len--;

    *err_code = CCONTAINER_OK;
    return value_out;
}

clist_node_t* clist_gen_get_first_node( const clist_gen_t* clist)
{
    assert( clist );
    return clist->first_node; 
}

clist_node_t* clist_gen_get_next_node(const clist_node_t* current_node)
{
    assert( current_node );
    assert( current_node != NULL );
    return current_node->next_node;
}

/***** Private functions implementation ****/
/** \cond Suppress_Doxygen */
clist_node_t* get_node_last( clist_node_t *fnode )
{
    if( !fnode )
        return fnode;

    /* test before update to return the last valid node */
    while( fnode->next_node != NULL ) {
        fnode = clist_gen_get_next_node(fnode);
    }
    return fnode;
}

clist_node_t* get_node_number( clist_node_t *fnode, size_t elem_nb)
{
    size_t compteur = 0;
    clist_node_t *curr = fnode;
    clist_node_t *next;
    while( compteur != elem_nb ) {
        next = clist_gen_get_next_node( curr );
        compteur++;
        curr = next;
    }
    return curr;
}
/** \endcond */