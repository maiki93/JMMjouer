#pragma once

/* @brief API of a concrete class impelmenting irecord_t
*/

#include "shared_EXPORTS.h"

/* forward declaration, need to use struct X in main caller */
/* TODO try the trick with tpyedef struct name1 name2 fwd.h*/
/*struct file_record_t;*/
/* easier to use */
typedef struct file_record_type file_record_t;

SHARED_EXPORT file_record_t* file_record_new();
SHARED_EXPORT void file_record_init( file_record_t *this, const char *filename);

SHARED_EXPORT void file_record_delete( file_record_t *this);

/** All behavior functions inherited from record, 
 * to call through the base class methods */

/* may add specific method in public API ? */