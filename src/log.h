#pragma once

#include "internal.h"

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