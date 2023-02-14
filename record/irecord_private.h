#pragma once

#include "joueur.h"

/* hide structure declarationn, in a private file for use by derived class only */
typedef struct {

    /* const not very restrictive in C, and to avoid (char*) cast in free / or strcpy */
    /* if really really want to protect should send back only copy of it.. more verbose for caller */
    char* storage_info; /* all have a string description */
    
    /* ptr function for "virtual" calls, use of void* to avoid incompatible pointer 
    cannot cast the signature of derived implementation */
    joueur_t (*get_joueur_from_name)( void *this, const char *name);

    /* may be base function, create dy derived class, for reuse */
    /*const char* (*get_storage_info)(void *this);*/
    
} irecord_t;

/* protected functions, accessible only by derived types */
irecord_t* irecord_new();
void irecord_init( irecord_t *this, const char *storage);
void irecord_delete( irecord_t *this);
