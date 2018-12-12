#include "asynclogsvr.h"
#include "event.h"
#include "ipc.h"

static void on_clock(posixc_event* e, int evmask, void* arg){
    printf("flush!\n");
}

static void on_dgram(posixc_event* e, int evmask, void* arg){
    char buf[1000];
    ssize_t nr = read(e->fd, buf, sizeof(buf));
    if (nr < 0){
        fprintf(stderr,"read: %s\n", strerror(errno));
    }
    if (nr == 0) {
        fprintf(stderr,"client: eof\n");
    } else {
        printf("received %ld bytes: %.*s\n", nr, (int)nr, buf);
    }
}

void posixc_asynclogsvr_init(posixc_asynclogsvr*svr,const char* dir, int max_file_size, int flush_interval){
    strcpy(svr->dir, dir);
    svr->max_file_size=max_file_size;
    svr->flush_interval=flush_interval;
    svr->reactor=posixc_reactor_create();
    //create dgram event
    int ipcfd=posixc_ipc_dgram_socket(usockpath_log);
    svr->dgram_event=posixc_event_create(svr->reactor,ipcfd, on_dgram, NULL, SOCK_EV);
    //create timer event
    svr->timer_event=posixc_event_create(svr->reactor, posixc_unique_timer_event_id(),on_clock,NULL,TIMER_EV);
    posixc_timer_event_set_interval(svr->timer_event,flush_interval,true);
    //submit both events
    posixc_event_submit(svr->dgram_event,POSIXC_EVENT_IN);
    posixc_event_submit(svr->timer_event,POSIXC_EVENT_TIMER);
}

void posixc_asynclogsvr_destroy(posixc_asynclogsvr*svr){
    posixc_event_destroy(svr->timer_event);
    posixc_event_destroy(svr->dgram_event);
    posixc_reactor_destroy(svr->reactor);
}