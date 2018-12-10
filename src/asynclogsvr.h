#pragma once

#include "internal.h"
#include "reactor.h"
#include "event.h"

// Async logging IPC server binds a dgram-mode unix domain socket to recv log msgs, and flushes buffer every flush_interval seconds.

#define max_asynclog_buffer 4096

struct posixc_asynclogsvr{
    char            dir[128];
    char            buf[max_asynclog_buffer];
    posixc_reactor* reactor;
    posixc_event*   timer_event;
    posixc_event*   dgram_event;
    uint32_t        max_file_size; //rerolling size
    int             flush_interval; //seconds
};

void posixc_asynclogsvr_init(posixc_asynclogsvr*svr,const char* dir, int max_file_size, int flush_interval);

void posixc_asynclogsvr_destroy(posixc_asynclogsvr*svr);