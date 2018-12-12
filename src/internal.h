#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h> 
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

typedef struct posixc_asynclogcli posixc_asynclogcli;
typedef struct posixc_asynclogsvr posixc_asynclogsvr;
typedef struct posixc_buffered_writable posixc_buffered_writable;
typedef struct posixc_slice posixc_slice;
typedef struct posixc_boyermoore posixc_boyermoore;
typedef struct posixc_event posixc_event;
typedef struct posixc_reactor posixc_reactor;

enum log_level {
    LOG_LEVEL_DEBUG = 0,
    LOG_LEVEL_INFO  = 1,
    LOG_LEVEL_WARN  = 2,
    LOG_LEVEL_ERR   = 3,
    LOG_LEVEL_FATAL = 4,
    LOG_LEVEL_NONE  = 5,
    LOG_LEVEL_NUMBER = 6
};

// Configurable constants
#define buffered_writable_size 4096 //used in fileio.c
#define usockpath_log "/tmp/asynclog" //used in asynclogsvr.c
