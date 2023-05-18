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
struct clist_node {
    /** ccontainer_value_t contained in the node */
    ccontainer_value_t value;
    /** pointer to the next clist_node */
    struct clist_node *next_node;
};

/** @brief Declaration of the list type.
 * Contains the first node and the actual size of the list.
 * \ingroup ccontainer_clist_gen_grp
*/
struct clist_type {
    /** First node of the list */
    struct clist_node* first_node;
    /** Number of actual elements stored in the list, it is not necessary for a list (vs vector)*/
    size_t len;
};

/* **** static functions *****/
/** \cond  Doxygen_Suprress */
/* a priori all could be const node*, need cast or other technique to check */
struct clist_node* get_node_last( struct clist_node *fnode );
struct clist_node* get_next_node( const struct clist_node* current_node);
struct clist_node* get_node_number( struct clist_node *fnode, size_t elem_nb);
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

/* need intermediate with clist_node , propagate deleter (of value) */
/* it is a method of nodes only in fact */
void clist_gen_delete(clist_gen_t *clist, deleter_value_t ptrf_deleter) 
{
    struct clist_node* next_node;
    struct clist_node* curr_node = clist->first_node;

   if( !curr_node ) {
        assert( clist->len == 0);
        return;
   }

   do {
        /* store next node before deleting the current one */
        next_node = get_next_node(curr_node);
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
ccontainer_err clist_gen_push_front(clist_gen_t *clist, ccontainer_value_t *value_in)
{
    /* store value of the first node */
    struct clist_node *first_node_copy = clist->first_node;

    /* creation and copy of value_in into a new node */
    struct clist_node *p_new_node = (struct clist_node*) malloc( sizeof(struct clist_node));
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

/** arument by pointer, avoid a copy 
 * push_back , const not ? but use of move */
ccontainer_err clist_gen_push_back(clist_gen_t *clist, ccontainer_value_t *value_in)
{
    struct clist_node *new_node;
    /* return a copy, may create the strange behaviour */
    struct clist_node *last_node = get_node_last(clist->first_node);
    /* create a  new node */
    new_node = (struct clist_node*) malloc( sizeof(struct clist_node));
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

/* get by number, not very "list", otherwise need to give access to clist_node interface in API
    not the point on this interface to get by number ? really needed ? */
ccontainer_value_t* clist_gen_get_at( const clist_gen_t *clist, size_t index, ccontainer_err *err_code)
{
    struct clist_node *node;
    if( index >= clist->len ) {
        *err_code = CCONTAINER_OUTOFRANGE;
        return NULL;
    }
    node = get_node_number( clist->first_node, index );
    *err_code = CCONTAINER_OK;
    return &(node->value);
}

ccontainer_value_t* clist_gen_find( const clist_gen_t *clist, 
                    const char* buffer, size_t buffer_len, ccontainer_err *err_code)
{
    ccontainer_value_t value_out;

    struct clist_node *curr_node = clist->first_node;

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
        curr_node = get_next_node( curr_node );
    } while ( curr_node != NULL );


    *err_code = CCONTAINER_NOT_FOUND;
    return NULL;
}

/** \cond Suppress_Doxygen */
/***** Private functions implementation ****/
struct clist_node* get_next_node(const struct clist_node* current_node) {
    assert( current_node );
    return current_node->next_node;
}

struct clist_node* get_node_last( struct clist_node *fnode )
{
    if( !fnode )
        return fnode;

    /* test before update to return the last valid node */
    while( fnode->next_node != NULL ) {
        fnode = get_next_node(fnode);
    }
    return fnode;
}

struct clist_node* get_node_number( struct clist_node *fnode, size_t elem_nb)
{
    size_t compteur = 0;
    struct clist_node *curr = fnode;
    struct clist_node *next;
    while( compteur != elem_nb ) {
        next = get_next_node( curr );
        compteur++;
        curr = next;
    }
    return curr;
}
/** \endcond */