#pragma once

/* inclusion of a logger, see test/c89_test
   other variables (not so portable : __func__, __FUNCTION__, __PRETTY_FUNCTION__
   __line__ may be a long on some compiler/platform (SO) */
#define CLOG_ERR(format, ...) \
    log_debug(__FILE__ , __LINE__ , format, __VA_ARGS__)

#define CLOG_DEBUG(format, ...) \
     log_debug(__FILE__ , __LINE__ , format, __VA_ARGS__)

#ifdef __cplusplus
extern "C" {
#endif

int init_clogger(const char *filename);
int close_clogger();

void log_debug(const char *filename, long line, const char* format, ...);

#ifdef __cplusplus
}
#endif