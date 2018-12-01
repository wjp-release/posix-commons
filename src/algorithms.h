#pragma once
#include "internal.h"

const char *boyer_moore_strnstr(const char* s,      const char* find, 
                                const size_t s_len, const size_t f_len,
                                const size_t alpha_skip_lookup[], 
                                const size_t* find_skip_lookup, 
                                const int case_insensitive);

