
#include <stdbool.h>
/* use clist_generic for files_extension */
#include "ccontainer/clist_generic.h"

#include "jmmjouer/game_loader/game_loader.h"

/* just an idea...
  typedef struct load_game_fctor {
    int (*p_fct)(cmap_ptrf_game_t, const char*);
    game_loader_t *gload;
};

*/
/* to see if can be extended with other than clist_gen_t (carray_gen_t ? possible ? minimum interface kinf of mix-in ?) */
/* @brief For each file in "dir" matching the extension "extension"
    invoke the p_fct function call ( this, file ) to make the insertion into "this".
    Can be used for any "derivative" of clist_gen_t, 
    fine if clist_gen_t *data_member ALWAYS first member and can be cast (see game_loader::loadd_all)*/
int for_files_with_extension( const char* dir, const char* extension, 
                              int (*p_fct)(clist_gen_t *,const char*), clist_gen_t *clist_to_fill );

/* generic function to loop over a directory and apply a simple callback function p_fct(char*->int),
   not used (keep exemple) */
int for_files_with_extension_callback( const char* dir, const char* extension, 
                                       int (*p_fct)(const char*) );

/* @brief Check if file ends/contains the extension 
  \param[in] name and extension */
bool check_extension( const char* name, const char* extension );
