#pragma once

#include "internal.h"
#include "reactor.h"
#include "event.h"
#include "fileio.h"

// Async logging IPC server binds a dgram-mode unix domain socket to recv log msgs, and flushes buffer every flush_interval seconds.

// Rolling event triggered every time a log file reaches max)file_size.

struct posixc_asynclogsvr{
    posixc_buffered_writable buffered_writefile;
    posixc_reactor* reactor;
    posixc_event*   timer_event;
    posixc_event*   dgram_event;
    uint32_t        max_file_size; //rerolling size
    int             flush_interval; //ms, can only be set on init
    uint64_t        bytes_written;
    char            filename[256]; //updated on each rolling event
    size_t          dirlen;  
};

void posixc_asynclogsvr_init(posixc_asynclogsvr*svr,const char* dir, int max_file_size, int flush_interval);

// Should be called when kill signal captured. 
void posixc_asynclogsvr_flush(posixc_asynclogsvr*svr);

void posixc_asynclogsvr_destroy(posixc_asynclogsvr*svr);