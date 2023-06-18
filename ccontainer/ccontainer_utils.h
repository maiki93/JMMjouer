#pragma once

#include "shared_EXPORTS.h"

#include "ccontainer/clist_generic.h"
#include "ccontainer/cvector_generic.h"

/** @file
 * 
 * @defgroup ccontainer_utilities_grp utilities
 * @ingroup ccontainer_grp
 * 
 * Regroup utilities functions related to ccontainer
 * 
 * Main methods
 * - transfert clist to cvector
 * - delete array of C-strings
 * - some algorithm ? but must be very generic and independent of the generic container...
 */

/** @{ \ingroup ccontainer_utilities_grp */

/** A unique simple interface to help deletion of 
        array of structure return by get_array methods.
    Otherwise typedef / struct overall complex and increase dependencies */
SHARED_EXPORT void carray_cstring_delete( char **carray, size_t len);

/** Transfert elements of a \ref clist_gen_t into a \ref cvector_gen_t.
 * \post clist is empty after the call
 * \param[in] clist src
 * \param[out] err_code code
 * \return a cvector allocated on heap */
SHARED_EXPORT cvector_gen_t* transfert_clist_to_cvector_gen(clist_gen_t* clist, ccontainer_err_t *err_code);

/** @} */ /* end utilities grp */