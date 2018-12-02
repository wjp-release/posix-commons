#include <stdarg.h>
#include <stdio.h>
#include <pthread.h>
#include "log.h"

static enum posixc_log_level        log_lowest = LOG_LEVEL_ERR;
static pthread_mutex_t              log_mtx;
static FILE*                        log_file = stderr;

void posixc_log_init(enum log_level lowest, FILE* file){
    log_lowest = lowest;
    log_file = file;
}

void posixc_log_close(){
    if(log_file!=stderr && log_file!=stdout) fclose(log_file);
    log_file=NULL;
}

void posixc_log_init_with_stderr(enum log_level lowest){
    posixc_log_init(lowest, stderr);
}

void posixc_log_init_with_stdout(enum log_level lowest){
    posixc_log_init(lowest, stdout);
}

void posixc_log_init_with_filename(enum log_level lowest, const char* fname){
    posixc_log_init(lowest, posixc_fopen(fname,"a"));
}

static void log(const int level, const char *fmt, va_list args) {
    if (level < log_lowest) return;
    pthread_mutex_lock(&log_mtx);
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
    pthread_mutex_unlock(&log_mtx);
}

void posixc_log(const int level, const char *fmt, ...) {
    if(!log_file) return;
    va_list args;
    va_start(args, fmt);
    log(level, fmt, args);
    va_end(args);
}


