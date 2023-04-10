#pragma once

#include "joueur/joueur.h"

/** \file
  Hide structure declarationn in a private file.
  For use only by derived class / concrete implementation */

/** Explicit definition of the interface irecord.
 * 
 * Contains a storage_info variable shared with concrete implementations of the interface.
 * Virtual functions are simulated by pointer of functions which must be set by the derived classes. 
 * \ingroup record_grp 
*/
typedef struct {
    /* const not very restrictive in C, and to avoid (char*) cast in free / or strcpy */
    /* if really really want to protect should send back only copy of it.. more verbose for caller */

    /** All record have a string description set at construction. */
    char* storage_info;
    
    /** ptr function for "virtual" calls, use of void* to avoid incompatible pointer. 
    cannot cast the signature of derived implementation */
    joueur_t (*get_joueur_from_name)( void *this, const char *name);    
} irecord_t;

/** \name private / protected allocation functions, accessible only by derived types */
/** \{ */
/** Private memory allocation. */
irecord_t* irecord_new();
/** Private constructor with description argument. */
void irecord_init( irecord_t *this, const char *storage);
/** Private destructor. */
void irecord_delete( irecord_t *this);
/** \} */
