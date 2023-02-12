#ifndef GAME_PENDU_H_INCLUDED
#define GAME_PENDU_H_INCLUDED

#include "person.h"
#include "victory.h"

/* needed to produce the lib file with the dll */
/* on windows declaration of declspec(dllexport) generates *lib and *exp files
  even when creating executables (e.g. unit tests)
  linker option to use /NOIMPLIB /NOEXP */
#if defined(_WIN32)
    #define SHARED_LIB __declspec(dllexport)
#else
    #undef SHARED_LIB
#endif

victory_t SHARED_LIB start_game_pendu(person_t person);
/*victory_t start_game_pendu(person_t person);*/

#endif
