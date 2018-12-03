#pragma once

#include "internal.h"

#define inline_cap 48

//sizeof posixc_slice === 64 bytes === cache line length
typedef struct{
    char        inlinebuf[inline_cap];  
    char*       buf; // points to inline_buf for small strings; malloc()-ed address otherwise
    uint64_t    size;   
}posixc_slice;

void posixc_slice_init(posixc_slice*slice, uint64_t size);
void posixc_slice_destroy(posixc_slice*slice);
void posixc_slice_init_as_aggregation(posixc_slice* aggregate, posixc_slice** slices, size_t nr_slices);
void posixc_slice_init_from(posixc_slice*slice, const char* str);
void posixc_slice_copy(posixc_slice*dst, posixc_slice*src);
bool posixc_slice_contains(posixc_slice*text, posixc_slice* pattern);
int  posixc_slice_append(posixc_slice* text, const char* suffix);

// These functions create inline slices from numeric types. So there is no need to
// call posixc_slice_destroy() on them.
posixc_slice posixc_slice_parse_int(int x); 
posixc_slice posixc_slice_parse_float(float x, int afterpoints); 
posixc_slice posixc_slice_parse_double(double x, int afterpoints); 