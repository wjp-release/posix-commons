#include <fcntl.h>
#include <sys/event.h>
#include <sys/socket.h>
#include <pthread.h>

#include "event.h"
#include "reactor.h"

void posixc_preparefd(int fd){
    fcntl(fd, F_SETFD, FD_CLOEXEC);
	fcntl(fd, F_SETFL, O_NONBLOCK);
    int one = 1;
	setsockopt(fd, SOL_SOCKET, SO_NOSIGPIPE, &one, sizeof(one)); 
}

static void posixc_event_init(posixc_event*event, posixc_reactor* reactor, int fd, posixc_event_cb cb, void* arg,uint8_t type){
    posixc_preparefd(fd);
    event->reactor=reactor;
    event->fd=fd;
    event->cb=cb;
    event->arg=arg;
    event->evmask=0;
    event->closing=false;
    event->type=type;
    event->data_1=0;
    event->data_2=0;
    pthread_mutex_init(&event->mtx, NULL);
    INIT_LIST_HEAD(&event->node);
}

static void posixc_event_release(posixc_event*event){
    close(event->fd);
    pthread_mutex_destroy(&event->mtx);
    free(event);
}

posixc_event* posixc_event_create(posixc_reactor* reactor, int fd, posixc_event_cb cb, void* arg, uint8_t type){
    posixc_event* event=(posixc_event*)malloc(sizeof(posixc_event));
    if(event==NULL) return NULL;
    posixc_event_init(event,reactor,fd,cb,arg,type);
    if(posixc_event_plat_create(event)){
         return event;
    }else{
		posixc_event_release(event);
		return NULL;
    }
}

static void add_to_gc_list(posixc_event*e){
    printf("add to gc list, reactor=%d\n", (int)e->reactor);
    list_add_tail(&e->node, &e->reactor->gc_events);
    posixc_event*tmp;
    list_for_each_entry(tmp, &e->reactor->gc_events, node){
	    printf("type= %d fd= %d\n", tmp->type, tmp->fd);
    }
}

void posixc_event_destroy(posixc_event*e){
    // Any user thread that holds a reference to the posixc_event could stop it from working.
	if (!e->closing) {  
        e->closing = true;
        posixc_event_plat_destroy(e);
		shutdown(e->fd, SHUT_RDWR);
        //add e into gc list, let reactor release it
        add_to_gc_list(e);
	}
    // Final destruction can only be performed by its owning reactor thread. This restriction ensures that the reactor won't process already destroyed posixc_events.
    if(pthread_self()==e->reactor->thread){
        posixc_event_release(e);
    }
}

bool posixc_event_submit(posixc_event* e, int evmask){
    if (e->closing) return false; 
	pthread_mutex_lock(&e->mtx);
	e->evmask |= evmask;
    bool success=posixc_event_plat_submit(e);
	pthread_mutex_unlock(&e->mtx);
    return success;
}

void posixc_event_consume(posixc_event*e, int evmask_to_consume, posixc_event_cb* cb, void** arg){
    pthread_mutex_lock(&e->mtx);
    e->evmask &= ~ evmask_to_consume;
    *cb=e->cb;
    *arg=e->arg;
    pthread_mutex_unlock(&e->mtx);
}

void posixc_timer_event_set_interval(posixc_event*e, int ms_interval, bool is_periodic){
    e->data_1=is_periodic? POSIXC_TIMER_PERIODIC:POSIXC_TIMER_ONESHOT;
    e->data_2=(uint64_t)ms_interval;
}

int posixc_unique_timer_event_id(){
    static uint64_t id=0;
    return id++;
}
int posixc_unique_sig_event_id(){
    static uint64_t id=0;
    return id++;
}
int posixc_unique_user_event_id(){
    static uint64_t id=0;
    return id++;
}