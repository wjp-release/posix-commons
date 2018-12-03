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

// invisible/ABI-irrelevant structures 

typedef struct posixc_reactor posixc_reactor;
typedef struct posixc_event posixc_event;
typedef void (*posixc_event_cb)(posixc_event*, int, void *);
