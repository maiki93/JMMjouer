#pragma once

/** @{ \ingroup jmmjouer_core_grp */

/** @file 
 *  Parameters to execute arcade::run() function.
*/

/** Structure passed to arcade for its initialization / execution.
    injection of dependencies reduce the size/need of this structure
    could be included in arcade.h, really need to separate (other uses ?) */
typedef struct {
    /** option to play anonymously */
    bool play_anonymous;
} arcade_params_t;

/** @} */