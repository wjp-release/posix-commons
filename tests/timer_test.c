#include "posixc.h"

static void on_clock(posixc_event* e, int evmask, void* arg){
    printf("haha!\n");
}

static void on_clock2(posixc_event* e, int evmask, void* arg){
    printf("bobo!\n");
}


int main(){
    posixc_reactor* reactor=posixc_reactor_create();
    posixc_event* event=posixc_event_create(reactor,0,on_clock,NULL,TIMER_EV);
    posixc_event* event2=posixc_event_create(reactor,1,on_clock2,NULL,TIMER_EV);

    posixc_timer_event_set_interval(event,1000,true);
    posixc_timer_event_set_interval(event2,3000,true);
    posixc_event_submit(event,POSIXC_EVENT_TIMER);
    printf("another one\n");
    posixc_event_submit(event2,POSIXC_EVENT_TIMER);
    while(true);
    posixc_event_destroy(event);
    posixc_event_destroy(event2);
    posixc_reactor_destroy(reactor);
    return 0;
}