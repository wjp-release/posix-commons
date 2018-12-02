#pragma once

#include "internal.h"

enum posixc_log_level {
    LOG_LEVEL_DEBUG = 0,
    LOG_LEVEL_INFO  = 1,
    LOG_LEVEL_WARN  = 2,
    LOG_LEVEL_ERR   = 3,
    LOG_LEVEL_FATAL = 4,
    LOG_LEVEL_NONE  = 5,
    LOG_LEVEL_NUMBER = 6
};
void posixc_log_init(enum log_level lowest, bool nonblock, FILE* file);
void posixc_log_init_with_stderr(enum log_level lowest, bool nonblock);
void posixc_log_init_with_stdout(enum log_level lowest, bool nonblock);
void posixc_log_init_with_filename(enum log_level lowest, bool nonblock, const char* fname);
void posixc_log(const unsigned int level, const char *fmt, ...);
