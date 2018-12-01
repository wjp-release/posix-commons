#pragma once

#include "internal.h"

extern void posixc_slice_init(posixc_slice*slice, uint64_t size);
extern void posixc_slice_destroy(posixc_slice*slice);
extern void posixc_slice_init_from(posixc_slice*slice, const char* str);