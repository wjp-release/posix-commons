#pragma once
#include "internal.h"
#include "list.h"

#define POSIXC_EVENT_IN 0x0001
#define POSIXC_EVENT_OUT 0x0004
#define POSIXC_EVENT_ERR 0x0010
// platform-independent

struct posixc_reactor;

struct posixc_event{
    posixc_reactor* reactor;
    int             fd;
    posixc_event_cb cb;
    void*           arg;
    int             evmask;
    bool            closing; 
    pthread_mutex_t mtx;
    list_node       node;
};

typedef void (*posixc_event_cb)(posixc_event*, int, void *);


// APIs
posixc_event* posixc_event_create(posixc_reactor* r, int fd, posixc_event_cb cb, void* arg);

void posixc_event_destroy(posixc_event*e);

bool posixc_event_submit(posixc_event* e, int evmask);

void posixc_preparefd(int fd);

void posixc_event_consume(posixc_event*e, int evmask_to_consume, posixc_event_cb* cb, void** arg);

// platform-specific (epoll or kqueue) impl functions

bool posixc_event_plat_create(posixc_event*e);

bool posixc_event_plat_destroy(posixc_event*e);

bool posixc_event_plat_submit(posixc_event*e);