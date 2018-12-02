#pragma once

#include "internal.h"

void posixc_boyermoore_init(posixc_boyermoore* boyermoore, char* query, size_t query_len, bool case_sensitive);
const char* posixc_boyermoore_search_in(posixc_boyermoore* boyermoore, const char* text, size_t text_len);
const char* posixc_boyermoore_contains(const char* text, size_t text_len, const char* query, size_t query_len);
const char* posixc_boyermoore_contains_case_ignored(const char* text, size_t text_len, const char* query, size_t query_len);