#include <assert.h>

#include "joueur/list_joueur.h"

#include "joueur/adapter_ccontainer_joueur.h"
#include "ccontainer/clist_generic.h"
#include "clogger/clogger.h"

list_joueur_t* list_joueur_new()
{
    list_joueur_t* this;
    this = (list_joueur_t*) malloc( sizeof(list_joueur_t) );
    this->clist = clist_gen_new();
    if( !this || !this->clist)
        return NULL;
    return this;
}

void list_joueur_init( list_joueur_t *this)
{
    clist_gen_init( this->clist );
    this->len = 0;
}

void list_joueur_free( list_joueur_t *this)
{
    clist_gen_free( this->clist, joueur_deleter_value );
    this->clist = NULL;
    free(this);
}

size_t list_joueur_size( const list_joueur_t *this )
{
    return this->len;
}

int list_joueur_push_back( list_joueur_t *this, const joueur_t *joueur_in )
{
    ccontainer_err_t err_code;
    ccontainer_value_t value_joueur;
    /* want a deep copy to be moved into a ccontainer */
    value_joueur = make_value_joueur( joueur_in, &err_code );
    if( err_code != CCONTAINER_OK)
        return 1;
    if( (err_code = clist_gen_push_back( this->clist, &value_joueur )) != CCONTAINER_OK )
        return 1;
    /* success */
    this->len++;
    return 0;
}

joueur_t list_joueur_pop_front(list_joueur_t *this)
{   
    joueur_t joueur_out;
    ccontainer_value_t value_joueur;
    ccontainer_err_t err_code;

    /* pop, we must get a copy because deleted from clist. (or split ?) 
        here a move allow to use default value_t copy and optimize the remove of node */
    value_joueur = clist_gen_pop_front( this->clist, &err_code);

    /* error : value_joueur.data == NULL */
    if( err_code != CCONTAINER_OK ) 
    {
        CLOG_DEBUG("ccontainer err_code : %d", err_code);
        joueur_default_init(&joueur_out);
        return joueur_out;
    }

    assert( value_joueur.data != NULL);
    assert( value_joueur.len > 0);

    /* extraction copy but know how to deserailze joueur */
    if( (err_code = extract_value_joueur( &value_joueur, &joueur_out ) )
                 != CCONTAINER_OK ) {
        /* status must be invalid here */
        joueur_default_init(&joueur_out);
        /* value_joueur also at this step */
        ccontainer_delete_value(&value_joueur);
        return joueur_out;
    }
    /* pop success size decreased */
    this->len--;
    /* value_joueur to delete */
    joueur_deleter_value( &value_joueur );
    /* success status valid */
    return joueur_out;
}