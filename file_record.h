#pragma once

/* @brief API of a concrete class impelmenting irecord_t
*/

/* forward declaration, need to use struct X in main caller */
/* TODO try the trick with tpyedef struct name1 name2 fwd.h*/
/*struct file_record_t;*/
/* easier to use */
typedef struct file_record_type file_record_t;

file_record_t* file_record_new();
void file_record_init( file_record_t *this, const char *filename);

void file_record_delete( file_record_t *this);

/** All behavior functions inherited from record, 
 * to call through the base class methods */

/* may add specific method in public API ? */