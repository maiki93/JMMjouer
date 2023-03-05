#include <stdio.h> /* need for include NULL */
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/*#include "irecord.h"*/
#include "irecord_private.h"
#include "clogger/clogger.h"

/* https://stackoverflow.com/questions/777261/avoiding-unused-variables-warnings-when-using-assert-in-a-release-build */
#define _unused(x) ((void)(x))

/*#include "irecord.h"*/

/* typedef (in private_header) would avoid recopy of complex type */
joueur_t __find_joueur_error( void *this, const char *name);

/* @brief Assign the description field in storage_info.
    Make a copy of the input description
    Only for internal usage, called from irecord_init() */
static int record_set_storage_info( irecord_t *this, const char * description);

irecord_t* irecord_new()
{
    irecord_t *this = (irecord_t *)malloc( sizeof(irecord_t) );
    this->storage_info = NULL;
    return this;
}

void irecord_init( irecord_t *this, const char *storage_description)
{
    int retour;
    /* safety set ptr_function to default */
    /*this->get_storage_info = __record_get_storage_info;*/
    this->get_joueur_from_name = __find_joueur_error; /* return an invalid Joueur, should never be called */

    /* initilize all members, info provided by derived type */
    retour = record_set_storage_info( this, storage_description );
    if ( retour ) {
        CLOG_ERR("Error in allocation for irecord.storage_info %d\n", retour);
        /* make a stop */
        exit(1);
    }
    CLOG_DEBUG("Initialization irecord done %d\n", retour);
}

void irecord_delete( irecord_t *this)
{
    CLOG_DEBUG("Destructor base class %d\n", 0);
    free( this->storage_info);
    this->storage_info = NULL;
    /* reset ptr function ? really not needed I guess */
}

const char* record_get_storage_info( const irecord_t *this)
{
    return this->storage_info;
}

/**** 
 * Methods meant to be overwritten, forward call to the actual ptr function 
 * *****/

joueur_t record_find_joueur_from_name( irecord_t *record, const char* name)
{
    /* maybe more logic to insert here, try to find from derived type
        but deal with error (INVALID_JOUEUR) at this level */
    return record->get_joueur_from_name( (void*)record, name );
}

/* TODO define a INVALID_JOUEUR, default init of irecord_t should never be called in fact
    or common place to deal with INVALID_JOUEUR 
    This function sould return error/assert in fact, MUST be overriden ! */
joueur_t __find_joueur_error( void *this, const char *name)
{
    joueur_t joueur;
    assert(this != NULL);
    _unused(this); /* nice trick working in release mode, both gcc/msvc*/
    assert(name); /* msvc forces to use argument variable */
    _unused(name);
    
    joueur_init(&joueur, "invalid", false);
    printf("-- In record base __get_joueur_error --\n");
    printf("joueur name: %s\n", joueur.person.nom);
    printf("daltonien: %d\n", joueur.person.is_daltonien);
    printf("adresss of joueur: %p\n",(void*) &joueur);
    printf("adresss of joueur.nom: %p\n",(void*) &joueur.person.nom);
    return joueur;
}
/************  private, static functions ***********/
int record_set_storage_info( irecord_t *this, const char *description)
{
    /* tmp string allow to keep const char * in irecord_t, simule a swap idiom */  
    char *tmp = (char*)malloc( strlen(description) + 1);
    if( !tmp ) {
        CLOG_ERR("Error in memory allocation return %d", -1);
        return -1;
    }
    strcpy( tmp, description ); /* check copy has correct size */
    this->storage_info = tmp; /* no error, assign pointer member */

   /*this->storage_info = description; */ /* would take ownership from caller */
   return 0;
}

/* may be derived, so void* necessary */
/* do not provide functionality to be overwritten at the moment */
/*
char* __record_get_storage_info( void *this)
{
    irecord_t *record = (irecord_t *)this;
    return ((irecord_t *)this)->get_storage_info( this );
}
*/
