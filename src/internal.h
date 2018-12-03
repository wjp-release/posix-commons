#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct posixc_slice        posixc_slice;
typedef struct posixc_boyermoore   posixc_boyermoore;
typedef struct posixc_logger       posixc_logger;
#ifdef __linux__
typedef struct posixc_epoller      posixc_epoller;
#endif
