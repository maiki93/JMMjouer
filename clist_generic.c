#include <stdio.h> /* only for print, to delete later if use log only */
/*#include <stdlib.h>*/
#include <string.h>
#include <assert.h>

#include "clist_generic.h"
#include "clogger.h"

struct clist_node {
    value_t value;
    struct clist_node *next_node;
};

struct clist_type {
    struct clist_node* first_node;
    size_t len;  /* not necessary for list */
    /* todo in specialization of clist_generic, when needed */
    /* pointer on current node for algorithm memorization.. or use internal only : thread-safe reading */
    /* pointer on last node for faster push_back */
};

/* a priori all could be const node*, need cast or other technique to check */
struct clist_node* get_node_last( struct clist_node *fnode );
struct clist_node* get_next_node( const struct clist_node* current_node);
struct clist_node* get_node_number( struct clist_node *fnode, size_t elem_nb);

/** methods to create/delete value_t ***/
/* declare const char* for usage, but cast used to allow the assignment.
    make value_t.data const char* ? (usage make sense...TODO check) */
value_t make_value(char *data, size_t len) {
    value_t value;
    value.data = data; /* (char*) if pass const char* */
    value.len = len;
    return value;
}

value_t copy_value(const char *data, size_t len) {
    char *buf = (char*)malloc( len * sizeof(char) );
    memcpy( buf, data, len);
    return make_value(buf, len);
}

void free_value(value_t* value) {
    if( value && value->data ) {
        free(value->data);
        value->data = NULL;
        value->len = 0;
    }
}

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

void clist_gen_del(clist_gen_t* clist, deleter_t deleter) {
    clist_gen_clear(clist, deleter);
    free( clist );
}

/* need intermediate with clist_node , propagate deleter (of value) */
/* it is a method of nodes only in fact */
void clist_gen_clear(clist_gen_t *clist, deleter_t deleter) 
{
    struct clist_node* next_node;
    struct clist_node* curr_node = clist->first_node;

   if(curr_node == NULL) {
        printf("First node is NULL, nothing to delelte\n");
        CLOG_DEBUG("First node is NULL, nothing to delete, size: %d\n",clist->len);
        return;
   }

   do {
        /* store next node before deleting the current one */
        next_node = get_next_node(curr_node);
        /* delete the current one */
        deleter(&curr_node->value);
        curr_node->next_node = NULL;
        free( curr_node );
        curr_node = NULL;
        
        clist->len --;
        curr_node = next_node; 
   } while (curr_node != NULL);
   /* all has been deleted, clist.first_node safetly nullified */
   clist->first_node = NULL;
   assert( clist->len == 0);
}

/*** Public API implementation ****/
size_t clist_gen_size(const clist_gen_t *clist)
{
    return clist->len;
}

/* push_front */
int clist_gen_push_front(clist_gen_t *clist, value_t value_in)
{
    /* store value of the first node */
    struct clist_node *first_node_copy = clist->first_node;

    /* creation and copy of value_in into a new node */
    struct clist_node *p_new_node = (struct clist_node*) malloc( sizeof(struct clist_node));
    p_new_node->value = copy_value( value_in.data, value_in.len );
    
    /* reassign first node to the new one, ok if NULL */
    p_new_node->next_node = first_node_copy;
    /* clist point to the new one */
    clist->first_node = p_new_node;
    clist->len++;
    return CLIST_OK;
}

/** arument by pointer, avoid a copy 
 * push_back */
int clist_gen_push_back(clist_gen_t *clist, const value_t *value_in)
{
    struct clist_node *new_node;
    /* return a copy, may create the strange behaviour */
    struct clist_node *last_node = get_node_last(clist->first_node);
    /* create a  new node */
    new_node = (struct clist_node*) malloc( sizeof(struct clist_node));
    new_node->next_node = NULL;
    if( !new_node )
        return CLIST_ALLOC_ERR;
    
    new_node->value = copy_value( value_in->data, value_in->len );
    /* specific for first one, 
        would need the adress of the pointer (and *node = new_node) to avoid the if ? */
   if( last_node == NULL)
        clist->first_node = new_node;
    else
        last_node->next_node = new_node;

    clist->len++;
    return CLIST_OK;
}

/* get by number, not very "list", otherwise need to give access to clist_node interface in API
    not the point on this interface to get by number ? really needed ? */
int clist_gen_get_value_ref( const clist_gen_t *clist, size_t elem_nb, value_t **value_out)
{
    struct clist_node *node;
    if( elem_nb >= clist->len )
        return CLIST_OUTOFRANGE;
    node = get_node_number( clist->first_node, elem_nb );
    *value_out = &(node->value);
    return CLIST_OK;
}

int clist_gen_get_value_copy( const clist_gen_t *clist, size_t elem_nb, value_t *value_out)
{
    struct clist_node *node;
    if( elem_nb >= clist->len )
        return CLIST_OUTOFRANGE;
    node = get_node_number( clist->first_node, elem_nb );
    *value_out = copy_value( node->value.data, node->value.len );
    return CLIST_OK;
}

/* cannot return ERR positif and size_t */
int clist_gen_find( const clist_gen_t *clist, value_t* value_out, 
                    const char* buffer, size_t buffer_len)
{
    struct clist_node *curr_node = clist->first_node;
    if( clist->first_node == NULL) {
        return CLIST_EMPTY;
    }
    /* loop over nodes */
    do {
        /* comparison criteria */
        if( memcmp( curr_node->value.data, buffer, buffer_len) == 0 ) {
            *value_out = copy_value( curr_node->value.data, curr_node->value.len );
            return CLIST_OK;
        }
        /* update node */
        curr_node = get_next_node( curr_node );
    } while ( curr_node != NULL );

    return CLIST_NOTFOUND;
}

/***** Private functions implementation ****/
/*** private, only dependent of node ? ***/
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