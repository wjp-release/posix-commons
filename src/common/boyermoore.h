#pragma once

#include "internal.h"

struct posixc_boyermoore{
    size_t      alpha_skip_lookup[256];
    size_t*     find_skip_lookup;
    bool        case_sensitive;
    const char* query;
    size_t      query_len;
};

void posixc_boyermoore_init(posixc_boyermoore* boyermoore, const char* query, size_t query_len, bool case_sensitive);
const char* posixc_boyermoore_search_in(posixc_boyermoore* boyermoore, const char* text, size_t text_len);
const char* posixc_boyermoore_contains(const char* text, size_t text_len, const char* query, size_t query_len);
const char* posixc_boyermoore_contains_case_ignored(const char* text, size_t text_len, const char* query, size_t query_len);