
#include "ccontainer/value.h"

// https://gcc.gnu.org/onlinedocs/gcc/Diagnostic-Pragmas.html

/* test values with simple strings, do not include final \0 */
#if defined(__GNUC__) || defined(__GNUG__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunused-variable"
#endif
static const ccontainer_value_t value_str1 = {"first", 5};
static const ccontainer_value_t value_str2 = {"second", 6};
static const ccontainer_value_t value_str3 = {.data="tree",.len=4}; /* available C90 */
static const ccontainer_value_t value_str4 = {.data="fourth",.len=6};
static const ccontainer_value_t value_str5 = {.data="five",.len=4};
static const ccontainer_value_t value_str6 = {.data="six",.len=3};
// add doublon
static const ccontainer_value_t value_str7 = {.data="tree",.len=4};
static const ccontainer_value_t value_str8 = {.data="second",.len=6};

#if defined(__GNUC__) || defined(__GNUG__)
    #pragma GCC diagnostic pop
#endif
