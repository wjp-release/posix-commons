#pragma once

#include "internal.h"

enum posixc_log_level {
    LOG_LEVEL_DEBUG = 10,
    LOG_LEVEL_INFO  = 20,
    LOG_LEVEL_WARN  = 30,
    LOG_LEVEL_ERR   = 40,
    LOG_LEVEL_FATAL = 50,
    LOG_LEVEL_NONE  = 60
};

void posixc_log_init(enum log_level lowest, bool nonblocking);
void posixc_log(const unsigned int level, const char *fmt, ...);
