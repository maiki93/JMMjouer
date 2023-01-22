#pragma once

#include "irecord_fwd.h" /* define irecord_t */
#include "joueur.h"      /* define Joueur with full structure available */

/* @brief Interface record storage of saved data (joueurs and scores)
    Try to follow c++ interface, abstract class with pure virtual functions.
*/

/* public interface : virtual (pure) and common implementation */

/* @brief Return a reference to the string description of the record.
    Stored internally in irecord_t, 
    Not mean to be overidden, derived types will provide a specific description at init()
*/
const char* record_get_storage_info(const irecord_t *this);

/* @brief Return a Joueur from its name
  An "invalid" joueur (to define) is returned if not found */
joueur_t record_find_joueur_from_name( irecord_t *record, const char* name);