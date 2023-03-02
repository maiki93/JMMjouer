#pragma once

/* #undef SHARED_EXPORT for linux ?*/
#ifdef withLIB
    #if defined(_WIN32) && defined(_MSC_VER)
        #ifdef shared_EXPORTS
            #define SHARED_EXPORT __declspec(dllexport)
        #else
            #define SHARED_EXPORT __declspec(dllimport)
        #endif
    #else
        #define SHARED_EXPORT
    #endif
#else
    #define SHARED_EXPORT
#endif

/* for windows compile without any library (export or import) */
/*#define SHARED_EXPORT*/
