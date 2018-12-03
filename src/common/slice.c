#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "boyermoore.h"
#include "kmp.h"
#include "utils.h"
#include "slice.h"

void posixc_slice_init(posixc_slice*slice, uint64_t size){
    assert(size>0);
    if(size>inline_cap){
        slice->buf=(char*)malloc(size);
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

void posixc_slice_init_as_aggregation(posixc_slice* aggregate, posixc_slice** slices, size_t nr_slices){
    size_t size=0;
    for(size_t i=0;i<nr_slices;i++){
        posixc_slice* current=slices[i];
        size+=current->size;
    }
    posixc_slice_init(aggregate,size);
    for(size_t i=0, pos=0;i<nr_slices;i++){
        posixc_slice* current=slices[i];
        memcpy(aggregate->buf+pos, current->buf, current->size);
        pos+=current->size;
    }
}

void posixc_slice_init_from(posixc_slice*slice, const char* str){
    int size=strlen(str);
    posixc_slice_init(slice,size);
    memcpy(slice->buf, str, size);
}

void posixc_slice_copy(posixc_slice*dst, posixc_slice*src){
    dst->size=src->size;
    if(dst->size<inline_cap){
        dst->buf=dst->inlinebuf;
    }else{
        dst->buf=(char*)malloc(src->size);
    }
    memcpy(dst->buf,src->buf,src->size);
}

bool posixc_slice_contains(posixc_slice*text, posixc_slice* pattern){
    if(pattern->size<=8){  // kmp works better when the pattern is small
        return posixc_kmp_contains(text->buf,text->size,pattern->buf,pattern->size)!=NULL;
    }else{ // boyermoore works better when the pattern and the text resemble "natural text" 
        return posixc_boyermoore_contains(text->buf,text->size,pattern->buf,pattern->size)!=NULL;
    }
}

int posixc_slice_append(posixc_slice* text, const char* suffix){
    int size=strlen(suffix);
    if(text->buf==text->inlinebuf){
        if(text->size+size>inline_cap){
            text->buf=(char*)malloc(text->size+size);
            memcpy(text->buf, text->inlinebuf, text->size);
        }
    }else{
        text->buf=realloc(text->buf, text->size+size);
        if(text->buf==NULL) return -1;
    }
    memcpy(text->buf+text->size, suffix, size);
    text->size+=size;
    return 0;
}

posixc_slice posixc_slice_parse_int(int x){
    posixc_slice s;
    s.size=posixc_itoa(x, s.inlinebuf); // 10^48 > 2^64, inline space is always sufficient.
    s.buf=s.inlinebuf;
    return s;
}

posixc_slice posixc_slice_parse_float(float x, int afterpoints){
    posixc_slice s;
    s.size=posixc_ftoa_within_given_bytes(x,s.inlinebuf,afterpoints, 48); //make sure space is enough
    s.buf=s.inlinebuf;
    return s;
}

posixc_slice posixc_slice_parse_double(double x, int afterpoints){
    posixc_slice s;
    s.size=posixc_dtoa_within_given_bytes(x,s.inlinebuf,afterpoints, 48); 
    s.buf=s.inlinebuf;
    return s;
}

