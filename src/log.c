#include <stdarg.h>
#include <stdio.h>
#include <pthread.h>
#include "log.h"

static enum posixc_log_level        log_lowest = LOG_LEVEL_ERR;
static pthread_mutex_t              mtx;
static bool                         nonblocking = false;
static FILE*                        logfile;

void posixc_log_init(enum log_level lowest, bool nonblocking, const char* filename){
    log_lowest = lowest;
    nonblocking = nonblocking;
    logfile = posixc_fopen(filename,"a");
}

static void nonblocking_log(const int level, const char *fmt, va_list args) {
    if (level < log_threshold) return;
    //todo!
    
}

static void blocking_log(const int level, const char *fmt, va_list args) {
    if (level < log_threshold) return;
    pthread_mutex_lock(&mtx);
    switch (level) {
        case LOG_LEVEL_DEBUG:
            fprintf(logfile, "DEBUG: ");
            break;
        case LOG_LEVEL_INFO:
            fprintf(logfile, "INFO: ");
            break;
        case LOG_LEVEL_WARN:
            fprintf(logfile, "WARN: ");
            break;
        case LOG_LEVEL_ERR:
            fprintf(logfile, "ERR: "); 
            break;
        case LOG_LEVEL_FATAL:
            fprintf(logfile, "FATAL: "); 
            break;
    }
    vfprintf(logfile, fmt, args);
    fprintf(logfile, "\n");
    pthread_mutex_unlock(&mtx);
}

void posixc_log(const int level, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    if(nonblocking){
        nonblocking_log(level, fmt, args);
    }else{
        blocking_log(level, fmt, args);
    }
    va_end(args);
}


