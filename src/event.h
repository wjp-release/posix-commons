#pragma once
#include "internal.h"
#include "common/list.h"

// platform-independent

#define POSIXC_EVENT_IN 0x0001
#define POSIXC_EVENT_OUT 0x0004
#define POSIXC_EVENT_ERR 0x0010
#define POSIXC_EVENT_SIG 0x0400
#define POSIXC_EVENT_TIMER 0x0100
#define POSIXC_EVENT_USER 0x0008

#define POSIXC_TIMER_PERIODIC 0
#define POSIXC_TIMER_ONESHOT 1

enum posixc_event_type {
    SOCK_EV     = 0, //socket
    TIMER_EV    = 1, //timer
    USER_EV     = 2, //user-defined 
    SIG_EV      = 3, //singal
};                      

typedef void (*posixc_event_cb)(posixc_event*, int, void *);

struct posixc_event{
    posixc_reactor* reactor;
    int             fd;  
    posixc_event_cb cb;
    void*           arg;
    int             evmask;
    bool            closing; 
    uint8_t         type;
    uint64_t        data_1; // if data_1 == 1, timer will be triggered only once
    uint64_t        data_2; // used as delay time for timer events
    pthread_mutex_t mtx;
    list_node       node; // helps graceful destruction of this event.
};

// APIs
posixc_event* posixc_event_create(posixc_reactor* r, int fd, posixc_event_cb cb, void* arg, uint8_t type);

//User code is not allowed to actually release any event since that could possibly convert this event to garbage data in current eventloop. The posixc_event_destroy() function called from user threads will simply mark it as closing and add it to the gc list. Actual garbage collection is performed by its owning reactor thread.
void posixc_event_destroy(posixc_event*e);

//Every event is triggered exactly once. If you want to continue watching this event, resubmit it.
bool posixc_event_submit(posixc_event* e, int evmask);

void posixc_preparefd(int fd);

void posixc_event_consume(posixc_event*e, int evmask_to_consume, posixc_event_cb* cb, void** arg);

void posixc_timer_event_set_interval(posixc_event*e, int ms_interval, bool is_periodic);

// platform-specific (epoll or kqueue) impl functions
bool posixc_event_plat_create(posixc_event*e);

bool posixc_event_plat_destroy(posixc_event*e);

bool posixc_event_plat_submit(posixc_event*e);

// Generate kqueue ids for non-file event handles
int posixc_unique_timer_event_id();
int posixc_unique_sig_event_id();
int posixc_unique_user_event_id();



