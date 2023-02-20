#pragma once

#include "shared_EXPORTS.h"
/*
#if defined(_WIN32) && defined(_MSC_VER)
    #define SHARED_LIB __declspec(dllexport)
#else
    #define SHARED_LIB __declspec(dllimport)
    #undef SHARED_LIB
#endif
*/

/* inclusion of a logger, see test/c89_test
   other variables (not so portable : __func__, __FUNCTION__, __PRETTY_FUNCTION__
   __line__ may be a long on some compiler/platform (SO) */
#define CLOG_ERR(format, ...) \
    log_debug(__FILE__ , __LINE__ , format, __VA_ARGS__)

#define CLOG_DEBUG(format, ...) \
     log_debug(__FILE__ , __LINE__ , format, __VA_ARGS__)

/* necessary for C++ ? already C , depends if build or lmport */
#ifdef __cplusplus
extern "C" {
#endif

int SHARED_EXPORT init_clogger(const char *filename);
int SHARED_EXPORT close_clogger();

void SHARED_EXPORT log_debug(const char *filename, long line, const char* format, ...);

#ifdef __cplusplus
}
#endif