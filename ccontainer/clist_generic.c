#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "ccontainer/clist_generic.h"

/** @file
    Implementation file */

/** Nodes of the list.
 * Structure kept in internal implementation
 * \ingroup ccontainer_clist_gen_grp */
struct clist_node_type {
    /** ccontainer_value_t contained in the node */
    ccontainer_value_t value;
    /** pointer to the previous clist_node */
    struct clist_node_type *prev_node;
    /** pointer to the next clist_node */
    struct clist_node_type *next_node;
}; /* clist_node_t */

/** Declaration of the list type.
 * Contains the first node and the actual size of the list.
 * \ingroup ccontainer_clist_gen_grp */
struct clist_type {
    /** First node of the list */
    clist_node_t* first_node;
};  /* clist_gen_t */

/* delete method in implementation because not possible to allocate on stack */
static void clist_gen_delete(clist_gen_t *clist, deleter_value_t ptrf_deleter); 

/* **** static functions *****/
/** \cond  Doxygen_Suprress */
/* a priori all could be const node*, need cast or other technique to check */
static clist_node_t* get_last_node( const clist_node_t *a_node );
/* current_node must not be null */
static size_t count_node_from( const clist_node_t *current_node );
/** \endcond */

/** Constructor/ destructor clist_gen */
clist_gen_t* clist_gen_new() 
{
    return (clist_gen_t*) malloc( sizeof(clist_gen_t) );
}

void clist_gen_init(clist_gen_t* clist)
{
    clist->first_node = NULL;
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
        assert( clist_gen_size(clist_src) == 0);
        *err_code = CCONTAINER_EMPTY;
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

    assert( clist_gen_size(clist_src) == clist_gen_size(clist_dest) );
    return clist_dest;
}

/* need intermediate with clist_node , propagate deleter (of value) */
/* it is a method of nodes only in fact */
void clist_gen_delete(clist_gen_t *clist, deleter_value_t ptrf_deleter) 
{
    clist_node_t* next_node;
    clist_node_t* curr_node = clist->first_node;

   if( !curr_node ) {
        assert( clist_gen_size( clist ) == 0);
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

        curr_node = next_node;
   } while (curr_node != NULL);
   /* all has been deleted, first_node can be safetly nullified */
   clist->first_node = NULL;
}

void clist_gen_free(clist_gen_t* clist, deleter_value_t ptrf_deleter) 
{
    clist_gen_delete(clist, ptrf_deleter);
    free( clist );
}

/*** Public API implementation ****/
size_t clist_gen_size(const clist_gen_t *clist)
{
    if( !clist->first_node )
        return 0;
    return count_node_from(clist->first_node);
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
    p_new_node->prev_node = NULL;
    /* clist point to the new one */
    clist->first_node = p_new_node;
    /*clist->len++;*/
    return CCONTAINER_OK;
}

ccontainer_err_t clist_gen_push_back(clist_gen_t *clist, ccontainer_value_t *value_in)
{
    clist_node_t *new_node;
    /* pointer to last node, a reference */
    clist_node_t *last_node = get_last_node(clist->first_node);
    /* create a  new node */
    new_node = (clist_node_t*) malloc( sizeof(clist_node_t));
    if( !new_node )
        return CCONTAINER_ALLOCERR;

    new_node->value = ccontainer_move_value( value_in );
    /* last one always */
    new_node->prev_node = last_node;
    new_node->next_node = NULL;
    /* specific for first one, 
        would need the adress of the pointer (and *node = new_node) to avoid the if ? */
   if( last_node == NULL) {
        assert( clist->first_node == NULL);
        clist->first_node = new_node;
        clist->first_node->prev_node = NULL;
   } else
        last_node->next_node = new_node;
    return CCONTAINER_OK;
}

ccontainer_value_t* clist_gen_find( const clist_gen_t *clist,
                        equalizer_value_t fct_equalizer, const ccontainer_value_t* value_to_match,
                        ccontainer_err_t *err_code)
{
    clist_node_t *node_match = clist_gen_find_node( clist, NULL, fct_equalizer, value_to_match, err_code);
    
    return (node_match != NULL) ? &(node_match->value)
                                 : NULL;
}

ccontainer_value_t clist_gen_pop_front(clist_gen_t* clist, ccontainer_err_t *err_code )
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
    
    *err_code = CCONTAINER_OK;
    return value_out;
}

clist_node_t* clist_gen_get_first_node( const clist_gen_t* clist)
{
    assert( clist );
    /* NULL <=> empty clist */
    return clist->first_node; 
}

clist_node_t* clist_gen_get_last_node( const clist_gen_t *clist)
{
    assert(clist);
    if( !clist->first_node ) {
        return NULL;
    }

    return get_last_node( clist->first_node);
}

clist_node_t* clist_gen_get_next_node(const clist_node_t* current_node)
{
    assert( current_node );
    return current_node->next_node;
}

clist_node_t* clist_gen_get_previous_node(const clist_node_t* current_node)
{
    assert( current_node );
    return current_node->prev_node;
}

ccontainer_value_t* clist_gen_get_node_value(const clist_node_t* current_node)
{
    assert(  current_node );
    return (ccontainer_value_t*) &(current_node->value);
}

clist_node_t* clist_gen_find_node( const clist_gen_t *clist, const clist_node_t *first_node,
                        equalizer_value_t fct_equalizer, const ccontainer_value_t* value_to_match,
                        ccontainer_err_t *err_code)
{
    clist_node_t *curr_node;

    assert( clist );
    /* case list empty */
    if( clist->first_node == NULL) {
        *err_code = CCONTAINER_EMPTY;
        return NULL;
    }

    /* check first node to start iteration, discard const but contract respected */
    curr_node = (first_node != NULL) ? (clist_node_t*) first_node
                                     : clist->first_node;
    do { 
        if( (*fct_equalizer)( &(curr_node->value), value_to_match) ) {
            /* a first match */
            *err_code = CCONTAINER_OK;
            return curr_node;
        }
        /* update node */
        curr_node = clist_gen_get_next_node( curr_node );
    } while( curr_node != NULL );

    /* not found, retunr NULL */
    *err_code = CCONTAINER_NOT_FOUND;
    return NULL;
}

clist_gen_t* clist_gen_find_all(const clist_gen_t *clist,
                        equalizer_value_t fct_equalizer, const ccontainer_value_t* value_to_match,
                        duplicater_value_t fct_duplicater, ccontainer_err_t *err_code)
{
    clist_gen_t *clist_out;
    clist_node_t* node_match, *node_start_match;
    ccontainer_value_t pvalue_copy;
    /* check / assert */
    assert( clist );

    clist_out = NULL;
    /* case input empty */
    if( ! clist->first_node ) {
        *err_code = CCONTAINER_EMPTY;
        return NULL;
    }

    node_start_match = clist->first_node;
    /* call recursive to clist_find_node */
    while( (node_match = clist_gen_find_node(clist, node_start_match, fct_equalizer, value_to_match, err_code) )
                       != NULL )
    {
        /* assert( node_match != NULL ); */
        /* value_copy is copy then moved into the list out */
        pvalue_copy = (*fct_duplicater)( &(node_match->value), err_code );
        /* check err_code for allocation ... what to do ? => abort ? */
        /* create only once */
        if( !clist_out ) {
            clist_out = clist_gen_new();
            clist_gen_init( clist_out );
        };
        *err_code = clist_gen_push_back( clist_out, &pvalue_copy );
        /* will be null if end of list, if not exit start again for start */
        if( node_match->next_node == NULL )
            break;
        node_start_match = node_match->next_node;
    };

    /* *err_code = CCONTAINER_OK; */
    /* NULL if no match */
    return clist_out;
}

SHARED_EXPORT ccontainer_err_t clist_gen_remove_node(clist_gen_t *clist, clist_node_t* node_to_delete, deleter_value_t ptrf_deleter )
{
    ccontainer_err_t err_code;
    clist_node_t *node_before, *node_after;
    assert(clist);
    
    if( !node_to_delete ) {
        err_code = CCONTAINER_NOT_FOUND; /* INVALID_NODE / INVALID_CCONTAINER better */
        return err_code;
    }

    node_before = node_to_delete->prev_node;
    node_after = node_to_delete->next_node;
    /* node_to_delete not the first node */
    if( node_before )
        node_before->next_node = node_after;
    /* we delete the first node */
    else 
        clist->first_node = node_after;
    
    /* node_to_delete not the last_node */
    if( node_after )
        node_after->prev_node = node_before;

    /* delete the current value and node */
    (*ptrf_deleter)( &(node_to_delete->value) );
    free(node_to_delete);

    err_code = CCONTAINER_OK;
    return err_code;
}

/***** Private functions implementation ****/
/** \cond Suppress_Doxygen */
clist_node_t* get_last_node( const clist_node_t *a_node )
{
    clist_node_t *local_node = (clist_node_t*) a_node;
    if( !local_node )
        return NULL;
    /* test before update to return the last valid node */
    while( local_node->next_node != NULL ) {
        local_node = clist_gen_get_next_node(local_node);
    }
    return local_node;
}

size_t count_node_from( const clist_node_t *current_node )
{
    clist_node_t *node, *next;
    size_t counter = 0;

    assert(current_node);
    /* current node valid, at least one */
    if( current_node->next_node == NULL )
        return 1;

    counter = 1;
    node = (clist_node_t*) current_node; /* discard const */
    while ( (next = clist_gen_get_next_node( node ) ) != NULL ) {
        counter++;
        node = next;
    }
    return counter;
}

/** \endcond */