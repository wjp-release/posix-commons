#include "slice.h"
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define inline_cap 48

//sizeof posixc_slice === 64 bytes === cache line length
struct posixc_slice{
    char        inlinebuf[inline_cap];  
    char*       buf; // points to inline_buf for small strings; malloc()-ed address otherwise
    uint64_t    size;   
};

void posixc_slice_init(posixc_slice*slice, uint64_t size){
    assert(size>0);
    if(size>inline_cap){
        buf=(char*)malloc(size);
    }else{
        slice->buf=slice->inlinebuf;
        slice->size=size;
    }
}

void posixc_slice_destroy(posixc_slice*slice){
    if(slice->size>inline_cap){
        free(slice->buf);
    }
}

void posixc_slice_init_from(posixc_slice*slice, const char* str){
    int size=strlen(str);
    posixc_slice_init(slice);
    memcpy(slice->buf, str, size);
}

void posixc_slice_copy(posixc_slice*dst, posixc_slice*src){
    dst->size=src->size;
    if(dst->size<inline_cap){
        dst->buf=dst->inlinebuf;
    }else{
        dst->buf=(char*)malloc(size);
    }
    memcpy(dst->buf,src->buf,src->size);
}

// match_ptr = boyer_moore_strnstr(match_ptr, opts.query, buf_len - buf_offset, opts.query_len, alpha_skip_lookup, find_skip_lookup, opts.casing == CASE_INSENSITIVE);
const char *boyer_moore_strnstr(const char *s, const char *find, const size_t s_len, const size_t f_len,
                                const size_t alpha_skip_lookup[], const size_t *find_skip_lookup, const int case_insensitive) {
    ssize_t i;
    size_t pos = f_len - 1;

    while (pos < s_len) {
        for (i = f_len - 1; i >= 0 && (case_insensitive ? tolower(s[pos]) : s[pos]) == find[i]; pos--, i--) {
        }
        if (i < 0) {
            return s + pos + 1;
        }
        pos += posixc_max(alpha_skip_lookup[(unsigned char)s[pos]], find_skip_lookup[i]);
    }

    return NULL;
}


bool posixc_slice_contains(posic_slice*slice){
    
}

