#pragma once

/** @file
 * 
 * @defgroup ccontainer_error_grp ccontainer error code
 * @ingroup ccontainer_grp
 * Error code for all ccontainer's of the library
 * 
 * First implementation : only in a header */

/** Error code for the library.
 * \ingroup ccontainer_error_grp */
typedef enum CCONTAINER_ENUM_ERROR { /* suggested E_CCONTAINER */
    CCONTAINER_OK = 0,
    CCONTAINER_OUTOFRANGE = 1,
    CCONTAINER_ALLOCERR = 2,
    CCONTAINER_NOT_FOUND = 3,
    CCONTAINER_EMPTY = 4,
    CCONTAINER_END = 5
} ccontainer_err_t;
