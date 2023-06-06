#pragma once

#include "shared_EXPORTS.h"

#include "ccontainer/clist_generic.h"
#include "ccontainer/cvector_generic.h"

/** @file
 * Regroup utilities functions related to ccontainer :
 * - transfert clist to cvector
 * - some algorithm ?
 * - other ???? */
/*
typedef struct clist_gen_t;
typedef struct cvector_gen_t;
*/

/** A unique simple interface to help deletion of 
        array of structure return by get_array methods.
    Otherwise typedef / struct overall complex and increase dependencies */
SHARED_EXPORT void carray_cstring_delete( char **carray, size_t len);

SHARED_EXPORT cvector_gen_t* transfert_clist_to_cvector_gen(clist_gen_t* clist, ccontainer_err_t *err_code);