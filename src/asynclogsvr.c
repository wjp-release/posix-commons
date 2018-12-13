#include "asynclogsvr.h"
#include "event.h"
#include "ipc.h"
#include <assert.h>
#include <stdlib.h>
#include <signal.h>

// Flushes every interval ms
static void on_clock(posixc_event* e, int evmask, void* arg){
    assert(arg!=NULL);
    posixc_asynclogsvr_flush((posixc_asynclogsvr*)arg);
}

void posixc_asynclogsvr_flush(posixc_asynclogsvr*svr){
    posixc_flush(&svr->buffered_writefile);
}

static inline void update_filename(posixc_asynclogsvr*svr){
    posixc_timestr_logfile(svr->filename+svr->dirlen); //timestr.h
}

void posixc_asynclogsvr_roll(posixc_asynclogsvr*svr){
    posixc_asynclogsvr_flush(svr);
    update_filename(svr);
    svr->bytes_written=0;
}

// Tries to exit gracefully
static void on_sigint(posixc_event* e, int evmask, void* arg){
    posixc_asynclogsvr*svr=arg;
    posixc_asynclogsvr_flush(svr);
    posixc_asynclogsvr_destroy(svr); // Note that on_sigint as well as other callbacks are executed in the reactor background thread. Therefore this call can release 
    printf("\nbye~\n");
    exit(0);
}

// Note that: even though our reactor is configured to use nonblocking sockets, a read() over datagram socket always returns a complete datagram or nothing at all. 
static void on_dgram(posixc_event* e, int evmask, void* arg){
    posixc_asynclogsvr*svr=arg;
    // The client side uses one page buffer as well. Overflowing data will be discarded. Don't log more than 4KB data each time. 
    char buf[one_page]; 
    ssize_t nr = read(e->fd, buf, sizeof(buf)); 
    if (nr > 0){
        posixc_buffered_write(&svr->buffered_writefile,buf,nr);
        svr->bytes_written+=nr;
        if(svr->bytes_written>svr->max_file_size){
            posixc_asynclogsvr_roll(svr);
        }
    }
}

void posixc_asynclogsvr_init(posixc_asynclogsvr*svr,const char* dir, int max_file_size, int flush_interval){
    posixc_block_all_signals();
    svr->bytes_written=0;
    // init filename
    svr->dirlen=strlen(dir);
    assert(svr->dirlen<180); 
    strcpy(svr->filename, dir);
    // make sure dir end with a /
    if(svr->filename[svr->dirlen-1]!='/'){
        svr->filename[svr->dirlen]='/';
        svr->dirlen++;
    }
    update_filename(svr);
    // init buffered writefile
    posixc_buffered_writable_init_from(&svr->buffered_writefile, svr->filename);
    svr->max_file_size=max_file_size;
    svr->flush_interval=flush_interval;
    svr->reactor=posixc_reactor_create();
    // create dgram event
    int ipcfd=posixc_ipc_dgram_socket(usockpath_log);
    assert(ipcfd!=-1); // posixc_ipc_dgram_socket can't fail
    svr->dgram_event=posixc_event_create(svr->reactor,ipcfd, on_dgram, svr, SOCK_EV);
    // create timer event
    svr->timer_event=posixc_event_create(svr->reactor, posixc_unique_timer_event_id(),on_clock,svr,TIMER_EV);
    // create signal events
    svr->sigint_event=posixc_event_create(svr->reactor, SIGINT,on_sigint,svr,SIG_EV);
    // creation of events can't fail
    assert(svr->dgram_event!=NULL && svr->timer_event!=NULL && svr->sigint_event!=NULL);  
    posixc_timer_event_set_interval(svr->timer_event,flush_interval,true);
    // submit events
    posixc_event_submit(svr->dgram_event,POSIXC_EVENT_IN);
    posixc_event_submit(svr->timer_event,POSIXC_EVENT_TIMER);
    posixc_event_submit(svr->sigint_event,POSIXC_EVENT_SIG);
}

void posixc_asynclogsvr_destroy(posixc_asynclogsvr*svr){
    posixc_event_destroy(svr->timer_event);
    posixc_event_destroy(svr->dgram_event);
    posixc_event_destroy(svr->sigint_event);
    posixc_reactor_destroy(svr->reactor);
}