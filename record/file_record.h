#pragma once

#include "shared_EXPORTS.h"

/* the file is added in module, not wanted */
/*ingroup record_grp */

/** @file
 * 
 * @ingroup record_grp
 * 
 * Concrete implemention of irecord_t for storage in text files.
*/

/** @{ \ingroup record_grp */

/** Forward declaration */
typedef struct file_record_type file_record_t;

/** \name Constructor / Destructor */
/** \{ */
/** Allocation. */
SHARED_EXPORT file_record_t* file_record_new();
/** Constructor with filename for input/output */
SHARED_EXPORT void file_record_init( file_record_t *this, const char *filename);
/** Destructor. */
SHARED_EXPORT void file_record_delete( file_record_t *this);
/** \} */

/** @} */ /* end record_grp */

/* All behavior functions inherited from record, 
 * to call through the base class methods
 * may add specific method in public API ? */