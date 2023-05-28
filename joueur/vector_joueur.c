#include "assert.h"

#include "joueur/adapter_ccontainer_joueur.h"
#include "joueur/vector_joueur.h"
#include "ccontainer/cvector_generic.h"

vector_joueur_t* vector_joueur_new()
{
    vector_joueur_t * this;
    this = (vector_joueur_t*) malloc( sizeof(vector_joueur_t) );
    if( !this )
        return NULL;
    return this;
}

int vector_joueur_init( vector_joueur_t *this )
{
    assert( this );
    this->cvect = cvector_gen_new(); /* possible error */
    if( !this->cvect)
        return -1;
    cvector_gen_init( this->cvect );
    cvector_gen_set_deleter( this->cvect, joueur_deleter_value );
    /*cvector_gen_set_duplicater( this->cvect, joueur_duplicater_value );*/
    return 0;
}

int vector_joueur_init_with_capacity( vector_joueur_t *this, size_t init_capacity )
{
    ccontainer_err_t err_code;
    this->cvect = cvector_gen_new(); /* possible error */
    if( !this->cvect)
        return -1;
    err_code = cvector_gen_init_with_capacity( this->cvect, init_capacity );
    if( err_code != CCONTAINER_OK )
        return -1;
    cvector_gen_set_deleter( this->cvect, joueur_deleter_value );
    /*cvector_gen_set_duplicater( this->cvect, joueur_duplicater_value );*/
    return 0;
}

void vector_joueur_delete( vector_joueur_t *this )
{
    cvector_gen_free( this->cvect );
    this->cvect = NULL;
}

void vector_joueur_free( vector_joueur_t *this )
{
    vector_joueur_delete( this );
    free(this);
}

/** \} */

size_t vector_joueur_size( const vector_joueur_t *this )
{
    return cvector_gen_size( this->cvect );
}

joueur_t vector_joueur_get_at( const vector_joueur_t *this, size_t index)
{
    joueur_t joueur_out;
    ccontainer_value_t* pvalue_joueur;
    ccontainer_err_t err_code;

    /* get a reference */
    pvalue_joueur = cvector_gen_get_at(this->cvect, index, &err_code);
    if( err_code == CCONTAINER_OK) {
        /* make a copy into joueur, err_code not used here... 
        // copy of cmap may cause error , != of invalid... or to male in extract
        //  (person_invalid very domain) */
        err_code = extract_value_joueur( pvalue_joueur, &joueur_out );
    } else {
        /* status invalid by default */
        joueur_default_init( &joueur_out );
    }
    /* valid or invalid */
    return joueur_out;
}

int vector_joueur_push_back( vector_joueur_t *this, const joueur_t *joueur_in )
{
    ccontainer_value_t tmp_value_joueur;
    ccontainer_err_t err_code;
    /* tmp value make a deep copy of the joueur */
    tmp_value_joueur = make_value_joueur( joueur_in, &err_code );
    if( err_code != CCONTAINER_OK)
        return -1;
    /* tmp_value is moved into the vector, no free to apply */
    err_code = cvector_gen_push_back( this->cvect, &tmp_value_joueur);
    return (err_code == CCONTAINER_OK) ? 0 : -1;
}