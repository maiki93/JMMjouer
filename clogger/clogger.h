#pragma once

#include "shared_EXPORTS.h"

/** @file
 * 
 * @defgroup clogger_grp a simple logger
 * 
 * One more Logger */

/** @{ \ingroup clogger_grp */

/* inclusion of a logger, see test/c89_test
   other variables (not so portable : __func__, __FUNCTION__, __PRETTY_FUNCTION__
   __line__ may be a long on some compiler/platform (SO) */
/** To log error */
#define CLOG_ERR(format, ...) \
    log_debug(__FILE__ , __LINE__ , format, __VA_ARGS__)
/** To log debug messages */
#define CLOG_DEBUG(format, ...) \
     log_debug(__FILE__ , __LINE__ , format, __VA_ARGS__)

/* necessary for C++ ? already C , depends if build or lmport */
#ifdef __cplusplus
extern "C" {
#endif

/** Constructor */
int SHARED_EXPORT init_clogger(const char *filename);
/** Destructor */
int SHARED_EXPORT close_clogger();
/** Log a string */
void SHARED_EXPORT log_debug(const char *filename, long line, const char* format, ...);

#ifdef __cplusplus
}
#endif

/** @} */ /** end ingroup ccontainer */