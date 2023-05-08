#pragma once

/** \file
 * Error code for all ccontainer's of the library.
 * 
 * First implementation : only in a header
*/

/** Error code for the library.
 * \ingroup ccontainer_grp
*/
typedef enum CCONTAINER_ERROR { /* suggested E_CCONTAINER */
    CCONTAINER_OK = 0,
    CCONTAINER_OUTOFRANGE = 1,
    CCONTAINER_ALLOCERR = 2,
    CCONTAINER_ERR_END = 3
} ccontainer_err;