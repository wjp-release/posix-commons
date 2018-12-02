#pragma once

#include "internal.h"

void posixc_slice_init(posixc_slice*slice, uint64_t size);
void posixc_slice_destroy(posixc_slice*slice);
void posixc_slice_init_from(posixc_slice*slice, const char* str);
void posixc_slice_copy(posixc_slice*dst, posixc_slice*src);
bool posixc_slice_contains(posic_slice*text, posic_slice* pattern);
