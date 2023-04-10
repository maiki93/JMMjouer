#pragma once

#include "irecord_fwd.h"     /* define irecord_t */
#include "joueur/joueur.h"    /* define Joueur with full structure available */

#include "shared_EXPORTS.h"

/** @file 
  Interface for the storage of data (joueur_t with theirs scores).

  Try to follow a c++ interface, irecord_t as an abstract class with pure virtual functions.

  \defgroup record_grp record 
  Interface for the storage of the data
*/

/** \name  public API : (pure) virtual and common implementation 
 * \ingroup record_grp */
/** @{ */
/** Return a description (string) of the record.
  Stored internally in the base irecord_t, not mean to be overidden.
  Derived types will provide a specific description at init()
*/
SHARED_EXPORT const char* record_get_storage_info(const irecord_t *this);

/** Retrive a joueur_t from its name.
  An "invalid" joueur is returned if not found.
  Pure virtual function to be overridden by a concrete implementation. */
SHARED_EXPORT joueur_t record_find_joueur_from_name( irecord_t *record, const char* name);
/** @} */
