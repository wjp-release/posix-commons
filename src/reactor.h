#pragma once

#include "internal.h"

struct posixc_reactor{
	pthread_mutex_t  mtx;
	pthread_t        thread;   
    int              id;  //native id, epfd or kq   
	list_head        gc_events; 
	bool 			 closing;
};

// kqueue identifier or epoll fd.
// API 
posixc_reactor* posixc_reactor_create();
// API 
void posixc_reactor_destroy(posixc_reactor* r);
// impl
void posixc_reactor_gc(posixc_reactor*r);

// platform specific impl functions
void* posixc_reactor_plat_threadfn(void*arg);
int posixc_reactor_plat_create();