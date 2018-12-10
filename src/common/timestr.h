#pragma once
#include "internal.h"
#include <time.h>

int posixc_timestr(char*buffer, const char* pattern,  int len);

int posixc_default_timestr(char*buffer);