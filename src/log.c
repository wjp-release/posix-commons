#include <stdarg.h>
#include <stdio.h>
#include <pthread.h>
#include "log.h"
#include "fileio.h"

struct posixc_logger{
    enum log_level lowest;
    pthread_mutex_t mtx;
    FILE* file;
};

void posixc_log_init(posixc_logger* logger, enum log_level lowest, FILE* file){
    logger->lowest = lowest;
    logger->file = file;
}

void posixc_log_close(posixc_logger* logger){
    if(logger->file!=stderr && logger->file!=stdout) fclose(logger->file);
}

void posixc_log_init_with_stderr(posixc_logger* logger, enum log_level lowest){
    posixc_log_init(logger, lowest, stderr);
}

void posixc_log_init_with_stdout(posixc_logger* logger, enum log_level lowest){
    posixc_log_init(logger, lowest, stdout);
}

void posixc_log_init_with_filename(posixc_logger* logger, enum log_level lowest, const char* fname){
    posixc_log_init(logger, lowest, posixc_fopen(fname,"a"));
}

static void log(posixc_logger* logger, const int level, const char *fmt, va_list args) {
    if (level < (int) logger->lowest) return;
    pthread_mutex_lock(&logger->mtx);
    switch (level) {
        case LOG_LEVEL_DEBUG:
            fprintf(logger->file, "DEBUG: ");
            break;
        case LOG_LEVEL_INFO:
            fprintf(logger->file, "INFO: ");
            break;
        case LOG_LEVEL_WARN:
            fprintf(logger->file, "WARN: ");
            break;
        case LOG_LEVEL_ERR:
            fprintf(logger->file, "ERR: "); 
            break;
        case LOG_LEVEL_FATAL:
            fprintf(logger->file, "FATAL: "); 
            break;
    }
    vfprintf(logger->file, fmt, args);
    fprintf(logger->file, "\n");
    pthread_mutex_unlock(&logger->mtx);
}

void posixc_log(posixc_logger* logger, const int level, const char *fmt, ...) {
    if(!logger->file) return;
    va_list args;
    va_start(args, fmt);
    log(logger, level, fmt, args);
    va_end(args);
}


