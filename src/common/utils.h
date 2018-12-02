#pragma once
#include "internal.h"

int     posixc_is_regex(const char *query);
int     posixc_binary_search(const char *needle, char **haystack, int start, int end);
bool    posixc_is_lowercase(const char *s);

