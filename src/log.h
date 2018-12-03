#pragma once

#include "internal.h"

enum log_level {
    LOG_LEVEL_DEBUG = 0,
    LOG_LEVEL_INFO  = 1,
    LOG_LEVEL_WARN  = 2,
    LOG_LEVEL_ERR   = 3,
    LOG_LEVEL_FATAL = 4,
    LOG_LEVEL_NONE  = 5,
    LOG_LEVEL_NUMBER = 6
};

typedef struct{
    enum log_level lowest;
    pthread_mutex_t mtx;
    FILE* file;
}posixc_logger;

void posixc_log_init(posixc_logger* logger, enum log_level lowest, FILE* file);
void posixc_log_close(posixc_logger* logger);
void posixc_log_init_with_stderr(posixc_logger* logger, enum log_level lowest);
void posixc_log_init_with_stdout(posixc_logger* logger, enum log_level lowest);
void posixc_log_init_with_filename(posixc_logger* logger, enum log_level lowest, const char* fname);
void posixc_log(posixc_logger* logger, const int level, const char *fmt, ...);