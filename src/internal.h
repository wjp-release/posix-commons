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

typedef struct posixc_asynclogsvr posixc_asynclogsvr;
typedef struct posixc_buffered_writable posixc_buffered_writable;
typedef struct posixc_slice posixc_slice;
typedef struct posixc_boyermoore posixc_boyermoore;
typedef struct posixc_event posixc_event;
typedef struct posixc_reactor posixc_reactor;

#define buffered_writable_size 4096