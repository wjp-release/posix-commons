#include "slice.h"
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define inline_cap 48

//sizeof slice = 64 bytes = cache line length
struct posixc_slice{
    char        inlinebuf[inline_cap];
    char*       buf;    
    uint64_t    size;   
};

void posixc_slice_init(posixc_slice*slice, uint64_t size){
    assert(size>0);
    if(size>inline_cap){
        buf=(char*)malloc(size);
    }else{
        slice->buf=inlinebuf;
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



