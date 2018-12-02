#include <stdarg.h>
#include <stdio.h>
#include <pthread.h>
#include "log.h"

static enum posixc_log_level        log_lowest = LOG_LEVEL_ERR;
static pthread_mutex_t              log_mtx;
static bool                         log_nonblock = false;
static FILE*                        log_file;

void posixc_log_init(enum log_level lowest, bool nonblock, FILE* file){
    log_lowest = lowest;
    log_nonblock = nonblock;
    log_file = file;
}

void posixc_log_init_with_stderr(enum log_level lowest, bool nonblock){
    posixc_log_init(lowest, nonblock, stderr);
}

void posixc_log_init_with_stdout(enum log_level lowest, bool nonblock){
    posixc_log_init(lowest, nonblock, stdout);
}

void posixc_log_init_with_filename(enum log_level lowest, bool nonblock, const char* fname){
    posixc_log_init(lowest, nonblock, posixc_fopen(fname,"a"));
}

static void async_log(const int level, const char *fmt, va_list args){

}

static void sync_log(const int level, const char *fmt, va_list args){
    switch (level) {
        case LOG_LEVEL_DEBUG:
            fprintf(log_file, "DEBUG: ");
            break;
        case LOG_LEVEL_INFO:
            fprintf(log_file, "INFO: ");
            break;
        case LOG_LEVEL_WARN:
            fprintf(log_file, "WARN: ");
            break;
        case LOG_LEVEL_ERR:
            fprintf(log_file, "ERR: "); 
            break;
        case LOG_LEVEL_FATAL:
            fprintf(log_file, "FATAL: "); 
            break;
    }
    vfprintf(log_file, fmt, args);
    fprintf(log_file, "\n");
}


static void log(const int level, const char *fmt, va_list args) {
    if (level < log_lowest) return;
    pthread_mutex_lock(&log_mtx);
    if(log_nonblock){
        async_log(level, fmt, args);
    }else{
        sync_log(level, fmt, args);
    }
    pthread_mutex_unlock(&log_mtx);
}

void posixc_log(const int level, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    log(level, fmt, args);
    va_end(args);
}


