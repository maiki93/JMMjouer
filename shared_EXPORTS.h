#pragma once

#if defined(_WIN32) && defined(_MSC_VER)

    #ifdef shared_EXPORTS
        #define SHARED_EXPORT __declspec(dllexport)
    #else
        #define SHARED_EXPORT __declspec(dllimport)
    #endif
#else
    #undef SHARED_EXPORT
#endif
