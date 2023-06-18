#ifndef MORPION_H_INCLUDED
#define MORPION_H_INCLUDED

#include "jmmjouer/ptrf_game_typedef.h"

#include "shared_EXPORTS.h"

/** @defgroup plugins_src_grp plugins source code */

/** @file
 * 
 * Game implementation of the Morpion as a plugin, dynamiccaly load during the game.
 * 
 * @ingroup plugins_src_grp 
*/

/*
#if defined(_WIN32)
    #define SHARED_LIB __declspec(dllexport)
#else
    #define SHARED_LIB __declspec(dllimport)
    #undef SHARED_LIB
#endif
*/
/* to check also exported __stdcall (more windows eg. win32) or __cdecl(more standard) */
/*#define SHARED_LIB __declspec(dllexport)*/
/* void test(); */

/** Plugins must indicate a unique name (check performed during loading) */
SHARED_EXPORT const char name_game_plugin[] = "morpion";

/* objdump.exe -tT game_morpion.dll | grep 'start' do not see difference start_game_morpion appears in both */
/** Run a game 
 * \param[in] user
 * \return the result of the game */
SHARED_EXPORT score_game_t start_game_morpion(user_t user);

#endif
